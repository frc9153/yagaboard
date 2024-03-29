#include <FastLED.h>
#include "font.h"
#include "image.h"

#define LED_PIN 7
#define INDIVIDUAL_HEIGHT 8
#define GRID_HEIGHT 16
#define GRID_WIDTH 32
#define NUM_LEDS GRID_HEIGHT* GRID_WIDTH
#define FLIP_SECOND_BOARD true

const CRGB COLORS[] = {
  CRGB(0, 0, 0),        // Black
  CRGB(25, 25, 25),     // Dark Grey
  CRGB(75, 75, 75),  // Gr(a|e)y
  CRGB(255, 255, 255),  // White
  CRGB(255, 0, 0),      // Red
  CRGB(0, 255, 0),      // Green
  CRGB(0, 45, 0),      // Dark Green
  CRGB(0, 0, 255),      // Blue
  CRGB(255, 0, 255),    // Magenta
  CRGB(127, 0, 255),    // Purple
  CRGB(0, 255, 255),    // Cyan
  CRGB(255, 255, 0),    // Yellow
  CRGB(255, 127, 0),    // Orange
  CRGB(150, 100, 0),    // Brown
};

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(25);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  //draw_string("123456\n0.789", 0, false, CRGB(255, 255, 255));
  //leds[pos_to_idx(0, 0)] = CRGB(0, 255, 0);
  //leds[pos_to_idx(1, 1)] = CRGB(0, 0, 255);
  draw_image();
  FastLED.show();
}

int pos_to_idx(int x, int y) {
  int out = 0;

  if (x >= GRID_WIDTH) return -1;
  if (y >= GRID_HEIGHT) return -1;

  if (y >= INDIVIDUAL_HEIGHT) {
    out += INDIVIDUAL_HEIGHT * GRID_WIDTH;
    y = GRID_HEIGHT - y - 1;
    x = GRID_WIDTH - x - 1;
  }

  if (x % 2 != 0) {
    y = INDIVIDUAL_HEIGHT - 1 - (y % 8);
  }


  out += (y % 8) + (x * INDIVIDUAL_HEIGHT);

  if (out > NUM_LEDS - 1) {
    // TODO: Find a graceful solution for this. If you do not have this bound
    // check the panel will turn white with random noise and the board will
    // get VERY HOT (!!!) I am adding this check because I just burned my finger
    return -1;
  }

  return out;
}

int draw_string(char* text, int x, bool dry, CRGB color) {
  // Takes in text you want to draw and where you want to draw it, and
  // returns the last X coordinate drawn upon. Will not draw if `dry` is passed
  int y = 0;

  for (int i = 0; i < strlen(text); i++) {
    char char_index = text[i];
    switch (char_index) {
      case ' ':
        x += 8;
        continue;
      case '\n':
        x += GRID_WIDTH;
        continue;
      case '!':
        char_index = 26;  // !
        break;
      case '`':
        char_index = 27;  // :)
        break;
      case '?':
        char_index = 28;  // ?
        break;
      case '.':
        char_index = 29;  // .
        break;
      case '\'':
        char_index = 30;  // '
        break;
      case '>':
        char_index = 31;  // >
        break;
      case '<':
        char_index = 32;  // <
        break;
      case '0':
        char_index = 33;  // 0
        break;
      case '1':
        char_index = 34;  // 1
        break;
      case '2':
        char_index = 35;  // 2
        break;
      case '3':
        char_index = 36;  // 3
        break;
      case '4':
        char_index = 37;  // 4
        break;
      case '5':
        char_index = 38;  // 5
        break;
      case '6':
        char_index = 39;  // 6
        break;
      case '7':
        char_index = 40;  // 7
        break;
      case '8':
        char_index = 41;  // 8
        break;
      case '9':
        char_index = 42;  // 9
        break;
      default:
        if (char_index >= FONT_BASE && char_index <= FONT_BASE + SUPPORTED_CHARACTERS - 1) {
          char_index -= FONT_BASE;
        } else {
          Serial.println(char_index);
          char_index = 28;  // ?
        }
    }


    char x_populated_mask = 0b00000000;
    for (int glyph_y = 0; glyph_y < GLYPH_HEIGHT; glyph_y++) {
      char row = pgm_read_byte(&(GLYPH_DATA[char_index][glyph_y]));
      
      if (x + 7 > GRID_WIDTH) {
        y += INDIVIDUAL_HEIGHT;
        x = 0;
      }

      for (int glyph_x = 0; glyph_x < 8; glyph_x++) {
        if (!bitRead(row, 7 - glyph_x)) continue;

        // Keep track of which columns are used so we can space letters based on content.
        bitSet(x_populated_mask, glyph_x - 1);

        if (dry) continue;


        int idx = pos_to_idx(x + glyph_x, y + glyph_y + (INDIVIDUAL_HEIGHT - GLYPH_HEIGHT));
        if (idx < 0) continue;

        leds[idx] = color;
      }
    }

    uint8_t bit_sum = 0;
    while (x_populated_mask > 0) {
      bit_sum++;
      x_populated_mask &= x_populated_mask - 1;
    }

    x += bit_sum + 2;
  }

  return x;
}

void print_binary(byte inByte, int go) {
  for (int b = go - 1; b >= 0; b--) {
    Serial.print(bitRead(inByte, b));
  }
  Serial.print("\n");
}

void draw_image() {
  int x = 0;
  int y = 0;

  // TODO: First two bytes will be 12 bits for size and 4 bits for repeat size
  // (or so) for animations. OR MAYBE IT WILL FIX THAT STUPID LAST PACKET BUG
  uint8_t bit_bite_this_part_away = pgm_read_byte(&(IMAGE_DATA[0]));
  uint8_t repeat_size_bits = pgm_read_byte(&(IMAGE_DATA[1]));

  uint32_t bit_index = (8 * 2);
  uint32_t bit_length = (sizeof(IMAGE_DATA) * 8) - (8 - bit_bite_this_part_away);
  char byte_content = 0x00;
  // length 00001001
  // 111111111
  // 1011

  auto eat_bits = [&](int bit_count) mutable {
    // We will output an n bit long variable read from the byte stream
    uint16_t out = 0;

    while (bit_count--) {
      if (bit_index >= bit_length) break;
      // If we're about to read the 8th bit from the byte, we need to get the
      // next byte into place. It's okay that this condition clears initally,
      // as the first byte is the header and we want to skip that anyway.
      if (bit_index % 8 == 0) {
        byte_content = pgm_read_byte(&(IMAGE_DATA[bit_index / 8]));
        if (bit_index / 8 == sizeof(IMAGE_DATA) - 1) {
          byte_content = byte_content << (8 - bit_bite_this_part_away);
        }
      }
      // Shift out left by one bit, and fill in the gap with the right bit from
      // the byte we last read.
      out = (out << 1) | ((byte_content >> (7 - (bit_index % 8))) & 0b1);
      bit_index++;
    }

    return out;
  };

  FastLED.clear();

  while (bit_index < bit_length) {
    uint16_t repeat_count = eat_bits(repeat_size_bits);
    uint16_t color_index = eat_bits(4);

    for (int i = 0; i < repeat_count; i++) {
      leds[pos_to_idx(x, y)] = COLORS[color_index];
      if (++x >= GRID_WIDTH) {
        x = 0;
        if (++y >= GRID_HEIGHT) break;
      }
    }
  }

  FastLED.show();
}

void loop() {
  return;
}
