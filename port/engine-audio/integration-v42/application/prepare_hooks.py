from pathlib import Path
import hashlib,json,difflib,subprocess
ROOT=Path(__file__).resolve().parents[4];OUT=Path(__file__).resolve().parent
subprocess.run([r'C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe',str(OUT/'prepare_java.py')],check=True)
def replace(text,old,new):
    assert text.count(old)==1,(old,text.count(old));return text.replace(old,new)
def patch_file(relative,transform):
    raw=(ROOT/relative).read_bytes();original=raw.decode();normal=original.replace('\r\n','\n');text=transform(normal)
    before=normal.splitlines(True);after=text.splitlines(True);rawlines=original.splitlines(True);result=[]
    for op,a,b,c,d in difflib.SequenceMatcher(a=before,b=after,autojunk=False).get_opcodes():
        if op=='equal':result+=rawlines[a:b]
        elif op in ('replace','insert'):
            ending='\r\n'if rawlines[min(a,len(rawlines)-1)].endswith('\r\n')else'\n';result+=[line.rstrip('\r\n')+ending for line in after[c:d]]
    candidate=''.join(result);destination=OUT/'staged'/relative;destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(candidate.encode())
    return ''.join(difflib.unified_diff(rawlines,candidate.splitlines(True),fromfile='a/'+relative,tofile='b/'+relative)),hashlib.sha256(raw).hexdigest()
native='port/android-native/app/src/main/cpp/native_app.cpp';renderer='port/android-native/app/src/main/cpp/model_renderer.hpp';cmake='port/android-native/app/src/main/cpp/CMakeLists.txt'
def native_patch(text):
    text=replace(text,'#include "model_renderer.hpp"','#include "model_renderer.hpp"\n#include "'+str((OUT/'native_audio_application_v42.hpp').relative_to(ROOT)).replace('\\','/')+'"')
    # Include is repository-relative; target include adds DH2_SOURCE_DIR.
    text=replace(text,'bool model_renderer::borrow_actual_application_services_v5(','''bool model_renderer::borrow_actual_application_audio_v42(dh2::audio::AudioApplicationBorrowV42& out,std::string& error){
 return dh2::android_audio::borrow_application_audio_v42(out,error);
}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_NativeBridge_audioApplicationReserveV42(JNIEnv*,jclass){
 return static_cast<jlong>(dh2::android_audio::reserve_application_audio_v42());
}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_NativeBridge_audioApplicationOwnerV42(JNIEnv*,jclass){
 return static_cast<jlong>(dh2::android_audio::application_audio_owner_v42());
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_audioApplicationCloseV42(JNIEnv*,jclass,jlong owner){
 if(owner>0)dh2::android_audio::request_application_audio_close_v42(static_cast<std::uint64_t>(owner));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_audioApplicationShutdownV42(JNIEnv* env,jclass,jlong owner){
 std::string error;return dh2::android_audio::shutdown_application_audio_v42(owner>0?static_cast<std::uint64_t>(owner):0,error)?nullptr:result(env,error);
}
bool model_renderer::borrow_actual_application_services_v5(''')
    text=replace(text,'      gameplay_assets=manager;','''      gameplay_assets=manager;
      // Original GSInit phase0 CreateInstance audio seam; exact pack bootstrap.
      // Full GSInit phase9 Vox settings/general/World initialization is separate.
      std::string audio_error;
      if(!dh2::android_audio::ensure_application_audio_v42(env,assets,audio_error))throw std::runtime_error(audio_error);''')
    return text
def renderer_patch(text):
    text=replace(text,'#include "application_services_owner_v5.hpp"','#include "application_services_owner_v5.hpp"\n#include "audio_application_manager_v42.hpp"')
    return replace(text,'namespace model_renderer {','namespace model_renderer {\n// True captures actual nullable global; false reports unknown/failed construction.\nbool borrow_actual_application_audio_v42(dh2::audio::AudioApplicationBorrowV42&,std::string&);')
def cmake_patch(text):
    addition='''
# Persistent Application audio: V42 only, never a second V40 session owner.
target_sources(dh2_native PRIVATE
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_clock_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_source_command_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/audio_gameplay_runtime_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/audio_native_session_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/audio_application_manager_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42/application/native_audio_application_v42.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_lifecycle_jni_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_output_v40.cpp"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v40/focus/audio_control_v40.cpp")
target_include_directories(dh2_native PRIVATE "${DH2_SOURCE_DIR}"
  "${DH2_SOURCE_DIR}/port/engine-audio/integration-v42")
'''
    return replace(text,'target_compile_features(dh2_native PRIVATE cxx_std_17)',addition+'\ntarget_compile_features(dh2_native PRIVATE cxx_std_17)')
patch='';hashes={}
for relative,transform in [(native,native_patch),(renderer,renderer_patch),(cmake,cmake_patch)]:
    diff,digest=patch_file(relative,transform);patch+=diff;hashes[relative]=digest
(OUT/'native-application.patch').write_bytes(patch.encode())
java=OUT/'staged/java/com/example/dh2';main=(java/'MainActivity.java').read_text();main=replace(main,'    private SharedAudioFocusV40 sharedAudio;','    private SharedAudioFocusV40 sharedAudio;\n    private volatile long audioApplicationOwnerV42;');main=replace(main,'                Log.i("DH2Native",NativeBridge.initialize(getAssets()));','                audioApplicationOwnerV42=NativeBridge.audioApplicationReserveV42();\n                Log.i("DH2Native",NativeBridge.initialize(getAssets()));');main=replace(main,'@Override protected void onPause(){sharedAudio.setResumed(false);','@Override protected void onPause(){sharedAudio.setResumed(false);if(isFinishing()||isChangingConfigurations())AudioApplicationLifecycleV42.closeBeforeProducerPause(surface,audioApplicationOwnerV42);');main=replace(main,'@Override protected void onDestroy(){sharedAudio.close();','@Override protected void onDestroy(){sharedAudio.close();AudioApplicationLifecycleV42.destroyRequestOnly(audioApplicationOwnerV42);');(java/'MainActivity.java').write_text(main)
bridge=(java/'NativeBridge.java').read_text();bridge=replace(bridge,'    static native boolean audioSourceReadyV40();','    static native boolean audioSourceReadyV40();\n    static native long audioApplicationReserveV42();\n    static native long audioApplicationOwnerV42();\n    static native void audioApplicationCloseV42(long expectedOwner);\n    static native String audioApplicationShutdownV42(long expectedOwner);');(java/'NativeBridge.java').write_text(bridge)
(java/'AudioApplicationLifecycleV42.java').write_bytes((OUT/'AudioApplicationLifecycleV42.java').read_bytes())
# Diff current root Java against the final V42 results, preserving old context
# line endings. New changed lines use their anchor's original ending.
java_patch=''
for name in ('MainActivity.java','FrontAudio.java','NativeBridge.java'):
    relative='port/android-native/app/src/main/java/com/example/dh2/'+name
    final=(java/name).read_text();diff,digest=patch_file(relative,lambda _text,final=final:final);java_patch+=diff;hashes[relative]=digest
(OUT/'application-java-v42.patch').write_bytes(java_patch.encode())
head=subprocess.run(['git','rev-parse','HEAD'],cwd=ROOT,capture_output=True,text=True).stdout.strip()
(OUT/'captured-root.json').write_text(json.dumps(dict(requested_base='655f986',observed_head=head,requested_commit_available=False,source_sha256=hashes,scope='Exact working file snapshots; requested commit absent in this checkout. Root must verify/rebase these hashes in its integration checkout.'),indent=2)+'\n')
