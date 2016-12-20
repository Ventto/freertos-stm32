/*
 * C
 *
 * Copyright 2013-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <math.h>

#include "stm32f4xx_rcc.h"
#include "usart.h"
#include "stm32f4xx_l3gd20.h"

#include "FreeRTOS.h"
#include "task.h"

#define SENSITIVITY_500DPS (17.5f / 1000.f)
#define MAX_VW (0xffff * SENSITIVITY_500DPS)
#define VW_THRESH (500 * SENSITIVITY_500DPS)

static volatile uint32_t delay = 1000;

static float read_coord(uint8_t addr)
{
    uint8_t lo = spi_read_register(addr);
    uint8_t hi = spi_read_register(addr + 1);

    return (lo | (hi << 8)) * SENSITIVITY_500DPS;
}

static void gyro_task(void *param)
{
    spi_write_register(L3GD20_CTRL1, 0xf);
    spi_write_register(L3GD20_CTRL4, 0x10);

    float x = 0, y = 0, z = 0;

    for (;;) {
        vTaskDelay(1);
        taskENTER_CRITICAL();
        float dx = fmodf(read_coord(L3GD20_OUT_X_LO) + MAX_VW / 2, MAX_VW) - MAX_VW / 2;
        float dy = fmodf(read_coord(L3GD20_OUT_Y_LO) + MAX_VW / 2, MAX_VW) - MAX_VW / 2;
        float dz = fmodf(read_coord(L3GD20_OUT_Z_LO) + MAX_VW / 2, MAX_VW) - MAX_VW / 2;

        if (dx > VW_THRESH || dy > VW_THRESH || dz > VW_THRESH)
        {
            GPIOG->ODR |= GPIO_Pin_13;
            GPIOG->ODR |= GPIO_Pin_14;
        } else {
            GPIOG->ODR &= ~((uint32_t)GPIO_Pin_13);
            GPIOG->ODR &= ~((uint32_t)GPIO_Pin_14);
        }

        printf("vw = (%16d∘, %16d∘, %16d∘)\r", (int) dx, (int) dy, (int) dz);
        taskEXIT_CRITICAL();
    }
}

static void blink_task(void *param)
{
    for (;;) {
        vTaskDelay(delay);
        /* Make XOR on output data register to make LEDs blink */
        GPIOG->ODR ^= GPIO_Pin_13;
        GPIOG->ODR ^= GPIO_Pin_14;
    }
}

int main(void)
{
    // Enable fault handlers
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

    initUSART();
    spi_init();

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the GPIOG Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    /* Configure the GPIOG pin 13/14 which are connected to user LED using ST API */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    printf("Hello, World!\n");

    xTaskCreate(gyro_task,  NULL, 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate(blink_task, NULL, 128, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start the FreeRTOS task scheduler
    vTaskStartScheduler();
}

