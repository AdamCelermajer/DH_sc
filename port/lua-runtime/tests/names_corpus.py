#!/usr/bin/env python3
"""Import exact original-reader name-table segments and query IDs through Lua."""
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
    trace_path=REPO/'reports/pydata-array-name-trace.json';trace=json.loads(trace_path.read_text())
    staging=ROOT/'build/names-corpus';staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-names-qa';paths=[];inputs=[];lines=[];statements=[];queries=0
    for file in trace['files']:
        path=a.cache/file['path'];raw=path.read_bytes();assert sha(path)==file['sha256']
        for table in file['tables']:
            target=staging/f'{len(inputs):03}.bin';segment=raw[table['start']:table['end']];target.write_bytes(segment)
            name=table['registered_name'];assert '\t'not in name and '\n'not in name
            selected=f'{base}/{target.name}'if a.adb else str(target.resolve());paths.append(selected)
            lines.append(name+'\t'+selected)
            inputs.append({'path':file['path'],'file_sha256':sha(path),'start':table['start'],'end':table['end'],
                           'segment_sha256':sha(target),'registered_name':name,'count':table['count']})
            first={}
            for i,value in enumerate(table['names']):first.setdefault(value,i)
            for value,index in first.items():
                statements.append(f'assert(GetPyOID({json.dumps(name)},{json.dumps(value)})=={index});');queries+=1
            statements.append(f'assert(GetPyOID({json.dumps(name)},"__missing__")==-1);')
    assert len(inputs)==71 and queries==8863
    listing=staging/'list.txt';listing.write_text('\n'.join(lines)+'\n',encoding='utf-8',newline='\n')
    statements.append('assert(GetPyOID("__missing__","__missing__")==-1);assert(GetPyStruct==nil);')
    assertions=staging/'assertions.lua';assertions.write_text('\n'.join(statements)+'\n',encoding='ascii',newline='\n')
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
        all_paths=[*paths,base+'/list.txt',base+'/assertions.lua',base+'/runner']
        for offset in range(0,len(all_paths),40):
            output=adb('shell','sha256sum',*all_paths[offset:offset+40])
            for line in output.splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for path,row in zip(paths,inputs):assert hashes[path]==row['segment_sha256']
        for path,target in ((base+'/list.txt',listing),(base+'/assertions.lua',assertions),(base+'/runner',a.runner)):
            assert hashes[path]==sha(target)
        output=adb('shell',base+'/runner','--names',base+'/list.txt',base+'/assertions.lua')
    else:output=run(a.runner.resolve(),'--names',listing.resolve(),assertions.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504']
    statuses=re.findall(r'^NAMES (\d+) (\d+)$',output,re.M);assert len(statuses)==71
    for index,(number,status)in enumerate(statuses):assert int(number)==index and int(status)==0
    assert output.splitlines()[-1]=='NAME LOOKUPS PASS 8863'
    result={'complete_game':False,'game_scripts_executed':False,'authored_getpyoid_queries':queries,
            'tables_imported':71,'original_files_hash_checked':35,'staged_table_segments':71,
            'caller_buffers_released_before_queries':True,'malformed_import_rollback':True,
            'memory_failure_rollback':True,'struct_field_names_installed':False,
            'inputs':inputs,'reader_trace_sha256':sha(trace_path),'runner_sha256':sha(a.runner),
            'test_sha256':sha(Path(__file__)),'assertions_sha256':sha(assertions),
            'lookup_expectations':'Ordered original reader names; 266 original lookup cases separately established by trace. This test uses authored queries, with array-record size agreement and original game scripts outside scope.',
            'source_sha256':{name:sha(ROOT/name)for name in ('runtime.c','runtime.h','tests/runner.c',
                            'tests/names.c','../pydata-names/names.c','../pydata-names/names.h','../pydata-names/lua-bridge.c')},
            'stdout':output}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('inputs','stdout')}))
if __name__=='__main__':main()
