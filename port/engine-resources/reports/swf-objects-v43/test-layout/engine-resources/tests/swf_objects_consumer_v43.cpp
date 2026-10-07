#include "swf_objects_source_v43.inc"
#include "../gpu_object_owner_v43.hpp"
#include <iostream>
#include <unordered_map>
#include <cstring>
#include <cstdlib>
#include <new>
// Inject a single publication allocation failure after successful GL upload.
static bool fail_next_allocation{};
void* operator new(std::size_t n){if(std::exchange(fail_next_allocation,false))throw std::bad_alloc();if(auto* p=std::malloc(n?n:1))return p;throw std::bad_alloc();}
void operator delete(void* p)noexcept{std::free(p);}
void operator delete(void* p,std::size_t)noexcept{std::free(p);}
namespace {
unsigned checks{},allocations{},uploads{},deletes{},detaches{},compile_calls{},shader_source_calls{};
GLuint next_name=1,bound_buffer{},active_program{};GLenum error_code{};
bool fail_upload{},fail_compile{},fail_link{},fail_interface{},fail_publication{},zero_name{},lose_context{};
std::unordered_map<GLuint,std::vector<float>> buffers;
std::unordered_map<GLuint,unsigned> shaders,programs;
std::shared_ptr<dh2::resources::ContextResourceBudgetV37> ledger;
void ck(bool b){++checks;if(!b)throw std::runtime_error("V43 check "+std::to_string(checks));}
template<class F> void throws(F f){bool raised=false;try{f();}catch(const std::exception&){raised=true;}ck(raised);}
void fresh(std::uint64_t cpu=1024,std::uint64_t gpu=1024){
 ck(buffers.empty()&&shaders.empty()&&programs.empty());dh2::resources::ResourceBudgetLimitsV37 limits;
 limits.cpu_bytes=cpu;limits.gpu_bytes=gpu;ledger=std::make_shared<dh2::resources::ContextResourceBudgetV37>(limits);std::string e;ck(ledger->begin_context(e));
}
void empty(){const auto s=ledger->snapshot();ck(!s.occupied_records&&!s.pending_records&&!s.requested.gpu_bytes&&!s.requested.cpu_bytes);ck(buffers.empty()&&shaders.empty()&&programs.empty());}
}
namespace dh2::android_resources {std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){return ledger;}}
namespace dh2::perf {TestState state;}
extern "C" {
void glGenBuffers(GLsizei n,GLuint* out){for(GLsizei i=0;i<n;++i){++allocations;out[i]=zero_name?0:next_name++;if(out[i])buffers.emplace(out[i],std::vector<float>{});}}
void glBindBuffer(GLenum,GLuint name){bound_buffer=name;}
void glBufferData(GLenum,GLsizeiptr bytes,const void* data,GLenum){++uploads;if(fail_upload){error_code=GL_OUT_OF_MEMORY;return;}ck(buffers.count(bound_buffer)&&bytes>=0&&bytes%4==0);auto& v=buffers.at(bound_buffer);v.resize(std::size_t(bytes)/4);if(bytes)std::memcpy(v.data(),data,std::size_t(bytes));}
void glDeleteBuffers(GLsizei n,const GLuint* values){for(GLsizei i=0;i<n;++i){ck(buffers.erase(values[i])==1);++deletes;}}
GLuint glCreateShader(GLenum){++allocations;const auto name=next_name++;shaders[name]=0;return name;}
void glShaderSource(GLuint shader,GLsizei n,const GLchar* const* chunks,const GLint* lengths){ck(shaders.count(shader)&&n==8);for(int i=0;i<8;++i)ck(chunks[i]&&lengths[i]==1);++shader_source_calls;}
void glCompileShader(GLuint){++compile_calls;}
void glGetShaderiv(GLuint,GLenum,GLint* value){*value=!fail_compile;}
void glGetShaderInfoLog(GLuint,GLsizei,GLsizei*,GLchar* value){std::strcpy(value,"fixture compile failure");}
void glDeleteShader(GLuint name){ck(shaders.count(name)&&shaders[name]==0);shaders.erase(name);++deletes;}
GLuint glCreateProgram(){++allocations;const auto name=next_name++;programs[name]=0;return name;}
void glAttachShader(GLuint program,GLuint shader){ck(programs.count(program)&&shaders.count(shader));++programs[program];++shaders[shader];}
void glDetachShader(GLuint program,GLuint shader){ck(programs.at(program)&&shaders.at(shader));--programs[program];--shaders[shader];++detaches;}
void glLinkProgram(GLuint){}
void glGetProgramiv(GLuint,GLenum,GLint* value){*value=!fail_link;}
void glGetProgramInfoLog(GLuint,GLsizei,GLsizei*,GLchar* value){std::strcpy(value,"fixture link failure");}
GLint glGetAttribLocation(GLuint,const GLchar*){return fail_interface?-1:1;}
GLint glGetUniformLocation(GLuint,const GLchar*){return fail_interface?-1:1;}
void glGetIntegerv(GLenum property,GLint* value){ck(property==GL_CURRENT_PROGRAM);*value=GLint(active_program);}
void glUseProgram(GLuint name){ck(!name||programs.count(name));active_program=name;}
void glDeleteProgram(GLuint name){ck(active_program!=name&&programs.erase(name)==1);for(auto& shader:shaders)shader.second=0;++deletes;}
GLenum glGetError(){return std::exchange(error_code,GL_NO_ERROR);}
}
namespace dh2::android_ui {
void SwfGpu::barrier_v37(const char* action){
 if(lose_context){lose_context=false;ledger->context_lost();buffers.clear();shaders.clear();programs.clear();std::string e;ck(ledger->begin_context(e));}
 if(glGetError()!=GL_NO_ERROR)throw std::runtime_error("fixture GL error");
 if(publication_failure_at_upload&&std::string_view(action)=="SWF retained vertex upload"){publication_failure_at_upload=false;fail_next_allocation=true;}
 if(context_loss_at_upload&&(std::string_view(action)=="SWF retained vertex upload"||std::string_view(action)=="SWF stream vertex upload")){
   context_loss_at_upload=false;ledger->context_lost();buffers.clear();shaders.clear();programs.clear();std::string e;ck(ledger->begin_context(e));}
 if(fail_publication){fail_publication=false;fail_next_allocation=true;}
}
}
int main(){try{
 using namespace dh2::resources;using namespace dh2::android_ui;using dh2::ui::SwfVertexCacheV36;
 const auto front=ResourceScopeV37::swf_front,game=ResourceScopeV37::swf_gameplay;
 const auto vk=std::size_t(ResourceKindV37::vertex_buffer),sk=std::size_t(ResourceKindV37::shader),pk=std::size_t(ResourceKindV37::program);
 std::string error;const std::vector<float> a{0,1,2,3},b{1,2,3,4},c{2,3,4,5};
 // Actual cache retains exact CPU bytes and caller GPU ownership, with scope isolation.
 fresh();{
 SwfGpu gpu(ledger,front);SwfVertexCacheV36 cache(32,2);ck(cache.bind_budget_v43(ledger,front,error));
 auto upload=gpu.actual_upload();auto release=[&](std::uintptr_t v){gpu.release_vertices_v43(v);};std::uintptr_t out;bool retained;
 ck(cache.acquire(a,4,upload,release,out,retained,error)&&retained);const auto name=out;ck(buffers.at(name)==a);
 auto s=ledger->snapshot();ck(s.live.cpu_bytes==16&&s.live.gpu_bytes==16&&s.live.objects[vk]==1);
 const auto baseline=uploads;for(int i=0;i<50;++i)ck(cache.acquire(a,4,upload,release,out,retained,error)&&out==name);ck(uploads==baseline);
 ck(cache.acquire(b,4,upload,release,out,retained,error));ck(cache.acquire(a,4,upload,release,out,retained,error));
 ck(cache.acquire(c,4,upload,release,out,retained,error));ck(cache.stats().evictions==1&&ledger->snapshot().live.cpu_bytes==32&&buffers.size()==2);
 ck(!cache.bind_budget_v43(ledger,game,error));
 const auto before_delete=deletes;ledger->context_lost();buffers.clear();ck(ledger->snapshot().live.cpu_bytes==32&&!ledger->snapshot().live.gpu_bytes);
 ck(ledger->begin_context(error));gpu.retained_owners_v43_.clear();cache.abandon_context();next_name=GLuint(name);
 ck(cache.acquire(a,4,upload,release,out,retained,error)&&out==name);ck(deletes==before_delete&&buffers.at(name)==a);
 cache.clear(release);ck(ledger->snapshot().occupied_records==0);
 }empty();
 // CPU quota denies a cache copy before upload. GPU quota leaves no published entry.
 fresh(8);{SwfGpu gpu(ledger,front);SwfVertexCacheV36 cache;ck(cache.bind_budget_v43(ledger,front,error));const auto before=allocations;std::uintptr_t out;bool retained;
 ck(!cache.acquire(a,4,gpu.actual_upload(),[&](auto v){gpu.release_vertices_v43(v);},out,retained,error));ck(allocations==before&&!cache.stats().resident_entries);}empty();
 fresh(1024,8);{SwfGpu gpu(ledger,game);SwfVertexCacheV36 cache;ck(cache.bind_budget_v43(ledger,game,error));const auto before=allocations;std::uintptr_t out;bool retained;
 ck(!cache.acquire(a,4,gpu.actual_upload(),[&](auto v){gpu.release_vertices_v43(v);},out,retained,error));ck(allocations==before&&!cache.stats().resident_entries);}empty();
 // Test actual upload callback OOM and registry publication failure after GL success.
 fresh();{SwfGpu gpu(ledger,game);std::uintptr_t out=0;fail_upload=true;throws([&]{gpu.actual_upload()(a.data(),a.size(),out,error);});fail_upload=false;ck(!out&&gpu.retained_owners_v43_.empty());}empty();
 fresh();{SwfGpu gpu(ledger,game);std::uintptr_t out=0;gpu.publication_failure_at_upload=true;throws([&]{gpu.actual_upload()(a.data(),a.size(),out,error);});ck(!out&&gpu.retained_owners_v43_.empty());}empty();
 fresh();{SwfGpu gpu(ledger,game);SwfVertexCacheV36 cache;ck(cache.bind_budget_v43(ledger,game,error));std::uintptr_t out;bool retained;
 auto failure=[&](const float* data,std::size_t count,std::uintptr_t& name,std::string& e){const auto result=gpu.actual_upload()(data,count,name,e);fail_next_allocation=true;return result;};
 ck(!cache.acquire(a,4,failure,[&](auto v){gpu.release_vertices_v43(v);},out,retained,error));ck(!cache.stats().resident_entries&&!ledger->snapshot().live.cpu_bytes);}empty();
 fresh();{SwfGpu gpu(ledger,game);std::uintptr_t out=0;gpu.context_loss_at_upload=true;const auto before=deletes;throws([&]{gpu.actual_upload()(a.data(),a.size(),out,error);});ck(!out&&deletes==before&&gpu.retained_owners_v43_.empty());}empty();
 // Actual stream same-name replacement peaks at old+new storage/count one.
 fresh(1024,40);{SwfGpu gpu(ledger,game);gpu.stream_storage_v43(a);const auto name=gpu.buffer_;gpu.stream_storage_v43(b);auto s=ledger->snapshot();ck(gpu.buffer_==name&&s.live.gpu_bytes==16&&s.peak.gpu_bytes==32&&s.peak.objects[vk]==1);
 ck(s.peak_by_scope[std::size_t(game)].gpu_bytes==32&&!s.peak_by_scope[std::size_t(front)].gpu_bytes);
 const std::vector<float> bigger(8,9);const auto before=uploads;throws([&]{gpu.stream_storage_v43(bigger);});ck(uploads==before&&gpu.buffer_==name&&buffers.at(name)==b);
 fail_upload=true;throws([&]{gpu.stream_storage_v43(a);});fail_upload=false;ck(!gpu.buffer_&&!gpu.stream_owner_v43_.name());gpu.stream_storage_v43(c);ck(buffers.at(gpu.buffer_)==c);}empty();
 fresh();{SwfGpu gpu(ledger,front);zero_name=true;throws([&]{gpu.stream_storage_v43(a);});zero_name=false;ck(!gpu.buffer_);}empty();
 fresh(1024,8);{SwfGpu gpu(ledger,front);const auto before=allocations;throws([&]{gpu.stream_storage_v43(a);});ck(allocations==before&&!gpu.buffer_);}empty();
 fresh();{SwfGpu gpu(ledger,front);gpu.stream_storage_v43(a);gpu.context_loss_at_upload=true;const auto before=deletes;throws([&]{gpu.stream_storage_v43(b);});ck(!gpu.buffer_&&deletes==before);}empty();
 // Oversized cache primitive uses unchanged stream bytes, without retained copies.
 fresh();{SwfGpu gpu(ledger,front);SwfVertexCacheV36 small(8,2);ck(small.bind_budget_v43(ledger,front,error));std::uintptr_t out;bool retained;const auto before=allocations;
 ck(small.acquire(a,4,gpu.actual_upload(),[&](auto v){gpu.release_vertices_v43(v);},out,retained,error)&&!retained);ck(allocations==before&&!ledger->snapshot().live.cpu_bytes);gpu.stream_storage_v43(a);ck(buffers.at(gpu.buffer_)==a);}empty();
 // Actual compile/create/release functions preserve eight source chunks and
 // detach temporary shader owners before releasing their admitted counts.
 fresh();{auto p=create(nullptr,false,front);auto s=ledger->snapshot();ck(p.name&&s.live.objects[pk]==1&&!s.live.objects[sk]);ck(s.live.unknown_gpu_storage_objects[pk]==1&&s.peak.unknown_gpu_storage_objects[sk]==2&&s.live.gpu_bytes==0);ck(detaches>=2&&shader_source_calls>=2);
 const auto name=p.name;Program moved=std::move(p);ck(!p.name&&moved.name==name);glUseProgram(name);release(moved);ck(!active_program);}empty();
 for(unsigned mode=0;mode<3;++mode){fresh();fail_compile=mode==0;fail_link=mode==1;fail_interface=mode==2;throws([&]{auto p=create(nullptr,true,game);});fail_compile=fail_link=fail_interface=false;empty();}
 fresh();{auto p=create(nullptr,false,front);const auto name=p.name,old_deletes=deletes;ledger->context_lost();programs.clear();shaders.clear();ck(ledger->begin_context(error));next_name=name;auto fresh_program=create(nullptr,true,game);ck(fresh_program.name!=0);release(p);ck(programs.count(fresh_program.name)==1&&deletes==old_deletes+2);release(fresh_program);}empty();
 // Count budget admission occurs before shader/program allocation.
 fresh();{ledger->limits();ResourceBudgetLimitsV37 limit;limit.objects[sk]=1;ledger=std::make_shared<ContextResourceBudgetV37>(limit);ck(ledger->begin_context(error));const auto before=allocations;throws([&]{auto p=create(nullptr,false,front);});ck(allocations==before+1);}empty();
 fresh();{ResourceBudgetLimitsV37 limit;limit.objects[pk]=1;ledger=std::make_shared<ContextResourceBudgetV37>(limit);ck(ledger->begin_context(error));auto first=create(nullptr,false,front);const auto before=allocations;throws([&]{auto second=create(nullptr,true,game);});ck(allocations==before+2&&ledger->snapshot().live.objects[pk]==1&&programs.count(first.name));release(first);}empty();
 std::cout<<"PASS source-bound SWF cache/upload/stream/shader lifetimes; checks="<<checks<<"\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
