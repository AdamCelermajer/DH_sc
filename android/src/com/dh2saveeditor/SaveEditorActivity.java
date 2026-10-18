package com.dh2saveeditor;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Process;
import android.util.Base64;
import android.webkit.JavascriptInterface;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * DH2 Save Editor (save_editor_dh2, not part of the game sources): the "DH2 Save Editor" launcher entry that
 * tools/build_modded_apk.py injects into a Track B APK (its own dex + a manifest with this activity added). It runs in its own process
 * (android:process=":editor") of the game's package, so it reads and writes the game's own files directory
 * (getExternalFilesDir(null) = /sdcard/Android/data/<pkg>/files, where the game keeps dh2_NNN.savegame) without any
 * storage permission (Android 11+ lets no other app into Android/data, so the editor has to be inside the game's
 * package). The UI is the shared editor (assets/dh2editor = save_editor_dh2/web plus the generated itemdata.js, the
 * same files as the Windows app). This class is only the file bridge (window.AndroidHost):
 *  - it lists the player saves (dh2_NNN.savegame) and their checkpoint copies;
 *  - it refuses every write while the game process (the package's main process) exists: the game keeps the hero in
 *    memory and would write it back over the edit. The UI offers to stop it;
 *  - before a write it copies the file to files/editor-backups/<name>.<date> and to <name>.bak (the game's own backup
 *    name, Savegame::UpdateJobs), then writes the new bytes to a temporary file and renames it over the save.
 */
public class SaveEditorActivity extends Activity {
    private static final Pattern SAVE = Pattern.compile("dh2_\\d{3}\\.savegame");
    private static final Pattern CHECKPOINT = Pattern.compile("dh2_\\d{3}_(single|multi)\\.checkpoint");
    private WebView web;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        web = new WebView(this);
        web.setBackgroundColor(Color.rgb(0x15, 0x12, 0x0f));
        WebSettings s = web.getSettings();
        s.setJavaScriptEnabled(true);
        s.setDomStorageEnabled(true);
        s.setAllowFileAccess(true);   // file:///android_asset only; no network content is loaded
        web.setWebViewClient(new WebViewClient());
        web.addJavascriptInterface(new Bridge(), "AndroidHost");
        setContentView(web);
        web.loadUrl("file:///android_asset/dh2editor/index.html");
    }

    @Override
    public void onBackPressed() {
        if (web != null && web.canGoBack()) web.goBack();
        else finish();
    }

    private File dir() {
        return getExternalFilesDir(null);
    }

    /** The game's main process (process name = package name), or 0 when it is not running. */
    private int gamePid() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> list = am.getRunningAppProcesses();
        if (list == null) return 0;
        for (ActivityManager.RunningAppProcessInfo p : list) {
            if (getPackageName().equals(p.processName) && p.pid != Process.myPid()) return p.pid;
        }
        return 0;
    }

    private static String json(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') b.append('\\').append(c);
            else if (c < 0x20) b.append(String.format(Locale.ROOT, "\\u%04x", (int) c));
            else b.append(c);
        }
        return b.append('"').toString();
    }

    private static String err(String m) {
        return "{\"error\":" + json(m) + "}";
    }

    private static byte[] readAll(File f) throws IOException {
        byte[] d = new byte[(int) f.length()];
        try (InputStream in = new FileInputStream(f)) {
            int o = 0;
            while (o < d.length) {
                int n = in.read(d, o, d.length - o);
                if (n < 0) throw new IOException("short read " + f);
                o += n;
            }
        }
        return d;
    }

    private static void writeAll(File f, byte[] d) throws IOException {
        try (FileOutputStream out = new FileOutputStream(f)) {
            out.write(d);
            out.getFD().sync();
        }
    }

    private File named(String name) throws IOException {
        if (!SAVE.matcher(name).matches() && !CHECKPOINT.matcher(name).matches()) throw new IOException("not a save file: " + name);
        return new File(dir(), name);
    }

    final class Bridge {
        @JavascriptInterface
        public String listSaves() {
            File d = dir();
            if (d == null) return err("the game's files folder is not available");
            String[] names = d.list();
            if (names == null) names = new String[0];
            Arrays.sort(names);
            StringBuilder b = new StringBuilder("{\"files\":[");
            boolean first = true;
            SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.ROOT);
            for (String n : names) {
                if (!SAVE.matcher(n).matches()) continue;
                File f = new File(d, n);
                StringBuilder cps = new StringBuilder();
                for (String cp : new String[] {n.substring(0, 7) + "_single.checkpoint", n.substring(0, 7) + "_multi.checkpoint"}) {
                    if (new File(d, cp).isFile()) cps.append(cps.length() > 0 ? "," : "").append(json(cp));
                }
                b.append(first ? "" : ",").append("{\"name\":").append(json(n)).append(",\"path\":").append(json(n))
                        .append(",\"info\":").append(json(f.length() + " bytes, " + fmt.format(new Date(f.lastModified()))))
                        .append(",\"checkpoints\":[").append(cps).append("]}");
                first = false;
            }
            return b.append("],\"gameRunning\":").append(gamePid() != 0).append("}").toString();
        }

        @JavascriptInterface
        public String read(String name) {
            try {
                return "{\"data\":\"" + Base64.encodeToString(readAll(named(name)), Base64.NO_WRAP) + "\"}";
            } catch (IOException e) {
                return err(String.valueOf(e.getMessage()));
            }
        }

        @JavascriptInterface
        public String write(String name, String b64) {
            if (gamePid() != 0) {
                return err("The game is still running (also in the background). Use \"Stop the game\" or swipe it away from recent apps, then save again.");
            }
            try {
                File f = named(name);
                byte[] bytes = Base64.decode(b64, Base64.DEFAULT);
                if (bytes.length < 8) return err("bad data");
                File bdir = new File(dir(), "editor-backups");
                if (!bdir.isDirectory() && !bdir.mkdirs()) return err("cannot create " + bdir);
                String backup = "";
                if (f.isFile()) {
                    byte[] old = readAll(f);
                    File dated = new File(bdir, name + "." + new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.ROOT).format(new Date()));
                    writeAll(dated, old);
                    writeAll(new File(dir(), name + ".bak"), old);
                    backup = dated.getPath();
                }
                File tmp = new File(bdir, name + ".new");
                writeAll(tmp, bytes);
                if (!tmp.renameTo(f)) return err("could not replace " + name + " (backup: " + backup + ")");
                if (!Arrays.equals(readAll(f), bytes)) return err("the file does not match after writing (backup: " + backup + ")");
                return "{\"ok\":true,\"note\":\"written\",\"backup\":" + json(backup) + "}";
            } catch (IOException e) {
                return err(String.valueOf(e.getMessage()));
            }
        }

        @JavascriptInterface
        public String gameRunning() {
            return "{\"running\":" + (gamePid() != 0) + "}";
        }

        /** Ends the game's process (same app, same user id) so that a save can be written safely. */
        @JavascriptInterface
        public String stopGame() {
            int pid = gamePid();
            if (pid != 0) Process.killProcess(pid);
            return "{\"ok\":true,\"stopped\":" + (pid != 0) + "}";
        }

        @JavascriptInterface
        public String launchGame() {
            runOnUiThread(() -> {
                // the game's launcher activity (Track B DataSetup: unpacks data on first run, then starts the game)
                Intent i = new Intent().setComponent(new ComponentName(getPackageName(), "com.gameloft.android.GAND.GloftD2SS.DataSetup"));
                i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(i);
                finish();
            });
            return "{\"ok\":true}";
        }
    }
}
