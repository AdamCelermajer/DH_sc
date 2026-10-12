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
struct PlacementV48 {std::map<std::string,std::string> attributes;};
int main(int argc,char** argv){require(argc==3,"V48 needs original cache/design fixture");auto archive=pack(argv[1]);auto tables=std::make_shared<CharacterTables>(archive,argv[2]);auto app=std::make_shared<ApplicationSpawnRandomOwnerV4>();std::string error;
 CanonicalCharacterFamilyServicesV4 services;services.world=tables;services.design=&tables->design;services.models=&tables->models;services.loot_tables=&tables->loot;services.loot_random=&app->channel(0);CanonicalCharacterFamilyFactoryV4 factory(services);
 CanonicalPropertyMapV1 property_map({nullptr,&canonical_vec3_origin_v1(),nullptr});unsigned cases{},live_name_cases{};
 auto make=[&](const std::string& name,const std::string& gametype){auto p=std::make_shared<PlacementV48>();p->attributes={{"name",name},{"gametype","Character"}};CanonicalSourceObjectRequestV1 source;source.source_lease=p;source.source_context=p.get();source.element=1;source.attribute=[](void* v,std::uint32_t,const char* key)->const char*{auto& attrs=static_cast<PlacementV48*>(v)->attributes;auto at=attrs.find(key);return at==attrs.end()?nullptr:at->second.c_str();};CanonicalClassReceiverV1 out;require(factory.construct({"Character",0x340800},source,out,error),error);auto r=factory.find(out.object.identity);require(bool(r),"actual factory record unavailable");auto properties=r->actor->properties();require(property_map.init_properties(properties,error)&&property_map.load_defaults(properties,error)&&property_map.load_overrides(properties,source,error)&&property_map.set_property(properties,"gametype",gametype.c_str(),error),error);require(r->actor->source_name()==name&&r->actor->source_archetype()==gametype,"same source Name30/Gametype48 transport");return r;};
 auto types=tables->design.borrow();const auto* ai=types.ai();require(ai!=nullptr,"actual AI owner unavailable");
 const char* names[]{"PlayerCharacterKnight","NPC","xPlayerCharacter","PlayerCharacter","playercharacter"};const char* gametypes[]{"NPC","PlayerCharacterKnight","PlayerCharacter","","PlayerCharacter"};
 for(int type:{0,1,2,3}){auto row=std::find_if(ai->rows.begin(),ai->rows.end(),[type](const auto& a){return a.type==type;});require(row!=ai->rows.end(),"actual AI row for predicate input unavailable");const auto index=static_cast<std::int32_t>(row-ai->rows.begin());
  for(unsigned i=0;i<5;++i){const std::string name=std::string(names[i])+".v48."+std::to_string(type);auto r=make(name,gametypes[i]);
   // Explicit native predicate input fixture selects a genuine existing AI
   // row in actual resolved-sheet storage. No model/template/Init is selected.
   r->view.resolved[1]=index;
   auto alias=std::shared_ptr<character::RetainedCharacterActorV1>(r,r->actor.get());character::CharacterIsPlayerFieldsV41 fields;require(alias->is_player_fields_v41(alias,fields,error),error);require(fields.live_name==&alias->source_name()&&fields.live_name==alias->source_string(0x30),"actual NAME field pointer differs");require(fields.live_name!=alias->source_string(0x48),"NAME/archetype storage aliased");
   character::NpcInitPostRequestV1 request{0x3a49f0,0,0,alias->shared_handle().cached,0,nullptr};character::NpcInitPostResponseV1 response;require(CanonicalCharacterRecordV4::invoke(r.get(),request,response,error),error);const bool expected=type?type==1:name.rfind("PlayerCharacter",0)==0;require((response.value!=0)==expected,"actual name/type predicate differs from original");++cases;
   auto props=alias->properties();const auto changed=expected&&type==0?std::string("NPC.changed"):std::string("PlayerCharacter.changed");require(property_map.set_property(props,"name",changed.c_str(),error),error);require(*fields.live_name==changed&&alias->source_name()==changed,"existing Name30 borrow didn't observe source store");require(property_map.set_property(props,"gametype",expected?"PlayerCharacter.fakeGametype":"NPC",error),error);
   require(CanonicalCharacterRecordV4::invoke(r.get(),request,response,error),error);const bool changed_expected=type?type==1:changed.rfind("PlayerCharacter",0)==0;require((response.value!=0)==changed_expected,"actual live name/type change differs");++live_name_cases;
   character::CharacterIsPlayerServicesV41 borrowed;borrowed.actual_get_char_type=[type](auto,std::int32_t& t,std::string& e){t=type;e.clear();return true;};auto no_name=fields;no_name.live_name=nullptr;bool output=true;if(type==0)require(!character::character_is_player_v41(no_name,borrowed,output,error)&&output,"missing type0 name did not preserve output");else require(character::character_is_player_v41(no_name,borrowed,output,error)&&output==(type==1),"nonzero type incorrectly requires name");
   character::CharacterLoaderFieldsV38 cache;require(alias->loader_fields_v38(alias,cache,error)&&*cache.properties13c8==-1&&*cache.template13ca==-1&&*cache.master418==0&&r->view.resolved[3]==r->design.rules()->defaults[3],"predicate test changed constructor model/template state");
  }
 }
 require(cases==20&&live_name_cases==20&&app->channel(0).calls==0&&app->channel(1).calls==0,"predicate case count/RNG changed");std::cout<<"PASS actual_factory_name_vs_gametype=20 live_Name30_changes=20 same_NAME30_buffer44_borrow=1 actual_AI_rows_resolved_fixture=1 missing_type0_name_preserves_output=1 nonzero_type_skips_name=1 no_model_template_preselection=1 rng0_calls=0 rng1_calls=0 whole_InitPost_success=0\n";
}
