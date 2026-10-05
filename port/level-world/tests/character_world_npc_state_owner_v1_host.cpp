#include "../character_world_npc_state_owner_v1.hpp"
#include <cstdio>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static unsigned checks;
static void check(bool value){++checks;if(!value)throw std::runtime_error("NPC owner composition");}
struct Fixture{std::vector<Request> requests;};
static void body(void* p,State*,const Request* r){static_cast<Fixture*>(p)->requests.push_back(*r);}
static int remaining(void*,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){
 return r->operation==state_owner_character_event||r->operation==state_owner_pin||r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end?0:1;
}
int main(){try{
 Facts facts{};facts.idle=215;facts.attack_moving=248;facts.attack_static=243;facts.stance_mask=210;facts.stance=5;
 Fixture fixture;Services bodies{&fixture,body};StateOwnerBehaviorPredicate8 predicates{};
 WorldNpcStateServicesV1 services{};services.facts=&facts;services.bodies=&bodies;services.predicates=&predicates;services.remaining_methods={&fixture,remaining};
 CharacterWorldNpcStateOwnerV1 npc(0xa123456789abcdefull,services);
 check(npc.native_fsm().state==&npc.state());check(npc.state().flags==0);check(npc.native_fsm().character==0xa123456789abcdefull);
 auto& fields=npc.combat_fields();check(!fields.combo&&!fields.invulnerable&&!fields.push_death&&fields.network_id==-1);
 check(npc.initialize_level("Idle")==1);check(npc.state().current==3);check(npc.state().flags==0x2380);check(!fixture.requests.empty());
 // Borrow identity survives transitions, and constructor flags are not copied.
 State* same=&npc.state();check(npc.transition(5,0xc354,0)==1);check(&npc.state()==same&&npc.state().current==5);check(npc.event(0x22,0)==1);check(npc.state().current==3);
 // Reached unsupported frame services fail; source state and identity remain.
 check(npc.update()<0);check(!npc.error().empty());check(&npc.state()==same&&npc.native_fsm().state==same);
 CharacterWorldNpcStateOwnerV1 missing(0xb123456789abcdefull,{});check(missing.initialize_level("Idle")<0);check(missing.state().flags==0);
 // Complete PreSpawn is required after the genuine constructor/init prefix.
 CharacterWorldNpcStateOwnerV1 spawn(0xc123456789abcdefull,services);check(spawn.initialize_level("PreSpawn")<0);check(spawn.state().current==17);check(!spawn.error().empty());
 check(dh2_character_constructor_combat_fields_v1(nullptr)==-1);
 DotCombatContext32 context{};check(dh2_world_combat_context_v1(&context,npc.native_fsm().character,spawn.native_fsm().character,7,12,2,1,1)==0);check(context.level_delta==-5&&context.reverse_level_delta==5);
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"single_state_authority\":true,\"missing_services_rejected\":true}\n",checks);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s check %u\n",e.what(),checks);return 1;}}
