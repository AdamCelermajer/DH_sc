"""Actual Savegame::_cacheFile false-flag section index and original STL.
Wrapper stream copy/size/seek/tell and storage are explicit caller services;
file open/backup/corruption replacement remain outside this valid-file corpus.
"""
import json,hashlib,random,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,ROOT,W
from unicorn import UC_HOOK_CODE
class Index(Save):
 def __init__(self):
  super().__init__();self.proxy=0;self.file=self.data+0x10000;self.calls=[];self.cursor=0
  self.pointer(self.vt+4,self.callback+80);self.pointer(self.vt+8,self.callback+96);self.pointer(self.vt+0x20,self.callback+112);self.pointer(self.vt+0x24,self.callback+128);self.pointer(self.vt+0x2c,self.callback+144)
  self.uc.hook_add(UC_HOOK_CODE,self.index_service)
 def index_service(self,uc,a,z,u):
  if a==0x3172d8:self.proxy=self.reg(0);assert self.reg(1)==self.stream;self.pointer(self.proxy,self.vt);self.returned(self.proxy)
 def external(self,uc,a,z,u):
  if a==self.callback+16:
   n=self.reg(2);assert self.reg(0) in (self.stream,self.proxy) and self.cursor+n<=len(self.blob)
   uc.mem_write(self.reg(1),self.blob[self.cursor:self.cursor+n]);self.cursor+=n;self.put(1,0);self.returned(n)
  elif a==self.callback+80:self.returned()
  elif a==self.callback+96:self.put(1,0);self.returned(len(self.blob))
  elif a==self.callback+112:assert self.reg(3)==0 and self.reg(2)<=len(self.blob);self.cursor=self.reg(2);self.put(1,0);self.returned()
  elif a==self.callback+128:self.put(1,0);self.returned(self.cursor)
  elif a==self.callback+144:self.returned()
  else:super().external(uc,a,z,u)
 def run(self,b):
  self.blob=b;self.cursor=0;self.uc.mem_write(self.file,bytes(0x40));header=self.file+0x20;self.pointer(header+8,header);self.pointer(header+12,header)
  self.invoke(0x315ad0,[self.file,self.stream],budget=20000000)
  def walk(node):
   if not node:return []
   p=self.word(node+0x24);name=bytearray()
   while self.uc.mem_read(p+len(name),1)!=b'\0':name.extend(self.uc.mem_read(p+len(name),1))
   offset=self.word(node+0x28);high=self.word(node+0x2c);assert high==0
   return walk(self.word(node+8))+[(bytes(name),offset,self.word(node+0x30))]+walk(self.word(node+12))
  return walk(self.word(header+4))
def main():
 c=Index();r=random.Random(20261004);cases=[]
 for k in range(96):
  entries=[(r.choice([b'name',b'levl',b'clss',b'skil',b'faer',b'invt',b'x\0zz']),bytes(r.randrange(256) for _ in range(r.randrange(40)))) for _ in range(k%12)]
  blob=W(len(entries))+b''.join(W(len(payload))+tag+payload for tag,payload in entries);rows=c.run(blob);gold=W(len(rows))
  for name,offset,size in rows:gold+=W(len(name))+name+W(offset,size)
  cases.append(W(len(blob))+blob+W(len(gold))+gold)
 result=b'PIX1'+W(len(cases))+b''.join(cases);ref=ROOT/'port/game-data/reference/player-profile-index-v1';ref.mkdir(parents=True,exist_ok=True);(ref/'fixtures.bin').write_bytes(result)
 report={'validation':'PASS','comparisons':len(cases),'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(result).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False};(ref/'original-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
