package com.example.dh2;

/** Experimental render scheduling only. Never supplies a simulation timestamp,
 * runs native work, swaps EGL, or treats a render request as presentation. */
public final class FramePacingControllerV44 {
    public interface Callback { void doFrame(long frameTimeNanos); }
    public interface Driver {
        long nowNanos();
        boolean isMainThread();
        void postFrame(Callback callback);
        void removeFrame(Callback callback);
        void postTask(Runnable task);
        void removeTask(Runnable task);
        void requestRender();
        void failure(RuntimeException error);
    }
    // An engineering experiment's cadence, not a game clock or physics step.
    // Floor division avoids accidentally rejecting exact 60 Hz timestamps at
    // 16,666,666 ns. Drift is 40 ns/s, immaterial for bounded comparisons.
    public static final long TARGET_PERIOD_NS=1_000_000_000L/60;
    private static final int RESUMED=1,ATTACHED=2,VISIBLE=4,FOCUSED=8,SURFACE=16,CONTEXT=32;
    private static final int ALL=RESUMED|ATTACHED|VISIBLE|FOCUSED|SURFACE|CONTEXT;
    private final Driver driver;
    private final Object lock=new Object();
    private final Runnable reconcileTask=this::reconcile;
    // Main-thread callback ownership. Each epoch allocates one callback, which
    // is reused for every vsync in that epoch. Old callbacks cannot rearm it.
    private EpochCallback callback;
    private boolean posted;
    // Protected by lock; renderer methods below may run on the existing GL thread.
    private int state;
    private boolean closed,failed,pending,dirty=true;
    private long epoch=1,minimumTimestamp,lastVsync=Long.MIN_VALUE,nextDue=Long.MIN_VALUE;
    private long sequence,activeFrame;
    private long vsyncs,requests,blocked,cadenceSkipped,staleCallbacks,staleCompletions;
    private long frameStarts,frameEnds,frameworkFrames,updates;

    public FramePacingControllerV44(Driver value) {
        if(value==null)throw new IllegalArgumentException("V44 driver required");
        driver=value;requireMain();minimumTimestamp=driver.nowNanos();
    }
    private void requireMain() {
        if(!driver.isMainThread())throw new IllegalStateException("V44 scheduling belongs to main thread");
    }
    private boolean activeLocked(){return !closed&&!failed&&state==ALL;}
    private void invalidateLocked() {
        if(epoch==Long.MAX_VALUE)throw new IllegalStateException("V44 epoch exhausted");
        ++epoch;minimumTimestamp=driver.nowNanos();lastVsync=nextDue=Long.MIN_VALUE;
        pending=false;activeFrame=0;dirty=true;
    }
    private void mainState(int mask,boolean value) {
        requireMain();synchronized(lock){if(closed)return;int next=value?state|mask:state&~mask;
            if(next!=state){state=next;invalidateLocked();}}
        reconcile();
    }
    public void onResume(){mainState(RESUMED,true);}
    public void onPause(){mainState(RESUMED,false);}
    public void onAttached(boolean value){mainState(ATTACHED,value);}
    public void onWindowVisible(boolean value){mainState(VISIBLE,value);}
    public void onWindowFocus(boolean value){mainState(FOCUSED,value);}
    public void onSurfaceAvailable(boolean value){mainState(value?SURFACE:SURFACE|CONTEXT,value);}

    // Called from GLSurfaceView.Renderer, never from Choreographer.
    public void onRendererContextCreated() {
        synchronized(lock){if(closed)return;state&=~CONTEXT;invalidateLocked();}
        driver.postTask(reconcileTask);
    }
    public void onRendererSurfaceReady(int width,int height) {
        synchronized(lock){if(closed)return;int next=width>1&&height>1?state|CONTEXT:state&~CONTEXT;
            if(next!=state){state=next;invalidateLocked();}dirty=true;}
        driver.postTask(reconcileTask);
    }
    /** Coalesces existing UI/load requests; vsync continues while active. This
     * method is safe on main or GL thread and never posts a per-frame task. */
    public void requestUpdate(){synchronized(lock){if(!closed){dirty=true;++updates;}}}

    /** Acknowledges callback execution, not EGL swap or display presentation.
     * Framework-mandated first/resize draws still execute the real source draw. */
    public long frameStarted() {
        synchronized(lock){if(sequence==Long.MAX_VALUE)throw new IllegalStateException("V44 frame ticket exhausted");
            if(activeFrame!=0)throw new IllegalStateException("V44 overlapping GL callbacks");
            activeFrame=++sequence;++frameStarts;
            if(!pending){++frameworkFrames;if(activeLocked())pending=true;}
            return activeFrame;}
    }
    public void frameFinished(long ticket) {
        synchronized(lock){if(ticket!=0&&ticket==activeFrame){activeFrame=0;pending=false;++frameEnds;}
            else ++staleCompletions;}
    }
    private final class EpochCallback implements Callback {
        private final long identity;
        EpochCallback(long value){identity=value;}
        @Override public void doFrame(long frameTimeNanos){onVsync(this,frameTimeNanos);}
    }
    private void onVsync(EpochCallback value,long time) {
        requireMain();
        if(value!=callback||!posted){synchronized(lock){++staleCallbacks;}return;}
        posted=false;boolean issue=false;
        synchronized(lock){
            if(!activeLocked()||value.identity!=epoch||time<minimumTimestamp||time<=lastVsync){++staleCallbacks;}
            else {
                lastVsync=time;++vsyncs;
                if(pending){++blocked;}
                else if(nextDue!=Long.MIN_VALUE&&time<nextDue){++cadenceSkipped;}
                else if(time>Long.MAX_VALUE-TARGET_PERIOD_NS){failed=true;++staleCallbacks;}
                else {
                    // Skip missed deadlines in O(1); never enqueue catch-up frames.
                    nextDue=time+TARGET_PERIOD_NS-(nextDue==Long.MIN_VALUE?0:(time-nextDue)%TARGET_PERIOD_NS);
                    pending=true;dirty=false;++requests;issue=true;
                }
            }
        }
        if(issue){try{driver.requestRender();}
            catch(RuntimeException error){synchronized(lock){failed=true;invalidateLocked();}driver.failure(error);}}
        reconcile();
    }
    private void reconcile() {
        requireMain();long identity;boolean active;
        synchronized(lock){identity=epoch;active=activeLocked();}
        if(!active||callback!=null&&callback.identity!=identity){
            if(callback!=null&&posted)driver.removeFrame(callback);
            callback=null;posted=false;
        }
        if(active){
            if(callback==null)callback=new EpochCallback(identity);
            if(!posted){posted=true;driver.postFrame(callback);}
        }
    }
    public void close() {
        requireMain();synchronized(lock){if(closed)return;closed=true;invalidateLocked();}
        reconcile();driver.removeTask(reconcileTask);
    }
    /** Explicit readiness/diagnostic capture only; never called once per vsync.
     * Counters report scheduler requests/callbacks, not an invented FPS value. */
    public Snapshot snapshot(){synchronized(lock){return new Snapshot(epoch,state,activeLocked(),pending,dirty,failed,
        vsyncs,requests,blocked,cadenceSkipped,staleCallbacks,staleCompletions,frameStarts,frameEnds,frameworkFrames,updates);}}
    public static final class Snapshot {
        public final long epoch,vsyncs,requests,blocked,cadenceSkipped,staleCallbacks,staleCompletions,frameStarts,frameEnds,frameworkFrames,updates;
        public final int state;
        public final boolean active,pending,dirty,failed;
        Snapshot(long e,int s,boolean a,boolean p,boolean d,boolean f,long v,long r,long b,long c,long sc,long sf,long fs,long fe,long ff,long u){
            epoch=e;state=s;active=a;pending=p;dirty=d;failed=f;vsyncs=v;requests=r;blocked=b;cadenceSkipped=c;
            staleCallbacks=sc;staleCompletions=sf;frameStarts=fs;frameEnds=fe;frameworkFrames=ff;updates=u;
        }
    }
}
