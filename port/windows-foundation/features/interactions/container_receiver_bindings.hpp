#pragma once

#include "live_receiver_bindings.hpp"
#include "../../../level-world/canonical_destructible_container_v16.hpp"
#include "../../../level-world/container_animation_connection_v21.hpp"

namespace dh::foundation::interactions {

struct DestructibleBorrowV16 {
    std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16> receiver;
};

using DestructibleResolverV16 = std::function<bool(
    std::uintptr_t, DestructibleBorrowV16&, std::string&)>;

// Installs the actual factory GO_ID1 receiver into LiveReceiverBindings' live
// source-type dispatch. The resolver must borrow the object already published
// by the shared factory/ObjectManager; it must not reconstruct from XML.
inline bool bind_destructible_receiver_v16(
    LiveReceiverServices& services,
    DestructibleResolverV16 resolve,
    std::string& error) {
    error.clear();
    if (!resolve) {
        error = "Interactions require retained DestructibleContainer resolver";
        return false;
    }
    constexpr std::uint32_t source_go_id = 1; // factory catalog GO_ID1
    auto [it, inserted] = services.authored_receivers.emplace(
        source_go_id,
        [resolve = std::move(resolve), source_go_id](
            const dh2::world::CanonicalObjectBorrowV1& published,
            std::uintptr_t actor,
            std::string& e) {
            if (!published.identity || !published.lease || !published.type_f4 ||
                *published.type_f4 != source_go_id || !actor) {
                e = "Interactions require published GO_ID1 target and actor";
                return false;
            }
            if (!published.class_name20 || !*published.class_name20 ||
                std::string(*published.class_name20) != "DestructibleContainer") {
                e = "Interactions GO_ID1 source class is not DestructibleContainer";
                return false;
            }
            DestructibleBorrowV16 borrowed;
            if (!resolve(published.identity, borrowed, e)) return false;
            if (!borrowed.receiver || borrowed.receiver->base().identity() != published.identity) {
                e = "Interactions DestructibleContainer receiver identity mismatch";
                return false;
            }
            // Check the canonical receiver's own source declaration too. The
            // published and resolved aliases must name the same factory GO_ID.
            auto actual = borrowed.receiver->canonical(borrowed.receiver);
            if (!actual.type_f4 || *actual.type_f4 != source_go_id ||
                !actual.class_name20 || !*actual.class_name20 ||
                std::string(*actual.class_name20) != "DestructibleContainer") {
                e = "Interactions resolved receiver lost original GO_ID1 declaration";
                return false;
            }
            return borrowed.receiver->interact(actor, e);
        });
    if (!inserted) {
        error = "Interactions GO_ID1 receiver handler already bound";
        return false;
    }
    return true;
}

struct DestructibleAnimationBindingV21 {
    std::shared_ptr<std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>> slot;
    void attach(const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>& receiver) {
        if (slot) *slot = receiver;
    }
};

// Prepare before the canonical receiver constructor copies its services, then
// attach that same receiver before InitPost installs the source callbacks. The
// provider resolves its CURRENT attached visual on every callback.
inline bool prepare_destructible_animation_v21(
    dh2::world::DestructibleContainerServicesV16& source_services,
    dh2::world::ContainerAnimatorProviderV21 current_visual_animator,
    DestructibleAnimationBindingV21& out,
    std::string& error) {
    error.clear();
    DestructibleAnimationBindingV21 next;
    next.slot = std::make_shared<std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>>();
    if (!dh2::world::connect_destructible_animation_v21(
            source_services, next.slot, std::move(current_visual_animator), error)) return false;
    out = std::move(next);
    return true;
}

struct OpenableAnimationBindingV21 {
    std::shared_ptr<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>> slot;
    void attach(const std::shared_ptr<dh2::world::CanonicalOpenableContainerV1>& receiver) {
        if (slot) *slot = receiver;
    }
};

// Equivalent preconstruction seam for the canonical OpenableContainer
// receiver; it supplies its actual retained timeline, never a timer.
inline bool prepare_openable_animation_v21(
    dh2::world::OpenableContainerServicesV1& source_services,
    dh2::world::ContainerAnimatorProviderV21 current_visual_animator,
    OpenableAnimationBindingV21& out,
    std::string& error) {
    error.clear();
    OpenableAnimationBindingV21 next;
    next.slot = std::make_shared<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>>();
    if (!dh2::world::connect_openable_animation_v21(
            source_services, next.slot, std::move(current_visual_animator), error)) return false;
    out = std::move(next);
    return true;
}

} // namespace dh::foundation::interactions
