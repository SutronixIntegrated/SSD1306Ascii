/**
 * Copyright (c) 2011-2023 Bill Greiman
 * This file is part of the Arduino SSD1306Ascii Library
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/**
 * @file SSD1306init.h
 * @brief Display controller initialization commands.
 */
#ifndef SSD1306init_h
#define SSD1306init_h
//------------------------------------------------------------------------------
#include <stdint.h>

#ifndef __AVR__
/** Handle AVR flash addressing. */
#define MEM_TYPE
#else  // __AVR__
#define MEM_TYPE __attribute__((progmem))
#endif  // __AVR__
//------------------------------------------------------------------------------
#include "SSD1306Cmd.h"
/**
 * @struct DevType
 * @brief Device initialization structure.
 */
struct DevType {
  /**
   * Pointer to initialization command bytes.
   */
  const uint8_t* initcmds;
  /**
   * Number of initialization bytes.
   */
  const uint8_t initSize;
  /**
   * Width of the diaplay in pixels.
   */
  const uint8_t lcdWidth;
  /**
   * Height of the display in pixels.
   */
  const uint8_t lcdHeight;
  /**
   * Column offset RAM to display.  Used to pick start column of SH1106.
   */
  const uint8_t colOffset;
};
// clang-format off
//------------------------------------------------------------------------------
/** Initialization commands for a 64x48 Micro OLED display (by r7) */
static const uint8_t MEM_TYPE MicroOLED64x48init[] = {
    // Init sequence for 64x48 Micro OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 0x2F,        //
    SSD1306_SETDISPLAYOFFSET, 0x0,     // no offset
    SSD1306_SETSTARTLINE,              // line #0
    SSD1306_CHARGEPUMP, 0x14,          // internal vcc
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_SEGREMAP | 0x1,            // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,                // column scan direction reversed
    SSD1306_SETCOMPINS, 0x12,          // 0x12 if height > 32 else 0x02
    SSD1306_SETCONTRAST, 0x7F,         // contrast level 127
    SSD1306_SETPRECHARGE, 0xF1,        // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x40,       // vcomh regulator level
    SSD1306_DISPLAYON
};
/** Initialize a 64x48 Micro OLED display. */
static const DevType MEM_TYPE MicroOLED64x48 = {
  MicroOLED64x48init,
  sizeof(MicroOLED64x48init),
  64,
  48,
  32
};
//------------------------------------------------------------------------------
// this section is based on
// https://github.com/olikraus/u8g2/blob/master/csrc/u8x8_d_ssd1306_96x16.c
/** Initialization commands for a 96x16 SSD1306 oled display. */
static const uint8_t MEM_TYPE SSD1306_96x16init[] = {
    // Init sequence for Generic 96x16 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // clock divide ratio and osc frequency
    SSD1306_SETMULTIPLEX, 0x0F,        // multiplex ratio
    SSD1306_SETDISPLAYOFFSET, 0x0,     // display offset zero
    SSD1306_SETSTARTLINE,              // set display start line to 0
    SSD1306_CHARGEPUMP, 0x14,          // charge pump setting enable
    SSD1306_MEMORYMODE, 0x00,          // page addressing mode
    SSD1306_SEGREMAP | 0xA1,           // segment remap
    SSD1306_COMSCANDEC,                // scan dir reverse
    SSD1306_SETCOMPINS, 0x02,          // com pin HW config
    SSD1306_SETCONTRAST, 0xAF,         // set contrast level 0xaf
    SSD1306_SETPRECHARGE, 0xF1,        // pre-charge period 0x0f1
    SSD1306_SETVCOMDETECT, 0x20,       // vcomh deselect level
    SSD1306_DEACTIVATE_SCROLL,         //  Deactivate scroll
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
};
/** Initialize a 96x16 SSD1306 oled display. */
static const DevType MEM_TYPE SSD1306_96x16 = {
  SSD1306_96x16init,
  sizeof(SSD1306_96x16init),
  96,
  16,
  0
};
//------------------------------------------------------------------------------
// this section is based on https://github.com/adafruit/Adafruit_SSD1306
/** Initialization commands for a 128x32 SSD1306 oled display. */
static const uint8_t MEM_TYPE Adafruit128x32init[] = {
    // Init sequence for Adafruit 128x32 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 0x1F,        // ratio 32
    SSD1306_SETDISPLAYOFFSET, 0x0,     // no offset
    SSD1306_SETSTARTLINE,              // line #0
    SSD1306_CHARGEPUMP, 0x14,          // internal vcc
    SSD1306_MEMORYMODE, 0x02,          // page mode
    SSD1306_SEGREMAP | 0x1,            // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,                // column scan direction reversed
    SSD1306_SETCOMPINS, 0x02,          // sequential COM pins, disable remap
    SSD1306_SETCONTRAST, 0x7F,         // contrast level 127
    SSD1306_SETPRECHARGE, 0xF1,        // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x40,       // vcomh regulator level
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
};
/** Initialize a 128x32 SSD1306 oled display. */
static const DevType MEM_TYPE Adafruit128x32 = {
  Adafruit128x32init,
  sizeof(Adafruit128x32init),
  128,
  32,
  0
};
//------------------------------------------------------------------------------
// This section is based on https://github.com/adafruit/Adafruit_SSD1306
/** Initialization commands for a 128x64 SSD1306 oled display. */
static const uint8_t MEM_TYPE Adafruit128x64init[] = {
    // Init sequence for Adafruit 128x64 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 0x3F,        // ratio 64
    SSD1306_SETDISPLAYOFFSET, 0x0,     // no offset
    SSD1306_SETSTARTLINE,              // line #0
    SSD1306_CHARGEPUMP, 0x14,          // internal vcc
    SSD1306_MEMORYMODE, 0x02,          // page mode
    SSD1306_SEGREMAP | 0x1,            // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,                // column scan direction reversed
    SSD1306_SETCOMPINS, 0x12,          // alt COM pins, disable remap
    SSD1306_SETCONTRAST, 0x7F,         // contrast level 127
    SSD1306_SETPRECHARGE, 0xF1,        // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x40,       // vcomh regulator level
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
};
/** Initialize a 128x64 oled display. */
static const DevType MEM_TYPE Adafruit128x64 = {
  Adafruit128x64init,
  sizeof(Adafruit128x64init),
  128,
  64,
  0
};
//------------------------------------------------------------------------------
// This section is based on https://github.com/stanleyhuangyc/MultiLCD
/** Initialization commands for a 128x64 SH1106 oled display. */
static const uint8_t MEM_TYPE SH1106_128x64init[] = {
  SSD1306_DISPLAYOFF,
  SSD1306_SETSTARTPAGE,                  // set page zero
  SSD1306_SETCONTRAST, 0x80,             // 128
  SSD1306_SEGREMAP | 0X1,                // set segment remap
  SSD1306_NORMALDISPLAY,                 // normal / reverse
  SSD1306_SETMULTIPLEX, 0x3F,            // ratio 64
  SH1106_SET_PUMP_MODE, SH1106_PUMP_ON,  // set charge pump enable
  SH1106_SET_PUMP_VOLTAGE | 0X2,         // 8.0 volts
  SSD1306_COMSCANDEC,                    // Com scan direction
  SSD1306_SETDISPLAYOFFSET, 0X00,        // set display offset
  SSD1306_SETDISPLAYCLOCKDIV, 0X80,      // set osc division
  SSD1306_SETPRECHARGE, 0X1F,            // set pre-charge period
  SSD1306_SETCOMPINS, 0X12,              // set COM pins
  SSD1306_SETVCOMDETECT,  0x40,          // set vcomh
  SSD1306_DISPLAYON
};
/** Initialize a 128x64 oled SH1106 display. */
static const DevType MEM_TYPE SH1106_128x64 =  {
  SH1106_128x64init,
  sizeof(SH1106_128x64init),
  128,
  64,
  2    // SH1106 is a 132x64 controller.  Use middle 128 columns.
};
// clang-format on
#endif  // SSD1306init_h
