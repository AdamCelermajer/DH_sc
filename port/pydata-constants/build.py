#!/usr/bin/env python3
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
ROOT=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('layout',ROOT/'../animation-ending/build.py')
layout=importlib.util.module_from_spec(spec);spec.loader.exec_module(layout)
def main():
    p=argparse.ArgumentParser();p.add_argument('--host',action='store_true');p.add_argument('--ndk',type=Path)
    p.add_argument('--report',type=Path,required=True);a=p.parse_args()
    build=ROOT/'build';build.mkdir(exist_ok=True);sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    flags=['-std=c99','-O2','-fPIC','-shared','-Wall','-Wextra','-Werror']
    artifacts={}
    if a.host:
        path=build/'constants-host.so'
        subprocess.run([os.environ.get('CC','cc'),*flags,str(ROOT/'constants.c'),'-o',str(path)],check=True)
        artifacts[path.name]={'sha256':sha(path)}
        safety=build/'constants-safety'
        subprocess.run([os.environ.get('CC','cc'),'-std=c99','-O1','-g',
                        '-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-omit-frame-pointer',
                        str(ROOT/'constants.c'),str(ROOT/'tests/safety.c'),'-o',str(safety)],check=True)
        subprocess.run([str(safety.resolve())],check=True)
    if a.ndk:
        cc=a.ndk/'toolchains/llvm/prebuilt/windows-x86_64/bin/clang.exe'
        path=build/'constants-arm64.so'
        subprocess.run([str(cc),'--target=aarch64-linux-android26',*flags,str(ROOT/'constants.c'),
                        '-Wl,-z,max-page-size=16384','-Wl,--no-undefined','-o',str(path)],check=True)
        artifacts[path.name]={'sha256':sha(path),'layout':layout.check_arm64(path)}
    if not artifacts:p.error('select --host and/or --ndk')
    result={'complete_game':False,'artifacts':artifacts,'source_sha256':{name:sha(ROOT/name)for name in ('constants.c','constants.h')},
            'build_tool_sha256':sha(Path(__file__))}
    if a.host:result['safety']={'iterations':12000,'address_sanitizer':True,'undefined_behavior_sanitizer':True,
                              'recover':False,'test_sha256':sha(ROOT/'tests/safety.c'),'seed':20261002}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
