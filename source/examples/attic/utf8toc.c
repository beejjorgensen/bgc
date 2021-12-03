#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UTF-8 to C Universal Character Name converter
//
// WARNING: undefined behavior on any invalid UTF-8 input.

// Macros to test how many bytes this character is
#define UTF8_1_BYTE(v) (((v) & 0x80) == 0x00)
#define UTF8_2_BYTES(v) (((v) & 0xE0) == 0xC0)
#define UTF8_3_BYTES(v) (((v) & 0xF0) == 0xE0)
#define UTF8_4_BYTES(v) (((v) & 0xF8) == 0xF0)

int main(void)
{
    char buf[1024];
    int bytes;

    do {
        bytes = fread(buf, sizeof buf[0], sizeof buf, stdin);

        if (bytes <= 0) continue;

        size_t offset = 0;

        while (offset < (size_t)bytes) {
            unsigned char val = buf[offset++], val2, val3, val4;
            unsigned int cp;

            if (UTF8_1_BYTE(val)) {
                cp = val;
            } else if (UTF8_2_BYTES(val)) {
                val2 = buf[offset++];
                cp = ((val & 0x1F) << 6) | (val2 & 0x3F);
            } else if (UTF8_3_BYTES(val)) {
                val2 = buf[offset++];
                val3 = buf[offset++];
                cp = ((val & 0x0F) << 12) | ((val2 & 0x3F) << 6) | (val3 & 0x3F);
            } else if (UTF8_4_BYTES(val)) {
                val2 = buf[offset++];
                val3 = buf[offset++];
                val4 = buf[offset++];
                cp = ((val & 0x07) << 18) | ((val2 & 0x3F) << 12) | ((val3 & 0x3F) << 6) | (val4 & 0x3F);
            }

            if (cp > 0xFFFFu) {
                // Bigger than 16 bits, so use U escape
                printf("\\U%08X", cp);
            } else {
                // Smaller than 16 bits can use u
                printf("\\u%04X", cp);
            }
        }
    } while (!feof(stdin));
}