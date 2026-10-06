#pragma once
#include "retained_generic_animator_v21.hpp"
#include "canonical_destructible_container_v16.hpp"
namespace dh2::world {
// The provider returns the retained controller for the CURRENT attached visual.
// A replacement visual must expose its own actual animator callback fields.
using ContainerAnimatorProviderV21=std::function<bool(std::shared_ptr<RetainedGenericAnimatorV21>&,std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)>;
bool connect_openable_animation_v21(OpenableContainerServicesV1&,std::shared_ptr<std::weak_ptr<CanonicalOpenableContainerV1>>,ContainerAnimatorProviderV21,std::string&);
bool connect_destructible_animation_v21(DestructibleContainerServicesV16&,std::shared_ptr<std::weak_ptr<CanonicalDestructibleContainerV16>>,ContainerAnimatorProviderV21,std::string&);
}
