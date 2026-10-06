#include "retained_gameobject_visual_asset_connection_v2.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
using namespace dh2::world;
int main(int argc,char** argv){
 assert(argc==2);unsigned cases=0;
 for(unsigned fail=1;fail<=5;++fail){
  auto lease=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};
  CanonicalGameObjectBaseOwnerV1 base(0x8200+fail,7,lease,runtime);std::string error;
  auto fields=base.properties().fields;assert(fields.write_vector3(fields.context,0x120,{1,1,1},error));
  assert(fields.write_vector3(fields.context,0x160,{0,0,0},error));assert(fields.write_vector3(fields.context,0x16c,{0,0,0},error));
  unsigned stage=0;std::vector<std::string> calls;bool fail_release=false,fail_force=false;
  RetainedGameObjectVisualServicesV1 s;s.owner=lease;
  auto prefix=[&](const char* name,std::string& e){calls.push_back(name);++stage;if(fail<5&&stage==fail){e=std::string("original diagnostic ")+name;return false;}return true;};
  s.read_asset=[&](const std::string& name,auto& bytes,bool& found,auto& e){if(!prefix("read",e))return false;std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
  s.register_root=[&](auto root,auto& e){assert(root);return prefix("register",e);};
  s.force_register=[&](auto,auto& e){if(fail_force){calls.push_back("force-failed");e="required ForceRegister delivery failed";return false;}return prefix("force",e);};
  s.update_pf=[&](auto& e){return prefix("pf",e);};
  s.release_root=[&](auto root,auto& e){assert(root);calls.push_back("release");if(fail_release){e="required root release failed";return false;}return true;};
  auto c=std::make_shared<RetainedGameObjectVisualAssetConnectionV2>(base,s);auto callbacks=c->services(c);
  std::uintptr_t id=123;bool constructed=callbacks.construct(base.identity(),"go_chest_swamp.bdae","",id,error);
  if(fail==5){
   assert(constructed&&id&&c->failed_count()==0&&c->retained_count()==1);
   GameObjectVisualAssetOwnerV1 assets(base,callbacks);assert(assets.set_visual(id,error));
   assert(c->attached()&&c->discard_failed(error)&&c->retained_count()==1);
   assert(!c->discard_unattached(error)&&c->retained_count()==1);
   assert(assets.set_visual(std::uintptr_t{0},error)&&c->retained_count()==0);
   assert(c->discard_unattached(error));++cases;continue;
  }
  assert(!constructed);
  assert(id==0&&c->failed_count()==1&&c->retained_count()==1&&!c->attached());
  std::string original=error;auto before=calls.size();
  if(fail>1){fail_release=true;assert(!c->discard_failed(error)&&c->failed_count()==1);assert(calls.back()=="release");fail_release=false;}
  fail_force=true;assert(!c->discard_failed(error)&&c->retained_count()==1);assert(calls.back()=="force-failed");
  // Source release successfully cleared the root before ForceRegister failed:
  // retry must invoke only ForceRegister, not repeat the completed root drop.
  auto tail=calls.size();fail_force=false;error=original;
  assert(c->discard_failed(error)&&c->retained_count()==0&&c->failed_count()==0);
  assert(error==original&&calls.size()==tail+1&&calls.back()=="force");
  assert(calls.size()>before);assert(c->discard_failed(error)&&c->discard_unattached(error));++cases;
 }
 std::cout<<"Retained failed visual teardown PASS source prefixes="<<cases<<"; actual cache, declared registration/PF failure boundaries\n";
}
