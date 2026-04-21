#ifndef WATCHY_SCREENS_MULTIDAY_H
#define WATCHY_SCREENS_MULTIDAY_H

#include <stdint.h>

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
//   Title:        baseline y=23, centered at x=87
//   Left labels:  "12" at y=39, "0" at y=78, "12" at y=100  (x≈5, left-aligned)
//   Right labels: "6" at y=54, "3" at y=66, "18" at y=94   (x≈170, right-aligned)
//   WEEK labels:  baseline y=110, centered under each bar
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

    display.setTextColor(BLACK);

    // Title — baseline y=23, centered on x=87.
    {
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(data.title, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor(ox + 87 - (int)bw/2 - bx, oy + 23);
        display.print(data.title);
    }

    auto labelAt = [&](int x, int yBase, const char* s, bool rightAlign) {
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(s, 0, 0, &bx, &by, &bw, &bh);
        int cx = rightAlign ? (x - (int)bw - bx) : (x - bx);
        display.setCursor(cx, yBase);
        display.print(s);
    };
    // Left column (left-aligned).
    labelAt(ox +  5, oy + 39,  "12", false);
    labelAt(ox +  7, oy + 78,  "0",  false);
    labelAt(ox +  5, oy + 100, "12", false);
    // Right column (right-aligned at x=170).
    labelAt(ox + 170, oy + 54, "6",  true);
    labelAt(ox + 170, oy + 66, "3",  true);
    labelAt(ox + 170, oy + 94, "18", true);

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

    // Day letters centered under each column, baseline y=110.
    for (int i = 0; i < 7; ++i) {
        int bx = chartLeft + i * barStride + barW / 2;
        char s[2] = { data.dayLabels[i], 0 };
        int16_t tx, ty; uint16_t tw, th;
        display.getTextBounds(s, 0, 0, &tx, &ty, &tw, &th);
        display.setCursor(bx - (int)tw/2 - tx, oy + 110);
        display.print(s);
    }

    // Current-day dot.
    {
        int bx = chartLeft + data.currentDay * barStride + barW / 2;
        display.fillCircle(bx, oy + 115, 2, BLACK);
    }
}

#endif // WATCHY_SCREENS_MULTIDAY_H
