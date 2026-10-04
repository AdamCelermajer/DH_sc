"""Verify explicit synchronous renderer nesting on emulator-5580.

Tiny generated AVM1 files and upload/draw providers are test fixtures. No APK
installation, screen switch or production menu stack is performed by this tool.
"""
import argparse
import hashlib
import json
import struct
import subprocess
from pathlib import Path


def fixture(label):
    # Original-style AVM1 bytecode executes SetVariable on the actual root.
    bits='01111'+''.join(format(n & 32767,'015b') for n in (0,9600,0,6400))
    bits+='0'*(-len(bits)%8)
    rect=int(bits,2).to_bytes(len(bits)//8,'big')
    push=b'\0Owner\0\0'+label.encode()+b'\0'
    action=b'\x96'+struct.pack('<H',len(push))+push+b'\x1d\0'
    body=rect+struct.pack('<HH',20*256,1)+struct.pack('<H',(12<<6)|len(action))+action+b'\x40\0\0\0'
    return b'FWS\x06'+struct.pack('<I',8+len(body))+body


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--snapshot',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
    ndk=sdk/'ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64'
    core=a.snapshot/'port/engine-ui'
    android=a.snapshot/'port/android-native'
    lib=android/'app/build/intermediates/cxx/Debug/432l4r6h/obj/x86_64/libdh2_engine_ui.so'
    source=android/'tools/menu_renderer_scope_audit.cpp'
    exe=a.output/'menu-renderer-scope-audit'
    subprocess.run([str(ndk/'bin/x86_64-linux-android26-clang++.cmd'),'-std=c++17','-w',
        '-DTU_CONFIG_LINK_TO_THREAD=0','-DTU_CONFIG_LINK_TO_FREETYPE=0',
        '-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_JPEGLIB=0',
        '-fno-sanitize=vptr','-I'+str(core),'-I'+str(core/'vendor/gameswf1714'),
        str(source),'-L'+str(lib.parent),'-ldh2_engine_ui','-o',str(exe)],check=True)
    fixtures=[]
    for name in ('a','b'):
        path=a.output/(name+'.swf');path.write_bytes(fixture(name.upper()));fixtures.append(path)
    adb=[str(sdk/'platform-tools/adb.exe'),'-P','5038','-s','emulator-5580']
    remote='/data/local/tmp/dh2-menu-renderer-scope-audit'
    def call(*args):return subprocess.check_output(adb+list(args),text=True,timeout=30)
    call('shell','mkdir','-p',remote)
    runtime=ndk/'sysroot/usr/lib/x86_64-linux-android/libc++_shared.so'
    call('push',str(exe),str(lib),str(runtime),*(str(x) for x in fixtures),remote+'/')
    call('shell','chmod','700',remote+'/menu-renderer-scope-audit')
    out=call('shell','LD_LIBRARY_PATH='+remote+' '+remote+'/menu-renderer-scope-audit '+remote)
    (a.output/'audit.log').write_text(out)
    assert out.startswith('PASS |'),out
    def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
    report=dict(validation='PASS',scope=__doc__,native_library_sha256=sha(lib),
        executable_sha256=sha(exe),fixture_sha256={x.name:sha(x) for x in fixtures},
        source_sha256={str(x.relative_to(a.snapshot)):sha(x) for x in
            [source,core/'swf_movie.hpp',core/'swf_movie.cpp']},
        production_menu_stack_connected=False,output=out.strip())
    (a.output/'scope-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(out.strip())


if __name__=='__main__':main()
