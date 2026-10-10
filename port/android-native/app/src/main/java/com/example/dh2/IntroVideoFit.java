package com.example.dh2;

/** Full-surface geometry for the opening movie and its modern display target. */
final class IntroVideoFit {
    static final class Result {
        final float scaleX;
        final float scaleY;
        final float width;
        final float height;
        final float left;
        final float top;

        private Result(float surfaceWidth, float surfaceHeight,
                       float width, float height) {
            this.scaleX = width / surfaceWidth;
            this.scaleY = height / surfaceHeight;
            this.width = width;
            this.height = height;
            this.left = (surfaceWidth - width) * 0.5f;
            this.top = (surfaceHeight - height) * 0.5f;
        }
    }

    private IntroVideoFit() { }

    static Result fullSurface(int videoWidth, int videoHeight,
                              int surfaceWidth, int surfaceHeight) {
        if (videoWidth <= 0 || videoHeight <= 0 ||
                surfaceWidth <= 0 || surfaceHeight <= 0) return null;
        // The game stretches its startup artwork and movie to the live driver
        // bounds. Deliberately use independent axes; do not create aspect bars.
        return new Result(surfaceWidth, surfaceHeight,
                surfaceWidth, surfaceHeight);
    }
}
