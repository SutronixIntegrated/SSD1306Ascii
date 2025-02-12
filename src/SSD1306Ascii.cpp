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
#include "SSD1306Ascii.h"
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::charWidth(uint8_t ch) const
 {
  if(font == NULL)return 0;
  if (ch < font->firstChar || ch >= (font->firstChar + font->charCount))return 0;
  ch -= font->firstChar;
  
  if(font->charOffsetTable != NULL)
  {
    uint16_t index = font->charOffsetTable[ch];
    return font->charOffsetTable[ch + 1] - index;
  }
  else return m_magFactor * font->width;
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clear() {
#if INCLUDE_SCROLLING
  m_pageOffset = 0;
  setStartLine(0);
#endif  // INCLUDE_SCROLLING
  clear(0, displayWidth() - 1, 0, displayRows() - 1);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clear(uint8_t c0, uint8_t c1, uint8_t r0, uint8_t r1) {
  // Cancel skip character pixels.
  m_skip = 0;

  // Insure only rows on display will be cleared.
  if (r1 >= displayRows()) r1 = displayRows() - 1;

  for (uint8_t r = r0; r <= r1; r++) {
    setCursor(c0, r);
    for (uint8_t c = c0; c <= c1; c++) {
      // Insure clear() writes zero. result is (m_invertMask^m_invertMask).
      ssd1306WriteRamBuf(m_invertMask);
    }
  }
  setCursor(c0, r0);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clearToEOL() {
  clear(m_col, displayWidth() - 1, m_row, m_row + fontRows() - 1);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clearField(uint8_t col, uint8_t row, uint8_t n) {
  clear(col, col + fieldWidth(n) - 1, row, row + fontRows() - 1);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::displayRemap(bool mode) {
  ssd1306WriteCmd(mode ? SSD1306_SEGREMAP : SSD1306_SEGREMAP | 1);
  ssd1306WriteCmd(mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC);
}
//------------------------------------------------------------------------------
size_t SSD1306Ascii::fieldWidth(uint8_t n) {
  return n * (fontWidth() + letterSpacing());
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontCharCount() const {
  return font ? font->charCount : 0;
}
//------------------------------------------------------------------------------
char SSD1306Ascii::fontFirstChar() const {
  return font ? font->firstChar : 0;
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontHeight() const {
  return font ? m_magFactor * font->height : 0;
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontRows() const {
  return font ? m_magFactor * ((font->height + 7) / 8) : 0;
}
//------------------------------------------------------------------------------
uint16_t SSD1306Ascii::fontSize() const {
  return font ? font->charCount : 0;
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontWidth() const {
  return font ? m_magFactor * font->width : 0;
}
//------------------------------------------------------------------------------
void SSD1306Ascii::init(const DevType* dev) {
  m_col = 0;
  m_row = 0;
#ifdef __AVR__
  const uint8_t* table =
      reinterpret_cast<const uint8_t*>(pgm_read_word(&dev->initcmds));
#else   // __AVR__
  const uint8_t* table = dev->initcmds;
#endif  // __AVR
  uint8_t size = readFontByte(&dev->initSize);
  m_displayWidth = readFontByte(&dev->lcdWidth);
  m_displayHeight = readFontByte(&dev->lcdHeight);
  m_colOffset = readFontByte(&dev->colOffset);
  for (uint8_t i = 0; i < size; i++) {
    ssd1306WriteCmd(readFontByte(table + i));
  }
  clear();
}
//------------------------------------------------------------------------------
void SSD1306Ascii::invertDisplay(bool invert) {
  ssd1306WriteCmd(invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setCol(uint8_t col) {
  if (col < m_displayWidth) {
    m_col = col;
    col += m_colOffset;
    ssd1306WriteCmd(SSD1306_SETLOWCOLUMN | (col & 0XF));
    ssd1306WriteCmd(SSD1306_SETHIGHCOLUMN | (col >> 4));
  }
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setContrast(uint8_t value) {
  ssd1306WriteCmd(SSD1306_SETCONTRAST);
  ssd1306WriteCmd(value);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setCursor(uint8_t col, uint8_t row) {
  setCol(col);
  setRow(row);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setFont(const font_t *fnt) {
  font = fnt;
  //m_letterSpacing = 0; // impliment this
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setRow(uint8_t row) {
  if (row < displayRows()) {
    m_row = row;
#if INCLUDE_SCROLLING
    ssd1306WriteCmd(SSD1306_SETSTARTPAGE | ((m_row + m_pageOffset) & 7));
#else   // INCLUDE_SCROLLING
    ssd1306WriteCmd(SSD1306_SETSTARTPAGE | m_row);
#endif  // INCLUDE_SCROLLING
  }
}
#if INCLUDE_SCROLLING
//------------------------------------------------------------------------------
void SSD1306Ascii::setPageOffset(uint8_t page) {
  m_pageOffset = page & 7;
  setRow(m_row);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setStartLine(uint8_t line) {
  m_startLine = line & 0X3F;
  ssd1306WriteCmd(SSD1306_SETSTARTLINE | m_startLine);
}
#endif  // INCLUDE_SCROLLING
//------------------------------------------------------------------------------
void SSD1306Ascii::ssd1306WriteRam(uint8_t c) {
  if (m_col < m_displayWidth) {
    writeDisplay(c ^ m_invertMask, SSD1306_MODE_RAM);
    m_col++;
  }
}
//------------------------------------------------------------------------------
void SSD1306Ascii::ssd1306WriteRamBuf(uint8_t c) {
  if (m_skip) {
    m_skip--;
  } else if (m_col < m_displayWidth) {
    writeDisplay(c ^ m_invertMask, SSD1306_MODE_RAM_BUF);
    m_col++;
  }
}
//------------------------------------------------------------------------------
GLCDFONTDECL(scaledNibble) = {0X00, 0X03, 0X0C, 0X0F, 0X30, 0X33, 0X3C, 0X3F,
                              0XC0, 0XC3, 0XCC, 0XCF, 0XF0, 0XF3, 0XFC, 0XFF};
//------------------------------------------------------------------------------
size_t SSD1306Ascii::strWidth(const char* str) const {
  size_t sw = 0;
  while (*str) {
    uint8_t cw = charWidth(*str++);
    if (cw == 0) {
      return 0;
    }
    sw += cw + letterSpacing();
  }
  return sw;
}
//------------------------------------------------------------------------------
void SSD1306Ascii::tickerInit(TickerState* state, const font_t *fnt,
                              uint8_t row, bool mag2X, uint8_t bgnCol,
                              uint8_t endCol) {
  // Initialize the ticker state with the specified parameters
  state->font = fnt;
  state->row = row;
  state->mag2X = mag2X;
  state->bgnCol = bgnCol;
  state->endCol = endCol < m_displayWidth ? endCol : m_displayWidth - 1;
  state->nQueue = 0;
}

// Add a new text string to the ticker queue
bool SSD1306Ascii::tickerText(TickerState* state, const char* text) {
  if (!text) {
    state->nQueue = 0;
    return true;
  }
  if (state->nQueue >= TICKER_QUEUE_DIM) {
    return false;
  }
  if (state->nQueue == 0) {
    state->init = true;
  }
  state->queue[state->nQueue++] = text;
  return true;
}

// Update the ticker state and scroll the text
int8_t SSD1306Ascii::tickerTick(TickerState* state) {
  if (!state->font) {
    return -1;
  }
  if (!state->nQueue) {
    return 0;
  }
  setFont(state->font);
  m_magFactor = state->mag2X ? 2 : 1;
  if (state->init) {
    clear(state->bgnCol, state->endCol, state->row,
          state->row + fontRows() - 1);
    state->col = state->endCol;
    state->skip = 0;
    state->init = false;
  }
  // Adjust display width to truncate pixels after endCol.  Find better way?
  uint8_t save = m_displayWidth;
  m_displayWidth = state->endCol + 1;

  // Skip pixels before bgnCol.
  skipColumns(state->skip);
  setCursor(state->col, state->row);
  for (uint8_t i = 0; i < state->nQueue; i++) {
    const char* str = state->queue[i];
    while (*str && m_col <= state->endCol) {
      write(*str++);
    }
    if (m_col > state->endCol) {
      break;
    }
  }
  if (m_col <= state->endCol) {
    clear(m_col, m_col, state->row, state->row + fontRows() - 1);
  }
  // Restore display width.
  m_displayWidth = save;

  if (state->nQueue == 1 && *state->queue[0] == 0) {
    state->nQueue = 0;
    return 0;
  }
  if (state->col > state->bgnCol) {
    state->col--;
  } else {
    state->skip++;
    if (state->skip >= charSpacing(*state->queue[0])) {
      state->skip = 0;
      state->queue[0]++;
      if (*state->queue[0] == 0 && state->nQueue > 1) {
        state->nQueue--;
        for (uint8_t i = 0; i < state->nQueue; i++) {
          state->queue[i] = state->queue[i + 1];
        }
      }
    }
  }
  return state->nQueue;
}
//------------------------------------------------------------------------------
size_t SSD1306Ascii::write(uint8_t ch)
{
  if(font == NULL)return 0;
  if(ch == '\r'){ setCol(0); return 1; }

  uint8_t numRows = (font->height + 7) >> 3;

  if(ch == '\n') 
  {
    setCol(0);
    uint8_t fr = m_magFactor * numRows;
#if INCLUDE_SCROLLING
    uint8_t dr = displayRows();
    uint8_t tmpRow = m_row + fr;
    int8_t delta = tmpRow + fr - dr;
    if (m_scrollMode == SCROLL_MODE_OFF || delta <= 0) {
      setRow(tmpRow);
    } else {
      m_pageOffset = (m_pageOffset + delta) & 7;
      m_row = dr - fr;
      // Cursor will be positioned by clearToEOL.
      clearToEOL();
      if (m_scrollMode == SCROLL_MODE_AUTO) {
        setStartLine(8 * m_pageOffset);
      }
    }
#else   // INCLUDE_SCROLLING
    setRow(m_row + fr);
#endif  // INCLUDE_SCROLLING
    return 1;
  }

  if(ch < font->firstChar || ch >= (font->firstChar + font->charCount))return 0;
  ch -= font->firstChar;

  uint8_t width;
  uint8_t thieleShift = 0;
  const uint8_t* base = font->bitmapTable;

  if(font->charOffsetTable != NULL)
  {
    if(font->firstChar & 7)thieleShift = 8 - (font->firstChar & 7);
    uint16_t index = font->charOffsetTable[ch];
    width = font->charOffsetTable[ch + 1] - index;
    base += numRows * index;
  }
  else
  {
    base += numRows * font->width * ch;
    width = font->width;
  }

  uint8_t scol = m_col;
  uint8_t srow = m_row;
  uint8_t skip = m_skip;

  for(uint8_t raw = 0; raw < numRows; raw++)
  {
    for(uint8_t mag = 0; mag < m_magFactor; mag++)
    {
      skipColumns(skip);

      if(raw || mag)setCursor(scol, m_row + 1);
    
      for(uint8_t colmn = 0; colmn < width; colmn++) 
      {
        uint8_t byte = readFontByte(base + colmn + raw * width);

        if(thieleShift && (raw + 1) == numRows)byte >>= thieleShift;
        
        if(m_magFactor == 2) 
        {
          byte = mag ? byte >> 4 : byte & 0XF;
          byte = readFontByte(scaledNibble + byte);
          ssd1306WriteRamBuf(byte);
        }

        ssd1306WriteRamBuf(byte);
      }

      uint8_t spacing = letterSpacing();
      for(uint8_t i = 0; i < spacing; i++)ssd1306WriteRamBuf(0);
    }
  }

  setRow(srow);
  return 1;
}
//------------------------------------------------------------------------------
void SSD1306Ascii::print(const char *str)
{
    while (*str)
    {
      write(*str++);
    }
}
