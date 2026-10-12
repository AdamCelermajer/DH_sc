"""Original stateless state-behavior singleton factories; no global constructor/vtable initialization or per-machine map allocation claim."""
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
engine=REPO/'.local-inputs/libDungeonHunter2.so';c=Cpu(engine,False,{'functions':[]})
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
with engine.open('rb') as f:symbols=list(ELFFile(f).get_section_by_name('.symtab').iter_symbols())
rows=[]
for name,address,lit0,lit1,pcadd in [('Scared',0x3c0534,0x3c0548,0x3c054c,0x3c053c),('Stunned',0x3c0550,0x3c0564,0x3c0568,0x3c0558)]:
 w=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
 got=(pcadd+8+w(lit0))&0xffffffff;slot=got+w(lit1);value=w(slot);assert c.invoke(address,[])==value==c.invoke(address,[])
 rows.append(dict(kind=name,factory=hex(address),GOT_slot=hex(slot),shared_object=hex(value),symbols=[s.name for s in symbols if s['st_value']==value],raw_object_word=hex(w(value))))
result=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),probe_sha256=sha(Path(__file__)),original_factory_instructions_executed=True,repeat_returns_identical=True,rows=rows);(HERE/'behavior-ownership.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',shared_behaviors=len(rows))))
