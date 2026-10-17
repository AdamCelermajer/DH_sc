package com.gameloft.android.GAND.GloftD2SS;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.widget.Toast;
import java.io.File;

/** Track B addition: the user-facing side of the session log (share, copy summary, mark issue, and the in-game dialog). */
public final class SessionUi {
    private static boolean dialogOpen;

    private SessionUi() {
    }

    /** Opens the chooser with dh2_report.zip (session logs + crash files). */
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

    /** The in-game dialog (three-finger long press). Losing window focus pauses the game through the existing focus handler. */
    public static void showGameDialog(final Activity a) {
        if (a == null || dialogOpen) {
            return;
        }
        dialogOpen = true;
        try {
            new AlertDialog.Builder(a)
                    .setTitle("Session log")
                    .setMessage("Share the session log, or mark this moment so it can be found in the log.")
                    .setPositiveButton("Share session log", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface d, int w) {
                            share(a);
                        }
                    })
                    .setNeutralButton("Mark issue", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface d, int w) {
                            int n = SessionLog.mark();
                            Toast.makeText(a, "Marked issue #" + n, Toast.LENGTH_SHORT).show();
                        }
                    })
                    .setNegativeButton("Close", null)
                    .setOnDismissListener(new DialogInterface.OnDismissListener() {
                        public void onDismiss(DialogInterface d) {
                            dialogOpen = false;
                        }
                    })
                    .show();
        } catch (Throwable t) {
            dialogOpen = false;
        }
    }
}
