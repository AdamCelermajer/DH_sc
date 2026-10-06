#pragma once
#include "canonical_level_config_module_v1.hpp"
namespace dh2::world {
struct LevelConfigPublicationBorrowV2 {
 std::shared_ptr<void> level_owner;
 std::uintptr_t* config38{};
 std::int32_t* music11c{};std::int32_t* safezone120{};std::int32_t* ambient124{};
 // SAME actual Arrays::Sounds names, index is original sound ID.
 std::shared_ptr<const void> arrays_owner;
 const std::vector<std::string>* sound_names{};
 std::function<const CanonicalLevelConfigV1*(std::uintptr_t)> config;
};
// Whole safe nonnull source Level::SetLevelConfig3f150c. Optional empty
// ambient/safezone strings preserve previous IDs; main music always resolves.
bool level_config_publication_v2(const LevelConfigPublicationBorrowV2&,
 std::uintptr_t identity,std::string&);
}
