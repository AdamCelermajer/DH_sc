package com.example.dh2;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
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
    private float musicVolume=1f,effectVolume=1f,musicGain=1f;
    private boolean musicPaused;
    private final Handler envelopeHandler=new Handler(Looper.getMainLooper());
    private int envelopeGeneration;
    private int nextFadeMs=2000;
    private void applyMusicVolume(){
        if(intro!=null)intro.setVolume(musicVolume*musicGain,musicVolume*musicGain);
        if(loop!=null)loop.setVolume(musicVolume*musicGain,musicVolume*musicGain);
    }
    private void fadeMusic(float target,int duration,boolean release){
        final int generation=++envelopeGeneration;
        final float start=musicGain;
        final long begin=SystemClock.uptimeMillis();
        envelopeHandler.post(new Runnable(){public void run(){
            if(generation!=envelopeGeneration)return;
            float fraction=duration<=0?1f:Math.min(1f,(SystemClock.uptimeMillis()-begin)/(float)duration);
            musicGain=start+(target-start)*fraction;applyMusicVolume();
            if(fraction<1f)envelopeHandler.postDelayed(this,20);
            else {Log.i("DH2Front","Menu music fade completed | duration_ms="+duration+" | gain="+musicGain);if(release)releaseMusic();}
        }});
    }
    private void releaseMusic(){
        ++envelopeGeneration;requested=false;musicPaused=false;
        if(intro!=null){intro.release();intro=null;}if(loop!=null){loop.release();loop=null;}
        current=null;started=introReady=loopReady=false;
        if(effects.isEmpty())abandonFocus();
        Log.i("DH2Front","Menu music released");
    }
    private void pauseMusic(){
        musicPaused=true;
        if(current!=null&&started&&current.isPlaying())current.pause();
        Log.i("DH2Front","Menu music source paused | position_ms="+(current!=null&&started?current.getCurrentPosition():-1));
    }
    private void stopMusic(int fadeMs){
        requested=false;
        if(current!=null&&started&&current.isPlaying())fadeMusic(0f,fadeMs,true);
        else releaseMusic();
    }
    void control(String command){
        if(command.equals("music-inspect")){
            Log.i("DH2Front","Menu music inspected | requested="+requested+" | resumed="+resumed+" | source_paused="+musicPaused+" | focused="+focused+" | phase="+(current==null?"none":current==intro?"intro":"loop")+" | playing="+(current!=null&&started&&current.isPlaying())+" | position_ms="+(current!=null&&started?current.getCurrentPosition():-1)+" | gain="+musicGain);
            return;
        }
        if(command.equals("resume")){resume();return;}
        if(command.equals("title")){title();return;}
        if(command.equals("music-pause")){pauseMusic();return;}
        if(command.equals("music-title,2000")){nextFadeMs=2000;title();return;}
        if(command.equals("music-stop,500")){stopMusic(500);return;}
        String[] fields=command.split(",");
        if(fields.length==3&&fields[0].equals("volume")){
            try {
                int music=Integer.parseInt(fields[1]),fx=Integer.parseInt(fields[2]);
                musicVolume=Math.max(0,Math.min(100,music))/100f;
                effectVolume=Math.max(0,Math.min(100,fx))/100f;
                applyMusicVolume();
                for(MediaPlayer p:effects)p.setVolume(effectVolume,effectVolume);
                Log.i("DH2Front","Menu audio volume applied | music="+music+" | fx="+fx);
            }catch(NumberFormatException e){Log.e("DH2Front","Invalid native volume command",e);}
            return;
        }
        Log.e("DH2Front","Unknown native audio command: "+command);
    }
    FrontAudio(Context c){context=c;audio=(AudioManager)c.getSystemService(Context.AUDIO_SERVICE);
        if(Build.VERSION.SDK_INT>=26)focus=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
            .setAudioAttributes(attributes()).setOnAudioFocusChangeListener(change->{
                focused=change==AudioManager.AUDIOFOCUS_GAIN;
                if(!focused)stopEffects();
                if(current!=null&&started){if(focused&&resumed&&requested&&!musicPaused)current.start();else if(current.isPlaying())current.pause();}
            }).build();
    }
    private AudioAttributes attributes(){return new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME).setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build();}
    private boolean requestFocus(){return (Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(focus):audio.requestAudioFocus(null,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN))==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;}
    private void abandonFocus(){if(Build.VERSION.SDK_INT>=26)audio.abandonAudioFocusRequest(focus);else audio.abandonAudioFocus(null);focused=false;}
    private MediaPlayer create(String name)throws Exception {
        MediaPlayer p=new MediaPlayer();
        try {p.setAudioAttributes(attributes());try(AssetFileDescriptor fd=context.getAssets().openFd("original-media/"+name)){
                p.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());}
            p.setVolume(musicVolume*musicGain,musicVolume*musicGain);
            p.setOnErrorListener((failed,what,extra)->{Log.e("DH2Front","Title music error "+what+"/"+extra);stop();return true;});return p;
        }catch(Exception e){p.release();throw e;}
    }
    /** Original NativePlaySoundFX/PlayMenu non-looping request, delivered on UI thread. */
    void effect(String file){
        if(!resumed)return;
        // Only the native source catalog queues these basename-only WAVs after
        // verifying the actual packaged asset. Keep traversal outside this API.
        if(!file.matches("[A-Za-z0-9_]+\\.wav")){
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
            p.setVolume(effectVolume,effectVolume);
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
        // A stopped request owns no current music identity, even while its
        // fade is draining. A new play must start a fresh intro.
        if(!requested&&intro!=null)releaseMusic();
        requested=true;musicPaused=false;
        if(intro!=null){
            // A repeated title request resumes the retained track and does not
            // replay its intro or change Android activity lifecycle state.
            ++envelopeGeneration;musicGain=1f;applyMusicVolume();
            if(resumed){focused=requestFocus();if(started&&current!=null&&focused&&!musicPaused)current.start();else startPrepared();}
            Log.i("DH2Front","Menu music retained | position_ms="+(current!=null&&started?current.getCurrentPosition():-1));return;
        }
        musicGain=0f;
        try {
            intro=create("title_intro.wav");loop=create("title_loop.wav");loop.setLooping(true);
            intro.setOnPreparedListener(p->{introReady=true;startPrepared();});
            loop.setOnPreparedListener(p->{loopReady=true;startPrepared();});
            intro.setOnCompletionListener(p->{current=loop;if(!resumed||!focused||musicPaused||!requested)loop.pause();Log.i("DH2Front","Title music loop entered | duration_ms="+loop.getDuration());});
            intro.prepareAsync();loop.prepareAsync();
        }catch(Exception e){Log.e("DH2Front","Title music preparation failed",e);stop();}
    }
    private void startPrepared(){
        if(!introReady||!loopReady||started||!requested||musicPaused||!resumed)return;
        intro.setNextMediaPlayer(loop);current=intro;focused=requestFocus();
        if(resumed&&focused){intro.start();started=true;fadeMusic(1f,nextFadeMs,false);Log.i("DH2Front","Title music intro started | duration_ms="+intro.getDuration()+" | loop_ms="+loop.getDuration());}
    }
    void pause(){resumed=false;stopEffects();if(current!=null&&started&&current.isPlaying())current.pause();abandonFocus();}
    void resume(){resumed=true;if(!requested)return;focused=requestFocus();
        if(started&&current!=null&&focused&&!musicPaused)current.start();else startPrepared();}
    void stop(){stopEffects();releaseMusic();abandonFocus();}
}
