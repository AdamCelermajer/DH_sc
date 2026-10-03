"""Original affine palette and key interpreters versus compiled ARM64 source.

Libc and imported soft-float/libm dependencies are modeled identically.
The original accessor and quaternion blender execute, with no getter mocked.
This validates arithmetic, not the original GPU shader or full game.
"""
import argparse,ctypes,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../scene-materials/tests'))
from transform_differential import Cpu as Parent,pack,f
def bits(x):return struct.unpack('<I',pack([x]))[0]
class Cpu(Parent):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address,'');short=name.removeprefix('__aeabi_')
  if short in ('fdiv','fcmplt','fcmple','fcmpgt','fcmpge'):
   a,b=f(self.reg(0)),f(self.reg(1))
   result=bits(a/b) if short=='fdiv' else int({'fcmplt':a<b,'fcmple':a<=b,'fcmpgt':a>b,'fcmpge':a>=b}[short])
   self.put(0,result)
  elif name in ('acosf','sinf','sqrtf'):
   library=ctypes.CDLL('ucrtbase');function=getattr(library,name);function.argtypes=[ctypes.c_float];function.restype=ctypes.c_float
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);value=bits(function(f(raw)))
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,value)
   else:self.put(0,value)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 skin=json.loads((ROOT/'original-functions.json').read_text());animation=json.loads((ROOT/'../engine-animation/original-functions.json').read_text());manifest={'functions':skin['functions']+animation['functions']}
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==skin['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261002)
 matrix_cases=200;quaternion_cases=0;position_cases=0;palette_cases=200
 for i in range(matrix_cases):
  left=[rng.uniform(-10,10) for _ in range(16)];right=[rng.uniform(-10,10) for _ in range(16)]
  for m in (left,right):m[3]=m[7]=m[11]=0;m[15]=1
  dst,lp,rp=[old.data+x for x in (0x1000,0x2000,0x3000)];old.uc.mem_write(lp,pack(left)+b'\0'*4);old.uc.mem_write(rp,pack(right));old.invoke(0x6651a0,[dst,lp,rp]);expected=bytes(old.uc.mem_read(dst,64))
  dst,lp,rp=[new.data+x for x in (0x1000,0x2000,0x3000)];new.uc.mem_write(lp,pack(left));new.uc.mem_write(rp,pack(right));new.invoke('dh2_skin_matrix',[dst,lp,rp]);actual=bytes(new.uc.mem_read(dst,64));assert actual==expected,('palette',i,expected.hex(),actual.hex())
 # Real prepareCache instruction range composes all three matrices. This
 # catches nonidentity bind-shapes omitted by a two-matrix-only comparison.
 for i in range(palette_cases):
  matrices=[[rng.uniform(-10,10) for _ in range(16)] for _ in range(3)]
  for m in matrices:m[3]=m[7]=m[11]=0;m[15]=1
  obj,skinptr,cache,targets,joint,bind,tmp1,tmp2=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000)]
  old.pointer(obj+12,skinptr);old.pointer(skinptr+4,bind);old.uc.mem_write(skinptr+16,pack(matrices[2]));old.pointer(cache+4,tmp2);old.pointer(targets,joint);old.uc.mem_write(joint,pack(matrices[0])+b'\0'*4);old.uc.mem_write(bind,pack(matrices[1]))
  for reg,value in {2:targets,3:cache,4:0,5:obj,6:0,7:tmp2,8:tmp1}.items():old.put(reg,value)
  old.uc.reg_write(old.sp,old.stack+0xe000);old.uc.emu_start(0x66fee0,0x66ff14,count=100000);assert old.uc.reg_read(old.pc)==0x66ff14;expected=bytes(old.uc.mem_read(tmp2,64))
  dst,wp,ip,bp=[new.data+x for x in (0x1000,0x2000,0x3000,0x4000)]
  for ptr,m in zip((wp,ip,bp),matrices):new.uc.mem_write(ptr,pack(m))
  new.invoke('dh2_skin_palette_matrix',[dst,wp,ip,bp]);actual=bytes(new.uc.mem_read(dst,64));assert actual==expected,('bind shape palette',i,expected.hex(),actual.hex())
 # Execute the original position-only vertex kernel in its real instruction
 # range. Caller-provided buffers replace the runtime GL-buffer setup only.
 # Its 68-byte runtime matrix stride is distinct from serialized matrices.
 for i in range(100):
  matrices=[]
  for j in range(4):
   m=[rng.uniform(-10,10) for _ in range(16)];m[3]=m[7]=m[11]=0;m[15]=1;matrices.append(m)
  count=1+i%4;weights=[rng.random() for _ in range(count)];total=sum(weights);weights=[w/total for w in weights]
  if i%10==0:weights=[1.]+[0.]*(count-1)
  if i%10==1:weights=[0.] * count
  indices=bytes(rng.randrange(4) for _ in range(4));point=[rng.uniform(-10,10) for _ in range(3)]
  obj,skinptr,cache,palette,influences,source,dest=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000)]
  old.uc.mem_write(obj,b'\0'*32);old.pointer(obj+12,skinptr);old.pointer(obj+16,cache);old.uc.mem_write(skinptr+152,bytes([count]));old.pointer(cache+4,palette)
  old.uc.mem_write(palette,b''.join(pack(m)+b'\0'*4 for m in matrices));old.uc.mem_write(influences,indices+pack(weights));old.uc.mem_write(source,pack(point))
  sp=old.stack+0xe000;old.uc.reg_write(old.sp,sp)
  for slot,value in {0x64:obj,0x54:influences+4,0x58:source,0x70:1,0x20:0,0x50:dest}.items():old.pointer(sp+slot,value)
  old.uc.emu_start(0x6705c8,0x6708bc,count=100000);assert old.uc.reg_read(old.pc)==0x6708bc
  expected=bytes(old.uc.mem_read(dest,12))
  dst,mp,ip,wp,pp=[new.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000)]
  new.uc.mem_write(mp,pack(sum(matrices,[])));new.uc.mem_write(ip,indices);new.uc.mem_write(wp,pack(weights));new.uc.mem_write(pp,pack(point))
  new.invoke('dh2_skin_point',[dst,mp,ip,wp,count,pp]);actual=bytes(new.uc.mem_read(dst,12));assert actual==expected,('skin vertex',i,expected.hex(),actual.hex())
 accessor,record,sampler,data,values,out=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000)]
 old.uc.mem_write(accessor,struct.pack('<4I',record,data,0,0));old.uc.mem_write(record,b'\0'*32);old.pointer(record+8,sampler);old.uc.mem_write(sampler,b'\0'*28);old.uc.mem_write(data,struct.pack('<III',1,2,values))
 for kind,address,symbol,width in [('position',0x6286cc,'dh2_animation_lerp3',3),('quaternion',0x613294,'dh2_animation_quaternion',4)]:
  for i in range(100):
   left=[rng.uniform(-1,1) for _ in range(width)];right=[rng.uniform(-1,1) for _ in range(width)]
   if width==4:
    for v in (left,right):norm=math.sqrt(sum(x*x for x in v));v[:]=[x/norm for x in v]
   fraction=(0,.001,.25,.5,.75,.999,1)[i%7]
   old.uc.mem_write(values,pack(left+right));old.invoke(address,[accessor,0,0,bits(fraction),out]);expected=bytes(old.uc.mem_read(out,width*4))
   dst,lp,rp=[new.data+x for x in (0x1000,0x2000,0x3000)];new.uc.mem_write(lp,pack(left));new.uc.mem_write(rp,pack(right));new.uc.reg_write(UC_ARM64_REG_S0,bits(fraction));new.invoke(symbol,[dst,lp,rp]);actual=bytes(new.uc.mem_read(dst,width*4));assert actual==expected,(kind,i,expected.hex(),actual.hex())
   if width==4:quaternion_cases+=1
   else:position_cases+=1
 report={'original_sha256':skin['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'affine_matrix_bit_exact_cases':matrix_cases,'three_matrix_palette_bit_exact_cases':palette_cases,'skinned_vertex_bit_exact_cases':100,'zero_influence_cases':10,'position_bit_exact_cases':position_cases,'quaternion_bit_exact_cases':quaternion_cases,'dependency_model':'UCRT float libm shared across CPUs; Python soft-float rounded to float32','scope':'original matrix and animation routines plus isolated prepareCache and vertex instruction ranges execute; no full game or GPU equivalence'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
