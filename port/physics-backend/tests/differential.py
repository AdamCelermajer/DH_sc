"""Original full Box2D 2.0.1 world/shape/mass/contact/solver instructions versus source-built ARM64.
Only allocation/libc/imported IEEE arithmetic/trig and contact listener observers are fixtures.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn import UC_HOOK_MEM_INVALID
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/level-world/tests'))
from body_transform_differential import Cpu as Base,float_bits,floating,equal
class Cpu(Base):
 def __init__(self,*args):
  super().__init__(*args);self.heap=self.data+0x400000;self.allocations=0;self.frees=0;self.events=[0,0,0,0]
  if self.arm64:
   # The shared resource loader handles PLT and RELATIVE relocations. This
   # complete library also imports its own exported globals through GOT.
   from elftools.elf.elffile import ELFFile
   with args[0].open('rb') as stream:
    elf=ELFFile(stream)
    for section in elf.iter_sections():
     if section['sh_type'] not in ('SHT_RELA','SHT_REL'):continue
     symbols=elf.get_section(section['sh_link'])
     for relocation in section.iter_relocations():
      if relocation['r_info_type'] not in (1025,257):continue
      symbol=symbols.get_symbol(relocation['r_info_sym'])
      if symbol['st_shndx']!='SHN_UNDEF':self.pointer(self.base+relocation['r_offset'],self.base+symbol['st_value']+relocation['r_addend'])
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('malloc','_Znwm','_Znwj'):
   count=self.reg(0);assert count<0x100000;pointer=self.heap;self.heap+=(count+31)&~15;assert self.heap<self.data+0x1e00000;self.allocations+=1;self.put(0,pointer)
  elif name in ('free','_ZdlPv','_ZdlPvm','__cxa_atexit'):self.frees+=name.startswith(('free','_Zdl'));self.put(0,0)
  elif name in ('__aeabi_idiv','__aeabi_uidiv'):
   x,y=self.reg(0),self.reg(1)
   if name=='__aeabi_idiv':x=x if x<0x80000000 else x-0x100000000;y=y if y<0x80000000 else y-0x100000000
   self.put(0,int(x/y))
  elif name.startswith('__aeabi_fcmp'):
   x,y=floating(self.reg(0)),floating(self.reg(1));op=name[len('__aeabi_fcmp'):];result={'eq':x==y,'lt':x<y,'le':x<=y,'gt':x>y,'ge':x>=y,'un':math.isnan(x) or math.isnan(y)}[op];self.put(0,int(result))
  elif name=='__aeabi_fdiv':self.put(0,float_bits(floating(self.reg(0))/floating(self.reg(1))))
  elif name in ('__aeabi_ui2f','__aeabi_i2f'):self.put(0,float_bits(self.reg(0) if name=='__aeabi_ui2f' else self.reg(0) if self.reg(0)<0x80000000 else self.reg(0)-0x100000000))
  elif name in ('__aeabi_f2uiz','__aeabi_f2iz'):self.put(0,int(floating(self.reg(0))))
  elif name=='finite':
   from unicorn.arm64_const import UC_ARM64_REG_D0
   raw=uc.reg_read(UC_ARM64_REG_D0) if self.arm64 else self.reg(0)|(self.reg(1)<<32);self.put(0,int(math.isfinite(struct.unpack('<d',struct.pack('<Q',raw))[0])))
  elif name=='__isfinitef':self.put(0,int(math.isfinite(floating(self.reg(0)))))
  elif name=='sqrtf':
   from unicorn.arm64_const import UC_ARM64_REG_S0
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);v=float_bits(math.sqrt(floating(raw)))
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,v)
   else:self.put(0,v)
  elif name=='physics_listener':self.events[(address-self.callback)//16]+=1
  elif name=='__aeabi_memclr4':uc.mem_write(self.reg(0),bytes(self.reg(1)))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def snapshot_equal(expected,actual,count):
 if expected[:28]!=actual[:28] or not equal(expected[28:32],actual[28:32]):return False
 for i in range(4):
  start=32+i*148
  if expected[start:start+16]!=actual[start:start+16] or not equal(expected[start+16:start+148],actual[start+16:start+148]):return False
 return True

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--cases',type=int,default=40);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 def badmem(uc,access,address,size,value,unused):
  pc=uc.reg_read(new.pc);near=sorted((abs(at-pc),name,hex(at)) for name,at in new.symbols.items() if at<=pc)[:3];raise AssertionError(('native invalid memory',hex(pc),hex(address),near,new.import_calls))
 new.uc.hook_add(UC_HOOK_MEM_INVALID,badmem)
 world=old.data+0x1000;bounds=old.data+0x20000;gravity=bounds+16;bodydef=bounds+32;shapedef=bounds+128;massdata=bounds+256;filter=bounds+320;fvt=filter+32;listener=filter+128;lvt=listener+32
 native_input=new.data+0x1000;native_output=new.data+0x2000
 old.pointer(filter,fvt);old.pointer(fvt+8,0x7e8c48);old.pointer(listener,lvt)
 for i in range(4):old.pointer(lvt+8+4*i,old.callback+16*i);old.imports[old.callback+16*i]='physics_listener'
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def packbody(shape=0,x=0.,y=0.,vx=0.,vy=0.,density=1.,sensor=0,fixed=0,pin=0,group=0,category=1,mask=0xffff,angle=0.,angular=0.,radius=.5,hx=.5,hy=.5,cx=0.,cy=0.,sleep=0,bullet=0):
  return struct.pack('<10I14f',shape,sensor,fixed,sleep,bullet,pin,group&0xffffffff,category,mask,0,x,y,vx,vy,angle,angular,hx,hy,radius,cx,cy,density,.3,.1)
 def original(scene):
  count,steps,iterations,actions,dt,gx,gy,xmin,ymin,xmax,ymax=struct.unpack_from('<4I7f',scene);old.heap=old.data+0x400000;old.events=[0]*4;old.uc.mem_write(world,bytes(0x19278));old.uc.mem_write(bounds,struct.pack('<4f',xmin,ymin,xmax,ymax));old.uc.mem_write(gravity,struct.pack('<2f',gx,gy));old.invoke(0x7e80b8,[world,bounds,gravity,1],budget=2000000);old.pointer(world+0x19260,filter);old.pointer(world+0x19264,listener);bodies=[]
  for i in range(count):
   raw=scene[44+i*96:44+(i+1)*96];sh,sensor,fixed,sleep,bullet,pin,group,cat,mask,_=struct.unpack_from('<10I',raw);values=struct.unpack_from('<14f',raw,40);x,y,vx,vy,angle,angular,hx,hy,radius,cx,cy,density,friction,restitution=values
   old.uc.mem_write(bodydef,bytes(16)+struct.pack('<I5f4B',i+1,x,y,angle,0.,0.,1,sleep,fixed,bullet));body=old.invoke(0x7e7ef8,[world,bodydef]);assert body;bodies.append(body)
   old.uc.mem_write(shapedef,bytes(100));old.uc.mem_write(shapedef+4,struct.pack('<II3fB1xHHH',sh,0,friction,restitution,density,sensor,cat,mask,group&0xffff))
   if sh==0:old.uc.mem_write(shapedef+32,struct.pack('<3f',cx,cy,radius))
   else:old.invoke(0x7e44b8,[shapedef,float_bits(hx),float_bits(hy)])
   old.invoke(0x7e1dc8,[body,shapedef]);old.invoke(0x7e1818,[body]);
   if pin:old.uc.mem_write(massdata,bytes(4)+bytes(old.uc.mem_read(body+0x1c,8))+bytes(4));old.invoke(0x7e1b28,[body,massdata])
   old.uc.mem_write(body+0x40,struct.pack('<3f',vx,vy,angular))
   if actions&1:old.uc.mem_write(massdata,struct.pack('<2f',200. if actions&2 else x+.25,y+.125));old.invoke(0x7e164c,[body,massdata,float_bits(angle)])
  for step in range(steps):old.invoke(0x7e8b1c,[world,float_bits(dt),iterations],budget=10000000)
  paircount=old.invoke(0x7e6984,[world])
  output=struct.pack('<7I',word(world+0x1923c),word(world+0x19240),paircount,*old.events)+bytes(old.uc.mem_read(world+0x1926c,4));rows=[]
  for body in bodies:
   shape=word(body+0x64);old.invoke(0x7e95ac if word(shape+4)==0 else 0x7e4e8c,[shape,massdata]);row=struct.pack('<4I',struct.unpack('<H',old.uc.mem_read(body,2))[0],struct.unpack('<H',old.uc.mem_read(body+2,2))[0],word(body+0x68),word(shape+4))+bytes(old.uc.mem_read(body+4,84))+bytes(old.uc.mem_read(body+0x74,28))+bytes(old.uc.mem_read(massdata,16))+bytes(old.uc.mem_read(shape+16,4));assert len(row)==148;rows.append(row)
  output+=b''.join(rows)+bytes(148*(4-count));old.invoke(0x7e7f7c,[world],budget=2000000);return output
 rows=[];rng=random.Random(20261017)
 for i in range(a.cases):
  shape=i%3;body_inputs=[]
  if i<4:body_inputs=[packbody(shape=i%2,vx=1.,vy=-2.,angular=.5,fixed=i//2,cx=.1,cy=-.2)]
  else:
   body_inputs=[packbody(shape=(i//3)%2,x=-.55,y=0.,vx=1.,density=1.,fixed=i%2,sensor=int(i%7==0),group=-1 if i%11==0 else 0,bullet=int(i%5==0),pin=int(24<=i<28)),packbody(shape=i%2,x=.55,y=0.,vx=-1.,density=0. if i%4==0 else 2.,group=-1 if i%11==0 else 0)]
   if i%3==0:body_inputs.append(packbody(shape=1,x=0.,y=-1.2,hx=3.,hy=.2,density=0.))
  steps=(0,1,4,12)[i%4];dt=(.01,.016,0.)[i%3];actions=3 if 32<=i<36 else 1 if 28<=i<32 else 0
  if 36<=i<40:
   body_inputs=[packbody(shape=i%2,x=-3.,vx=1000.,radius=.2,hx=.2,hy=.2,bullet=1),packbody(shape=1,x=0.,hx=.2,hy=3.,density=0.)];steps=1;dt=.016
  scene=struct.pack('<4I7f',len(body_inputs),steps,10,actions,dt,0.,-1. if i%3==0 else 0.,-100.,-100.,100.,100.)+b''.join(body_inputs)+bytes(96*(4-len(body_inputs)))
  expected=original(scene);new.heap=new.data+0x400000;new.uc.mem_write(native_input,scene)
  try:assert new.invoke('dh2_backend_audit_scene',[native_output,native_input],budget=15000000)==0
  except Exception:
   print('Native PC',hex(new.uc.reg_read(new.pc)),'registers',[hex(new.reg(j)) for j in range(5)],'imports',new.import_calls,flush=True);raise
  actual=bytes(new.uc.mem_read(native_output,624))
  if not snapshot_equal(expected,actual,len(body_inputs)):
   diffs=[(j,hex(x),hex(y),floating(x),floating(y)) for j,(x,y) in enumerate(zip(struct.unpack('<156I',expected),struct.unpack('<156I',actual))) if x!=y];raise AssertionError((i,'state',diffs[:20]))
  rows.append(scene+expected);print(f'physics scene {i+1}/{a.cases}',flush=True)
 reference=struct.pack('<II',0x31504842,len(rows))+b''.join(rows);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference)
 executed=[]
 for row in manifest['functions']:
  start=int(row['elf_address'],16);seen=sum(start<=v<start+row['size'] for v in old.seen)
  if seen:executed.append({'symbol':row['original_symbol'],'address':row['elf_address'],'instruction_addresses_seen':seen})
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'scenes':len(rows),'mismatches':0,'original_allocations':old.allocations,'native_allocations':new.allocations,'original_import_calls':old.import_calls,'native_import_calls':new.import_calls,'original_physics_functions_executed':len(executed),'coverage':executed,'body_state_words_compared_per_body':37,'world_and_listener_words_compared':8,'proxy_capacity':2048,'pair_capacity':16384,'scope':__doc__+' Cases cover circle/polygon mass, dynamic/static/sensor/filter/fixed-rotation/bullet policies, gravity and zero dt, successful/out-of-range SetXForm, zero-mass pin, high-speed velocity limiting and continuous collision. Joint dynamics and arbitrary authored polygon shapes are not exercised.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='coverage'}))
if __name__=='__main__':main()
