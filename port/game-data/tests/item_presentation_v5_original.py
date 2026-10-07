"""Execute genuine Item Name/Stats/Req callers. Localized strings, constants,
class-name OIDs and StringManager::parse varargs are declared caller services.
No fake item metadata, name grammar or source caller branch is substituted.
"""
import sys,struct,json
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from item_inventory_v1_original import Inventory,W
from unicorn import UC_HOOK_CODE
class OriginalPresentation(Inventory):
 def __init__(self):
  super().__init__();self.item=self.allocate(0x6c);self.textheap=self.allocate(0x100000);self.textat=self.textheap;self.trace=[];self.keys={};self.localized={};self.loadingsource=False
  # Actual singleton pointer from caller GOT. Only its manager is borrowed.
  got=(0x3fb2c8+self.word(0x3fb478))&0xffffffff;self.application=got+self.word(0x3fb47c);self.manager=self.allocate(64);self.pointer(self.application+0x34,self.manager)
  # Name uses the same Application GOT entry.
  got=(0x3fb76c+self.word(0x3fbc1c))&0xffffffff;assert got==0x994a98;self.pointer(got+self.word(0x3fbc2c),self.application)
  self.uc.hook_add(UC_HOOK_CODE,self.present)
 def txt(self,b):
  p=self.textat;self.textat+=len(b)+1;self.uc.mem_write(p,b+b'\0');return p
 def stringvalue(self,obj):
  begin,end=self.word(obj+0x14),self.word(obj+0x10);return bytes(self.uc.mem_read(begin,end-begin))
 def external(self,uc,a,z,u):
  n=self.imports.get(a)
  if n=='malloc':self.returned(self.allocate(self.reg(0)))
  elif n=='free':self.returned()
  elif n=='strstr':
   p=self.reg(0);at=self.cstring(p).find(self.cstring(self.reg(1)));self.returned(p+at if at>=0 else 0)
  else:super().external(uc,a,z,u)
 def present(self,uc,a,z,u):
  if not self.loadingsource:return
  if a==0x4c4bdc:
   group,key=self.cstring(self.reg(1)),self.cstring(self.reg(2));self.trace.append(['constant',group.decode(),key.decode()]);i=self.keys.setdefault(key,100000+len(self.keys));self.localized[i]=key+b'(^d)' if key not in (b'GLOBAL_LIST_SEPERATOR',b'INGAME_REQUIREMENTS',b'INGAME_REQUIRES_CLASS') else {b'GLOBAL_LIST_SEPERATOR':b', ',b'INGAME_REQUIREMENTS':b'Requires:',b'INGAME_REQUIRES_CLASS':b'^s'}[key];self.returned(i)
  elif a==0x508edc:
   i=self.reg(1);self.trace.append(['string',i]);self.returned(self.txt(self.localized.get(i,b'OID'+str(i).encode())))
  elif a==0x508ef4:
   obj=self.reg(1);fmt=self.cstring(self.reg(2));args=[self.reg(3)]+list(struct.unpack('<8I',uc.mem_read(uc.reg_read(self.sp),32)));self.trace.append(['parse',fmt.decode(),args[0],args[1]]);out=bytearray();arg=0;esc=False
   for c in fmt:
    if not esc:
     if c==94:esc=True
     else:out.append(c)
    else:
     esc=False
     if c==ord('d'):out+=str(struct.unpack('<i',W(args[arg]))[0]).encode();arg+=1
     elif c==ord('s'):out+=self.cstring(args[arg]) if args[arg] else b'';arg+=1
     elif c==ord('n'):out+=b'\n'
     elif c in (35,42,94):out.append(c)
   before=self.stringvalue(obj);p=self.txt(before+out);saved=uc.context_save();self.loadingsource=False;priorstack=self.stack;self.stack-=0x8000;self.invoke(0x3109e0,[obj,p,p+len(before+out)]);self.stack=priorstack;uc.context_restore(saved);self.loadingsource=True;self.returned()
 def run(self,id,op=0x3fb754,value=123,name=None,material=None):
  self.trace=[];self.textat=self.textheap;self.pointer(self.item+4,id);self.pointer(self.item+0x54,value&0xffffffff)
  for off in (8,0x20,0x38):p=self.txt(b'old');self.invoke(0x3140ec,[self.item+off,p,0])
  if name is not None:self.localized[self.word(self.table+164*id+68)]=name
  if material is not None:self.localized[self.word(self.table+164*id+72)]=material
  self.loadingsource=True
  try:self.invoke(op,[self.item],budget=10000000)
  finally:self.loadingsource=False
  return self.stringvalue(self.item+{0x3fb754:8,0x3fb290:0x20,0x3facdc:0x38}[op]),list(self.trace)
def main():
 o=OriginalPresentation()
 for a in (0x3fb290,0x3facdc):
  out,tr=o.run(664,a);print(hex(a),out,tr)
 for name in (b'Sword',b'Sword[f]',b'Sword[fs]',b'Sword[s]'):
  # Caller projection of one real metadata row's source NameOID/MaterialOID.
  o.pointer(o.table+164*664+68,40000);o.pointer(o.table+164*664+72,40001)
  out,tr=o.run(664,name=name,material=b'iron#female#single#both[a]');print(name,out,tr)
if __name__=='__main__':main()



