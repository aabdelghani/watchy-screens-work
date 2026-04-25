#ifndef WATCHY_SCREENS_GOODMORNING_H
#define WATCHY_SCREENS_GOODMORNING_H

#include <stdint.h>

#include "goodmorning_glyphs.h"
#include "../fonts/WatchyDigits4x7.h"
#include "../fonts/WatchyDigits5x7.h"

// Pixel-exact render of references/goodmorning.png with data-driven value
// rotation. Title / labels / row-4 / subtitle / footer stay static. Rows
// 1-3 cycle through 5 snapshots using only the digits we have in the
// dithered row-1-3 value font: {0,1,3,4,5,7,8}.
struct GoodMorningData {
    int sceneIndex;
    const char* todaySleep;       // e.g. "7.45"   — uses VAL_0..VAL_8 + VAL_DOT
    const char* coinsDelta;       // e.g. "+18"    — leading '+' uses VAL_PLUS
    const char* sleepDebtDelta;   // e.g. "+130"
    int resiliencePercent;        // 1..9 — rendered via WatchyDigits4x7
    int conversionDays;           // 1..9 — rendered via WatchyDigits5x7
};

template <typename Display>
void drawGoodMorningFace(Display& display, int ox, int oy, const GoodMorningData& data) {
    const uint16_t BLACK = 0x0000;

    auto blit = [&](int fx, int fy, int w, int h, int stride, const uint8_t* rows) {
        for (int r = 0; r < h; ++r) {
            const uint8_t* row = rows + r * stride;
            for (int c = 0; c < w; ++c) {
                if (row[c >> 3] & (0x80 >> (c & 7)))
                    display.drawPixel(ox + fx + c, oy + fy + r, BLACK);
            }
        }
    };

    // Dispatcher for the row 1-3 value-column digit/punct font. The
    // reference uses 1-col gaps between consecutive digits and 2-col gaps
    // around '.' / '+', which our advance-rule encodes as xAdv = w+1 for
    // digits and xAdv = w+2 for punct.
    auto blitValChar = [&](int fx, int fy, char c) -> int {
        #define V(g, gap) blit(fx, fy, GLYPH_GM_##g##_W, GLYPH_GM_##g##_H, \
                               GLYPH_GM_##g##_STRIDE, &GLYPH_GM_##g##_ROWS[0][0]); \
                          return fx + GLYPH_GM_##g##_W + (gap)
        switch (c) {
            case '0': V(VAL_0, 1);
            case '1': V(VAL_1, 1);
            case '3': V(VAL_3, 1);
            case '4': V(VAL_4, 1);
            case '5': V(VAL_5, 1);
            case '7': V(VAL_7, 1);
            case '8': V(VAL_8, 1);
            case '+': V(VAL_PLUS, 2);
            case '.': // sits on baseline (1 tall); fy passed is glyph top row.
                blit(fx, fy + 6, GLYPH_GM_VAL_DOT_W, GLYPH_GM_VAL_DOT_H,
                     GLYPH_GM_VAL_DOT_STRIDE, &GLYPH_GM_VAL_DOT_ROWS[0][0]);
                return fx + GLYPH_GM_VAL_DOT_W + 2;
        }
        #undef V
        return fx;
    };
    // Walk a string and blit each char with 2-col kerning.
    auto drawValStr = [&](int fx, int fy, const char* s) {
        while (*s) {
            fx = blitValChar(fx, fy, *s);
            ++s;
        }
    };

    // ── Inner rectangle frame (2 px thick) at face-rel (23..152, 13..116).
    display.drawFastHLine(ox + 23, oy + 13,  130, BLACK);
    display.drawFastHLine(ox + 23, oy + 14,  130, BLACK);
    display.drawFastHLine(ox + 23, oy + 115, 130, BLACK);
    display.drawFastHLine(ox + 23, oy + 116, 130, BLACK);
    display.drawFastVLine(ox + 23,  oy + 13, 104, BLACK);
    display.drawFastVLine(ox + 24,  oy + 13, 104, BLACK);
    display.drawFastVLine(ox + 151, oy + 13, 104, BLACK);
    display.drawFastVLine(ox + 152, oy + 13, 104, BLACK);

    // ── Helper macros to keep the long blit list readable ─────────────
    #define GM_BLIT(name, x, y) \
        blit((x), (y), GLYPH_GM_##name##_W, GLYPH_GM_##name##_H, \
             GLYPH_GM_##name##_STRIDE, &GLYPH_GM_##name##_ROWS[0][0])

    // ── Title "GOOD MORNING" at y=18..28 ──────────────────────────────
    GM_BLIT(TITLE_G,  34, 18);  // G
    GM_BLIT(TITLE_O,  44, 18);  // O
    GM_BLIT(TITLE_O,  54, 18);  // O
    GM_BLIT(TITLE_D,  64, 18);  // D
    // 8-col gap (space)
    GM_BLIT(TITLE_M,  81, 18);  // M
    GM_BLIT(TITLE_O,  92, 18);  // O
    GM_BLIT(TITLE_R, 102, 18);  // R
    GM_BLIT(TITLE_N, 112, 18);  // N
    GM_BLIT(TITLE_I, 122, 18);  // I
    GM_BLIT(TITLE_N, 125, 18);  // N
    GM_BLIT(TITLE_G, 135, 18);  // G

    // ── Row 1: "TODAY SLEEP" + "7.45" at y=39..45 ─────────────────────
    GM_BLIT(LBL_T, 34, 39);
    GM_BLIT(LBL_O, 41, 39);
    GM_BLIT(LBL_D, 48, 39);
    GM_BLIT(LBL_A, 55, 39);
    GM_BLIT(LBL_Y, 62, 39);
    GM_BLIT(LBL_S, 73, 39);
    GM_BLIT(LBL_L, 80, 39);
    GM_BLIT(LBL_E, 87, 39);
    GM_BLIT(LBL_E, 94, 39);
    GM_BLIT(LBL_P, 101, 39);
    // Value (data-driven via dispatcher)
    drawValStr(125, 39, data.todaySleep);

    // ── Row 2: "COINS" + "+18" at y=52..58 ────────────────────────────
    GM_BLIT(LBL_C, 34, 52);
    GM_BLIT(LBL_O, 41, 52);
    GM_BLIT(LBL_I, 48, 52);
    GM_BLIT(LBL_N, 51, 52);
    GM_BLIT(LBL_S, 58, 52);
    // Value: '+' sits 1 row down from digits (centered in row), so split.
    drawValStr(130, 53, "+");
    drawValStr(137, 52, data.coinsDelta + 1);  // skip leading '+' since we drew it

    // ── Row 3: "SLEEP DEBT" + "+130" at y=65..71 ──────────────────────
    GM_BLIT(LBL_S, 34, 65);
    GM_BLIT(LBL_L, 41, 65);
    GM_BLIT(LBL_E, 48, 65);
    GM_BLIT(LBL_E, 55, 65);
    GM_BLIT(LBL_P, 62, 65);
    GM_BLIT(LBL_D, 72, 65);
    GM_BLIT(LBL_E, 79, 65);
    GM_BLIT(LBL_B, 86, 65);
    GM_BLIT(LBL_T_R3, 93, 65);
    // Value: '+' sits 1 row down from digits.
    drawValStr(123, 66, "+");
    drawValStr(130, 65, data.sleepDebtDelta + 1);

    // ── Row 4: "RESILIENCE" + "+3%" at y=77..84 ───────────────────────
    GM_BLIT(LBL_R, 34, 78);
    GM_BLIT(LBL_E, 41, 78);
    GM_BLIT(LBL_S, 48, 78);
    GM_BLIT(LBL_I, 55, 78);
    GM_BLIT(LBL_L, 58, 78);
    GM_BLIT(LBL_I, 65, 78);
    GM_BLIT(LBL_E, 68, 78);
    GM_BLIT(LBL_N, 75, 78);
    GM_BLIT(LBL_C, 82, 78);
    GM_BLIT(LBL_E, 89, 78);
    // Value "+N%" — N comes from data (1..9) rendered via WatchyDigits4x7.
    // The PCT bitmap stays (¢-style).
    GM_BLIT(VAL_PLUS,   126, 78);
    {
        char rb[2] = { (char)('0' + (data.resiliencePercent % 10)), 0 };
        display.setFont(&WatchyDigits4x7);
        display.setTextColor(BLACK);
        display.setCursor(ox + 133, oy + 77 + 6);   // baseline = top + (h-1)
        display.print(rb);
        display.setFont(nullptr);
    }
    GM_BLIT(VAL_PCT,    139, 77);

    // ── Subtitle "SLEEP DEBT" (small 5×7) at y=93..99 ─────────────────
    GM_BLIT(SUB_S,  63, 93);
    GM_BLIT(SUB_L,  69, 93);
    GM_BLIT(SUB_E1, 74, 93);
    GM_BLIT(SUB_E2, 80, 93);
    GM_BLIT(SUB_P,  86, 93);
    GM_BLIT(SUB_D,  95, 93);
    GM_BLIT(SUB_E3, 101, 93);
    GM_BLIT(SUB_B,  107, 93);
    GM_BLIT(SUB_T,  113, 93);

    // ── Footer "CONVERT IN 3 DAYS" at y=104..110 ──────────────────────
    GM_BLIT(FOOT_C,  45, 104);
    GM_BLIT(FOOT_O,  51, 104);
    GM_BLIT(FOOT_N,  57, 104);
    GM_BLIT(FOOT_V,  63, 104);
    GM_BLIT(FOOT_E,  69, 104);
    GM_BLIT(FOOT_R,  75, 104);
    GM_BLIT(FOOT_T,  81, 104);
    GM_BLIT(FOOT_I,  90, 104);
    GM_BLIT(FOOT_N2, 92, 104);
    {
        char db[2] = { (char)('0' + (data.conversionDays % 10)), 0 };
        display.setFont(&WatchyDigits5x7);
        display.setTextColor(BLACK);
        display.setCursor(ox + 101, oy + 104 + 6);
        display.print(db);
        display.setFont(nullptr);
    }
    GM_BLIT(FOOT_D, 109, 104);
    GM_BLIT(FOOT_A, 115, 104);
    GM_BLIT(FOOT_Y, 121, 104);
    GM_BLIT(FOOT_S, 127, 104);

    #undef GM_BLIT
}

// 5 snapshots using only the digits we have (0,1,3,4,5,7,8 plus '+' '.').
struct GoodMorningSnapshot {
    const char* todaySleep;
    const char* coinsDelta;
    const char* sleepDebtDelta;
    int  resiliencePercent;
    int  conversionDays;
};
static const GoodMorningSnapshot kGoodMorningScenes[5] = {
    // COINS bounded to [1, 99], integer only. RESILIENCE % and DAYS in
    // [1, 9]. Sleep stays decimal hours.
    { "7.45", "+18", "+130", 3, 3 },   // reference
    { "8.00", "+85", "+145", 5, 5 },
    { "5.10", "+5",  "+85",  1, 2 },
    { "4.30", "+71", "+180", 8, 7 },
    { "3.45", "+33", "+178", 6, 4 },
};

inline GoodMorningData goodMorningScene(int idx) {
    if (idx < 0) idx = 0;
    idx %= 5;
    const GoodMorningSnapshot& s = kGoodMorningScenes[idx];
    GoodMorningData d{};
    d.sceneIndex          = idx;
    d.todaySleep          = s.todaySleep;
    d.coinsDelta          = s.coinsDelta;
    d.sleepDebtDelta      = s.sleepDebtDelta;
    d.resiliencePercent   = s.resiliencePercent;
    d.conversionDays      = s.conversionDays;
    return d;
}

inline GoodMorningData referenceGoodMorning() { return goodMorningScene(0); }

#endif // WATCHY_SCREENS_GOODMORNING_H
