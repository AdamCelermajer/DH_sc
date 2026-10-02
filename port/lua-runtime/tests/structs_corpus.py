#!/usr/bin/env python3
"""Check built-in field IDs and controlled exact shared-script execution."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
TRACE_SHA='94201dae07d7dee055afef052670d65433b8f9fae53f2db42556803e2002b464'
SCRIPT_PATHS=['data/scripts/ai/_commons.luac','data/scripts/skills/_commons.luac',
              'data/scripts/level/combat_formulas.luac']

def main():
    p=argparse.ArgumentParser()
    for name in ('runner','report'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be supplied together')
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    trace_path=REPO/'reports/pydata-struct-name-trace.json';assert sha(trace_path)==TRACE_SHA
    trace=json.loads(trace_path.read_text());manifest_path=REPO/'recovered/scripts/manifest.json'
    manifest=json.loads(manifest_path.read_text());rows={r['path']:r for r in manifest['files']}
    staging=ROOT/'build/structs-corpus';staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-structs-qa';selected=[];paths=[];targets=[]
    for i,name in enumerate(SCRIPT_PATHS):
        path='recovered/scripts/original/'+name;source=REPO/path
        assert sha(source)==rows[path]['sha256'];target=staging/f'{i:03}.luac'
        target.write_bytes(source.read_bytes());targets.append(target)
        selected.append({'path':path,'bytes':target.stat().st_size,'sha256':sha(target)})
        paths.append(f'{base}/{target.name}'if a.adb else str(target.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='utf-8',newline='\n')
    statements=[]
    for table in trace['tables']:
        first={}
        for i,name in enumerate(table['names']):first.setdefault(name,i)
        for name in table['names']:
            args=json.dumps(table['registered_name'])+','+json.dumps(name)
            statements.append(f'assert(GetPyStruct({args})=={first[name]});assert(GetPyOID({args})=={first[name]});')
        statements.append('assert(GetPyStruct('+json.dumps(table['registered_name'])+',"__missing__")==-1);')
    assertions=staging/'assertions.lua';authored=ROOT/'tests/shared-script-assertions.lua'
    assertions.write_text('\n'.join(statements)+'\n'+authored.read_text(),encoding='ascii',newline='\n')
    def run(*args):return subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True).stdout.strip()
    hashes={};device=None
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
        all_paths=[*paths,base+'/list.txt',base+'/assertions.lua',base+'/runner']
        for line in adb('shell','sha256sum',*all_paths).splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for remote,local in zip(all_paths,[*targets,listing,assertions,a.runner]):assert hashes[remote]==sha(local)
        output=adb('shell',base+'/runner','--execute',base+'/list.txt',base+'/assertions.lua')
    else:output=run(a.runner.resolve(),'--execute',listing.resolve(),assertions.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504']
    assert re.findall(r'^EXECUTED (\d+) (\d+)$',output,re.M)==[('0','0'),('1','0'),('2','0')]
    assert output.splitlines()[-1]=='ORIGINAL SHARED SCRIPTS PASS 3'
    result={'complete_game':False,'original_game_scripts_executed':3,'original_gameplay_equivalence_tested':False,
            'field_entries_checked':636,'getpystruct_and_getpyoid_alias_queries':1272,
            'builtin_tables':71,'unique_registered_classes':67,'original_initializer_lookup_checks':693,
            'scope':'Exact AI/skill shared scripts and combat formula definitions execute. Authored tests cover animation-event dispatch/detach, skill registration/defaults/select/unselect, and combat without combatants. No entity callbacks, timers, real combat, AI behavior or game loop execute.',
            'inputs':selected,'field_trace_sha256':sha(trace_path),'script_manifest_sha256':sha(manifest_path),
            'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),
            'assertions_sha256':sha(assertions),'authored_calls_sha256':sha(authored),
            'source_sha256':{name:sha(ROOT/name)for name in ('runtime.c','runtime.h','tests/runner.c',
                'tests/execution.c','../pydata-names/lua-bridge.c','../pydata-names/struct-names.h')},'stdout':output}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('inputs','stdout')}))
if __name__=='__main__':main()
