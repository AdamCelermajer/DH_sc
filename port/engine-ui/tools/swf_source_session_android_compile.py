"""Compile the additive source-session batch for both Android ABIs; no link/APK."""
import argparse, hashlib, json, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
UI=ROOT/'port/engine-ui'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args(); output=a.output.resolve()
    assert output.is_relative_to(ROOT) and not a.report.exists()
    output.mkdir(parents=True,exist_ok=True)
    sources=[UI/(s+'.cpp') for s in ['swf_source_movie_v1','swf_input_session_v2']]
    sources += [UI/'overlays/loader-lifetime-v1/gameswf_impl.cpp',ROOT/'port/android-native/app/src/main/cpp/original_ui_input_session_v1.cpp']
    headers=[UI/(s+'.hpp') for s in ['swf_source_movie_v1','swf_input_session_v2']]+[ROOT/'port/android-native/app/src/main/cpp/original_ui_input_session_v1.hpp',UI/'gameswf_loader_lifetime_overlay_v1.cmake']
    before={str(x.relative_to(ROOT)):sha(x) for x in sources+headers}; results={}
    for abi,target in [('arm64-v8a','aarch64-linux-android26'),('x86_64','x86_64-linux-android26')]:
        results[abi]={}
        for source in sources:
            object_file=output/(source.stem+'-'+abi+'.o'); dependency=Path(str(object_file)+'.d')
            args=['--target='+target,'-std=c++17','-O2','-w','-fno-fast-math','-ffp-contract=off','-fPIC']
            args += ['-DTU_CONFIG_LINK_TO_'+s+'=0' for s in ['JPEGLIB','LIBPNG','FREETYPE','THREAD']]
            args += ['-I'+str(UI),'-I'+str(UI/'vendor/gameswf1714'),'-I'+str(ROOT/'port/android-native/app/src/main/cpp'),'-MD','-MF',str(dependency),'-c',str(source),'-o',str(object_file)]
            subprocess.run([str(a.compiler)]+args,check=True)
            results[abi][str(source.relative_to(ROOT))]={'arguments':args,'object_sha256':sha(object_file),'dependency_file_sha256':sha(dependency)}
    assert all(sha(ROOT/x)==h for x,h in before.items()),'Source changed during compilation'
    a.report.parent.mkdir(parents=True,exist_ok=True)
    a.report.write_text(json.dumps(dict(validation='PASS',compile_only=True,Android_link_or_GPU_test=False,compiler_sha256=sha(a.compiler),compiler_version=subprocess.check_output([str(a.compiler),'--version'],text=True).strip(),source_sha256=before,abis=results,tool_sha256=sha(Path(__file__))),indent=2)+'\n')
    print(json.dumps({'validation':'PASS','objects':8,'Android_link_or_GPU_test':False}))
if __name__=='__main__':main()
