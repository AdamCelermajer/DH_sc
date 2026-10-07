package com.example.dh2;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import java.util.concurrent.atomic.AtomicLong;

/** Sole Android focus requester for both front MediaPlayers and native mixer. */
final class SharedAudioFocusV40 {
    private static final AtomicLong NEXT_OWNER=new AtomicLong();
    private final AudioFocusPolicyV40 policy;
    SharedAudioFocusV40(Context context,AudioFocusPolicyV40.NativeEndpoint endpoint) {
        final AudioManager audio=(AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        final Handler main=new Handler(Looper.getMainLooper());
        final AudioFocusRequest request;
        final AudioManager.OnAudioFocusChangeListener listener=this::focusChanged;
        if(Build.VERSION.SDK_INT>=26)request=new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
            .setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build())
            .setAcceptsDelayedFocusGain(false).setWillPauseWhenDucked(true)
            .setOnAudioFocusChangeListener(listener,main).build();
        else request=null;
        policy=new AudioFocusPolicyV40(NEXT_OWNER.incrementAndGet(),new AudioFocusPolicyV40.Backend() {
            @SuppressWarnings("deprecation") public boolean request() {
                return (Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(request):audio.requestAudioFocus(listener,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN))==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;
            }
            @SuppressWarnings("deprecation") public void abandon() {
                if(Build.VERSION.SDK_INT>=26)audio.abandonAudioFocusRequest(request);else audio.abandonAudioFocus(listener);
            }
        },endpoint);
    }
    private void focusChanged(int change) {
        requireMain();policy.focusChanged(change==AudioManager.AUDIOFOCUS_GAIN,change==AudioManager.AUDIOFOCUS_LOSS);
    }
    private static void requireMain() { if(Looper.myLooper()!=Looper.getMainLooper())throw new IllegalStateException("Audio focus requires main thread"); }
    void setFrontListener(AudioFocusPolicyV40.FrontEndpoint endpoint) { requireMain();policy.frontListener(endpoint); }
    void setFrontDemand(boolean value) { requireMain();policy.frontDemand(value); }
    void setNativeDemand(boolean actualSourceReady) { requireMain();policy.nativeDemand(actualSourceReady); }
    void setResumed(boolean actualResumed) { requireMain();policy.resumed(actualResumed); }
    void setWindowFocused(boolean actualWindowFocused) { requireMain();policy.windowFocused(actualWindowFocused); }
    boolean canPlay() { requireMain();return policy.canPlay(); }
    void close() { requireMain();policy.close(); }
}
