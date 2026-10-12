#pragma once
#include <canonical_colbox_v71.hpp>
#include "object_base_source_destroy_v92.hpp"
#include "container_source_destroy_v92.hpp"
#include "source_release_journal_v69.hpp"
namespace dh2::loader {
struct NonCharacterNativeReleasePrimitivesV92 {
 std::shared_ptr<void> owner; // independent actual Main primitive authority
 world::ColBoxServicesV71 gameobject; // existing exact GameObject source kernel ABI
 ObjectBaseDestroyLeavesV92 object_base;
 ContainerNetworkDestroyV92 container_network;
 // Actual owning thread / frame/contact/scene delivery barrier before native D0.
 std::function<bool(std::uintptr_t,std::string&)> require_cleanup_delivery;
 // Actual GPU/draw/contact/network/observer aliases AFTER native unpublication.
 std::function<bool(std::uintptr_t,std::string&)> require_external_aliases_unpublished;
 // Native failed constructor/resource prefixes not covered by pure host C1.
 std::function<bool(const SourceConstructorPrefixBorrowV89&,std::string&)> require_constructor_delivery;
 std::function<bool(const SourceConstructorPrefixBorrowV89&,std::string&)> unwind_constructor_prefix;
};
}
