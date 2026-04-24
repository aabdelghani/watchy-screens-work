#ifndef WATCHY_SCREENS_FRAME_H
#define WATCHY_SCREENS_FRAME_H

#include <stdint.h>

// 200×200 Watchy screen chrome. Outside the face octagon is filled with a
// 50% checkerboard dither; inside is plain white. The face is a 176×136
// octagon whose corner chamfers match references/176x136.png exactly:
// a 2-pixel gutter all around, then 34-row corner chamfers with a ~3:4
// slope (x steps 3 per 4 rows of y).
template <typename Display>
void drawWatchyChrome(Display& display) {
    const uint16_t BLACK = 0x0000;
    const uint16_t WHITE = 0xFFFF;
    const int SCREEN_W  = 200;
    const int SCREEN_H  = 200;
    const int BOX_W     = 176;
    const int BOX_H     = 136;
    const int OX        = (SCREEN_W - BOX_W) / 2;  // 12
    const int OY        = 32;
    const int MARGIN    = 2;   // gutter between box edge and face edge
    const int CHAMFER_H = 34;  // rows of vertical extent at each corner
    const int CHAMFER_W = 26;  // horizontal Δx across the chamfer

    // For each face-relative row dy (0..BOX_H-1), return the horizontal
    // inset from the box edge to the face boundary, or -1 if the row is
    // fully outside the face (the 2-px top/bottom gutters).
    auto insetAtY = [&](int dy) -> int {
        if (dy < MARGIN || dy >= BOX_H - MARGIN) return -1;
        int k;
        if (dy - MARGIN < CHAMFER_H) {
            // Top chamfer: k = 0 at dy=MARGIN, k = CHAMFER_H-1 at end.
            k = dy - MARGIN;
        } else if (dy >= BOX_H - MARGIN - CHAMFER_H) {
            // Bottom chamfer: k = 0 at dy=BOX_H-1-MARGIN, k = CHAMFER_H-1 at top.
            k = (BOX_H - 1 - MARGIN) - dy;
        } else {
            return MARGIN; // straight middle section
        }
        // Step pattern matching 176x136.png: 3 px horizontal per 4 px vertical,
        // so inset shrinks by (3k+3)/4 from (MARGIN + CHAMFER_W) at k=0.
        return (MARGIN + CHAMFER_W) - (3 * k + 3) / 4;
    };

    // 1. Fill whole screen with 50% dither (black pixels on white bg).
    for (int y = 0; y < SCREEN_H; ++y) {
        for (int x = 0; x < SCREEN_W; ++x) {
            if (((x + y) & 1) == 0) display.drawPixel(x, y, BLACK);
        }
    }

    // 2. Carve out white face interior.
    for (int dy = 0; dy < BOX_H; ++dy) {
        int ins = insetAtY(dy);
        if (ins < 0) continue;
        int x0 = OX + ins;
        int x1 = OX + BOX_W - 1 - ins;
        for (int x = x0; x <= x1; ++x) display.drawPixel(x, OY + dy, WHITE);
    }
}

#endif // WATCHY_SCREENS_FRAME_H
