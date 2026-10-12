"""Actual room GetChar/GetHandle/map instructions; caller-owned storage only."""
import json,struct,itertools
from unicorn import UC_HOOK_CODE
from character_target_providers_differential import Source,Native,ELF,REF,words

class HandleOracle:
 def __init__(self,library=None):
  self.old=Source(ELF,json.loads((REF/'original-functions.json').read_text()));c=self.old
  self.manager=c.data+0x80000;self.context=c.data+0x81000;self.shared=c.data+0x82000;self.local=c.data+0x82100;self.owner=c.data+0x83000;self.room=c.data+0x84000;self.vtable=c.data+0x85000;self.keybuf=c.data+0x86000;self.objects=[c.data+0x90000+i*0x100 for i in range(4)];self.callback=c.data+0xa0000
  got=0x33dd40+c.word(0x33dd68);c.pointer(got+c.word(0x33dd6c),self.context)
  got=0x33fde4+c.word(0x33fe90);c.pointer(got+c.word(0x33fe94),self.context)
  c.pointer(self.context+0x38,self.manager);c.pointer(self.room,self.vtable);c.pointer(self.vtable+0x18,self.callback)
  c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  self.new=Native(library,True,{'functions':[]}) if library else None
  if self.new:
   n=self.new;self.nr=n.data+0x80000;self.records=n.data+0x81000;self.nshared=n.data+0x82000;self.nlocal=n.data+0x82100;self.out=n.data+0x83000;self.svc=n.data+0x84000;self.ncallback=n.data+0x85000
   n.uc.mem_write(self.ncallback,bytes.fromhex('c0035fd6'));n.uc.mem_write(self.svc,struct.pack('<QQ',0,self.ncallback));n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def ret(self,c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def old_hook(self,uc,address,size,unused):
  c=self.old
  if address==0x708ec0:
   count=c.word(c.reg(0));p=c.heap;c.heap+=(count+15)&~15;c.uc.mem_write(p,bytes(count));self.ret(c,p)
  elif address==0x708f00:self.ret(c)
  elif address==self.callback:self.ret(c,self.owner)
  elif address==0x33ff54:self.observed_local=c.reg(0)
  elif address in (0x3a2e1c,0x33dcd0):
   self.trace.append([5,self.objects.index(c.reg(0))+1]);self.observe(False)
 def new_hook(self,uc,address,size,unused):
  if address!=self.ncallback:return
  c=self.new;op,_,obj,other=struct.unpack('<IIQQ',c.uc.mem_read(c.reg(1),24));assert op==5 and other==0
  self.trace.append([5,obj]);self.observe(True);c.uc.mem_write(c.reg(2),struct.pack('<Q',int(obj%2==1)));self.ret(c)
 def observe(self,native):
  if self.reentry and not self.triggered:
   self.triggered=True;self.trace.append([7,0]);c=self.new if native else self.old;saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:
    if native:
     c.uc.mem_write(self.nshared+32,struct.pack('<iIQ',-5,self.frame,3));assert c.invoke('dh2_target_handle_character',[self.out+32,self.nlocal+32,self.nshared+32,self.nr,self.svc])==0
    else:
     c.uc.mem_write(self.shared+32,words(-5,self.objects[2],self.frame));c.pointer(self.owner+0x2c,self.shared+32);assert c.invoke(0x4a1cfc,[self.room])==self.objects[2];c.pointer(self.owner+0x2c,self.shared)
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.trace.append([8,0])
 def execute(self,row,native=False):
  key,cached,oldframe,frame,reentry=row;self.trace=[];self.frame=frame;self.reentry=reentry;self.triggered=False;c=self.new if native else self.old
  initial=[(-9,1),(-1,2),(2,3),(0x7fffffff,4)]
  if native:
   c.uc.mem_write(self.records,b''.join(struct.pack('<iIQ',k,0,v) for k,v in initial)+bytes(16*12));c.uc.mem_write(self.nr,struct.pack('<Q4I',self.records,len(initial),16,frame,0));c.uc.mem_write(self.nshared,struct.pack('<iIQ',key,oldframe,cached))
   assert c.invoke('dh2_target_handle_character',[self.out,self.nlocal,self.nshared,self.nr,self.svc])==0
   result=struct.unpack('<Q',c.uc.mem_read(self.out,8))[0];local=struct.unpack('<iIQ',c.uc.mem_read(self.nlocal,16));shared=struct.unpack('<iIQ',c.uc.mem_read(self.nshared,16));count=struct.unpack('<I',c.uc.mem_read(self.nr+8,4))[0];entries=[(k,v) for k,_,v in struct.iter_unpack('<iIQ',c.uc.mem_read(self.records,16*count))]
  else:
   c.heap=c.data+0x100000;c.uc.mem_write(self.manager,bytes(0x100));m=self.manager+12;c.uc.mem_write(m,words(0,0,m,m,0));c.pointer(self.manager+0x78,frame)
   for i,obj in enumerate(self.objects):
    vt=self.vtable+0x100+i*0x100;c.pointer(obj,vt);c.pointer(vt+0x24,0x3a2e1c if i%2==0 else 0x33dcd0)
   for k,v in initial:
    c.uc.mem_write(self.keybuf,words(k));p=c.invoke(0x33fc88,[m,self.keybuf]);c.pointer(p+0x18,self.objects[v-1])
   c.pointer(self.owner+0x2c,self.shared);c.uc.mem_write(self.shared,words(key,self.objects[cached-1] if cached else 0,oldframe))
   # Both the room virtual and actual GetChar/GetHandle/resolver/classifier execute.
   outer_local=c.stack+0xe000-8-16+4
   result=c.invoke(0x4a1cfc,[self.room]);result=self.objects.index(result)+1 if result else 0
   lk,lc,lf=struct.unpack('<3I',c.uc.mem_read(outer_local,12));local=(lk if lk<0x80000000 else lk-0x100000000,lf,self.objects.index(lc)+1 if lc else 0)
   s=struct.unpack('<3I',c.uc.mem_read(self.shared,12));shared=(s[0] if s[0]<0x80000000 else s[0]-0x100000000,s[2],self.objects.index(s[1])+1 if s[1] else 0)
   entries=[]
   def visit(p):
    if not p:return
    visit(c.word(p+8));k=c.word(p+16);v=c.word(p+44);entries.append((k if k<0x80000000 else k-0x100000000,self.objects.index(v)+1 if v else 0));visit(c.word(p+12))
   visit(c.word(m+4))
  return result,local,shared,entries,self.trace.copy()

def run(library):
 o=HandleOracle(library);records=[]
 for key,cached,oldframe,frame,reentry in itertools.product((-2147483648,-9,-1,0,2,17,2147483647),(0,1,2),(0,0xffffffff),(0,1,0xffffffff),(0,1)):
  row=[key,cached,oldframe,frame,reentry];expected=o.execute(row)
  if library:actual=o.execute(row,True);assert actual==expected,(row,expected,actual)
  records.append({'input':row,'output':expected})
 return records
