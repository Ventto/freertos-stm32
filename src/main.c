/*
 * C
 *
 * Copyright 2013-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <stdio.h>
#include <assert.h>

#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "devices/usart.h"
#include "devices/adc.h"
#include "devices/led.h"
#include "devices/gyro.h"
#include "devices/button.h"
#include "event.h"

static void gyro_task(void *param)
{
    xQueueHandle event_queue = param;

    for (;;) {
        vTaskDelay(200);

        struct vector3 vw;
        gyro_get_velocity(&vw);

        enum event_kind k = vector3_norm2(&vw) > GYRO_VELOCITY_THRESHOLD
            ? EVENT_GYRO_MOVED
            : EVENT_GYRO_IDLE;

        struct event ev = { .kind = k };
        xQueueSend(event_queue, &ev, portMAX_DELAY);
    }
}

static void tick_task(void *param)
{
    xQueueHandle event_queue = param;

    for (;;) {
        vTaskDelay(200);
        struct event ev = { .kind = EVENT_TICK };
        xQueueSend(event_queue, &ev, portMAX_DELAY);
    }
}

static void bright_task(void *param)
{
    xQueueHandle event_queue = param;

    int oldval;
    for (;;) {
        int val = adc_readval();
        struct event ev = {
            .kind = val > oldval
                ? EVENT_BRIGHT
                : EVENT_DIM
        };
        oldval = val;
        xQueueSend(event_queue, &ev, portMAX_DELAY);
    }
}

static struct {
    void (*fn)(void *);
    const char *name;
    xTaskHandle handle;
} tasks[] = {
    { .fn = tick_task,   .name = "Ticker"            },
    { .fn = gyro_task,   .name = "Gyroscope poller"  },
    { .fn = bright_task, .name = "Brightness poller" },
};

static void event_reaper(void *param)
{
    xQueueHandle event_queue = param;

    led_init();

    struct event ev;
    for (;;) {
        xQueueReceive(event_queue, &ev, portMAX_DELAY);

        switch (ev.kind) {
            case EVENT_GYRO_IDLE:
            case EVENT_SWITCH:
            case EVENT_DIM:
                led_off();
                break;
            case EVENT_TICK:
                led_toggle();
                break;
            case EVENT_GYRO_MOVED:
            case EVENT_BRIGHT:
                led_on();
                break;
        }
    }
}

static void button_action(void *param)
{
    static size_t idx = 0;
    xQueueHandle event_queue = param;

    /* Switch between ticker and gyro on button press */
    vTaskSuspend(tasks[idx].handle);

    struct event ev = { .kind = EVENT_SWITCH };
    xQueueSend(event_queue, &ev, portMAX_DELAY);

    idx = (idx + 1) % (sizeof (tasks) / sizeof (tasks[0]));
    vTaskResume(tasks[idx].handle);
}

int main(void)
{
    // Enable fault handlers
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

    usart_init();

    puts("Initialization.");

    adc_init();
    gyro_init();

    xQueueHandle q = xQueueCreate(128, sizeof (struct event));
    assert(q);

    xTaskCreate(event_reaper, "Event Reaper", 128, q, tskIDLE_PRIORITY + 1, NULL);

    for (size_t i = 0; i < sizeof (tasks) / sizeof (tasks[0]); ++i)
        xTaskCreate(tasks[i].fn, tasks[i].name, 128, q, tskIDLE_PRIORITY + 1, &tasks[i].handle);
    for (size_t i = 1; i < sizeof (tasks) / sizeof (tasks[0]); ++i)
        vTaskSuspend(tasks[i].handle);

    button_init(button_action, q);

    puts("Initialization OK, starting scheduler!");
    vTaskStartScheduler();
}

