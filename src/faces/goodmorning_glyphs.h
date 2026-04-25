#ifndef WATCHY_SCREENS_GOODMORNING_GLYPHS_H
#define WATCHY_SCREENS_GOODMORNING_GLYPHS_H

#include <stdint.h>

// ===== row y=18..28 =====
// TITLE_G at face-rel (34, 18), 9x11
static const int GLYPH_GM_TITLE_G_W = 9;
static const int GLYPH_GM_TITLE_G_H = 11;
static const int GLYPH_GM_TITLE_G_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_G_ROWS[11][2] = {
    { 0x7f, 0x80 },
    { 0xff, 0x80 },
    { 0xe0, 0x00 },
    { 0xe0, 0x00 },
    { 0xe0, 0x00 },
    { 0xe0, 0x00 },
    { 0xe1, 0x80 },
    { 0xe1, 0x80 },
    { 0xe1, 0x80 },
    { 0xff, 0x80 },
    { 0x7f, 0x80 },
};

// TITLE_O at face-rel (44, 18), 9x11
static const int GLYPH_GM_TITLE_O_W = 9;
static const int GLYPH_GM_TITLE_O_H = 11;
static const int GLYPH_GM_TITLE_O_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_O_ROWS[11][2] = {
    { 0x7f, 0x00 },
    { 0xff, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xff, 0x80 },
    { 0x7f, 0x00 },
};

// TITLE_D at face-rel (64, 18), 9x11
static const int GLYPH_GM_TITLE_D_W = 9;
static const int GLYPH_GM_TITLE_D_H = 11;
static const int GLYPH_GM_TITLE_D_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_D_ROWS[11][2] = {
    { 0xfc, 0x00 },
    { 0xfe, 0x00 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xfe, 0x00 },
    { 0xfc, 0x00 },
};

// TITLE_M at face-rel (81, 18), 10x11
static const int GLYPH_GM_TITLE_M_W = 10;
static const int GLYPH_GM_TITLE_M_H = 11;
static const int GLYPH_GM_TITLE_M_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_M_ROWS[11][2] = {
    { 0xc0, 0xc0 },
    { 0xc0, 0xc0 },
    { 0xc0, 0xc0 },
    { 0xc0, 0xc0 },
    { 0xe1, 0xc0 },
    { 0xf3, 0xc0 },
    { 0xff, 0xc0 },
    { 0xde, 0xc0 },
    { 0xcc, 0xc0 },
    { 0xcc, 0xc0 },
    { 0xc0, 0xc0 },
};

// TITLE_R at face-rel (102, 18), 9x11
static const int GLYPH_GM_TITLE_R_W = 9;
static const int GLYPH_GM_TITLE_R_H = 11;
static const int GLYPH_GM_TITLE_R_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_R_ROWS[11][2] = {
    { 0xff, 0x00 },
    { 0xff, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xff, 0x80 },
    { 0xfe, 0x00 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
};

// TITLE_N at face-rel (112, 18), 9x11
static const int GLYPH_GM_TITLE_N_W = 9;
static const int GLYPH_GM_TITLE_N_H = 11;
static const int GLYPH_GM_TITLE_N_STRIDE = 2;
static const uint8_t GLYPH_GM_TITLE_N_ROWS[11][2] = {
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xc1, 0x80 },
    { 0xe1, 0x80 },
    { 0xf1, 0x80 },
    { 0xd9, 0x80 },
    { 0xcd, 0x80 },
    { 0xc7, 0x80 },
    { 0xc3, 0x80 },
    { 0xc1, 0x80 },
};

// TITLE_I at face-rel (122, 18), 2x11
static const int GLYPH_GM_TITLE_I_W = 2;
static const int GLYPH_GM_TITLE_I_H = 11;
static const int GLYPH_GM_TITLE_I_STRIDE = 1;
static const uint8_t GLYPH_GM_TITLE_I_ROWS[11][1] = {
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
};

// ===== row y=39..45 =====
// LBL_T at face-rel (34, 39), 6x7
static const int GLYPH_GM_LBL_T_W = 6;
static const int GLYPH_GM_LBL_T_H = 7;
static const int GLYPH_GM_LBL_T_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_T_ROWS[7][1] = {
    { 0xfc },
    { 0xfc },
    { 0x30 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
};

// LBL_O at face-rel (41, 39), 6x7
static const int GLYPH_GM_LBL_O_W = 6;
static const int GLYPH_GM_LBL_O_H = 7;
static const int GLYPH_GM_LBL_O_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_O_ROWS[7][1] = {
    { 0x78 },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0x78 },
};

// LBL_D at face-rel (48, 39), 6x7
static const int GLYPH_GM_LBL_D_W = 6;
static const int GLYPH_GM_LBL_D_H = 7;
static const int GLYPH_GM_LBL_D_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_D_ROWS[7][1] = {
    { 0xf8 },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xf8 },
};

// LBL_A at face-rel (55, 39), 6x7
static const int GLYPH_GM_LBL_A_W = 6;
static const int GLYPH_GM_LBL_A_H = 7;
static const int GLYPH_GM_LBL_A_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_A_ROWS[7][1] = {
    { 0x78 },
    { 0xc4 },
    { 0xc4 },
    { 0xc4 },
    { 0xfc },
    { 0xc4 },
    { 0xc4 },
};

// LBL_Y at face-rel (62, 39), 6x7
static const int GLYPH_GM_LBL_Y_W = 6;
static const int GLYPH_GM_LBL_Y_H = 7;
static const int GLYPH_GM_LBL_Y_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_Y_ROWS[7][1] = {
    { 0x84 },
    { 0xcc },
    { 0xfc },
    { 0x78 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
};

// LBL_S at face-rel (73, 39), 6x7
static const int GLYPH_GM_LBL_S_W = 6;
static const int GLYPH_GM_LBL_S_H = 7;
static const int GLYPH_GM_LBL_S_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_S_ROWS[7][1] = {
    { 0x7c },
    { 0xc4 },
    { 0xc0 },
    { 0xfc },
    { 0x0c },
    { 0x8c },
    { 0xf8 },
};

// LBL_L at face-rel (80, 39), 6x7
static const int GLYPH_GM_LBL_L_W = 6;
static const int GLYPH_GM_LBL_L_H = 7;
static const int GLYPH_GM_LBL_L_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_L_ROWS[7][1] = {
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc4 },
    { 0xfc },
};

// LBL_E at face-rel (87, 39), 6x7
static const int GLYPH_GM_LBL_E_W = 6;
static const int GLYPH_GM_LBL_E_H = 7;
static const int GLYPH_GM_LBL_E_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_E_ROWS[7][1] = {
    { 0xfc },
    { 0xc0 },
    { 0xc0 },
    { 0xf0 },
    { 0xc0 },
    { 0xc0 },
    { 0xfc },
};

// LBL_P at face-rel (101, 39), 6x7
static const int GLYPH_GM_LBL_P_W = 6;
static const int GLYPH_GM_LBL_P_H = 7;
static const int GLYPH_GM_LBL_P_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_P_ROWS[7][1] = {
    { 0xf8 },
    { 0xcc },
    { 0xcc },
    { 0xcc },
    { 0xf8 },
    { 0xc0 },
    { 0xc0 },
};

// ===== row y=39..45 =====
// VAL_7 at face-rel (125, 39), 4x7
static const int GLYPH_GM_VAL_7_W = 4;
static const int GLYPH_GM_VAL_7_H = 7;
static const int GLYPH_GM_VAL_7_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_7_ROWS[7][1] = {
    { 0xf0 },
    { 0x10 },
    { 0x20 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
};

// VAL_DOT at face-rel (130, 45), 1x1
static const int GLYPH_GM_VAL_DOT_W = 1;
static const int GLYPH_GM_VAL_DOT_H = 1;
static const int GLYPH_GM_VAL_DOT_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_DOT_ROWS[1][1] = {
    { 0x80 },
};

// VAL_4 at face-rel (133, 39), 5x7
static const int GLYPH_GM_VAL_4_W = 5;
static const int GLYPH_GM_VAL_4_H = 7;
static const int GLYPH_GM_VAL_4_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_4_ROWS[7][1] = {
    { 0x10 },
    { 0x30 },
    { 0x50 },
    { 0x90 },
    { 0x90 },
    { 0xf8 },
    { 0x10 },
};

// VAL_5 at face-rel (140, 39), 4x7
static const int GLYPH_GM_VAL_5_W = 4;
static const int GLYPH_GM_VAL_5_H = 7;
static const int GLYPH_GM_VAL_5_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_5_ROWS[7][1] = {
    { 0xf0 },
    { 0x80 },
    { 0x80 },
    { 0xf0 },
    { 0x10 },
    { 0x10 },
    { 0xf0 },
};

// ===== row y=52..58 =====
// LBL_C at face-rel (34, 52), 6x7
static const int GLYPH_GM_LBL_C_W = 6;
static const int GLYPH_GM_LBL_C_H = 7;
static const int GLYPH_GM_LBL_C_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_C_ROWS[7][1] = {
    { 0x78 },
    { 0xc4 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc4 },
    { 0x78 },
};

// LBL_I at face-rel (48, 52), 2x7
static const int GLYPH_GM_LBL_I_W = 2;
static const int GLYPH_GM_LBL_I_H = 7;
static const int GLYPH_GM_LBL_I_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_I_ROWS[7][1] = {
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
    { 0xc0 },
};

// LBL_N at face-rel (51, 52), 6x7
static const int GLYPH_GM_LBL_N_W = 6;
static const int GLYPH_GM_LBL_N_H = 7;
static const int GLYPH_GM_LBL_N_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_N_ROWS[7][1] = {
    { 0xc4 },
    { 0xc4 },
    { 0xc4 },
    { 0xe4 },
    { 0xd4 },
    { 0xcc },
    { 0xc4 },
};

// VAL_PLUS at face-rel (130, 53), 5x5
static const int GLYPH_GM_VAL_PLUS_W = 5;
static const int GLYPH_GM_VAL_PLUS_H = 5;
static const int GLYPH_GM_VAL_PLUS_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_PLUS_ROWS[5][1] = {
    { 0x20 },
    { 0x20 },
    { 0xf8 },
    { 0x20 },
    { 0x20 },
};

// VAL_1 at face-rel (137, 52), 2x7
static const int GLYPH_GM_VAL_1_W = 2;
static const int GLYPH_GM_VAL_1_H = 7;
static const int GLYPH_GM_VAL_1_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_1_ROWS[7][1] = {
    { 0xc0 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
    { 0x40 },
};

// VAL_8 at face-rel (140, 52), 4x7
static const int GLYPH_GM_VAL_8_W = 4;
static const int GLYPH_GM_VAL_8_H = 7;
static const int GLYPH_GM_VAL_8_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_8_ROWS[7][1] = {
    { 0x60 },
    { 0x90 },
    { 0x90 },
    { 0x60 },
    { 0x90 },
    { 0x90 },
    { 0x60 },
};

// ===== row y=65..71 =====
// LBL_B at face-rel (86, 65), 6x7
static const int GLYPH_GM_LBL_B_W = 6;
static const int GLYPH_GM_LBL_B_H = 7;
static const int GLYPH_GM_LBL_B_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_B_ROWS[7][1] = {
    { 0xf8 },
    { 0xc4 },
    { 0xc4 },
    { 0xf8 },
    { 0xc4 },
    { 0xc4 },
    { 0xfc },
};

// LBL_T_R3 at face-rel (93, 65), 6x7
static const int GLYPH_GM_LBL_T_R3_W = 6;
static const int GLYPH_GM_LBL_T_R3_H = 7;
static const int GLYPH_GM_LBL_T_R3_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_T_R3_ROWS[7][1] = {
    { 0xfc },
    { 0x30 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
    { 0x30 },
};

// VAL_3 at face-rel (133, 65), 6x7
static const int GLYPH_GM_VAL_3_W = 6;
static const int GLYPH_GM_VAL_3_H = 7;
static const int GLYPH_GM_VAL_3_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_3_ROWS[7][1] = {
    { 0x18 },
    { 0x24 },
    { 0x04 },
    { 0x08 },
    { 0x04 },
    { 0x24 },
    { 0x98 },
};

// VAL_0 at face-rel (140, 65), 4x7
static const int GLYPH_GM_VAL_0_W = 4;
static const int GLYPH_GM_VAL_0_H = 7;
static const int GLYPH_GM_VAL_0_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_0_ROWS[7][1] = {
    { 0x60 },
    { 0x90 },
    { 0x90 },
    { 0x90 },
    { 0x90 },
    { 0x90 },
    { 0x60 },
};

// ===== row y=77..84 =====
// LBL_R at face-rel (34, 78), 6x7
static const int GLYPH_GM_LBL_R_W = 6;
static const int GLYPH_GM_LBL_R_H = 7;
static const int GLYPH_GM_LBL_R_STRIDE = 1;
static const uint8_t GLYPH_GM_LBL_R_ROWS[7][1] = {
    { 0xf8 },
    { 0xc4 },
    { 0xc4 },
    { 0xc4 },
    { 0xf8 },
    { 0xc4 },
    { 0xc4 },
};

// VAL_3_R4 at face-rel (133, 77), 4x7
static const int GLYPH_GM_VAL_3_R4_W = 4;
static const int GLYPH_GM_VAL_3_R4_H = 7;
static const int GLYPH_GM_VAL_3_R4_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_3_R4_ROWS[7][1] = {
    { 0x60 },
    { 0x90 },
    { 0x10 },
    { 0x20 },
    { 0x10 },
    { 0x90 },
    { 0x60 },
};

// VAL_PCT at face-rel (139, 77), 5x7
static const int GLYPH_GM_VAL_PCT_W = 5;
static const int GLYPH_GM_VAL_PCT_H = 7;
static const int GLYPH_GM_VAL_PCT_STRIDE = 1;
static const uint8_t GLYPH_GM_VAL_PCT_ROWS[7][1] = {
    { 0xc8 },
    { 0xd8 },
    { 0x10 },
    { 0x20 },
    { 0x40 },
    { 0xd8 },
    { 0x98 },
};

// ===== row y=93..99 =====
// SUB_S at face-rel (63, 93), 5x7
static const int GLYPH_GM_SUB_S_W = 5;
static const int GLYPH_GM_SUB_S_H = 7;
static const int GLYPH_GM_SUB_S_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_S_ROWS[7][1] = {
    { 0x78 },
    { 0x80 },
    { 0x80 },
    { 0x70 },
    { 0x08 },
    { 0x08 },
    { 0xf0 },
};

// SUB_L at face-rel (69, 93), 4x7
static const int GLYPH_GM_SUB_L_W = 4;
static const int GLYPH_GM_SUB_L_H = 7;
static const int GLYPH_GM_SUB_L_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_L_ROWS[7][1] = {
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0xf0 },
};

// SUB_E1 at face-rel (74, 93), 5x7
static const int GLYPH_GM_SUB_E1_W = 5;
static const int GLYPH_GM_SUB_E1_H = 7;
static const int GLYPH_GM_SUB_E1_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_E1_ROWS[7][1] = {
    { 0xf8 },
    { 0x80 },
    { 0x80 },
    { 0xe0 },
    { 0x80 },
    { 0x80 },
    { 0xf8 },
};

// SUB_E2 at face-rel (80, 93), 5x7
static const int GLYPH_GM_SUB_E2_W = 5;
static const int GLYPH_GM_SUB_E2_H = 7;
static const int GLYPH_GM_SUB_E2_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_E2_ROWS[7][1] = {
    { 0xf8 },
    { 0x80 },
    { 0x80 },
    { 0xe0 },
    { 0x80 },
    { 0x80 },
    { 0xf8 },
};

// SUB_P at face-rel (86, 93), 5x7
static const int GLYPH_GM_SUB_P_W = 5;
static const int GLYPH_GM_SUB_P_H = 7;
static const int GLYPH_GM_SUB_P_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_P_ROWS[7][1] = {
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0xf0 },
    { 0x80 },
    { 0x80 },
};

// SUB_D at face-rel (95, 93), 5x7
static const int GLYPH_GM_SUB_D_W = 5;
static const int GLYPH_GM_SUB_D_H = 7;
static const int GLYPH_GM_SUB_D_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_D_ROWS[7][1] = {
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0xf0 },
};

// SUB_E3 at face-rel (101, 93), 5x7
static const int GLYPH_GM_SUB_E3_W = 5;
static const int GLYPH_GM_SUB_E3_H = 7;
static const int GLYPH_GM_SUB_E3_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_E3_ROWS[7][1] = {
    { 0xf8 },
    { 0x80 },
    { 0x80 },
    { 0xe0 },
    { 0x80 },
    { 0x80 },
    { 0xf8 },
};

// SUB_B at face-rel (107, 93), 5x7
static const int GLYPH_GM_SUB_B_W = 5;
static const int GLYPH_GM_SUB_B_H = 7;
static const int GLYPH_GM_SUB_B_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_B_ROWS[7][1] = {
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0xf0 },
    { 0x88 },
    { 0xf8 },
};

// SUB_T at face-rel (113, 93), 5x7
static const int GLYPH_GM_SUB_T_W = 5;
static const int GLYPH_GM_SUB_T_H = 7;
static const int GLYPH_GM_SUB_T_STRIDE = 1;
static const uint8_t GLYPH_GM_SUB_T_ROWS[7][1] = {
    { 0xf8 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
};

// ===== row y=104..110 =====
// FOOT_C at face-rel (45, 104), 5x7
static const int GLYPH_GM_FOOT_C_W = 5;
static const int GLYPH_GM_FOOT_C_H = 7;
static const int GLYPH_GM_FOOT_C_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_C_ROWS[7][1] = {
    { 0x78 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x78 },
};

// FOOT_O at face-rel (51, 104), 5x7
static const int GLYPH_GM_FOOT_O_W = 5;
static const int GLYPH_GM_FOOT_O_H = 7;
static const int GLYPH_GM_FOOT_O_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_O_ROWS[7][1] = {
    { 0x70 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x70 },
};

// FOOT_N at face-rel (57, 104), 5x7
static const int GLYPH_GM_FOOT_N_W = 5;
static const int GLYPH_GM_FOOT_N_H = 7;
static const int GLYPH_GM_FOOT_N_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_N_ROWS[7][1] = {
    { 0x88 },
    { 0x88 },
    { 0xc8 },
    { 0xa8 },
    { 0x98 },
    { 0x88 },
    { 0x88 },
};

// FOOT_V at face-rel (63, 104), 5x7
static const int GLYPH_GM_FOOT_V_W = 5;
static const int GLYPH_GM_FOOT_V_H = 7;
static const int GLYPH_GM_FOOT_V_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_V_ROWS[7][1] = {
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x50 },
    { 0x20 },
};

// FOOT_E at face-rel (69, 104), 5x7
static const int GLYPH_GM_FOOT_E_W = 5;
static const int GLYPH_GM_FOOT_E_H = 7;
static const int GLYPH_GM_FOOT_E_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_E_ROWS[7][1] = {
    { 0xf8 },
    { 0x80 },
    { 0x80 },
    { 0xe0 },
    { 0x80 },
    { 0x80 },
    { 0xf8 },
};

// FOOT_R at face-rel (75, 104), 5x7
static const int GLYPH_GM_FOOT_R_W = 5;
static const int GLYPH_GM_FOOT_R_H = 7;
static const int GLYPH_GM_FOOT_R_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_R_ROWS[7][1] = {
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
};

// FOOT_T at face-rel (81, 104), 5x7
static const int GLYPH_GM_FOOT_T_W = 5;
static const int GLYPH_GM_FOOT_T_H = 7;
static const int GLYPH_GM_FOOT_T_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_T_ROWS[7][1] = {
    { 0xf8 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
};

// FOOT_I at face-rel (90, 104), 1x7
static const int GLYPH_GM_FOOT_I_W = 1;
static const int GLYPH_GM_FOOT_I_H = 7;
static const int GLYPH_GM_FOOT_I_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_I_ROWS[7][1] = {
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
    { 0x80 },
};

// FOOT_N2 at face-rel (92, 104), 5x7
static const int GLYPH_GM_FOOT_N2_W = 5;
static const int GLYPH_GM_FOOT_N2_H = 7;
static const int GLYPH_GM_FOOT_N2_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_N2_ROWS[7][1] = {
    { 0x88 },
    { 0x88 },
    { 0xc8 },
    { 0xa8 },
    { 0x98 },
    { 0x88 },
    { 0x88 },
};

// FOOT_3 at face-rel (101, 104), 5x7
static const int GLYPH_GM_FOOT_3_W = 5;
static const int GLYPH_GM_FOOT_3_H = 7;
static const int GLYPH_GM_FOOT_3_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_3_ROWS[7][1] = {
    { 0x70 },
    { 0x88 },
    { 0x08 },
    { 0x30 },
    { 0x08 },
    { 0x88 },
    { 0x70 },
};

// FOOT_D at face-rel (109, 104), 5x7
static const int GLYPH_GM_FOOT_D_W = 5;
static const int GLYPH_GM_FOOT_D_H = 7;
static const int GLYPH_GM_FOOT_D_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_D_ROWS[7][1] = {
    { 0xf0 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0xf0 },
};

// FOOT_A at face-rel (115, 104), 5x7
static const int GLYPH_GM_FOOT_A_W = 5;
static const int GLYPH_GM_FOOT_A_H = 7;
static const int GLYPH_GM_FOOT_A_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_A_ROWS[7][1] = {
    { 0x70 },
    { 0x88 },
    { 0x88 },
    { 0x88 },
    { 0xf8 },
    { 0x88 },
    { 0x88 },
};

// FOOT_Y at face-rel (121, 104), 5x7
static const int GLYPH_GM_FOOT_Y_W = 5;
static const int GLYPH_GM_FOOT_Y_H = 7;
static const int GLYPH_GM_FOOT_Y_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_Y_ROWS[7][1] = {
    { 0x88 },
    { 0xd8 },
    { 0x70 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
    { 0x20 },
};

// FOOT_S at face-rel (127, 104), 5x7
static const int GLYPH_GM_FOOT_S_W = 5;
static const int GLYPH_GM_FOOT_S_H = 7;
static const int GLYPH_GM_FOOT_S_STRIDE = 1;
static const uint8_t GLYPH_GM_FOOT_S_ROWS[7][1] = {
    { 0x70 },
    { 0x88 },
    { 0x80 },
    { 0x70 },
    { 0x08 },
    { 0x88 },
    { 0x70 },
};

#endif // WATCHY_SCREENS_GOODMORNING_GLYPHS_H
