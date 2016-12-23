#include "vector3.h"

float vector3_norm2(const struct vector3 *v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}
