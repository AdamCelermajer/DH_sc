#include "source_campaign_items_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <character_candidate_cache_v62.hpp>
#include "renderer_campaign_conditions_v75.hpp"
#include "source_campaign_events_v75.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "source_process_trophies_v100.hpp"
#include "renderer_loot_gpu_v27.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_services_owner_v5.hpp>
#include <application_spawn_random_owner_v4.hpp>
#include <character_loot_actor_binding_v31.hpp>
#include <character_loot_pickup_fx_v32.hpp>
#include <loot_source_fields_v47.hpp>
#include <loot_item_frame_source_v47.hpp>
#include <campaign_frame_scratch_v76.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <canonical_zone_physical_v82.hpp>
#include <floors.hpp>
#include <source_assertion_process_v76.hpp>
#include <character_menu_font_palette_v1.hpp>
#include <script_manager_owner_v52.hpp>
#include <stdexcept>
namespace model_renderer {
bool borrow_source_campaign_inventory_creation_v114(const std::shared_ptr<void>& world,dh2::character::WorldItemLootCreationServicesV10& out,std::shared_ptr<void>& provider,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,source,e)||!source||!source->prepared_items_v88||!source->item_transport_v88){if(e.empty())e="Required SAME prepared Item/creation transport";return false;}
 if(!source->prepared_items_v88->borrow_creation_services_v114(out,e))return false;provider=source->item_transport_v88;e.clear();return true;
}
class SourceCampaignItemsV88 final:public std::enable_shared_from_this<SourceCampaignItemsV88> {
 using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
 struct Provider {std::weak_ptr<SourceCampaignItemsV88> owner;};
 std::weak_ptr<SourceWorldBorrowV61> world_;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level_;
 std::shared_ptr<Provider> provider_=std::make_shared<Provider>();
 std::shared_ptr<dh2::character::SourceItemResourcesV88> resources_;
 std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets_;
 dh2::character::CharacterGameDesign::Borrow design_;
 SourceCampaignItemLeavesV88 leaves_;
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings_;
 std::unique_ptr<dh2::character::CharacterLootActorBindingV31> actors_;
 std::unique_ptr<dh2::character::LootPickupSourceLeavesV47> pickup_leaves_;
 std::shared_ptr<dh2::character::WorldLootGameplayV23> gameplay_;
 std::shared_ptr<void> fx_pin_;
 std::unique_ptr<dh2::character::CharacterLootPickupFxV32> pickup_fx_;
 struct Frame {std::unique_ptr<dh2::character::LootItemFrameSourceBridgeV47> bridge;std::shared_ptr<void> workspace;};
 std::map<std::uintptr_t,Frame> frames_;
 //Capture stack is transport-only. Source GetCurrentLevel is invoked before
 //constant lookup; synchronous nested pickup pins a separate original Level.
 std::vector<SourceCampaignLevelEventsBorrowV88> pickup_levels_;
 bool attempted_{},released_{};
 static std::shared_ptr<SourceCampaignItemsV88> self(void* raw,std::string& e){
  auto p=static_cast<Provider*>(raw);auto s=p?p->owner.lock():nullptr;
  if(!s||s->released_){e="Released actual campaign Item callback owner";return {};}return s;
 }
 bool current(std::shared_ptr<SourceWorldBorrowV61>& w,SourceCampaignCandidateBorrowV55& c,std::string& e)const{
  w=world_.lock();auto level=level_.lock();if(!w||!level||!w->owner||!w->canonical_world||!w->player_manager||!w->player_manager->manager()||
   !borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=w->owner||c.level!=level||c.objects!=w->canonical_world->manager_lease||c.properties!=&w->canonical_world->properties){
   if(e.empty())e="Required SAME current Item World/Level/ObjectManager/PM";return false;}return true;
 }
 bool record(std::uintptr_t id,std::shared_ptr<Record>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!current(w,c,e))return false;
  SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(w->owner,id,b,e)||!b.character||!b.character->actor||!b.character->properties)return false;
  out=std::move(b.character);return true;
 }
 static bool actor_fields(void* p,std::uintptr_t id,dh2::character::CharacterLootActorFieldsV31& out,std::string& e){
  auto s=self(p,e);if(!s)return false;std::uintptr_t character{};if(!cast(p,id,character,e))return false;
  if(!character){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!s->current(w,c,e)||!borrow_source_campaign_object_base_v77(c,id,pin,base,e)||!base)return false;
   out={base->canonical(pin),nullptr,nullptr,nullptr,pin};return true;
  }
  std::shared_ptr<Record> r;if(!s->record(character,r,e))return false;
  auto& actor=*r->actor;auto* properties=r->player_script_owner_v62?&r->player_script_owner_v62->session().property_view():
   actor.session?&actor.session->property_view():&r->view;
  if(!properties||properties->resolved!=r->properties->resolved.data()){e="Item Character property transport differs from actual cached sheet";return false;}
  out={actor.canonical(r),properties,&actor.source_ooi14a4,r->prepared_equipment_v60,r};return true;
 }
 static bool pickup_actor(void* p,std::uintptr_t id,dh2::character::LootPickupActorV23& out,std::string& e){auto s=self(p,e);return s&&s->actors_->pickup_actor(id,out,e);}
 static bool is_player(void* p,std::uintptr_t id,bool& out,std::string& e){auto s=self(p,e);return s&&s->actors_->is_player(id,out,e);}
 static bool cast(void* p,std::uintptr_t id,std::uintptr_t& out,std::string& e){
  auto s=self(p,e);if(!s)return false;if(!id){out=0;return true;}
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s->current(w,c,e))return false;
  std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* o{};bool more=c.objects->source_ordered_begin_v38(key,o);
  while(more){if(o&&o->identity==id){if(!o->as_character){e="Required real Item peer virtual24 Character cast";return false;}return o->as_character(o->context,out,e);}more=c.objects->source_ordered_next_v38(key,key,o);}
  e="Item peer does not belong to actual canonical manager";return false;
 }
 static bool local(void* p,std::uintptr_t id,bool& out,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;return s&&s->current(w,c,e)&&w->player_manager->source_is_local_player_v61(id,out,e);}
 static bool online(void* p,bool& out,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;auto net=w->application->get_online_loading_v55();if(!net){e="Required actual Item COnline receiver";return false;}out=net->byte5()!=0;return true;}
 static bool save_fields(void* p,std::uintptr_t id,dh2::character::LootCharacterSaveBorrowV44& out,const std::uintptr_t*& slot,std::string& e){
  auto s=self(p,e);std::shared_ptr<Record> r;if(!s||!s->record(id,r,e)||!r->save_fields)return false;
  slot=r->save_fields->save_slot14e8();return r->save_fields->borrow_save(r,out,e);
 }
 static bool factory_source(void* p,dh2::data::LootTablesV2::Borrow& tables,dh2::data::ItemPowerTablesV5::Borrow& powers,
  dh2::data::ItemTextServicesV5& text,dh2::data::LootRandom8V2*& rng,std::string& e){
  auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;
  auto random=w->application->source_random_v62();if(!random){e="Required SAME source Random process channel0";return false;}
  tables=s->resources_->loot();powers=s->resources_->definitions();text=s->resources_->text();rng=&random->channel(0);return true;
 }
 static bool drop_actor(void* p,std::uintptr_t id,dh2::character::CharacterLootLiveBorrowV22& out,std::string& e){auto s=self(p,e);return s&&s->actors_->drop_actor(id,out,e);}
 static bool constant(void* p,const char* group,const char* key,std::int32_t& out,std::string& e){auto s=self(p,e);return s&&dh2::character::world_loot_design_constant_v44(s->design_.design(),group,key,out,e);}
 static bool level(void* p,dh2::character::LootCurrentLevelV23& out,std::string& e){
  auto s=self(p,e);if(!s)return false;dh2::loader::CanonicalCurrentLevelBorrowV1 l;if(!borrow_current_native_level_v27(l,e))return false;out={};if(!l)return true;
  if(l.level()!=s->level_.lock()){e="Item current Level differs from source campaign";return false;}
  dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 f;if(!l.level()->loading_fields_v26(f,e))return false;
  out={l.identity(),&l.level()->constructor_fields_v3().mode118,f.state130,l.level()};return true;
 }
 static bool resolve(void* p,dh2::target_providers::Handle16& h,bool assert_null,const dh2::world::CanonicalObjectBorrowV1*& out,std::string& e){
  auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;
  return c.objects->resolve_handle_v4(h,assert_null,out,[](std::string& error){error="Reached original NULL ObjectHandle assertion in Item Spawn";return false;},e);
 }
 struct EnableScope {std::shared_ptr<SourceCampaignItemsV88> owner;std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;};
 static bool local_profile(void* p,bool& character,bool& save,std::uint8_t& byte,std::string& e){
  auto& s=*static_cast<EnableScope*>(p);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s.owner->current(w,c,e))return false;
  dh2::player::PlayerInfoFieldsV1* info{};if(!w->player_manager->manager()->get_local_player(0,true,info,e)||!info)return false;
  character=info->character660!=0;save=false;byte=0;if(!character)return true;
  dh2::character::LootCharacterSaveBorrowV44 fields;const std::uintptr_t* slot{};if(!save_fields(s.owner->provider_.get(),info->character660,fields,slot,e))return false;
  if(!fields.save)return true;save=true;byte=fields.save->source_quest_sync_ready14_v3();return true;
 }
 static bool enable_level(void* p,bool& present,std::int32_t& difficulty,std::string& e){auto& s=*static_cast<EnableScope*>(p);dh2::character::LootCurrentLevelV23 l;if(!level(s.owner->provider_.get(),l,e))return false;present=l.identity!=0;difficulty=present?*l.difficulty118:0;return true;}
 static bool truth(void* p,std::uintptr_t id,bool& out,std::string& e){auto& s=*static_cast<EnableScope*>(p);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s.owner->current(w,c,e)||!w->conditions_v70){if(e.empty())e="Required SAME compiled Item condition arena";return false;}return w->conditions_v70->evaluate(id,out,e);}
 static bool enable_event(void* p,bool value,std::string& e){auto& s=*static_cast<EnableScope*>(p);return s.owner->gameplay_->items()->source_enabled_event_v88(s.item->base().identity(),value,e);}
 static bool test_enable(void* p,const dh2::world::CanonicalObjectBorrowV1& o,bool mark,std::string& e){
  auto s=self(p,e);auto item=s&&s->gameplay_&&s->gameplay_->items()?s->gameplay_->items()->factory().find(o.identity):nullptr;
  if(!item||o.shared_handle!=&item->base().shared_handle()){e="Required SAME source Item enable receiver";return false;}
  EnableScope scope{s,item};auto& b=item->base();bool enabled{};
  return dh2::world::object_test_enable_condition_v2({b.byte(0x8a),b.integer(0xec),b.byte(0xf1),b.pointer(0xa8),b.byte(0xac)},
   {&scope,local_profile,enable_level,truth,enable_event},mark,enabled,e);
 }
 static bool unknown(void*,const char*,std::string& e){e="Actual Item factory unknown source type";return false;}
 static bool debug(void* p,const char* key,bool& out,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e)||!w->debug||!w->debug_files)return false;
  std::uint32_t bits{};if(dh2_character_debug_load(w->debug.get(),w->debug_files)!=1||dh2_character_debug_get(&bits,w->debug.get(),key,w->debug_files)!=1){e="Actual Item Debug source query failed";return false;}out=bits!=0;return true;}
 static bool manager(void* p,dh2::world::CanonicalObjectManagerV1*& out,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;out=c.objects.get();return true;}
 static bool palette(void*,std::int32_t row,std::uint32_t& out,std::string& e){const dh2::ui::CharacterMenuFontPaletteV1* p{};return borrow_actual_font_palette_v4(p,e)&&p&&p->text_color(row,out,e);}
 static bool graph(void* p,dh2::character::RetainedWorldItemObjectV1& receiver,dh2::character::WorldItemGraphServicesV3& out,std::string& e){
  auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;
  auto item=s->gameplay_->items()->factory().find(receiver.base().identity());if(!item||item.get()!=&receiver){e="Required SAME source Item graph";return false;}
  out.visual.owner=s->provider_;std::weak_ptr<SourceWorldBorrowV61> weak=w;
  out.visual.read_asset=[weak](const auto& uri,auto& bytes,bool& found,auto& e){auto w=weak.lock();if(!w||!w->read){e="Released Item visual APK transport";return false;}return w->read(uri,found,bytes,e);};
  // ItemObject virtual80 is IsAnimated3ebbfc (mov r0,#1; bx lr).
  // This is the campaign Item factory's service packet, before the live owner
  // copies it into the retained VisualObject registry.
  out.visual.parent_is_animated=[](bool& animated,std::string& e){animated=true;e.clear();return true;};
  out.initialization.owner=s->provider_;if(!bind_campaign_item_conditions_v75(c,item,out,e))return false;
  std::weak_ptr<SourceCampaignItemsV88> owner=s;
  out.initialization.check_spawn_probability=[owner,item](std::int32_t& roll,std::string& e){auto s=owner.lock();std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;
   auto random=w->application->source_random_v62();auto net=w->application->get_online_loading_v55();if(!random||!net){e="Required actual Item spawn Random/COnline";return false;}
   auto& b=item->base();std::int32_t owner_fc{};if(!b.source_online_owner_fc_v4(owner_fc,e))return false;
   dh2::world::GameObjectSpawnProbabilityBorrowV1 fields;fields.owner=s->provider_;fields.cached_roll270=b.integer(0x270);fields.probability274=b.integer(0x274);
   fields.network_id108=b.integer(0x108);fields.online_owner_fc=&owner_fc;fields.byte82=b.byte(0x82);fields.random0=&random->channel(0);fields.random1=&random->channel(1);
   fields.handle_as_player_character=[item](bool& out,std::string& e){auto o=item->canonical(item);std::uintptr_t id{};if(!o.as_character||!o.as_character(o.context,id,e))return false;out=id!=0;return true;};
   fields.online_byte5=[net](bool& out,std::string&){out=net->byte5()!=0;return true;};
   std::int32_t probability{};return dh2::world::game_object_check_spawn_probability_v1(fields,roll,probability,e);
  };
  out.initialization.device_high_performance=[](bool& out,std::string& e){return borrow_actual_device_high_performance_v54(out,e);};
  out.initialization.light_set_id=[owner](const auto& name,std::int32_t& id,std::string& e){auto s=owner.lock();if(!s||!s->leaves_.light_names){e="Required actual shared LightSet name authority";return false;}id=s->leaves_.light_names->get_id(name);return true;};
  out.position.owner=s->provider_;out.physical.owner=s->provider_;
  out.physical.debug=[owner](const char* key,bool& out,std::string& e){auto s=owner.lock();return s&&debug(s->provider_.get(),key,out,e);};
  out.physical.destroy_previous=[weak](auto id,auto& e){auto w=weak.lock();dh2::world::CanonicalZonePhysicalServicesV82 fields;if(!w||!borrow_source_campaign_physical_services_v90(w,fields,e)||!fields.destroy_previous)return false;return fields.destroy_previous(id,e);};
  out.physical.peer_owner=[weak](void* peer,auto& id,auto& e){auto w=weak.lock();dh2::world::CanonicalZonePhysicalServicesV82 fields;if(!w||!borrow_source_campaign_physical_services_v90(w,fields,e)||!fields.peer_owner)return false;return fields.peer_owner(peer,id,e);};
  out.physical.peer_visible=[weak](auto id,auto& value,auto& e){auto w=weak.lock();dh2::world::CanonicalZonePhysicalServicesV82 fields;if(!w||!borrow_source_campaign_physical_services_v90(w,fields,e)||!fields.peer_visible80)return false;return fields.peer_visible80(id,value,e);};
  out.physical.as_character=[owner](auto id,auto& out,auto& e){auto s=owner.lock();return s&&cast(s->provider_.get(),id,out,e);};
  out.physical.character_ooi=[owner](auto id,auto& out,auto& e){auto s=owner.lock();std::shared_ptr<Record> r;if(!s||!s->record(id,r,e))return false;out=r->actor->source_ooi14a4;return true;};
  out.color.design=w->design->borrow();out.color.font_text_color=palette;return true;
 }
 static bool outer(void* p,const dh2::character::WorldItemRequestV1& q,std::int32_t& out,std::string& e){auto s=self(p,e);if(!s)return false;
  if(q.operation==dh2::character::WorldItemOperationV1::is_moving){std::shared_ptr<Record> r;if(!s->record(q.character,r,e)||!r->actor->machine)return false;
   const std::int32_t* state{};if(!dh2::character::loot_state_info_borrow_v47(r->actor->machine->owner(),q.character,state,e))return false;out=state&&(*state==4||*state==19);return true;}
  if(q.operation==dh2::character::WorldItemOperationV1::is_local_player){bool value{};if(!local(p,q.character,value,e))return false;out=value;return true;}
  if(!s->leaves_.item_outer){e="Required positive Item tooltip/native continuation "+std::to_string(unsigned(q.operation));return false;}return s->leaves_.item_outer(q,out,e);
 }
 static bool tooltip(void* p,std::uintptr_t id,std::uintptr_t& out,std::string& e){auto s=self(p,e);std::shared_ptr<Record> r;if(!s||!s->record(id,r,e))return false;out=r->actor->source_ooi14a4;return true;}
 static bool frame(void* p,dh2::character::WorldItemGraphV3& graph,dh2::character::WorldItemFrameServicesV5& out,std::string& e){
  auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e)||!c.floors||!c.navigation_registry)return false;
  if(!w->frame_scratch_v76)w->frame_scratch_v76=std::make_shared<dh2::navigation::CampaignFrameScratchV76>();
   auto& entry=s->frames_[graph.receiver_v4().base().identity()];out.geometry=&c.floors->collision_world;out.paths=&c.floors->graph;out.obstacles=&c.navigation_registry->registry();out.motion=&c.floors->source_motion_policy_v95;out.roots=c.roots;
  if(!w->frame_scratch_v76->borrow(graph.receiver_v4().runtime().path.count,0,out.obstacles->floor_capacity,entry.workspace,out.workspace,e))return false;
  if(s->leaves_.frame&&!s->leaves_.frame(graph,out,e))return false;
  if(!out.online.online_byte5)out.online={s->provider_.get(),online,nullptr,nullptr};
  if(!entry.bridge)entry.bridge=std::make_unique<dh2::character::LootItemFrameSourceBridgeV47>(graph);
  dh2::character::WorldItemFrameServicesV5 bound;if(!entry.bridge->bind(std::move(out),bound,e))return false;out=std::move(bound);return true;
 }
 static bool status(void* p,const char* text,std::uint32_t metadata,std::string& e){auto s=self(p,e);if(!s||!s->leaves_.enqueue_status){if(e.empty())e="Required actual shared StatusMsg queue";return false;}return s->leaves_.enqueue_status(text,metadata,e);}
 static bool notifications(void* p,dh2::data::LootTemporaryInventoryV8& inv,dh2::data::ItemInstanceV1& item,std::string& e){auto s=self(p,e);if(!s||!s->leaves_.full_notifications){if(e.empty())e="Required original NULL-character inventory full notification tail";return false;}return s->leaves_.full_notifications(inv,item,e);}
 static bool assertion(void* p,const dh2::data::LootEntryRequestV8& q,std::int32_t& out,std::string& e){auto s=self(p,e);if(!s)return false;
  auto process=dh2::world::SourceAssertionProcessV76::borrow();if(!process){e="Required actual process assertion mode";return false;}
  const auto mode=*process->source_level();if(mode!=1&&mode!=2){out=0;return true;}
  if(!s->leaves_.assertion){e="Required original loot assertion diagnostic/fault site "+std::to_string(q.caller);return false;}return s->leaves_.assertion(q,out,e);}
 static bool achievement(void* p,std::uintptr_t,const char* name,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e)||!name)return false;
  dh2::android_ui::ProcessTrophyBorrowV100 trophies;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophies,e)||!trophies.native)return false;
  if(trophies.native->unlock_named(name)){e=trophies.manager?trophies.manager->error():"Original named Item Trophy delivery failed";return false;}return true;
 }
 static bool item_actions(void* p,std::uintptr_t id,dh2::ui::CharacterMenuItemActionsGraphV1& out,std::string& e){auto s=self(p,e);std::shared_ptr<Record> r;if(!s||!s->record(id,r,e)||!r->prepared_equipment_v60||!r->prepared_equipment_v60->bind_menu_item_actions_v4(out,e))return false;
  out.owner=s->provider_;std::weak_ptr<SourceCampaignItemsV88> weak=s;
  out.transmute_multiplier=[weak](auto& value,auto& e){auto s=weak.lock();return s&&constant(s->provider_.get(),"CharacterDesign","TransmuteMultiplier",value,e);};
  out.is_player=[weak,id](auto& value,auto& e){auto s=weak.lock();return s&&is_player(s->provider_.get(),id,value,e);};
  out.is_local_player=[weak,id](auto& value,auto& e){auto s=weak.lock();return s&&local(s->provider_.get(),id,value,e);};
  out.online=[weak](auto& value,auto& e){auto s=weak.lock();return s&&online(s->provider_.get(),value,e);};
  out.achievement=[weak,id](const char* name,auto& e){auto s=weak.lock();return s&&achievement(s->provider_.get(),id,name,e);};return true;
 }
 static bool quest_level(void* p,std::uintptr_t& out,std::string& e){auto s=self(p,e);std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s||!s->current(w,c,e))return false;
  SourceCampaignLevelEventsBorrowV88 captured;if(!borrow_source_campaign_level_events_v88(w->owner,captured,e))return false;out=captured.identity;
  if(out)s->pickup_levels_.push_back(std::move(captured));return true;
 }
 static bool quest_raise(void* p,std::uintptr_t id,const dh2::character::LootPickupQuestEventV10& q,std::string& e){auto s=self(p,e);if(!s||s->pickup_levels_.empty()||s->pickup_levels_.back().identity!=id){e="Pickup quest lost captured original current Level";return false;}
  auto captured=std::move(s->pickup_levels_.back());s->pickup_levels_.pop_back();
  std::uint8_t flag0=q.flag0,flag1=q.flag1;std::int32_t quantity=q.subject_id;std::uintptr_t actor=q.character;
  dh2::loader::GameEventQuestBorrowV75 fields;fields.receiver=captured.level;fields.character8=actor;fields.character8_cell=&actor;fields.id18=q.item_id;fields.id18_cell=&q.item_id;
  fields.pending_network10=&flag0;fields.from_network11=&flag1;fields.quantity14=&quantity;
  dh2::loader::ScopedGameQuestEventV75 event(reinterpret_cast<std::uintptr_t>(&q),&q.objective_type,std::move(fields));
  return raise_source_campaign_level_event_v88(captured,event.borrow(),e);
 }
 static bool remaining(void* p,const dh2::character::LootInteractRequestV8& q,dh2::character::LootInteractResponseV8& out,std::string& e){auto s=self(p,e);if(!s)return false;
  bool handled{};if(!s->pickup_leaves_->route(q,out,handled,e))return false;if(handled)return true;
  if(q.operation==dh2::character::LootInteractOperationV8::tutorial_id||q.operation==dh2::character::LootInteractOperationV8::start_tutorial){
   std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!s->current(w,c,e))return false;
   auto scripts=w->application->source_script_manager_v52();if(!scripts){e="Required SAME actual ScriptManager pickup tutorial receiver";return false;}
   if(q.operation==dh2::character::LootInteractOperationV8::tutorial_id){if(!q.key){e="Required original tutorial CString";return false;}out.value=scripts->id_from_name(q.key,q.flag);return true;}
   return scripts->start_script_v96(q.argument,-1,false,e);
  }
  if(q.operation==dh2::character::LootInteractOperationV8::loot_fx){
   if(!s->pickup_fx_){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;dh2::fx::CharacterMeshFxOwnerV4* fx{};
    if(!s->current(w,c,e)||!w->effects||!borrow_source_campaign_fx_v77(w->owner,s->fx_pin_,fx,e)||!fx)return false;
    s->pickup_fx_=std::make_unique<dh2::character::CharacterLootPickupFxV32>(*fx,w->effects->borrow(),*s->actors_,*s->gameplay_->items());}
   return s->pickup_fx_->route(q,out,handled,e)&&handled;
  }
  if(!s->leaves_.pickup){e="Required positive native Item pickup presentation "+std::to_string(unsigned(q.operation));return false;}return s->leaves_.pickup(q,out,e);
 }
public:
 SourceCampaignItemsV88(std::shared_ptr<SourceWorldBorrowV61> w,const SourceCampaignCandidateBorrowV55& c,SourceCampaignItemLeavesV88 leaves)
  :world_(w),level_(c.level),resources_(w->item_resources_v88),design_(w->design->borrow()),leaves_(std::move(leaves)){}
 bool prepare(std::string& e){if(attempted_){e="Native Item factory retains reached prefix; no reconstruction";return false;}attempted_=true;provider_->owner=shared_from_this();
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!current(w,c,e)||!resources_||!resources_->ready()||!c.floors||!c.navigation_registry||!c.roots||!c.physical_world||!w->debug||!w->debug_files)return false;
  if(!borrow_source_campaign_character_targets_v88(c.actual_world,targets_,e)||!targets_||!design_.ai())return false;
  settings_=w->application->source_settings4c_v67();if(!settings_){e="Required SAME original Application4c pickup settings";return false;}
  actors_=std::make_unique<dh2::character::CharacterLootActorBindingV31>(*targets_,*design_.ai(),dh2::character::CharacterLootActorServicesV31{provider_,provider_.get(),actor_fields});
  dh2::character::LootPickupLeavesServicesV47 leaves;leaves.application_lease=provider_;leaves.context=provider_.get();leaves.actor=pickup_actor;leaves.named_is_player=is_player;leaves.is_local_player=local;leaves.online=online;leaves.save_fields=save_fields;leaves.bind_item_actions=item_actions;leaves.achievement=achievement;
  std::shared_ptr<void> text_owner;if(!borrow_native_item_localization_v88(w->owner,leaves.text,leaves.text_environment,text_owner,e))return false;
  leaves.item_text=resources_->text();leaves.design=design_.design();leaves.settings=settings_.get();leaves.difficulty_global=dh2::player::process_player_save_difficulty_global_v29();
  pickup_leaves_=std::make_unique<dh2::character::LootPickupSourceLeavesV47>(std::move(leaves));
  dh2::character::WorldLootGameplayServicesV23 s;s.application_lease=provider_;s.context=provider_.get();s.factory_source_v88=factory_source;s.debug=w->debug.get();s.debug_files=*w->debug_files;s.presentation=resources_->presentation();s.light_names=leaves_.light_names;
  s.current_level=level;s.frame_services=frame;s.tooltip_ooi=tooltip;s.sound=leaves_.sound;
  s.creation={provider_,provider_.get(),nullptr,assertion,notifications};
  s.loot.provider_lease=provider_;s.loot.context=provider_.get();s.loot.actor=drop_actor;s.loot.is_player=is_player;s.loot.drop.context=provider_.get();s.loot.drop.constant=constant;
  s.items.world=provider_;s.items.roots=c.roots;s.items.context=provider_.get();s.items.graph_services=graph;s.items.outer_item=outer;s.items.factory.context=provider_.get();s.items.factory.resolve=resolve;s.items.factory.test_enable_condition=test_enable;s.items.factory.unknown_type_debug=unknown;
  s.items.destroy_tooltip_v104=[](void* raw,std::uintptr_t id,std::string& error){auto owner=self(raw,error);if(!owner)return false;if(!owner->leaves_.release_item_ui){error="Required actual Item tooltip D1 UI receiver";return false;}return owner->leaves_.release_item_ui(id,error);};
  s.pickup.provider_lease=provider_;s.pickup.context=provider_.get();s.pickup.actor=pickup_actor;s.pickup.character_cast=cast;s.pickup.settings=settings_.get();s.pickup.enqueue_status=status;s.pickup.colors.design=w->design->borrow();s.pickup.colors.font_text_color=palette;
  s.pickup.quests.context=provider_.get();s.pickup.quests.current_game_state=quest_level;s.pickup.quests.constant=constant;s.pickup.quests.raise_async=quest_raise;s.pickup.remaining=remaining;s.pickup.vox_identity=leaves_.vox_identity;
  s.update_prefix={provider_,w->debug.get(),*w->debug_files,provider_.get(),manager};
  gameplay_=std::make_shared<dh2::character::WorldLootGameplayV23>(*targets_,*w->player_manager->manager(),*c.objects,*c.properties,*c.physical_world,&c.floors->collision_world,&c.navigation_registry->registry(),resources_->powers(),resources_->audiovisual(),std::move(s));
  //Publish failed construction prefix as well. The source factory consumes
  //this SAME owner before XML; only stage29 can fill its145 pool later.
  w->prepared_items_v88=gameplay_;return gameplay_->prepare_source_factory_v88(e);
 }
 bool precache(std::string& e){dh2::character::LootCurrentLevelV23 l;if(!level(provider_.get(),l,e)||!l.identity||!l.phase130||*l.phase130!=29){if(e.empty())e="Item145 PreCache requires actual current Level state29";return false;}return gameplay_&&gameplay_->precache_source_pool_v88(e);}
 bool update(std::uint32_t ms,std::uint32_t dt,std::string& e){if(!gameplay_||!gameplay_->update(ms,dt,e))return false;std::vector<dh2::character::LootDrawSceneV23> scenes;if(!gameplay_->draw_scenes(scenes,e))return false;
  std::vector<LootVisualDrawSourceV27> draws;draws.reserve(scenes.size());for(auto& v:scenes)draws.push_back({v.object,std::move(v.visual)});return sync_loot_visual_draws_v27(draws,e);}
 bool update_item(std::uintptr_t id,std::string& e){std::uint32_t ms{},dt{};
  if(!borrow_application_time_v68(ms,e)||!borrow_application_dt_v93(dt,e)||!gameplay_)return false;
  return gameplay_->update_item_source_v104(id,ms,dt,e);
 }
 bool capture(std::string& e){std::vector<dh2::character::LootDrawSceneV23> scenes;if(!gameplay_||!gameplay_->draw_scenes(scenes,e))return false;
  std::vector<LootVisualDrawSourceV27> draws;draws.reserve(scenes.size());for(auto& v:scenes)draws.push_back({v.object,std::move(v.visual)});return sync_loot_visual_draws_v27(draws,e);
 }
 bool release(std::string& e){if(released_){e="Item source release already delivered";return false;}if(!gameplay_||!gameplay_->release(e))return false;pickup_fx_.reset();fx_pin_.reset();frames_.clear();pickup_levels_.clear();released_=true;
  //Campaign roots use the SAME source geometry publisher, retired by the
  //actual scene/Level release. Never borrow legacy Crypt equipment_platform.
  e.clear();return true;}
};
bool prepare_source_campaign_item_resources_v88(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->item_resources_v88){if(world->item_resources_v88->ready()){e.clear();return true;}e="Original source Item resource prefix failed; cannot recreate its authority";return false;}
 std::shared_ptr<const dh2::character::CharacterCandidateCacheV62> cache;
 if(!borrow_source_campaign_character_cache_v81(candidate.actual_world,cache,e)||!cache||!world->design||!world->files_owner)return false;
 dh2::character::SourceItemResourceInputsV88 inputs;inputs.owner=world->files_owner;inputs.loot=cache->loot().borrow();inputs.design=world->design->borrow();
 if(!borrow_native_item_localization_v88(candidate.actual_world,inputs.localization,inputs.text_environment,inputs.localization_owner,e))return false;
 std::weak_ptr<SourceWorldBorrowV61> weak=world;inputs.asset=[weak](const char* uri,bool& found,auto& bytes,auto& e){auto world=weak.lock();if(!world||!uri||!world->read){e="Released actual Item APK/cache transport";return false;}return world->read(uri,found,bytes,e);};
 //Publish the actual failed prefix as well. A later caller cannot overwrite
 //resource identities after another native receiver has borrowed them.
 world->item_resources_v88=std::make_shared<dh2::character::SourceItemResourcesV88>(std::move(inputs));
 return world->item_resources_v88->load(e);
}
bool borrow_source_campaign_item_resources_v88(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<dh2::character::SourceItemResourcesV88>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=actual_world||!borrow_source_campaign_condition_world_v70(candidate,world,e)||
  !world->item_resources_v88||!world->item_resources_v88->ready()){if(e.empty())e="Required actual prepared source Item Arrays/Text authority";return false;}
 out=world->item_resources_v88;e.clear();return true;
}
bool prepare_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55& candidate,
 SourceCampaignItemLeavesV88 leaves,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->item_transport_v88||world->prepared_items_v88){e="Original campaign Item factory already retains its construction prefix";return false;}
 if(!prepare_source_campaign_item_resources_v88(candidate,e)||!bind_native_item_presentation_v88(candidate.actual_world,leaves,e))return false;
 world->item_transport_v88=std::make_shared<SourceCampaignItemsV88>(world,candidate,std::move(leaves));
 return world->item_transport_v88->prepare(e);
}
bool precache_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->item_transport_v88){if(e.empty())e="Required actual preXML campaign Item factory";return false;}return world->item_transport_v88->precache(e);
}
bool update_source_campaign_items_v88(const std::shared_ptr<void>& actual_world,std::uint32_t ms,std::uint32_t dt,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=actual_world||!borrow_source_campaign_condition_world_v70(c,world,e)||!world->item_transport_v88){if(e.empty())e="Required actual campaign Item frame owner";return false;}return world->item_transport_v88->update(ms,dt,e);
}
bool source_campaign_reward_drop_v108(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::int32_t table,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=actual_world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->prepared_items_v88||!w->prepared_items_v88->loot()){if(e.empty())e="Required SAME prepared145 reward Item owner";return false;}
 return w->prepared_items_v88->loot()->drop_explicit_v108(table,character,e);
}
bool release_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->item_transport_v88){if(e.empty())e="Required actual Item release owner";return false;}return world->item_transport_v88->release(e);
}
bool source_campaign_item_update_v104(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->item_transport_v88){if(e.empty())e="Required SAME source Item.Update native visitor";return false;}
 return w->item_transport_v88->update_item(id,e);
}
bool source_campaign_container_drop_v104(const std::shared_ptr<void>& world,std::uintptr_t container,std::int32_t table,std::uintptr_t opener,std::int32_t fixed_powers,bool source_flag,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->prepared_items_v88||!w->prepared_items_v88->ready()||!w->prepared_items_v88->loot()){if(e.empty())e="Required SAME stage29 Item145 pool before Container DropLootTable";return false;}
 return w->prepared_items_v88->loot()->drop_table_v104(table,container,opener,fixed_powers,source_flag,e);
}
bool capture_source_campaign_item_draws_v104(const std::shared_ptr<void>& world,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->item_transport_v88){if(e.empty())e="Required SAME source Item draw capture";return false;}
 return w->item_transport_v88->capture(e);
}
bool capture_source_campaign_item_visuals_v104(const std::shared_ptr<void>& world,std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->prepared_items_v88){if(e.empty())e="Required SAME prepared Item authority before scene capture";return false;}
 return w->prepared_items_v88->source_visuals_v104(out,e);
}
namespace {
bool same_interaction_item_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,source,e)||!source->prepared_items_v88||!source->prepared_items_v88->items()){if(e.empty())e="Required SAME source Item interaction owner";return false;}
 out=source->prepared_items_v88->items()->factory().find(id);const auto* published=out?candidate.objects->object(out->base().shared_handle().key):nullptr;
 if(!out||!published||published->identity!=id||published->shared_handle!=&out->base().shared_handle()){e="Unpublished/foreign actual Item interaction receiver";return false;}return true;
}
}
bool borrow_source_campaign_item_owner_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::shared_ptr<void>& pin,const std::uintptr_t*& owner,std::string& e){std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;if(!same_interaction_item_v107(world,id,item,e))return false;owner=&item->fields().owner3bc;pin=std::move(item);e.clear();return true;}
bool source_campaign_item_interactive_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t character,bool& value,std::string& e){(void)character;std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;return same_interaction_item_v107(world,id,item,e)&&item->is_interactive(value,e);}
bool source_campaign_item_interaction_type_v107(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t character,std::int32_t& value,std::string& e){(void)character;std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;if(!same_interaction_item_v107(world,id,item,e))return false;bool interactive{};if(!item->is_interactive(interactive,e))return false;
 if(interactive&&!item->inventory().peek()){e="Original Item GetInteractionType lost actual GetItem0";return false;}value=-1;e.clear();return true;}
}
