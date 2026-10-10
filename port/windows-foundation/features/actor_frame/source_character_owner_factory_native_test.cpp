#include "source_character_owner_factory.hpp"
#include "../../../level-world/canonical_class_receiver_bindings_v1.hpp"
#include "../../../level-world/canonical_object_factory_v1.hpp"
#include "../../../level-world/canonical_point3d_globals_v1.hpp"
#include "../../../level-world/source_process_objects_v121.hpp"
#include "../../../level-world/application_spawn_random_owner_v4.hpp"
#include "../../../level-world/native_conditions_v69.hpp"
#include "../../../level-world/character_design_services.hpp"
#include "../../platform_win32.hpp"
#include "../../renderer.hpp"
#include "source_character_owner_factory_visual_binding.hpp"
#include "../frontend/creation/selected_profile_binding_v1.hpp"
#include "../../../engine-ui/owned_hud_settings_v1.hpp"
#include "../../source_root_scopes.hpp"
#include "../../source_module_floors.hpp"
#include "../../source_profile_reader_tables.hpp"
#include "../inventory/source_character_item_resources.hpp"
#include "../character_menu/menu_text.hpp"
#include "../../../level-loader/native_gslevel_runtime_v27.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"
#include "source_current_level_backend_v1.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <cstdio>
using namespace dh2;
int run_private_save_transport_win32_tests(int,char**,
 const std::function<bool(const std::shared_ptr<application::ApplicationSaveFilesOwnerV61>&,std::string&)>&,
 const std::shared_ptr<application::ApplicationServicesOwnerV5>&);
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Required cache "+p);return {std::istreambuf_iterator<char>(f),{}};}
struct SourceFactoryContext {dh::foundation::features::SourceCharacterOwnerFactory* facade{};const char* catalog{};};
static std::vector<std::string> source_sound_names(const std::vector<std::uint8_t>& raw){
 std::size_t at=0;auto word=[&](){if(at>raw.size()||raw.size()-at<4)throw std::runtime_error("Truncated original Sounds names word");std::uint32_t v=std::uint32_t(raw[at])|std::uint32_t(raw[at+1])<<8|std::uint32_t(raw[at+2])<<16|std::uint32_t(raw[at+3])<<24;at+=4;return v;};
 std::vector<std::string> logical;
 for(unsigned group=0;group<5;++group){auto count=word();if(count>65536)throw std::runtime_error("Original Sounds name group outside source count");
  for(std::uint32_t i=0;i<count;++i){auto length=word();if(length>raw.size()-at)throw std::runtime_error("Truncated original Sounds CString");
   std::string name(reinterpret_cast<const char*>(raw.data()+at),length);at+=length;while(!name.empty()&&name.back()=='\0')name.pop_back();if(group==4)logical.push_back(std::move(name));
  }
 }
 if(at!=raw.size()||logical.empty())throw std::runtime_error("Original Sounds names have an unsupported suffix or empty logical table");return logical;
}
struct SourceDebugFiles {std::string directory;};
static int source_debug_open(void* p,const char* name,std::uintptr_t* out){
 auto& files=*static_cast<SourceDebugFiles*>(p);if(!name||!out)return 1;*out=0;
 auto* file=std::fopen((files.directory+"/"+name).c_str(),"rb");if(!file)return errno==ENOENT?0:1;
 *out=reinterpret_cast<std::uintptr_t>(file);return 0;
}
static int source_debug_close(void*,std::uintptr_t handle){return handle&&std::fclose(reinterpret_cast<std::FILE*>(handle))==0?0:1;}
static bool construct_source_character(void* p,const world::CanonicalSourceObjectRequestV1& source,
 world::CanonicalClassReceiverV1& receiver,std::string& error){
 auto& context=*static_cast<SourceFactoryContext*>(p);
 return context.facade->construct({context.catalog,0x340800},source,receiver,error);
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;std::string path=argv[1],condition_path=argv[2],e;std::vector<std::vector<std::uint8_t>> raw;raw.reserve(32);
 dh::foundation::Window window;
 if(!window.open("Native actor visual-loading policy",320,240))throw std::runtime_error(window.error());
 dh::foundation::Renderer renderer;
 if(!renderer.initialize(window.width(),window.height()))throw std::runtime_error("Actual WGL renderer initialization failed");
 auto bytes=[&](const std::string& n){raw.push_back(read(path+"/"+n));auto& b=raw.back();return data::Bytes{b.data(),b.size()};};
 character::GameDesignInputs256 in{};character::GameDesignTableInput48* tables[]{&in.characters,&in.classes,&in.ai,&in.factions,&in.levels};const char* names[]{"character_properties","character_classes","ai","ai_factions","levels"};
 for(unsigned i=0;i<5;++i)*tables[i]={bytes(std::string(names[i])+"_pyarray.bin"),bytes(std::string(names[i])+"_pyarraynames.bin"),bytes(std::string(names[i])+"_pystructnames.bin")};
 character::CharacterGameDesign design;if(!design.initialize(in,e))throw std::runtime_error(e);
 data::LootTablesV2 loot;if(!loot.load(bytes("loot_table_pyarray.bin"),bytes("loot_table_pyarraynames.bin"),bytes("loot_table_pystructnames.bin"),e))throw std::runtime_error(e);
 auto models=std::make_shared<data::Dictionary>();if(!data::load_dictionary(bytes("character_models_dictionary_pyarraynames.bin"),bytes("character_models_dictionary_pyarray.bin"),*models,e))throw std::runtime_error("actual Character model dictionary: "+e);
 auto source_sound_names_v70=std::make_shared<const std::vector<std::string>>(source_sound_names(read(path+"/sounds_pyarraynames.bin")));
 assert(!source_sound_names_v70->empty());
 auto condition_cache=std::make_shared<std::string>(condition_path);
 world::NativeConditionCacheInputsV69 condition_input;condition_input.actual_cache=condition_cache;
 condition_input.read=[condition_cache](const char* name,bool& found,std::vector<std::uint8_t>& out,std::string& error){
  const std::string prefix="data/pydata/";if(std::string(name).rfind(prefix,0)!=0){error="Unexpected actual v2conditions cache path";return false;}
  std::ifstream file(*condition_cache+"/"+name,std::ios::binary);found=bool(file);if(!found){error=std::string("Required original cache file ")+name;return false;}
  out={std::istreambuf_iterator<char>(file),{}};error.clear();return true;
 };
 std::shared_ptr<const world::NativeConditionTableV69> condition_table;
 if(!world::read_native_condition_table_v69(condition_input,condition_table,e))throw std::runtime_error("actual Arrays::v2Conditions cache: "+e);
 assert(condition_table->ready()&&!condition_table->condition_data_rows().empty());
 std::shared_ptr<world::NativeConditionRuntimeV69> condition_runtime;
 if(!world::NativeConditionRuntimeV69::create(condition_table,{},condition_runtime,e))throw std::runtime_error("actual NativeConditionRuntime: "+e);
 // Constructor-only lifetime pin; no World services are invoked in this
 // deliberately failed/uninitialized prefix. Loaded visual cases below use
 // the typed backend world with real floors/cache/SceneManager/ObjectManager.
 auto world=std::make_shared<std::string>(path);data::LootRandom8V2 rng{};
 world::CanonicalCharacterCandidateServicesV60 services;services.world=world;services.design=&design;services.loot_tables=&loot;services.random=&rng;
 std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> observed;unsigned reached=0;
 services.visual=[&](auto& r,auto&,std::string& error){observed=r.shared_from_this();++reached;error="intentional test fault at actual V60 visual provider";return false;};
 auto native=std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(services);dh::foundation::features::SourceCharacterOwnerFactory facade(native);
 for(const char* catalog:{"Character","Player"}){
  world::CanonicalSourceObjectRequestV1 source;source.source_lease=world;source.native_spawn_name_v68=design.borrow().characters()->names.at(2).c_str();
  world::CanonicalClassReceiverV1 receiver;assert(!facade.construct({catalog,0x340800},source,receiver,e));assert(e=="intentional test fault at actual V60 visual provider");assert(observed);
  auto& r=*observed;const auto id=r.actor->object->identity;assert(native->find(id)==observed);assert(r.failed&&r.error==e&&!r.init_attempted&&!r.init_complete);assert(!receiver.object.identity);
  assert(r.actor->object->properties==r.properties&&r.actor->object->life==r.life);assert(r.view.base==r.properties->base.data()&&r.view.saved==r.properties->saved.data()&&r.view.gear==r.properties->gear.data()&&r.view.resolved==r.properties->resolved.data());
  assert(r.inventory37c==r.constructor_inventory.get());assert(r.inventory37c->character()==id&&r.inventory37c->properties()==r.properties&&r.inventory37c->items().empty());
  assert(r.actor->machine&&r.actor->machine->native_fsm().character==id);assert(r.actor->shared_handle().cached==id);assert(r.save_fields&&*r.save_fields->save_slot14e8()==0&&!r.save);assert(r.faery_association_v68);assert(!r.visual&&!r.position&&!r.player_script_owner_v62&&!r.equipment);assert(r.actor->source_visual()==0);assert(r.actor->source_position160_v7());
  dh::foundation::features::SourceCharacterOwnerAliases loan;assert(!facade.borrow_constructed_prefix(id,loan,e));assert(!facade.borrow_completed_character(id,loan,e));std::cout<<catalog<<" actual V60 native C1 retained, failed visual prefix aliases PASS\n";
 }
 assert(reached==2&&rng.calls==0);
 data::Dictionary dictionary;if(!data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dictionary,e))throw std::runtime_error(e);
 auto scene_manager=std::make_shared<world::GameObjectSceneRootRegistryV1>();auto animation_manager=std::make_shared<character::CharacterAnimationSetCacheV6>();
 services.visual=[&](auto&,auto& visual,std::string& error){visual.world=world;visual.scene_manager=scene_manager;visual.animation_manager=animation_manager;visual.animation_dictionary=&dictionary;error.clear();return true;};
 auto admitted=std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(services);dh::foundation::features::SourceCharacterOwnerFactory admitted_facade(admitted);
 for(const char* catalog:{"Character","Player"}){
  world::CanonicalSourceObjectRequestV1 source;source.source_lease=world;source.native_spawn_name_v68=design.borrow().characters()->names.at(2).c_str();world::CanonicalClassReceiverV1 receiver;
  assert(admitted_facade.construct({catalog,0x340800},source,receiver,e));auto r=admitted->find(receiver.object.identity);assert(r&&!r->failed&&!r->init_attempted&&!r->init_complete&&r->visual&&r->position);assert(r->visual->animator()&&!r->visual->animator()->ready()&&r->visual->animator()->set_id()==-1);assert(!r->visual->visual()&&r->actor->source_visual()==0&&animation_manager->size()==0);
  dh::foundation::features::SourceCharacterOwnerAliases loan;assert(admitted_facade.borrow_constructed_prefix(receiver.object.identity,loan,e)&&loan.validate(e));assert(loan.lifetime==r&&loan.visual_owner==r->visual.get()&&loan.visual2d8==&r->actor->source_visual()&&loan.position160==r->actor->source_position160_v7());assert(!admitted_facade.borrow_completed_character(receiver.object.identity,loan,e));
  auto stale=loan;stale.identity+=1;assert(!stale.validate(e));stale=loan;stale.property_view=nullptr;assert(!stale.validate(e));stale=loan;stale.visual2d8=nullptr;assert(!stale.validate(e));stale=loan;stale.save14e8=reinterpret_cast<data::PlayerSavegameV1*>(1);assert(!stale.validate(e));
  std::cout<<catalog<<" actual V60 whole constructor capsule and uninitialized loans PASS\n";
 }
 assert(rng.calls==0);std::cout<<"No completed InitPost/InitFinal/visual readiness or host publication claimed\n";

 // Run the actual retained class receiver's source Load callbacks against the
 // same admitted native graph, including the real offline process Add owner.
 dh::foundation::AssetCatalog floor_assets(std::filesystem::path(condition_path).parent_path());
 const std::string validation_uri="original-cache/data/scene/001_swamp.mlx";
 std::vector<dh::foundation::ActorDefinition> floor_definitions;
 if(!dh::foundation::load_actor_definitions(floor_assets,validation_uri,floor_definitions,e))throw std::runtime_error(e);
 dh::foundation::SourceRootScopes floor_scopes;
 if(!floor_scopes.load(floor_assets,validation_uri,floor_definitions,e))throw std::runtime_error(e);
 auto pf_floors=std::make_shared<dh2::floors::World>();
 dh::foundation::SourceModuleFloors module_floors(pf_floors);
 for(const auto& trace:floor_scopes.modules().entries()){
  dh::foundation::SourceModuleFloorRequest request;request.trace=trace;
  request.pose_phase=dh::foundation::SourceModulePosePhase::constructor_degrees;
  request.admitted=true; // explicit diagnostic geometry admission, not campaign activation
  for(const auto& module:floor_scopes.module_records())if(module->receiver->base().identity()==trace.receiver.identity)request.record=module;
  std::shared_ptr<dh::foundation::SourceModuleFloorRoom> room;
  if(!module_floors.load(floor_assets,request,room,e))throw std::runtime_error(e);
 }
 if(!module_floors.post_load(e))throw std::runtime_error(e);
 auto navigation=std::make_shared<dh::foundation::SourceNavigationWorldStorage>(pf_floors,2);
 character::ScriptAssetServicesV1 visual_files;
 const auto template_cache=std::filesystem::current_path()/".local-inputs/publication/checkpoint/session-contributions/level-loader/source/port/level-loader/reference/character-templates-v35";
 const auto loot_cache=std::filesystem::current_path()/".local-inputs/loot-world-v8/cache"; // original-audit.json input hashes verified
 visual_files.read=[path,condition_path,template_cache,loot_cache](const std::string& uri,bool& found,std::vector<std::uint8_t>& output,std::string& error){
  std::vector<std::filesystem::path> candidates;
  const std::string tables="data/pydata/";
  if(uri.rfind(tables,0)==0){const auto name=uri.substr(tables.size());candidates.push_back(std::filesystem::path(path)/name);candidates.push_back(std::filesystem::path(condition_path)/uri);candidates.push_back(template_cache/name);candidates.push_back(loot_cache/name);}
  else candidates.push_back(std::filesystem::path(condition_path)/uri);
  for(const auto& file:candidates){
   std::error_code ec;found=std::filesystem::exists(file,ec);
   if(ec){error="Actual cache filesystem query failed: "+ec.message();return false;}
   if(!found)continue;
   std::ifstream input(file,std::ios::binary);if(!input){error="Actual cache file unreadable: "+file.string();return false;}
   output={std::istreambuf_iterator<char>(input),{}};
   if(input.bad()){error="Actual cache read failed: "+file.string();return false;}
   error.clear();return true;
  }
  found=false;output.clear();error.clear();return true;
 };
 auto visual_cache=std::make_shared<character::CharacterCandidateCacheV62>(visual_files);
 if(!visual_cache->load(e))throw std::runtime_error(e);
 std::cout<<"Actual sewn navigation floors="<<pf_floors->records.size()<<" rooms="<<module_floors.rooms().size()<<" original character cache loaded\n";
 // Original menu CreatePlayer profile bootstrap uses the canonical Character
 // Save14e8 and its actual direct Save slot, without inventing PlayerInfo664.
 auto profile_application=std::make_shared<application::ApplicationServicesOwnerV5>();
 auto profile_gs_globals=std::make_shared<loader::NativeGSLevelGlobalsV27>(); // actual source s_level storage starts empty before GSLevel C1
 std::shared_ptr<world::SourceProcessObjectsV121> profile_process;
 if(!world::SourceProcessObjectsV121::acquire(profile_application,profile_process,e))throw std::runtime_error(e);
 auto profile_target_design=std::make_shared<character::CharacterGameDesign::Borrow>(design.borrow());
 auto profile_backend=std::make_shared<dh::foundation::features::SourceCharacterOwnerFactoryVisualWorldV1>(navigation,scene_manager,visual_cache,profile_process->manager(),profile_target_design);
 auto profile_targets=profile_backend->target_directory();
 assert(profile_targets&&!profile_targets.owner_before(profile_backend)&&!profile_backend.owner_before(profile_targets));
 assert(profile_backend->target_directory().get()==profile_targets.get());
 auto profile_gs_runtime=std::make_shared<loader::NativeGSLevelRuntimeV27>(profile_gs_globals);
 dh::foundation::actor_frame::SourceCurrentLevelGraphV1 profile_current_graph;
 profile_current_graph.root_scope=profile_backend;profile_current_graph.application=profile_application;
 profile_current_graph.world=profile_backend->lifetime();profile_current_graph.objects=profile_process->manager();
 profile_current_graph.navigation=navigation;profile_current_graph.floors=pf_floors;
 profile_current_graph.gs_globals=profile_gs_globals;profile_current_graph.gs_runtime=profile_gs_runtime;
 profile_current_graph.actor_world=profile_targets;profile_current_graph.level_tables=profile_target_design->levels();
 auto profile_current_backend=std::make_shared<dh::foundation::actor_frame::SourceCurrentLevelBackendV1>(profile_current_graph);
 assert(!profile_current_backend->construction_attempted()&&!profile_gs_runtime->connection()&&!profile_gs_globals->s_level);
 dh::foundation::actor_frame::SourceCurrentLevelBorrowV1 absent_level;
 if(!profile_current_backend->current(absent_level,e))throw std::runtime_error(e);
 assert(!absent_level&&!profile_gs_globals->s_level&&!profile_gs_runtime->fields().level34);
 e.clear();
 std::cout<<"Actual native World owns one original-AI target directory and same-control-block current-Level graph; pristine menu GS slot remains empty\n";
 auto profile_services=services;profile_services.world=profile_backend->lifetime();
 profile_services.canonical_objects=profile_process->manager();profile_services.models=&visual_cache->models();
 profile_services.animation_tables=&visual_cache->animation_tables();profile_services.loot_tables=&visual_cache->loot();
 profile_services.model_name.high_performance=[&renderer](bool& full,std::string& error){return renderer.game_object_visual_quality(full,error);}; // explicit port Full/Reduced model policy, not Android driver identity
 auto profile_random=profile_application->source_random_v62();profile_services.random=&profile_random->channel(0);
 auto profile_debug=std::shared_ptr<character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
 if(!profile_application->publish_source_debug_services_v55(profile_debug,e))throw std::runtime_error(e);
 profile_services.debug=profile_debug.get();
 auto profile_debug_files=std::make_shared<SourceDebugFiles>(SourceDebugFiles{condition_path});
 const character::DebugFileServices24 profile_debug_io{profile_debug_files.get(),source_debug_open,source_debug_close};
 profile_services.debug_files=&profile_debug_io;
 profile_services.visual=[profile_backend,animation_manager,visual_cache](auto& record,auto& output,std::string& error){
  output.animation_manager=animation_manager;output.animation_dictionary=&visual_cache->animations();return profile_backend->bind(record,output,error);
 };
 auto profile_factory=std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(profile_services);
 dh::foundation::features::SourceCharacterOwnerFactory profile_facade(profile_factory);
 world::CanonicalSourceObjectRequestV1 profile_request;
 profile_request.source_lease=profile_backend;profile_request.native_spawn_name_v68="PlayerCharacter_0";
 world::CanonicalClassReceiverV1 profile_receiver;
 if(!profile_facade.construct({"Character",0x340800},profile_request,profile_receiver,e))throw std::runtime_error(e);
 auto profile_record=profile_factory->find(profile_receiver.object.identity);assert(profile_record);
 // Deliver source Spawn's InitProperties/LoadDefaults leaves before SetName.
 // Their actual fields are required later by the InitPost field borrower.
 world::CanonicalPropertySourceServicesV1 profile_property_services{};
 profile_property_services.position_rotation_default=&world::canonical_vec3_origin_v1();
 world::CanonicalPropertyMapV1 profile_properties(profile_property_services);
 auto profile_property_actor=profile_receiver.properties();
 if(!profile_properties.init_properties(profile_property_actor,e)||!profile_properties.load_defaults(profile_property_actor,e))throw std::runtime_error(e);
 // Spawn's actual SetName leaf precedes CreatePlayer's IsPlayer query. C1's
 // source name is still empty even though its host object has a spawn label.
 if(!profile_receiver.object.set_name||!profile_receiver.object.set_name(profile_receiver.object.context,"PlayerCharacter_0",e))throw std::runtime_error(e);
 bool source_player{};if(!profile_record->is_player(source_player,e))throw std::runtime_error(e);assert(source_player);
 if(!profile_record->initialize_player_save(e))throw std::runtime_error(e);
 auto transport_root=std::filesystem::current_path().string();
 char* transport_args[]{argv[0],transport_root.data()};
 const auto selected_probe=[&](const std::shared_ptr<application::ApplicationSaveFilesOwnerV61>& files,std::string& error){
  auto quest_tables=std::make_shared<data::QuestTablesPersistenceV51>();
  const auto quest_cache=std::filesystem::current_path()/"port/level-world/reference/character-menu-profile-v51/cache";
  const auto array=read((quest_cache/"v2quests_pyarray.bin").string());
  const auto names=read((quest_cache/"v2quests_pyarraynames.bin").string());
  if(!quest_tables->decode({array.data(),array.size()},{names.data(),names.size()},error))return false;
  auto maps=std::make_shared<data::WorldMapProfileTableV59>();
  const auto map_cache=std::filesystem::current_path()/"port/level-world/reference/character-menu-profile-v51/worldmap-v59";
  const auto map_records=read((map_cache/"worldmap_pyarray.bin").string());
  const auto map_names=read((map_cache/"worldmap_pyarraynames.bin").string());
  const auto map_schema=read((map_cache/"worldmap_pystructnames.bin").string());
  if(!maps->decode({map_records.data(),map_records.size()},{map_names.data(),map_names.size()},{map_schema.data(),map_schema.size()},error))return false;
  auto skills=std::make_shared<data::SkillTables>();
  if(!skills->load(bytes("skills_pyarray.bin"),bytes("skills_pyarraynames.bin"),bytes("skills_pystructnames.bin"),error))return false;
  profile_record->services.skills=skills->borrow();
  auto settings=std::make_shared<ui::OwnedHudSettingsV1>(); // original fresh difficulty0c=0
  character::CharacterProfileBootstrapInputsV59 input{};
  input.save=profile_record->save;input.load=profile_record->load;
  input.character=profile_record->actor->object->identity;
  input.actual_source_cells_lease=profile_record->save_fields;
  input.source_save14e8=profile_record->save_fields->save_slot14e8();
  input.source_direct_save_slot_v122=profile_record->save->source_slot_field_v122();
  input.source_set_slot=[save=profile_record->save](std::int32_t slot,std::string& error){save->set_slot(slot);error.clear();return true;};
  if(!dh::foundation::bind_source_profile_reader_tables(profile_record,profile_record->services.skills,maps,quest_tables,input.reads,error))return false;
  input.reads.store_current_difficulty=[settings](std::int32_t value,std::string& error){settings->source_set_current_difficulty_v67(value);error.clear();return true;};
  input.remaining.owner=profile_application;
  input.remaining.invoke=[profile_application](const auto& query,auto& out,std::string& error){
   if(query.operation==data::PlayerSaveLoadOpV1::online){const auto& online=profile_application->get_online_loading_v55();if(!online){error="Required actual Application COnline";return false;}out.flag=online->byte5()!=0;error.clear();return true;}
   error="Required remaining actual player Save operation "+std::to_string(unsigned(query.operation));return false;
  };
  input.defer_mask2_to_initpost_v62=true; // original later LoadBase/recalc precedes mask2
  // Deliver the actual setter once, then retain its same-Save execution receipt.
  if(!input.source_set_slot(0,error))return false;
  const character::CharacterProfileSlotStoreReceiptV59 slot{profile_record->save.get(),profile_record->save->slot()};
  auto selected=std::make_shared<dh::foundation::frontend::creation::SelectedProfileBindingV1>();
  if(!selected->prepare(files,0,slot,std::move(input),error))return false;
  assert(selected->ready()&&selected->file_receipt()->files==files);
  assert(profile_record->load->profile().identity==0); // preparation does not deliver Load1
  profile_record->profile_bootstrap=std::make_shared<character::CharacterProfileBootstrapV59>(selected->bootstrap_inputs());
  if(!profile_record->profile_bootstrap->prepare(error))return false;
  assert(profile_record->save->character()==profile_record->actor->object->identity);
  assert(*profile_record->save_fields->save_slot14e8()==reinterpret_cast<std::uintptr_t>(profile_record->save.get()));
  assert(profile_record->save->slot()==0&&profile_record->save->name()=="Native Save"&&profile_record->save->level()==1);
  const auto& character_names=profile_record->design.characters()->names;
  const auto class_row=std::find(character_names.begin(),character_names.end(),"KnightPlayerBase")-character_names.begin();
  assert(profile_record->save->class_id()==class_row);
  assert(profile_record->load->profile().identity==reinterpret_cast<std::uintptr_t>(selected->profile().get()));
  assert(!profile_record->save->skills_initialized()&&!profile_record->equipment&&!profile_record->init_attempted);
  assert(profile_record->inventory37c==profile_record->constructor_inventory.get());
  assert(settings->current_difficulty_v67()==0&&!profile_record->profile_bootstrap->finished());
  // Stage genuine source leaves without claiming complete InitPost delivery.
  character::NpcInitPostResponseV1 response;
  const auto invoke=[&](std::uint32_t entry,std::uint32_t argument){
   response={};return world::CanonicalCharacterCandidateRecordV60::init_service(profile_record.get(),
    {entry,argument,0,profile_record->actor->object->identity,0,nullptr},response,error);
  };
  if(!invoke(0x3df2a4,static_cast<std::uint32_t>(class_row)))return false;
  assert(profile_record->properties->base==profile_record->design.characters()->rows.at(class_row));
  if(!invoke(0x3e0810,1))return false; // original class formulas modify base before resolving
  if(!invoke(0x3bc4d0,2))return false;
  assert(profile_record->save->skills_initialized());
  assert(profile_record->save->regular_quests_v45().initialized()&&profile_record->save->volatile_quests_v45().initialized());
  for(unsigned difficulty=0;difficulty<3;++difficulty){
   assert(profile_record->save->faeries_initialized()[difficulty]);
   assert(profile_record->save->level_states_v45()[difficulty].size()==profile_record->design.levels()->levels.size());
   assert(profile_record->save->map_states_v45()[difficulty]==maps->defaults8());
  }
  assert(!profile_record->equipment&&!profile_record->init_attempted&&profile_record->inventory37c==profile_record->constructor_inventory.get());
  if(!profile_record->actor->init_post_fields(profile_record->init_fields,error)||!invoke(0x3b3d38,0)||!invoke(0x3a54d4,0))return false;
  if(!response.text){error="Required actual selected player model";return false;}
  *profile_record->init_fields.model290=response.text; // original InitPost model290 store
  if(dh2_character_visual_scale(profile_record->init_fields.scale120,profile_record->view.base+12)){error="Original selected Character scale projection failed";return false;}
  if(!profile_record->visual->load_visual(error))return false;
  const auto actual_visual=profile_record->visual->visual();
  if(!actual_visual){error="Required same loaded player visual for Gear";return false;}
  auto skin_resources=std::make_shared<skinning::VisualSkinResourcesV6>();
  const auto& bres=actual_visual->bres();const std::vector<std::uint8_t> body(bres.bytes,bres.bytes+bres.size);
  if(!skin_resources->load(body,error))return false;
  auto localization=std::make_shared<dh::foundation::character_menu::MenuLocalization>();
  if(!localization->load(floor_assets,"original-cache/data",0,error)||!localization->bind_profile(nullptr,error))return false;
  ui::HudTextV1* text{};ui::HudTextEnvironmentV1 environment;
  if(!localization->borrow_text(text,environment,error))return false;
  character::SourceItemResourceInputsV88 item_inputs;
  item_inputs.owner=visual_cache;item_inputs.localization_owner=localization;item_inputs.loot=visual_cache->loot().borrow();
  item_inputs.design=design.borrow();item_inputs.localization=text;item_inputs.text_environment=environment;
  item_inputs.asset=[visual_cache](const char* uri,bool& found,auto& bytes,std::string& error){return visual_cache->files().read(uri,found,bytes,error);};
  auto items=std::make_shared<character::SourceItemResourcesV88>(std::move(item_inputs));
  if(!items->load(error))return false;
  auto item_registry=std::make_shared<dh::foundation::inventory::SourceCharacterItemResourcesV1>();
  struct GearWorld {std::shared_ptr<application::ApplicationServicesOwnerV5> app;std::shared_ptr<loader::NativeGSLevelGlobalsV27> globals;};
  auto gear_world=std::make_shared<GearWorld>(GearWorld{profile_application,profile_gs_globals});
  profile_record->services.equipment_inputs=[skin_resources,actual_visual,localization,items,item_registry,environment,gear_world,visual_cache,profile_debug,profile_debug_files,profile_debug_io](auto& record,auto& output,std::string& error){
   output.resources=skin_resources->borrow();output.live_scene=&actual_visual->scene();output.language_pack=0;
   output.debug=profile_debug.get();output.debug_files=profile_debug_io;
   output.assets={visual_cache.get(),[](void* raw,const char* uri,auto& bytes,std::string& error){bool found{};auto& cache=*static_cast<character::CharacterCandidateCacheV62*>(raw);if(!cache.files().read(uri,found,bytes,error))return skinning::VisualAssetResultV6::failed;return found?skinning::VisualAssetResultV6::found:skinning::VisualAssetResultV6::missing;}};
   output.world={gear_world.get(),[](void* raw,player::EquipmentWorldQueryV1 query,std::uintptr_t,std::uintptr_t& identity,std::int32_t& value,std::string& error){
    auto& world=*static_cast<GearWorld*>(raw);identity=0;
    if(query==player::EquipmentWorldQueryV1::online){const auto& online=world.app->get_online_loading_v55();if(!online){error="Required actual COnline";return false;}value=online->byte5()!=0;return true;}
    if(query==player::EquipmentWorldQueryV1::current_level){const auto level=world.globals->s_level;identity=level?level->identity():0;value=0;error.clear();return true;}
    if(query==player::EquipmentWorldQueryV1::current_level_difficulty){const auto level=world.globals->s_level;if(!level){error="Required actual current Level before +118 read";return false;}identity=level->identity();value=level->constructor_fields_v3().mode118;error.clear();return true;}
    error="Required actual PlayerManager equipment query "+std::to_string(unsigned(query));return false;
   }};
   return item_registry->configure_gear_inputs(record.shared_from_this(),items,output,environment,{skin_resources,actual_visual,localization,gear_world,visual_cache,profile_debug,profile_debug_files},error);
  };
  const auto original_inventory=profile_record->inventory37c;
  if(!profile_record->prepare_equipment(error))return false;
  assert(profile_record->inventory_transferred&&!profile_record->constructor_inventory);
  assert(profile_record->equipment->inventory()==original_inventory&&profile_record->inventory37c==original_inventory);
  assert(profile_record->equipment->prepared_v60()&&!profile_record->equipment->ready());
  assert(!profile_record->profile_bootstrap->gear_delivered()); // original fresh metadata has no GEAR
  assert(original_inventory->items().empty()&&!profile_record->init_attempted);
  if(!profile_record->equipment->finish_initial_grants_v60(error))return false;
  assert(profile_record->equipment->ready()&&profile_record->equipment->inventory()==original_inventory);
  assert(!original_inventory->items().empty());
  const auto* starting_weapon=original_inventory->equipment()[original_inventory->current_equipment()][1];
  if(!starting_weapon){error="Original starting grants did not equip a primary weapon";return false;}
  std::uint32_t weapon_index{};
  while(weapon_index<original_inventory->items().size()&&original_inventory->items()[weapon_index].get()!=starting_weapon)++weapon_index;
  assert(weapon_index<original_inventory->items().size());
  const auto source_gear=profile_record->properties->gear;
  if(!profile_record->equipment->unequip(1,error))return false;
  assert(!original_inventory->equipment()[original_inventory->current_equipment()][1]);
  assert(profile_record->properties->gear!=source_gear);
  if(!profile_record->equipment->equip(1,weapon_index,error))return false;
  assert(original_inventory->equipment()[original_inventory->current_equipment()][1]==starting_weapon);
  assert(profile_record->properties->gear==source_gear);
  const std::vector<skinning::VisualDrawViewV32>* draw_views{};
  if(!profile_record->equipment->draw_views(draw_views,error))return false;
  assert(draw_views&&!draw_views->empty());
  // Bind the real backend route owner to the already constructed Character.
  // This menu/no-Level prefix does not invoke campaign callbacks or claim a
  // completed native InitPost, registered campaign world, or playable FSM.
  character::WorldNpcStateServicesV1 source_fsm_services;
  auto* same_machine=&profile_record->actor->machine->native_fsm();
  auto* same_path=&profile_record->actor->runtime.path;
  if(!model_renderer::bind_backend_character_fsm_v1(*profile_record,profile_application,
      profile_backend->lifetime(),pf_floors,navigation->registry,source_fsm_services,error))return false;
  assert(profile_record->fsm_context_v101&&source_fsm_services.remaining_methods.context&&source_fsm_services.outer.context);
  model_renderer::SourceCharacterPathBorrowV105 source_path;
  if(!model_renderer::borrow_source_campaign_character_path_v105(profile_record,source_path,error))return false;
  assert(source_path.character==profile_record->actor->object->identity&&source_path.machine==same_machine&&source_path.path==same_path);
  assert(source_path.record_lease.get()==profile_record.get()&&source_path.context_lease.get()==profile_record->fsm_context_v101.get());
  assert(!profile_gs_globals->s_level&&!profile_record->init_complete);
  std::cout<<"Actual same-Character backend FSM bound; retained path/machine identities verified; menu GS Level remains absent\n";
  source_path={};source_fsm_services={};
  profile_record->actor->bodies={};profile_record->actor->diagnostics.reset();
  profile_record->fsm_context_v101.reset(); // retire route before source actor/world teardown
  std::cout<<"Native source starting inventory items="<<original_inventory->items().size()<<"; equip/unequip same pointer + exact Gear sheet restore; draw views="<<draw_views->size()<<'\n';
  for(const auto& slot:original_inventory->items())std::cout<<"Original granted item="<<original_inventory->table().identifiers.at(slot->item->id)<<" quantity="<<slot->item->quantity<<'\n';
  profile_record->equipment->detach_visual(); // close skin borrow before source visual root teardown
  assert(!profile_record->physical_owner_v62&&profile_record->actor->runtime.object.motion.floor==UINT32_MAX);
  if(!profile_record->visual->close(error))return false;
  assert(scene_manager->roots().empty());
  std::cout<<"Actual canonical Character profile/mask2 + original player visual/shared Item resources + same inventory Gear/mask4/initial grants/equip roundtrip PASS; whole InitPost remains pending\n";
  return true;
 };
 if(run_private_save_transport_win32_tests(2,transport_args,selected_probe,profile_application))throw std::runtime_error("Actual native selected Character profile verification failed");

 for(const char* catalog:{"Character","Player"}){
  auto source_values=std::make_shared<std::map<std::string,std::string>>();
  (*source_values)["name"]="WanderingPriest_Loaded";
  (*source_values)["gametype"]=catalog;
  (*source_values)["position"]="10,20,30";
  (*source_values)["charpropsname"]="WanderingPriest";
  (*source_values)["template"]=""; // actual empty-template SetTemplate branch
  world::CanonicalSourceObjectRequestV1 source;source.source_lease=source_values;source.source_context=source_values.get();
  source.native_spawn_name_v68=design.borrow().characters()->names.at(2).c_str();
  source.attribute=[](void* p,std::uint32_t,const char* key)->const char*{
   auto& values=*static_cast<std::map<std::string,std::string>*>(p);auto at=values.find(key);
   return at==values.end()?nullptr:at->second.c_str();
  };
  world::CanonicalObjectBorrowV1 object;
  std::shared_ptr<application::ApplicationServicesOwnerV5> app;
  std::shared_ptr<world::SourceProcessObjectsV121> process_objects;
  std::shared_ptr<world::ApplicationSpawnRandomOwnerV4> application_random;
  std::shared_ptr<character::DebugSwitches> application_debug;
  SourceDebugFiles debug_files_context{condition_path};
  character::DebugFileServices24 debug_files{&debug_files_context,source_debug_open,source_debug_close};
  auto load_services=services;
  {
   app=std::make_shared<application::ApplicationServicesOwnerV5>();
   const auto& online=app->get_online_loading_v55();
   assert(online&&online->byte5()==0); // genuine fresh COnline C1 offline store
   application_random=app->source_random_v62();assert(application_random);
   application_debug=std::shared_ptr<character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);assert(application_debug);
   if(!app->publish_source_debug_services_v55(application_debug,e))throw std::runtime_error("actual Application Debug owner: "+e);
   load_services.random=&application_random->channel(0);load_services.alternate_random=&application_random->channel(1);
   load_services.models=models.get();
   load_services.debug=application_debug.get();load_services.debug_files=&debug_files;
   load_services.online_byte5=[online](bool& value,std::string& error){value=online->byte5()!=0;error.clear();return true;};
   load_services.conditions=condition_runtime->condition_data_services();
   load_services.inherited=[source_sound_names_v70,&renderer](world::CanonicalCharacterCandidateRecordV60& record,world::GameObjectInitializationServicesV1& inherited,std::string& error){
    if(!record.actor||!record.actor->object||record.actor->shared_handle().cached!=record.actor->object->identity){
     error="Required same source Character inherited actor field borrower";return false;
    }
    inherited.sound_names=source_sound_names_v70.get(); // actual source Arrays::Sounds logical-name group
    // Honest port-owned visual asset policy, not an Android driver enum query.
    // All actual LoadVisual/Sync/PF providers remain required and unchanged.
    inherited.device_high_performance=[&renderer](bool& selected,std::string& error){return renderer.game_object_visual_quality(selected,error);};
    error.clear();return true;
   };
   assert(load_services.conditions.owner==condition_runtime&&load_services.conditions.conditions==&condition_table->condition_data_rows());
   if(!world::SourceProcessObjectsV121::acquire(app,process_objects,e))throw std::runtime_error("source process ObjectManager: "+e);
   auto backend=std::make_shared<dh::foundation::features::SourceCharacterOwnerFactoryVisualWorldV1>(navigation,scene_manager,visual_cache,process_objects->manager());
   load_services.world=backend->lifetime();load_services.models=&visual_cache->models();
   load_services.canonical_objects=process_objects->manager();
   load_services.animation_tables=&visual_cache->animation_tables();load_services.loot_tables=&visual_cache->loot();
   load_services.visual=[backend,animation_manager,visual_cache](auto& record,auto& output,std::string& error){
    output.animation_manager=animation_manager;output.animation_dictionary=&visual_cache->animations();
    return backend->bind(record,output,error);
   };
  }
  auto loaded=std::make_shared<world::CanonicalCharacterCandidateFactoryV60>(load_services);
  dh::foundation::features::SourceCharacterOwnerFactory loaded_facade(loaded);
  SourceFactoryContext context{&loaded_facade,catalog};
  world::CanonicalPropertySourceServicesV1 property_services{};
  property_services.position_rotation_default=&world::canonical_vec3_origin_v1();
  world::CanonicalPropertyMapV1 property_map(property_services);
  auto class_services=world::CanonicalClassReceiverBindingsV1(property_map,{&context,construct_source_character,nullptr,nullptr,nullptr}).services();
  {
   world::CanonicalObjectFactoryAttemptV1 attempt(source);
   if(!attempt.execute(*process_objects->manager(),class_services,e))throw std::runtime_error("actual source factory/Add/Load attempt: "+e);
   assert(attempt.stage()==world::CanonicalFactoryStageV1::complete);
   const auto* actual=process_objects->manager()->object(attempt.handle().key);
   assert(actual&&actual->identity&&actual->shared_handle&&actual->shared_handle->cached==actual->identity);
   object=*actual;
   assert(process_objects->manager()->source_count50()==1&&process_objects->manager()->source_next_key4c()==2);
  }
  assert(object.identity&&object.lease&&object.class_name20&&std::string(*object.class_name20)==catalog);
  auto record=loaded->find(object.identity);assert(record&&!record->failed&&!record->init_attempted&&!record->init_complete);
  auto name=record->actor->source_string(0x30),props_name=record->actor->source_string(0x13b0);
  assert(name&&*name=="WanderingPriest_Loaded"&&props_name&&*props_name=="WanderingPriest");
  assert((record->actor->object->position==std::array<float,3>{10,20,30}));
  assert(record->properties==record->actor->object->properties&&record->view.base==record->properties->base.data());
  assert(record->view.saved==record->properties->saved.data()&&record->view.gear==record->properties->gear.data()&&record->view.resolved==record->properties->resolved.data());
  assert(record->inventory37c==record->constructor_inventory.get()&&record->inventory37c->properties()==record->properties);
  assert(record->actor->machine&&record->actor->machine->native_fsm().character==object.identity);
  dh::foundation::features::SourceCharacterOwnerAliases loan;
  assert(loaded_facade.borrow_constructed_prefix(object.identity,loan,e)&&loan.validate(e));
  assert(!loaded_facade.borrow_completed_character(object.identity,loan,e));
  assert(!record->visual->visual()&&record->actor->source_visual()==0&&rng.calls==0);
  {
   assert(!class_services.init_post(class_services.context,object,e));
   if(e!="Required actual NPC script cache/target/controller/timer inputs")throw std::runtime_error("unexpected actual Character InitPost boundary: "+e);
   const auto actual_visual=record->visual->visual();
   assert(actual_visual&&record->actor->source_visual()==reinterpret_cast<std::uintptr_t>(actual_visual.get()));
   assert(!actual_visual->scene().instances.empty()&&!actual_visual->skinned_meshes().empty());
   assert(record->failed&&record->init_attempted&&!record->init_complete&&record->inherited_init);
   assert(record->actor->source_string(0xd4)&&record->actor->source_string(0xd4)->empty()); // no missing difficulty catalog synthesized
   assert(record->actor->source_string(0x90)&&record->actor->source_string(0xb4));
   world::GameObjectInitializationFieldsV62 actor_fields;assert(record->actor->inherited_initialization_fields_v62(record,actor_fields,e));
   auto* condition_name_a=actor_fields.string(0x90);auto* condition_a=actor_fields.pointer(0xa8);auto* tested_a=actor_fields.byte(0xac);
   auto* condition_name_b=actor_fields.string(0xb4);auto* condition_b=actor_fields.pointer(0xcc);auto* tested_b=actor_fields.byte(0xd0);
   assert(condition_name_a&&condition_a&&tested_a&&condition_name_b&&condition_b&&tested_b);
   assert(*tested_a==0&&*tested_b==0);
   assert((*condition_name_a==""||*condition_name_a=="Invalid")&&(*condition_name_b==""||*condition_name_b=="Invalid"));
   assert(!*condition_a&&!*condition_b&&record->actor->source_position160_v7());
   assert(actor_fields.integer(0x270)&&*actor_fields.integer(0x270)==-2); // actual eligible source spawn result cached on same actor
   assert(record->services.random==&application_random->channel(0)&&application_random->channel(0).calls==1);
   assert(record->services.alternate_random==&application_random->channel(1)&&application_random->channel(1).calls==0);
   // This failed prefix never reached the physical owner: release its actual
   // source visual before another actor is admitted to the shared SceneManager.
   // Whole ObjectManager/Character deletion remains a separate source lifecycle.
   assert(!record->physical_owner_v62);
   assert(record->actor->runtime.object.motion.floor==UINT32_MAX);
   assert(scene_manager->roots()==std::vector<std::uintptr_t>{actual_visual->root_identity()});
   if(!record->visual->close(e))throw std::runtime_error("actual failed-prefix visual teardown: "+e);
   assert(record->actor->source_visual()==0&&!record->visual->visual());
   assert(!record->visual->animator()&&!actual_visual->root_identity());
   assert(scene_manager->roots().empty());
   assert(record->visual->close(e)&&scene_manager->roots().empty());
   assert(record->failed&&!record->init_complete);
  }
  std::cout<<catalog<<" actual source PropertyMap registration/empty-template/default/override prefix PASS; "
   <<"actual process Add/offline network; actual WGL quality/native Priest visual and mesh bounds loaded and released; original script cache/target/controller/timer inputs required"
   <<'\n';
 }
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}

