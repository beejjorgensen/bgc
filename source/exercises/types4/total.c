// total.c

// file scope variables are automatically initialized to zero

int count;

static int t;  // total

void add(int x)
{
    t += x;
    count++;
}

int total(void)
{
    return t;
}
