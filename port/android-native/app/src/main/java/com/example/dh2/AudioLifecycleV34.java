package com.example.dh2;
import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Build;

/** One app focus owner for title/UI/gameplay output; caller supplies native endpoint. */
final class AudioLifecycleV34 implements AutoCloseable {
    interface Endpoint { void lifecycle(boolean resumed, boolean focused, float duckGain); }
    private final AudioManager manager;
    private final AudioFocusRequest request;
    private final Endpoint endpoint;
    private boolean resumed, requested, focused, closed;
    private float duck=1f;
    AudioLifecycleV34(Context context, Endpoint endpoint) {
        if(Build.VERSION.SDK_INT<26)throw new UnsupportedOperationException("Native AAudio requires Android 26+");
        if(endpoint==null)throw new IllegalArgumentException("Native audio endpoint required");
        this.endpoint=endpoint;manager=(AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        request=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
            .setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build())
            .setAcceptsDelayedFocusGain(false).setWillPauseWhenDucked(true)
            .setOnAudioFocusChangeListener(change->{
                if(closed)return;
                focused=change==AudioManager.AUDIOFOCUS_GAIN;
                duck=1f;publish();
            },new Handler(Looper.getMainLooper())).build();
    }
    private void publish(){endpoint.lifecycle(resumed&&!closed,focused&&!closed,duck);}
    // Android focus is acquired only while the Activity is actually resumed.
    boolean resume(){
        if(closed)return false;resumed=true;
        if(!requested||!focused){focused=manager.requestAudioFocus(request)==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;requested=focused;}
        publish();return focused;
    }
    void pause(){resumed=false;focused=false;publish();if(requested){manager.abandonAudioFocusRequest(request);requested=false;}}
    public void close(){if(closed)return;pause();closed=true;publish();}
}
