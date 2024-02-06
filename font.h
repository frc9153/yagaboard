#define GLYPH_HEIGHT 7
#define FONT_BASE 0x41  // ASCII A
#define SUPPORTED_CHARACTERS 30

// Keep the mem low!

const char GLYPH_DATA[SUPPORTED_CHARACTERS][GLYPH_HEIGHT] = {
  // A
  {
    0b01100000,
    0b10010000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,
    0b10010000,
  },
  // B
  {
    0b11100000,
    0b10010000,
    0b10010000,
    0b11100000,
    0b10010000,
    0b10010000,
    0b11100000,
  },
  // C
  {
    0b01110000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b01110000,
  },
  // D
  {
    0b11100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11100000,
  },
  // E
  {
    0b11110000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000,
    0b11110000,
  },
  // F
  {
    0b11110000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000,
    0b10000000,
  },
  // G
  {
    0b01100000,
    0b10010000,
    0b10000000,
    0b10000000,
    0b10110000,
    0b10010000,
    0b01100000,
  },
  // H
  {
    0b10010000,
    0b10010000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,
    0b10010000,
  },
  // I
  {
    0b11111000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b11111000,
  },
  // J
  {
    0b00111000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b10010000,
    0b01100000,
  },
  // K
  {
    0b10010000,
    0b10010000,
    0b10100000,
    0b11000000,
    0b10100000,
    0b10100000,
    0b10010000,
  },
  // L
  {
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11110000,
  },
  // M
  {
    0b01010000,
    0b10101000,
    0b10101000,
    0b10101000,
    0b10101000,
    0b10101000,
    0b10101000,
  },
  // N
  {
    0b10001000,
    0b11001000,
    0b10101000,
    0b10101000,
    0b10011000,
    0b10001000,
    0b10001000,
  },
  // O
  {
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
  },
  // P
  {
    0b11100000,
    0b10010000,
    0b10010000,
    0b11100000,
    0b10000000,
    0b10000000,
    0b10000000,
  },
  // Q
  {
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10110000,
    0b01110000,
    0b00001000,
  },
  // R
  {
    0b11100000,
    0b10010000,
    0b10010000,
    0b11100000,
    0b10010000,
    0b10010000,
    0b10010000,
  },
  // S
  {
    0b01100000,
    0b10010000,
    0b10000000,
    0b01100000,
    0b00010000,
    0b10010000,
    0b01100000,
  },
  // T
  {
    0b11111000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
  },
  // U
  {
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
  },
  // V
  {
    0b10001000,
    0b10001000,
    0b10001000,
    0b01010000,
    0b01010000,
    0b01010000,
    0b00100000,
  },
  // W
  {
    0b10001000,
    0b10001000,
    0b10001000,
    0b10101000,
    0b10101000,
    0b01010000,
    0b01010000,
  },
  // X
  {
    0b10001000,
    0b10001000,
    0b01010000,
    0b00100000,
    0b01010000,
    0b10001000,
    0b10001000,
  },
  // Y
  {
    0b10001000,
    0b10001000,
    0b01010000,
    0b01010000,
    0b00100000,
    0b00100000,
    0b00100000,
  },
  // Z
  {
    0b11111000,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b11111000,
  },
  // !
  {
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b10000000,
  },
  // :)
  {
    0b01010000,
    0b01010000,
    0b01010000,
    0b00000000,
    0b00000000,
    0b10001000,
    0b01110000,
  },
};