#pragma once
#include "canonical_existing_actor_publication_v3.hpp"
#include "character_script_objects.hpp"
#include "actor_runtime.hpp"
namespace dh2::world {
// A facet of the existing player, never a second Character/GameObject owner.
// Every field must be provided by the caller's retained source producer.
struct CanonicalPlayerFieldsV3 {
 std::shared_ptr<character::ScriptCharacterObject> object;
 actor::RuntimeState* runtime{};
 target_providers::Handle16* handle{};
 const std::uint32_t* type_f4{};
 std::int32_t* room64{};
 // Optional until the source name lookup reaches a room-mismatch branch.
 // Manager rejects that reached branch if its actual producer is unavailable.
 const std::uint8_t* across_rooms87{};
 const char** class_name20{};
 std::string* name{};
 std::string* archetype{};
 std::shared_ptr<void> world_lease;
};
class CanonicalPlayerFacetV3 {
 CanonicalPlayerFieldsV3 fields_;
 static bool name(void*,const char*,std::string&);
 static bool archetype(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);
public:
 explicit CanonicalPlayerFacetV3(CanonicalPlayerFieldsV3 fields):fields_(std::move(fields)){}
 bool borrow(std::shared_ptr<void> facet_lease,CanonicalExistingActorV3&,std::string&);
};
}
