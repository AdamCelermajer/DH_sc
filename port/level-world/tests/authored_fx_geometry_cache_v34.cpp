#include "../authored_fx_geometry_packet_v7.hpp"
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <new>
static std::uint64_t allocations;static bool counting;
void* operator new(std::size_t n){if(counting)++allocations;if(auto p=std::malloc(n))return p;throw std::bad_alloc();}
void operator delete(void* p)noexcept{std::free(p);}void operator delete(void* p,std::size_t)noexcept{std::free(p);}
using namespace dh2;
struct Input {
 skinning::VisualGeometryV6 geometry;std::vector<scene::Material> materials{1};std::vector<std::uint32_t> selected{0};skinning::VisualDrawPartV6 part;
 Input(){materials[0].id="actual-layout-fixture";geometry.positions.resize(128);geometry.attributes.resize(2);
  auto& color=geometry.attributes[0];color.type=1;color.components=4;color.values.resize(128*4,127);
  auto& uv=geometry.attributes[1];uv.components=2;uv.values.resize(128*2,.5f);
  skinning::VisualPrimitiveV6 p;p.attributes.fill(-1);p.attributes[2]=0;p.attributes[4]=1;p.collada_type=0;p.engine_type=6;p.material_symbol=materials[0].id;
  for(unsigned i=0;i<126;i+=3){p.indices.push_back(i);p.indices.push_back(i+1);p.indices.push_back(i+2);}geometry.primitives.push_back(std::move(p));
  part.geometry=&geometry;part.material_table=&materials;part.materials=&selected;part.positions.resize(128);
  for(unsigned i=0;i<128;++i)part.positions[i]={float(i),float(i*2),float(i*3)};
 }
};
static void same(const fx::AuthoredFxGeometryPacketV7& a,const fx::AuthoredFxGeometryPacketV7& b){assert(a.indices==b.indices&&a.source_color_missing==b.source_color_missing&&a.vertices.size()==b.vertices.size());assert(!std::memcmp(a.vertices.data(),b.vertices.data(),a.vertices.size()*sizeof(objects::Vertex)));}
int main(){Input input;fx::AuthoredFxGeometryCacheV34 cache;fx::AuthoredFxGeometryChangeV34 changes;fx::AuthoredFxGeometryPacketV7 legacy;std::string e;unsigned checks{};
 auto update=[&](){assert(fx::authored_fx_geometry_packet_v7(input.part,0,legacy,e));assert(cache.update(input.part,0,changes,e));same(legacy,cache.packet());++checks;};
 update();assert(changes.vertices&&changes.indices);update();assert(!changes.vertices&&!changes.indices);checks+=2;
 // World pose is a uniform; changing it never changes local geometry bytes.
 input.part.world[12]=37;update();assert(!changes.vertices&&!changes.indices);checks++;
 input.part.positions[1][0]+=37;update();assert(changes.vertices&&!changes.indices);checks++;
 input.geometry.attributes[1].values[2]=.25f;update();assert(changes.vertices&&!changes.indices);checks++;
 input.geometry.attributes[0].values[2]=63;update();assert(changes.vertices&&!changes.indices);checks++;
 std::swap(input.geometry.primitives[0].indices[0],input.geometry.primitives[0].indices[1]);update();assert(!changes.vertices&&changes.indices);checks++;
 input.geometry.primitives[0].attributes[2]=-1;update();assert(changes.vertices&&!changes.indices&&cache.packet().source_color_missing);checks++;
 // Invalid topology and count shrink preserve accepted packet and flags.
 auto snapshot=cache.packet();input.geometry.primitives[0].indices[0]=65535;
 assert(!cache.update(input.part,0,changes,e)&&!changes.vertices&&!changes.indices);same(snapshot,cache.packet());checks++;
 input.geometry.primitives[0].indices[0]=1;update();
 input.part.positions.resize(1);input.geometry.positions.resize(1);input.geometry.attributes[1].values.resize(2);snapshot=cache.packet();assert(!cache.update(input.part,0,changes,e));same(snapshot,cache.packet());checks++;
 // Warm fixed topology with live position/color/UV updates: compare every
 // byte against the old allocating API before measuring repeated work.
 Input dynamic;fx::AuthoredFxGeometryCacheV34 live;
 for(unsigned i=0;i<500;++i){dynamic.part.positions[i%128][1]+=1;dynamic.geometry.attributes[1].values[i%256]+=0.001f;assert(fx::authored_fx_geometry_packet_v7(dynamic.part,0,legacy,e));assert(live.update(dynamic.part,0,changes,e));same(legacy,live.packet());++checks;}
 assert(live.counters().topology_rebuilds==1);checks++;
 constexpr unsigned frames=4000;allocations=0;counting=true;auto t0=std::chrono::steady_clock::now();
 for(unsigned i=0;i<frames;++i){dynamic.part.positions[i%128][0]+=1;assert(fx::authored_fx_geometry_packet_v7(dynamic.part,0,legacy,e));}
 auto t1=std::chrono::steady_clock::now();counting=false;const auto old_allocations=allocations;
 // Warm both scratch and published buffers before the allocation sample.
 assert(live.update(dynamic.part,0,changes,e));assert(live.update(dynamic.part,0,changes,e));allocations=0;counting=true;auto t2=std::chrono::steady_clock::now();
 for(unsigned i=0;i<frames;++i){dynamic.part.positions[i%128][0]+=1;assert(live.update(dynamic.part,0,changes,e));}
 auto t3=std::chrono::steady_clock::now();counting=false;const auto new_allocations=allocations;
 assert(old_allocations==frames*2&&new_allocations==0);checks++;
 std::cout<<"PASS geometry cacheV34 "<<checks<<" byte/stream/topology/failure checks; frames="<<frames<<" old_allocations="<<old_allocations<<" cache_allocations="<<new_allocations<<" topology_builds="<<live.counters().topology_rebuilds<<" old_us="<<std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count()<<" cache_us="<<std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count()<<"; synthetic actual-layout geometry, no live FPS claim\n";
}
