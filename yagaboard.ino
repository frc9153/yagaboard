#include <FastLED.h>
#include "font.h"

#define LED_PIN 7
#define GRID_HEIGHT 8
#define GRID_WIDTH 32 * 2
#define NUM_LEDS GRID_HEIGHT * GRID_WIDTH

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

int pos_to_idx(int x, int y) {
  // TODO: MULTISCREEN
  if (x % 2 != 0) {
    y = GRID_HEIGHT - 1 - y;
  }

  int out = y + (x * GRID_HEIGHT);

  if (out > NUM_LEDS - 1) {
    // TODO: Find a graceful solution for this. If you do not have this bound
    // check the panel will turn white with random noise and the board will
    // get VERY HOT (!!!) I am adding this check because I just burned my finger
    return -1;
  }

  return out;
}

void wrap_right() {
  // Wraps the current screen contents to the right. Does not buffer.

  // ... except for this
  CRGB buffer[GRID_HEIGHT];

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      CRGB current_color = leds[pos_to_idx(x, y)];
      if (x == GRID_WIDTH - 1) current_color = buffer[y];

      if (x - 1 < 0) {
        buffer[y] = current_color;
      } else {
        leds[pos_to_idx(x - 1, y)] = current_color;
      }
    }
  }
}

int draw_string(char* text, int x, bool dry = false) {
  // Takes in text you want to draw and where you want to draw it, and
  // returns the last X coordinate drawn upon. Will not draw if `dry` is passed

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
      default:
        if (char_index >= FONT_BASE && char_index <= FONT_BASE + SUPPORTED_CHARACTERS - 1) {
          char_index -= FONT_BASE;
        } else {
          char_index = 28;  // ?
        }
    }

    char x_populated_mask = 0b00000000;
    for (int glyph_y = 0; glyph_y < GLYPH_HEIGHT; glyph_y++) {
      char row = pgm_read_byte(&(GLYPH_DATA[char_index][glyph_y]));

      for (int glyph_x = 0; glyph_x < 8; glyph_x++) {
        if (!bitRead(row, 7 - glyph_x)) continue;

        // Keep track of which columns are used so we can space letters based on content.
        bitSet(x_populated_mask, glyph_x - 1);

        if (dry) continue;

        int idx = pos_to_idx(x + glyph_x, glyph_y + (GRID_HEIGHT - GLYPH_HEIGHT));
        if (idx < 0) continue;

        leds[idx] = CRGB(10, 5, 0);
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

void loop() {
  const char* text = "BABA YAGA ` \nROBOTS R EVIL LOLOLOLOL!!! ";
  const int width = draw_string(text, 0, true);

  for (int i = 0; i < width + GRID_WIDTH; i++) {
    FastLED.clear();
    draw_string(text, GRID_WIDTH - i);
    FastLED.show();
    delay(20);
  }
}
