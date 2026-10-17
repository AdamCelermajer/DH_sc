package com.example.dh2;

/** Source-compatible pointer handoff for the gameplay camera touch gesture. */
final class CameraTouchGestureV1 {
    private CameraTouchGestureV1() {}

    // ZoomHandler consumes the raw touch coordinates carried by the device
    // event. Keep these in drawable pixels; SWF stage conversion is separate.
    static float drawableDelta(float current, float previous) {
        return current - previous;
    }

    static float drawableDistance(float x1, float y1, float x2, float y2) {
        return (float)Math.hypot(x1 - x2, y1 - y2);
    }

    static int resumedPanPointerIndex(boolean endingPinch, int pointerCount, int liftedIndex) {
        if (!endingPinch || pointerCount != 2 || liftedIndex < 0 || liftedIndex > 1) return -1;
        return 1 - liftedIndex;
    }
}
