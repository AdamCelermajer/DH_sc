"""Original placement conversion, quaternion/node matrix and marker bounds.

Imports use common IEEE/libm services on both CPUs. Mesh bounds are read from
the authored BRES fixture; original CSceneNode group bounds execute with the
factory-created identity mesh child. No original matrix or extent algorithm
is replaced. Complete Collada allocation/material/animation factories are
traced source boundaries, not executed by this bounded oracle.
"""
import argparse,ctypes,hashlib,importlib.util,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('engine_math_differential',ROOT.parent/'engine-math/tests/differential.py')
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
Cpu=module.Cpu
def pack(values):return struct.pack('<'+'f'*len(values),*values)
def same(a,b):
 return all(a[i:i+4]==b[i:i+4] or (math.isnan(struct.unpack('<f',a[i:i+4])[0]) and math.isnan(struct.unpack('<f',b[i:i+4])[0])) for i in range(0,len(a),4))
class Dependencies:
 def call(self,cpu,name):
  name=name.removeprefix('__aeabi_')
  if name in ('memcpy','memmove','memset'):
   dst,src,n=[cpu.reg(i) for i in range(3)];cpu.uc.mem_write(dst,bytes([src&255])*n if name=='memset' else bytes(cpu.uc.mem_read(src,n)));cpu.write_reg(0,dst)
  elif name.startswith(('fcmp','dcmp')):
   kind=name[0];a,b=[cpu.get_float(kind,i) for i in range(2)];comparison={'eq':a==b,'lt':a<b,'le':a<=b,'gt':a>b,'ge':a>=b,'un':math.isnan(a) or math.isnan(b)};cpu.write_reg(0,int(comparison[name[4:]]))
  else:
   kind='f' if name.startswith('f') or name.endswith('f') else 'd'
   if name=='f2d':kind='f'
   if name=='d2f':kind='d'
   a=cpu.get_float(kind,0)
   if name in ('f2d','d2f'):value=a
   elif name in ('fadd','fsub','fmul','fdiv','dadd','dsub','dmul','ddiv'):
    b=cpu.get_float(kind,1)
    if name.endswith('add'):value=a+b
    elif name.endswith('sub'):value=a-b
    elif name.endswith('mul'):value=a*b
    else:value=a/b if b else math.nan if not a or math.isnan(a) else math.copysign(math.inf,a)*math.copysign(1,b)
   elif name in ('sin','cos','sinf','cosf','sqrtf'):
    try:value={'sin':math.sin,'cos':math.cos,'sinf':math.sin,'cosf':math.cos,'sqrtf':math.sqrt}[name](a)
    except ValueError:value=math.nan
   else:raise AssertionError('unmodeled dependency '+name)
   outkind='d' if name=='f2d' else 'f' if name=='d2f' else kind
   cpu.put_float(ctypes.c_float(value).value if outkind=='f' else value,outkind)
  cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--engine',type=Path,required=True);parser.add_argument('--library',type=Path,required=True);parser.add_argument('--reference-output',type=Path,required=True);parser.add_argument('--report',type=Path,required=True);parser.add_argument('--cases',type=int,default=400);args=parser.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/decor-scene/original-functions.json').read_text());assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256'];deps=Dependencies();old=Cpu(args.engine,False,deps,manifest);new=Cpu(args.library,True,deps,{'functions':[]})
 for address in (0x38be84,0x472874,0x4727ac,0x59712c,0x598908,0x65cda8,0x47211c):old.symbols[str(address)]=address
 owner,visual,root,marker,parent,mesh,rootvt,markervt,meshvt,inputptr=[old.data+offset for offset in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0x9000,0xa000)]
 def pointer(at,value):old.uc.mem_write(at,struct.pack('<I',value))
 def returned(value=0):old.write_reg(0,value);old.uc.reg_write(old.pc_reg,old.uc.reg_read(old.lr_reg))
 phase=''
 def hook(uc,address,size,unused):
  if address==0x38be84:uc.reg_write(UC_ARM_REG_R4,owner)
  elif address==0x38bf30:uc.reg_write(old.pc_reg,old.stop)
  elif address==0x470a54 or (phase=='setters' and address==0x47211c):returned()
  elif address==0x597290:assert old.reg(0)==marker;returned(parent)
  elif address==old.extern+0xff00:assert old.reg(0)==mesh;returned(0x6d656164)
 old.uc.hook_add(UC_HOOK_CODE,hook)
 # Custom mesh type method lives in the normal mapped data region to avoid
 # imported dependency dispatch; geometry bounds and matrix use real methods.
 mesh_type=old.data+0xf000;old.uc.mem_write(mesh_type,struct.pack('<I',0xe12fff1e))
 def meshhook(uc,address,size,unused):assert old.reg(0)==mesh;returned(0x6d656164)
 old.uc.hook_add(UC_HOOK_CODE,meshhook,begin=mesh_type,end=mesh_type)
 for off,target in ((0x40,0x598908),(0x90,0x5970bc),(0x94,0x5970c4),(0x98,0x5970ec),(0x9c,0x5970f4)):pointer(rootvt+off,target)
 pointer(markervt+0x30,0x5839b0);pointer(meshvt+0xbc,mesh_type);pointer(meshvt+0x30,0x5839b0);pointer(meshvt+0x40,0x598908)
 identity=pack([1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1])+bytes((1,))
 records=[];authored=[];group_checks=0
 def compare(packed,label,check_group=False):
  nonlocal phase,group_checks
  values=struct.unpack('<18f',packed);old.uc.mem_write(owner,bytes(0x400));old.uc.mem_write(root,bytes(0x200));old.uc.mem_write(visual,bytes(0x100));old.uc.mem_write(marker,bytes(0x200));old.uc.mem_write(mesh,bytes(0x200));old.uc.mem_write(parent,bytes(0x200));old.uc.mem_write(inputptr,packed)
  old.uc.mem_write(owner+0x120,packed[24:36]);old.uc.mem_write(owner+0x16c,packed[12:24]);phase='conversion';old.invoke(str(0x38be84),[])
  effective=bytes(old.uc.mem_read(owner+0x120,12));radians=bytes(old.uc.mem_read(owner+0x16c,12))
  pointer(root,rootvt);pointer(visual+8,root);pointer(visual+12,marker);pointer(marker,markervt);pointer(mesh,meshvt)
  old.uc.mem_write(root+0xb8,pack([0,0,0,1]));old.uc.mem_write(root+0xc8,pack([1,1,1]));old.uc.mem_write(root+0x68,identity);pointer(root+0x11c,6)
  old.uc.mem_write(parent+0xc8,packed[60:72]);pointer(parent,rootvt)
  old.uc.mem_write(mesh+0x130,packed[36:60]);old.uc.mem_write(mesh+0x68,identity);pointer(marker+0xf4,mesh+4);pointer(mesh+4,marker+0xf4)
  phase='group';old.invoke(str(0x65cda8),[marker]);group=bytes(old.uc.mem_read(marker+0x130,24));assert same(group,packed[36:60]),(label,'identity geometry group bounds',group.hex(),packed[36:60].hex())
  if check_group:group_checks+=1
  phase='setters';old.invoke(str(0x59712c),[root,inputptr]);old.invoke(str(0x472874),[visual,owner+0x16c]);old.invoke(str(0x4727ac),[visual,owner+0x120]);old.invoke(str(0x598908),[root]);phase='mesh';old.invoke(str(0x47211c),[visual])
  expected=effective+radians+bytes(old.uc.mem_read(root+0xb8,16))+bytes(old.uc.mem_read(root+0x68,64))+bytes(old.uc.mem_read(visual+0x10,24));assert len(expected)==128
  new.uc.mem_write(new.data+0x1000,packed);assert new.invoke('dh2_decor_scene',[new.data+0x2000,new.data+0x1000])==0;actual=bytes(new.uc.mem_read(new.data+0x2000,128));assert same(expected,actual),(label,'scene',[(i//4,expected[i:i+4].hex(),actual[i:i+4].hex()) for i in range(0,128,4) if expected[i:i+4]!=actual[i:i+4]])
  records.append(packed+expected);return expected
 inventory=json.loads((ROOT/'reference/decor-body-config/crypt-colbox-inventory.json').read_text());templates=[]
 for model in inventory['models']:
  markers=model['colbox_nodes']
  if not markers:continue
  markerrow=markers[0];assert len(markerrow['instances'])==1;assert not any(n['parent']==markerrow['id'] for n in model['nodes']);parentrow=next(n for n in model['nodes'] if n['id']==markerrow['parent']);bounds=markerrow['instances'][0]['mesh_bounds'];templates.append({'model':model['model'],'bounds':bounds,'parent_scale':parentrow['scale'],'marker_name':markerrow['name'],'geometry_record':markerrow['instances'][0]['geometry_record']})
  for row in model['authored_instances']:
   packed=pack(row['position']+row['rotation_degrees']+row['scale']+bounds+parentrow['scale']);expected=compare(packed,row['name'],True);authored.append({'model':model['model'],'name':row['name'],'input_words':list(struct.unpack('<18I',packed)),'output_words':list(struct.unpack('<32I',expected)),'mesh_box':list(struct.unpack('<6f',expected[-24:]))})
 rng=random.Random(20261021)
 for i in range(args.cases):
  position=[rng.uniform(-20000,20000) for _ in range(3)];rotation=[rng.uniform(-720,720) for _ in range(3)];scale=[rng.choice([0,-0.0,0.00009999999747378752,-0.00009999999747378752,0.0001,-0.0001,1,-1,2]) if i%3==0 else rng.uniform(-3,3) for _ in range(3)];lower=[rng.uniform(-100,0) for _ in range(3)];upper=[rng.uniform(1,100) for _ in range(3)];compare(pack(position+rotation+scale+lower+upper+[rng.uniform(-2,2) for _ in range(3)]),('random',i))
 for angle in (0,-0.0,90,-90,180,270,360):
  for axis in range(3):
   rotation=[0.0]*3;rotation[axis]=angle;compare(pack([100,200,300]+rotation+[1,1,1]+[-10,-20,-30,10,20,30]+[1,1,1]),('axis',axis,angle))
 before=bytes(new.uc.mem_read(new.data+0x2000,128));assert new.invoke('dh2_decor_scene',[new.data+0x2000,0])&0xffffffff==0xffffffff;assert before==bytes(new.uc.mem_read(new.data+0x2000,128));assert new.invoke('dh2_decor_scene',[0,new.data+0x1000])&0xffffffff==0xffffffff
 template_records=[]
 for model in inventory['models']:
  name=model['model'].encode();template=next((t for t in templates if t['model']==model['model']),None)
  template_records.append(struct.pack('<2I',len(name),int(template is not None))+name+pack(template['bounds']+template['parent_scale'] if template else [0]*9))
 reference=struct.pack('<3I',0x31534344,len(records),len(template_records))+b''.join(records)+b''.join(template_records);args.reference_output.write_bytes(reference)
 (ROOT/'reference/decor-scene/crypt-placement-fixture.json').write_text(json.dumps({'templates':templates,'authored':authored},indent=2)+'\n')
 coverage={r['original_symbol']+'@'+r['elf_address']:sum(int(r['elf_address'],16)<=v<int(r['elf_address'],16)+r['size'] for v in old.seen) for r in manifest['functions']}
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(args.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'authored_placements':len(authored),'authored_group_bound_checks':group_checks,'authored_templates':len(templates),'mismatches':0,'original_coverage':coverage,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='original_coverage'}))
if __name__=='__main__':main()
