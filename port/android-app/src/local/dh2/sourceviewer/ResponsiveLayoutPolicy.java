package local.dh2.sourceviewer;

/** Small, platform-independent policy for arranging the preview and scrollable controls. */
final class ResponsiveLayoutPolicy {
    private static final float SIDE_BY_SIDE_MIN_WIDTH_DP = 720.0f;

    static final class Layout {
        final boolean sideBySide;
        final float previewWeight;
        final float controlsWeight;

        Layout(boolean sideBySide, float previewWeight, float controlsWeight) {
            this.sideBySide = sideBySide;
            this.previewWeight = previewWeight;
            this.controlsWeight = controlsWeight;
        }
    }

    /** Compact size choices for the SWAMP preview's optional diagnostics overlay. */
    static final class PreviewOverlay {
        final boolean landscape;
        final int movementHudWidthDp;
        final int detailsWidthDp;
        final int detailsHeightDp;

        PreviewOverlay(boolean landscape, int movementHudWidthDp,
                       int detailsWidthDp, int detailsHeightDp) {
            this.landscape = landscape;
            this.movementHudWidthDp = movementHudWidthDp;
            this.detailsWidthDp = detailsWidthDp;
            this.detailsHeightDp = detailsHeightDp;
        }
    }

    static Layout choose(int widthPx, int heightPx, float density) {
        if (widthPx <= 0 || heightPx <= 0 || density <= 0.0f) {
            return new Layout(false, 0.45f, 0.55f);
        }
        float widthDp = widthPx / density;
        boolean sideBySide = widthPx > heightPx && widthDp >= SIDE_BY_SIDE_MIN_WIDTH_DP;
        return sideBySide
                ? new Layout(true, 0.54f, 0.46f)
                : new Layout(false, 0.45f, 0.55f);
    }

    static PreviewOverlay choosePreviewOverlay(int widthPx, int heightPx, float density) {
        if (widthPx <= 0 || heightPx <= 0 || density <= 0.0f) {
            return new PreviewOverlay(false, 320, 320, 180);
        }
        int widthDp = Math.max(1, Math.round(widthPx / density));
        int heightDp = Math.max(1, Math.round(heightPx / density));
        boolean landscape = widthPx > heightPx;
        int availableWidthDp = Math.max(200, widthDp - 24);
        int hudWidthDp = Math.min(400, availableWidthDp);
        int detailsWidthDp = Math.min(400, availableWidthDp);
        int bottomControlReserveDp = landscape ? 172 : 208;
        int availableDetailsHeightDp = Math.max(120,
                heightDp - 48 - bottomControlReserveDp);
        int maxDetailsHeightDp = landscape ? 220 : 280;
        int detailsHeightDp = Math.min(maxDetailsHeightDp, availableDetailsHeightDp);
        return new PreviewOverlay(landscape, hudWidthDp,
                detailsWidthDp, detailsHeightDp);
    }

    private ResponsiveLayoutPolicy() {}
}
