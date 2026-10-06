#pragma once
#include "canonical_level_module_bindings_v2.hpp"
#include "canonical_character_family_v4.hpp"
#include "canonical_openable_graph_v4.hpp"
#include "canonical_openable_graph_v21.hpp"
#include "application_spawn_random_owner_v4.hpp"
namespace dh2::loader {
struct CanonicalLevelClassDispatchInputsV1 {
    std::shared_ptr<void> providers_owner;
    world::CanonicalLevelModuleBindingsV2* modules{};
    std::shared_ptr<world::CanonicalCharacterFamilyFactoryV4> characters;
    // Both channels belong to the real application lifetime, outside Level.
    std::shared_ptr<void> application_owner;
    world::ApplicationSpawnRandomOwnerV4* random{};
    std::function<bool(const world::CanonicalSourceObjectRequestV1&,
        world::CanonicalOpenableGraphServicesV4&,std::string&)> openable_services;
    std::function<bool(const std::shared_ptr<world::CanonicalOpenableGraphV4>&,
        world::CanonicalSpawnApplicationServicesV4&,std::string&)> spawn_services;
    // Every remaining catalog class goes to its actual engine constructor.
    std::function<bool(const world::CanonicalFactoryEntryV1&,
        const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&)> remaining;
    // Additive same-receiver construction hook. Bind callbacks to this weak
    // publication cell; the constructor fills it with its actual result.
    // Preferred when table/conditions/PF services need the actual chest base.
    std::function<bool(const world::CanonicalSourceObjectRequestV1&,
        const std::shared_ptr<std::weak_ptr<world::CanonicalOpenableGraphV4>>&,
        world::CanonicalOpenableGraphServicesV4&,std::string&)> openable_services_with_receiver;
    // Preferred source-positive graph. Legacy V4 hooks remain available only
    // for existing constructor clients; they do not prove event delivery.
    std::function<bool(const world::CanonicalSourceObjectRequestV1&,
        const std::shared_ptr<std::weak_ptr<world::CanonicalOpenableGraphV21>>&,
        world::CanonicalOpenableGraphServicesV21&,std::string&)> openable_v21_services_with_receiver;
    std::function<bool(const std::shared_ptr<world::CanonicalOpenableGraphV21>&,
        world::CanonicalSpawnApplicationServicesV4&,std::string&)> spawn_v21_services;
};
// Constructor composition only. CanonicalReceiverTransportV1 still supplies
// the SAME PropertyMap/factory/manager flow. No Character positioning, visual,
// script, physics or gameplay implementation is fabricated by this adapter.
class CanonicalLevelClassDispatchV1 final {
    CanonicalLevelClassDispatchInputsV1 input_;
    std::vector<std::shared_ptr<world::CanonicalOpenableGraphV4>> containers_;
    std::vector<std::shared_ptr<world::CanonicalOpenableGraphV21>> containers_v21_;
public:
    explicit CanonicalLevelClassDispatchV1(CanonicalLevelClassDispatchInputsV1 in):input_(std::move(in)){}
    bool construct(const world::CanonicalFactoryEntryV1&,
        const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
    const auto& containers()const noexcept{return containers_;}
    const auto& containers_v21()const noexcept{return containers_v21_;}
    const auto& characters()const noexcept{return input_.characters;}
    // Call only after actual manager unpublication and object destruction.
    void erased(std::uintptr_t);
};
}
