// Keep the earlier cache-backed player audit unchanged while sharing helpers.
#define main historical_player_objects_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_objects.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_spatial_bindings.hpp"
#include <limits>
namespace {
struct PositionMutation {
 std::shared_ptr<ScriptCharacterObject> owner;
 unsigned calls=0;
 static int invoke(void* p,const dh2_script_value*,std::uint32_t,
  dh2_script_value*,std::uint32_t,std::uint32_t* returned,char*,std::size_t){
  auto& self=*static_cast<PositionMutation*>(p);++self.calls;
  self.owner->position={-13,27,41};*returned=0;return 0;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==5);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);
 CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));
 auto tables=design.borrow();Host host(tables);Debug debug(argv[4]);
 CharacterScriptObjects objects(design.borrow(),debug.owner,&debug.files);
 auto make=[&](const char* preset,std::uintptr_t identity,const std::array<float,3>& point){
  const auto& names=tables.characters()->names;auto found=std::find(names.begin(),names.end(),preset);
  check(found!=names.end());auto p=std::make_shared<dh2::data::PropertyState>();
  dh2::data::reset_properties(*tables.rules(),*p,&tables.characters()->rows[found-names.begin()]);
  check(dh2::data::recalc_properties_with_class(*tables.classes(),*tables.rules(),*p,error));
  return objects.add(identity,preset,p,std::make_shared<dh2::data::CombatActorState>(),point);
 };
 auto owner=make("Crypt_Skeleton",UINT64_C(0x100000002),{10,20,30});
 auto player=make("KnightPlayerBase",UINT64_C(0x100000001),{11,22,33});
 CharacterScriptSessionInput input;input.identity=owner->identity;input.name=owner->name;
 input.properties=owner->properties;input.combat=owner->life;input.position=owner->position;input.source_is_character=1;
 input.common={common.data(),common.size()};input.external={monster.data(),monster.size()};
 input.host=&host.host;input.level=&debug.level;input.target=&owner->binding;input.objects=&objects.services();
 auto session=CharacterScriptSession::create(design.borrow(),input,error);check(session&&error.empty());
 check(!session->start());ScriptSessionView view;check(session->owner().active(view));
 source(view.vm,"function BindPositions(o,p) owner_proxy=o; player_proxy=p end");
 dh2_script_value args[2]{};for(auto& v:args)v.type=DH2_SCRIPT_SOURCE_OBJECT;
 args[0].identity=owner->identity;args[1].identity=player->identity;
 check(!dh2_script_vm_call_discard_source_objects(view.vm,"BindPositions",args,2));
 source(view.vm,"assert(select('#',player_proxy:GetPosition())==3); local x,y,z=player_proxy:GetPosition(); assert(x==11 and y==22 and z==33); x,y,z=owner_proxy:GetPosition(); assert(x==10 and y==20 and z==30); SetTarget(player_proxy); x,y,z=GetTarget():GetPosition(); assert(x==11 and y==22 and z==33)");
 player->position={-111,222,-333};
 source(view.vm,"local x,y,z=GetTarget():GetPosition(nil,true,123,'ignored',player_proxy); assert(x==-111 and y==222 and z==-333); saved_player=GetTarget()");
 PositionMutation mutation{owner};
 check(!dh2_script_vm_bind_source_values(view.vm,"MutatePosition",PositionMutation::invoke,&mutation));
 // Method captures owner identity before the argument's normal _this lookup;
 // the live raw point is read after that lookup's native mutation.
 source(view.vm,"owner_identity=owner_proxy._this; local arg=setmetatable({}, {__index=function(_,k) if k=='_this' then MutatePosition(); owner_proxy._this=player_proxy._this; return player_proxy._this end end}); local x,y,z=owner_proxy:GetPosition(arg); assert(x==-13 and y==27 and z==41); owner_proxy._this=owner_identity; assert(select('#',owner_proxy:GetPosition())==3)");
 check((mutation.calls==1&&owner->position==std::array<float,3>{-13,27,41}));
 source(view.vm,"local fake=setmetatable({}, {__index=function(_,k) if k=='_this' then return player_proxy._this end end}); local x,y,z=owner_proxy.GetPosition(fake); assert(x==-111 and y==222 and z==-333); local ok=pcall(function() owner_proxy.GetPosition({_this=nil}) end); assert(not ok)");
 float nan=0;std::uint32_t nan_bits=0x7fc12345;std::memcpy(&nan,&nan_bits,4);
 player->position={nan,std::numeric_limits<float>::infinity(),-0.0f};
 source(view.vm,"local x,y,z=saved_player:GetPosition(); assert(x~=x and y==math.huge and 1/z==-math.huge)");
 player->position={7,8,9};const auto identity=player->identity;player.reset();
 source(view.vm,"local x,y,z=saved_player:GetPosition(); assert(x==7 and y==8 and z==9); proxy=newproxy(true); getmetatable(proxy).__gc=function() local a,b,c=saved_player:GetPosition(); assert(a==7 and b==8 and c==9); ClearTarget() end");
 session.reset();check((!owner->target.target&&objects.find(identity)->position==std::array<float,3>{7,8,9}));
 Dl_info origin{};check(dladdr(reinterpret_cast<void*>(dh2_character_get_position),&origin));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_monster_Init\":1,\"actual_player_and_monster_points\":true,\"three_raw_numbers\":true,\"capture_before_argument_projection\":true,\"position_read_after_argument_projection\":true,\"ignored_extra_arguments\":true,\"IEEE_values_preserved\":true,\"retained_position_finalizer\":true,\"world_library\":\""<<origin.dli_fname<<"\",\"full_game\":false}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
