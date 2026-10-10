#include "../combat_application.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace {
using namespace dh2::data;
void check(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
struct Observation {
 PropertyView* defender;
 CombatActorState* state;
 const PropertyView* attacker;
 std::int32_t hp=0;
 std::uint32_t dead=0,threat_bits=0,calls=0,after_damage_calls=0;
 std::uint32_t before_dead_calls=0,dead_seen_by_kill=1;
 std::int32_t hp_before_regen=0,attacker_hp_before_regen=0;
};
void before_hit(void* context,float threat){
 auto* observation=static_cast<Observation*>(context);
 if(!observation)return;
 ++observation->calls;
 observation->hp=observation->defender->resolved[36];
 observation->dead=observation->state->dead;
 std::memcpy(&observation->threat_bits,&threat,sizeof threat);
}
int before_dead(void* context,CombatResult& result){
 auto* observation=static_cast<Observation*>(context);if(!observation)return -1;
 ++observation->before_dead_calls;
 observation->hp_before_regen=observation->defender->resolved[36];
 observation->dead_seen_by_kill=observation->state->dead;
 if(observation->hp_before_regen!=0||observation->state->dead!=0||!result.amount)return -1;
 observation->state->dead=1;
 dh2_property_set(observation->defender,36,0);
 return 0;
}
int reject_before_dead(void* context,CombatResult& result){
 auto* observation=static_cast<Observation*>(context);if(!observation)return -1;
 ++observation->before_dead_calls;
 observation->hp_before_regen=observation->defender->resolved[36];
 observation->dead_seen_by_kill=observation->state->dead;
 (void)result;
 return -1;
}
int after_damage(void* context,CombatResult& result){
 auto* observation=static_cast<Observation*>(context);if(!observation)return -1;
 ++observation->after_damage_calls;
 observation->hp_before_regen=observation->defender->resolved[36];
 observation->attacker_hp_before_regen=observation->attacker->resolved[36];
 (void)result;
 return 0;
}
}

int main(){try{
 using namespace dh2::data;
 PropertyRules rules{};rules.types.fill(8);
 PropertyState attacker{},defender{};
 attacker.resolved[204]=256;attacker.resolved[36]=9000;attacker.resolved[38]=10000;
 defender.resolved[36]=10000;defender.resolved[38]=20000;
 auto attacker_view=property_view(rules,attacker),defender_view=property_view(rules,defender);
 CombatActorState attacker_state{},defender_state{};
 CombatResult result{};result.amount=1;result.hp_leech=256;
 MonsterApplication application{};
 Observation observation{&defender_view,&defender_state,&attacker_view};
 MonsterApplicationRequest request{&result,&attacker_view,&defender_view,&attacker_state,&defender_state,
  &before_hit,&observation,&before_dead,&observation,&after_damage,&observation};

 const auto hp_before=defender.resolved[36];
 check(dh2_combat_apply_monster(&application,&request)==0,"Nonlethal application failed");
 std::uint32_t returned_threat=0;std::memcpy(&returned_threat,&application.threat,sizeof application.threat);
 check(observation.calls==1&&observation.hp==hp_before&&observation.dead==0&&
       observation.threat_bits==returned_threat&&application.hit_called==1&&
       application.health.before==hp_before&&application.health.after<hp_before&&
       defender_state.dead==0&&observation.after_damage_calls==1&&
       observation.hp_before_regen==application.health.after&&
       observation.attacker_hp_before_regen==9000&&attacker.resolved[36]==9256,
       "source after-HitFor/pre-regen callback order changed");

 defender.resolved[36]=1;result.amount=1;observation.calls=0;
 check(dh2_combat_apply_monster(&application,&request)==0,"Lethal application failed");
 check(observation.calls==1&&observation.hp==1&&observation.dead==0&&
       application.health.kill_requested==1&&observation.before_dead_calls==1&&
       observation.hp_before_regen==0&&observation.dead_seen_by_kill==0&&defender_state.dead==1&&
       defender.resolved[36]==0&&observation.after_damage_calls==2,
       "source Kill callback did not run between HP=0 and the dead-state write");

 defender.resolved[36]=1;defender_state.dead=0;observation.before_dead_calls=0;
 const auto expected_before_dead=before_dead;
 request.before_dead=nullptr;result.amount=1;
 check(dh2_combat_apply_monster(&application,&request)==0&&observation.before_dead_calls==0&&
       defender_state.dead==1&&defender.resolved[36]==0,
       "unbound source Kill callback changed the standalone lethal fallback");
 request.before_dead=expected_before_dead;observation.after_damage_calls=0;

 defender.resolved[36]=1;defender_state.dead=0;observation.before_dead_calls=0;
 request.before_dead=&reject_before_dead;
 check(dh2_combat_apply_monster(&application,&request)==2&&observation.before_dead_calls==1&&
       observation.hp_before_regen==0&&observation.dead_seen_by_kill==0&&
       defender_state.dead==0&&defender.resolved[36]==0,
       "failed source Kill callback was mistaken for a completed death prefix");
 request.before_dead=expected_before_dead;

 result.amount=0;observation.calls=0;observation.after_damage_calls=0;
 check(dh2_combat_apply_monster(&application,&request)==0&&observation.calls==0&&
       observation.after_damage_calls==0&&application.hit_called==0,
       "No-hit result invoked a positive-damage callback");

 result.amount=1;result.mask=0x400000u;observation.calls=0;
 const auto old_application=application;const auto old_defender=defender;
 const auto old_state=defender_state;const auto old_result=result;
 check(dh2_combat_apply_monster(&application,&request)==1&&observation.calls==0&&
       std::memcmp(&application,&old_application,sizeof application)==0&&
       std::memcmp(&defender,&old_defender,sizeof defender)==0&&
       std::memcmp(&defender_state,&old_state,sizeof defender_state)==0&&
       std::memcmp(&result,&old_result,sizeof result)==0,
       "Rejected application invoked callback or mutated its inputs");
 std::cout<<"combat application order: pre-hit, HP-zero Kill callback, fallback, no-hit and rejection passed\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
