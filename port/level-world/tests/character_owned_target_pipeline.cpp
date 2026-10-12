// Preserve the historical gold-backed pipeline audit and reuse its fixtures.
#define main historical_target_pipeline_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_target_pipeline.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../../game-data/design_settings.hpp"
int main(int argc,char** argv){try{
 check(argc==7);Inputs inputs(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);
 auto records=file(argv[4]),names=file(argv[5]),schema=file(argv[6]);std::string error;
 auto settings=std::make_unique<dh2::data::DesignSettingsOwner>();
 check(settings->load({records.data(),records.size()},{names.data(),names.size()},
  {schema.data(),schema.size()},error));
 auto authored=settings->borrow();auto row=authored.row_index("Default");
 check(row==0&&authored.field_index("EnemySpottedAggro")==11);
 const auto* threat=authored.enemy_spotted_aggro_bits(row);check(threat&&*threat==0x41200000);
 settings.reset();records.clear();records.shrink_to_fit();names.clear();names.shrink_to_fit();schema.clear();schema.shrink_to_fit();
 CharacterGameDesign design;check(design.initialize(inputs.input,error));Fixture f(design);
 // The expected word is a test assertion only. The actual producer now reads
 // the owned, pinned authored setting instead of a word borrowed from gold.
 f.threat=*threat;f.enemy_services.initial_threat=threat;
 const auto& presets=*f.design.characters();auto it=std::find(presets.names.begin(),presets.names.end(),"KnightPlayerBase");
 check(it!=presets.names.end());auto properties=std::make_shared<dh2::data::PropertyState>();
 dh2::data::reset_properties(*f.design.rules(),*properties,&presets.rows[it-presets.names.begin()]);
 check(dh2::data::recalc_properties_with_class(*f.design.classes(),*f.design.rules(),*properties,error));
 f.enemy=f.objects.add(UINT64_C(0x100000001),"PlayerCharacterPrince",properties,
  std::make_shared<dh2::data::CombatActorState>(),{1,2,3});
 f.create(design,common,monster);check(!f.dispatch(9));f.clean_scope();
 check(f.aggro_adds==1&&f.path_queries==1&&f.owner->target.target==f.enemy->identity);
 f.lua("local x,y,z=GetTarget():GetPosition(); assert(x==1 and y==2 and z==3); assert(HasTarget())");
 f.enemy->position={17,19,23};
 f.lua("local x,y,z=GetTarget():GetPosition(); assert(x==17 and y==19 and z==23); saved_player=GetTarget(); proxy=newproxy(true); getmetatable(proxy).__gc=function() local a,b,c=saved_player:GetPosition(); assert(a==17 and b==19 and c==23); ClearTarget() end");
 const auto identity=f.enemy->identity;f.enemy.reset();f.session.reset();
 check(!f.owner->target.target&&f.objects.find(identity)->properties==properties&&*threat==0x41200000);
 Dl_info world{},data{};check(dladdr(reinterpret_cast<void*>(dh2_character_enemy_spotted),&world));
 check(dladdr(reinterpret_cast<void*>(dh2_design_settings_decode_record),&data));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_monster_EnemySpotted\":1,\"actual_KnightPlayerBase\":true,\"owned_authored_threat_bits\":"<<*threat<<",\"native_controller_path_prefix\":true,\"live_target_position_method\":true,\"retained_target_finalizer\":true,\"world_library\":\""<<world.dli_fname<<"\",\"data_library\":\""<<data.dli_fname<<"\",\"full_original_AI\":false}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
