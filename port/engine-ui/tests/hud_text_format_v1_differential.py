"""Complete original parseEx/UTF instructions versus compiled O2 ARM64 source.
Localized defaults, application version/title and libc/AEABI are declared services.
"""
import argparse,hashlib,json,math,random,struct,time
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_D0
from pathlib import Path
from hud_formatting_v1_original import Cpu,Original,ROOT,REPO,bits,floating,words

class NativeCpu(Cpu):
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='modff':
   frac,whole=math.modf(floating(uc.reg_read(UC_ARM64_REG_S0)));uc.mem_write(self.reg(0),words(bits(whole)));uc.reg_write(UC_ARM64_REG_S0,bits(frac));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(address) in ('snprintf','__snprintf_chk'):
   fmt=self.string(self.reg(2));a=[self.reg(i)for i in range(3,8)]
   if fmt in (b'%.1f',b'%.2f'):out=(fmt.decode()%struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0)))[0]).encode()
   elif fmt==b'%d':out=str(signed(a[0])).encode()
   elif fmt==b'%d%s%03d':out=(str(signed(a[0])).encode()+self.string(a[1])+('%03d'%signed(a[2])).encode())
   elif fmt==b'%d%s%03d%s%03d':out=(str(signed(a[0])).encode()+self.string(a[1])+('%03d'%signed(a[2])).encode()+self.string(a[3])+('%03d'%signed(a[4])).encode())
   else:raise AssertionError(fmt)
   if self.reg(1):uc.mem_write(self.reg(0),out[:self.reg(1)-1]+b'\0')
   self.put(0,len(out));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def signed(v):v&=0xffffffff;return v if v<0x80000000 else v-0x100000000
def block(v):return words(len(v))+v
def trace_words(trace):
 out=[];keys=['GLOBAL_DECIMAL_SEPERATOR','GLOBAL_THOUSANDS_SEPERATOR','GLOBAL_THOUSANDS_GROUP_AT']
 for q in trace:
  if q[0]=='constant':out.extend([1,keys.index(q[2])])
  elif q[0]=='string':out.extend([2,q[1]])
  elif q[0]=='version':out.extend([3,q[1],q[2]])
  elif q[0]=='title':out.extend([4,q[1],q[2]])
  elif q[0]=='pack':out.extend([5,q[1]&0xffffffff])
  else:raise AssertionError(q)
 return words(*out)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();started=time.monotonic();old=Original();new=NativeCpu(a.library,True,{'functions':[]});ni=new.data+0x1000;no=ni+0x400000;records=[];rng=random.Random(20261005)
 def compare(text,values=(),prefix=b'',pack=-1,decimal=b'.',separator=b',',group=b'1000'):
  returned,out,trace=old.run(text,values,prefix,pack,decimal,separator,group);blob=words(pack,len(values))
  for v in values:
   raw=v if isinstance(v,bytes) else b'';blob+=words(0 if raw or v is None else bits(v),isinstance(v,bytes))+block(raw)
  blob+=b''.join(block(x)for x in (text,prefix,decimal,separator,group));tr=trace_words(trace);expected=words(returned)+block(out)+words(len(tr)//4)+tr
  new.uc.mem_write(ni,blob);n=new.invoke('dh2_hud_text_format_test',[ni,len(blob),no],budget=20000000);actual=bytes(new.uc.mem_read(no,len(expected)));assert n==len(expected) and actual==expected,(len(records),text,values,n,actual,expected);records.append(block(blob)+block(expected))
 for c in range(1,256):compare(b'a^'+bytes([c])+b'z',[1234.567,b'arg'])
 for code in b'dfghikmp':
  for v in [0.,-0.,.001,-.001,.005,-.005,.05,-.05,.099,-.099,1.234,-1.234,999.999,-999.999,1000.,-1000.,999999.,1000000.,123456789.]:compare(b'^'+bytes([code]),[v])
 for _ in range(600):
  values=[rng.uniform(-1e6,1e6)for _ in range(4)];text=b' / '.join(b'^'+bytes([rng.choice(b'dfghikmp')])for _ in values);compare(text,values,b'prefix:',rng.choice([-1,*range(9)]),rng.choice([b'.',b',',b'X']),rng.choice([b',',b' ',b'Y']),rng.choice([b'1000',b'999',b'0',b'-1']))
 for pack in [-1,*range(9)]:
  for text in [b'',b'plain',b'|',b'^n',b'^s:^s:^s',b'^v ^t',b'one ! two ? end',b'^unknown']:
   compare(text,[b'x',None,b'y'] if text.startswith(b'^s') else [],b'prefix:',pack)
 gold=ROOT/'reference/hud-formatting-v1/format-gold.bin';gold.write_bytes(words(0x31465448,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),original_instructions_executed=True,optimized_arm64_instructions_executed=True,mismatches=0,original_sha256=hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),library_sha256=hashlib.sha256(a.library.read_bytes()).hexdigest(),gold_sha256=hashlib.sha256(gold.read_bytes()).hexdigest(),source_sha256={x.relative_to(REPO).as_posix():hashlib.sha256(x.read_bytes()).hexdigest()for x in [ROOT/'hud_text_format_v1.hpp',ROOT/'hud_text_format_v1.cpp',ROOT/'tests/hud_text_format_v1.cpp',Path(__file__),ROOT/'tests/hud_formatting_v1_original.py']},scope=__doc__,elapsed_seconds=time.monotonic()-started);(ROOT/'reports/hud-text-format-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
