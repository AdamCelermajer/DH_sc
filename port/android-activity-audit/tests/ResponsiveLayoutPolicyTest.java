package local.dh2.sourceviewer;

public final class ResponsiveLayoutPolicyTest {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    private static void checkWeights(ResponsiveLayoutPolicy.Layout layout) {
        check(layout.previewWeight > 0.0f, "preview must keep positive space");
        check(layout.controlsWeight > 0.0f, "controls must keep positive space");
        check(Math.abs(layout.previewWeight + layout.controlsWeight - 1.0f) < 0.0001f,
                "pane weights must fill the available space");
    }

    public static void main(String[] args) {
        ResponsiveLayoutPolicy.Layout portrait = ResponsiveLayoutPolicy.choose(412, 915, 1.0f);
        check(!portrait.sideBySide, "portrait layout must stack panes");
        checkWeights(portrait);

        ResponsiveLayoutPolicy.Layout wideLandscape =
                ResponsiveLayoutPolicy.choose(1000, 600, 1.0f);
        check(wideLandscape.sideBySide, "wide landscape must put controls beside preview");
        checkWeights(wideLandscape);

        ResponsiveLayoutPolicy.Layout threshold =
                ResponsiveLayoutPolicy.choose(720, 400, 1.0f);
        check(threshold.sideBySide, "720 dp landscape should use the two-pane layout");
        ResponsiveLayoutPolicy.Layout justNarrow =
                ResponsiveLayoutPolicy.choose(719, 400, 1.0f);
        check(!justNarrow.sideBySide, "narrow landscape should retain scrollable stacked panes");

        ResponsiveLayoutPolicy.Layout scaledLandscape =
                ResponsiveLayoutPolicy.choose(1080, 600, 1.5f);
        check(scaledLandscape.sideBySide, "breakpoint must use dp rather than raw pixels");
        checkWeights(scaledLandscape);

        ResponsiveLayoutPolicy.Layout empty = ResponsiveLayoutPolicy.choose(0, 400, 1.0f);
        check(!empty.sideBySide, "unmeasured layout should safely default to stacked panes");
        checkWeights(empty);
    }
}
