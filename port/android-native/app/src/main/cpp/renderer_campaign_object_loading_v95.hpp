#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <functional>
namespace model_renderer {
// Only reached native resource leaves; no whole InitPost/InitFinal provider.
struct CampaignObjectLoadingNativeV95 {
 std::shared_ptr<void> owner;
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,bool,std::string&)> physical_filter;
 std::function<bool(std::uintptr_t,std::string&)> module_sync_visibility;
 std::function<bool(std::string&)> null_handle_assertion;
};
}
