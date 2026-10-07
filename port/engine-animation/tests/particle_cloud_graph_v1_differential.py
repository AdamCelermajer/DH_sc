from pathlib import Path
import sys,struct,random,json,hashlib
sys.path.insert(0,str(Path(__file__).parent))
exec(Path(__file__).with_name('particle_cloud_models_v1_differential.py').read_text().split('rng=random.Random')[0])
class GraphCpu(ModelCpu):
 def external(self,uc,a,size,user):
  # Emulator allocator lifetime boundary only; real shared ownership and
  # deletion are independently exercised under both host sanitizers.
  if self.imports.get(a)=='_ZNSt6__ndk119__shared_weak_count14__release_weakEv':uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,user)
new=GraphCpu(REPO/'.local-inputs/particle_cloud_graph_v1_arm64.so',True,{'functions':[]})
# A reserved-capacity vector fixture receives actual source generation. Every
# reached model, gravity, compaction, resize, sort and bounds method executes
# original instructions. No source allocation/material success is mocked.
base=old.data+0x10000;ctx=base;vtable=base+0x100;seed_word=base+0x300;world=base+0x400;ambient=base+0x500;particles=base+0x1000;dispatch=base+0x5000
receiver={name:base+0x6000+0x200*i for i,name in enumerate(['life','color','size','emitter','motion','spin','gravity','render','generation'])}
for name,addr in receiver.items():old.pointer(addr,vtable);old.pointer(vtable-12,0)
# Every model borrows this sole context via its actual virtual-base offset.
for addr in receiver.values():
 model_vt=addr+0x100;old.pointer(addr,model_vt);old.pointer(model_vt-12,(ctx-addr)&0xffffffff);old.pointer(model_vt+0x18,dispatch+0x100);old.pointer(model_vt+0x20,dispatch+0x104)
old.pointer(ctx,vtable);old.pointer(vtable-12,0)
old.pointer(vtable+0x18,dispatch+0x100);old.pointer(vtable+0x20,dispatch+0x104)
targets={0x80:('life',0x64c2fc),0x3c:('color',0x64bf00),0x30:('size',0x64bd1c),0x48:('emitter',0x64c08c),0x68:(None,0x64c234),0x50:('motion',0x653bb8),0x74:('spin',0x64e3c8),0x8c:(None,0x64c504),0x84:('life',0x64c3f4),0x40:('color',0x64e830),0x34:('size',0x64be08),0x6c:('gravity',0x633624),0x54:('motion',0x64c190),0x78:('spin',0x64c25c),0x90:('render',0x651eec)}
for slot in [0x28,*targets]:old.pointer(vtable+slot,dispatch+slot)
domain=base+0x8000;gravity_params=domain+0x100
old.pointer(receiver['emitter']+4,domain);old.pointer(receiver['gravity'],gravity_params)
new_first=0;new_end=0
def graph_callback(uc,a,size,user):
 if a==dispatch+0x100:old.put(0,seed_word);uc.reg_write(old.pc,uc.reg_read(old.lr));return
 if a==dispatch+0x104:old.put(0,world);uc.reg_write(old.pc,uc.reg_read(old.lr));return
 slot=a-dispatch
 if slot==0x28:old.put(0,receiver['generation']);uc.reg_write(old.pc,0x65317c);return
 if slot in targets:
  name,target=targets[slot]
  if name:old.put(0,receiver[name])
  if slot==0x6c:old.put(3,ctx)
  uc.reg_write(old.pc,target)
old.uc.hook_add(UC_HOOK_CODE,graph_callback)
rng=random.Random(0xF0112026);records=[]
for trial in range(200):
 n=rng.randrange(31);k=rng.randrange(31-n);seed_value=rng.randrange(1,2147483647)
 life=struct.pack('<2f',rng.choice([0.2,0.5,2]),0.1);size=struct.pack('<4f',200,0.2,0.064,0);motion=struct.pack('<6f',0,0,1,0.3,360,0.15);spin=struct.pack('<4fI4f',0,6.66,1.575,1,0,0,0,0,0);sphere=struct.pack('<6fI',0,0,0,2,0,2,0);models=life+size+motion+spin+sphere;assert len(models)==112
 matrix_bytes=identity;color=0xffffffff;camera=struct.pack('<3f',3,0,20);untouched=bytes([0xcd])*100;existing=bytearray(n*100)
 for i in range(n):
  struct.pack_into('<25f',existing,i*100,*[rng.uniform(-10,10)for _ in range(25)]);struct.pack_into('<2f',existing,i*100+0x3c,rng.uniform(-2,2),rng.uniform(0.1,3))
 old.uc.mem_write(ctx+4,bytes(92-4));old.uc.mem_write(seed_word,words([seed_value]));old.uc.mem_write(world,matrix_bytes+bytes([0]));old.uc.mem_write(ambient,words([color]));old.uc.mem_write(particles,bytes(existing)+untouched*k);new_first=particles+100*n;new_end=particles+100*(n+k)
 old.pointer(ctx+0x24,particles);old.pointer(ctx+0x28,particles+100*n);old.pointer(ctx+0x2c,particles+3000);old.uc.mem_write(receiver['generation']+4,struct.pack('<fIfI',float(k),30,0,0))
 old.uc.mem_write(receiver['life']+4,life);old.uc.mem_write(receiver['size']+4,size);old.uc.mem_write(receiver['color']+4,bytes(36));old.uc.mem_write(receiver['motion']+4,motion);old.uc.mem_write(receiver['spin']+4,spin[:16]);old.uc.mem_write(receiver['spin']+0x14,spin[20:32]);old.uc.mem_write(receiver['spin']+0x20,spin[32:36]);old.uc.mem_write(receiver['spin']+0x24,spin[16:20]);old.uc.mem_write(receiver['render']+0x60,camera)
 old.uc.mem_write(domain+0x80,bytes(12));old.invoke(0x69bb80,[domain,domain+0x80,0x40000000,0],budget=100000)
 old.pointer(gravity_params,world);old.uc.mem_write(gravity_params+4,struct.pack('<2fI',1,0,0));old.invoke(0x6532cc,[ctx,0x3f800000,ambient],budget=5000000)
 count=(u32(ctx+0x28)-particles)//100;expected=words([u32(seed_word),count])+bytes(old.uc.mem_read(receiver['render']+0x74,24))+bytes(old.uc.mem_read(particles,count*100));payload=words([n,k,seed_value])+models+matrix_bytes+camera+words([color])+untouched+bytes(existing)
 new.uc.mem_write(ni,payload);length=new.invoke('dh2_particle_cloud_graph_test_v1',[ni,no],budget=5000000);actual=bytes(new.uc.mem_read(no,length));assert actual==expected,(trial,n,k,len(actual),len(expected),next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None))
 records.append(words([len(payload)])+payload+words([len(expected)])+expected)
out=ROOT/'reference/particle-cloud-models-v1';gold=out/'graph-fixtures.bin';gold.write_bytes(words([0x31475250,len(records)])+b''.join(records));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();report=dict(validation='PASS',comparisons=len(records),original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),arm64_sha256=sha(REPO/'.local-inputs/particle_cloud_graph_v1_arm64.so'),reference_sha256=sha(gold),scope='Whole original update6532cc reached Life/Color/Size/actual sphereEmitter/Motion variation/Spin/directionalGravity/compaction/vector resize/AlphaSort/BBox vs same native graph. Actual source generation65317c and update6532cc run over a reserved-capacity vector fixture; every reached model and sort/BBox runs original instructions. Allocator cleanup is an explicit ABI boundary; no material/live renderer claim.')
(ROOT/'reports/particle-cloud-graph-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
