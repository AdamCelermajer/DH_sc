#pragma once
#include "animation.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::animation {
// A caller-owned canonical resource identity replaces the original CCDB's
// first reference-control word. The Player and its resource remain borrowed.
struct RegistrationOccurrence {
    std::int32_t engine_index=0,dictionary_id=0;
    std::uint64_t resource_identity=0;
    const Player* player=nullptr;
};
struct RegistrationEntry {
    std::int32_t dictionary_id=0,engine_index=0;
    std::uint64_t resource_identity=0;
};
class RegistrationSet {
    std::vector<RegistrationOccurrence> occurrences_;
    std::vector<RegistrationEntry> entries_; // source game-ID map, sorted by ID
    const Player* default_player_=nullptr;
    std::uint64_t default_identity_=0;
public:
    // LoadAnimation appends before unique ID insertion. Repeated IDs keep the
    // first entry's identity/index while every occurrence is retained.
    // Bounded caller contract: nonnegative ID, nonzero identity, nonnull Player,
    // at most1024 libraries. Failure preserves all registration/default state.
    bool append(std::int32_t dictionary_id,std::uint64_t resource_identity,
                const Player*,std::string& error);
    // AddTemplateAnim's separate second load designates a default database;
    // selecting a default does not append or alter the game-ID map.
    bool set_default(std::uint64_t resource_identity,const Player*,std::string& error);
    // _UpdateAnimationIndices scans the original database vector for the first
    // occurrence of each first-inserted resource identity (CCDB first word).
    void refresh_indices();
    std::int32_t lookup(std::int32_t dictionary_id)const;
    const std::vector<RegistrationOccurrence>& occurrences()const{return occurrences_;}
    const std::vector<RegistrationEntry>& entries()const{return entries_;}
    const Player* default_player()const{return default_player_;}
    std::uint64_t default_identity()const{return default_identity_;}
    // Synthetic keys are engine occurrence indices, not source dictionary IDs.
    // Supply these to TransformSet::compile_dynamic, then resolve source game
    // IDs through lookup for selection. Caller must keep all Players alive.
    std::vector<TransformClipInput> compiled_inputs()const;
};
}
