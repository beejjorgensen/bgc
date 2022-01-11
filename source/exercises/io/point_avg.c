#include <stdio.h>

int main(void)
{
    float x, y, z;
    float tx = 0, ty = 0, tz = 0; // totals
    int count = 0;

    FILE *fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error opening input.txt\n");
        return 1;
    }

    while(fscanf(fp, "%f,%f,%f", &x, &y, &z) != EOF) {
        tx += x;
        ty += y;
        tz += z;
        count++;
    }

    fclose(fp);

    printf("Average point <%f,%f,%f>\n", tx / count, ty / count, tz / count);
}
