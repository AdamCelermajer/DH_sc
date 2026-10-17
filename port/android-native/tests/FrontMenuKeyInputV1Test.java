package com.example.dh2;

public final class FrontMenuKeyInputV1Test {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        check(FrontMenuKeyInputV1.gameSwfCode(29) == 65, "Android A maps to GameSWF A");
        check(FrontMenuKeyInputV1.gameSwfCode(54) == 90, "Android Z maps to GameSWF Z");
        check(FrontMenuKeyInputV1.gameSwfCode(8) == 49, "Android digit 1 maps to GameSWF 1");
        check(FrontMenuKeyInputV1.gameSwfCode(7) == 48, "Android digit 0 maps to GameSWF 0");
        check(FrontMenuKeyInputV1.gameSwfCode(66) == 13, "Enter reaches editable text input");
        check(FrontMenuKeyInputV1.gameSwfCode(67) == 8, "Backspace reaches editable text input");
        check(FrontMenuKeyInputV1.gameSwfCode(112) == 46, "Forward delete reaches editable text input");
        check(FrontMenuKeyInputV1.gameSwfCode(21) == 37 &&
              FrontMenuKeyInputV1.gameSwfCode(22) == 39, "Left and right arrows reach GameSWF");
        check(FrontMenuKeyInputV1.gameSwfCode(19) == 38 &&
              FrontMenuKeyInputV1.gameSwfCode(20) == 40, "Up and down arrows reach GameSWF");
        check(FrontMenuKeyInputV1.gameSwfCode(92) == 34 &&
              FrontMenuKeyInputV1.gameSwfCode(93) == 33, "Page-up and page-down ordering matches GameSWF");
        check(FrontMenuKeyInputV1.gameSwfCode('a') == 65 &&
              FrontMenuKeyInputV1.gameSwfCode('A') == 65, "Printable letters share source key with shift state");
        check(FrontMenuKeyInputV1.requiresShift('A') && !FrontMenuKeyInputV1.requiresShift('a'),
                "Printable case is carried by shift state");
        check(FrontMenuKeyInputV1.gameSwfCode(' ') == 32, "Printable space reaches GameSWF");
        check(FrontMenuKeyInputV1.gameSwfCode('@') == 50 &&
              FrontMenuKeyInputV1.requiresShift('@'), "IME punctuation uses its shifted GameSWF key");
        check(FrontMenuKeyInputV1.gameSwfCode('\u00e9') == -1, "Unsupported Unicode fails closed");
        check(FrontMenuKeyInputV1.gameSwfCode(4) == -1, "Android Back is not intercepted as text input");
        check(FrontMenuKeyInputV1.shouldForward(true, true, 0, 65),
                "active name field receives a printable key-down");
        check(FrontMenuKeyInputV1.shouldForward(true, true, 1, 13),
                "active name field receives Enter key-up");
        check(!FrontMenuKeyInputV1.shouldForward(true, false, 0, 65),
                "keyboard events are ignored outside authored EnterName");
        check(!FrontMenuKeyInputV1.shouldForward(false, true, 0, 65),
                "name-entry flag cannot forward keys from gameplay or another screen");
        check(!FrontMenuKeyInputV1.shouldForward(true, true, 0, -1),
                "unsupported keys remain unhandled");
        System.out.println("PASS: authored front-menu keyboard mapping");
    }
}
