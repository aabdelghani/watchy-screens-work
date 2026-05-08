#ifndef WATCHY_SCREENS_BIOSYNC_H
#define WATCHY_SCREENS_BIOSYNC_H

#include <stdint.h>

#include "biosync_static.h"

// BIOSYNC face. Reference: references/biosync.png. For now a pure
// static blit of the 176x136 reference — the dynamic overlays (BIG
// value, time/date/weekday, perimeter dial) will be wired up in a
// follow-up pass, mirroring the HOURLY rollout.
struct BiosyncData {
    int sceneIndex;   // placeholder for future animation
};

template <typename Display>
void drawBiosyncFace(Display& display, int ox, int oy, const BiosyncData& /*data*/) {
    const uint16_t BLACK = 0x0000;
    for (int r = 0; r < GLYPH_BIO_FACE_H; ++r) {
        const uint8_t* row = GLYPH_BIO_FACE_ROWS[r];
        for (int c = 0; c < GLYPH_BIO_FACE_W; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }
}

#endif // WATCHY_SCREENS_BIOSYNC_H
