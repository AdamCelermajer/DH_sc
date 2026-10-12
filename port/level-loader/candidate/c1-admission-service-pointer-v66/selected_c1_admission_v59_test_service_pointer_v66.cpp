#include <canonical_level_context_v1.hpp>
#include <selected_c1_admission_v59.hpp>
#include <stdexcept>
#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <utility>
using namespace dh2::loader;
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
// Explicit fixture callbacks for admission/control checks only. This source
// test does not prove real Lua/Save construction, generator or restoration.
struct Fixture {
 dh2::data::LevelTables levels;std::uint32_t debug{},module_id{99};unsigned callbacks{};
 std::shared_ptr<std::string> shared_swamp_asset=std::make_shared<std::string>("data/3D/Modules/Swamp/swamp.bdae");
 std::vector<LevelSaveConstructionV3> saves;
 Fixture(){levels.level_names.resize(51);levels.levels.resize(51);for(unsigned i=0;i<51;++i){levels.level_names[i]="fixture_"+std::to_string(i);levels.levels[i].file="fixture_"+std::to_string(i)+".mlx";}levels.level_names[41]="SWAMP";levels.levels[41].file="001_swamp.mlx";levels.level_names[42]="SWAMP_02";levels.levels[42].file="022_swamp2.rule.xml";levels.levels[42].scalar.words[5]=1;}
};
static LevelConstructorServicesV3 services(const std::shared_ptr<Fixture>& app){
 LevelConstructorServicesV3 s;s.application=app;s.debug_level_load_count=&app->debug;s.module_id_global=&app->module_id;s.levels=&app->levels;
 s.construct_script=[app](auto,bool,auto& out,auto&){++app->callbacks;out=std::make_shared<unsigned>(1);return true;};
 s.script_assign_path=[app](const auto&,const char*,std::size_t,auto&){++app->callbacks;return true;};
 s.script_load=[app](const auto&,const char*,auto&){++app->callbacks;return true;};
 s.online_byte5=[app](auto& byte,auto&){++app->callbacks;byte=0;return true;};
 s.allocate_save=[app](auto& out,auto&){++app->callbacks;out=std::make_shared<unsigned>(2);return true;};
 s.construct_save=[app](const auto&,const auto& q,auto&){++app->callbacks;app->saves.push_back(q);return true;};return s;
}
static std::shared_ptr<CanonicalLevelContextV1> create(const std::shared_ptr<Fixture>& app,const std::string& identity,std::uint32_t seed,LevelSourceKindV1 kind=LevelSourceKindV1::fixed){
 LevelSourceRequestV1 request;request.identity=identity;request.definition=kind==LevelSourceKindV1::fixed?"data/scene/001_swamp.mlx":"data/scene/022_swamp2.rule.xml";request.kind=kind;request.seed=seed;
 std::shared_ptr<CanonicalLevelContextV1> out;std::string e;check(CanonicalLevelContextV1::create(request,app,out,e),e);return out;
}
static LevelConstructorArgumentsV3 input(const char* name,std::uint32_t seed){LevelConstructorArgumentsV3 a;a.name=name;a.seed=seed;a.requested_difficulty=-1;return a;}
static void rejected(const std::shared_ptr<CanonicalLevelContextV1>& level,const std::shared_ptr<Fixture>& app,LevelConstructorArgumentsV3 a){
 std::string e;auto owner=level->constructor_owner_v3();auto callbacks=app->callbacks;auto identity=level->identity();auto script=level->constructor_fields_v3().script44,save=level->constructor_fields_v3().save_ec;
 level->source_word150()=0xabc;check(!level->construct_source_v3(a,services(app),e),"invalid admission succeeded");
 check(!e.empty()&&level->constructor_owner_v3()==owner&&app->callbacks==callbacks&&level->identity()==identity&&level->source_word150()==0xabc&&level->constructor_fields_v3().script44==script&&level->constructor_fields_v3().save_ec==save,"rejection mutated constructor/fields/output/callbacks");
}
struct C1AdmissionSnapshotV66 {
 const void* events{};std::shared_ptr<void> script,save;const LevelConstructorV3* constructor{};
 std::uintptr_t identity{};std::string name,request_identity,request_definition;
 std::vector<std::uint64_t> words;std::array<float,3> offset{};
};
static C1AdmissionSnapshotV66 snapshot_c1_v66(const std::shared_ptr<CanonicalLevelContextV1>& level){
 const auto b=level->constructor_borrow_v3();const auto& f=*b.fields;const auto& q=level->source_request();
 return {f.events.get(),f.script44,f.save_ec,level->constructor_owner_v3(),level->identity(),f.name_f8,q.identity,q.definition,
  {f.phase30,f.party_dc,f.word_e0,f.phase_e4,std::uint64_t(f.row3c),std::uint64_t(f.difficulty40),
   f.byte_e8,f.byte_f0,f.byte_f1,f.byte_f2,f.byte_f3,f.byte_f4,f.byte_f5,
   std::uint64_t(f.level110),std::uint64_t(f.mode118),f.seed114,f.field128,f.field12c,f.field130,f.field134,
   f.field138,f.field13c,f.field140,f.byte144,f.byte145,std::uint64_t(f.field148),f.field14c,
   f.field154,f.field158,f.field15c,f.field194,f.field19c,f.field1a0,f.field1a4,f.byte16c,f.byte198,f.byte1a8,
   *b.config38,std::uint64_t(*b.music11c),std::uint64_t(*b.safezone120),std::uint64_t(*b.ambient124),
   *b.gate150,std::uint64_t(*b.module_id18c),std::uint64_t(q.kind),q.seed,q.repair_known_references,q.allow_original_backup},
  {b.module_offset160[0],b.module_offset160[1],b.module_offset160[2]}};
}
static bool same_c1_v66(const C1AdmissionSnapshotV66& a,const C1AdmissionSnapshotV66& b){
 return a.events==b.events&&a.script==b.script&&a.save==b.save&&a.constructor==b.constructor&&a.identity==b.identity&&
  a.name==b.name&&a.request_identity==b.request_identity&&a.request_definition==b.request_definition&&a.words==b.words&&a.offset==b.offset;
}
int main(){
 auto app=std::make_shared<Fixture>();std::string e;auto swamp=create(app,"SWAMP",7);auto swamp2=create(app,"SWAMP_02",7,LevelSourceKindV1::procedural);
 check(swamp->identity()!=swamp2->identity()&&swamp->source_request().identity!=swamp2->source_request().identity&&*app->shared_swamp_asset=="data/3D/Modules/Swamp/swamp.bdae","shared assets collapsed selected identities");
 rejected(swamp,app,input(nullptr,7));rejected(swamp2,app,input("001_swamp.mlx",7));rejected(swamp,app,input("001_swamp.mlx",8));auto unknown=create(app,"NOT_SWAMP",7);rejected(unknown,app,input("001_swamp.mlx",7));
 check(swamp->construct_source_v3(input("data/scene/001_swamp.mlx",7),services(app),e),e);check(swamp2->construct_source_v3(input("022_swamp2.rule.xml",7),services(app),e),e);
 check(swamp->constructor_fields_v3().row3c==41&&swamp2->constructor_fields_v3().row3c==42&&app->saves.size()==2&&app->saves[0].row==41&&app->saves[1].row==42,"C1/save row identity changed");
 auto completed=swamp;auto completed_owner=completed->constructor_owner_v3();auto completed_script=completed->constructor_fields_v3().script44,completed_save=completed->constructor_fields_v3().save_ec;auto before=app->callbacks;
 check(!completed->construct_source_v3(input(nullptr,9),services(app),e)&&completed==swamp&&completed->constructor_owner_v3()==completed_owner&&completed->constructor_fields_v3().script44==completed_script&&completed->constructor_fields_v3().save_ec==completed_save&&app->callbacks==before,"completed C1/output replaced");
 auto zero=create(app,"SWAMP_02",0,LevelSourceKindV1::procedural);rejected(zero,app,input("022_swamp2.rule.xml",7));check(zero->construct_source_v3(input("022_swamp2.rule.xml",0),services(app),e)&&zero->source_request().seed==0&&zero->constructor_fields_v3().seed114==0&&app->saves.back().seed==0,"zero source seed rewritten/resolved by guard");
 auto precedence_app=std::make_shared<Fixture>();precedence_app->levels.level_names[0]="FIRST_SWAMP_ALIAS";precedence_app->levels.levels[0].file="swamp";auto later=create(precedence_app,"SWAMP",7);rejected(later,precedence_app,input("001_swamp.mlx",7));
 auto first=create(precedence_app,"FIRST_SWAMP_ALIAS",7);check(first->construct_source_v3(input("001_swamp.mlx",7),services(precedence_app),e)&&first->constructor_fields_v3().row3c==0,"original first-substring precedence changed");
 // Missing actual LevelList pointer must stop before the source C1 owner or helper prefix.
 auto pointer_app=std::make_shared<Fixture>();auto pointer_level=create(pointer_app,"SWAMP",7);pointer_level->source_word150()=0xabc;
 auto pointer_before=snapshot_c1_v66(pointer_level);auto missing_table=services(pointer_app);missing_table.levels=nullptr;auto pointer_output=pointer_level;
 check(!pointer_level->construct_source_v3(input("001_swamp.mlx",7),std::move(missing_table),e)&&
  e=="Required selected Level request and SAME complete Arrays::LevelList"&&same_c1_v66(pointer_before,snapshot_c1_v66(pointer_level))&&
  !pointer_level->constructor_owner_v3()&&pointer_output==pointer_level&&pointer_app->callbacks==0&&pointer_app->debug==0&&pointer_app->module_id==99&&pointer_app->saves.empty(),
  "missing LevelList pointer entered/mutated source C1 or helper prefix");
 check(pointer_level->construct_source_v3(input("001_swamp.mlx",7),services(pointer_app),e)&&
  pointer_level->constructor_owner_v3()&&pointer_level->constructor_owner_v3()->phase()==LevelConstructorPhaseV3::complete&&
  pointer_level->constructor_fields_v3().row3c==41&&pointer_level->constructor_fields_v3().seed114==7&&
  pointer_app->callbacks==7&&pointer_app->debug==1&&pointer_app->module_id==0&&pointer_app->saves.size()==1&&pointer_app->saves[0].row==41&&pointer_app->saves[0].seed==7,
  "missing-table preflight poisoned subsequent valid C1 retry");
 // Existing owner rejection must precede even the missing-table admission guard.
 auto completed_before=snapshot_c1_v66(pointer_level);auto completed_callbacks=pointer_app->callbacks;
 auto completed_missing_table=services(pointer_app);completed_missing_table.levels=nullptr;e.clear();
 check(!pointer_level->construct_source_v3(input("001_swamp.mlx",7),std::move(completed_missing_table),e)&&
  e=="Same Level constructor continuation already attempted"&&same_c1_v66(completed_before,snapshot_c1_v66(pointer_level))&&
  pointer_output==pointer_level&&pointer_app->callbacks==completed_callbacks&&pointer_app->debug==1&&pointer_app->module_id==0&&pointer_app->saves.size()==1,
  "completed owner retry reached missing-table guard or replaced source fields/owners");
 std::cout<<"PASS synthetic_optional_admission fixed41_procedural42=1 same_asset_distinct_IDs=1 null_name_identity_seed_before_C1=1 zero_forwarded=1 first_substring=1 completed_C1_out_preserved=1 missing_table_atomic_retry=1 completed_null_table_owner_first=1 real_Lua_Save_restore_proof=0\n";
}
