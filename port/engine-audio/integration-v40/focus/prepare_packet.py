from pathlib import Path
import hashlib,json,difflib
ROOT=Path(__file__).resolve().parents[4]
OUT=Path(__file__).resolve().parent
JAVA=ROOT/'port/android-native/app/src/main/java/com/example/dh2'
staged=OUT/'staged/java/com/example/dh2';staged.mkdir(parents=True,exist_ok=True)
def replace(text,old,new):
    assert text.count(old)==1,(old[:80],text.count(old))
    return text.replace(old,new)
original_bytes={name:(JAVA/name).read_bytes() for name in ('MainActivity.java','FrontAudio.java','NativeBridge.java')}
originals={name:data.decode().replace('\r\n','\n') for name,data in original_bytes.items()}
front=originals['FrontAudio.java']
front=replace(front,'private final AudioManager audio;\n    private AudioFocusRequest focus;','private final SharedAudioFocusV40 focusOwner;\n    private boolean requestingSharedFocus;')
front=replace(front,'requested, resumed=true,','requested, resumed=false,')
begin=front.index('    FrontAudio(Context c)');end=front.index('    private AudioAttributes attributes()',begin)
front=front[:begin]+'''    FrontAudio(Context c,SharedAudioFocusV40 owner){context=c;focusOwner=owner;
        owner.setFrontListener(granted->{
            focused=granted;
            if(!focused)stopEffects();
            if(requestingSharedFocus)return;
            if(current!=null&&started){if(focused&&resumed&&requested&&!musicPaused)current.start();else if(current.isPlaying())current.pause();}
            else if(focused)startPrepared();
        });
    }
'''+front[end:]
front=replace(front,'private boolean requestFocus(){return (Build.VERSION.SDK_INT>=26?audio.requestAudioFocus(focus):audio.requestAudioFocus(null,AudioManager.STREAM_MUSIC,AudioManager.AUDIOFOCUS_GAIN))==AudioManager.AUDIOFOCUS_REQUEST_GRANTED;}','private boolean requestFocus(){requestingSharedFocus=true;try{focusOwner.setFrontDemand(true);return focusOwner.canPlay();}finally{requestingSharedFocus=false;}}')
front=replace(front,'private void abandonFocus(){if(Build.VERSION.SDK_INT>=26)audio.abandonAudioFocusRequest(focus);else audio.abandonAudioFocus(null);focused=false;}','private void abandonFocus(){focusOwner.setFrontDemand(false);focused=false;}')
front=replace(front,'private void releaseEffect(MediaPlayer p){if(effects.remove(p))p.release();}','private void releaseEffect(MediaPlayer p){if(effects.remove(p))p.release();if(effects.isEmpty()&&!requested)abandonFocus();}')
for item in ('import android.media.AudioFocusRequest;\n','import android.media.AudioManager;\n','import android.os.Build;\n'):front=replace(front,item,'')
main=originals['MainActivity.java']
main=replace(main,'    private FrontAudio frontAudio;','    private FrontAudio frontAudio;\n    private SharedAudioFocusV40 sharedAudio;')
main=replace(main,'        frontAudio=new FrontAudio(this);','        sharedAudio=new SharedAudioFocusV40(this,NativeBridge::audioActivityV40);\n        frontAudio=new FrontAudio(this,sharedAudio);')
main=replace(main,'            private boolean initialLoadPending;','            private boolean initialLoadPending;\n            private boolean lastNativeAudioReady;')
main=replace(main,'                String audio;','                final boolean nativeAudioReady=NativeBridge.audioSourceReadyV40();\n                if(nativeAudioReady!=lastNativeAudioReady){lastNativeAudioReady=nativeAudioReady;runOnUiThread(()->sharedAudio.setNativeDemand(nativeAudioReady));}\n                String audio;')
main=replace(main,'@Override protected void onPause(){','@Override protected void onPause(){sharedAudio.setResumed(false);')
main=replace(main,'@Override protected void onResume(){super.onResume();frontAudio.resume();','@Override protected void onResume(){super.onResume();sharedAudio.setResumed(true);frontAudio.resume();')
main=replace(main,'@Override protected void onDestroy(){','@Override public void onWindowFocusChanged(boolean focused){super.onWindowFocusChanged(focused);if(sharedAudio!=null)sharedAudio.setWindowFocused(focused);}\n    @Override protected void onDestroy(){sharedAudio.close();')
bridge=originals['NativeBridge.java']
bridge=replace(bridge,'    static native String initialize(android.content.res.AssetManager assets);','    static native String initialize(android.content.res.AssetManager assets);\n    static native void audioActivityV40(long owner,long sequence,boolean resumed,boolean windowFocused,boolean granted,boolean destroyed);\n    static native boolean audioSourceReadyV40();')
modified={'MainActivity.java':main,'FrontAudio.java':front,'NativeBridge.java':bridge}
patch=''
for name,text in modified.items():
    raw_lines=original_bytes[name].decode().splitlines(True)
    before=originals[name].splitlines(True);after=text.splitlines(True);result=[]
    matcher=difflib.SequenceMatcher(a=before,b=after,autojunk=False)
    for operation,a,b,c,d in matcher.get_opcodes():
        if operation=='equal':result.extend(raw_lines[a:b])
        elif operation in ('replace','insert'):
            ending='\r\n' if raw_lines[min(a,len(raw_lines)-1)].endswith('\r\n') else '\n'
            result.extend(line.rstrip('\r\n')+ending for line in after[c:d])
    rendered=''.join(result)
    (staged/name).write_bytes(rendered.encode())
    path='port/android-native/app/src/main/java/com/example/dh2/'+name
    patch+=''.join(difflib.unified_diff(raw_lines,rendered.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
(OUT/'application-java.patch').write_bytes(patch.encode())
for name in ('AudioFocusPolicyV40.java','SharedAudioFocusV40.java'):(staged/name).write_bytes((OUT/name).read_bytes())
sources=list(JAVA.glob('*.java'))+[ROOT/'port/android-native/app/src/main/cpp/native_app.cpp',ROOT/'port/android-native/app/src/main/cpp/audio_output_v34.cpp',ROOT/'port/android-native/app/src/main/cpp/audio_output_v34.hpp']
manifest={str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}
(OUT/'captured-source-sha256.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('Prepared exact staged Java patch; shared sources unchanged')
