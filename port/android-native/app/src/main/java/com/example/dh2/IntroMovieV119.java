package com.example.dh2;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.view.Gravity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Button;
import android.widget.FrameLayout;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/** One actual GS intro delivery; no completion is inferred from elapsed time. */
final class IntroMovieV119 {
    private final Activity activity;
    private final FrameLayout parent;
    private final ExecutorService io=Executors.newSingleThreadExecutor();
    private FrameLayout overlay;
    private SurfaceView surface;
    private MediaPlayer player;
    private AssetFileDescriptor descriptor;
    private File staged;
    private long generation;
    private boolean terminal=true,prepared,paused,started;

    IntroMovieV119(Activity activity,FrameLayout parent){this.activity=activity;this.parent=parent;}
    void show(long request,String name){
        if(!terminal){NativeBridge.introMovieEventV119(request,3,"Intro UI already owns a live movie");return;}
        generation=request;terminal=false;prepared=false;
        overlay=new FrameLayout(activity);overlay.setBackgroundColor(Color.BLACK);overlay.setClickable(true);
        surface=new SurfaceView(activity);surface.setZOrderMediaOverlay(true);
        overlay.addView(surface,new FrameLayout.LayoutParams(-1,-1));
        Button skip=new Button(activity);skip.setText("Skip");skip.setContentDescription("Skip intro movie");skip.setOnClickListener(v->finish(2,null));
        FrameLayout.LayoutParams button=new FrameLayout.LayoutParams(-2,-2,Gravity.BOTTOM|Gravity.RIGHT);overlay.addView(skip,button);
        parent.addView(overlay,new FrameLayout.LayoutParams(-1,-1));overlay.bringToFront();
        surface.getHolder().addCallback(new SurfaceHolder.Callback(){
            public void surfaceCreated(SurfaceHolder holder){try{if(player!=null){player.setDisplay(holder);startIfReady();}else prepareIfReady();}catch(IllegalStateException error){finish(3,error.toString());}}
            public void surfaceChanged(SurfaceHolder holder,int format,int width,int height){}
            public void surfaceDestroyed(SurfaceHolder holder){try{if(player!=null){if(started){player.pause();started=false;}player.setDisplay(null);}}catch(IllegalStateException error){finish(3,error.toString());}}
        });
        io.execute(()->{
            AssetFileDescriptor fd=null;File file=null;
            try{
                try{fd=activity.getAssets().openFd("original-media/"+name);if(fd.getLength()<=0||fd.getLength()>64L*1024*1024)throw new java.io.IOException("Intro direct media exceeds admitted size");}
                catch(java.io.FileNotFoundException compressedOrMissing){
                    file=new File(activity.getCacheDir(),"source-intro-v119-"+request+".mp4");File part=new File(activity.getCacheDir(),file.getName()+".part");
                    try(FileOutputStream out=new FileOutputStream(part)){
                        InputStream asset=null;try{asset=activity.getAssets().open("original-media/"+name);}catch(java.io.FileNotFoundException absent){}
                        if(asset!=null){try(InputStream input=asset){byte[] buffer=new byte[65536];long total=0;int count;while((count=input.read(buffer))!=-1){if(Thread.currentThread().isInterrupted())throw new java.io.IOException("Intro delivery cancelled");total+=count;if(total>64L*1024*1024)throw new java.io.IOException("Intro media exceeds 64MiB admission");out.write(buffer,0,count);}if(total==0)throw new java.io.IOException("Empty actual intro media");}}
                        else{byte[] bytes=NativeBridge.introMovieCacheV119(request);if(bytes==null||bytes.length==0)throw new java.io.IOException("Missing original intro media: "+name);if(bytes.length>64*1024*1024)throw new java.io.IOException("Intro cache exceeds admitted size");out.write(bytes);}
                        out.getFD().sync();
                    }catch(Exception failure){part.delete();throw failure;}
                    if(!part.renameTo(file)){part.delete();throw new java.io.IOException("Intro private media publication failed");}
                }
                final AssetFileDescriptor delivered=fd;final File media=file;
                activity.runOnUiThread(()->{if(terminal||generation!=request){close(delivered);if(media!=null)media.delete();return;}descriptor=delivered;staged=media;prepareIfReady();});
            }catch(Exception error){close(fd);if(file!=null)file.delete();final String message=error.toString();activity.runOnUiThread(()->{if(!terminal&&generation==request)finish(3,message);});}
        });
    }
    private void prepareIfReady(){
        if(terminal||player!=null||surface==null||!surface.getHolder().getSurface().isValid()||(descriptor==null&&staged==null))return;
        try{
            player=new MediaPlayer();player.setDisplay(surface.getHolder());
            player.setOnPreparedListener(actual->{if(terminal||actual!=player)return;prepared=true;NativeBridge.introMovieEventV119(generation,0,null);startIfReady();});
            player.setOnCompletionListener(actual->{if(actual==player&&!terminal)finish(1,null);});
            player.setOnErrorListener((actual,what,extra)->{if(actual==player&&!terminal)finish(3,"Android intro MediaPlayer error "+what+"/"+extra);return true;});
            if(descriptor!=null){player.setDataSource(descriptor.getFileDescriptor(),descriptor.getStartOffset(),descriptor.getLength());close(descriptor);descriptor=null;}
            else player.setDataSource(staged.getAbsolutePath());
            player.prepareAsync();
        }catch(Exception error){finish(3,error.toString());}
    }
    private void startIfReady(){
        if(terminal||paused||!prepared||started||player==null||surface==null||!surface.getHolder().getSurface().isValid())return;
        try{player.start();started=true;}catch(IllegalStateException error){finish(3,error.toString());}
    }
    private static void close(AssetFileDescriptor fd){if(fd!=null)try{fd.close();}catch(Exception ignored){}}
    private void finish(int result,String error){
        if(terminal)return;terminal=true;
        if(player!=null){player.setOnPreparedListener(null);player.setOnCompletionListener(null);player.setOnErrorListener(null);player.release();player=null;}started=false;
        close(descriptor);descriptor=null;if(staged!=null){staged.delete();staged=null;}
        if(overlay!=null){parent.removeView(overlay);overlay=null;}surface=null;
        NativeBridge.introMovieEventV119(generation,result,error);
    }
    void pause(){paused=true;if(started&&player!=null&&!terminal)try{player.pause();started=false;}catch(IllegalStateException error){finish(3,error.toString());}}
    void resume(){paused=false;startIfReady();}
    void skip(){if(!terminal)finish(2,null);}
    boolean visible(){return !terminal;}
    void destroy(){if(!terminal)finish(3,"Actual intro Activity destroyed before completion");io.shutdownNow();}
}
