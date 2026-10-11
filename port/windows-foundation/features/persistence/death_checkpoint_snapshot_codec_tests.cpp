#include "death_checkpoint_snapshot_v1.hpp"
#include "../../character_state.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::persistence::death_flow;
namespace dh::foundation {
bool validate_game_save(const GameSave&,std::string& error){error.clear();return true;}
ValidationResult validate_character_state(const CharacterState&){return {};}
const OriginalCombatProperties* PlayableActorWorld::combat_properties(ActorId)const noexcept{return nullptr;}
const PlayableActorTraits* PlayableActorWorld::traits(ActorId)const noexcept{return nullptr;}
ActorState* PlayableActorWorld::find_actor(ActorId){return nullptr;}
const ActorState* PlayableActorWorld::find_actor(ActorId)const{return nullptr;}
WorldObject* PlayableActorWorld::find_object(ObjectId)noexcept{return nullptr;}
const WorldObject* PlayableActorWorld::find_object(ObjectId)const noexcept{return nullptr;}
dh2::data::CombatRandom PlayableActorWorld::random_state()const noexcept{return {};}
bool PlayableActorWorld::replace_state(const std::vector<PersistedPlayableActor>&,
    std::vector<WorldObject>,dh2::data::CombatRandom,std::string& error){error.clear();return true;}
bool PlayableActorWorld::eligible_target(const ActorState&,const ActorState&)const{return false;}
float PlayableActorWorld::target_radius(const ActorState&)const{return 0.0f;}
bool PlayableActorWorld::resolve_damage(const std::string&,const ActorState&,const ActorState&,
    const std::string&,float&,std::string& error)const{error="unused test stub";return false;}
bool PlayableActorWorld::resolve_damage_with_outcomes(const std::string&,const ActorState&,const ActorState&,
    const std::string&,float&,std::optional<std::uint32_t>&,std::optional<std::uint32_t>&,
    std::string& error)const{error="unused test stub";return false;}
}
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
DeathCheckpointSnapshotV1 sample(){
    DeathCheckpointSnapshotV1 s;s.level_uri="data/scene/003_darkwood.mlx";s.checkpoint_name="Checkpoint_A";
    s.controlled_actor_id=1;s.checkpoint_key_position={12,34,56};s.revive_position={7,8,9};
    s.qest_present=true;s.qest={0,1,2,255};
    DeathCheckpointActorV1 a;a.id=1;a.definition_id="Character_Hero";a.player=true;
    a.transform.position={1,2,3};a.source_cs_state=3;a.source_dead=false;s.actors.push_back(a);
    WorldObject o;o.id=42;o.name="Chest_A";o.visual.model="chest_closed";o.visual.material="oak";
    o.visual.animation="idle";o.visual.visible=true;o.state_components["container_v1"]={1,0,0};s.objects.push_back(o);
    return s;
}
void test_snapshot_codec_and_strict_validation(){
    auto original=sample();std::string error;std::vector<std::uint8_t> bytes;
    check(encode_death_checkpoint_snapshot_v1(original,bytes,error),"source checkpoint snapshot encodes");
    DeathCheckpointSnapshotV1 decoded;
    check(decode_death_checkpoint_snapshot_v1(bytes,decoded,error),"source checkpoint snapshot decodes");
    check(decoded.level_uri==original.level_uri&&decoded.checkpoint_name==original.checkpoint_name&&
          decoded.controlled_actor_id==1&&decoded.checkpoint_key_position==original.checkpoint_key_position&&
          decoded.revive_position==original.revive_position&&decoded.qest==original.qest&&
          decoded.actors.size()==1&&decoded.actors[0].source_cs_state==3&&
          decoded.actors[0].source_dead==false&&decoded.objects.size()==1&&
          decoded.objects[0].visual.animation=="idle"&&decoded.objects[0].state_components==original.objects[0].state_components,
          "round trip retains checkpoint key/revive, QEST, CS/dead and object visual/state payloads");
    bytes.back()^=0x80;
    check(!decode_death_checkpoint_snapshot_v1(bytes,decoded,error),"checksum corruption is rejected");
    auto duplicate=original;duplicate.actors.push_back(duplicate.actors.front());
    check(!validate_death_checkpoint_snapshot_v1(duplicate,error),"duplicate source actor identity is rejected");
    auto invalidQest=original;invalidQest.qest_present=false;
    check(!validate_death_checkpoint_snapshot_v1(invalidQest,error),"QEST bytes cannot be smuggled when QEST is absent");
    auto invalidTransform=original;invalidTransform.actors[0].transform.position[0]=std::numeric_limits<float>::infinity();
    check(!validate_death_checkpoint_snapshot_v1(invalidTransform,error),"nonfinite source transform is rejected");
}
}
int main(){try{test_snapshot_codec_and_strict_validation();std::cout<<"PASS: source checkpoint subset round-trip, checksum, duplicate identity, QEST presence and finite-transform validation\n";}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return EXIT_FAILURE;}return EXIT_SUCCESS;}
