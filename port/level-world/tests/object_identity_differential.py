"""Actual pointer-handle/GetHandle/deletion-list instructions vs optimized ARM64.

Allocation and memcpy are explicit storage services; no object manager Add,
Remove, whole Lua VM or borrowed pointer lifetime parity is claimed.
"""
import argparse,hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
from navigation_differential import Cpu
REF=ROOT/'reference/object-identity-lifecycle'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Oracle:
 def __init__(self,library):
  self.old=Original(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.new=Cpu(library,True,{'functions':[]})
  c=self.old;self.manager=c.data+0x8000;self.context=c.data+0x9000;self.owner=c.data+0xa000;self.shared=c.data+0xb000;self.out=c.data+0xc000
  got=0x33dd40+c.word(0x33dd68);c.pointer(got+c.word(0x33dd6c),self.context);c.pointer(self.context+0x38,self.manager)
  c.uc.hook_add(UC_HOOK_CODE,self.hook);self.allocations=0
 def hook(self,uc,address,size,unused):
  c=self.old
  if address==0x708ec0:
   assert c.word(c.reg(0))==12
   p=c.heap;c.heap+=16;uc.mem_write(p,bytes(12));self.allocations+=1;c.returned(p)
 def handle(self,row,native):
  present,key,cached,oldframe,frame=row;c=self.new if native else self.old
  if native:
   out,shared=c.data+0x1000,c.data+0x1100;c.uc.mem_write(shared,struct.pack('<iIQ',key,oldframe,cached));assert c.invoke('dh2_object_handle_from_pointer',[out,shared if present else 0,frame])==0
   result=list(struct.unpack('<iIQ',c.uc.mem_read(out,16)));after=list(struct.unpack('<iIQ',c.uc.mem_read(shared,16)))
  else:
   c.pointer(self.manager+0x78,frame);c.pointer(self.owner+0x2c,self.shared);c.uc.mem_write(self.shared,words(key,cached,oldframe));c.invoke(0x340c54,[self.out,self.manager,self.owner if present else 0])
   def proj(p):
    k,cache,f=struct.unpack('<3I',c.uc.mem_read(p,12));return [k if k<0x80000000 else k-0x100000000,f,cache]
   result=proj(self.out);after=proj(self.shared)
  return result+after
 def flags(self,row,native):
  op,disabled,delay,updating,value=row;c=self.new if native else self.old
  if native:
   p=c.data+0x1000;c.uc.mem_write(p,bytes([disabled,delay,updating,17,18,19,20,21]));assert c.invoke('dh2_object_mark_deleted' if op==2 else 'dh2_object_set_updating',[p,value])==0;return list(c.uc.mem_read(p,8))
  p=self.owner;c.uc.mem_write(p,bytes(0x100));c.uc.mem_write(p+0x81,bytes([disabled,delay]));c.uc.mem_write(p+0x85,bytes([updating]));c.uc.mem_write(p+0x90,bytes([17,18,19,20,21]));c.invoke(0x33ddb4 if op==2 else 0x33dcf0,[p,value]);return list(c.uc.mem_read(p+0x81,2))+[c.uc.mem_read(p+0x85,1)[0]]+list(c.uc.mem_read(p+0x90,5))
 def queue(self,values,identity,native):
  c=self.new if native else self.old
  if native:
   p,a=c.data+0x1000,c.data+0x1100;c.uc.mem_write(a,struct.pack('<'+'Q'*len(values),*values)+bytes(8));c.uc.mem_write(p,struct.pack('<QII',a,len(values),len(values)+1));assert c.invoke('dh2_object_queue_deletion',[p,identity])==0;count=struct.unpack('<I',c.uc.mem_read(p+8,4))[0];return list(struct.unpack('<'+'Q'*count,c.uc.mem_read(a,8*count)))
  c.heap=c.data+0x100000;sent=self.manager+0x3c;nodes=[self.manager+0x100+i*16 for i in range(len(values))]
  c.uc.mem_write(sent,words(nodes[0] if nodes else sent,nodes[-1] if nodes else sent))
  for i,(node,value) in enumerate(zip(nodes,values)):c.uc.mem_write(node,words(nodes[i+1] if i+1<len(nodes) else sent,nodes[i-1] if i else sent,value))
  c.invoke(0x3432f8,[self.manager,identity]);out=[];p=c.word(sent);previous=sent
  while p!=sent:
   assert c.word(p+4)==previous;out.append(c.word(p+8));previous=p;p=c.word(p)
  assert c.word(sent+4)==previous;return out
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();o=Oracle(a.library);records=[]
 for row in itertools.product((0,1),(-2147483648,-1,0,1,2147483647),(0,1,0xffffffff),(0,1,0xffffffff),(0,1,0xffffffff)):
  old=o.handle(row,False);assert old==o.handle(row,True),(row,old);records.append([1,list(row),old])
 handle_count=len(records)
 for row in itertools.product((2,3),(0,1,2,255),(0,1,2,255),(0,1,255),(0,1,2,255,256,0xffffffff)):
  old=o.flags(row,False);assert old==o.flags(row,True),(row,old);records.append([row[0],list(row),old])
 rng=random.Random(20261004)
 for i in range(320):
  values=[rng.randrange(0,20) for _ in range(i%33)];identity=rng.choice([0,0xffffffff,rng.randrange(20)]);old=o.queue(values,identity,False);assert old==o.queue(values,identity,True);records.append([4,[identity,*values],old])
 gold=REF/'identity-fixtures.json';gold.write_text(json.dumps({'format':'OIL1','records':records},separators=(',',':'))+'\n')
 # A fixed-width gold allows the sanitizer replay without a JSON dependency.
 binary=b'OIL1'+words(len(records))
 for op,args,out in records:binary+=words(op,len(args),len(out),*args,*out)
 (REF/'identity-fixtures.bin').write_bytes(binary)
 result={'validation':'PASS','comparisons':len(records),'handle_cases':handle_count,'flag_cases':len(records)-handle_count-320,'deletion_list_cases':320,'source_list_allocations':o.allocations,'mismatches':0,'original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'arm64_sha256':sha(a.library),'manifest_sha256':sha(REF/'original-functions.json'),'corpus_sha256':hashlib.sha256(binary).hexdigest(),'source_sha256':{str(x.relative_to(REPO)):sha(x) for x in [ROOT/'object_identity.hpp',ROOT/'object_identity.cpp',Path(__file__)]},'scope':__doc__}
 (ROOT/'reports/object-identity-arm64-differential.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
