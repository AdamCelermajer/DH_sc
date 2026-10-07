#include "character_world_self_dot_v7.hpp"
namespace dh2::character::skills {
int character_world_self_dot_v7(WorldSelfDotOutputV7* output,
 CharacterWorldSkillExecutionV6& execution,DotCombatContext32& shared,
 std::uintptr_t identity,std::int32_t amount,std::int32_t element,
 const DotServices16* calculate_services,const DotPlayerServicesV7* player) {
 if(!output||!identity||amount<=0||element<-1||element>4||!calculate_services||!player)return -1;
 *output={};
 auto world=execution.combat().native_world();
 SkillAttackActorV6* attack{};SkillApplyActorV6* application{};
 if(!world.actor||world.actor(world.context,identity,&attack,&application)||
  !attack||!application||attack->identity!=identity||application->identity!=identity||
  attack->properties!=application->properties||!application->network_id||
  !application->combo||!application->push_death||!application->invulnerable)
  return output->status=-2;
 // This calculation-only projection borrows the same native property view.
 // It is never used as application authority or persisted across callbacks.
 DotActor32 calculation_actor{identity,application->properties,
  *application->network_id,*application->combo,*application->push_death,
  *application->invulnerable,0};
 const auto calculated=dh2_character_dot_calculate(&output->calculate,
  &output->attack,&shared,&calculation_actor,amount,element,calculate_services);
 if(calculated!=1)return output->status=calculated;
 // Source application rereads the actor after CalculateResult. Services may
 // have advanced its lifetime/fields; never apply through the prior snapshot.
 attack=nullptr;application=nullptr;
 if(world.actor(world.context,identity,&attack,&application)||!application||
  application->identity!=identity)return output->status=-2;
 const auto applied=character_dot_apply_v7(&output->apply,&output->attack,
  application,&execution.combat().application_services(),player);
 // Preserve genuine source HP/lifecycle/constructor-field prefixes even when
 // a later status/text/audio/Kill provider fails. This only republishes mirrors.
 execution.publish_compatibility_fields();
 return output->status=applied;
}
}
