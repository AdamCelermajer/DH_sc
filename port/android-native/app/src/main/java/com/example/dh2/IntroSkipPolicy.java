package com.example.dh2;

/** Matches MyVideoView's tap-to-toggle skip control after its 7300 ms lead-in. */
final class IntroSkipPolicy {
    private boolean skipVisible;

    boolean onMovieTap(int positionMs) {
        if (positionMs <= 7300) return skipVisible;
        skipVisible = !skipVisible;
        return skipVisible;
    }

    boolean isSkipVisible() { return skipVisible; }
}
