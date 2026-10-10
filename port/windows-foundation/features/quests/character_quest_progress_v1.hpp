#pragma once

#include "../../character_state.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"
#include <functional>
#include <optional>

namespace dh::foundation {

struct CharacterQuestIdV1 {
    std::uint32_t collection{}; // source regular=0, volatile=1
    std::int32_t difficulty{}, row{};
};
enum class CharacterQuestCategoryV1 { assigned, completed };
struct CharacterQuestLogPolicyV1 {
    bool display_all_quests{};
    bool display_all_debug_only_quests{};
    std::int32_t debug_priority{};
};
struct CharacterQuestStateV1 {
    std::int32_t state{};
    std::optional<std::int32_t> current_quest_row;
};
struct CharacterQuestPageRowV1 {
    CharacterQuestIdV1 id;
    std::string source_name;
    std::array<std::int32_t,4> text_ids{};
    std::optional<std::string> title;
    std::int32_t priority{}, act{}, target_level{};
    bool repeatable{};
    std::int32_t state{};
    bool current{};
};
using CharacterQuestTextV1 = std::function<bool(
    const CharacterState&, std::int32_t, std::string&, std::string&)>;

// Generic, portable progress owned alongside one CharacterState. It records
// source Quest state cells and currentquest only; objective/reward/QEST native
// cells remain outside this model. Default construction is explicitly unknown.
class CharacterQuestProgressV1 {
public:
    enum class Origin : std::uint8_t {
        unknown=0, fresh_source_initialized=1, modified=2, loaded=3
    };
    struct BucketView {
        Origin origin{Origin::unknown};
        std::int32_t current_quest{-1};
        const std::vector<std::int32_t>* states{};
    };
private:
    struct Bucket {
        Origin origin{Origin::unknown};
        std::int32_t current_quest{-1};
        std::vector<std::int32_t> states;
    };
    std::string character_id_;
    std::array<std::array<Bucket,3>,2> buckets_{};
    bool valid_id(const CharacterQuestIdV1&) const noexcept;
public:
    static constexpr std::uint32_t codec_version = 1;
    static constexpr std::size_t maximum_quest_rows = 4096;

    bool belongs_to(const CharacterState&) const noexcept;
    const std::string& character_id() const noexcept { return character_id_; }
    bool bind_character(const CharacterState&, std::string& error);
    bool initialize_fresh(const CharacterState&,
        const dh2::data::QuestTablesPersistenceV51&, std::string& error);
    bool encode(const dh2::data::QuestTablesPersistenceV51&,
                std::vector<std::uint8_t>&, std::string& error) const;
    bool decode(const CharacterState&, const dh2::data::QuestTablesPersistenceV51&,
                const std::vector<std::uint8_t>&, std::string& error);
    bool bucket(const CharacterState&, std::uint32_t collection,
                std::int32_t difficulty, BucketView&, std::string& error) const;
    bool query(const CharacterState&, const CharacterQuestIdV1&,
               CharacterQuestStateV1&, std::string& error) const;
    // Called only after the generic gameplay producer accepts a source state
    // update. This records that producer's exact value; it implements no
    // prerequisite/objective/transition engine itself.
    bool record_source_state(const CharacterState&, const CharacterQuestIdV1&,
                             std::int32_t source_state, std::string& error);
    bool filter(const CharacterState&, const dh2::data::QuestTablesPersistenceV51&,
                std::uint32_t collection, std::int32_t difficulty,
                CharacterQuestCategoryV1, const CharacterQuestLogPolicyV1&,
                const CharacterQuestTextV1&, std::vector<CharacterQuestPageRowV1>&,
                bool& title_sorted, std::string& error) const;
    bool activate(const CharacterState&, const dh2::data::QuestTablesPersistenceV51&,
                  const CharacterQuestIdV1&, const CharacterQuestLogPolicyV1&,
                  std::string& error);
    // Original Character::SG_SetCurrentQuest from quest state transitions (Active
    // sets the row, PostActive clears it with row -1). Not a player action: the
    // Make Active path must use activate().
    bool set_current_quest(const CharacterState&, std::uint32_t collection,
                           std::int32_t difficulty, std::int32_t row, std::string& error);
};

} // namespace dh::foundation
