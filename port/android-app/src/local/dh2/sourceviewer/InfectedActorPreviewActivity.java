package local.dh2.sourceviewer;

import android.app.Activity;
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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** Hash-locked render-only view of source-compatible infected actor assets. */
public final class InfectedActorPreviewActivity extends Activity {
    static { System.loadLibrary("dh2source"); }

    private static final String ROOT = "dh2/infectedactors/";
    private static final String[] MODEL_KEYS = {
        "infected", "burned", "inf_blacksmith", "inf_maid", "inf_merchant", "inf_nun"};
    private static final String[] MODEL_PATHS = {
        "data/3d/characters/infected/infected.bdae",
        "data/3d/characters/infected/burned.bdae",
        "data/3d/characters/infected/inf_blacksmith.bdae",
        "data/3d/characters/infected/inf_maid.bdae",
        "data/3d/characters/infected/inf_merchant.bdae",
        "data/3d/characters/infected/inf_nun.bdae"};
    private static final String[] CONTROLLER_IDS = {
        "zombie02-mesh-skin", "_mesh_burned-mesh-skin", "blacksmith-mesh-skin",
        "_mesh_castle_worker02-mesh-skin", "_mesh_merchant_skinned_batch-mesh-skin",
        "_mesh_nun-mesh-skin"};
    private static final String[] CLIP_LABELS = {"Idle A", "Idle B", "Walk"};
    private static final String[] CLIP_STATES = {"Idle", "Idle", "Walk"};
    private static final int[] CLIP_TEMPLATE_IDS = {321, 321, 328};
    private static final String[] CLIP_PATHS = {
        "data/3d/characters/infected/animations/infected_idle.bdae",
        "data/3d/characters/infected/animations/infected_idle_02.bdae",
        "data/3d/characters/infected/animations/infected_walk.bdae"};
    private static final String[] TEXTURE_PATHS = {
        "data/3d/textures/atlas_dh2_game_objects_001.tga",
        "data/3d/textures/atlas_dh2_game_objects_001_alpha.tga",
        "data/3d/textures/atlas_dh2_game_objects_001_specular.tga",
        "data/3d/textures/atlas_skinned_characters_animdecor_gameobjects_002.tga"};
    private static final String SOURCE_MANIFEST_SHA256 =
        "d322afdd1f052e771b3fcf697cc5fa4bbd9d930ebc0e8084a82ebc9899136a19";
    private static final String COMPATIBILITY_SHA256 =
        "f87698aa63d2fd481b3cd20009a2f55291ea4c48cc218cd1c917270d7ebd52d7";

    private static native String loadActor(byte[] model, byte[] clip, int modelIndex, int clipIndex,
            String[] texturePaths, byte[][] textureBytes);
    private static native void setView(float yaw, float pitch, float zoom);
    private static native void surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native void draw();
    private static native void destroyActorPreview();

    private GLSurfaceView surface;
    private TextView status;
    private final HashMap<String, byte[]> assets = new HashMap<>();
    private final String[] characterNames = new String[MODEL_KEYS.length];
    private boolean assetsRead;
    private int modelIndex, clipIndex;
    private float yaw, pitch = .72f, zoom = 1.0f, lastX, lastY;

    private static byte[] readLimited(InputStream source, int limit) throws Exception {
        if (source == null) throw new IllegalArgumentException("A required pinned actor file is missing.");
        try (InputStream in = source; ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[65536]; int count;
            while ((count = in.read(buffer)) != -1) {
                if (out.size() + count > limit) throw new IllegalArgumentException("Pinned actor file exceeds its size limit.");
                out.write(buffer, 0, count);
            }
            return out.toByteArray();
        }
    }

    private static String hex(byte[] bytes) {
        StringBuilder result = new StringBuilder(bytes.length * 2);
        for (byte value : bytes) result.append(String.format(Locale.ROOT, "%02x", value & 255));
        return result.toString();
    }

    private byte[] checkedAsset(JSONObject row) throws Exception {
        String path = row.getString("path");
        int expectedBytes = row.getInt("size_bytes");
        if (!path.startsWith("data/") || path.contains("..") || path.indexOf('\\') >= 0 ||
                expectedBytes <= 0 || expectedBytes > 32 * 1024 * 1024)
            throw new IllegalArgumentException("Pinned actor manifest has an invalid file entry: " + path);
        byte[] bytes = readLimited(getAssets().open(ROOT + path), expectedBytes);
        String actual = hex(MessageDigest.getInstance("SHA-256").digest(bytes));
        if (bytes.length != expectedBytes || !actual.equals(row.getString("sha256")))
            throw new IllegalArgumentException("Pinned actor asset hash mismatch: " + path);
        return bytes;
    }

    private void readBundle() throws Exception {
        JSONObject manifest = new JSONObject(new String(readLimited(
                getAssets().open(ROOT + "actor-preview-manifest.json"), 128 * 1024),
                StandardCharsets.UTF_8));
        if (!"dh2.infected-actor-preview-bundle.v1".equals(manifest.getString("format")) ||
                !SOURCE_MANIFEST_SHA256.equals(manifest.getString("source_asset_manifest_sha256")) ||
                !COMPATIBILITY_SHA256.equals(manifest.getString("compatibility_evidence_sha256")))
            throw new IllegalArgumentException("Actor preview manifest or compatibility evidence is not the pinned source set.");

        JSONArray models = manifest.getJSONArray("models");
        JSONArray clips = manifest.getJSONArray("clips");
        JSONArray textures = manifest.getJSONArray("textures");
        JSONArray files = manifest.getJSONArray("files");
        if (models.length() != MODEL_KEYS.length || clips.length() != CLIP_PATHS.length ||
                textures.length() != TEXTURE_PATHS.length || files.length() !=
                MODEL_PATHS.length + CLIP_PATHS.length + TEXTURE_PATHS.length)
            throw new IllegalArgumentException("Pinned actor asset manifest has incomplete model, clip, or texture coverage.");
        HashMap<String, JSONObject> byPath = new HashMap<>();
        for (int i = 0; i < files.length(); i++) {
            JSONObject row = files.getJSONObject(i);
            String path = row.getString("path");
            if (byPath.put(path, row) != null)
                throw new IllegalArgumentException("Pinned actor manifest repeats: " + path);
        }
        HashSet<String> expectedPaths = new HashSet<>();
        for (String path : MODEL_PATHS) expectedPaths.add(path);
        for (String path : CLIP_PATHS) expectedPaths.add(path);
        for (String path : TEXTURE_PATHS) expectedPaths.add(path);
        if (!byPath.keySet().equals(expectedPaths))
            throw new IllegalArgumentException("Pinned actor manifest does not contain exactly the six models, three compatible clips, and four shared textures.");

        for (int i = 0; i < models.length(); i++) {
            JSONObject row = models.getJSONObject(i);
            if (!MODEL_KEYS[i].equals(row.getString("model_key")) ||
                    !MODEL_PATHS[i].equals(row.getString("path")) ||
                    !CONTROLLER_IDS[i].equals(row.getString("skin_controller_id")))
                throw new IllegalArgumentException("Source model order/controller mapping changed at index " + i);
            characterNames[i] = row.getString("character_name");
        }
        for (int i = 0; i < clips.length(); i++) {
            JSONObject row = clips.getJSONObject(i);
            if (!CLIP_PATHS[i].equals(row.getString("path")) ||
                    !CLIP_LABELS[i].equals(row.getString("label")) ||
                    !CLIP_STATES[i].equals(row.getString("state")) || row.getInt("segment") != 0)
                throw new IllegalArgumentException("Source Idle/Walk clip mapping changed at index " + i);
            if (row.getInt("anim_tpl_id") != CLIP_TEMPLATE_IDS[i])
                throw new IllegalArgumentException("Source animation-template identity changed at index " + i);
        }
        for (int i = 0; i < TEXTURE_PATHS.length; i++)
            if (!TEXTURE_PATHS[i].equals(textures.getString(i)))
                throw new IllegalArgumentException("The source actor texture closure changed at index " + i);

        for (String path : expectedPaths) {
            JSONObject row = byPath.get(path);
            byte[] bytes = checkedAsset(row);
            assets.put(path, bytes);
        }
        assetsRead = true;
    }

    private void loadSelectedOnGlThread() {
        try {
            if (!assetsRead) readBundle();
            byte[][] textureBytes = new byte[TEXTURE_PATHS.length][];
            for (int i = 0; i < TEXTURE_PATHS.length; i++) textureBytes[i] = assets.get(TEXTURE_PATHS[i]);
            String result = loadActor(assets.get(MODEL_PATHS[modelIndex]),
                    assets.get(CLIP_PATHS[clipIndex]), modelIndex, clipIndex,
                    TEXTURE_PATHS, textureBytes);
            final String report = "" + characterNames[modelIndex] + " · " + CLIP_LABELS[clipIndex] +
                    "\n" + result + "\nRecovered Idle/Walk clip preview only. No source runtime state selection or level activation.";
            runOnUiThread(() -> status.setText(report));
        } catch (Exception error) {
            final String report = "Actor diagnostic preview unavailable: " + error.getMessage();
            runOnUiThread(() -> status.setText(report));
        }
    }

    private void requestSelection() {
        status.setText("Loading " + characterNames[modelIndex] + " · " + CLIP_LABELS[clipIndex] + "…");
        if (surface != null) {
            surface.queueEvent(this::loadSelectedOnGlThread);
            surface.requestRender();
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
                loadSelectedOnGlThread();
            }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) {
                surfaceChanged(width, height);
            }
            @Override public void onDrawFrame(GL10 ignored) { draw(); }
        });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        surface.setOnTouchListener((view, event) -> {
            if (event.getPointerCount() != 1) return true;
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                lastX = event.getX(); lastY = event.getY(); return true;
            }
            if (event.getActionMasked() != MotionEvent.ACTION_MOVE) return true;
            float x = event.getX(), y = event.getY();
            yaw += (x - lastX) * 4.0f / Math.max(1, view.getWidth());
            pitch = Math.max(.25f, Math.min(1.35f,
                    pitch + (y - lastY) * 2.0f / Math.max(1, view.getHeight())));
            lastX = x; lastY = y;
            setView(yaw, pitch, zoom);
            return true;
        });
        root.addView(surface, new FrameLayout.LayoutParams(-1, -1));

        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(dp(14), dp(10), dp(14), dp(10));
        panel.setBackgroundColor(0xc0121a24);
        TextView title = new TextView(this);
        title.setText("INFECTED ACTOR · SOURCE CLIP DIAGNOSTIC");
        title.setTextColor(Color.WHITE); title.setTextSize(16); panel.addView(title);
        status = new TextView(this);
        status.setText("Checking pinned model, clip, skin and texture files…");
        status.setTextColor(0xffd7e1ea); status.setTextSize(12); status.setMaxLines(7);
        panel.addView(status);
        LinearLayout actions = new LinearLayout(this);
        Button model = new Button(this); model.setText("Next model"); model.setAllCaps(false);
        model.setOnClickListener(v -> { modelIndex = (modelIndex + 1) % MODEL_KEYS.length; requestSelection(); });
        Button clip = new Button(this); clip.setText("Next clip"); clip.setAllCaps(false);
        clip.setOnClickListener(v -> { clipIndex = (clipIndex + 1) % CLIP_PATHS.length; requestSelection(); });
        actions.addView(model, new LinearLayout.LayoutParams(0, -2, 1));
        actions.addView(clip, new LinearLayout.LayoutParams(0, -2, 1));
        panel.addView(actions);
        TextView scope = new TextView(this);
        scope.setText("Orbit by dragging. Diagnostic render only · no AI, source state machine, collision, activation, spawning, or playable level.");
        scope.setTextColor(0xffc6d4df); scope.setTextSize(11); panel.addView(scope);
        root.addView(panel, new FrameLayout.LayoutParams(-1, -2, Gravity.TOP));
        Button back = new Button(this); back.setText("Return to encounter"); back.setAllCaps(false);
        back.setOnClickListener(v -> finish());
        FrameLayout.LayoutParams backPosition = new FrameLayout.LayoutParams(-2, -2, Gravity.BOTTOM | Gravity.RIGHT);
        backPosition.setMargins(0, 0, dp(16), dp(16)); root.addView(back, backPosition);
        setContentView(root);
    }

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }

    @Override protected void onPause() { if (surface != null) surface.onPause(); super.onPause(); }
    @Override protected void onResume() { super.onResume(); if (surface != null) surface.onResume(); }
    @Override protected void onDestroy() { destroyActorPreview(); super.onDestroy(); }
}
