"""Complete original CharacterFX registration and Grab invalid-ID guards.
Original recursive preload executes. Debug/file/string ownership is an explicit
synchronous service fixture. No enabled valid Grab factory is accepted.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from visual_fx_preload_differential import Audit as Base,words,sha
class Audit(Base):
 def __init__(self,path,native,manifest):
  super().__init__(path,native,manifest);d=self.c.data;self.char=d+0x9000;self.rows=d+0xb000;self.view=d+0xc000;self.out=d+0xd000
 def hook(self,uc,address,size,unused):
  # Original singleton storage is a required caller identity projection. Its
  # callee runs real instructions against the fixture manager/vector backing.
  if not self.native and address==0x4967e8:self.c.put(0,self.manager)
  return super().hook(uc,address,size,unused)
 def event(self,op,name):
  super().event(op,name)
  if self.x.get('row_mutation') and len(self.trace)==1:
   # All three getter results must remain captured through this callback.
   self.write_rows([[15,15,15,15],[15,15,15,15],[15,15,15,15]])
 def write_rows(self,rows):
  c=self.c
  for i,row in enumerate(rows):c.uc.mem_write(self.rows+(20 if self.native else 24)*i,struct.pack('<4iB3x',*row,0)if self.native else words([0,*row,0]))
 def fixture(self,x):
  super().fixture(x);c=self.c;self.write_rows(x['rows']);c.uc.mem_write(self.out,struct.pack('<Q',123))
  if self.native:c.uc.mem_write(self.view,struct.pack('<QIi',self.rows,3,x['index']))
  else:
   c.uc.mem_write(self.char,bytes(0x1800));c.uc.mem_write(self.char+0x1014,words([x['index']]))
   for key,value in [('size',3),('members',self.rows)]:
    name=next(n for n in c.symbols if 'CharEffectTable' in n and n.endswith(str(len(key))+key+'E'));c.uc.mem_write(c.symbols[name],words([value]))
 def run(self,id=None,effect=None):
  c=self.c
  if self.x['kind']==0:
   if self.native:return c.invoke('dh2_character_init_fx_register',[self.view,self.table,self.manager,self.services])
   c.invoke(0x3b4738,[self.char]);return 1
  if self.native:return c.invoke('dh2_character_init_fx_negative_grab',[self.out,self.x['id']&0xffffffff,3,self.services])
  result=c.invoke(0x495430,[self.manager,self.x['id']&0xffffffff,self.char]);assert result==0;return 1
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-init-fx';ref.mkdir(exist_ok=True);m=json.loads((ref/'original-functions.json').read_text());old=Audit(REPO/'.local-inputs/libDungeonHunter2.so',False,m);new=Audit(a.library,True,{'functions':[]});rng=random.Random(495430);records=[]
 for i in range(480):
  kind=int(i>=360);module=int(i%7!=0);id=rng.choice([-1,-2147483648,3,4,2147483647])if module else rng.choice([-1,0,1,3]);x=dict(kind=kind,id=id,effect=0,index=rng.choice([-1,0,1,2,3,2147483647]),rows=[[rng.choice([-1,0,1,2,3,9]),rng.choice([-1,0,1,2,3,9]),rng.choice([-1,0,1,2,3,9]),-1]for _ in range(3)],sets=[[[7,0]],[[1,0],[2,0]],[]],initial=[],module=module,mutate=-1,reentry=-1,row_mutation=int(i%5==0))
  old.fixture(x);new.fixture(x);
  try: a0=old.run()
  except Exception: print(i,x,old.trace);raise
  assert a0==new.run()==1;assert old.trace==new.trace and old.output()==new.output(),(i,x,old.trace,new.trace,old.output(),new.output())
  if kind:assert struct.unpack('<Q',new.c.uc.mem_read(new.out,8))[0]==0
  records.append(dict(config=x,trace=old.trace,output=old.output()))
 gold=ref/'init-fx-fixtures.json';gold.write_text(json.dumps({'records':records},separators=(',',':'))+'\n');b=bytearray(b'IFX1'+words([len(records)]))
 for r in records:
  x=r['config'];b+=words([x['kind'],x['id'],x['index'],x['module'],x['row_mutation'],len(r['trace']),len(r['output'])]);b+=b''.join(struct.pack('<4i',*row)for row in x['rows'])
  for op,name in r['trace']:s=name.encode();b+=words([op,len(s)])+s
  b+=words(r['output'])
 (ref/'init-fx-fixtures.bin').write_bytes(b)
 report=dict(validation='PASS',scope=__doc__,original_sha256=m['original_sha256'],manifest_sha256=sha(ref/'original-functions.json'),library_sha256=sha(a.library),source_sha256={v:sha(REPO/v)for v in ['port/game-data/effects_tables.hpp','port/level-world/character_init_fx.hpp','port/level-world/character_init_fx.cpp','port/level-world/visual_fx_preload.hpp','port/level-world/visual_fx_preload.cpp','port/level-world/tests/character_init_fx_differential.py']},comparisons=len(records),register_cases=360,negative_grab_cases=120,captured_rows_mutation_cases=72,ordered_services=sum(len(r['trace'])for r in records),mismatches=0,gold_sha256=sha(gold),binary_gold_sha256=sha(ref/'init-fx-fixtures.bin'))
 (ROOT/'reports/character-init-fx-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
