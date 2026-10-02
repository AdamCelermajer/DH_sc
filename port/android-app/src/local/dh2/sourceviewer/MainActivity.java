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
    static { System.loadLibrary("dh2source"); }
    private static final int BRES = 1;
    private static final int TEXTURE = 2;
    private static final int ANIMATION = 3;
    private GLSurfaceView surface;
    private TextView status;
    private SeekBar timeline;
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
    private static native String loadTexture(byte[] data);
    private static native String loadAnimation(byte[] data);
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
        if (request != BRES && request != TEXTURE && request != ANIMATION) return;
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
                status.setText(request == BRES ? loadBres(out.toByteArray()) : loadAnimation(out.toByteArray()));
                int duration = animationDuration();
                timeline.setEnabled(duration > 0);
                playback.setEnabled(duration > 0);
                timeline.setMax(Math.max(1, duration));
                timeline.setProgress(0);
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
}
