"""Stage explicit debug-only activity pacing; no live activity/native changes."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/frame-pacing-v44';out.mkdir(parents=True,exist_ok=True)
path='port/android-native/app/src/main/java/com/example/dh2/MainActivity.java'
before=(root/path).read_text();s=before
s=s.replace('    private GLSurfaceView surface;','    private VsyncSurfaceViewV44 surface;\n    private volatile FramePacingControllerV44 framePacer;')
s=s.replace('surface=new GLSurfaceView(this);','surface=new VsyncSurfaceViewV44(this);')
s=s.replace('            @Override public void onSurfaceCreated(GL10 gl,EGLConfig config){',
'''            @Override public void onSurfaceCreated(GL10 gl,EGLConfig config){
                if(framePacer!=null)framePacer.onRendererContextCreated();''')
anchor='''                    if(assets.length>0)loadSelected();else show("No bundled asset fixtures");
                }
            }'''
assert anchor in s;s=s.replace(anchor,anchor[:-13]+'''                if(framePacer!=null)framePacer.onRendererSurfaceReady(w,h);
            }''',1)
s=s.replace('''                final long frameTicket=FrameBoundaryV42.beginFrame();
                try {''','''                final long pacingTicket=framePacer==null?0:framePacer.frameStarted();
                long frameTicket=0;
                try {
                frameTicket=FrameBoundaryV42.beginFrame();''',1)
s=s.replace('''                } finally { FrameBoundaryV42.endFrame(frameTicket); }''',
'''                } finally {
                    try { FrameBoundaryV42.endFrame(frameTicket); }
                    finally { if(framePacer!=null)framePacer.frameFinished(pacingTicket); }
                }''',1)
s=s.replace('''        surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);''',
'''        final boolean debugPacing=(getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0&&"vsync60".equals(getIntent().getStringExtra("frame_pacing"));
        if(debugPacing)framePacer=surface.enableVsync60();
        else {
            surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
            if(getIntent().hasExtra("frame_pacing"))Log.i("DH2Pacing","V44 opt-in refused or unknown; continuous default retained");
        }''')
s=s.replace('surface.requestRender();','requestFrameV44();')
insert='''    private void requestFrameV44(){
        final FramePacingControllerV44 pacing=framePacer;
        if(pacing!=null)pacing.requestUpdate();else surface.requestRender();
    }
'''
s=s.replace('    private void loadSelected(){',insert+'    private void loadSelected(){')
s=s.replace('@Override protected void onPause(){super.onPause();','@Override protected void onPause(){if(framePacer!=null)framePacer.onPause();super.onPause();')
s=s.replace('frontAudio.resume();surface.onResume();','frontAudio.resume();surface.onResume();if(framePacer!=null)framePacer.onResume();')
s=s.replace('@Override protected void onDestroy(){if(debugAttackReceiver','@Override protected void onDestroy(){if(surface!=null)surface.destroyPacingV44();if(debugAttackReceiver')
assert s!=before
(out/'MainActivity.java').write_text(s)
(out/'integration.patch').write_bytes(''.join(difflib.unified_diff(before.splitlines(True),s.splitlines(True),fromfile='a/'+path,tofile='b/'+path)).encode())
(out/'baseline-source-sha256.json').write_text(json.dumps({path:hashlib.sha256((root/path).read_bytes()).hexdigest()},indent=2)+'\n')
# Static source contract receipt: scheduling never supplies dt to the JNI bridge.
model=root/'port/android-native/app/src/main/cpp/model_renderer.cpp';text=model.read_text()
start=text.index('  if(world_mode){const auto now=std::chrono::steady_clock::now();',text.index('void draw(int width,int height)'))
end=text.index('    const auto& target=',start);timing=text[start:end]
assert 'const unsigned dt_ms=now_ms-previous_ms;last_frame=now;' in timing
assert 'if(dt_ms<=2000)' in timing and 'advance_native_actor(dt_ms)' in timing
assert s.count('NativeBridge.draw();')==before.count('NativeBridge.draw();')==1
assert 'finally { FrameBoundaryV42.endNative(frameTicket); }' in s
(out/'native-wall-clock-contract.json').write_text(json.dumps({'status':'UNCHANGED_SOURCE_CONTRACT','native_file':str(model),'timing_block':timing,'sha256':hashlib.sha256(timing.encode()).hexdigest(),'NativeBridge.draw_calls':1,'changes_to_native':False,'changes_to_egl':False,'simulation_dt_from_choreographer':False},indent=2)+'\n')
print('Staged MainActivity explicit debug opt-in; native timing contract unchanged')
