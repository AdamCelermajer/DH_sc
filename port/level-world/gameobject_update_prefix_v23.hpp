#pragma once
#include "canonical_object_manager_v1.hpp"
#include "character_design_services.hpp"
namespace dh2::world {
struct GameObjectUpdatePrefixServicesV23 {
 std::shared_ptr<void> provider_lease;
 character::DebugSwitches* debug{};
 character::DebugFileServices24 files{};
 void* context{};
 // Fresh Application+38 borrow AFTER Debug. Do not retain a stale manager
 // snapshot across callbacks; source Debug/file providers may reenter.
 bool(*manager38)(void*,CanonicalObjectManagerV1*&,std::string&){};
};
bool gameobject_update_begin_v23(const GameObjectUpdatePrefixServicesV23&,std::string&);
// Push/PopProfilingContext3136b4/3136b8 are literal bx lr in original ELF.
inline bool gameobject_update_end_v23(std::string&)noexcept{return true;}
}
