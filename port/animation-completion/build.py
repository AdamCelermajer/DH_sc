#!/usr/bin/env python3
import argparse, hashlib, json, os, struct, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parent

def check_arm64(path):
    data=path.read_bytes()
    assert data[:6]==b'\x7fELF\x02\x01' and struct.unpack_from('<HH',data,16)==(3,183)
    phoff=struct.unpack_from('<Q',data,32)[0]
    phsize,phcount=struct.unpack_from('<HH',data,54)
    segments=[]
    for i in range(phcount):
        kind,_,offset,vaddr,_,_,_,align=struct.unpack_from('<IIQQQQQQ',data,phoff+i*phsize)
        if kind==1:
            assert align>=16384 and offset%16384==vaddr%16384
            segments.append({'offset':offset,'alignment':align})
    assert segments
    return {'machine':183,'pt_load':segments}
def main():
    p=argparse.ArgumentParser();p.add_argument('--ndk',type=Path);p.add_argument('--arm64-only',action='store_true')
    p.add_argument('--report',type=Path,default=ROOT/'build-validation.json');a=p.parse_args()
    folder=ROOT/'build';folder.mkdir(exist_ok=True)
    flags=['-std=c++17','-O2','-fPIC','-shared','-fno-fast-math','-ffp-contract=off',
           '-fno-builtin','-fno-rtti','-fno-exceptions','-Wall','-Wextra','-Werror']
    binaries=[]
    if not a.arm64_only:
        binary=folder/'completion-host.so'
        subprocess.run([os.environ.get('CXX','c++'),*flags,str(ROOT/'completion.cpp'),'-lm','-o',str(binary)],check=True)
        binaries.append(binary)
        safety=folder/'completion-safety'
        subprocess.run([os.environ.get('CXX','c++'),'-std=c++17','-O1','-g','-fno-fast-math',
                        '-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',
                        str(ROOT/'completion.cpp'),str(ROOT/'tests/safety.cpp'),'-lm','-o',str(safety)],check=True)
        subprocess.run([str(safety.resolve())],check=True)
    if a.ndk:
        platform='windows-x86_64'if os.name=='nt'else'linux-x86_64'
        cc=a.ndk/f'toolchains/llvm/prebuilt/{platform}/bin/clang++'
        if os.name=='nt':cc=cc.with_suffix('.exe')
        binary=folder/'completion-arm64.so'
        subprocess.run([str(cc),'--target=aarch64-linux-android26',*flags,str(ROOT/'completion.cpp'),
                        '-nostdlib++','-Wl,-z,max-page-size=16384','-Wl,--no-undefined','-lm','-o',str(binary)],check=True)
        binaries.append(binary)
        arm64_layout=check_arm64(binary)
    if not binaries:p.error('--arm64-only requires --ndk')
    sha=lambda x:hashlib.sha256(x.read_bytes()).hexdigest()
    r={'complete_game':False,'android_integrated':False,
       'source_sha256':{x.name:sha(x)for x in (ROOT/'completion.cpp',ROOT/'completion.hpp')},
       'artifact_sha256':{x.name:sha(x)for x in binaries}}
    if a.ndk:r['arm64_layout']=arm64_layout
    if not a.arm64_only:r['safety']={'address_sanitizer':True,'undefined_behavior_sanitizer':True,
        'sequences':5000,'checks':10000,'seed':20261002,'test_sha256':sha(ROOT/'tests/safety.cpp')}
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
if __name__=='__main__':main()
