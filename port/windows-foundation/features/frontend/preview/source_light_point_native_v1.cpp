#include "source_light_point_native_v1.hpp"

#include <algorithm>
#include <cstring>
#include <exception>
#include <utility>

namespace dh::foundation::frontend {
namespace {
bool required(std::string& error, const char* provider) {
    error = std::string("Required actual LightPoint ") + provider;
    return false;
}

bool same_owner(const std::shared_ptr<void>& a, const std::shared_ptr<void>& b) {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
}

SourceLightPointNativeV1::SourceLightPointNativeV1(
    dh2::world::CanonicalObjectBorrowV1 object, std::shared_ptr<void> application,
    std::uintptr_t application_identity, SourceLightPointNativeServicesV1 services)
    : object_(std::move(object)), application_owner_(std::move(application)),
      application_identity_(application_identity), services_(std::move(services)) {}

SourceLightPointNativeV1::~SourceLightPointNativeV1() noexcept {
    if (node_owner_) {
        node_owner_->drop();
        node_owner_.reset();
    }
}

bool SourceLightPointNativeV1::current(std::string& error) {
    if (!application_owner_ || !application_identity_ ||
        !same_owner(application_owner_, services_.application_owner) ||
        services_.application_identity != application_identity_ ||
        !services_.current_application)
        return required(error, "same live Application owner");
    if (!services_.current_application(error)) {
        if (error.empty()) error = "Actual LightPoint Application owner is no longer current";
        return false;
    }
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::fail(std::string error, std::string& out) {
    if (error.empty()) error = "Actual LightPoint source operation failed";
    error_ = std::move(error);
    stage_ = SourceLightPointNativeStageV1::failed;
    out = error_;
    return false;
}

bool SourceLightPointNativeV1::adopt_type19_v1(
    dh2::world::CanonicalObjectBorrowV1 object, std::shared_ptr<void> application_owner,
    std::uintptr_t application_identity, SourceLightPointNativeServicesV1 services,
    std::shared_ptr<SourceLightPointNativeV1>& out, std::string& error) {
    out.reset();
    if (!object.identity || !object.lease || !object.type_f4 ||
        *object.type_f4 != SourceLightPointNativeFieldsV1::object_type)
        return required(error, "published canonical ObjectBase with type_f4 19");
    if (!object.class_name20 || !*object.class_name20 ||
        std::strcmp(*object.class_name20, "LightPoint") != 0)
        return required(error, "source factory class-name20 LightPoint publication");
    if (!application_owner || !application_identity ||
        !same_owner(application_owner, services.application_owner) ||
        application_identity != services.application_identity || !services.current_application)
        return required(error, "same actual Application source owner");
    if (!services.current_application(error)) {
        if (error.empty()) error = "Actual LightPoint Application owner is no longer current";
        return false;
    }
    try {
        out = std::shared_ptr<SourceLightPointNativeV1>(new SourceLightPointNativeV1(
            std::move(object), std::move(application_owner), application_identity,
            std::move(services)));
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::set_light_node(
    const dh2::world::NativeLightNodeV113& node, std::string& error) {
    if (stage_ != SourceLightPointNativeStageV1::constructed)
        return fail("LightBase::setLightNode source prefix cannot replay", error);
    if (!current(error)) return fail(error, error);
    if (!node.owner || !node.identity || !node.light || node.light->native_destroyed_v113)
        return fail("same live NativeSceneLightNodeV113/CLight alias", error);

    auto candidate = std::static_pointer_cast<dh2::world::NativeSceneLightNodeV113>(node.owner);
    if (!candidate || candidate->identity() != node.identity || !candidate->live() ||
        candidate->light() != node.light)
        return fail("NativeSceneLightNodeV113 owner matching its CLight borrow", error);
    if (!candidate->grab(error)) return fail(error, error);

    // Source LightBase::setLightNode 0x40b498 retains the node first, then
    // copies CLight radius/attenuation/ambient/diffuse/specular into fields.
    auto previous = std::move(node_owner_);
    node_owner_ = std::move(candidate);
    node_ = node;
    fields_.light_node288 = node.identity;
    std::memcpy(&fields_.radius304_bits, &node.light->radius40, sizeof(fields_.radius304_bits));
    fields_.radius304_produced = true;
    fields_.attenuation308_ = node.light->attenuation34;
    std::copy_n(node.light->ambient4.begin(), 3, fields_.ambient320_.begin());
    std::copy_n(node.light->diffuse14.begin(), 3, fields_.diffuse332_.begin());
    std::copy_n(node.light->specular24.begin(), 3, fields_.specular344_.begin());
    if (previous) previous->drop();
    stage_ = SourceLightPointNativeStageV1::node_bound;
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::init_post(std::string& error) {
    if (stage_ != SourceLightPointNativeStageV1::node_bound)
        return fail("LightPoint::InitPost source prefix requires setLightNode", error);
    if (!current(error)) return fail(error, error);
    if (!services_.light_base_init_post)
        return fail("LightBase::InitPost application/driver/SceneManager source service", error);
    fields_.refresh_state432 = 0;
    fields_.refresh_state433 = 0;
    if (!services_.light_base_init_post(object_.identity, fields_, node_, error))
        return fail(error.empty() ? "LightBase::InitPost source service" : error, error);
    if (!node_owner_ || node_.identity != node_owner_->identity() ||
        node_.light != node_owner_->light())
        return fail("same retained CLightSceneNode after LightBase::InitPost", error);
    // LightPoint::InitPost writes CLight type 0 before virtual RefreshAttachment.
    if (!node_owner_->set_type(0, error)) return fail(error, error);
    if (!refresh_attachment(error)) return false;
    stage_ = SourceLightPointNativeStageV1::initialized;
    error_.clear();
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::update(std::string& error) {
    if (stage_ != SourceLightPointNativeStageV1::initialized)
        return fail("LightPoint::Update requires completed InitPost", error);
    if (!current(error)) return fail(error, error);
    if (!services_.light_base_update)
        return fail("LightBase::Update source Scene/driver service", error);
    if (!services_.light_base_update(object_.identity, fields_, node_, error))
        return fail(error.empty() ? "LightBase::Update source service" : error, error);
    if (!node_owner_ || node_.identity != node_owner_->identity() ||
        node_.light != node_owner_->light())
        return fail("same retained CLightSceneNode after LightBase::Update", error);
    // Source LightPoint::Update continues with ObjectHandle actor resolution
    // and attached-position writes. The provider must execute that exact tail.
    if (!services_.light_point_update_tail)
        return fail("LightPoint::Update ObjectHandle/attachment source tail", error);
    if (!services_.light_point_update_tail(object_.identity, fields_, node_, error))
        return fail(error.empty() ? "LightPoint::Update ObjectHandle/attachment source tail" : error, error);
    error_.clear();
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::refresh_attachment(std::string& error) {
    if (stage_ != SourceLightPointNativeStageV1::node_bound &&
        stage_ != SourceLightPointNativeStageV1::initialized)
        return fail("LightPoint::RefreshAttachment source prefix requires a bound node", error);
    if (!current(error)) return fail(error, error);
    if (!node_owner_ || !node_owner_->live() || node_.identity != node_owner_->identity() ||
        node_.light != node_owner_->light())
        return fail("same live LightPoint node/CLight before RefreshAttachment", error);
    if (!services_.refresh_attachment)
        return fail("LightPoint::RefreshAttachment ObjectManager/Application source service", error);
    if (!services_.refresh_attachment(object_.identity, fields_, node_, error))
        return fail(error.empty() ? "LightPoint::RefreshAttachment source service" : error, error);
    error.clear();
    return true;
}

bool SourceLightPointNativeV1::destroy_after_object_manager_unpublish(std::string& error) {
    if (stage_ == SourceLightPointNativeStageV1::destroyed)
        return fail("LightPoint source destructor cannot replay", error);
    if (stage_ == SourceLightPointNativeStageV1::failed)
        return fail("failed LightPoint source prefix requires original ObjectManager rollback", error);
    if (!current(error)) return fail(error, error);
    if (!services_.source_objectbase_destroy)
        return fail("ObjectBase deleting-destructor tail after ObjectManager unpublish", error);

    // LightPoint::~LightPoint 0x40bd1c first destroys attachedTo and then
    // invokes LightBase D1. The latter releases the retained source node here;
    // only then may the original ObjectBase deleting-destructor tail run.
    fields_.attached_to384.clear();
    fields_.light_base_string360.clear();
    if (node_owner_) {
        node_owner_->drop();
        node_owner_.reset();
    }
    node_ = {};
    fields_.light_node288 = 0;
    if (!services_.source_objectbase_destroy(application_identity_, object_.identity, error))
        return fail(error.empty() ? "ObjectBase deleting-destructor source service" : error, error);
    stage_ = SourceLightPointNativeStageV1::destroyed;
    error_.clear();
    error.clear();
    return true;
}

} // namespace dh::foundation::frontend
