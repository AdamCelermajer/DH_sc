package com.example.dh2;
import java.util.LinkedHashMap;
import java.util.Map;

/** Explicit one-shot export outside a draw callback. Pure data conversion, no
 * Android SDK mirror, scheduling, clocks, native call, frame wait or IO. */
public final class FramePacingReportV46 {
    private FramePacingReportV46(){}
    public static Map<String,Object> capture(FramePacingControllerV44 controller){
        final Map<String,Object> out=new LinkedHashMap<>();
        out.put("schema","dh2-frame-pacing-v46");
        out.put("mode",controller==null?"continuous-default":"vsync60-debug-opt-in");
        out.put("presented_fps_measured",false);
        out.put("request_count_is_fps",false);
        out.put("callback_completion_is_presentation",false);
        out.put("simulation_dt_from_scheduler",false);
        out.put("native_timing_reference","boundary.rows and guestThread; presentation requires external capture");
        if(controller==null){out.put("scheduler_snapshot_available",false);return out;}
        final FramePacingControllerV44.Snapshot snapshot=controller.snapshot();
        out.put("scheduler_snapshot_available",true);out.put("target_period_ns",FramePacingControllerV44.TARGET_PERIOD_NS);
        out.put("epoch",snapshot.epoch);out.put("lifecycle_state_bits",snapshot.state);
        out.put("active",snapshot.active);out.put("pending_request",snapshot.pending);out.put("update_dirty",snapshot.dirty);out.put("failed",snapshot.failed);
        out.put("vsync_callbacks",snapshot.vsyncs);out.put("render_requests",snapshot.requests);
        out.put("pending_request_skips",snapshot.blocked);out.put("cadence_skips",snapshot.cadenceSkipped);
        out.put("stale_callbacks",snapshot.staleCallbacks);out.put("stale_completions",snapshot.staleCompletions);
        out.put("renderer_callback_starts",snapshot.frameStarts);out.put("renderer_callback_ends",snapshot.frameEnds);
        out.put("framework_or_unrequested_callbacks",snapshot.frameworkFrames);out.put("coalesced_update_requests",snapshot.updates);
        return out;
    }
}
