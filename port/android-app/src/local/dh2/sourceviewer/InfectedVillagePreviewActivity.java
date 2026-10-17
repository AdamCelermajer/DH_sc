package local.dh2.sourceviewer;

import android.app.Activity;
import android.app.AlertDialog;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import org.json.JSONArray;
import org.json.JSONObject;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Locale;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** Static imported source geometry; this screen does not start gameplay systems. */
public final class InfectedVillagePreviewActivity extends Activity {
    static { System.loadLibrary("dh2source"); }
    private static final String ROOT = "dh2/infectedvillage/";
    private static final String CATALOGUE = "data/3d/modules/infectedvillage/infectedvillage.bdae";
    private static final String MODULE_BDAE = CATALOGUE;
    private static final String[] REQUIRED = {
        "data/scene/005_infectedvillage.mlx",
        "data/3d/modules/infectedvillage/mgp/infected01.mgp",
        "data/3d/modules/infectedvillage/mvp/infected01.mvp",
        "data/3d/modules/infectedvillage/mgp/infected02.mgp",
        "data/3d/modules/infectedvillage/mvp/infected02.mvp"
    };

    private static native String loadInfectedVillage(byte[] mlx, byte[] catalogue,
            byte[] mgp0, byte[] mvp0, byte[] mgp1, byte[] mvp1,
            String[] texturePaths, byte[][] textureBytes);
    private static native void setView(float yawRadians, float pitchRadians, float zoom);
    private static native void surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native void draw();
    private static native void destroyPreview();

    private GLSurfaceView surface;
    private TextView overlay;
    private String sourceDetails = "Source preview details are loading.";
    private byte[] mlx, catalogue, mgp0, mvp0, mgp1, mvp1;
    private String[] texturePaths = new String[0];
    private byte[][] textureBytes = new byte[0][];
    private String[] unresolved = new String[0];
    private boolean assetsRead;
    private float yaw = 0.0f, pitch = 0.72f, zoom = 1.5f;
    private float lastX, lastY, pinchStart;
    private float pinchZoom;

    private static byte[] readLimited(InputStream source, int limit) throws Exception {
        if (source == null) throw new IllegalArgumentException("Bundled source file is missing");
        try (InputStream in = source; ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[65536]; int count;
            while ((count = in.read(buffer)) != -1) {
                if (out.size() + count > limit) throw new IllegalArgumentException("Bundled asset exceeds size limit");
                out.write(buffer, 0, count);
            }
            return out.toByteArray();
        }
    }

    private byte[] checkedAsset(JSONObject file) throws Exception {
        String path = file.getString("path");
        int size = file.getInt("size_bytes");
        if (size <= 0 || size > 32 * 1024 * 1024) throw new IllegalArgumentException("Invalid manifest size for " + path);
        byte[] data = readLimited(getAssets().open(ROOT + path), size);
        if (data.length != size || !hex(MessageDigest.getInstance("SHA-256").digest(data))
                .equalsIgnoreCase(file.getString("sha256")))
            throw new IllegalArgumentException("Packaged source hash mismatch: " + path);
        return data;
    }

    private static String hex(byte[] bytes) {
        StringBuilder value = new StringBuilder(bytes.length * 2);
        for (byte b : bytes) value.append(String.format(Locale.ROOT, "%02x", b & 255));
        return value.toString();
    }

    private void readBundle() throws Exception {
        JSONObject manifest = new JSONObject(new String(
                readLimited(getAssets().open(ROOT + "asset-manifest.json"), 2 * 1024 * 1024),
                StandardCharsets.UTF_8));
        JSONArray entries = manifest.getJSONArray("files");
        java.util.HashMap<String, JSONObject> byPath = new java.util.HashMap<>();
        for (int i = 0; i < entries.length(); i++) {
            JSONObject file = entries.getJSONObject(i);
            byPath.put(file.getString("path"), file);
        }
        byte[][] source = new byte[REQUIRED.length][];
        for (int i = 0; i < REQUIRED.length; i++) {
            JSONObject file = byPath.get(REQUIRED[i]);
            if (file == null) throw new IllegalArgumentException("Manifest lacks required source: " + REQUIRED[i]);
            source[i] = checkedAsset(file);
        }
        JSONObject catalogueFile = byPath.get(CATALOGUE);
        if (catalogueFile == null) throw new IllegalArgumentException("Manifest lacks module BDAE");
        catalogue = checkedAsset(catalogueFile);
        mlx = source[0]; mgp0 = source[1]; mvp0 = source[2]; mgp1 = source[3]; mvp1 = source[4];

        ArrayList<String> texturePathList = new ArrayList<>();
        ArrayList<byte[]> textureDataList = new ArrayList<>();
        for (int i = 0; i < entries.length(); i++) {
            JSONObject file = entries.getJSONObject(i);
            String path = file.getString("path");
            JSONArray roles = file.getJSONArray("roles");
            boolean samplerTexture = false;
            for (int r = 0; r < roles.length(); r++)
                samplerTexture |= "bdae-material-sampler-texture".equals(roles.getString(r));
            if (!samplerTexture) continue;
            JSONArray references = file.getJSONArray("references");
            boolean usedByModuleScene = false;
            for (int r = 0; r < references.length(); r++) {
                JSONObject ref = references.getJSONObject(r);
                if (MODULE_BDAE.equals(ref.optString("source_bdae"))) {
                    usedByModuleScene = true; break;
                }
            }
            if (!usedByModuleScene) continue;
            texturePathList.add(path);
            textureDataList.add(checkedAsset(file));
        }
        if (texturePathList.isEmpty()) throw new IllegalArgumentException("Manifest has no module material textures");
        texturePaths = texturePathList.toArray(new String[0]);
        textureBytes = textureDataList.toArray(new byte[0][]);

        ArrayList<String> unresolvedList = new ArrayList<>();
        JSONObject textureAudit = manifest.optJSONObject("material_texture_resolution");
        JSONArray gaps = textureAudit == null ? null : textureAudit.optJSONArray("unresolved_sampler_references");
        if (gaps != null) for (int i = 0; i < gaps.length(); i++) {
            JSONObject gap = gaps.getJSONObject(i);
            if (!MODULE_BDAE.equals(gap.optString("source_bdae"))) continue;
            unresolvedList.add(gap.optString("material_id", "unknown material") +
                    " sampler " + gap.optInt("parameter_index", -1) + " (" +
                    gap.optString("status", "unresolved") + ")");
        }
        unresolved = unresolvedList.toArray(new String[0]);
        assetsRead = true;
    }

    private String unresolvedSummary() {
        StringBuilder result = new StringBuilder("Unresolved source inputs: ");
        if (unresolved.length == 0) result.append("no unbound module samplers reported");
        else for (int i = 0; i < unresolved.length; i++) {
            if (i != 0) result.append("; ");
            result.append(unresolved[i]);
        }
        result.append("; external effect/shader pass selection; animated fog/decor scenes; numeric dialog/text and music/audio resource IDs.");
        return result.toString();
    }

    private void showSourceDetails() {
        new AlertDialog.Builder(this)
                .setTitle("Source preview details")
                .setMessage(sourceDetails)
                .setPositiveButton("Close", null)
                .show();
    }

    private void loadSourcePreview() {
        try {
            if (!assetsRead) readBundle();
            String report = loadInfectedVillage(mlx, catalogue, mgp0, mvp0, mgp1, mvp1,
                    texturePaths, textureBytes);
            overlay.post(() -> {
                sourceDetails = report + "\n" + unresolvedSummary() +
                    "\nStatic source geometry only · no gameplay activation, AI, collision, triggers, or level transitions. Material lighting/effect parity is not claimed.";
                overlay.setText(report + "\nStatic source geometry only · Tap Source details for unresolved inputs and limits.");
                if (report.startsWith("Loaded ") && surface != null) surface.requestRender();
            });
        } catch (Exception error) {
            overlay.post(() -> overlay.setText("Static source preview unavailable: " + error.getMessage()));
        }
    }

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        FrameLayout root = new FrameLayout(this);
        root.setBackgroundColor(Color.rgb(8, 12, 17));
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8, 8, 8, 8, 24, 0);
        surface.setPreserveEGLContextOnPause(true);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            @Override public void onSurfaceCreated(GL10 ignored, EGLConfig config) {
                surfaceCreated();
                loadSourcePreview();
            }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) {
                surfaceChanged(width, height);
            }
            @Override public void onDrawFrame(GL10 ignored) { draw(); }
        });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        surface.setOnTouchListener((view, event) -> {
            if (event.getPointerCount() >= 2) {
                float dx = event.getX(0) - event.getX(1), dy = event.getY(0) - event.getY(1);
                float distance = (float)Math.hypot(dx, dy);
                if (event.getActionMasked() == MotionEvent.ACTION_POINTER_DOWN) {
                    pinchStart = Math.max(1, distance); pinchZoom = zoom;
                } else if (event.getActionMasked() == MotionEvent.ACTION_MOVE) {
                    zoom = Math.max(0.55f, Math.min(4.0f, pinchZoom * distance / Math.max(1, pinchStart)));
                    setView(yaw, pitch, zoom); surface.requestRender();
                }
                return true;
            }
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                lastX = event.getX(); lastY = event.getY(); return true;
            }
            if (event.getActionMasked() == MotionEvent.ACTION_MOVE) {
                float x = event.getX(), y = event.getY();
                yaw += (x - lastX) * 4.0f / Math.max(1, view.getWidth());
                pitch = Math.max(0.25f, Math.min(1.35f,
                        pitch + (y - lastY) * 2.0f / Math.max(1, view.getHeight())));
                lastX = x; lastY = y;
                setView(yaw, pitch, zoom); surface.requestRender(); return true;
            }
            return true;
        });
        root.addView(surface, new FrameLayout.LayoutParams(-1, -1));

        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(dp(14), dp(10), dp(14), dp(10));
        panel.setBackgroundColor(0xc0121a24);
        LinearLayout heading = new LinearLayout(this);
        heading.setOrientation(LinearLayout.HORIZONTAL);
        heading.setGravity(Gravity.CENTER_VERTICAL);
        TextView title = new TextView(this);
        title.setText("INFECTED VILLAGE · STATIC SOURCE PREVIEW");
        title.setTextColor(Color.WHITE); title.setTextSize(16);
        heading.addView(title, new LinearLayout.LayoutParams(0, -2, 1));
        Button details = new Button(this); details.setText("Source details"); details.setAllCaps(false);
        details.setTextSize(12); details.setOnClickListener(v -> showSourceDetails());
        heading.addView(details, new LinearLayout.LayoutParams(-2, -2));
        panel.addView(heading);
        overlay = new TextView(this);
        overlay.setText("Loading bundled MLX, module objects, catalogue, and manifest-listed textures…");
        overlay.setTextColor(0xffd7e1ea); overlay.setTextSize(12); overlay.setMaxLines(3);
        panel.addView(overlay);
        root.addView(panel, new FrameLayout.LayoutParams(-1, -2, Gravity.TOP));
        Button back = new Button(this); back.setText("Return to encounter"); back.setAllCaps(false);
        back.setOnClickListener(v -> finish());
        FrameLayout.LayoutParams backPosition = new FrameLayout.LayoutParams(-2, -2, Gravity.BOTTOM | Gravity.RIGHT);
        backPosition.setMargins(0, 0, dp(16), dp(16)); root.addView(back, backPosition);
        setContentView(root);
        setView(yaw, pitch, zoom);
    }

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }
    @Override protected void onPause() { if (surface != null) surface.onPause(); super.onPause(); }
    @Override protected void onResume() { super.onResume(); if (surface != null) surface.onResume(); }
    @Override protected void onDestroy() {
        destroyPreview();
        super.onDestroy();
    }
}
