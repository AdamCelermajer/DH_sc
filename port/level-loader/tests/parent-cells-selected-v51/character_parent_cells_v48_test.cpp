#include "canonical_character_family_v4.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "canonical_point3d_globals_v1.hpp"
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
struct PlacementV48 {std::map<std::string,std::string> attrs;};
int main(int argc,char** argv){require(argc==3,"V48 parent cells require original cache/design");auto archive=pack(argv[1]);auto tables=std::make_shared<CharacterTables>(archive,argv[2]);auto app=std::make_shared<ApplicationSpawnRandomOwnerV4>();std::string error;CanonicalCharacterFamilyServicesV4 services;services.world=tables;services.design=&tables->design;services.models=&tables->models;services.loot_tables=&tables->loot;services.loot_random=&app->channel(0);CanonicalCharacterFamilyFactoryV4 factory(services);CanonicalPropertyMapV1 property_map({nullptr,&canonical_vec3_origin_v1(),nullptr});
 auto make=[&](const char* name){auto p=std::make_shared<PlacementV48>();p->attrs={{"name",name},{"gametype","Character"},{"activate_cond","Invalid"}};CanonicalSourceObjectRequestV1 source;source.source_lease=p;source.source_context=p.get();source.element=1;source.attribute=[](void* p,std::uint32_t,const char* key)->const char*{auto& attrs=static_cast<PlacementV48*>(p)->attrs;auto i=attrs.find(key);return i==attrs.end()?nullptr:i->second.c_str();};CanonicalClassReceiverV1 out;require(factory.construct({"Character",0x340800},source,out,error),error);return std::pair{factory.find(out.object.identity),source};};
 auto first=make("v48_parent_fresh");auto alias=std::shared_ptr<character::RetainedCharacterActorV1>(first.first,first.first->actor.get());character::CharacterParentInitFieldsV48 fields;fields.identity=48;require(!alias->parent_init_fields_v48(alias,fields,error)&&fields.identity==48,"unproduced properties accepted/mutated output");auto props=alias->properties();require(property_map.init_properties(props,error)&&property_map.load_defaults(props,error)&&property_map.load_overrides(props,first.second,error),error);error="old";require(alias->parent_init_fields_v48(alias,fields,error)&&error.empty(),error);
 require(fields.identity==alias->shared_handle().cached&&fields.receiver_lease.get()==alias.get(),"wrong actual receiver field alias");require(fields.conditions[0].source_offset==0x8c&&fields.conditions[1].source_offset==0xb0&&*fields.conditions[0].compiled==0&&*fields.conditions[0].tested==0&&*fields.conditions[1].compiled==0&&*fields.conditions[1].tested==0,"original ConditionData C1 cells differ");require(*fields.min_difficulty_ec==-1&&*fields.idle_sound370==-1&&*fields.checkpoint28==0&&*fields.optimization10c==0,"original scalar C1 cells differ");require(fields.conditions[0].name==alias->source_string(0x90)&&*fields.conditions[0].name=="Invalid"&&fields.conditions[1].name==alias->source_string(0xb4)&&fields.idle_sound358==alias->source_string(0x358),"condition/sound CString shadow");
 require(fields.position160==alias->object->position.data()&&fields.rotation16c==alias->runtime.rotation.rotation&&fields.heading178==&alias->runtime.rotation.heading_angle&&fields.relative144==alias->runtime.subobjects.local_bounds&&fields.absolute12c==alias->runtime.subobjects.absolute_bounds&&fields.destination1a8==alias->runtime.controller.destination,"transform/runtime storage shadow");require(fields.physical2dc==&alias->position_fields_v7().physical2dc&&fields.attached2e0==&alias->position_fields_v7().attached2e0&&fields.visual2d8==&alias->source_visual()&&fields.skip_checkpoint15c==&alias->source_bounds_flat&&fields.checkpoint28==alias->source_bool_field(0x28)&&fields.optimization10c==alias->source_bool_field(0x10c),"inherited pointer/byte storage shadow");
 character::NpcInitPostBorrowV1 npc;require(alias->init_post_fields(npc,error)&&fields.position160==npc.position160&&fields.rotation16c==npc.rotation16c&&fields.scale120==npc.scale120&&fields.probability274==npc.spawn_probability274&&fields.visual2d8==npc.visual2d8,"parent/NPC field alias differs");
 auto second=make("v48_parent_foreign");auto other=std::shared_ptr<character::RetainedCharacterActorV1>(second.first,second.first->actor.get());auto sentinel=fields;require(!alias->parent_init_fields_v48(other,fields,error)&&fields.conditions[0].compiled==sentinel.conditions[0].compiled,"foreign receiver accepted/output changed");require(!alias->parent_init_fields_v48(first.first,fields,error)&&fields.identity==sentinel.identity,"raw record facade accepted");
 auto adopted=std::make_shared<character::RetainedCharacterActorV1>(42,tables,"Character");require(!adopted->parent_init_fields_v48(adopted,fields,error)&&fields.identity==sentinel.identity,"adopted actor invented fresh authority");
 require(property_map.set_property(props,"activate_cond","RENE_FOLLOW",error)&&*sentinel.conditions[0].name=="RENE_FOLLOW","live actual condition store not observed");require(property_map.set_property(props,"idle_sound","source-idle",error)&&*sentinel.idle_sound358=="source-idle","live actual sound store not observed");*sentinel.min_difficulty_ec=2;*sentinel.idle_sound370=-7;*sentinel.checkpoint28=1;character::CharacterParentInitFieldsV48 again;require(alias->parent_init_fields_v48(alias,again,error)&&again.min_difficulty_ec==sentinel.min_difficulty_ec&&*again.min_difficulty_ec==2&&again.idle_sound370==sentinel.idle_sound370&&*again.idle_sound370==-7&&*again.checkpoint28==1,"second borrow reset/source-shadowed cells");
 character::CharacterLoaderFieldsV38 cache;require(alias->loader_fields_v38(alias,cache,error)&&*cache.properties13c8==-1&&*cache.template13ca==-1&&*cache.master418==0&&first.first->view.resolved[3]==first.first->design.rules()->defaults[3],"parent cell transport selected a model/template");
 auto lifetime=make("v48_parent_lifetime");auto life_alias=std::shared_ptr<character::RetainedCharacterActorV1>(lifetime.first,lifetime.first->actor.get());auto life_props=life_alias->properties();require(property_map.init_properties(life_props,error)&&property_map.load_defaults(life_props,error)&&property_map.load_overrides(life_props,lifetime.second,error),error);character::CharacterParentInitFieldsV48 pin;require(life_alias->parent_init_fields_v48(life_alias,pin,error),error);auto id=life_alias->shared_handle().cached;std::weak_ptr<CanonicalCharacterRecordV4> weak=lifetime.first;factory.erase_after_unpublication(id);life_alias.reset();lifetime.first.reset();require(!weak.expired()&&*pin.min_difficulty_ec==-1,"record alias lifetime not pinned");pin.receiver_lease.reset();require(weak.expired(),"typed view introduced ownership cycle");
 require(app->channel(0).calls==0&&app->channel(1).calls==0,"constructor field transport consumed RNG");std::cout<<"PASS original_parent_cells=8 actual_factory_records=3 parent_NPC_pose_visual_alias=1 constructor_property_guard=1 foreign_record_adoption_guard=1 failed_borrow_preserves_output=1 live_condition_sound_source_stores=1 scalar_second_borrow_not_reset=1 record_alias_lifetime_no_cycle=1 no_model_template_selection=1 rng0_calls=0 whole_GameObject_InitPost_wired=0 compiled_condition_allocated=0\n";
}
