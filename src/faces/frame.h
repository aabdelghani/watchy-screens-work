#ifndef WATCHY_SCREENS_FRAME_H
#define WATCHY_SCREENS_FRAME_H

#include <stdint.h>

// 200×200 Watchy screen chrome per references/200x200.png:
//   - dithered-gray (50% checkerboard) everywhere by default
//   - a 176×136 white content box centered horizontally (ox=12), offset
//     oy=32 from top (20px header, 32px footer of gray dither)
//   - content box has 16-px chamfered octagonal corners
//   - a single-pixel black outline around the octagon
//
// Each face template renders *inside* the white content box using
// face-relative coordinates (ox=12, oy=32).
template <typename Display>
void drawWatchyChrome(Display& display) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;
    const int SCREEN_W = 200;
    const int SCREEN_H = 200;
    const int BOX_W = 176;
    const int BOX_H = 136;
    const int OX = (SCREEN_W - BOX_W) / 2;          // 12
    const int OY = 32;                               // header gap
    const int CHAMFER = 16;

    // 1. Fill whole screen with 50% dither (black pixels on white GxEPD bg).
    for (int y = 0; y < SCREEN_H; ++y) {
        for (int x = 0; x < SCREEN_W; ++x) {
            if (((x + y) & 1) == 0) display.drawPixel(x, y, BLACK);
        }
    }

    // 2. Carve out the octagonal content box: fill interior white.
    for (int dy = 0; dy < BOX_H; ++dy) {
        int inset = 0;
        if (dy < CHAMFER)            inset = CHAMFER - dy;
        else if (dy >= BOX_H - CHAMFER) inset = dy - (BOX_H - CHAMFER) + 1;
        int x0 = OX + inset;
        int x1 = OX + BOX_W - 1 - inset;
        for (int x = x0; x <= x1; ++x) display.drawPixel(x, OY + dy, WHITE);
    }

    // 3. Draw black octagonal outline around the content box.
    //    Top + bottom straight edges.
    display.drawFastHLine(OX + CHAMFER, OY,                  BOX_W - 2*CHAMFER, BLACK);
    display.drawFastHLine(OX + CHAMFER, OY + BOX_H - 1,      BOX_W - 2*CHAMFER, BLACK);
    //    Left + right straight edges.
    for (int dy = CHAMFER; dy < BOX_H - CHAMFER; ++dy) {
        display.drawPixel(OX,              OY + dy, BLACK);
        display.drawPixel(OX + BOX_W - 1,  OY + dy, BLACK);
    }
    //    Four 45° chamfers (one pixel per step on the diagonal).
    for (int i = 0; i < CHAMFER; ++i) {
        int up = OY + i;
        int dn = OY + BOX_H - 1 - i;
        int lx = OX + (CHAMFER - i);
        int rx = OX + BOX_W - 1 - (CHAMFER - i);
        display.drawPixel(lx, up, BLACK);
        display.drawPixel(rx, up, BLACK);
        display.drawPixel(lx, dn, BLACK);
        display.drawPixel(rx, dn, BLACK);
    }
}

#endif // WATCHY_SCREENS_FRAME_H
