// Actual generic buffer owner + actual FX cache + real skin deformation.
// Fake GL stores capacities and at most a 256-byte sample, never large payloads.
#include "../reports/buffer-resource-v41/test-layout/engine-resources/cpu_vector_capacity_v41.hpp"
#include "../reports/buffer-resource-v41/test-layout/level-world/authored_fx_geometry_packet_v7.hpp"
#include "../../engine-skinning/skinning.hpp"
#include <GLES2/gl2.h>
#include <algorithm>
#include <array>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::resources;
static unsigned checks;
static void ck(bool value,const char* message){if(!value)throw std::runtime_error(message);++checks;}
static std::shared_ptr<ContextResourceBudgetV37> ledger;
namespace dh2::android_resources {std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){return ledger;}}
struct Stored {std::uint64_t bytes{};std::array<std::uint8_t,256> sample{};std::size_t sampled{};};
struct Driver {
 std::map<GLuint,Stored> buffers;GLuint next{1},array{},elements{};
 unsigned creates{},deletes{},storage_calls{},sub_calls{};bool zero_name{},fail_storage{},fail_sub{};
 GLenum error{};std::function<void()> after_data;
} driver;
extern "C" {
void glGenBuffers(GLsizei count,GLuint* out){
 const auto s=ledger->snapshot();ck(s.pending.objects[std::size_t(ResourceKindV37::vertex_buffer)]+s.pending.objects[std::size_t(ResourceKindV37::index_buffer)]>=std::uint64_t(count),"Buffer name creation preceded object admission");
 for(int i=0;i<count;++i){if(driver.zero_name){driver.zero_name=false;out[i]=0;continue;}out[i]=driver.next++;driver.buffers.emplace(out[i],Stored{});++driver.creates;}
}
void glDeleteBuffers(GLsizei n,const GLuint* names){for(int i=0;i<n;++i)if(names[i]){driver.buffers.erase(names[i]);++driver.deletes;}}
void glBindBuffer(GLenum target,GLuint name){(target==GL_ARRAY_BUFFER?driver.array:driver.elements)=name;}
void glBufferData(GLenum target,GLsizeiptr bytes,const void* data,GLenum){
 const auto kind=target==GL_ARRAY_BUFFER?ResourceKindV37::vertex_buffer:ResourceKindV37::index_buffer;
 ck(bytes>0&&ledger->snapshot().pending.gpu_bytes_by_kind[std::size_t(kind)]>=std::uint64_t(bytes),"Buffer storage preceded full typed byte admission");
 auto& s=driver.buffers.at(target==GL_ARRAY_BUFFER?driver.array:driver.elements);s.bytes=bytes;s.sampled=data?std::min<std::size_t>(bytes,s.sample.size()):0;
 if(data)std::memcpy(s.sample.data(),data,s.sampled);
 ++driver.storage_calls;
 if(driver.after_data){auto callback=std::move(driver.after_data);driver.after_data={};callback();}
 if(driver.fail_storage){driver.fail_storage=false;driver.error=GL_OUT_OF_MEMORY;}
}
void glBufferSubData(GLenum target,GLintptr offset,GLsizeiptr bytes,const void* data){
 ck(offset==0,"Source whole-geometry update offset changed");auto& s=driver.buffers.at(target==GL_ARRAY_BUFFER?driver.array:driver.elements);
 ck(bytes>=0&&std::uint64_t(bytes)<=s.bytes,"Driver update exceeded actual storage");s.sampled=std::min<std::size_t>(bytes,s.sample.size());std::memcpy(s.sample.data(),data,s.sampled);++driver.sub_calls;
 if(driver.fail_sub){driver.fail_sub=false;driver.error=GL_OUT_OF_MEMORY;}
}
GLenum glGetError(){return std::exchange(driver.error,GL_NO_ERROR);}
}
static void check(const char*){if(glGetError()!=GL_NO_ERROR)throw std::runtime_error("Injected GL allocation/update failure");}
#include "../../android-native/app/src/main/cpp/renderer_buffer_budget_v41.inc"
static void start(std::uint64_t gpu=512,std::uint64_t cpu=4096){
 ck(generic_buffer_charges_v41.empty(),"Previous GPU owner remains");ResourceBudgetLimitsV37 limits;limits.gpu_bytes=gpu;limits.texture_gpu_bytes=gpu;limits.cpu_bytes=cpu;limits.individual_gpu_bytes=4096;limits.individual_cpu_bytes=4096;
 ledger=std::make_shared<ContextResourceBudgetV37>(limits);std::string e;ck(ledger->begin_context(e),"Context begin failed");driver={};
}
static void empty(){ck(driver.buffers.empty()&&ledger->snapshot().occupied_records==0,"Actual buffer/CPU owner leaked");}
struct FxInput {
 skinning::VisualGeometryV6 geometry;std::vector<scene::Material> materials{1};std::vector<std::uint32_t> selected{0};skinning::VisualDrawPartV6 part;
 FxInput(){materials[0].id="source-numeric-triangle";geometry.positions.resize(4);geometry.attributes.resize(2);auto& color=geometry.attributes[0];color.type=1;color.components=4;color.values.resize(16,127);auto& uv=geometry.attributes[1];uv.components=2;uv.values.resize(8,.5f);
  skinning::VisualPrimitiveV6 p;p.attributes.fill(-1);p.attributes[2]=0;p.attributes[4]=1;p.collada_type=0;p.engine_type=6;p.material_symbol=materials[0].id;p.indices={0,1,2};geometry.primitives.push_back(std::move(p));
  part.geometry=&geometry;part.material_table=&materials;part.materials=&selected;part.positions.resize(4);for(unsigned i=0;i<4;++i)part.positions[i]={float(i),float(i*2),float(i*3)};
 }
};
int main(){try{
 std::string e;start();GLuint vbo{},ebo{};
 ck(buffer_bytes_v41(3,2)==6,"Checked index byte producer wrong");
 buffer_storage_v41(vbo,GL_ARRAY_BUFFER,100,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);
 const auto name=vbo;buffer_storage_v41(vbo,GL_ARRAY_BUFFER,150,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);
 ck(vbo==name&&ledger->snapshot().live.gpu_bytes==150&&ledger->snapshot().peak.gpu_bytes==250&&ledger->snapshot().peak.objects[std::size_t(ResourceKindV37::vertex_buffer)]==1,"Same-name replacement peak/count wrong");
 buffer_storage_v41(vbo,GL_ARRAY_BUFFER,60,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);ck(ledger->snapshot().live.gpu_bytes==60&&driver.buffers.at(vbo).bytes==60,"Shrinking retained stale charges");
 const auto calls=driver.storage_calls,deletes=driver.deletes;bool threw=false;try{buffer_storage_v41(vbo,GL_ARRAY_BUFFER,500,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);}catch(const std::exception&){threw=true;}
 ck(threw&&vbo==name&&driver.storage_calls==calls&&driver.deletes==deletes&&ledger->snapshot().live.gpu_bytes==60,"Quota denial mutated old GL storage/owner");
 driver.fail_storage=true;threw=false;try{buffer_storage_v41(vbo,GL_ARRAY_BUFFER,120,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);}catch(const std::exception&){threw=true;}
 ck(threw&&!vbo&&generic_buffer_charges_v41.empty()&&ledger->snapshot().occupied_records==0,"Mutation OOM retained invalid buffer or locked token");empty();
 driver.zero_name=true;threw=false;try{buffer_storage_v41(ebo,GL_ELEMENT_ARRAY_BUFFER,6,nullptr,GL_STATIC_DRAW,ResourceScopeV37::world);}catch(const std::exception&){threw=true;}
 ck(threw&&!ebo,"Zero buffer GL name accepted");empty();
 start(512,128);{
  CpuGeometryStorageV41<std::uint32_t> a,b;
  ck(reserve_cpu_vector_v41(a.vertices,a.capacity,ledger,ResourceScopeV37::world,8,e),"CPU vector cold admission failed");a.vertices.resize(8,7);auto* previous=a.vertices.data();
  ck(!reserve_cpu_vector_v41(a.vertices,a.capacity,ledger,ResourceScopeV37::world,40,e)&&a.vertices.data()==previous&&a.vertices[0]==7&&a.vertices.capacity()==8,"CPU quota denial allocated/mutated accepted vector");
  ck(reserve_cpu_vector_v41(a.vertices,a.capacity,ledger,ResourceScopeV37::world,12,e)&&ledger->snapshot().peak.cpu_bytes==80&&ledger->snapshot().live.cpu_bytes==48,"CPU vector old/new capacity peak wrong");
  ck(!reserve_cpu_vector_v41(a.vertices,a.capacity,ledger,ResourceScopeV37::equipment,8,e),"Warm CPU vector changed owner scope");
  ck(reserve_cpu_vector_v41(b.vertices,b.capacity,ledger,ResourceScopeV37::world,2,e),"Move destination setup failed");b.vertices.resize(2);
  b=std::move(a);ck(a.vertices.empty()&&b.vertices[0]==7&&ledger->snapshot().live.cpu_bytes==48,"CPU vector relocation lost or double charged storage");
 }empty();
 // Actual skin-point pipeline; retained CPU vector and accepted GPU sample.
 start();{
  CpuGeometryStorageV41<objects::Vertex> geometry;
  ck(reserve_cpu_vector_v41(geometry.vertices,geometry.capacity,ledger,ResourceScopeV37::actor,2,e),"Actor CPU vertices rejected");geometry.vertices.resize(2);
  skinning::Skin skin;skin.nodes={0};skin.influence_count=1;skin.influences.resize(2);for(auto& influence:skin.influences){influence.weights[0]=1;influence.joints[0]=0;}
  skinning::Matrix pose{1,0,0,0,0,1,0,0,0,0,1,0,10,20,30,1};std::vector<std::array<float,3>> rest{{1,2,3},{4,5,6}},deformed;
  ck(skinning::positions(skin,{pose},rest,deformed,e),"Actual skin-point pipeline failed");
  for(unsigned i=0;i<2;++i)std::copy(deformed[i].begin(),deformed[i].end(),geometry.vertices[i].p);
  buffer_storage_v41(vbo,GL_ARRAY_BUFFER,72,geometry.vertices.data(),GL_DYNAMIC_DRAW,ResourceScopeV37::actor);
  pose[12]=40;ck(skinning::positions(skin,{pose},rest,deformed,e),"Second actual pose failed");for(unsigned i=0;i<2;++i)std::copy(deformed[i].begin(),deformed[i].end(),geometry.vertices[i].p);
  const auto storage=driver.storage_calls;buffer_subdata_v41(vbo,GL_ARRAY_BUFFER,72,geometry.vertices.data(),ResourceScopeV37::actor);
  ck(driver.storage_calls==storage&&ledger->snapshot().live.gpu_bytes==72&&ledger->snapshot().live.cpu_bytes==72,"Warm skin update reallocated or mischarged");
  ck(!std::memcmp(driver.buffers.at(vbo).sample.data(),geometry.vertices.data(),72)&&geometry.vertices[0].p[0]==41,"Real skinned vertex bytes did not reach GPU update");
  threw=false;try{buffer_subdata_v41(vbo,GL_ARRAY_BUFFER,73,geometry.vertices.data(),ResourceScopeV37::actor);}catch(const std::exception&){threw=true;}
  ck(threw&&driver.sub_calls==1,"Oversized skin update reached driver");
  driver.fail_sub=true;threw=false;try{buffer_subdata_v41(vbo,GL_ARRAY_BUFFER,72,geometry.vertices.data(),ResourceScopeV37::actor);}catch(const std::exception&){threw=true;}
  ck(threw&&!vbo&&ledger->snapshot().live.gpu_bytes==0,"Failed skin update retained invalid GPU owner");
  buffer_subdata_v41(vbo,GL_ARRAY_BUFFER,72,geometry.vertices.data(),ResourceScopeV37::actor);ck(vbo&&driver.storage_calls==storage+1,"Dirty source skin retry did not recreate owner");
  release_buffer_v41(vbo,false);
 }empty();
 // Lost-name reuse: current actor VBO must survive stale owner cleanup.
 start();buffer_storage_v41(vbo,GL_ARRAY_BUFFER,32,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);auto old=vbo;ResourceTokenV37 unrelated;GLuint unrelated_name{};
 driver.after_data=[&](){ledger->context_lost();driver.buffers.clear();driver.next=old;ck(ledger->begin_context(e),"Replacement context failed");ResourceReservationV37 q;ck(ledger->reserve_create({ResourceKindV37::vertex_buffer,ResourceScopeV37::world,1,0},q,e),"Unrelated VBO admission failed");glGenBuffers(1,&unrelated_name);driver.buffers.at(unrelated_name).bytes=1;ck(q.commit(unrelated,e),"Unrelated VBO commit failed");};
 threw=false;try{buffer_storage_v41(vbo,GL_ARRAY_BUFFER,48,nullptr,GL_DYNAMIC_DRAW,ResourceScopeV37::actor);}catch(const std::exception&){threw=true;}
 ck(threw&&!vbo&&unrelated_name==old&&driver.buffers.count(unrelated_name)&&ledger->snapshot().live.gpu_bytes==1,"Replacement cleanup deleted a recycled unrelated VBO");glDeleteBuffers(1,&unrelated_name);ck(ledger->release(unrelated,e),"Unrelated VBO release failed");empty();
 // All five actual FX cache buffers are admitted, including scratch+topology.
 start();FxInput input;fx::AuthoredFxGeometryPacketV7 legacy;ck(fx::authored_fx_geometry_packet_v7(input.part,0,legacy,e),"Legacy numeric packet reference failed");
 {fx::AuthoredFxGeometryCacheV34 cache;fx::AuthoredFxGeometryChangeV34 changed;ck(cache.bind_cpu_budget_v41(ledger,e),"FX CPU budget bind failed");ck(cache.update(input.part,0,changed,e),"Bound FX cold update failed");
  ck(cache.packet().vertices.size()==legacy.vertices.size()&&!std::memcmp(cache.packet().vertices.data(),legacy.vertices.data(),144)&&cache.packet().indices==legacy.indices,"CPU admission altered original FX packet bytes");
  ck(ledger->snapshot().live.cpu_bytes==162,"Cold FX capacities omitted packet/topology storage");ck(cache.update(input.part,0,changed,e)&&!changed.vertices&&!changed.indices,"Warm FX parity failed");ck(ledger->snapshot().live.cpu_bytes==306,"Warm FX scratch retained capacity missing");
  input.part.positions[0][0]+=5;ck(cache.update(input.part,0,changed,e)&&changed.vertices&&!changed.indices,"Changed FX pose skipped");
  cache.reset_buffers_v41();ck(ledger->snapshot().live.cpu_bytes==0,"FX safe reset released charge before/without storage cleanup");ck(cache.update(input.part,0,changed,e)&&changed.vertices&&changed.indices,"Bound FX retry failed after reset");
 }empty();
 // Force failure at source topology snapshot: previously accepted packet must
 // remain byte-identical, and retry must not hide a partially published index set.
 start(512,330);{fx::AuthoredFxGeometryCacheV34 cache;fx::AuthoredFxGeometryChangeV34 changed;ck(cache.bind_cpu_budget_v41(ledger,e)&&cache.update(input.part,0,changed,e)&&cache.update(input.part,0,changed,e),"FX quota case setup failed");
  auto before=cache.packet();input.geometry.primitives[0].indices={0,1,2,0,2,3};
  ck(!cache.update(input.part,0,changed,e),"FX topology growth exceeded CPU quota");ck(cache.packet().indices==before.indices&&!std::memcmp(cache.packet().vertices.data(),before.vertices.data(),144),"Rejected FX topology partially published packet");
  cache.reset_buffers_v41();ck(cache.update(input.part,0,changed,e)&&cache.packet().indices.size()==6,"FX quota retry hid pending source topology");
 }empty();
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"actual_skin_update\":true,\"actual_FX_cache_parity\":true,\"fake_GL_only\":true,\"final_records\":"<<ledger->snapshot().occupied_records<<"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
