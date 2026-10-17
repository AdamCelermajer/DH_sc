#pragma once
#include <cstdint>

namespace dh2::character_interactive {
struct Character {
    std::uintptr_t identity, ai;
    std::uint32_t flags_520;
    // byte415 aliases embedded CharAI+4d; _SetIsTargetable changes it.
    std::uint8_t deleted_81, enabled_8a, interactive_415, reserved;
};
enum class Query : std::uint32_t { is_dead, is_friend, is_monster, is_faerie, is_summoned };
struct Services {
    void* context;
    // Return0 means success; value is the raw source predicate word. is_friend
    // uses the captured embedded CharAI identity and fixed interacting object.
    // Other queries use the fixed Character identity. IsDead is queried twice
    // on the normal path; each query must read current source facts. The three
    // classification predicates use genuine fresh AIProps Type queries.
    std::int32_t (*invoke)(void*, Character*, Query, std::uintptr_t subject,
                           std::uintptr_t interacting_object, std::uint32_t* value);
};
struct Result { std::uint32_t value, service_calls, last_query; };
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed,
};

// Complete original Character::IsInteractive(GameObject*) const,192B0x3a4870.
// Dead+nonnull interactor+friend+nonMonster returns1 before the ordinary gates.
// Otherwise deleted/disabled, Faerie, Summoned or freshly dead rejects; flag
// 0x2000 is then required, and the source byte415 is returned without boolean
// normalization. Visibility and IsInvisibleMan are not predicates here.
// One owning thread retains the fixed Character/embedded AI/interactor, facts,
// provider/context and retired backing through return. Providers may mutate
// live scalar facts, but may not change identities, destroy borrowed backing,
// overwrite control/output storage or reenter with the same Character/output.
// Independent objects/outputs and other source modules may nest. Services are
// captured once; errors stop preserving prior provider effects, without rollback.
Status evaluate(Character*, std::uintptr_t interacting_object, const Services*, Result*);
} // namespace dh2::character_interactive
