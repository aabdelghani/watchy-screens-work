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

inline MultidayData MockState::currentMultiday() const {
    MultidayData d{};
    // Static title + day labels for demo; bars wobble and dot advances.
    d.title      = "NIGHTS";
    d.dayLabels  = "MTWTFSS";
    d.currentDay = (frame_ / 20) % 7; // dot moves every ~20s in real time

    const float basedBed[7]  = { 22.5f, 23.0f, 22.0f, 23.5f, 23.25f, 0.5f,  22.75f };
    const float basedWake[7] = {  7.0f,  6.5f,  8.0f,  7.5f,  6.75f, 7.25f, 6.25f };
    for (int i = 0; i < 7; ++i) {
        float phase = (frame_ + i * 3) * 0.12f;
        float wobble = 0.8f * (0.5f - 0.5f * cosf(phase));
        d.bars[i].bedtime   = basedBed[i] - wobble;
        d.bars[i].wakeup    = basedWake[i] + wobble;
        d.bars[i].check     = ((frame_ + i) / 5) % 2;
        d.bars[i].deltaUp   = ((frame_ + i) % 6) < 3;
        d.bars[i].deltaDown = !d.bars[i].deltaUp;
    }
    return d;
}

// Static snapshot matching the reference PNG exactly (no animation, no scroll).
inline MultidayData referenceMultiday() {
    MultidayData d{};
    d.title      = "NIGHTS";
    d.dayLabels  = "MTWTFSS";
    d.currentDay = 3;
    const MultidayBar ref[7] = {
        { 21.0f,  4.0f, false, true,  true  },
        { 21.5f,  3.5f, true,  false, true  },
        { 20.5f,  4.5f, true,  true,  true  },
        { 23.0f,  3.5f, true,  true,  false },
        { 22.0f,  3.0f, false, true,  true  },
        { 22.5f,  4.0f, true,  true,  true  },
        { 23.5f,  3.5f, true,  true,  false },
    };
    for (int i = 0; i < 7; ++i) d.bars[i] = ref[i];
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
