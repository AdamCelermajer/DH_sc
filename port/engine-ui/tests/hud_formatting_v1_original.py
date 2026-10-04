"""Execute complete original parseEx with declared constants/libc dependencies.

The source formatter and UTF transform execute. Numeric/localization defaults
are explicit inputs here; genuine owned cache replay is a separate composition.
"""
import hashlib,json,math,re,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from localization_transform_probe import Cpu as BaseCpu,ROOT,REPO

def bits(x):
 try:return struct.unpack('<I',struct.pack('<f',x))[0]
 except OverflowError:return 0x7f800000 if x>0 else 0xff800000
def floating(x):return struct.unpack('<f',struct.pack('<I',x&0xffffffff))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
def words(*x):return struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
class Cpu(BaseCpu):
 def external(self,uc,address,size,user):
  n=self.imports.get(address)
  if n=='__aeabi_f2d':
   lo,hi=struct.unpack('<II',struct.pack('<d',floating(self.reg(0))));self.put(0,lo);self.put(1,hi)
  elif n=='__aeabi_f2iz':
   x=floating(self.reg(0));self.put(0,0 if math.isnan(x) else max(-2147483648,min(2147483647,int(x))) if math.isfinite(x) else 2147483647 if x>0 else -2147483648)
  elif n in ('__aeabi_fadd','__aeabi_fsub','__aeabi_fmul','__aeabi_fdiv'):
   a,b=floating(self.reg(0)),floating(self.reg(1));v=a+b if n.endswith('add') else a-b if n.endswith('sub') else a*b if n.endswith('mul') else a/b if b else math.nan if not a else math.copysign(math.inf,a*math.copysign(1,b));self.put(0,bits(v))
  elif n=='__aeabi_fcmplt':self.put(0,floating(self.reg(0))<floating(self.reg(1)))
  elif n=='modff':
   frac,whole=math.modf(floating(self.reg(0)));uc.mem_write(self.reg(1),words(bits(whole)));self.put(0,bits(frac))
  elif n=='snprintf':
   fmt=self.string(self.reg(2));args=[self.reg(3)]+list(struct.unpack('<8I',uc.mem_read(uc.reg_read(self.sp),32)))
   if fmt in (b'%.1f',b'%.2f'):
    # ARM AAPCS aligns the double vararg onto the stack (r3 skipped).
    value=struct.unpack('<d',words(args[1],args[2]))[0];out=(fmt.decode()%value).encode()
   elif fmt==b'%c':out=bytes([args[0]&255])
   elif fmt==b'%d':out=str(signed(args[0])).encode()
   elif fmt==b'%d%s%03d':out=(str(signed(args[0])).encode()+self.string(args[1])+('%03d'%signed(args[2])).encode())
   elif fmt==b'%d%s%03d%s%03d':out=(str(signed(args[0])).encode()+self.string(args[1])+('%03d'%signed(args[2])).encode()+self.string(args[3])+('%03d'%signed(args[4])).encode())
   elif b'%' not in fmt:out=fmt
   else:raise AssertionError(('Unsupported source libc format',fmt))
   cap=self.reg(1)
   if cap:uc.mem_write(self.reg(0),out[:cap-1]+b'\0')
   self.put(0,len(out))
  else:return super().external(uc,address,size,user)
  self.import_calls[n]=self.import_calls.get(n,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

class Original:
 def __init__(self):
  self.c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});c=self.c;c.pointer(0x99f698,1);d=c.data
  self.inp=d+0x1000;self.out=d+0x10000;self.manager=d+0x20000;self.args=d+0x21000;self.vec=d+0x22000;self.texts=d+0x30000;self.app=d+0x50000
  # Actual formatter application singleton GOT: original literal PC-relative.
  c.pointer(0x994a98+struct.unpack('<I',c.uc.mem_read(0x50a430,4))[0],self.app)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,address,size,user):
  c=self.c
  if address==0x4c4bdc:
   key=c.string(c.reg(2)).decode();self.trace.append(['constant',c.string(c.reg(1)).decode(),key]);self.ret(['GLOBAL_DECIMAL_SEPERATOR','GLOBAL_THOUSANDS_SEPERATOR','GLOBAL_THOUSANDS_GROUP_AT'].index(key))
  elif address==0x508edc:
   i=c.reg(1);self.trace.append(['string',i]);self.ret(self.texts+i*256)
  elif address in (0x31f6b0,0x3206d0):
   self.trace.append(['version' if address==0x31f6b0 else 'title',c.reg(2),c.reg(3) if address==0x31f6b0 else 0]);uc.mem_write(c.reg(1),(b'1.0.2' if address==0x31f6b0 else b'Dungeon Hunter 2')+b'\0');self.ret()
  elif address==0x50750c:self.trace.append(['pack',signed(struct.unpack('<I',uc.mem_read(self.manager+4,4))[0])])
 def run(self,text,values=(),prefix=b'',pack=-1,decimal=b'.',separator=b',',group=b'1000'):
  c=self.c;self.trace=[];c.pointer(self.manager+4,pack&0xffffffff)
  for i,s in enumerate((decimal,separator,group)):c.uc.mem_write(self.texts+i*256,s+b'\0')
  c.uc.mem_write(self.inp,text+b'\0');c.uc.mem_write(self.inp+0x8000,prefix+b'\0');c.invoke(0x3140ec,[self.out,self.inp+0x8000,0])
  for i,v in enumerate(values):
   if isinstance(v,bytes):p=self.texts+0x1000+i*1024;c.uc.mem_write(p,v+b'\0');raw=words(0,0,p)
   elif v is None:raw=words(0,0,0)
   else:raw=words(bits(float(v)),int(v) if math.isfinite(v) else 0,0)
   c.uc.mem_write(self.vec+i*12,raw)
  c.uc.mem_write(self.args,words(0,self.vec,self.vec+len(values)*12,self.vec+len(values)*12))
  returned=c.invoke(0x509aec,[self.manager,self.out,self.inp,self.args],budget=10000000)
  begin,end=struct.unpack('<2I',c.uc.mem_read(self.out+16,8));result=bytes(c.uc.mem_read(end,begin-end));c.invoke(0x3139ac,[self.out]);return returned,result,list(self.trace)

def main():
 o=Original();rows=[]
 for directive in range(32,127):
  text=b'a^'+bytes([directive])+b'z';r,out,trace=o.run(text,[1234.567,b'arg'])
  rows.append(dict(input=text.hex(),values=[1234.567,'arg'],returned=r,output=out.hex(),trace=trace))
 for code in b'dfghikmp':
  for v in [0.,-0.,.001,-.001,.005,-.005,.05,-.05,.099,-.099,1.234,-1.234,999.999,-999.999,1000.,-1000.,999999.,1000000.,123456789.]:
   text=b'^'+bytes([code]);r,out,trace=o.run(text,[v]);rows.append(dict(input=text.hex(),values=[v],returned=r,output=out.hex(),trace=trace))
 for pack in [-1,0,4,5,6,7]:
  for text in [b'',b'plain',b'|',b'^n',b'^s:^s:^s',b'^f / ^d',b'^v ^t',b'one ! two ? end',b'^unknown']:
   r,out,trace=o.run(text,[b'x',None,b'y'] if text.startswith(b'^s') else [1234.56,77],b'prefix:',pack);rows.append(dict(input=text.hex(),prefix='prefix:',pack=pack,returned=r,output=out.hex(),trace=trace))
 p=ROOT/'reference/hud-formatting-v1/parse-original-probes.json';p.write_text(json.dumps(dict(validation='PASS',original_sha256=hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),original_instructions_executed=True,explicit_services=['three StrID defaults','libc snprintf/modff/AEABI','Application title/version'],cases=rows),indent=2)+'\n');print(json.dumps(dict(validation='PASS',cases=len(rows))));print([(bytes.fromhex(x['input']),bytes.fromhex(x['output']))for x in rows if x['input'].startswith('5e')][:35])
if __name__=='__main__':main()
