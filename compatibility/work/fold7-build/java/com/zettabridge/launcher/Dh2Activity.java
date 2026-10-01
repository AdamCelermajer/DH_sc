package com.zettabridge.launcher;

import android.app.*;
import android.content.*;
import android.net.Uri;
import android.os.*;
import android.view.*;
import android.widget.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.*;

/** Single-game setup, local cache import and diagnostics for the user's DH2 APK. */
public class Dh2Activity extends Activity {
    private final ExecutorService worker=Executors.newSingleThreadExecutor();
    private TextView status,location;
    private Button play,importButton;
    private static final int PICK=21;
    private static final String REVISION="dh2-fold7-test2-media-query";
    private File dataRoot() { return new File(getExternalFilesDir(null),"plugins/"+CacheArchive.GAME); }

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        Diagnostics.installCrashRecorder(this);
        LinearLayout panel=new LinearLayout(this); panel.setOrientation(1); panel.setPadding(32,32,32,32);panel.setFitsSystemWindows(true);
        TextView title=new TextView(this);title.setText("Dungeon Hunter 2");title.setTextSize(26);panel.addView(title);
        TextView note=new TextView(this);note.setText("Fold7 test 2 — music-query startup fix\n\nYour imported cache is kept when updating. Import a complete cache ZIP only if needed, then launch. Gameplay has not yet been verified.");panel.addView(note);
        status=new TextView(this);status.setPadding(0,24,0,24);panel.addView(status);
        importButton=new Button(this);importButton.setText("Import cache ZIP");importButton.setOnClickListener(v -> pick());panel.addView(importButton);
        play=new Button(this);play.setText("Launch game");play.setOnClickListener(v -> launchGame());panel.addView(play);
        Button report=new Button(this);report.setText("View / share diagnostic report");report.setOnClickListener(v -> showReport());panel.addView(report);
        location=new TextView(this);location.setTextIsSelectable(true);location.setText("Cache destination:\n"+dataRoot().getAbsolutePath());panel.addView(location);
        TextView credit=new TextView(this);credit.setPadding(0,24,0,0);credit.setText("Uses ZettaBridge and Dynarmic for ARM32 translation. Private compatibility build; upstream notices are included.");panel.addView(credit);
        ScrollView scroll=new ScrollView(this);scroll.addView(panel);setContentView(scroll);
        setBusy(true,"Preparing game and ARM64 runtime...");
        worker.execute(() -> {
            try {
                if (!REVISION.equals(getPreferences(0).getString("prepared", "")) || PluginStore.find(this,CacheArchive.GAME)==null) {
                    stopGuest();
                    File temporary=new File(getCacheDir(),"dh2-bundled.apk");
                    try (InputStream in=getAssets().open("dh2/game.apk"); OutputStream out=new FileOutputStream(temporary)) {
                        byte[] b=new byte[65536];int n;while ((n=in.read(b))!=-1) out.write(b,0,n);
                    }
                    PluginRecord record=PluginStore.importApk(this,Uri.fromFile(temporary));
                    if (!CacheArchive.GAME.equals(record.packageName)) throw new IOException("Unexpected bundled package");
                    temporary.delete();
                    getPreferences(0).edit().putString("prepared",REVISION).apply();
                }
                configurePath();
                runOnUiThread(() -> setBusy(false,getPreferences(0).getBoolean("cacheImported",false)?"Ready. Existing cache retained; launch the game.":"Ready. Import the cache before the first launch."));
            } catch (Throwable e) { failed("Preparation failed",e); }
        });
    }

    private void configurePath() throws IOException {
        File root=dataRoot();if (!root.isDirectory() && !root.mkdirs()) throw new IOException("External game storage is unavailable");
        getSharedPreferences(CacheArchive.GAME+"__DungeonHunter2Prefs",0).edit().putString("SDFolder",root.getAbsolutePath()).commit();
    }
    private void setBusy(boolean busy,String text) { play.setEnabled(!busy);importButton.setEnabled(!busy);status.setText(text); }
    private void pick() {
        Intent i=new Intent(Intent.ACTION_OPEN_DOCUMENT).addCategory(Intent.CATEGORY_OPENABLE).setType("*/*");
        i.putExtra(Intent.EXTRA_MIME_TYPES,new String[]{"application/zip","application/x-zip-compressed","application/octet-stream"});startActivityForResult(i,PICK);
    }
    @Override protected void onActivityResult(int request,int result,Intent data) {
        super.onActivityResult(request,result,data);
        if (request!=PICK || result!=RESULT_OK || data==null || data.getData()==null) return;
        Uri selected=data.getData();stopGuest();setBusy(true,"Importing cache. Keep this screen open...");
        worker.execute(() -> {
            try (InputStream in=getContentResolver().openInputStream(selected)) {
                if (in==null) throw new IOException("Cannot open selected archive");
                final long[] last={0};
                CacheArchive.Result r=CacheArchive.install(in,dataRoot(),(bytes,files)->{
                    long now=SystemClock.elapsedRealtime();if(now-last[0]>500){last[0]=now;runOnUiThread(()->status.setText("Importing: "+files+" files, "+(bytes/(1024*1024))+" MiB"));}
                });
                configurePath();getPreferences(0).edit().putBoolean("cacheImported",true).apply();
                runOnUiThread(()->setBusy(false,"Imported "+r.files+" files ("+(r.bytes/(1024*1024))+" MiB). Ready to launch."));
            } catch(Exception e) { failed("Cache import failed",e); }
        });
    }
    private void launchGame() {
        try {
            long page=android.system.Os.sysconf(android.system.OsConstants._SC_PAGESIZE);
            if(page!=4096)throw new IOException("This translation runtime currently requires 4096-byte memory pages. This phone reports "+page+". Please share the diagnostic report.");
            configurePath();startActivity(PluginSwitchActivity.intent(this,CacheArchive.GAME));
        }
        catch(Exception e){failed("Launch failed",e);}
    }
    private void stopGuest() {
        ActivityManager am=(ActivityManager)getSystemService(ACTIVITY_SERVICE);
        java.util.List<ActivityManager.RunningAppProcessInfo> list=am.getRunningAppProcesses();
        if(list!=null)for(ActivityManager.RunningAppProcessInfo p:list)
            if(p.uid==android.os.Process.myUid() && p.processName.equals(getPackageName()+":guest")) android.os.Process.killProcess(p.pid);
    }
    private void failed(String label,Throwable e) {
        Diagnostics.report(this,label,e,false);
        runOnUiThread(()->setBusy(false,label+": "+e.getMessage()+"\nUse the diagnostic report for details."));
    }
    private void showReport() {
        StringBuilder b=new StringBuilder("DH2 Fold7 test 2 (versionCode 2)\nModel: "+Build.MODEL+"\nAndroid: "+Build.VERSION.RELEASE+"\nABIs: "+java.util.Arrays.toString(Build.SUPPORTED_ABIS)+"\nPage size: "+android.system.Os.sysconf(android.system.OsConstants._SC_PAGESIZE)+"\nCache: "+dataRoot()+"\n\n");
        File media=new File(dataRoot(),"dh2-media-status.txt");
        try{b.append("dh2-media-status.txt:\n").append(new String(java.nio.file.Files.readAllBytes(media.toPath()),StandardCharsets.UTF_8)).append("\n");}
        catch(IOException ignored){b.append("No test 2 media query report yet.\n\n");}
        for(String name:new String[]{"zb-runtime-report.txt","zb-errors.txt"}){
            File f=new File(getExternalFilesDir(null),name);b.append(name).append(":\n");
            try{byte[] all=java.nio.file.Files.readAllBytes(f.toPath());int start=Math.max(0,all.length-120000);b.append(new String(all,start,all.length-start,StandardCharsets.UTF_8));}
            catch(IOException e){b.append("No report recorded yet.\n");}b.append("\n");
        }
        String text=b.toString();TextView view=new TextView(this);view.setText(text);view.setTextIsSelectable(true);view.setPadding(24,12,24,12);
        ScrollView scroll=new ScrollView(this);scroll.addView(view);
        new AlertDialog.Builder(this).setTitle("Diagnostic report").setView(scroll).setPositiveButton("Share",(d,w)->startActivity(Intent.createChooser(new Intent(Intent.ACTION_SEND).setType("text/plain").putExtra(Intent.EXTRA_TEXT,text),"Share report"))).setNegativeButton("Close",null).show();
    }
    @Override protected void onDestroy(){worker.shutdownNow();super.onDestroy();}
}
