"""Original source-name/config/GL source-vector instructions vs O2 ARM64.

File, cache miss, base refcount owner and GL compile results are explicit services.
No shader GPU, complete resource manager, or material selection parity is claimed.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
class Cpu(FactoryCpu):
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='strcpy':
   uc.mem_write(self.reg(0),self.string(self.reg(1))+b'\0');uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def text(s):return words([len(s)])+s
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/fx-render-connection-discovery/shader_sources64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/shader-sources-arm64-differential.json');a=p.parse_args();started=time.monotonic();mp=ROOT/'reference/shader-sources/original-functions.json';m=json.loads(mp.read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==m['original_sha256'];old=Cpu(engine,False,m);new=Cpu(a.library,True,{'functions':[]});old.pointer(0x99f698,1)
 pool=0x534608+struct.unpack('<i',old.uc.mem_read(0x534680,4))[0];arena=old.data+0x1000000
 base=old.data+0x1000;manager=base;driver=base+0x200;dev=base+0x400;fs=base+0x600;fv=base+0x700;file=base+0x800;filevt=base+0x900;out=base+0xa00;strings=base+0x1000
 service=old.data+0x100;file_data=b'';sources=[];key=b'';gltype=0;calls={}
 def ret(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,at,size,user):
  nonlocal sources,key,gltype
  if at in (0x6e0b64,0x6e0c10,0x6e0a3c,0x6dfe68,0x6df738,0x6df3c0):calls[hex(at)]=calls.get(hex(at),0)+1
  if at==0x6df738:
   ptrs=struct.unpack('<9I',uc.mem_read(old.reg(2),36));assert ptrs[7],('empty body pointer',ptrs,file_data)
  if at==0x6e1b08:key=old.string(old.reg(2));old.pointer(old.reg(0),0);ret(old.reg(0)) # required cache-miss service
  elif at==0x6e2178:old.pointer(old.reg(0)+4,0);ret(old.reg(0)) # base name/refcount owner
  elif at==0x31d584:ret() # borrowed service-owner releases
  elif at==0x30e538:gltype=old.reg(0);ret(77) # glCreateShader
  elif at==0x30ed84:
   assert old.reg(3)==0;sources=[old.string(struct.unpack('<I',uc.mem_read(old.reg(2)+i*4,4))[0])for i in range(old.reg(1))];ret()
  elif at==0x30e46c:ret() # glCompileShader
  elif at==0x30ed54:old.pointer(old.reg(2),1 if old.reg(1)==0x8b81 else 0);ret()
  elif at==service:ret(file)
  elif at==service+4:ret(len(file_data))
  elif at==service+8:
   assert old.reg(2)==len(file_data)
   if file_data:uc.mem_write(old.reg(1),file_data)
   ret(len(file_data))
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def setup(extra,flags):
  old.heap=old.data+0x800000;old.allocations={};old.uc.mem_write(pool,words([arena,arena+0x100000,arena,0x100000,0]));old.uc.mem_write(manager,bytes(0x100));old.pointer(manager+0x2c,driver);old.pointer(driver+0xd4,dev);old.pointer(dev+0x34,fs);old.pointer(driver+0x88,flags);old.pointer(fs,fv);old.pointer(fs+4,1);old.pointer(fv+0xc,service);old.pointer(file,filevt);old.pointer(file+4,1);old.pointer(filevt+0x20,service+4);old.pointer(filevt+0xc,service+8)
  old.pointer(manager+0x80,0 if extra is None else len(extra));old.pointer(manager+0x7c,0 if extra is None else strings+0x4000)
  if extra is not None:old.uc.mem_write(strings+0x4000,extra+b'\0')
 def c(i,s):
  if s is None:return 0
  at=strings+i*0x800;old.uc.mem_write(at,s+b'\0');return at
 ni=new.data+0x1000;no=new.data+0x20000;records=[];counts={}
 def compare(op,input,expected):
  new.uc.mem_write(ni,input);new.uc.mem_write(no,bytes(max(4,len(expected))));n=new.invoke('dh2_shader_sources_test',[op,ni,len(input),no],budget=20000000);actual=bytes(new.uc.mem_read(no,len(expected)));assert n==len(expected) and actual==expected,(op,n,len(expected),expected,actual);records.append(words([op,len(input)])+input+words([len(expected)])+expected);counts[str(op)]=counts.get(str(op),0)+1
 rng=random.Random(0x8eef90);raws=[b'',b'one',b'^first^^last^',b'#define X 1\r\n',bytes(range(1,128))]
 for i in range(400):
  flags=rng.getrandbits(32);typ=rng.choice([0,4,14,0xffffffff]);filename=rng.choice([b'GameSWFVS.glsl',b'GameSWFFS.glsl',b'UnlitTexturedFP.glsl',b'',b'same-key']);caller=rng.choice(raws);extra=rng.choice([None,*raws]);file_data=(REPO/'.local-inputs/fx-render-connection-discovery/entries'/filename.decode()).read_bytes() if filename in (b'GameSWFVS.glsl',b'GameSWFFS.glsl',b'UnlitTexturedFP.glsl') else rng.choice(raws);setup(extra,flags);sources=[];key=b'';old.invoke(0x6dfe68,[out,manager,c(0,filename),typ,c(1,caller),file],budget=30000000);assert len(sources)==8,(len(sources),sources,hex(gltype),old.reg(0))
  expected=words([gltype,8,len(key),*[len(s)for s in sources]])+key+b'\0'+b''.join(s+b'\0'for s in sources);input=words([flags,typ,extra is not None])+b''.join(text(s)for s in (filename,caller,extra or b'',file_data));compare(0,input,expected)
 for i in range(300):
  extra=rng.choice([None,*raws]);setup(extra,0);abc=[rng.choice(raws)for j in range(3)];length=out+4;ptr=old.invoke(0x6e0c10,[manager,*[c(j,s)for j,s in enumerate(abc)],length]);result=old.string(ptr);assert struct.unpack('<I',old.uc.mem_read(length,4))[0]==len(result);compare(1,words([extra is not None])+b''.join(text(s)for s in (*abc,extra or b'')),words([len(result)])+result)
 for i in range(160):
  file_data=rng.choice(raws) if i<5 else bytes(rng.choice([94,10,13,*range(1,127)])for j in range(rng.randrange(300)));setup(None,0);old.pointer(manager+0x80,0xffffffff);old.invoke(0x6e0a3c,[manager,c(0,b'glsl.config')]);ptr=struct.unpack('<I',old.uc.mem_read(manager+0x7c,4))[0];result=old.string(ptr);assert len(result)==len(file_data);compare(2,text(file_data),words([len(result)])+result)
 gp=ROOT/'reference/shader-sources/shader-source-fixtures.bin';gp.write_bytes(words([0x31534853,len(records)])+b''.join(records));sources_paths=[ROOT/'shader_sources.hpp',ROOT/'shader_sources.cpp',ROOT/'tests/shader_sources.cpp',Path(__file__)];report={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(mp),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(gp),'source_sha256':{str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in sources_paths},'comparisons':len(records),'operation_counts':counts,'original_source_calls':calls,'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'mismatches':0,'shader_pack_sha256':sha(REPO/'.local-inputs/fx-render-connection-discovery/shaders.pak'),'services':['initialized original process-buffer arena1MiB','modeled libc strcpy','cache miss projection','borrowed IReadFile exact length/read','base shader name/refcount owner','borrowed reference releases','glCreateShader token77','glShaderSource observation','glCompileShader success/no info log'],'scope':'Original name concatenate/config caret rewrite/eight compile-source strings and stage projection. GPU compilation/rendering, complete shader resource manager, material selection/binding and archive parser parity are outside this proof.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('source_sha256','scope')}))
if __name__=='__main__':main()


