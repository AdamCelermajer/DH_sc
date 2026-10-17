package com.example.dh2;

public final class FrontMenuTouchGestureV1Test {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    public static void main(String[] args) {
        FrontMenuTouchGestureV1 gate = new FrontMenuTouchGestureV1();
        check(gate.accept(FrontMenuTouchGestureV1.Action.DOWN, false) ==
                FrontMenuTouchGestureV1.Decision.FORWARD, "initial menu press is forwarded");
        check(gate.accept(FrontMenuTouchGestureV1.Action.POINTER_DOWN, false) ==
                FrontMenuTouchGestureV1.Decision.CANCEL, "second pointer cancels the pressed SWF button");
        check(gate.accept(FrontMenuTouchGestureV1.Action.MOVE, false) ==
                FrontMenuTouchGestureV1.Decision.SUPPRESS, "canceled gesture movement stays suppressed");
        check(gate.accept(FrontMenuTouchGestureV1.Action.POINTER_UP, false) ==
                FrontMenuTouchGestureV1.Decision.SUPPRESS, "pointer lift cannot revive a canceled click");
        check(gate.accept(FrontMenuTouchGestureV1.Action.UP, false) ==
                FrontMenuTouchGestureV1.Decision.SUPPRESS, "final lift cannot dispatch a canceled click");
        check(gate.accept(FrontMenuTouchGestureV1.Action.DOWN, false) ==
                FrontMenuTouchGestureV1.Decision.FORWARD, "a fresh gesture re-enables menu input");
        check(gate.accept(FrontMenuTouchGestureV1.Action.POINTER_UP, true) ==
                FrontMenuTouchGestureV1.Decision.CANCEL, "lifting the active pointer cancels while another remains");
        check(gate.accept(FrontMenuTouchGestureV1.Action.UP, false) ==
                FrontMenuTouchGestureV1.Decision.SUPPRESS, "remaining pointer cannot click after active pointer lifts");
        System.out.println("PASS: front-menu multi-touch cancellation");
    }
}
