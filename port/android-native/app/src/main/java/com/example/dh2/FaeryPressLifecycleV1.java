package com.example.dh2;

/** One physical press owns one native BeginCast / EndCast pair. */
public final class FaeryPressLifecycleV1 {
    public interface Commands { void send(int operation); }
    private final Commands commands;
    private boolean pressed;

    public FaeryPressLifecycleV1(Commands commands) {
        if (commands == null) throw new IllegalArgumentException("commands");
        this.commands = commands;
    }

    public void press() {
        if (pressed) return;
        pressed = true;
        commands.send(1);
    }

    /** Release, pointer cancellation, loss of window or detach share this path. */
    public void release() {
        if (!pressed) return;
        pressed = false;
        commands.send(3);
    }

    public boolean isPressed() { return pressed; }
}
