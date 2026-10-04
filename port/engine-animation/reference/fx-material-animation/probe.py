"""Actual FX77 factory, material metadata and SelfIllum virtual dispatch discovery."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,relocate,word
def words(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def main():
 manifest=json.loads((HERE/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';cpu=Cpu(engine,False,manifest)
 raw=(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae').read_bytes();base=cpu.data+0x10000;root=relocate(cpu,raw,base);record=word(raw,word(raw,32)+40)
 # Initialized C++ singleton projection, retaining actual selected typed vtable.
 matches=[(n,a)for n,a in cpu.symbols.items()if 'CApplyValueExIA4_h' in n and 'Li3Eh' in n and 'getInstance' in n]
 instance=next(a for n,a in matches if n.startswith('_ZZ'));guard=next(a for n,a in matches if n.startswith('_ZGV'))
 vt=next(a for n,a in cpu.symbols.items()if n.startswith('_ZTV')and 'CApplyValueExIA4_h' in n and 'Li3Eh' in n)
 cpu.pointer(instance,vt+8);cpu.pointer(guard,1)
 selected=cpu.invoke(0x611ae0,[base+record]);assert selected==instance
 fptr=word(bytes(cpu.uc.mem_read(selected,4)),0);slots={hex(o):hex(word(bytes(cpu.uc.mem_read(fptr+o,4)),0))for o in range(0,0x98,4)}
 node=cpu.data+0x2000;pos=node+0x800;quat=pos+16;scale=quat+16
 cpu.uc.mem_write(pos,bytes(16));cpu.uc.mem_write(quat,words([0,0,0,0x3f800000]));cpu.uc.mem_write(scale,words([0x3f800000]*3))
 cpu.invoke(0x599268,[node,123,pos,quat,scale]);primary=word(bytes(cpu.uc.mem_read(node,4)),0)
 assert primary==cpu.symbols['_ZTVN6glitch5scene10ISceneNodeE']+0x1c
 assert word(bytes(cpu.uc.mem_read(primary+0x88,4)),0)==0x5970b0
 visits=[]
 def observe(uc,a,s,u):
  if a in (0x5970b0,0x6461cc):visits.append({'function':hex(a),'node':cpu.reg(0)-node})
 cpu.uc.hook_add(UC_HOOK_CODE,observe)
 # An actual base node with two original-layout children. getMaterialCount has
 # no effects; the traversal source reads the intrusive next after recursion.
 children=[node+0x200,node+0x400]
 for c in children:cpu.uc.mem_write(c,bytes(0x180));cpu.pointer(c,primary);cpu.pointer(c+0xf4,c+0xf4)
 cpu.pointer(node+0xf4,children[0]+4);cpu.pointer(children[0]+4,children[1]+4);cpu.pointer(children[1]+4,node+0xf4)
 before=[bytes(cpu.uc.mem_read(c,0x180))for c in [node,*children]]
 cpu.invoke(0x50e398,[node]);assert visits==[{'function':'0x5970b0','node':x-node}for x in [node,*children]]
 assert before==[bytes(cpu.uc.mem_read(c,0x180))for c in [node,*children]]
 # Actual forceBind/parameter-index lookup, with explicit search/intern/target
 # services. The named runtime material directory is a fixture, not a loader.
 animator=node+0x1000;avt=animator+0x100;service=avt+0x200;material=service+0x200;mh=material+0x100;definitions=mh+0x100;intern=definitions+0x100;captured=intern+0x100
 ch=word(raw,record+16);uri=base+word(raw,ch+4);name=base+word(raw,ch+12)
 cpu.pointer(animator,avt);cpu.pointer(animator+16,node);cpu.pointer(material+4,mh);cpu.pointer(mh+32,definitions);cpu.uc.mem_write(mh+14,struct.pack('<H',1));cpu.pointer(definitions,intern);cpu.uc.mem_write(intern+4,b'diffuse-color\0')
 for off,fn in [(0x70,service),(0x6c,service+4),(0x54,service+8),(0x68,service+12)]:cpu.pointer(avt+off,fn)
 traces=[];mode=0;clone_pointer=captured+0x100
 def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def binding_service(uc,a,s,u):
  if a==service:ret(1)
  elif a==service+4:ret(uri)
  elif a==service+8:ret(base+ch)
  elif a==0x65ca30:
   assert cpu.reg(1)==node and cpu.string(cpu.reg(2))==b'Standard_13' and cpu.reg(3)==0
   traces.append(['find_material','Standard_13',mode!=2]);cpu.pointer(cpu.reg(0),material if mode!=2 else 0);ret(cpu.reg(0))
  elif a==0x6a5074:
   assert cpu.string(cpu.reg(0))==b'diffuse-color' and cpu.reg(1)==0
   traces.append(['intern','diffuse-color']);ret(intern)
  elif a==service+12:
   pointer=cpu.reg(3);traces.append(['set_target',cpu.reg(1),bool(cpu.reg(2)),word(bytes(cpu.uc.mem_read(pointer,12)),8)if pointer else None]);
   if pointer:cpu.uc.mem_write(captured,bytes(cpu.uc.mem_read(pointer,12)))
   ret()
  elif a==0x5341ac:assert cpu.reg(0)==12;ret(clone_pointer)
 cpu.uc.hook_add(UC_HOOK_CODE,binding_service)
 binding=[]
 for mode in range(3):
  traces=[];cpu.pointer(material,1000);cpu.pointer(intern,1000);cpu.pointer(definitions,intern if mode==0 else intern+0x80)
  cpu.invoke(0x667f18,[animator]);binding.append({'mode':['named_parameter','missing_parameter','missing_material'][mode],'trace':traces})
  if mode!=2:
   expected=0 if mode==0 else 65535;assert traces[-1]==['set_target',0,True,expected]
   original=bytes(cpu.uc.mem_read(captured,12));cloned=cpu.invoke(0x667ed0,[captured]);assert cloned==clone_pointer and bytes(cpu.uc.mem_read(cloned+4,8))==original[4:];binding[-1]['clone_owned_fields_match']=True
   if mode==1:
    color=captured+0x200;cpu.uc.mem_write(color,b'\x01\x02\x03\x04');before=bytes(cpu.uc.mem_read(material,64));result=cpu.invoke(0x5cad38,[material,65535,0,color]);assert result==0 and before==bytes(cpu.uc.mem_read(material,64));binding[-1]['missing_parameter_setter_return_false_unchanged']=True
  else:assert traces==[['find_material','Standard_13',False],['set_target',0,False,None]]
 z=lambda o:raw[o:raw.index(0,o)].decode();r=word(raw,32);rows=[]
 for i in range(word(raw,r+92)):
  p=word(raw,r+96)+36*i;params=[]
  for j in range(word(raw,p+16)):
   q=word(raw,p+20)+24*j;params.append({'name':z(word(raw,q)),'type':word(raw,q+8),'count':word(raw,q+12),'element_counts_offset':word(raw,q+16),'element_counts':[word(raw,word(raw,q+16)+k*4)for k in range(word(raw,q+12))],'data_offset':word(raw,q+20)})
  rows.append({'id':z(word(raw,p)),'effect_uri':z(word(raw,p+12)),'raw_material_type':word(raw,p+28),'parameters':params})
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'resource_sha256':hashlib.sha256(raw).hexdigest(),'factory_executed':True,'factory_result':'uchar4 material component3','factory_instance':hex(instance),'factory_vtable':hex(vt),'typed_slots':slots,'material_parameters':rows,'selfillum_original_ctor_executed':True,'selfillum_primary_address_point':hex(primary),'selfillum_virtual_88':'ISceneNode::getMaterialCount5970b0','selfillum_visits':visits,'selfillum_node_bytes_unchanged':True,'factory_guard_fixture':'initialized static singleton only; factory selection instructions execute','particle_type28_supported':False}
 report['binding_original_forceBind_and_parameter_lookup_executed']=True;report['binding_traces']=binding;report['binding_services_explicit']=['material search65ca30 result','interned name6a5074 result','setTarget receiver/clone allocation5341ac'];report['source_sha256']={'probe.py':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()};report['original_manifest_sha256']=hashlib.sha256((HERE/'original-functions.json').read_bytes()).hexdigest()
 (HERE/'probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()


