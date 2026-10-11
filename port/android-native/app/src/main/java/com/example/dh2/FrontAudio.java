package com.example.dh2;

import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Build;
import android.util.Log;
import android.view.animation.LinearInterpolator;
import java.util.ArrayList;
import java.util.HashSet;

/** Owns the recovered title intro and loop on Android's audio backend. */
final class FrontAudio {
    private final Context context;
    private final AudioManager audio;
    private final AudioManager.OnAudioFocusChangeListener focusListener;
    private AudioFocusRequest focus;
    private MediaPlayer intro, loop, current;
    private GameplayVxnMusic gameplayMusic;
    private static final class ActiveEffect {
        final MediaPlayer player;
        final long owner;
        final int soundId;
        ValueAnimator fade;
        float gain=1f;
        float leftMix=1f,rightMix=1f;
        int fadeInMs,resumeFadeMs;
        boolean prepared,playing,sourcePaused,pauseTransition;
        ActiveEffect(MediaPlayer player,long owner,int soundId){this.player=player;this.owner=owner;this.soundId=soundId;}
    }
    private final HashSet<ActiveEffect> effects=new HashSet<>();
    private boolean requested, resumed=true, introReady, loopReady, started, focused, focusStateKnown;
    private boolean nativeAudioPaused, nativeMusicPaused;
    private ValueAnimator nativeMusicFade;
    private float nativeMusicGain=1f;
    private boolean windowFocused, focusRequestPendingDelayed;
    private IntroCinematicView cinematic;
    private float musicVolume=1f,effectVolume=1f;
    void control(String command){
        if(command.equals("resume")){
            resumeSourceAudio();
            if(requested&&!started)startPrepared();
            return;
        }
        if(command.equals("title")){title();return;}
        if(command.equals("pause-all")){
            nativeAudioPaused=true;
            for(ActiveEffect effect:new ArrayList<>(effects))pauseEffect(effect,200);
            fadeNativeMusic(200,true);
            updateEffectPlayback();updateGameplayMusicPlayback();updateTitleMusicPlayback();
            Log.i("DH2Front","Source game paused active audio voices");
            return;
        }
        if(command.equals("resume-all")){
            resumeSourceAudio();
            Log.i("DH2Front","Source game resumed active audio voices");
            return;
        }
        if(command.equals("pause-music")){
            if((gameplayMusic==null||!gameplayMusic.isRunning())&&
                    (current==null||!current.isPlaying()))return;
            nativeMusicPaused=true;fadeNativeMusic(50,true);updateGameplayMusicPlayback();
            Log.i("DH2Front","Source game paused active music");
            return;
        }
        String[] fields=command.split(",");
        if(fields.length==3&&fields[0].equals("volume")){
            try {
                int music=Integer.parseInt(fields[1]),fx=Integer.parseInt(fields[2]);
                musicVolume=OriginalMusicVolume.gain(music);
                effectVolume=OriginalMusicVolume.gain(fx);
                applyNativeMusicVolume();
                for(ActiveEffect effect:effects)effect.player.setVolume(effectVolume*effect.gain*effect.leftMix,effectVolume*effect.gain*effect.rightMix);
                Log.i("DH2Front","Menu audio volume applied | music="+music+" | fx="+fx);
            }catch(NumberFormatException e){Log.e("DH2Front","Invalid native volume command",e);}
            return;
        }
        Log.e("DH2Front","Unknown native audio command: "+command);
    }
    FrontAudio(Context c){context=c;audio=(AudioManager)c.getSystemService(Context.AUDIO_SERVICE);
        focusListener=this::onAudioFocusChange;
        if(Build.VERSION.SDK_INT>=26)focus=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
            .setAudioAttributes(attributes()).setAcceptsDelayedFocusGain(true)
            .setOnAudioFocusChangeListener(focusListener).build();
    }
    private void onAudioFocusChange(int change){
        focused=AudioFocusStartPolicy.focusGained(change,AudioManager.AUDIOFOCUS_GAIN);
        focusStateKnown=true;
        if(focused)focusRequestPendingDelayed=false;
        updateEffectPlayback();
        if(cinematic!=null)cinematic.setAudioFocusGained(focused&&resumed);
        updateGameplayMusicPlayback();
        updateTitleMusicPlayback();
        if(!started)startPrepared();
    }
    private AudioAttributes attributes(){return new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME).setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build();}
    private boolean requestFocus(){
        int result=Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(focus):audio.requestAudioFocus(focusListener,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN);
        focused=result==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;
        focusStateKnown=true;
        focusRequestPendingDelayed=Build.VERSION.SDK_INT>=26&&result==AudioManager.AUDIOFOCUS_REQUEST_DELAYED;
        String outcome=focused?"granted":focusRequestPendingDelayed?"delayed":"failed";
        Log.i("DH2Front","Audio focus request | result="+result+" | outcome="+outcome);
        return focused;
    }
    private void abandonFocus(){if(Build.VERSION.SDK_INT>=26)audio.abandonAudioFocusRequest(focus);else audio.abandonAudioFocus(focusListener);focused=false;focusRequestPendingDelayed=false;}
    float musicVolume(){return musicVolume;}
    void setCinematic(IntroCinematicView view){
        cinematic=view;
        if(view!=null){
            // The view can be attached after a native volume command or a
            // focus transition. Seed its decoder state immediately instead
            // of leaving the first prepared frames at the view's defaults.
            view.setMusicVolume(musicVolume);
            // Do not silence the intro during onCreate, before the Activity has
            // made its first focus request. Once focus is known, honor loss.
            if(focusStateKnown)view.setAudioFocusGained(focused&&resumed);
        }
        if(view==null&&!requested&&current==null&&gameplayMusic==null)abandonFocus();
    }
    private MediaPlayer create(String name)throws Exception {
        MediaPlayer p=new MediaPlayer();
        try {p.setAudioAttributes(attributes());try(AssetFileDescriptor fd=context.getAssets().openFd("original-media/"+name)){
                p.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());}
            p.setVolume(musicVolume*nativeMusicGain,musicVolume*nativeMusicGain);
            p.setOnErrorListener((failed,what,extra)->{
                Log.e("DH2Front","Title music error "+what+"/"+extra);
                if(AudioFocusStartPolicy.isCurrentPlayer(failed,intro,requested)||
                        AudioFocusStartPolicy.isCurrentPlayer(failed,loop,requested))stop();
                return true;
            });return p;
        }catch(Exception e){p.release();throw e;}
    }
    /** Native menu effects and ordered gameplay audio commands, delivered on UI thread. */
    void effect(String file){
        if(!resumed)return;
        if(file.startsWith("dh2music,")){gameplayMusicCommand(file);return;}
        if(file.startsWith("dh2fx,")){audioCommand(file);return;}
        final String assetPath;
        if(file.startsWith("gameplay:")){
            String gameplayFile=file.substring("gameplay:".length());
            if(!gameplayFile.matches("sfx_[a-z0-9_]+\\.wav")){
                Log.e("DH2Front","Unsupported gameplay effect asset: "+file);return;
            }
            assetPath="original-media/gameplay/"+gameplayFile;
        }else if(file.matches("sfx_menu_(back|confirm|select|spending|tab)\\.wav")){
            assetPath="original-media/"+file;
        }else{
            Log.e("DH2Front","Unsupported menu/gameplay effect asset: "+file);return;
        }
        startEffect(assetPath,0,-1,false,0,16384,16384);
    }
    private void gameplayMusicCommand(String command){
        String[] fields=command.split(",",-1);
        if(fields.length!=3||!fields[0].equals("dh2music")){
            Log.e("DH2Front","Invalid source VXN music command: "+command);return;
        }
        try{
            int soundUid=Integer.parseInt(fields[1]);
            SourceMusicResources.Resource resource=SourceMusicResources.resolve(soundUid,fields[2]);
            if(resource==null){Log.e("DH2Front","Unsupported source music UID/state: "+command);return;}
            playGameplayMusic(soundUid,fields[2],resource);
        }catch(NumberFormatException e){Log.e("DH2Front","Invalid source music UID: "+command,e);}
    }
    private void playGameplayMusic(int soundUid,String state,SourceMusicResources.Resource resource){
        releaseTitleMusic();
        if(!focused)focused=requestFocus();
        try{
            if(gameplayMusic==null)gameplayMusic=new GameplayVxnMusic(context);
            gameplayMusic.selectState(soundUid,state,resource);gameplayMusic.setVolume(musicVolume);
            updateGameplayMusicPlayback();
            Log.i("DH2Front","Source music mixer started | UID="+soundUid+" | state="+state+" | PCM16 stereo/32kHz");
        }catch(Exception e){Log.e("DH2Front","Source music decoder setup failed | UID="+soundUid+" | state="+state,e);releaseGameplayMusic();}
    }
    private void updateGameplayMusicPlayback(){
        if(gameplayMusic!=null)gameplayMusic.setPlaybackEnabled(
            resumed&&focused&&(!nativeAudioPaused||nativeMusicFade!=null)&&
                (!nativeMusicPaused||nativeMusicFade!=null));
    }
    private boolean titlePlaybackAllowed(){
        return AudioFocusStartPolicy.mayStart(focused,resumed,requested)&&
            (!nativeAudioPaused||nativeMusicFade!=null);
    }
    private void updateTitleMusicPlayback(){
        if(current==null||!started)return;
        try{
            if(titlePlaybackAllowed()&&!current.isPlaying())current.start();
            else if(!titlePlaybackAllowed()&&current.isPlaying())current.pause();
        }catch(IllegalStateException e){Log.e("DH2Front","Title music pause/resume transition failed",e);}
    }
    private void pauseGameplayMusic(){
        if(gameplayMusic!=null)gameplayMusic.setPlaybackEnabled(false);
    }
    private void resumeSourceAudio(){
        nativeAudioPaused=false;nativeMusicPaused=false;
        for(ActiveEffect effect:new ArrayList<>(effects))resumeEffect(effect,200);
        fadeNativeMusic(200,false);
        updateEffectPlayback();updateGameplayMusicPlayback();updateTitleMusicPlayback();
    }
    private void applyNativeMusicVolume(){
        float gain=musicVolume*nativeMusicGain;
        if(intro!=null)intro.setVolume(gain,gain);
        if(loop!=null)loop.setVolume(gain,gain);
        if(gameplayMusic!=null)gameplayMusic.setVolume(gain);
    }
    private void fadeNativeMusic(int durationMs,boolean pauseAtEnd){
        if(nativeMusicFade!=null){nativeMusicFade.cancel();nativeMusicFade=null;}
        if(!pauseAtEnd){
            if(gameplayMusic!=null)gameplayMusic.setPlaybackEnabled(resumed&&focused);
            if(current!=null&&requested&&resumed&&focused&&!current.isPlaying())current.start();
        }
        ValueAnimator fade=ValueAnimator.ofFloat(nativeMusicGain,pauseAtEnd?0f:1f);
        nativeMusicFade=fade;fade.setDuration(durationMs);fade.setInterpolator(new LinearInterpolator());
        fade.addUpdateListener(animation->{
            nativeMusicGain=(float)animation.getAnimatedValue();applyNativeMusicVolume();
        });
        fade.addListener(new android.animation.AnimatorListenerAdapter(){
            private boolean cancelled;
            @Override public void onAnimationCancel(android.animation.Animator animation){cancelled=true;}
            @Override public void onAnimationEnd(android.animation.Animator animation){
                if(nativeMusicFade!=fade)return;
                nativeMusicFade=null;
                if(cancelled)return;
                nativeMusicGain=pauseAtEnd?0f:1f;applyNativeMusicVolume();
                if(pauseAtEnd){
                    if(gameplayMusic!=null)gameplayMusic.setPlaybackEnabled(false);
                    if(current!=null&&current.isPlaying())current.pause();
                }
                updateGameplayMusicPlayback();updateTitleMusicPlayback();
            }
        });
        fade.start();updateNativeFadeLifecycle();
    }
    private void updateNativeFadeLifecycle(){
        if(nativeMusicFade!=null){
            if(resumed&&focused){if(nativeMusicFade.isPaused())nativeMusicFade.resume();}
            else if(nativeMusicFade.isStarted()&&!nativeMusicFade.isPaused())nativeMusicFade.pause();
        }
        for(ActiveEffect effect:effects){
            if(effect.fade==null)continue;
            if(resumed&&focused){if(effect.fade.isPaused())effect.fade.resume();}
            else if(effect.fade.isStarted()&&!effect.fade.isPaused())effect.fade.pause();
        }
    }
    private void releaseGameplayMusic(){
        if(gameplayMusic!=null){gameplayMusic.stop();gameplayMusic=null;}
    }
    private void releaseTitleMusic(){
        requested=false;
        if(intro!=null){intro.release();intro=null;}if(loop!=null){loop.release();loop=null;}
        current=null;started=introReady=loopReady=false;
    }
    private void audioCommand(String command){
        String[] fields=command.split(",",-1);
        try{
            if((fields.length==7||fields.length==10)&&fields[0].equals("dh2fx")&&fields[1].equals("play")){
                long owner=Long.parseLong(fields[2]);
                int soundId=Integer.parseInt(fields[3]),fadeMs=Integer.parseInt(fields[4]);
                boolean looping=fields[5].equals("1");
                int leftQ14=16384,rightQ14=16384;
                boolean hasSpatialMix=fields.length==10;
                if(hasSpatialMix){
                    if(!fields[7].equals("1")){Log.e("DH2Front","Invalid native spatial mix marker: "+command);return;}
                    leftQ14=Integer.parseInt(fields[8]);rightQ14=Integer.parseInt(fields[9]);
                }
                if(soundId<0||soundId>65535||(!looping&&!fields[5].equals("0"))||
                        !fields[6].matches("sfx_[a-z0-9_]+\\.wav")||
                        leftQ14<0||leftQ14>16384||rightQ14<0||rightQ14>16384){
                    Log.e("DH2Front","Invalid native gameplay play command: "+command);return;
                }
                startEffect("original-media/gameplay/"+fields[6],owner,soundId,looping,fadeMs,leftQ14,rightQ14);
            }else if(fields.length==5&&fields[0].equals("dh2fx")&&fields[1].equals("stop")){
                long owner=Long.parseLong(fields[2]);
                int soundId=Integer.parseInt(fields[3]),fadeMs=Integer.parseInt(fields[4]);
                if(soundId<0||soundId>65535){Log.e("DH2Front","Invalid native gameplay stop command: "+command);return;}
                stopSound(owner,soundId,fadeMs);
            }else Log.e("DH2Front","Invalid native gameplay audio command: "+command);
        }catch(NumberFormatException e){Log.e("DH2Front","Invalid native gameplay audio number: "+command,e);}
    }
    private void startEffect(String assetPath,long owner,int soundId,boolean looping,int fadeMs){
        startEffect(assetPath,owner,soundId,looping,fadeMs,16384,16384);
    }
    private void startEffect(String assetPath,long owner,int soundId,boolean looping,int fadeMs,
                             int leftQ14,int rightQ14){
        if(!resumed)return;
        if(!focused)focused=requestFocus();
        if(!focused){Log.e("DH2Front","Effect audio focus unavailable: "+assetPath);return;}
        MediaPlayer p=new MediaPlayer();ActiveEffect active=new ActiveEffect(p,owner,soundId);
        active.leftMix=leftQ14/16384f;active.rightMix=rightQ14/16384f;
        active.gain=fadeMs>0?0f:1f;effects.add(active);
        try{
            p.setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION).build());
            try(AssetFileDescriptor fd=context.getAssets().openFd(assetPath)){
                p.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
            }
            p.setLooping(looping);
            p.setVolume(effectVolume*active.gain*active.leftMix,effectVolume*active.gain*active.rightMix);
            p.setOnPreparedListener(prepared->{
                if(!effects.contains(active))return;
                active.prepared=true;active.fadeInMs=fadeMs;
                updateEffectPlayback();
                Log.i("DH2Front","Effect started | asset="+assetPath+" | sound_id="+soundId+" | owner="+Long.toUnsignedString(owner)+" | duration_ms="+prepared.getDuration());
            });
            p.setOnCompletionListener(done->{Log.i("DH2Front","Effect completed | asset="+assetPath);releaseEffect(active);});
            p.setOnErrorListener((failed,what,extra)->{
                Log.e("DH2Front","Effect failed | asset="+assetPath+" | error="+what+"/"+extra);releaseEffect(active);return true;
            });
            p.prepareAsync();
        }catch(Exception e){Log.e("DH2Front","Effect preparation failed | asset="+assetPath,e);releaseEffect(active);}
    }
    private void stopSound(long owner,int soundId,int fadeMs){
        int stopped=0;
        for(ActiveEffect effect:new ArrayList<>(effects)){
            if(effect.owner!=owner||effect.soundId!=soundId)continue;
            if(fadeMs<=0)releaseEffect(effect);else fadeEffect(effect,fadeMs,0f);
            if(++stopped==10)break;
        }
    }
    private void fadeEffect(ActiveEffect effect,int durationMs,float target){
        if(effect.fade!=null){effect.fade.cancel();effect.fade=null;}
        ValueAnimator fade=ValueAnimator.ofFloat(effect.gain,target);effect.fade=fade;
        final boolean[] cancelled={false};
        fade.setDuration(Math.max(1,durationMs));
        fade.addUpdateListener(animation->{
            if(!effects.contains(effect))return;
            effect.gain=(float)animation.getAnimatedValue();
            effect.player.setVolume(effectVolume*effect.gain*effect.leftMix,effectVolume*effect.gain*effect.rightMix);
        });
        fade.addListener(new android.animation.AnimatorListenerAdapter(){
            @Override public void onAnimationCancel(android.animation.Animator animation){cancelled[0]=true;}
            @Override public void onAnimationEnd(android.animation.Animator animation){
                if(effect.fade==fade)effect.fade=null;
                if(!cancelled[0]&&target==0f)releaseEffect(effect);
            }
        });
        fade.start();
    }
    private void pauseEffect(ActiveEffect effect,int durationMs){
        if(!effect.prepared){effect.sourcePaused=true;return;}
        if(effect.fade!=null){ValueAnimator old=effect.fade;effect.fade=null;effect.pauseTransition=false;old.cancel();}
        effect.pauseTransition=true;
        ValueAnimator fade=ValueAnimator.ofFloat(effect.gain,0f);effect.fade=fade;
        fade.setDuration(durationMs);fade.setInterpolator(new LinearInterpolator());
        fade.addUpdateListener(animation->{
            if(!effects.contains(effect))return;
            effect.gain=(float)animation.getAnimatedValue();
            effect.player.setVolume(effectVolume*effect.gain*effect.leftMix,effectVolume*effect.gain*effect.rightMix);
        });
        fade.addListener(new android.animation.AnimatorListenerAdapter(){
            private boolean cancelled;
            @Override public void onAnimationCancel(android.animation.Animator animation){cancelled=true;}
            @Override public void onAnimationEnd(android.animation.Animator animation){
                if(effect.fade==fade)effect.fade=null;
                if(cancelled||!effects.contains(effect))return;
                effect.gain=0f;effect.pauseTransition=false;effect.sourcePaused=true;
                effect.player.setVolume(0f,0f);
                if(effect.playing){effect.player.pause();effect.playing=false;}
            }
        });
        fade.start();
    }
    private void resumeEffect(ActiveEffect effect,int durationMs){
        if(!effect.sourcePaused&&!effect.pauseTransition)return;
        if(effect.fade!=null){ValueAnimator old=effect.fade;effect.fade=null;effect.pauseTransition=false;old.cancel();}
        effect.sourcePaused=false;effect.pauseTransition=false;
        if(!effect.prepared){effect.resumeFadeMs=durationMs;return;}
        if(resumed&&focused&&effect.prepared&&!effect.playing){
            try{effect.player.start();effect.playing=true;}
            catch(IllegalStateException e){Log.e("DH2Front","Effect resume transition failed",e);return;}
        }
        ValueAnimator fade=ValueAnimator.ofFloat(effect.gain,1f);effect.fade=fade;effect.pauseTransition=true;
        fade.setDuration(durationMs);fade.setInterpolator(new LinearInterpolator());
        fade.addUpdateListener(animation->{
            if(!effects.contains(effect))return;
            effect.gain=(float)animation.getAnimatedValue();
            effect.player.setVolume(effectVolume*effect.gain*effect.leftMix,effectVolume*effect.gain*effect.rightMix);
        });
        fade.addListener(new android.animation.AnimatorListenerAdapter(){
            private boolean cancelled;
            @Override public void onAnimationCancel(android.animation.Animator animation){cancelled=true;}
            @Override public void onAnimationEnd(android.animation.Animator animation){
                if(effect.fade==fade)effect.fade=null;
                if(!cancelled&&effects.contains(effect)){
                    effect.gain=1f;effect.fadeInMs=0;effect.pauseTransition=false;
                }
            }
        });
        fade.start();updateNativeFadeLifecycle();
    }
    private void releaseEffect(ActiveEffect effect){
        if(!effects.remove(effect))return;
        if(effect.fade!=null){ValueAnimator fade=effect.fade;effect.fade=null;fade.cancel();}
        effect.player.release();
    }
    private void stopEffects(){
        if(!effects.isEmpty())Log.i("DH2Front","Menu effects released | count="+effects.size());
        for(ActiveEffect effect:new ArrayList<>(effects))releaseEffect(effect);
    }
    private void updateEffectPlayback(){
        boolean allowed=resumed&&focused;
        for(ActiveEffect effect:new ArrayList<>(effects)){
            if(!effect.prepared)continue;
            try{
                if(allowed&&!effect.playing&&effect.resumeFadeMs>0){
                    int fade=effect.resumeFadeMs;effect.resumeFadeMs=0;effect.sourcePaused=true;
                    resumeEffect(effect,fade);continue;
                }
                if(allowed&&!effect.sourcePaused&&!effect.playing){
                    effect.player.start();effect.playing=true;
                    if(effect.fadeInMs>0){int fade=effect.fadeInMs;effect.fadeInMs=0;fadeEffect(effect,fade,1f);}
                }else if(!allowed&&effect.playing){
                    effect.player.pause();effect.playing=false;
                }
            }catch(IllegalStateException e){Log.e("DH2Front","Effect pause/resume transition failed",e);}
        }
        updateNativeFadeLifecycle();
    }
    void title(){
        releaseGameplayMusic();requested=true;if(intro!=null){resume();return;}
        try {
            intro=create("title_intro.wav");loop=create("title_loop.wav");loop.setLooping(true);
            intro.setOnPreparedListener(p->{
                if(!AudioFocusStartPolicy.isCurrentPlayer(p,intro,requested))return;
                introReady=true;startPrepared();
            });
            loop.setOnPreparedListener(p->{
                if(!AudioFocusStartPolicy.isCurrentPlayer(p,loop,requested))return;
                loopReady=true;startPrepared();
            });
            intro.setOnCompletionListener(p->{
                if(!AudioFocusStartPolicy.isCurrentPlayer(p,intro,requested)||loop==null)return;
                current=loop;
                Log.i("DH2Front","Title music loop entered | duration_ms="+loop.getDuration());
                // Do not let MediaPlayer's automatic next-player handoff start
                // the loop after Activity pause or focus loss. Resume and focus
                // callbacks start this prepared player when playback is allowed.
                if(titlePlaybackAllowed()){
                    try{loop.start();}
                    catch(IllegalStateException e){Log.e("DH2Front","Could not start title music loop",e);stop();}
                }
            });
            intro.prepareAsync();loop.prepareAsync();
        }catch(Exception e){Log.e("DH2Front","Title music preparation failed",e);stop();}
    }
    private void startPrepared(){
        if(!introReady||!loopReady||started||!requested)return;
        current=intro;
        if(!focused)focused=requestFocus();
        if(titlePlaybackAllowed()){intro.start();started=true;Log.i("DH2Front","Title music intro started | duration_ms="+intro.getDuration()+" | loop_ms="+loop.getDuration());}
    }
    void pause(){resumed=false;windowFocused=false;focusStateKnown=true;updateEffectPlayback();pauseGameplayMusic();if(cinematic!=null)cinematic.setAudioFocusGained(false);if(current!=null&&started&&current.isPlaying())current.pause();abandonFocus();}
    void resume(){resumed=true;if(!requested&&cinematic==null&&gameplayMusic==null&&effects.isEmpty())return;
        // Android may reject an early focus request while the Activity is
        // resumed but its window is not yet foreground-focused. Defer the
        // initial request until onWindowFocusChanged(true) in that case.
        if(windowFocused)requestFocus();
        updateEffectPlayback();
        if(cinematic!=null&&focusStateKnown)cinematic.setAudioFocusGained(focused);
        updateGameplayMusicPlayback();
        updateTitleMusicPlayback();
        if(requested&&!started)startPrepared();}
    void onWindowFocusChanged(boolean hasFocus){
        if(!hasFocus){windowFocused=false;return;}
        if(windowFocused)return;
        windowFocused=true;
        if(!resumed||focused||focusRequestPendingDelayed||
           (!requested&&cinematic==null&&gameplayMusic==null&&effects.isEmpty()))return;
        focused=requestFocus();
        if(cinematic!=null)cinematic.setAudioFocusGained(focused);
        updateEffectPlayback();
        updateGameplayMusicPlayback();
        updateTitleMusicPlayback();
        if(requested&&!started)startPrepared();
    }
    void stop(){requested=false;
        stopEffects();
        releaseGameplayMusic();
        if(intro!=null){intro.release();intro=null;}if(loop!=null){loop.release();loop=null;}
        current=null;started=introReady=loopReady=false;cinematic=null;abandonFocus();}
}
