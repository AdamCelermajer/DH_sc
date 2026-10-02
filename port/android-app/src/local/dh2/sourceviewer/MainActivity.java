package local.dh2.sourceviewer;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** A deliberately small source-based renderer, not the reconstructed game. */
public final class MainActivity extends Activity {
    static { System.loadLibrary("dh2source"); }
    private static final int BRES = 1;
    private static final int TEXTURE = 2;
    private GLSurfaceView surface;
    private TextView status;

    private static native String loadBres(byte[] data);
    private static native String loadTexture(byte[] data);
    private static native void surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native void draw();

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
        status.setText("Source renderer ready. Import a BRES scene and its PVRTC texture from your own cache. This is an asset preview, not gameplay.");
        layout.addView(status);
        Button mesh = new Button(this);
        mesh.setText("Import BRES scene");
        mesh.setOnClickListener(v -> pick(BRES));
        layout.addView(mesh);
        Button texture = new Button(this);
        texture.setText("Import PVRTC texture");
        texture.setOnClickListener(v -> pick(TEXTURE));
        layout.addView(texture);
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(2);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            @Override public void onSurfaceCreated(GL10 ignored, EGLConfig config) { surfaceCreated(); }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) { surfaceChanged(width, height); }
            @Override public void onDrawFrame(GL10 ignored) { draw(); }
        });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
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
        if (request != BRES && request != TEXTURE) return;
        try (InputStream in = getContentResolver().openInputStream(data.getData());
             ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            if (in == null) throw new IllegalArgumentException("Cannot open selected file");
            byte[] buffer = new byte[65536];
            int n;
            while ((n = in.read(buffer)) >= 0) {
                if (out.size() + n > 32 * 1024 * 1024) throw new IllegalArgumentException("File exceeds 32 MiB limit");
                out.write(buffer, 0, n);
            }
            status.setText(request == BRES ? loadBres(out.toByteArray()) : loadTexture(out.toByteArray()));
            surface.requestRender();
        } catch (Exception e) {
            status.setText("Import failed: " + e.getMessage());
        }
    }

    @Override protected void onPause() { surface.onPause(); super.onPause(); }
    @Override protected void onResume() { super.onResume(); if (surface != null) surface.onResume(); }
}
