#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "queue.h"

typedef enum E_USART_Msg_type {
    USART_MSG_BUTTON_PUSHED,
    USART_MSG_ADC_CONVERTION,
    USART_MSG_CHAR_RECEVIED,
    USART_MSG_I2C_VALUE,
    USART_MSG_LAST
} USART_Msg_type;

typedef struct S_USART_Msg {
    USART_Msg_type msg_type;
    uint16_t adc;
    uint16_t temp;
    char usart;
} USART_Msg;

static xQueueHandle USARTQueue;

void initUSART(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* init working queue */
    USARTQueue = xQueueCreate(50, sizeof (USART_Msg));

    if (USARTQueue == NULL)
        /* Error while creating queue */
        return;

    /* Enable GPIO and USART clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Connecting GPIOA 9/10 to alternate function USARTx_Tx/Rx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    /* Configure GPIOA pin 9/10*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);

    /* Enable RXNE interrupt, IRQHandler will be called when data available */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* Add USART1 IRQ to NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;
}

void USART1_IRQHandler(void)
{
    char rx;

    /* Check if interrupt was because data is received */
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        if (USART_GetFlagStatus(USART1, USART_SR_RXNE) != RESET)
            rx = USART_ReceiveData(USART1);
            /* code to handle the received data */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        USART_SendData(USART1, (uint8_t) rx);

        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;
    }
}
