"""Original complete DebugSwitches save/writer order and live-map proof."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/debug-switches-persistence/original-functions.json'
D,D2,FS,F,A,E,VF,VS=0x9a1d18,0x10030000,0x10040000,0x10050000,0x10042000,0x10041000,0x10043000,0x10044000
OPEN,CLOSE=0x10011000,0x10011004
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def configuration(modules,switches):
    out=bytearray(struct.pack('<III',0x44425357,0x20000,len(modules)))
    def rows(items):
        for key,value in items:
            key=key.encode() if isinstance(key,str) else key
            out.extend(struct.pack('<I',len(key))+key+bytes([value]))
    rows(modules);out.extend(struct.pack('<I',len(switches)));rows(switches);return bytes(out)
def parse(raw):
    pos=0
    def word():
        nonlocal pos
        value=struct.unpack_from('<I',raw,pos)[0];pos+=4;return value
    assert word()==0x44425357 and word()==0x20000
    def rows():
        nonlocal pos
        count=word();out={}
        for _ in range(count):
            n=word();key=raw[pos:pos+n];pos+=n;out[key]=raw[pos];pos+=1
        return out
    modules,switches=rows(),rows();assert pos==len(raw);return modules,switches
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
            at=(int(row['pc_add_address'],0)+8+struct.unpack('<I',data(int(row['literal_address'],0),4))[0])&0xffffffff;b=row['text'].encode()+b'\0'
            assert at==int(row['elf_address'],0) and data(at,len(b))==b and hashlib.sha256(b).hexdigest()==row['sha256']
        for row in manifest['global_objects']:
            s=symbols[row['symbol']];assert (s['st_value'],s['st_size'])==(int(row['target'],0),row['bytes'])
            assert struct.unpack('<I',data(int(row['slot'],0),4))[0]==int(row['target'],0)
    return manifest
def fixtures(cache,folder):
    simple=folder/'simple.bin';simple.write_bytes(configuration([], [('A',1),('B',0)]))
    modules=folder/'modules.bin';modules.write_bytes(configuration([('Beta',7),('Alpha',1)], [('B',0),('A',1)]))
    empty=folder/'empty.bin';empty.write_bytes(configuration([],[]))
    unusual=folder/'unusual.bin';unusual.write_bytes(configuration([(b'\0M',255)],[(b'',255),(b'A\0Z',7),(b'\x80',1)]))
    rows=[]
    def add(name,file,mode=0,loaded=1,mutation=0,missing=0,null_fs=0,null_stream=0):rows.append((name,file,[mode,loaded,mutation,missing,null_fs,null_stream]))
    for file in (simple,modules,empty,unusual,cache/'DebugSwitches.savegame'):
        for mode in (0,1):
            for loaded in (0,1):add(f'{file.stem}_mode{mode}_loaded{loaded}',file,mode,loaded)
    for mutation in (1,2,3,4,5,6,7,8,10):
        for mode in (0,1):
            if mutation==1 and mode==0:continue
            add(f'live_mutation{mutation}_mode{mode}',simple,mode,mutation=mutation)
    add('module_value_fresh',modules,mutation=5)
    add('module_count_before_insert',empty,mutation=3)
    add('switch_count_before_insert',empty,mutation=4)
    for loaded in (0,1):
        add(f'normal_open_miss{loaded}',simple,mode=1,loaded=loaded,missing=1)
        add(f'normal_null_filesystem{loaded}',simple,mode=1,loaded=loaded,null_fs=1)
        add(f'normal_null_stream{loaded}',simple,loaded=loaded,null_stream=1)
    forced=folder/'forced.bin';forced.write_bytes(configuration([], [('IsDeactivatingFlashMenus',1),('IsDeactivatingFlashMenusUpdate',1),('IsDeactivatingFlashMenusRender',1)]))
    add('source_nested_saves',forced,loaded=0)
    add('source_nested_saves_outer_save',forced,mode=1,loaded=0)
    return rows
def oracle(original,exe,cache,folder):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=verify(original);old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x100000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def cstring(at):
        out=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':out+=old.uc.mem_read(at,1);at+=1
        return bytes(out)
    records=[]
    for name,file,args in fixtures(cache,folder):
        mode,loaded,mutation,missing,null_fs,null_stream=args
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(0x10010000,bytes(0xf0000));old.uc.mem_write(D,bytes(48));old.uc.mem_write(0x9a1d48,bytes([loaded]))
        old.pointer(0x99531c,D);old.pointer(0x99828c,A);old.pointer(A+0x10,E);old.pointer(E+0x34,0 if null_fs else FS);old.pointer(FS,VF);old.pointer(VF+0x94,OPEN);old.pointer(VF+0x78,CLOSE)
        maps={D:{},D+0x18:{},D2:{},D2+0x18:{}};strings={};next_node=[0x10060000];streams={};calls=[];opened=[0];mutated=[False];counts={};writes={}
        def value(owner,key):return old.uc.mem_read(maps[owner][key]+0x28,1)[0]
        def rebuild(owner):
            order=sorted(maps[owner]);nodes=[maps[owner][key] for key in order]
            old.pointer(owner+4,nodes[0] if nodes else 0);old.pointer(owner+8,nodes[0] if nodes else owner);old.pointer(owner+0xc,nodes[-1] if nodes else owner);old.pointer(owner+0x10,len(nodes))
            for i,node in enumerate(nodes):
                old.pointer(node+4,owner if i==0 else nodes[i-1]);old.pointer(node+8,0);old.pointer(node+0xc,nodes[i+1] if i+1<len(nodes) else 0)
        def insert(owner,key,raw=None):
            if key not in maps[owner]:
                node=next_node[0];next_node[0]+=0x400;assert next_node[0]<0x100f0000;maps[owner][key]=node;old.pointer(node+0x24,node+0x30);old.pointer(node+0x20,node+0x30+len(key));old.uc.mem_write(node+0x30,key+b'\0');old.uc.mem_write(node+0x28,b'\0');rebuild(owner)
            if raw is not None:old.uc.mem_write(maps[owner][key]+0x28,bytes([raw]))
            return maps[owner][key]+0x28
        module_rows,switch_rows=parse(file.read_bytes())
        for owner,rows in ((D+0x18,module_rows),(D,switch_rows),(D2,{}),(D2+0x18,{})):
            rebuild(owner)
            for key,raw in rows.items():insert(owner,key,raw)
        def snapshot(op,owner,fs=0,stream=0,word=0,text=b''):
            calls.append([op,owner,fs,stream,word,text.hex()])
            if mutation==1 and op==0:old.pointer(E+0x34,0);old.pointer(A+0x10,0);old.pointer(0x99828c,0)
            if mutated[0]:return
            if mutation==2 and op==1 and word==0x44425357:insert(D+0x18,b'AddedModule',1);insert(D,b'AddedSwitch',1);mutated[0]=True
            if mutation==3 and op==1 and len(calls)==3:insert(D+0x18,b'LaterModule',1);mutated[0]=True
            if mutation==4 and op==1 and len(calls)==4:insert(D,b'AfterCount',1);mutated[0]=True
            if mutation==5 and op==2:insert(D if text in maps[D] else D+0x18,text,7);mutated[0]=True
            if mutation==6 and op==3:insert(D,b'ZZInserted',1);mutated[0]=True
            if mutation==7 and op==2:old.pointer(0x99531c,D2);mutated[0]=True
            if mutation==8 and op==1:mutated[0]=True
            if mutation==10 and op==1 and len(calls)==4:old.pointer(0x99531c,D2);mutated[0]=True
        def observe(_,at,__,___):
            lr=old.uc.reg_read(old.lr)
            if at==OPEN:
                assert old.reg(0)==FS and cstring(old.reg(1))==b'DebugSwitches.savegame'
                if lr==0x3378f8:snapshot(5,old.reg(7),FS,text=b'DebugSwitches.savegame');returned(0)
                else:
                    assert lr==0x337d9c and old.reg(2)==1;snapshot(0,old.reg(5),FS,word=1,text=b'DebugSwitches.savegame')
                    if missing:returned(0)
                    else:opened[0]+=1;stream=F+4*opened[0];streams[stream]=bytearray();old.pointer(stream,VS);returned(stream)
            elif at==CLOSE:
                assert old.reg(0)==FS;stream=get(old.reg(1));assert stream in streams;snapshot(4,old.reg(5),FS,stream);returned()
            elif at==0x33665c:
                stream,word=old.reg(0),old.reg(1);streams.setdefault(stream,bytearray()).extend(struct.pack('<I',word));snapshot(1,old.reg(8),stream=stream,word=word)
                if lr==0x337ba4:counts.setdefault(stream,[0,0])[0]=word
                if lr==0x337c10:counts.setdefault(stream,[0,0])[1]=word
                returned()
            elif at==0x317490:
                stream,pointer,size,high=(old.reg(i) for i in range(4));assert high==0;raw=bytes(old.uc.mem_read(pointer,size));streams.setdefault(stream,bytearray()).extend(struct.pack('<I',size)+raw);snapshot(2,old.reg(8),stream=stream,text=raw);returned(1)
            elif at==0x336718:
                stream,word=old.reg(0),old.reg(1);assert word<=255;streams.setdefault(stream,bytearray()).append(word);snapshot(3,old.reg(8),stream=stream,word=word)
                part=0 if lr==0x337bd8 else 1;assert lr in (0x337bd8,0x337c84);writes.setdefault(stream,[0,0])[part]+=1;returned()
            elif at==0x3140ec:strings[old.reg(0)]=cstring(old.reg(1));returned(old.reg(0))
            elif at==0x318254:assert old.reg(0) in strings;strings.pop(old.reg(0));returned()
            elif at==0x3369a8:
                owner=old.reg(0);key=strings[old.reg(1)];assert owner in maps;returned(maps[owner].get(key,owner))
            elif at==0x337288:
                owner=old.reg(0);key=strings[old.reg(1)];assert owner in maps;returned(insert(owner,key))
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        if mode:old.invoke(0x337d54,[D])
        else:old.invoke(0x337b4c,[D,0 if null_stream else F])
        old.uc.hook_del(hook)
        top_stream=F+4 if mode and not missing and not null_fs else (F if not mode and not null_stream else 0)
        top_calls=sum(bool(row[0]!=5 and ((row[3]==top_stream and top_stream) or (row[0]==0 and i==0))) for i,row in enumerate(calls))
        header=counts.get(top_stream,[0,0]);completed=writes.get(top_stream,[0,0])
        expected={'status':0,'loaded':old.uc.mem_read(0x9a1d48,1)[0],'singleton':get(0x99531c),'modules':{k.hex():value(D+0x18,k) for k in maps[D+0x18]},'first':{k.hex():value(D,k) for k in maps[D]},'second':{k.hex():value(D2,k) for k in maps[D2]},'streams':{str(k):v.hex() for k,v in streams.items()},'calls':calls,'result':[top_calls,*header,*completed]}
        actual=json.loads(subprocess.check_output([str(exe),str(file),*map(str,args)],text=True));assert actual==expected,(name,actual,expected)
        records.append({'case':name,'arguments':args,'file_sha256':digest(file),'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,'executed_scope':'Actual complete520B _saveSwitches and136B save instructions, including original live-tree next-node traversal/count reads/string field/byte rereads. Complete original load/GetSwitch/SetSwitch callers also execute for nested source recursion; their maps/string allocation are named fixtures with retained real node identities/byte cells. Named188B word/byte and184B string stream-write dependencies emit real bytes in memory-backed fixtures. No full native stream/STL/string/allocator/OS filebody claim; unknown imports fail.','new_complete_callers':2,'new_complete_dependency_bodies':0}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');p.add_argument('--output',type=Path,default=MODULE/'build/debug-switches-persistence/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/debug-switches-persistence/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'debug_switches_runtime.cpp',MODULE/'debug_switches_persistence.cpp',MODULE/'tests/debug_switches_persistence.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];result=subprocess.run(command,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+result.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':44,'mismatches':0}
    manifest=verify(a.original_elf.resolve());cache=a.cache.resolve();assert digest(cache/'DebugSwitches.savegame')==manifest['cache_debug_configuration_sha256']
    folder=exe.parent/'fixtures';folder.mkdir(exist_ok=True);comparison=oracle(a.original_elf.resolve(),exe,cache,folder);assert comparison['comparisons']==48
    owned=[MODULE/'debug_switches_persistence.hpp',MODULE/'debug_switches_persistence.cpp',MODULE/'tests/debug_switches_persistence.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/debug-switches-persistence/NOTES.md']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':manifest['original_sha256'],'source_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):digest(f) for f in owned},'reused_runtime_sha256':{name:digest(MODULE/name) for name in ('debug_switches_runtime.hpp','debug_switches_runtime.cpp')},'reused_primitive_sha256':digest(ROOT/'port/persistence/binary.h'),'oracle_loader_sha256':digest(ROOT/'port/engine-resources/tests/cpu.py'),'compiler_command':command,'native_wired':False,'new_complete_caller_bodies':2,'new_complete_dependency_bodies':0}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_cases':44,'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
