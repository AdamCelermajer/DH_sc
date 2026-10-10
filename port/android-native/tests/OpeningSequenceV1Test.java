package com.example.dh2;

public final class OpeningSequenceV1Test {
    private static void check(boolean value, String message) {
        if (!value) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        OpeningSequenceV1 opening = new OpeningSequenceV1(true);
        check(opening.usesSplashStage() && opening.awaitsIntroLanguage(),
                "fresh launch keeps native loading stage behind intro");
        check(!opening.mayLoadMenu(10_000), "menu cannot initialize during intro");
        opening.introFinished(10_000);
        check(opening.stage() == OpeningSequenceV1.SPLASH && !opening.mayLoadMenu(10_599),
                "completion reveals the splash before the menu");
        check(opening.mayLoadMenu(10_600), "splash holds for the authored transition interval");
        opening.menuLoaded();
        check(!opening.usesSplashStage() && !opening.mayLoadMenu(20_000),
                "menu stage is terminal for the opening sequence");

        OpeningSequenceV1 skipped = OpeningSequenceV1.restore(OpeningSequenceV1.SPLASH, 0, 20_000);
        check(skipped.stage() == OpeningSequenceV1.SPLASH && skipped.usesSplashStage() &&
                        !skipped.awaitsIntroLanguage() && !skipped.mayLoadMenu(20_599) &&
                        skipped.mayLoadMenu(20_600),
                "explicit skip bypasses video but preserves source splash before menu");
        OpeningSequenceV1 restored = OpeningSequenceV1.restore(OpeningSequenceV1.SPLASH, 599, 50_000);
        check(!restored.mayLoadMenu(50_000) && restored.mayLoadMenu(50_001),
                "activity recreation retains remaining splash time");
        System.out.println("PASS opening order INTRO -> SPLASH -> MENU; skip and recreation preserved");
    }
}
