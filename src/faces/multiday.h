#ifndef WATCHY_SCREENS_MULTIDAY_H
#define WATCHY_SCREENS_MULTIDAY_H

#include <stdint.h>

#include "multiday_glyphs.h"

// Per-bar state for the 7-night actogram.
struct MultidayBar {
    float bedtime;    // 24h float, e.g. 22.75 = 10:45 PM
    float wakeup;     // 24h float, e.g. 7.25 = 7:15 AM (next day, 0..12)
    bool  deltaUp;    // '+' marker = bedtime later than reference
    bool  deltaDown;  // '-' marker = bedtime earlier than reference
    bool  check;      // '✓' marker = on-target
};

struct MultidayData {
    const char* title;        // "NIGHTS"
    const char* dayLabels;    // 7 chars, e.g. "MTWTFSS"
    int         currentDay;   // 0..6, weekday that gets the dot marker
    MultidayBar bars[7];
};

// Geometry calibrated against references/multyday.ase (176×136 face),
// measured pixel-exact from per-layer PNG exports. All offsets face-relative:
// the caller passes ox=12, oy=32 to center the face in a 200×200 screen.
//
// Coordinates (face-relative):
//   Chart area:   x=17..158,  y=31..103   (141 wide × 73 tall)
//   Midline:      y=82        (asymmetric: 51 px above, 21 px below)
//   LINE overrun: x=14..161   (midline extends 3 px past chart each side)
//   Bars:         width 15, stride 21 (15 bar + 6 gap) at x=17,38,59,80,101,122,143
//   Title NIGHTS: top-left x=71, y=17  (31×7 blob)
//   Left labels:  "12" top-y 33 / "0" top-y 72 / "12" top-y 94  (left-aligned x≈5)
//   Right labels: "6" top-y 48 / "3" top-y 60 / "18" top-y 88   (right edge x≈168)
//   WEEK labels:  top y=105, each letter centered under its bar column
//   Day dot:      5-px circle, y=115
//
// Hour → Y mapping is intentionally asymmetric to match the PNG: the upper
// 51 px encodes noon→midnight (12 h), and the lower 21 px encodes midnight→
// 5 am (5 h). Labels "12/0/12" are the designer's cosmetic hint, not a
// strict scale — follow the PNG pixels, not the math.
template <typename Display>
void drawMultidayFace(Display& display, int ox, int oy, const MultidayData& data) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    const int chartTop    = oy + 31;
    const int chartBottom = oy + 103;
    const int chartMid    = oy + 82;
    const int chartLeft   = ox + 17;
    const int lineLeft    = ox + 14;
    const int lineRight   = ox + 161;
    const int barW        = 15;
    const int barStride   = 21;

    // Blit a packed-rows glyph: MSB-first, leftmost column = bit 7.
    // `rows` points at row 0; each row is `stride` bytes wide.
    auto blitGlyph = [&](int xLeft, int yTop, int w, int h, int stride,
                         const uint8_t* rows) {
        for (int r = 0; r < h; ++r) {
            const uint8_t* row = rows + r * stride;
            for (int c = 0; c < w; ++c) {
                if (row[c >> 3] & (0x80 >> (c & 7)))
                    display.drawPixel(xLeft + c, yTop + r, BLACK);
            }
        }
    };

    // Title NIGHTS — 31×7 blob at face-rel (71, 17).
    blitGlyph(ox + 71, oy + 17, GLYPH_NIGHTS_W, GLYPH_NIGHTS_H,
              GLYPH_NIGHTS_STRIDE, &GLYPH_NIGHTS_ROWS[0][0]);

    // Axis labels — left column (left-aligned).
    // Upper "12" at y=33
    blitGlyph(ox + 5, oy + 33, GLYPH_DIG_1_W, GLYPH_DIG_1_H,
              GLYPH_DIG_1_STRIDE, GLYPH_DIG_1_ROWS);
    blitGlyph(ox + 8, oy + 33, GLYPH_DIG_2_W, GLYPH_DIG_2_H,
              GLYPH_DIG_2_STRIDE, GLYPH_DIG_2_ROWS);
    // Middle "0" at y=72
    blitGlyph(ox + 7, oy + 72, GLYPH_DIG_0_W, GLYPH_DIG_0_H,
              GLYPH_DIG_0_STRIDE, GLYPH_DIG_0_ROWS);
    // Lower "12" at y=94
    blitGlyph(ox + 5, oy + 94, GLYPH_DIG_1_W, GLYPH_DIG_1_H,
              GLYPH_DIG_1_STRIDE, GLYPH_DIG_1_ROWS);
    blitGlyph(ox + 8, oy + 94, GLYPH_DIG_2_W, GLYPH_DIG_2_H,
              GLYPH_DIG_2_STRIDE, GLYPH_DIG_2_ROWS);

    // Right column (right-aligned at extracted PNG positions).
    // "6" at y=48, right edge x=167
    blitGlyph(ox + 164, oy + 48, GLYPH_DIG_6_W, GLYPH_DIG_6_H,
              GLYPH_DIG_6_STRIDE, GLYPH_DIG_6_ROWS);
    // "3" at y=60, right edge x=167
    blitGlyph(ox + 164, oy + 60, GLYPH_DIG_3_W, GLYPH_DIG_3_H,
              GLYPH_DIG_3_STRIDE, GLYPH_DIG_3_ROWS);
    // "18" at y=88, right edge x=169
    blitGlyph(ox + 163, oy + 88, GLYPH_DIG_1_W, GLYPH_DIG_1_H,
              GLYPH_DIG_1_STRIDE, GLYPH_DIG_1_ROWS);
    blitGlyph(ox + 166, oy + 88, GLYPH_DIG_8_W, GLYPH_DIG_8_H,
              GLYPH_DIG_8_STRIDE, GLYPH_DIG_8_ROWS);

    auto hourToY = [&](float h) -> int {
        if (h <= 0.0f) {
            float t = (h + 12.0f) / 12.0f;
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            return chartTop + (int)(t * (chartMid - chartTop));
        } else {
            float t = h / 5.0f;
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            return chartMid + (int)(t * (chartBottom - chartMid));
        }
    };

    for (int i = 0; i < 7; ++i) {
        int bx = chartLeft + i * barStride;
        int bh = chartBottom - chartTop + 1;

        // Full-column checkerboard dither.
        for (int yy = 0; yy < bh; ++yy) {
            for (int xx = 0; xx < barW; ++xx) {
                if (((xx + yy) & 1) == 0)
                    display.drawPixel(bx + xx, chartTop + yy, BLACK);
            }
        }

        // Solid-black sleep interval.
        float bed  = data.bars[i].bedtime;
        float wake = data.bars[i].wakeup;
        float bedOff  = (bed >= 12.0f) ? (bed - 24.0f) : bed;
        float wakeOff = wake;
        int y0 = hourToY(bedOff);
        int y1 = hourToY(wakeOff);
        if (y0 > y1) { int t = y0; y0 = y1; y1 = t; }
        display.fillRect(bx, y0, barW, y1 - y0 + 1, BLACK);

        // White markers inside solid fill.
        int cx = bx + barW / 2;
        int fillH = y1 - y0;
        if (data.bars[i].check && fillH >= 8) {
            int cy = y0 + 5;
            display.drawLine(cx - 2, cy,     cx - 1, cy + 2, WHITE);
            display.drawLine(cx - 1, cy + 2, cx + 2, cy - 1, WHITE);
        }
        if (data.bars[i].deltaUp && fillH >= 10) {
            int cy = y0 + fillH / 4;
            display.drawFastHLine(cx - 2, cy, 5, WHITE);
            display.drawLine(cx, cy - 2, cx, cy + 2, WHITE);
        }
        if (data.bars[i].deltaDown && fillH >= 10) {
            int cy = y1 - fillH / 4;
            display.drawFastHLine(cx - 2, cy, 5, WHITE);
        }
    }

    // Midline (LINE layer overruns the chart by 3 px each side).
    display.drawFastHLine(lineLeft, chartMid, lineRight - lineLeft + 1, BLACK);

    // Day letters, top y=105. Per-slot x positions measured from the PNG:
    // slots 4 and 6 are nudged right in the reference, so we use a lookup
    // instead of centering algorithmically.
    auto dayGlyph = [&](char c, int& w, int& h, int& stride,
                        const uint8_t*& rows) -> bool {
        switch (c) {
            case 'M': w = GLYPH_DAY_M_W; h = GLYPH_DAY_M_H;
                      stride = GLYPH_DAY_M_STRIDE; rows = GLYPH_DAY_M_ROWS;
                      return true;
            case 'T': w = GLYPH_DAY_T_W; h = GLYPH_DAY_T_H;
                      stride = GLYPH_DAY_T_STRIDE; rows = GLYPH_DAY_T_ROWS;
                      return true;
            case 'W': w = GLYPH_DAY_W_W; h = GLYPH_DAY_W_H;
                      stride = GLYPH_DAY_W_STRIDE; rows = GLYPH_DAY_W_ROWS;
                      return true;
            case 'F': w = GLYPH_DAY_F_W; h = GLYPH_DAY_F_H;
                      stride = GLYPH_DAY_F_STRIDE; rows = GLYPH_DAY_F_ROWS;
                      return true;
            case 'S': w = GLYPH_DAY_S_W; h = GLYPH_DAY_S_H;
                      stride = GLYPH_DAY_S_STRIDE; rows = GLYPH_DAY_S_ROWS;
                      return true;
        }
        return false;
    };
    const int dayX[7] = { 22, 44, 64, 86, 108, 127, 150 };
    for (int i = 0; i < 7; ++i) {
        int gw, gh, gs; const uint8_t* gr;
        if (dayGlyph(data.dayLabels[i], gw, gh, gs, gr)) {
            blitGlyph(ox + dayX[i], oy + 105, gw, gh, gs, gr);
        }
    }

    // Current-day dot.
    {
        int bx = chartLeft + data.currentDay * barStride + barW / 2;
        display.fillCircle(bx, oy + 115, 2, BLACK);
    }
}

#endif // WATCHY_SCREENS_MULTIDAY_H
