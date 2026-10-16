package com.gameloft.android.GAND.GloftD2SS;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Track B addition (not in the original dex): launcher activity that makes the installed build playable without
 * adb steps. The original game expects its data (about 650 MB) in <external>/Android/data/<pkg>/files, put there by
 * Gameloft's downloader (GameInstaller), whose servers are gone. This activity looks for the user's own data archive
 * (Dungeon-Hunter-2-HD-v1-0-2-cache.zip, or an expansion file renamed to *.obb) and unpacks it into that directory,
 * then starts the game activity (DungeonHunter2) exactly as the DRM launcher would after a successful check.
 *
 * Searched, in this order: getObbDir() (/sdcard/Android/obb/<pkg>/*.obb|*.zip), getExternalFilesDir()'s parent
 * (/sdcard/Android/data/<pkg>/*.zip), /sdcard/Download/*.zip, /sdcard/*.zip. A zip whose entries start with
 * ".../files/" is unpacked relative to that "files/"; any other zip is unpacked as the content of files/ itself.
 * Savegames/settings inside the archive are skipped, so a fresh install starts a clean profile.
 */
public class DataSetup extends Activity {
    private static final int REQ_STORAGE = 77;
    private TextView text;
    private ProgressBar bar;
    private volatile boolean working;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setGravity(Gravity.CENTER);
        root.setBackgroundColor(Color.BLACK);
        root.setPadding(48, 48, 48, 48);
        text = new TextView(this);
        text.setTextColor(Color.WHITE);
        text.setTextSize(18);
        text.setGravity(Gravity.CENTER);
        bar = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        bar.setMax(1000);
        bar.setVisibility(View.GONE);
        root.addView(text);
        root.addView(bar, new LinearLayout.LayoutParams(600, LinearLayout.LayoutParams.WRAP_CONTENT));
        setContentView(root);
        proceed();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!working && text != null && text.getText().length() > 0 && !dataReady()) {
            proceed();   // the user may have copied the archive while this screen was open
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        proceed();
    }

    private File filesDir() {
        File f = getExternalFilesDir(null);
        if (f == null) {
            f = new File("/sdcard/Android/data/" + getPackageName() + "/files");
        }
        f.mkdirs();
        return f;
    }

    private boolean dataReady() {
        File files = filesDir();
        if (new File(files, ".dh2_extracting").exists()) {
            return false;
        }
        return new File(files, "d_o_w_n_l_o_a_d_e_d.txt").exists() && new File(files, "data").isDirectory();
    }

    private void proceed() {
        if (working) {
            return;
        }
        if (dataReady()) {
            startGame();
            return;
        }
        if (Build.VERSION.SDK_INT >= 23 && checkSelfPermission("android.permission.READ_EXTERNAL_STORAGE") != PackageManager.PERMISSION_GRANTED
                && getSharedPreferences("dh2setup", 0).getInt("asked", 0) == 0) {
            getSharedPreferences("dh2setup", 0).edit().putInt("asked", 1).commit();
            requestPermissions(new String[]{"android.permission.READ_EXTERNAL_STORAGE", "android.permission.READ_PHONE_STATE"}, REQ_STORAGE);
            return;
        }
        final File src = findArchive();
        if (src == null) {
            text.setText("Dungeon Hunter 2 data not found.\n\nCopy Dungeon-Hunter-2-HD-v1-0-2-cache.zip to\n"
                    + getObbDir().getPath() + "/ (as main." + versionCode() + "." + getPackageName() + ".obb)\nor to the Download folder, then come back to this screen.");
            return;
        }
        working = true;
        bar.setVisibility(View.VISIBLE);
        text.setText("Preparing game data from\n" + src.getName() + "\n(first start only)");
        new Thread(new Runnable() {
            public void run() {
                String err = null;
                try {
                    extract(src, filesDir());
                } catch (Throwable t) {
                    err = t.toString();
                }
                final String e = err;
                runOnUiThread(new Runnable() {
                    public void run() {
                        working = false;
                        if (e != null) {
                            text.setText("Could not unpack the game data:\n" + e);
                        } else {
                            startGame();
                        }
                    }
                });
            }
        }).start();
    }

    private int versionCode() {
        try {
            return getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
        } catch (Exception e) {
            return 102;
        }
    }

    private void startGame() {
        startActivity(new Intent(this, DungeonHunter2.class));
        finish();
    }

    private File findArchive() {
        List<File> dirs = new ArrayList<File>();
        dirs.add(getObbDir());
        File ext = getExternalFilesDir(null);
        if (ext != null && ext.getParentFile() != null) {
            dirs.add(ext.getParentFile());
        }
        File sd = Environment.getExternalStorageDirectory();
        dirs.add(new File(sd, "Download"));
        dirs.add(sd);
        for (File d : dirs) {
            File[] list = d == null ? null : d.listFiles();
            if (list == null) {
                continue;
            }
            for (File f : list) {
                String n = f.getName().toLowerCase();
                if (f.isFile() && (n.endsWith(".obb") || n.endsWith(".zip")) && f.length() > 50L * 1024 * 1024) {
                    if (looksLikeData(f)) {
                        return f;
                    }
                }
            }
        }
        return null;
    }

    private static boolean looksLikeData(File f) {
        ZipFile z = null;
        try {
            z = new ZipFile(f);
            Enumeration<? extends ZipEntry> en = z.entries();
            int n = 0;
            while (en.hasMoreElements() && n++ < 200000) {
                String name = en.nextElement().getName();
                if (name.endsWith("d_o_w_n_l_o_a_d_e_d.txt") || name.contains("files/data/") || name.startsWith("data/3d/")) {
                    return true;
                }
            }
        } catch (Exception e) {
            // not a zip
        } finally {
            try {
                if (z != null) {
                    z.close();
                }
            } catch (Exception e) {
            }
        }
        return false;
    }

    private void extract(File archive, File dest) throws Exception {
        File flag = new File(dest, ".dh2_extracting");
        flag.createNewFile();
        ZipFile z = new ZipFile(archive);
        try {
            String prefix = "";
            long total = 0;
            Enumeration<? extends ZipEntry> en = z.entries();
            while (en.hasMoreElements()) {
                ZipEntry e = en.nextElement();
                total += e.getSize();
                int i = e.getName().indexOf("/files/");
                if (i >= 0 && prefix.length() == 0 && e.getName().startsWith(e.getName().substring(0, i + 1))) {
                    prefix = e.getName().substring(0, i + 7);
                }
            }
            final long all = Math.max(total, 1);
            long done = 0;
            String destCanon = dest.getCanonicalPath() + File.separator;
            byte[] buf = new byte[1 << 16];
            en = z.entries();
            int lastPct = -1;
            while (en.hasMoreElements()) {
                ZipEntry e = en.nextElement();
                String name = e.getName();
                if (prefix.length() > 0) {
                    if (!name.startsWith(prefix)) {
                        continue;
                    }
                    name = name.substring(prefix.length());
                }
                if (name.length() == 0) {
                    continue;
                }
                File out = new File(dest, name);
                if (!out.getCanonicalPath().startsWith(destCanon)) {
                    continue;   // path traversal
                }
                if (e.isDirectory()) {
                    out.mkdirs();
                    continue;
                }
                if (name.indexOf('/') < 0 && isUserState(name)) {
                    done += e.getSize();
                    continue;
                }
                out.getParentFile().mkdirs();
                InputStream in = z.getInputStream(e);
                OutputStream os = new FileOutputStream(out);
                try {
                    int r;
                    while ((r = in.read(buf)) > 0) {
                        os.write(buf, 0, r);
                        done += r;
                    }
                } finally {
                    os.close();
                    in.close();
                }
                final int pct = (int) (done * 1000 / all);
                if (pct != lastPct) {
                    lastPct = pct;
                    runOnUiThread(new Runnable() {
                        public void run() {
                            bar.setProgress(pct);
                        }
                    });
                }
            }
        } finally {
            z.close();
        }
        flag.delete();
    }

    /** Files of the developer's profile inside the cache zip: savegames, checkpoints and the settings (they pin the UI language). */
    private static boolean isUserState(String n) {
        return n.endsWith(".savegame") || n.endsWith(".bak") || n.contains(".checkpoint") || n.equals("dh2_settings.savegame") || n.equals("InsTime");
    }
}
