package com.example.dh2;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.Button;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import org.json.JSONArray;
import org.json.JSONObject;
import java.io.InputStream;
import java.util.ArrayList;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** Private loader inspection surface. No gameplay or campaign save state. */
public final class LoaderPreviewActivity extends Activity {
    static { System.loadLibrary("dh2_loader_preview"); }
    private static native String initialize(AssetManager assets, String identity, String definition, int seed);
    private static native void resize(int width, int height);
    private static native void draw();
    private static native void orbit(float dx, float dy, float zoom);
    private static native String focus(int module);
    private static native String reload(String identity, String definition, int seed);
    private GLSurfaceView view;
    private TextView status;
    private volatile String identity, definition;
    private volatile int seed;
    private int module = -1;
    private void show(String text) { runOnUiThread(() -> status.setText(text)); }
    private void queue(Runnable action) { view.queueEvent(action); view.requestRender(); }
    private Button button(LinearLayout row, String label, android.view.View.OnClickListener action) {
        Button button = new Button(this); button.setText(label); button.setTextSize(11);
        button.setOnClickListener(action);
        row.addView(button,new LinearLayout.LayoutParams(0,-2,1)); return button;
    }
    @Override public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        identity = getIntent().getStringExtra("level");
        definition = getIntent().getStringExtra("definition");
        if (identity == null) identity = "SWAMP";
        if (definition == null) definition = "001_swamp.mlx";
        seed = getIntent().getIntExtra("seed",0);
        LinearLayout layout = new LinearLayout(this); layout.setOrientation(LinearLayout.VERTICAL);
        if (android.os.Build.VERSION.SDK_INT >= 30) {
            layout.setOnApplyWindowInsetsListener((v,insets) -> {
                android.graphics.Insets bars = insets.getInsets(android.view.WindowInsets.Type.systemBars());
                v.setPadding(bars.left,bars.top,bars.right,bars.bottom); return insets;
            });
        }
        status = new TextView(this); status.setText("Preparing " + identity + " from original cache…");
        status.setLines(2);
        status.setTextColor(0xffeeeeee); status.setBackgroundColor(0xff202329);
        status.setTextSize(12); layout.addView(status);
        // Original definitions, with native fixed/procedural preparation receipts.
        LinearLayout selection = new LinearLayout(this);
        Spinner maps = new Spinner(this);
        ArrayList<String> names = new ArrayList<>(), definitions = new ArrayList<>(), labels = new ArrayList<>();
        try (InputStream input = getAssets().open("loader-map-catalog.json")) {
            java.io.ByteArrayOutputStream bytes = new java.io.ByteArrayOutputStream();
            byte[] buffer = new byte[4096]; int count;
            while ((count=input.read(buffer))!=-1) bytes.write(buffer,0,count);
            JSONArray catalog = new JSONObject(bytes.toString("UTF-8")).getJSONArray("maps");
            for (int i=0;i<catalog.length();++i) {
                names.add(catalog.getJSONObject(i).getString("identity"));
                definitions.add(catalog.getJSONObject(i).getString("definition"));
                labels.add(catalog.getJSONObject(i).getString("label"));
            }
        } catch (Exception error) { status.setText("Map picker unavailable: "+error.getMessage()); }
        maps.setAdapter(new ArrayAdapter<>(this,android.R.layout.simple_spinner_dropdown_item,labels));
        int selected = names.indexOf(identity); if(selected>=0) maps.setSelection(selected);
        selection.addView(maps,new LinearLayout.LayoutParams(0,-2,3));
        Spinner seeds = new Spinner(this);
        seeds.setAdapter(new ArrayAdapter<>(this,android.R.layout.simple_spinner_dropdown_item,new String[]{"Seed 0","Seed 1"}));
        seeds.setSelection(seed==1?1:0);selection.addView(seeds,new LinearLayout.LayoutParams(0,-2,1));
        Button load = button(selection,"Load map",v -> {
            int index=maps.getSelectedItemPosition(); if(index<0) return;
            final String nextIdentity=names.get(index), nextDefinition=definitions.get(index);
            final int nextSeed=seeds.getSelectedItemPosition();
            status.setText("Preparing "+nextIdentity+"…");
            queue(() -> {
                String result=reload(nextIdentity,nextDefinition,nextSeed);
                if(!result.startsWith("Preparation failed")) {
                    identity=nextIdentity; definition=nextDefinition;
                    seed=nextSeed;
                    runOnUiThread(() -> module=-1);
                }
                show(result);
            });
        });
        load.setEnabled(!names.isEmpty()); layout.addView(selection);
        view = new GLSurfaceView(this); view.setEGLContextClientVersion(2);
        view.setEGLConfigChooser(8,8,8,8,24,0);
        view.setRenderer(new GLSurfaceView.Renderer() {
            public void onSurfaceCreated(GL10 gl, EGLConfig config) { show(initialize(getAssets(), identity, definition, seed)); }
            public void onSurfaceChanged(GL10 gl, int w, int h) { resize(w,h); }
            public void onDrawFrame(GL10 gl) { draw(); }
        });
        view.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        final float[] previous = new float[2];
        view.setOnTouchListener((v,e) -> {
            if(e.getActionMasked()==MotionEvent.ACTION_MOVE && e.getPointerCount()==1) {
                final float dx=(e.getX()-previous[0])*.006f, dy=(e.getY()-previous[1])*.006f;
                queue(() -> orbit(dx,dy,1));
            }
            previous[0]=e.getX(); previous[1]=e.getY(); return true;
        });
        layout.addView(view,new LinearLayout.LayoutParams(-1,0,1));
        LinearLayout controls = new LinearLayout(this);
        button(controls,"Whole map",v -> {module=-1;queue(() -> show(focus(-1)));});
        button(controls,"Next module",v -> {final int chosen=++module;queue(() -> show(focus(chosen)));});
        button(controls,"Reload",v -> queue(() -> show(reload(identity,definition,seed))));
        layout.addView(controls);
        LinearLayout zoom = new LinearLayout(this);
        button(zoom,"Zoom +",v -> queue(() -> orbit(0,0,.8f)));
        button(zoom,"Zoom −",v -> queue(() -> orbit(0,0,1.25f)));
        button(zoom,"Failed-load check",v -> queue(() -> show(reload(identity,"missing-loader-test.mlx",seed))));
        layout.addView(zoom); setContentView(layout);
    }
    @Override protected void onPause() {super.onPause();view.onPause();}
    @Override protected void onResume() {super.onResume();if(view!=null)view.onResume();}
}
