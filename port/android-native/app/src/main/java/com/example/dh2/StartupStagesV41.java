package com.example.dh2;

/** Debug startup measurement only. Caller loads the existing native library,
 * begins a real request and explicitly records success; no game progress/state
 * or background work is inferred. No logging or library loading in this class. */
public final class StartupStagesV41 {
    private StartupStagesV41() {}
    public static final int COLD_START=0, CONTEXT_RESTORE=1, DEMO_LAUNCH=2, SOURCE_CAMPAIGN=3;
    public static final int MEASURED_RETURN=0, CANCELLED=1, FAILED=2;
    public static final int ACTIVITY_CREATE=0, ASSET_LISTING=1, SURFACE_CREATE=2,
        WAIT_VALID_SURFACE=3, NATIVE_INITIALIZE=4, SHADER_VALIDATION=5,
        UI_GPU_INITIALIZE=6, FRONT_INITIALIZE=7, SURFACE_RESIZE=8,
        LOAD_SELECTED=9, JAVA_ASSET_READ=10, JAVA_PROVENANCE_READ=11,
        JNI_INPUT_COPY=12, WORLD_LOAD=13, DESIGN_ASSETS=14, WORLD_PARSE=15,
        OBJECT_RESOURCES=16, TEXTURE_DECODE=17, TEXTURE_UPLOAD=18,
        PLAYER_CONSTRUCT=19, LEVEL_C1=20, WORLD_PUBLISH=21, HUD_LOAD=22,
        UI_CONSTANTS=23, UI_LOCALIZATION=24, SWF_PARSE=25,
        UI_BITMAP_DECODE=26, UI_BITMAP_UPLOAD=27, UI_FONT_UPLOAD=28,
        HUD_BIND=29, FIRST_WORLD_SUBMIT=30, FIRST_HUD_SUBMIT=31,
        NATIVE_DRAW=32, MENU_LAUNCH=33;
    public static native long begin(int request,long javaOriginNanos);
    private static native long start(long generation,int stage,long bytesIn);
    private static native void end(long ticket,boolean success,long bytesOut);
    public static native String finish(long generation,int outcome,boolean includeEvents);
    public static Scope scope(long generation,int stage,long bytesIn) {
        return new Scope(start(generation,stage,bytesIn));
    }
    public static final class Scope implements AutoCloseable {
        private long ticket;
        private boolean success;
        private long bytesOut;
        private Scope(long value) { ticket=value; }
        public void succeeded(long bytes) { success=true; bytesOut=Math.max(0,bytes); }
        @Override public void close() {
            if(ticket!=0) { end(ticket,success,bytesOut); ticket=0; }
        }
    }
}
