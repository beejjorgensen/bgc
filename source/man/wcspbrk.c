#include <wchar.h>

int main(void)
{
    //  p points here after wcspbrk
    //                  v
    wchar_t *s1 = L"Hello, world!";
    wchar_t *s2 = L"dow!";  // Match any of these chars

    wchar_t *p = wcspbrk(s1, s2);  // p points to the o

    wprintf(L"%ls\n", p);  // "o, world!"
}

