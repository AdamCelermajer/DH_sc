package com.example.dh2;
import java.util.Map;
public final class FramePacingReportV46Test {
    private static int checks;
    private static void check(boolean value){++checks;if(!value)throw new AssertionError("V46 report "+checks);}
    private static final class Driver implements FramePacingControllerV44.Driver {
        long requests;FramePacingControllerV44.Callback callback;
        public long nowNanos(){return 0;}
        public boolean isMainThread(){return true;}
        public void postFrame(FramePacingControllerV44.Callback c){callback=c;}
        public void removeFrame(FramePacingControllerV44.Callback c){if(callback==c)callback=null;}
        public void postTask(Runnable r){r.run();}
        public void removeTask(Runnable r){}
        public void requestRender(){++requests;}
        public void failure(RuntimeException e){throw e;}
    }
    public static void main(String[] args){
        Map<String,Object> defaults=FramePacingReportV46.capture(null);
        check("continuous-default".equals(defaults.get("mode")));check(Boolean.FALSE.equals(defaults.get("scheduler_snapshot_available")));
        check(Boolean.FALSE.equals(defaults.get("presented_fps_measured"))&&!defaults.containsKey("render_requests"));
        Driver d=new Driver();FramePacingControllerV44 c=new FramePacingControllerV44(d);
        c.onAttached(true);c.onResume();c.onWindowVisible(true);c.onWindowFocus(true);c.onSurfaceAvailable(true);
        c.onRendererContextCreated();c.onRendererSurfaceReady(1280,720);
        FramePacingControllerV44.Callback before=d.callback;before.doFrame(1_000_000_000L);
        long token=c.frameStarted();c.frameFinished(token);c.requestUpdate();
        before=d.callback;long requests=d.requests;Map<String,Object> exported=FramePacingReportV46.capture(c);
        check(d.callback==before&&d.requests==requests);check(exported.get("render_requests").equals(1L));
        check(exported.get("renderer_callback_starts").equals(1L)&&exported.get("renderer_callback_ends").equals(1L));
        check(exported.get("coalesced_update_requests").equals(1L));check(Boolean.FALSE.equals(exported.get("request_count_is_fps")));
        check(Boolean.FALSE.equals(exported.get("simulation_dt_from_scheduler")));check(Boolean.TRUE.equals(exported.get("active")));
        c.onPause();check(Boolean.FALSE.equals(FramePacingReportV46.capture(c).get("active")));
        c.close();check(Boolean.FALSE.equals(FramePacingReportV46.capture(c).get("pending_request")));
        System.out.println("PASS V46 one-shot pacing report: checks="+checks+" no scheduling/clock/IO mutation; no presented FPS claim");
    }
}
