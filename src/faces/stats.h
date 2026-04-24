#ifndef WATCHY_SCREENS_STATS_H
#define WATCHY_SCREENS_STATS_H

#include <stdint.h>
#include <stdio.h>

#include "stats_glyphs.h"
#include "../fonts/RainyHearts6pt7b.h"

struct StatsData {
    const char* labelCoins;
    const char* valueCoins;         // number only, e.g. "218"
    const char* labelSleepDebt;
    const char* valueSleepDebt;     // number only, e.g. "2.45"
    const char* labelChronotype;
    const char* valueChronotype;
    const char* labelResilience;
    const char* valueResilience;    // e.g. "76"

    int hour;
    int minute;
    int day;
    int month;
    const char* dow;                // 3-letter day, e.g. "SUN"

    int sceneIndex;                 // 0 = reference snapshot
};

// Labels, day-name letters and unit symbols (¢, h, %) stay as baked
// bitmaps. The value digits, big time, and date are all rendered
// through RainyHearts6pt7b so they're data-driven from StatsData.
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

    // ── Row 0 label: "COINS" at y=42..48 ────────────────────────────
    blit(35, 42, GLYPH_STATS_LBL_C_W,  GLYPH_STATS_LBL_C_H,  GLYPH_STATS_LBL_C_STRIDE,  &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(42, 42, GLYPH_STATS_LBL_O_W,  GLYPH_STATS_LBL_O_H,  GLYPH_STATS_LBL_O_STRIDE,  &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(49, 42, GLYPH_STATS_LBL_I_W,  GLYPH_STATS_LBL_I_H,  GLYPH_STATS_LBL_I_STRIDE,  &GLYPH_STATS_LBL_I_ROWS[0][0]);
    blit(52, 42, GLYPH_STATS_LBL_N_W,  GLYPH_STATS_LBL_N_H,  GLYPH_STATS_LBL_N_STRIDE,  &GLYPH_STATS_LBL_N_ROWS[0][0]);
    blit(59, 42, GLYPH_STATS_LBL_S_W,  GLYPH_STATS_LBL_S_H,  GLYPH_STATS_LBL_S_STRIDE,  &GLYPH_STATS_LBL_S_ROWS[0][0]);

    // ── Row 1 label: "SLEEP DEBT" at y=55..61 ───────────────────────
    blit(35, 55, GLYPH_STATS_LBL_S_W,  GLYPH_STATS_LBL_S_H,  GLYPH_STATS_LBL_S_STRIDE,  &GLYPH_STATS_LBL_S_ROWS[0][0]);
    blit(42, 55, GLYPH_STATS_LBL_L_W,  GLYPH_STATS_LBL_L_H,  GLYPH_STATS_LBL_L_STRIDE,  &GLYPH_STATS_LBL_L_ROWS[0][0]);
    blit(49, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(56, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(63, 55, GLYPH_STATS_LBL_P_W,  GLYPH_STATS_LBL_P_H,  GLYPH_STATS_LBL_P_STRIDE,  &GLYPH_STATS_LBL_P_ROWS[0][0]);
    blit(73, 55, GLYPH_STATS_LBL_D_W,  GLYPH_STATS_LBL_D_H,  GLYPH_STATS_LBL_D_STRIDE,  &GLYPH_STATS_LBL_D_ROWS[0][0]);
    blit(80, 55, GLYPH_STATS_LBL_E_W,  GLYPH_STATS_LBL_E_H,  GLYPH_STATS_LBL_E_STRIDE,  &GLYPH_STATS_LBL_E_ROWS[0][0]);
    blit(87, 55, GLYPH_STATS_LBL_B_W,  GLYPH_STATS_LBL_B_H,  GLYPH_STATS_LBL_B_STRIDE,  &GLYPH_STATS_LBL_B_ROWS[0][0]);
    blit(94, 55, GLYPH_STATS_LBL_T_W,  GLYPH_STATS_LBL_T_H,  GLYPH_STATS_LBL_T_STRIDE,  &GLYPH_STATS_LBL_T_ROWS[0][0]);

    // ── Row 2 label: "CRONOTYPE" at y=68..74 (reference typo) ───────
    blit(35, 68, GLYPH_STATS_LBL_C_W,     GLYPH_STATS_LBL_C_H,     GLYPH_STATS_LBL_C_STRIDE,     &GLYPH_STATS_LBL_C_ROWS[0][0]);
    blit(42, 68, GLYPH_STATS_LBL_R_A_W,   GLYPH_STATS_LBL_R_A_H,   GLYPH_STATS_LBL_R_A_STRIDE,   &GLYPH_STATS_LBL_R_A_ROWS[0][0]);
    blit(49, 68, GLYPH_STATS_LBL_O_W,     GLYPH_STATS_LBL_O_H,     GLYPH_STATS_LBL_O_STRIDE,     &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(56, 68, GLYPH_STATS_LBL_N_R2_W,  GLYPH_STATS_LBL_N_R2_H,  GLYPH_STATS_LBL_N_R2_STRIDE,  &GLYPH_STATS_LBL_N_R2_ROWS[0][0]);
    blit(63, 68, GLYPH_STATS_LBL_O_W,     GLYPH_STATS_LBL_O_H,     GLYPH_STATS_LBL_O_STRIDE,     &GLYPH_STATS_LBL_O_ROWS[0][0]);
    blit(70, 68, GLYPH_STATS_LBL_T2_W,    GLYPH_STATS_LBL_T2_H,    GLYPH_STATS_LBL_T2_STRIDE,    &GLYPH_STATS_LBL_T2_ROWS[0][0]);
    blit(77, 68, GLYPH_STATS_LBL_Y_W,     GLYPH_STATS_LBL_Y_H,     GLYPH_STATS_LBL_Y_STRIDE,     &GLYPH_STATS_LBL_Y_ROWS[0][0]);
    blit(84, 68, GLYPH_STATS_LBL_P_R2_W,  GLYPH_STATS_LBL_P_R2_H,  GLYPH_STATS_LBL_P_R2_STRIDE,  &GLYPH_STATS_LBL_P_R2_ROWS[0][0]);
    blit(91, 68, GLYPH_STATS_LBL_E_W,     GLYPH_STATS_LBL_E_H,     GLYPH_STATS_LBL_E_STRIDE,     &GLYPH_STATS_LBL_E_ROWS[0][0]);

    // ── Row 3 label: "RESILIENCE" at y=81..87 ───────────────────────
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

    // ── Unit symbols (bitmap; stay fixed per row) ───────────────────
    blit(143, 41, GLYPH_STATS_CENT_W,     GLYPH_STATS_CENT_H,     GLYPH_STATS_CENT_STRIDE,     &GLYPH_STATS_CENT_ROWS[0][0]);
    blit(144, 55, GLYPH_STATS_LBL_LO_H_W, GLYPH_STATS_LBL_LO_H_H, GLYPH_STATS_LBL_LO_H_STRIDE, &GLYPH_STATS_LBL_LO_H_ROWS[0][0]);
    blit(144, 68, GLYPH_STATS_LBL_LO_H_W, GLYPH_STATS_LBL_LO_H_H, GLYPH_STATS_LBL_LO_H_STRIDE, &GLYPH_STATS_LBL_LO_H_ROWS[0][0]);
    blit(142, 80, GLYPH_STATS_PCT_W,      GLYPH_STATS_PCT_H,      GLYPH_STATS_PCT_STRIDE,      &GLYPH_STATS_PCT_ROWS[0][0]);

    // ── Number rendering via RainyHearts6pt7b ───────────────────────
    display.setFont(&rainyhearts6pt7b);
    display.setTextColor(BLACK);

    // Right-align `s` so its rendered right edge sits at face-rel x=right_fx,
    // with top of glyph row at face-rel y=top_fy.
    auto printRight = [&](const char* s, int right_fx, int top_fy) {
        if (!s || !*s) return;
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(s, 0, 0, &bx, &by, &bw, &bh);
        int curX = ox + right_fx - bx - (int)bw + 1;
        int curY = oy + top_fy + 6;  // RainyHearts dY=-6
        display.setCursor(curX, curY);
        display.print(s);
    };
    // Left-anchor (cursor X set so glyph top-left sits at face-rel (left_fx, top_fy)).
    auto printAt = [&](const char* s, int left_fx, int top_fy) {
        if (!s || !*s) return;
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(s, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor(ox + left_fx - bx, oy + top_fy + 6);
        display.print(s);
    };

    // Values: right-align to just before their unit bitmap.
    printRight(data.valueCoins,      141, 42);   // ¢ starts at 143
    printRight(data.valueSleepDebt,  142, 55);   // h starts at 144
    printRight(data.valueChronotype, 142, 68);
    printRight(data.valueResilience, 140, 80);   // % starts at 142

    // Time "H:MM" — native MinecraftBold (1× scale), right-anchored at
    // face-rel x=96. Same size as the values; no special hierarchy.
    {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d:%02d", data.hour, data.minute);
        printRight(buf, 96, 104);
    }

    // Date "D/M" at top y=101, left-anchored around x=103.
    {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d/%d", data.day, data.month);
        printAt(buf, 103, 101);
    }

    display.setFont(nullptr);  // restore default font for any subsequent draws

    // ── Day letters (bitmap; stay at x=103/110/117, y=110) ──────────
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
    if (data.dow && data.dow[0] && data.dow[1] && data.dow[2]) {
        blitLet(103, data.dow[0]);
        blitLet(110, data.dow[1]);
        blitLet(117, data.dow[2]);
    }
}

#endif // WATCHY_SCREENS_STATS_H
