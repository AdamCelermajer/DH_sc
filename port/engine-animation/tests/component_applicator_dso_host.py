"""Build and prove actual DSO-linked component replay with compiler input hashes."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
WSL_REPO='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
TARGET='engine-skinning/engine-animation/component_applicator_audit'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(arguments):
 result=subprocess.run(['wsl',*arguments],capture_output=True,text=True)
 assert result.returncode==0,(arguments,result.stdout,result.stderr)
 return result
def wsl_sha(path):return run(['sha256sum',path]).stdout.split()[0]
def compiler_inputs(build):
 commands=run(['ninja','-C',build,'-t','commands',TARGET]).stdout
 inputs=set();objects=[]
 for line in commands.splitlines():
  tokens=shlex.split(line)
  if '-c' not in tokens:continue
  source=tokens[tokens.index('-c')+1];assert source.startswith(WSL_REPO+'/'),source
  inputs.add(source);objects.append(tokens[tokens.index('-o')+1])
 assert len(objects)>=12,len(objects)
 dependencies=run(['ninja','-C',build,'-t','deps',*objects]).stdout
 for line in dependencies.splitlines():
  name=line.strip()
  if name.startswith(WSL_REPO+'/'):inputs.add(name)
 sources={name[len(WSL_REPO)+1:]:sha(REPO/name[len(WSL_REPO)+1:]) for name in sorted(inputs)}
 sources['port/engine-animation/CMakeLists.txt']=sha(ROOT/'CMakeLists.txt')
 sources['port/scene-materials/CMakeLists.txt']=sha(REPO/'port/scene-materials/CMakeLists.txt')
 return sources,commands
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build-dir',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,default=ROOT/'reports/component-applicator-dso-host-audit.json');a=p.parse_args()
 kernel_path=ROOT/'reports/component-applicator-arm64-differential.json';kernel=json.loads(kernel_path.read_text());assert kernel['validation']=='PASS' and kernel['mismatches']==0
 for name,value in kernel['source_sha256'].items():assert sha(REPO/name)==value,name
 gold=ROOT/'reference/component-applicator/original-corpus.bin';assert sha(gold)==kernel['reference_sha256']
 before,commands=compiler_inputs(a.build_dir)
 build_result=run(['cmake','--build',a.build_dir,'--target','component_applicator_audit','-j','2'])
 sources,new_commands=compiler_inputs(a.build_dir);assert before==sources and commands==new_commands,'Compiler inputs changed during audited build'
 executable=a.build_dir+'/'+TARGET
 result=run(['--cd',WSL_REPO,'env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',executable,'port/engine-animation/reference/component-applicator/original-corpus.bin'])
 assert not result.stderr.strip(),result.stderr
 rows=[json.loads(line) for line in result.stdout.splitlines()];assert len(rows)==2
 audit,bindings=rows;assert audit['validation']=='PASS' and audit['cases']==1872 and audit['atomic_rejections']==14 and audit['world_refresh_checks']==1 and audit['scene_bridge_applies']==1248 and audit['sanitizer_findings']==0
 for name in ('cases','direct_applies','blended_applies','contributions'):assert audit[name]==kernel[name]
 assert bindings['actual_dso_symbols_checked']==7
 ldd=run(['ldd',executable]).stdout;assert 'libasan.so' in ldd and 'libubsan.so' in ldd
 libraries={name:{'path':bindings[name+'_library'],'sha256':wsl_sha(bindings[name+'_library'])} for name in ('animation','scene')}
 for library in libraries.values():assert library['path'] in ldd
 undefined=run(['nm','-D','--undefined-only',executable]).stdout
 for symbol in ('dh2_animation_component_apply','dh2_animation_component_apply_blended','dh2_animation_component_blend','dh2_animation_blend_vector3'):assert symbol in undefined,symbol
 after,_=compiler_inputs(a.build_dir);assert sources==after,'Compiler inputs changed during audited execution'
 sources['port/engine-animation/tests/component_applicator_dso_host.py']=sha(Path(__file__))
 report={'validation':'PASS','host_audit':audit,'actual_dso_linked':True,'actual_dso_symbols_checked':7,'libraries':libraries,'executable':executable,'executable_sha256':wsl_sha(executable),'source_sha256':sources,'compiler_input_count':len(sources)-3,'compiler_input_capture':'Actual Ninja compile command source files and .o dependency headers; hashes unchanged across build and run. Audit executable has undefined kernel symbols and runtime dladdr resolves genuine DSO owners.','compiler_commands_sha256':hashlib.sha256(commands.encode()).hexdigest(),'cmake_cache_sha256':wsl_sha(a.build_dir+'/CMakeCache.txt'),'build_output_sha256':hashlib.sha256(build_result.stdout.encode()).hexdigest(),'executable_undefined_symbols_sha256':hashlib.sha256(undefined.encode()).hexdigest(),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'original_instruction_evidence':{'original_sha256':kernel['original_sha256'],'manifest_sha256':kernel['manifest_sha256'],'kernel_report_sha256':sha(kernel_path),'reference_sha256':sha(gold),'factory_type_checks':kernel['factory_type_checks']},'scope':'Genuine production DSO-linked concrete component2/3/4 and11/12/13 contribution/application plus borrowed scene graph replay. Includes actual graph world update, malformed/type/alias atomic rejection and original-derived IEEE corpus. Production raw animation/angle exports are linked but not audited by this component corpus; their source hashes identify actual library inputs, not additional behavior parity. No Android/APK/GPU/whole-animation/runtime claim.'}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_audit':audit,'libraries':libraries,'compiler_input_count':report['compiler_input_count']}))
if __name__=='__main__':main()
