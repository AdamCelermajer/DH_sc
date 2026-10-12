#pragma once
#include "game_save.hpp"
#include <algorithm>
#include <set>
namespace dh::foundation {
// Receipt must be copied from the freshly source-initialized world BEFORE
// applying any user snapshot/input. Its ID was formerly omitted solely because
// an ordinary authored monster had no combat policy. Existing records win.
inline bool migrate_missing_authored_actor_records_v1(
    GameSave& saved, const std::vector<PersistedPlayableActor>& sourceInitial,
    const std::vector<ActorId>& eligible, std::string& error) {
    std::set<ActorId> permitted(eligible.begin(),eligible.end()), sourceIds, savedIds;
    if(permitted.size()!=eligible.size()) {error="Duplicate source admission receipt";return false;}
    for(const auto& source:sourceInitial) {
        if(!sourceIds.insert(source.actor.id).second) {error="Duplicate source-initial actor";return false;}
    }
    for(const auto id:permitted) {
        const auto source=std::find_if(sourceInitial.begin(),sourceInitial.end(),[&](const auto& a){return a.actor.id==id;});
        if(source==sourceInitial.end()||source->traits.is_player||id==saved.controlled_actor_id) {
            error="Admission migration lacks genuine non-player source receipt";return false;
        }
    }
    for(const auto& prior:saved.actors) {
        if(!savedIds.insert(prior.actor.id).second) {error="Duplicate saved actor";return false;}
        const auto source=std::find_if(sourceInitial.begin(),sourceInitial.end(),[&](const auto& a){return a.actor.id==prior.actor.id;});
        // Dynamic/script-created rows remain governed by restore_game_save's
        // strict live-world identity checks; compare initial rows when present.
        if(source!=sourceInitial.end()&&source->actor.definition_id!=prior.actor.definition_id) {
            error="Saved actor ID/definition differs from source-initial registry";return false;
        }
    }
    auto merged=saved.actors;
    for(const auto& source:sourceInitial)if(!savedIds.count(source.actor.id)) {
        if(!permitted.count(source.actor.id)) {error="Saved roster omits actor without source admission receipt";return false;}
        merged.push_back(source);
    }
    // Only the actor vector changes. Original saved vitals/dead witnesses,
    // objects, RNG, character and version remain byte/field-equivalent.
    saved.actors=std::move(merged);error.clear();return true;
}
}
#include "features/persistence/death_checkpoint_snapshot_v1.hpp"
namespace dh::foundation {
inline bool migrate_missing_authored_checkpoint_actor_records_v1(
    persistence::death_flow::DeathCheckpointSnapshotV1& saved,
    const std::vector<persistence::death_flow::DeathCheckpointActorV1>& sourceInitial,
    const std::vector<ActorId>& eligible,std::string& error) {
    std::set<ActorId> permitted(eligible.begin(),eligible.end()), sourceIds, savedIds;
    if(permitted.size()!=eligible.size()){error="Duplicate source checkpoint admission receipt";return false;}
    for(const auto& source:sourceInitial)if(!sourceIds.insert(source.id).second){error="Duplicate initial source checkpoint actor";return false;}
    for(const auto& prior:saved.actors)if(!savedIds.insert(prior.id).second){error="Duplicate saved source checkpoint actor";return false;}
    auto merged=saved.actors;
    for(const auto id:eligible) {
        const auto source=std::find_if(sourceInitial.begin(),sourceInitial.end(),[&](const auto& a){return a.id==id;});
        if(source==sourceInitial.end()||source->player||id==saved.controlled_actor_id){error="Missing genuine non-player source checkpoint receipt";return false;}
        const auto prior=std::find_if(saved.actors.begin(),saved.actors.end(),[&](const auto& a){return a.id==id;});
        if(prior!=saved.actors.end()) {
            if(prior->definition_id!=source->definition_id||prior->player!=source->player){error="Source checkpoint admission identity/role differs";return false;}
            continue; // Existing source state/dead/transform always wins.
        }
        merged.push_back(*source);
    }
    // Unrelated omissions retain source's existing missing-record semantics;
    // validate/restore services still own source-state/role admission.
    saved.actors=std::move(merged);error.clear();return true;
}
}
