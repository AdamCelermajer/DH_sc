package com.example.dh2;

public final class CameraTouchGestureV1Test {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        check(CameraTouchGestureV1.resumedPanPointerIndex(true, 2, 0) == 1,
                "remaining pointer after first pointer lifts");
        check(CameraTouchGestureV1.resumedPanPointerIndex(true, 2, 1) == 0,
                "remaining pointer after second pointer lifts");
        check(CameraTouchGestureV1.resumedPanPointerIndex(false, 2, 0) == -1,
                "ordinary multi-touch must not become a camera pan");
        check(CameraTouchGestureV1.resumedPanPointerIndex(true, 3, 1) == -1,
                "three-pointer gesture remains canceled");
        check(Math.abs(CameraTouchGestureV1.drawableDistance(100, 100, 220, 100) - 120) < 0.001f,
                "pinch distance remains in drawable pixels instead of 480x320 SWF units");
        check(Math.abs(CameraTouchGestureV1.drawableDelta(150, 100) - 50) < 0.001f,
                "camera pan delta remains in drawable pixels");
        System.out.println("PASS: camera pinch-to-pan handoff");
    }
}
