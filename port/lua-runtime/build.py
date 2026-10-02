#!/usr/bin/env python3
"""Build preserved upstream Lua with explicit patches and an owned runtime."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
ROOT=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('elf_layout',ROOT/'../animation-ending/build.py')
layout=importlib.util.module_from_spec(spec);spec.loader.exec_module(layout)
CORE='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio'
LIBRARIES='lauxlib lbaselib lmathlib lstrlib ltablib'
def main():
    p=argparse.ArgumentParser();p.add_argument('--ndk',type=Path);p.add_argument('--host',action='store_true')
    p.add_argument('--sanitize',action='store_true');p.add_argument('--report',type=Path,required=True);a=p.parse_args()
    if not a.host and not a.ndk:p.error('select --host and/or --ndk')
    if a.sanitize and not a.host:p.error('--sanitize requires --host')
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    manifest_path=ROOT/'vendor-manifest.json';manifest=json.loads(manifest_path.read_text())
    for row in manifest['files']:
        path=ROOT/row['path'];assert path.stat().st_size==row['bytes']and sha(path)==row['sha256']
    vendor=ROOT/'vendor/lua-5.1.4/src'
    sources=[vendor/(name+'.c')for name in (CORE+' '+LIBRARIES).split()]+[
        ROOT/'runtime.c',ROOT/'../lua-numeric/numeric.c',ROOT/'../lua-numeric/bridge.c']
    build=ROOT/'build';build.mkdir(exist_ok=True)
    # Preserve the upstream import; apply the reviewed repair only to a build copy.
    patch_path=ROOT/'patches/ltable-array-index.json'
    patch=json.loads(patch_path.read_text())
    original=ROOT/patch['source'];original_text=original.read_text()
    for replacement in patch['replacements']:
        assert original_text.count(replacement['before'])==1
        original_text=original_text.replace(replacement['before'],replacement['after'])
    patched=build/'ltable.c'
    patched.write_text(original_text)
    sources=[patched if path==original else path for path in sources]
    flags=['-std=c99','-O2','-fno-fast-math','-ffp-contract=off','-I',str(vendor),
           '-I',str(ROOT),'-DLUA_USER_H="dh2_lua_config.h"','-Wall']
    variants=[]
    if a.host:variants.append(('host',os.environ.get('CC','cc'),[]))
    if a.ndk:
        platform='windows-x86_64'if os.name=='nt'else'linux-x86_64'
        clang=a.ndk/f'toolchains/llvm/prebuilt/{platform}/bin/clang'
        if os.name=='nt':clang=clang.with_suffix('.exe')
        variants.extend((name,str(clang),['--target='+target,'-Wl,-z,max-page-size=16384'])for name,target in
            [('arm64','aarch64-linux-android26'),('x86_64','x86_64-linux-android26')])
    artifacts={};warnings={}
    for name,cc,extra in variants:
        for kind in ('shared','runner'):
            output=build/(f'lua-{name}.so'if kind=='shared'else f'lua-{name}-runner')
            args=[cc,*extra,*flags,*map(str,sources)]
            if kind=='shared':args+=['-fPIC','-shared','-Wl,--no-undefined']
            else:args+=['-fPIE','-pie',str(ROOT/'tests/runner.c'),str(ROOT/'tests/numeric.c')]
            args+=['-lm','-o',str(output)]
            result=subprocess.run(args,capture_output=True,text=True)
            if result.returncode:raise RuntimeError(result.stderr)
            warnings[output.name]=result.stderr
            row={'sha256':sha(output),'bytes':output.stat().st_size}
            if name=='arm64':row['layout']=layout.check_arm64(output)
            artifacts[output.name]=row
        if name=='host':
            subprocess.run([str((build/'lua-host-runner').resolve())],check=True)
    safety=None
    if a.sanitize:
        output=build/'lua-host-safety'
        subprocess.run([os.environ.get('CC','cc'),'-std=c99','-O1','-g','-I',str(vendor),'-I',str(ROOT),
                        '-DLUA_USER_H="dh2_lua_config.h"',
                        '-fsanitize=address,undefined,float-cast-overflow','-fno-sanitize-recover=all',
                        '-fno-omit-frame-pointer',*map(str,sources),
                        str(ROOT/'tests/runner.c'),str(ROOT/'tests/numeric.c'),'-lm','-o',str(output)],check=True)
        subprocess.run([str(output.resolve())],check=True)
        safety={'address_sanitizer':True,'undefined_behavior_sanitizer':True,
                'float_cast_overflow_sanitizer':True,'recover':False,
                'test':'runtime selftest and 504 numeric arithmetic vectors'}
    result={'complete_game':False,'numeric_callbacks_installed':True,'gameplay_object_callbacks_installed':False,
            'original_lua_equivalence_tested':False,
            'vendor_manifest_sha256':sha(manifest_path),'number_profile':'float32 / int32 via LUA_USER_H',
            'upstream_patches':[{'path':str(patch_path.relative_to(ROOT)),
                'sha256':sha(patch_path),'original_sha256':sha(original),'compiled_sha256':sha(patched)}],
            'artifacts':artifacts,'compiler_warnings':warnings,
            'source_sha256':{os.path.relpath(path,ROOT).replace('\\','/'):sha(path)for path in
                [*sources,ROOT/'runtime.h',ROOT/'dh2_lua_config.h',ROOT/'tests/runner.c',ROOT/'tests/numeric.c',
                 ROOT/'tests/numeric-vectors.h',ROOT/'../lua-numeric/numeric.h']},'build_tool_sha256':sha(Path(__file__))}
    if safety:result['safety']=safety
    a.report.write_text(json.dumps(result,indent=2)+'\n');print('Built:',', '.join(artifacts))
if __name__=='__main__':main()
