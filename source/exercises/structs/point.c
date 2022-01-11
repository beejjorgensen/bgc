#include <stdio.h>

struct point {
    float x, y;
};

struct point add_points(struct point a, struct point b)
{
    struct point result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

int main(void)
{
    struct point p0, p1, p2;

    p0.x = 10;
    p0.y = 20;

    p1.x = 30;
    p1.y = 40;

    p2 = add_points(p0, p1);

    printf("<%f,%f> + <%f,%f> = <%f,%f>\n", p0.x, p0.y, p1.x,
                                            p1.y, p2.x, p2.y);
}
