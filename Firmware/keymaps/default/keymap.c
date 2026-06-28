 // Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SKCH = SAFE_RANGE,
    PARAM,
    COMP,
    NTAB,
    CTAB,
    RTAB,
    DASH,
    ELIP,
    CARD,
    ITLC,
    BOLD,
    LAYER,
    LYR_ENT,
    LYR_BLD,
    LYR_EXT,
    ESC_SKCH,
    CHMF,
    DIMENSION,
    EXTRUDE,
};

// Layer declarations
enum {
    NumPad = 0,  // Number pad
    Fusion = 1,  // Autodesk Fusion
    Gen = 2,  // General shortcuts
    Sketch = 3, // In sketch shortcuts
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [NumPad] = LAYOUT_3x4(  // Number pad
        KC_P7,   KC_P8,   KC_P9,
        KC_P4,   KC_P5,   KC_P6,
        KC_P1,   KC_P2,   KC_P3,
        KC_PDOT,   KC_P0, LYR_ENT  // Last one is enter/layer
    ),
    [Fusion] = LAYOUT_3x4(  // Autodesk Fusion
        KC_UNDO,   KC_AGAIN,   KC_M,  // Undo, redo, move/copy
        SKCH,   ESC_SKCH,   KC_J,  // New sketch, escape+sketch, joint
        CHMF,   KC_F,   EXTRUDE,  // Chamfer, fillet, extrude
        PARAM,   COMP,   LYR_ENT  // New parameter, new component, enter/layer
    ),
    [Sketch] = LAYOUT_3x4(  // In sketch shortcuts
        KC_UNDO,   KC_AGAIN,   KC_M,  // Undo, redo, move/copy
        SKCH,   KC_R,   KC_C,  // New sketch, rectangle, circle
        KC_L,   DIMENSION,   LYR_EXT,  // Line, dimension, extrude
        PARAM,   COMP,  KC_ENTER  // New parameter, new component, enter
    ),
    [Gen] = LAYOUT_3x4(  // General shortcuts
        KC_UNDO,   KC_AGAIN,   KC_COPY,  // Undo, redo, copy
        KC_PASTE,   NTAB,   CTAB,  // Paste, new tab, close tab
        RTAB,   DASH,   ELIP,  // Restore tab, em dash, elipses
        CARD,   ITLC,   LYR_BLD  // Card suites, italic, bold/layer
    )
};

#define TAPPING_TERM 200

//    0
// 1     2
//    3
// 4     5
//    6

int numberpad[4] = {0, 0, 255, 0};  // Layer 0: Number pad: Blue
int fusion[4] = {255, 255, 0, 1};  // Layer 1: Fusion: Yellow/orange
int sketch[4] = {255, 0, 0, 3};  // Layer 3: Sketch: Red
int generalshortcuts[4] = {0, 255, 0, 2};  // Layer 2: General shortcuts: Green

void display_num(int list[]) {
    int r = list[0];
    int g = list[1];
    int b = list[2];
    int layer = list[3];
    if (layer == 0) {
        rgblight_setrgb_range(r, g, b, 0, 7); // Turn all LED on
        rgblight_setrgb_at(0, 0, 0, 3);  // Turn LED 3 off
    }
    if (layer == 1) {
        rgblight_setrgb_range(0, 0, 0, 0, 7); // Turn all LED off
        rgblight_setrgb_at(r, g, b, 2);
        rgblight_setrgb_at(r, g, b, 5);
    }
    if (layer == 2) {
        rgblight_setrgb_range(r, g, b, 0, 7); // Turn all LED on
        rgblight_setrgb_at(0, 0, 0, 1);
        rgblight_setrgb_at(0, 0, 0, 5);
    }
    if (layer == 3) {
        rgblight_setrgb_range(r, g, b, 0, 7); // Turn all LED on
        rgblight_setrgb_at(0, 0, 0, 1);
        rgblight_setrgb_at(0, 0, 0, 4);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case SKCH:  // s to search, type sketch, then hit enter
            if (record->event.pressed) {
                tap_code(KC_S);
                SEND_STRING("sketch");
                tap_code(KC_ENTER);
                layer_off(Fusion);
                layer_on(Sketch);
                display_num(sketch);
            }
            return false;
        case PARAM:
            if (record->event.pressed) {
                tap_code(KC_S);
                SEND_STRING("parameter");
                tap_code(KC_ENTER);
            }
            return false;
        case COMP:
            if (record->event.pressed) {
                tap_code(KC_S);
                SEND_STRING("new component");
                tap_code(KC_ENTER);
            }
            return false;
        case NTAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_T);
                unregister_code(KC_LCTL);
            }
            return false;
        case CTAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_W);
                unregister_code(KC_LCTL);
            }
            return false;
        case RTAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                tap_code(KC_T);
                unregister_code(KC_LSFT);
                unregister_code(KC_LCTL);
            }
            return false;
        case ELIP:
            if (record->event.pressed) {
                // register_code(KC_U);
                // tap_code(KC_P2);
                // tap_code(KC_P0);
                // tap_code(KC_P2);
                // tap_code(KC_P6);
                // unregister_code(KC_U);
                SEND_STRING("…");
            }
            return false;
        case DASH:
            if (record->event.pressed) {
                // register_code(KC_U);
                // tap_code(KC_P2);
                // tap_code(KC_P0);
                // tap_code(KC_P1);
                // tap_code(KC_P4);
                // unregister_code(KC_U);
                SEND_STRING("—");
            }
            return false;
        case CARD:
            if (record->event.pressed) {
                SEND_STRING("♣♥♠♦");
            }
            return false;
        case ITLC:
            if (record->event.pressed) {
                register_code(KC_RCTL);
                tap_code(KC_I);
                unregister_code(KC_RCTL);
            }
            return false;
        case LYR_ENT:
            static uint16_t key_timer;
            if (record->event.pressed) {
                key_timer = timer_read();
            } else {
                if (timer_elapsed(key_timer) < TAPPING_TERM) {
                    tap_code(KC_ENTER);
                } else {
                    if (layer_state_is(NumPad)) {
                        layer_off(NumPad);
                        layer_on(Fusion);
                        display_num(fusion);
                    }
                    if (layer_state_is(Fusion)) {
                        layer_off(Fusion);
                        layer_on(Gen);
                        display_num(generalshortcuts);
                    }
                }
            }
            return false;
        case LYR_BLD:
            // static uint16_t key_timer;
            if (record->event.pressed) {
                key_timer = timer_read();
            } else {
                if (timer_elapsed(key_timer) < TAPPING_TERM) {
                    if (record->event.pressed) {
                        register_code(KC_RCTL);
                        tap_code(KC_B);
                        unregister_code(KC_RCTL);
                    }
                    return false;
                } else {
                    layer_off(Gen);
                    layer_on(NumPad);
                    display_num(numberpad);
                }
            }
            return false;
        case LYR_EXT:
            if (record->event.pressed) {
                tap_code(KC_E);
                layer_off(Sketch);
                layer_on(NumPad);
                display_num(numberpad);
            }
            return false;
        case ESC_SKCH:
            if (record->event.pressed) {
                tap_code(KC_ESC);
                layer_off(NumPad);
                layer_on(Sketch);
                display_num(sketch);
            }
            return false;
        case CHMF:
            if (record->event.pressed) {
                tap_code(KC_S);
                SEND_STRING("CHAMFER");
                tap_code(KC_ENTER);
            }
            return false;
        case DIMENSION:
            if (record->event.pressed) {
                tap_code(KC_D);
                layer_off(Sketch);
                layer_on(NumPad);
                display_num(numberpad);
            }
            return false;
        case EXTRUDE:
            if (record->event.pressed) {
                tap_code(KC_E);
                layer_off(Fusion);
                layer_on(NumPad);
                display_num(numberpad);
            }
            return false;
    }
    return true;
}
