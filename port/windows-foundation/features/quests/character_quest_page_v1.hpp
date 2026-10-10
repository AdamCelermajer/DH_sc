#pragma once

#include "character_quest_progress_v1.hpp"
#include "quest_text_resolver_v1.hpp"

namespace dh::foundation {

struct CharacterQuestPageSnapshotV1 {
    std::uint32_t collection{};
    std::int32_t difficulty{};
    CharacterQuestCategoryV1 category{CharacterQuestCategoryV1::assigned};
    std::vector<CharacterQuestPageRowV1> rows;
    bool title_sorted{};
};
struct CharacterQuestPageSelectionV1 {
    CharacterQuestPageRowV1 row;
    SourceQuestPageTextV1 details;
    bool activation_visible{};
};

// Root-callable generic Quest Page over the same live CharacterState and its
// feature-owned per-character progress model. Quest definitions and native
// SWF routing stay owned by the caller; localization remains an explicit
// source-backed callback.
class SourceCharacterQuestPageV1 {
    CharacterState* character_{};
    CharacterQuestProgressV1* progress_{};
    std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> tables_;
    CharacterQuestTextV1 text_;
public:
    SourceCharacterQuestPageV1(CharacterState&, CharacterQuestProgressV1&,
        std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>,
        CharacterQuestTextV1 = {});
    bool refresh(std::uint32_t collection, std::int32_t difficulty,
        CharacterQuestCategoryV1, const CharacterQuestLogPolicyV1&,
        CharacterQuestPageSnapshotV1&, std::string& error) const;
    bool select(const CharacterQuestPageSnapshotV1&, const CharacterQuestIdV1&,
        CharacterQuestPageSelectionV1&, std::string& error) const;
    bool activate(const CharacterQuestPageSnapshotV1&, const CharacterQuestIdV1&,
        const CharacterQuestLogPolicyV1&, std::string& error) const;
};

} // namespace dh::foundation
