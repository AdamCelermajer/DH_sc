package com.example.dh2;

/** Typed delivery from the retained native OriginalUiSession request queue. */
final class NativeLaunchRequest {
    static final int START_GAME = 0;
    static final int LEVEL_TRANSITION = 1;

    final int kind;
    final int slot;
    final boolean hasNumericDifficulty;
    final int requestedDifficulty;
    final String destinationName;
    final double rawEntryPoint;

    NativeLaunchRequest(int kind,int slot,boolean hasNumericDifficulty,
                        int requestedDifficulty,String destinationName,
                        double rawEntryPoint) {
        this.kind=kind;
        this.slot=slot;
        this.hasNumericDifficulty=hasNumericDifficulty;
        this.requestedDifficulty=requestedDifficulty;
        this.destinationName=destinationName;
        this.rawEntryPoint=rawEntryPoint;
    }
}
