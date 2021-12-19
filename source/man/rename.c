#include <stdio.h>

int main(void)
{
    // Rename a file
    rename("foo", "bar");

    // Rename and move to another directory:
    rename("/home/beej/evidence.txt", "/tmp/nothing.txt");
}
