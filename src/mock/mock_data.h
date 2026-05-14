#ifndef WATCHY_SCREENS_MOCK_DATA_H
#define WATCHY_SCREENS_MOCK_DATA_H

#include <math.h>
#include <stdio.h>
#include "../faces/multiday.h"
#include "../faces/stats.h"
#include "../faces/goodmorning.h"
#include "../faces/power.h"
#include "../faces/hourly.h"
#include "../faces/biosync.h"

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
    BiosyncData currentBiosync() const;

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

    // Ball cycles through 8 discrete stop positions along the U-track,
    // dwelling at each for kDwell ticks before teleporting to the next.
    // Two of the stops (#3 and #5) sit on the dark-to-light boundary
    // on the bottom corridor (cols 50 and 125), where the per-pixel
    // rule renders the ball half-WHITE / half-BLACK at the
    // cap-edge vertical bands.
    struct BallStop { int x; int y; };
    constexpr BallStop kStops[8] = {
        {  50, 28 },   // 0: top stub L (middle, beside "60")
        {  35, 50 },   // 1: vertical L (upper portion)
        {  35, 80 },   // 2: vertical L (lower portion)
        {  50, 96 },   // 3: bottom corridor — LEFT half/half boundary
        {  87, 96 },   // 4: bottom corridor — middle (full dither)
        { 125, 96 },   // 5: bottom corridor — RIGHT half/half boundary
        { 140, 80 },   // 6: vertical R (lower portion)
        { 125, 28 },   // 7: top stub R (middle, beside "0")
    };
    constexpr uint32_t kDwell = 4;  // ticks per stop (~4 s)
    const BallStop& stop = kStops[(frame_ / kDwell) % 8];
    d.ballX = stop.x;
    d.ballY = stop.y;
    return d;
}

// ── Hourly mock ────────────────────────────────────────────────────
//
// 7 hand-picked scenes that rotate every kDwell ticks. Times mirror
// the STATS face exactly (10:13, 11:30, 01:30, 03:13, 13:13, 10:01,
// 00:30) so the three time-displaying faces stay in sync. BIG values
// come from the user-requested test pool {47, 56, 06, 25}, cycled.
inline HourlyData MockState::currentHourly() const {
    // 3 scenes whose HH:MM digits collectively exercise every glyph
    // 0..9 in the new WatchyDigitsRef10x15 font, all valid clock
    // times: 12:34 → {1,2,3,4}, 06:57 → {0,6,5,7}, 09:18 → {0,9,1,8}.
    static const HourlyData kScenes[3] = {
        // sceneIndex, hour, minute, day, month, dowIndex,    dotX, checkerVariant
        { 47, 12, 34,  4,  5, 6 /*SUN*/,                       87, 0 },  // 12:34
        { 56,  6, 57,  1,  3, 0 /*MON*/,                       87, 0 },  // 06:57
        {  6,  9, 18,  2,  8, 2 /*WED*/,                       87, 0 },  // 09:18
    };
    constexpr uint32_t kDwell = 4;  // ticks per scene (~4 s)
    return kScenes[(frame_ / kDwell) % 3];
}

// ── Biosync mock ───────────────────────────────────────────────────
//
// 3 scenes whose HH:MM digits collectively exercise every glyph 0..9
// in the new WatchyDigitsRef10x15 font, all valid clock times
// (12:34, 06:57, 09:18). BIG values from the test pool {20, 32, 64}.
inline BiosyncData MockState::currentBiosync() const {
    struct Scene { int big, hour, minute; };
    static constexpr Scene kScenes[3] = {
        { 20, 12, 34 },
        { 32,  6, 57 },
        { 64,  9, 18 },
    };
    constexpr uint32_t kDwell = 4;  // ticks per scene (~4 s)
    const Scene& s = kScenes[(frame_ / kDwell) % 3];
    BiosyncData d{};
    d.sceneIndex  = s.big;
    d.hour        = s.hour;
    d.minute      = s.minute;
    // Hour-marker diamond cycles through a small set of dial positions
    // (hours 2, 3, 6, 19), one step per tick (~1 s).
    // markerIndex stores hour-1 since kDialPos[] is 0-indexed.
    static constexpr int kMarkerHours[4] = { 2, 3, 6, 19 };
    d.markerIndex = kMarkerHours[frame_ % 4] - 1;
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
