package com.gameloft.android.GAND.GloftD2SS;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.text.InputType;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.PixelCopy;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;

/** Track B addition: the user-facing side of the session log (share, copy summary, floating report bubble, report dialog with text + screenshot). */
public final class SessionUi {
    private static boolean dialogOpen;

    private SessionUi() {
    }

    /** Opens the chooser with dh2_report.zip (README with the marks, session logs, crash files, marks screenshots). */
    public static void share(Context c) {
        try {
            File zip = SessionLog.buildBundle(c);
            if (zip == null) {
                Toast.makeText(c, "No session log available", Toast.LENGTH_LONG).show();
                return;
            }
            Uri uri = Uri.parse("content://" + SessionProvider.AUTHORITY + "/" + SessionProvider.NAME);
            Intent send = new Intent(Intent.ACTION_SEND);
            send.setType("application/zip");
            send.putExtra(Intent.EXTRA_STREAM, uri);
            send.putExtra(Intent.EXTRA_SUBJECT, "Dungeon Hunter 2 session log");
            send.putExtra(Intent.EXTRA_TEXT, "Dungeon Hunter 2 (Track B) session report attached (" + zip.length() + " bytes).");
            send.setClipData(ClipData.newRawUri("dh2 report", uri));
            send.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            Intent chooser = Intent.createChooser(send, "Share session log");
            chooser.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            if (!(c instanceof Activity)) {
                chooser.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            }
            c.startActivity(chooser);
        } catch (Throwable t) {
            Toast.makeText(c, "Could not share: " + t, Toast.LENGTH_LONG).show();
        }
    }

    public static void copySummary(Context c) {
        try {
            ((ClipboardManager) c.getSystemService(Context.CLIPBOARD_SERVICE)).setPrimaryClip(ClipData.newPlainText("dh2 session", SessionLog.summary(c)));
            Toast.makeText(c, "Summary copied", Toast.LENGTH_SHORT).show();
        } catch (Throwable t) {
            Toast.makeText(c, "Could not copy: " + t, Toast.LENGTH_LONG).show();
        }
    }

    private static int dp(Context c, int v) {
        return (int) (v * c.getResources().getDisplayMetrics().density + 0.5f);
    }

    /**
     * Adds the always-visible floating report bubble (semi-transparent, draggable; a tap opens the report dialog) over the game view.
     * It is an ordinary child view of the same window, so touches on it never reach the GL view's touch handler.
     */
    public static void attachBubble(final Activity a, final FrameLayout root, final GameGLSurfaceView game) {
        final int size = dp(a, 36);
        final TextView b = new TextView(a);
        b.setText("!");
        b.setTextColor(Color.WHITE);
        b.setTextSize(18);
        b.setGravity(Gravity.CENTER);
        GradientDrawable bg = new GradientDrawable();
        bg.setShape(GradientDrawable.OVAL);
        bg.setColor(0xCC555555);
        bg.setStroke(dp(a, 2), 0xFFFFFFFF);
        b.setBackground(bg);
        b.setAlpha(0.4f);
        b.setContentDescription("Report an issue");
        FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(size, size, Gravity.TOP | Gravity.LEFT);
        android.content.SharedPreferences sp = a.getSharedPreferences("dh2bubble", 0);
        lp.leftMargin = sp.getInt("x", dp(a, 4));
        lp.topMargin = sp.getInt("y", dp(a, 120));
        root.addView(b, lp);
        final float[] st = new float[5];   // downRawX, downRawY, startLeft, startTop, moved
        b.setOnTouchListener(new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent e) {
                FrameLayout.LayoutParams p = (FrameLayout.LayoutParams) v.getLayoutParams();
                switch (e.getActionMasked()) {
                    case MotionEvent.ACTION_DOWN:
                        st[0] = e.getRawX();
                        st[1] = e.getRawY();
                        st[2] = p.leftMargin;
                        st[3] = p.topMargin;
                        st[4] = 0;
                        v.setAlpha(0.9f);
                        return true;
                    case MotionEvent.ACTION_MOVE: {
                        float dx = e.getRawX() - st[0];
                        float dy = e.getRawY() - st[1];
                        if (st[4] == 0 && Math.abs(dx) + Math.abs(dy) > dp(a, 8)) {
                            st[4] = 1;
                        }
                        if (st[4] == 1) {
                            p.leftMargin = Math.max(0, Math.min(root.getWidth() - v.getWidth(), (int) (st[2] + dx)));
                            p.topMargin = Math.max(0, Math.min(root.getHeight() - v.getHeight(), (int) (st[3] + dy)));
                            v.setLayoutParams(p);
                        }
                        return true;
                    }
                    case MotionEvent.ACTION_UP:
                        v.setAlpha(0.4f);
                        if (st[4] == 1) {
                            a.getSharedPreferences("dh2bubble", 0).edit().putInt("x", p.leftMargin).putInt("y", p.topMargin).apply();
                        } else {
                            showGameDialog(a, game);
                        }
                        return true;
                    case MotionEvent.ACTION_CANCEL:
                        v.setAlpha(0.4f);
                        return true;
                    default:
                        return true;
                }
            }
        });
    }

    /**
     * The in-game report dialog (bubble tap or three-finger long press). A screenshot of the game surface is taken first (PixelCopy), then the dialog
     * opens; its window takes focus from the game, whose focus handler (GameGLSurfaceView.onWindowFocusChanged) calls nativePause(1) and, when the
     * dialog goes away, nativeResume(1). Held touches are released to the game first so no finger stays "down" across the pause.
     */
    public static void showGameDialog(final Activity a, final GLSurfaceView game) {
        if (a == null || dialogOpen) {
            return;
        }
        dialogOpen = true;
        if (game instanceof GameGLSurfaceView) {
            ((GameGLSurfaceView) game).releaseTouches();
        }
        int w = game != null ? game.getWidth() : 0;
        int h = game != null ? game.getHeight() : 0;
        if (w <= 0 || h <= 0) {
            buildDialog(a, null);
            return;
        }
        try {
            final Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
            PixelCopy.request(game, bmp, new PixelCopy.OnPixelCopyFinishedListener() {
                public void onPixelCopyFinished(int result) {
                    buildDialog(a, result == PixelCopy.SUCCESS ? bmp : null);
                }
            }, new Handler(Looper.getMainLooper()));
        } catch (Throwable t) {
            buildDialog(a, null);
        }
    }

    private static void buildDialog(final Activity a, final Bitmap shot) {
        try {
            final EditText text = new EditText(a);
            text.setHint("Describe the issue (what happened, what you expected)");
            text.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_TEXT_FLAG_CAP_SENTENCES);
            text.setMinLines(3);
            text.setMaxLines(6);
            text.setGravity(Gravity.TOP | Gravity.LEFT);
            FrameLayout box = new FrameLayout(a);
            box.setPadding(dp(a, 16), dp(a, 4), dp(a, 16), 0);
            box.addView(text, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            AlertDialog d = new AlertDialog.Builder(a)
                    .setTitle("Report an issue")
                    .setMessage(shot != null ? "The game is paused. A screenshot of this moment is attached." : "The game is paused.")
                    .setView(box)
                    .setPositiveButton("Save mark", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface di, int w) {
                            int n = SessionLog.mark(text.getText().toString(), shot);
                            Toast.makeText(a, "Saved mark #" + n, Toast.LENGTH_SHORT).show();
                        }
                    })
                    .setNeutralButton("Share log", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface di, int w) {
                            if (text.getText().toString().trim().length() > 0) {
                                SessionLog.mark(text.getText().toString(), shot);
                            }
                            share(a);
                        }
                    })
                    .setNegativeButton("Cancel", null)
                    .setOnDismissListener(new DialogInterface.OnDismissListener() {
                        public void onDismiss(DialogInterface di) {
                            dialogOpen = false;
                        }
                    })
                    .create();
            d.getWindow().setSoftInputMode(android.view.WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
            d.show();
        } catch (Throwable t) {
            dialogOpen = false;
        }
    }
}
