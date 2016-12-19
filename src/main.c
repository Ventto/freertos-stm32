/*
 * C
 *
 * Copyright 2013-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <stdio.h>

#include "stm32f4xx_rcc.h"
#include "usart.h"
#include "stm32f4xx_l3gd20.h"
#include "blink.h"

#include "FreeRTOS.h"
#include "task.h"

/*
 * Generic main function
 */
int main(void)
{
    // Enable fault handlers
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

    initUSART();

    xTaskCreate(usart_task_handleevents, NULL, 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blink_task, NULL, 128, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start the FreeRTOS task scheduler
    vTaskStartScheduler();
}

