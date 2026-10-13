package com.example.dh2;

public final class GameplayTouchSlotsV1Test {
    private static int checks;

    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
        checks++;
    }

    public static void main(String[] args) {
        GameplayTouchSlotsV1 slots = new GameplayTouchSlotsV1();
        // Movement stick held by the first finger, attack pressed by a second finger.
        check(slots.down(11) == 0, "first finger takes source finger index 0");
        check(slots.down(23) == 1, "second finger takes source finger index 1, not the stick's cursor");
        check(slots.slotOf(11) == 0 && slots.slotOf(23) == 1, "both fingers stay mapped while held");
        check(slots.down(11) == 0, "repeated DOWN for a held pointer keeps its index");
        check(slots.up(23) == 1 && slots.slotOf(23) == -1, "attack finger lift frees only its own index");
        check(slots.slotOf(11) == 0, "stick finger is untouched by the attack finger lift");
        check(slots.down(40) == 1, "next finger reuses the lowest free index");
        check(slots.down(41) == 2 && slots.down(42) == 3, "four fingers fit");
        check(slots.down(43) == -1 && slots.slotOf(43) == -1,
                "fifth finger is consumed like MenuManager::onEvent finger index > 3");
        check(slots.pointerIdAt(2) == 41 && slots.pointerIdAt(4) == -1, "index to pointer lookup");
        check(slots.up(99) == -1, "lifting an unknown pointer is ignored");
        check(slots.up(11) == 0 && slots.down(44) == 0, "freed index 0 is reused");
        slots.clear();
        check(slots.slotOf(40) == -1 && slots.slotOf(44) == -1 && slots.down(7) == 0,
                "CANCEL clears every finger");
        System.out.println("PASS GameplayTouchSlotsV1 checks=" + checks);
    }
}
