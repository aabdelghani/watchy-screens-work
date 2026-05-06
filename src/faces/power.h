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
    int sceneIndex;       // BIG center value, 0..99
    int hour;             // 0..23
    int minute;           // 0..59
    int day;              // 1..31
    int month;            // 1..12 (currently unused — only "/day" is shown)
    int dowIndex;         // 0=MON .. 6=SUN
    int bottomLeftValue;  // 1..59  — left bottom label (was static "38")
    int bottomRightValue; // 1..59  — right bottom label (was static "19")
    int ballX;            // face-rel x of ball center
    int ballY;            // face-rel y of ball center (mock sweeps along U-path)
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

// Draw one GFXfont glyph relative to (xRef, yBaseline). Honours
// xOffset/yOffset so glyphs like ':' (which sit lower than digits)
// align correctly. ESP32 unified memory — PROGMEM data is read directly.
template <typename Display>
inline int drawGfxChar(Display& d, const GFXfont& font, int xRef, int yBaseline, char c, uint16_t color) {
    uint8_t u = (uint8_t)c;
    if (u < font.first || u > font.last) return 0;
    const GFXglyph& g = font.glyph[u - font.first];
    const uint8_t* bm = font.bitmap + g.bitmapOffset;
    int total = (int)g.width * (int)g.height;
    for (int i = 0; i < total; ++i) {
        if (bm[i >> 3] & (0x80 >> (i & 7))) {
            d.drawPixel(xRef + g.xOffset + (i % g.width),
                        yBaseline + g.yOffset + (i / g.width), color);
        }
    }
    return g.xAdvance;
}

template <typename Display>
inline void drawGfxStr(Display& d, const GFXfont& font, int xRef, int yBaseline, const char* s, uint16_t color) {
    int x = xRef;
    for (; *s; ++s) x += drawGfxChar(d, font, x, yBaseline, *s, color);
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

    // Weekday clear is 22 wide so the rightmost column of the static
    // "SUN" — the N's right edge sits at face-x 121..122 — is fully
    // wiped before we draw a shorter day name like "TUE" / "SAT".
    constexpr int kDowX = 103, kDowY = 124;
    constexpr int kDowW = 22,  kDowH = 7;

    // U-shaped track (left vertical bar + bottom corridor + right
    // vertical bar) and the bottom value labels.
    //
    // - Left bar: solid black, cols 30..40 — centerline x=35.
    // - Right bar: solid black, cols 135..145 — centerline x=140.
    // - Bottom corridor centerline at y=96, spanning x=35..140 (the
    //   centroids of the two bars define the horizontal endpoints).
    constexpr int kTrackXL       = 35;   // left bar centerline
    constexpr int kTrackXR       = 140;  // right bar centerline
    constexpr int kTrackYTop     = 38;   // top of straight vertical bars
    constexpr int kTrackYBottom  = 96;   // corridor centerline
    constexpr int kBtmLeftTickX  = 50;   // tick centerline (left, baked into static)
    constexpr int kBtmRightTickX = 125;  // tick centerline (right)
    constexpr int kBtmY          = 104;  // top y of bottom value glyphs
    constexpr int kBtmH          = 7;
    constexpr int kBtmSlotW      = 13;   // wide enough to wipe original "38"/"19"
    constexpr int kBallR         = 3;    // 7-px diameter
    constexpr int kDarkLeftXR    = 47;   // bottom corridor: x ≤ this → on left cap
    constexpr int kDarkRightXL   = 128;  // bottom corridor: x ≥ this → on right cap

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

    // 1b. Erase the baked-in static "diamond" ball in the corridor
    //     middle (rows 96..101, cols 85..91) — 15 extras that violate
    //     the corridor's dither rule (BLACK iff (x+y) is odd). The
    //     dynamic ball drawn at step 8 is responsible for the moving
    //     ball; clearing the even-parity pixels removes the static
    //     decoy so the dither continues uninterrupted there.
    //
    //     The cap→dither "fuzzy edge" extras (cols 48..50 left and
    //     125..127 right) are kept on purpose: they form the two
    //     vertical bands the ball's half/half transition aligns with.
    for (int yy = 96; yy <= 101; ++yy)
        for (int xx = 85; xx <= 91; ++xx)
            if (((xx + yy) & 1) == 0)
                display.drawPixel(ox + xx, oy + yy, WHITE);

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

    // 4. Time "HH:MM" via WatchyDigits10x15. Reference positions
    //    (face-x): H1@50, H2@58, ':'@72, M1@79, M2@87 — extra 2 px
    //    of breathing room on each side of the colon vs auto-advance.
    //    yBaseline = top + 14 (15-tall digits with yOffset=-14).
    {
        const int yB = oy + kTimeY + 14;
        char h1 = '0' + (data.hour   / 10) % 10;
        char h2 = '0' + (data.hour   % 10);
        char m1 = '0' + (data.minute / 10) % 10;
        char m2 = '0' + (data.minute % 10);
        drawGfxChar(display, WatchyDigits10x15, ox + kTimeX +  0, yB, h1,  BLACK);  // col 50
        drawGfxChar(display, WatchyDigits10x15, ox + kTimeX +  8, yB, h2,  BLACK);  // col 58
        drawGfxChar(display, WatchyDigits10x15, ox + kTimeX + 22, yB, ':', BLACK);  // col 72
        drawGfxChar(display, WatchyDigits10x15, ox + kTimeX + 29, yB, m1,  BLACK);  // col 79
        drawGfxChar(display, WatchyDigits10x15, ox + kTimeX + 37, yB, m2,  BLACK);  // col 87
    }

    // 5. Date "D/M". Positions hand-tuned to match the weekday's
    //    cols 103..121 span: digit / slash / digit at (103, 109, 117).
    //    yBaseline = top + 6 (7-tall glyphs with yOffset=-6).
    {
        const int yB = oy + kDateY + 6;
        char dDigit = '0' + (data.day   % 10);
        char mDigit = '0' + (data.month % 10);
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX + 0,  yB, dDigit, BLACK);  // X at col 103
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX + 6,  yB, '/',    BLACK);  // / at col 109
        drawGfxChar(display, WatchyDigits5x7, ox + kDateX + 14, yB, mDigit, BLACK);  // Y at col 117
    }

    // 6. Weekday "MON".."SUN" via 5×7 SM letters. Reference uses a
    //    2-px gap between letters (advance = 7), giving the static
    //    "SUN" at cols 103/110/117.
    {
        const char* name = dowName(data.dowIndex);
        int x = kDowX;
        for (int i = 0; name[i]; ++i) {
            const uint8_t* g = smLetter(name[i]);
            if (g) drawSm(display, g, ox + x, oy + kDowY, 5, 7, BLACK);
            x += 7;
        }
    }

    // 7. Bottom values "38" / "19" — code-editable 1..59 each. The
    //    originals are baked into the static blit; clear an 11×7 white
    //    rect around each tick centerline, then render the new digits
    //    centered horizontally on that tick using WatchyDigits5x7.
    auto drawBtmValue = [&](int tickX, int value) {
        if (value < 1)  value = 1;
        if (value > 59) value = 59;

        const int slotX = tickX - kBtmSlotW / 2;
        for (int r = 0; r < kBtmH; ++r)
            for (int c = 0; c < kBtmSlotW; ++c)
                display.drawPixel(ox + slotX + c, oy + kBtmY + r, WHITE);

        char buf[3];
        if (value < 10) { buf[0] = '0' + value; buf[1] = 0; }
        else { buf[0] = '0' + (value / 10); buf[1] = '0' + (value % 10); buf[2] = 0; }

        int totalAdv = 0;
        for (int i = 0; buf[i]; ++i) {
            const GFXglyph& g =
                ((const GFXglyph*)WatchyDigits5x7.glyph)[(uint8_t)buf[i] - WatchyDigits5x7.first];
            totalAdv += g.xAdvance;
        }
        const int visualW = totalAdv - 1;     // drop trailing kerning
        const int xRef    = ox + tickX - visualW / 2;
        const int yB      = oy + kBtmY + (kBtmH - 1);
        drawGfxStr(display, WatchyDigits5x7, xRef, yB, buf, BLACK);
    };
    drawBtmValue(kBtmLeftTickX,  data.bottomLeftValue);
    drawBtmValue(kBtmRightTickX, data.bottomRightValue);

    // 8. Ball: 5×5 disc (rounded-corner) drawn pixel-by-pixel so that
    //    at the dark↔light boundary (bottom corridor caps meeting the
    //    dither) the ball reads half-white / half-black. Per-pixel rule:
    //    color WHITE if that pixel sits over a solid-black region of
    //    the static art, BLACK otherwise. Solid-black regions:
    //      - left bar  cols 30..40  rows 38..90
    //      - right bar cols 135..145 rows 38..90
    //      - bottom corridor caps  cols ≤ 47 or ≥ 128, rows 91..101
    {
        const int cx = data.ballX;
        const int cy = data.ballY;
        static const uint8_t kBallMask[5] = { 0x0E, 0x1F, 0x1F, 0x1F, 0x0E };
        auto isDark = [](int px, int py) -> bool {
            // Above frame: claim dark so ball pixels don't paint stray
            // BLACK on the white face above the top tabs.
            if (py < 30) return true;
            // Top tabs + chamfer-transition rows (rows 30..37): the
            // tab spans cols 30..58 (left) and 117..145 (right) at
            // rows 30..32; rows 33..37 only have the bar (cols 30..40
            // / 135..145) but we widen the dark zone to the tab range
            // so chamfer-corner ball pixels in the white frame
            // interior become harmless WHITE-on-WHITE no-ops.
            if (py <= 37)
                return (px >= 30 && px <= 58) || (px >= 117 && px <= 145);
            if (py <= 90)
                return (px >= 30 && px <= 40) || (px >= 135 && px <= 145);
            // Bottom corridor: dark zone extends OUTWARD to include the
            // fuzzy-edge vertical bands (cols 48..50 left, 125..127
            // right) so the ball's half/half transition straddles
            // them rather than the cap's hard edge.
            if (py <= 101)
                return (px <= 50) || (px >= 125);
            return false;
        };
        for (int dy = -2; dy <= 2; ++dy) {
            const uint8_t row = kBallMask[dy + 2];
            for (int dx = -2; dx <= 2; ++dx) {
                if (!(row & (0x10 >> (dx + 2)))) continue;
                const int px = cx + dx, py = cy + dy;
                display.drawPixel(ox + px, oy + py,
                                  isDark(px, py) ? WHITE : BLACK);
            }
        }
    }
}

#endif // WATCHY_SCREENS_POWER_H
