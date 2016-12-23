#ifndef VECTOR3_H_
#define VECTOR3_H_

struct vector3 {
    float x;
    float y;
    float z;
};

float vector3_norm2(const struct vector3 *v);

#endif /* !VECTOR3_H_ */
