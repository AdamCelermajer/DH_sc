package com.example.dh2;

public final class AudioFocusStartPolicyTest {
    private static void check(boolean actual, boolean expected, String message) {
        if (actual != expected) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        check(AudioFocusStartPolicy.focusGained(1, 1), true,
                "legacy and modern focus callbacks recognize gain");
        check(AudioFocusStartPolicy.focusGained(-1, 1), false,
                "focus loss clears active ownership");
        Object current = new Object();
        Object stale = new Object();
        check(AudioFocusStartPolicy.isCurrentPlayer(current, current, true), true,
                "current title player callbacks are accepted");
        check(AudioFocusStartPolicy.isCurrentPlayer(stale, current, true), false,
                "callbacks from a released title player are ignored");
        check(AudioFocusStartPolicy.isCurrentPlayer(current, current, false), false,
                "stopped title playback ignores queued callbacks");
        check(AudioFocusStartPolicy.mayStart(true, true, true), true,
                "delayed focus grant starts requested title audio");
        check(AudioFocusStartPolicy.mayStart(false, true, true), false,
                "focus loss blocks audio start");
        check(AudioFocusStartPolicy.mayStart(true, false, true), false,
                "activity pause blocks audio start");
        check(AudioFocusStartPolicy.mayStart(true, true, false), false,
                "unrequested audio stays stopped");
        System.out.println("PASS: delayed Android audio-focus startup policy");
    }
}
