#include <stdio.h>

#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

void blink_task(void *pvParameters)
{
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

    for (;;) {
        vTaskDelay(1000);
        /* Make XOR on output data register to make LEDs blink */
        GPIOG->ODR ^= GPIO_Pin_13;
        GPIOG->ODR ^= GPIO_Pin_14;
        printf("Hello World!\n");
    }
}
