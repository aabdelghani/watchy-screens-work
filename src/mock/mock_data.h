#ifndef WATCHY_SCREENS_MOCK_DATA_H
#define WATCHY_SCREENS_MOCK_DATA_H

#include <math.h>
#include <stdio.h>
#include "../faces/multiday.h"
#include "../faces/stats.h"

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
    // 7 data-driven snapshots rotating every ~3 ticks. Labels stay
    // constant; values, time, date, and day-of-week cycle.
    struct Snapshot {
        const char* vCoins;
        const char* vSleep;
        const char* vChrono;
        const char* vResi;
        int h, m, d, mo;
        const char* dow;
    };
    static const Snapshot kScenes[7] = {
        { "218",  "2.45", "2.15", "76",  10, 13, 4, 5, "SUN" },  // reference
        { "412",  "1.10", "2.20", "81",  11, 30, 5, 5, "MON" },
        { "507",  "0.45", "2.05", "88",   1, 30, 6, 5, "TUE" },
        { "600",  "0.10", "2.00", "55",   3, 13, 7, 5, "WED" },
        { "725",  "1.50", "2.10", "60",  13, 13, 8, 5, "THU" },
        { "847",  "2.10", "2.12", "70",  10,  1, 1, 6, "FRI" },
        { "164",  "1.45", "2.08", "85",   0, 30, 2, 6, "SAT" },
    };

    int idx = (int)((frame_ / 3) % 7);
    const Snapshot& s = kScenes[idx];

    StatsData d{};
    d.labelCoins      = "COINS";
    d.labelSleepDebt  = "SLEEP DEBT";
    d.labelChronotype = "CRONOTYPE";
    d.labelResilience = "RESILIENCE";
    d.valueCoins      = s.vCoins;
    d.valueSleepDebt  = s.vSleep;
    d.valueChronotype = s.vChrono;
    d.valueResilience = s.vResi;
    d.hour = s.h; d.minute = s.m;
    d.day  = s.d; d.month  = s.mo;
    d.dow  = s.dow;
    d.sceneIndex = idx;
    return d;
}

// Reference snapshot. Number values now render through RainyHearts,
// so this no longer produces a byte-exact match against stats.png —
// it's the scene-0 data snapshot.
inline StatsData referenceStats() {
    StatsData d{};
    d.labelCoins      = "COINS";
    d.valueCoins      = "218";
    d.labelSleepDebt  = "SLEEP DEBT";
    d.valueSleepDebt  = "2.45";
    d.labelChronotype = "CRONOTYPE";
    d.valueChronotype = "2.15";
    d.labelResilience = "RESILIENCE";
    d.valueResilience = "76";
    d.hour = 10; d.minute = 13;
    d.day  = 4;  d.month  = 5;
    d.dow  = "SUN";
    d.sceneIndex = 0;
    return d;
}

#endif // WATCHY_SCREENS_MOCK_DATA_H
