"""Execute source CharAI owner association and verify every unchanged byte."""
import argparse,hashlib,json,struct,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/level-world'
MANIFEST=MODULE/'reference/character-ai-association/original-functions.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',required=True);p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-association-host')
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);exe=out/'host.exe'
    sources=[MODULE/'character_ai_association.cpp',MODULE/'character_ai_association.hpp',MODULE/'character_ai_initialization.hpp',MODULE/'tests/character_ai_association.cpp']
    cmd=[a.compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',str(sources[0]),str(sources[3]),'-o',str(exe)]
    subprocess.run(cmd,check=True);host=json.loads(subprocess.check_output([str(exe)],text=True))
    assert host['validation']=='PASS';m=json.loads(MANIFEST.read_text(encoding='utf-8'))
    assert sha(a.original_elf)==m['original_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    with a.original_elf.open('rb') as stream:
        elf=ELFFile(stream);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};raw=a.original_elf.read_bytes()
        def read(at,n):
            segment=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=segment['p_offset']+at-segment['p_vaddr'];return raw[offset:offset+n]
        f=m['functions'][0];at=int(f['elf_address'],0);symbol=symbols[f['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(at,f['size'])
        assert hashlib.sha256(read(at,f['size'])).hexdigest()==f['sha256']
        for caller in m['constructor_call_sites']:
            callsite=int(caller['callsite'],0);instruction=struct.unpack('<I',read(callsite,4))[0]
            assert instruction>>24==0xeb and hashlib.sha256(read(callsite,4)).hexdigest()==caller['callsite_sha256']
            offset=instruction&0xffffff
            if offset&0x800000:offset-=0x1000000
            assert callsite+8+offset*4==at
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    old=Cpu(a.original_elf,False,m);old.uc.mem_map(0x10000000,0x30000);ai=0x10010000;cases=[]
    for fill in (0,17,90,165,238,255):
        for owner in (1,0x10020000,0x7fffffff,0xffffffff):
            before=bytes([fill])*0xd4;old.uc.mem_write(ai,before);old.invoke(at,[ai,owner])
            after=bytes(old.uc.mem_read(ai,len(before)))
            assert after==before[:4]+struct.pack('<I',owner)+before[8:]
            expected={'status':0,'owner':owner,'other_fields_unchanged':True}
            actual=json.loads(subprocess.check_output([str(exe),str(fill),str(owner)],text=True))
            assert actual==expected
            cases.append({'initial_fill':fill,'owner':owner,'matched':True})
    paths=sources+[Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_cases':len(cases),'mismatches':0,
        'compiler_command':cmd,'cases':cases,'source_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in paths},
        'original_sha256':m['original_sha256'],'native_wired':False,
        'scope':'Actual148B CharAI::SetCharacter executes valid nonnull branch; all bytes except owner04 retained. Source Character constructor BL callsites verified; whole Character constructors and null diagnostics not reconstructed. Native identities above4GiB pass host contract.'}
    (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':len(cases),'mismatches':0}))
if __name__=='__main__':main()
