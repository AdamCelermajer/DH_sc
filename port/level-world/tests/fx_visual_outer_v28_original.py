"""Whole original VisualObject.SetRotation + scene-node relative TRS versus
current compiled ARM64 FX owner outer transform. Child authored poses and GPU
appearance remain separate; no emulator or application mutations."""
from pathlib import Path
import hashlib,json,sys,struct,math
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
import visual_motion_differential as visual_oracle
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_D0,UC_ARM64_REG_D1
class AngleCpu(visual_oracle.RotationCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  def double(index):return struct.unpack('<d',struct.pack('<Q',uc.reg_read(UC_ARM64_REG_D0 if index==0 else UC_ARM64_REG_D1)))[0]if self.arm64 else struct.unpack('<d',struct.pack('<II',self.reg(index*2),self.reg(index*2+1)))[0]
  if name=='asinf':
   value=struct.unpack('<f',struct.pack('<I',uc.reg_read(UC_ARM64_REG_S0)if self.arm64 else self.reg(0)))[0];raw=struct.unpack('<I',struct.pack('<f',math.asin(value)))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,raw)
   else:self.put(0,raw)
  elif name in ['atan2','fabs']:
   value=math.atan2(double(0),double(1))if name=='atan2'else abs(double(0));raw=struct.unpack('<Q',struct.pack('<d',value))[0]
   if self.arm64:uc.reg_write(UC_ARM64_REG_D0,raw)
   else:self.put(0,raw&0xffffffff);self.put(1,raw>>32)
  elif name and name.startswith('__aeabi_dcmp'):
   a,b=double(0),double(1);operation=name.removeprefix('__aeabi_dcmp');self.put(0,int(dict(eq=a==b,lt=a<b,gt=a>b,le=a<=b,ge=a>=b)[operation]))
  else:return super().external(uc,address,size,unused)
  uc.reg_write(self.pc,uc.reg_read(self.lr))
visual_oracle.RotationCpu=AngleCpu
Rotation=visual_oracle.Rotation
manifest=json.loads((ROOT/'reference/visual-motion/original-functions.json').read_text())
engine=REPO/'.local-inputs/libDungeonHunter2.so';library=REPO/'.local-inputs/fx-alignment-v28/libfx_visual_outer_v28.so'
assert hashlib.sha256(engine.read_bytes()).hexdigest()==manifest['original_sha256']
old=Rotation(engine,library,manifest);native=old.new;rows=[];anchor_cases=0
for degrees in range(0,360,15):
 for tilt in [(0.,0.),(.25,0.),(0.,-.25),(.15,-.12)]:
  for scale in [(1.,1.,1.),(1.2,.9,1.1)]:
   position=(113.,227.,331.);rotation=(*tilt,math.radians(degrees));raw=struct.pack('<3f',*rotation)
   q,_=old.execute(raw);c=old.old;node=old.root
   c.uc.mem_write(node+0xac,struct.pack('<3f',*position));c.uc.mem_write(node+0xb8,q);c.uc.mem_write(node+0xc8,struct.pack('<3f',*scale));c.uc.mem_write(node+0x11c,struct.pack('<I',14))
   matrix_at=c.invoke(0x598908,[node]);expected=bytes(c.uc.mem_read(matrix_at,65))
   out=native.data+0x1000;pp=native.data+0x2000;rp=native.data+0x3000;sp=native.data+0x4000
   for at,value in [(pp,position),(rp,rotation),(sp,scale)]:native.uc.mem_write(at,struct.pack('<3f',*value))
   assert native.invoke('dh2_fx_visual_outer_v28',[out,pp,rp,sp])==0
   actual=bytes(native.uc.mem_read(out,65));assert actual==expected,(degrees,tilt,scale,struct.unpack('<16f',expected[:64]),struct.unpack('<16f',actual[:64]))
   # Positive visual branch consumes SAME owner root, not raw Euler16c.
   degree_out=c.data+0x9000;c.invoke(0x432bbc,[degree_out,matrix_at]);values=struct.unpack('<3f',bytes(c.uc.mem_read(degree_out,12)));factor=struct.unpack('<f',struct.pack('<I',0x3c8efa35))[0];expected_rotation=struct.pack('<3f',*(v*factor for v in values))
   root=native.data+0x6000;rotation_out=native.data+0x7000;native.uc.mem_write(root,struct.pack('<3f',*position)+q+struct.pack('<3f',*scale)+bytes(56))
   assert native.invoke('dh2_fx_anchor_rotation_v28',[rotation_out,root])==0
   actual_rotation=bytes(native.uc.mem_read(rotation_out,12));assert actual_rotation==expected_rotation,(degrees,tilt,scale,actual_rotation.hex(),expected_rotation.hex());anchor_cases+=1
   rows.append(dict(heading_degrees=degrees,tilt=list(tilt),scale=list(scale),matrix65=actual.hex()))
report=dict(validation='PASS',cases=len(rows),anchor_rotation_cases=anchor_cases,bit_exact_bytes65=True,bit_exact_anchor_radians=True,original_entries=['VisualObject.SetRotation472874','ISceneNode.getRelativeTransformation598908','CMatrix4.getRotationDegrees432bbc'],
 original_sha256=hashlib.sha256(engine.read_bytes()).hexdigest(),native_sha256=hashlib.sha256(library.read_bytes()).hexdigest(),
 scope=__doc__,rows=rows)
out=ROOT/'reference/fx-alignment-v28/visual-outer-original.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k!='rows'}))
