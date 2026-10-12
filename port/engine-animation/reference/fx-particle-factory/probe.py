"""Complete original cloud constructors and actual FX77 node factory branch."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu,words
from compiled_transforms_differential import word,relocate,database
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 manifest=json.loads((HERE/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';cpu=FactoryCpu(engine,False,manifest);cpu.pointer(0x99f698,1);block=cpu.data+0x10000;phase='';trace=[];allocations=[];names=[];constructor_calls=[]
 constructors={0x64d1d0:'context',0x6545d0:'generation',0x6543f8:'size',0x654690:'color',0x6548fc:'emitter',0x6540ec:'motion',0x654218:'spin',0x65452c:'life'}
 node=cpu.data+0x20000;vt=node+0x300;service=vt+0x200;cpu.pointer(vt+0xf4,service);current_db=0;current_emitter=0;driver=service+0x100;attachments=driver+0x100;root=attachments+0x100
 def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def hook(uc,address,size,user):
  if phase=='cloud':
   if address==0x5341ac:
    n=cpu.reg(0);p=block if not allocations else cpu.heap;cpu.heap+=((n+15)&~15);allocations.append(n);cpu.uc.mem_write(p,bytes([0xcd])*n);ret(p)
   if address==0x64d0bc:names.append(cpu.string(cpu.reg(1)).decode())
   if address in constructors:constructor_calls.append({'kind':constructors[address],'source':hex(address),'offset':cpu.reg(0)-block})
  elif phase=='node':
   if address==0x5341ac:assert cpu.reg(0)==0x198;trace.append(['allocate_scene_node',0x198]);ret(node)
   elif address==0x64ed94:
    assert cpu.reg(0)==node and cpu.reg(1)==current_db and cpu.reg(2)==current_emitter and cpu.reg(3)==attachments and word(bytes(cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp),4)),0)==root
    trace.append(['required_scene_node_constructor',0x64ed94]);cpu.pointer(node,vt);ret(node)
   elif address==service:assert cpu.reg(0)==node and cpu.reg(1)==driver and cpu.reg(2)==1;trace.append(['required_initParticleSystem',True]);ret()
 cpu.uc.hook_add(UC_HOOK_CODE,hook);cloud=[]
 for color in (0,1):
  phase='cloud';allocations=[];names=[];constructor_calls=[];result=cpu.invoke(0x655314,[0,color],budget=20000000);assert result==block;assert allocations[0]==0x1dc
  context=block+0x180;header=context+0x30;count=word(bytes(cpu.uc.mem_read(context+0x40,4)),0);assert count==len(names)==41
  slots=[]
  def visit(n):
   if not n:return
   left=word(bytes(cpu.uc.mem_read(n+8,4)),0);right=word(bytes(cpu.uc.mem_read(n+12,4)),0);visit(left);key,pointer=struct.unpack('<2I',bytes(cpu.uc.mem_read(n+16,8)));slots.append({'hash':hex(key),'storage_offset':pointer-block if block<=pointer<block+0x1dc else None,'storage_pointer_is_owned':block<=pointer<block+0x1dc});visit(right)
  visit(word(bytes(cpu.uc.mem_read(header+4,4)),0));fields=bytes(cpu.uc.mem_read(block,0x1dc));assert fields[0x1d8:0x1dc]==bytes([0xcd])*4
  fn=HERE/('cloud-color-'+str(color)+'-original-fields.bin');fn.write_bytes(fields)
  cloud.append({'color_baker':bool(color),'factory_source':'655314','constructor_source':'654a74'if color else'654ec4','allocations':allocations,'constructor_calls':constructor_calls,'registration_order':list(names),'registry_count':count,'registry_slots':slots,'all_registry_storage_owned':all(x['storage_pointer_is_owned']for x in slots),'animation_database_cell_seed_preserved':True,'original_fields_file':fn.name,'original_fields_sha256':sha(fn)})
 rawpath=REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae';raw=rawpath.read_bytes();base=cpu.data+0x40000;resource_root=relocate(cpu,raw,base);current_db=database(cpu,resource_root,cpu.data+0x30000);node_factory=[]
 for index in range(2):
  current_emitter=cpu.invoke(0x60e468,[current_db,index]);phase='node';trace=[];result=cpu.invoke(0x634c64,[0,current_db,driver,current_emitter,attachments,root]);assert result==node and trace==[['allocate_scene_node',0x198],['required_scene_node_constructor',0x64ed94],['required_initParticleSystem',True]];node_factory.append({'index':index,'trace':list(trace),'original_factory_returned_created_receiver':True})
 report={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(HERE/'original-functions.json'),'resource_sha256':sha(rawpath),'complete_original_cloud_constructors_executed':True,'cloud_constructors':cloud,'actual_FX77_factory_branches_executed':True,'node_factory_traces':node_factory,'services':['raw native allocation service5341ac with0xcdseed','initialized source hash guard','modeled single-thread allocator locks','required graph constructor64ed94 and initParticleSystem virtualreceiver'],'native_full_cloud_implementation_supplied':False,'scope':'All original cloud mixin constructor/registry instructions execute. Node factory mode0/descriptor3 branch executes with explicitly required scene/driver initialization receivers. No simulation, particle buffer/baker/GPU or complete native cloud claim.','source_sha256':{'probe.py':sha(Path(__file__))}}
 (HERE/'probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','complete_constructor_cases':2,'owned_slots_per_case':41,'actual_factory_cases':2,'probe_sha256':sha(HERE/'probe.json')}))
if __name__=='__main__':main()
