package com.example.dh2;

public final class MovementControlPointerV1Test {
    private static int checks;

    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
        checks++;
    }

    public static void main(String[] args) {
        MovementControlPointerV1 state = new MovementControlPointerV1();
        check(state.down(7, 100, 200) == MovementControlPointerV1.Decision.UPDATE &&
                state.activePointerId() == 7, "first pointer owns movement");
        state.drag(100, 200, 80);
        check(state.axisX() == 0 && state.axisY() == 0,
                "touch-down establishes the source joystick origin without movement");
        state.drag(140, 160, 80);
        check(state.axisX() == 0.5f && state.axisY() == -0.5f,
                "movement is measured from the captured touch origin, not the view center");
        state.drag(180, 260, 80);
        check(Math.abs((float) Math.hypot(state.axisX(), state.axisY()) - 1.0f) < 0.0001f,
                "drag beyond the joystick radius remains capped to the authored circle");
        check(state.pointerDown() == MovementControlPointerV1.Decision.IGNORE &&
                state.activePointerId() == 7, "second pointer cannot take over");
        check(state.move(true) == MovementControlPointerV1.Decision.UPDATE,
                "move continues from captured pointer");
        check(state.pointerUp(9) == MovementControlPointerV1.Decision.IGNORE &&
                state.activePointerId() == 7, "secondary lift leaves owner active");
        check(state.pointerUp(7) == MovementControlPointerV1.Decision.RELEASE &&
                state.activePointerId() == -1, "captured pointer lift releases without handoff");
        check(state.down(4) == MovementControlPointerV1.Decision.UPDATE,
                "fresh gesture reacquires control");
        check(state.move(false) == MovementControlPointerV1.Decision.RELEASE &&
                state.activePointerId() == -1, "missing captured pointer fails safe to release");
        check(state.down(2) == MovementControlPointerV1.Decision.UPDATE &&
                state.cancel() == MovementControlPointerV1.Decision.RELEASE &&
                state.cancel() == MovementControlPointerV1.Decision.IGNORE,
                "cancel releases once and is idempotent");
        System.out.println("PASS MovementControlPointerV1 checks=" + checks);
    }
}
