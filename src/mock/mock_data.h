#ifndef WATCHY_SCREENS_MOCK_DATA_H
#define WATCHY_SCREENS_MOCK_DATA_H

#include <math.h>
#include <stdio.h>
#include "../faces/multiday.h"
#include "../faces/stats.h"
#include "../faces/goodmorning.h"
#include "../faces/power.h"

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

    // Midline oscillates as a 40-tick triangle wave around y=82 (±10 px),
    // staying well within the chart bounds (y=31..103).
    uint32_t p = frame_ % 40;
    int off = (p < 20) ? (int)p : (int)(40 - p);  // 0..20..0
    d.midlineY = 72 + off;

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
inline PowerData MockState::currentPower() const {
    static const PowerData kScenes[3] = {
        // sceneIndex (BIG), hour, minute, day, month, dowIndex
        { 27, 10, 13, 4, 5, 6 /*SUN*/ },  // 0: reference snapshot
        {  5, 10, 10, 3, 3, 0 /*MON*/ },  // 1
        { 42, 13, 10, 8, 4, 1 /*TUE*/ },  // 2
    };
    return kScenes[(frame_ / 3) % 3];
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
