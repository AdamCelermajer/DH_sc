package com.example.dh2;
import android.opengl.GLSurfaceView;
import android.util.Log;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
final class AudioApplicationLifecycleV42 {
    private AudioApplicationLifecycleV42() {}
    static void closeBeforeProducerPause(GLSurfaceView surface,long expectedOwner) {
        NativeBridge.audioApplicationCloseV42(expectedOwner); // independent native CV request
        final CountDownLatch done=new CountDownLatch(1);
        surface.queueEvent(()-> {
            try {
                final String required=NativeBridge.audioApplicationShutdownV42(expectedOwner);
                if(required!=null)Log.e("DH2Native","Application audio owner retained: "+required);
            } finally { done.countDown(); }
        });
        try {
            if(!done.await(2,TimeUnit.SECONDS))Log.e("DH2Native","Application audio producer barrier pending; owner retained");
        } catch(InterruptedException interrupted) { Thread.currentThread().interrupt();Log.e("DH2Native","Audio barrier interrupted; owner retained",interrupted); }
    }
    static void destroyRequestOnly(long expectedOwner) {
        // onDestroy can follow an already paused GL thread. Never infer a
        // completed producer barrier from a queued task or free native storage.
        NativeBridge.audioApplicationCloseV42(expectedOwner);
    }
}
