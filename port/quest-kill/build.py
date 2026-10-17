#!/usr/bin/env python3
import argparse,hashlib,importlib.util,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('layout',ROOT/'../animation-ending/build.py')
layout=importlib.util.module_from_spec(spec);spec.loader.exec_module(layout)
def main():
    p=argparse.ArgumentParser();p.add_argument('--host',action='store_true');p.add_argument('--ndk',type=Path)
    p.add_argument('--report',type=Path,required=True);a=p.parse_args();b=ROOT/'build';b.mkdir(exist_ok=True)
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    sources=[ROOT/'quest.c']
    flags=['-std=c99','-O2','-fPIC','-shared','-Wall','-Wextra','-Werror'];artifacts={}
    if a.host:
        cc=os.environ.get('CC','cc');path=b/'quest-host.so'
        subprocess.run([cc,*flags,*map(str,sources),'-o',str(path)],check=True);artifacts[path.name]={'sha256':sha(path)}
        safety=b/'quest-safety'
        subprocess.run([cc,'-std=c99','-O1','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',
                        '-fno-sanitize-recover=all','-fno-omit-frame-pointer',*map(str,sources),str(ROOT/'tests/safety.c'),'-o',str(safety)],check=True)
        subprocess.run([str(safety.resolve())],check=True)
    if a.ndk:
        cc=a.ndk/'toolchains/llvm/prebuilt/windows-x86_64/bin/clang.exe';path=b/'quest-arm64.so'
        subprocess.run([str(cc),'--target=aarch64-linux-android26',*flags,*map(str,sources),
                        '-Wl,-z,max-page-size=16384','-Wl,--no-undefined','-o',str(path)],check=True)
        artifacts[path.name]={'sha256':sha(path),'layout':layout.check_arm64(path)}
    if not artifacts:p.error('select --host and/or --ndk')
    headers=[ROOT/'quest.h']
    result={'complete_game':False,'artifacts':artifacts,'source_sha256':{os.path.relpath(n,ROOT).replace('\\','/'):sha(n)for n in [*sources,*headers]},'build_tool_sha256':sha(Path(__file__))}
    if a.host:result['safety']={'iterations':12000,'asan':True,'ubsan':True,'recover':False,'test_sha256':sha(ROOT/'tests/safety.c'),'seed':20261002}
    a.report.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');print(json.dumps(result))
if __name__=='__main__':main()
