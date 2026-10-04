"""Full relationship bodies, genuine faction loader, handle resolver and Character virtuals."""
import argparse,json,itertools,struct,sys,hashlib
from pathlib import Path
from unicorn import UC_HOOK_CODE
from character_target_providers_differential import Source,Native,ELF,ASSETS,words,NAMES
from character_target_provider_handles import HandleOracle
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-relationships'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Oracle:
 def __init__(self,library):
  self.handle=HandleOracle();self.old=self.handle.old;c=self.old;self.handle.objects=[c.data+0xb0000+i*0x2000 for i in range(4)];self.objects=self.handle.objects;self.trace=[]
  c.blob=(ASSETS/'ai_factions_pyarray.bin').read_bytes();c.cursor=0;c.invoke(0x4b3a38,[c.stream]);assert c.cursor==len(c.blob)
  got=0x3a31cc+c.word(0x3a31e0);self.faction_table=c.word(c.word(got+c.word(0x3a31e4)));self.factions=[]
  got=0x3a3194+c.word(0x3a31b0);count=c.word(c.word(got+c.word(0x3a31b4)));assert count==16
  for i in range(count):
   row=self.faction_table+i*12;size=c.word(row+4);p=c.word(row+8);self.factions.append([struct.unpack('<ii',c.uc.mem_read(p+j*12+4,8)) for j in range(size)])
  self.faction_pointers=[c.word(self.faction_table+i*12+8) for i in range(count)]
  self.types=[];self.type_table=c.data+0xc0000;c.blob=(ASSETS/'ai_pyarray.bin').read_bytes();c.cursor=4
  for i in range(struct.unpack_from('<I',c.blob)[0]):c.invoke(0x506f3c,[self.type_table+68*i,c.stream]);self.types.append(c.word(self.type_table+68*i+56))
  self.crypt_properties=self.resolve_crypt(c)
  got=0x3a3038+c.word(0x3a304c);p=c.data+0xd0000;c.pointer(got+c.word(0x3a3050),p);c.pointer(p,self.type_table)
  got=0x3a3000+c.word(0x3a301c);p+=16;c.pointer(got+c.word(0x3a3020),p);c.pointer(p,len(self.types))
  got=0x33dd84+c.word(0x33ddac);c.pointer(got+c.word(0x33ddb0),self.handle.context)
  self.shared=[c.data+0xd1000+i*0x20 for i in range(4)];self.vtables=[c.data+0xd2000+i*0x100 for i in range(4)];self.callbacks=[c.data+0xd3000+i*0x20 for i in range(3)];self.names=[c.data+0xd4000+i*64 for i in range(len(NAMES))]
  for p,raw in zip(self.names,NAMES):c.uc.mem_write(p,raw+b'\0')
  for pc,target in zip(self.callbacks,(0x3a49f0,0x3a4870,0x3a47e8)):c.uc.mem_write(pc,words(0xea000000|(((target-pc-8)//4)&0xffffff)))
  c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  self.new=Native(library,True,{'functions':[]});n=self.new;self.ns=[n.data+0xb0000+i*0x2000 for i in range(4)];self.np=[p+0x100 for p in self.ns];self.nh=[p+0x500 for p in self.ns];self.nstates=[p+0x600 for p in self.ns];self.nr=n.data+0xc0000;self.nrecords=self.nr+0x100;self.nt=n.data+0xc1000;self.nrows=self.nt+0x100;self.nentries=self.nt+0x1000;self.ntypes=n.data+0xc8000;self.typeheader=self.ntypes+0x400;self.nsvc=n.data+0xc9000;self.npsvc=self.nsvc+0x100;self.ncallback=self.nsvc+0x200;self.npcallback=self.nsvc+0x240;self.out=n.data+0xca000;self.nnames=[n.data+0xcb000+i*64 for i in range(len(NAMES))]
  for p,raw in zip(self.nnames,NAMES):n.uc.mem_write(p,raw+b'\0')
  n.uc.mem_write(self.ntypes,words(*self.types));n.uc.mem_write(self.typeheader,struct.pack('<QII',self.ntypes,len(self.types),0));n.uc.mem_write(self.nsvc,struct.pack('<QQ',0,self.ncallback));n.uc.mem_write(self.npsvc,struct.pack('<QQ',0,self.npcallback));n.uc.mem_write(self.ncallback,bytes.fromhex('c0035fd6'));n.uc.mem_write(self.npcallback,bytes.fromhex('c0035fd6'));n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def resolve_crypt(self,c):
  raw=(ASSETS/'character_properties_pyarray.bin').read_bytes();b=(ASSETS/'character_properties_pyarraynames.bin').read_bytes();at=4;names=[]
  for i in range(struct.unpack_from('<I',b)[0]):n=struct.unpack_from('<I',b,at)[0];at+=4;names.append(b[at:at+n].decode());at+=n
  defaults=raw[4:900];types=raw[900:1796];table=c.data+0xe0000;owner=c.data+0xe1000;c.pointer(0x9a645c,table);c.uc.mem_write(table,bytes(4)+defaults+bytes(4)+types);records=[]
  for index,name in enumerate(names):
   if not name.startswith('Crypt') and name!='KnightPlayerBase':continue
   c.uc.mem_write(owner,bytes(0xf00));base=raw[4+index*896:4+(index+1)*896]
   for offset,sheet in zip((8,0x38c,0x710,0xa94),(base,defaults,defaults,defaults)):c.uc.mem_write(owner+offset,bytes(4)+sheet)
   sentinel=owner+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(owner+0xe28,0)
   result=[c.invoke(0x3dfe60,[owner,prop]) for prop in (0,1,198,199)];records.append({'row':index,'name':name,'resolved':result})
  return records
 def ret(self,c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def nested(self,native,fn):
  c=self.new if native else self.old;saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
  try:return fn(c)
  finally:c.stack=stack;c.uc.context_restore(saved)
 def observe(self,native,obj):
  if self.triggered or not self.row[12]:return
  mode=self.row[12]
  if mode==4 and obj==1:return
  self.triggered=True;c=self.new if native else self.old
  if mode==1:
   if native:c.uc.mem_write(self.nstates[0],struct.pack('<Q',self.ns[3]))
   else:c.pointer(self.objects[0]+0x3cc,self.objects[3])
  elif mode in(2,4):
   value=7 if mode==2 else 10
   if native:c.uc.mem_write(self.np[obj-1],words(value))
   else:c.pointer(self.objects[obj-1]+0xff8,value)
  elif mode==3:
   self.trace.append([7,0])
   if native:
    def f(n):assert n.invoke('dh2_character_relationship',[self.out+32,2,self.nstates[0],self.ns[1],self.nr,self.nt,self.nsvc])==0
   else:
    def f(o):o.invoke(0x3d511c,[self.objects[0]+0x3c8,self.objects[1]])
   self.nested(native,f);self.trace.append([8,0])
 def old_hook(self,uc,address,size,unused):
  c=self.old
  if address in self.callbacks:
   op=self.callbacks.index(address)+1;obj=self.objects.index(c.reg(0))+1;other=self.objects.index(c.reg(1))+1 if op!=1 else 0;self.trace.append([op,obj,other]);self.observe(False,obj) if op==1 else None
  elif address in (0x3883b0,0x38ad74):self.trace.append([2 if address==0x3883b0 else 3,self.objects.index(c.reg(0))+1,self.objects.index(c.reg(1))+1])
  elif address==0x3a2ed4:self.trace.append([11,self.objects.index(c.reg(0))+1,0])
  elif address in (0x3d511c,0x3d574c) and self.started:
   # Entry into a relationship from GetInteractionType/IsInteractive.
   if c.uc.reg_read(c.lr) in (0x3a4800,0x3a48a4):self.trace.append([13 if address==0x3d511c else 14,self.objects.index(c.reg(0)-0x3c8)+1,self.objects.index(c.reg(1))+1])
 def new_hook(self,uc,address,size,unused):
  if address not in (self.ncallback,self.npcallback):return
  c=self.new;op,_,obj,other=struct.unpack('<IIQQ',c.uc.mem_read(c.reg(1),24));kind=address==self.npcallback;self.trace.append([op+10 if kind else op,obj,other]);v=0
  if not kind:
   if op==1:
    self.observe(True,obj)
    def f(n):assert n.invoke('dh2_character_target_query',[self.out+64,5,self.ns[obj-1],0,self.typeheader,self.npsvc])==0;return struct.unpack('<I',n.uc.mem_read(self.out+64,4))[0]
    v=self.nested(True,f)
   elif self.row[13]:v=0 if op==2 else 0xffffffff
   else:
    def f(n):assert n.invoke('dh2_character_target_query',[self.out+64,6 if op==2 else 7,self.ns[obj-1],self.ns[other-1],self.typeheader,self.npsvc])==0;return struct.unpack('<I',n.uc.mem_read(self.out+64,4))[0]
    v=self.nested(True,f)
  elif op==1:v=c.uc.mem_read(self.ns[obj-1]+28,1)[0]
  elif op==2:
   # Provider's virtual IsPlayer is the same original vslot observed above.
   self.trace.pop();self.trace.append([1,obj,other]);self.observe(True,obj)
   def f(n):assert n.invoke('dh2_character_target_query',[self.out+80,5,self.ns[obj-1],0,self.typeheader,self.npsvc])==0;return struct.unpack('<I',n.uc.mem_read(self.out+80,4))[0]
   v=self.nested(True,f)
  elif op in (3,4):
   def f(n):assert n.invoke('dh2_character_relationship',[self.out+80,2 if op==3 else 1,self.nstates[obj-1],self.ns[other-1],self.nr,self.nt,self.nsvc])==0;return struct.unpack('<I',n.uc.mem_read(self.out+80,4))[0]
   v=self.nested(True,f)
  c.uc.mem_write(c.reg(2),struct.pack('<Q',v));self.ret(c)
 def execute(self,row,native=False):
  self.row=row;self.triggered=False;self.trace=[];self.started=False;c=self.new if native else self.old
  op,of,tf,oa,ta,on,tn,typ,cache,key,explicit,present,mutation,generic=row[:14]
  dead=row[16] if len(row)>16 else 0;interactive=row[17] if len(row)>17 else 1
  factions=[r.copy() for r in self.factions]
  if len(row)>14 and row[14]:factions[0]=[(15,1000),(1,struct.unpack('<i',words(row[15]))[0]),(1,-1 if row[15]<0x80000000 else 1)]
  objects=self.ns if native else self.objects;frames=[]
  if native:
   p=self.nentries
   for i,entries in enumerate(factions):
    raw=b''.join(struct.pack('<ii',*e) for e in entries);c.uc.mem_write(p,raw or bytes(8));c.uc.mem_write(self.nrows+16*i,struct.pack('<QII',p,len(entries),0));p+=max(16,len(raw))
   c.uc.mem_write(self.nt,struct.pack('<QII',self.nrows,len(self.factions),0));c.uc.mem_write(self.nrecords,struct.pack('<iIQ',7,0,self.ns[2])+bytes(15*16));c.uc.mem_write(self.nr,struct.pack('<Q4I',self.nrecords,1,16,9,0))
   for i in range(4):
    c.uc.mem_write(self.np[i],bytes(896));c.uc.mem_write(self.np[i],words(of if i==0 else 10 if i==3 else tf,oa if i==0 else 44 if i==3 else ta));c.uc.mem_write(objects[i],struct.pack('<QQQI4BQII',i+1,0 if generic and typ and i in(1,2) else self.np[i],self.nnames[(on if i==0 else tn)-1],0x2000,dead if i in(1,2) else 0,0,1,interactive if i in(1,2) else 1,self.nh[i],typ if i in (1,2) else 0,0));c.uc.mem_write(self.nh[i],struct.pack('<iIQ',key if i==1 else 7,3,self.ns[cache-1] if i==1 and cache else 0 if i==1 else objects[i]));c.uc.mem_write(self.nstates[i],struct.pack('<QQ',objects[i],objects[1] if present else 0))
   self.started=True;assert c.invoke('dh2_character_relationship',[self.out,op,self.nstates[0],objects[1] if explicit else 0,self.nr,self.nt,self.nsvc])==0;result=struct.unpack('<I',c.uc.mem_read(self.out,4))[0];frames=[struct.unpack('<I',c.uc.mem_read(p+4,4))[0] for p in self.nh];owner=self.ns.index(struct.unpack('<Q',c.uc.mem_read(self.nstates[0],8))[0])+1;factions=[struct.unpack('<I',c.uc.mem_read(p,4))[0] for p in self.np];count=struct.unpack('<I',c.uc.mem_read(self.nr+8,4))[0];entries=[[k,self.ns.index(obj)+1 if obj else 0] for k,_,obj in struct.iter_unpack('<iIQ',c.uc.mem_read(self.nrecords,count*16))]
  else:
   for i,entries in enumerate(factions):
    c.pointer(self.faction_table+i*12+4,len(entries));p=self.faction_pointers[i]
    for j,(ident,value) in enumerate(entries):c.uc.mem_write(p+j*12+4,words(ident,value))
   c.uc.mem_write(self.handle.manager,bytes(256));m=self.handle.manager+12;c.uc.mem_write(m,words(0,0,m,m,0));c.pointer(self.handle.manager+0x78,9);c.uc.mem_write(self.handle.keybuf,words(7));c.heap=c.data+0x140000;p=c.invoke(0x33fc88,[m,self.handle.keybuf]);c.pointer(p+24,self.objects[2])
   for i in range(4):
    p=objects[i];vt=self.vtables[i];c.uc.mem_write(p,bytes(0x1800));c.pointer(p,vt);c.pointer(p+0x2c,self.shared[i]);c.pointer(p+0x44,self.names[(on if i==0 else tn)-1]);c.pointer(p+0xf4,typ if i in (1,2) else 0);c.uc.mem_write(p+0xff8,words(of if i==0 else 10 if i==3 else tf));c.pointer(p+0xffc,oa if i==0 else 44 if i==3 else ta);c.pointer(p+0x3cc,p);c.pointer(p+0x408,objects[1] if present else 0);c.uc.mem_write(p+0x8a,b'\1');c.uc.mem_write(p+0x1449,bytes([dead if i in(1,2) else 0]));c.uc.mem_write(p+0x415,bytes([interactive if i in(1,2) else 1]));c.pointer(p+0x520,0x2000);c.uc.mem_write(self.shared[i],words(key if i==1 else 7,self.objects[cache-1] if i==1 and cache else 0 if i==1 else p,3));c.pointer(vt+0x28,self.callbacks[0]);c.pointer(vt+0x34,0x3a2ed4);c.pointer(vt+0x88,0x3883b0 if generic and i==1 else self.callbacks[1]);c.pointer(vt+0x90,0x38ad74 if generic and i==1 else self.callbacks[2])
   self.started=True;result=c.invoke(0x3d574c if op==1 else 0x3d511c,[objects[0]+0x3c8,objects[1] if explicit else 0]);frames=[c.word(p+8) for p in self.shared];owner=self.objects.index(c.word(objects[0]+0x3cc))+1;factions=[c.word(p+0xff8) for p in objects];count=c.word(m+16);entries=[]
   def visit(p):
    if not p:return
    visit(c.word(p+8));key=struct.unpack('<i',c.uc.mem_read(p+16,4))[0];obj=c.word(p+44);entries.append([key,self.objects.index(obj)+1 if obj else 0]);visit(c.word(p+12))
   visit(c.word(m+4))
  return result,self.trace.copy(),frames,owner,factions,count,entries
def cases():
 rows=[]
 for op,of,tf,oa,ta in itertools.product((1,2),range(16),range(16),(44,1),(44,1)):rows.append([op,of,tf,oa,ta,1,1,0,2,7,1,1,0,0])
 for op,of,tf,oa,ta in itertools.product((1,2),(-1,16,0x80000000,0x7fffffff),(0,10,15,-1),(0,44),(0,44)):rows.append([op,of,tf,oa,ta,4,1,0,2,7,1,1,0,0])
 for op,typ,cache,key,explicit,present,generic in itertools.product((1,2),(0,2,15),(0,1,2,3),(-2147483648,-9,-1,0,7,99,2147483647),(0,1),(0,1),(0,1)):
  rows.append([op,0,1,44,1,1,1,typ,cache,key,explicit,present,0,generic])
 for op,mode,oa,ta in itertools.product((1,2),(1,2,3,4),(0,1,44),(0,1,44)):rows.append([op,11,7,oa,ta,1,1,0,3,7,1,1,mode,0])
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();o=Oracle(a.library);records=[];callbacks=0
 rows=cases()
 for op,value in itertools.product((1,2),(0,1,0xffffffff,0x80000000,0x7fffffff,0x80001234)):rows.append([op,0,1,1,1,1,1,0,2,7,1,1,0,0,1,value])
 for owner,target,op in itertools.product(o.crypt_properties,o.crypt_properties,(1,2)):rows.append([op,owner['resolved'][0],target['resolved'][0],owner['resolved'][1],target['resolved'][1],1,1,0,2,7,1,1,0,0])
 for op,of,tf,ta,dead,interactive in itertools.product((1,2),(0,7,11),(0,7,11),(0,1,44),(0,1,255),(0,1,127)):rows.append([op,of,tf,44,ta,1,1,0,2,0,1,1,0,0,0,0,dead,interactive])
 for i,row in enumerate(rows):
  expected=o.execute(row);actual=o.execute(row,True);assert actual==expected,(i,row,expected,actual);records.append({'input':row,'output':expected});callbacks+=len(expected[1])
 blob=b'REL1'+words(len(o.types),len(o.factions),len(records))+words(*o.types)
 for f in o.factions:blob+=words(len(f))+b''.join(words(*entry) for entry in f)
 for rec in records:
  row=rec['input'].copy();size=len(row);row+=(18-size)*[0]
  if size<18:row[17]=1
  result,trace,frames,owner,factions,count,entries=rec['output'];blob+=words(*row,result,*frames,owner,*factions,count,len(trace))+b''.join(words(*e) for e in entries)+b''.join(words(*(t+[0]*(3-len(t)))) for t in trace)
 gold=REF/'relationship-fixtures.bin';gold.write_bytes(blob)
 bindings=[ROOT/'character_relationships.cpp',ROOT/'character_relationships.hpp',ROOT/'character_target_providers.cpp',ROOT/'character_target_providers.hpp',Path(__file__),ROOT/'tests/character_target_provider_handles.py',ROOT/'tests/character_target_providers_differential.py',*REF.rglob('original-functions.json')]
 report={'validation':'PASS','comparisons':len(records),'ordered_callbacks':callbacks,'mismatches':0,'original_sha256':sha(ELF),'arm64_library_sha256':sha(a.library),'corpus_sha256':sha(gold),'source_bindings':{str(p.relative_to(REPO)):sha(p) for p in bindings},'asset_bindings':{str(p.relative_to(REPO)):sha(p) for p in (ASSETS/'ai_factions_pyarray.bin',ASSETS/'ai_pyarray.bin',ASSETS/'character_properties_pyarray.bin',ASSETS/'character_properties_pyarraynames.bin')},'factions':o.factions,'types':o.types,'genuine_character_property_producers':o.crypt_properties,'scope':__doc__,'records':records};(REF/'relationship-probes.json').write_text(json.dumps(report,indent=2)+'\n');summary={k:v for k,v in report.items() if k!='records'};summary['probe_sha256']=sha(REF/'relationship-probes.json');(ROOT/'reports/character-relationships-arm64-differential.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary))
if __name__=='__main__':main()
