package com.example.dh2;

import java.util.Locale;

public final class IntroSubtitleTrackTest {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        check(IntroSubtitleTrack.textAt(10300, 0).isEmpty(), "source leaves opening lead-in blank");
        check(IntroSubtitleTrack.textAt(10301, 0).startsWith("Gothicus, land of fear"),
                "first English caption follows the original 7300+3000ms cue");
        check(IntroSubtitleTrack.textAt(14300, 0).isEmpty(), "first caption uses the source exclusive end");
        check(IntroSubtitleTrack.textAt(17301, 0).startsWith("The tale of a kingdom"),
                "third caption follows the original timeline");
        check(IntroSubtitleTrack.textAt(24301, 0).isEmpty(),
                "source timeline retains the clear interval between cue four and five");
        check(IntroSubtitleTrack.textAt(32301, 0).startsWith("Armies are raised"),
                "seventh caption follows the original timeline");
        check(IntroSubtitleTrack.textAt(46301, 0).startsWith("be written in fire"),
                "last caption ends at the original 48.3s mark");
        check(IntroSubtitleTrack.textAt(48300, 0).isEmpty(), "last caption end is exclusive");
        check(IntroSubtitleTrack.textAt(10301, 1).startsWith("Gothicus, terre de peur"),
                "French source caption table is selectable");
        check(IntroSubtitleTrack.textAt(10301, 4).startsWith("ゴシカス"),
                "Japanese source caption table is selectable");
        check(IntroSubtitleTrack.textAt(10301, 5).startsWith("고디커스"),
                "Korean source caption table is selectable");
        check(IntroSubtitleTrack.languageIndex(Locale.FRENCH) == 1 &&
              IntroSubtitleTrack.languageIndex(Locale.GERMAN) == 2 &&
              IntroSubtitleTrack.languageIndex(Locale.ITALIAN) == 3 &&
              IntroSubtitleTrack.languageIndex(Locale.JAPANESE) == 4 &&
              IntroSubtitleTrack.languageIndex(Locale.KOREAN) == 5 &&
              IntroSubtitleTrack.languageIndex(Locale.SIMPLIFIED_CHINESE) == 6 &&
              IntroSubtitleTrack.languageIndex(new Locale("es")) == 7 &&
              IntroSubtitleTrack.languageIndex(Locale.ENGLISH) == 0,
                "Android locale selects the matching original caption table");
        check(IntroSubtitleTrack.textAt(10301, 100).equals(IntroSubtitleTrack.textAt(10301, 0)),
                "unknown subtitle table safely falls back to English");
        System.out.println("PASS: original opening subtitle language and cue schedule");
    }
}
