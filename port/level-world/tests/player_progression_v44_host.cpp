#include "../player_progression_v1.hpp"
#include "../player_xp_text_v1.hpp"
#include "../character_skill_combat_v6.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
using namespace dh2::character;using namespace dh2::data;
unsigned checks{};void check(bool b){++checks;if(!b)throw std::runtime_error("XP check "+std::to_string(checks));}
std::uint32_t read(std::istream& f){std::uint32_t u;f.read(reinterpret_cast<char*>(&u),4);check(bool(f));return u;}
float fp(std::uint32_t u){float f;std::memcpy(&f,&u,4);return f;}
std::uint32_t bits(float f){std::uint32_t u;std::memcpy(&u,&f,4);return u;}
std::vector<std::uint8_t> file(std::string p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
int main(){try{
 std::ifstream gold("port/level-world/reference/player-progression-v1/math-fixtures.bin",std::ios::binary);check(bool(gold));auto nm=read(gold),ns=read(gold);
 for(unsigned i=0;i<nm;++i){auto raw=read(gold),bonus=read(gold),expected=read(gold);check(std::uint32_t(progression_modified_xp_v1(std::int32_t(raw),std::int32_t(bonus)))==expected);}
 for(unsigned i=0;i<ns;++i){auto base=read(gold);auto player=std::int32_t(read(gold)),victim=std::int32_t(read(gold));DesignSettingsProjection176 d{};for(unsigned j=0;j<5;++j)d.words[0x8c/4+j]=read(gold);auto expected=read(gold);check(bits(progression_scaled_xp_v1(fp(base),player,victim,d))==expected);}
 const std::string assets="port/android-native/app/src/main/assets/data/";std::string error;
 auto a=file(assets+"character_properties_pyarray.bin"),b=file(assets+"character_properties_pyarraynames.bin"),c=file(assets+"character_properties_pystructnames.bin");CharacterTable actors;check(load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},actors,error));
 PropertyRules rules;check(load_property_rules(actors,rules,error));
 a=file(assets+"character_classes_pyarray.bin");b=file(assets+"character_classes_pyarraynames.bin");c=file(assets+"character_classes_pystructnames.bin");ClassTables classes;check(load_classes({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},classes,error));std::vector<ClassRow> rows;for(auto& r:classes.rows)rows.push_back({r.data(),unsigned(r.size())});
 const auto knight=std::int32_t(std::find(classes.names.begin(),classes.names.end(),"KnightPlayerClass")-classes.names.begin());
 auto actor=std::find_if(actors.rows.begin(),actors.rows.end(),[&](const auto& row){return row[26]==knight;});check(actor!=actors.rows.end());const auto actor_index=std::int32_t(actor-actors.rows.begin());
 PropertyState state;reset_properties(rules,state,&*actor);auto view=property_view(rules,state);check(!dh2_class_recalc_base(rows.data(),unsigned(rows.size()),state.base.data(),&view));
 PlayerSavegameV1 save;save.set_character(0x100000123ULL);
 ProgressionActorV1 player{save.character(),&state,&view,&save,&actors,rows.data(),unsigned(rows.size()),actor_index,0,0,true,false,true,0,0};
 ProgressionServicesV1 service;std::vector<std::string> trace;
 service.constant=[&](const char* name,std::int32_t& value,std::string&){trace.push_back(name);value=100;return true;};
 service.debug=[&](const char* name,bool& value,std::string&){trace.push_back(name);value=false;return true;};
 service.statistics_player_lookup=[&](ProgressionActorV1& p,std::string&){check(&p==&player);trace.push_back("player_lookup");return true;};
 ProgressionResultV1 result;
 // Actual cache/class baseline. Below-threshold mutation uses real saved
 // and resolved rules, not a replacement XP counter.
 check(view.resolved[34]>0);const auto before=view.resolved[33];
 check(progression_give_xp_v1(player,256,true,service,result,error));check(result.accepted&&result.complete&&!result.leveled);check(view.resolved[33]==before+progression_modified_xp_v1(256,view.resolved[201]));
 // Reached missing regen is honest after level/base mutation, before Save.
 const auto level=view.resolved[19];check(!progression_give_xp_v1(player,view.resolved[34],false,service,result,error));check(result.leveled&&view.resolved[19]==level+256);check(error.find("RegenHP/MP")!=std::string::npos);check(save.level()==0);
 // Explicit test services observe the whole ordered native core. They do
 // not claim production disk/menu/FX backends.
 service.regen_full=[&](ProgressionActorV1& p,bool mp,std::string&){trace.push_back(mp?"mp":"hp");const dh2::character::skills::SkillAttackNativeServicesV6 debug{nullptr,[](void*,const dh2::character::skills::SkillAttackNativeRequestV6* q,std::uintptr_t* out){using namespace dh2::character::skills;switch(q->service){case skill_attack_debug_load_v6:case skill_attack_string_destroy_v6:return 0;case skill_attack_string_construct_v6:*out=1;return 0;case skill_attack_debug_get_v6:*out=0;return 0;default:return -1;}}};return dh2::character::skills::dh2_character_skill_regen_v6(p.properties,mp?1u:0u,-1,&debug)==0;};
 service.save=[&](ProgressionActorV1& p,std::string&){check(p.save==&save&&save.level()==(view.resolved[19]>>8));trace.push_back("save");return true;};
 service.level_presentation=[&](ProgressionActorV1&,std::int32_t,std::string&){trace.push_back("presentation");return true;};
 check(!dh2_property_set(&view,36,0)&&!dh2_property_set(&view,41,0));
 const auto previous=view.resolved[19];const auto stat_points=view.resolved[148],skill_points=view.resolved[157];
 check(progression_give_xp_v1(player,view.resolved[34]*10,false,service,result,error));check(view.resolved[19]==previous+256&&result.leveled&&result.complete);check(view.resolved[33]<view.resolved[34]);check(save.level()==view.resolved[19]>>8);check(view.resolved[36]==view.resolved[38]&&view.resolved[41]==view.resolved[43]);
 check(view.resolved[148]>stat_points&&view.resolved[157]>skill_points);
 // Receipt retains an attempted source prefix on failure, so a second
 // kill delivery cannot duplicate the XP already added before missing UI.
 PropertyState victim_state;reset_properties(rules,victim_state,&actors.rows[0]);auto victim_view=property_view(rules,victim_state);check(!dh2_class_recalc_base(rows.data(),unsigned(rows.size()),victim_state.base.data(),&victim_view));ProgressionActorV1 victim{0x100000456ULL,&victim_state,&victim_view};
 DesignSettingsProjection176 settings{};settings.words[0x94/4]=bits(100);settings.words[0x98/4]=bits(100);settings.words[0x9c/4]=bits(10);settings.words[0xa0/4]=bits(100);ProgressionActorV1* party[]={&player};ProgressionDeathReceiptV1 receipt(victim.identity);std::vector<ProgressionResultV1> results;
 check(!receipt.dispatch(&player,victim,true,party,1,settings,service,results,error));check(receipt.attempted());const auto after=state.saved;check(receipt.dispatch(&player,victim,true,party,1,settings,service,results,error)&&state.saved==after);
 struct TextFixture {unsigned queued{};} text;
 dh2::character::skills::PlayerXPTextServicesV1 text_services{};text_services.common.context=&text;
 text_services.common.position=[](void*,std::uintptr_t id,float* out){check(id==0x100000456ULL);out[0]=2;out[1]=3;out[2]=4;return 0;};
 text_services.common.height=[](void*,std::uintptr_t,float* out){*out=5;return 0;};
 text_services.common.constant=[](void*,const char* group,const char* key,std::int32_t* out){check(std::string(group)=="StrID"&&std::string(key)=="GAMEPLAYMENUS_REWARD_XP");*out=777;return 0;};
 text_services.common.localized=[](void*,std::int32_t id,const char** out){check(id==777);*out="actual-localized-format";return 0;};
 text_services.format=[](void*,const char* format,std::int32_t n,std::string* out){check(std::string(format)=="actual-localized-format"&&n==42);*out="formatted-XP";return 0;};
 text_services.common.enqueue=[](void* raw,const dh2::character::skills::CombatTextRequestV1* q){check(std::string(q->style)=="anim_sct_xp"&&!q->numeric&&q->color==123&&q->position[2]==9&&std::string(q->text)=="formatted-XP");++static_cast<TextFixture*>(raw)->queued;return 0;};
 check(dh2::character::skills::player_xp_text_v1(victim.identity,42,123,text_services)==1&&text.queued==1);
 text_services.format=nullptr;check(dh2::character::skills::player_xp_text_v1(victim.identity,42,123,text_services)<0&&text.queued==1);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_math_cases\":"<<nm+ns<<",\"actual_class_stat_points_before\":"<<stat_points<<",\"actual_class_skill_points_before\":"<<skill_points<<",\"actual_class_stat_points_after\":"<<view.resolved[148]<<",\"actual_class_skill_points_after\":"<<view.resolved[157]<<",\"production_save_and_presentation\":false}\n";
 }catch(std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
