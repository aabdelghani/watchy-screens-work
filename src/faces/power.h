#ifndef WATCHY_SCREENS_POWER_H
#define WATCHY_SCREENS_POWER_H

#include <stdint.h>
#include <gfxfont.h>

#include "frame.h"        // kFaceLeftInset / kFaceRightInset
#include "power_static.h"
#include "power_glyphs.h"
#include "../fonts/WatchyDigits10x15.h"
#include "../fonts/WatchyDigits5x7.h"

// POWER face. The 176×136 reference bitmap is the base layer (clipped
// to the octagon). On top of it we clear and re-draw the dynamic
// regions: BIG center number, time, date, weekday.
struct PowerData {
    int sceneIndex;   // BIG center value, 0..99
    int hour;         // 0..23
    int minute;       // 0..59
    int day;          // 1..31
    int month;        // 1..12 (currently unused — only "/day" is shown)
    int dowIndex;     // 0=MON .. 6=SUN
};

namespace watchy_power {

// Draw a packed-bitmap glyph (1 byte per row, ≤8 cols, MSB-first).
template <typename Display>
inline void drawSm(Display& d, const uint8_t* rows, int x, int y, int w, int h, uint16_t color) {
    for (int r = 0; r < h; ++r) {
        uint8_t b = rows[r];
        for (int c = 0; c < w; ++c) {
            if (b & (0x80 >> c)) d.drawPixel(x + c, y + r, color);
        }
    }
}

// Draw one GFXfont glyph at the given top-left (no baseline math).
// ESP32 unified memory — PROGMEM data is read directly.
template <typename Display>
inline int drawGfxChar(Display& d, const GFXfont& font, int xLeft, int yTop, char c, uint16_t color) {
    uint8_t u = (uint8_t)c;
    if (u < font.first || u > font.last) return 0;
    const GFXglyph& g = font.glyph[u - font.first];
    const uint8_t* bm = font.bitmap + g.bitmapOffset;
    int total = (int)g.width * (int)g.height;
    for (int i = 0; i < total; ++i) {
        if (bm[i >> 3] & (0x80 >> (i & 7))) {
            d.drawPixel(xLeft + (i % g.width), yTop + (i / g.width), color);
        }
    }
    return g.xAdvance;
}

template <typename Display>
inline void drawGfxStr(Display& d, const GFXfont& font, int xLeft, int yTop, const char* s, uint16_t color) {
    int x = xLeft;
    for (; *s; ++s) x += drawGfxChar(d, font, x, yTop, *s, color);
}

inline const uint8_t* smLetter(char c) {
    switch (c) {
        case 'M': return &GLYPH_POW_SM_M_ROWS[0][0];
        case 'O': return &GLYPH_POW_SM_O_ROWS[0][0];
        case 'N': return &GLYPH_POW_SM_N_ROWS[0][0];
        case 'T': return &GLYPH_POW_SM_T_ROWS[0][0];
        case 'U': return &GLYPH_POW_SM_U_ROWS[0][0];
        case 'E': return &GLYPH_POW_SM_E_ROWS[0][0];
        case 'W': return &GLYPH_POW_SM_W_ROWS[0][0];
        case 'D': return &GLYPH_POW_SM_D_ROWS[0][0];
        case 'H': return &GLYPH_POW_SM_H_ROWS[0][0];
        case 'F': return &GLYPH_POW_SM_F_ROWS[0][0];
        case 'R': return &GLYPH_POW_SM_R_ROWS[0][0];
        case 'I': return &GLYPH_POW_SM_I_ROWS[0][0];
        case 'S': return &GLYPH_POW_SM_S_ROWS[0][0];
        case 'A': return &GLYPH_POW_SM_A_ROWS[0][0];
        default:  return nullptr;
    }
}

// 3-letter weekday strings, indexed 0=MON .. 6=SUN.
inline const char* dowName(int idx) {
    static const char* table[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
    if (idx < 0 || idx > 6) return "MON";
    return table[idx];
}

}  // namespace watchy_power

template <typename Display>
void drawPowerFace(Display& display, int ox, int oy, const PowerData& data) {
    using namespace watchy_power;
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    // ── Slot positions (face-relative, scanned from power.png) ──
    constexpr int kBigY      = 39;
    constexpr int kBigLeftX  = 54;
    constexpr int kBigRightX = 93;
    constexpr int kBigW      = 29;
    constexpr int kBigH      = 47;

    constexpr int kTimeX = 50, kTimeY = 116;
    constexpr int kTimeW = 47, kTimeH = 15;

    constexpr int kDateX = 103, kDateY = 115;
    constexpr int kDateW = 22,  kDateH = 7;

    constexpr int kDowX = 103, kDowY = 124;
    constexpr int kDowW = 18,  kDowH = 7;

    // 1. Base: blit static reference, clipped to octagon.
    for (int r = 0; r < GLYPH_POW_FACE_H; ++r) {
        const uint8_t* row = &GLYPH_POW_FACE_ROWS[r][0];
        const int xLo = kFaceLeftInset[r];
        const int xHi = GLYPH_POW_FACE_W - 1 - kFaceRightInset[r];
        for (int c = xLo; c <= xHi; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }

    // 2. Clear dynamic slots back to white.
    auto clearRect = [&](int x, int y, int w, int h) {
        for (int r = 0; r < h; ++r)
            for (int c = 0; c < w; ++c)
                display.drawPixel(ox + x + c, oy + y + r, WHITE);
    };
    clearRect(kBigLeftX,  kBigY,  kBigW, kBigH);
    clearRect(kBigRightX, kBigY,  kBigW, kBigH);
    clearRect(kTimeX,     kTimeY, kTimeW, kTimeH);
    clearRect(kDateX,     kDateY, kDateW, kDateH);
    clearRect(kDowX,      kDowY,  kDowW,  kDowH);

    // 3. BIG center digits.
    auto bigRows = [](int digit) -> const uint8_t* {
        switch (digit) {
            case 0: return &GLYPH_POW_BIG_0_ROWS[0][0];
            case 1: return &GLYPH_POW_BIG_1_ROWS[0][0];
            case 2: return &GLYPH_POW_BIG_2_ROWS[0][0];
            case 3: return &GLYPH_POW_BIG_3_ROWS[0][0];
            case 4: return &GLYPH_POW_BIG_4_ROWS[0][0];
            case 5: return &GLYPH_POW_BIG_5_ROWS[0][0];
            case 6: return &GLYPH_POW_BIG_6_ROWS[0][0];
            case 7: return &GLYPH_POW_BIG_7_ROWS[0][0];
            case 8: return &GLYPH_POW_BIG_8_ROWS[0][0];
            default: return &GLYPH_POW_BIG_9_ROWS[0][0];
        }
    };
    auto drawBig = [&](int digit, int slotX) {
        const uint8_t* rows = bigRows(digit);
        for (int r = 0; r < kBigH; ++r) {
            const uint8_t* row = rows + r * 4;
            for (int c = 0; c < kBigW; ++c) {
                if (row[c >> 3] & (0x80 >> (c & 7)))
                    display.drawPixel(ox + slotX + c, oy + kBigY + r, BLACK);
            }
        }
    };
    int n = data.sceneIndex;
    if (n < 0) n = 0;
    if (n > 99) n = 99;
    drawBig(n / 10, kBigLeftX);
    drawBig(n % 10, kBigRightX);

    // 4. Time "HH:MM" via WatchyDigits10x15.
    {
        char buf[6];
        buf[0] = '0' + (data.hour / 10) % 10;
        buf[1] = '0' + data.hour % 10;
        buf[2] = ':';
        buf[3] = '0' + (data.minute / 10) % 10;
        buf[4] = '0' + data.minute % 10;
        buf[5] = 0;
        drawGfxStr(display, WatchyDigits10x15, ox + kTimeX, oy + kTimeY, buf, BLACK);
    }

    // 5. Date "D/M" — just the day for now (matches reference's "4/5"
    //    style but with both fields dynamic).
    {
        char buf[6];
        int i = 0;
        if (data.day >= 10) buf[i++] = '0' + (data.day / 10) % 10;
        buf[i++] = '0' + data.day % 10;
        buf[i++] = '/';
        if (data.month >= 10) buf[i++] = '0' + (data.month / 10) % 10;
        buf[i++] = '0' + data.month % 10;
        buf[i] = 0;
        drawGfxStr(display, WatchyDigits5x7, ox + kDateX, oy + kDateY, buf, BLACK);
    }

    // 6. Weekday "MON".."SUN" via 5×7 SM letters, 6 px advance.
    {
        const char* name = dowName(data.dowIndex);
        int x = kDowX;
        for (int i = 0; name[i]; ++i) {
            const uint8_t* g = smLetter(name[i]);
            if (g) drawSm(display, g, ox + x, oy + kDowY, 5, 7, BLACK);
            x += 6;
        }
    }
}

#endif // WATCHY_SCREENS_POWER_H
