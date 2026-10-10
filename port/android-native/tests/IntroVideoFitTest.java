package com.example.dh2;

public final class IntroVideoFitTest {
    private static void near(float actual, float expected, String message) {
        if (Math.abs(actual - expected) > 0.01f)
            throw new AssertionError(message + ": got " + actual + ", expected " + expected);
    }

    private static IntroVideoFit.Result stretch(int width, int height) {
        IntroVideoFit.Result result = IntroVideoFit.fullSurface(1280, 720, width, height);
        if (result == null) throw new AssertionError("valid surface returned no geometry");
        near(result.width, width, "surface-filling width");
        near(result.height, height, "surface-filling height");
        near(result.scaleX, 1f, "no horizontal matte/crop transform");
        near(result.scaleY, 1f, "no vertical matte/crop transform");
        return result;
    }

    public static void main(String[] args) {
        IntroVideoFit.Result authored = stretch(1920, 1080);
        near(authored.left, 0, "16:9 left edge");
        near(authored.top, 0, "16:9 top edge");

        IntroVideoFit.Result wide = stretch(2400, 1080);
        near(wide.width, 2400, "20:9 full width");
        near(wide.height, 1080, "20:9 frame height");
        near(wide.left, 0, "20:9 left edge");
        near(wide.top, 0, "20:9 top edge");

        IntroVideoFit.Result portrait = stretch(1080, 2400);
        near(portrait.width, 1080, "portrait frame width");
        near(portrait.height, 2400, "portrait full height");
        near(portrait.left, 0, "portrait left edge");
        near(portrait.top, 0, "portrait top edge");

        // Simulate a TextureView resize/recreation and ensure the movie again
        // fills the new bounds on every supported surface shape.
        IntroVideoFit.Result recreated = stretch(1280, 720);
        near(recreated.width, 1280, "recreated 16:9 width");
        near(recreated.height, 720, "recreated 16:9 height");
        recreated = stretch(2400, 1080);
        near(recreated.width, wide.width, "recreated 20:9 width");
        near(recreated.height, wide.height, "recreated 20:9 height");
        near(recreated.left, wide.left, "recreated 20:9 centering");

        if (IntroVideoFit.fullSurface(1280, 720, 0, 1080) != null)
            throw new AssertionError("invalid surface dimensions must defer fitting");
        if (IntroVideoFit.fullSurface(0, 720, 2400, 1080) != null)
            throw new AssertionError("invalid video dimensions must defer fitting");
        System.out.println("PASS: intro fills 16:9, 20:9, and portrait surfaces without bars");
    }
}
