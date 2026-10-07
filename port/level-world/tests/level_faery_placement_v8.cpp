#include "../level_faery_placement_v8.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::world;
namespace {
unsigned checks{};void need(bool ok,const std::string& e){++checks;if(!ok)throw std::runtime_error(e);}
struct Actor {std::uintptr_t identity;std::uint32_t type{};std::uint8_t across{};std::int32_t room{-1};dh2::target_providers::Handle16 handle{};std::string name;
 static bool text(void*,const char*,std::string&){return true;}
 static bool as_char(void* raw,std::uintptr_t& id,std::string&){id=static_cast<Actor*>(raw)->identity;return true;}
};
}
int main(){try{
 std::string error;CanonicalObjectManagerServicesV1 manager_services;
 manager_services.assign_network_id=[](void*,auto&,auto&){return true;}; // Declared offline network fixture.
 CanonicalObjectManagerV1 manager(manager_services);std::vector<std::shared_ptr<Actor>> owners;
 for(unsigned i=1;i<=12;++i){auto a=std::make_shared<Actor>();a->identity=i;owners.push_back(a);
  CanonicalObjectBorrowV1 b{a->identity,a,&a->handle,&a->type,&a->across,&a->room,a.get(),Actor::text,Actor::text,Actor::as_char};
  dh2::target_providers::Handle16 handle;need(manager.add(b,("Character"+std::to_string(i)).c_str(),"Fixture",-1,true,handle,error),error);}
 need(manager.characters().size()==12,"Actual retained manager Character publication");
 LevelFaeryPlacementServicesV8 s;s.owner=owners[0];s.objects=&manager;
 std::vector<std::string> trace;unsigned faery=0,follower=0;std::uintptr_t field{},master=77;
 s.char_type=[&](auto id,auto& type,auto&){trace.push_back("type"+std::to_string(id));type=id==faery?3:id==follower?2:4;return true;};
 s.look_at_vec=[&](auto id,auto& p,auto&){need(id==1,"LookAtVec SAME selected");trace.push_back("look");p={2,3,4};return true;};
 s.target_position=[&](auto id,auto& p,auto&){need(id==1,"TargetPosition SAME selected");trace.push_back("target");p={10,20,30};return true;};
 s.set_position=[&](auto,const auto& p,bool flag,auto&){need(flag&&p==std::array<float,3>{12,23,34},"Source placement arithmetic/flag");trace.push_back("position");return true;};
 s.force_position=[&](auto,auto&){trace.push_back("force");return true;};
 s.disable_zoning=[&](auto id,auto&){need(id==follower,"Follower zoning identity");trace.push_back("zoning");return true;};
 s.source_player_count6c4=[&](auto& count,auto&){count=1;trace.push_back("count");return true;};
 s.player=[&](auto index,bool flag,auto& id,auto&){need(index==0&&!flag,"GetPlayer arguments");id=1;trace.push_back("player");return true;};
 s.faery420_field=[&](auto id,auto*& out,auto&){need(id==1,"Same CharAI58 field");out=&field;trace.push_back("420");return true;};
 s.master50=[&](auto,auto& value,auto&){value=master;trace.push_back("master-read");return true;};
 s.local_player=[&](auto index,bool flag,auto& id,auto&){need(index==0&&flag,"GetLocalPlayer arguments");id=1;trace.push_back("local");return true;};
 s.set_master=[&](auto,auto id,auto&){master=id;trace.push_back("master"+std::to_string(id));return true;};
 s.selected_faery=[&](auto id,auto& chosen,auto&){need(id==1,"Same selected Save");chosen=2;trace.push_back("chosen");return true;};
 s.change_faery=[&](auto id,auto chosen,auto&){need(id==1&&chosen==2,"Nested same ChangeFaery");trace.push_back("change");return true;};
 need(level_place_faery_followers_v8(1,s,error),error);need(trace.size()==24,"Nonqualifying source list traversed twice");
 faery=3;follower=5;trace.clear();need(level_place_faery_followers_v8(1,s,error),error);
 need(field==3&&master==77,"Source faery association and master restoration");
 const std::vector<std::string> expected{"look","target","position","force","count","player","420","count","master-read","local","master1","chosen","change","master77","look","target","position","force","zoning"};
 std::vector<std::string> reached;for(const auto& t:trace)if(t.rfind("type",0)!=0)reached.push_back(t);need(reached==expected,"Actual reached source order differs");
 auto positive=s;s.change_faery={};trace.clear();master=77;need(!level_place_faery_followers_v8(1,s,error)&&master==1&&field==3,"Missing nestedChange must preserve source mutationprefix");
 need(error.find("nested")!=std::string::npos,error);error.clear();need(!level_place_faery_followers_v8(0,positive,error),"Null caller must require real online/local continuation");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"same_manager_characters\":12,\"source_faery_follower_order_and_failure_prefix\":true,\"world_body_visual_player_endpoints\":\"declared fixtures\"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
