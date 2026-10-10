#pragma once

#include "character_faery_links_v1.hpp"
#include "character_ai_classification.hpp"
#include "../game-data/ai.hpp"

namespace dh2::character_faery_factory_bridge_v1 {

namespace factory = character_runtime_factory_v1;
namespace links = character_faery_links_v1;
namespace manager = object_manager_runtime_owner_v1;

enum class Status : unsigned char {
    complete,
    missing_factory_character,
    missing_canonical_ai,
    source_ai_not_ready,
    source_type_unavailable,
    source_type_query_failed,
    link_owner_rejected,
};

// Register the same stable Character and CharAI owners held by the source
// factory. The call belongs after InitFinal; it creates no roster or owner.
Status register_character(factory::Owner&, manager::SourceHandle,
                          std::int32_t source_character_type,
                          links::Role, links::Owner&, std::string& error);

// Resolve a Faery's CharType through the existing GetCharType kernel using
// the AI id retained by its completed non-player InitPost branch and the
// caller's live source AIProps table. No type is inferred from its role.
Status register_faery_from_source(factory::Owner&, manager::SourceHandle,
                                  const data::AiTables&, links::Owner&,
                                  std::string& error);

// Remove role bindings before factory retirement so +0x420 references are
// cleared while both canonical Character Records are still alive.
Status unregister_character(factory::Owner&, manager::SourceHandle,
                            links::Owner&, std::string& error) noexcept;

} // namespace dh2::character_faery_factory_bridge_v1
