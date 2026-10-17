"""Owned original DebugSwitch callers and evidenced binary switch-loop replay."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/debug-switches-runtime/original-functions.json'
D,D2,FS,F,A,E,VF,VS,KEY=0x9a1d18,0x10030000,0x10040000,0x10050000,0x10042000,0x10041000,0x10043000,0x10044000,0x10051000
OPEN,CLOSE,SIZE,POSITION=0x10011000,0x10011004,0x10011008,0x1001100c
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def configuration(rows,version=0x20000):
    b=bytearray(struct.pack('<II',0x44425357,version))
    if version>=0x20000:b+=struct.pack('<I',0)
    b+=struct.pack('<I',len(rows))
    for name,value in rows:
        text=name.encode();b+=struct.pack('<I',len(text))+text+bytes([value])
    return bytes(b)
def fixtures(cache,folder):
    rows=[]
    def add(name,file,mode=0,loaded=0,mutation=0,missing=0,key='isTracingChar_Stats'):
        rows.append((name,file,[mode,loaded,mutation,missing,key]))
    actual=cache/'DebugSwitches.savegame'
    for mode in range(4):
        for loaded in (0,1):add(f'actual_mode{mode}_loaded{loaded}',actual,mode,loaded)
    for mutation in range(1,7):
        for mode in (1,3):add(f'fresh_key_global_{mutation}_{mode}',actual,mode,mutation=mutation)
    for mode in range(4):add(f'missing_file_{mode}',actual,mode,missing=1)
    for n in range(12):
        p=folder/f'short{n}.bin';p.write_bytes(bytes(n));add(f'source_short_file{n}',p)
    configs=[('v1',configuration([('New',1)],0x10000)),('v1_latest',configuration([('New',7)],0x1ffff)),('v2',configuration([('New',1)])),('v2_latest',configuration([('New',1)],0x20001)),('negative_modules',struct.pack('<IIII',0x44425357,0x20000,0xffffffff,0)),('negative_switches',struct.pack('<IIII',0x44425357,0x20000,0,0xffffffff)),('duplicate_keys',configuration([('Dup',1),('Dup',0),('Dup',1)])),('forced_flags',configuration([('IsDeactivatingFlashMenus',1),('IsDeactivatingFlashMenusUpdate',1),('IsDeactivatingFlashMenusRender',1)])),('trace_keys_true',configuration([('isTracingDebugSwitches',1),('isTracingDebugSwitchesFile',1),('Keep',0)])),('empty_switches',configuration([])),('embedded_nul',configuration([('Before\0After',1)]))]
    for name,raw in configs:
        p=folder/(name+'.bin');p.write_bytes(raw);add(name,p)
        if name=='forced_flags':add('captured_default_owner',p,mutation=7)
    return rows
def verify(original):
    from elftools.elf.elffile import ELFFile
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);o=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[o:o+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=symbols[row['original_symbol']];assert (s['st_value'],s['st_size'])==(at,n) and hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['literals']:
            at=(int(row['pc_add_address'],0)+8+struct.unpack('<I',data(int(row['literal_address'],0),4))[0])&0xffffffff;text=row['text'].encode()+b'\0'
            assert at==int(row['elf_address'],0) and data(at,len(text))==text and hashlib.sha256(text).hexdigest()==row['sha256']
        for row in manifest['global_objects']:
            s=symbols[row['symbol']];assert (s['st_value'],s['st_size'])==(int(row['target'],0),row['bytes'])
            assert struct.unpack('<I',data(int(row['slot'],0),4))[0]==int(row['target'],0)
    return manifest
def oracle(original,exe,cache,folder):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=verify(original);objects={r['symbol']:r for r in manifest['global_objects']}
    dbg_slot=int(objects['_ZN13DebugSwitches6s_instE']['slot'],0);loaded_at=int(objects['_ZN13DebugSwitches8s_loadedE']['target'],0);app_slot=int(objects['_ZN9SingletonI11ApplicationE6s_instE']['slot'],0)
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x80000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value=0,high=None):
        old.put(0,value)
        if high is not None:old.put(1,high)
        old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def text(at):
        b=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':b+=old.uc.mem_read(at,1);at+=1
        return b.decode('latin1')
    records=[]
    for name,file,args in fixtures(cache,folder):
        mode,loaded,mutation,missing,key=args;content=bytearray(file.read_bytes());position=[0];cells={};strings={KEY:key};next_cell=[0x10056000];calls=[]
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(0x10010000,bytes(0x70000));old.pointer(dbg_slot,D);old.pointer(app_slot,A);old.uc.mem_write(loaded_at,bytes([loaded]));old.pointer(A+0x10,E);old.pointer(E+0x34,FS)
        old.pointer(FS,VF);old.pointer(VF+0x94,OPEN);old.pointer(VF+0x78,CLOSE);old.pointer(F,VS);old.pointer(VS+8,SIZE);old.pointer(VS+0x24,POSITION)
        def maps(owner):return {key:old.uc.mem_read(at,1)[0] for (which,key),at in cells.items() if which==owner}
        def snapshot(op,owner,fs=0,stream=0):
            current=maps(owner);calls.append([op,owner,fs,stream,len(current),int('isTracingDebugSwitches' in current),int('isTracingDebugSwitchesFile' in current),int('isTracingChar_Stats' in current)])
            if mutation==1 and op==0:old.pointer(dbg_slot,D2)
            if mutation==2 and op==0:old.pointer(E+0x34,0);old.pointer(A+0x10,0);old.pointer(app_slot,0)
            if mutation==3 and op==0:strings[KEY]='ChangedKey'
            if mutation==4 and op==2:strings[KEY]='ChangedKey'
            if mutation==6 and op==2 and content:content[-1]=1
            if mutation==7 and op==2 and any(row[0]==1 for row in calls):old.pointer(dbg_slot,D2)
        def word():
            assert position[0]+4<=len(content);v=struct.unpack_from('<I',content,position[0])[0];position[0]+=4;return v
        def observe(_,at,__,___):
            if at==OPEN:
                assert old.reg(0)==FS and text(old.reg(1))=='DebugSwitches.savegame';snapshot(0,old.reg(7),FS);returned(0 if missing else F)
            elif at==CLOSE:
                assert old.reg(0)==FS and get(old.reg(1))==F;snapshot(1,old.reg(7),FS,F);returned()
            elif at==SIZE:assert old.reg(0)==F;returned(len(content),0)
            elif at==POSITION:assert old.reg(0)==F;returned(position[0],0)
            elif at==0x3364ec:assert old.reg(0)==F;returned(word())
            elif at==0x3365a4:
                assert old.reg(0)==F and position[0]<len(content);v=content[position[0]];position[0]+=1;returned(v)
            elif at==0x317734:
                assert old.reg(0)==F and old.reg(2)==255 and old.reg(3)==0;n=word();assert n<=255 and position[0]+n<=len(content);b=bytes(content[position[0]:position[0]+n]);position[0]+=n;old.uc.mem_write(old.reg(1),b+b'\0');returned()
            elif at==0x3140ec:strings[old.reg(0)]=text(old.reg(1));returned(old.reg(0))
            elif at==0x318254:assert old.reg(0) in strings;strings.pop(old.reg(0));returned()
            elif at==0x3369a8:
                owner=old.reg(0);assert owner in (D,D2);k=strings[old.reg(1)];returned(owner+0x80 if (owner,k) in cells else owner)
            elif at==0x337288:
                owner=old.reg(0);assert owner in (D,D2);k=strings[old.reg(1)]
                if (owner,k) not in cells:cells[(owner,k)]=next_cell[0];next_cell[0]+=4;old.uc.mem_write(cells[(owner,k)],b'\0')
                returned(cells[(owner,k)])
            elif at==0x337d54:snapshot(2,old.reg(0));returned()
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        if mode==0:old.invoke(0x337888,[D]);value=77
        elif mode==1:value=old.invoke(0x337a88,[D,KEY])&255
        else:old.invoke(0x337ddc,[D,KEY,int(mode==3)]);value=77
        old.uc.hook_del(hook)
        expected={'status':0,'value':value,'loaded':old.uc.mem_read(loaded_at,1)[0],'singleton':get(dbg_slot),'position':position[0],'first':maps(D),'second':maps(D2),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),str(file),*map(str,args)],text=True));assert actual==expected,(name,actual,expected)
        records.append({'case':name,'arguments':args,'file_sha256':digest(file),'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,'executed_scope':'Actual complete original512B load,196B GetSwitch and236B SetSwitch instructions, including internal nested recursion and stores. Actual932B _loadSwitches instructions execute only evidenced WSBD versions1/2 with nonpositive module count, bounded names and valid switch loops (plus source<=11B early-return). Data primitive184/184/244B, string52/68B, map-find212B/operator[]380B and save136B entries are explicitly named provider fixtures. Source getter/setter instructions write/read actual fake-container bool cells; map/storage allocation bodies are outside the comparison. Filesystem virtual size/position/open/close are explicit memory-stream fixtures. Unknown original imports fail. No full932B/text/module/save/file/map/string body claim.','new_complete_callers':3,'new_complete_dependency_bodies':0}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');p.add_argument('--output',type=Path,default=MODULE/'build/debug-switches-runtime/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/debug-switches-runtime/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'debug_switches_runtime.cpp',MODULE/'tests/debug_switches_runtime.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    result=subprocess.run(command,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+result.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':41,'mismatches':0}
    manifest=verify(a.original_elf.resolve());cache=a.cache.resolve();file=cache/'DebugSwitches.savegame';authored=manifest['cache_debug_configuration'];assert file.stat().st_size==665 and digest(file)==authored['sha256']
    fixtures_folder=exe.parent/'fixtures';fixtures_folder.mkdir(exist_ok=True);comparison=oracle(a.original_elf.resolve(),exe,cache,fixtures_folder);assert comparison['comparisons']==48
    owned=[*sources,MODULE/'debug_switches_runtime.hpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/debug-switches-runtime/NOTES.md']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':manifest['original_sha256'],'source_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):digest(f) for f in owned},'reused_primitive_sha256':digest(ROOT/'port/persistence/binary.h'),'cache_debug_configuration':authored,'compiler_command':command,'native_wired':False,'new_complete_caller_bodies':3,'new_complete_dependency_bodies':0}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_cases':41,'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
