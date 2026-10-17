"""Compare native AIS binding callers with original ARM registration order."""
import argparse,hashlib,json,struct,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/level-world'
MANIFEST=MODULE/'reference/ais-native-bindings/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS=0x10010000
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',required=True)
    p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/ais-native-bindings-host')
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'ais_native_bindings.cpp',MODULE/'ais_native_bindings.hpp',MODULE/'tests/ais_native_bindings.cpp']
    exe=out/'host.exe'
    command=[a.compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',str(sources[0]),str(sources[2]),'-o',str(exe)]
    subprocess.run(command,check=True)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS'
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'))
    assert digest(a.original_elf)==manifest['original_sha256']==SHA
    with a.original_elf.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        raw=a.original_elf.read_bytes();loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def read(at,n):
            segment=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=segment['p_offset']+at-segment['p_vaddr'];return raw[offset:offset+n]
        for row in manifest['functions']:
            at=int(row['elf_address'],0);n=row['size'];sym=syms[row['original_symbol']]
            assert (sym['st_value'],sym['st_size'])==(at,n)
            assert hashlib.sha256(read(at,n)).hexdigest()==row['sha256']
        for binding in manifest['bindings']:
            name=binding['name'];at=int(binding['name_address'],0)
            assert read(at,len(name)+1)==name.encode()+b'\0'
            assert hashlib.sha256(read(at,len(name)+1)).hexdigest()==binding['name_sha256']
            assert syms[binding['original_symbol']]['st_value']==int(binding['function_address'],0)
        for library in manifest['libraries']:
            at=int(library['wrapper_address'],0);leaf=int(library['leaf_address'],0)
            load,branch=struct.unpack('<2I',read(at,8))
            assert load==0xe5900004 and branch>>24==0xea
            displacement=branch&0xffffff
            if displacement&0x800000:displacement-=0x1000000
            assert at+12+displacement*4==leaf==syms[library['leaf_symbol']]['st_value']
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    from unicorn import UC_HOOK_CODE
    old=Cpu(a.original_elf,False,manifest);old.uc.mem_map(0x10000000,0x30000)
    leaf_to_library={int(r['leaf_address'],0):r['library_id'] for r in manifest['libraries']}
    name_to_binding={r['name']:r for r in manifest['bindings']}
    def string(at):
        data=bytearray()
        while len(data)<512:
            byte=old.uc.mem_read(at+len(data),1)[0]
            if not byte:return data.decode('utf-8')
            data.append(byte)
        raise AssertionError('Missing binding string terminator')
    cases=[]
    for entry,at in enumerate((0x37b5a0,0x3d8ec8,0x3d8f2c)):
        for mutation in (0,1):
            events=[];old.pointer(AIS+8,0x10020000)
            def trace(uc,address,size,user):
                if address in leaf_to_library:
                    events.append({'kind':'open_library','library':leaf_to_library[address],
                        'wrapper':AIS+4,'raw_vm':old.reg(0)})
                elif address==0x31a4d4:
                    name=string(old.reg(1));binding=name_to_binding[name]
                    assert old.reg(2)==int(binding['function_address'],0)
                    events.append({'kind':'binding','binder':old.reg(0),'name':name,
                        'function_id':binding['function_id'],'userdata':old.reg(3)})
                else:return
                if mutation:
                    raw_vm=struct.unpack('<I',old.uc.mem_read(AIS+8,4))[0]
                    old.pointer(AIS+8,raw_vm+16)
                uc.reg_write(old.pc,uc.reg_read(old.lr))
            hook=old.uc.hook_add(UC_HOOK_CODE,trace)
            old.invoke(at,[AIS]);old.uc.hook_del(hook)
            opened=sum(event['kind']=='open_library' for event in events)
            expected={'status':0,'calls':len(events),'libraries_opened':opened,
                'functions_bound':len(events)-opened,'events':events}
            actual=json.loads(subprocess.check_output([str(exe),str(entry),str(AIS),str(mutation)],text=True))
            assert actual==expected,(entry,mutation,actual,expected)
            cases.append({'entry':manifest['functions'][entry]['original_symbol'],
                'live_vm_mutation':bool(mutation),'matched':True,'source_result':expected})
    # Native opaque identities must survive above the old ARM32 address range.
    native_high=json.loads(subprocess.check_output([str(exe),'2',str(0x100000001),'1'],text=True))
    assert all(event.get('userdata',event.get('wrapper',0))>0xffffffff for event in native_high['events'])
    paths=sources+[Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_cases':len(cases),'mismatches':0,
        'original_sha256':SHA,'compiler_command':command,'cases':cases,'native_high_pointer':native_high,
        'source_sha256':{path.relative_to(ROOT).as_posix():digest(path) for path in paths},
        'native_wired':False,'registered_callback_bodies_reconstructed':0,
        'scope':'Original1252B base,100B AI and24B composite callers plus actual8B library wrappers execute. 35 exact names/function identities/userdata and base/math/table/string open order match. Library bodies and Binder344B remain named provider boundaries. Native service failures are host contract tests, not source ARM failure branches. No active Ghost runtime claim.'}
    (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':len(cases),'mismatches':0}))
if __name__=='__main__':main()
