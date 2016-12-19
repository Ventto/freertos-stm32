#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_l3gd20.h"

void spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStruct;
	uint16_t dummy;

	/* Enable GPIOC, GPIOF and SPI5 clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

	/* Configure GPIOF 7/8/9 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* Enable SPI5 alternate function */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);

	/* Configure GPIOC1. Used for L3GD20 CS */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);

	/* Configure SPI5 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI5, &SPI_InitStruct);

	/* Enable SPI5 */
	SPI_Cmd(SPI5, ENABLE);

	// Read 'WHO AM I' register (address : 0x0F)
	uint8_t device_id = read_spi_register(0x0F);

	// check for L3GD20 device (b11010100 from datasheet)
	if(dummy == 0b11010100)
		printf("L3GD20 found on SPI (dev id : 0b11010100)\n");
	else
		printf("No L3GD20 found on SPI !!!\n");
}

void spi_write_register(uint8_t register_addr, uint8_t reg_value)
{
	// Device CS goes low to start transmission
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);

	// Send register address
	SPI_I2S_SendData(SPI5, register_addr & 0x3F);
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI5); // Dummy read

	// Send register value
	SPI_I2S_SendData(SPI5, reg_value); // Write register value
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI5); // Dummy read

	// Device CS goes to high to stop transmission
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
}

uint8_t spi_read_register(uint8_t register_addr)
{
	uint8_t reg_value = 0;

	// Device CS goes low to start transmission
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);

	// Send register address
	SPI_I2S_SendData(SPI5, register_addr | 0x80);
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI5); // Dummy read

	// Read register value
	SPI_I2S_SendData(SPI5, 0xFF); // Dummy write
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	reg_value =  SPI_I2S_ReceiveData(SPI5); // register value

	// Device CS goes to high to stop transmission
	GPIO_SetBits(GPIOC, GPIO_Pin_1);

	return reg_value;
}
