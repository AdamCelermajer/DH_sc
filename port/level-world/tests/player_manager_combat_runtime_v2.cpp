#include "../player_manager_combat_runtime_v2.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::player;
using namespace dh2::character;
using namespace dh2::character::skills;
static void check(bool b){if(!b)throw std::runtime_error("check failed");}
static bool platform(void*,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& r,std::string& e){
 if(q.operation==PlayerManagerOperationV1::online_enabled){r.value=0;return true;}
 e="Required actual unprovided lifecycle";return false;
}
int main(){try{
 PlayerManagerCombatRuntimeV2 runtime({nullptr,platform});
 std::uintptr_t value=0xa5;
 HitRequest32 main{hit_main_player,0,0,0,nullptr};
 check(runtime.hit(main,&value)<0);check(value==0xa5);
 check(runtime.initialize_development_scalar_projection());
 check(runtime.hit(main,&value)==1&&value==0);
 std::int16_t base=290;const std::uintptr_t character=0x123456789ULL;
 check(runtime.adopt_created_character({0,0,0,true,character,&base}));
 check(runtime.hit(main,&value)==1&&value==character);
 HitRequest32 local{hit_local_player_v6,0,character,0,nullptr};
 check(runtime.hit(local,&value)==1&&value==1);
 std::string error;PlayerInfoFieldsV1* record{};
 check(runtime.manager().get_by_internal(0,false,record,error));
 check(record->character660==character&&record->character_base_id13c8==&base);
 check(record->internal670==0&&record->local_remote67c==0&&record->friendly678==0);
 check(!runtime.adopt_created_character({0,0,0,true,character+1,&base}));
 check(runtime.hit(main,&value)==1&&value==character);
 SkillApplyRequestV6 query{};query.service=skill_apply_player_lookup_v6;query.subject=character;
 SkillApplyResponseV6 response{};
 check(runtime.application(query,&response)==1&&response.identity==reinterpret_cast<std::uintptr_t>(record));
 HitRequest32 other{hit_online,0,0,0,nullptr};check(runtime.hit(other,&value)==0);
 check(!runtime.manager().add_character(0,error));
 check(record->character660==character);
 check(runtime.adopt_created_character({8,1,0,false,character+8,&base}));
 local.subject=character+8;check(runtime.hit(local,&value)==1&&value==0);
 check(runtime.hit(main,&value)==1&&value==character);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":19,\"development_launch\":true,\"whole_add_character\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
