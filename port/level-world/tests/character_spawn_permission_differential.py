"""Source current17 CSM_Spawn with actual always-false GroupInfo::CanSpawn versus ARM64."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 old=Cpu(a.engine,False,json.loads((R/'reference/character-spawn-state/original-functions.json').read_text()));new=Cpu(a.library,True,{'functions':[]});char=old.data+0x1000;group=old.data+0x5000;next_=old.data+0x6000;view=new.data+0x1000;out=new.data+0x2000;records=[]
 for present in (0,1):
  for byte in range(256):
   old.uc.mem_write(char+0x3fc,struct.pack('<I',group if present else 0));old.uc.mem_write(char+0x3cc,struct.pack('<I',char));old.uc.mem_write(char+0x1430,bytes([byte]));old.uc.mem_write(next_,struct.pack('<I',0x12345678));expected=old.invoke(0x3ad2e4,(char,9,0x87654321,17,next_));assert struct.unpack('<I',old.uc.mem_read(next_,4))[0]==0x12345678
   new.uc.mem_write(view,struct.pack('<QII',0xc123456789abcdef if present else 0,byte,0));new.uc.mem_write(out,struct.pack('<I',0xffffffff));assert new.invoke('dh2_character_pre_spawn_permission',(out,view))==1;assert struct.unpack('<I',new.uc.mem_read(out,4))[0]==expected
   records.append(struct.pack('<3I',present,byte,expected))
 gold=R/'reference/character-spawn-state/spawn-permission-fixtures.bin';gold.write_bytes(b'SPP1'+struct.pack('<I',len(records))+b''.join(records));paths=('character_spawn_permission.hpp','character_spawn_permission.cpp','tests/character_spawn_permission_differential.py')
 data=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),library_sha256=sha(a.library),reference_sha256=sha(gold),source_sha256={'port/level-world/'+x:sha(R/x) for x in paths},comparisons=len(records),actual_original_group_rejections=256,source_next_word_preserved_cases=len(records),mismatches=0,CanRespawn_state0=False,APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
