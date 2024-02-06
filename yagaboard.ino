#include <FastLED.h>
#include "font.h"

#define LED_PIN 7
#define GRID_HEIGHT 8
#define GRID_WIDTH 32
#define NUM_LEDS GRID_HEIGHT* GRID_WIDTH * 2

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  draw_string("BABAYAGA");
  FastLED.show();
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

void draw_string(char* text) {
  int x = 0;

  for (int i = 0; i < strlen(text); i++) {
    char c = text[i];
    if (c == ' ') {
      x += 8;
      continue;
    }
    
    if (c == '!') c = 26; // !
    else if (c == '`') c = 27; // :)
    else if (c >= FONT_BASE && c <= FONT_BASE + SUPPORTED_CHARACTERS - 1) {
      c -= FONT_BASE;
    } else {
      continue;
    }

    char x_populated_mask = 0b00000000;

    for (int glyph_y = 0; glyph_y < GLYPH_HEIGHT; glyph_y++) {
      char row = GLYPH_DATA[c][glyph_y];
      
      for (int glyph_x = 0; glyph_x < 8; glyph_x++) {
        if (!bitRead(row, 7 - glyph_x)) continue;
        
        // Keep track of which columns are used so we can space letters based on content.
        bitSet(x_populated_mask, glyph_x - 1);

        leds[pos_to_idx(x + glyph_x, glyph_y + (GRID_HEIGHT - GLYPH_HEIGHT))] = CRGB(5, 5, 0);
      }
    }

    uint8_t bit_sum = 0;
    while (x_populated_mask > 0) {
      bit_sum++;
      x_populated_mask &= x_populated_mask - 1;
    }

    x += bit_sum + 2;
  }
}

void loop() {
}
