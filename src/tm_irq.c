#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "tm_irq.h"

void init_timer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	/* Enable TIM2 Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* TIMPRE not set in RCC register */
	/* AP1 prescaler set to 4 so timer clock is 2 * APB clock(45MHz) = 90MHz */
	/* Configure timer for 1Hz frequency */
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = (90000000 / 1000000) - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 1000000 - 1;   /* TIM2 is 32bit counter */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	/* Enable TIMER interrupt on counter reload */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* Add TIM2 IRQ to NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	TIM_Cmd(TIM2, ENABLE);
}

void init_timer_pwm(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCStruct;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable Timer1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* Enable the GPIOE Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure the GPIOE 9 to TIM1 alternate function (output channel 1) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);

	/* TIMPRE not set in RCC register */
	/* AP1 prescaler set to 4 so timer clock is 2 * APB clock(90MHz) = 180MHz */
	/* Configure timer for 100Hz frequency */
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = (180000000 / 1000000) - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	/* Configure TIM1 Output Channel for 15% duty */
	TIM_OCStructInit(&TIM_OCStruct);
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCStruct.TIM_Pulse = 1500 - 1;
	TIM_OC1Init(TIM1, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* Enable TIM1 */
	TIM_Cmd(TIM1, ENABLE);

	/* Enable TIM1 outputs */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*
 * This IRQHandler is called when TIM2 counter is reloaded.
 */
void tm2_irqhandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		GPIOG->ODR ^= GPIO_Pin_13;
		GPIOG->ODR ^= GPIO_Pin_14;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
