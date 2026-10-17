package com.example.dh2;

/** Android key-code adapter for GameSWF's focused edit-text input. */
final class FrontMenuKeyInputV1 {
    private FrontMenuKeyInputV1() {}

    // Android KeyEvent key codes are stable API constants. Keep this adapter
    // Android-free so the exact mapping can be checked with host javac.
    static int gameSwfCode(int androidCode) {
        if (androidCode >= 29 && androidCode <= 54) return 65 + androidCode - 29;
        if (androidCode >= 7 && androidCode <= 16) return androidCode == 7 ? 48 : 48 + androidCode - 7;
        switch (androidCode) {
            case 55: return 188; // comma
            case 56: return 190; // period
            case 59: case 60: return 16; // either shift
            case 61: return 9; // tab
            case 62: return 32; // space
            case 66: return 13; // enter
            case 67: return 8; // backspace
            case 68: return 192; // grave
            case 69: return 189; // minus
            case 70: return 187; // equals
            case 71: return 219; // left bracket
            case 72: return 221; // right bracket
            case 73: return 220; // backslash
            case 74: return 186; // semicolon
            case 75: return 222; // apostrophe
            case 76: return 191; // slash
            case 92: return 34; // page up
            case 93: return 33; // page down
            case 111: return 27; // Escape
            case 112: return 46; // forward delete
            case 122: return 36; // home
            case 123: return 35; // end
            case 19: return 38; // d-pad up
            case 20: return 40; // d-pad down
            case 21: return 37; // d-pad left
            case 22: return 39; // d-pad right
            default: return -1;
        }
    }

    /** Return the GameSWF base key code for one ASCII character, or -1. */
    static int gameSwfCode(char value) {
        if (value >= 'a' && value <= 'z') return 65 + value - 'a';
        if (value >= 'A' && value <= 'Z') return 65 + value - 'A';
        if (value >= '0' && value <= '9') return value;
        switch (value) {
            case ' ': return 32;
            case ',': case '<': return 188;
            case '.': case '>': return 190;
            case ';': case ':': return 186;
            case '\'': case '"': return 222;
            case '/': case '?': return 191;
            case '`': case '~': return 192;
            case '-': case '_': return 189;
            case '=': case '+': return 187;
            case '[': case '{': return 219;
            case ']': case '}': return 221;
            case '\\': case '|': return 220;
            case '!': return 49;
            case '@': return 50;
            case '#': return 51;
            case '$': return 52;
            case '%': return 53;
            case '^': return 54;
            case '&': return 55;
            case '*': return 56;
            case '(': return 57;
            case ')': return 48;
            default: return -1;
        }
    }

    static boolean requiresShift(char value) {
        return (value >= 'A' && value <= 'Z') || "!@#$%^&*()_+{}|:\"<>?~".indexOf(value) >= 0;
    }

    static boolean shouldForward(boolean originalFrontMenu, boolean nameEntryActive,
                                 int action, int gameSwfCode) {
        return originalFrontMenu && nameEntryActive && gameSwfCode >= 0 &&
                (action == 0 || action == 1); // Android ACTION_DOWN / ACTION_UP
    }
}
