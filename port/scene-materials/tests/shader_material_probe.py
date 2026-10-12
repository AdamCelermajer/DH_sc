"""Read-only source GLES2 pass selection using exact cached GameSWF material."""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from shader_sources_differential import Cpu,sha,words
from compiled_transforms_differential import relocate
def main():
 mp=ROOT/'reference/shader-sources/original-functions.json';manifest=json.loads(mp.read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256'];cpu=Cpu(engine,False,manifest);cpu.pointer(0x99f698,1);p=REPO/'.local-inputs/fx-render-connection-discovery/gameswf_effects.bdae';raw=p.read_bytes();image=cpu.data+0x10000;relocate(cpu,raw,image);rows=[];observed=[]
 def hook(uc,at,size,user):
  if at==0x6e01b4:
   sp=uc.reg_read(cpu.sp);values=[cpu.reg(2),cpu.reg(3),*[struct.unpack('<I',uc.mem_read(sp+i*4,4))[0]for i in range(5)]];observed.append([cpu.string(v).decode('ascii')if v else None for v in values[:5]]);assert values[5:]==[0,0];cpu.pointer(cpu.reg(0),0);uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
 cpu.uc.hook_add(UC_HOOK_CODE,hook)
 for technique,at in [('default',0x974),('multiply',0x9f4),('screen',0xa74),('overlay',0xaf4)]:
  cpu.invoke(0x634b30,[cpu.data+0x1000,cpu.data+0x2000,image+at]);program,vs,vpre,fs,fpre=observed[-1];assert program==vs+vpre+fs+fpre;rows.append({'technique':technique,'pass_offset':at,'program_name':program,'vertex_file':vs,'vertex_preamble':vpre,'fragment_file':fs,'fragment_preamble':fpre,'raw_pass_sha256':hashlib.sha256(raw[at:at+128]).hexdigest(),'raw_render_state_64_hex':raw[at+28:at+92].hex(),'source_sampler_mapping':{'TextureSampler':0}})
 result={'validation':'PASS','original_sha256':sha(engine),'original_manifest_sha256':sha(mp),'resource_sha256':sha(p),'resource_size':len(raw),'source_function':'SProfileGLES2Traits.createShader634b30','original_instructions_executed':True,'actual_resource_passes':rows,'constructor_capture':'render_handler_glitch7d60f4','source_constructor_registration':{'resource':'gameswf_effects.bdae','material':'_1_-_Default-fx','mode_to_technique':{'0':'default','1':'default','3':'multiply','4':'screen','13':'overlay','15':'default','16':'default'}},'services':['IShaderManager.createShader request observer returning null','original serialized BRES relocation','initialized allocation singleton'],'scope':'Actual GLES2 pass selection/string concatenation and raw authored state evidence. No GL driver/render-state execution, SWF display-list drawing or complete material factory claim.'};out=ROOT/'reference/shader-sources/gameswf-material-probe.json';out.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
