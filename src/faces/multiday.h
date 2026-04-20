#ifndef WATCHY_SCREENS_MULTIDAY_H
#define WATCHY_SCREENS_MULTIDAY_H

#include <stdint.h>
#include <string.h>

// Per-bar state for the 7-night history.
struct MultidayBar {
    float bedtime;   // 24h float, e.g. 22.75 = 10:45 PM
    float wakeup;    // 24h float, e.g. 7.25  = 7:15 AM  (next day, 0..12)
    bool  deltaUp;   // renders '+' marker inside bar (above midline)
    bool  deltaDown; // renders '-' marker inside bar (below midline)
    bool  check;     // renders a small tick inside the bar
};

struct MultidayData {
    const char* title;        // "NIGHTS"
    const char* dayLabels;    // 7 chars, e.g. "MTWTFSS"
    int         currentDay;   // 0..6, which day gets the dot marker below
    MultidayBar bars[7];
};

// drawMultidayFace: header-only template so the same source compiles
// against GxEPD2_GFX on hardware and against DisplaySim in the Qt simulator.
// All calls go through the Adafruit_GFX subset: drawLine, drawRect, fillRect,
// drawPixel, drawCircle, fillCircle, drawFastHLine, setCursor, setFont, print.
template <typename Display>
void drawMultidayFace(Display& display, int ox, int oy, const MultidayData& data) {
    // Face content is 176x136; caller passes ox=12, oy=32 to center inside 200x200.
    const int W = 176;
    const int H = 136;
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    // 1. Octagonal frame (chamfered 16px corners).
    const int C = 16;
    display.drawFastHLine(ox + C,        oy,           W - 2*C, BLACK);
    display.drawFastHLine(ox + C,        oy + H - 1,   W - 2*C, BLACK);
    for (int dy = 0; dy < H; ++dy) {
        int x0 = ox;
        int x1 = ox + W - 1;
        if (dy < C)            { x0 += (C - dy); x1 -= (C - dy); }
        else if (dy >= H - C)  { int d = dy - (H - C) + 1; x0 += d; x1 -= d; }
        display.drawPixel(x0, oy + dy, BLACK);
        display.drawPixel(x1, oy + dy, BLACK);
    }

    // 2. Title "NIGHTS" centered at top (default font, height ~8).
    display.setTextColor(BLACK);
    {
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(data.title, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor(ox + (W - (int)bw) / 2 - bx, oy + 12);
        display.print(data.title);
    }

    // 3. Chart geometry. Y-range chosen to leave room for day labels + dot below.
    const int chartTop    = oy + 18;
    const int chartBottom = oy + H - 22;
    const int chartMid    = (chartTop + chartBottom) / 2;
    const int chartLeft   = ox + 20;
    const int chartRight  = ox + W - 22;
    const int chartW      = chartRight - chartLeft;
    const int barW        = 15;
    const int barGap      = (chartW - 7*barW) / 6;

    // 4. Y-axis labels (left: 12/0/12, right: 6/0/18 — 24h scale).
    auto labelLeft = [&](int y, const char* s) {
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(s, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor(chartLeft - 4 - (int)bw - bx, y + (int)bh/2);
        display.print(s);
    };
    auto labelRight = [&](int y, const char* s) {
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(s, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor(chartRight + 4 - bx, y + (int)bh/2);
        display.print(s);
    };
    labelLeft(chartTop,    "12");
    labelLeft(chartMid,    "0");
    labelLeft(chartBottom, "12");
    labelRight(chartTop,    "6");
    labelRight(chartMid,    "0");
    labelRight(chartBottom, "18");

    // 5. Each bar: dithered outline + solid fill between bedtime and wakeup.
    //    Y maps: chartTop = -12h (noon prev day), chartMid = 0 (midnight),
    //            chartBottom = +12h (noon next day).
    // Chart Y-axis spans ±8h from midnight (16h window).
    // Axis labels ("12/0/12" left, "6/0/18" right) are cosmetic; the data-to-pixel
    // mapping below is the single source of truth for bar placement.
    auto hourToY = [&](float h) {
        float c = (h < -8.0f) ? -8.0f : (h > 8.0f ? 8.0f : h);
        float t = (c + 8.0f) / 16.0f;
        return chartTop + (int)(t * (chartBottom - chartTop));
    };

    for (int i = 0; i < 7; ++i) {
        int bx = chartLeft + i * (barW + barGap);
        int by = chartTop;
        int bh = chartBottom - chartTop;

        // Dithered background (50% checkerboard: matches reference density).
        for (int y = 0; y < bh; ++y) {
            for (int x = 0; x < barW; ++x) {
                if (((x + y) & 1) == 0) display.drawPixel(bx + x, by + y, BLACK);
            }
        }

        // Convert bedtime/wakeup to "hours from midnight" in [-12, 12].
        float bed = data.bars[i].bedtime;
        float wake = data.bars[i].wakeup;
        float bedOff = (bed >= 12.0f) ? (bed - 24.0f) : bed;   // evening -> negative
        float wakeOff = wake;                                   // morning -> positive

        int y0 = hourToY(bedOff);
        int y1 = hourToY(wakeOff);
        if (y0 > y1) { int t = y0; y0 = y1; y1 = t; }
        display.fillRect(bx, y0, barW, y1 - y0 + 1, BLACK);

        // Inside markers on the solid fill.
        // check mark (simple V shape) near top of solid region
        if (data.bars[i].check) {
            int cx = bx + barW/2;
            int cy = y0 + 6;
            display.drawLine(cx - 3, cy - 1, cx - 1, cy + 2, WHITE);
            display.drawLine(cx - 1, cy + 2, cx + 3, cy - 3, WHITE);
        }
        // '+' and '-' markers
        if (data.bars[i].deltaUp) {
            int cx = bx + barW/2;
            int cy = y0 + (y1 - y0)/4;
            display.drawFastHLine(cx - 2, cy, 5, WHITE);
            display.drawLine(cx, cy - 2, cx, cy + 2, WHITE);
        }
        if (data.bars[i].deltaDown) {
            int cx = bx + barW/2;
            int cy = y1 - (y1 - y0)/4;
            display.drawFastHLine(cx - 2, cy, 5, WHITE);
        }
    }

    // 6. Midnight line (strong horizontal across chart).
    display.drawFastHLine(chartLeft - 2, chartMid, chartW + 4, BLACK);

    // 7. Day labels below chart + current-day dot marker.
    for (int i = 0; i < 7; ++i) {
        int bx = chartLeft + i * (barW + barGap) + barW/2;
        char s[2] = { data.dayLabels[i], 0 };
        int16_t tx, ty; uint16_t tw, th;
        display.getTextBounds(s, 0, 0, &tx, &ty, &tw, &th);
        display.setCursor(bx - (int)tw/2 - tx, chartBottom + 10);
        display.print(s);
        if (i == data.currentDay) {
            display.fillCircle(bx, chartBottom + 20, 3, BLACK);
        }
    }

    // 8. Small right-pointing triangle on left edge, between "12" and "0" labels
    //    (design accent from reference, sits ~25% down the chart).
    int ty = chartTop + (chartMid - chartTop) / 2;
    display.fillTriangle(chartLeft - 10, ty - 3,
                         chartLeft - 10, ty + 3,
                         chartLeft - 5,  ty,     BLACK);
}

#endif // WATCHY_SCREENS_MULTIDAY_H
