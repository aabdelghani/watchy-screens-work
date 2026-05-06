// Wokwi custom chip: SSD1681 1.54" 200×200 e-paper controller (Watchy V3 panel).
//
// Minimal first-pass: paint a static test pattern via buffer_write so we can
// confirm the chip renders at all. SPI command parsing will land in the next
// commit once the rendering path is verified.

#include "wokwi-api.h"
#include <stdint.h>
#include <stdlib.h>

#define DISPLAY_W 200
#define DISPLAY_H 200

typedef struct {
    uint8_t r, g, b, a;
} rgba_t;

typedef struct {
    buffer_t framebuffer;
    uint32_t width;
    uint32_t height;
} chip_state_t;

void chip_init(void) {
    chip_state_t *chip = malloc(sizeof(chip_state_t));
    chip->width  = DISPLAY_W;
    chip->height = DISPLAY_H;
    chip->framebuffer = framebuffer_init(&chip->width, &chip->height);

    rgba_t white  = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF };
    rgba_t black  = { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF };
    rgba_t accent = { .r = 0x00, .g = 0x60, .b = 0xC0, .a = 0xFF };

    for (uint32_t y = 0; y < chip->height; ++y) {
        for (uint32_t x = 0; x < chip->width; ++x) {
            uint32_t offset = (y * chip->width + x) * 4u;
            rgba_t color = white;
            // Border + diagonal stripes so we can tell rendering is alive.
            if (x < 4 || x >= chip->width - 4 || y < 4 || y >= chip->height - 4) {
                color = black;
            } else if (((x ^ y) & 8) == 0) {
                color = accent;
            }
            buffer_write(chip->framebuffer, offset, &color, sizeof(color));
        }
    }
}
