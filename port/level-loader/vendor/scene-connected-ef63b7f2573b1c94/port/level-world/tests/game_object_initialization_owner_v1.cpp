#include "game_object_initialization_owner_v1.hpp"
#include "game_object_visual_asset_owner_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace dh2::world;
namespace {
unsigned checks{};
void check(bool v){++checks;if(!v){std::fprintf(stderr,"FAIL check %u\n",checks);std::exit(1);}}
struct Fixture {
 std::shared_ptr<int> lease=std::make_shared<int>(1);
 dh2::actor::RuntimeState runtime{};
 CanonicalGameObjectBaseOwnerV1 base{0xabcdef,7,lease,runtime};
 CanonicalPropertyMapV1 properties{{nullptr,&canonical_vec3_origin_v1(),nullptr}};
 std::vector<std::string> difficulties{"Easy","Normal","Hard"},sounds{"a","idle"};
 std::vector<std::string> calls;unsigned fail{};std::int32_t roll{-2};bool high{true};std::string error;
 Fixture(){base.class_name20()="GameObject";auto actor=base.properties();check(properties.init_properties(actor,error)&&properties.load_defaults(actor,error));}
 bool call(const std::string& name){calls.push_back(name);if(fail==calls.size()){error="declared reached fixture failure";return false;}return true;}
 GameObjectInitializationServicesV1 services(){
  GameObjectInitializationServicesV1 s;s.owner=lease;s.difficulty_names=&difficulties;s.sound_names=&sounds;
  s.condition_init=[this](std::uint32_t o,std::string&){return call(o==0x8c?"condition8c":"conditionb0");};
  s.check_spawn_probability=[this](std::int32_t& out,std::string&){out=roll;return call("spawn");};
  s.set_position=[this](const float* p,bool destination,std::string&){check(p==base.vector3(0x160)&&destination);base.update_absolute_aabb();return call("position");};
  s.device_high_performance=[this](bool& v,std::string&){v=high;return call("device");};
  s.load_visual=[this](std::string&){if(!call("load"))return false;*base.pointer(0x2d8)=901;return true;};
  s.visual_sync=[this](std::uintptr_t p,std::string&){check(p==901);return call("sync");};
  s.set_visible=[this](bool v,std::string&){check(v);return call("visible");};
  s.init_pf_object=[this](bool stat,const float* pos,float radius,std::uintptr_t id,std::string&){check(!stat&&pos==runtime.subobjects.position&&radius==400&&id==base.identity());return call("pfinit");};
  s.light_set_id=[this](const std::string& name,std::int32_t& id,std::string&){check(name=="PlayerLight");id=6;return call("light");};
  s.visual_set_light_set=[this](std::uintptr_t visual,std::int32_t id,std::string&){check(visual==901&&id==6);return call("lightstore");};
  s.visual_root=[this](std::uintptr_t visual,std::uintptr_t& root,std::string&){check(visual==901);root=902;return call("root");};
  s.node_from_name=[this](std::uintptr_t root,const char* name,std::uintptr_t& node,std::string&){check(root==902&&std::string(name)=="target_node");node=903;return call("node");};
  s.update_pf_object=[this](std::string&){return call("pfupdate");};return s;
 }
 void authored(){auto a=base.properties();check(properties.set_property(a,"scale","2,0,-3",error));check(properties.set_property(a,"rotation","90,180,-90",error));check(properties.set_property(a,"position","10,20,30",error));check(properties.set_property(a,"skip_checkpoint","1",error));check(properties.set_property(a,"idle_sound","idle",error));check(properties.set_property(a,"min_difficulty","Hard",error));*base.byte(0x28)=1;*base.pointer(0x2dc)=987;}
};
}
int main(){
 const std::vector<std::string> post{"condition8c","conditionb0","spawn","position","device","load","sync"};
 {Fixture f;f.authored();GameObjectInitializationOwnerV1 o(f.base,f.services());bool eligible;check(o.init_post(eligible,f.error)&&eligible);check(f.calls==post);check(*f.base.integer(0xec)==2&&*f.base.integer(0x370)==1&&*f.base.pointer(0x2dc)==0&&*f.base.byte(0x28)==0);auto* s=f.base.vector3(0x120);check(s[0]==2&&s[1]==1&&s[2]==-3);check(std::fabs(f.runtime.rotation.rotation[0]-1.5707964f)<0.000001f&&*f.base.scalar(0x178)==f.runtime.rotation.rotation[2]);check(f.runtime.subobjects.local_bounds[0]==-200&&f.runtime.subobjects.local_bounds[3]==200&&f.runtime.subobjects.absolute_bounds[0]==-190);f.calls.clear();*f.base.byte(0x2ed)=1;check(o.init_final(eligible,f.error)&&eligible);check(f.calls==std::vector<std::string>({"spawn","visible","pfinit","sync","light","lightstore","root","node","pfupdate"}));check(*f.base.pointer(0x180)==903&&*f.base.string(0x254)==*f.base.string(0x48));}
 for(unsigned fail=1;fail<=post.size();++fail){Fixture f;f.authored();f.fail=fail;GameObjectInitializationOwnerV1 o(f.base,f.services());bool eligible;check(!o.init_post(eligible,f.error));check(f.calls==std::vector<std::string>(post.begin(),post.begin()+fail));if(fail<=3)check(*f.base.pointer(0x2dc)==987&&f.runtime.rotation.rotation[0]==90);if(fail>=4)check(*f.base.pointer(0x2dc)==0);}
 {Fixture f;f.authored();f.roll=100;GameObjectInitializationOwnerV1 o(f.base,f.services());bool eligible;check(o.init_post(eligible,f.error)&&!eligible&&f.calls.size()==3&&*f.base.pointer(0x2dc)==987);f.calls.clear();check(o.init_final(eligible,f.error)&&!eligible&&f.calls.size()==1);}
 {Fixture f;f.authored();f.high=false;*f.base.byte(0x10c)=1;GameObjectInitializationOwnerV1 o(f.base,f.services());bool eligible;check(o.init_post(eligible,f.error)&&eligible&&*f.base.pointer(0x2d8)==0);check(f.calls==std::vector<std::string>({"condition8c","conditionb0","spawn","position","device"}));}
 {Fixture f;auto s=f.services();s.condition_init={};GameObjectInitializationOwnerV1 o(f.base,s);bool eligible;check(!o.init_post(eligible,f.error)&&f.calls.empty());*f.base.byte(0x81)=1;check(o.init_final(eligible,f.error)&&!eligible&&f.calls==std::vector<std::string>({"spawn"}));}
 {Fixture f;std::vector<std::uintptr_t> dead;bool miss=false;unsigned constructs=0;GameObjectVisualAssetServicesV1 s;s.owner=f.lease;
  s.construct=[&](std::uintptr_t id,const std::string& model,const std::string& xref,std::uintptr_t& receiver,std::string&){check(id==f.base.identity()&&model=="chest"&&xref=="lid");++constructs;receiver=1000;return true;};
  s.root=[&](std::uintptr_t receiver,std::uintptr_t& root,std::string&){check(receiver==1000);root=miss?0:1001;return true;};
  s.destroy=[&](std::uintptr_t receiver,std::string&){dead.push_back(receiver);return true;};
  s.set_root_game_object=[&](std::uintptr_t root,std::uintptr_t id,std::string&){check(root==1001&&id==f.base.identity());return true;};
  GameObjectVisualAssetOwnerV1 o(f.base,s);*f.base.pointer(0x2d8)=777;
  miss=true;check(o.set_visual("chest","lid",true,f.error));check(*f.base.pointer(0x2d8)==777&&dead==std::vector<std::uintptr_t>({1000}));
  miss=false;check(o.load_visual(f.error));check(*f.base.pointer(0x2d8)==1000&&dead==std::vector<std::uintptr_t>({1000,777}));
  check(o.set_visual("chest",nullptr,false,f.error)&&constructs==2&&*f.base.string(0x2a8)=="lid");
  check(o.set_visual(std::uintptr_t{1000},f.error)&&dead.size()==2);
  check(o.set_visual(nullptr,"ignored",false,f.error));check(*f.base.pointer(0x2d8)==0&&f.base.string(0x290)->empty()&&f.base.string(0x2a8)->empty()&&dead.back()==1000);
 }
 std::printf("GameObject init/visual lifetime PASS %u checks; real base/default storage, declared condition/visual/PF service fixtures\n",checks);return 0;
}
