#include "../app/src/main/cpp/native_menu_preview_lifecycle_v123.hpp"
#include "../../level-world/character_clean_v123.hpp"
#include "../../level-world/character_update_pointers_v105.hpp"
#include "../../level-world/source_process_objects_v121.hpp"
#include "../../level-world/canonical_character_spawn_select_v87.hpp"
#include "../../level-world/canonical_character_save_v86.hpp"
#include "../../level-world/canonical_point3d_globals_v1.hpp"
#include "../../level-world/character_ai_groups_v87.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <vector>
using namespace dh2;
using Record=world::CanonicalCharacterCandidateRecordV60;
namespace {
void check(bool value,const std::string& why){if(!value)throw std::runtime_error(why);}
std::shared_ptr<Record> fresh(std::uintptr_t id,const std::shared_ptr<void>& domain,const char* name){
 auto r=std::make_shared<Record>();r->services.world=domain;r->properties=std::make_shared<data::PropertyState>();r->life=std::make_shared<data::CombatActorState>();
 r->actor=std::make_shared<character::RetainedCharacterActorV1>(id,domain,"Character",character::RetainedCharacterConstructionV7::fresh_canonical);
 r->save_fields=std::make_shared<character::LootPlayerFieldAssociationV47>(id);
 auto object=std::make_shared<character::ScriptCharacterObject>(id,name,r->properties,r->life,std::array<float,3>{});
 check(r->actor->construct_fields(object,{}),r->actor->error());
 std::string e;world::CanonicalPropertyMapV1 properties({nullptr,&world::canonical_vec3_origin_v1()});
 auto fields=r->actor->properties();check(properties.init_properties(fields,e),e);check(properties.load_defaults(fields,e),e);
 return r;
}
void actual_empty_owner(){
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();std::string e;
 std::shared_ptr<world::SourceProcessObjectsV121> process;check(world::SourceProcessObjectsV121::acquire(app,process,e),e);
 model_renderer::MenuPreviewProcessDomainV121 domain;domain.owner=std::make_shared<int>(1);domain.objects=process->manager();
 domain.scene=std::make_shared<world::GameObjectSceneRootRegistryV1>();domain.physical=std::make_shared<physical::NativeWorld>();
 std::shared_ptr<model_renderer::NativeMenuPreviewLifecycleV123> owner;
 check(model_renderer::acquire_native_menu_preview_lifecycle_v123(app,domain,owner,e),e);
 auto other=std::make_shared<application::ApplicationServicesOwnerV5>();std::shared_ptr<model_renderer::NativeMenuPreviewLifecycleV123> rejected;
 check(!model_renderer::acquire_native_menu_preview_lifecycle_v123(other,domain,rejected,e),"foreign App admitted same process manager");
 check(process->flush([](auto&,auto& error){error.clear();return true;},e),e);
 check(process->manager()->source_flush_complete_v121(),"empty real process owner Flush did not produce its native receipt");
 std::weak_ptr<application::ApplicationServicesOwnerV5> weak=app;app.reset();check(weak.expired(),"process lifecycle creates an App cycle");
}
void native_flush_order(){
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();std::string e;
 std::shared_ptr<world::SourceProcessObjectsV121> process;check(world::SourceProcessObjectsV121::acquire(app,process,e),e);
 auto manager=process->manager();auto domain=std::make_shared<int>(2);
 auto first=fresh(101,domain,"PreviewOne"),second=fresh(102,domain,"PreviewTwo");
 std::map<std::uintptr_t,std::shared_ptr<Record>> records{{101,first},{102,second}};
 for(auto& at:records){at.second->services.canonical_objects=manager;target_providers::Handle16 h;
  check(manager->add(at.second->actor->canonical(at.second),at.first==101?"PreviewOne":"PreviewTwo","Warrior",-1,true,h,e),e);
 }
 auto& a=*first->actor;auto& b=*second->actor;
 a.object->target.target=102;a.object->target.last_target=102;a.source_ooi14a4=102;a.kill_fields_v42().killer144c=102;
 a.source_ai_pointers_v105()->master50=102;a.source_ai_pointers_v105()->auxiliary58=102;a.source_ai_pointers_v105()->observers5c.emplace(7,102);
 b.object->target.target=101;b.source_ai_pointers_v105()->observers5c.emplace(9,101);
 first->timer_util_v107=std::make_unique<character::CharacterTimerUtilFieldsV106>();const auto timer=reinterpret_cast<std::uintptr_t>(first->timer_util_v107.get());a.source_frame_fields_v106()->timer14e0=timer;
 // Transport fixtures below observe draw/FX retirement boundaries. Native
 // Character/Delete/TargetList/AI/Clean/D0/storage bodies execute unchanged.
 std::vector<std::string> order;std::map<std::uintptr_t,bool> draws{{101,true},{102,true}};
 world::CanonicalObjectLifecycleV1 lifecycle;lifecycle.owner=domain;
 auto record=[&](const auto& object){auto r=records.at(object.identity);check(r==object.lease&&r->actor->object,"changed native record loan");return r;};
 lifecycle.flush_target_list=[&](const auto& object,auto& error){order.push_back("target"+std::to_string(object.identity));return record(object)->actor->source_target_list304_v111()->flush_backup_results_v111(error);};
 lifecycle.object_delete=[&](const auto& object,auto&){order.push_back("delete"+std::to_string(object.identity));record(object)->actor->source_delete_v111();return true;};
 lifecycle.ai_update_pointers=[&](const auto& object,auto& error){
  auto r=record(object);auto& a=*r->actor;check(*first->actor->source_bool_field(0x81)&&*second->actor->source_bool_field(0x81),"AI pass ran before every source Delete");order.push_back("ai"+std::to_string(object.identity));
  return character::character_update_pointers_v105(a.object->target,*a.source_ai_pointers_v105(),a.source_ooi14a4,a.kill_fields_v42().killer144c,[&](auto id,auto& disabled,auto& e){disabled=records.at(id)->actor->source_bool_field(0x81);e.clear();return disabled!=nullptr;},error);
 };
 lifecycle.class_d0=[&](const auto& object,auto& error){
  auto r=record(object);order.push_back("d0"+std::to_string(object.identity));
  check(!r->actor->object->target.target,"D0 ran before cross-receiver AI pointer pass");
  world::CharacterCleanServicesV123 clean;
  clean.retire_draws=[&](auto& e){check(manager->object(object.shared_handle->key)!=nullptr,"draw retirement must precede manager map erase");draws.at(object.identity)=false;order.push_back("draw"+std::to_string(object.identity));e.clear();return true;};
  clean.detach_fx_anchors=[&](auto& e){check(!draws.at(object.identity),"FX detach preceded draw retirement");order.push_back("fx"+std::to_string(object.identity));e.clear();return true;};
  if(!world::character_clean_v123(*r,clean,error))return false;
  if(!r->actor->source_target_list304_v111()->destroy(error))return false;
  check(!draws.at(object.identity),"native visual destruction retained a draw loan");return r->close_after_unpublication(error);
 };
 lifecycle.retire_after_unpublication=[&](auto& actual,auto& error){
  check(&actual==manager.get()&&actual.characters().empty()&&actual.source_count50()==0,"retirement precedes manager/list unpublication");order.push_back("retire");error.clear();return true;
 };
 check(process->bind_lifecycle(lifecycle,e),e);check(process->flush([](auto&,auto& error){error.clear();return true;},e),e);
 check(order==std::vector<std::string>{"target101","delete101","target102","delete102","ai101","ai102","d0101","draw101","fx101","d0102","draw102","fx102","retire"},"source three-pass Flush/Clean/D0 order changed");
 check(!first->timer_util_v107&&first->actor->source_frame_fields_v106()->timer14e0==timer,"Clean must free actual TimerUtil without inventing source NULL store");
 check(!a.source_ai_pointers_v105()->observers5c.at(7)&&a.source_ai_pointers_v105()->observers5c.size()==1,"AI observer keys/nodes were erased instead of their pointer cells");
 check(!a.source_ai_pointers_v105()->master50&&!a.source_ai_pointers_v105()->auxiliary58&&!a.source_ooi14a4&&!a.kill_fields_v42().killer144c,"AI raw pointer cells survived disabled receiver cleanup");
 check(!first->actor->object&&!second->actor->object&&manager->source_flush_complete_v121(),"actual native receiver/storage teardown incomplete");
}
void failure_is_nonretryable(){
 auto domain=std::make_shared<int>(3);auto r=fresh(201,domain,"FailedClean");std::string e;unsigned draws{};
 auto save=std::make_shared<data::PlayerSavegameV1>();check(r->save_fields->source_fresh_save_store_then_character_v60(save,e),e);
 world::CharacterCleanServicesV123 clean;clean.retire_draws=[&](auto&){++draws;return true;};clean.detach_fx_anchors=[](auto&){return true;};
 check(!world::character_clean_v123(*r,clean,e)&&e.find("PlayerSavegame14e8 D0")!=std::string::npos,"positive Save D0 was stubbed");
 check(!world::character_clean_v123(*r,clean,e)&&draws==1&&e.find("cannot replay")!=std::string::npos,"failed Clean prefix retried draw/native leaves");
}
void actual_orphan_flush(){
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();std::string e;
 std::shared_ptr<world::SourceProcessObjectsV121> process;check(world::SourceProcessObjectsV121::acquire(app,process,e),e);
 auto manager=process->manager();auto domain=std::make_shared<int>(4);auto r=fresh(301,domain,"PreviewOrphan");r->services.canonical_objects=manager;
 target_providers::Handle16 h;check(manager->add(r->actor->canonical(r),"PreviewOrphan","Warrior",-1,true,h,e),e);
 world::GameObjectInitializationFieldsV62 fields;check(r->actor->inherited_initialization_fields_v62(r,fields,e),e);*fields.byte(0x2fc)=1;
 std::map<std::uintptr_t,world::CanonicalObjectBorrowV1> orphans;unsigned d0{},retire{};
 world::CanonicalObjectLifecycleV1 s;s.owner=domain;
 s.require_quiescent=[](auto&,auto&){return true;};s.native_storage=[manager](auto& receiver,auto& out,auto& error){return receiver.borrow_native_storage_v108(manager,out,error);};
 s.game_object=[&](const auto& object,bool& game,auto& out,auto&){game=true;out={r,object.identity,fields.pointer(0x2f4),fields.byte(0x2f8),fields.byte(0x2fc)};return true;};
 s.room_remove=[](const auto&,auto,auto& error){error="fixture has no positive Room producer";return false;};
 s.ai_remove_from_group=[&](const auto&,auto& error){return character::source_character_remove_from_group_v87(*r,error);};
 s.online_byte5=[app](bool& out,auto&){out=app->get_online_loading_v55()->byte5()!=0;return true;};
 s.orphan_admit=[&](const auto& object,auto&){return orphans.emplace(object.identity,object).second;};
 s.native_receiver=[&](auto id,auto& object,auto& error){const auto found=orphans.find(id);if(found==orphans.end()){error="fixture orphan is not admitted";return false;}object=found->second;return true;};
 s.class_d0=[&](const auto& object,auto& error){
  check(!manager->object(h.key)&&object.lease==r,"orphan D0 must reload retained native receiver after map erase");
  world::ObjectManagerNativeStorageV1 storage;check(manager->borrow_native_storage_v108(manager,storage,error),error);
  check(std::find(storage.orphan4->begin(),storage.orphan4->end(),object.identity)!=storage.orphan4->end(),"orphan D0 lacks original queue membership");
  ++d0;world::CharacterCleanServicesV123 clean;clean.retire_draws=[](auto&){return true;};clean.detach_fx_anchors=[](auto&){return true;};
  return world::character_clean_v123(*r,clean,error)&&r->actor->source_target_list304_v111()->destroy(error)&&r->close_after_unpublication(error);
 };
 s.retire_after_unpublication=[&](auto&,auto&){++retire;if(d0)orphans.clear();return true;};
 check(process->bind_lifecycle(s,e),e);check(manager->remove_source_v1(h,s,e),e);
 check(!manager->object(h.key)&&r->actor->object&&orphans.size()==1&&d0==0&&retire==1,"Remove discarded queued orphan or delivered D0 early");
 check(process->flush([](auto&,auto&){return true;},e),e);world::ObjectManagerNativeStorageV1 storage;check(manager->borrow_native_storage_v108(manager,storage,e),e);
 check(d0==1&&retire==2&&orphans.empty()&&storage.orphan4->empty()&&!r->actor->object,"Flush did not D0/null/retire actual orphan queue");
}
}
int main(){try{actual_empty_owner();native_flush_order();actual_orphan_flush();failure_is_nonretryable();std::cout<<"PASS actual process owner App isolation, native Flush/Delete/AI/Clean/D0 order, real TimerUtil and pointer storage, native orphan map-erase/reload/D0/null/retirement, strict positive Save/nonretryable prefix; draw/FX transports declared fixtures\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
