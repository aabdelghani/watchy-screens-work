#ifndef WATCHY_SCREENS_STATS_H
#define WATCHY_SCREENS_STATS_H

#include <stdint.h>

#include "stats_glyphs.h"
#include "../fonts/WatchyDigits4x7.h"
#include "../fonts/WatchyDigits5x7.h"
#include "../fonts/WatchyDigits10x15.h"

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

    // Which of the 7 hardcoded value snapshots to render in the right
    // column. 0 = reference (matches references/stats.png byte-for-byte).
    int sceneIndex;
};

// Template renderer: draws the static chrome/labels/time/date/SUN and
// dispatches to one of 7 hardcoded value-column snapshots. Scene 0 is
// the reference (preserves byte-exact match to references/stats.png).
// Scenes 1..6 use a simpler constant-gap layout for the right column.
template <typename Display>
void drawStatsFace(Display& display, int ox, int oy, const StatsData& data) {
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

    // ── Top & bottom rules (2 px thick, x=28..147, y=32/33 and y=96/97).
    display.drawFastHLine(ox + 28, oy + 32, 120, BLACK);
    display.drawFastHLine(ox + 28, oy + 33, 120, BLACK);
    display.drawFastHLine(ox + 28, oy + 96, 120, BLACK);
    display.drawFastHLine(ox + 28, oy + 97, 120, BLACK);

    // ── Row 0: "COINS"  at y=42..48 ─────────────────────────────────
    blit(35, 42, GLYPH_STATS_LBL_C_W,  GLYPH_STATS_LBL_C_H,  GLYPH_STATS_LBL_C_STRIDE,  &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(42, 42, GLYPH_STATS_LBL_O_W,  GLYPH_STATS_LBL_O_H,  GLYPH_STATS_LBL_O_STRIDE,  &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(49, 42, GLYPH_STATS_LBL_I_W,  GLYPH_STATS_LBL_I_H,  GLYPH_STATS_LBL_I_STRIDE,  &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(52, 42, GLYPH_STATS_LBL_N_W,  GLYPH_STATS_LBL_N_H,  GLYPH_STATS_LBL_N_STRIDE,  &GLYPH_STATS_LBL_N_ROWS[0][0]);
    blit(59, 42, GLYPH_STATS_LBL_S_W,  GLYPH_STATS_LBL_S_H,  GLYPH_STATS_LBL_S_STRIDE,  &GLYPH_STATS_LBL_S_ROWS[0][0]);

    // ── Row 1: "SLEEP DEBT"  at y=55..61 ────────────────────────────
    blit(35, 55, GLYPH_STATS_LBL_S_W,  GLYPH_STATS_LBL_S_H,  GLYPH_STATS_LBL_S_STRIDE,  &GLYPH_STATS_LBL_S_ROWS[0][0]);
    blit(42, 55, GLYPH_STATS_LBL_L_W,  GLYPH_STATS_LBL_L_H,  GLYPH_STATS_LBL_L_STRIDE,  &GLYPH_STATS_LBL_L_ROWS[0][0]);
    blit(49, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(56, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(63, 55, GLYPH_STATS_LBL_P_W,  GLYPH_STATS_LBL_P_H,  GLYPH_STATS_LBL_P_STRIDE,  &GLYPH_STATS_LBL_P_ROWS[0][0]);
    blit(73, 55, GLYPH_STATS_LBL_D_W,  GLYPH_STATS_LBL_D_H,  GLYPH_STATS_LBL_D_STRIDE,  &GLYPH_STATS_LBL_D_ROWS[0][0]);
    blit(80, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(87, 55, GLYPH_STATS_LBL_B_W,  GLYPH_STATS_LBL_B_H,  GLYPH_STATS_LBL_B_STRIDE,  &GLYPH_STATS_LBL_B_ROWS[0][0]);
    blit(94, 55, GLYPH_STATS_LBL_T_W,  GLYPH_STATS_LBL_T_H,  GLYPH_STATS_LBL_T_STRIDE,  &GLYPH_STATS_LBL_T_ROWS[0][0]);

    // ── Row 2: "CRONOTYPE"  at y=68..74 ─────────────────────────────
    blit(35, 68, GLYPH_STATS_LBL_C_W,     GLYPH_STATS_LBL_C_H,     GLYPH_STATS_LBL_C_STRIDE,     &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(42, 68, GLYPH_STATS_LBL_R_A_W,   GLYPH_STATS_LBL_R_A_H,   GLYPH_STATS_LBL_R_A_STRIDE,   &GLYPH_STATS_LBL_R_A_ROWS[0][0]);
    blit(49, 68, GLYPH_STATS_LBL_O_W,     GLYPH_STATS_LBL_O_H,     GLYPH_STATS_LBL_O_STRIDE,     &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(56, 68, GLYPH_STATS_LBL_N_R2_W,  GLYPH_STATS_LBL_N_R2_H,  GLYPH_STATS_LBL_N_R2_STRIDE,  &GLYPH_STATS_LBL_N_R2_ROWS[0][0]);
    blit(63, 68, GLYPH_STATS_LBL_O_W,     GLYPH_STATS_LBL_O_H,     GLYPH_STATS_LBL_O_STRIDE,     &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(70, 68, GLYPH_STATS_LBL_T2_W,    GLYPH_STATS_LBL_T2_H,    GLYPH_STATS_LBL_T2_STRIDE,    &GLYPH_STATS_LBL_T2_ROWS[0][0]);
    blit(77, 68, GLYPH_STATS_LBL_Y_W,     GLYPH_STATS_LBL_Y_H,     GLYPH_STATS_LBL_Y_STRIDE,     &GLYPH_STATS_LBL_Y_ROWS[0][0]);
    blit(84, 68, GLYPH_STATS_LBL_P_R2_W,  GLYPH_STATS_LBL_P_R2_H,  GLYPH_STATS_LBL_P_R2_STRIDE,  &GLYPH_STATS_LBL_P_R2_ROWS[0][0]);
    blit(91, 68, GLYPH_STATS_LBL_E_W,     GLYPH_STATS_LBL_E_H,     GLYPH_STATS_LBL_E_STRIDE,     &GLYPH_STATS_LBL_E_ROWS[0][0]);

    // ── Row 3: "RESILIENCE"  at y=81..87 ────────────────────────────
    blit(35, 81, GLYPH_STATS_LBL_R_B_W,  GLYPH_STATS_LBL_R_B_H,  GLYPH_STATS_LBL_R_B_STRIDE,  &GLYPH_STATS_LBL_R_B_ROWS[0][0]);
    blit(42, 81, GLYPH_STATS_LBL_E_W,    GLYPH_STATS_LBL_E_H,    GLYPH_STATS_LBL_E_STRIDE,    &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(49, 81, GLYPH_STATS_LBL_S_W,    GLYPH_STATS_LBL_S_H,    GLYPH_STATS_LBL_S_STRIDE,    &GLYPH_STATS_LBL_S_ROWS[0][0]);
    blit(56, 81, GLYPH_STATS_LBL_I_W,    GLYPH_STATS_LBL_I_H,    GLYPH_STATS_LBL_I_STRIDE,    &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(59, 81, GLYPH_STATS_LBL_L_R3_W, GLYPH_STATS_LBL_L_R3_H, GLYPH_STATS_LBL_L_R3_STRIDE, &GLYPH_STATS_LBL_L_R3_ROWS[0][0]);
    blit(66, 81, GLYPH_STATS_LBL_I_W,    GLYPH_STATS_LBL_I_H,    GLYPH_STATS_LBL_I_STRIDE,    &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(69, 81, GLYPH_STATS_LBL_E_W,    GLYPH_STATS_LBL_E_H,    GLYPH_STATS_LBL_E_STRIDE,    &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(76, 81, GLYPH_STATS_LBL_N_W,    GLYPH_STATS_LBL_N_H,    GLYPH_STATS_LBL_N_STRIDE,    &GLYPH_STATS_LBL_N_ROWS[0][0]);
    blit(83, 81, GLYPH_STATS_LBL_C_W,    GLYPH_STATS_LBL_C_H,    GLYPH_STATS_LBL_C_STRIDE,    &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(90, 81, GLYPH_STATS_LBL_E_W,    GLYPH_STATS_LBL_E_H,    GLYPH_STATS_LBL_E_STRIDE,    &GLYPH_STATS_LBL_E_ROWS[0][0]);

    // ── 7-snapshot rotation: right-column values + big time + date + day
    // Scene 0 preserves the exact reference positions so --dump Stats
    // stays byte-for-byte aligned to references/stats.png. Scenes 1..6
    // swap in alternate values, times (using BIG_{0,1,3} only), dates
    // (using SM_{0,1,2,4,5,6,7,8}), and day names (SUN..SAT).
    int scene = data.sceneIndex;
    if (scene < 0) scene = 0;
    scene %= 7;

    auto cent = [&](int fx) {
        blit(fx, 41, GLYPH_STATS_CENT_W, GLYPH_STATS_CENT_H,
             GLYPH_STATS_CENT_STRIDE, &GLYPH_STATS_CENT_ROWS[0][0]);
    };
    auto hSuffix = [&](int fy) {
        blit(144, fy, GLYPH_STATS_LBL_LO_H_W, GLYPH_STATS_LBL_LO_H_H,
             GLYPH_STATS_LBL_LO_H_STRIDE, &GLYPH_STATS_LBL_LO_H_ROWS[0][0]);
    };
    auto pctSuffix = [&]() {
        blit(142, 80, GLYPH_STATS_PCT_W, GLYPH_STATS_PCT_H,
             GLYPH_STATS_PCT_STRIDE, &GLYPH_STATS_PCT_ROWS[0][0]);
    };
    auto dotRow1 = [&](int fx) {
        blit(fx, 61, GLYPH_STATS_DOT_W, GLYPH_STATS_DOT_H,
             GLYPH_STATS_DOT_STRIDE, &GLYPH_STATS_DOT_ROWS[0][0]);
    };
    // Print a numeric string at face-rel (left_fx, baseline_fy) using `font`.
    auto printNum = [&](const GFXfont* font, int left_fx, int baseline_fy, const char* s) {
        display.setFont(font);
        display.setTextColor(BLACK);
        display.setCursor(ox + left_fx, oy + baseline_fy);
        display.print(s);
        display.setFont(nullptr);
    };
    // 4×7 value row (digits and '.'), baseline = top + 6.
    auto printValue = [&](int left_fx, int top_fy, const char* s) {
        printNum(&WatchyDigits4x7, left_fx, top_fy + 6, s);
    };
    // 10×15 big time, baseline = top + 14.
    auto printBigTime = [&](int left_fx, const char* s) {
        printNum(&WatchyDigits10x15, left_fx, 102 + 14, s);
    };
    // 5×7 date "D/M", baseline = top + 6 (date top is y=101).
    auto printDate = [&](int left_fx, const char* s) {
        printNum(&WatchyDigits5x7, left_fx, 101 + 6, s);
    };

    // Day-name letters at y=110 (3 letters at x=103, 110, 117; all 5×7).
    auto blitLet = [&](int fx, char c) {
        switch (c) {
            case 'S': blit(fx, 110, GLYPH_STATS_SM_S_W, GLYPH_STATS_SM_S_H, GLYPH_STATS_SM_S_STRIDE, &GLYPH_STATS_SM_S_ROWS[0][0]); break;
            case 'U': blit(fx, 110, GLYPH_STATS_SM_U_W, GLYPH_STATS_SM_U_H, GLYPH_STATS_SM_U_STRIDE, &GLYPH_STATS_SM_U_ROWS[0][0]); break;
            case 'N': blit(fx, 110, GLYPH_STATS_SM_N_W, GLYPH_STATS_SM_N_H, GLYPH_STATS_SM_N_STRIDE, &GLYPH_STATS_SM_N_ROWS[0][0]); break;
            case 'M': blit(fx, 110, GLYPH_STATS_SM_M_W, GLYPH_STATS_SM_M_H, GLYPH_STATS_SM_M_STRIDE, &GLYPH_STATS_SM_M_ROWS[0][0]); break;
            case 'O': blit(fx, 110, GLYPH_STATS_SM_O_W, GLYPH_STATS_SM_O_H, GLYPH_STATS_SM_O_STRIDE, &GLYPH_STATS_SM_O_ROWS[0][0]); break;
            case 'T': blit(fx, 110, GLYPH_STATS_SM_T_W, GLYPH_STATS_SM_T_H, GLYPH_STATS_SM_T_STRIDE, &GLYPH_STATS_SM_T_ROWS[0][0]); break;
            case 'E': blit(fx, 110, GLYPH_STATS_SM_E_W, GLYPH_STATS_SM_E_H, GLYPH_STATS_SM_E_STRIDE, &GLYPH_STATS_SM_E_ROWS[0][0]); break;
            case 'W': blit(fx, 110, GLYPH_STATS_SM_W_W, GLYPH_STATS_SM_W_H, GLYPH_STATS_SM_W_STRIDE, &GLYPH_STATS_SM_W_ROWS[0][0]); break;
            case 'D': blit(fx, 110, GLYPH_STATS_SM_D_W, GLYPH_STATS_SM_D_H, GLYPH_STATS_SM_D_STRIDE, &GLYPH_STATS_SM_D_ROWS[0][0]); break;
            case 'H': blit(fx, 110, GLYPH_STATS_SM_H_W, GLYPH_STATS_SM_H_H, GLYPH_STATS_SM_H_STRIDE, &GLYPH_STATS_SM_H_ROWS[0][0]); break;
            case 'F': blit(fx, 110, GLYPH_STATS_SM_F_W, GLYPH_STATS_SM_F_H, GLYPH_STATS_SM_F_STRIDE, &GLYPH_STATS_SM_F_ROWS[0][0]); break;
            case 'R': blit(fx, 110, GLYPH_STATS_SM_R_W, GLYPH_STATS_SM_R_H, GLYPH_STATS_SM_R_STRIDE, &GLYPH_STATS_SM_R_ROWS[0][0]); break;
            case 'I': blit(fx, 110, GLYPH_STATS_SM_I_W, GLYPH_STATS_SM_I_H, GLYPH_STATS_SM_I_STRIDE, &GLYPH_STATS_SM_I_ROWS[0][0]); break;
            case 'A': blit(fx, 110, GLYPH_STATS_SM_A_W, GLYPH_STATS_SM_A_H, GLYPH_STATS_SM_A_STRIDE, &GLYPH_STATS_SM_A_ROWS[0][0]); break;
        }
    };
    auto drawDay = [&](char a, char b, char c) {
        blitLet(103, a); blitLet(110, b); blitLet(117, c);
    };

    switch (scene) {
        case 0: {
            // Reference snapshot: 218 ¢ / 2.45 h / 2.15 h / 76% · 10:13 · 4/5 SUN.
            printValue(125, 42, "218"); cent(143);
            printValue(119, 55, "2.45"); hSuffix(55);
            printValue(123, 68, "2.15"); hSuffix(68);
            printValue(129, 80, "76");  pctSuffix();
            printBigTime(50, "10:13");
            printDate(103, "4/5");
            drawDay('S', 'U', 'N');
            break;
        }
        case 1: {
            printValue(124, 42, "412"); cent(143);
            printValue(124, 55, "1.10"); hSuffix(55);
            printValue(120, 68, "2.20"); hSuffix(68);
            printValue(131, 80, "81");  pctSuffix();
            printBigTime(50, "11:30");
            printDate(103, "5/5");
            drawDay('M', 'O', 'N');
            break;
        }
        case 2: {
            printValue(123, 42, "507"); cent(143);
            printValue(119, 55, "0.45"); hSuffix(55);
            printValue(120, 68, "2.05"); hSuffix(68);
            printValue(129, 80, "88");  pctSuffix();
            printBigTime(46, "01:30");
            printDate(103, "6/5");
            drawDay('T', 'U', 'E');
            break;
        }
        case 3: {
            printValue(123, 42, "600"); cent(143);
            printValue(122, 55, "0.10"); hSuffix(55);
            printValue(120, 68, "2.00"); hSuffix(68);
            printValue(129, 80, "55");  pctSuffix();
            printBigTime(46, "03:13");
            printDate(103, "7/5");
            drawDay('W', 'E', 'D');
            break;
        }
        case 4: {
            printValue(123, 42, "725"); cent(143);
            printValue(122, 55, "1.50"); hSuffix(55);
            printValue(122, 68, "2.10"); hSuffix(68);
            printValue(129, 80, "60");  pctSuffix();
            printBigTime(50, "13:13");
            printDate(103, "8/5");
            drawDay('T', 'H', 'U');
            break;
        }
        case 5: {
            printValue(122, 42, "847"); cent(143);
            printValue(122, 55, "2.10"); hSuffix(55);
            printValue(122, 68, "2.12"); hSuffix(68);
            printValue(129, 80, "70");  pctSuffix();
            printBigTime(50, "10:01");
            printDate(103, "1/6");
            drawDay('F', 'R', 'I');
            break;
        }
        case 6: {
            printValue(124, 42, "164"); cent(143);
            printValue(121, 55, "1.45"); hSuffix(55);
            printValue(120, 68, "2.08"); hSuffix(68);
            printValue(129, 80, "85");  pctSuffix();
            printBigTime(42, "00:30");
            printDate(103, "2/6");
            drawDay('S', 'A', 'T');
            break;
        }
    }
}

#endif // WATCHY_SCREENS_STATS_H
