#include <math.h>

#include "l3gd20.h"
#include "gyro.h"

static float read_coord(uint8_t addr)
{
    uint8_t lo = spi_read_register(addr);
    uint8_t hi = spi_read_register(addr + 1);

    float raw = (lo | (hi << 8)) * GYRO_SENSITIVITY_500DPS;
    return fmodf(raw + GYRO_VELOCITY_MAX / 2, GYRO_VELOCITY_MAX) - GYRO_VELOCITY_MAX / 2;
}

void gyro_init(void)
{
    spi_init();

    spi_write_register(L3GD20_CTRL1, 0xf);
    spi_write_register(L3GD20_CTRL4, 0x10);
}

void gyro_get_velocity(struct vector3 *v)
{
    v->x = read_coord(L3GD20_OUT_X_LO);
    v->y = read_coord(L3GD20_OUT_Y_LO);
    v->z = read_coord(L3GD20_OUT_Z_LO);
}
