"""Actual filename/wrap/renderpass and observed GL/material requests vs O2 ARM64."""
import argparse,json,random,struct,sys,time,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from swf_texture_resolver_probe import Cpu
from shader_sources_differential import sha,words
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/swf-render-connection-discovery/swf_texture64.so');p.add_argument('--report',type=Path,default=ROOT/'reports/swf-texture-arm64-differential.json');a=p.parse_args();started=time.monotonic();engine=REPO/'.local-inputs/libDungeonHunter2.so';mp=ROOT/'reference/swf-render-connection/original-functions.json';manifest=json.loads(mp.read_text());assert sha(engine)==manifest['original_sha256'];old=Cpu(engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});old.pointer(0x99f698,1)
 b=old.data+0x1000;obj=b;vt=b+0x200;input=b+0x1000;out=b+0x2000;texture=b+0x3000;driver=b+0x4000;renderer=b+0x5000;buffer=b+0x6000;material=b+0x7000;ni=new.data+0x1000;no=new.data+0x4000;old.pointer(obj,vt);old.pointer(vt+0x2c,0x56c214)
 records=[];counts={};rng=random.Random(0x7d6894);trace=[];color=None;prepared=None;mutate=None;blend=[]
 def ret(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,at,size,user):
  nonlocal color,prepared
  if at==0x5783c4:
   prepared=old.string(struct.unpack('<I',uc.mem_read(old.reg(1)+0x2c,4))[0]);ret(0xffffffff)
  elif at in (0x7935ec,0x5a0b34,0x7d4ef8,0x7d4ebc):trace.append(hex(at));ret()
  elif at==0x5cd324:
   trace.append('texture');assert old.reg(2)==0
   if mutate is not None:old.pointer(renderer+0x108,texture if mutate else 0)
   ret()
  elif at==0x5cb6e8:trace.append('diffuse');color=bytes(uc.mem_read(old.reg(3),16));assert old.reg(2)==0;ret()
  elif at==0x30e910:trace.append([old.reg(i)for i in range(3)]);ret()
  elif at==0x30e544:blend.append(['enable',old.reg(0)]);ret()
  elif at==0x30df14:blend.append(['equation',old.reg(0)]);ret()
  elif at==0x30e418:blend.append(['factors',old.reg(0),old.reg(1)]);ret()
  elif at==0x30e364:blend.append(['color',*[old.reg(i)for i in range(4)]]);ret()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def compare(op,value,expected):
  new.uc.mem_write(ni,value);new.uc.mem_write(no,bytes(max(len(expected),4)));n=new.invoke('dh2_swf_texture_test',[op,ni,len(value),no]);actual=bytes(new.uc.mem_read(no,len(expected)));assert n==len(expected)and actual==expected,(op,n,expected.hex(),actual.hex());records.append(words([op,len(value)])+value+words([len(expected)])+expected);counts[str(op)]=counts.get(str(op),0)+1
 names=[b'data/menus/MenusGraphics_droid.tga',b'data/menus/map_top.tga',b'data/menus/map_bottom.tga',b'data/menus/Foo.TGA',b'menus/MenusGraphics_droid.tga',b'/a.tga',b'data/a.tga.bdae',b'x',b'data\\menus\\x.tga'];resolved=[]
 for i in range(300):
  cwd=rng.choice([b'',b'/cache',b'DATA',b'part']);name=rng.choice(names)if i<len(names)*3 else bytes(rng.choice(b'abcXYZ/\\._-')for _ in range(rng.randrange(1,80)))+rng.choice([b'.tga',b'.TGA',b'.bdae',b'']);name=(cwd+b'/'+name)if cwd and rng.randrange(2)else name
  old.uc.mem_write(0x99b138,cwd+b'\0');old.uc.mem_write(input,name+b'\0');old.invoke(0x34e96c,[out,obj,input]);value=old.string(struct.unpack('<I',old.uc.mem_read(out+20,4))[0]);compare(0,words([len(cwd),len(name)])+cwd+name,value)
  if i<27:resolved.append({'cwd':cwd.decode(),'request':name.decode(),'resolved':value.decode()})
 for i in range(700):
  packed=rng.getrandbits(32);dirty=rng.getrandbits(16);reserved=rng.getrandbits(16);request=rng.choice([0,1,2,3,4,5,6,7,rng.getrandbits(32)]);initial=struct.pack('<IHH',packed,dirty,reserved);old.pointer(texture+0x38,packed);old.uc.mem_write(texture+0x40,initial[4:]);old.invoke(0x7d3bb4,[texture,request]);expected=bytes(old.uc.mem_read(texture+0x38,4))+bytes(old.uc.mem_read(texture+0x40,4));compare(1,initial+words([request]),expected)
 for i in range(260):
  flags=(i%64)<<4|rng.getrandbits(32)&~0x3f0;present=i%3!=0;param=0xffff if i%7==0 else rng.randrange(65535);mutate=bool(i%2) if i%11==0 else None;old.uc.mem_write(renderer,bytes(0x110));old.pointer(renderer+0x10,buffer);old.pointer(buffer,1);old.pointer(buffer+8,1);old.pointer(renderer+0x40,material);old.uc.mem_write(renderer+0x44,struct.pack('<HH',7,param));old.pointer(renderer+0x108,texture if present else 0);old.pointer(texture+0x38,flags);color=None;trace.clear();old.invoke(0x7d6894,[renderer]);after=bool(struct.unpack('<I',old.uc.mem_read(renderer+0x108,4))[0]);expected=words([color is not None])+(color or b'');compare(2,words([after,param,flags]),expected);assert trace.index('texture')<trace.index('0x7d4ef8');assert color is None or trace.index('texture')<trace.index('diffuse')<trace.index('0x7d4ef8');mutate=None
 for op,domain,shift,param in [(3,5,18,0x2802),(4,6,12,0x2801)]:
  for code in range(domain):
   old.uc.mem_write(texture,bytes(0x58));old.pointer(texture+0x34,driver);old.pointer(texture+0x38,code<<shift);old.uc.mem_write(texture+0x40,struct.pack('<H',0x10 if op==3 else 4));trace.clear();old.invoke(0x5afd40,[texture]);assert len(trace)==1 and trace[0][:2]==[0xde1,param],trace;compare(op,words([code]),words([trace[0][2]]))
 for i in range(500):
  source=words([rng.getrandbits(32)for _ in range(19)]);old.uc.mem_write(input,source);old.invoke(0x5d7a10,[out,input]);compare(5,source,bytes(old.uc.mem_read(out,32)))
 raw=(REPO/'.local-inputs/fx-render-connection-discovery/gameswf_effects.bdae').read_bytes();passes=[]
 for name,offset in [('default',0x974),('multiply',0x9f4),('screen',0xa74),('overlay',0xaf4)]:
  source=raw[offset+28:offset+104];old.uc.mem_write(input,source);old.invoke(0x5d7a10,[out,input]);state=bytes(old.uc.mem_read(out,32));compare(5,source,state);old.uc.mem_write(driver,bytes(0x210));old.pointer(driver+0x1fc,0xffffffff);old.pointer(driver+0x200,0xffffffff);blend.clear();old.invoke(0x5af3f8,[driver,out]);assert blend[0]==['enable',0xbe2];eq=next(x[1]for x in blend if x[0]=='equation');factors=next(x[1:]for x in blend if x[0]=='factors');compare(6,state,words([1,eq,*factors]));passes.append({'technique':name,'state32_hex':state.hex(),'blend_calls':list(blend)})
 for i in range(220):
  equation=rng.randrange(5);src=rng.randrange(15);dest=rng.randrange(15);state=words([(equation<<24)|(dest<<4)|src,rng.getrandbits(32),0,0,0,0,0,0]);old.uc.mem_write(out,state);old.uc.mem_write(driver,bytes(0x210));old.pointer(driver+0x1fc,0xffffffff);old.pointer(driver+0x200,0xffffffff);blend.clear();old.invoke(0x5af3f8,[driver,out]);eq=next(x[1]for x in blend if x[0]=='equation');factors=next(x[1:]for x in blend if x[0]=='factors');enabled=(struct.unpack('<I',state[4:8])[0]>>16)&1;compare(6,state,words([enabled,eq,*factors]))
 for i in range(240):
  name=rng.choice([b'',b'/A.TGA',b'/X/a.Tga',b'DATA/menus/MenusGraphics_droid.tga',b'data\\UI\\x.tga',b'X/',b'X\\',b'bare.TGA']);ignorecase=i%2;ignorepath=(i//2)%2;old.uc.mem_write(obj,bytes(0x20));old.uc.mem_write(obj+0xc,bytes([ignorecase,ignorepath]));old.uc.mem_write(input,name+b'\0');prepared=None;old.invoke(0x578520,[obj,input]);assert prepared is not None;compare(7,words([ignorecase,ignorepath])+name,prepared)
 for i in range(320):
  value=words([rng.getrandbits(32)for _ in range(8)]) if i%2 else struct.pack('<8f',*[rng.choice([0.,-0.,1.,.5,.1,255.,-255.,256.,1.00001,-.00001])for _ in range(8)]);rgba=bytes(rng.randrange(256)for _ in range(4));old.uc.mem_write(input,value);expected=old.invoke(0x794f8c,[input,int.from_bytes(rgba,'little')]);compare(8,value+rgba,words([expected]));old.uc.mem_write(out,bytes(76));old.uc.mem_write(out+0x44,b'\xff');old.uc.mem_write(input+64,struct.pack('<6f',1,0,0,0,1,0));old.invoke(0x7d4430,[out,1,input+64,0,input]);expected=bytes(old.uc.mem_read(out+0x24,32))+bytes(old.uc.mem_read(out+4,4))+words([old.uc.mem_read(out+0x44,1)[0]]);compare(9,value,expected)
 gp=ROOT/'reference/swf-render-connection/swf-texture-fixtures.bin';gp.write_bytes(words([0x31544653,len(records)])+b''.join(records));sources=[ROOT/'swf_texture.hpp',ROOT/'swf_texture.cpp',ROOT/'tests/swf_texture.cpp',Path(__file__)];report={'validation':'PASS','original_sha256':sha(engine),'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'arm64_library_sha256':sha(a.library),'reference_sha256':sha(gp),'source_sha256':{str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in sources},'comparisons':len(records),'operation_counts':counts,'mismatches':0,'actual_material_passes':passes,'filename_examples':resolved,'services':['original allocator singleton','modeled libc strstr','original cwd/global getter snapshot','actual original basename/lower routines','archive prepared-key observer; search result missing','required buffer flush/upload/material bind/submit observers','texture bind service can mutate current texture, original reread executes','original GL parameter/blend request observers'],'scope':'Exact source filename/archive query preparation/wrap/Format2 vector/renderpass conversion/GL enum requests. Actual archive registration/disk/cache owner, GL upload/rendering, full SWF geometry/cxform/masking/texture-loader lifetimes remain integration providers.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':len(records),'operation_counts':counts,'mismatches':0,'elapsed_seconds':report['elapsed_seconds']}))
if __name__=='__main__':main()
