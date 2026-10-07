package com.example.dh2;

/** Main-thread state owner shared by title MediaPlayers and native audio. */
final class AudioFocusPolicyV40 {
    interface Backend { boolean request(); void abandon(); }
    interface NativeEndpoint { void publish(long owner,long sequence,boolean resumed,boolean windowFocused,boolean granted,boolean destroyed); }
    interface FrontEndpoint { void changed(boolean granted); }
    private final Backend backend;
    private final NativeEndpoint nativeEndpoint;
    private final long owner;
    private long sequence;
    private FrontEndpoint frontEndpoint;
    private boolean resumed,windowFocused,frontDemand,nativeDemand,held,attempted,granted,destroyed,lastFront;
    AudioFocusPolicyV40(long owner,Backend backend,NativeEndpoint endpoint) {
        if(owner<=0||owner>0xffffffffL)throw new IllegalArgumentException("Activity audio owner range");
        this.owner=owner;this.backend=backend;nativeEndpoint=endpoint;publish();
    }
    void frontListener(FrontEndpoint endpoint) { frontEndpoint=endpoint;endpoint.changed(canPlay()); }
    boolean canPlay() { return !destroyed&&resumed&&windowFocused&&granted; }
    void resumed(boolean value) { if(destroyed)return;resumed=value;update(); }
    void windowFocused(boolean value) { if(destroyed)return;windowFocused=value;update(); }
    void frontDemand(boolean value) { if(destroyed)return;frontDemand=value;update(); }
    void nativeDemand(boolean value) { if(destroyed)return;nativeDemand=value;update(); }
    private boolean wanted() { return resumed&&windowFocused&&(frontDemand||nativeDemand); }
    private void update() {
        if(!wanted()) { granted=false;if(held)backend.abandon();held=false;attempted=false; }
        else if(!held&&!attempted) { attempted=true;held=backend.request();granted=held; }
        publish();
    }
    // All loss modes pause. No second focus request or automatic retry from
    // the callback. A real foreground/demand transition can request again.
    void focusChanged(boolean gain,boolean permanentLoss) {
        if(destroyed)return;
        granted=gain&&wanted()&&attempted;
        if(permanentLoss)held=false;
        publish();
    }
    void close() {
        if(destroyed)return;
        destroyed=true;granted=false;if(held)backend.abandon();held=false;publish();
    }
    private void publish() {
        final boolean front=canPlay();
        if(front!=lastFront) { lastFront=front;if(frontEndpoint!=null)frontEndpoint.changed(front); }
        if(++sequence>0xffffffL)throw new IllegalStateException("Activity audio sequence exhausted");
        nativeEndpoint.publish(owner,sequence,resumed,windowFocused,granted,destroyed);
    }
}
