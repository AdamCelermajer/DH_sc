"""Original cached GFNT instruction raster/metrics vs optimized native ARM64.
Imported compiler float32 operations/conversions are explicit IEEE services.
No font manager, FreeType, texture cache or GPU effect is modeled.
"""
import argparse,hashlib,json,math,pathlib,struct,sys
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_TPIDR_EL0
ROOT=pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu as Base
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def bits(x):
 try:return struct.unpack('<I',struct.pack('<f',x))[0]
 except OverflowError:return 0x7f800000 if x>0 else 0xff800000
def floating(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
class Cpu(Base):
 def __init__(self,*args):
  super().__init__(*args)
  if self.arm64:self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0,self.data+0x1ff0000)
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('__aeabi_i2f','__aeabi_ui2f'):self.put(0,bits(signed(self.reg(0)) if name=='__aeabi_i2f' else self.reg(0)))
  elif name in ('__aeabi_fdiv','__aeabi_fmul'):
   a,b=floating(self.reg(0)),floating(self.reg(1));v=a*b if name.endswith('mul') else a/b if b else math.nan if a==0 else math.copysign(math.inf,a*math.copysign(1,b));self.put(0,bits(v))
  elif name=='__aeabi_f2iz':
   v=floating(self.reg(0));self.put(0,0 if math.isnan(v) else max(-2147483648,min(2147483647,int(v))) if math.isfinite(v) else 2147483647 if v>0 else -2147483648)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=pathlib.Path,required=True);p.add_argument('--font',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'port/level-world/reference/font-text/glyph-backend/original-functions.json').read_text());old=Cpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Cpu(a.library,True,{'functions':[]});b=a.font.read_bytes();head=struct.unpack_from('>10I',b);count,w,h,first=head[3],head[4],head[5],head[9];offsets=struct.unpack_from('>'+str(count+1)+'I',b,40)
 for c in [old,new]:c.uc.mem_write(c.data+0x2000,b)
 obody=old.data+0x1000;omet=old.data+0x30000;od=old.data+0x30100;op=old.data+0x40000;mb=old.data+0x50000;old.pointer(obody+0x54,old.data+0x2000);old.pointer(obody+0x5c,mb);old.pointer(mb+8,old.data+0x2000);old.pointer(obody+0x2c,op);old.pointer(obody+0x30,w*h);old.pointer(obody+0x34,w*h)
 ni=new.data+0x1000;no=new.data+0x30000;np=new.data+0x40000;new.uc.mem_write(ni,struct.pack('<QII',new.data+0x2000,len(b),0));fixture=bytearray(struct.pack('<II',0x31544647,0));cases=0;present=0;scale=0
 def stop_scale(uc,address,size,u):
  if address==0x7c6a4c:uc.reg_write(old.pc,old.stop)
 old.uc.hook_add(UC_HOOK_CODE,stop_scale,begin=0x7c6a4c,end=0x7c6a4c);old.put(4,obody);old.invoke(0x7c69f4,[]);scale=struct.unpack('<I',old.uc.mem_read(obody+0x28,4))[0]
 def compare(code,height):
  nonlocal cases,present
  old.uc.mem_write(omet,b'\xa5'*20);old.uc.mem_write(od,b'\xa5'*16);new.uc.mem_write(no,b'\xa5'*32);new.uc.mem_write(np,b'\xa5'*(w*h*4));rc=old.invoke(0x7c4698,[obody,od,code,height&0xffffffff,omet]);nr=new.invoke('dh2_gfnt_raster',[no,np,w*h*4,ni,code,height&0xffffffff]);assert rc==nr,(code,height,rc,nr)
  if rc:
   metric=bytes(old.uc.mem_read(omet,20));native=bytes(new.uc.mem_read(no,32));assert native[:20]==metric,(code,height,metric.hex(),native.hex());assert struct.unpack_from('<III',native,20)==(w*4,scale,0);rgba=bytes(old.uc.mem_read(op,w*h*4));assert bytes(new.uc.mem_read(np,w*h*4))==rgba;present+=1
  else:metric=b'\xa5'*20;rgba=b'';assert bytes(new.uc.mem_read(no,32))==b'\xa5'*32
  fixture.extend(struct.pack('<IiI',code,height,rc));fixture.extend(metric);fixture.extend(struct.pack('<I',len(rgba)));fixture.extend(rgba);cases+=1
 for i in range(count):compare(first+i,19)
 for height in [-2147483648,-2048,-1,0,1,12,24,1024,2147483647]:
  for code in [32,33,48,65,97,first+count-1,first+count,0,65535]:compare(code,height)
 struct.pack_into('<I',fixture,4,cases);ref=ROOT/'port/engine-ui/reference/gfnt';ref.mkdir(parents=True,exist_ok=True);(ref/'original-fixtures.bin').write_bytes(fixture)
 report={'validation':'PASS','comparisons':cases,'present_glyphs':present,'mismatches':0,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'font_sha256':sha(a.font),'library_sha256':sha(a.library),'gold_sha256':sha(ref/'original-fixtures.bin'),'source_sha256':{str(x.relative_to(ROOT)).replace('\\','/'):sha(x) for x in [ROOT/'port/engine-ui/gfnt.hpp',ROOT/'port/engine-ui/gfnt.cpp',pathlib.Path(__file__)]},'original_import_services':old.import_calls,'scope':'Actual original cached glyph raster and complete Metrics20, plus original constructor font-scale arithmetic block, vs optimized ARM64 kernel. Original imported float32/compiler conversions modeled explicitly. All8451 slots plus81height/boundary cases. No source texture-cache allocation, TTF, layout, GPU or packaged parity.'};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','present_glyphs','mismatches']}))
if __name__=='__main__':main()
