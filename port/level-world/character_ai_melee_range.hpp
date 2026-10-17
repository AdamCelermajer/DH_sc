#pragma once
#include "character_ai_sight.hpp"
#include "character_enemy_retention.hpp"
#include <cstdint>
namespace dh2::character_ai_melee_range {
using State=character_ai_sight::State;
using Point=character_ai_sight::Point;
using AiRow=character_enemy_retention::AiRow;
using AiTable=character_enemy_retention::AiTable;
struct ResolvedObject { std::uintptr_t identity; std::uint32_t word_f4; };
enum class Operation : std::uint32_t {
    resolve_object, interaction_type, target_position, melee_radius,
    diagnostic_switch, interaction_range,
};
enum class Subject : std::uint32_t { object, original_ai, resolved_character_ai, global };
struct Request {
    Operation operation; Subject kind;
    std::uintptr_t subject, other;
    std::uint32_t key;
};
struct Response { const void* view; std::uint32_t word; };
struct Services {
    void* context;
    // Zero success. resolve_object performs const GetHandle/GetObject(false),
    // returning borrowed ResolvedObject* (null allowed); no Character RTTI.
    // target_position returns borrowed Point*. Predicate/radius words are raw.
    // resolved_character_ai subject is the resolved Character identity; provider
    // resolves its genuine embedded CharAI, not identity arithmetic on port IDs.
    // Diagnostic keys1/2 encapsulate their exact global load/Load/GetSwitch/string
    // cleanup sequences. Key1 truthiness selects key2; key2 return is ignored.
    // interaction_range is the original AI_IsInInteractionRange boundary. A
    // missing provider reports unavailable; no approximate generic range is used.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
struct Result {
    std::uint32_t value,calls,distance_word,owner_radius_word,target_radius_word,
        sum_word,square_word,used_interaction_range;
    std::uintptr_t candidate,resolved;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact,
};
Status evaluate_object(State*,std::uintptr_t candidate,const Services*,Result*);

enum class RadiusOperation : std::uint32_t { inventory_can_melee_attack, ai_table, char_ai_id };
struct RadiusRequest { RadiusOperation operation; std::uintptr_t owner; };
struct RadiusResponse { const AiTable* table; std::uint32_t word; };
struct RadiusServices {
    void* context;
    // inventory_can_melee_attack projects ItemInventory::CanMeleeAttack(int&)
    // with its output initialized to zero. Its bool return is ignored; word is
    // the final int output, including retention of zero for a ranged weapon.
    // ai_table returns captured global rows; char_ai_id is genuine getter with
    // fallback8 semantics. Both may change the live owner and backing row values.
    std::int32_t (*invoke)(void*,State*,const RadiusRequest*,RadiusResponse*);
};
struct RadiusResult {
    std::uint32_t value_word,calls,equipment_word,ai_id,row_word;
};
// Original 104-byte AI_GetMeleeRadius caller: first owner inventory output,
// second owner captured BEFORE global AIProps base, GetCharAIId on that owner,
// signed int→binary32, late captured-row+20 read, separately rounded float add.
Status get_radius(State*,const RadiusServices*,RadiusResult*);

// One owning thread retains original AI, candidate, resolved object/Character,
// owners, returned points/tables/rows and provider storage through return,
// including retired backing after replacement. Stable identity keys do not
// change. Providers may change live owner/target/row/point facts; same-State
// reentry and output/service overwrite or borrowed destruction are forbidden.
// Independent sessions may nest. Missing/error/throwing providers preserve prior
// effects; no rollback or fabricated classifications. Soft-float import bodies
// remain modeled IEEE binary32 operations; no NaN payload propagation claim.
} // namespace dh2::character_ai_melee_range
