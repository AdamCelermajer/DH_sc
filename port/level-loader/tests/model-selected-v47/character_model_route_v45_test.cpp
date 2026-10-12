#include <character_model_route_v45.hpp>
#include <application_spawn_random_owner_v4.hpp>
#include <game_object_spawn_probability_v1.hpp>
#include <canonical_point3d_globals_v1.hpp>
#include <character_scene.hpp>
#include <player_savegame_v1.hpp>
#include <faery_tables.hpp>
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::world;using namespace dh2::loader;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);require(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 z;std::string e;require(z.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return z;
}
// Standalone original-cache GameDesign table setup for this source probe.
// The application must borrow its existing initialized Arrays/GameDesign.
struct CharacterTables {
 std::vector<std::vector<std::uint8_t>> raw;
 character::CharacterGameDesign design;data::Dictionary models;data::LootTablesV2 loot;std::vector<data::Bytes> constant_views;
 explicit CharacterTables(assets::ZipAssetPackV1& archive,const char* design_fixture){
  raw.reserve(32);std::string e;
  auto bytes=[&](const std::string& name){raw.emplace_back();bool found{};require(archive.read("data/pydata/"+name,found,raw.back(),e)&&found,"Required original Character table "+name+": "+e);auto& b=raw.back();return data::Bytes{b.data(),b.size()};};
  character::GameDesignInputs256 input{};character::GameDesignTableInput48* tables[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};const char* prefixes[]{"character_properties","character_classes","ai","ai_factions","levels"};
  for(unsigned i=0;i<5;++i)*tables[i]={bytes(std::string(prefixes[i])+"_pyarray.bin"),bytes(std::string(prefixes[i])+"_pyarraynames.bin"),bytes(std::string(prefixes[i])+"_pystructnames.bin")};
  // Original full GameDesign constant names from the independently captured
  // source fixture. Every stream is reread and compared with the real ZIP.
  std::ifstream fixture(design_fixture,std::ios::binary);require(bool(fixture),"Original design fixture unavailable");
  auto word=[&](){std::uint32_t value{};fixture.read(reinterpret_cast<char*>(&value),4);require(bool(fixture),"Short original design word");return value;};
  auto blob=[&](){std::vector<std::uint8_t> value(word());fixture.read(reinterpret_cast<char*>(value.data()),value.size());require(bool(fixture),"Short original design blob");return value;};
  require(word()==0x314f4447,"Original design fixture magic differs");
  for(unsigned i=0;i<15;++i){auto original=blob();require(original==raw[i],"Original design table differs from source ZIP");}
  const auto count=word();constant_views.reserve(count);
  for(unsigned i=0;i<count;++i){auto name_bytes=blob(),expected=blob();std::string name(name_bytes.begin(),name_bytes.end());auto actual=bytes(name);require(actual.size==expected.size()&&std::equal(expected.begin(),expected.end(),actual.data),"Original constant stream differs: "+name);constant_views.push_back(actual);}
  input.constants=constant_views.data();input.constant_count=constant_views.size();
  require(design.initialize(input,e),e);
  require(data::load_dictionary(bytes("character_models_dictionary_pyarraynames.bin"),bytes("character_models_dictionary_pyarray.bin"),models,e),e);
  require(loot.load(bytes("loot_table_pyarray.bin"),bytes("loot_table_pyarraynames.bin"),bytes("loot_table_pystructnames.bin"),e),e);
 }
};
struct PlacementV45 {std::map<std::string,std::string> attributes;};
struct ModelCaseV45 {std::string mode;unsigned model_calls{},saved_calls{},faery_calls{},parent_calls{};};
int main(int argc,char** argv){require(argc==3,"V45 needs cache/design fixture");auto archive=pack(argv[1]);auto tables=std::make_shared<CharacterTables>(archive,argv[2]);auto app=std::make_shared<ApplicationSpawnRandomOwnerV4>();auto route=std::make_shared<CharacterModelRouteV45>();route->actual_models={std::shared_ptr<const void>(tables,&tables->models),&tables->models.values};auto cases=std::make_shared<std::map<std::string,ModelCaseV45>>();auto save=std::make_shared<data::PlayerSavegameV1>();save->initialize_faeries();std::string error;assert(save->set_current_faery(2,1,error));
 data::FaeryTables faery_owner;auto read=[&](const char* file){bool found{};std::vector<std::uint8_t> raw;require(archive.read(std::string("data/pydata/")+file,found,raw,error)&&found,error);return raw;};auto faery_rows=read("faeries_pyarray.bin"),faery_names=read("faeries_pyarraynames.bin"),faery_schema=read("faeries_pystructnames.bin");require(faery_owner.load({faery_rows.data(),faery_rows.size()},{faery_names.data(),faery_names.size()},{faery_schema.data(),faery_schema.size()},error),error);auto faeries=faery_owner.borrow();const auto list=faeries.list_index("DEFAULT");assert(list>=0);const auto selected=faeries.lists()[list][save->current_faery(1)];const auto saved_model=static_cast<std::int32_t>(faeries.faeries()[selected].scalar.words[3]);assert(saved_model>=0&&static_cast<std::size_t>(saved_model)<tables->models.values.size());
 CanonicalCharacterFamilyServicesV4 services;services.world=tables;services.design=&tables->design;services.models=&tables->models;services.loot_tables=&tables->loot;services.loot_random=&app->channel(0);
 services.remaining_init=[route,cases,app](auto& record,const auto& q,auto& response,std::string& e){auto& test=(*cases)[record.actor->source_name()];
  if(q.source_entry==0x38bd64){auto same=route->actual_factory.lock()->find(q.subject);auto alias=std::shared_ptr<character::RetainedCharacterActorV1>(same,same->actor.get());character::CharacterSpawnFieldsV42 fields;if(!alias->spawn_fields_v42(alias,fields,e))return false;GameObjectSpawnProbabilityBorrowV1 borrow;borrow.owner=alias;borrow.cached_roll270=fields.cached_roll270;borrow.probability274=fields.probability274;borrow.random0=&app->channel(0);borrow.random1=&app->channel(1);static const std::int32_t offline_network_fixture=-1;borrow.network_id108=&offline_network_fixture;borrow.online_byte5=[](bool& out,std::string&){out=false;return true;};borrow.handle_as_player_character=[&record](bool& out,std::string& error){character::NpcInitPostResponseV1 r;character::NpcInitPostRequestV1 request{0x3a49f0,0,0,record.actor->shared_handle().cached,0,nullptr};if(!CanonicalCharacterRecordV4::invoke(&record,request,r,error))return false;out=r.value!=0;return true;};std::int32_t probability{};return game_object_check_spawn_probability_v1(borrow,response.value,probability,e);}
  if(q.source_entry==0x337888||q.source_entry==0x337a88)return true; // explicit debug provider fixtures, not whole main DebugSwitches
  if(q.source_entry==0x3a54d4){++test.model_calls;if(test.mode=="missing_dictionary"){auto missing=*route;missing.actual_models.dictionary_lease.reset();return missing.respond(record,q,response,e);}if(test.mode=="wrong_view"){auto* original=record.fields.properties;data::PropertyView foreign=record.view;record.fields.properties=&foreign;auto ok=route->respond(record,q,response,e);record.fields.properties=original;return ok;}return route->respond(record,q,response,e);}
  if(q.source_entry==0x38be5c){++test.parent_calls;e="Required whole actual GameObject InitPost after produced model/scale";return false;}
  e="Required whole source service beyond bounded V45 model prefix";return false;
 };
 auto factory=std::make_shared<CanonicalCharacterFamilyFactoryV4>(services);route->actual_factory=factory;std::weak_ptr<CanonicalCharacterFamilyFactoryV4> factory_weak=factory;
 route->main_services=[factory_weak,cases,save,faeries,list](auto& record,character::CharacterModelServicesV38& s,std::string& e){const auto mode=(*cases)[record.actor->source_name()].mode;
  if(mode=="missing_type")return true;
  s.character_type=[factory_weak](auto id,std::int32_t& type,std::string& error){auto factory=factory_weak.lock();auto record=factory?factory->find(id):nullptr;if(!record){error="Required actual AI/type receiver";return false;}character::NpcInitPostResponseV1 response;character::NpcInitPostRequestV1 q{0x3a2fec,0,0,id,0,nullptr};if(!CanonicalCharacterRecordV4::invoke(record.get(),q,response,error)||!response.ai)return false;type=response.ai->type;return true;};
  s.is_player=[factory_weak](auto id,bool& player,std::string& error){auto factory=factory_weak.lock();auto record=factory?factory->find(id):nullptr;if(!record){error="Required actual IsPlayer receiver";return false;}character::NpcInitPostResponseV1 response;character::NpcInitPostRequestV1 q{0x3a49f0,0,0,id,0,nullptr};if(!CanonicalCharacterRecordV4::invoke(record.get(),q,response,error))return false;player=response.value!=0;return true;};
  if(mode=="owned_faery"){
   auto& state=(*cases)[record.actor->source_name()];auto* state_ptr=&state;
   // Real retained Save getter + original Faery table; difficulty1/DEFAULT
   // list and Save/master association are explicit test-domain producers.
   s.saved_current_faery=[save,state_ptr](auto master,std::int32_t difficulty,std::int32_t& out,std::string& error){if(master!=save->character()||difficulty!=-1){error="Required actual saved Faery/master association";return false;}++state_ptr->saved_calls;out=save->current_faery(1);return true;};
   s.faery_model=[save,faeries,list,state_ptr](auto master,std::int32_t id,std::int32_t& out,std::string& error){if(master!=save->character()||id<0||static_cast<std::size_t>(id)>=faeries.lists()[list].size()){error="Required actual Faery list/model provider";return false;}++state_ptr->faery_calls;auto row=faeries.lists()[list][id];out=static_cast<std::int32_t>(faeries.faeries()[row].scalar.words[3]);return true;};
  }
  e.clear();return true;
 };
 unsigned assertions=0;CanonicalPropertySourceServicesV1 property_source{&assertions,&canonical_vec3_origin_v1(),[](void* p,std::string&){++*static_cast<unsigned*>(p);return true;}};CanonicalPropertyMapV1 properties(property_source);
 auto make=[&](const std::string& name,const std::string& description,const std::string& mode){auto placement=std::make_shared<PlacementV45>();placement->attributes={{"name",name},{"gametype","Character"},{"charpropsname",description},{"archetype","NPC"},{"dae","authored.before.init"}};CanonicalSourceObjectRequestV1 source;source.source_lease=placement;source.source_context=placement.get();source.element=1;source.attribute=[](void* p,std::uint32_t,const char* key)->const char*{const auto& attrs=static_cast<PlacementV45*>(p)->attributes;auto at=attrs.find(key);return at==attrs.end()?nullptr:at->second.c_str();};CanonicalClassReceiverV1 out;require(factory->construct({"Character",0x340800},source,out,error),error);auto record=factory->find(out.object.identity);auto p=record->actor->properties();require(properties.init_properties(p,error)&&properties.load_defaults(p,error)&&properties.load_overrides(p,source,error),error);auto* authored=record->actor->source_string(0x290);require(authored&&*authored=="authored.before.init","Required original dae property transport into actual CString290");(*cases)[name].mode=mode;return record;};
 auto raw=make("v45_missing_order","DefaultFairy","normal");character::NpcInitPostResponseV1 preserve;preserve.text="sentinel";character::NpcInitPostRequestV1 model_request{0x3a54d4,0,0,raw->actor->shared_handle().cached,0,nullptr};assert(!CanonicalCharacterRecordV4::invoke(raw.get(),model_request,preserve,error)&&preserve.text==std::string("sentinel")&&error.find("ordered")!=std::string::npos&&raw->properties->resolved[3]==raw->design.rules()->defaults[3]);
 auto npc=make("v45_npc","WanderingPriest","normal");assert(!npc->initialize(error)&&npc->init->last_entry()==0x38be5c&&error.find("whole actual")!=std::string::npos);assert(npc->fields.model290&&*npc->fields.model290==tables->models.values[npc->view.resolved[3]]&&npc->view.resolved[3]>=0);float expected[3];assert(dh2_character_visual_scale(expected,npc->view.base+12)==0);for(unsigned i=0;i<3;++i)assert(npc->fields.scale120[i]==expected[i]);assert((*cases)["v45_npc"].model_calls==1&&(*cases)["v45_npc"].parent_calls==1);
 auto fairy=make("v45_null_faery","DefaultFairy","normal");assert(!fairy->initialize(error)&&fairy->init->last_entry()==0x38be5c&&fairy->view.resolved[3]==34&&*fairy->fields.model290==tables->models.values[34]);
 auto master=make("v45_master","KnightPlayerBase","normal");save->set_character(master->actor->shared_handle().cached);
 auto owned=make("v45_owned_faery","DefaultFairy","owned_faery");auto owned_alias=std::shared_ptr<character::RetainedCharacterActorV1>(owned,owned->actor.get());character::CharacterLoaderFieldsV38 owned_fields;assert(owned_alias->loader_fields_v38(owned_alias,owned_fields,error));*owned_fields.master418=master->actor->shared_handle().cached;assert(!owned->initialize(error)&&owned->init->last_entry()==0x38be5c&&*owned->fields.model290==tables->models.values[saved_model]&&(*cases)["v45_owned_faery"].saved_calls==1&&(*cases)["v45_owned_faery"].faery_calls==1);
 auto missing_owned=make("v45_missing_saved","DefaultFairy","normal");auto missing_alias=std::shared_ptr<character::RetainedCharacterActorV1>(missing_owned,missing_owned->actor.get());assert(missing_alias->loader_fields_v38(missing_alias,owned_fields,error));*owned_fields.master418=master->actor->shared_handle().cached;auto missing_ok=missing_owned->initialize(error);require(!missing_ok&&missing_owned->init->last_entry()==0x3a54d4&&error.find("Save/Faery")!=std::string::npos&&*missing_owned->fields.model290=="authored.before.init",std::string("missing saved diagnostic: success=")+std::to_string(missing_ok)+" entry="+std::to_string(missing_owned->init->last_entry())+" model="+*missing_owned->fields.model290+" error="+error);auto count=(*cases)["v45_missing_saved"].model_calls;assert(!missing_owned->initialize(error)&&(*cases)["v45_missing_saved"].model_calls==count);preserve.text="sentinel";model_request.subject=missing_owned->actor->shared_handle().cached;assert(!CanonicalCharacterRecordV4::invoke(missing_owned.get(),model_request,preserve,error)&&preserve.text==std::string("sentinel"));
 for(const auto& mode:{"missing_dictionary","missing_type","wrong_view"}){auto record=make(std::string("v45_")+mode,"DefaultFairy",mode);assert(!record->initialize(error)&&record->init->last_entry()==0x3a54d4&&*record->fields.model290=="authored.before.init"&&(*cases)[std::string("v45_")+mode].parent_calls==0);}
 auto absent=make("v45_missing_row","not-an-original-row","missing_type");assert(!absent->initialize(error)&&absent->init->last_entry()==0x38be5c&&*absent->fields.properties_id13c8==-1&&*absent->fields.model290=="authored.before.init");
 auto actual_properties=npc->properties;assert(npc->actor->object->properties.get()==actual_properties.get()&&npc->fields.properties==&npc->view&&npc->view.resolved==actual_properties->resolved.data());
 std::cout<<"PASS actual_ordered_LoadBase_Recalc=1 actual_model_property_backing=1 actual_family_model_route=1 NPC_Faery_NULL=1 owned_Faery_real_save_getter_original_table_callbacks=1 missing_order_dictionary_type_backing_saved=5 failed_model_not_replayed=1 NULL_model_preserves_authored_CString=1 source_scale_before_missing_parent=1 whole_InitPost_success=0 app_rng0_calls="<<app->channel(0).calls<<" saved_model="<<saved_model<<'\n';}
