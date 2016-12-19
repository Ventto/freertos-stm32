#ifndef USART_H_
#define USART_H_

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

void initUSART(void);
void usart_task_handleevents(void *pvParameters);

#endif /* !USART_H_ */
