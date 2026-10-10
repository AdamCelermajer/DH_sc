package com.example.dh2;

/** Source-order gate: intro movie, original splash/loading stage, authored menu. */
final class OpeningSequenceV1 {
    static final long SPLASH_HOLD_MS = 600L;
    static final int MENU = 0;
    static final int INTRO = 1;
    static final int SPLASH = 2;

    private volatile int stage;
    private volatile long splashStartedAtMs;

    OpeningSequenceV1(boolean playIntro) {
        stage = playIntro ? INTRO : MENU;
    }

    private OpeningSequenceV1(int restoredStage, long splashElapsedMs, long nowMs) {
        stage = restoredStage >= MENU && restoredStage <= SPLASH ? restoredStage : MENU;
        if (stage == SPLASH) {
            long elapsed = Math.max(0L, Math.min(SPLASH_HOLD_MS, splashElapsedMs));
            splashStartedAtMs = nowMs - elapsed;
        }
    }

    static OpeningSequenceV1 restore(int stage, long splashElapsedMs, long nowMs) {
        return new OpeningSequenceV1(stage, splashElapsedMs, nowMs);
    }

    boolean usesSplashStage() { return stage == INTRO || stage == SPLASH; }
    boolean awaitsIntroLanguage() { return stage == INTRO; }
    int stage() { return stage; }

    void introFinished(long nowMs) {
        if (stage != INTRO) return;
        stage = SPLASH;
        splashStartedAtMs = nowMs;
    }

    boolean mayLoadMenu(long nowMs) {
        return stage == SPLASH && nowMs - splashStartedAtMs >= SPLASH_HOLD_MS;
    }

    void menuLoaded() { stage = MENU; }

    long splashElapsedMs(long nowMs) {
        if (stage != SPLASH) return 0L;
        return Math.max(0L, Math.min(SPLASH_HOLD_MS, nowMs - splashStartedAtMs));
    }
}
