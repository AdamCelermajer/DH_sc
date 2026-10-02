package local.dh2.sourceviewer;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.SeekBar;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** A deliberately small source-based renderer, not the reconstructed game. */
public final class MainActivity extends Activity {
    static { System.loadLibrary("dh2lua"); System.loadLibrary("dh2source"); }
    private static final int BRES = 1;
    private static final int TEXTURE = 2;
    private static final int ANIMATION = 3;
    private static final int BLEND = 4;
    private static final int SCRIPT = 5;
    private static final int PROPERTIES = 6;
    private static final int CLASSES = 7;
    private static final int ITEMS = 8;
    private static final int POWERS = 9;
    private static final int CONSTANTS = 10;
    private GLSurfaceView surface;
    private TextView status;
    private TextView scriptStatus;
    private long scriptSession;
    private SeekBar timeline;
    private SeekBar mix;
    private Button playback;
    private boolean playing;
    private final Handler clockUi = new Handler(Looper.getMainLooper());
    private final Runnable showPosition = new Runnable() {
        @Override public void run() {
            if (!playing) return;
            updatePosition();
            clockUi.postDelayed(this, 100);
        }
    };
    private float lastX, lastY, yaw = 0.6f, pitch = 0.9f;

    private static native String loadBres(byte[] data);
    private static native long createScriptSession();
    private static native void destroyScriptSession(long session);
    private static native String executeScript(long session, byte[] source);
    private static native String importProperties(long session, byte[] data);
    private static native String importClasses(long session, byte[] data);
    private static native String importItems(long session, byte[] data);
    private static native String importPowers(long session, byte[] data);
    private static native String importConstants(long session, byte[] data);
    private static native String loadTexture(byte[] data);
    private static native String loadAnimation(byte[] data);
    private static native String loadBlendAnimation(byte[] data);
    private static native boolean blendAvailable();
    private static native int blendPercent();
    private static native void setBlendPercent(int percent);
    private static native int animationDuration();
    private static native int animationPosition();
    private static native void seekAnimation(int milliseconds);
    private static native void playAnimation(boolean playing);
    private static native void setView(float yaw, float pitch, float zoom);
    private static native void surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native void draw();

    private void updatePosition() {
        int position = animationPosition();
        timeline.setProgress(position);
        status.setText("Animation preview: " + position + " / " + timeline.getMax() + " ms. No gameplay.");
    }

    private byte[] readScript(InputStream input) throws Exception {
        return readLimited(input, 1024 * 1024);
    }

    private byte[] readLimited(InputStream input, int limit) throws Exception {
        if (input == null) throw new IllegalArgumentException("Cannot open selected file");
        try (InputStream in = input; ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[65536]; int count;
            while ((count = in.read(buffer)) >= 0) {
                if (out.size() + count > limit) throw new IllegalArgumentException("File exceeds " + (limit / (1024 * 1024)) + " MiB limit");
                out.write(buffer, 0, count);
            }
            return out.toByteArray();
        }
    }

    private void startScripts() {
        scriptSession = createScriptSession();
        if (scriptSession == 0) { scriptStatus.setText("Scripts unavailable: out of memory"); return; }
        try {
            for (String name : new String[]{"ai-commons.lua", "skills-commons.lua", "combat-formulas.lua"}) {
                String result = executeScript(scriptSession, readScript(getAssets().open("dh2/scripts/" + name)));
                if (!result.startsWith("Script loaded.")) throw new IllegalArgumentException(result);
            }
            scriptStatus.setText("Scripts ready: 3 shared files. Game objects are not connected yet.");
        } catch (Exception error) {
            destroyScriptSession(scriptSession); scriptSession = 0;
            scriptStatus.setText("Scripts unavailable: " + error.getMessage());
        }
    }

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(14, 14, 14, 14);
        layout.setOnApplyWindowInsetsListener((view, insets) -> {
            layout.setPadding(14, insets.getSystemWindowInsetTop() + 14,
                              14, insets.getSystemWindowInsetBottom() + 14);
            return insets;
        });
        status = new TextView(this);
        status.setText("Source renderer ready. Import a BRES scene and its PVRTC texture from your own cache. Drag the preview to rotate it. This is an asset preview, not gameplay.");
        layout.addView(status);
        Button mesh = new Button(this);
        mesh.setText("Import BRES scene");
        mesh.setOnClickListener(v -> pick(BRES));
        layout.addView(mesh);
        Button texture = new Button(this);
        texture.setText("Import PVRTC texture");
        texture.setOnClickListener(v -> pick(TEXTURE));
        layout.addView(texture);
        Button animation = new Button(this);
        animation.setText("Import character animation");
        animation.setOnClickListener(view -> pick(ANIMATION));
        layout.addView(animation);
        Button second = new Button(this);
        second.setText("Import second animation");
        second.setOnClickListener(view -> pick(BLEND));
        layout.addView(second);
        playback = new Button(this);
        playback.setText("Play animation");
        playback.setEnabled(false);
        playback.setOnClickListener(view -> {
            playing = !playing;
            playAnimation(playing);
            clockUi.removeCallbacks(showPosition);
            if (playing) clockUi.post(showPosition);
            else updatePosition();
            playback.setText(playing ? "Pause animation" : "Play animation");
            surface.setRenderMode(playing ? GLSurfaceView.RENDERMODE_CONTINUOUSLY
                                        : GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            surface.requestRender();
        });
        layout.addView(playback);
        timeline = new SeekBar(this);
        timeline.setContentDescription("Animation time");
        timeline.setEnabled(false);
        timeline.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar bar, int value, boolean fromUser) {
                if (!fromUser) return;
                playing = false;
                playAnimation(false);
                clockUi.removeCallbacks(showPosition);
                playback.setText("Play animation");
                surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
                seekAnimation(value);
                status.setText("Animation preview: " + value + " / " + bar.getMax() + " ms. No gameplay.");
                surface.requestRender();
            }
            @Override public void onStartTrackingTouch(SeekBar bar) {}
            @Override public void onStopTrackingTouch(SeekBar bar) {}
        });
        layout.addView(timeline);
        mix = new SeekBar(this);
        mix.setContentDescription("Motion mix");
        mix.setMax(100);
        mix.setEnabled(false);
        mix.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar bar, int value, boolean fromUser) {
                if (!fromUser) return;
                playing = false;
                playAnimation(false);
                clockUi.removeCallbacks(showPosition);
                playback.setText("Play animation");
                surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
                setBlendPercent(value);
                status.setText("Motion mix: " + value + "% second animation. No gameplay.");
                surface.requestRender();
            }
            @Override public void onStartTrackingTouch(SeekBar bar) {}
            @Override public void onStopTrackingTouch(SeekBar bar) {}
        });
        layout.addView(mix);
        scriptStatus = new TextView(this);
        scriptStatus.setContentDescription("Script status");
        layout.addView(scriptStatus);
        Button scripts = new Button(this);
        scripts.setText("Import script source");
        scripts.setOnClickListener(view -> pick(SCRIPT));
        layout.addView(scripts);
        Button properties = new Button(this);
        properties.setText("Import character properties");
        properties.setOnClickListener(view -> pick(PROPERTIES));
        layout.addView(properties);
        Button classes = new Button(this);
        classes.setText("Import character classes");
        classes.setOnClickListener(view -> pick(CLASSES));
        layout.addView(classes);
        Button items = new Button(this);
        items.setText("Import item data");
        items.setOnClickListener(view -> pick(ITEMS));
        layout.addView(items);
        Button powers = new Button(this);
        powers.setText("Import item powers");
        powers.setOnClickListener(view -> pick(POWERS));
        layout.addView(powers);
        Button constants = new Button(this);
        constants.setText("Import script constants");
        constants.setOnClickListener(view -> pick(CONSTANTS));
        layout.addView(constants);
        startScripts();
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            @Override public void onSurfaceCreated(GL10 ignored, EGLConfig config) { surfaceCreated(); }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) { surfaceChanged(width, height); }
            @Override public void onDrawFrame(GL10 ignored) { draw(); }
        });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        surface.setOnTouchListener((view, event) -> {
            if (event.getPointerCount() != 1) return true;
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                lastX = event.getX();
                lastY = event.getY();
                return true;
            }
            if (event.getActionMasked() != MotionEvent.ACTION_MOVE) return true;
            float x = event.getX(), y = event.getY();
            yaw += (x - lastX) * 3.0f / Math.max(1, view.getWidth());
            pitch = Math.max(0.15f, Math.min(1.5f,
                    pitch + (y - lastY) * 2.0f / Math.max(1, view.getHeight())));
            lastX = x;
            lastY = y;
            setView(yaw, pitch, 1.0f);
            surface.requestRender();
            return true;
        });
        layout.addView(surface, new LinearLayout.LayoutParams(-1, 0, 1));
        setContentView(layout);
        layout.requestApplyInsets();
    }

    private void pick(int request) {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, request);
    }

    @Override protected void onActivityResult(int request, int result, Intent data) {
        super.onActivityResult(request, result, data);
        if (result != RESULT_OK || data == null || data.getData() == null) return;
        if (request == SCRIPT) {
            try {
                scriptStatus.setText(executeScript(scriptSession, readScript(getContentResolver().openInputStream(data.getData()))));
            } catch (Exception error) { scriptStatus.setText("Script rejected: " + error.getMessage()); }
            return;
        }
        if (request == CONSTANTS) {
            try {
                scriptStatus.setText(importConstants(scriptSession,
                    readLimited(getContentResolver().openInputStream(data.getData()), 4 * 1024 * 1024)));
            } catch (Exception error) { scriptStatus.setText("Constants rejected: " + error.getMessage()); }
            return;
        }
        if (request == PROPERTIES) {
            try {
                scriptStatus.setText(importProperties(scriptSession,
                    readLimited(getContentResolver().openInputStream(data.getData()), 4 * 1024 * 1024)));
            } catch (Exception error) { scriptStatus.setText("Properties rejected: " + error.getMessage()); }
            return;
        }
        if (request == CLASSES) {
            try {
                scriptStatus.setText(importClasses(scriptSession,
                    readLimited(getContentResolver().openInputStream(data.getData()), 4 * 1024 * 1024)));
            } catch (Exception error) { scriptStatus.setText("Classes rejected: " + error.getMessage()); }
            return;
        }
        if (request == POWERS) {
            try {
                scriptStatus.setText(importPowers(scriptSession,
                    readLimited(getContentResolver().openInputStream(data.getData()), 4 * 1024 * 1024)));
            } catch (Exception error) { scriptStatus.setText("Powers rejected: " + error.getMessage()); }
            return;
        }
        if (request == ITEMS) {
            try {
                scriptStatus.setText(importItems(scriptSession,
                    readLimited(getContentResolver().openInputStream(data.getData()), 4 * 1024 * 1024)));
            } catch (Exception error) { scriptStatus.setText("Items rejected: " + error.getMessage()); }
            return;
        }
        if (request != BRES && request != TEXTURE && request != ANIMATION && request != BLEND) return;
        try (InputStream in = getContentResolver().openInputStream(data.getData());
             ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            if (in == null) throw new IllegalArgumentException("Cannot open selected file");
            byte[] buffer = new byte[65536];
            int n;
            while ((n = in.read(buffer)) >= 0) {
                if (out.size() + n > 32 * 1024 * 1024) throw new IllegalArgumentException("File exceeds 32 MiB limit");
                out.write(buffer, 0, n);
            }
            if (request == TEXTURE) status.setText(loadTexture(out.toByteArray()));
            else {
                playing = false;
                playAnimation(false);
                surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
                playback.setText("Play animation");
                status.setText(request == BRES ? loadBres(out.toByteArray()) :
                    request == BLEND ? loadBlendAnimation(out.toByteArray()) : loadAnimation(out.toByteArray()));
                int duration = animationDuration();
                timeline.setEnabled(duration > 0);
                playback.setEnabled(duration > 0);
                timeline.setMax(Math.max(1, duration));
                timeline.setProgress(animationPosition());
                mix.setEnabled(blendAvailable());
                mix.setProgress(blendPercent());
            }
            surface.requestRender();
        } catch (Exception e) {
            status.setText("Import failed: " + e.getMessage());
        }
    }

    @Override protected void onPause() {
        playing = false;
        playAnimation(false);
        clockUi.removeCallbacks(showPosition);
        playback.setText("Play animation");
        surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        surface.onPause(); super.onPause();
    }
    @Override protected void onResume() { super.onResume(); if (surface != null) surface.onResume(); }
    @Override protected void onDestroy() {
        clockUi.removeCallbacks(showPosition);
        destroyScriptSession(scriptSession); scriptSession = 0;
        super.onDestroy();
    }
}
