"""Complete original SendEvent versus O2 ARM64, mutable native/AS services explicit."""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from character_script_selection_differential import Cpu,string
from navigation_search_differential import word
NAMES=['button','btnDelete','btn_GAMEPLAYMENUS_ACCEPT','btn_GAMEPLAYMENUS_REFUSE','btn_Legend','btn_deadzone','\u00e9button']
METHODS=['on_focus_in','on_focus_out','on_clicked','onPress','onRelease','onReleaseOutside','onRollOver','onRollOut','onDragOver','onDragOut']
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class EventCpu(Cpu):
 def external(self,uc,address,size,unused):
  if address in (self.callback+32,self.callback+48,self.callback+64):
   self.handler(address)
   if uc.reg_read(self.pc)==address:uc.reg_write(self.pc,uc.reg_read(self.lr))
   return
  return super().external(uc,address,size,unused)
class Machine:
 def __init__(self,p,native,manifest):
  self.c=EventCpu(p,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.renderer=d+0x2000;self.owner=d+0x3000;self.vt=d+0x4000;self.services=d+0x5000;self.global_=d+0x6000;self.character=d+0x7000;self.names=[d+0x8000+i*256 for i in range(len(NAMES))]
  for a,s in zip(self.names,NAMES):c.uc.mem_write(a,s.encode()+b'\0')
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,self.write)
  if not native:
   c.pointer(self.renderer+0xfc,self.owner);c.pointer(self.owner,self.vt);c.pointer(self.vt,c.callback+32)
   table=(0x7abf6c+word(c,0x7ac1e4))&0xffffffff;c.pointer(table+word(c,0x7ac1f0),self.global_)
  else:c.uc.mem_write(self.services,struct.pack('<QQQ',0,c.callback+32,c.callback+48))
 def snapshot(self):
  c=self.c
  if self.native:
   a,n=struct.unpack('<QQ',bytes(c.uc.mem_read(self.s,16)));tail=bytes(c.uc.mem_read(self.s+16,32))
  else:
   a,n=struct.unpack('<II',bytes(c.uc.mem_read(self.s,8)));tail=bytes(c.uc.mem_read(self.s+8,32))
  return words(bool(a),self.names.index(n))+tail
 def mutate(self,action,as_call=False):
  c=self.c;off=8 if self.native else 0
  if as_call:
   if action:
    c.pointer(self.s+(8 if self.native else 4),self.names[[0,4,2,3,5][action]])
    c.uc.mem_write(self.s+8+off,words(0));c.uc.mem_write(self.s+36+off,b'\x01')
  elif action==1:c.uc.mem_write(self.s+36+off,b'\x01')
  elif action==2:c.uc.mem_write(self.s+8+off,words(6));c.pointer(self.s+(8 if self.native else 4),self.names[1])
  elif action==3:c.uc.mem_write(self.s+8+off,words(11));c.pointer(self.s,0)
  elif action==4:c.uc.mem_write(self.s+24+off,words(0xffffffff));c.uc.mem_write(self.s+37+off,b'\xfe')
 def callback(self,address):
  c=self.c
  if address==c.callback+32:
   self.events.append(words(1)+self.snapshot());self.mutate(self.na);c.put(0,1)
   if self.na==5 and not self.depth:
    self.depth=1;self.outer_lr=c.uc.reg_read(c.lr);c.uc.mem_write(self.s+(16 if self.native else 8),words(6))
    c.put(0,self.s if self.native else self.renderer);c.put(1,self.global_ if self.native else self.s)
    if self.native:c.put(2,self.services)
    c.uc.reg_write(c.lr,c.callback+64);c.uc.reg_write(c.pc,c.symbols['dh2_ui_swf_send_event'] if self.native else 0x7abf34)
  elif address==c.callback+48:
   name=string(c,c.reg(2)).decode();self.events.append(words(2,bool(c.reg(1)),METHODS.index(name)));self.mutate(self.aa,True);c.put(0,1)
  elif address==c.callback+64:
   self.depth=0;c.put(0,1);c.uc.reg_write(c.lr,self.outer_lr)
  else:raise AssertionError(hex(address))
 def hook(self,uc,address,size,unused):
  if not self.native and address==0x7abe0c:
   c=self.c;assert c.reg(3)==0 and word(c,uc.reg_read(c.sp))==0
   name=string(c,c.reg(2)).decode();self.events.append(words(2,bool(c.reg(1)),METHODS.index(name)));self.mutate(self.aa,True)
   c.put(0,self.as_found);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def write(self,uc,access,address,size,value,unused):
  if address==self.global_ and self.executing:self.events.append(words(3,value))
 def run(self,raw):
  c=self.c;logical=raw[:40];char,n=struct.unpack_from('<II',logical);self.na,self.aa,self.as_found=struct.unpack_from('<III',raw,44)
  ptr=self.character if char else 0;tail=logical[8:];state=struct.pack('<QQ' if self.native else '<II',ptr,self.names[n])+tail
  self.executing=False;c.uc.mem_write(self.s,state);c.uc.mem_write(self.global_,raw[40:44]);self.events=[];self.depth=0;self.executing=True
  result=c.invoke('dh2_ui_swf_send_event' if self.native else 0x7abf34,[self.s,self.global_,self.services] if self.native else [self.renderer,self.s]);self.executing=False
  if self.native:assert result==0
  return self.snapshot()+bytes(c.uc.mem_read(self.global_,4)),self.events
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'reference/swf-input-connection/dispatch/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261004);records=[];counts={};callbacks=0
 for i in range(2400):
  kind=[*range(14),0xffffffff][i%15];name=i%len(NAMES);na=i%6;aa=(i//5)%5
  raw=words(i%2,name,kind,rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),i%4,(i%7==0)|((i%256)<<8),rng.getrandbits(32),na,aa,i%2)
  e,events=old.run(raw);v,calls=new.run(raw);assert e==v,(i,e.hex(),v.hex());assert events==calls,(i,events,calls)
  records.append(words(len(raw),len(e),len(events))+raw+e+b''.join(words(len(x))+x for x in events));callbacks+=len(events);counts[str(kind)]=counts.get(str(kind),0)+1
 gold=words(0x31455653,len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 report={'validation':'PASS','comparisons':len(records),'ordered_services_and_global_writes':callbacks,'operation_counts':counts,'mismatches':0,'original_sha256':sha(a.engine),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'source_sha256':{str(x.relative_to(ROOT.parents[1])).replace('\\','/'):sha(x) for x in [ROOT/'swf_event_dispatch.hpp',ROOT/'swf_event_dispatch.cpp',Path(__file__)]},'scope':__doc__,'whole_input_or_frame_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
