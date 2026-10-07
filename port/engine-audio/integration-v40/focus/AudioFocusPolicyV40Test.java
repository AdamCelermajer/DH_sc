package com.example.dh2;
final class AudioFocusPolicyV40Test {
    private static int checks;
    private static void check(boolean value) { ++checks;if(!value)throw new AssertionError("focus policy check "+checks); }
    public static void main(String[] args) {
        final int[] requests={0},abandons={0},publishes={0},frontChanges={0};
        final boolean[] allow={true};
        final long[] lastSequence={0};
        AudioFocusPolicyV40 policy=new AudioFocusPolicyV40(1,new AudioFocusPolicyV40.Backend() {
            public boolean request() { ++requests[0];return allow[0]; }
            public void abandon() { ++abandons[0]; }
        },(owner,sequence,resumed,window,granted,dead)-> {
            check(owner==1&&sequence>lastSequence[0]);lastSequence[0]=sequence;++publishes[0];
            if(dead)check(!granted);
        });
        policy.frontListener(granted->frontChanges[0]++);
        policy.frontDemand(true);check(requests[0]==0&&!policy.canPlay());
        policy.resumed(true);check(requests[0]==0&&!policy.canPlay());
        policy.windowFocused(true);check(requests[0]==1&&policy.canPlay());
        policy.nativeDemand(true);policy.frontDemand(false);check(requests[0]==1&&abandons[0]==0&&policy.canPlay());
        policy.focusChanged(false,false);check(!policy.canPlay()&&requests[0]==1);
        policy.nativeDemand(true);check(requests[0]==1&&!policy.canPlay());
        policy.focusChanged(true,false);check(policy.canPlay());
        policy.windowFocused(false);check(!policy.canPlay()&&abandons[0]==1);
        policy.focusChanged(true,false);check(!policy.canPlay());
        policy.windowFocused(true);check(policy.canPlay()&&requests[0]==2);
        policy.resumed(false);check(!policy.canPlay()&&abandons[0]==2);
        allow[0]=false;policy.resumed(true);check(!policy.canPlay()&&requests[0]==3);
        for(int i=0;i<20;++i)policy.nativeDemand(true);
        check(requests[0]==3);
        policy.nativeDemand(false);allow[0]=true;policy.nativeDemand(true);check(requests[0]==4&&policy.canPlay());
        policy.focusChanged(false,true);policy.nativeDemand(true);check(!policy.canPlay()&&requests[0]==4);
        policy.nativeDemand(false);policy.nativeDemand(true);check(policy.canPlay()&&requests[0]==5);
        policy.close();check(!policy.canPlay()&&abandons[0]==3);
        int before=publishes[0];policy.resumed(true);policy.focusChanged(true,false);policy.frontDemand(true);policy.close();
        check(publishes[0]==before&&!policy.canPlay());
        check(frontChanges[0]>1);
        System.out.println("PASS "+checks+" focus checks; one shared requester, lifecycle and late callback gates");
    }
}
