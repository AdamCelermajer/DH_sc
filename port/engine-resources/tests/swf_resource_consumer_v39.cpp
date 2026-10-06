// Actual staged SwfGpu methods/header, fake GL metadata only. No GL driver.
#include "../reports/swf-resource-v39/test-header.hpp"
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <new>
#include <stdexcept>
#include <utility>
using namespace dh2::resources;
static ContextResourceBudgetV37* active_budget;
static std::shared_ptr<ContextResourceBudgetV37> active_lease;
static bool observe_array,fail_array,fail_map_node;
static unsigned allocations,checks;
static unsigned singleton_calls;
static std::uint64_t expected_requested;
static void ck(bool value,const char* error){if(!value)throw std::runtime_error(error);++checks;}
void* operator new[](std::size_t n){
 if(observe_array){
  ck(active_budget&&active_budget->snapshot().pending.cpu_bytes>=n,"CPU allocation preceded byte admission");
  ++allocations;
  if(fail_array){fail_array=false;throw std::bad_alloc();}
 }
 if(auto* p=std::malloc(n?n:1))return p;
 throw std::bad_alloc();
}
void operator delete[](void* p)noexcept{std::free(p);}
void operator delete[](void* p,std::size_t)noexcept{std::free(p);}
void* operator new(std::size_t n){
 if(fail_map_node){fail_map_node=false;throw std::bad_alloc();}
 if(auto* p=std::malloc(n?n:1))return p;
 throw std::bad_alloc();
}
void operator delete(void* p)noexcept{std::free(p);}
void operator delete(void* p,std::size_t)noexcept{std::free(p);}
namespace dh2::android_resources {
ContextResourceBudgetV37& budget_v38(){return *active_budget;}
std::shared_ptr<ContextResourceBudgetV37> budget_lease_v39(){++singleton_calls;return active_lease;}
void release_v38(ResourceTokenV37& token){if(token){std::string error;if(!active_budget->release(token,error))throw std::runtime_error(error);}}
}
struct Driver {
 std::map<GLuint,std::pair<ResourceKindV37,std::uint64_t>> storage;
 GLuint next{1},texture{},framebuffer{},renderbuffer{};
 unsigned creates{},deletes{},uploads{},frame_checks{},fail_frame_at{};
 GLenum error{};bool zero_name{},fail_upload{};
 std::function<void()> at_barrier;
} driver;
static void create_names(ResourceKindV37 kind,GLsizei n,GLuint* names){
 if(expected_requested){ck(active_budget->snapshot().requested.gpu_bytes==expected_requested,"Full resize candidate not admitted beside actual old storage");expected_requested=0;}
 ck(active_budget->snapshot().pending.objects[std::size_t(kind)]>=std::uint64_t(n),"GL name creation preceded object admission");
 for(GLsizei i=0;i<n;++i){if(driver.zero_name){driver.zero_name=false;names[i]=0;continue;}names[i]=driver.next++;driver.storage.emplace(names[i],std::make_pair(kind,0));++driver.creates;}
}
static void delete_names(GLsizei n,const GLuint* names){for(GLsizei i=0;i<n;++i)if(names[i]){driver.storage.erase(names[i]);++driver.deletes;}}
extern "C" {
void glGenTextures(GLsizei n,GLuint* p){create_names(ResourceKindV37::texture,n,p);}
void glGenFramebuffers(GLsizei n,GLuint* p){create_names(ResourceKindV37::framebuffer,n,p);}
void glGenRenderbuffers(GLsizei n,GLuint* p){create_names(ResourceKindV37::renderbuffer,n,p);}
void glDeleteTextures(GLsizei n,const GLuint* p){delete_names(n,p);}
void glDeleteFramebuffers(GLsizei n,const GLuint* p){delete_names(n,p);}
void glDeleteRenderbuffers(GLsizei n,const GLuint* p){delete_names(n,p);}
void glBindTexture(GLenum,GLuint n){driver.texture=n;}
void glBindFramebuffer(GLenum,GLuint n){driver.framebuffer=n;}
void glBindRenderbuffer(GLenum,GLuint n){driver.renderbuffer=n;}
void glPixelStorei(GLenum,GLint){}
void glTexParameteri(GLenum,GLenum,GLint){}
void glFramebufferTexture2D(GLenum,GLenum,GLenum,GLuint,GLint){}
void glFramebufferRenderbuffer(GLenum,GLenum,GLenum,GLuint){}
void glTexImage2D(GLenum,GLint,GLint,GLsizei w,GLsizei h,GLint,GLenum format,GLenum,const void*){
 const auto bytes=std::uint64_t(w)*h*(format==GL_ALPHA?1:format==GL_RGB?3:4);
 ck(active_budget->snapshot().pending.texture_gpu_bytes>=bytes,"Texture storage preceded byte admission");
 ck(driver.storage.count(driver.texture)!=0,"Texture storage has no driver owner");
 driver.storage.at(driver.texture).second=bytes;++driver.uploads;
 if(driver.fail_upload){driver.fail_upload=false;driver.error=GL_OUT_OF_MEMORY;}
}
void glRenderbufferStorage(GLenum,GLenum,GLsizei w,GLsizei h){
 const auto bytes=std::uint64_t(w)*h*4;
 ck(active_budget->snapshot().pending.gpu_bytes_by_kind[std::size_t(ResourceKindV37::renderbuffer)]>=bytes,"Depth-stencil storage preceded admission");
 driver.storage.at(driver.renderbuffer).second=bytes;++driver.uploads;
}
GLenum glCheckFramebufferStatus(GLenum){++driver.frame_checks;return driver.frame_checks==driver.fail_frame_at?GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:GL_FRAMEBUFFER_COMPLETE;}
void glGetIntegerv(GLenum name,GLint* p){if(name!=GL_STENCIL_BITS)throw std::runtime_error("Unexpected query");*p=8;}
GLenum glGetError(){if(driver.at_barrier&&driver.uploads){auto callback=std::move(driver.at_barrier);driver.at_barrier={};callback();}return std::exchange(driver.error,GL_NO_ERROR);}
}
#include "../reports/swf-resource-v39/actual-methods.inc"
using dh2::android_ui::SwfGpu;
static void clear(SwfGpu& gpu){gpu.release_targets_v39(false);for(auto& p:gpu.textures_)gpu.release_texture_v39(p.second,false);gpu.textures_.clear();}
static std::uint64_t storage_bytes(){std::uint64_t n=0;for(const auto& p:driver.storage)n+=p.second.second;return n;}
static void context(ContextResourceBudgetV37& ledger){active_budget=&ledger;active_lease={&ledger,[](ContextResourceBudgetV37*){}};std::string error;ck(ledger.begin_context(error),"Context begin failed");driver={};}
static void configure(SwfGpu& gpu){gpu.budget_v39_=active_lease;gpu.scope_v39_=ResourceScopeV37::swf_gameplay;gpu.max_texture_size_v37_=16384;gpu.normal_.name=1;}
int main(){try{
 ResourceBudgetLimitsV37 limits;limits.gpu_bytes=4096;limits.texture_gpu_bytes=4096;limits.cpu_bytes=4096;limits.individual_cpu_bytes=4096;limits.individual_gpu_bytes=4096;
 ContextResourceBudgetV37 ledger(limits);context(ledger);SwfGpu gpu;configure(gpu);
 std::uint8_t pixels[64]{};dh2::ui::SwfTexture out{999,7,7};std::string error;
 observe_array=true;fail_array=true;
 ck(!gpu.image(4,4,4,pixels,16,out,error),"Injected CPU allocation failure accepted");
 ck(out.identity==999&&gpu.textures_.empty()&&driver.creates==0&&ledger.snapshot().occupied_records==0,"CPU failure published or leaked");
 // CPU quota rejection must not call new[], even before any GL context work.
 const auto calls=allocations;std::uint8_t one=0;
 ck(!gpu.image(16384,1024,4,&one,65536,out,error),"Oversized CPU bitmap accepted");
 ck(allocations==calls&&driver.creates==0&&ledger.snapshot().occupied_records==0,"CPU quota refusal happened after allocation");
 gpu.max_texture_size_v37_=2;const auto dimension_calls=allocations;
 ck(!gpu.image(4,4,4,pixels,16,out,error),"Unsupported GPU bitmap dimensions accepted");
 ck(allocations==dimension_calls&&driver.creates==0,"Unsupported GPU dimensions allocated CPU/GL first");
 gpu.max_texture_size_v37_=16384;
 observe_array=false;
 ResourceReservationV37 blocked;ResourceTokenV37 blocker;
 ck(ledger.reserve_create({ResourceKindV37::texture,ResourceScopeV37::actor,4050,0},blocked,error)&&blocked.commit(blocker,error),"GPU quota fixture admission failed");
 ck(!gpu.image(4,4,4,pixels,16,out,error),"GPU-quota-rejected bitmap published");
 ck(driver.creates==0&&gpu.textures_.empty()&&ledger.snapshot().live.cpu_bytes==0&&ledger.snapshot().live.gpu_bytes==4050,"GPU quota refusal leaked new CPU/GL owner");
 ck(ledger.release(blocker,error),"GPU quota fixture release failed");
 // Direct allocator publication failure, before creating any GL object.
 fail_map_node=true;ck(!gpu.image(4,4,4,pixels,16,out,error),"Injected map publication failure accepted");
 ck(driver.creates==0&&gpu.textures_.empty()&&ledger.snapshot().live.cpu_bytes==0,"Map publication failure leaked retained pixels");
 driver.fail_upload=true;
 ck(!gpu.image(4,4,4,pixels,16,out,error),"Injected bitmap OOM accepted");
 ck(driver.storage.empty()&&gpu.textures_.empty()&&ledger.snapshot().occupied_records==0&&out.identity==999,"Bitmap OOM failed owner cleanup/output atomicity");
 driver.zero_name=true;ck(!gpu.image(4,4,4,pixels,16,out,error),"Zero bitmap GL name accepted");
 ck(driver.storage.empty()&&ledger.snapshot().occupied_records==0,"Zero bitmap name leaked admission");
 observe_array=true;ck(gpu.image(4,4,4,pixels,16,out,error),"Valid bitmap failed");observe_array=false;
 ck(out.identity==1&&ledger.snapshot().live.cpu_bytes==64&&ledger.snapshot().live.texture_gpu_bytes==64,"Bitmap bytes/count publication wrong");
 gpu.target(8,8);ck(storage_bytes()==832&&ledger.snapshot().live.gpu_bytes==832,"Initial target accounting wrong");
 expected_requested=3904;gpu.target(16,16);ck(storage_bytes()==3136&&ledger.snapshot().live.gpu_bytes==3136,"Resize storage/accounting mismatch");
 ck(!expected_requested&&ledger.snapshot().peak.gpu_bytes>=3904&&ledger.snapshot().peak.gpu_bytes<=4096,"Resize requested peak outside admitted overlap/cap");
 const auto old_target=gpu.target_,old_color=gpu.target_color_;const auto old_creates=driver.creates;const auto old_deletes=driver.deletes;
 bool thrown=false;try{gpu.target(32,32);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown&&driver.creates==old_creates&&driver.deletes==old_deletes&&gpu.target_==old_target,"Target quota denial mutated valid old owner");
 // Resize within byte quota but second framebuffer is incomplete.
 driver.fail_frame_at=driver.frame_checks+2;thrown=false;try{gpu.target(4,4);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown&&gpu.target_==old_target&&gpu.target_color_==old_color&&driver.storage.size()==6&&ledger.snapshot().live.gpu_bytes==3136,"Incomplete candidate did not preserve old target and clean partial names");
 driver.fail_frame_at=0;driver.fail_upload=true;thrown=false;try{gpu.target(4,4);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown&&gpu.target_==old_target&&driver.storage.size()==6&&ledger.snapshot().pending.gpu_bytes==0,"Target OOM left partial owner/admission");
 // Context loss at actual final GL barrier, then numeric-name reuse by another
 // admitted owner. Stale candidate cleanup must leave that new-context owner.
 const auto old_pixels=gpu.textures_.at(1).pixels.data();ResourceTokenV37 unrelated;
 driver.at_barrier=[&](){ledger.context_lost();driver.storage.clear();driver.next=1;ck(ledger.begin_context(error),"Second context begin failed");ResourceReservationV37 q;ck(ledger.reserve_create({ResourceKindV37::texture,ResourceScopeV37::actor,1,0},q,error),"Unrelated texture admission failed");GLuint n;glGenTextures(1,&n);driver.storage.at(n).second=1;ck(q.commit(unrelated,error),"Unrelated texture commit failed");};
 // Skip entry barrier callback; activate it only once new candidate writes occur.
 driver.uploads=0;thrown=false;try{gpu.target(4,4);}catch(const std::runtime_error&){thrown=true;}
 ck(thrown&&driver.storage.size()==1&&driver.storage.count(1)&&ledger.snapshot().live.gpu_bytes==1,"Old-context cleanup deleted recycled unrelated name");
 gpu.release_targets_v39(true);gpu.release_texture_v39(gpu.textures_.at(1),true);
 ck(gpu.textures_.at(1).pixels.data()==old_pixels&&ledger.snapshot().live.cpu_bytes==64,"Context loss discarded retained pixels");
 gpu.upload(gpu.textures_.at(1));ck(ledger.snapshot().live.gpu_bytes==65&&ledger.snapshot().live.cpu_bytes==64,"Reupload double charged CPU or retained stale GPU charge");
 clear(gpu);GLuint unrelated_name=1;glDeleteTextures(1,&unrelated_name);ck(ledger.release(unrelated,error),"Unrelated release failed");
 ck(driver.storage.empty()&&ledger.snapshot().occupied_records==0&&ledger.snapshot().live.cpu_bytes==0,"Final teardown leaked resources");
 gpu.scope_v39_=ResourceScopeV37::swf_front;
 ck(gpu.image(4,4,1,pixels,4,out,error),"Actual GL_ALPHA glyph upload failed");
 ck(ledger.snapshot().live.gpu_bytes==16&&ledger.snapshot().live.cpu_bytes==16&&ledger.snapshot().live_by_scope[std::size_t(ResourceScopeV37::swf_front)].gpu_bytes==16,"Font channel bytes/front scope wrong");
 ck(gpu.image(4,4,3,pixels,12,out,error),"Actual RGB upload failed");
 ck(ledger.snapshot().live.gpu_bytes==64&&ledger.snapshot().live.cpu_bytes==64,"Actual RGB channel bytes wrong");
 clear(gpu);
 ck(singleton_calls==0,"Retained HUD allocation/cleanup called singleton factory instead of held ledger lease");
 // Independent front scope and move-safe retained storage, no GL required.
 {RetainedBytesV39 a;observe_array=true;ck(a.allocate(dh2::android_resources::budget_lease_v39(),ResourceScopeV37::swf_front,12,error),"Front pixels failed");observe_array=false;auto* pointer=a.data();RetainedBytesV39 b(std::move(a));ck(!a.data()&&b.data()==pointer&&ledger.snapshot().live_by_scope[std::size_t(ResourceScopeV37::swf_front)].cpu_bytes==12,"CPU owner relocation lost ownership/scope");}
 ck(ledger.snapshot().occupied_records==0,"Moved CPU owner destructor leaked");
 {auto owner=std::make_shared<ContextResourceBudgetV37>();std::weak_ptr<ContextResourceBudgetV37> weak=owner;RetainedBytesV39 bytes;ck(bytes.allocate(owner,ResourceScopeV37::swf_front,7,error),"Owned ledger retained bytes failed");owner.reset();ck(!weak.expired(),"Retained bytes did not preserve ledger lease");bytes.reset();ck(weak.expired(),"Last retained byte release leaked ledger lease");}
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"actual_SwfGpu_methods\":5,\"fake_GL_only\":true,\"retained_CPU_before_allocation\":true,\"final_records\":"<<ledger.snapshot().occupied_records<<"}\n";
 return 0;
}catch(const std::exception& error){observe_array=false;std::cerr<<error.what()<<'\n';return 1;}}
