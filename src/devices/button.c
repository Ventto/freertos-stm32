#include <stdlib.h>
#include "stm32f429i_discovery.h"
#include "FreeRTOS.h"
#include "task.h"

#include "button.h"

static xTaskHandle button_task;

struct ctx {
    void (*action)(void *);
    void *param;
};

static void button_press(void *param)
{
    struct ctx *ctx = param;

    for (;;) {
        ctx->action(ctx->param);
        vTaskSuspend(NULL);
    }
}

void button_init(void (*action)(void *), void *ptr)
{
    struct ctx *ctx = malloc(sizeof (*ctx));
    ctx->action = action;
    ctx->param = ptr;

    xTaskCreate(button_press, NULL, 128, ctx, tskIDLE_PRIORITY + 1, &button_task);
    vTaskSuspend(button_task);

    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
}

void EXTI0_IRQHandler(void) {
    /* Check if we are indeed reacting to a button press */
    if (EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) != RESET) {
        EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
        xTaskResumeFromISR(button_task);
    }
}
