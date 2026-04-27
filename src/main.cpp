#include <Watchy.h>
#include "faces/stats.h"
#include "faces/goodmorning.h"
#include "faces/multiday.h"
#include "mock/mock_data.h"

// Persisted across deep sleep. Storing only a tick counter — not the
// MockState itself — because MockState has member initializers whose ctor
// re-runs on every wake and would clobber the persistent value. A fresh
// MockState is fast-forwarded to mockTickCount on each refresh.
RTC_DATA_ATTR uint32_t mockTickCount = 0;
RTC_DATA_ATTR uint8_t  currentFaceIdx = 0;   // 0=stats, 1=goodmorning, 2=multiday

template <typename Display>
static void drawDitheredBorder(Display& d) {
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
            faceLeft = FACE_W;
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
                d.drawPixel(x, y, 0);
            }
        }
    }
}

const watchySettings kSettings = {
    /* cityID                */ "",
    /* lat                   */ "",
    /* lon                   */ "",
    /* weatherAPIKey         */ "",
    /* weatherURL            */ "",
    /* weatherUnit           */ "imperial",
    /* weatherLang           */ "en",
    /* weatherUpdateInterval */ 30,
    /* ntpServer             */ "pool.ntp.org",
    /* gmtOffset             */ 0,
    /* vibrateOClock         */ false,
};

class MyWatchFace : public Watchy {
public:
    MyWatchFace() : Watchy(kSettings) {}

    void drawWatchFace() override {
        // Rebuild mock at the persistent frame.
        MockState mock;
        for (uint32_t i = 0; i < mockTickCount; ++i) mock.tick();

        display.fillScreen(0xFFFF);
        drawDitheredBorder(display);

        switch (currentFaceIdx) {
            case 0: drawStatsFace      (display, 12, 32, mock.currentStats());       break;
            case 1: drawGoodMorningFace(display, 12, 32, mock.currentGoodMorning()); break;
            case 2: drawMultidayFace   (display, 12, 32, mock.currentMultiday());    break;
        }

        // Each refresh advances animation by one tick so the once-per-minute
        // RTC wake yields a visible scene change.
        mockTickCount++;
    }

    void handleButtonPress() override {
        // Only intercept buttons while showing the watch face — leave the
        // SDK's menu navigation alone.
        if (guiState == WATCHFACE_STATE) {
            if (digitalRead(UP_BTN_PIN) == LOW) {
                mockTickCount++;
                showWatchFace(true);
                return;
            }
            if (digitalRead(DOWN_BTN_PIN) == LOW) {
                // Step "back" by ticking 6× — stats and goodmorning cycle on
                // (frame/3) % 7 and % 5 respectively, so this reverses both
                // approximately enough for design preview.
                mockTickCount += 6;
                showWatchFace(true);
                return;
            }
            if (digitalRead(MENU_BTN_PIN) == LOW) {
                currentFaceIdx = (currentFaceIdx + 1) % 3;
                showWatchFace(true);
                return;
            }
        }
        Watchy::handleButtonPress();
    }
};

MyWatchFace watchy;

void setup() { watchy.init(); }
void loop()  {}
