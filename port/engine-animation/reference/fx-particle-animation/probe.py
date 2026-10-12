"""Actual type28 selection, forceBind/map lookup/insertion and attach lifetime."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,relocate,word
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class ProbeCpu(Cpu):
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='strcasecmp':
   left=self.string(self.reg(0)).lower();right=self.string(self.reg(1)).lower();self.put(0,((left>right)-(left<right))&0xffffffff);self.import_calls['strcasecmp']=self.import_calls.get('strcasecmp',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def main():
 manifest=json.loads((HERE/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';cpu=ProbeCpu(engine,False,manifest)
 raw=(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae').read_bytes();base=cpu.data+0x10000;relocate(cpu,raw,base);records=word(raw,word(raw,32)+40)
 instance=cpu.symbols['_ZN6glitch7collada15animation_track8CFloatEx10s_InstanceE'];vt=cpu.symbols['_ZTVN6glitch7collada15animation_track8CFloatExE'];cpu.pointer(instance,vt+8)
 factory=[]
 for i in (1,2):
  rec=records+i*32;assert cpu.invoke(0x611ae0,[base+rec])==instance;ch=word(raw,rec+16);uri=word(raw,ch+4);name=raw[uri:raw.index(0,uri)].decode();factory.append({'uri':name,'type':word(raw,ch+8),'instance':hex(instance),'vtable':hex(vt+8)})
 # Initialized source hash singleton, with actual hash/STL map instructions.
 cpu.pointer(0x99f698,1)
 root=cpu.data+0x2000;particle=root+0x400;pvt=particle+0x300;context=pvt+0x200;cvt=context+0x200;name=cvt+0x100;parameter=name+0x100
 cpu.pointer(particle,pvt);cpu.pointer(particle+0x178,context);cpu.pointer(context,cvt);cpu.pointer(cvt-12,0)
 header=context+0x30
 def clear_map():cpu.uc.mem_write(header,words([0,0,header,header,0]))
 clear_map();cpu.uc.mem_write(name,b'BirthRate\0');h=cpu.invoke(0x64d0bc,[context,name]);assert h==0x7d66d025
 assert cpu.invoke(0x651924,[particle,name])==0;node=word(bytes(cpu.uc.mem_read(header+4,4)),0);assert node and word(bytes(cpu.uc.mem_read(node+16,8)),0)==h
 assert word(bytes(cpu.uc.mem_read(node+16,8)),4)==0;assert word(bytes(cpu.uc.mem_read(header+16,4)),0)==1
 cpu.pointer(node+20,parameter);cpu.uc.mem_write(parameter,words([0x41700000]));assert cpu.invoke(0x651924,[particle,name])==parameter
 assert word(bytes(cpu.uc.mem_read(header+16,4)),0)==1
 # Actual PGenerationModel C2 registers its OWN float+4 storage under BirthRate.
 # VTT/virtual-base placement is an explicit constructor-layout fixture; the
 # field initialization, hash and unique STL insertion execute unchanged.
 generation=cpu.data+0x20000;generation_context=generation+0x100;vtt=generation+0x300;gvt=vtt+0x100;secondary=gvt+0x100
 cpu.pointer(vtt,gvt);cpu.pointer(vtt+4,secondary);cpu.pointer(gvt-12,0x100);gh=generation_context+0x30;cpu.uc.mem_write(gh,words([0,0,gh,gh,0]))
 cpu.invoke(0x6545d0,[generation,vtt]);cpu.pointer(particle+0x178,generation)
 owned=cpu.invoke(0x651924,[particle,name]);assert owned==generation+4 and word(bytes(cpu.uc.mem_read(owned,4)),0)==0x3f800000
 cpu.invoke(0x650400,[generation_context,name,0x42240000]);assert word(bytes(cpu.uc.mem_read(owned,4)),0)==0x42240000
 generation_before=bytes(cpu.uc.mem_read(generation,0x180));cpu.invoke(0x6e3258,[instance,parameter,owned,0]);generation_after=bytes(cpu.uc.mem_read(generation,0x180));assert generation_after[4:8]==words([0x41700000]) and generation_after[:4]+generation_after[8:]==generation_before[:4]+generation_before[8:]
 cpu.pointer(particle+0x178,context)
 animator=parameter+0x100;avt=animator+0x100;service=avt+0x100;head=root+0x158;item=service+0x100;resolved_uri=0;trace=[]
 cpu.pointer(animator,avt);cpu.pointer(animator+16,root);cpu.pointer(pvt+0x54,service+16);cpu.pointer(pvt+0xfc,0x651924)
 for off,fn in [(0x70,service),(0x6c,service+4),(0x54,service+8),(0x68,service+12)]:cpu.pointer(avt+off,fn)
 current_ch=0;current_uri=0
 def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 attachments=[];children=[];namesvec=service+0x600;namesptr=namesvec+0x100;attachroot=namesptr+0x200;lookupnode=attachroot+0x200;childvt=lookupnode+0x600;backendvt=childvt+0x200
 def hook(uc,a,s,u):
  if a==service:ret(1)
  elif a==service+4:ret(current_uri)
  elif a==service+8:ret(current_ch)
  elif a==service+12:
   trace.append(['setTarget',cpu.reg(1),cpu.reg(2)==parameter,cpu.reg(2)==0,cpu.reg(3)]);ret()
  elif a==service+16:trace.append(['particle_getName',cpu.string(resolved_uri).decode()]);ret(resolved_uri)
  elif a==0x64d0bc:trace.append(['hashString',cpu.string(cpu.reg(1)).decode()])
  elif a==0x5985e4:
   attachments.append(['findNode',cpu.string(cpu.reg(1)).decode()]);ret(lookupnode if cpu.string(cpu.reg(1))==b'Anchor' else 0)
  elif a==service+20:
   idx=children.index(cpu.reg(0));attachments.append(['getType',idx]);ret(0 if idx==0 else 0x66656164)
  elif a==service+24:
   idx=children.index(cpu.reg(0));assert cpu.reg(1)==particle;attachments.append(['attach',idx]);
   if idx==1:cpu.pointer(children[1]+4,lookupnode+0xf4) # Original rereads next after callback.
   ret()
  elif a==service+28:attachments.append(['destroyParticleSystem',cpu.reg(0)==context]);ret()
  elif a==0x6678b8:attachments.append(['baseDestructor',cpu.reg(0)==particle]);ret()
 cpu.uc.hook_add(UC_HOOK_CODE,hook)
 binding=[]
 for i in (1,2):
  current_ch=base+word(raw,records+i*32+16);current_uri=word(bytes(cpu.uc.mem_read(current_ch+4,4)),0);resolved_uri=current_uri
  for mode in ('found','missing_parameter','missing_particle'):
   if mode=='found':
    cpu.invoke(0x651924,[particle,name]);node=word(bytes(cpu.uc.mem_read(header+4,4)),0);cpu.pointer(node+20,parameter)
   else:clear_map()
   cpu.pointer(head,head if mode=='missing_particle' else item);cpu.pointer(item,head);cpu.pointer(item+8,particle);trace=[];cpu.invoke(0x667f18,[animator]);binding.append({'uri':cpu.string(current_uri).decode(),'mode':mode,'trace':list(trace)})
   if mode=='found':assert trace[-1]==['setTarget',0,True,False,0],trace
   else:assert trace[-1]==['setTarget',0,False,True,0]
   if mode=='missing_parameter':node=word(bytes(cpu.uc.mem_read(header+4,4)),0);assert node and word(bytes(cpu.uc.mem_read(node+20,4)),0)==0
 # Actual attachment vector allocation/zero fill, fixture root-name lookup and
 # virtual receivers. These services are intentionally NOT implemented no-ops.
 cpu.pointer(particle+0x150,namesvec);cpu.uc.mem_write(namesvec,words([2,namesptr]));cpu.pointer(namesptr,namesptr+0x40);cpu.pointer(namesptr+4,namesptr+0x60);cpu.uc.mem_write(namesptr+0x40,b'#Anchor\0');cpu.uc.mem_write(namesptr+0x60,b'#Missing\0');cpu.uc.mem_write(particle+0x164,bytes(12))
 children=[lookupnode+0x200,lookupnode+0x300,lookupnode+0x400];cpu.pointer(lookupnode+0xf4,children[0]+4)
 for j,c in enumerate(children):cpu.pointer(c,childvt);cpu.pointer(c+4,children[j+1]+4 if j+1<len(children) else lookupnode+0xf4)
 cpu.pointer(childvt+0xbc,service+20);cpu.pointer(childvt+0xf4,service+24);attachments=[];cpu.invoke(0x64f6f8,[particle,attachroot]);attachtrace=list(attachments)
 assert attachtrace==[['findNode','Anchor'],['getType',0],['getType',1],['attach',1],['findNode','Missing']]
 vector=bytes(cpu.uc.mem_read(particle+0x164,12));start,end,capacity=struct.unpack('<3I',vector);assert end-start==8 and capacity>=end and bytes(cpu.uc.mem_read(start,8))==bytes(8)
 cpu.pointer(context,backendvt);cpu.pointer(backendvt+8,service+28);attachments=[];cpu.invoke(0x64ecac,[particle]);assert attachments==[['destroyParticleSystem',True],['baseDestructor',True]]
 report={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(HERE/'original-functions.json'),'resource_sha256':hashlib.sha256(raw).hexdigest(),'factory_initialized_singleton_fixture':True,'factory_calls':factory,'source_key':'BirthRate','birthrate_hash':hex(h),'original_hash_and_stl_map_executed':True,'missing_parameter_inserts_null_pointer':True,'registered_parameter_returned_without_insertion':True,'binding_traces':binding,'actual_forceBind_and_particle_lookup_executed':True,'binding_services':['animator channel count/URI/channel getters and setTarget','particle name virtual getter'],'attach_trace':attachtrace,'attachment_vector_actual_zero_fill':True,'attachment_next_reloaded_after_callback':True,'attachment_services':['root-node name lookup5985e4','child type vBC and attach vF4'],'destructor_trace':attachments,'destructor_services':['owned particle-system deleting v8','base destructor6678b8'],'scope':'Selected CFloatEx and source binding/map/attachment orchestration. No real particle-system factory, emitter virtual implementation, simulation/baker/GPU or complete scene owner claimed.','source_sha256':{'probe.py':sha(Path(__file__))}}
 report['generation_model_original_ctor_executed']=True;report['generation_model_birthrate_owned_offset']=4;report['generation_model_birthrate_initial_bits']='0x3f800000';report['original_setParameter_writes_registered_storage']=True;report['original_CFloat_apply_writes_only_owned_birthrate']=True;report['constructor_layout_fixture']='VTT primary/secondary address points and virtual-base displacement; original constructor/hash/STL insertion execute.';report['libc_strcasecmp_contract']='ASCII lower-case byte comparison for authored ASCII URI names'
 (HERE/'probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
