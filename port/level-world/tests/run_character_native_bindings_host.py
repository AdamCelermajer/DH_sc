"""Ordered GameObject/Character registrations vs actual original ARM callers."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-native-bindings/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for rows in manifest['registrations'].values():
            for row in rows:
                text=row['name'].encode()+b'\0';literal=data(int(row['name_elf_address'],0),len(text))
                assert literal==text and hashlib.sha256(literal).hexdigest()==row['name_sha256']
                assert int(symbols[row['original_symbol']]['st_value'])==int(row['target_elf_address'],0)
        for row in manifest['on_init_dependency_facts']['set_level']['design_literals']:
            pc,pool=int(row['pc_add_address'],0),int(row['literal_address'],0)
            at=(pc+8+struct.unpack('<I',data(pool,4))[0])&0xffffffff
            text=row['text'].encode()+b'\0'
            assert at==int(row['elf_address'],0) and data(at,len(text))==text
            assert hashlib.sha256(text).hexdigest()==row['sha256']
    target_keys={int(row['target_elf_address'],0):row['function_key'] for row in manifest['registrations']['character']}
    assert len(target_keys)==128 and len(set(target_keys.values()))==128
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def string(at):
        raw=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':raw+=old.uc.mem_read(at,1);at+=1
        return raw.decode('ascii')
    records=[]
    identities=[(0x10014000,0x10020000),(0x10014100,0x10021000),(0x10018000,0x10030000),
        (0x10022000,0x10012000),(0x10033000,0x10031000)]
    for character,entry,key in ((False,0x38d7ec,'game_object'),(True,0x3b56bc,'character')):
        for owner,binder in identities:
            old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(owner,bytes(0x100));old.uc.mem_write(binder,bytes(0x100))
            calls=[];evidence=[]
            def observe(_,at,__,___):
                if at not in (0x31a4d4,0x319af4):return
                assert old.reg(0)==binder
                kind=0 if at==0x31a4d4 else 1;name=string(old.reg(1));target=old.reg(2)
                userdata=old.reg(3) if kind==0 else 0
                if kind==0:assert userdata in (0,owner)
                calls.append([kind,name,target_keys[target],userdata])
                evidence.append({'kind':kind,'name':name,'target':hex(target),'binder':hex(old.reg(0)),
                    'userdata':hex(userdata),'caller_return':hex(old.uc.reg_read(old.lr))})
                # Named Binder installation fixture retains contexts and succeeds.
                # Original caller instructions still select names/functions/order.
                old.put(0,0xfeed);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
            hook=old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(entry,[owner,binder]);old.uc.hook_del(hook)
            pinned=[[0 if row['kind']=='function' else 1,row['name'],row['function_key'],
                owner if row['context']=='character' else 0] for row in manifest['registrations'][key]]
            assert calls==pinned,(key,owner,binder)
            functions=sum(call[0]==0 for call in calls)
            expected={'status':0,'functions':functions,'methods':len(calls)-functions,'calls':calls}
            actual=json.loads(subprocess.check_output([str(exe),str(int(character)),str(owner),str(binder)],text=True))
            assert actual==expected,(key,owner,binder,actual,expected)
            records.append({'entry':hex(entry),'owner':hex(owner),'binder':hex(binder),'matched':True,
                'source_result':expected,'compiled_result':actual,'original_registration_evidence':evidence})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'Complete original2648B GameObject and5332B Character createBindings caller instructions, including the actual inherited GameObject call and original tail registrations. Only concrete Binder bindFunction344B/bindMethod336B installation bodies are named fixture services. Caller branches, ordered strings, GOT callback targets and explicit function userdata are not mocked. Method registration has no explicit userdata; actual object-wrapper receiver dispatch remains an external provider. No callback bodies, VM installation or floating-point helpers are modeled/executed.',
        'native_wired':False}

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files')
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-native-bindings/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-native-bindings/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_native_bindings.cpp',MODULE/'tests/character_native_bindings.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if built.returncode:raise RuntimeError(built.stdout+built.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==733
    comparison=oracle(args.original_elf.resolve(),exe);assert comparison['comparisons']==10 and comparison['mismatches']==0
    manifest=json.loads(MANIFEST.read_text());cache=args.cache.resolve()
    for row in manifest['authored_cache_evidence']:
        p=cache/row['path'];assert p.stat().st_size==row['bytes'] and digest(p)==row['sha256']
    def strings(p):
        raw=p.read_bytes();count=struct.unpack_from('<I',raw)[0];at=4;result=[]
        assert 0<count<=10000
        for _ in range(count):
            size=struct.unpack_from('<I',raw,at)[0];at+=4;assert 0<size<=4096 and at+size<=len(raw)
            result.append(raw[at:at+size].decode('ascii'));at+=size
        return result
    pydata=cache/'data/pydata';names=strings(pydata/'character_properties_pyarraynames.bin')
    fields=strings(pydata/'character_properties_pystructnames.bin');assert len(fields)==224
    raw=(pydata/'character_properties_pyarray.bin').read_bytes();assert struct.unpack_from('<I',raw)[0]==len(names)
    for key,value in manifest['on_init_dependency_facts']['level_property_ids'].items():assert fields[value]==key
    for row in manifest['authored_ghost_rows']:
        assert names[row['id']]==row['name'];values=struct.unpack_from('<224i',raw,4+row['id']*896)
        for key,value in row['properties'].items():assert values[fields.index(key)]==value
    class_names=strings(pydata/'character_classes_pyarraynames.bin');buff=manifest['authored_class_name']
    assert class_names[buff['id']]==buff['name'] and class_names.count(buff['name'])==1
    for row in manifest['unchanged_script_evidence']:
        p=ROOT/row['path'];assert p.stat().st_size==row['bytes'] and digest(p)==row['sha256']
    from elf_import_identity import verify_imports
    verified_imports=verify_imports(args.original_elf.resolve(),{0x30e4cc:'__aeabi_f2iz'})
    paths=sources+[MODULE/'character_native_bindings.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in paths},'native_wired':False,'new_callback_bodies':0,'whole_original_vm_parity':False,
        'authored_cache_evidence':manifest['authored_cache_evidence'],'authored_ghost_rows':manifest['authored_ghost_rows'],
        'verified_on_init_import_identity':verified_imports,'import_identity_helper_sha256':digest(MODULE/'tests/elf_import_identity.py')}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'registrations':265,'mismatches':0}))
if __name__=='__main__':main()
