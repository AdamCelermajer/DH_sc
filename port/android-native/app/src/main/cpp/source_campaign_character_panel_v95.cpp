#include "source_campaign_character_panel_v95.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_faery_v109.hpp"
#include "world_map_profile_table_v59.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include <exception>
#include <array>
#include <utility>

namespace dh2::android_ui {
namespace {
bool required(const char* what,std::string& e){e=std::string("Required campaign character-panel ")+what;return false;}
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){
 return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
}
}
SourceCampaignCharacterPanelV95::SourceCampaignCharacterPanelV95(
 model_renderer::SourceCampaignCharacterBorrowV62 selected,
SourceCampaignCharacterPanelServicesV95 services):selected_(std::move(selected)),services_(std::move(services)){}

bool SourceCampaignCharacterPanelV95::bind_campaign_save(
 model_renderer::SourceCampaignCharacterBorrowV62 selected,
 std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> player,
 std::shared_ptr<level::CampaignSaveProfileV45> profile,
 character::CharacterMenuCampaignSaveServicesV50 services,
 data::PlayerSaveLoadServicesV1 readers,
 std::shared_ptr<character::CharacterMenuCampaignSaveV50>& out,std::string& e){
 if(out||!selected.actual_world||!selected.character||!selected.character->load||!player||
    !profile||!readers.owner||!readers.invoke)
  return required("once-only published profile writer and existing full source readers",e);
 const auto& authority=selected.character->load;
 if(authority->profile().identity!=reinterpret_cast<std::uintptr_t>(profile.get())||
    authority->profile().owner.get()!=profile.get())return required("published SAME Save+8 profile",e);
 const std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> weak_character=selected.character;
 const std::weak_ptr<void> weak_world=selected.actual_world;
 auto live=[weak_character,weak_world,player](model_renderer::PlayerGameplayBinding& p,auto& error){
  const auto c=weak_character.lock();const auto world=weak_world.lock();
  if(!c||!world||!player(p,error))return false;
  if(!same_owner(p.world_owner,world)||!p.active||!p.save||p.save!=c->save.get()||
     !c->load||&c->load->save()!=p.save||p.skills!=c->player_script_owner_v62.get()||
     !p.gear||p.gear!=c->prepared_equipment_v60||!p.gear->ready()||
     p.gear->inventory()!=c->inventory37c||p.save->character()!=p.character||
     p.gear->properties()!=c->properties||!p.skills||p.skills->native_savegame()!=p.save)
   return required("fresh SAME selected Character for profile serialization",error);
  model_renderer::SourceCampaignCharacterBorrowV62 published;
  if(!model_renderer::borrow_source_campaign_character_v62(world,p.character,published,error))return false;
  if(published.character!=c)return required("published selected campaign serializer receiver",error);
  return true;
 };
 services.actor=[live](auto& actor,auto& error){
  model_renderer::PlayerGameplayBinding p;if(!live(p,error))return false;
  actor.receiver=p.world_owner;actor.properties=&p.skills->session().property_view();
  actor.inventory=p.gear->inventory();return true;
 };
 services.current_difficulty=[live](auto& difficulty,auto& error){
  model_renderer::PlayerGameplayBinding p;if(!live(p,error))return false;
  difficulty=p.difficulty;return true;
 };
 // Full15-section registration and PROP reader use the exact startup SaveLoad
 // object. No publication, loading mask or profile-cache refresh is replayed.
 out=std::make_shared<character::CharacterMenuCampaignSaveV50>(authority,std::move(profile),std::move(services));
 if(!out->bind(e))return false;
 return authority->bind_services_v50(out->load_services_v50(std::move(readers)),e);
}

bool SourceCampaignCharacterPanelV95::check(const model_renderer::PlayerGameplayBinding& p,std::string& e)const{
 const auto& c=selected_.character;
 if(!c||!same_owner(p.world_owner,selected_.actual_world)||!p.active||
    !p.character||!p.gear||!p.gear->ready()||!p.skills||!p.skills->ready()||
    p.skills!=c->player_script_owner_v62.get()||p.gear!=c->prepared_equipment_v60||
    p.save!=c->save.get()||!c->load||&c->load->save()!=p.save||
    p.save->character()!=p.character||!p.gear->inventory()||
    p.gear->inventory()!=c->inventory37c||p.gear->inventory()->character()!=p.character||
    p.gear->properties()!=c->properties||p.skills->session().properties()!=c->properties||
    p.skills->native_savegame()!=p.save||p.life!=c->life.get()||
    !p.properties||p.properties->resolved!=p.skills->session().property_view().resolved||
    p.properties->saved!=p.skills->session().property_view().saved||
    !c->services.design||!p.design||p.design.characters()!=c->design.characters()||
    !p.skill_tables||!p.skill_list_index||!p.temporary)
  return required("SAME selected V60 Character/SaveLoad/Gear/skills/property backing",e);
 // The fresh factory lookup rejects a retired Character even while this
 // adapter's lease keeps its backing allocated during movie finalization.
 model_renderer::SourceCampaignCharacterBorrowV62 current;
 if(!model_renderer::borrow_source_campaign_character_v62(selected_.actual_world,p.character,current,e))return false;
 if(current.character!=c||!same_owner(current.actual_world,selected_.actual_world))
  return required("current published selected Character",e);
 if(!services_.players||!services_.players->manager())return required("actual Application PlayerManager",e);
 return true;
}
bool SourceCampaignCharacterPanelV95::player(model_renderer::PlayerGameplayBinding& out,std::string& e)const{
 model_renderer::PlayerGameplayBinding p;
 if(!services_.player||!services_.player(p,e))return false;
 if(!check(p,e))return false;
 out=std::move(p);return true;
}
bool SourceCampaignCharacterPanelV95::connect(model_renderer::SourceCampaignCharacterBorrowV62 selected,
 SourceCampaignCharacterPanelServicesV95 services,CharacterPanelMovieRuntimeV3 movie,
 std::shared_ptr<SourceCampaignCharacterPanelV95>& out,std::string& e){
 try{
  if(!selected.actual_world||!selected.character||!services.owner||!services.player||
     !services.players||!movie.owner||!movie.player_index||!movie.current_menu_fx||!movie.invoke)
   return required("retained gameplay/PlayerManager/current movie providers",e);
  auto candidate=std::shared_ptr<SourceCampaignCharacterPanelV95>(
   new SourceCampaignCharacterPanelV95(std::move(selected),std::move(services)));
  model_renderer::PlayerGameplayBinding p;if(!candidate->player(p,e))return false;
  const auto& c=candidate->selected_.character;
  if(c->load->profile().identity&&(!candidate->services_.writer||!candidate->services_.writer->ready()))
   return required("already registered SAME selected-profile campaign writer",e);
  candidate->reload_=std::make_unique<CharacterPanelRuntimeV3>(candidate->selected_.actual_world,
   *p.skills,*p.gear,*candidate->services_.players->manager(),*c->load,
   c->services.design->borrow(),std::move(movie));
  out=std::move(candidate);e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}

bool SourceCampaignCharacterPanelV95::action_graph(const model_renderer::PlayerGameplayBinding& p,
 ui::CharacterMenuActionsGraphV1& g,std::string& e){
 if(!check(p,e))return false;
 auto self=shared_from_this();g.owner=self;g.equipment=p.gear;g.skills=p.skills;g.save=p.save;
 g.skill_tables=p.skill_tables;g.skill_list_index=p.skill_list_index;
 g.save_binding=[self](auto& save,const auto& skills,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  if(&save!=current.save||skills.identity()!=current.skills)return required("same native skill Save binding",error);
  return true;
 };
 const auto* rows=p.design.class_rows();
 g.stats={p.gear->properties().get(),p.gear->property_view(),p.design.characters(),
  rows?rows->data():nullptr,rows?std::uint32_t(rows->size()):0,p.actor_index,{},{}};
 g.stats.debug_load=[self](auto& error){model_renderer::PlayerGameplayBinding current;
  if(!self->player(current,error))return false;
  if(current.debug&&current.debug_files&&dh2_character_debug_load(current.debug,current.debug_files)==1)return true;
  return required("actual Debug.Load",error);
 };
 g.stats.debug_query=[self](const char* name,bool& result,auto& error){model_renderer::PlayerGameplayBinding current;
  if(!self->player(current,error))return false;std::uint32_t value{};
  if(!current.debug||!current.debug_files||dh2_character_debug_get(&value,current.debug,name,current.debug_files)!=1)
   return required("actual Debug.GetSwitch",error);
  result=value!=0;return true;
 };
 g.potion_capacity_store=[self](std::uint8_t value,auto& error){model_renderer::PlayerGameplayBinding current;
  if(!self->player(current,error))return false;
  if(!current.potion_capacity_store)return required("actual Character potion-capacity byte store",error);
  return current.potion_capacity_store(current.potion_capacity_context,value,error);
 };
 g.swap_hud=[self](const char* name,auto& error){model_renderer::PlayerGameplayBinding current;
  if(!self->player(current,error))return false;
  if(!self->services_.swap_hud)return required("actual HUD AS swap continuation",error);
  return self->services_.swap_hud(name,error);
 };
 // CharacterMenuActionsOwner executes the existing complete IncSkill kernel
 // against these SAME saved rows, source constants, properties and byte store.
 return true;
}

bool SourceCampaignCharacterPanelV95::query_graph(const model_renderer::PlayerGameplayBinding& p,
 ui::CharacterMenuActionsOwnerV1& actions,ui::CharacterMenuQueriesGraphV1& g,std::string& e){
 if(!check(p,e))return false;auto self=shared_from_this();
 g.player=[self](auto index,bool remote,auto& identity,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  player::PlayerInfoFieldsV1* info{};
  if(!self->services_.players->manager()->get_player(index,remote,info,error))return false;
  identity=info?info->character660:0;return true;
 };
 g.difficulty=[self](auto& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  // Difficulty belongs to the fresh combat/Level projection; mode118 is a
  // different source field and cannot be used as a difficulty substitute.
  value=current.difficulty;return true;
 };
 g.temporary_binding=[self](const auto& skills,const auto& temporary,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  if(skills.identity()!=current.skills||temporary!=current.temporary)
   return required("same VM temporary skill-info property sheet",error);
  return true;
 };
 auto mutations=services_.mutations;
 character::CharacterMenuFaeryServicesV8 actual_faery;
 if(services_.continuations&&!services_.continuations(p,mutations,actual_faery,e))return false;
 mutations.owner=self;
 mutations.constant=[self](const char* group,const char* key,auto& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  const auto* design=current.design.design();
  if(!design||!design->lookup||design->lookup(design->context,0,group,key,&value)!=0)
   return required("actual GameDesign constant",error);
  return true;
 };
 mutations.is_player=[self](auto identity,bool& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  if(identity!=current.character)return required("selected Character IsPlayer receiver",error);
  return self->selected_.character->is_player(value,error);
 };
 mutations.is_local_player=[self](auto identity,bool& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  return self->services_.players->source_is_local_player_v61(identity,value,error);
 };
 mutations.online=[self](bool& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  const auto& get=self->selected_.character->services.online_byte5;
  if(!get)return required("actual Application GetOnline byte5",error);
  return get(value,error);
 };
 g.item_equippable=[self](const auto& item,bool& value,auto& error){
  model_renderer::PlayerGameplayBinding current;if(!self->player(current,error))return false;
  bool online{},local{};const auto& get=self->selected_.character->services.online_byte5;
  if(!get||!get(online,error))return false;
  if(online&&!self->services_.players->source_is_local_player_v61(current.character,local,error))return false;
  const auto* row=data::item(current.gear->inventory()->table(),item.id);
  if(!row)return required("actual inventory item metadata",error);
  return ui::character_menu_item_equippable_v1(row->record,current.gear->properties()->resolved,
   current.save->class_id(),*current.design.characters(),online,online&&!local,value,error);
 };
 if(selected_.character->load->profile().identity){
  if(!services_.writer||!services_.writer->ready())return required("actual selected-profile writer",e);
  mutations.save=services_.writer->write_services();
 }else mutations.save={}; // Genuine source NULL profile branch, never a slot heuristic.
 if(!character::bind_character_menu_mutations_v4(actions,g,*p.skills,selected_.character->load,std::move(mutations),e))return false;
 if(services_.continuations||services_.faery){
  if(!services_.continuations&&!services_.faery(p,actual_faery,e))return false;
  if(!actual_faery.owner)return required("retained actual faery provider",e);
  if(!character::bind_character_menu_faery_v8(actions,g,*p.skills,std::move(actual_faery),e))return false;
 }
 return true;
}

CharacterPanelGameplayServicesV2 SourceCampaignCharacterPanelV95::gameplay_services(){
 auto self=shared_from_this();CharacterPanelGameplayServicesV2 out;out.owner=self;
 out.actions=[self](const auto& p,auto& graph,auto& e){return self->action_graph(p,graph,e);};
 out.queries=[self](const auto& p,auto& actions,auto& graph,auto& e){return self->query_graph(p,actions,graph,e);};
 out.reload=[self](const auto& p,auto& graph,auto& e){return self->check(p,e)&&self->reload_->bind(p,graph,e);};
 return out;
}
bool SourceCampaignCharacterPanelV95::bind_session(CharacterPanelSessionV1& session,std::string& e){
 model_renderer::PlayerGameplayBinding p;if(!player(p,e))return false;
 session.bind_gameplay_services(gameplay_services(),p.world_owner);return true;
}
bool SourceCampaignCharacterPanelV95::dispatch(CharacterPanelSessionV1& session,const char* name,ui::CharacterMenuCallV1& call,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.dispatch(p,name,call,e);
}
bool SourceCampaignCharacterPanelV95::initialize_authored(CharacterPanelSessionV1& session,
 const ui::AuthoredCharacterPanelServicesV2& services,std::string& e){
 model_renderer::PlayerGameplayBinding p;if(!player(p,e)||!bind_session(session,e))return false;
 return session.initialize_authored(p,services,e);
}
bool SourceCampaignCharacterPanelV95::open(CharacterPanelSessionV1& session,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.authored_open(p,e);
}
bool SourceCampaignCharacterPanelV95::back(CharacterPanelSessionV1& session,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.authored_back(p,e);
}
bool SourceCampaignCharacterPanelV95::tab(CharacterPanelSessionV1& session,unsigned tab,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.authored_tab(p,tab,e);
}
bool SourceCampaignCharacterPanelV95::pointer(CharacterPanelSessionV1& session,int action,int id,float x,float y,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.authored_pointer(p,action,id,x,y,e);
}
bool SourceCampaignCharacterPanelV95::release(CharacterPanelSessionV1& session,const char* path,std::string& e){
 model_renderer::PlayerGameplayBinding p;return player(p,e)&&session.authored_release(p,path,e);
}
bool SourceCampaignCharacterPanelV95::inventory_views(const std::vector<skinning::VisualDrawViewV32>*& out,
 model_renderer::PlayerGameplayBinding& lease,std::string& e){
 out=nullptr;if(!player(lease,e))return false;return lease.gear->draw_views(out,e);
}
bool SourceCampaignCharacterPanelV95::hud_view(const std::int32_t*& resolved,std::size_t& count,
 std::uintptr_t& identity,bool& dead,std::string& e)const{
 resolved=nullptr;count=0;identity=0;dead=false;model_renderer::PlayerGameplayBinding p;
 if(!player(p,e))return false;resolved=p.properties->resolved;count=p.gear->properties()->resolved.size();
 identity=p.character;dead=p.life&&p.life->dead;return true;
}
bool SourceCampaignCharacterPanelV95::equipment_action(int operation,int index,int slot,std::string& e){
 model_renderer::PlayerGameplayBinding p;if(!player(p,e))return false;
 if(!p.life||p.life->dead)return required("living selected Character",e);
 ui::CharacterMenuActionsGraphV1 graph;if(!action_graph(p,graph,e))return false;
 ui::CharacterMenuActionsOwnerV1 actions(std::move(graph));
 switch(operation){
 case 0:{if(index<0)return required("valid inventory index",e);std::int32_t result{};
  return p.gear->auto_equip(std::uint32_t(index),result,e);}
 case 1:if(index<0||slot<0)return required("valid equipment slot/index",e);
  return actions.equip(std::uint32_t(slot),std::uint32_t(index),e);
 case 2:if(slot<0)return required("valid equipment slot",e);return actions.unequip(std::uint32_t(slot),e);
 case 3:return actions.swap(e);
 default:return required("known native equipment operation",e);
}
}
bool SourceCampaignCharacterPanelV95::character_action(int operation,int index,int slot,std::string& e){
 if(operation==0)return equipment_action(0,index,-1,e);
 if(operation==1)return equipment_action(2,-1,slot,e);
 if(operation==5)return equipment_action(3,-1,-1,e);
 model_renderer::PlayerGameplayBinding p;if(!player(p,e))return false;
 if(!p.life||p.life->dead)return required("living selected Character",e);
 ui::CharacterMenuActionsGraphV1 graph;if(!action_graph(p,graph,e))return false;
 ui::CharacterMenuActionsOwnerV1 actions(std::move(graph));
 switch(operation){
 case 2:if(index<0||index>3)return required("valid stat index",e);
  return actions.assign_stat(std::uint32_t(index),e);
 case 3:{if(index<0)return required("valid skill row",e);std::int32_t points{};
  return actions.train_skill(index,points,e);}
 case 4:if(index<0||slot<0||slot>2)return required("valid skill slot/row",e);
  return actions.equip_skill(slot,index,e);
 default:return required("known native character operation",e);
}
}
void SourceCampaignCharacterPanelV95::bind_actor_menu_continuations(
 character::CharacterMenuMutationServicesV4 source,
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
  character::CharacterMenuFaeryServicesV8&,std::string&)> faery){
 // Services carry their real provider lease; preserve it in the adapter even
 // though query_graph's source Frame uses this adapter as its outer owner.
 services_.mutations=std::move(source);services_.faery=std::move(faery);
}

namespace {
struct CampaignMenuWriterTablesV95 {
 character::CharacterGameDesign::Borrow design;
 data::SkillTables::Borrow skills;
 data::ItemPowerTablesV5::Borrow powers;
 data::WorldMapProfileTableV59 maps;
};
bool finish_source_campaign_menu_writer_v95(
 const model_renderer::SourceCampaignCharacterBorrowV62& selected,
 const std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)>& player,
 std::shared_ptr<character::CharacterMenuCampaignSaveV50>& out,std::string& e){
 const auto& record=selected.character;
 if(!record||!record->services.design||!record->profile_bootstrap||!record->save||!record->load||
    record->profile_bootstrap->save()!=record->save||
    record->profile_bootstrap->load_owner()!=record->load)
  return required("SAME selected profile bootstrap before menu composition",e);
 const auto& bootstrap=record->profile_bootstrap;
 if(bootstrap->finished()){
  out=bootstrap->campaign_writer();
  if(!out||!out->ready())return required("completed bootstrap campaign writer",e);
  return true;
 }
 // A failed finish retains its writer. Never replay registration against the
 // same profile and hide a partial section-registration prefix.
 if(bootstrap->campaign_writer())return required("unfailed source campaign writer registration",e);
 // V67 gameplay admission itself requires bootstrap.finished(). Therefore
 // writer registration must use the already constructed canonical receiver,
 // before attempting that gameplay borrow, without fabricating active=true.
 auto* gear=record->prepared_equipment_v60;
 if(!gear||!gear->ready()||gear->inventory()!=record->inventory37c||
    gear->properties()!=record->properties||&record->load->save()!=record->save.get())
  return required("ready SAME canonical Gear/Save before writer finish",e);
 auto tables=std::make_shared<CampaignMenuWriterTablesV95>();
 tables->design=record->services.design->borrow();tables->skills=record->services.skills;
 data::LootTablesV2::Borrow loot;data::ItemTextServicesV5 text;data::LootRandom8V2* random{};
 if(!gear->loot_sources_v8(loot,tables->powers,text,random,e))return false;
 if(!tables->powers||!tables->design.levels())return required("actual Gear PowerNames/Level table cache",e);
 // Borrow the existing Character asset service. Only immutable WorldMap
 // names/default-state metadata is decoded, never a second map/save state.
 character::CharacterFamilyVisualServicesV6 visual;
 if(!record->services.visual||!record->services.visual(*record,visual,e)||!visual.visual.read_asset)
  return required("existing campaign cache asset service",e);
 std::array<std::vector<std::uint8_t>,3> bytes;
 const char* names[]{"data/pydata/worldmap_pyarray.bin","data/pydata/worldmap_pyarraynames.bin","data/pydata/worldmap_pystructnames.bin"};
 for(unsigned i=0;i<3;++i){bool found{};
  if(!visual.visual.read_asset(names[i],bytes[i],found,e))return false;
  if(!found)return required("actual original WorldMap table bytes",e);
 }
 if(!tables->maps.decode({bytes[0].data(),bytes[0].size()},
   {bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},e))return false;
 const std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> weak=record;
 const std::weak_ptr<void> weak_world=selected.actual_world;
 character::CharacterMenuCampaignSaveServicesV50 services;
 services.tables=tables;services.characters=tables->design.characters();services.skills=tables->skills;
 services.power_names=&tables->powers.names();services.level_names=&tables->design.levels()->level_names;
 services.map_names=&tables->maps.names();
 services.actor=[weak,weak_world,player](auto& actor,auto& error){
  auto c=weak.lock();auto world=weak_world.lock();model_renderer::PlayerGameplayBinding current;
  if(!c||!world||!player(current,error))return false;
  if(!same_owner(world,current.world_owner)||!current.skills||current.skills!=c->player_script_owner_v62.get()||
     current.save!=c->save.get()||current.gear!=c->prepared_equipment_v60||
     !current.gear||current.gear->inventory()!=c->inventory37c)
   return required("fresh SAME campaign writer actor/Gear/Save",error);
  struct Pins {std::shared_ptr<void> world;std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;};
  actor.receiver=std::make_shared<Pins>(Pins{world,c});
  actor.properties=&current.skills->session().property_view();actor.inventory=current.gear->inventory();return true;
 };
 services.current_difficulty=[weak](auto& value,auto& error){auto c=weak.lock();
  if(!c||!c->actor||!c->save_fields)return required("actual campaign difficulty receiver",error);
  dh2::player::CharacterSaveDifficultyBorrowV29 actual{c->actor,c->save->character(),
   c->save_fields->save_slot14e8(),c->services.difficulty_global};
  return dh2::player::character_game_difficulty_v29(actual,value,error);
 };
 // Profile writes/job submission are executed by CampaignSaveProfileV45.
 // Only original remaining online/network/checkpoint queries reach here.
 services.remaining={tables,[weak](const auto& request,auto& response,auto& error){auto c=weak.lock();
  if(!c||request.authority!=c->load.get()||request.save!=c->save.get())
   return required("SAME campaign Save writer request",error);
  if(request.operation==data::PlayerSaveWriteOpV1::online){
   if(!c->services.online_byte5)return required("actual Application GetOnline",error);
   return c->services.online_byte5(response.flag,error);
  }
  return required("reached source online/network/checkpoint save continuation",error);
 }};
 // finish retains source Quest receivers and installs writer PROP reload into
 // the same full source reader coordinator. No new standalone Load binding.
 if(!bootstrap->finish(*gear,std::move(services),e)){out=bootstrap->campaign_writer();return false;}
 out=bootstrap->campaign_writer();return out&&out->ready();
}
}
bool connect_source_campaign_character_panel_v95(
 model_renderer::SourceCampaignCharacterBorrowV62 selected,CharacterPanelMovieRuntimeV3 movie,
 std::shared_ptr<SourceCampaignCharacterPanelV95>& out,std::string& e){
 try{
  if(out||!selected.actual_world||!selected.character)return required("new panel over actual selected Character",e);
  model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!model_renderer::borrow_source_campaign_candidate_runtime_v61(current,e))return false;
  if(!same_owner(current.actual_world,selected.actual_world)||!current.application||
     !current.application->source_player_manager_v59())return required("current actual Application PlayerManager",e);
  const std::weak_ptr<void> weak_world=selected.actual_world;
  const auto identity=selected.character->save?selected.character->save->character():0;
  auto player=[weak_world,identity](model_renderer::PlayerGameplayBinding& p,auto& error){
   auto world=weak_world.lock();if(!world)return required("live selected campaign World",error);
   if(!model_renderer::borrow_source_campaign_player_gameplay_v67(world,p,error))return false;
   if(p.character!=identity||!same_owner(p.world_owner,world))return required("current selected campaign player",error);
   return true;
  };
  SourceCampaignCharacterPanelServicesV95 services;services.owner=current.application;
  services.players=current.application->source_player_manager_v59();services.player=player;
  if(!finish_source_campaign_menu_writer_v95(selected,player,services.writer,e))return false;
  model_renderer::PlayerGameplayBinding p;if(!player(p,e))return false;
  const std::weak_ptr<data::PlayerSaveLoadOwnerV1> weak_load=selected.character->load;
  services.continuations=[weak_world,weak_load,identity](const auto& current,
   auto& mutations,auto& faery,auto& error){
   auto world=weak_world.lock();auto expected=weak_load.lock();
   if(!world||!expected||current.character!=identity||!same_owner(current.world_owner,world)||
      current.save!=&expected->save())return required("SAME selected Save for actor menu continuations",error);
   std::shared_ptr<void> actor;std::shared_ptr<data::PlayerSaveLoadOwnerV1> load;
   if(!model_renderer::borrow_source_campaign_menu_continuations_v68(
      world,identity,actor,load,mutations,faery,error))return false;
   if(!actor||load!=expected||!mutations.owner||!faery.owner)
    return required("actual retained actor/SaveLoad mutation/faery services",error);
   if(!model_renderer::borrow_source_campaign_menu_faery_v109(world,identity,faery,error))return false;
   return true;
  };
  character::CharacterMenuFaeryServicesV8 faery;
  if(!services.continuations(p,services.mutations,faery,e))return false;
  return SourceCampaignCharacterPanelV95::connect(std::move(selected),std::move(services),std::move(movie),out,e);
 }catch(const std::exception& failure){e=failure.what();return false;}
}
}
