package com.example.dh2;

/** Keeps Android movement attached to the first pointer, like the authored HUD joystick. */
final class MovementControlPointerV1 {
    enum Decision { UPDATE, IGNORE, RELEASE }

    private int activePointerId = -1;
    private float originX;
    private float originY;
    private float axisX;
    private float axisY;

    int activePointerId() { return activePointerId; }

    Decision down(int pointerId) {
        return down(pointerId, 0.0f, 0.0f);
    }

    Decision down(int pointerId, float x, float y) {
        activePointerId = pointerId;
        originX = x;
        originY = y;
        axisX = axisY = 0.0f;
        return Decision.UPDATE;
    }

    void drag(float x, float y, float radius) {
        if (radius <= 0.0f) {
            axisX = axisY = 0.0f;
            return;
        }
        axisX = (x - originX) / radius;
        axisY = (y - originY) / radius;
        float length = (float) Math.hypot(axisX, axisY);
        if (length > 1.0f) {
            axisX /= length;
            axisY /= length;
        }
    }

    float axisX() { return axisX; }
    float axisY() { return axisY; }

    Decision move(boolean activePointerPresent) {
        if (activePointerId < 0) return Decision.IGNORE;
        if (!activePointerPresent) return cancel();
        return Decision.UPDATE;
    }

    Decision pointerDown() { return Decision.IGNORE; }

    Decision pointerUp(int pointerId) {
        return pointerId == activePointerId ? cancel() : Decision.IGNORE;
    }

    Decision up(int pointerId) {
        return pointerId == activePointerId ? cancel() : Decision.IGNORE;
    }

    Decision cancel() {
        if (activePointerId < 0) return Decision.IGNORE;
        activePointerId = -1;
        originX = originY = 0.0f;
        axisX = axisY = 0.0f;
        return Decision.RELEASE;
    }
}
