#include "canonical_character_family_v4.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "game_object_spawn_probability_v1.hpp"
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::world;
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
struct PlacementV42 {std::map<std::string,std::string> attributes;};
struct LifecycleV42 {std::vector<std::string> calls;std::uint8_t byte82{};std::string fail;};
int main(int argc,char** argv){require(argc==3,"V42 needs original cache/design fixture");auto archive=pack(argv[1]);CharacterTables tables(archive,argv[2]);auto app=std::make_shared<ApplicationSpawnRandomOwnerV4>();auto world=std::make_shared<int>(1);std::string error;
 CanonicalCharacterFamilyServicesV4 services;services.world=world;services.design=&tables.design;services.models=&tables.models;services.loot_tables=&tables.loot;services.loot_random=&app->channel(0);CanonicalCharacterFamilyFactoryV4 factory(services);
 auto make=[&](const std::string& name){auto placement=std::make_shared<PlacementV42>();placement->attributes={{"name",name},{"gametype","Character"}};CanonicalSourceObjectRequestV1 source;source.source_lease=placement;source.source_context=placement.get();source.element=1;source.attribute=[](void* context,std::uint32_t,const char* key)->const char*{auto& attrs=static_cast<PlacementV42*>(context)->attributes;auto found=attrs.find(key);return found==attrs.end()?nullptr:found->second.c_str();};CanonicalClassReceiverV1 out;require(factory.construct({"Character",0x340800},source,out,error),error);auto record=factory.find(out.object.identity);require(record&&record->actor,"V42 real factory record missing");return record;};
 auto alias=[](const auto& record){return std::shared_ptr<character::RetainedCharacterActorV1>(record,record->actor.get());};
 auto property=[&](const auto& record,std::int32_t value,const std::string& archetype){auto p=record->actor->properties();require(p.fields.write_int(p.fields.context,0x274,value,error),error);require(p.fields.write_string(p.fields.context,0x48,archetype,error),error);};
 auto predicate=[&](const auto& record){character::CharacterIsPlayerFieldsV41 fields;auto a=alias(record);require(a->is_player_fields_v41(a,fields,error),error);character::CharacterIsPlayerServicesV41 s; s.actual_get_char_type=[record](auto id,std::int32_t& type,std::string& e){character::NpcInitPostResponseV1 response;character::NpcInitPostRequestV1 request{0x3a2fec,0,0,id,0,nullptr};if(!CanonicalCharacterRecordV4::invoke(record.get(),request,response,e)||!response.ai)return false;type=response.ai->type;return true;};return std::pair{fields,s};};
 struct Pins {character::CharacterSpawnFieldsV42 fields;std::shared_ptr<ApplicationSpawnRandomOwnerV4> app;};
 unsigned predicates=0;
 auto spawn=[&](const auto& record,const std::shared_ptr<LifecycleV42>& lifecycle){auto a=alias(record);character::CharacterSpawnFieldsV42 fields;require(a->spawn_fields_v42(a,fields,error),error);auto p=predicate(record);GameObjectSpawnProbabilityBorrowV1 b;b.owner=std::make_shared<Pins>(Pins{fields,app});b.cached_roll270=fields.cached_roll270;b.probability274=fields.probability274;b.random0=&app->channel(0);b.random1=&app->channel(1);
  // Explicit offline/network and lifecycle observers: no main deletion/online
  // behavior is claimed. Cached/player source branches do not read network.
  static const std::int32_t source_network_fixture=-1;b.network_id108=&source_network_fixture;b.online_byte5=[](bool& value,std::string&){value=false;return true;};b.handle_as_player_character=[&,p](bool& value,std::string& e){++predicates;return character::character_is_player_v41(p.first,p.second,value,e);};b.byte82=&lifecycle->byte82;
  b.set_visible_false=[lifecycle](std::string& e){lifecycle->calls.push_back("visible_false");if(lifecycle->fail=="visible"){e="fixture visibility failure";return false;}return true;};b.object_base_delete=[lifecycle](std::string& e){lifecycle->calls.push_back("delete");lifecycle->byte82=2;if(lifecycle->fail=="delete"){e="fixture delete failure";return false;}return true;};b.mark_for_deletion=[lifecycle](std::string& e){assert(lifecycle->byte82==0);lifecycle->calls.push_back("mark");if(lifecycle->fail=="mark"){e="fixture mark failure";return false;}return true;};return b;};
 auto first=make("v42_positive");auto first_alias=alias(first);character::CharacterSpawnFieldsV42 fields;fields.identity=0x42;assert(!first_alias->spawn_fields_v42(first_alias,fields,error)&&fields.identity==0x42);property(first,100,"NPC");error="old";assert(first_alias->spawn_fields_v42(first_alias,fields,error)&&error.empty()&&*fields.cached_roll270==-1&&*fields.probability274==100);const auto* probability_address=fields.probability274;
 auto second=make("v42_foreign");property(second,100,"NPC");auto other_alias=alias(second);auto before=fields;assert(!first_alias->spawn_fields_v42(other_alias,fields,error)&&fields.cached_roll270==before.cached_roll270);assert(!first_alias->spawn_fields_v42(first,fields,error)&&fields.identity==before.identity);
 character::CharacterIsPlayerFieldsV41 is_fields;assert(!first_alias->is_player_fields_v41(other_alias,is_fields,error));assert(first_alias->is_player_fields_v41(first_alias,is_fields,error)&&is_fields.live_archetype==&first_alias->source_archetype());
 auto adopted=std::make_shared<character::RetainedCharacterActorV1>(42,world,"Character");auto adopted_property=adopted->properties();assert(adopted_property.fields.write_int(adopted_property.fields.context,0x274,100,error));assert(!adopted->spawn_fields_v42(adopted,fields,error)&&!adopted->is_player_fields_v41(adopted,is_fields,error));
 auto life=std::make_shared<LifecycleV42>();auto b=spawn(first,life);std::int32_t roll{},prob{};const auto calls0=app->channel(0).calls;assert(game_object_check_spawn_probability_v1(b,roll,prob,error)&&roll==-2&&prob==100&&app->channel(0).calls==calls0+1&&app->channel(1).calls==0&&life->calls.empty());auto seed=app->channel(0).seed;auto pred=predicates;assert(game_object_check_spawn_probability_v1(b,roll,prob,error)&&roll==-2&&app->channel(0).seed==seed&&app->channel(0).calls==calls0+1&&predicates==pred+1);
 property(first,0,"NPC");assert(first_alias->spawn_fields_v42(first_alias,fields,error)&&fields.probability274==probability_address&&*fields.probability274==0);assert(game_object_check_spawn_probability_v1(b,roll,prob,error)&&roll==-2&&prob==0&&app->channel(0).calls==calls0+1);
 auto zero=make("v42_zero");property(zero,0,"NPC");auto zero_life=std::make_shared<LifecycleV42>();auto z=spawn(zero,zero_life);const auto before_zero=app->channel(0).calls;assert(game_object_check_spawn_probability_v1(z,roll,prob,error)&&roll>=0&&prob==0&&app->channel(0).calls==before_zero+1&&zero_life->byte82==0&&(zero_life->calls==std::vector<std::string>{"visible_false","delete","mark"}));auto zero_seed=app->channel(0).seed;assert(game_object_check_spawn_probability_v1(z,roll,prob,error)&&app->channel(0).seed==zero_seed&&zero_life->calls.size()==3);
 for(const auto& fail:{"visible","delete","mark"}){auto record=make(std::string("v42_failure_")+fail);property(record,0,"NPC");auto lifecycle=std::make_shared<LifecycleV42>();lifecycle->fail=fail;auto actual=spawn(record,lifecycle);auto n=app->channel(0).calls;assert(!game_object_check_spawn_probability_v1(actual,roll,prob,error)&&app->channel(0).calls==n+1&&*actual.cached_roll270>=0);assert(lifecycle->calls.size()==(std::string(fail)=="visible"?1u:std::string(fail)=="delete"?2u:3u));if(std::string(fail)=="mark")assert(lifecycle->byte82==0);}
 auto player=make("v42_live_player");property(player,0,"PlayerCharacterKnight");auto player_predicate=predicate(player);bool is_player=false;assert(character::character_is_player_v41(player_predicate.first,player_predicate.second,is_player,error)&&is_player);auto player_life=std::make_shared<LifecycleV42>();auto player_spawn=spawn(player,player_life);auto player_calls=app->channel(0).calls;assert(game_object_check_spawn_probability_v1(player_spawn,roll,prob,error)&&roll==-2&&app->channel(0).calls==player_calls&&player_life->calls.empty());property(player,0,"xPlayerCharacter");assert(character::character_is_player_v41(player_predicate.first,player_predicate.second,is_player,error)&&!is_player);
 auto table=player->design.ai();auto type1=std::find_if(table->rows.begin(),table->rows.end(),[](const auto& ai){return ai.type==1;});assert(type1!=table->rows.end());player->view.resolved[1]=static_cast<std::int32_t>(type1-table->rows.begin());assert(character::character_is_player_v41(player_predicate.first,player_predicate.second,is_player,error)&&is_player); // explicit type producer fixture, no model/template preselection
 for(const auto& record:{first,second,zero,player}){character::CharacterLoaderFieldsV38 cache;auto a=alias(record);assert(a->loader_fields_v38(a,cache,error)&&*cache.properties13c8==-1&&*cache.template13ca==-1&&*cache.master418==0);assert(record->view.resolved[3]==record->design.rules()->defaults[3]);}
 // Factory has no published manager entry in this constructor test. Erase its
 // fixture-only map ownership, then prove the actual field alias pins record.
 auto lifetime=make("v42_lifetime");property(lifetime,100,"NPC");auto lifetime_alias=alias(lifetime);character::CharacterSpawnFieldsV42 pin;assert(lifetime_alias->spawn_fields_v42(lifetime_alias,pin,error));auto identity=lifetime_alias->shared_handle().cached;std::weak_ptr<CanonicalCharacterRecordV4> weak=lifetime;factory.erase_after_unpublication(identity);lifetime_alias.reset();lifetime.reset();assert(!weak.expired()&&*pin.cached_roll270==-1);pin.receiver_lease.reset();assert(weak.expired());
 std::cout<<"PASS real_factory_records=8 actual_actor270=1 same_probability_backing=1 property_guard_foreign_alias_adoption=1 shared_application_positive_zero_cached=1 actual_live_type_archetype=1 lifecycle_order_failures=3 record_alias_lifetime=1 no_model_template_selection=1 rng0_calls="<<app->channel(0).calls<<" rng1_calls="<<app->channel(1).calls<<'\n';}
