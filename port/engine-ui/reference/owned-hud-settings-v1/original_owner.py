import pathlib,sys,json,struct,random,hashlib
R=pathlib.Path(__file__).resolve().parents[2];D=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(R/'port/game-data/tests'))
from items_differential import Original
from unicorn import UC_HOOK_CODE
W=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
class Owner(Original):
 def __init__(self):
  super().__init__(R/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
  self.uc.hook_add(UC_HOOK_CODE,self.owner_service)
  self.manager=self.data+0x4000;self.obj=self.data+0x5000;self.text=self.data+0x6000
  self.app=self.word(0x43b2f0+self.word(0x43b37c)+self.word(0x43b380))
  self.pointer(self.app+0x38,self.obj);self.pointer(self.app+0x34,self.text);self.pointer(self.app+0x4c,self.manager)
  self.pointer(self.obj+0x60,self.obj+0x60);self.pointer(self.obj+0x14,self.obj+0xc)
  self.calls=[];self.present=False;self.platform=0
  self.blob=(R/'.local-inputs/design-settings/design_pyarray.bin').read_bytes();self.cursor=240;self.invoke(0x4bc8e0,[self.stream])
  self.blob=(R/'.local-inputs/design-settings/design_pyarraynames.bin').read_bytes();self.cursor=56;self.invoke(0x4b360c,[self.stream])
  self.names=[x['name'] for x in json.loads((D/'original-cache.json').read_text())['rows']]
 def allocate(self,n):
  p=self.heap;self.heap+=(n+15)&~15;assert self.heap<self.data+0x2000000
  self.uc.mem_write(p,bytes(n));return p
 def external(self,uc,a,size,u):
  if self.imports.get(a)=='strlen':
   p=self.reg(0);n=0
   while uc.mem_read(p+n,1)!=b'\0':n+=1
   self.returned(n)
  elif a==self.callback+32:self.returned()
  elif self.imports.get(a)=='memcmp':
   x,y,n=self.reg(0),self.reg(1),self.reg(2);left=bytes(uc.mem_read(x,n));right=bytes(uc.mem_read(y,n));self.returned((left>right)-(left<right))
  else:super().external(uc,a,size,u)
 def owner_service(self,uc,a,size,u):
  if a==0x310570:self.returned(self.allocate(self.reg(0)))
  elif a==0x708ec0:self.returned(self.allocate(self.word(self.reg(0))))
  elif a==0x708f00:self.returned()
  elif a==0x315ad0:
   assert uc.mem_read(self.reg(0)+0x38,1)==b'\1'
   self.calls.append(['file_copy',int(self.present)]);self.pointer(self.reg(0)+0x1c,self.stream if self.present else 0);self.returned()
  elif a==0x507a94:
   assert self.reg(0)==self.text and self.reg(2)==1
   self.calls.append(['switch_pack',self.reg(1)]);self.returned()
  elif a==0x531ab0:self.calls.append(['platform',self.platform]);self.returned(self.platform)
  elif a==self.callback+32:self.returned()
 def fresh(self):
  self.uc.mem_write(self.manager,bytes(0x100));self.invoke(0x46cd4c,[self.manager]);self.calls=[]
  self.pointer(self.vt+4,self.callback+32)
 def query(self,name):
  p=self.data+0x7000;self.uc.mem_write(p,name.encode()+b'\0')
  return self.invoke(0x46d474,[self.manager,p])
 def run(self,blob,present,language_only,korean,japanese,platform):
  self.fresh();self.blob=blob;self.cursor=0;self.present=present;self.platform=platform
  self.uc.mem_write(0x9f640b,bytes([korean]));self.uc.mem_write(0x9f640c,bytes([japanese]))
  self.invoke(0x46e584,[self.manager,language_only],budget=5000000)
  return {'options':[self.query(n) for n in self.names], 'tutorials':bytes(self.uc.mem_read(self.manager+0x29,14)).hex(), 'loaded':self.uc.mem_read(self.manager+0x28,1)[0],'new':self.uc.mem_read(self.manager+0x37,1)[0],'hint':self.word(self.manager+0x38),'orientation':self.uc.mem_read(self.manager+0x3c,1)[0],'cursor':self.cursor,'calls':self.calls.copy()}
def stream(rows,tutorial=bytes(range(14))):return W(len(rows))+b''.join(W(len(k))+k+W(v) for k,v in rows)+tutorial
def main():
 c=Owner();rng=random.Random(20261004);cases=[]
 blobs=[stream([]),stream([(b'Language',3),(b'VolumeMusic',31)]),stream([(b'unknown',-123),(b'VolumeFX',99),(b'VolumeFX',-7)]),stream([(b'Language\0ignored',2)]),stream([(b'x'*128,55)]),stream([(b'x'*127,5),(b'Language',4)])]
 for i in range(96):
  rows=[]
  for _ in range(rng.randrange(8)):
   key=rng.choice((b'Language',b'VolumeFX',b'AutoOrientation',b'Unknown',b'Language\0tail'))
   value=rng.choice((-1,0,1,2,7) if key.startswith(b'Language') else (-1,0,1,2,7,0x80000000,0x7fffffff));rows.append((key,value))
  blobs.append(stream(rows,bytes(rng.randrange(256) for _ in range(14))))
 for b in blobs:
  for mode in (0,1):
   args=[True,mode,0,0,rng.choice((0,1,2,3,4,5,6,7,8,0xffffffff))];cases.append({'blob':b.hex(),'args':args,'output':c.run(b,*args)})
 for mode in (0,1):
  for k,j,p in ((0,0,0),(1,0,7),(0,1,0),(255,255,0),(0,0,0xffffffff)):
   args=[False,mode,k,j,p];cases.append({'blob':'','args':args,'output':c.run(b'',*args)})
 # Platform language selection independently covers all eight cases and unsigned default.
 for p in range(10):
  for k,j in ((0,0),(1,0),(0,1),(1,1)):
   b=stream([(b'Language',7),(b'AutoOrientation',2),(b'ignored',123)]);args=[True,1,k,j,p];cases.append({'blob':b.hex(),'args':args,'output':c.run(b,*args)})
 out={'validation':'PASS','original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'comparisons':len(cases),'cases':cases,'scope':'Actual original manager constructor, actual cache descriptors/names, private STL option nodes, loadSettings/initSettings/options/tutorials/language/getters. File copy/storage, empty object-manager graph, TextManager switch and native platform enum are explicit services.'}
 (D/'original-owner.json').write_text(json.dumps(out,indent=2)+'\n');print({k:out[k] for k in ('validation','comparisons')})
if __name__=='__main__':main()
