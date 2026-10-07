"""Saved original instruction service/mutation corpus vs optimized ARM64 skill kernels."""
import argparse,json,hashlib,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/level-world/reference/character-skills'
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Native(Cpu):
 def external(self,uc,a,n,u):
  if a==self.callback:return
  super().external(uc,a,n,u)
class Probe:
 def __init__(self,path,params):
  self.c=c=Native(path,True,{'functions':[]});self.params=params;d=c.data;self.state=d+0x1000;self.owner=0xabcdef0123000011;self.ais=[0xabcdef0123000051,0xabcdef0123000052];self.active=0;self.charactive=0;self.heap=d+0x10000;self.trace=[];self.touched=False;self.depth=0;self.paths=['original/path/0/','original/path/1/'];self.slotbase=[self.alloc(256),self.alloc(256)];self.counts=[0,0];self.list=[self.alloc(16),self.alloc(16)];self.rows=[[],[]]
  for k in (0,1):
   c.uc.mem_write(self.list[k],struct.pack('<QII',0,params['counts'][k],0))
   for i in range(4):
    name='' if params['empty_mask']&(1<<(k*4+i)) else ('skill' if k==0 else 'spell')+str(i);p=self.alloc(16);c.uc.mem_write(p,struct.pack('<QII',self.cstr(name),len(name),0));self.rows[k].append(p)
   for i in range(params.get('existing',[0,0])[k]):self.append(k,self.instance('existing'+str(i),i if not k else -1))
  self.sync();self.services=self.alloc(16);c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback));c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def alloc(self,n):p=self.heap;self.heap+=(n+15)&~15;return p
 def cstr(self,s):raw=s.encode()+b'\0';p=self.alloc(len(raw));self.c.uc.mem_write(p,raw);return p
 def text(self,p):return bytes(self.c.uc.mem_read(p,128)).split(b'\0')[0].decode()
 def sync(self):self.c.uc.mem_write(self.state,struct.pack('<QQIIQII',self.owner,self.slotbase[0],self.counts[0],0,self.slotbase[1],self.counts[1],0))
 def append(self,k,p):self.c.pointer(self.slotbase[k]+self.counts[k]*8,p);self.counts[k]+=1
 def instance(self,name,index):p=self.alloc(32);self.c.uc.mem_write(p,struct.pack('<QQIifI',self.owner,self.cstr(name),index&0xffffffff,-1,float(index),0));return p
 def request(self,op,*args):
  self.trace.append([op,*args]);m=self.params['mutation']
  if not self.touched and op==('debug_switch' if m==7 else 'declare' if m in (2,3,4) else 'load' if m==1 else 'set_skill'):
   self.touched=True
   if m in (1,7):self.active=self.charactive=1
   elif m==2:self.c.uc.mem_write(self.list[0]+8,struct.pack('<I',1))
   elif m==3:self.c.uc.mem_write(self.list[0]+8,struct.pack('<I',3))
   elif m==4:self.append(1,self.instance('reentered',-1));self.sync()
   elif m==8:self.charactive=1
   elif m==9:
    for k in (0,1):new=self.alloc(256);self.c.uc.mem_write(new,bytes(self.c.uc.mem_read(self.slotbase[k],self.counts[k]*8)));self.slotbase[k]=new
    self.sync()
   elif m==10:self.append(1,self.instance('added_spell',-1));self.sync()
   elif m==11 and not self.depth:
    ctx=self.c.uc.context_save();old=self.c.stack;self.depth+=1;self.c.stack-=0x4000;assert self.c.invoke('dh2_character_skills_update',[self.state,self.services])==1;self.c.stack=old;self.depth-=1;self.c.uc.context_restore(ctx)
 def hook(self,uc,a,n,u):
  c=self.c
  if a!=c.callback:return
  op,k,i,count,script,name,item,payload=struct.unpack('<4I4Q',uc.mem_read(c.reg(2),48));response=c.reg(3);obj=lst=row=status=number=0;aid=self.ais.index(script) if script in self.ais else -1
  if op==1:self.request('debug_load')
  elif op==2:self.request('debug_switch',self.text(name))
  elif op==3:obj=self.ais[self.charactive if k else self.active]
  elif op==4:self.request('capture_path',self.paths[aid]);obj=self.cstr(self.paths[aid])
  elif op==5:
   value=self.text(name or payload);self.paths[aid]=value;self.request('path',aid,value)
  elif op==6:pass
  elif op==7:self.request('list',k);lst=self.list[k]
  elif op==8:self.request('reserve',k,count)
  elif op in (9,16):pass
  elif op==10:self.request('row',k,i);row=self.rows[k][i]
  elif op==11:
   s=self.text(name);self.request('load',aid,s,self.paths[aid]);idx=int(s[-1])+(4 if s.startswith('spell') else 0) if s!='_commons' else -1;number=int(idx<0 or not(self.params['fail_mask']&(1<<idx)))
  elif op==12:
   owner,s,index,word,arg,res=struct.unpack('<QQIifI',uc.mem_read(item,32));assert owner==self.owner and word==-1 and not res;self.request('declare',aid,self.text(name),self.text(s),arg)
  elif op==13:self.request('reset',aid,self.text(name))
  elif op==14:obj=self.alloc(32);uc.mem_write(obj,bytes(uc.mem_read(item,32)))
  elif op==15:self.append(k,payload);self.sync()
  elif op==17:self.request('init_vcb',aid)
  elif op==18:state=self.params.get('state',3);self.request('predicate',i,state);number=int(state==i)
  elif op==19:
   owner,s,index,word,arg,res=struct.unpack('<QQIifI',uc.mem_read(item,32));self.request('set_skill',aid,self.text(name),self.text(s),arg);status=self.params.get('status',0);number=self.params.get('returned',0);obj=0xabcdef0123000077
  elif op==20:self.request('erase_results',count)
  elif op==21:self.request('update_skill',aid,self.text(name))
  elif op==23:pass
  else:raise AssertionError(op)
  uc.mem_write(response,struct.pack('<3Q2I',obj,lst,row,status,number));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def run(self,update):
  assert self.c.invoke('dh2_character_skills_update' if update else 'dh2_character_skills_configure',[self.state,self.services])==1
  slots=[]
  for k in (0,1):
   values=[]
   for i in range(self.counts[k]):
    p=struct.unpack('<Q',self.c.uc.mem_read(self.slotbase[k]+i*8,8))[0]
    if not p:values.append(None)
    else:_,s,index,_,_,_=struct.unpack('<QQIifI',self.c.uc.mem_read(p,32));values.append([self.text(s),index])
   slots.append(values)
  return {'trace':self.trace,'slots':slots,'paths':self.paths,'active':self.active}
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();original=json.loads((REF/'original-probe.json').read_text());calls=0
 for i,row in enumerate(original['cases']):
  new=Probe(a.library,row['params']).run(row['update']);want={k:row[k] for k in ('trace','slots','paths','active')};assert new==want,(i,new,want);calls+=len(new['trace'])
 sources=['port/level-world/character_skills.hpp','port/level-world/character_skills.cpp','port/level-world/tests/character_skills_differential.py'];report={'validation':'PASS','original_sha256':original['original_sha256'],'comparisons':len(original['cases']),'ordered_requests':calls,'mismatches':0,'library_sha256':sha(a.library),'original_gold_sha256':sha(REF/'original-probe.json'),'source_sha256':{s:sha(ROOT/s) for s in sources},'scope':__doc__+' Original dependencies are explicit synchronous Debug/STL/Lua/table/allocation services. Native core executes full setup/update ordering and 64-bit identities; owned facade and real VM are separate host proof.'};(ROOT/'port/level-world/reports/character-skills-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
