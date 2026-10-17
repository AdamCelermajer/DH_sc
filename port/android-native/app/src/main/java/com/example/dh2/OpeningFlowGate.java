package com.example.dh2;

/** Keeps title music behind both the opening movie and front-menu readiness. */
final class OpeningFlowGate {
    private OpeningFlowGate() { }

    static boolean mayStartTitleMusic(boolean frontMenuReady,
                                      boolean cinematicPending,
                                      boolean titleMusicAlreadyStarted) {
        return frontMenuReady && !cinematicPending && !titleMusicAlreadyStarted;
    }

    static boolean mayApplyCinematicCompletion(boolean activityFinishing,
                                                boolean activityDestroyed) {
        return !activityFinishing && !activityDestroyed;
    }
}
