package com.example.dh2;

/** Playback actions permitted by current Activity and audio-focus state. */
final class AudioFocusStartPolicy {
    private AudioFocusStartPolicy() { }

    static boolean focusGained(int change, int gainChange) {
        return change == gainChange;
    }

    static boolean isCurrentPlayer(Object callbackPlayer, Object currentPlayer,
                                   boolean playbackRequested) {
        return playbackRequested && callbackPlayer != null && callbackPlayer == currentPlayer;
    }

    static boolean mayStart(boolean focusGained, boolean activityResumed,
                            boolean playbackRequested) {
        return focusGained && activityResumed && playbackRequested;
    }
}
