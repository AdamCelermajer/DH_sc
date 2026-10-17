package com.example.dh2;

/** Keeps a multi-pointer gesture from releasing a pressed authored menu button. */
final class FrontMenuTouchGestureV1 {
    enum Action { DOWN, MOVE, UP, CANCEL, POINTER_DOWN, POINTER_UP }
    enum Decision { FORWARD, CANCEL, SUPPRESS }

    private boolean active;

    Decision accept(Action action, boolean activePointerLifted) {
        switch (action) {
            case DOWN:
                active = true;
                return Decision.FORWARD;
            case MOVE:
                return active ? Decision.FORWARD : Decision.SUPPRESS;
            case UP:
                if (!active) return Decision.SUPPRESS;
                active = false;
                return Decision.FORWARD;
            case CANCEL:
            case POINTER_DOWN:
                if (!active) return Decision.SUPPRESS;
                active = false;
                return Decision.CANCEL;
            case POINTER_UP:
                if (!active || !activePointerLifted) return Decision.SUPPRESS;
                active = false;
                return Decision.CANCEL;
            default:
                return Decision.SUPPRESS;
        }
    }
}
