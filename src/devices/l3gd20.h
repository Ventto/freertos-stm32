#ifndef STM32F4XX_L3GD20_H
#define STM32F4XX_L3GD20_H

#include <stdint.h>

#define L3GD20_CTRL1 0x20
#define L3GD20_CTRL2 0x21
#define L3GD20_CTRL3 0x22
#define L3GD20_CTRL4 0x23

#define L3GD20_OUT_X_LO 0x28
#define L3GD20_OUT_X_HI 0x29

#define L3GD20_OUT_Y_LO 0x2a
#define L3GD20_OUT_Y_HI 0x2b

#define L3GD20_OUT_Z_LO 0x2c
#define L3GD20_OUT_Z_HI 0x2d

void spi_init(void);

void spi_write_register(uint8_t register_addr, uint8_t reg_value);

uint8_t spi_read_register(uint8_t register_addr);

#endif /* !STM32F4XX_L3GD20_H */
