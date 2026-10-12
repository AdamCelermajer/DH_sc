"""Execute original Timer.GetID and bind both actual timer vtables.

The native Timer32.id projection is owned by the frozen native timer store;
this evidence establishes its selected original callable, not an original
Application/frame or a second timer implementation.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words
def main():
 original=ROOT/'.local-inputs/libDungeonHunter2.so';c=Cpu(original,False,{'functions':[]});p=c.data+4096
 rng=random.Random(20261006);rows=[]
 for identity in [0,1,0x7fffffff,0x80000000,0xffffffff]+[rng.getrandbits(32)for _ in range(251)]:
  c.uc.mem_write(p,words(0x966958,identity));got=c.invoke(0x3db288,[p]);assert got==identity;rows.append(identity)
 with original.open('rb')as f:
  e=ELFFile(f)
  def read(address):
   for segment in e.iter_segments():
    h=segment.header
    if h.p_type=='PT_LOAD'and h.p_vaddr<=address<h.p_vaddr+h.p_filesz:return struct.unpack_from('<I',segment.data(),address-h.p_vaddr)[0]
   raise AssertionError(address)
  vtables={hex(a):read(a+8)for a in (0x966940,0x966950)}
 assert vtables=={'0x966940':0x3db288,'0x966950':0},vtables
 report=dict(validation='PASS',original_cases=len(rows),original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),original_instructions_executed=True,get_id=0x3db288,original_field_offset=4,native_projection='Timer32.id at offset0 (native vtable omitted)',vtables=vtables,identities=rows,scope=__doc__)
 target=ROOT/'port/level-world/reference/character-skill-gameplay-v3/timer-id/probe.json';target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',original_cases=len(rows),vtables=vtables)))
if __name__=='__main__':main()
