#include <stdio.h>
#include <locale.h>

int main(void)
{
    char *loc;

    // Get the current locale
    loc = setlocale(LC_ALL, NULL);

    printf("Starting locale: %s\n", loc);

    // Set (and get) the locale to native locale
    loc = setlocale(LC_ALL, "");
    
    printf("Native locale: %s\n", loc);
}
