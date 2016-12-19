/*
 * C
 *
 * Copyright 2013-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "stm32f4xx_rcc.h"
#include "usart.h"

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

	while(1){}

    // Start the FreeRTOS task scheduler
    vTaskStartScheduler();
}

