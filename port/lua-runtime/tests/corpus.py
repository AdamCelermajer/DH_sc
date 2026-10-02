#!/usr/bin/env python3
"""Run the source-built Lua selftest and parse all exact scripts plus override."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def main():
    p=argparse.ArgumentParser()
    p.add_argument('--runner',type=Path,required=True);p.add_argument('--report',type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be supplied together')
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    manifest_path=REPO/'recovered/scripts/manifest.json';manifest=json.loads(manifest_path.read_text())
    rows=manifest['files']+manifest['overrides'];assert len(rows)==220
    staging=ROOT/'build/corpus';staging.mkdir(parents=True,exist_ok=True)
    def run(*args):
        result=subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True)
        return result.stdout.strip()
    base='/data/local/tmp/dh2-lua-source-qa'
    selected=[];remote_hashes={};source_paths=[]
    for i,row in enumerate(rows):
        source=REPO/row['path'];assert sha(source)==row['sha256']
        target=staging/f'{i:03}.luac';target.write_bytes(source.read_bytes())
        selected.append({'path':row['path'],'sha256':row['sha256'],'bytes':target.stat().st_size})
        source_paths.append(f'{base}/{target.name}'if a.adb else str(target.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(source_paths)+'\n',encoding='utf-8',newline='\n')
    device=None
    if a.adb:
        adb=lambda *args:run(a.adb,'-s',a.serial,*args)
        assert adb('shell','getprop','ro.kernel.qemu')=='1','This test is for emulators only'
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),
                'sdk':int(adb('shell','getprop','ro.build.version.sdk')),
                'page_size':int(adb('shell','getconf','PAGE_SIZE')),
                'abi':adb('shell','getprop','ro.product.cpu.abi'),
                'fingerprint':adb('shell','getprop','ro.build.fingerprint')}
        assert device['sdk']==37 and device['android_release']=='17'and device['abi']=='x86_64'
        assert device['page_size']in (4096,16384)
        adb('shell','mkdir','-p',base)
        adb('push',str(staging)+'/.' ,base+'/')
        adb('push',a.runner,base+'/runner')
        adb('shell','chmod','755',base+'/runner')
        paths=[*source_paths,base+'/list.txt',base+'/runner']
        for offset in range(0,len(paths),40):
            output=adb('shell','sha256sum',*paths[offset:offset+40])
            for line in output.splitlines():
                digest,path=line.split(None,1);remote_hashes[path.strip()]=digest
        for path,row in zip(source_paths,selected):assert remote_hashes[path]==row['sha256']
        assert remote_hashes[base+'/list.txt']==sha(listing)
        assert remote_hashes[base+'/runner']==sha(a.runner)
        output=adb('shell',base+'/runner',base+'/list.txt')
    else:output=run(a.runner.resolve(),listing.resolve())
    assert output.splitlines()[0]=='SELFTEST PASS'
    numeric_path=ROOT/'original-numeric-validation.json';numeric=json.loads(numeric_path.read_text())
    assert output.splitlines()[1]==f"NUMERIC PASS {numeric['numeric_arith_vectors']}"
    result_rows=[]
    for index,status in re.findall(r'^SOURCE (\d+) (\d+)$',output,re.M):
        index,status=int(index),int(status);assert index==len(result_rows)
        row=selected[index];expected=3 if index<219 and row['path'].endswith('/ai/sandworm_small_core.luac')else 0
        assert status==expected,(index,row,status,expected)
        result_rows.append({**row,'lua_load_status':status,'expected_status':expected})
    assert len(result_rows)==220 and output.splitlines()[-1]=='FILES 220'
    result={'complete_game':False,'game_scripts_executed':False,'game_callbacks_installed':False,
            'source_built_runtime':'official Lua 5.1.4 plus modern owned wrapper',
            'number_profile':'float32 / int32 via LUA_USER_H',
            'original_numeric_vectors_matched':numeric['numeric_arith_vectors'],
            'numeric_comparison':'exact float32 bits except signed zero; host dependency model for original imports',
            'original_numeric_report_sha256':sha(numeric_path),
            'runtime_selftest':{'standard_libraries':True,'missing_game_callbacks_explicit':True,
                'instruction_budget':True,'memory_budget':True,'recovery_after_runtime_errors':True,
                'compiler_does_not_execute':True,'bytecode_rejection':True,'invalid_arguments':True},
            'files':220,'original_syntax_pass':218,'original_syntax_fail':1,'override_syntax_pass':1,
            'script_manifest_sha256':sha(manifest_path),'vendor_manifest_sha256':sha(ROOT/'vendor-manifest.json'),
            'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),
            'source_sha256':{name:sha(ROOT/name)for name in ('runtime.c','runtime.h','dh2_lua_config.h',
                            'tests/runner.c','tests/numeric.c','tests/numeric-vectors.h')},
            'per_file':result_rows,'selftest_and_corpus_stdout':output}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(remote_hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('per_file','selftest_and_corpus_stdout')}))
if __name__=='__main__':main()
