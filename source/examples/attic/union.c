#include <stdio.h>

struct variable {
    // 0 = int, 1 = float, 2 = string
    int type;

    union data {
        int i;
        float f;
        char *s;
    } data;
};

void print_variable(struct variable *v)
{
    switch(v->type) {
    case 0: // int
        printf("%d\n", v->data.i);
        break;
    case 1: // float
        printf("%f\n", v->data.f);
        break;
    case 2: // string
        printf("%s\n", v->data.s);
        break;
    }
}

int main(void)
{
    struct variable v;

    v.type = 1;
    v.data.f = 0.8;

    print_variable(&v);
}
