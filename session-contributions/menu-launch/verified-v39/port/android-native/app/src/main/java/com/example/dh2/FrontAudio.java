package com.example.dh2;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Build;
import android.util.Log;
import java.util.HashSet;

/** Owns the recovered title intro and loop on Android's audio backend. */
final class FrontAudio {
    private final Context context;
    private final AudioManager audio;
    private AudioFocusRequest focus;
    private MediaPlayer intro, loop, current;
    private final HashSet<MediaPlayer> effects=new HashSet<>();
    private boolean requested, resumed=true, introReady, loopReady, started, focused;
    FrontAudio(Context c){context=c;audio=(AudioManager)c.getSystemService(Context.AUDIO_SERVICE);
        if(Build.VERSION.SDK_INT>=26)focus=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
            .setAudioAttributes(attributes()).setOnAudioFocusChangeListener(change->{
                focused=change==AudioManager.AUDIOFOCUS_GAIN;
                if(!focused)stopEffects();
                if(current!=null&&started){if(focused&&resumed&&requested)current.start();else if(current.isPlaying())current.pause();}
            }).build();
    }
    private AudioAttributes attributes(){return new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME).setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build();}
    private boolean requestFocus(){return (Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(focus):audio.requestAudioFocus(null,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN))==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;}
    private void abandonFocus(){if(Build.VERSION.SDK_INT>=26)audio.abandonAudioFocusRequest(focus);else audio.abandonAudioFocus(null);focused=false;}
    private MediaPlayer create(String name)throws Exception {
        MediaPlayer p=new MediaPlayer();
        try {p.setAudioAttributes(attributes());try(AssetFileDescriptor fd=context.getAssets().openFd("original-media/"+name)){
                p.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());}
            p.setOnErrorListener((failed,what,extra)->{Log.e("DH2Front","Title music error "+what+"/"+extra);stop();return true;});return p;
        }catch(Exception e){p.release();throw e;}
    }
    /** Original NativePlaySoundFX/PlayMenu non-looping request, delivered on UI thread. */
    void effect(String file){
        if(!resumed)return;
        if(!file.matches("sfx_menu_(back|confirm|select|spending|tab)\\.wav")){
            Log.e("DH2Front","Unsupported menu effect asset: "+file);return;
        }
        if(!focused)focused=requestFocus();
        if(!focused){Log.e("DH2Front","Menu effect audio focus unavailable: "+file);return;}
        MediaPlayer p=new MediaPlayer();effects.add(p);
        try{
            p.setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION).build());
            try(AssetFileDescriptor fd=context.getAssets().openFd("original-media/"+file)){
                p.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
            }
            p.setLooping(false);
            p.setOnPreparedListener(prepared->{
                if(!effects.contains(prepared))return;
                if(!resumed||!focused){releaseEffect(prepared);return;}
                prepared.start();Log.i("DH2Front","Menu effect started | file="+file+" | duration_ms="+prepared.getDuration());
            });
            p.setOnCompletionListener(done->{Log.i("DH2Front","Menu effect completed | file="+file);releaseEffect(done);});
            p.setOnErrorListener((failed,what,extra)->{
                Log.e("DH2Front","Menu effect failed | file="+file+" | error="+what+"/"+extra);releaseEffect(failed);return true;
            });
            p.prepareAsync();
        }catch(Exception e){Log.e("DH2Front","Menu effect preparation failed | file="+file,e);releaseEffect(p);}
    }
    private void releaseEffect(MediaPlayer p){if(effects.remove(p))p.release();}
    private void stopEffects(){
        if(!effects.isEmpty())Log.i("DH2Front","Menu effects released | count="+effects.size());
        for(MediaPlayer p:effects)p.release();effects.clear();
    }
    void title(){
        requested=true;if(intro!=null){resume();return;}
        try {
            intro=create("title_intro.wav");loop=create("title_loop.wav");loop.setLooping(true);
            intro.setOnPreparedListener(p->{introReady=true;startPrepared();});
            loop.setOnPreparedListener(p->{loopReady=true;startPrepared();});
            intro.setOnCompletionListener(p->{current=loop;Log.i("DH2Front","Title music loop entered | duration_ms="+loop.getDuration());});
            intro.prepareAsync();loop.prepareAsync();
        }catch(Exception e){Log.e("DH2Front","Title music preparation failed",e);stop();}
    }
    private void startPrepared(){
        if(!introReady||!loopReady||started||!requested)return;
        intro.setNextMediaPlayer(loop);current=intro;focused=requestFocus();
        if(resumed&&focused){intro.start();started=true;Log.i("DH2Front","Title music intro started | duration_ms="+intro.getDuration()+" | loop_ms="+loop.getDuration());}
    }
    void pause(){resumed=false;stopEffects();if(current!=null&&started&&current.isPlaying())current.pause();abandonFocus();}
    void resume(){resumed=true;if(!requested)return;focused=requestFocus();
        if(started&&current!=null&&focused)current.start();else startPrepared();}
    void stop(){requested=false;
        stopEffects();
        if(intro!=null){intro.release();intro=null;}if(loop!=null){loop.release();loop=null;}
        current=null;started=introReady=loopReady=false;abandonFocus();}
}
