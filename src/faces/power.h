#ifndef WATCHY_SCREENS_POWER_H
#define WATCHY_SCREENS_POWER_H

#include <stdint.h>

#include "frame.h"        // kFaceLeftInset / kFaceRightInset
#include "power_static.h"
#include "power_glyphs.h"

// POWER face. The base layer is the 176×136 reference bitmap clipped
// to the octagon. The BIG center number ("27" in the reference) is
// then cleared and re-rendered from PowerData::sceneIndex (0..99) so
// it can animate. Other regions (time, corner numbers, date, weekday)
// remain from the static reference for now.
struct PowerData {
    int sceneIndex;  // BIG center value; rendered as two 29×47 digits.
};

template <typename Display>
void drawPowerFace(Display& display, int ox, int oy, const PowerData& data) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    // BIG slot positions, found by scanning power.png for the "27" bbox.
    constexpr int kBigY      = 39;
    constexpr int kBigLeftX  = 54;
    constexpr int kBigRightX = 93;
    constexpr int kBigW      = 29;
    constexpr int kBigH      = 47;

    // Base layer: blit the static reference, clipped to the octagon.
    for (int r = 0; r < GLYPH_POW_FACE_H; ++r) {
        const uint8_t* row = &GLYPH_POW_FACE_ROWS[r][0];
        const int xLo = kFaceLeftInset[r];
        const int xHi = GLYPH_POW_FACE_W - 1 - kFaceRightInset[r];
        for (int c = xLo; c <= xHi; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }

    // Clear the two BIG slots — the static "27" is still painted underneath.
    for (int r = 0; r < kBigH; ++r) {
        for (int c = 0; c < kBigW; ++c) {
            display.drawPixel(ox + kBigLeftX  + c, oy + kBigY + r, WHITE);
            display.drawPixel(ox + kBigRightX + c, oy + kBigY + r, WHITE);
        }
    }

    auto bigRows = [](int digit) -> const uint8_t* {
        switch (digit) {
            case 0: return &GLYPH_POW_BIG_0_ROWS[0][0];
            case 1: return &GLYPH_POW_BIG_1_ROWS[0][0];
            case 2: return &GLYPH_POW_BIG_2_ROWS[0][0];
            case 3: return &GLYPH_POW_BIG_3_ROWS[0][0];
            case 4: return &GLYPH_POW_BIG_4_ROWS[0][0];
            case 5: return &GLYPH_POW_BIG_5_ROWS[0][0];
            case 6: return &GLYPH_POW_BIG_6_ROWS[0][0];
            case 7: return &GLYPH_POW_BIG_7_ROWS[0][0];
            case 8: return &GLYPH_POW_BIG_8_ROWS[0][0];
            default: return &GLYPH_POW_BIG_9_ROWS[0][0];
        }
    };

    auto drawBig = [&](int digit, int slotX) {
        const uint8_t* rows = bigRows(digit);
        for (int r = 0; r < kBigH; ++r) {
            const uint8_t* row = rows + r * 4;
            for (int c = 0; c < kBigW; ++c) {
                if (row[c >> 3] & (0x80 >> (c & 7)))
                    display.drawPixel(ox + slotX + c, oy + kBigY + r, BLACK);
            }
        }
    };

    int n = data.sceneIndex;
    if (n < 0) n = 0;
    if (n > 99) n = 99;
    drawBig(n / 10, kBigLeftX);
    drawBig(n % 10, kBigRightX);
}

#endif // WATCHY_SCREENS_POWER_H
