package local.dh2.sourceviewer;

import android.app.Activity;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.DisplayMetrics;
import android.view.Choreographer;
import android.view.Gravity;
import android.view.MotionEvent;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** Developer movement test over source SWAMP module zero, with the intro trace retained. */
public final class SwampPreviewActivity extends Activity {
    private static final Pattern POSITION = Pattern.compile(
            "X (-?\\d+(?:\\.\\d+)?) Y (-?\\d+(?:\\.\\d+)?) Z (-?\\d+(?:\\.\\d+)?)");
    private static final Pattern YAW = Pattern.compile("yaw (-?\\d+(?:\\.\\d+)?)");
    private static final Pattern PATH_MASK = Pattern.compile(
            "(?:player path mask |player mask=)(0x[0-9A-Fa-f]+|[0-9A-Fa-f]+)");
    static { System.loadLibrary("dh2lua"); System.loadLibrary("dh2source"); }
    private static native String loadSwamp(byte[] bres, byte[] mlx, byte[] diffuse, byte[] alpha,
            byte[] moduleZeroMgp, byte[] heroBres, byte[] heroTexture, byte[] idleClip,
            byte[] walkClip);
    private static native String validateScriptTables(byte[] commonNames, byte[] commonPrograms,
            byte[] swampNames, byte[] swampPrograms);
    private static native String startIntroTrace(byte[] commonNames, byte[] commonPrograms,
            byte[] swampNames, byte[] swampPrograms, byte[] mlx, byte[] moduleOneMgp);
    private static native String advanceIntroTrace(int deltaMs);
    private static native void destroyIntroTrace();
    private static native String stepPlayer(float stickX, float stickY, float dtSeconds);
    private static native void destroySwampPreview();
    private static native void surfaceCreated();
    private static native void surfaceChanged(int width, int height);
    private static native void draw();
    private TextView status;
    private TextView movementStatus;
    private TextView movementDetails;
    private TextView traceStatus;
    private ScrollView traceScroll;
    private Button detailsButton;
    private ResponsiveLayoutPolicy.PreviewOverlay previewOverlay;
    private GLSurfaceView surface;
    private TouchPad movementPad;
    private boolean assetsLoaded;
    private volatile boolean movementReady;
    private boolean activityResumed;
    private boolean movementFrameScheduled;
    private boolean traceStarted;
    private boolean traceComplete;
    private boolean frameScheduled;
    private long lastFrameNanos;
    private long accumulatorNanos;
    private long lastMovementFrameNanos;
    private long movementAccumulatorNanos;
    private long lastMovementReportNanos;
    private float stickX;
    private float stickY;
    private String lastMovementText;
    private int simulationTimeMs;
    private int lastDisplayedHundredMs = -1;
    private final Choreographer.FrameCallback traceFrame = new Choreographer.FrameCallback() {
        @Override public void doFrame(long frameTimeNanos) {
            frameScheduled = false;
            if (!traceStarted || traceComplete || isFinishing()) return;
            long now = SystemClock.elapsedRealtimeNanos();
            if (lastFrameNanos == 0) lastFrameNanos = now;
            long elapsed = Math.max(0, Math.min(now - lastFrameNanos, 250_000_000L));
            lastFrameNanos = now;
            accumulatorNanos += elapsed;
            int deltaMs = (int) (accumulatorNanos / 10_000_000L) * 10;
            if (deltaMs > 0) {
                accumulatorNanos -= deltaMs * 1_000_000L;
                String report = advanceIntroTrace(deltaMs);
                simulationTimeMs += deltaMs;
                int hundredMs = simulationTimeMs / 100;
                if (hundredMs != lastDisplayedHundredMs || simulationTimeMs >= 4000) {
                    traceStatus.setText(report);
                    lastDisplayedHundredMs = hundredMs;
                    if (simulationTimeMs >= 4000) {
                        traceScroll.post(() -> traceScroll.fullScroll(View.FOCUS_DOWN));
                    }
                }
                if (simulationTimeMs >= 4010 && report.contains("End: complete")) {
                    traceComplete = true;
                }
            }
            scheduleTraceFrame();
        }
    };
    private final Choreographer.FrameCallback movementFrame = new Choreographer.FrameCallback() {
        @Override public void doFrame(long frameTimeNanos) {
            movementFrameScheduled = false;
            if (!activityResumed || isFinishing()) return;
            long now = SystemClock.elapsedRealtimeNanos();
            if (lastMovementFrameNanos == 0) lastMovementFrameNanos = now;
            long elapsed = Math.max(0, Math.min(now - lastMovementFrameNanos, 100_000_000L));
            lastMovementFrameNanos = now;
            movementAccumulatorNanos += elapsed;
            if (movementReady) {
                final long fixedStepNanos = 20_000_000L;
                int steps = 0;
                String report = null;
                while (movementAccumulatorNanos >= fixedStepNanos && steps < 5) {
                    report = stepPlayer(stickX, stickY, 0.02f);
                    movementAccumulatorNanos -= fixedStepNanos;
                    steps++;
                }
                boolean modeChanged = report != null && lastMovementText != null &&
                        report.contains("WALK") != lastMovementText.contains("WALK");
                boolean newFloorFailure = report != null && report.startsWith("NO PATH-ELIGIBLE MODULE-0") &&
                        !report.equals(lastMovementText);
                if (report != null && (now - lastMovementReportNanos >= 100_000_000L ||
                        newFloorFailure || modeChanged)) {
                    movementStatus.setText(compactMovementStatus(report));
                    movementDetails.setText(report);
                    lastMovementText = report;
                    lastMovementReportNanos = now;
                }
            }
            scheduleMovementFrame();
        }
    };

    private byte[] asset(String name) throws Exception {
        try (InputStream in = getAssets().open(name); ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[65536]; int count;
            while ((count = in.read(buffer)) != -1) {
                if (out.size() + count > 32 * 1024 * 1024) throw new IllegalArgumentException("SWAMP asset is too large");
                out.write(buffer, 0, count);
            }
            return out.toByteArray();
        }
    }

    private String compactMovementStatus(String report) {
        if (report == null || report.isEmpty()) return "MOVE · unavailable\nOpen Details for status";
        boolean rejected = report.startsWith("NO PATH-ELIGIBLE MODULE-0") ||
                report.startsWith("MOVEMENT INPUT REJECTED");
        Matcher position = POSITION.matcher(report);
        if (!position.find()) {
            return rejected ? "BLOCKED · position held\nOpen Details for source status"
                    : "MOVE · loading\nFloor check pending";
        }
        String state = rejected ? "BLOCKED" : report.contains("WALK") ? "WALK" : "IDLE";
        String firstLine = state + " · X " + position.group(1) +
                " Y " + position.group(2);
        String secondLine = "Z " + position.group(3);
        Matcher yaw = YAW.matcher(report);
        if (yaw.find()) secondLine += " · yaw " + yaw.group(1);
        Matcher mask = PATH_MASK.matcher(report);
        if (mask.find()) secondLine += " · mask " + mask.group(1);
        if (rejected) secondLine += " · no floor · held";
        return firstLine + "\n" + secondLine;
    }

    private void setDetailsVisible(boolean visible) {
        if (traceScroll == null || detailsButton == null) return;
        traceScroll.setVisibility(visible ? View.VISIBLE : View.GONE);
        detailsButton.setText(visible ? "Hide details" : "Details");
        detailsButton.setContentDescription(visible
                ? "Hide source and trace details" : "Show source and trace details");
    }

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        FrameLayout root = new FrameLayout(this);
        root.setBackgroundColor(Color.rgb(6, 9, 14));
        DisplayMetrics metrics = getResources().getDisplayMetrics();
        previewOverlay = ResponsiveLayoutPolicy.choosePreviewOverlay(
                metrics.widthPixels, metrics.heightPixels, metrics.density);
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8, 8, 8, 8, 24, 0);
        surface.setPreserveEGLContextOnPause(true);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            @Override public void onSurfaceCreated(GL10 ignored, EGLConfig config) {
                surfaceCreated();
                if (!assetsLoaded) {
                    try {
                        String message = loadSwamp(
                            asset("dh2/encounter/swamp.bdae"),
                            asset("dh2/encounter/swamp.mlx"),
                            asset("dh2/encounter/swamp-diffuse.tga"),
                            asset("dh2/encounter/swamp-alpha.tga"),
                            asset("dh2/encounter/swamp-entry-mgp.mgp"),
                            asset("dh2/encounter/hero.bdae"),
                            asset("dh2/encounter/hero.tga"),
                            asset("dh2/encounter/idle.bdae"),
                            asset("dh2/encounter/walk.bdae"));
                        assetsLoaded = message.startsWith("SWAMP module 0 ·");
                        movementReady = assetsLoaded && message.contains("Player path mask 2");
                        String scriptMessage = validateScriptTables(
                            asset("dh2/encounter/common-script-names.bin"),
                            asset("dh2/encounter/common-script-programs.bin"),
                            asset("dh2/encounter/swamp-script-names.bin"),
                            asset("dh2/encounter/swamp-script-programs.bin"));
                        final String fullMessage = message + "\n" + scriptMessage;
                        runOnUiThread(() -> status.setText(fullMessage));
                    } catch (Exception error) {
                        String message = "SWAMP preview could not load: " + error.getMessage();
                        runOnUiThread(() -> status.setText(message));
                    }
                }
            }
            @Override public void onSurfaceChanged(GL10 ignored, int width, int height) {
                surfaceChanged(width, height);
            }
            @Override public void onDrawFrame(GL10 ignored) { draw(); }
        });
        root.addView(surface, new FrameLayout.LayoutParams(-1, -1));
        TextView header = new TextView(this);
        header.setText("SWAMP · first module"); header.setTextColor(Color.WHITE); header.setTextSize(16);
        header.setBackgroundColor(0xb0101720); header.setPadding(dp(12), dp(6), dp(104), dp(6));
        root.addView(header, new FrameLayout.LayoutParams(-1, dp(42), Gravity.TOP));
        detailsButton = new Button(this);
        detailsButton.setText("Details"); detailsButton.setAllCaps(false);
        detailsButton.setTextSize(12); detailsButton.setMinHeight(dp(32));
        detailsButton.setMinimumHeight(dp(32));
        detailsButton.setPadding(dp(10), 0, dp(10), 0);
        detailsButton.setOnClickListener(v -> setDetailsVisible(
                traceScroll.getVisibility() != View.VISIBLE));
        FrameLayout.LayoutParams detailsButtonPosition = new FrameLayout.LayoutParams(
                -2, dp(36), Gravity.TOP | Gravity.RIGHT);
        detailsButtonPosition.topMargin = dp(3); detailsButtonPosition.rightMargin = dp(8);
        root.addView(detailsButton, detailsButtonPosition);
        status = new TextView(this); status.setText("Reading bundled level and module data…");
        status.setContentDescription("SWAMP source and script diagnostics");
        status.setTextColor(0xffd2dbe4); status.setTextSize(12); status.setBackgroundColor(0xb0101720);
        status.setPadding(dp(12), dp(8), dp(12), dp(8));
        movementStatus = new TextView(this);
        movementStatus.setContentDescription("Source movement status");
        movementStatus.setText("MOVE · loading\nFloor check pending");
        movementStatus.setTextColor(0xfff0f4f8); movementStatus.setTextSize(12);
        movementStatus.setBackgroundColor(0xb0101720);
        movementStatus.setPadding(dp(12), dp(8), dp(12), dp(8));
        movementStatus.setMaxLines(2);
        FrameLayout.LayoutParams movementStatusPosition = new FrameLayout.LayoutParams(
                dp(previewOverlay.movementHudWidthDp), -2, Gravity.TOP | Gravity.LEFT);
        movementStatusPosition.topMargin = dp(48); movementStatusPosition.leftMargin = dp(12);
        root.addView(movementStatus, movementStatusPosition);
        traceScroll = new ScrollView(this);
        traceScroll.setFillViewport(true);
        traceScroll.setBackgroundColor(0xb0101720);
        LinearLayout detailsContent = new LinearLayout(this);
        detailsContent.setOrientation(LinearLayout.VERTICAL);
        traceStatus = new TextView(this);
        traceStatus.setText("Intro trace idle. This developer action runs the source script without collision.");
        traceStatus.setTextColor(0xffd2dbe4); traceStatus.setTextSize(12);
        traceStatus.setPadding(dp(12), dp(10), dp(12), dp(10));
        movementDetails = new TextView(this);
        movementDetails.setContentDescription("Movement source diagnostics");
        movementDetails.setText("Detailed movement report will appear after the source checks.");
        movementDetails.setTextColor(0xffd2dbe4); movementDetails.setTextSize(12);
        movementDetails.setPadding(dp(12), dp(10), dp(12), dp(10));
        detailsContent.addView(status, new LinearLayout.LayoutParams(-1, -2));
        detailsContent.addView(movementDetails, new LinearLayout.LayoutParams(-1, -2));
        detailsContent.addView(traceStatus, new LinearLayout.LayoutParams(-1, -2));
        traceScroll.addView(detailsContent, new ScrollView.LayoutParams(-1, -2));
        FrameLayout.LayoutParams tracePanel = new FrameLayout.LayoutParams(
                dp(previewOverlay.detailsWidthDp), dp(previewOverlay.detailsHeightDp),
                Gravity.TOP | Gravity.RIGHT);
        tracePanel.topMargin = dp(48); tracePanel.rightMargin = dp(12);
        root.addView(traceScroll, tracePanel);
        traceScroll.setVisibility(View.GONE);
        Button runTrace = new Button(this); runTrace.setText("Run LizardMan_Intro trace");
        runTrace.setAllCaps(false);
        runTrace.setOnClickListener(v -> startTraceFromSource(runTrace));
        movementPad = new TouchPad();
        movementPad.setContentDescription("Movement pad");
        FrameLayout.LayoutParams padPosition = new FrameLayout.LayoutParams(dp(144), dp(144),
                Gravity.BOTTOM | Gravity.LEFT);
        padPosition.leftMargin = dp(16); padPosition.bottomMargin = dp(16);
        root.addView(movementPad, padPosition);
        FrameLayout.LayoutParams runPosition = new FrameLayout.LayoutParams(-2, -2,
                Gravity.BOTTOM | Gravity.RIGHT);
        runPosition.rightMargin = dp(16); runPosition.bottomMargin = dp(72);
        root.addView(runTrace, runPosition);
        Button back = new Button(this); back.setText("Return to encounter"); back.setAllCaps(false);
        back.setOnClickListener(v -> finish());
        FrameLayout.LayoutParams backPosition = new FrameLayout.LayoutParams(-2, -2, Gravity.BOTTOM | Gravity.RIGHT);
        backPosition.rightMargin = dp(16); backPosition.bottomMargin = dp(16); root.addView(back, backPosition);
        setContentView(root);
    }

    private void updateStick(float x, float y) {
        stickX = Math.max(-1, Math.min(1, x));
        stickY = Math.max(-1, Math.min(1, y));
        double length = Math.hypot(stickX, stickY);
        if (length > 1) { stickX /= (float)length; stickY /= (float)length; }
        if (movementPad != null) movementPad.invalidate();
    }

    private void scheduleMovementFrame() {
        if (!movementFrameScheduled && activityResumed && !isFinishing()) {
            movementFrameScheduled = true;
            Choreographer.getInstance().postFrameCallback(movementFrame);
        }
    }

    private final class TouchPad extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private int activePointer = -1;
        private boolean requireFreshDown;

        void cancelActiveTouch() {
            activePointer = -1;
            requireFreshDown = true;
            updateStick(0, 0);
        }

        TouchPad() { super(SwampPreviewActivity.this); setFocusable(true); }

        private void setFromTouch(float x, float y) {
            float cx = getWidth() * 0.5f, cy = getHeight() * 0.5f;
            float radius = Math.max(1, Math.min(getWidth(), getHeight()) * 0.36f);
            float sx = (x - cx) / radius;
            float sy = (cy - y) / radius;
            double length = Math.hypot(sx, sy);
            if (length > 1) { sx /= (float)length; sy /= (float)length; }
            updateStick(sx, sy);
        }

        @Override public boolean onTouchEvent(MotionEvent event) {
            int action = event.getActionMasked();
            if (requireFreshDown && action != MotionEvent.ACTION_DOWN) return true;
            if (action == MotionEvent.ACTION_DOWN) {
                requireFreshDown = false;
                activePointer = event.getPointerId(0);
                setFromTouch(event.getX(0), event.getY(0));
                return true;
            }
            if (action == MotionEvent.ACTION_MOVE && activePointer >= 0) {
                int index = event.findPointerIndex(activePointer);
                if (index >= 0) setFromTouch(event.getX(index), event.getY(index));
                return true;
            }
            if (action == MotionEvent.ACTION_CANCEL) {
                activePointer = -1;
                updateStick(0, 0);
                return true;
            }
            if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_POINTER_UP) {
                int index = action == MotionEvent.ACTION_POINTER_UP ? event.getActionIndex() : 0;
                if (activePointer >= 0 && event.getPointerId(index) == activePointer) {
                    activePointer = -1;
                    updateStick(0, 0);
                    if (action == MotionEvent.ACTION_UP) performClick();
                }
                return true;
            }
            return true;
        }

        @Override public boolean performClick() { super.performClick(); return true; }

        @Override protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            float cx = getWidth() * 0.5f, cy = getHeight() * 0.5f;
            float radius = Math.min(getWidth(), getHeight()) * 0.36f;
            paint.setStyle(Paint.Style.FILL); paint.setColor(0x990d1822);
            canvas.drawCircle(cx, cy, radius * 1.28f, paint);
            paint.setStyle(Paint.Style.STROKE); paint.setStrokeWidth(dp(2));
            paint.setColor(0xffa8bdca); canvas.drawCircle(cx, cy, radius, paint);
            paint.setStrokeWidth(dp(1)); paint.setColor(0x997f9aaa);
            canvas.drawLine(cx-radius, cy, cx+radius, cy, paint);
            canvas.drawLine(cx, cy-radius, cx, cy+radius, paint);
            float knobX = cx + stickX * radius, knobY = cy - stickY * radius;
            paint.setStyle(Paint.Style.FILL); paint.setColor(0xff67d6aa);
            canvas.drawCircle(knobX, knobY, radius * 0.3f, paint);
            paint.setColor(Color.WHITE); paint.setTextAlign(Paint.Align.CENTER);
            paint.setTextSize(dp(11)); canvas.drawText("X / Y", cx, getHeight()-dp(5), paint);
        }
    }

    private void startTraceFromSource(Button button) {
        if (traceStarted) return;
        button.setEnabled(false);
        setDetailsVisible(true);
        try {
            String report = startIntroTrace(
                asset("dh2/encounter/common-script-names.bin"),
                asset("dh2/encounter/common-script-programs.bin"),
                asset("dh2/encounter/swamp-script-names.bin"),
                asset("dh2/encounter/swamp-script-programs.bin"),
                asset("dh2/encounter/swamp.mlx"),
                asset("dh2/encounter/swamp-lizard-intro.mgp"));
            if (report == null || !report.startsWith("LizardMan_Intro")) {
                traceStatus.setText(report == null ? "Trace start failed." : report);
                button.setEnabled(true);
                return;
            }
            traceStarted = true;
            traceStatus.setText(report);
            simulationTimeMs = 0;
            accumulatorNanos = 0;
            lastFrameNanos = SystemClock.elapsedRealtimeNanos();
            scheduleTraceFrame();
        } catch (Exception error) {
            traceStatus.setText("Could not start source trace: " + error.getMessage());
            button.setEnabled(true);
        }
    }

    private void scheduleTraceFrame() {
        if (!frameScheduled && traceStarted && !traceComplete && !isFinishing()) {
            frameScheduled = true;
            Choreographer.getInstance().postFrameCallback(traceFrame);
        }
    }

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }
    @Override protected void onPause() {
        activityResumed = false;
        if (movementPad != null) movementPad.cancelActiveTouch();
        else updateStick(0, 0);
        if (movementFrameScheduled) {
            Choreographer.getInstance().removeFrameCallback(movementFrame);
            movementFrameScheduled = false;
        }
        lastMovementFrameNanos = 0;
        movementAccumulatorNanos = 0;
        if (movementReady) {
            String report = stepPlayer(0, 0, 0);
            if (report != null && movementStatus != null) {
                movementStatus.setText(compactMovementStatus(report));
                if (movementDetails != null) movementDetails.setText(report);
            }
        }
        if (frameScheduled) {
            Choreographer.getInstance().removeFrameCallback(traceFrame);
            frameScheduled = false;
        }
        lastFrameNanos = 0;
        if (surface != null) surface.onPause();
        super.onPause();
    }
    @Override protected void onResume() {
        super.onResume();
        if (movementPad != null) movementPad.cancelActiveTouch();
        activityResumed = true;
        lastMovementFrameNanos = 0;
        movementAccumulatorNanos = 0;
        scheduleMovementFrame();
        if (surface != null) surface.onResume();
        if (traceStarted && !traceComplete) {
            lastFrameNanos = SystemClock.elapsedRealtimeNanos();
            scheduleTraceFrame();
        }
    }
    @Override protected void onDestroy() {
        if (frameScheduled) Choreographer.getInstance().removeFrameCallback(traceFrame);
        if (movementFrameScheduled) Choreographer.getInstance().removeFrameCallback(movementFrame);
        destroyIntroTrace();
        destroySwampPreview();
        super.onDestroy();
    }
}
