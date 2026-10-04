"""Original named Player name/level/class section readers, actual class name
cache bytes, original string and stream instructions. Storage/stream services
are explicit; no creation default/name/class source is synthesized.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,ROOT,W,encoded
from items_differential import strings
class Metadata(Save):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='malloc':self.returned(self.allocate(self.reg(0)))
  elif self.imports.get(a)=='free':self.returned()
  else:super().external(uc,a,z,u)
 def __init__(self):
  super().__init__();self.classnames=strings((ROOT/'.local-inputs/combat-data/character_classes_pyarraynames.bin').read_bytes())[0];base=0x469da0+self.word(0x469e5c);table=self.allocate(4*len(self.classnames))
  for i,n in enumerate(self.classnames):p=self.allocate(len(n)+1);self.uc.mem_write(p,n+b'\0');self.pointer(table+4*i,p)
  self.pointer(self.word(base+self.word(0x469e64)),len(self.classnames));self.pointer(self.word(base+self.word(0x469e68)),table)
def main():
 c=Metadata();r=random.Random(20261004);cases=[]
 for k in range(128):
  c.fresh([0]);name=r.choice([b'',b'Prince',b'Player'+bytes([65+k%26]),b'prefix\0tail',b'X'*(k*3)]);raw=encoded(name);c.blob=raw;c.cursor=0;c.invoke(0x4698dc,[c.stream,c.obj]);p=c.word(c.obj+0x18+0x14);actual=bytes(c.uc.mem_read(p,len(name)));assert c.cursor==len(raw)
  value=r.choice([0,1,-1,2,100,0x7fffffff,-2147483648]);level=W(value);c.blob=level;c.cursor=0;c.invoke(0x4689a0,[c.stream,c.obj]);actuallevel=c.word(c.obj+0x30)
  key=r.choice(c.classnames+[b'absent',b'Knight\0ignored',b'']);cl=encoded(key);c.blob=cl;c.cursor=0;c.invoke(0x469d88,[c.stream,c.obj]);actualclass=c.word(c.obj+0x34)
  cases.append(W(len(raw))+raw+W(len(actual))+actual+level+W(actuallevel)+W(len(cl))+cl+W(actualclass))
 result=b'MSG1'+W(len(cases))+b''.join(cases);path=ROOT/'port/game-data/reference/player-savegame-v1';(path/'metadata-fixtures.bin').write_bytes(result)
 report={'validation':'PASS','original_cases':len(cases),'reader_comparisons':len(cases)*3,'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(result).hexdigest(),'class_names_sha256':hashlib.sha256((ROOT/'.local-inputs/combat-data/character_classes_pyarraynames.bin').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__};(path/'original-metadata-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()

