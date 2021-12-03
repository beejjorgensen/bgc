#include <stdio.h>

int plus_one(int n)  // The "definition"
{
    return n + 1;
}
 
int main(void)
{
    int i = 10, j;
    
    j = plus_one(i);  // The "call"

    printf("i + 1 is %d\n", j);
}
