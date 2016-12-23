#ifndef GYRO_H_
#define GYRO_H_

#include "../vector3.h"

#define GYRO_SENSITIVITY_500DPS (17.5f / 1000.f)
#define GYRO_VELOCITY_MAX (0xffff * GYRO_SENSITIVITY_500DPS)
#define GYRO_VELOCITY_THRESHOLD (500 * GYRO_SENSITIVITY_500DPS)

void gyro_init(void);
void gyro_get_velocity(struct vector3 *v);

#endif /* !GYRO_H_ */
