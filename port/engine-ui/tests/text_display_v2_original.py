"""Execute original whole display_glyph_records; record explicit render/cache services."""
import argparse, hashlib, json, math, random, struct, sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import TextCpu,words,fw,fl

class DisplayCpu(TextCpu):
 def external(self,uc,a,size,unused):
  if self.callback+112<=a<=self.callback+128:
   self.machine.hook(uc,a,size,unused);return
  n=self.imports.get(a)
  if n in ('__aeabi_fcmpge','__aeabi_fcmplt','__aeabi_fcmpne'):
   x,y=fl(self.reg(0)),fl(self.reg(1));self.put(0,{'__aeabi_fcmpge':lambda:x>=y,'__aeabi_fcmplt':lambda:x<y,'__aeabi_fcmpne':lambda:x!=y}[n]())
  elif n in ('sqrtf','sqrt'):self.put(0,fw(math.sqrt(fl(self.reg(0)))))
  else:return super().external(uc,a,size,unused)
  self.import_calls[n]=self.import_calls.get(n,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

class Original:
 def __init__(self):
  manifest=json.loads((ROOT/'port/engine-ui/reference/text-display-v2/original-functions.json').read_text())
  source=ROOT/'.local-inputs/libDungeonHunter2.so';assert hashlib.sha256(source.read_bytes()).hexdigest()==manifest['original_sha256']
  self.c=DisplayCpu(source,False,manifest);self.c.machine=self;self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
  d=self.c.data;self.character=d+0x1000;self.cv=d+0x2000;self.player=d+0x3000;self.library=d+0x4000
  self.ft=d+0x5000;self.bp=d+0x6000;self.ftcache=d+0x7000;self.bcache=d+0x8000;self.renderer=d+0x9000;self.rv=d+0xa000
  self.matrix=d+0xb000;self.cx=d+0xc000;self.array=d+0xd000;self.records=d+0xe000;self.font=d+0xf000
  self.bitmap=d+0x11000;self.other=d+0x12000;self.bv=d+0x13000;self.override=d+0x14000;self.shape=d+0x15000
  c=self.c;c.pointer(self.character,self.cv);c.pointer(self.character+0x30,self.player);c.pointer(self.player+0xac,self.library)
  c.pointer(self.library+12,self.ft);c.pointer(self.library+16,self.bp);c.pointer(self.renderer,self.rv)
  c.pointer(self.cv+0x78,c.callback+32)
  for offset,entry in [(0x50,48),(0x78,64),(0x60,80),(0x80,96)]:c.pointer(self.rv+offset,c.callback+entry)
  for p in (self.bitmap,self.other):c.pointer(p,self.bv)
  c.pointer(self.bv+0x24,c.callback+112);c.pointer(self.bv+0x28,c.callback+128)
  got=(0x78fab8+self.word(0x790908))&0xffffffff;self.render_global=self.word(got+self.word(0x79090c))
  self.active=False
 def read(self,p,n):return bytes(self.c.uc.mem_read(p,n))
 def word(self,p):return struct.unpack('<I',self.read(p,4))[0]
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def cstr(self,p):raise AssertionError('Unexpected string dependency')
 def event(self,op,*items):self.events+=words(op,*items)
 def hook(self,uc,address,size,unused):
  if not self.active:return
  if uc.reg_read(self.c.pc)!=address:return # external dispatch already returned
  c=self.c;a,b,d,f=(c.reg(i) for i in range(4));sp=c.uc.reg_read(c.sp)
  if address==c.callback+32:self.ret(self.cfg[7])
  elif address==c.callback+48:self.drawmatrix=self.read(b,24);self.ret()
  elif address==c.callback+64:self.drawcolor=b;self.ret()
  elif address==c.callback+80:
   self.event(2,self.drawcolor,d);self.events+=self.drawmatrix+self.read(b,d*8);self.mutate();self.ret()
  elif address==c.callback+96:
   self.event(3,1 if d==self.bitmap else 2,self.word(sp+4));self.events+=self.read(b,24)+self.read(f,16)+self.read(self.word(sp),16);self.mutate();self.ret()
  elif address in (c.callback+112,c.callback+128):
   self.ret((128 if a==self.bitmap else 32) if address==c.callback+112 else (64 if a==self.bitmap else 16))
  elif address in (0x784a8c,0x784b20,0x78b8d0,0x76150c,0x7613e4,0x784cac):self.ret()
  elif address==0x76146c:
   c.pointer(a,c.data+0x16000);c.pointer(a+4,b);self.ret()
  elif address==0x753f74:self.ret(self.matrix)
  elif address==0x753ec0:self.ret(self.cx)
  elif address==0x78ae50:self.current_record=(a-self.records)//48;self.current_glyph=0;self.event(1,self.current_record);self.ret()
  elif address==0x794f8c:self.ret(b^0x00112233)
  elif address==0x7ce3b8:self.lastindex=b;self.ret(0 if self.cfg[9]&8 else self.shape)
  elif address==0x77a990:
   self.event(4,self.lastindex,self.cfg[7],self.current_style_color());self.events+=self.read(b,24);self.mutate();self.ret()
  elif address in (0x7c55d8,0x7d266c):
   ft=address==0x7d266c;out=self.word(sp+4 if ft else sp);filter=self.read(self.word(sp),3) if ft else b'\0\0\0'
   self.event(5,int(ft),b,f,*filter)
   vals=(3.0,43.0,5.0,25.0) if filter==b'\0\0\0' else (7.0,99.0,9.0,87.0)
   uc.mem_write(out,struct.pack('<4f',*vals));self.ret()
 def current_style_color(self):
  # Shape fill is the source color, not the transformed color. Current record
  # can be obtained from the last resolve request, explicitly a render fixture.
  return self.word(self.records+self.current_record*48+8)
 def mutate(self):
  if self.cfg[9]&4:
   p=self.glyphbase+self.current_record*0x1000+self.current_glyph*36
   self.c.pointer(p,fw(37.5));self.current_glyph+=1
 def configure(self,cfg,records):
  c=self.c;self.cfg=cfg;self.events=b'';self.current_record=0;self.current_glyph=0
  c.pointer(self.render_global,self.renderer if cfg[9]&1 else 0);c.uc.mem_write(self.matrix,words(*cfg[:6]));c.pointer(self.ft+4,cfg[6])
  c.pointer(self.ft+0x28,self.ftcache if cfg[9]&16 else 0);c.pointer(self.bp+12,self.bcache if cfg[9]&32 else 0);c.pointer(self.ftcache+0x34,self.bitmap)
  c.pointer(self.array,self.records);c.pointer(self.array+4,len(records));c.pointer(self.override,cfg[8]);c.pointer(self.font+0x7c,cfg[9]>>8&1)
  self.glyphbase=c.data+0x20000
  for i,(style,glyphs) in enumerate(records):
   p=self.records+48*i;flags=style[6]
   c.uc.mem_write(p,words(0,self.font if flags&8 else 0,style[0],style[1],style[2],style[3],style[4],flags&1,(self.glyphbase+i*0x1000),len(glyphs),len(glyphs),0))
   c.uc.mem_write(p+28,bytes((bool(flags&1),bool(flags&2),bool(flags&4))))
   for j,g in enumerate(glyphs):
    q=self.glyphbase+i*0x1000+j*36;bmp=0 if g[1]==0 else self.bitmap if g[1]==1 else self.other
    c.uc.mem_write(q,words(g[0],bmp,*g[2:6],0x12345678,g[6],g[7]))
  self.active=True
 def execute(self):
  self.c.invoke(0x78faa0,[0,self.character,self.array,0],stack=(self.override if self.cfg[9]&2 else 0,*self.cfg[10:13]),budget=1000000)
  self.active=False;return self.events

def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();m=Original();cases=[]
 rng=random.Random(0x78faa0)
 # All paths: no renderer, null font records, images, missing/shape/bitmap,
 # two cache producers, all filter branches, font3, matrices and overrides.
 for k in range(768):
  mats=[(1,0,13,0,1,-9),(2,0,13,0,3,-9),(0.5,0.25,13,-0.75,1.5,-9),(-1,0,0,0,-1,0)]
  flags=1|(2 if k&1 else 0)|(16 if k&2 else 0)|(32 if k&4 else 0)|(256 if k&8 else 0)|(8 if k&16 else 0)
  # Original cached draw dereferences the FT cache before choosing the bitmap
  # cache. A bitmap-only cache is outside that source pointer domain.
  if flags&32:flags|=16
  if k%37==0:flags&=~1
  cfg=[*(fw(v) for v in mats[k%4]),fw([1,0.5,2][k%3]),fw(1.25),0x80554433,flags,*[(0,0,0),(3,0,0),(0,2,4),(3,2,4)][(k//4)%4]]
  recs=[]
  for i in range(3):
   style=[0xbbaabbcc,int(k+i)%2,fw(i*19-7),fw(i*13-5),fw([240,360,480][i]),0,15 if i!=1 else (3 if k%5==0 else 7 if k%7==0 else 12)]
   glyphs=[]
   for j in range(5):
    typ=(k+j+i)%5;bmp=0 if typ<2 else 1 if typ!=4 else 2;index=-1 if typ==0 else j
    if flags&16 and not flags&32 and bmp==2:bmp=1
    # Null-font records in real source layout contain inline-image glyphs.
    # Do not hide a null font.get_glyph_by_index behind the shape fixture.
    if not style[6]&8 and not style[6]&4:typ=3;bmp=1;index=-1
    glyphs.append([fw(20+j*3),bmp,fw(-0.25),fw(0 if k%11==0 and j==2 else 0.75),fw(0.5),fw(0.625),((index&65535)<<16)|([12,18,24][i]),(2 if typ==3 else 0)<<16|65+j])
   recs.append((style,glyphs))
  m.configure(cfg,recs);out=m.execute();data=words(*cfg,len(recs))
  for style,glyphs in recs:data+=words(*style,len(glyphs))+b''.join(words(*g) for g in glyphs)
  cases.append(data+words(len(out))+out)
 blob=words(0x32445354,len(cases))+b''.join(cases);assert not a.output.exists();a.output.write_bytes(blob)
 print(json.dumps({'cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
