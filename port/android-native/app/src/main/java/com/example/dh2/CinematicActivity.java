package com.example.dh2;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetFileDescriptor;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

/** Modern Android video owner for the byte-identical original intro resource. */
public final class CinematicActivity extends Activity implements TextureView.SurfaceTextureListener {
    private TextureView video;
    private MediaPlayer player;
    private Surface videoSurface;
    private AudioManager audio;
    private AudioFocusRequest focus;
    private boolean prepared, resumed, completed, focusHeld, firstFrame;
    private int position, videoWidth, videoHeight;
    private TextView error;

    @Override public void onCreate(Bundle state) {
        setTheme(android.R.style.Theme_Material_NoActionBar);
        super.onCreate(state);
        if(android.os.Build.VERSION.SDK_INT>=33)
            getOnBackInvokedDispatcher().registerOnBackInvokedCallback(
                android.window.OnBackInvokedDispatcher.PRIORITY_DEFAULT,()->complete("back"));
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        getWindow().getDecorView().setSystemUiVisibility(5894);
        position=state==null?0:state.getInt("position",0);
        FrameLayout root=new FrameLayout(this);root.setBackgroundColor(android.graphics.Color.BLACK);
        video=new TextureView(this);video.setSurfaceTextureListener(this);
        root.addView(video,new FrameLayout.LayoutParams(-1,-1));
        Button skip=new Button(this);skip.setText("Skip");skip.setContentDescription("Skip intro cinematic");
        FrameLayout.LayoutParams button=new FrameLayout.LayoutParams(-2,-2,Gravity.RIGHT|Gravity.BOTTOM);
        int margin=(int)(16*getResources().getDisplayMetrics().density);button.setMargins(margin,margin,margin,margin);
        root.addView(skip,button);skip.setOnClickListener(v->complete("skipped"));
        error=new TextView(this);error.setTextColor(android.graphics.Color.WHITE);error.setVisibility(View.GONE);
        root.addView(error,new FrameLayout.LayoutParams(-2,-2,Gravity.CENTER));setContentView(root);
        audio=(AudioManager)getSystemService(AUDIO_SERVICE);
        if(android.os.Build.VERSION.SDK_INT>=26)focus=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN_TRANSIENT)
            .setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_MEDIA)
                .setContentType(AudioAttributes.CONTENT_TYPE_MOVIE).build())
            .setOnAudioFocusChangeListener(change->{
                if(player==null||!prepared)return;
                if(change==AudioManager.AUDIOFOCUS_GAIN){focusHeld=true;if(resumed)player.start();}
                else {focusHeld=false;if(player.isPlaying())player.pause();}
            }).build();
    }

    @Override public void onSurfaceTextureAvailable(SurfaceTexture texture,int w,int h) {
        if(completed)return;
        firstFrame=false;
        try {
            player=new MediaPlayer();videoSurface=new Surface(texture);player.setSurface(videoSurface);
            player.setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_MEDIA)
                .setContentType(AudioAttributes.CONTENT_TYPE_MOVIE).build());
            try(AssetFileDescriptor fd=getAssets().openFd("original-media/intro.mp4")){
                player.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
            }
            player.setOnVideoSizeChangedListener((p,vw,vh)->{videoWidth=vw;videoHeight=vh;fit();});
            player.setOnPreparedListener(p->{prepared=true;if(position>0)p.seekTo(position);
                focusHeld=requestFocus();
                if(resumed&&focusHeld)p.start();
                Log.i("DH2Front","Intro prepared | duration_ms="+p.getDuration()+" | original H264/AAC asset | position="+position);
            });
            player.setOnCompletionListener(p->complete("completed"));
            player.setOnErrorListener((p,what,extra)->{failure("Video decoder error "+what+"/"+extra);return true;});
            player.prepareAsync();
        }catch(Exception e){failure(e.toString());release();}
    }

    private boolean requestFocus(){
        return (android.os.Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(focus):audio.requestAudioFocus(null,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN_TRANSIENT))==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;
    }
    private void fit(){
        if(videoWidth<=0||videoHeight<=0||video.getWidth()<=0||video.getHeight()<=0)return;
        float w=video.getWidth(),h=video.getHeight();float scale=Math.min(w/videoWidth,h/videoHeight);
        Matrix transform=new Matrix();transform.setScale(videoWidth*scale/w,videoHeight*scale/h,w/2,h/2);
        video.setTransform(transform);
        Log.i("DH2Front","Intro fit | surface="+(int)w+"x"+(int)h+" | video="+videoWidth+"x"+videoHeight+" | fitted="+(videoWidth*scale)+"x"+(videoHeight*scale));
    }
    @Override public void onSurfaceTextureSizeChanged(SurfaceTexture t,int w,int h){fit();}
    @Override public void onSurfaceTextureUpdated(SurfaceTexture t){if(prepared&&!firstFrame){firstFrame=true;Log.i("DH2Front","Intro first video frame | position_ms="+player.getCurrentPosition());}}
    @Override public boolean onSurfaceTextureDestroyed(SurfaceTexture t){release();return true;}
    private void failure(String message){Log.e("DH2Front",message);error.setText(message);error.setVisibility(View.VISIBLE);}
    private void release(){
        if(player!=null){if(prepared)position=player.getCurrentPosition();player.release();player=null;}
        if(videoSurface!=null){videoSurface.release();videoSurface=null;}prepared=false;
    }
    private void complete(String reason){
        if(completed)return;completed=true;Log.i("DH2Front","Intro "+reason);
        startActivity(new Intent(this,MainActivity.class).putExtra("front_screen","main"));finish();
    }
    @Override public void onResume(){super.onResume();resumed=true;
        if(prepared){focusHeld=requestFocus();if(focusHeld)player.start();Log.i("DH2Front","Intro resumed | position_ms="+player.getCurrentPosition());}}
    @Override public void onPause(){resumed=false;
        if(prepared){position=player.getCurrentPosition();if(player.isPlaying())player.pause();Log.i("DH2Front","Intro paused | position_ms="+position);}
        if(android.os.Build.VERSION.SDK_INT>=26&&focus!=null)audio.abandonAudioFocusRequest(focus);
        else audio.abandonAudioFocus(null);focusHeld=false;super.onPause();}
    @Override public void onDestroy(){release();super.onDestroy();}
    @Override public void onSaveInstanceState(Bundle state){if(prepared)position=player.getCurrentPosition();state.putInt("position",position);super.onSaveInstanceState(state);}
    @Override public void onBackPressed(){complete("back");}
}
