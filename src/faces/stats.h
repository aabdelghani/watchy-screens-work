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
};

// Pixel-exact static render of references/stats.png.
// Every element is blitted at its measured face-relative position; this
// matches the reference byte-for-byte under an alpha>128 content mask.
// `data` is currently unused — the animation phase will add a char→glyph
// fallback path driven by the StatsData fields.
template <typename Display>
void drawStatsFace(Display& display, int ox, int oy, const StatsData& data) {
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
    // Row 0 value "218 ¢"
    blit(125, 42, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H, GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]);
    blit(131, 42, GLYPH_STATS_DIG_1_W, GLYPH_STATS_DIG_1_H, GLYPH_STATS_DIG_1_STRIDE, &GLYPH_STATS_DIG_1_ROWS[0][0]);
    blit(135, 42, GLYPH_STATS_DIG_8_W, GLYPH_STATS_DIG_8_H, GLYPH_STATS_DIG_8_STRIDE, &GLYPH_STATS_DIG_8_ROWS[0][0]);
    blit(143, 41, GLYPH_STATS_CENT_W,  GLYPH_STATS_CENT_H,  GLYPH_STATS_CENT_STRIDE,  &GLYPH_STATS_CENT_ROWS[0][0]);

    // ── Row 1: "SLEEP DEBT"  at y=55..61 ────────────────────────────
    blit(35, 55, GLYPH_STATS_LBL_S_W,  GLYPH_STATS_LBL_S_H,  GLYPH_STATS_LBL_S_STRIDE,  &GLYPH_STATS_LBL_S_ROWS[0][0]);
    blit(42, 55, GLYPH_STATS_LBL_L_W,  GLYPH_STATS_LBL_L_H,  GLYPH_STATS_LBL_L_STRIDE,  &GLYPH_STATS_LBL_L_ROWS[0][0]);
    blit(49, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(56, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(63, 55, GLYPH_STATS_LBL_P_W,  GLYPH_STATS_LBL_P_H,  GLYPH_STATS_LBL_P_STRIDE,  &GLYPH_STATS_LBL_P_ROWS[0][0]);
    // 4-col space between P and D (x=69..72)
    blit(73, 55, GLYPH_STATS_LBL_D_W,  GLYPH_STATS_LBL_D_H,  GLYPH_STATS_LBL_D_STRIDE,  &GLYPH_STATS_LBL_D_ROWS[0][0]);
    blit(80, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(87, 55, GLYPH_STATS_LBL_B_W,  GLYPH_STATS_LBL_B_H,  GLYPH_STATS_LBL_B_STRIDE,  &GLYPH_STATS_LBL_B_ROWS[0][0]);
    blit(94, 55, GLYPH_STATS_LBL_T_W,  GLYPH_STATS_LBL_T_H,  GLYPH_STATS_LBL_T_STRIDE,  &GLYPH_STATS_LBL_T_ROWS[0][0]);
    // Row 1 value "2.45 h"
    blit(119, 55, GLYPH_STATS_DIG_2_W, GLYPH_STATS_DIG_2_H, GLYPH_STATS_DIG_2_STRIDE, &GLYPH_STATS_DIG_2_ROWS[0][0]);
    blit(125, 61, GLYPH_STATS_DOT_W,   GLYPH_STATS_DOT_H,   GLYPH_STATS_DOT_STRIDE,   &GLYPH_STATS_DOT_ROWS[0][0]);
    blit(128, 55, GLYPH_STATS_DIG_4_W, GLYPH_STATS_DIG_4_H, GLYPH_STATS_DIG_4_STRIDE, &GLYPH_STATS_DIG_4_ROWS[0][0]);
    blit(135, 55, GLYPH_STATS_DIG_5_W, GLYPH_STATS_DIG_5_H, GLYPH_STATS_DIG_5_STRIDE, &GLYPH_STATS_DIG_5_ROWS[0][0]);
    blit(144, 55, GLYPH_STATS_LBL_LO_H_W, GLYPH_STATS_LBL_LO_H_H, GLYPH_STATS_LBL_LO_H_STRIDE, &GLYPH_STATS_LBL_LO_H_ROWS[0][0]);

    // ── Row 2: "CRONOTYPE"  at y=68..74 ─────────────────────────────
    // Reference typo: 9 letters, not "CHRONOTYPE".
    blit(35, 68, GLYPH_STATS_LBL_C_W,   GLYPH_STATS_LBL_C_H,   GLYPH_STATS_LBL_C_STRIDE,   &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(42, 68, GLYPH_STATS_LBL_R_A_W, GLYPH_STATS_LBL_R_A_H, GLYPH_STATS_LBL_R_A_STRIDE, &GLYPH_STATS_LBL_R_A_ROWS[0][0]);
    blit(49, 68, GLYPH_STATS_LBL_O_W,   GLYPH_STATS_LBL_O_H,   GLYPH_STATS_LBL_O_STRIDE,   &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(56, 68, GLYPH_STATS_LBL_N_R2_W, GLYPH_STATS_LBL_N_R2_H, GLYPH_STATS_LBL_N_R2_STRIDE, &GLYPH_STATS_LBL_N_R2_ROWS[0][0]);
    blit(63, 68, GLYPH_STATS_LBL_O_W,   GLYPH_STATS_LBL_O_H,   GLYPH_STATS_LBL_O_STRIDE,   &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(70, 68, GLYPH_STATS_LBL_T2_W,  GLYPH_STATS_LBL_T2_H,  GLYPH_STATS_LBL_T2_STRIDE,  &GLYPH_STATS_LBL_T2_ROWS[0][0]);
    blit(77, 68, GLYPH_STATS_LBL_Y_W,   GLYPH_STATS_LBL_Y_H,   GLYPH_STATS_LBL_Y_STRIDE,   &GLYPH_STATS_LBL_Y_ROWS[0][0]);
    blit(84, 68, GLYPH_STATS_LBL_P_R2_W, GLYPH_STATS_LBL_P_R2_H, GLYPH_STATS_LBL_P_R2_STRIDE, &GLYPH_STATS_LBL_P_R2_ROWS[0][0]);
    blit(91, 68, GLYPH_STATS_LBL_E_W,   GLYPH_STATS_LBL_E_H,   GLYPH_STATS_LBL_E_STRIDE,   &GLYPH_STATS_LBL_E_ROWS[0][0]);
    // Row 2 value "2.15 h"
    blit(123, 68, GLYPH_STATS_DIG_2_W,     GLYPH_STATS_DIG_2_H,     GLYPH_STATS_DIG_2_STRIDE,     &GLYPH_STATS_DIG_2_ROWS[0][0]);
    blit(129, 74, GLYPH_STATS_DOT_W,       GLYPH_STATS_DOT_H,       GLYPH_STATS_DOT_STRIDE,       &GLYPH_STATS_DOT_ROWS[0][0]);
    blit(131, 68, GLYPH_STATS_DIG_1_R2_W,  GLYPH_STATS_DIG_1_R2_H,  GLYPH_STATS_DIG_1_R2_STRIDE,  &GLYPH_STATS_DIG_1_R2_ROWS[0][0]);
    blit(135, 68, GLYPH_STATS_DIG_5_W,     GLYPH_STATS_DIG_5_H,     GLYPH_STATS_DIG_5_STRIDE,     &GLYPH_STATS_DIG_5_ROWS[0][0]);
    blit(144, 68, GLYPH_STATS_LBL_LO_H_W,  GLYPH_STATS_LBL_LO_H_H,  GLYPH_STATS_LBL_LO_H_STRIDE,  &GLYPH_STATS_LBL_LO_H_ROWS[0][0]);

    // ── Row 3: "RESILIENCE"  at y=81..87 ────────────────────────────
    blit(35, 81, GLYPH_STATS_LBL_R_B_W, GLYPH_STATS_LBL_R_B_H, GLYPH_STATS_LBL_R_B_STRIDE, &GLYPH_STATS_LBL_R_B_ROWS[0][0]);
    blit(42, 81, GLYPH_STATS_LBL_E_W,   GLYPH_STATS_LBL_E_H,   GLYPH_STATS_LBL_E_STRIDE,   &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(49, 81, GLYPH_STATS_LBL_S_W,   GLYPH_STATS_LBL_S_H,   GLYPH_STATS_LBL_S_STRIDE,   &GLYPH_STATS_LBL_S_ROWS[0][0]);
    blit(56, 81, GLYPH_STATS_LBL_I_W,   GLYPH_STATS_LBL_I_H,   GLYPH_STATS_LBL_I_STRIDE,   &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(59, 81, GLYPH_STATS_LBL_L_R3_W, GLYPH_STATS_LBL_L_R3_H, GLYPH_STATS_LBL_L_R3_STRIDE, &GLYPH_STATS_LBL_L_R3_ROWS[0][0]);
    blit(66, 81, GLYPH_STATS_LBL_I_W,   GLYPH_STATS_LBL_I_H,   GLYPH_STATS_LBL_I_STRIDE,   &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(69, 81, GLYPH_STATS_LBL_E_W,   GLYPH_STATS_LBL_E_H,   GLYPH_STATS_LBL_E_STRIDE,   &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(76, 81, GLYPH_STATS_LBL_N_W,   GLYPH_STATS_LBL_N_H,   GLYPH_STATS_LBL_N_STRIDE,   &GLYPH_STATS_LBL_N_ROWS[0][0]);
    blit(83, 81, GLYPH_STATS_LBL_C_W,   GLYPH_STATS_LBL_C_H,   GLYPH_STATS_LBL_C_STRIDE,   &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(90, 81, GLYPH_STATS_LBL_E_W,   GLYPH_STATS_LBL_E_H,   GLYPH_STATS_LBL_E_STRIDE,   &GLYPH_STATS_LBL_E_ROWS[0][0]);
    // Row 3 value "76%"
    blit(129, 80, GLYPH_STATS_DIG_7_W, GLYPH_STATS_DIG_7_H, GLYPH_STATS_DIG_7_STRIDE, &GLYPH_STATS_DIG_7_ROWS[0][0]);
    blit(135, 80, GLYPH_STATS_DIG_6_W, GLYPH_STATS_DIG_6_H, GLYPH_STATS_DIG_6_STRIDE, &GLYPH_STATS_DIG_6_ROWS[0][0]);
    blit(142, 80, GLYPH_STATS_PCT_W,   GLYPH_STATS_PCT_H,   GLYPH_STATS_PCT_STRIDE,   &GLYPH_STATS_PCT_ROWS[0][0]);

    // ── Big time "10:13" at y=102..116 ──────────────────────────────
    blit(50, 102, GLYPH_STATS_BIG_1_W,   GLYPH_STATS_BIG_1_H,   GLYPH_STATS_BIG_1_STRIDE,   &GLYPH_STATS_BIG_1_ROWS[0][0]);
    blit(58, 102, GLYPH_STATS_BIG_0_W,   GLYPH_STATS_BIG_0_H,   GLYPH_STATS_BIG_0_STRIDE,   &GLYPH_STATS_BIG_0_ROWS[0][0]);
    blit(72, 103, GLYPH_STATS_BIG_COL_W, GLYPH_STATS_BIG_COL_H, GLYPH_STATS_BIG_COL_STRIDE, &GLYPH_STATS_BIG_COL_ROWS[0][0]);
    blit(79, 102, GLYPH_STATS_BIG_1_W,   GLYPH_STATS_BIG_1_H,   GLYPH_STATS_BIG_1_STRIDE,   &GLYPH_STATS_BIG_1_ROWS[0][0]);
    blit(87, 102, GLYPH_STATS_BIG_3_W,   GLYPH_STATS_BIG_3_H,   GLYPH_STATS_BIG_3_STRIDE,   &GLYPH_STATS_BIG_3_ROWS[0][0]);

    // ── Right-side "4/5" then "SUN" ─────────────────────────────────
    blit(103, 101, GLYPH_STATS_SM_4_W,     GLYPH_STATS_SM_4_H,     GLYPH_STATS_SM_4_STRIDE,     &GLYPH_STATS_SM_4_ROWS[0][0]);
    blit(109, 101, GLYPH_STATS_SM_SLASH_W, GLYPH_STATS_SM_SLASH_H, GLYPH_STATS_SM_SLASH_STRIDE, &GLYPH_STATS_SM_SLASH_ROWS[0][0]);
    blit(118, 101, GLYPH_STATS_SM_5_W,     GLYPH_STATS_SM_5_H,     GLYPH_STATS_SM_5_STRIDE,     &GLYPH_STATS_SM_5_ROWS[0][0]);
    blit(103, 110, GLYPH_STATS_SM_S_W,     GLYPH_STATS_SM_S_H,     GLYPH_STATS_SM_S_STRIDE,     &GLYPH_STATS_SM_S_ROWS[0][0]);
    blit(110, 110, GLYPH_STATS_SM_U_W,     GLYPH_STATS_SM_U_H,     GLYPH_STATS_SM_U_STRIDE,     &GLYPH_STATS_SM_U_ROWS[0][0]);
    blit(117, 110, GLYPH_STATS_SM_N_W,     GLYPH_STATS_SM_N_H,     GLYPH_STATS_SM_N_STRIDE,     &GLYPH_STATS_SM_N_ROWS[0][0]);
}

#endif // WATCHY_SCREENS_STATS_H
