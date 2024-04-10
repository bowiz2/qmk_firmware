/*
Copyright 2020 Pierre Chevalier <pierrechevalier83@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Pick good defaults for enabling homerow modifiers
#undef TAPPING_TERM
#define TAPPING_TERM 100
//// #define PERMISSIVE_HOLD

//#define IGNORE_MOD_TAP_INTERRUPT
//#define TAPPING_FORCE_HOLD

////#define RETRO_TAPPING

// default rgb settings
#define RGBLIGHT_DEFAULT_HUE 169
#define RGBLIGHT_DEFAULT_VAL 10
#define RGBLIGHT_SLEEP  // enable rgblight_suspend() and rgblight_wakeup() in keymap.c
#define RGBLIGHT_TIMEOUT 300000  // ms to wait until rgblight time out, 300K ms is 5min.

//#define RGBLIGHT_LAYERS

//#define RGBLIGHT_MAX_LAYERS 8
//#define RGBLIGHT_LAYERS_RETAIN_VAL 
//#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF

//#define UNICODE_SELECTED_MODES UC_MAC
