"""Whole original text formatter/reader/layout execution, explicit font services.
ARM instructions implement all layout/parser/image branches. Owning array,
string and hash ABI projections are test services, not a runtime emulator.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu,i32
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def fl(w):return struct.unpack('<f',words(w))[0]
def txt(s):b=s.encode();return words(len(b))+b

class TextCpu(Cpu):
 def external(self,uc,address,size,unused):
  if self.callback+32<=address<=self.callback+96:
   self.machine.hook(uc,address,size,unused);return
  n=self.imports.get(address);a,b=self.reg(0),self.reg(1)
  if n=='__aeabi_fadd':self.put(0,fw(fl(a)+fl(b)))
  elif n=='__aeabi_fsub':self.put(0,fw(fl(a)-fl(b)))
  elif n=='__aeabi_fmul':self.put(0,fw(fl(a)*fl(b)))
  elif n=='__aeabi_fdiv':self.put(0,fw(fl(a)/fl(b)))
  elif n=='__aeabi_fcmpeq':self.put(0,fl(a)==fl(b))
  elif n=='__aeabi_fcmpgt':self.put(0,fl(a)>fl(b))
  elif n=='__aeabi_fcmple':self.put(0,fl(a)<=fl(b))
  elif n=='__aeabi_f2iz':self.put(0,int(fl(a)))
  elif n=='__aeabi_i2f':self.put(0,fw(i32(a)))
  elif n=='__aeabi_ui2f':self.put(0,fw(a))
  elif n=='__aeabi_i2d':
   lo,hi=struct.unpack('<II',struct.pack('<d',float(i32(a))));self.put(0,lo);self.put(1,hi)
  elif n=='__aeabi_d2iz':self.put(0,int(struct.unpack('<d',words(a,b))[0]))
  elif n in ('strlen','strcmp','strncmp','strchr','strncpy','atoi'):
   s=self.machine.cstr(a)
   if n=='strlen':self.put(0,len(s.encode()))
   elif n in ('strcmp','strncmp'):
    t=self.machine.cstr(b);limit=self.reg(2) if n=='strncmp' else max(len(s),len(t))+1
    self.put(0,0 if s[:limit]==t[:limit] else 1)
   elif n=='strchr':p=s.find(chr(b&255));self.put(0,0 if p<0 else a+len(s[:p].encode()))
   elif n=='strncpy':z=self.machine.cstr(b).encode();count=self.reg(2);uc.mem_write(a,z[:count].ljust(count,b'\0'));self.put(0,a)
   else:
    import re
    m=re.match(r'\s*([+-]?\d+)',s);self.put(0,int(m[1]) if m else 0)
  elif n=='__stack_chk_fail':raise AssertionError('source stack failure')
  else:return super().external(uc,address,size,unused)
  self.import_calls[n]=self.import_calls.get(n,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

class Original:
 def __init__(self,engine,manifest):
  self.c=TextCpu(engine,False,manifest);self.c.machine=self;self.c.uc.hook_add(UC_HOOK_CODE,self.hook);self.active=False
  d=self.c.data;self.state=d+0x1000;self.definition=d+0x2000;self.root=d+0x3000;self.library=d+0x4000;self.provider=d+0x5000;self.vt=d+0x6000;self.image=d+0x7000;self.imagevt=d+0x8000;self.bitmap=d+0x9000;self.bitmapvt=d+0xa000
  self.c.pointer(self.vt+0x84,self.c.callback+32);self.c.pointer(self.image,self.imagevt);self.c.pointer(self.imagevt+8,self.c.callback+48);self.c.pointer(self.imagevt+0x2c,self.c.callback+64);self.c.pointer(self.bitmap,self.bitmapvt);self.c.pointer(self.bitmapvt+0x24,self.c.callback+80);self.c.pointer(self.bitmapvt+0x28,self.c.callback+96)
  # Original Bionic imported C-locale lowercase table. This test supplies the
  # external table; the original hex loop/shift instructions still execute.
  self.c.pointer(0x998178,d+0xb000);self.c.pointer(d+0xb000,d+0xc000)
  self.c.uc.mem_write(d+0xc000,struct.pack('<257h',-1,*(i+32 if 65<=i<=90 else i for i in range(256))))
  self.diag=(0x78ce74+self.word(0x78d6c4))&0xffffffff
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def read(self,p,n):return bytes(self.c.uc.mem_read(p,n))
 def word(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def cstr(self,p):
  b=bytearray()
  while (v:=self.c.uc.mem_read(p,1)[0]):b.append(v);p+=1
  return b.decode()
 def string(self,p):return self.cstr(self.word(p+12) if self.read(p,1)==b'\xff' else p+1)
 def alloc(self,n):p=self.next;self.next+=(n+15)&~15;assert self.next<self.c.data+0x1800000;self.c.uc.mem_write(p,bytes(n));return p
 def store(self,p,s):
  b=s.encode()+b'\0';self.c.uc.mem_write(p,bytes(16))
  if len(b)<16:self.c.uc.mem_write(p,bytes((len(b),))+b)
  else:q=self.alloc(len(b));self.c.uc.mem_write(q,b);self.c.uc.mem_write(p,words(255,len(b),len(b),q))
 def array(self,p):return self.word(p),self.word(p+4)
 def resize(self,p,n,stride):
  q,count=self.array(p);cap=self.word(p+8)
  if n>cap:
   new=self.alloc(max(n,8)*stride)
   if count:self.c.uc.mem_write(new,self.read(q,count*stride))
   q=new;self.c.pointer(p,q);self.c.pointer(p+8,max(n,8))
  self.c.pointer(p+4,n);return q
 def push(self,p,src,stride):
  _,count=self.array(p);q=self.resize(p,count+1,stride);self.c.uc.mem_write(q+count*stride,self.read(src,stride))
  if stride==48:
   gp,gn=self.array(src+32);new=self.alloc(max(gn*36,16))
   if gn:self.c.uc.mem_write(new,self.read(gp,gn*36))
   self.c.pointer(q+count*stride+32,new);self.c.pointer(q+count*stride+40,gn)
 def font(self,p):return txt(self.string(p+0x30))+words(self.read(p+0x4d,1)[0],self.read(p+0x4c,1)[0])
 def event(self,op,font=0,*values):self.events+=words(op)+(self.font(font) if font else b'')+words(*values)
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;a,b,d,f=(c.reg(i) for i in range(4))
  if address==c.callback+32:self.ret(self.image if self.cfg[10]&1 else 0)
  elif address==c.callback+48:self.ret(1)
  elif address==c.callback+64:self.ret(self.bitmap)
  elif address==c.callback+80:self.ret(7)
  elif address==c.callback+96:self.ret(9)
  elif address in (0x751eb4,0x413a7c,0x75302c,0x752f50):
   s=self.string(b) if address in (0x75302c,0x752f50) else self.cstr(b)
   if address==0x751eb4:s=s.encode()[:d].decode()
   self.store(a,s);self.ret(a)
  elif address==0x751d14:self.store(a,self.string(a)[:b]);self.ret()
  elif address==0x78b94c:self.store(a,self.string(a)[:b]);self.ret()
  elif address in (0x41fed8,0x752b38,0x75a240,0x78a5b8):self.ret()
  elif address in (0x764234,0x77a740):c.pointer(a,b);self.ret()
  elif address in (0x78bccc,0x78a4ec,0x78bc0c):self.resize(a,b,{0x78bccc:48,0x78a4ec:36,0x78bc0c:16}[address]);self.ret()
  elif address==0x78a67c:self.ret()
  elif address in (0x78a948,0x78a81c,0x78ad54):self.push(a,b,{0x78a948:48,0x78a81c:36,0x78ad54:16}[address]);self.ret()
  elif address==0x78e010:self.hashes.setdefault(a,{})[self.string(b).lower()]=self.string(d);self.ret()
  elif address==0x78dfbc:
   value=self.hashes.get(a,{}).get(self.string(b).lower());
   if value is not None:self.store(d,value)
   self.ret(value is not None)
  elif address==0x78c600:self.hashes.pop(a,None);self.ret()
  elif address==0x78aebc:self.ret(self.string(a)==self.cstr(b))
  elif address==0x780374:self.ret(self.root)
  elif address==0x752ba8:self.ret(self.alloc(a))
  elif address==0x7cf8d8:self.store(a+0x30,'');self.ret()
  elif address==0x7ce628:uc.mem_write(a,self.read(b,0x88));self.event(5,b);self.ret()
  elif address==0x78dde8:self.ret(self.root)
  elif address==0x7cf41c:self.event(1,a);self.ret(fw(1024))
  elif address==0x7cf520:self.event(2,a);self.ret(fw(1024))
  elif address==0x7ce4a8:
   self.event(3,a,b,d);self.ret(fw(-16 if b==65 and d==86 else 0))
  elif address==0x7d01bc:
   self.lastfont=a;self.event(4,a,d,f);uc.mem_write(b,words(fw(512 if d==32 else 600),0,0,fw(1),0,fw(1),0,0xffffffff,0));self.ret(not (self.cfg[10]&2))
  elif address==0x78c2d0:self.event(6);self.ret()
  elif address==0x761184:self.event(7,self.lastfont,b);self.ret()
 def configure(self,cfg,text):
  c=self.c;self.cfg=cfg;self.next=c.data+0x10000;self.hashes={};self.events=b'';s=self.state;c.uc.mem_write(s,bytes(0x200));c.pointer(s,self.vt);c.pointer(s+0xa0,self.definition);c.pointer(s+0x30,self.root);self.fontptr=self.alloc(0x88);self.store(self.fontptr+0x30,'Original');c.pointer(self.fontptr+0x58,fw(800));c.pointer(self.fontptr+0x5c,fw(200));c.pointer(self.fontptr+0x7c,cfg[4]);c.pointer(s+0x178,self.fontptr if cfg[11]&1 else 0);c.pointer(s+0x174,cfg[0]);c.pointer(s+0x17c,cfg[1]);c.pointer(s+0x180,cfg[5]);c.pointer(s+0x184,cfg[6]);c.pointer(s+0x188,cfg[7]);c.pointer(s+0x18c,cfg[8]);c.pointer(s+0x190,cfg[9]);c.pointer(s+0x150,3);self.store(s+0x138,text);c.pointer(self.definition+0x24,0);c.pointer(self.definition+0x28,cfg[2]);c.uc.mem_write(self.definition+0x49,bytes((cfg[3],)));c.pointer(self.root+0xac,self.library);c.pointer(self.library+12,self.provider);c.pointer(self.provider+4,fw(1));c.uc.mem_write(self.root+0x87,b'\1');c.pointer(self.diag,cfg[11]>>8);self.active=True
 def snapshot(self):
  s=self.state;data=self.read(s+0xd8,16)+self.read(s+0x128,16)+self.read(s+0x154,28);p,n=self.array(s+0xa4);data+=words(n)
  for i in range(n):
   r=p+i*48;f=self.word(r+4);data+=words(self.word(r))+words(bool(f))+(self.font(f) if f else b'')+self.read(r+8,4)+words(self.read(r+12,1)[0])+self.read(r+16,12)+words(*self.read(r+28,3));gp,gn=self.array(r+32);data+=words(gn)
   for k in range(gn):
    g=gp+k*36;data+=self.read(g,4)+words(bool(self.word(g+4)))+self.read(g+8,16)+self.read(g+28,4)+words(*self.read(g+32,3))
  return data+words(self.word(self.diag),len(self.events))+self.events
 def execute(self,html):self.c.invoke(0x78efb8,[self.state,html],budget=5000000);self.active=False;return self.snapshot()

def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'port/engine-ui/reference/text-layout-v1/original-functions.json').read_text());engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert hashlib.sha256(engine.read_bytes()).hexdigest()==manifest['original_sha256'];m=Original(engine,manifest);cases=[];texts=['','AV','Hello world test','one\r\ntwo\nthree','x\x08z','word\x11break','one\u00a0two','日本語Ω😀','<p>one</p><p>two</p>','A<font color="#Ab12ef" size="18">BIG</font>Z','<font face="Other" size="150%"><b>A<i>V</i></b></font>tail','<u>line\ntext</u>','<font size="+2">up</font><font size="-1">down</font>','A<img src="Icon" width="11" height="13">B','<img src="Icon">text','one&nbsp;two&amp;three','<P>upper</P><unknown>A</unknown>B','A<font color="123456" size="0">B</font>C','<font size="12%bad">A</font>','A<broken','<font color="bad">A</font>','<font color="#g12">A</font>','<font size="-0">A</font>','<font size="18" bad=no>A</font>']
 for html in (0,1):
  for alignment in range(4):
   for multi in (0,1):
    for define in (0,1):
     for i,text in enumerate(texts):
      cfg=[fw(240 if i%3 else 360),alignment,fw(1000 if i%2 else 4000),multi,define,fw(20),fw(40),fw(10),fw(5),fw(2),int(i%4!=0),int(i%7!=0)]
      m.configure(cfg,text)
      try:out=m.execute(html)
      except Exception:
       print('failed case',len(cases),html,cfg,text,'pc',hex(m.c.uc.reg_read(m.c.pc)),file=sys.stderr);raise
      cases.append(words(html,*cfg)+txt(text)+words(len(out))+out)
 # Original missing-glyph writes survive a delivered false result; only the
 # first ten calls log. This counter is shared across chunks and cloned fonts.
 for html in (0,1):
  for initial in (0,9,10,11):
   for text in texts:
    cfg=[fw(240),2,fw(1000),1,0,fw(20),fw(40),fw(10),fw(5),fw(2),3,1|(initial<<8)]
    m.configure(cfg,text);out=m.execute(html);cases.append(words(html,*cfg)+txt(text)+words(len(out))+out)
 blob=words(0x31545854,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);assert not a.output.exists();a.output.write_bytes(blob);print(json.dumps({'cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
