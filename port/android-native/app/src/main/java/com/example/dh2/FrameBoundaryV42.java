package com.example.dh2;
import android.os.Debug;
import android.os.Build;
import android.os.Process;
import android.os.Trace;
import org.json.JSONArray;
import org.json.JSONObject;

/** Explicit bounded diagnostics on the existing GL thread. Never swaps,
 * schedules a frame, finishes GPU work, logs per frame or changes game state. */
public final class FrameBoundaryV42 {
    private static final int LIMIT=512;
    private static final long DURATION=20_000_000_000L;
    private static final long[] starts=new long[LIMIT],nativeStarts=new long[LIMIT],
        nativeEnds=new long[LIMIT],ends=new long[LIMIT],cpuStarts=new long[LIMIT],cpuEnds=new long[LIMIT];
    private static boolean enabled,nativeOpen,tailOpen;
    private static int count,ownerTid;
    private static long origin,ticket;
    private FrameBoundaryV42() {}
    // Configure only on the owning GL thread, outside an active callback.
    public static void configure(boolean value) {
        if(ticket!=0)throw new IllegalStateException("Active frame measurement");
        enabled=value;count=0;ownerTid=Process.myTid();origin=0;nativeOpen=false;tailOpen=false;
    }
    public static long beginFrame() {
        if(!enabled||count==LIMIT||Build.VERSION.SDK_INT<29||!Trace.isEnabled())return 0;
        long now=System.nanoTime();if(origin==0)origin=now;
        if(now-origin>=DURATION){enabled=false;return 0;}
        if(ownerTid!=Process.myTid()||ticket!=0)throw new IllegalStateException("Frame measurement owner/order");
        starts[count]=now;nativeStarts[count]=nativeEnds[count]=ends[count]=0;
        cpuStarts[count]=Debug.threadCpuTimeNanos();ticket=count+1;
        Trace.beginSection("DH2:onDrawFrame");return ticket;
    }
    public static void beginNative(long value) {
        if(value==0)return;require(value);if(nativeOpen)throw new IllegalStateException("Nested native frame");
        nativeStarts[count]=System.nanoTime();nativeOpen=true;Trace.beginSection("DH2:NativeBridge.draw");
    }
    public static void endNative(long value) {
        if(value==0)return;require(value);if(!nativeOpen)throw new IllegalStateException("Unopened native frame");
        nativeEnds[count]=System.nanoTime();Trace.endSection();nativeOpen=false;
        Trace.beginSection("DH2:JavaPostNative");tailOpen=true;
    }
    public static void endFrame(long value) {
        if(value==0)return;require(value);
        if(nativeOpen){nativeEnds[count]=System.nanoTime();Trace.endSection();nativeOpen=false;}
        if(tailOpen){Trace.endSection();tailOpen=false;}
        ends[count]=System.nanoTime();cpuEnds[count]=Debug.threadCpuTimeNanos();Trace.endSection();
        ticket=0;++count;if(count==LIMIT)enabled=false;
    }
    private static void require(long value) {
        if(value!=ticket||ownerTid!=Process.myTid())throw new IllegalStateException("Stale frame measurement");
    }
    // Call on the owning GL thread after capture, outside its draw callback;
    // transfer the returned immutable String for persistence on another thread.
    // Raw times do not prove
    // EGL swap/presentation or hostQEMU scheduling; source success is separate.
    public static String report() throws org.json.JSONException {
        if(ticket!=0||enabled)throw new IllegalStateException("Capture still active; finish first");
        if(ownerTid!=Process.myTid())throw new IllegalStateException("Wrong report owner");
        JSONObject report=new JSONObject();report.put("schema","dh2-frame-boundary-v42");report.put("tid",ownerTid);
        report.put("frames",count);report.put("max_frames",LIMIT);report.put("max_ns",DURATION);
        JSONArray rows=new JSONArray();for(int i=0;i<count;++i){JSONArray row=new JSONArray();
            row.put(starts[i]);row.put(nativeStarts[i]);row.put(nativeEnds[i]);row.put(ends[i]);row.put(cpuStarts[i]);row.put(cpuEnds[i]);rows.put(row);}
        report.put("columns","callback_begin_ns,native_begin_ns,native_end_ns,callback_end_ns,guest_thread_cpu_begin_ns,guest_thread_cpu_end_ns");report.put("rows",rows);return report.toString();
    }
    public static void finish(){if(ticket!=0||ownerTid!=Process.myTid())throw new IllegalStateException("Active callback or wrong owner");enabled=false;}
    // Explicit one-off GL-thread discovery only; library must already be loaded.
    public static native String discoverPresentCapabilities();
    public static native String sampleGuestThreadCounters();
}
