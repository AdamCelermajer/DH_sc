#include "../character_saved_class_v1.hpp"
#include "../source_character_safeget_v1.hpp"
#include "../../game-data/properties.hpp"
#include "../../game-data/player_profile_filename_v1.hpp"
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
using namespace dh2;using namespace character_saved_class_v1;
using B=std::vector<std::uint8_t>;
void check(bool v,const char* text){if(!v)throw std::runtime_error(text);}
B file(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);check(bool(f),"fixture file required");return B(std::istreambuf_iterator<char>(f),{});}
struct Reader{B b;std::size_t at=0;std::uint32_t word(){check(at+4<=b.size(),"gold word");std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(b[at++])<<(i*8);return v;}B bytes(std::size_t n){check(n<=b.size()-at,"gold bytes");B v(b.begin()+at,b.begin()+at+n);at+=n;return v;}};
void word(B& b,std::uint32_t v){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(v>>(i*8)));}
std::int32_t signed_word(std::uint32_t v){std::int32_t x;std::memcpy(&x,&v,4);return x;}
std::int16_t half(std::uint32_t v){const auto bits=std::uint16_t(v);std::int16_t x;std::memcpy(&x,&bits,2);return x;}
struct Fixture{
 static constexpr std::uintptr_t identity=0x100000001ULL;
 std::int16_t cache=-1,template_cache=-1;
 data::PlayerSavegameV1 save,other;data::PlayerSavegameV1* current=&save;
 std::shared_ptr<int> lifetime=std::make_shared<int>(1);
 data::PlayerSaveProfileV1 profile{1,lifetime,{}};
 data::PlayerSaveLoadOwnerV1 loader;data::PlayerSaveLoadOwnerV1* current_loader=&loader;
 character::template_factory::Source authored;
 character::template_factory::Catalog catalog;
 data::CharacterTable characters;dh2_random_state random{{0,77},{0,19}};
 std::uint32_t player=0,calls=0,sections=0;std::int32_t after=-1;
 int fail_query=0,fail_section=0;bool replace=false,reenter=false;Runtime* runtime=nullptr;
 Fixture():loader(save,profile,{lifetime,[this](const data::PlayerSaveLoadRequestV1& q,data::PlayerSaveLoadResponseV1&,std::string& error){
  check(q.operation==data::PlayerSaveLoadOpV1::load_section&&q.save==&save,"same LoadOwner Save");++sections;
  if(!std::strcmp(q.section,"PCLS")){save.set_class(after);if(replace)current=&other;}
  if(int(sections)==fail_section){error="reached required metadata failure";return false;}return true;
 }}){
  save.set_character(identity);other.set_character(identity);
  catalog.templates={{"Mixed",{35,35,37,32768,65536}},{"Empty",{}},{"Mixed",{99}}};
  characters.names={"Other","KnightPlayerBase","MagePlayerBase","KnightPlayerBase"};
 }
 Bindings binding(){return {identity,&cache,&template_cache,&authored,&catalog,&characters,&random,&current,&current_loader,{this,
  [](void* p,std::uintptr_t character,std::uint32_t* out,std::string& error){auto& f=*static_cast<Fixture*>(p);check(character==identity,"IsPlayer Character");++f.calls;
   if(f.reenter){Result nested;std::string e;check(f.runtime->resolve(&nested,e)==Status::busy,"source reentry guard");}
   if(f.fail_query){error="reached IsPlayer failure";return 1;}*out=f.player;return 0;
  }}};}
};
unsigned original_classes(const std::filesystem::path& p){
 Reader r{file(p)};check(r.bytes(4)==B({'C','S','C','1'}),"class magic");const auto count=r.word();
 for(unsigned k=0;k<count;++k){std::array<std::uint32_t,11> a;for(auto& x:a)x=r.word();std::array<std::uint32_t,8> expected;for(auto& x:expected)x=r.word();
  Fixture f;f.cache=half(a[0]);f.player=a[1];f.current=a[2]?&f.save:nullptr;f.save.set_class(signed_word(a[3]));f.after=signed_word(a[4]);
  f.authored.template_name=std::array<std::string,4>{"","Mixed","Empty","Missing"}.at(a[5]);
  f.authored.explicit_property_name=std::array<std::string,5>{"","MagePlayerBase","Missing","KnightPlayerBase",std::string("MagePlayerBase\0suffix",21)}.at(a[6]);
  f.authored.template_data_class="ignored_source_field";
  if(a[7])f.characters.names={"Other","MagePlayerBase"};
  f.random.seeds[0]=a[8];f.random.counters[0]=a[9];f.template_cache=half(a[10]);
  Runtime runtime(f.binding());Result result;std::string error;check(runtime.resolve(&result,error)==Status::complete,"original class delivery");
  const std::array<std::uint32_t,8> actual={std::uint32_t(result.value),std::uint32_t(f.cache),std::uint32_t(f.template_cache),f.current?std::uint32_t(f.save.class_id()):0xffffffffu,f.random.seeds[0],f.random.counters[0],f.calls,result.load_calls};
  check(actual==expected,"original class/RNG/Save/callback mismatch");
 }
 check(r.at==r.b.size(),"class gold consumed");return count;
}
unsigned original_properties(const std::filesystem::path& p){
 Reader r{file(p)};check(r.bytes(4)==B({'C','S','P','1'}),"PROP magic");const auto count=r.word();
 for(unsigned k=0;k<count;++k){auto payload=r.bytes(r.word());data::PropertyRules rules;data::PropertyState state;
  for(auto& x:rules.types)x=signed_word(r.word());
  for(auto& x:state.saved)x=signed_word(r.word());
  const auto flag=r.bytes(1)[0];
  const auto complete=r.word(),used=r.word();auto expected=r.bytes(896);const auto after_flag=r.bytes(1)[0];
  data::PlayerSavegameV1 save;save.set_character(Fixture::identity);auto view=data::property_view(rules,state);std::size_t consumed;std::string error;
  // Seed the same Save's raw byte through an earlier genuine complete reader.
  auto initial_types=rules.types;rules.types.fill(0);B seed;word(seed,224);seed.resize(900);seed.push_back(flag);
  check(save.load_properties({seed.data(),seed.size()},view,consumed,error),"PROP prior flag producer");rules.types=initial_types;
  const auto before_other=state;check(save.load_properties({payload.data(),payload.size()},view,consumed,error)==bool(complete),"PROP source completion");
  check(consumed==used&&!std::memcmp(state.saved.data(),expected.data(),896)&&save.saved_properties_byte_194()==after_flag,"PROP complete/failure prefix");
  check(state.base==before_other.base&&state.gear==before_other.gear&&state.resolved==before_other.resolved,"PROP does not recalc or heal");
 }
 check(r.at==r.b.size(),"PROP gold consumed");return count;
}
unsigned failure_guards(){unsigned cases=0;std::string error;Result result;
 {Fixture f;f.cache=7;auto b=f.binding();b.services={};b.current_loader=nullptr;b.characters=nullptr;b.authored=nullptr;b.templates=nullptr;b.random=nullptr;Runtime runtime(b);check(runtime.resolve(&result,error)==Status::complete&&result.value==7&&f.calls==0,"cache before missing providers");++cases;}
 {Fixture f;f.player=1;f.fail_query=1;Runtime runtime(f.binding());check(runtime.resolve(&result,error)==Status::failed&&f.cache==-1&&f.sections==0&&error=="reached IsPlayer failure","query failure prefix");++cases;}
 for(int fail=1;fail<=7;++fail){Fixture f;f.player=1;f.after=263;f.fail_section=fail;Runtime runtime(f.binding());check(runtime.resolve(&result,error)==Status::failed&&f.cache==-1&&int(f.sections)==fail,"SG_Load failure prefix");check(f.save.class_id()==(fail>=3?263:-1),"reached Save class retained");++cases;}
 {Fixture f;f.player=1;f.current_loader=nullptr;Runtime runtime(f.binding());check(runtime.resolve(&result,error)==Status::failed&&f.cache==-1&&f.sections==0,"missing same Save loader");++cases;}
 {Fixture f;f.player=1;f.after=263;f.other.set_class(65536);f.replace=true;Runtime runtime(f.binding());check(runtime.resolve(&result,error)==Status::complete&&f.cache==0&&f.other.class_id()==0&&f.save.class_id()==263,"fresh Save after original load");++cases;}
 {Fixture f;f.authored.template_name="Mixed";auto b=f.binding();b.random=nullptr;Runtime runtime(b);check(runtime.resolve(&result,error)==Status::failed&&f.template_cache==0&&f.cache==-1,"template cache before missing RNG");++cases;}
 {Fixture f;Runtime runtime(f.binding());f.runtime=&runtime;f.reenter=true;check(runtime.resolve(&result,error)==Status::complete&&f.calls==1,"reentry outer completes");++cases;}
 {Fixture f;auto b=f.binding();b.property_cache=&f.template_cache;Runtime runtime(b);check(runtime.resolve(&result,error)==Status::invalid_argument,"cache alias guard");++cases;}
 {Fixture f;f.authored.template_name="Mixed";Runtime runtime(f.binding());auto& alias=f.authored.template_name;check(runtime.resolve(&result,alias)==Status::invalid_argument&&alias=="Mixed"&&f.calls==0,"authored error alias guard");++cases;}
 {Fixture f;Runtime runtime(f.binding());auto* alias=reinterpret_cast<Result*>(&f.random);check(runtime.resolve(alias,error)==Status::invalid_argument&&f.random.seeds[1]==77,"RNG output alias guard");++cases;}
 {data::PlayerSavegameV1 save;data::PropertyRules rules;data::PropertyState state;auto v=data::property_view(rules,state);std::size_t used;B payload;word(payload,224);check(!save.load_properties({payload.data(),payload.size()},v,used,error)&&used==0,"PROP null Character boundary");++cases;}
 {data::PlayerSavegameV1 save;save.set_character(Fixture::identity);data::PropertyView missing;std::size_t used;B payload;word(payload,223);check(save.load_properties({payload.data(),payload.size()},missing,used,error)&&used==4,"PROP count decline precedes type provider");++cases;}
 {data::PlayerSavegameV1 save;save.set_character(Fixture::identity);data::PropertyRules rules;data::PropertyState state;auto v=data::property_view(rules,state);v.saved=v.resolved;std::size_t used;B payload;word(payload,224);check(!save.load_properties({payload.data(),payload.size()},v,used,error)&&used==4&&state.resolved[0]==0,"PROP authority alias after count prefix");++cases;}
 return cases;
}
unsigned source_import_resolution(){
 using namespace dh2::source_character_safeget_v1;
 Fixture f;
 f.catalog.templates={{"SwampCommon",{1,1}}};
 f.characters.names={"Other","SwampMonster"};
 f.characters.rows.resize(f.characters.names.size());
 dh2::world::SourceCharacterCensusV1 source{};
 source.module_index=0;source.source_record=23;source.source_handle=41;
 source.name="_prim_swamp_monster";
 source.template_name="MonsterCommonType1";
 source.char_template_pydata="Charater_Templates";
 source.char_template="SwampCommon";
 source.activate_condition="IsBefore_Swamp_Escape";
 std::string error;
 character::template_factory::Source authored{};
 check(make_authored_source(source,&authored,error),"source MGP fields construct canonical template source");
 check(authored.object_name==source.name&&
       authored.editor_template_name=="MonsterCommonType1"&&
       authored.template_data_class=="Charater_Templates"&&
       authored.template_name=="SwampCommon"&&
       authored.explicit_property_name.empty(),
       "source MGP fields remain distinct from editor/table labels");
 auto bindings=f.binding();bindings.authored=nullptr;
 Resolved resolved{};
 check(resolve(source,f.characters,bindings,&resolved,error),
       "source MGP Character SafeGet resolution");
 check(resolved.module_index==0&&resolved.source_record==23&&
       resolved.source_handle==41&&resolved.character_identity==Fixture::identity&&
       resolved.object_name==source.name&&resolved.character_table_name=="SwampMonster"&&
       resolved.property_cache==1&&resolved.template_cache==0&&
       resolved.random_draws==1&&f.cache==1&&f.template_cache==0,
       "source MGP provenance/template/property cache retention");
 source.duplicate_name=true;f.cache=-1;f.template_cache=-1;
 const auto seed=f.random.seeds[0],counter=f.random.counters[0];
 check(!resolve(source,f.characters,bindings,&resolved,error)&&
       f.cache==-1&&f.template_cache==-1&&f.random.seeds[0]==seed&&
       f.random.counters[0]==counter,
       "duplicate ObjectManager row must not create another Character owner");
 return 3;
}void real_mask1(const std::filesystem::path& cache){
 data::CharacterTable characters;auto records=file(cache/"data/pydata/character_properties_pyarray.bin"),names=file(cache/"data/pydata/character_properties_pyarraynames.bin"),schema=file(cache/"data/pydata/character_properties_pystructnames.bin");std::string error;
 check(data::load_characters({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},characters,error),"real CharacterTable");
 auto bytes=file(cache/"dh2_000.savegame");auto index=std::make_shared<data::PlayerProfileIndexV1>();check(index->load({bytes.data(),bytes.size()},error),"private campaign index");
 data::PlayerSavegameV1 save;save.set_character(Fixture::identity);save.set_slot(0);data::PlayerSaveProfileV1 profile;std::int32_t difficulty=17;unsigned sections=0;
 data::PlayerSaveLoadOwnerV1 loader(save,profile,{index,[&](const data::PlayerSaveLoadRequestV1& q,data::PlayerSaveLoadResponseV1& reply,std::string& e){
  if(q.operation==data::PlayerSaveLoadOpV1::filename){reply.text=data::player_profile_filename_v1(q.argument,false,false);return true;}
  if(q.operation==data::PlayerSaveLoadOpV1::create_profile){reply.profile={reinterpret_cast<std::uintptr_t>(index.get()),index,index->borrow()};return true;}
  check(q.operation==data::PlayerSaveLoadOpV1::load_section&&q.save==&save,"real same gameplay Save");++sections;std::size_t consumed;
  return data::load_player_metadata_section_v1(q,{&characters,&difficulty,[](void* p,std::int32_t value,std::string&){*static_cast<std::int32_t*>(p)=value;return true;}},consumed,e);
 }});
 auto* current=&save;auto* current_loader=&loader;std::int16_t property=-1;
 Bindings b; b.character=Fixture::identity;b.property_cache=&property;b.current_savegame=&current;b.current_loader=&current_loader;b.characters=&characters;
 b.services={nullptr,[](void*,std::uintptr_t,std::uint32_t* p,std::string&){*p=1;return 0;}};Runtime runtime(b);Result result;
 check(runtime.resolve(&result,error)==Status::complete&&property==263&&save.class_id()==263&&save.level()==1&&sections==7&&difficulty==0,"actual private gameplay mask1/class bridge");
}
}
int main(int argc,char** argv){try{
 if(argc==2&&std::string(argv[1])=="--source-only"){
  const auto rows=source_import_resolution();
  std::cout<<"{\"validation\":\"PASS\",\"source_import_resolution_cases\":"<<rows<<"}\n";
  return 0;
 }
 check(argc==3,"usage: saved_class <reference> <cache>");const std::filesystem::path ref=argv[1];const auto classes=original_classes(ref/"class-fixtures.bin"),props=original_properties(ref/"prop-fixtures.bin"),guards=failure_guards(),source_rows=source_import_resolution();real_mask1(argv[2]);
 std::cout<<"{\"validation\":\"PASS\",\"class_original_cases\":"<<classes<<",\"prop_original_cases\":"<<props<<",\"failure_guard_cases\":"<<guards<<",\"source_import_resolution_cases\":"<<source_rows<<",\"actual_gameplay_mask1\":true,\"saved_class_id\":263,\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
