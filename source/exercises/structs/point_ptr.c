#include <stdio.h>

struct point {
    float x, y;
};

struct point *add_points(struct point *a, struct point *b)
{
    a->x += b->x;
    a->y += b->y;

    // We'll be nice and return a pointer to the result,
    // even if it's unused. Sometimes this might be convenient
    // for the caller.

    return a;
}

int main(void)
{
    struct point p0, p1;

    p0.x = 10;
    p0.y = 20;

    p1.x = 30;
    p1.y = 40;

    add_points(&p0, &p1);

    printf("Result: <%f,%f>\n", p0.x, p0.y);
}
