"""Execute actual Application getter; capture original GSLevel publication/destruction, not whole lifecycle."""
import pathlib,sys,hashlib,struct,json,subprocess
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sys.path.insert(0,str(root.parent/'dependencies'));sys.path.insert(0,str(root/'port/engine-math/tests'))
from differential import Cpu
from elftools.elf.elffile import ELFFile
elf=pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Imports:
    def call(self,c,name):raise AssertionError('unmodeled import '+name)
c=Cpu(elf,False,Imports(),{'functions':[]})
symbol='_ZNK11Application15GetCurrentLevelEv';assert c.symbols[symbol]==0x31f594
global_symbol='_ZN7GSLevel7s_levelE';assert c.symbols[global_symbol]==0x9a2638
got=0x9967fc;original_got=struct.unpack('<I',c.uc.mem_read(got,4))[0]
with elf.open('rb') as stream:
    image=ELFFile(stream);relocations=[]
    for section in image.iter_sections():
        if section['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
        symbols=image.get_section(section['sh_link'])
        for relocation in section.iter_relocations():
            if relocation['r_offset']==got:
                relocations.append((relocation['r_info_type'],symbols.get_symbol(relocation['r_info_sym']).name))
    assert relocations==[(23,'')],relocations # R_ARM_RELATIVE
# At the oracle's ELF base zero, the original RELATIVE addend already is the
# exact global symbol address. Read the unmodified GOT cell; no override.
assert c.base==0 and original_got==c.symbols[global_symbol]
rows=[]
for level in (0,c.data+0x1000,c.data+0x5000):
    for application in (0,0xdeadbeef):
        c.uc.mem_write(c.symbols[global_symbol],struct.pack('<I',level))
        result=c.invoke(symbol,(application,))
        assert result==level,(result,level)
        rows.append({'application_argument':application,'s_level':level,'returned_level':result})
objdump=pathlib.Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-objdump.exe')
parts=[]
for start,stop in ((0x31f594,0x31f5b4),(0x3860b0,0x386144),(0x386190,0x38621c)):
    parts.append(subprocess.check_output([str(objdump),'--disassemble','--demangle',f'--start-address={start:#x}',f'--stop-address={stop:#x}',str(elf)]))
out=pathlib.Path(__file__).parent;asm=b'\n'.join(parts);(out/'gslevel-current-original.asm').write_bytes(asm)
report={'validation':'PASS','scope':__doc__,'engine_sha256':sha(elf),'symbol':symbol,'entry':'0x31f594','global_symbol':global_symbol,'global_address':'0x9a2638','got_address':'0x9967fc',
    'loader_relocation':{'kind':'original R_ARM_RELATIVE addend at ELF base zero; unmodified GOT','elf_type':23,'target_symbol_verified':global_symbol,'original_cell':original_got,'got_overridden':False},'cases':rows,
    'publication_capture':{'level_constructor_call':'0x386200','gslevel_field34_store':'0x386214','s_level_store':'0x386218'},
    'destruction_capture':{'virtual_level_destruction':'0x386118','field34_clear':'0x386120','s_level_clear':'0x386130'},
    'capture_sha256':hashlib.sha256(asm).hexdigest(),'oracle_sha256':sha(pathlib.Path(__file__)),'cpu_helper_sha256':sha(pathlib.Path(sys.modules['differential'].__file__)),
    'gslevel_constructor_executed':False,'gslevel_destructor_executed':False,'full_level_constructor_verified':False,'gameplay_verified':False}
(out/'gslevel-current-original.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'validation':'PASS','getter_cases':len(rows),'global_address':report['global_address']}))
