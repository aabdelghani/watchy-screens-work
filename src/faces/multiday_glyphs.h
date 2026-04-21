#ifndef WATCHY_SCREENS_MULTIDAY_GLYPHS_H
#define WATCHY_SCREENS_MULTIDAY_GLYPHS_H

// AUTO-GENERATED from references/multyday.png. Do not edit by hand.
// Bits packed MSB-first per row, leftmost column = bit 7.

#include <stdint.h>

// NIGHTS: tight bbox (x=71..102, y=17..23)
#define GLYPH_NIGHTS_W 32
#define GLYPH_NIGHTS_H 7
#define GLYPH_NIGHTS_STRIDE 4
static const uint8_t GLYPH_NIGHTS_ROWS[7][4] = {
    { 0x85, 0x39, 0x17, 0xDF },
    { 0xC5, 0x45, 0x11, 0x10 },
    { 0xA5, 0x41, 0x11, 0x10 },
    { 0x95, 0x41, 0xF1, 0x1F },
    { 0x8D, 0x4D, 0x11, 0x01 },
    { 0x85, 0x45, 0x11, 0x01 },
    { 0x85, 0x39, 0x11, 0x1F },
};

// DIG_0: tight bbox (x=7..11, y=72..78)
#define GLYPH_DIG_0_W 5
#define GLYPH_DIG_0_H 7
#define GLYPH_DIG_0_STRIDE 1
static const uint8_t GLYPH_DIG_0_ROWS[7] = { 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70 };

// DIG_1: tight bbox (x=5..6, y=33..39)
#define GLYPH_DIG_1_W 2
#define GLYPH_DIG_1_H 7
#define GLYPH_DIG_1_STRIDE 1
static const uint8_t GLYPH_DIG_1_ROWS[7] = { 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 };

// DIG_2: tight bbox (x=8..11, y=33..39)
#define GLYPH_DIG_2_W 4
#define GLYPH_DIG_2_H 7
#define GLYPH_DIG_2_STRIDE 1
static const uint8_t GLYPH_DIG_2_ROWS[7] = { 0xF0, 0x10, 0x10, 0xF0, 0x80, 0x80, 0xF0 };

// DIG_3: tight bbox (x=164..167, y=60..66)
#define GLYPH_DIG_3_W 4
#define GLYPH_DIG_3_H 7
#define GLYPH_DIG_3_STRIDE 1
static const uint8_t GLYPH_DIG_3_ROWS[7] = { 0x60, 0x90, 0x10, 0x20, 0x10, 0x90, 0x60 };

// DIG_6: tight bbox (x=164..167, y=48..54)
#define GLYPH_DIG_6_W 4
#define GLYPH_DIG_6_H 7
#define GLYPH_DIG_6_STRIDE 1
static const uint8_t GLYPH_DIG_6_ROWS[7] = { 0x60, 0x90, 0x80, 0xE0, 0x90, 0x90, 0x60 };

// DIG_8: tight bbox (x=166..169, y=88..94)
#define GLYPH_DIG_8_W 4
#define GLYPH_DIG_8_H 7
#define GLYPH_DIG_8_STRIDE 1
static const uint8_t GLYPH_DIG_8_ROWS[7] = { 0x60, 0x90, 0x90, 0x60, 0x90, 0x90, 0x60 };

// DAY_M: tight bbox (x=22..26, y=105..109)
#define GLYPH_DAY_M_W 5
#define GLYPH_DAY_M_H 5
#define GLYPH_DAY_M_STRIDE 1
static const uint8_t GLYPH_DAY_M_ROWS[5] = { 0x88, 0x88, 0xD8, 0xA8, 0x88 };

// DAY_T: tight bbox (x=44..46, y=105..109)
#define GLYPH_DAY_T_W 3
#define GLYPH_DAY_T_H 5
#define GLYPH_DAY_T_STRIDE 1
static const uint8_t GLYPH_DAY_T_ROWS[5] = { 0xE0, 0x40, 0x40, 0x40, 0x40 };

// DAY_W: tight bbox (x=64..68, y=105..109)
#define GLYPH_DAY_W_W 5
#define GLYPH_DAY_W_H 5
#define GLYPH_DAY_W_STRIDE 1
static const uint8_t GLYPH_DAY_W_ROWS[5] = { 0x88, 0x88, 0xA8, 0xA8, 0xD8 };

// DAY_F: tight bbox (x=108..110, y=105..109)
#define GLYPH_DAY_F_W 3
#define GLYPH_DAY_F_H 5
#define GLYPH_DAY_F_STRIDE 1
static const uint8_t GLYPH_DAY_F_ROWS[5] = { 0xE0, 0x80, 0xC0, 0x80, 0x80 };

// DAY_S: tight bbox (x=127..130, y=105..109)
#define GLYPH_DAY_S_W 4
#define GLYPH_DAY_S_H 5
#define GLYPH_DAY_S_STRIDE 1
static const uint8_t GLYPH_DAY_S_ROWS[5] = { 0x70, 0x80, 0x60, 0x10, 0xE0 };

#endif // WATCHY_SCREENS_MULTIDAY_GLYPHS_H
