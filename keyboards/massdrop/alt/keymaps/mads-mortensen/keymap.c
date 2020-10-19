#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, \
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN, \
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [1] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, KC_PSCR, KC_SLCK, KC_PAUS, _______, KC_END, \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______,          _______, KC_VOLU, \
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, DBG_TOG, _______, _______, _______, _______,          KC_PGUP, KC_VOLD, \
        _______, _______, _______,                            _______,                            _______, _______, KC_HOME, KC_PGDN, KC_END  \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    */
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

void matrix_init_user(void) {
    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT);
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 0:
        rgb_matrix_set_color_all(0, 0, 0);
        break;
    case 1:
        break;
    default:
        break;
    }
  return state;
}

void rgb_matrix_indicators_user(void) {
    int max = 105;

    led_flags_t flags = rgb_matrix_get_flags();

    // If we're in either keylight or underglow modes (but not both simultaneously)
    if (HAS_FLAGS(flags, LED_FLAG_KEYLIGHT) != HAS_FLAGS(flags, LED_FLAG_UNDERGLOW)) {

        // This fixes a bug where the caps lock LED flickers when toggled in either keylight or underglow modes
        if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {
            rgb_matrix_set_color(30, RGB_WHITE); // capslock
        } else {
            rgb_matrix_set_color(30, 0, 0, 0); // capslock
        }
    }

    switch (get_highest_layer(layer_state)) {
        case 0:
            break;
        case 1:
            for (int i = 15; i < max; i++) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
            rgb_matrix_set_color(16, 20, 0, 20); // Q: slower!
            rgb_matrix_set_color(17, 20, 80, 50); // W: value up
            rgb_matrix_set_color(18, 20, 0, 20); // E: faster!
            rgb_matrix_set_color(19, 60, 60, 90); // R: hue up
            rgb_matrix_set_color(20, 80, 50, 20); // T: satuation up
            rgb_matrix_set_color(31, 50, 100, 0); // A: prev mode
            rgb_matrix_set_color(32, 20, 80, 50); // S: value down
            rgb_matrix_set_color(33, 50, 100, 0); // D: next mode
            rgb_matrix_set_color(34, 60, 60, 90); // F: hue down
            rgb_matrix_set_color(35, 80, 50, 20); // G: satuation down

            rgb_matrix_set_color(49, 100, 0, 0); // B: reboot

            rgb_matrix_set_color(67 + rgb_matrix_get_mode(), 50, 100, 0); // bottom glow, current mode

            // value (led brightness) indicator
            // 17 steps, value changes by 15 every step
            // max = 255
            int cur_val = rgb_matrix_get_val();
            for (int j = 0; j < 6; j++) {
                int step = cur_val/15;
                if (j < step) {
                    if (j == 5 && cur_val == 255) {
                        rgb_matrix_set_color(75 + j, 80, 80, 80);
                    } else {
                        rgb_matrix_set_color(75 + j, 20, 80, 50);
                    }
                }
            }

            // LED toggle indicator
            switch (flags) {
                case LED_FLAG_ALL:
                    rgb_matrix_set_color(45, 0, 100, 0);
                    rgb_matrix_set_color(46, 0, 100, 0);
                    break;
                case LED_FLAG_KEYLIGHT:
                    rgb_matrix_set_color(45, 0, 100, 0);
                    rgb_matrix_set_color(46, 0, 30, 0);
                    break;
                case LED_FLAG_UNDERGLOW:
                    rgb_matrix_set_color(45, 0, 30, 0);
                    rgb_matrix_set_color(46, 0, 100, 0);
                    break;
                default:
                    rgb_matrix_set_color(45, 0, 30, 0);
                    rgb_matrix_set_color(46, 0, 30, 0);
                    break;
            }
        }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_KEYLIGHT: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    //rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}
