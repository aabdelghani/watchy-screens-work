#ifndef WATCHY_SCREENS_FRAME_H
#define WATCHY_SCREENS_FRAME_H

#include <stdint.h>

// 200×200 Watchy screen chrome. Outside the face octagon is filled with a
// 50% checkerboard dither; inside is plain white. The face is a 176×136
// octagon whose boundary was extracted pixel-exact from the alpha channel
// of references/multyday.png (the authoritative content reference). The
// per-row insets are slightly asymmetric where the reference's
// anti-aliasing snapped one way or the other; we keep the asymmetry so the
// rendered edge matches the reference exactly under an alpha>128 mask.
static const int8_t kFaceLeftInset[136] = {
     24, 23, 23, 22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 15, 15, 14,
     14, 13, 12, 12, 11, 10, 10,  9,  8,  8,  7,  6,  6,  5,  4,  4,
      3,  2,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  1,  1,  2,  2,  3,  4,  5,  5,  6,  6,  7,  8,
      9, 10, 10, 11, 12, 12, 13, 13, 14, 14, 15, 16, 16, 17, 18, 18,
     19, 20, 21, 21, 22, 23, 23, 24,
};
static const int8_t kFaceRightInset[136] = {
     24, 24, 23, 22, 22, 21, 20, 20, 19, 18, 18, 17, 16, 16, 15, 14,
     14, 13, 12, 12, 11, 10, 10,  9,  8,  8,  7,  7,  6,  5,  5,  4,
      3,  3,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,
      8,  9,  9, 10, 11, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17, 18,
     19, 20, 21, 21, 22, 23, 23, 24,
};

template <typename Display>
void drawWatchyChrome(Display& display) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;
    const int SCREEN_W  = 200;
    const int SCREEN_H  = 200;
    const int BOX_W     = 176;
    const int BOX_H     = 136;
    const int OX        = (SCREEN_W - BOX_W) / 2;  // 12
    const int OY        = 32;

    // 1. Fill whole screen with 50% dither (black pixels on white bg).
    for (int y = 0; y < SCREEN_H; ++y) {
        for (int x = 0; x < SCREEN_W; ++x) {
            if (((x + y) & 1) == 0) display.drawPixel(x, y, BLACK);
        }
    }

    // 2. Carve out white face interior using the per-row insets extracted
    //    from references/multyday.png.
    for (int dy = 0; dy < BOX_H; ++dy) {
        int x0 = OX + kFaceLeftInset[dy];
        int x1 = OX + BOX_W - 1 - kFaceRightInset[dy];
        for (int x = x0; x <= x1; ++x) display.drawPixel(x, OY + dy, WHITE);
    }
}

#endif // WATCHY_SCREENS_FRAME_H
