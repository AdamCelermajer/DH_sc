"""Whole original menu entry wrappers + stack versus native O2 action composition.
AS to_xstring and singleton acquisition are explicit synchronous services.
"""
import argparse, hashlib, json, random, struct, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(Path(__file__).parent))
import menu_stack_v1_differential as stack
words=stack.words
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Actions(stack.Machine):
 def __init__(self,path,native):
  super().__init__(path,native);d=self.c.data;self.acall=d+0x1b000;self.avalue=d+0x1b100;self.aservices=d+0x1b200
  self.boundaries=[d+0x1b300,d+0x1b310];self.atrampoline=d+0x1b320
  for p in self.boundaries+[self.atrampoline]:self.c.uc.mem_write(p,words([0xd65f03c0]if native else[0xe12fff1e]))
  self.mode=0;self.boundary_nested=False;self.nested_boundary_done=False
 def boundary(self,op):
  self.emit(op,value=self.rawtype if op==21 else 0)
  if self.mode==op-19:self.c.uc.mem_write(self.r[0]+(8 if self.native else 0xf8),words([0x41]))
  if self.boundary_nested and not self.nested_boundary_done:
   self.nested_boundary_done=True;c=self.c;self.saved_lr=c.uc.reg_read(c.lr)
   self.saved_return=0 if self.native else self.s+0x400 if op==20 else self.names[self.arg]
   c.put(0,self.s if self.native else self.s+0x400);c.put(1,self.m[4])
   if self.native:c.put(2,self.sv)
   c.uc.reg_write(c.lr,self.atrampoline);c.uc.reg_write(c.pc,c.symbols['dh2_menu_stack_manager_push_v1']if self.native else 0x4317e8);return True
  return False
 def hook(self,uc,a,z,u):
  c=self.c
  if a==self.atrampoline:c.put(0,self.saved_return);c.uc.reg_write(c.lr,self.saved_lr);self.ret(self.saved_return);return
  if self.native and a in self.boundaries:
   op=20+self.boundaries.index(a);assert c.reg(0)==0xfedcba0987654321
   if op==20:c.pointer(c.reg(1),self.s)
   else:
    identity,tag,reserved=struct.unpack('<QII',bytes(uc.mem_read(c.reg(1),16)));assert identity==0xf123456789abcdef and tag==self.rawtype and not reserved
    c.pointer(c.reg(2),self.names[self.arg])
   if self.boundary(op):return
   self.ret();return
  if not self.native:
   if a==0x42ca8c and c.uc.reg_read(c.lr) in (0x43b1d8,0x43b164,0x43ac70,0x439de0):
    if self.boundary(20):return
    self.ret(self.s+0x400);return
   if a==0x796f5c:
    assert self.rawtype==uc.mem_read(c.reg(0)+1,1)[0]
    if self.boundary(21):return
    self.ret(self.names[self.arg]);return
  super().hook(uc,a,z,u)
 def action(self,op,count,tag,mode,nested,arg,seq,flags,globals,pf):
  self.kind=6+op;self.arg=arg;self.rawtype=tag;self.mode=mode;self.boundary_nested=nested;self.nested_boundary_done=False;self.nested=False
  self.setup(seq,flags,globals,pf,False);c=self.c;self.query_result=0
  if self.native:
   c.uc.mem_write(self.avalue,struct.pack('<QII',0xf123456789abcdef,tag,0))
   c.uc.mem_write(self.acall,struct.pack('<QII',self.avalue,count,0))
   c.uc.mem_write(self.aservices,struct.pack('<QQQ',0xfedcba0987654321,*self.boundaries))
   rc=c.invoke('dh2_menu_stack_action_v1',[op,self.acall,self.aservices,self.sv]);assert rc==0
  else:
   fn=self.s+0x600;header=self.s+0x640;values=self.s+0x680;result=self.s+0x700
   c.uc.mem_write(fn,bytes(32));c.pointer(fn,result);c.pointer(fn+12,header);c.pointer(header,values);c.uc.mem_write(fn+16,words([count]));c.uc.mem_write(values,bytes([0,tag])+bytes(34));sentinel=bytes.fromhex('fa030b001122334455667788');c.uc.mem_write(result,sentinel)
   c.invoke([0x43b1b4,0x43b158,0x43ac28,0x439dd8][op],[fn]);assert bytes(c.uc.mem_read(result,12))==sentinel
  return self.snapshot(),self.logs
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);ap.add_argument('--gold',type=Path,required=True);ap.add_argument('--report',type=Path,required=True);a=ap.parse_args()
 old=Actions(ROOT/'.local-inputs/libDungeonHunter2.so',False);new=Actions(a.library,True);rng=random.Random(43_158);records=[];services=0;order={};nested=0;ignored=0
 for op in range(4):
  for count in ((1,2,3)if op==0 else(0,1,2)):
   for tag in (0,1,2,3,4,5,6,255):
    for variant in range(10):
     arg=rng.randrange(6);seq=[rng.randrange(6)for _ in range(rng.randrange(6))];flags=[rng.choice((0,1,8,9,0x40,0x41,0x48,0x49))for _ in range(3)];g=[rng.randrange(20),rng.randrange(2),rng.randrange(2),0,0,0,0];pf=variant%2;mode=variant%3;nest=variant==9
     stack.NAMES=stack.POOL[:6]
     expected,el=old.action(op,count,tag,mode,nest,arg,seq,flags,g,pf);actual,al=new.action(op,count,tag,mode,nest,arg,seq,flags,g,pf)
     assert expected==actual,(op,count,tag,variant,'state');assert el==al,(op,count,tag,variant,'ordered service',[(x[:6])for x in el],[(x[:6])for x in al])
     boundary_order=','.join(str(x[0])for x in el if x[0]>=20);order[boundary_order]=order.get(boundary_order,0)+1;nested+=old.nested_boundary_done;ignored+=not el
     fixture=words([6+op,arg,0,pf,0,0,*range(6),len(seq),*seq,*flags,*g]);inp=words([op,count,tag,mode,int(nest)])+fixture
     record=words([len(inp),len(expected),len(el)])+inp+expected+b''.join(words([*x[:4],x[5],len(x[4].encode()),len(x[6])])+x[4].encode()+x[6]for x in el);records.append(record);services+=len(el)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words([0x3141534d,len(records)])+b''.join(records))
 source=['port/engine-ui/menu_stack_actions_v1.hpp','port/engine-ui/menu_stack_actions_v1.cpp','port/engine-ui/menu_stack_v1.hpp','port/engine-ui/menu_stack_v1.cpp','port/engine-ui/menu_stack_owner_v1.hpp','port/engine-ui/menu_stack_owner_v1.cpp','port/engine-ui/tests/menu_stack_v1_differential.py','port/engine-ui/tests/menu_stack_actions_v1_differential.py','port/engine-ui/character_menu_queries_owner_v1.hpp']
 report={'validation':'PASS','comparisons':len(records),'ordered_service_comparisons':services,'boundary_order_counts':order,'actual_boundary_nested_pushes':nested,'source_ignored_signatures':ignored,'source_result_preservation':len(records),'mismatches':0,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'source_sha256':{p:sha(ROOT/p)for p in source},'scope':__doc__,'as_conversion_algorithm_reconstructed':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
