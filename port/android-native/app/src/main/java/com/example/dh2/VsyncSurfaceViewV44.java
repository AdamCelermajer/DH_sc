package com.example.dh2;

import android.content.Context;
import android.hardware.display.DisplayManager;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Choreographer;
import android.view.Display;
import android.view.SurfaceHolder;
import android.view.View;

/** Thin opt-in Android bridge. GLSurfaceView retains its existing GL thread,
 * context, renderer and EGL swap implementation; only request scheduling changes. */
public final class VsyncSurfaceViewV44 extends GLSurfaceView {
    private FramePacingControllerV44 controller;
    private AndroidDriver driver;
    public VsyncSurfaceViewV44(Context context){super(context);}
    /** Main thread, after setRenderer(). Caller must enforce explicit debug opt-in. */
    public FramePacingControllerV44 enableVsync60() {
        if(controller!=null)throw new IllegalStateException("V44 pacing already enabled");
        if(Looper.myLooper()!=Looper.getMainLooper())throw new IllegalStateException("V44 enable on main thread");
        driver=new AndroidDriver();controller=new FramePacingControllerV44(driver);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        getHolder().addCallback(driver);
        driver.displays.registerDisplayListener(driver,driver.handler);
        controller.onAttached(isAttachedToWindow());controller.onWindowVisible(getWindowVisibility()==View.VISIBLE);
        controller.onWindowFocus(hasWindowFocus());
        controller.onSurfaceAvailable(getHolder().getSurface().isValid()&&getWidth()>1&&getHeight()>1);
        driver.reportDisplay("enable");return controller;
    }
    public void destroyPacingV44() {
        if(controller==null)return;
        controller.close();getHolder().removeCallback(driver);driver.displays.unregisterDisplayListener(driver);
        controller=null;driver=null;
    }
    @Override protected void onAttachedToWindow(){super.onAttachedToWindow();if(controller!=null){controller.onAttached(true);driver.reportDisplay("attach");}}
    @Override protected void onDetachedFromWindow(){if(controller!=null)controller.onAttached(false);super.onDetachedFromWindow();}
    @Override protected void onWindowVisibilityChanged(int visibility){super.onWindowVisibilityChanged(visibility);if(controller!=null)controller.onWindowVisible(visibility==View.VISIBLE);}
    @Override public void onWindowFocusChanged(boolean focused){super.onWindowFocusChanged(focused);if(controller!=null)controller.onWindowFocus(focused);}

    private final class AndroidDriver implements FramePacingControllerV44.Driver,SurfaceHolder.Callback,DisplayManager.DisplayListener {
        private final Handler handler=new Handler(Looper.getMainLooper());
        private final Choreographer choreographer=Choreographer.getInstance();
        private final DisplayManager displays=(DisplayManager)getContext().getSystemService(Context.DISPLAY_SERVICE);
        private AndroidCallback bridge;
        private final class AndroidCallback implements Choreographer.FrameCallback {
            private final FramePacingControllerV44.Callback source;
            AndroidCallback(FramePacingControllerV44.Callback callback){source=callback;}
            @Override public void doFrame(long nanos){source.doFrame(nanos);}
        }
        @Override public long nowNanos(){return System.nanoTime();}
        @Override public boolean isMainThread(){return Looper.myLooper()==Looper.getMainLooper();}
        @Override public void postFrame(FramePacingControllerV44.Callback callback){
            if(bridge==null||bridge.source!=callback)bridge=new AndroidCallback(callback);
            choreographer.postFrameCallback(bridge);
        }
        @Override public void removeFrame(FramePacingControllerV44.Callback callback){if(bridge!=null&&bridge.source==callback)choreographer.removeFrameCallback(bridge);}
        @Override public void postTask(Runnable task){handler.removeCallbacks(task);handler.post(task);}
        @Override public void removeTask(Runnable task){handler.removeCallbacks(task);}
        @Override public void requestRender(){VsyncSurfaceViewV44.this.requestRender();}
        @Override public void failure(RuntimeException error){Log.e("DH2Pacing","V44 experiment stopped; relaunch without frame_pacing",error);}
        @Override public void surfaceCreated(SurfaceHolder holder){if(controller!=null)controller.onSurfaceAvailable(false);}
        @Override public void surfaceChanged(SurfaceHolder holder,int format,int width,int height){if(controller!=null)controller.onSurfaceAvailable(width>1&&height>1);}
        @Override public void surfaceDestroyed(SurfaceHolder holder){if(controller!=null)controller.onSurfaceAvailable(false);}
        @Override public void onDisplayAdded(int id){reportDisplay("display-added");}
        @Override public void onDisplayRemoved(int id){reportDisplay("display-removed");}
        @Override public void onDisplayChanged(int id){final Display display=getDisplay();if(display!=null&&display.getDisplayId()==id)reportDisplay("display-change");}
        private void reportDisplay(String reason){
            final Display display=getDisplay();final float rate=display==null?0:display.getRefreshRate();
            final String support=display==null||!Float.isFinite(rate)||rate<=0?"refresh-metadata-unavailable":rate<59?"display-below-60-target":"cadence-eligible-unverified";
            Log.i("DH2Pacing","V44 vsync60 opt-in | "+reason+" | display="+(display==null?-1:display.getDisplayId())+" refresh_hz="+rate+" | "+support+
                " | native wall clock unchanged | callback completion is not presentation");
        }
    }
}
