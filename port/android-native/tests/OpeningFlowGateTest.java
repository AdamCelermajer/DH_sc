package com.example.dh2;

public final class OpeningFlowGateTest {
    private static void check(boolean value, String message) {
        if (!value) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        check(!OpeningFlowGate.mayStartTitleMusic(false, false, false),
                "front audio must wait for menu readiness");
        check(!OpeningFlowGate.mayStartTitleMusic(true, true, false),
                "front audio must wait for cinematic completion");
        check(!OpeningFlowGate.mayStartTitleMusic(true, false, true),
                "repeated menu frames must not restart title music");
        check(OpeningFlowGate.mayStartTitleMusic(true, false, false),
                "ready menu starts title music after the cinematic");
        check(OpeningFlowGate.mayApplyCinematicCompletion(false, false),
                "live activity applies a completed cinematic");
        check(!OpeningFlowGate.mayApplyCinematicCompletion(true, false),
                "finishing activity ignores queued completion");
        check(!OpeningFlowGate.mayApplyCinematicCompletion(false, true),
                "destroyed activity cannot restart title audio");
        System.out.println("PASS: opening cinematic/menu/title-audio transition gate");
    }
}
