// Wokwi custom chip: SSD1681 1.54" 200×200 e-paper controller
// (the panel on Watchy V3). Mirrors the SSD1306 example pattern from
// docs.wokwi.com/chips-api but speaks SPI instead of I2C.
//
// Datasheet: https://www.solomon-systech.com/product/ssd1681/
// Subset of commands implemented covers what GxEPD2_154_D67 issues
// during normal display update; everything else is acknowledged and
// ignored.

#include "wokwi-api.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DISPLAY_W 200
#define DISPLAY_H 200
#define RAM_BYTES ((DISPLAY_W * DISPLAY_H) / 8)

// ── SSD1681 commands we implement ──────────────────────────────────
#define CMD_DRIVER_OUTPUT   0x01
#define CMD_GATE_DRIVING    0x03
#define CMD_DEEP_SLEEP      0x10
#define CMD_DATA_ENTRY_MODE 0x11
#define CMD_SW_RESET        0x12
#define CMD_TEMP_SENSOR     0x18
#define CMD_MASTER_ACT      0x20
#define CMD_DISPLAY_UPDATE2 0x22
#define CMD_WRITE_RAM_BW    0x24
#define CMD_WRITE_RAM_RED   0x26
#define CMD_BORDER_WAVEFORM 0x3C
#define CMD_RAM_X_RANGE     0x44
#define CMD_RAM_Y_RANGE     0x45
#define CMD_RAM_X_COUNTER   0x4E
#define CMD_RAM_Y_COUNTER   0x4F

// Refresh delay (microseconds). Real panel takes ~2 s for a full
// refresh; we cut it down so the simulator feels responsive.
#define REFRESH_US 800000

typedef struct {
    // Pins
    pin_t cs_pin;
    pin_t dc_pin;
    pin_t rst_pin;
    pin_t busy_pin;

    // SPI
    spi_dev_t spi;
    uint8_t   spi_rx;     // single-byte receive scratch

    // Display memory
    uint8_t bw_ram[RAM_BYTES];
    uint8_t red_ram[RAM_BYTES];   // unused by Watchy but kept for spec compliance
    uint8_t entry_mode;           // bits: x++/-, y++/-, x/y address counter direction
    uint16_t x_start, x_end;      // in bytes (each byte = 8 horizontal pixels)
    uint16_t y_start, y_end;      // in rows (pixels)
    uint16_t ram_x;               // current write column (bytes)
    uint16_t ram_y;               // current write row (pixels)

    // Command parsing state
    bool     selected;            // CS low?
    bool     in_command;          // DC low when this byte arrived
    uint8_t  cmd;                 // last command byte
    bool     ram_writing_bw;      // streaming into bw_ram
    bool     ram_writing_red;     // streaming into red_ram
    uint8_t  arg_buf[8];
    uint8_t  arg_idx;

    // Render
    buffer_t framebuffer;
    timer_t  refresh_timer;
} ssd1681_state_t;


// ── Helpers ────────────────────────────────────────────────────────

static void reset_state(ssd1681_state_t *s) {
    memset(s->bw_ram,  0xFF, sizeof(s->bw_ram));   // e-paper default = white
    memset(s->red_ram, 0x00, sizeof(s->red_ram));
    s->entry_mode = 0x03;                          // x↑, y↑, x address counter
    s->x_start = 0;
    s->x_end   = (DISPLAY_W / 8) - 1;
    s->y_start = 0;
    s->y_end   = DISPLAY_H - 1;
    s->ram_x   = 0;
    s->ram_y   = 0;
    s->ram_writing_bw = false;
    s->ram_writing_red = false;
    s->arg_idx = 0;
    s->cmd = 0;
}

static void render_framebuffer(ssd1681_state_t *s) {
    // bw_ram layout: row-major, 1 bit per pixel, MSB = leftmost.
    // 0 = black, 1 = white (typical SSD1681 inversion).
    for (int y = 0; y < DISPLAY_H; ++y) {
        for (int x = 0; x < DISPLAY_W; ++x) {
            int bit_index = (y * DISPLAY_W) + x;
            int byte_index = bit_index >> 3;
            int bit = 0x80 >> (bit_index & 7);
            bool white = s->bw_ram[byte_index] & bit;
            uint32_t pixel = white ? 0xFFFFFFFFu : 0xFF000000u;
            buffer_write(s->framebuffer,
                         (uint32_t)(y * DISPLAY_W + x) * 4u,
                         &pixel, sizeof(pixel));
        }
    }
}

// Advance RAM write pointer based on entry mode after one byte (8 px).
static void advance_ram_pointer(ssd1681_state_t *s) {
    bool x_inc      = (s->entry_mode & 0x01) != 0;   // bit 0
    bool y_inc      = (s->entry_mode & 0x02) != 0;   // bit 1
    bool addr_is_y  = (s->entry_mode & 0x04) != 0;   // bit 2: 1 = update Y first
    if (!addr_is_y) {
        // Update X first, then wrap to next Y line.
        if (x_inc) {
            if (s->ram_x >= s->x_end) {
                s->ram_x = s->x_start;
                if (y_inc) {
                    if (s->ram_y < s->y_end) s->ram_y++;
                    else s->ram_y = s->y_start;
                } else {
                    if (s->ram_y > s->y_start) s->ram_y--;
                    else s->ram_y = s->y_end;
                }
            } else s->ram_x++;
        } else {
            if (s->ram_x <= s->x_start) {
                s->ram_x = s->x_end;
                // also step Y
                if (y_inc) s->ram_y = (s->ram_y < s->y_end) ? s->ram_y + 1 : s->y_start;
                else       s->ram_y = (s->ram_y > s->y_start) ? s->ram_y - 1 : s->y_end;
            } else s->ram_x--;
        }
    } else {
        // Y first
        if (y_inc) {
            if (s->ram_y >= s->y_end) {
                s->ram_y = s->y_start;
                if (x_inc) s->ram_x = (s->ram_x < s->x_end) ? s->ram_x + 1 : s->x_start;
                else       s->ram_x = (s->ram_x > s->x_start) ? s->ram_x - 1 : s->x_end;
            } else s->ram_y++;
        } else {
            if (s->ram_y <= s->y_start) {
                s->ram_y = s->y_end;
                if (x_inc) s->ram_x = (s->ram_x < s->x_end) ? s->ram_x + 1 : s->x_start;
                else       s->ram_x = (s->ram_x > s->x_start) ? s->ram_x - 1 : s->x_end;
            } else s->ram_y--;
        }
    }
}

static void write_ram_byte(ssd1681_state_t *s, uint8_t value) {
    int byte_index = s->ram_y * (DISPLAY_W / 8) + s->ram_x;
    if (byte_index >= 0 && byte_index < RAM_BYTES) {
        if (s->ram_writing_bw)  s->bw_ram[byte_index]  = value;
        if (s->ram_writing_red) s->red_ram[byte_index] = value;
    }
    advance_ram_pointer(s);
}

// ── Command dispatch ───────────────────────────────────────────────

static void process_command(ssd1681_state_t *s) {
    switch (s->cmd) {
        case CMD_SW_RESET:
            reset_state(s);
            // brief BUSY pulse (1 ms simulated)
            pin_write(s->busy_pin, HIGH);
            timer_start(s->refresh_timer, 1000, false);
            break;

        case CMD_DATA_ENTRY_MODE:
            if (s->arg_idx == 1) s->entry_mode = s->arg_buf[0];
            break;

        case CMD_RAM_X_RANGE:
            if (s->arg_idx == 2) {
                s->x_start = s->arg_buf[0] & 0x3F;
                s->x_end   = s->arg_buf[1] & 0x3F;
                s->ram_x   = s->x_start;
            }
            break;

        case CMD_RAM_Y_RANGE:
            if (s->arg_idx == 4) {
                s->y_start = (uint16_t)s->arg_buf[0] | ((uint16_t)(s->arg_buf[1] & 0x01) << 8);
                s->y_end   = (uint16_t)s->arg_buf[2] | ((uint16_t)(s->arg_buf[3] & 0x01) << 8);
                s->ram_y   = s->y_start;
            }
            break;

        case CMD_RAM_X_COUNTER:
            if (s->arg_idx == 1) s->ram_x = s->arg_buf[0] & 0x3F;
            break;

        case CMD_RAM_Y_COUNTER:
            if (s->arg_idx == 2)
                s->ram_y = (uint16_t)s->arg_buf[0] | ((uint16_t)(s->arg_buf[1] & 0x01) << 8);
            break;

        case CMD_MASTER_ACT:
            // Full refresh: hold BUSY, render after the timer fires.
            pin_write(s->busy_pin, HIGH);
            timer_start(s->refresh_timer, REFRESH_US, false);
            break;

        // Acknowledged but no behaviour change in the simulator:
        case CMD_DRIVER_OUTPUT:
        case CMD_GATE_DRIVING:
        case CMD_DEEP_SLEEP:
        case CMD_TEMP_SENSOR:
        case CMD_DISPLAY_UPDATE2:
        case CMD_BORDER_WAVEFORM:
        default:
            break;
    }
}

// ── SPI byte handler ───────────────────────────────────────────────

static void on_spi_done(void *user_data, uint8_t *buffer, uint32_t count);

static void queue_next_byte(ssd1681_state_t *s) {
    spi_start(s->spi, &s->spi_rx, 1);
}

static void on_spi_done(void *user_data, uint8_t *buffer, uint32_t count) {
    ssd1681_state_t *s = (ssd1681_state_t *)user_data;
    if (count == 0) {
        queue_next_byte(s);
        return;
    }
    uint8_t value = buffer[0];

    // CS must be low for the chip to listen.
    if (!s->selected) {
        queue_next_byte(s);
        return;
    }

    bool dc_high = pin_read(s->dc_pin);   // HIGH = data, LOW = command

    if (!dc_high) {
        // New command byte: any in-flight RAM stream ends here.
        s->ram_writing_bw  = false;
        s->ram_writing_red = false;
        s->cmd     = value;
        s->arg_idx = 0;

        // Some commands have no args — dispatch immediately.
        if (s->cmd == CMD_SW_RESET || s->cmd == CMD_MASTER_ACT) {
            process_command(s);
        } else if (s->cmd == CMD_WRITE_RAM_BW) {
            s->ram_writing_bw = true;
        } else if (s->cmd == CMD_WRITE_RAM_RED) {
            s->ram_writing_red = true;
        }
    } else {
        // Data byte
        if (s->ram_writing_bw || s->ram_writing_red) {
            write_ram_byte(s, value);
        } else if (s->arg_idx < sizeof(s->arg_buf)) {
            s->arg_buf[s->arg_idx++] = value;
            process_command(s);   // some commands dispatch incrementally
        }
    }
    queue_next_byte(s);
}

// ── Pin watchers ───────────────────────────────────────────────────

static void on_cs_change(void *user_data, pin_t pin, uint32_t value) {
    ssd1681_state_t *s = (ssd1681_state_t *)user_data;
    s->selected = (value == LOW);
}

static void on_rst_change(void *user_data, pin_t pin, uint32_t value) {
    ssd1681_state_t *s = (ssd1681_state_t *)user_data;
    if (value == LOW) {
        reset_state(s);
        pin_write(s->busy_pin, HIGH);
        timer_start(s->refresh_timer, 1000, false);
    }
}

// ── Refresh timer callback ─────────────────────────────────────────

static void on_refresh_done(void *user_data) {
    ssd1681_state_t *s = (ssd1681_state_t *)user_data;
    render_framebuffer(s);
    pin_write(s->busy_pin, LOW);
}

// ── chip_init ──────────────────────────────────────────────────────

void chip_init(void) {
    ssd1681_state_t *s = malloc(sizeof(ssd1681_state_t));
    memset(s, 0, sizeof(*s));
    reset_state(s);

    // Allocate the framebuffer (200×200 px × 4 bytes).
    uint32_t w = DISPLAY_W, h = DISPLAY_H;
    s->framebuffer = framebuffer_init(&w, &h);

    // Pin configuration
    s->cs_pin   = pin_init("CS",  INPUT);
    s->dc_pin   = pin_init("DC",  INPUT);
    s->rst_pin  = pin_init("RST", INPUT);
    s->busy_pin = pin_init("BUSY", OUTPUT);
    pin_write(s->busy_pin, LOW);

    const pin_watch_config_t cs_watch = {
        .edge = BOTH, .pin_change = on_cs_change, .user_data = s,
    };
    pin_watch(s->cs_pin, &cs_watch);

    const pin_watch_config_t rst_watch = {
        .edge = FALLING, .pin_change = on_rst_change, .user_data = s,
    };
    pin_watch(s->rst_pin, &rst_watch);

    // SPI setup — DIN = MOSI, CLK = SCK. We don't drive MISO.
    const spi_config_t spi_cfg = {
        .sck   = pin_init("CLK", INPUT),
        .miso  = NO_PIN,
        .mosi  = pin_init("DIN", INPUT),
        .done  = on_spi_done,
        .user_data = s,
    };
    s->spi = spi_init(&spi_cfg);
    queue_next_byte(s);

    // Refresh timer
    const timer_config_t timer_cfg = {
        .callback = on_refresh_done,
        .user_data = s,
    };
    s->refresh_timer = timer_init(&timer_cfg);
}
