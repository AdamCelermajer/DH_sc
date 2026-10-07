#pragma once
#include "character_frame_fields_v106.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::character {
struct CharacterInterestFieldsV106 {
 std::uintptr_t identity{};
 std::uintptr_t* object14a4{};
 std::int32_t* type14a8{}; // SAME AnimationAI signed-byte projection.
 std::int16_t* delay14aa{};
 std::uint8_t *blocked14ac{},*changed14ad{};
};
struct CharacterInterestCandidateV106 {std::uintptr_t identity{};std::uint32_t flags{};};
struct CharacterInterestServicesV106 {
 std::shared_ptr<void> owner;
 // Actual selected virtual8c/90; callbacks may change the lent owner fields.
 std::function<bool(std::uintptr_t,std::uintptr_t,bool&,std::string&)> interactive;
 std::function<bool(std::uintptr_t,const std::uint8_t*&,std::shared_ptr<void>&,std::string&)> disabled;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::int32_t&,std::string&)> interaction_type;
 // Produces original TargetList(owner,1,0,1), then character_flags34=89 /
 // object_type38=1 Search over
 // actual manager list80 at GetTargetPosition and InteractionRadius constant,
 // full 2*pi angle. Results are actual priority-queue pop order, not map order.
 std::function<bool(std::uintptr_t,std::vector<CharacterInterestCandidateV106>&,std::string&)> search;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> item_owner3bc;
};
// Whole Character.UpdateObjectOfInterest3abb9c, including the signed16 wrap,
// repeated virtual90 queries, item ownership rejection and change14ad latch.
bool source_character_update_interest_v106(CharacterInterestFieldsV106&,
 std::uint32_t actual_dt,const CharacterInterestServicesV106&,std::string&);
}
