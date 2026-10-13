package com.example.dh2;

/**
 * Maps Android MotionEvent pointer ids to the four source finger indices that
 * MenuManager::onEvent (0x430ad4) accepts. The lowest free index is used, so a
 * stick held by finger 0 and an attack pressed later by finger 1 stay separate
 * HUD cursors, and a fifth finger is consumed without dispatch like the source.
 */
final class GameplayTouchSlotsV1 {
    static final int SLOT_COUNT = 4;
    private final int[] pointerIds = {-1, -1, -1, -1};

    /** Slot now owned by {@code pointerId}, or -1 when all four fingers are busy. */
    int down(int pointerId) {
        int existing = slotOf(pointerId);
        if (existing >= 0) return existing;
        for (int slot = 0; slot < SLOT_COUNT; slot++) {
            if (pointerIds[slot] < 0) {
                pointerIds[slot] = pointerId;
                return slot;
            }
        }
        return -1;
    }

    /** Slot owned by {@code pointerId}, or -1. */
    int slotOf(int pointerId) {
        for (int slot = 0; slot < SLOT_COUNT; slot++) if (pointerIds[slot] == pointerId) return slot;
        return -1;
    }

    /** Frees the pointer's slot and returns it, or -1 when it owned none. */
    int up(int pointerId) {
        int slot = slotOf(pointerId);
        if (slot >= 0) pointerIds[slot] = -1;
        return slot;
    }

    int pointerIdAt(int slot) { return slot >= 0 && slot < SLOT_COUNT ? pointerIds[slot] : -1; }

    void clear() { java.util.Arrays.fill(pointerIds, -1); }
}
