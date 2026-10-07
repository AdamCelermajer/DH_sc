// Source fixtures explicitly supply network/state/debug external facts.
// Existing positive145/cache/BDAE regression runs first; no runtime acceptance.
#include "../loot_item_target_source_v47.hpp"
#include "../loot_item_frame_source_v47.hpp"
static void v47_item_target_check(dh2::character::WorldItemGraphV3&,dh2::character::WorldItemLiveOwnerV5&,dh2::world::CanonicalObjectManagerV1&,dh2::data::ItemTextServicesV5);
#include "loot_root_v47_cached.inc"
#undef ck
#define ck(value) ((value)?++checks:throw std::runtime_error(std::string("Loot V47 check ")+ #value))
#include "../loot_source_fields_v47.hpp"
#include "../loot_item_frame_source_v47.hpp"
#include "../character_use_ooi_v47.hpp"
struct OoiFixtureV47 {
 std::shared_ptr<void> pin=std::make_shared<int>(1);
 TargetOwner16 target_owner{101,7,0,0};
 TargetState48 target{101,&target_owner,0,0,0,0,0,0,0,0};
 std::uintptr_t ooi=202,actor_id=101,controllable=101;
 std::uint8_t heading=0,action=0,actor_byte=0;
 std::uint32_t forced=0,locked=0,network=1,blocked=0;
 std::uint16_t selected_half=0;std::uint32_t actor_net=0xABCDE1,target_net=0x1234BEEF;
 std::int32_t current=3;bool remote=false,online=false,dead=false,character=false,fail=false;
 int state_calls=0,interaction_calls=0,tail_calls=0,send_calls=0;
 std::uintptr_t tail_request=999;std::vector<std::string> log;
 std::int32_t first_type=-1,second_type=-1;
 bool reload_ooi=false,target_fail=false,send_fail=false;
 CharacterUseOoiFieldsV47 char_fields(){return {pin,101,&target,&ooi,&heading};}
 static bool state(void* p,std::uintptr_t id,std::int32_t& out,std::string&){
  auto& f=*static_cast<OoiFixtureV47*>(p);ck(id==101);++f.state_calls;
  if(f.reload_ooi)f.ooi=303;out=f.current;return !f.fail;
 }
 static bool remotely(void* p,std::uintptr_t id,bool& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);ck(id==101);f.log.push_back("remote");out=f.remote;return !f.fail;}
 static int target_call(void* p,TargetState48*,const TargetRequest24* q,std::uint32_t* out){
  auto& f=*static_cast<OoiFixtureV47*>(p);f.log.push_back("target"+std::to_string(q->service));
  *out=q->service==target_in_sight?1:0;return f.target_fail&&q->service==target_virtual_dead?1:0;
 }
 CharacterUseOoiServicesV47 char_services(){return {this,remotely,state,{this,target_call}};}
 static bool online_call(void* p,bool& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);f.log.push_back("online");out=f.online;return !f.fail;}
 static bool actor_call(void* p,std::uintptr_t id,UseOoiNetworkActorV47& out,std::string&){
  auto& f=*static_cast<OoiFixtureV47*>(p);ck(id==101||id==202||id==303);f.log.push_back("actor"+std::to_string(id));
  out={f.pin,id,&f.ooi,id==101?&f.actor_net:&f.target_net};return !f.fail;
 }
 static bool character_call(void* p,std::uintptr_t,bool& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);f.log.push_back("character");out=f.character;return !f.fail;}
 static bool dead_call(void* p,std::uintptr_t,bool& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);f.log.push_back("dead");out=f.dead;return !f.fail;}
 static bool interaction_call(void* p,std::uintptr_t,std::uintptr_t actor,std::int32_t& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);ck(actor==101);f.log.push_back("interaction");out=f.interaction_calls++?f.second_type:f.first_type;return !f.fail;}
 static bool client_call(void* p,std::uintptr_t& out,std::shared_ptr<void>& pin,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);f.log.push_back("client");out=404;pin=f.pin;return !f.fail;}
 static bool message_call(void* p,const char* name,bool force,UseOoiMessageV47& out,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);ck(std::string(name)=="CMsgControllerAction"&&force);f.log.push_back("factory");out={f.pin,505,&f.action,&f.actor_byte,&f.selected_half};return !f.fail;}
 static bool send_call(void* p,std::uintptr_t client,const UseOoiMessageV47& q,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);ck(client==404&&q.identity==505);++f.send_calls;f.log.push_back("send");return !f.send_fail;}
 static bool tail(void* p,std::uintptr_t id,std::uintptr_t requested,std::string&){auto& f=*static_cast<OoiFixtureV47*>(p);ck(id==101);f.log.push_back("tail");++f.tail_calls;f.tail_request=requested;return !f.fail;}
 ControllerUseOoiFieldsV47 controller_fields(){return {pin,999,&forced,&locked,&network,&blocked,&actor_id,&controllable};}
 ControllerUseOoiServicesV47 controller_services(){return {this,online_call,actor_call,character_call,dead_call,interaction_call,client_call,message_call,send_call,tail};}
};
static int state_fixture(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*){return 0;}
static void v47_item_target_check(WorldItemGraphV3& graph,WorldItemLiveOwnerV5& items,CanonicalObjectManagerV1& manager,ItemTextServicesV5 text){
 std::string e;auto& receiver=graph.receiver_v4();const auto item=receiver.base().identity();
 AiTables ai;ai.rows.resize(9);ai.rows[8].view_radius=10000;PropertyState props;props.resolved[1]=-1;
 PropertyView view;view.resolved=props.resolved.data();OoiFixtureV47 fixture;float owner_position[3]{};
 LootItemTargetSourceV47 extension(items,manager,{&fixture,OoiFixtureV47::target_call},ai,view,owner_position,
  [](void* raw,std::uintptr_t id,const float*& out,std::string&){ck(id==101);out=static_cast<float*>(raw);return true;});
 auto source=extension.borrow();TargetRequest24 request{target_virtual_dead,0,item,nullptr};std::uint32_t result=9;
 ck(source.invoke(source.context,&fixture.target,&request,&result)==0&&result==0);
 request.service=target_in_sight;ck(source.invoke(source.context,&fixture.target,&request,&result)==0&&result==1);
 ck(dh2_character_ai_set_target(&fixture.target,item,0,&source)==0&&fixture.target.target==item&&fixture.target.alive==1&&fixture.target.sight==1);
 auto* node=receiver.base().pointer(0x180);auto* visible=receiver.base().byte(0x80);ck(node&&visible&&!*node);
 const auto old_visible=*visible;*node=1;*visible=1;ck(source.invoke(source.context,&fixture.target,&request,&result)!=0&&extension.error().find("position184")!=std::string::npos);
 *visible=0;ck(source.invoke(source.context,&fixture.target,&request,&result)==0);*visible=old_visible;*node=0;
 dh2::navigation::CollisionWorld geometry;dh2::navigation::Graph paths;dh2::navigation::ObstacleRegistry obstacles;
 dh2::navigation::MotionPolicy motion;dh2::navigation::ControllerWorkspace workspace;
 WorldItemFrameServicesV5 input,output;input.geometry=&geometry;input.paths=&paths;input.obstacles=&obstacles;input.motion=&motion;input.workspace=&workspace;
 input.roots=std::make_shared<GameObjectSceneRootRegistryV1>();LootItemFrameSourceBridgeV47 bridge(graph);ck(bridge.bind(input,output,e));
 float speed{};ck(output.actual_world.invoke(output.actual_world.context,dh2::subobjects::get_speed,&speed)==1&&speed==receiver.fields().speed3b0);
 ck(output.actual_world.invoke(output.actual_world.context,dh2::subobjects::visual_sync_scaling,nullptr)==1);
 LootPickupLeavesServicesV47 leaves;leaves.application_lease=fixture.pin;leaves.item_text=text;
 LootPickupSourceLeavesV47 pickup(leaves);LootInteractResponseV8 response;bool handled{};
 LootInteractRequestV8 format{LootInteractOperationV8::transmute_format,item,101,nullptr,receiver.inventory().peek(),"^s ^s ^d","Sword",9,false,0xff0055};
 ck(pickup.route(format,response,handled,e)&&handled&&response.text=="ff0055 Sword 9");
 format.operation=LootInteractOperationV8::tutorial_enabled;ck(!pickup.route(format,response,handled,e)&&handled);
}
int loot_v47_main(int argc,char** argv){if(positive145_v44(argc,argv))return 1;try{
 std::string e;auto pin=std::make_shared<int>(1);LootPlayerFieldAssociationV47 fields(101);
 LootCharacterSaveBorrowV44 borrowed;ck(fields.borrow_save(pin,borrowed,e)&&!borrowed.save&&!*fields.save_slot14e8());
 ck(!fields.visible80());fields.source_visible_store(7);ck(fields.visible80()&&*fields.visible80()==7);
 auto save=std::make_shared<PlayerSavegameV1>();save->set_character(101);ck(fields.source_save_store_3b36d8(save,e));
 ck(fields.borrow_save(pin,borrowed,e)&&borrowed.save==save.get()&&*fields.save_slot14e8()==reinterpret_cast<std::uintptr_t>(save.get()));
 auto other=std::make_shared<PlayerSavegameV1>();other->set_character(101);ck(!fields.source_save_store_3b36d8(other,e));
 other->set_character(202);LootPlayerFieldAssociationV47 virgin(101);ck(!virgin.source_save_store_3b36d8(other,e)&&!*virgin.save_slot14e8());
 CharacterStateOwner state(101);const std::int32_t* cell=reinterpret_cast<const std::int32_t*>(1);
 ck(loot_state_info_borrow_v47(state,101,cell,e)&&!cell);
 for(int id=-1;id<20;++id){ck(state.transition(id,0,0,{nullptr,state_fixture})==1);ck(loot_state_info_borrow_v47(state,101,cell,e));ck(id<0?!cell:cell==&state.machine().states[id].id&&*cell==id);}
 ck(!loot_state_info_borrow_v47(state,202,cell,e));state.state().current=3;ck(!loot_state_info_borrow_v47(state,101,cell,e));state.state().current=19;ck(state.transition(3,0,0,{nullptr,state_fixture})==1);
 AiTables ai;ai.rows.resize(9);PropertyState properties;PropertyView view;view.resolved=properties.resolved.data();
 bool player{};for(auto type:{0,1,2,-1})for(auto name:{"PlayerCharacter","PlayerCharacterHero","NPCPlayerCharacter","KnightPlayerBase",""}){
  ai.rows[8].type=type;properties.resolved[1]=-99;ck(loot_character_is_player_v47(ai,view,name,player,e));
  ck(player==(type?type==1:std::string(name).find("PlayerCharacter")==0));
 }
 for(int id=-1;id<21;++id)for(bool force:{false,true}){
  OoiFixtureV47 f;f.current=id;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());
  ck(force?owner.force(202,e):owner.control(0,e));bool expected=id==3||id==13||id==18||id==4||id==19;
  ck(f.target.target==(expected?202u:0u)&&f.heading==expected);
  ck(f.state_calls==((id==3||id==13||id==18)?1:2));
 }
 {OoiFixtureV47 f;f.remote=true;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());ck(owner.control(202,e)&&!f.state_calls&&!f.heading);}
 {OoiFixtureV47 f;f.target.target=303;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());ck(owner.use(e)&&!f.state_calls&&!f.heading);}
 {OoiFixtureV47 f;f.ooi=0;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());ck(owner.use(e)&&owner.force(0,e)&&!f.state_calls);}
 {OoiFixtureV47 f;f.reload_ooi=true;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());ck(owner.use(e)&&f.target.target==303&&f.heading==1);}
 {OoiFixtureV47 f;f.target_fail=true;CharacterUseOoiV47 owner(f.char_fields(),f.char_services());ck(!owner.use(e)&&f.target.target==202&&f.target.candidate==202&&!f.heading&&f.target_owner.word14d0==0);}
 for(unsigned forced:{0u,1u,255u})for(unsigned blocked:{0u,1u,255u})for(unsigned locked:{0u,1u,255u}){
  OoiFixtureV47 f;f.forced=forced;f.blocked=blocked;f.locked=locked;ControllerUseOoiV47 owner(f.controller_fields(),f.controller_services());ck(owner.command(0,e));
  ck(f.tail_calls==(forced||(!blocked&&!locked)?1:0));if(f.tail_calls)ck(f.tail_request==0);
 }
 for(bool online:{false,true})for(bool character:{false,true})for(bool dead:{false,true})for(int first:{-1,1,8})for(int second:{-1,1,8}){
  OoiFixtureV47 f;f.online=online;f.character=character;f.dead=dead;f.first_type=first;f.second_type=second;
  ControllerUseOoiV47 owner(f.controller_fields(),f.controller_services());ck(owner.command(0,e));
  bool send=online&&!(character&&dead)&&first!=1&&second!=8;
  ck(f.send_calls==send&&f.tail_calls==1&&f.tail_request==0);
  ck(f.interaction_calls==(!online||(character&&dead)?0:first==1?1:2));
  if(send)ck(f.action==5&&f.actor_byte==0xE1&&f.selected_half==0xBEEF);
 }
 {OoiFixtureV47 f;f.online=true;f.ooi=303;ControllerUseOoiV47 owner(f.controller_fields(),f.controller_services());ck(owner.command(202,e)&&f.tail_request==202);}
 {OoiFixtureV47 f;f.online=true;f.ooi=0;ControllerUseOoiV47 owner(f.controller_fields(),f.controller_services());ck(owner.command(0,e)&&!f.send_calls&&!f.interaction_calls);}
 {OoiFixtureV47 f;f.online=true;f.send_fail=true;ControllerUseOoiV47 owner(f.controller_fields(),f.controller_services());ck(!owner.command(0,e)&&f.send_calls==1&&!f.tail_calls&&f.action==5&&f.selected_half==0xBEEF);}
 std::cout<<"PASS V47 SAME Save/state/NAME, source Cmd/Ctrl/Force/UseOOI gates/reloads/network fields/failure prefixes checks="<<checks<<"; fixture external providers; no root gameplay acceptance\n";return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}
}
