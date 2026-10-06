#include "../gameplay_skybox_v24.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
#include <stdexcept>
using namespace dh2;
struct MaterialFieldsFixture {std::uint32_t flags{0xffffffffu};std::uint8_t dirty{7};};
int main(int argc,char**argv){try{if(argc!=2)return 2;const std::string path=argv[1];std::string e;auto roots=std::make_shared<world::GameObjectSceneRootRegistryV1>();auto scope=std::make_shared<int>(1);
 camera::SkyboxLoadServicesV24 services;services.actual_resource_owner=scope;services.same_roots=roots;
 services.read=[&](const std::string&uri,auto&bytes,bool&found,auto&){const auto split=uri.find_last_of("/\\");std::ifstream f(path+"/"+uri.substr(split+1),std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
 bool reject_material=false;std::vector<std::shared_ptr<MaterialFieldsFixture>> source_field_fixtures;
 // Explicit external COMMON renderer field fixture: not a real GPU material
 // constructor. Actual BRES geometry/links/texture bytes and native source
 // skybox mutation/root/queue/render sequence are tested independently.
 services.material_renderer=[&](const auto&resource,std::uint32_t index,auto&out,auto&error){assert(resource&&index<resource->scene.materials.size());if(reject_material){error="fixture missing COMMON renderer";return false;}auto fields=std::make_shared<MaterialFieldsFixture>();source_field_fixtures.push_back(fields);out.owner=fields;out.flags4=&fields->flags;out.dirty30=&fields->dirty;return true;};
 camera::GameplaySkyboxV24 owner(services);unsigned assets=0,buffers=0,draws=0;
 for(const auto*file:{"skybox_swamp.bdae","skybox_blood.bdae","skybox_under.bdae","skybox_darktemple.bdae","skybox_wind.bdae"}){
  if(!owner.add(file,e))throw std::runtime_error(e);auto node=owner.current();assert(node&&node->source_references()==2&&node->source_culling118()==0&&roots->roots().size()==1);
  auto fields=source_field_fixtures.back();assert(fields->flags==0xffefffffu&&fields->dirty==1);const auto resource=node->resource();assert(resource&&resource->buffers.size()==1&&resource->textures.size()==1&&!resource->textures[0]->rgba.empty());
  assert(resource->textures[0]->rgba.size()==std::size_t(resource->textures[0]->width)*resource->textures[0]->height*4);
  assert(owner.add("",e)&&owner.current()==node);assert(owner.add("actual-absent-skybox.bdae",e)&&owner.current()==node);
  reject_material=true;assert(!owner.add(file,e)&&owner.current()==node&&roots->roots().size()==1);reject_material=false;
  std::vector<camera::SkyboxQueueEntryV24> queue;assert(node->register_buffers(true,[&](const auto&entry,auto&){assert(entry.node==node&&entry.pass==2&&entry.priority==INT32_MAX&&entry.buffer_parameter==queue.size()+1&&entry.flags==0);queue.push_back(entry);return true;},e));assert(queue.size()==1);buffers+=queue.size();
  std::vector<std::string> calls;camera::SkyboxRenderServicesV24 render;render.actual_driver_present=render.actual_active_camera_present=true;
  render.active_camera_position=[&](auto&eye,auto&){eye={100,200,300};calls.push_back("position");return true;};render.active_camera_near=[&](float&near,auto&){near=900;calls.push_back("near");return true;};
  render.set_world_transform=[&](const auto&m,auto&){assert(m[12]==100&&m[13]==200&&m[14]==300&&m[0]==1800&&m[5]==1800&&m[10]==1800);calls.push_back("world1");return true;};
  render.set_material=[&](const auto&r,const auto&b,auto&){assert(&r==resource.get()&&!b.indices.empty()&&!b.positions.empty()&&!b.uv.empty());calls.push_back("material");return true;};render.set_shadow_cascade=[&](auto id,auto&){assert(id==0);calls.push_back("cascade0");return true;};render.draw_buffer=[&](const auto&r,const auto&b,auto&){assert(&r==resource.get()&&b.indices.size()==b.primitive.index_count);++draws;calls.push_back("draw");return true;};
  assert(node->render(queue[0].buffer_parameter,render,e));assert(calls==std::vector<std::string>({"position","near","world1","material","cascade0","draw"}));
  calls.clear();assert(node->render(0,render,e)&&calls==std::vector<std::string>({"position","near","world1"}));
  calls.clear();node->source_enabled138()=0;assert(node->render(1,render,e)&&calls.empty());node->source_enabled138()=1;
  queue.clear();std::cout<<file<<" vertices="<<resource->buffers[0].positions.size()<<" indices="<<resource->buffers[0].indices.size()<<" texture="<<resource->textures[0]->uri<<" size="<<resource->textures[0]->width<<'x'<<resource->textures[0]->height<<'\n';++assets;
 }
 assert(owner.release(e)&&roots->roots().empty()&&!owner.current());assert(owner.release(e));
 camera::GameplaySkyboxV24 missing({});assert(!missing.add("skybox_swamp.bdae",e)&&!missing.current());
 std::cout<<"Positive skybox assets="<<assets<<" buffers="<<buffers<<" draws="<<draws<<" PASS; COMMON renderer and GPU callbacks declared fixtures\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
