"""Original ARM32 node matrix instructions versus compiled ARM64 port.

Unicorn and the original ELF are validation inputs only, never APK contents.
External libc and soft-float helpers are modeled; unknown calls fail.
This validates local TRS matrices, not full scene or GPU equivalence.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn.arm64_const import UC_ARM64_REG_TPIDR_EL0
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu
def pack(v):return struct.pack('<'+'f'*len(v),*v)
def f(raw):return struct.unpack('<f',struct.pack('<I',raw&0xffffffff))[0]
class Cpu(BaseCpu):
 def __init__(self,path,arm64,manifest):
  super().__init__(path,arm64,manifest)
  if arm64:
   self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0,self.data+0x8000)
   self.uc.mem_write(self.data+0x8028,struct.pack('<Q',0xD22026))
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('__aeabi_fadd','__aeabi_fsub','__aeabi_fmul','__aeabi_fcmpeq'):
   a,b=f(self.reg(0)),f(self.reg(1))
   if name.endswith('fcmpeq'):self.put(0,int(a==b))
   else:
    value=a+b if name.endswith('fadd') else a-b if name.endswith('fsub') else a*b
    self.put(0,struct.unpack('<I',pack([value]))[0])
   self.import_calls[name]=self.import_calls.get(name,0)+1;self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
 p.add_argument('--report',type=Path,required=True);p.add_argument('fixtures',type=Path,nargs='+');a=p.parse_args()
 manifest=json.loads((ROOT/'original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});cases=[]
 for file in a.fixtures:
  b=file.read_bytes()
  def w(p):return struct.unpack_from('<I',b,p)[0]
  def node(p):
   cases.append((list(struct.unpack_from('<3f',b,p+12)),list(struct.unpack_from('<4f',b,p+24)),list(struct.unpack_from('<3f',b,p+40))))
   for i in range(w(p+56)):node(w(p+60)+80*i)
  r=w(32)
  for i in range(w(r+152)):
   vs=w(r+156)+16*i
   for j in range(w(vs+8)):node(w(vs+12)+80*j)
 fixture_count=len(cases);rng=random.Random(22026)
 for i in range(200):
  q=[rng.uniform(-1,1) for _ in range(4)];n=math.sqrt(sum(x*x for x in q));q=[x/n for x in q]
  cases.append(([rng.uniform(-10000,10000) for _ in range(3)],q,[rng.uniform(-4,4) for _ in range(3)] if i%2 else [1,1,1]))
 for t,q,s in cases:
  obj=old.data+0x1000;old.uc.mem_write(obj,b'\0'*0x200)
  old.uc.mem_write(obj+0xac,pack(t));old.uc.mem_write(obj+0xb8,pack(q));old.uc.mem_write(obj+0xc8,pack(s));old.uc.mem_write(obj+0x11c,struct.pack('<I',14))
  out=old.invoke(0x598908,[obj]);assert out==obj+0x68
  dst=new.data+0x1000;tp=new.data+0x2000;qp=new.data+0x3000;sp=new.data+0x4000
  for ptr,value in ((tp,t),(qp,q),(sp,s)):new.uc.mem_write(ptr,pack(value))
  new.invoke('dh2_node_matrix',[dst,tp,qp,sp])
  actual=bytes(new.uc.mem_read(dst,64));expected=bytes(old.uc.mem_read(out,64))
  assert actual==expected,(t,q,s,struct.unpack('<16f',expected),struct.unpack('<16f',actual))
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),
  'original_address':'0x598908','fixture_matrices':fixture_count,'synthetic_matrices':200,'bit_exact_matches':len(cases),
  'scope':'local node TRS only; imported libc and soft-float modeled; no full-scene or GPU equivalence'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
