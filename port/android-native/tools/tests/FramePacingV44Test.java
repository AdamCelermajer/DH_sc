package com.example.dh2;

import java.lang.management.ManagementFactory;
import com.sun.management.ThreadMXBean;

/** Tests the actual portable scheduler, not a mirror of Android SDK classes. */
public final class FramePacingV44Test {
    private static int checks;
    private static void check(boolean value){++checks;if(!value)throw new AssertionError("V44 check "+checks);}
    private static final class FakeDriver implements FramePacingControllerV44.Driver {
        final Thread main=Thread.currentThread();
        FramePacingControllerV44 controller;
        FramePacingControllerV44.Callback queued,lastIdentity;
        Runnable task;
        long now,requests,posts,removes,identities,failures;
        boolean complete,throwRequest;
        @Override public long nowNanos(){return now;}
        @Override public boolean isMainThread(){return Thread.currentThread()==main;}
        @Override public void postFrame(FramePacingControllerV44.Callback callback){
            if(queued!=null)throw new AssertionError("Duplicate pending callback");
            queued=callback;++posts;if(lastIdentity!=callback){lastIdentity=callback;++identities;}
        }
        @Override public void removeFrame(FramePacingControllerV44.Callback callback){if(queued==callback)queued=null;++removes;}
        @Override public synchronized void postTask(Runnable value){task=value;}
        @Override public synchronized void removeTask(Runnable value){if(task==value)task=null;}
        @Override public void requestRender(){
            if(throwRequest)throw new IllegalStateException("Synthetic request failure");
            ++requests;if(complete){long token=controller.frameStarted();controller.frameFinished(token);}
        }
        @Override public void failure(RuntimeException error){++failures;}
        void drain(){Runnable value; synchronized(this){value=task;task=null;}if(value!=null)value.run();}
        void frame(long timestamp){now=timestamp;FramePacingControllerV44.Callback current=queued;queued=null;if(current!=null)current.doFrame(timestamp);}
    }
    private static FakeDriver create(){FakeDriver d=new FakeDriver();d.controller=new FramePacingControllerV44(d);return d;}
    private static void activate(FakeDriver d){
        FramePacingControllerV44 c=d.controller;
        c.onAttached(true);c.onWindowVisible(true);c.onWindowFocus(true);c.onSurfaceAvailable(true);c.onResume();
        check(d.queued==null);c.onRendererContextCreated();c.onRendererSurfaceReady(1280,720);d.drain();
        check(c.snapshot().active&&d.queued!=null);
    }
    public static void main(String[] args)throws Exception {
        FakeDriver firstLoad=create();firstLoad.controller.requestUpdate();
        long automaticFirst=firstLoad.controller.frameStarted();firstLoad.controller.frameFinished(automaticFirst);
        check(firstLoad.queued==null&&firstLoad.controller.snapshot().frameworkFrames==1);
        activate(firstLoad);firstLoad.complete=true;firstLoad.frame(1_000_000_000L);
        check(firstLoad.requests==1&&firstLoad.controller.snapshot().updates==1);firstLoad.controller.close();
        for(int hz:new int[]{30,60,90,120,144,240}){
            FakeDriver d=create();activate(d);d.complete=true;
            for(int i=0;i<hz*2;++i)d.frame(1_000_000_000L+(long)i*1_000_000_000L/hz);
            long expected=Math.min(hz,60)*2L;
            check(Math.abs(d.requests-expected)<=1);check(d.identities==1);
            check(d.controller.snapshot().frameStarts==d.requests);
            d.controller.close();check(d.queued==null);
        }
        // Cadence responds to timestamps, not an assumed display tick divisor.
        FakeDriver irregular=create();activate(irregular);irregular.complete=true;long timestamp=1_000_000_000L;
        for(int i=0;i<400;++i){timestamp+=i%2==0?7_500_000L:9_200_000L;irregular.frame(timestamp);}
        check(Math.abs(irregular.requests-(timestamp-1_000_000_000L)/FramePacingControllerV44.TARGET_PERIOD_NS)<=2);
        irregular.controller.close();

        FakeDriver pending=create();activate(pending);pending.frame(1_000_000_000L);check(pending.requests==1);
        for(int i=1;i<=20;++i)pending.frame(1_000_000_000L+i*8_333_333L);
        check(pending.requests==1&&pending.controller.snapshot().pending&&pending.controller.snapshot().blocked==20);
        long first=pending.controller.frameStarted();pending.controller.frameFinished(first);
        pending.frame(1_500_000_000L);check(pending.requests==2); // One, never a catch-up burst.
        FramePacingControllerV44.Callback old=pending.queued;
        long oldFrame=pending.controller.frameStarted();pending.now=1_600_000_000L;pending.controller.onPause();check(pending.queued==null);
        pending.now=1_700_000_000L;pending.controller.requestUpdate();pending.controller.onResume();
        FramePacingControllerV44.Callback fresh=pending.queued;check(fresh!=old);
        old.doFrame(1_800_000_000L);check(pending.queued==fresh&&pending.requests==2);
        pending.frame(1_800_000_000L);check(pending.requests==3);
        pending.controller.frameFinished(oldFrame);check(pending.controller.snapshot().pending);
        long newFrame=pending.controller.frameStarted();pending.controller.frameFinished(newFrame);check(!pending.controller.snapshot().pending);
        check(pending.controller.snapshot().staleCompletions==1);
        // A stale timestamp in a new epoch cannot consume a legitimate new request.
        pending.controller.onWindowFocus(false);pending.now=2_000_000_000L;pending.controller.onWindowFocus(true);
        pending.frame(1_900_000_000L);check(pending.requests==3&&pending.queued!=null);
        pending.complete=true;pending.frame(2_100_000_000L);check(pending.requests==4);
        pending.controller.onWindowVisible(false);check(pending.queued==null&&!pending.controller.snapshot().active);
        pending.controller.onWindowVisible(true);check(pending.queued!=null);
        pending.controller.onAttached(false);check(pending.queued==null);pending.controller.onAttached(true);check(pending.queued!=null);
        pending.controller.onSurfaceAvailable(false);check(pending.queued==null);
        pending.controller.onSurfaceAvailable(true);check(pending.queued==null); // Real renderer readiness is required.
        pending.controller.onRendererContextCreated();pending.controller.onRendererSurfaceReady(0,0);pending.drain();check(pending.queued==null);
        pending.controller.onRendererSurfaceReady(1920,1080);pending.drain();check(pending.queued!=null);
        pending.controller.onRendererContextCreated();pending.controller.onRendererSurfaceReady(1920,1080);
        pending.controller.onPause();pending.drain();check(pending.queued==null);
        pending.controller.onResume();check(pending.queued!=null);
        pending.controller.close();check(pending.queued==null&&pending.task==null);
        fresh.doFrame(5_000_000_000L);check(pending.queued==null);pending.controller.requestUpdate();check(pending.queued==null);

        // Renderer acknowledgements/context events are safe on the GL thread;
        // main scheduling/lifecycle calls from that thread are rejected.
        FakeDriver threaded=create();activate(threaded);final boolean[] rejected={false};
        Thread worker=new Thread(()->{
            try{threaded.controller.onPause();}catch(IllegalStateException expected){rejected[0]=true;}
            threaded.controller.requestUpdate();threaded.controller.onRendererContextCreated();
            threaded.controller.onRendererSurfaceReady(1280,720);long frame=threaded.controller.frameStarted();threaded.controller.frameFinished(frame);
        });worker.start();worker.join(2000);check(!worker.isAlive()&&rejected[0]);threaded.drain();check(threaded.queued!=null);
        check(threaded.controller.snapshot().frameworkFrames==1);threaded.controller.close();

        FakeDriver fault=create();activate(fault);fault.throwRequest=true;fault.frame(1_000_000_000L);
        check(fault.controller.snapshot().failed&&!fault.controller.snapshot().pending&&fault.failures==1&&fault.queued==null);
        fault.controller.close();

        // Warm controller path has no epoch/callback allocation. FakeDriver's
        // bridge also retains one identity; Android SDK/framework allocations
        // are explicitly outside this host assertion.
        FakeDriver warm=create();activate(warm);warm.complete=true;
        for(int i=0;i<20_000;++i)warm.frame(1_000_000_000L+i*8_333_333L);
        ThreadMXBean bean=(ThreadMXBean)ManagementFactory.getThreadMXBean();bean.setThreadAllocatedMemoryEnabled(true);
        long owner=Thread.currentThread().threadId(),before=bean.getThreadAllocatedBytes(owner);
        for(int i=20_000;i<120_000;++i)warm.frame(1_000_000_000L+i*8_333_333L);
        long allocated=bean.getThreadAllocatedBytes(owner)-before;
        check(warm.identities==1&&allocated<=4096);warm.controller.close();
        System.out.println("PASS V44 actual controller: checks="+checks+" warm_callbacks=100000 warm_allocated_bytes="+allocated+
            " cadence=30/60/90/120/144/240Hz lifecycle=window/surface/context/pause/close stale_ticket_safe=true");
    }
}
