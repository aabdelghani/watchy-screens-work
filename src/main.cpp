#include "faces/stats.h"
#include "faces/goodmorning.h"
#include "faces/multiday.h"
#include "mock/mock_data.h"

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

#ifdef WOKWI_SIM
// ────────────────────────────────────────────────────────────────────
// Wokwi simulator build. Faces draw into a 200×200 RGB565 canvas via
// Adafruit_GFX, then the canvas is blitted to a stock ILI9341 240×320
// LCD at offset (20, 60). Adafruit_GFX produces identical pixels
// regardless of the underlying driver, so a render that looks correct
// on the LCD will look the same on the real Watchy V3 SSD1681 panel.
//
// Pin 21 LED is a heartbeat: it stays HIGH while we're inside setup()
// (so a stuck LED means tft.begin() hung), then alternates per redraw.
// ────────────────────────────────────────────────────────────────────
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

static constexpr int8_t BLINK_PIN    = 21;
static constexpr int8_t TFT_CS_PIN   = 10;
static constexpr int8_t TFT_DC_PIN   = 9;
static constexpr int8_t TFT_MOSI_PIN = 11;
static constexpr int8_t TFT_SCK_PIN  = 12;

static constexpr int8_t BTN_MENU_PIN = 7;
static constexpr int8_t BTN_BACK_PIN = 6;
static constexpr int8_t BTN_UP_PIN   = 0;
static constexpr int8_t BTN_DOWN_PIN = 8;

static constexpr int LCD_OFFSET_X = 20;
static constexpr int LCD_OFFSET_Y = 60;
static constexpr int CANVAS_W     = 200;
static constexpr int CANVAS_H     = 200;

static Adafruit_ILI9341 tft(TFT_CS_PIN, TFT_DC_PIN, -1);
static GFXcanvas16 canvas(CANVAS_W, CANVAS_H);

static uint32_t mockTickCount  = 0;
static uint8_t  currentFaceIdx = 0;

static void renderFrame() {
    MockState mock;
    for (uint32_t i = 0; i < mockTickCount; ++i) mock.tick();

    canvas.fillScreen(0xFFFF);
    if (currentFaceIdx != 3) drawDitheredBorder(canvas);  // POWER reference has clean chamfer

    switch (currentFaceIdx) {
        case 0: drawStatsFace      (canvas, 12, 32, mock.currentStats());       break;
        case 1: drawGoodMorningFace(canvas, 12, 32, mock.currentGoodMorning()); break;
        case 2: drawMultidayFace   (canvas, 12, 32, mock.currentMultiday());    break;
        case 3: drawPowerFace      (canvas, 12, 32, mock.currentPower());       break;
    }

    tft.drawRGBBitmap(LCD_OFFSET_X, LCD_OFFSET_Y, canvas.getBuffer(), CANVAS_W, CANVAS_H);
}

void setup() {
    pinMode(BLINK_PIN, OUTPUT);
    digitalWrite(BLINK_PIN, HIGH);

    pinMode(BTN_MENU_PIN, INPUT_PULLUP);
    pinMode(BTN_BACK_PIN, INPUT_PULLUP);
    pinMode(BTN_UP_PIN,   INPUT_PULLUP);
    pinMode(BTN_DOWN_PIN, INPUT_PULLUP);

    SPI.begin(TFT_SCK_PIN, -1, TFT_MOSI_PIN, TFT_CS_PIN);
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(ILI9341_WHITE);

    renderFrame();
    digitalWrite(BLINK_PIN, LOW);
}

void loop() {
    static bool prevMenu = false, prevBack = false, prevUp = false, prevDown = false;
    static uint32_t lastAutoTick = 0;
    bool needRedraw = false;

    bool menu = digitalRead(BTN_MENU_PIN) == LOW;
    bool back = digitalRead(BTN_BACK_PIN) == LOW;
    bool up   = digitalRead(BTN_UP_PIN)   == LOW;
    bool down = digitalRead(BTN_DOWN_PIN) == LOW;
    if (up   && !prevUp)   { currentFaceIdx = (currentFaceIdx + 1) % 4;     needRedraw = true; }
    if (down && !prevDown) { currentFaceIdx = (currentFaceIdx + 3) % 4;     needRedraw = true; }
    if (menu && !prevMenu) { mockTickCount++;                               needRedraw = true; }
    if (back && !prevBack) { mockTickCount += 6;                            needRedraw = true; }
    prevMenu = menu; prevBack = back; prevUp = up; prevDown = down;

    uint32_t now = millis();
    if (now - lastAutoTick >= 1000) {
        mockTickCount++;
        lastAutoTick = now;
        needRedraw = true;
    }

    if (needRedraw) {
        digitalWrite(BLINK_PIN, HIGH);
        renderFrame();
        digitalWrite(BLINK_PIN, LOW);
    }
    delay(20);
}

#else
// ────────────────────────────────────────────────────────────────────
// Real-hardware build (Watchy V3 SSD1681 via sqfmi/Watchy).
// ────────────────────────────────────────────────────────────────────
#include <Watchy.h>

// Persisted across deep sleep. Storing only a tick counter — not the
// MockState itself — because MockState has member initializers whose ctor
// re-runs on every wake and would clobber the persistent value. A fresh
// MockState is fast-forwarded to mockTickCount on each refresh.
RTC_DATA_ATTR uint32_t mockTickCount = 0;
RTC_DATA_ATTR uint8_t  currentFaceIdx = 0;   // 0=stats, 1=goodmorning, 2=multiday, 3=power

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
            case 3: drawPowerFace      (display, 12, 32, mock.currentPower());       break;
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
                currentFaceIdx = (currentFaceIdx + 1) % 4;
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

#endif  // WOKWI_SIM
