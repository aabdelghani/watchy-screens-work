#ifndef WATCHY_SCREENS_MOCK_DATA_H
#define WATCHY_SCREENS_MOCK_DATA_H

#include <math.h>
#include <stdio.h>
#include "../faces/multiday.h"
#include "../faces/stats.h"
#include "../faces/goodmorning.h"
#include "../faces/power.h"
#include "../faces/hourly.h"

// Mock state evolves over ticks (1 tick = ~1 real second).
// Animation spec from brief:
//  - Bars slowly grow/shrink to varying heights
//  - Current-day dot moves to next day periodically
//  - All text fields scroll one character per second through a-z 0-9
class MockState {
public:
    void tick();                          // advance by 1
    MultidayData currentMultiday() const; // snapshot for rendering
    StatsData currentStats() const;       // snapshot for rendering
    GoodMorningData currentGoodMorning() const;
    PowerData currentPower() const;
    HourlyData currentHourly() const;

private:
    uint32_t frame_ = 0;

    // Produce a window over the scrolling alphabet at offset `offset`.
    // Caller owns the buffer.
    static void scrollText(char* out, int len, uint32_t offset);

    mutable char titleBuf_[16] = "NIGHTS";
    mutable char dayBuf_[8]    = "MTWTFSS";

    // Stats scroll buffers
    mutable char statLabel_[4][16];
    mutable char statValue_[4][16];
};

inline void MockState::tick() { frame_++; }

inline void MockState::scrollText(char* out, int len, uint32_t offset) {
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    const int N = (int)(sizeof(alpha) - 1);
    for (int i = 0; i < len; ++i) {
        out[i] = alpha[(offset + i) % N];
    }
    out[len] = 0;
}

// ── Multiday mock ──────────────────────────────────────────────────

// Reference bar pattern measured pixel-exact from references/multyday.png
// (face-relative; chart spans y=31..103). Each entry is a full per-bar
// snapshot — fill extent + marker flags — so the mock can rotate whole
// patterns across slots without splitting geometry from glyphs.
struct MultidayRefBar {
    int  y0, y1;
    bool deltaUp, deltaDown, check;
};
static const MultidayRefBar kRefBars[7] = {
    { 47, 82,  false, false, true  }, // M
    { 48, 82,  true,  false, true  }, // T
    { 31, 82,  true,  false, true  }, // W
    { 41, 103, true,  true,  false }, // T
    { 40, 82,  false, true,  true  }, // F
    { 35, 82,  false, true,  true  }, // S
    { 37, 82,  false, false, false }, // S
};

inline MultidayData MockState::currentMultiday() const {
    MultidayData d{};
    d.title      = "NIGHTS";
    d.dayLabels  = "MTWTFSS";
    d.currentDay = (int)(frame_ % 7);
    for (int i = 0; i < 7; ++i) {
        d.bars[i].fillY0    = kRefBars[i].y0;
        d.bars[i].fillY1    = kRefBars[i].y1;
        d.bars[i].deltaUp   = kRefBars[i].deltaUp;
        d.bars[i].deltaDown = kRefBars[i].deltaDown;
        d.bars[i].check     = kRefBars[i].check;
    }

    // Midline oscillates as an 8-tick triangle wave between y=79..83
    // so the right-edge value label cycles 15..19 (slope +1 unit/px
    // from y=82 → 18).
    uint32_t p = frame_ % 8;
    int off = (p < 4) ? (int)p : (int)(8 - p);  // 0..4..0
    d.midlineY = 79 + off;

    // Random per-frame permutation of the 7 source bars across slots:
    // Fisher-Yates seeded from frame_, so the shuffle is deterministic
    // (reproducible in --dump) but visually pseudo-random.
    int perm[7] = { 0, 1, 2, 3, 4, 5, 6 };
    uint32_t s = frame_ * 2654435761u + 1u;
    for (int i = 6; i > 0; --i) {
        s = s * 1664525u + 1013904223u;
        int j = (int)((s >> 16) % (uint32_t)(i + 1));
        int t = perm[i]; perm[i] = perm[j]; perm[j] = t;
    }
    for (int i = 0; i < 7; ++i) d.slotSource[i] = perm[i];
    return d;
}

// Static snapshot matching the reference PNG exactly (no animation).
inline MultidayData referenceMultiday() {
    MultidayData d{};
    d.title      = "NIGHTS";
    d.dayLabels  = "MTWTFSS";
    d.currentDay = 3;
    d.midlineY   = 82;
    for (int i = 0; i < 7; ++i) {
        d.bars[i].fillY0    = kRefBars[i].y0;
        d.bars[i].fillY1    = kRefBars[i].y1;
        d.bars[i].deltaUp   = kRefBars[i].deltaUp;
        d.bars[i].deltaDown = kRefBars[i].deltaDown;
        d.bars[i].check     = kRefBars[i].check;
        d.slotSource[i]     = i;  // identity mapping → reference PNG
    }
    return d;
}

// ── Stats mock ─────────────────────────────────────────────────────

inline StatsData MockState::currentStats() const {
    StatsData d{};

    // Scrolling lengths match the reference strings:
    // COINS=5, SLEEP DEBT=10, CHRONOTYPE=10, RESILIENCE=10
    // 218 c=5, 2.45 h=6, 2.15 h=6, 76%=3
    const int labelLen[4] = { 5, 10, 10, 10 };
    const int valueLen[4] = { 5, 6, 6, 3 };

    for (int i = 0; i < 4; ++i) {
        scrollText(statLabel_[i], labelLen[i], frame_ + i * 7);
        scrollText(statValue_[i], valueLen[i], frame_ + i * 7 + 50);
    }

    d.labelCoins      = statLabel_[0];
    d.valueCoins      = statValue_[0];
    d.labelSleepDebt  = statLabel_[1];
    d.valueSleepDebt  = statValue_[1];
    d.labelChronotype = statLabel_[2];
    d.valueChronotype = statValue_[2];
    d.labelResilience = statLabel_[3];
    d.valueResilience = statValue_[3];

    // Accelerated clock (x20)
    uint32_t totalSec = frame_ * 20;
    d.hour   = (totalSec / 3600) % 24;
    d.minute = (totalSec / 60) % 60;
    d.day    = 4 + (totalSec / 86400) % 28;
    d.month  = 5;
    d.dow    = "SUN";

    // Rotate the 4 right-column values across 7 hardcoded snapshots,
    // switching every 3 ticks (~3 seconds in the sim).
    d.sceneIndex = (int)((frame_ / 3) % 7);

    return d;
}

// ── Goodmorning mock ───────────────────────────────────────────────

inline GoodMorningData MockState::currentGoodMorning() const {
    // 5 snapshots, switching every ~3 ticks.
    return goodMorningScene((int)((frame_ / 3) % 5));
}

// ── Power mock ─────────────────────────────────────────────────────

// Three hand-picked snapshots that rotate every ~3 sim ticks. Scene 0
// is pinned to the reference PNG ("27 / 10:13 / 4/5 / SUN") so design
// review against the reference is always one keypress away.
//
// bottomLeftValue / bottomRightValue are arbitrary 1..59 values
// (customer spec: "any value 1..59, just make it editable from code").
// ballX/ballY are overwritten below with a U-path sweep so the ball
// glides continuously even while the digit scenes cycle.
inline PowerData MockState::currentPower() const {
    static const PowerData kScenes[3] = {
        // sceneIndex, h, m, d, mo, dow,           btmL, btmR, ballX, ballY
        { 27, 10, 13, 4, 5, 6 /*SUN*/,             38,   19,   0,    0 },  // 0: reference
        {  5, 10, 10, 3, 3, 0 /*MON*/,             12,   47,   0,    0 },  // 1
        { 42, 13, 10, 8, 4, 1 /*TUE*/,              5,   33,   0,    0 },  // 2
    };
    PowerData d = kScenes[(frame_ / 3) % 3];

    // Ball travels the full inverted-U around the BIG digits with 45°
    // chamfered corners and small horizontal stubs at the top flanking
    // "60" and "0". Top stubs sit at y=28 (3 px higher than the tab's
    // mid-row) so the corner sits flush with the frame's top edge:
    //   0) Top stub L:     (x=60..41, y=28)         20 steps (going left)
    //   1) Top chamfer L:  (40,28)→(36,32)           5 steps (down-left)
    //   2) Vertical L:     (35, y=33..90)           58 steps
    //   3) Bottom ch. L:   (35,91)→(39,95)           5 steps (down-right)
    //   4) Bottom horiz.:  (x=40..134, y=96)        95 steps
    //   5) Bottom ch. R:   (135,96)→(139,92)         5 steps (up-right)
    //   6) Vertical R:     (140, y=91..33)          59 steps
    //   7) Top chamfer R:  (140,33)→(136,29)         5 steps (up-left)
    //   8) Top stub R:     (x=135..116, y=28)       20 steps (going left)
    // Total 272 path positions. 180-tick triangle wave (~3 px/tick).
    constexpr int kSegs[9] = { 20, 5, 58, 5, 95, 5, 59, 5, 20 };
    constexpr int kTotal   = 272;
    constexpr int kHalf    = 90;

    uint32_t p = frame_ % (2 * kHalf);
    int phase = (p < (uint32_t)kHalf) ? (int)p : (int)(2 * kHalf - p);  // 0..90..0
    int s = phase * kTotal / kHalf;

    int seg = 0;
    while (seg < 9 && s >= kSegs[seg]) { s -= kSegs[seg]; ++seg; }
    switch (seg) {
        case 0:  d.ballX = 60 - s;   d.ballY = 28;        break;  // top stub L
        case 1:  d.ballX = 40 - s;   d.ballY = 28 + s;    break;  // top chamfer L
        case 2:  d.ballX = 35;       d.ballY = 33 + s;    break;  // vertical L
        case 3:  d.ballX = 35 + s;   d.ballY = 91 + s;    break;  // bottom chamfer L
        case 4:  d.ballX = 40 + s;   d.ballY = 96;        break;  // bottom horizontal
        case 5:  d.ballX = 135 + s;  d.ballY = 96 - s;    break;  // bottom chamfer R
        case 6:  d.ballX = 140;      d.ballY = 91 - s;    break;  // vertical R
        case 7:  d.ballX = 140 - s;  d.ballY = 33 - s;    break;  // top chamfer R
        default: d.ballX = 135 - s;  d.ballY = 28;        break;  // top stub R
    }
    return d;
}

// ── Hourly mock ────────────────────────────────────────────────────
//
// Phase 1: pin to the reference snapshot ("42 / 10:13 / 4/5 / SUN /
// dot at x=87"). The 5 dynamic fields (sceneIndex, time, day/month,
// dowIndex, dotX) are exposed in HourlyData ready to be animated in
// a follow-up; for now they all hold the reference values.
inline HourlyData MockState::currentHourly() const {
    HourlyData d{};
    d.sceneIndex = 42;
    d.hour       = 10;
    d.minute     = 13;
    d.day        = 4;
    d.month      = 5;
    d.dowIndex   = 6;   // SUN
    d.dotX       = 87;  // reference position; future animation will sweep
    return d;
}

// Static snapshot matching the reference PNG exactly.
inline StatsData referenceStats() {
    StatsData d{};
    d.labelCoins      = "COINS";
    d.valueCoins      = "218 c";
    d.labelSleepDebt  = "SLEEP DEBT";
    d.valueSleepDebt  = "2.45 h";
    d.labelChronotype = "CHRONOTYPE";
    d.valueChronotype = "2.15 h";
    d.labelResilience = "RESILIENCE";
    d.valueResilience = "76%";
    d.hour = 10; d.minute = 13;
    d.day  = 4;  d.month  = 5;
    d.dow  = "SUN";
    d.sceneIndex = 0;  // pins --dump Stats to the reference scene
    return d;
}

#endif // WATCHY_SCREENS_MOCK_DATA_H
