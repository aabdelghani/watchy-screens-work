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
    d.currentDay = 3;
    for (int i = 0; i < 7; ++i) {
        d.bars[i].fillY0    = kRefBars[i].y0;
        d.bars[i].fillY1    = kRefBars[i].y1;
        d.bars[i].deltaUp   = kRefBars[i].deltaUp;
        d.bars[i].deltaDown = kRefBars[i].deltaDown;
        d.bars[i].check     = kRefBars[i].check;
    }
    return d;
}

// Static snapshot matching the reference PNG exactly (no animation).
inline MultidayData referenceMultiday() {
    MultidayData d{};
    d.title      = "NIGHTS";
    d.dayLabels  = "MTWTFSS";
    d.currentDay = 3;
    for (int i = 0; i < 7; ++i) {
        d.bars[i].fillY0    = kRefBars[i].y0;
        d.bars[i].fillY1    = kRefBars[i].y1;
        d.bars[i].deltaUp   = kRefBars[i].deltaUp;
        d.bars[i].deltaDown = kRefBars[i].deltaDown;
        d.bars[i].check     = kRefBars[i].check;
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
    return d;
}

#endif // WATCHY_SCREENS_MOCK_DATA_H
