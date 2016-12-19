#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "usart.h"

#include "FreeRTOS.h"

/*
 * This function configure ADC1
 */
void adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable GPIOA and ADC1 clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Configure GPIOA 1 to use analog mode */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Common configuration for all ADC */
    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* Configure ADC1 and select TIM2 has external trigger for conversion */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Add channel 1 to regular group */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);

    /* Enable End Of Conversion interrupt on ADC1 */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    ADC_Cmd(ADC1, ENABLE);

    /* Add ADC1 IRQ to NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Start conversion
    ADC_SoftwareStartConv(ADC1);
}

/*
 * This IRQHandler is called each time the ADC has finished his conversion.
 */
void adc_irqhandler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    USART_Msg message;

    if(ADC_GetITStatus(ADC1, ADC_IT_EOC))
    {
        if(USARTQueue)
        {
            message.msg_type = USART_MSG_ADC_CONVERTION;
            message.adc = ADC_GetConversionValue(ADC1);

            xQueueSendFromISR(USARTQueue, (void*)&message,
                    &xHigherPriorityTaskWoken);
        }
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
