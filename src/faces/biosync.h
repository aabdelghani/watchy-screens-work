#ifndef WATCHY_SCREENS_BIOSYNC_H
#define WATCHY_SCREENS_BIOSYNC_H

#include <stdint.h>
#include <gfxfont.h>

#include "biosync_static.h"
#include "power.h"        // watchy_power::drawGfxChar
#include "../fonts/WatchyDigits10x15.h"

// BIOSYNC face. Reference: references/biosync.png. The 176×136 chrome
// (title, perimeter dial, date, weekday, decorations) comes from the
// static blit. The BIG center value is a procedural 7-segment glyph,
// the U-band below it is a dynamic black/dither progress fill, and
// HH:MM under the BIG is rendered with WatchyDigits10x15.
struct BiosyncData {
    int sceneIndex;   // 0..99 — drives the BIG center value
    int hour;         // 0..23
    int minute;       // 0..59
};

template <typename Display>
void drawBiosyncFace(Display& display, int ox, int oy, const BiosyncData& data) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;

    // 1. Base: blit the static reference.
    for (int r = 0; r < GLYPH_BIO_FACE_H; ++r) {
        const uint8_t* row = GLYPH_BIO_FACE_ROWS[r];
        for (int c = 0; c < GLYPH_BIO_FACE_W; ++c) {
            if (row[c >> 3] & (0x80 >> (c & 7)))
                display.drawPixel(ox + c, oy + r, BLACK);
        }
    }

    // ── BIG digits (15×28 7-segment) ──────────────────────────────
    // Slot positions scanned from biosync.png:
    //   left  digit "4" at cols 71..85
    //   right digit "5" at cols 91..105
    // both span rows 51..78. Segment thickness = 4 px.
    constexpr int kBigY      = 51;
    constexpr int kBigLeftX  = 71;
    constexpr int kBigRightX = 91;
    constexpr int kBigW      = 15;
    constexpr int kBigH      = 28;

    // Segment masks per digit. Horizontals (A=top, D=mid, G=bottom)
    // and verticals — either full (LV/RV) or half (LT/LB/RT/RB) so
    // that digits without the middle bar (0, 1, 7) keep continuous
    // side bars instead of the 4-px gap that half-segments leave.
    constexpr uint16_t SA  = 1 << 0;  // top horizontal
    constexpr uint16_t SD  = 1 << 1;  // middle horizontal
    constexpr uint16_t SG  = 1 << 2;  // bottom horizontal
    constexpr uint16_t SLV = 1 << 3;  // full left vertical (rows 0..27)
    constexpr uint16_t SRV = 1 << 4;  // full right vertical
    constexpr uint16_t SLT = 1 << 5;  // top-left half (rows 0..11)
    constexpr uint16_t SLB = 1 << 6;  // bottom-left half (rows 16..27)
    constexpr uint16_t SRT = 1 << 7;  // top-right half
    constexpr uint16_t SRB = 1 << 8;  // bottom-right half
    constexpr uint16_t kSeg[10] = {
        /*0*/ SA|SG|SLV|SRV,
        /*1*/ SRV,
        /*2*/ SA|SD|SG|SRT|SLB,
        /*3*/ SA|SD|SG|SRV,
        /*4*/ SD|SLT|SRV,
        /*5*/ SA|SD|SG|SLT|SRB,
        /*6*/ SA|SD|SG|SLV|SRB,
        /*7*/ SA|SRV,
        /*8*/ SA|SD|SG|SLV|SRV,
        /*9*/ SA|SD|SG|SLT|SRV,
    };

    auto fillRect = [&](int x, int y, int w, int h, uint16_t col) {
        for (int yy = 0; yy < h; ++yy)
            for (int xx = 0; xx < w; ++xx)
                display.drawPixel(ox + x + xx, oy + y + yy, col);
    };

    auto drawDigit = [&](int digit, int slotX) {
        fillRect(slotX, kBigY, kBigW, kBigH, WHITE);
        const uint16_t segs = kSeg[digit % 10];
        if (segs & SA)  fillRect(slotX,      kBigY,      15,  4, BLACK);
        if (segs & SD)  fillRect(slotX,      kBigY + 12, 15,  4, BLACK);
        if (segs & SG)  fillRect(slotX,      kBigY + 24, 15,  4, BLACK);
        if (segs & SLV) fillRect(slotX,      kBigY,       4, 28, BLACK);
        if (segs & SRV) fillRect(slotX + 11, kBigY,       4, 28, BLACK);
        if (segs & SLT) fillRect(slotX,      kBigY,       4, 12, BLACK);
        if (segs & SLB) fillRect(slotX,      kBigY + 16,  4, 12, BLACK);
        if (segs & SRT) fillRect(slotX + 11, kBigY,       4, 12, BLACK);
        if (segs & SRB) fillRect(slotX + 11, kBigY + 16,  4, 12, BLACK);
    };

    int n = data.sceneIndex;
    if (n < 0) n = 0;
    if (n > 99) n = 99;
    drawDigit(n / 10, kBigLeftX);
    drawDigit(n % 10, kBigRightX);

    // ── U-shaped progress band ────────────────────────────────────
    // The U wraps the BIG digits: left vertical (cols 50..56) and
    // right vertical (cols 120..126), connected by a bottom strip
    // (rows 88..94) with 1-px-per-row rounded corners — at row 88+d
    // the bottom spans cols (50+d)..(126-d). Walk right-top → bottom
    // → left-top; the first n% along the path renders solid black,
    // the rest renders as 50% checker dither (parity (x+y)&1 == 0).
    constexpr int kULeftX0  = 50,  kULeftX1  = 56;
    constexpr int kURightX0 = 120, kURightX1 = 126;
    constexpr int kUVertY0  = 42,  kUVertY1  = 87;
    constexpr int kUBotY0   = 88,  kUBotY1   = 94;
    constexpr int kUBotX0   = 50,  kUBotX1   = 126;
    constexpr int kRVLen  = kUVertY1 - kUVertY0 + 1;       // 46
    constexpr int kBotLen = kUBotX1  - kUBotX0  + 1;       // 77
    constexpr int kLVLen  = kRVLen;                         // 46
    constexpr int kUTotal = kRVLen + kBotLen + kLVLen;     // 169

    const int kThresh = (n * kUTotal + 50) / 100;

    auto plotBand = [&](int x, int y, int t) {
        if (t < kThresh) {
            display.drawPixel(ox + x, oy + y, BLACK);
        } else if (((x + y) & 1) == 0) {
            display.drawPixel(ox + x, oy + y, BLACK);
        } else {
            display.drawPixel(ox + x, oy + y, WHITE);
        }
    };

    // Right vertical: t = (y - kUVertY0).
    for (int y = kUVertY0; y <= kUVertY1; ++y) {
        const int t = y - kUVertY0;
        for (int x = kURightX0; x <= kURightX1; ++x)
            plotBand(x, y, t);
    }
    // Bottom strip with rounded corners. Walk right→left.
    for (int y = kUBotY0; y <= kUBotY1; ++y) {
        const int d  = y - kUBotY0;
        const int x0 = kUBotX0 + d;
        const int x1 = kUBotX1 - d;
        for (int x = x0; x <= x1; ++x) {
            const int t = kRVLen + (kUBotX1 - x);
            plotBand(x, y, t);
        }
    }
    // Left vertical: t = kRVLen + kBotLen + (kUVertY1 - y).
    for (int y = kUVertY0; y <= kUVertY1; ++y) {
        const int t = kRVLen + kBotLen + (kUVertY1 - y);
        for (int x = kULeftX0; x <= kULeftX1; ++x)
            plotBand(x, y, t);
    }

    // ── HH:MM under the BIG digits ────────────────────────────────
    // Static "10:13" lives at rows 98..112, spanning cols 50..98 (the
    // '1' glyph carries a left-extending serif down to col 50). Clear
    // that 49×15 slot to white, then redraw HH:MM with the same
    // WatchyDigits10x15 + drawGfxChar advance loop HOURLY uses.
    constexpr int kTimeX = 50, kTimeY = 98;
    constexpr int kTimeW = 49, kTimeH = 15;
    fillRect(kTimeX, kTimeY, kTimeW, kTimeH, WHITE);
    {
        using namespace watchy_power;
        const int yB = oy + kTimeY + 14;
        const char h1 = '0' + (data.hour   / 10) % 10;
        const char h2 = '0' + (data.hour   % 10);
        const char m1 = '0' + (data.minute / 10) % 10;
        const char m2 = '0' + (data.minute % 10);
        int x = ox + kTimeX;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, h1, BLACK);
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, h2, BLACK);
        x += 2;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, ':', BLACK);
        x += 2;
        x += drawGfxChar(display, WatchyDigits10x15, x, yB, m1, BLACK);
        drawGfxChar(display, WatchyDigits10x15, x, yB, m2, BLACK);
    }
}

#endif // WATCHY_SCREENS_BIOSYNC_H
