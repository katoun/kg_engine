/*
 * keynames.c : Keyboard name definitions
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

// Includes
#include "config.h"
#include "openinput.h"
#include "internal.h"

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Fill keyboard name table
 *
 * @param kn pointer to array to be filled with button name strings
 * @returns errorcode, see @ref PErrors
 *
 * Fill the table (pointed to by "kn") with keyboard
 * string names. This is used in both oi_key_getname and
 * oi_key_getcode to translate between stings and keycodes.
 *
 * And yup - this function is lengthy but quite trivial!
 */
int keyboard_fillnames(char **kn) {
    int i;

    // Fill all with the unknown key
    for(i=OIK_FIRST; i<OIK_LAST; i++) {
        kn[i] = "key_unknown";
    }

    // Regulars
    kn[OIK_BACKSPACE] =    "key_backspace";
    kn[OIK_TAB] =          "key_tab";
    kn[OIK_CLEAR] =        "key_clear";
    kn[OIK_RETURN] =       "key_return";
    kn[OIK_PAUSE] =        "key_pause";
    kn[OIK_ESC] =          "key_escape";
    kn[OIK_SPACE] =        "key_space";
    kn[OIK_EXCLAIM] =      "key_exclaim";
    kn[OIK_QUOTEDBL] =     "key_dblquote";
    kn[OIK_HASH] =         "key_hash";
    kn[OIK_DOLLAR] =       "key_dollar";
    kn[OIK_AMPERSAND] =    "key_and";
    kn[OIK_QUOTE] =        "key_quote";
    kn[OIK_LEFTPAREN] =    "key_leftparan";
    kn[OIK_RIGHTPAREN] =   "key_rightparan";
    kn[OIK_ASTERISK] =     "key_asterisk";
    kn[OIK_PLUS] =         "key_plus";
    kn[OIK_COMMA] =        "key_comma";
    kn[OIK_MINUS] =        "key_minus";
    kn[OIK_PERIOD] =       "key_period";
    kn[OIK_SLASH] =        "key_slash";
    kn[OIK_0] =            "key_0";
    kn[OIK_1] =            "key_1";
    kn[OIK_2] =            "key_2";
    kn[OIK_3] =            "key_3";
    kn[OIK_4] =            "key_4";
    kn[OIK_5] =            "key_5";
    kn[OIK_6] =            "key_6";
    kn[OIK_7] =            "key_7";
    kn[OIK_8] =            "key_8";
    kn[OIK_9] =            "key_9";
    kn[OIK_COLON] =        "key_colon";
    kn[OIK_SEMICOLON] =    "key_semicolon";
    kn[OIK_LESS] =         "key_less";
    kn[OIK_EQUALS] =       "key_equal";
    kn[OIK_GREATER] =      "key_greater";
    kn[OIK_QUESTION] =     "key_question";
    kn[OIK_AT] =           "key_at";
    kn[OIK_LEFTBRACKET] =  "key_leftbracket";
    kn[OIK_BACKSLASH] =    "key_backslash";
    kn[OIK_RIGHTBRACKET] = "key_rightbracket";
    kn[OIK_CARET] =        "key_caret";
    kn[OIK_UNDERSCORE] =   "key_underscore";
    kn[OIK_BACKQUOTE] =    "key_backquote";
    kn[OIK_A] =            "key_a";
    kn[OIK_B] =            "key_b";
    kn[OIK_C] =            "key_c";
    kn[OIK_D] =            "key_d";
    kn[OIK_E] =            "key_e";
    kn[OIK_F] =            "key_f";
    kn[OIK_G] =            "key_g";
    kn[OIK_H] =            "key_h";
    kn[OIK_I] =            "key_i";
    kn[OIK_J] =            "key_j";
    kn[OIK_K] =            "key_k";
    kn[OIK_L] =            "key_l";
    kn[OIK_M] =            "key_m";
    kn[OIK_N] =            "key_n";
    kn[OIK_O] =            "key_o";
    kn[OIK_P] =            "key_p";
    kn[OIK_Q] =            "key_q";
    kn[OIK_R] =            "key_r";
    kn[OIK_S] =            "key_s";
    kn[OIK_T] =            "key_t";
    kn[OIK_U] =            "key_u";
    kn[OIK_V] =            "key_v";
    kn[OIK_W] =            "key_w";
    kn[OIK_X] =            "key_x";
    kn[OIK_Y] =            "key_y";
    kn[OIK_Z] =            "key_z";
    kn[OIK_DELETE] =       "key_delete";

    // International
    kn[OIK_INT_0] =        "key_int0";
    kn[OIK_INT_1] =        "key_int1";
    kn[OIK_INT_2] =        "key_int2";
    kn[OIK_INT_3] =        "key_int3";
    kn[OIK_INT_4] =        "key_int4";
    kn[OIK_INT_5] =        "key_int5";
    kn[OIK_INT_6] =        "key_int6";
    kn[OIK_INT_7] =        "key_int7";
    kn[OIK_INT_8] =        "key_int8";
    kn[OIK_INT_9] =        "key_int9";
    kn[OIK_INT_10] =       "key_int10";
    kn[OIK_INT_11] =       "key_int11";
    kn[OIK_INT_12] =       "key_int12";
    kn[OIK_INT_13] =       "key_int13";
    kn[OIK_INT_14] =       "key_int14";
    kn[OIK_INT_15] =       "key_int15";
    kn[OIK_INT_16] =       "key_int16";
    kn[OIK_INT_17] =       "key_int17";
    kn[OIK_INT_18] =       "key_int18";
    kn[OIK_INT_19] =       "key_int19";
    kn[OIK_INT_20] =       "key_int20";
    kn[OIK_INT_21] =       "key_int21";
    kn[OIK_INT_22] =       "key_int22";
    kn[OIK_INT_23] =       "key_int23";
    kn[OIK_INT_24] =       "key_int24";
    kn[OIK_INT_25] =       "key_int25";
    kn[OIK_INT_26] =       "key_int26";
    kn[OIK_INT_27] =       "key_int27";
    kn[OIK_INT_28] =       "key_int28";
    kn[OIK_INT_29] =       "key_int29";
    kn[OIK_INT_30] =       "key_int30";
    kn[OIK_INT_31] =       "key_int31";
    kn[OIK_INT_32] =       "key_int32";
    kn[OIK_INT_33] =       "key_int33";
    kn[OIK_INT_34] =       "key_int34";
    kn[OIK_INT_35] =       "key_int35";
    kn[OIK_INT_36] =       "key_int36";
    kn[OIK_INT_37] =       "key_int37";
    kn[OIK_INT_38] =       "key_int38";
    kn[OIK_INT_39] =       "key_int39";
    kn[OIK_INT_40] =       "key_int40";
    kn[OIK_INT_41] =       "key_int41";
    kn[OIK_INT_42] =       "key_int42";
    kn[OIK_INT_43] =       "key_int43";
    kn[OIK_INT_44] =       "key_int44";
    kn[OIK_INT_45] =       "key_int45";
    kn[OIK_INT_46] =       "key_int46";
    kn[OIK_INT_47] =       "key_int47";
    kn[OIK_INT_48] =       "key_int48";
    kn[OIK_INT_49] =       "key_int49";
    kn[OIK_INT_50] =       "key_int50";
    kn[OIK_INT_51] =       "key_int51";
    kn[OIK_INT_52] =       "key_int52";
    kn[OIK_INT_53] =       "key_int53";
    kn[OIK_INT_54] =       "key_int54";
    kn[OIK_INT_55] =       "key_int55";
    kn[OIK_INT_56] =       "key_int56";
    kn[OIK_INT_57] =       "key_int57";
    kn[OIK_INT_58] =       "key_int58";
    kn[OIK_INT_59] =       "key_int59";
    kn[OIK_INT_60] =       "key_int60";
    kn[OIK_INT_61] =       "key_int61";
    kn[OIK_INT_62] =       "key_int62";
    kn[OIK_INT_63] =       "key_int63";
    kn[OIK_INT_64] =       "key_int64";
    kn[OIK_INT_65] =       "key_int65";
    kn[OIK_INT_66] =       "key_int66";
    kn[OIK_INT_67] =       "key_int67";
    kn[OIK_INT_68] =       "key_int68";
    kn[OIK_INT_69] =       "key_int69";
    kn[OIK_INT_70] =       "key_int70";
    kn[OIK_INT_71] =       "key_int71";
    kn[OIK_INT_72] =       "key_int72";
    kn[OIK_INT_73] =       "key_int73";
    kn[OIK_INT_74] =       "key_int74";
    kn[OIK_INT_75] =       "key_int75";
    kn[OIK_INT_76] =       "key_int76";
    kn[OIK_INT_77] =       "key_int77";
    kn[OIK_INT_78] =       "key_int78";
    kn[OIK_INT_79] =       "key_int79";
    kn[OIK_INT_80] =       "key_int80";
    kn[OIK_INT_81] =       "key_int81";
    kn[OIK_INT_82] =       "key_int82";
    kn[OIK_INT_83] =       "key_int83";
    kn[OIK_INT_84] =       "key_int84";
    kn[OIK_INT_85] =       "key_int85";
    kn[OIK_INT_86] =       "key_int86";
    kn[OIK_INT_87] =       "key_int87";
    kn[OIK_INT_88] =       "key_int88";
    kn[OIK_INT_89] =       "key_int89";
    kn[OIK_INT_90] =       "key_int90";
    kn[OIK_INT_91] =       "key_int91";
    kn[OIK_INT_92] =       "key_int92";
    kn[OIK_INT_93] =       "key_int93";
    kn[OIK_INT_94] =       "key_int94";
    kn[OIK_INT_95] =       "key_int95";

    // Numeric keypad
    kn[OIK_N_0] =          "key_num_0";
    kn[OIK_N_1] =          "key_num_1";
    kn[OIK_N_2] =          "key_num_2";
    kn[OIK_N_3] =          "key_num_3";
    kn[OIK_N_4] =          "key_num_4";
    kn[OIK_N_5] =          "key_num_5";
    kn[OIK_N_6] =          "key_num_6";
    kn[OIK_N_7] =          "key_num_7";
    kn[OIK_N_8] =          "key_num_8";
    kn[OIK_N_9] =          "key_num_9";
    kn[OIK_N_PERIOD] =     "key_num_period";
    kn[OIK_N_DIVIDE] =     "key_num_divide";
    kn[OIK_N_MULTIPLY] =   "key_num_multiply";
    kn[OIK_N_MINUS] =      "key_num_minus";
    kn[OIK_N_PLUS] =       "key_num_plus";
    kn[OIK_N_ENTER] =      "key_num_enter";
    kn[OIK_N_EQUALS] =     "key_num_equals";

    // Arrows + home/end pad
    kn[OIK_UP] =           "key_up";
    kn[OIK_DOWN] =         "key_down";
    kn[OIK_RIGHT] =        "key_right";
    kn[OIK_LEFT] =         "key_left";
    kn[OIK_INSERT] =       "key_insert";
    kn[OIK_HOME] =         "key_home";
    kn[OIK_END] =          "key_end";
    kn[OIK_PAGEUP] =       "key_pageup";
    kn[OIK_PAGEDOWN] =     "key_pagedown";

    // Function keys
    kn[OIK_F1] =           "key_f1";
    kn[OIK_F2] =           "key_f2";
    kn[OIK_F3] =           "key_f3";
    kn[OIK_F4] =           "key_f4";
    kn[OIK_F5] =           "key_f5";
    kn[OIK_F6] =           "key_f6";
    kn[OIK_F7] =           "key_f7";
    kn[OIK_F8] =           "key_f8";
    kn[OIK_F9] =           "key_f9";
    kn[OIK_F10] =          "key_f10";
    kn[OIK_F11] =          "key_f11";
    kn[OIK_F12] =          "key_f12";
    kn[OIK_F13] =          "key_f13";
    kn[OIK_F14] =          "key_f14";
    kn[OIK_F15] =          "key_f15";

    // Key state modifiers
    kn[OIK_NUMLOCK] =      "key_numlock";
    kn[OIK_CAPSLOCK] =     "key_capslock";
    kn[OIK_SCROLLOCK] =    "key_scrolllock";
    kn[OIK_RSHIFT] =       "key_shift_right";
    kn[OIK_LSHIFT] =       "key_shift_left";
    kn[OIK_RCTRL] =        "key_control_right";
    kn[OIK_LCTRL] =        "key_control_left";
    kn[OIK_RALT] =         "key_alt_right";
    kn[OIK_LALT] =         "key_alt_left";
    kn[OIK_RMETA] =        "key_meta_right";
    kn[OIK_LMETA] =        "key_meta_left";
    kn[OIK_RWINDOWS] =     "key_windows_right";
    kn[OIK_LWINDOWS] =     "key_windows_left";
    kn[OIK_ALTGR] =        "key_altgr";
    kn[OIK_COMPOSE] =      "key_compose";

    // Miscellaneous function keys
    kn[OIK_HELP] =         "key_help";
    kn[OIK_PRINT] =        "key_print";
    kn[OIK_SYSREQ] =       "key_sysrq";
    kn[OIK_BREAK] =        "key_break";
    kn[OIK_MENU] =         "key_menu";
    kn[OIK_POWER] =        "key_power";
    kn[OIK_EURO] =         "key_euro";
    kn[OIK_UNDO] =         "key_undo";

    return OI_ERR_OK;
}

/* ******************************************************************** */
