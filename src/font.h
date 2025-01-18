#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stddef.h>

typedef struct 
{
    uint8_t width;
    uint8_t height;
    uint8_t firstChar;
    uint8_t charCount;
    const uint8_t* bitmapTable;
    const uint16_t* charOffsetTable;
}font_t;

extern font_t const font_5x7;
extern font_t const font_callibri10;

#endif /* FONT_H */
