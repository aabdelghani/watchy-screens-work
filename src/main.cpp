#include <Arduino.h>
#include <GxEPD2_BW.h>
#include "faces/stats.h"
#include "mock/mock_data.h"

// ── Hardware configuration (Watchy v2) ─────────────────────────────
// GDEH0154D67 200×200 1.54" E-Ink
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(
    GxEPD2_154_D67(/*CS=*/5, /*DC=*/10, /*RST=*/9, /*BUSY=*/19));

const int PIN_UP   = 26;
const int PIN_DOWN = 25;

// ── State ──────────────────────────────────────────────────────────
MockState mock;
enum Face { FACE_STATS, FACE_COUNT };
Face currentFace = FACE_STATS;
bool forceRedraw = true;

// ── Dithered border (checkerboard outside octagonal face area) ─────
template <typename Display>
void drawDitheredBorder(Display& d) {
    const int SCREEN = 200;
    const int FACE_W = 176;
    const int FACE_H = 136;
    const int OX = 12;
    const int OY = 32;
    const int C  = 16;   // chamfer

    for (int y = 0; y < SCREEN; ++y) {
        int localY = y - OY;
        int faceLeft, faceRight;

        if (localY < 0 || localY >= FACE_H) {
            faceLeft = FACE_W;   // no face in this row
            faceRight = -1;
        } else {
            if (localY < C) {
                faceLeft  = C - localY;
                faceRight = FACE_W - 1 - (C - localY);
            } else if (localY >= FACE_H - C) {
                int dd = localY - (FACE_H - C) + 1;
                faceLeft  = dd;
                faceRight = FACE_W - 1 - dd;
            } else {
                faceLeft  = 0;
                faceRight = FACE_W - 1;
            }
        }

        for (int x = 0; x < SCREEN; ++x) {
            int localX = x - OX;
            bool inside = (localY >= 0 && localY < FACE_H) &&
                          (localX >= faceLeft && localX <= faceRight);
            if (!inside && ((x + y) & 1) == 0) {
                d.drawPixel(x, y, 0);   // black checker
            }
        }
    }
}

// ── Button handling ────────────────────────────────────────────────
void handleButtons() {
    static bool lastUp = HIGH;
    static bool lastDown = HIGH;
    bool up   = digitalRead(PIN_UP);
    bool down = digitalRead(PIN_DOWN);

    if (lastUp == HIGH && up == LOW) {
        currentFace = static_cast<Face>((currentFace + 1) % FACE_COUNT);
        forceRedraw = true;
    }
    if (lastDown == HIGH && down == LOW) {
        currentFace = static_cast<Face>((currentFace - 1 + FACE_COUNT) % FACE_COUNT);
        forceRedraw = true;
    }
    lastUp   = up;
    lastDown = down;
}

// ── Setup ──────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(PIN_UP,   INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);

    display.init(0, true);          // 0 = full refresh on init
    display.setFullWindow();
    display.fillScreen(0xFFFF);     // white
    display.display(false);
}

// ── Main loop ──────────────────────────────────────────────────────
void loop() {
    handleButtons();

    // Update mock state once per real second
    mock.tick();

    // Redraw every frame (E-Ink partial update is fast enough for 1 Hz)
    display.fillScreen(0xFFFF);     // clear to white
    drawDitheredBorder(display);    // checkerboard border

    switch (currentFace) {
        case FACE_STATS:
        default: {
            StatsData d = mock.currentStats();
            drawStatsFace(display, 12, 32, d);
            break;
        }
    }

    display.display(true);          // partial update
    delay(1000);
}
