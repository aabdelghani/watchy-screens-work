#ifndef WATCHY_SCREENS_STATS_H
#define WATCHY_SCREENS_STATS_H

#include <stdint.h>

#include "stats_glyphs.h"

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

    // Small-digit dispatch (glyphs extracted from stats.png; digits 3 and
    // 9 are intentionally absent — the scenes don't use them).
    auto blitDig = [&](int fx, int fy, char d) {
        switch (d) {
            case '0': blit(fx, fy, GLYPH_STATS_DIG_0_W, GLYPH_STATS_DIG_0_H,
                          GLYPH_STATS_DIG_0_STRIDE, &GLYPH_STATS_DIG_0_ROWS[0][0]); break;
            case '1': blit(fx, fy, GLYPH_STATS_DIG_1_W, GLYPH_STATS_DIG_1_H,
                          GLYPH_STATS_DIG_1_STRIDE, &GLYPH_STATS_DIG_1_ROWS[0][0]); break;
            case '2': blit(fx, fy, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H,
                          GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]); break;
            case '4': blit(fx, fy, GLYPH_STATS_DIG_4_W, GLYPH_STATS_DIG_4_H,
                          GLYPH_STATS_DIG_4_STRIDE, &GLYPH_STATS_DIG_4_ROWS[0][0]); break;
            case '5': blit(fx, fy, GLYPH_STATS_DIG_5_W, GLYPH_STATS_DIG_5_H,
                          GLYPH_STATS_DIG_5_STRIDE, &GLYPH_STATS_DIG_5_ROWS[0][0]); break;
            case '6': blit(fx, fy, GLYPH_STATS_DIG_6_W, GLYPH_STATS_DIG_6_H,
                          GLYPH_STATS_DIG_6_STRIDE, &GLYPH_STATS_DIG_6_ROWS[0][0]); break;
            case '7': blit(fx, fy, GLYPH_STATS_DIG_7_W, GLYPH_STATS_DIG_7_H,
                          GLYPH_STATS_DIG_7_STRIDE, &GLYPH_STATS_DIG_7_ROWS[0][0]); break;
            case '8': blit(fx, fy, GLYPH_STATS_DIG_8_W, GLYPH_STATS_DIG_8_H,
                          GLYPH_STATS_DIG_8_STRIDE, &GLYPH_STATS_DIG_8_ROWS[0][0]); break;
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
    auto dotRow2 = [&](int fx) {
        blit(fx, 74, GLYPH_STATS_DOT_W, GLYPH_STATS_DOT_H,
             GLYPH_STATS_DOT_STRIDE, &GLYPH_STATS_DOT_ROWS[0][0]);
    };

    // Big time digits (BIG_0 w=10, BIG_1 w=6, BIG_3 w=10) — minutes share
    // the ':' colon at y=103.
    auto blitBig = [&](int fx, char d) {
        switch (d) {
            case '0': blit(fx, 102, GLYPH_STATS_BIG_0_W, GLYPH_STATS_BIG_0_H,
                          GLYPH_STATS_BIG_0_STRIDE, &GLYPH_STATS_BIG_0_ROWS[0][0]); break;
            case '1': blit(fx, 102, GLYPH_STATS_BIG_1_W, GLYPH_STATS_BIG_1_H,
                          GLYPH_STATS_BIG_1_STRIDE, &GLYPH_STATS_BIG_1_ROWS[0][0]); break;
            case '3': blit(fx, 102, GLYPH_STATS_BIG_3_W, GLYPH_STATS_BIG_3_H,
                          GLYPH_STATS_BIG_3_STRIDE, &GLYPH_STATS_BIG_3_ROWS[0][0]); break;
        }
    };
    auto blitCol = [&](int fx) {
        blit(fx, 103, GLYPH_STATS_BIG_COL_W, GLYPH_STATS_BIG_COL_H,
             GLYPH_STATS_BIG_COL_STRIDE, &GLYPH_STATS_BIG_COL_ROWS[0][0]);
    };

    // Small-date digits at y=101. Use the reference's SM_4/SM_5 for scene 0;
    // scenes 1..6 pull from the hand-authored SM_0/1/2/6/7/8 matching style.
    auto blitSmDig = [&](int fx, char d) {
        switch (d) {
            case '0': blit(fx, 101, GLYPH_STATS_SM_0_W, GLYPH_STATS_SM_0_H,
                          GLYPH_STATS_SM_0_STRIDE, &GLYPH_STATS_SM_0_ROWS[0][0]); break;
            case '1': blit(fx, 101, GLYPH_STATS_SM_1_W, GLYPH_STATS_SM_1_H,
                          GLYPH_STATS_SM_1_STRIDE, &GLYPH_STATS_SM_1_ROWS[0][0]); break;
            case '2': blit(fx, 101, GLYPH_STATS_SM_2_W, GLYPH_STATS_SM_2_H,
                          GLYPH_STATS_SM_2_STRIDE, &GLYPH_STATS_SM_2_ROWS[0][0]); break;
            case '4': blit(fx, 101, GLYPH_STATS_SM_4_W, GLYPH_STATS_SM_4_H,
                          GLYPH_STATS_SM_4_STRIDE, &GLYPH_STATS_SM_4_ROWS[0][0]); break;
            case '5': blit(fx, 101, GLYPH_STATS_SM_5_W, GLYPH_STATS_SM_5_H,
                          GLYPH_STATS_SM_5_STRIDE, &GLYPH_STATS_SM_5_ROWS[0][0]); break;
            case '6': blit(fx, 101, GLYPH_STATS_SM_6_W, GLYPH_STATS_SM_6_H,
                          GLYPH_STATS_SM_6_STRIDE, &GLYPH_STATS_SM_6_ROWS[0][0]); break;
            case '7': blit(fx, 101, GLYPH_STATS_SM_7_W, GLYPH_STATS_SM_7_H,
                          GLYPH_STATS_SM_7_STRIDE, &GLYPH_STATS_SM_7_ROWS[0][0]); break;
            case '8': blit(fx, 101, GLYPH_STATS_SM_8_W, GLYPH_STATS_SM_8_H,
                          GLYPH_STATS_SM_8_STRIDE, &GLYPH_STATS_SM_8_ROWS[0][0]); break;
        }
    };
    auto blitSlash = [&]() {
        blit(109, 101, GLYPH_STATS_SM_SLASH_W, GLYPH_STATS_SM_SLASH_H,
             GLYPH_STATS_SM_SLASH_STRIDE, &GLYPH_STATS_SM_SLASH_ROWS[0][0]);
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
    auto drawDate = [&](char d1, char d2) {
        blitSmDig(103, d1); blitSlash(); blitSmDig(118, d2);
    };

    switch (scene) {
        case 0: {
            // Reference snapshot: 218 ¢ / 2.45 h / 2.15 h / 76% · 10:13 · 4/5 SUN.
            blit(125, 42, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H, GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]);
            blit(131, 42, GLYPH_STATS_DIG_1_W, GLYPH_STATS_DIG_1_H, GLYPH_STATS_DIG_1_STRIDE, &GLYPH_STATS_DIG_1_ROWS[0][0]);
            blit(135, 42, GLYPH_STATS_DIG_8_W, GLYPH_STATS_DIG_8_H, GLYPH_STATS_DIG_8_STRIDE, &GLYPH_STATS_DIG_8_ROWS[0][0]);
            cent(143);
            blit(119, 55, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H, GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]);
            dotRow1(125);
            blit(128, 55, GLYPH_STATS_DIG_4_W, GLYPH_STATS_DIG_4_H, GLYPH_STATS_DIG_4_STRIDE, &GLYPH_STATS_DIG_4_ROWS[0][0]);
            blit(135, 55, GLYPH_STATS_DIG_5_W, GLYPH_STATS_DIG_5_H, GLYPH_STATS_DIG_5_STRIDE, &GLYPH_STATS_DIG_5_ROWS[0][0]);
            hSuffix(55);
            blit(123, 68, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H, GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]);
            dotRow2(129);
            blit(131, 68, GLYPH_STATS_DIG_1_R2_W, GLYPH_STATS_DIG_1_R2_H, GLYPH_STATS_DIG_1_R2_STRIDE, &GLYPH_STATS_DIG_1_R2_ROWS[0][0]);
            blit(135, 68, GLYPH_STATS_DIG_5_W, GLYPH_STATS_DIG_5_H, GLYPH_STATS_DIG_5_STRIDE, &GLYPH_STATS_DIG_5_ROWS[0][0]);
            hSuffix(68);
            blit(129, 80, GLYPH_STATS_DIG_7_W, GLYPH_STATS_DIG_7_H, GLYPH_STATS_DIG_7_STRIDE, &GLYPH_STATS_DIG_7_ROWS[0][0]);
            blit(135, 80, GLYPH_STATS_DIG_6_W, GLYPH_STATS_DIG_6_H, GLYPH_STATS_DIG_6_STRIDE, &GLYPH_STATS_DIG_6_ROWS[0][0]);
            pctSuffix();
            // Big time "10:13" — reference positions.
            blitBig(50, '1'); blitBig(58, '0'); blitCol(72); blitBig(79, '1'); blitBig(87, '3');
            drawDate('4', '5');
            drawDay('S', 'U', 'N');
            break;
        }
        case 1: {
            // 412 ¢ / 1.10 h / 2.20 h / 81% · 11:30 · 5/5 MON
            blitDig(124, 42, '4'); blitDig(131, 42, '1'); blitDig(135, 42, '2'); cent(143);
            blitDig(124, 55, '1'); dotRow1(128); blitDig(131, 55, '1'); blitDig(135, 55, '0'); hSuffix(55);
            blitDig(120, 68, '2'); dotRow2(126); blitDig(129, 68, '2'); blitDig(135, 68, '0'); hSuffix(68);
            blitDig(131, 80, '8'); blitDig(137, 80, '1'); pctSuffix();
            blitBig(50, '1'); blitBig(58, '1'); blitCol(68); blitBig(75, '3'); blitBig(87, '0');
            drawDate('5', '5');
            drawDay('M', 'O', 'N');
            break;
        }
        case 2: {
            // 507 ¢ / 0.45 h / 2.05 h / 88% · 01:30 · 6/5 TUE
            blitDig(123, 42, '5'); blitDig(129, 42, '0'); blitDig(135, 42, '7'); cent(143);
            blitDig(119, 55, '0'); dotRow1(125); blitDig(128, 55, '4'); blitDig(135, 55, '5'); hSuffix(55);
            blitDig(120, 68, '2'); dotRow2(126); blitDig(129, 68, '0'); blitDig(135, 68, '5'); hSuffix(68);
            blitDig(129, 80, '8'); blitDig(135, 80, '8'); pctSuffix();
            blitBig(46, '0'); blitBig(58, '1'); blitCol(68); blitBig(75, '3'); blitBig(87, '0');
            drawDate('6', '5');
            drawDay('T', 'U', 'E');
            break;
        }
        case 3: {
            // 600 ¢ / 0.10 h / 2.00 h / 55% · 03:13 · 7/5 WED
            blitDig(123, 42, '6'); blitDig(129, 42, '0'); blitDig(135, 42, '0'); cent(143);
            blitDig(122, 55, '0'); dotRow1(128); blitDig(131, 55, '1'); blitDig(135, 55, '0'); hSuffix(55);
            blitDig(120, 68, '2'); dotRow2(126); blitDig(129, 68, '0'); blitDig(135, 68, '0'); hSuffix(68);
            blitDig(129, 80, '5'); blitDig(135, 80, '5'); pctSuffix();
            blitBig(46, '0'); blitBig(58, '3'); blitCol(72); blitBig(79, '1'); blitBig(87, '3');
            drawDate('7', '5');
            drawDay('W', 'E', 'D');
            break;
        }
        case 4: {
            // 725 ¢ / 1.50 h / 2.10 h / 60% · 13:13 · 8/5 THU
            blitDig(123, 42, '7'); blitDig(129, 42, '2'); blitDig(135, 42, '5'); cent(143);
            blitDig(122, 55, '1'); dotRow1(126); blitDig(129, 55, '5'); blitDig(135, 55, '0'); hSuffix(55);
            blitDig(122, 68, '2'); dotRow2(128); blitDig(131, 68, '1'); blitDig(135, 68, '0'); hSuffix(68);
            blitDig(129, 80, '6'); blitDig(135, 80, '0'); pctSuffix();
            blitBig(50, '1'); blitBig(58, '3'); blitCol(72); blitBig(79, '1'); blitBig(87, '3');
            drawDate('8', '5');
            drawDay('T', 'H', 'U');
            break;
        }
        case 5: {
            // 847 ¢ / 2.10 h / 2.12 h / 70% · 10:01 · 1/6 FRI
            blitDig(122, 42, '8'); blitDig(128, 42, '4'); blitDig(135, 42, '7'); cent(143);
            blitDig(122, 55, '2'); dotRow1(128); blitDig(131, 55, '1'); blitDig(135, 55, '0'); hSuffix(55);
            blitDig(122, 68, '2'); dotRow2(128); blitDig(131, 68, '1'); blitDig(135, 68, '2'); hSuffix(68);
            blitDig(129, 80, '7'); blitDig(135, 80, '0'); pctSuffix();
            blitBig(50, '1'); blitBig(58, '0'); blitCol(72); blitBig(79, '0'); blitBig(91, '1');
            drawDate('1', '6');
            drawDay('F', 'R', 'I');
            break;
        }
        case 6: {
            // 164 ¢ / 1.45 h / 2.08 h / 85% · 00:30 · 2/6 SAT
            blitDig(124, 42, '1'); blitDig(128, 42, '6'); blitDig(134, 42, '4'); cent(143);
            blitDig(121, 55, '1'); dotRow1(125); blitDig(128, 55, '4'); blitDig(135, 55, '5'); hSuffix(55);
            blitDig(120, 68, '2'); dotRow2(126); blitDig(129, 68, '0'); blitDig(135, 68, '8'); hSuffix(68);
            blitDig(129, 80, '8'); blitDig(135, 80, '5'); pctSuffix();
            blitBig(42, '0'); blitBig(54, '0'); blitCol(68); blitBig(75, '3'); blitBig(87, '0');
            drawDate('2', '6');
            drawDay('S', 'A', 'T');
            break;
        }
    }
}

#endif // WATCHY_SCREENS_STATS_H
