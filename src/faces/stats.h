#ifndef WATCHY_SCREENS_STATS_H
#define WATCHY_SCREENS_STATS_H

#include <stdint.h>
#include <stdio.h>

struct StatsData {
    const char* labelCoins;
    const char* valueCoins;
    const char* labelSleepDebt;
    const char* valueSleepDebt;
    const char* labelChronotype;
    const char* valueChronotype;
    const char* labelResilience;
    const char* valueResilience;

    int hour;
    int minute;
    int day;
    int month;
    const char* dow;
};

// Template so the same source compiles against GxEPD2_GFX on hardware
// and against a simulator Display class.  Uses only Adafruit_GFX primitives.
template <typename Display>
void drawStatsFace(Display& display, int ox, int oy, const StatsData& data) {
    const int W = 176;
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    display.setTextColor(BLACK);
    display.setTextSize(1);

    // ── Top divider line (2 px thick) ───────────────────────────────
    const int marginX = 28;
    const int topLineY = 32;
    display.drawFastHLine(ox + marginX, oy + topLineY,     W - 2 * marginX, BLACK);
    display.drawFastHLine(ox + marginX, oy + topLineY + 1, W - 2 * marginX, BLACK);

    // ── Four text rows (label left, value right) ────────────────────
    const int labelX = ox + 35;
    const int valueRightX = ox + W - 35;   // right edge of value column
    const int rowY[4] = { 42, 55, 68, 81 };

    auto drawRow = [&](int idx, const char* label, const char* value) {
        display.setTextSize(1);

        // Label – left aligned
        display.setCursor(labelX, oy + rowY[idx]);
        display.print(label);

        // Value – right aligned
        int16_t bbX, bbY;
        uint16_t bbW, bbH;
        display.getTextBounds(value, 0, 0, &bbX, &bbY, &bbW, &bbH);
        display.setCursor(valueRightX - (int)bbW - bbX, oy + rowY[idx]);
        display.print(value);
    };

    drawRow(0, data.labelCoins,      data.valueCoins);
    drawRow(1, data.labelSleepDebt,  data.valueSleepDebt);
    drawRow(2, data.labelChronotype, data.valueChronotype);
    drawRow(3, data.labelResilience, data.valueResilience);

    // ── Bottom divider line (2 px thick) ────────────────────────────
    const int botLineY = 96;
    display.drawFastHLine(ox + marginX, oy + botLineY,     W - 2 * marginX, BLACK);
    display.drawFastHLine(ox + marginX, oy + botLineY + 1, W - 2 * marginX, BLACK);

    // ── Clock footer ────────────────────────────────────────────────
    char timeStr[8];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", data.hour, data.minute);

    char dateStr[16];
    snprintf(dateStr, sizeof(dateStr), "%d/%d %s", data.day, data.month, data.dow);

    // Time – larger, left of centre
    display.setTextSize(2);
    int16_t tX, tY;
    uint16_t tW, tH;
    display.getTextBounds(timeStr, 0, 0, &tX, &tY, &tW, &tH);
    int timeX = ox + (W / 2) - tW - 8;
    int timeY = oy + 106;
    display.setCursor(timeX, timeY);
    display.print(timeStr);

    // Date – smaller, right of centre, vertically centred on the time block
    display.setTextSize(1);
    int16_t dX, dY;
    uint16_t dW, dH;
    display.getTextBounds(dateStr, 0, 0, &dX, &dY, &dW, &dH);
    int dateX = ox + (W / 2) + 4;
    int dateY = timeY + (tH - dH) / 2 - dY;   // align visually with size-2 time
    display.setCursor(dateX, dateY);
    display.print(dateStr);

    // Reset text size for any subsequent drawing
    display.setTextSize(1);
}

#endif // WATCHY_SCREENS_STATS_H
