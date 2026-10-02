#!/usr/bin/env python3
"""Import exact owner constant files and query all checked values through Lua."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
def main():
    p=argparse.ArgumentParser()
    for name in ('cache','runner','report'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be supplied together')
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    trace_path=REPO/'reports/pydata-constant-reader-trace.json';trace=json.loads(trace_path.read_text())
    staging=ROOT/'build/constants-corpus';staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-constants-qa';paths=[];inputs=[];expected={}
    for i,row in enumerate(trace['files']):
        path=a.cache/row['path'];assert sha(path)==row['sha256']
        target=staging/f'{i:03}.bin';target.write_bytes(path.read_bytes())
        paths.append(f'{base}/{target.name}'if a.adb else str(target.resolve()))
        inputs.append({'path':row['path'],'sha256':sha(path),'bytes':path.stat().st_size,
                       'expected_status':0 if row['fully_consumed']else -1})
        if row['fully_consumed']:
            for entry in row['rows']:expected[(entry['group'],entry['name'])]=entry['value']
    assert len(expected)==5608
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='utf-8',newline='\n')
    assertions=staging/'assertions.lua'
    statements=[f'assert(GetPyCst({json.dumps(group)},{json.dumps(key)})=={value});'for(group,key),value in sorted(expected.items())]
    statements+=['assert(GetPyCst("__missing__","__missing__")==0);',
                 'assert(GetPyCst("SoundBus","Amb_loop_omni")==0);']
    assertions.write_text('\n'.join(statements)+'\n',encoding='ascii',newline='\n')
    def run(*args):return subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True).stdout.strip()
    device=None;hashes={}
    if a.adb:
        adb=lambda *args:run(a.adb,'-s',a.serial,*args)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),
                'sdk':int(adb('shell','getprop','ro.build.version.sdk')),
                'page_size':int(adb('shell','getconf','PAGE_SIZE')),
                'abi':adb('shell','getprop','ro.product.cpu.abi'),
                'fingerprint':adb('shell','getprop','ro.build.fingerprint')}
        assert device['android_release']=='17'and device['sdk']==37 and device['abi']=='x86_64'
        assert device['page_size']in (4096,16384)
        adb('shell','mkdir','-p',base);adb('push',str(staging)+'/.' ,base+'/')
        adb('push',a.runner,base+'/runner');adb('shell','chmod','755',base+'/runner')
        output=adb('shell','sha256sum',*paths,base+'/list.txt',base+'/assertions.lua',base+'/runner')
        for line in output.splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for path,row in zip(paths,inputs):assert hashes[path]==row['sha256']
        for path,target in ((base+'/list.txt',listing),(base+'/assertions.lua',assertions),(base+'/runner',a.runner)):
            assert hashes[path]==sha(target)
        output=adb('shell',base+'/runner','--constants',base+'/list.txt',base+'/assertions.lua')
    else:output=run(a.runner.resolve(),'--constants',listing.resolve(),assertions.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504']
    statuses=re.findall(r'^CONSTANT (\d+) (-?\d+)$',output,re.M);assert len(statuses)==27
    for index,(number,status)in enumerate(statuses):assert int(number)==index and int(status)==inputs[index]['expected_status']
    assert output.splitlines()[-1]=='CONSTANT LOOKUPS PASS 5608'
    result={'complete_game':False,'game_scripts_executed':False,'authored_getpycst_queries':5608,
            'full_constant_files_imported':26,'mixed_sound_file_rejected':True,
            'ordered_names_bridge_installed':True,
            'caller_buffers_released_before_queries':True,'malformed_import_rollback':True,
            'memory_failure_rollback':True,'native_bridge_selftest':True,'missing_game_objects_explicit':True,
            'inputs':inputs,'reader_trace_sha256':sha(trace_path),'runner_sha256':sha(a.runner),
            'test_sha256':sha(Path(__file__)),'assertions_sha256':sha(assertions),
            'lookup_expectations':'Original reader insertion values converted by the float32 source Lua profile; original game scripts and original map lookup are not executed',
            'source_sha256':{name:sha(ROOT/name)for name in ('runtime.c','runtime.h','tests/runner.c',
                            'tests/constants.c','../pydata-constants/constants.c',
                            '../pydata-constants/constants.h','../pydata-constants/lua-bridge.c',
                            '../pydata-names/names.c','../pydata-names/names.h','../pydata-names/lua-bridge.c')},
            'stdout':output}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('inputs','stdout')}))
if __name__=='__main__':main()
