"""Actual context database cell and streaming block retain/release branches."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu,words
from compiled_transforms_differential import word
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 manifest=HERE/'original-functions.json';original=REPO/'.local-inputs/libDungeonHunter2.so';cpu=FactoryCpu(original,False,json.loads(manifest.read_text()));cpu.pointer(0x99f698,1);ctx=cpu.data+0x1000;name=ctx+0x1000;cpu.uc.mem_write(name,b'AnimationDatabase\0');trace=[];services=[]
 def hook(uc,at,n,user):
  if at in (0x64d1d0,0x64fcbc,0x64d298,0x63a1c4,0x60c598,0x60bbd4):trace.append(hex(at))
  if at in (0x60c31c,0x60bad0):services.append(hex(at));uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
 cpu.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
 for seed,new in [(0xcdcdcdcd,0xdeadc0de),(0,0),(0x12345678,0xffffffff)]:
  cpu.uc.mem_write(ctx,bytes(92));cpu.pointer(ctx+0x58,seed);cpu.invoke(0x64d1d0,[ctx]);assert word(bytes(cpu.uc.mem_read(ctx+0x58,4)),0)==seed
  cpu.invoke(0x64fcbc,[ctx,name,new]);assert word(bytes(cpu.uc.mem_read(ctx+0x58,4)),0)==new
  cpu.invoke(0x64d298,[ctx]);assert word(bytes(cpu.uc.mem_read(ctx+0x58,4)),0)==new and word(bytes(cpu.uc.mem_read(ctx+0x40,4)),0)==0;cases.append({'seed':hex(seed),'set_pointer':hex(new),'no_database_dereference_or_release':True,'registry_cleared':True})
 block=ctx+0x2000;neighbor=block+0x100;block_cases=[]
 for count in (0,1,2,3,0xffffffff):
  for linked in (0,neighbor):
   cpu.uc.mem_write(block,words([count,0,0,0,0,0,linked,0]));services.clear();cpu.invoke(0x60c598,[block]);after=word(bytes(cpu.uc.mem_read(block,4)),0);assert after==(count+1)&0xffffffff;assert services==(['0x60c31c']if count==1 and linked==0 else[]);block_cases.append({'operation':'add_ref','count':count,'previous_link_present':bool(linked),'after':after,'required_services':list(services)})
 for count in (1,2,3,4):
  for nextcount in (None,1,2):
   for prevcount in (None,1,2):
    cpu.uc.mem_write(block,words([count,0,0,0,0,0,neighbor if prevcount else 0,neighbor+32 if nextcount else 0]));cpu.pointer(neighbor,prevcount or 0);cpu.pointer(neighbor+32,nextcount or 0);services.clear();cpu.invoke(0x60bbd4,[block]);after=word(bytes(cpu.uc.mem_read(block,4)),0);assert after==(count-1)&0xffffffff
    detach=count==2 and (nextcount is None or nextcount==1 or prevcount==1);assert services==(['0x60bad0']if detach else[]);block_cases.append({'operation':'release','count':count,'next_count':nextcount,'previous_count':prevcount,'after':after,'required_services':list(services)})
 report={'validation':'PASS','original_sha256':sha(original),'manifest_sha256':sha(manifest),'source_probe_sha256':sha(Path(__file__)),'context_cases':cases,'block_cases':block_cases,'ordered_original_calls':trace,'source_block_reference_functions_are_thunks':True,'original_instructions_executed':True,'scope':'Raw AnimationDatabase storage is assigned without retention by setParameter<SAnimation*>. Context destructor clears registry and particle buffer, never dereferences/releases+58. Streaming block add/ref release thunks execute actual count/link branches; deeper manager link/unlink60c31c/60bad0 are required service fixtures. External type28 owned BRES sampling is a separate native owner.'};(HERE/'probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','context_cases':len(cases),'block_reference_cases':len(block_cases)}))
if __name__=='__main__':main()
