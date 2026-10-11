// P16 CONTEXT: interaction-type provider registry. A test double chest/barrel/NPC registers its type; the OOI owner reads it.
#include "interactable_registry_v1.hpp"
#include "character_interest_admission_v1.hpp"

#include <cstdio>

using namespace dh::foundation;

namespace {
int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}
constexpr ActorId player = 1;
constexpr ActorId chest = 100;
constexpr ActorId barrel = 101;
} // namespace

int main() {
    InteractableRegistryV1 registry;
    std::string error;
    InteractableProviderV1 empty;
    check(!registry.upsert({chest, false, {0, 80, 0}, 0.0f, true, empty}, error) && !error.empty(),
          "provider without interaction_type is rejected");
    InteractableProviderV1 chest_provider;
    chest_provider.interaction_type = [](ActorId) { return 0; };   // OpenableContainer
    check(registry.upsert({chest, false, {0, 80, 0}, 0.0f, true, chest_provider}, error), "chest registers");
    InteractableProviderV1 barrel_provider;
    barrel_provider.interaction_type = [](ActorId) { return 8; };  // DestructibleContainer
    check(registry.upsert({barrel, false, {0, 40, 0}, 0.0f, true, barrel_provider}, error), "barrel registers");
    check(registry.size() == 2, "two interactables registered");

    const auto list = registry.candidates(player);
    bool chest_type_ok = false, barrel_type_ok = false;
    for (const auto& c : list) {
        if (c.id == chest) chest_type_ok = c.interaction_type == 0 && !c.is_character;
        if (c.id == barrel) barrel_type_ok = c.interaction_type == 8;
    }
    check(chest_type_ok && barrel_type_ok, "candidates carry each provider's type for the viewer");

    // Viewer-dependent provider (source vt+144(obj, owner)): a chest that only opens for the viewer.
    InteractableProviderV1 locked;
    locked.interaction_type = [](ActorId viewer) { return viewer == player ? 0 : -1; };
    registry.upsert({102, false, {0, 10, 0}, 0.0f, true, locked}, error);
    bool viewer_ok = false, other_ok = false;
    for (const auto& c : registry.candidates(player)) if (c.id == 102) viewer_ok = c.interaction_type == 0;
    for (const auto& c : registry.candidates(7)) if (c.id == 102) other_ok = c.interaction_type == -1;
    check(viewer_ok && other_ok, "provider is evaluated per viewer");

    registry.erase(barrel);
    check(registry.size() == 2, "erase removes one entry");
    registry.clear();
    check(registry.size() == 0 && registry.candidates(player).empty(), "clear empties the registry");

    CharacterInterestAdmissionV1 status;
    status.enabled138=true;status.flags1312=0x2380;status.interactive1045=true;
    status.registered_talk762=true;status.friend_to_viewer=status.viewer_friend_to_candidate=true;
    status.viewer_is_player=true;status.owner_state=status.candidate_state=3;
    ObjectOfInterestCandidateV1 current{200,true,{0,100,0},0,3,false,true};
    InteractableProviderV1 npc;
    npc.interaction_type=[](ActorId){return 3;};
    npc.current_candidate=[&](ActorId,ObjectOfInterestCandidateV1& out){out=current;out.eligible=character_interest_admitted_v1(status);out.interaction_type=character_interest_type_v1(status);return true;};
    registry.upsert({200,true,{0,10,0},0,true,npc},error);
    ObjectOfInterestCandidateV1 enemy{201,true,{100,80,0},0,8,false,true};
    ObjectOfInterestOwnerStateV1 viewer;viewer.owner=player;
    const auto selected=[&]{auto candidates=registry.candidates(player);candidates.push_back(enemy);return select_object_of_interest_v1(player,order_object_of_interest_candidates_v1(viewer,candidates)).object;};
    check(selected()==200,"registered frontal NPC beats nearer side enemy");
    status.enabled138=false;
    check(selected()==201,"disabled/host-hidden registered NPC cannot shadow enemy");
    status.enabled138=true;status.registered_talk762=false;
    check(selected()==201,"unregistered friendly NPC cannot shadow enemy");
    status.registered_talk762=true;current.position={500,500,0};
    check(selected()==201,"live position out of range overrides authored registry position");
    current.position={100,80,0};enemy.position={0,80,0};
    check(selected()==201,"more frontal enemy beats registered side NPC");
    current.position={0,100,0};enemy.position={100,80,0};
    ObjectOfInterestOwnerV1 owner;owner.update(viewer,0,registry.candidates(player));
    current.position={600,600,0};owner.update(viewer,.1,registry.candidates(player));
    check(owner.object()==200,"movement keeps cached OOI before source 500ms refresh");
    owner.update(viewer,.4,registry.candidates(player));
    check(owner.object()==invalid_actor_id,"source refresh uses current position");
    current.position={0,100,0};status.registered_talk762=false;status.merchant=true;
    check(selected()==200,"permanent merchant remains available without talk registration");
    status.merchant=false;status.registered_talk762=true;
    const auto rejected=[&](const char* name){check(!character_interest_admitted_v1(status),name);};
    status.deleted129=true;rejected("deleted living NPC rejected");status.deleted129=false;
    status.interactive1045=false;rejected("live source interactive byte gates registration");status.interactive1045=true;
    status.flags1312=0x241;rejected("source spawn or dead flags lack interactive bit");status.flags1312=0x2380;
    status.faery=true;rejected("faery rejected before registration mask");status.faery=false;
    status.summoned=true;rejected("summoned rejected before registration mask");status.summoned=false;
    status.zonable=status.zoned750=true;rejected("zoned object outside zone rejected");status.in_zone752=true;
    status.owner_word4884=1;status.candidate_word4880=2;rejected("source character word admission gate preserved");
    status.owner_word4884=2;status.candidate_word4880=1;
    status.dead5193=true;status.registered_talk762=false;
    check(character_interest_admitted_v1(status)&&character_interest_type_v1(status)==-1,"dead friendly branch preserved with no talk interaction type");
    status.candidate_state=16;rejected("dead-friendly candidate state16 rejected by mask8");status.candidate_state=3;
    status.owner_state=15;rejected("dead-friendly owner state15 rejected by mask8");status.owner_state=3;
    status.monster=true;rejected("dead monster does not take dead-friendly interactive exception");
    // Source IsSummoned is a type query. A pool membership has no authority
    // over admission; changing actual source type to5 does.
    ActorState live_npc,live_viewer;live_npc.faction_id=1;live_viewer.faction_id=0;
    dh2::data::AiTables factions;factions.factions={{{1,1},{2,-1}},{{0,1}},{{0,-1}}};
    status={};status.enabled138=true;status.interactive1045=true;status.flags1312=0x2380;
    status.registered_talk762=true;status.viewer_is_player=true;
    const bool pool_owned=true; // separate despawn/reward owner; intentionally not an identity input
    project_character_interest_identity_v1(status,2,1,&live_npc,live_viewer,factions);
    check(pool_owned&&selected()==200&&!status.summoned,"pooled source type2 NPC retains registered talk admission");
    project_character_interest_identity_v1(status,5,1,&live_npc,live_viewer,factions);
    check(selected()==201&&status.summoned,"actual source type5 NPC is rejected before registered talk mask");
    status.registered_talk762=false;
    project_character_interest_identity_v1(status,2,1,&live_npc,live_viewer,factions);
    check(selected()==201&&status.friend_to_viewer&&status.viewer_friend_to_candidate,"current friendly faction rejects unregistered NPC");
    live_npc.faction_id=2;
    project_character_interest_identity_v1(status,2,1,&live_npc,live_viewer,factions);
    check(selected()==200&&status.enemy_to_viewer&&status.viewer_enemy_to_candidate&&!status.friend_to_viewer,
          "changed bound faction admits living enemy and refreshes both relationship directions");
    project_character_interest_identity_v1(status,2,1,nullptr,live_viewer,factions);
    check(selected()==201&&status.friend_to_viewer&&status.viewer_friend_to_candidate,"population-only NPC uses resolved initial faction fallback");
    std::printf("%s\n", failures == 0 ? "interactable registry tests passed" : "interactable registry tests FAILED");
    return failures == 0 ? 0 : 1;
}
