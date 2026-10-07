package com.example.dh2;
public final class FramePacingQueueV47Test {
    private static int checks;
    private static void check(boolean b){++checks;if(!b)throw new AssertionError("V47 check "+checks);}
    private static final class Driver implements FramePacingControllerV44.Driver {
        final Thread main=Thread.currentThread();FramePacingControllerV44.Callback queued;Runnable task;FramePacingControllerV44 controller;
        long now,requests;public long nowNanos(){return now;}public boolean isMainThread(){return Thread.currentThread()==main;}
        public void postFrame(FramePacingControllerV44.Callback c){check(queued==null);queued=c;}
        public void removeFrame(FramePacingControllerV44.Callback c){if(queued==c)queued=null;}
        public void postTask(Runnable r){task=r;}public void removeTask(Runnable r){if(task==r)task=null;}
        public void requestRender(){++requests;}public void failure(RuntimeException e){throw e;}
        void flush(){Runnable r=task;task=null;if(r!=null)r.run();}
        void vsync(long t){now=t;FramePacingControllerV44.Callback c=queued;queued=null;if(c!=null)c.doFrame(t);}
    }
    private static Driver active(){
        Driver d=new Driver();FramePacingControllerV44 c=new FramePacingControllerV44(d);d.controller=c;
        c.onResume();c.onAttached(true);c.onWindowVisible(true);c.onWindowFocus(true);c.onSurfaceAvailable(true);
        c.onRendererContextCreated();c.onRendererSurfaceReady(1280,720);d.flush();return d;
    }
    public static void main(String[] args)throws Exception {
        Driver d=active();FramePacingControllerV44 c=d.controller;d.vsync(1_000_000_000L);
        check(d.requests==1&&c.snapshot().pending&&!c.snapshot().inFlight);
        long first=c.frameStarted();check(!c.snapshot().pending&&c.snapshot().inFlight);
        d.vsync(1_016_666_667L);check(d.requests==2&&c.snapshot().pending&&c.snapshot().inFlight);
        for(int i=2;i<8;++i)d.vsync(1_000_000_000L+i*16_666_667L);
        check(d.requests==2&&c.snapshot().pending); // At most one queued behind the slow callback.
        c.frameFinished(first);check(c.snapshot().pending&&!c.snapshot().inFlight);
        long second=c.frameStarted();check(!c.snapshot().pending&&c.snapshot().inFlight);
        c.frameFinished(second);check(!c.snapshot().pending&&!c.snapshot().inFlight);
        d.vsync(1_200_000_000L);check(d.requests==3);
        // A framework-mandated draw without a request can still accept one
        // queued successor, and completing it must preserve that successor.
        long third=c.frameStarted();c.frameFinished(third);long automatic=c.frameStarted();
        d.vsync(1_216_666_667L);check(d.requests==4);c.frameFinished(automatic);check(c.snapshot().pending);
        check(c.snapshot().frameworkFrames==1);
        long old=c.frameStarted();FramePacingControllerV44.Callback oldCallback=d.queued;
        c.onPause();c.onResume();d.vsync(1_300_000_000L);check(c.snapshot().pending);
        c.frameFinished(old);check(c.snapshot().pending&&c.snapshot().staleCompletions==1);
        FramePacingControllerV44.Callback fresh=d.queued;oldCallback.doFrame(1_400_000_000L);check(d.queued==fresh);
        long current=c.frameStarted();c.frameFinished(current);
        // Context loss invalidates both ownerships without accepting stale work.
        d.vsync(1_500_000_000L);long contextOld=c.frameStarted();d.vsync(1_516_666_667L);
        check(c.snapshot().pending&&c.snapshot().inFlight);c.onRendererContextCreated();d.flush();
        check(!c.snapshot().pending&&!c.snapshot().inFlight&&d.queued==null);
        c.onRendererSurfaceReady(1280,720);d.flush();d.vsync(1_600_000_000L);c.frameFinished(contextOld);
        check(c.snapshot().pending);current=c.frameStarted();c.frameFinished(current);
        c.onSurfaceAvailable(false);check(d.queued==null&&!c.snapshot().pending&&!c.snapshot().inFlight);
        c.onSurfaceAvailable(true);c.onRendererSurfaceReady(1280,720);d.flush();
        d.vsync(1_700_000_000L);long closing=c.frameStarted();d.vsync(1_716_666_667L);c.close();c.frameFinished(closing);
        check(d.queued==null&&!c.snapshot().pending&&!c.snapshot().inFlight);
        // A real separate GL thread completion cannot clear the next queued request.
        Driver threaded=active();FramePacingControllerV44 tc=threaded.controller;threaded.vsync(1_000_000_000L);
        long ticket=tc.frameStarted();threaded.vsync(1_016_666_667L);
        Thread gl=new Thread(()->tc.frameFinished(ticket));gl.start();gl.join(2000);
        check(!gl.isAlive()&&tc.snapshot().pending&&!tc.snapshot().inFlight);tc.close();
        System.out.println("PASS V47 queued/in-flight ownership: checks="+checks+" single queued successor preserved across completion/epoch/context/close; no FPS claim");
    }
}
