/*
 * openinput_keys.h : Basic key and modifier definitions
 *
 * This file is a part of the OpenInput library.
 * Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ******************************************************************** */

#ifndef _OPENINPUT_KEYS_H_
#define _OPENINPUT_KEYS_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @defgroup PKeyname Keyboard button definitons
 * @brief Definition of keyboard button names.
 * @ingroup PTypes
 *
 * The lengthy definition of keyboard buttons
 * for use in your event handler. Each key also have a
 * "symbolic" string name.
 *
 * @{
 */
typedef enum {
    OIK_UNKNOWN         = 0,   /**< key_unknown */
    OIK_FIRST           = 0,   /**< key_unknown */

    // Ascii-keys
    OIK_BACKSPACE       = 8,   /**< key_backspace */
    OIK_TAB             = 9,   /**< key_tab */
    OIK_CLEAR           = 12,  /**< key_clear */
    OIK_RETURN          = 13,  /**< key_return */
    OIK_PAUSE           = 19,  /**< key_pause */
    OIK_ESC             = 27,  /**< key_escape */
    OIK_SPACE           = 32,  /**< key_space */
    OIK_EXCLAIM         = 33,  /**< key_exclaim*/
    OIK_QUOTEDBL        = 34,  /**< key_dblquote */
    OIK_HASH            = 35,  /**< key_hash */
    OIK_DOLLAR          = 36,  /**< key_dollar */
    OIK_AMPERSAND       = 38,  /**< key_and */
    OIK_QUOTE           = 39,  /**< key_quote */
    OIK_LEFTPAREN       = 40,  /**< key_leftparan */
    OIK_RIGHTPAREN      = 41,  /**< key_rightparan */
    OIK_ASTERISK        = 42,  /**< key_asterisk */
    OIK_PLUS            = 43,  /**< key_plus */
    OIK_COMMA           = 44,  /**< key_comma */
    OIK_MINUS           = 45,  /**< key_minus */
    OIK_PERIOD          = 46,  /**< key_period */
    OIK_SLASH           = 47,  /**< key_slash */
    OIK_0               = 48,  /**< key_0 */
    OIK_1               = 49,  /**< key_1 */
    OIK_2               = 50,  /**< key_2 */
    OIK_3               = 51,  /**< key_3 */
    OIK_4               = 52,  /**< key_4 */
    OIK_5               = 53,  /**< key_5 */
    OIK_6               = 54,  /**< key_6 */
    OIK_7               = 55,  /**< key_7 */
    OIK_8               = 56,  /**< key_8 */
    OIK_9               = 57,  /**< key_9 */
    OIK_COLON           = 58,  /**< key_colon */
    OIK_SEMICOLON       = 59,  /**< key_semicolon */
    OIK_LESS            = 60,  /**< key_less */
    OIK_EQUALS          = 61,  /**< key_equal */
    OIK_GREATER         = 62,  /**< key_greater */
    OIK_QUESTION        = 63,  /**< key_question */
    OIK_AT              = 64,  /**< key_at */
    // Here we skip uppercase letters
    OIK_LEFTBRACKET     = 91,  /**< key_leftbracket */
    OIK_BACKSLASH       = 92,  /**< key_backslash */
    OIK_RIGHTBRACKET    = 93,  /**< key_rightbracket */
    OIK_CARET           = 94,  /**< key_caret */
    OIK_UNDERSCORE      = 95,  /**< key_underscore */
    OIK_BACKQUOTE       = 96,  /**< key_backquote */
    OIK_A               = 97,  /**< key_a */
    OIK_B               = 98,  /**< key_b */
    OIK_C               = 99,  /**< key_c */
    OIK_D               = 100, /**< key_d */
    OIK_E               = 101, /**< key_e */
    OIK_F               = 102, /**< key_f */
    OIK_G               = 103, /**< key_g */
    OIK_H               = 104, /**< key_h */
    OIK_I               = 105, /**< key_i */
    OIK_J               = 106, /**< key_j */
    OIK_K               = 107, /**< key_k */
    OIK_L               = 108, /**< key_l */
    OIK_M               = 109, /**< key_m */
    OIK_N               = 110, /**< key_n */
    OIK_O               = 111, /**< key_o */
    OIK_P               = 112, /**< key_p */
    OIK_Q               = 113, /**< key_q */
    OIK_R               = 114, /**< key_r */
    OIK_S               = 115, /**< key_s */
    OIK_T               = 116, /**< key_t */
    OIK_U               = 117, /**< key_u */
    OIK_V               = 118, /**< key_v */
    OIK_W               = 119, /**< key_w */
    OIK_X               = 120, /**< key_x */
    OIK_Y               = 121, /**< key_y */
    OIK_Z               = 122, /**< key_z */
    OIK_DELETE          = 127, /**< key_delete */

    // International character (no more ascii)
    OIK_INT_0           = 160, /**< key_int0 */ // 0xA0
    OIK_INT_1           = 161, /**< key_int1 */
    OIK_INT_2           = 162, /**< key_int2 */
    OIK_INT_3           = 163, /**< key_int3 */
    OIK_INT_4           = 164, /**< key_int4 */
    OIK_INT_5           = 165, /**< key_int5 */
    OIK_INT_6           = 166, /**< key_int6 */
    OIK_INT_7           = 167, /**< key_int7 */
    OIK_INT_8           = 168, /**< key_int8 */
    OIK_INT_9           = 169, /**< key_int9 */
    OIK_INT_10          = 170, /**< key_int10 */
    OIK_INT_11          = 171, /**< key_int11 */
    OIK_INT_12          = 172, /**< key_int12 */
    OIK_INT_13          = 173, /**< key_int13 */
    OIK_INT_14          = 174, /**< key_int14 */
    OIK_INT_15          = 175, /**< key_int15 */
    OIK_INT_16          = 176, /**< key_int16 */
    OIK_INT_17          = 177, /**< key_int17 */
    OIK_INT_18          = 178, /**< key_int18 */
    OIK_INT_19          = 179, /**< key_int19 */
    OIK_INT_20          = 180, /**< key_int20 */
    OIK_INT_21          = 181, /**< key_int21 */
    OIK_INT_22          = 182, /**< key_int22 */
    OIK_INT_23          = 183, /**< key_int23 */
    OIK_INT_24          = 184, /**< key_int24 */
    OIK_INT_25          = 185, /**< key_int25 */
    OIK_INT_26          = 186, /**< key_int26 */
    OIK_INT_27          = 187, /**< key_int27 */
    OIK_INT_28          = 188, /**< key_int28 */
    OIK_INT_29          = 189, /**< key_int29 */
    OIK_INT_30          = 190, /**< key_int30 */
    OIK_INT_31          = 191, /**< key_int31 */
    OIK_INT_32          = 192, /**< key_int32 */
    OIK_INT_33          = 193, /**< key_int33 */
    OIK_INT_34          = 194, /**< key_int34 */
    OIK_INT_35          = 195, /**< key_int35 */
    OIK_INT_36          = 196, /**< key_int36 */
    OIK_INT_37          = 197, /**< key_int37 */
    OIK_INT_38          = 198, /**< key_int38 */
    OIK_INT_39          = 199, /**< key_int39 */
    OIK_INT_40          = 200, /**< key_int40 */
    OIK_INT_41          = 201, /**< key_int41 */
    OIK_INT_42          = 202, /**< key_int42 */
    OIK_INT_43          = 203, /**< key_int43 */
    OIK_INT_44          = 204, /**< key_int44 */
    OIK_INT_45          = 205, /**< key_int45 */
    OIK_INT_46          = 206, /**< key_int46 */
    OIK_INT_47          = 207, /**< key_int47 */
    OIK_INT_48          = 208, /**< key_int48 */
    OIK_INT_49          = 209, /**< key_int49 */
    OIK_INT_50          = 210, /**< key_int50 */
    OIK_INT_51          = 211, /**< key_int51 */
    OIK_INT_52          = 212, /**< key_int52 */
    OIK_INT_53          = 213, /**< key_int53 */
    OIK_INT_54          = 214, /**< key_int54 */
    OIK_INT_55          = 215, /**< key_int55 */
    OIK_INT_56          = 216, /**< key_int56 */
    OIK_INT_57          = 217, /**< key_int57 */
    OIK_INT_58          = 218, /**< key_int58 */
    OIK_INT_59          = 219, /**< key_int59 */
    OIK_INT_60          = 220, /**< key_int60 */
    OIK_INT_61          = 221, /**< key_int61 */
    OIK_INT_62          = 222, /**< key_int62 */
    OIK_INT_63          = 223, /**< key_int63 */
    OIK_INT_64          = 224, /**< key_int64 */
    OIK_INT_65          = 225, /**< key_int65 */
    OIK_INT_66          = 226, /**< key_int66 */
    OIK_INT_67          = 227, /**< key_int67 */
    OIK_INT_68          = 228, /**< key_int68 */
    OIK_INT_69          = 229, /**< key_int69 */
    OIK_INT_70          = 230, /**< key_int70 */
    OIK_INT_71          = 231, /**< key_int71 */
    OIK_INT_72          = 232, /**< key_int72 */
    OIK_INT_73          = 233, /**< key_int73 */
    OIK_INT_74          = 234, /**< key_int74 */
    OIK_INT_75          = 235, /**< key_int75 */
    OIK_INT_76          = 236, /**< key_int76 */
    OIK_INT_77          = 237, /**< key_int77 */
    OIK_INT_78          = 238, /**< key_int78 */
    OIK_INT_79          = 239, /**< key_int79 */
    OIK_INT_80          = 240, /**< key_int80 */
    OIK_INT_81          = 241, /**< key_int81 */
    OIK_INT_82          = 242, /**< key_int82 */
    OIK_INT_83          = 243, /**< key_int83 */
    OIK_INT_84          = 244, /**< key_int84 */
    OIK_INT_85          = 245, /**< key_int85 */
    OIK_INT_86          = 246, /**< key_int86 */
    OIK_INT_87          = 247, /**< key_int87 */
    OIK_INT_88          = 248, /**< key_int88 */
    OIK_INT_89          = 249, /**< key_int89 */
    OIK_INT_90          = 250, /**< key_int90 */
    OIK_INT_91          = 251, /**< key_int91 */
    OIK_INT_92          = 252, /**< key_int92 */
    OIK_INT_93          = 253, /**< key_int93 */
    OIK_INT_94          = 254, /**< key_int94 */
    OIK_INT_95          = 255, /**< key_int95 */ // 0xFF

    // Numeric keypad
    OIK_N_0             = 256, /**< key_num_0 */
    OIK_N_1             = 257, /**< key_num_1 */
    OIK_N_2             = 258, /**< key_num_2 */
    OIK_N_3             = 259, /**< key_num_3 */
    OIK_N_4             = 260, /**< key_num_4 */
    OIK_N_5             = 261, /**< key_num_5 */
    OIK_N_6             = 262, /**< key_num_6 */
    OIK_N_7             = 263, /**< key_num_7 */
    OIK_N_8             = 264, /**< key_num_8 */
    OIK_N_9             = 265, /**< key_num_9 */
    OIK_N_PERIOD        = 266, /**< key_num_period */
    OIK_N_DIVIDE        = 267, /**< key_num_divide */
    OIK_N_MULTIPLY      = 268, /**< key_num_multiply */
    OIK_N_MINUS         = 269, /**< key_num_minus */
    OIK_N_PLUS          = 270, /**< key_num_plus */
    OIK_N_ENTER         = 271, /**< key_num_enter */
    OIK_N_EQUALS        = 272, /**< key_num_equals */

    // Arrows + home/end pad
    OIK_UP              = 273, /**< key_up */
    OIK_DOWN            = 274, /**< key_down */
    OIK_RIGHT           = 275, /**< key_right */
    OIK_LEFT            = 276, /**< key_left */
    OIK_INSERT          = 277, /**< key_insert */
    OIK_HOME            = 278, /**< key_home */
    OIK_END             = 279, /**< key_end */
    OIK_PAGEUP          = 280, /**< key_pageup */
    OIK_PAGEDOWN        = 281, /**< key_pagedown */

    // Function keys
    OIK_F1              = 282, /**< key_f1 */
    OIK_F2              = 283, /**< key_f2 */
    OIK_F3              = 284, /**< key_f3 */
    OIK_F4              = 285, /**< key_f4 */
    OIK_F5              = 286, /**< key_f5 */
    OIK_F6              = 287, /**< key_f6 */
    OIK_F7              = 288, /**< key_f7 */
    OIK_F8              = 289, /**< key_f8 */
    OIK_F9              = 290, /**< key_f9 */
    OIK_F10             = 291, /**< key_f10 */
    OIK_F11             = 292, /**< key_f11 */
    OIK_F12             = 293, /**< key_f12 */
    OIK_F13             = 294, /**< key_f13 */
    OIK_F14             = 295, /**< key_f14 */
    OIK_F15             = 296, /**< key_f15 */

    // Key state modifiers
    OIK_NUMLOCK         = 300, /**< key_numlock */
    OIK_CAPSLOCK        = 301, /**< key_capslock */
    OIK_SCROLLOCK       = 302, /**< key_scrolllock */
    OIK_RSHIFT          = 303, /**< key_shift_right */
    OIK_LSHIFT          = 304, /**< key_shift_left */
    OIK_RCTRL           = 305, /**< key_control_right */
    OIK_LCTRL           = 306, /**< key_control_left */
    OIK_RALT            = 307, /**< key_alt_right */
    OIK_LALT            = 308, /**< key_alt_left */
    OIK_RMETA           = 309, /**< key_meta_right */
    OIK_LMETA           = 310, /**< key_meta_left */
    OIK_RWINDOWS        = 311, /**< key_windows_right */
    OIK_LWINDOWS        = 312, /**< key_windows_left */
    OIK_ALTGR           = 313, /**< key_altgr */
    OIK_COMPOSE         = 314, /**< key_compose */

    // Miscellaneous function keys
    OIK_HELP            = 315, /**< key_help */
    OIK_PRINT           = 316, /**< key_print */
    OIK_SYSREQ          = 317, /**< key_sysrq */
    OIK_BREAK           = 318, /**< key_break */
    OIK_MENU            = 319, /**< key_menu (windows menu key) */
    OIK_POWER           = 320, /**< key_power (mac power key) */
    OIK_EURO            = 321, /**< key_euro */
    OIK_UNDO            = 322, /**< key_undo (atari undo key) */

    OIK_LAST                   /**< key_unknown */
} oi_key;                      /**< Definition of keyboard button names */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PModname Keyboard modifier definitons
 * @brief Keyboard modifier bit definitions
 *
 * Modifiers are reported on keyboard events using
 * a bitmask.
 *
 * @{
 */
#define OIM_NONE         0x0000 /**< Nothing */
#define OIM_LSHIFT       0x0001 /**< Left shift */
#define OIM_RSHIFT       0x0002 /**< Right shift */
#define OIM_LCTRL        0x0040 /**< Left control */
#define OIM_RCTRL        0x0080 /**< Right control */
#define OIM_LALT         0x0100 /**< Left alt */
#define OIM_RALT         0x0200 /**< Right alt */
#define OIM_LMETA        0x0400 /**< Left meta */
#define OIM_RMETA        0x0800 /**< Right meta */
#define OIM_NUMLOCK      0x1000 /**< NumLock */
#define OIM_CAPSLOCK     0x2000 /**< CapsLock */
#define OIM_ALTGR        0x4000 /**< Alt graphics */
#define OIM_SCROLLOCK    0x8000 /**< ScrollLock */
/** @} */


/**
 * @ingroup PKeyboard
 * @brief Keyboard symbol structure
 *
 * Used in keyboard-related events
 */
typedef struct {
    unsigned char scancode;       /**< Hardware scancode */
    oi_key sym;                   /**< Key symbol */
    unsigned int mod;             /**< Modifier symbol */
} oi_keysym;


/* ******************************************************************** */

#endif
