"""Stage queued-versus-in-flight ownership; no MainActivity/native/GL edits."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/pacing-request-lifetime-v47';out.mkdir(parents=True,exist_ok=True)
package='port/android-native/app/src/main/java/com/example/dh2/';baseline={};patch=''
def change(name,fn):
 global patch
 path=package+name;before=(root/path).read_text();after=fn(before);assert after!=before
 baseline[path]=hashlib.sha256((root/path).read_bytes()).hexdigest();(out/name).write_text(after)
 patch+=''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
def core(s):
 old='''            if(!pending){++frameworkFrames;if(activeLocked())pending=true;}
            return activeFrame;}'''
 new='''            if(!pending)++frameworkFrames;
            // A callback consumes the queued request. One subsequent request may
            // be queued while this callback runs; it belongs to the NEXT frame.
            pending=false;
            return activeFrame;}'''
 assert old in s;s=s.replace(old,new)
 s=s.replace('activeFrame=0;pending=false;++frameEnds;','activeFrame=0;++frameEnds;')
 s=s.replace('public long frameStarted() {','public long frameStarted() {')
 s=s.replace('vsyncs,requests,blocked,cadenceSkipped,staleCallbacks,staleCompletions,frameStarts,frameEnds,frameworkFrames,updates);}}',
 'activeFrame!=0,vsyncs,requests,blocked,cadenceSkipped,staleCallbacks,staleCompletions,frameStarts,frameEnds,frameworkFrames,updates);}}')
 s=s.replace('public final boolean active,pending,dirty,failed;','public final boolean active,pending,dirty,failed,inFlight;')
 s=s.replace('boolean d,boolean f,long v,','boolean d,boolean f,boolean flight,long v,')
 s=s.replace('dirty=d;failed=f;vsyncs=v;','dirty=d;failed=f;inFlight=flight;vsyncs=v;')
 s=s.replace('private boolean closed,failed,pending,dirty=true;',
 '''// pending is only a queued render request. activeFrame is the separate
    // in-flight renderer callback; completing it never clears a newer request.
    private boolean closed,failed,pending,dirty=true;''')
 return s
change('FramePacingControllerV44.java',core)
def report(s):return s.replace('out.put("active",snapshot.active);out.put("pending_request",snapshot.pending);',
 'out.put("pending_request_definition","queued request, independent of renderer callback in flight");\n        out.put("renderer_callback_in_flight",snapshot.inFlight);\n        out.put("active",snapshot.active);out.put("pending_request",snapshot.pending);')
change('FramePacingReportV46.java',report)
(out/'integration.patch').write_bytes(patch.encode());(out/'baseline-source-sha256.json').write_text(json.dumps(baseline,indent=2)+'\n')
print('Staged single queued request plus one in-flight callback; default unchanged')
