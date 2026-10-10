#include "../native_unused_sweep_v50.hpp"
#include <iostream>
#include <map>
#include <array>
#include <memory>
#include <stdexcept>
#include "../../android-native/app/src/main/cpp/renderer_native_cleanup_retention_v125.inc"
using namespace dh2::resources;
int main(){try{
 unsigned checks=0;
 auto check=[&](bool b){++checks;if(!b)throw std::runtime_error("native unused sweep regression");};
 struct Owner{unsigned generation;};
 std::map<std::uint32_t,Owner> actual{{1,{7}},{2,{7}},{3,{7}},{4,{7}}};
 std::set<std::uint32_t> retained{1,3}; // owning image and independent raw Draw
 std::vector<std::uint32_t> deleted;
 auto validate=[](auto name,const auto& owner,std::string& error){if(!name||owner.generation!=7){error="stale";return false;}return true;};
 auto release=[&](auto name,std::string&){deleted.push_back(name);return actual.erase(name)==1;};
 std::string error;NativeUnusedSweepV50 report;
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted==std::vector<std::uint32_t>({2,4}));
 check(actual.size()==2&&actual.count(1)&&actual.count(3));
 check(report.inspected==4&&report.preserved==2&&report.released==2);
 deleted.clear();
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted.empty()&&report.inspected==2&&report.preserved==2&&report.released==0);
 // A stale last allocation aborts the entire pass before deleting an earlier
 // otherwise-unused name. Preserve the caller's previous completed receipt.
 actual.emplace(2,Owner{7});actual.emplace(5,Owner{6});
 check(!sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted.empty()&&actual.size()==4&&report.inspected==2);
 actual.erase(5);error.clear();
 auto failure=[&](auto name,std::string& e){deleted.push_back(name);e="actual release failed";return false;};
 check(!sweep_native_unused_v50(actual,retained,validate,failure,report,error));
 check(deleted==std::vector<std::uint32_t>({2})&&report.released==0&&error=="actual release failed");
 actual.clear();deleted.clear();
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(report.inspected==0&&report.released==0&&deleted.empty());
 // Stage24 creates owners before any visible draw list is published. The
 // Stage25 collector must preserve ordinary Draw streams and nested shader
 // attribute/sampler names, then release them after their owner is removed.
 struct Shader {std::array<std::uint32_t,18> attributes{};std::vector<std::uint32_t> sampler_textures;};
 struct Draw {std::uint32_t diffuse{},alpha{},vertices{},indices{};std::shared_ptr<Shader> native_shader_v113;};
 struct Batches {std::vector<Draw> batches;std::vector<std::uint32_t> private_images;};
 struct Geometry {std::map<int,Batches> meshes,actors,objects,compiled;std::vector<Draw> skybox_draws_v124;std::vector<std::uint32_t> textures;};
 Geometry campaign,menu;
 auto shader=std::make_shared<Shader>();shader->attributes[0]=21;shader->attributes[7]=22;shader->sampler_textures={31,32};
 campaign.compiled[1].batches.push_back({0,0,0,20,shader});
 campaign.meshes[1].batches.push_back({33,0,23,24,{}});
 campaign.actors[1].private_images={34};campaign.objects[1].batches.push_back({35,0,25,26,{}});
 campaign.skybox_draws_v124.push_back({36,0,27,28,{}});campaign.textures={37};
 menu.actors[1].batches.push_back({38,0,29,30,{}});menu.actors[1].private_images={39};
 std::set<std::uint32_t> texture_uses,buffer_uses;
 retain_native_geometry_resources_v125(campaign,texture_uses,buffer_uses);
 retain_native_geometry_resources_v125(menu,texture_uses,buffer_uses);
 check(texture_uses==std::set<std::uint32_t>({31,32,33,34,35,36,37,38,39}));
 check(buffer_uses==std::set<std::uint32_t>({20,21,22,23,24,25,26,27,28,29,30}));
 std::map<std::uint32_t,Owner> buffers,textures;
 for(auto name:buffer_uses)buffers.emplace(name,Owner{7});
 buffers.emplace(90,Owner{7});
 for(auto name:texture_uses)textures.emplace(name,Owner{7});
 textures.emplace(91,Owner{7});
 auto sweep=[&](auto& registry,const auto& uses){return sweep_native_unused_v50(registry,uses,validate,[&](auto name,std::string&){deleted.push_back(name);return registry.erase(name)==1;},report,error);};
 check(sweep(buffers,buffer_uses)&&sweep(textures,texture_uses));
 check(deleted==std::vector<std::uint32_t>({90,91})&&buffers.size()==11&&textures.size()==9);
 // Invisible/undrawn owners stay alive, but retirement removes their names
 // from the next snapshot. No names are retained by the collector itself.
 campaign={};menu={};texture_uses.clear();buffer_uses.clear();deleted.clear();
 retain_native_geometry_resources_v125(campaign,texture_uses,buffer_uses);
 retain_native_geometry_resources_v125(menu,texture_uses,buffer_uses);
 check(sweep(buffers,buffer_uses)&&sweep(textures,texture_uses));
 check(buffers.empty()&&textures.empty()&&deleted.size()==20);
 std::cout<<"PASS "<<checks<<" native registry/ownership/preflight/release checks\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
