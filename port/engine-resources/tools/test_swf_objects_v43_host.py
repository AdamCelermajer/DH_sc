"""Tiny fake GL executes exact staged SWF upload/stream and shader lifecycle bodies."""
from pathlib import Path
import hashlib,json,runpy,subprocess
root=Path(__file__).resolve().parents[3];runpy.run_path(str(root/'port/engine-resources/tools/prepare_swf_objects_v43.py'))
out=root/'port/engine-resources/reports/swf-objects-v43';layout=out/'test-layout'
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
copies={
 'engine-resources/resource_budget_v37.hpp':out/'resource_budget_v37.hpp',
 'engine-resources/resource_budget_v37.cpp':out/'resource_budget_v37.cpp',
 'engine-resources/cpu_vector_capacity_v41.hpp':root/'port/engine-resources/cpu_vector_capacity_v41.hpp',
 'engine-resources/gpu_object_owner_v43.hpp':root/'port/engine-resources/gpu_object_owner_v43.hpp',
 'engine-ui/swf_vertex_cache_v36.hpp':out/'swf_vertex_cache_v36.hpp',
 'engine-ui/swf_vertex_cache_v36.cpp':out/'swf_vertex_cache_v36.cpp',
 'android-native/app/src/main/cpp/authored_shader_program.hpp':out/'authored_shader_program.hpp',
 'engine-resources/tests/swf_objects_consumer_v43.cpp':root/'port/engine-resources/tests/swf_objects_consumer_v43.cpp'}
for path,src in copies.items():
 dst=layout/path;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(src.read_bytes())
swf=(out/'swf_gpu.cpp').read_text();shader=(out/'authored_shader_program.cpp').read_text()
upload=swf[swf.index('    const auto upload=[this]'):swf.index('    const auto release=[this]')]
upload=upload.replace('    const auto upload=','return ',1)
methods=swf[swf.index('void SwfGpu::release_vertices_v43'):swf.index('void SwfGpu::mask_rectangle')]
compile_body=shader[shader.index('resources::GpuObjectOwnerV43 compile('):shader.index('void draw_quad(')]
compile_body=compile_body.replace('}\nProgram create(', 'Program create(',1) # Remove source anonymous-namespace close.
inc='''#include "../gpu_object_owner_v43.hpp"
#include "../../engine-ui/swf_vertex_cache_v36.hpp"
#include "../../android-native/app/src/main/cpp/authored_shader_program.hpp"
#include <limits>
#include <unordered_map>
namespace dh2::android_resources {std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39();}
namespace dh2::perf {struct TestState {std::uint64_t uploads{};};extern TestState state;}
namespace dh2::scene {
struct ShaderSourcePack {bool load(const std::uint8_t*,std::size_t,std::string&){return true;}};
struct ShaderSourcePlan {unsigned gl_type{};std::array<std::string,8> chunks;};
}
namespace dh2::android_ui {
std::string asset_bytes(AAssetManager*,const char*,const char*){return "fixture";}
scene::ShaderSourcePlan plan(const scene::ShaderSourcePack&,const char*,unsigned type){scene::ShaderSourcePlan p;p.gl_type=type;for(auto& c:p.chunks)c="x";return p;}
class SwfGpu {public:
 std::shared_ptr<resources::ContextResourceBudgetV37> budget_v39_;
 resources::ResourceScopeV37 scope_v39_;
 resources::GpuObjectOwnerV43 stream_owner_v43_;
 std::unordered_map<std::uint32_t,resources::GpuObjectOwnerV43> retained_owners_v43_;
 GLuint buffer_{};bool publication_failure_at_upload{},context_loss_at_upload{};
 SwfGpu(std::shared_ptr<resources::ContextResourceBudgetV37> b,resources::ResourceScopeV37 s):budget_v39_(b),scope_v39_(s){}
 void barrier_v37(const char*);
 void release_vertices_v43(std::uintptr_t);
 void stream_storage_v43(const std::vector<float>&);
 ui::SwfVertexCacheV36::Upload actual_upload(){
'''+upload+'}\n};\n'+methods+compile_body+'}\n'
destination=layout/'engine-resources/tests/swf_objects_source_v43.inc';destination.write_text(inc)
# One fixture hook at the production upload barrier schedules the allocator fault.
test=(layout/'engine-resources/tests/swf_objects_consumer_v43.cpp')
s=test.read_text().replace('namespace dh2::perf {struct TestState {std::uint64_t uploads{};}state;}','namespace dh2::perf {TestState state;}')
s=s.replace(' if(fail_publication){fail_publication=false;fail_next_allocation=true;}',
 ''' if(publication_failure_at_upload&&std::string_view(action)=="SWF retained vertex upload"){publication_failure_at_upload=false;fail_next_allocation=true;}
 if(context_loss_at_upload&&(std::string_view(action)=="SWF retained vertex upload"||std::string_view(action)=="SWF stream vertex upload")){
   context_loss_at_upload=false;ledger->context_lost();buffers.clear();shaders.clear();programs.clear();std::string e;ck(ledger->begin_context(e));}
 if(fail_publication){fail_publication=false;fail_next_allocation=true;}''')
s=s.replace('void SwfGpu::barrier_v37(const char*)','void SwfGpu::barrier_v37(const char* action)')
test.write_text(s)
bindings={str(p.relative_to(root)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in [*copies.values(),destination,test]}
receipts=[]
for opt in ['O1','O2']:
 exe=out/('fixture-'+opt)
 command=['g++','-std=c++17','-'+opt,'-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread','-Iport/engine-ui/reports/swf-gpu-host-v38/headers',test.relative_to(root).as_posix(),(layout/'engine-resources/resource_budget_v37.cpp').relative_to(root).as_posix(),(layout/'engine-ui/swf_vertex_cache_v36.cpp').relative_to(root).as_posix(),'-o',unix(exe)]
 for command in [command,['timeout','--signal=TERM','--kill-after=2','20s','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',unix(exe)]]:
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=55)
  receipts.append({'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'host-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'bindings_sha256':bindings,'receipts':receipts},indent=2)+'\n')
 print(p.returncode,p.stdout,p.stderr[:11000])
 if p.returncode:raise SystemExit(p.returncode)
# Independent legacy authority/cache regressions compile against the staged
# ledger/header, including exact strip topology, signed zero and mode keys.
for source in ['port/engine-resources/tests/resource_budget_v37.cpp','port/engine-ui/tests/swf_vertex_cache_v36.cpp']:
 relative=Path(source).relative_to('port');dst=layout/relative;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes((root/source).read_bytes())
 exe=out/(Path(source).stem+'-regression')
 sources=[dst,(layout/'engine-resources/resource_budget_v37.cpp')]
 if 'engine-ui' in source:sources.append(layout/'engine-ui/swf_vertex_cache_v36.cpp')
 command=['g++','-std=c++17','-O2','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread',*[p.relative_to(root).as_posix() for p in sources],'-o',unix(exe)]
 for command in [command,['timeout','--signal=TERM','--kill-after=2','20s','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',unix(exe)]]:
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=55)
  receipts.append({'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'host-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'bindings_sha256':bindings,'receipts':receipts},indent=2)+'\n');print(p.returncode,p.stdout,p.stderr[:9000])
  if p.returncode:raise SystemExit(p.returncode)
