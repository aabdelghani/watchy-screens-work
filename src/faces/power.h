#ifndef WATCHY_SCREENS_POWER_H
#define WATCHY_SCREENS_POWER_H

#include <stdint.h>

#include "frame.h"        // kFaceLeftInset / kFaceRightInset
#include "power_static.h"

// First-cut POWER face renderer. The whole 176×136 reference is blitted
// as a single static bitmap, clipped to the octagon interior so the
// outline diagonals at the chamfer corners don't bleed into the white
// border area. Animation (rotating big number, corner numbers, time,
// date) is left for a follow-up; once we settle the layout we'll pull
// dynamic regions out and overlay glyphs from power_glyphs.h.
struct PowerData {
    int sceneIndex;  // Reserved for animation; ignored for now.
};

template <typename Display>
void drawPowerFace(Display& display, int ox, int oy, const PowerData& /*data*/) {
    const uint16_t BLACK = 0x0000;
    for (int r = 0; r < GLYPH_POW_FACE_H; ++r) {
        const uint8_t* row = &GLYPH_POW_FACE_ROWS[r][0];
        const int xLo = kFaceLeftInset[r];
        const int xHi = GLYPH_POW_FACE_W - 1 - kFaceRightInset[r];
        for (int c = xLo; c <= xHi; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }
}

#endif // WATCHY_SCREENS_POWER_H
