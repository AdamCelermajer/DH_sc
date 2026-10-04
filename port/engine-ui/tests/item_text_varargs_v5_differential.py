"""Exact original integer/string va_list formatter domain vs typed O2 ARM64.
Localized numeric defaults/current pack/libc are explicit services. Floating
and dollar directives are not claimed; they reject in native production.
"""
import sys,struct,json,hashlib,argparse,random
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-ui/tests'));sys.path.insert(0,str(R/'port/game-data/tests'))
from hud_formatting_v1_original import Original,words
from hud_text_format_v1_differential import NativeCpu
from fresh_inventory_owned_v4_arm64 import InventoryCpu
class Native(InventoryCpu):
 string=InventoryCpu.text
 def external(self,uc,a,z,u):
  if self.imports.get(a)in('snprintf','__snprintf_chk'):return NativeCpu.external(self,uc,a,z,u)
  return super().external(uc,a,z,u)
def block(b):return words(len(b))+b
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=Original();new=Native(a.library);cases=[];rng=random.Random(0x49565435)
 def compare(text,values,prefix=b'',pack=-1,sep=b',',group=b'1000'):
  c=old.c;old.trace=[];c.pointer(old.manager+4,pack&0xffffffff)
  for i,s in enumerate((b'.',sep,group)):c.uc.mem_write(old.texts+i*256,s+b'\0')
  c.uc.mem_write(old.inp,text+b'\0');c.uc.mem_write(old.inp+0x8000,prefix+b'\0');c.invoke(0x3140ec,[old.out,old.inp+0x8000,0]);args=[];blob=words(pack,len(values))
  for j,v in enumerate(values):
   if isinstance(v,bytes):p=old.texts+0x1000+j*1024;c.uc.mem_write(p,v+b'\0');args.append(p);blob+=words(0,1)+block(v)
   elif v is None:args.append(0);blob+=words(0,0)+block(b'')
   else:args.append(v);blob+=words(v,0)+block(b'')
  returned=c.invoke(0x508ef4,[old.manager,old.out,old.inp,*args],budget=10000000);end,begin=struct.unpack('<II',c.uc.mem_read(old.out+16,8));result=bytes(c.uc.mem_read(begin,end-begin));c.invoke(0x3139ac,[old.out]);expected=words(returned)+block(result);blob+=b''.join(block(x)for x in(text,prefix,b'.',sep,group));new.heap=new.data+0x200000;new.uc.mem_write(new.data+0x1000,blob);n=new.invoke('dh2_item_text_varargs_fixture_v5',[new.data+0x1000,new.data+0x10000],budget=10000000);actual=bytes(new.uc.mem_read(new.data+0x10000,n))if n!=0xffffffff else None;assert actual==expected,(len(cases),text,values,pack,actual,expected);cases.append(block(blob)+block(expected))
 for code in b'dkp':
  for v in[-2147483648,-2147483647,-16777217,-1001,-1000,-999,-1,0,1,999,1000,1001,16777217,2147483646,2147483647]:
   for group in[b'1000',b'0',b'-1']:compare(b'amount ^'+bytes([code]),[v],b'prefix:',group=group)
 for pack in[-1,*range(9)]:
  for text,values in[(b'',[]),(b'plain ! ? end',[]),(b'^^ ^# ^* ^n |',[]),(b'^s:^s:^s',[b'A',None,b'B']),(b'^d / ^s',[16777217,b'item']), (b'^[s]',[])]:compare(text,values,pack=pack)
 for _ in range(160):compare(b'^d:^k:^p',[rng.randrange(-2147483648,2147483648)for _ in range(3)],sep=rng.choice([b',',b' ',b'X']),group=rng.choice([b'1000',b'999',b'-1']))
 dest=R/'port/engine-ui/reference/item-text-varargs-v5';dest.mkdir(parents=True,exist_ok=True);gold=dest/'fixtures.bin';gold.write_bytes(b'IVF5'+words(len(cases))+b''.join(cases));sources=['port/engine-ui/item_text_varargs_v5.hpp','port/engine-ui/item_text_varargs_v5.cpp','port/engine-ui/tests/item_text_varargs_v5_fixture.cpp',Path(__file__).relative_to(R).as_posix()];report=dict(validation='PASS',comparisons=len(cases),mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={x:sha(R/x)for x in sources},scope=__doc__);p=R/'port/engine-ui/reports/item-text-varargs-v5-arm64-differential.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()

