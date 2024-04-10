#include "oneshot.h"
#include <stdint.h>
#include <sys/types.h>
#include "keycodes.h"

u_int16_t mac_swap(uint16_t mod, bool mac_mode) {
    if (mac_mode) {
        switch (mod) {
        case KC_LCTL:
            return KC_LGUI;
            break;
        case KC_LGUI:
            return KC_LALT;
            break;
        case KC_LALT:
            return KC_LCTL;
            break;
        }
    }
    return mod;
}

uint16_t queued_oneshot_keycode_to_mod = 0;

void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record,
    bool mac_mode
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                register_code(mac_swap(mod, mac_mode));
            }
            *state = os_down_unused;
        } else {
            // Trigger keyup
            switch (*state) {
            case os_down_unused:
                // If we didn't use the mod while trigger was held, queue it.
                *state = os_up_queued;
                break;
            case os_down_used:
                // If we did use the mod while trigger was held, unregister it.
                *state = os_up_unqueued;
                queued_oneshot_keycode_to_mod = 0;
                unregister_code(mac_swap(mod, mac_mode));
                break;
            default:
                break;
            }
        }
    } else {
        if (record->event.pressed) {
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                // Cancel oneshot on designated cancel keydown.
                *state = os_up_unqueued;
                queued_oneshot_keycode_to_mod = 0;
                unregister_code(mac_swap(mod, mac_mode));
            } else {
            if (!is_oneshot_ignored_key(keycode) && *state == os_up_queued) {
                // On non-ignored keydown when queued, save the keycode as the only key to mod.
                if (queued_oneshot_keycode_to_mod == 0) {
                    queued_oneshot_keycode_to_mod = keycode;
                } else if (keycode != queued_oneshot_keycode_to_mod) {
                    // Cancel the oneshot on any subsequent non ignored keydown, since we only want the first key modded. 
                    *state = os_up_unqueued;
                    queued_oneshot_keycode_to_mod = 0;
                    unregister_code(mac_swap(mod, mac_mode));
                }
                }
            }
        } else {
            if (!is_oneshot_ignored_key(keycode)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (*state) {
                case os_down_unused:
                    *state = os_down_used;
                    break;
                case os_up_queued:
                    *state = os_up_unqueued;
                    queued_oneshot_keycode_to_mod = 0;
                    unregister_code(mac_swap(mod, mac_mode));
                    break;
                default:
                    break;
                }
            }
        }
    }
}
