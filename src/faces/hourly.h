#ifndef WATCHY_SCREENS_HOURLY_H
#define WATCHY_SCREENS_HOURLY_H

#include <stdint.h>
#include <gfxfont.h>

#include "frame.h"        // kFaceLeftInset / kFaceRightInset
#include "hourly_static.h"
#include "hourly_glyphs.h"
#include "power.h"        // reuses watchy_power::drawSm/drawGfxChar/drawGfxStr/smLetter/dowName
#include "../fonts/WatchyDigits10x15.h"
#include "../fonts/WatchyDigits5x7.h"

// HOURLY (PERFORMANCE) face. Reference: references/hourly.png. The
// 176×136 chrome (title, ACTIVE MINUTES subtitle, clock-position
// labels 60/45/15/30, frame outline) is the static blit. On top we
// re-draw the dynamic regions:
//   - BIG sceneIndex (00..99) at 17×32 per digit
//   - HH:MM at 10×15
//   - D/M at 5×7
//   - DOW (3 letters) at 5×7 SM
//   - The single black dot indicator at the top
struct HourlyData {
    int sceneIndex;     // 0..99 BIG center value (was static "42")
    int hour;           // 0..23
    int minute;         // 0..59
    int day;            // 1..31
    int month;          // 1..12
    int dowIndex;       // 0=MON .. 6=SUN
    int dotX;           // face-rel x of dot center; reference value 87
    int checkerVariant; // 0..3 — pattern drawn in the four corner chevron boxes
};

template <typename Display>
void drawHourlyFace(Display& display, int ox, int oy, const HourlyData& data) {
    using namespace watchy_power;
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    // ── Slot positions (face-relative, scanned from hourly.png) ──
    // BIG "42": left digit 17×32 at (70, 50), right at (95, 50).
    // 8-px gap between digits at cols 87..94 — wider than the static
    // reference's 4-px gap so the digits don't read as cramped on
    // tight pairings (e.g. "06", "08", "25").
    constexpr int kBigY      = 50;
    constexpr int kBigLeftX  = 70;
    constexpr int kBigRightX = 95;
    constexpr int kBigW      = 17;
    constexpr int kBigH      = 32;

    // Time HH:MM, 10×15. Anchored at x=47 — 5 px left of the original
    // reference position so the time block reads with more breathing
    // room on the left and even all-wide pairings like "08:08" stay
    // well clear of the date column at x=105.
    constexpr int kTimeX = 47, kTimeY = 101;
    constexpr int kTimeW = 56, kTimeH = 15;

    // Date D/M and weekday SUN, stacked on the right. Anchored at
    // x=105 (was 106) so the leftmost static-art pixel of '4' / 'S'
    // (col 105) gets cleared and replaced by the dynamic glyph.
    constexpr int kDateX = 105, kDateY = 100;
    constexpr int kDateW = 22,  kDateH = 7;
    constexpr int kDowX  = 105, kDowY = 109;
    constexpr int kDowW  = 22,  kDowH = 7;

    // 1. Base: blit static reference, clipped to octagon.
    for (int r = 0; r < GLYPH_HRL_FACE_H; ++r) {
        const uint8_t* row = &GLYPH_HRL_FACE_ROWS[r][0];
        const int xLo = kFaceLeftInset[r];
        const int xHi = GLYPH_HRL_FACE_W - 1 - kFaceRightInset[r];
        for (int c = xLo; c <= xHi; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }

    // 2. Clear dynamic slots back to white.
    auto clearRect = [&](int x, int y, int w, int h) {
        for (int r = 0; r < h; ++r)
            for (int c = 0; c < w; ++c)
                display.drawPixel(ox + x + c, oy + y + r, WHITE);
    };
    // BIG slot spans the leftmost-edge of the left digit to the
    // rightmost-edge of the right digit (cols 70..109 with the
    // current layout). Computed from the layout constants so
    // changes to spacing automatically resize the clear.
    clearRect(kBigLeftX, kBigY, kBigRightX + kBigW - kBigLeftX, kBigH);
    clearRect(kTimeX,    kTimeY, kTimeW, kTimeH);
    clearRect(kDateX,    kDateY, kDateW, kDateH);
    clearRect(kDowX,     kDowY,  kDowW,  kDowH);
    // Static dot above "60" is left intact (no clear / no redraw).

    // 3. BIG digits.
    auto bigRows = [](int digit) -> const uint8_t* {
        switch (digit) {
            case 0: return &GLYPH_HRL_BIG_0_ROWS[0][0];
            case 1: return &GLYPH_HRL_BIG_1_ROWS[0][0];
            case 2: return &GLYPH_HRL_BIG_2_ROWS[0][0];
            case 3: return &GLYPH_HRL_BIG_3_ROWS[0][0];
            case 4: return &GLYPH_HRL_BIG_4_ROWS[0][0];
            case 5: return &GLYPH_HRL_BIG_5_ROWS[0][0];
            case 6: return &GLYPH_HRL_BIG_6_ROWS[0][0];
            case 7: return &GLYPH_HRL_BIG_7_ROWS[0][0];
            case 8: return &GLYPH_HRL_BIG_8_ROWS[0][0];
            default: return &GLYPH_HRL_BIG_9_ROWS[0][0];
        }
    };
    auto drawBig = [&](int digit, int slotX) {
        const uint8_t* rows = bigRows(digit);
        for (int r = 0; r < kBigH; ++r) {
            const uint8_t* row = rows + r * GLYPH_HRL_BIG_STRIDE;
            for (int c = 0; c < kBigW; ++c) {
                if (row[c >> 3] & (0x80 >> (c & 7)))
                    display.drawPixel(ox + slotX + c, oy + kBigY + r, BLACK);
            }
        }
    };
    int n = data.sceneIndex;
    if (n < 0)  n = 0;
    if (n > 99) n = 99;
    drawBig(n / 10, kBigLeftX);
    drawBig(n % 10, kBigRightX);

    // 4. Time HH:MM via WatchyDigits10x15. Each digit advances by its
    //    own xAdvance (so wide pairs like "30", "20", "08" don't
    //    overlap), with a 2-px padding around the colon so it doesn't
    //    read as flush against the digits.
    {
        const int yB = oy + kTimeY + 14;
        char h1 = '0' + (data.hour   / 10) % 10;
        char h2 = '0' + (data.hour   % 10);
        char m1 = '0' + (data.minute / 10) % 10;
        char m2 = '0' + (data.minute % 10);
        int x = ox + kTimeX;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, h1, BLACK);
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, h2, BLACK);
        x += 2;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, ':', BLACK);
        x += 2;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, m1, BLACK);
        drawGfxChar(display, WatchyDigits10x15, x, yB, m2, BLACK);
    }

    // 5. Date "D/M" at (kDateX, kDateY). Single-digit day/month per
    //    the reference scene "4/5".
    {
        const int yB = oy + kDateY + 6;
        char dDigit = '0' + (data.day   % 10);
        char mDigit = '0' + (data.month % 10);
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX +  0, yB, dDigit, BLACK);
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX +  7, yB, '/',    BLACK);
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX + 16, yB, mDigit, BLACK);
    }

    // 6. Weekday "MON".."SUN" via 5×7 SM letters, advance 7 (5 width + 2 gap).
    {
        const char* name = dowName(data.dowIndex);
        int x = kDowX;
        for (int i = 0; name[i]; ++i) {
            const uint8_t* g = smLetter(name[i]);
            if (g) drawSm(display, g, ox + x, oy + kDowY, 5, 7, BLACK);
            x += 7;
        }
    }

    // (Dot indicator and per-scene corner chevrons are intentionally
    // disabled for now. The static art keeps the reference dot above
    // "60" and the static fish-scale chevrons at the four corners.
    // HourlyData::dotX and HourlyData::checkerVariant remain in the
    // struct for future re-enabling.)
}

#endif // WATCHY_SCREENS_HOURLY_H
