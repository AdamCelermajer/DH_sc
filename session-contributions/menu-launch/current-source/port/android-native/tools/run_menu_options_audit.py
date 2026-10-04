"""Compile/run the bounded option reader audit on emulator-5580 only.

Does not install an APK or change the visible application. String and Sharp
callbacks are test fixtures; settings load deliberately rejects a missing scene.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--snapshot',type=Path,default=Path(__file__).resolve().parents[3])
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
    ndk=sdk/'ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64'
    core=a.snapshot/'port/engine-ui'
    android=a.snapshot/'port/android-native'
    lib=android/'app/build/intermediates/cxx/Debug/432l4r6h/obj/x86_64/libdh2_engine_ui.so'
    exe=a.output/'menu-options-audit'
    source=android/'tools/menu_options_audit.cpp'
    subprocess.run([str(ndk/'bin/x86_64-linux-android26-clang++.cmd'),'-std=c++17','-w',
        '-DTU_CONFIG_LINK_TO_THREAD=0','-DTU_CONFIG_LINK_TO_FREETYPE=0',
        '-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_JPEGLIB=0',
        '-fno-sanitize=vptr','-I'+str(core),'-I'+str(core/'vendor/gameswf1714'),
        str(source),'-L'+str(lib.parent),'-ldh2_engine_ui','-o',str(exe)],check=True)
    adb=[str(sdk/'platform-tools/adb.exe'),'-P','5038','-s','emulator-5580']
    remote='/data/local/tmp/dh2-options-audit-v29'
    def call(*args):return subprocess.check_output(adb+list(args),text=True,timeout=30)
    call('shell','mkdir','-p',remote)
    inputs=[android/'app/src/main/assets/data'/('design_'+k+'.bin') for k in ('pyarray','pyarraynames','pystructnames')]
    runtime=ndk/'sysroot/usr/lib/x86_64-linux-android/libc++_shared.so'
    call('push',str(exe),str(lib),str(runtime),*(str(p) for p in inputs),remote+'/')
    call('shell','chmod','700',remote+'/menu-options-audit')
    out=call('shell','LD_LIBRARY_PATH='+remote+' '+remote+'/menu-options-audit '+remote)
    assert out.startswith('PASS |'),out
    (a.output/'audit.log').write_text(out,encoding='utf-8')
    def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
    report=dict(validation='PASS',scope=__doc__,native_library_sha256=sha(lib),
        executable_sha256=sha(exe),input_sha256={p.name:sha(p) for p in inputs},
        source_sha256={str(p.relative_to(a.snapshot)):sha(p) for p in
            [source,core/'swf_menu_options.hpp',core/'swf_menu_options.cpp',
             core/'owned_hud_settings_v1.hpp',core/'owned_hud_settings_v1.cpp',
             core/'swf_menu_navigation.hpp',core/'swf_menu_navigation.cpp']},
        options_screen_connected=False,full_settings_backend=False,output=out.strip())
    (a.output/'option-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(out.strip())

if __name__=='__main__':main()
