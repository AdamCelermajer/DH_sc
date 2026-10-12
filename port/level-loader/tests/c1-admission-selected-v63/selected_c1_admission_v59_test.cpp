#include <canonical_level_context_v1.hpp>
#include <selected_c1_admission_v59.hpp>
#include <stdexcept>
#include <iostream>
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
 std::cout<<"PASS synthetic_optional_admission fixed41_procedural42=1 same_asset_distinct_IDs=1 null_name_identity_seed_before_C1=1 zero_forwarded=1 first_substring=1 completed_C1_out_preserved=1 real_Lua_Save_restore_proof=0\n";
}