#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_quests_v76.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "character_menu_quests_v51.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_services_owner_v5.hpp"
#include "game_event_state_borrow_v74.hpp"
#include <exception>
namespace model_renderer {namespace {
bool required(std::string& e,const char* operation){if(e.empty())e=std::string("Required original campaign condition provider: ")+operation;return false;}
struct ConditionTransportV70 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 bool resolve(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();if(!out||!out->owner||!out->application||!out->player_manager||out->player_manager!=out->application->source_player_manager_v59())return required(e,"SAME retained World/Application/PM lifetime");
  return true;
 }
 bool assertion_mode(std::int32_t& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!resolve(actual,e))return false;
  const auto& p=actual->condition_dependencies_v70;
  if(!p||!p->assertion_owner||!p->assertion_mode)return required(e,"actual process assertion mode cell");
  out=*p->assertion_mode;return true;
 }
 bool assertion(const char* file,std::int32_t line,const char* expression,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!resolve(actual,e))return false;
  const auto& p=actual->condition_dependencies_v70;
  if(!p||!p->assertion_owner||!p->assertion)return required(e,"actual process assertion/diagnostic receiver");
  return p->assertion(file,line,expression,e);
 }
 bool quest(std::uintptr_t character,std::int32_t id,std::int32_t requested,
            dh2::world::NativeConditionStateV69& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!resolve(actual,e))return false;
  SourceCampaignCharacterBorrowV62 borrow;
  if(!borrow_source_campaign_character_v62(actual->owner,character,borrow,e)||!borrow.character)return false;
  const auto& record=borrow.character;
  if(!record->save_fields||!record->save_fields->save_slot14e8())return required(e,"SAME constructor Character.Save14e8 cell");
  const auto saved=*record->save_fields->save_slot14e8();
  if(!saved){out={};return true;} //3bc30c actual NULL-save branch
  if(!record->save||saved!=reinterpret_cast<std::uintptr_t>(record->save.get())||record->save->character()!=character)return required(e,"SAME published Save14e8/SetCharacter owner");
  auto difficulty=requested;
  if(requested==-1){if(!record->services.difficulty_global)return required(e,"actual PlayerSavegame.m_difficultyLevel global");difficulty=record->services.difficulty_global->value();}
  if(difficulty<0||difficulty>=3){e="Original SG_GetQuestByID difficulty indexes outside retained source arrays";return false;}
  //466564 chooses the collection by the real process COnline byte5, not
  //Save.quest_sync_ready14 or an inferred multiplayer/profile flag.
  const auto online=actual->application->get_online_loading_v55();
  if(!online)return required(e,"actual COnline.GetInstance");
  auto& collection=online->byte5()?record->save->volatile_quests_v45():record->save->regular_quests_v45();
  const auto valid=[&](){return id>=0&&static_cast<std::size_t>(id)<collection.source_quests_v45()[static_cast<std::size_t>(difficulty)].size();};
  if(!valid()){
   std::int32_t mode{};if(!assertion_mode(mode,e))return false;
   if(mode==2){e="Original QuestSavegame.GetQuestByID assertion mode2 would write through NULL";return false;}
   if(mode==1&&!assertion("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\SaveGames\\QuestSavegame.cpp",0x126,"id >= 0 && id < (int)m_quests[diff].size()",e))return false;
   //Original checks bounds again AFTER its diagnostic callback.
   if(!valid()){out={};return true;}
  }
  auto quests=record->profile_bootstrap?record->profile_bootstrap->quest_owner_v70():nullptr;
  if(!quests||quests->save()!=record->save)return required(e,"SAME actual initialized profile Quest owner");
  if(!quests->runtime_failure_v76().empty()){e=quests->runtime_failure_v76();return false;}
  //A missing native leaf is an interrupted source prefix, not a successful
  //compile. Its reached cache28 store remains; do not let a subsequent cache
  //early-return turn that retained engineering failure into readiness.
  if(quests->runtime_v76()&&quests->runtime_v76()->failed()){e=quests->runtime_v76()->failure();return false;}
  //46b940 passes true, so CompileQuests(false) is mandatory before returning
  //a state. Callback may mutate source arrays; fetch the actual slot afresh.
  dh2::world::QuestConditionCompileBorrowV70 fields;
  fields.receiver=std::shared_ptr<void>(record->save,&collection);fields.collection=&collection;
  fields.character5c=collection.source_character5c_v70();fields.compiled28=collection.source_compiled28_v70();
  if(*fields.character5c!=character){e="Quest collection Character5c differs from SAME SG_SetPlayer owner";return false;}
  dh2::world::QuestConditionCompileServicesV70 compilation;compilation.transport=record;
  const auto weak_record=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(record);
  compilation.character_difficulty3bb8e4=[weak_record](auto subject,auto& result,auto& error){
   auto record=weak_record.lock();
   if(!record||!record->actor||record->actor->canonical(record).identity!=subject||!record->save_fields||!record->save_fields->save_slot14e8())return required(error,"SAME Character.SG_GetGameDifficulty receiver");
   if(!*record->save_fields->save_slot14e8()){result=-1;return true;}
   if(!record->save||*record->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record->save.get()))return required(error,"SAME Character.Save14e8 for source difficulty");
   if(!record->services.difficulty_global)return required(error,"actual PlayerSavegame difficulty global");
   result=record->services.difficulty_global->value();return true;
  };
  const auto weak_world=world; //already weak; avoid World -> callback -> World cycle
  compilation.quest_compile480178=[weak_world,quests](auto identity,auto& error){
   auto actual=weak_world.lock();
   if(!actual)return required(error,"live campaign Quest.Compile receiver");
   const auto& provider=actual->condition_dependencies_v70;
   if(!provider||!provider->quest_compile_owner||!provider->quest_compile480178)return required(error,"whole Quest.Compile480178 Objective/Reward/marker services");
   return provider->quest_compile480178(quests,identity,error);
  };
  if(!dh2::world::quest_condition_compile_v70(fields,false,compilation,e))return false;
  if(!valid()){e="Quest source array changed to an invalid index during CompileQuests";return false;}
  const auto identity=collection.source_quests_v45()[static_cast<std::size_t>(difficulty)][static_cast<std::size_t>(id)];
  if(!identity){out={};return true;}
  auto* quest=quests->resolve_v70(identity);
  if(!quest||quest->character_owner!=character||quest->difficulty!=difficulty||quest->index!=id)return required(e,"SAME retained selected Quest/state0 receiver");
  out.receiver=std::shared_ptr<void>(quests,quest);out.state0=&quest->state;return true;
 }
 dh2::world::NativeConditionServicesV69 services(const std::shared_ptr<ConditionTransportV70>& self){
  dh2::world::NativeConditionServicesV69 s;s.transport=self;
  s.local_player=[self](auto index,auto flag,auto& out,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   dh2::player::PlayerInfoFieldsV1* info{};if(!actual->player_manager->get_local_player(index,flag,info,e)||!info)return required(e,"actual selected PlayerInfo local lookup");
   out.receiver=std::shared_ptr<void>(actual->player_manager,info);out.player_info=reinterpret_cast<std::uintptr_t>(info);out.character660=&info->character660;return true;};
  s.current_level=[self](auto& out,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;out={};if(!current)return true;
   const auto expected=self->level.lock();if(!expected||current.level()!=expected)return required(e,"SAME published source current Level");
   const auto& f=current.level()->constructor_fields_v3();out={current.level(),current.identity(),&f.row3c,&f.field194};return true;};
  s.quest_state=[self](auto character,auto id,auto requested,auto& out,auto& e){return self->quest(character,id,requested,out,e);};
  s.event_state=[self](auto manager,auto id,auto& out,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   return dh2::loader::borrow_game_event_state_v74(self->level,manager,id,out.receiver,out.state0,e);};
  s.assertion_mode=[self](auto& out,auto& e){return self->assertion_mode(out,e);};
  s.assertion=[self](auto file,auto line,auto expression,auto& e){return self->assertion(file,line,expression,e);};
  return s;
 }
};
}
bool borrow_source_campaign_conditions_v70(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::world::ConditionDataInitServicesV3& services,std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!world->conditions_v70){
  if(world->condition_creation_attempted_v70){e=world->condition_creation_failure_v70.empty()?"Condition arena retains failed initialization prefix; cannot replay":world->condition_creation_failure_v70;return false;}
  world->condition_creation_attempted_v70=true;
  auto fail=[&](){world->condition_creation_failure_v70=e;return false;};
  try{
   if(!world->read||!world->files_owner){required(e,"SAME immutable APK cache reader");return fail();}
   dh2::world::NativeConditionCacheInputsV69 cache;cache.actual_cache=world->files_owner;
   const auto weak=std::weak_ptr<SourceWorldBorrowV61>(world);
   cache.read=[weak](const char* name,bool& found,auto& bytes,auto& e){auto world=weak.lock();if(!world||!world->read)return required(e,"live actual source cache transport");return world->read(name,found,bytes,e);};
   if(!dh2::world::read_native_condition_table_v69(cache,world->condition_tables_v70,e))return fail();
   auto transport=std::make_shared<ConditionTransportV70>();transport->world=world;transport->level=candidate.level;
   if(!dh2::world::NativeConditionRuntimeV69::create(world->condition_tables_v70,transport->services(transport),world->conditions_v70,e))return fail();
  }catch(const std::exception& exception){e=exception.what();return fail();}
 }
 if(!world->condition_dependencies_v70){
  auto dependencies=std::make_shared<SourceConditionDependenciesV70>();
  source_campaign_condition_dependencies_v76(world,*dependencies);world->condition_dependencies_v70=std::move(dependencies);
 }
 out=world->conditions_v70;services=out->condition_data_services();e.clear();return true;
}
bool bind_source_campaign_condition_dependencies_v70(const SourceCampaignCandidateBorrowV55& candidate,
 SourceConditionDependenciesV70 dependencies,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 const auto owns_world=[&](const auto& owner){return owner&&!owner.owner_before(world->owner)&&!world->owner.owner_before(owner);};
 if(!dependencies.assertion_owner||!dependencies.assertion_mode||owns_world(dependencies.assertion_owner)||
    owns_world(dependencies.quest_compile_owner)){e="Condition dependencies require genuine globals/Quest owners without containing World cycles";return false;}
 if(world->condition_dependencies_v70){e="Source condition dependency transport already published";return false;}
 world->condition_dependencies_v70=std::make_shared<SourceConditionDependenciesV70>(std::move(dependencies));e.clear();return true;
}
}
