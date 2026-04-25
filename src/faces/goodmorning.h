#ifndef WATCHY_SCREENS_GOODMORNING_H
#define WATCHY_SCREENS_GOODMORNING_H

#include <stdint.h>

#include "goodmorning_glyphs.h"

// Pixel-exact static render of references/goodmorning.png. The face has a
// rectangular inner frame (not the octagon chamfers used elsewhere) at
// face-rel (23..150, 13..116). All content is bitmap-blitted using glyphs
// hand-extracted from the reference. Once verified, the digit/letter
// glyphs will be folded into the WatchyDigits + a future WatchyHand font.
struct GoodMorningData {
    int sceneIndex;  // currently unused — reference snapshot only
};

template <typename Display>
void drawGoodMorningFace(Display& display, int ox, int oy, const GoodMorningData& data) {
    (void)data;
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
    // Value "7.45"
    GM_BLIT(VAL_7,   125, 39);
    GM_BLIT(VAL_DOT, 130, 45);   // sits on bottom row
    GM_BLIT(VAL_4,   133, 39);
    GM_BLIT(VAL_5,   140, 39);

    // ── Row 2: "COINS" + "+18" at y=52..58 ────────────────────────────
    GM_BLIT(LBL_C, 34, 52);
    GM_BLIT(LBL_O, 41, 52);
    GM_BLIT(LBL_I, 48, 52);
    GM_BLIT(LBL_N, 51, 52);
    GM_BLIT(LBL_S, 58, 52);
    // Value "+18"
    GM_BLIT(VAL_PLUS, 130, 53);   // 5×5 plus, vertically centered in row
    GM_BLIT(VAL_1,    137, 52);
    GM_BLIT(VAL_8,    140, 52);

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
    // Value "+130"
    GM_BLIT(VAL_PLUS, 123, 66);
    GM_BLIT(VAL_1,    130, 65);
    GM_BLIT(VAL_3,    133, 65);
    GM_BLIT(VAL_0,    140, 65);

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
    // Value "+3%" — note 3 and % top y=77 (one row above the letters),
    // and the row uses VAL_3_R4 (4×7 rounded) not VAL_3 (6×7 dithered).
    GM_BLIT(VAL_PLUS,   126, 78);
    GM_BLIT(VAL_3_R4,   133, 77);
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
    GM_BLIT(FOOT_3, 101, 104);
    GM_BLIT(FOOT_D, 109, 104);
    GM_BLIT(FOOT_A, 115, 104);
    GM_BLIT(FOOT_Y, 121, 104);
    GM_BLIT(FOOT_S, 127, 104);

    #undef GM_BLIT
}

inline GoodMorningData referenceGoodMorning() {
    GoodMorningData d{};
    d.sceneIndex = 0;
    return d;
}

#endif // WATCHY_SCREENS_GOODMORNING_H
