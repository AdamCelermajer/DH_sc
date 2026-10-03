package local.dh2.sourceviewer;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.File;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicBoolean;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** Bundled, authored encounter for testing recovered source gameplay components. */
public final class GameplayActivity extends Activity {
    static { System.loadLibrary("dh2lua"); System.loadLibrary("dh2source"); }
    private static native String loadWorld(byte[] room, byte[] floor, byte[] hero, byte[] texture, byte[] walk);
    private static native String loadMotions(byte[] idle, byte[] attack);
    private static native String surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native boolean drawFrame(float[] snapshot);
    private static native long sessionInit(byte[] properties, byte[] classes, byte[] loot,
        byte[] powers, byte[] quests, byte[] constants, byte[] combat, byte[] generation);
    private static native float[] sessionStep(long session, float x, float y, float dt, boolean attack);
    private static native String sessionStatus(long session);
    private static native String sessionReset(long session);
    private static native byte[] sessionSnapshot(long session);
    private static native String sessionRestore(long session, byte[] save);
    private GLSurfaceView surface;
    private TextView status;
    private volatile float moveX, moveY;
    private volatile boolean attackHeld;
    private final AtomicBoolean attackTap = new AtomicBoolean();
    private final AtomicBoolean resetRequested = new AtomicBoolean();
    private final AtomicBoolean resetClock = new AtomicBoolean(true);
    private final Object stateGuard = new Object();
    private volatile boolean paused = true;
    private boolean initialized;
    private boolean failed;
    private long previousFrame, lastHud;
    private long sessionId;
    private EncounterSaveStore saveStore;
    private byte[] latestCheckpoint;
    private boolean allowSave;
    private String restoreNote = "";

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }
    private byte[] asset(String name) throws Exception {
        try (InputStream in = getAssets().open(name); ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[65536]; int count;
            while ((count = in.read(buffer)) != -1) {
                if (out.size() + count > 32 * 1024 * 1024) throw new IllegalArgumentException("Asset is too large");
                out.write(buffer, 0, count);
            }
            return out.toByteArray();
        }
    }
    private void showStatus(String text) { runOnUiThread(() -> {
        if (!isFinishing() && !isDestroyed()) status.setText(text);
    }); }
    private byte[] generation(byte[][] world, byte[][] game) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        // Change this definition revision when encounter orchestration/balance changes.
        digest.update("DH2 authored cross encounter r1; persistent schema 1".getBytes(StandardCharsets.UTF_8));
        for (byte[][] group : new byte[][][]{world, game}) for (byte[] bytes : group) {
            digest.update(ByteBuffer.allocate(4).putInt(bytes.length).array()); digest.update(bytes);
        }
        return digest.digest();
    }
    private String hud() {
        String text = sessionStatus(sessionId);
        if (!restoreNote.isEmpty()) text += "\n" + restoreNote;
        String saved = saveStore != null && allowSave ? saveStore.status() : "";
        if (!saved.isEmpty()) text += "\n" + saved;
        return text;
    }
    private Button button(String title) {
        Button b = new Button(this); b.setText(title); b.setAllCaps(false);
        return b;
    }
    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN |
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        FrameLayout root = new FrameLayout(this);
        root.setBackgroundColor(Color.rgb(6, 9, 14));
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        surface.setPreserveEGLContextOnPause(true);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            @Override public void onSurfaceCreated(GL10 ignored, EGLConfig config) {
                synchronized (stateGuard) {
                try {
                    byte[][] world = {asset("dh2/encounter/room.bdae"), asset("dh2/encounter/floor.tga"),
                        asset("dh2/encounter/hero.bdae"), asset("dh2/encounter/hero.tga"), asset("dh2/encounter/walk.bdae"),
                        asset("dh2/encounter/idle.bdae"), asset("dh2/encounter/attack.bdae")};
                    String loaded = loadWorld(world[0], world[1], world[2], world[3], world[4]);
                    if (!loaded.startsWith("Development room ready")) throw new IllegalStateException(loaded);
                    String motions = loadMotions(world[5], world[6]);
                    if (!"Original idle and attack motions ready.".equals(motions)) throw new IllegalStateException(motions);
                    String graphics = surfaceCreated();
                    if (!"World renderer ready.".equals(graphics)) throw new IllegalStateException(graphics);
                    if (!initialized) {
                        byte[][] game = {asset("dh2/encounter/properties.bin"), asset("dh2/encounter/classes.bin"),
                            asset("dh2/encounter/loot.bin"), asset("dh2/encounter/powers.bin"),
                            asset("dh2/encounter/quests.bin"), asset("dh2/encounter/constants.bin"),
                            asset("dh2/scripts/combat-formulas.lua")};
                        sessionId = sessionInit(game[0], game[1], game[2], game[3], game[4], game[5], game[6], generation(world, game));
                        String ready = sessionStatus(sessionId);
                        if (!ready.startsWith("HP ")) throw new IllegalStateException(ready);
                        saveStore = EncounterSaveStore.get(new File(getFilesDir(), "source-encounter-saves"));
                        saveStore.attach(sessionId);
                        boolean restored = false;
                        for (byte[] checkpoint : saveStore.candidates()) {
                            String result = sessionRestore(sessionId, checkpoint);
                            if ("Encounter restored.".equals(result)) { restored = true; break; }
                            saveStore.rejected(checkpoint);
                        }
                        allowSave = restored || !saveStore.hasFiles();
                        restoreNote = restored ? "Progress restored." : allowSave ? "" :
                            "Saved progress could not restore. Reset starts a new saved run.";
                        latestCheckpoint = sessionSnapshot(sessionId);
                        if (latestCheckpoint == null) throw new IllegalStateException("Initial checkpoint could not be captured");
                        if (allowSave) saveStore.checkpoint(latestCheckpoint, sessionId, true, false);
                        initialized = true;
                    }
                    previousFrame = 0; failed = false;
                    showStatus(hud());
                } catch (Exception error) {
                    failed = true; showStatus("Encounter could not start: " + error.getMessage());
                }
                }
            }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) { surfaceChanged(width, height); }
            @Override public void onDrawFrame(GL10 ignored) {
                synchronized (stateGuard) {
                    if (!initialized || failed || paused) return;
                    if (resetClock.getAndSet(false)) previousFrame = 0;
                    long now = System.nanoTime();
                    float dt = previousFrame == 0 ? 0 : Math.min(0.1f, (now - previousFrame) / 1000000000f);
                    previousFrame = now;
                    if (resetRequested.getAndSet(false)) {
                        String result = sessionReset(sessionId);
                        if (!result.startsWith("HP ")) { showStatus("Reset failed: " + result); return; }
                        allowSave = true; restoreNote = ""; latestCheckpoint = sessionSnapshot(sessionId);
                        if (latestCheckpoint == null) { failed = true; showStatus("Reset checkpoint failed"); return; }
                        saveStore.checkpoint(latestCheckpoint, sessionId, true, true);
                    }
                    float[] snapshot = sessionStep(sessionId, moveX, moveY, dt, attackHeld | attackTap.getAndSet(false));
                    if (snapshot == null) { failed = true; showStatus(sessionStatus(sessionId)); return; }
                    byte[] checkpoint = sessionSnapshot(sessionId);
                    if (checkpoint == null) { failed = true; showStatus("Checkpoint failed: " + sessionStatus(sessionId)); return; }
                    // Quest/death/RNG changes make combat checkpoints urgent. Position
                    // changes are coalesced; every frame still publishes exact state.
                    boolean combatChanged = latestCheckpoint != null && !Arrays.equals(
                        Arrays.copyOfRange(latestCheckpoint, latestCheckpoint.length - 72, latestCheckpoint.length),
                        Arrays.copyOfRange(checkpoint, checkpoint.length - 72, checkpoint.length));
                    latestCheckpoint = checkpoint;
                    if (allowSave) saveStore.checkpoint(checkpoint, sessionId, combatChanged, false);
                    if (!drawFrame(snapshot)) {
                        failed = true;
                        if (allowSave) {
                            saveStore.checkpoint(checkpoint, sessionId, true, false);
                            waitForSave();
                        }
                        showStatus("Encounter renderer failed. Progress is retained if the save store reports saved; reopen this screen."); return;
                    }
                    if (now - lastHud > 200000000L) { lastHud = now; showStatus(hud()); }
                }
            }
        });
        root.addView(surface, new FrameLayout.LayoutParams(-1, -1));
        LinearLayout header = new LinearLayout(this);
        header.setOrientation(LinearLayout.VERTICAL);
        header.setPadding(dp(12), dp(6), dp(12), dp(6));
        header.setBackgroundColor(0xb0101720);
        TextView title = new TextView(this);
        title.setText("Source encounter · development room"); title.setTextColor(Color.WHITE); title.setTextSize(16);
        header.addView(title);
        status = new TextView(this); status.setText("Loading bundled assets…");
        status.setTextColor(0xffc8d5df); status.setTextSize(13); header.addView(status);
        root.addView(header, new FrameLayout.LayoutParams(-1, -2, Gravity.TOP));
        LinearLayout actions = new LinearLayout(this);
        Button reset = button("Reset"); reset.setOnClickListener(v -> resetRequested.set(true));
        Button diagnostics = button("Diagnostics");
        diagnostics.setOnClickListener(v -> startActivity(new Intent(this, MainActivity.class)));
        actions.addView(reset); actions.addView(diagnostics);
        FrameLayout.LayoutParams actionLayout = new FrameLayout.LayoutParams(-2, -2, Gravity.TOP | Gravity.RIGHT);
        actionLayout.topMargin = dp(64); actionLayout.rightMargin = dp(12); root.addView(actions, actionLayout);
        Joystick stick = new Joystick();
        FrameLayout.LayoutParams stickLayout = new FrameLayout.LayoutParams(dp(150), dp(150), Gravity.BOTTOM | Gravity.LEFT);
        stickLayout.leftMargin = dp(24); stickLayout.bottomMargin = dp(12); root.addView(stick, stickLayout);
        Button attack = button("Attack"); attack.setTextSize(20); attack.setContentDescription("Attack nearest sentry");
        attack.setOnClickListener(v -> attackTap.set(true));
        attack.setOnTouchListener((v, event) -> {
            switch (event.getActionMasked()) {
                case MotionEvent.ACTION_DOWN: attackHeld = true; break;
                case MotionEvent.ACTION_UP: case MotionEvent.ACTION_CANCEL: attackHeld = false; break;
            }
            return false;
        });
        FrameLayout.LayoutParams attackLayout = new FrameLayout.LayoutParams(dp(136), dp(88), Gravity.BOTTOM | Gravity.RIGHT);
        attackLayout.rightMargin = dp(30); attackLayout.bottomMargin = dp(30); root.addView(attack, attackLayout);
        root.setOnApplyWindowInsetsListener((view, insets) -> {
            view.setPadding(insets.getSystemWindowInsetLeft(), insets.getSystemWindowInsetTop(),
                insets.getSystemWindowInsetRight(), insets.getSystemWindowInsetBottom()); return insets;
        });
        setContentView(root); root.requestApplyInsets();
    }
    private final class Joystick extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private int pointer = -1;
        private float knobX, knobY;
        Joystick() { super(GameplayActivity.this); setContentDescription("Movement pad"); }
        @Override protected void onDraw(Canvas c) {
            float x = getWidth() / 2f, y = getHeight() / 2f, r = getWidth() * 0.42f;
            paint.setColor(0x604b647b); c.drawCircle(x, y, r, paint);
            paint.setColor(0xffb9d4e9); c.drawCircle(x + knobX * r, y + knobY * r, r * 0.28f, paint);
            paint.setTextAlign(Paint.Align.CENTER); paint.setTextSize(dp(12));
            c.drawText("Move", x, getHeight() - dp(3), paint);
        }
        @Override public boolean onTouchEvent(MotionEvent event) {
            int action = event.getActionMasked();
            if (action == MotionEvent.ACTION_DOWN) { pointer = event.getPointerId(0); getParent().requestDisallowInterceptTouchEvent(true); }
            if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL ||
                (action == MotionEvent.ACTION_POINTER_UP && event.getPointerId(event.getActionIndex()) == pointer)) {
                pointer = -1; knobX = knobY = moveX = moveY = 0; invalidate(); performClick(); return true;
            }
            int index = event.findPointerIndex(pointer);
            if (index < 0) return true;
            float r = getWidth() * 0.42f;
            knobX = (event.getX(index) - getWidth() / 2f) / r;
            knobY = (event.getY(index) - getHeight() / 2f) / r;
            float length = (float)Math.sqrt(knobX * knobX + knobY * knobY);
            if (length > 1) { knobX /= length; knobY /= length; }
            moveX = knobX; moveY = -knobY; invalidate(); return true;
        }
        @Override public boolean performClick() { super.performClick(); return true; }
    }
    @Override protected void onPause() {
        paused = true; moveX = moveY = 0; attackHeld = false; attackTap.set(false);
        flushCheckpoint();
        surface.onPause(); super.onPause();
    }
    @Override protected void onStop() {
        flushCheckpoint();
        super.onStop();
    }
    private void flushCheckpoint() {
        synchronized (stateGuard) {
            if (saveStore != null && allowSave && latestCheckpoint != null) {
                saveStore.checkpoint(latestCheckpoint, sessionId, true, false);
                waitForSave();
            }
        }
    }
    private void waitForSave() {
        try {
            if (saveStore != null && !saveStore.awaitIdle(2000))
                showStatus("Progress is still being saved.");
        } catch (InterruptedException interrupted) {
            Thread.currentThread().interrupt();
            showStatus("Progress save was interrupted.");
        }
    }
    @Override protected void onResume() {
        super.onResume(); resetClock.set(true); paused = false; if (surface != null) surface.onResume();
    }
}
