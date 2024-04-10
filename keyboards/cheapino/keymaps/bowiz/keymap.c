#include <stdint.h>
#include "action.h"
#include "action_layer.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "print.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "rgblight.h"
#include "send_string_keycodes.h"
#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"
#include "sentence_case.h"
#include "cheapino.h"

// tasks left: fix rotary, shift tab (if shift is down - is swapper ignored key)

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_RSHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OS_MEH,

    SW_WIN,  // Switch to next window         (cmd-tab)
    SW_LANG, // Switch to next input language (ctl-spc)
    SW_STAB, // Switch to previous window
    SW_LAY, // Toggle layout
    SW_OS, // Toggle os
    NXT_TAB,
    PRV_TAB,
    CL_TAB,
    COPY,
    CUT,
    PASTE,
    UNDO,
    REDO,
    FIND,
    SPTLT,
    B_LNE,
    F_LNE,
    B_WRD,
    F_WRD,
    D_WRD
};

enum layers {
    CLMK,
    QWTY,
    NAV,
    SYM,
    NUM,
    LMO,
};

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TAB_L G(S(KC_LBRC))
#define TAB_R G(S(KC_RBRC))
#define SPACE_L A(G(KC_LEFT))
#define SPACE_R A(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define NXT_TAB LCTL(KC_TAB)
#define PRV_TAB RCS(KC_TAB)
#define S_TAB S(KC_TAB)
#define LINE_MOD MO(LMO)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[CLMK] = LAYOUT_split_3x5_3(
		KC_Q, KC_W, KC_F, KC_P, KC_B, 		KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
		KC_A, KC_R, KC_S, KC_T, KC_G, 		KC_M, KC_N, KC_E, KC_I, KC_O, 
		KC_Z, KC_X, KC_C, KC_D, KC_V, 		KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH, 
	        KC_SPC, LA_NAV, KC_ESC, 		KC_ENT, LA_SYM, KC_BSPC),

    [QWTY] = LAYOUT_split_3x5_3(
		KC_Q, KC_W, KC_E, KC_R, KC_T, 		KC_Y, KC_U, KC_I, KC_O, KC_P,
		KC_A, KC_S, KC_D, KC_F, KC_G, 		KC_H, KC_J, KC_K, KC_L, KC_QUOT, 
		KC_Z, KC_X, KC_C, KC_V, KC_B, 		KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, 
	        KC_SPC, LA_NAV, KC_ESC, 		KC_ENT, LA_SYM, KC_BSPC),

	[NAV] = LAYOUT_split_3x5_3(
	    SW_WIN, CL_TAB, KC_NO, KC_DEL, CW_TOGG, 			KC_HOME, KC_END, PRV_TAB, NXT_TAB, KC_WH_U, 
		OS_CMD, OS_ALT, OS_CTRL, OS_SHFT, LINE_MOD, 		KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_WH_D, 
		UNDO, CUT, COPY, REDO, PASTE, 	                FIND, KC_TAB, SPTLT, OS_MEH, KC_NO,
							KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  
	[SYM] = LAYOUT_split_3x5_3(
		KC_LCBR,KC_RCBR, KC_LBRC, KC_RBRC,KC_NO, 		KC_NO, KC_PERC, KC_GRAVE, KC_CIRC, KC_NO, 
		KC_HASH, KC_AMPR, KC_MINS, KC_UNDS, KC_PIPE, 		KC_PLUS, KC_QUOTE, KC_DOUBLE_QUOTE, KC_EQUAL, KC_SCLN, 
		KC_DLR, KC_TILD, KC_LPRN, KC_RPRN, KC_AT, 			KC_BSLS, KC_ASTR, KC_LT, KC_GT, KC_EXLM, 
						KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	[NUM] = LAYOUT_split_3x5_3(
		SW_STAB, KC_F2, KC_F3, KC_F4, KC_F5, 		KC_F6, KC_F7, KC_F8, QK_BOOT, KC_F10, 
		KC_1, KC_2, KC_3, KC_4, KC_5, 			KC_6, KC_7, KC_8, KC_9, KC_0, 
		SW_OS, SW_LANG, SW_LAY, KC_F1, KC_F9, 		KC_F11, KC_F12, KC_NO , KC_DOT, EE_CLR, 
						KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	
    [LMO] = LAYOUT_split_3x5_3(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 			B_LNE, B_WRD, F_WRD, F_LNE, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 		KC_TRNS, D_WRD, KC_TRNS , KC_TRNS, KC_TRNS, 
						KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case KC_ESCAPE:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_RSHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
    case OS_MEH:
        return true;
    default:
        return false;
    }
}
bool sw_win_active = false;
//bool sw_swin_active = false;
bool sw_lang_active = false;
oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_rshft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_meh_state = os_up_unqueued;

//bool mac_mode = false;
void send_macmode_key(bool mac_mode, uint16_t win_kc, uint16_t mac_kc, keyrecord_t *record) {
    if (!record->event.pressed) {
        if (mac_mode) {
            tap_code16(mac_kc);
        } else {
            tap_code16(win_kc);
        }
    } 
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_sentence_case(keycode, record)) { return false; }
    if (mac_mode) {
        update_swapper(
            &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
            keycode, record
        );
//        update_swapper(
//            &sw_swin_active, KC_LGUI, S_TAB, SW_STAB,
//            keycode, record
//        );
        update_swapper(
            &sw_lang_active, KC_LCTL, KC_SPACE, SW_LANG,
            keycode, record
        );
    } else {
        update_swapper(
            &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
            keycode, record
        );
//        update_swapper(
//          &sw_swin_active, KC_LALT, S_TAB, SW_WIN,
//          keycode, record
//        );
        update_swapper(
            &sw_lang_active, KC_LALT, KC_LSFT, SW_LANG,
            keycode, record
        );
    }
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record, mac_mode
    );
    update_oneshot(
        &os_rshft_state, KC_RSFT, OS_RSHFT,
        keycode, record, mac_mode
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record, mac_mode
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record, mac_mode
    );
    update_oneshot(
        &os_cmd_state, KC_LGUI, OS_CMD,
        keycode, record, mac_mode
    );
    switch (keycode) {
        case SW_LANG:
            if (!record->event.pressed) {
                layer_invert(QWTY);
                layer_invert(CLMK);
                }
                break;
        case SW_LAY:
            if (!record->event.pressed) {
                layer_invert(QWTY);
                layer_invert(CLMK);
                }
                break;
        case SW_OS:
            if (!record->event.pressed) {
                mac_mode = !mac_mode;
                }
                break;
        case CL_TAB:
            send_macmode_key(mac_mode, C(KC_W), G(KC_W), record);
            break;
        case COPY:
            send_macmode_key(mac_mode, C(KC_C), G(KC_C), record);
            break;
        case CUT:
            send_macmode_key(mac_mode, C(KC_X), G(KC_X), record);
            break;
        case PASTE:
            send_macmode_key(mac_mode, C(KC_V), G(KC_V), record);
            break;
        case UNDO:
            send_macmode_key(mac_mode, C(KC_Z), G(KC_Z), record);
            break;
        case REDO:
            send_macmode_key(mac_mode, C(S(KC_Z)), G(S(KC_Z)), record);
            break;
        case FIND:
            send_macmode_key(mac_mode, C(KC_F), G(KC_F), record);
            break;
        case SPTLT:
            send_macmode_key(mac_mode, KC_LEFT_GUI, G(KC_SPACE), record);
            break;
        case B_LNE:
            send_macmode_key(mac_mode, KC_HOME, C(KC_A), record);
            break;
        case F_LNE:
            send_macmode_key(mac_mode, KC_END, C(KC_E), record);
            break;
        case B_WRD:
            send_macmode_key(mac_mode, C(KC_LEFT), A(KC_LEFT), record);
            break;
        case F_WRD:
            send_macmode_key(mac_mode, C(KC_RIGHT) , A(KC_RIGHT) , record);
            break;
        case D_WRD:
            send_macmode_key(mac_mode, C(KC_BACKSPACE), A(KC_BACKSPACE), record);
            break;
        }
        // do a bunch of small shortcut macro swaps here for windows/mac
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (IS_LAYER_ON_STATE(state, CLMK)) {
         rgblight_sethsv(169, 255, 10);
    }
    if (IS_LAYER_ON_STATE(state, QWTY)) {
         rgblight_sethsv(43, 255, 10);
    }
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
