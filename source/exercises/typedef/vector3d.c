#include <stdio.h>

typedef struct {
    float x, y, z;
} vector3d;

float dot3d(vector3d *a, vector3d *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

int main(void)
{
    vector3d v1, v2;

    v1.x = 1.0;
    v1.y = 2.0;
    v1.z = 3.0;

    v2.x = 10.0;
    v2.y = 20.0;
    v2.z = 30.0;

    float dotproduct = dot3d(&v1, &v2);

    printf("%f\n", dotproduct);  // 140.000000
}
