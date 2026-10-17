package local.dh2.sourceviewer;

/** Executable host test for the compact SWAMP overlay layout policy. */
public final class ResponsiveLayoutPolicyTest {
    private static void require(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        ResponsiveLayoutPolicy.Layout mainLandscape =
                ResponsiveLayoutPolicy.choose(1600, 900, 1.0f);
        require(mainLandscape.sideBySide, "wide main screen should use side-by-side panes");

        ResponsiveLayoutPolicy.PreviewOverlay phoneLandscape =
                ResponsiveLayoutPolicy.choosePreviewOverlay(800, 360, 1.0f);
        require(phoneLandscape.landscape, "landscape phone preview must be recognized");
        require(phoneLandscape.movementHudWidthDp <= 400,
                "movement HUD must stay compact on landscape phones");
        require(phoneLandscape.detailsWidthDp <= 400,
                "optional details panel must stay narrower than the scene");
        require(48 + phoneLandscape.detailsHeightDp <= 360 - 172,
                "details panel must leave room for bottom movement/actions");

        ResponsiveLayoutPolicy.PreviewOverlay portrait =
                ResponsiveLayoutPolicy.choosePreviewOverlay(360, 800, 1.0f);
        require(!portrait.landscape, "portrait layout must not be treated as landscape");
        require(portrait.movementHudWidthDp == 336 && portrait.detailsWidthDp == 336,
                "portrait overlays must fit within both 12 dp side insets");
        require(48 + portrait.detailsHeightDp <= 800 - 208,
                "portrait details panel must leave bottom controls clear");

        ResponsiveLayoutPolicy.PreviewOverlay invalid =
                ResponsiveLayoutPolicy.choosePreviewOverlay(0, 0, 0.0f);
        require(invalid.movementHudWidthDp == 320 && invalid.detailsHeightDp == 180,
                "invalid display metrics must produce safe compact defaults");
        System.out.println("SWAMP responsive preview overlay policy passed");
    }

    private ResponsiveLayoutPolicyTest() {}
}
