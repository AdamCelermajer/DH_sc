#include "../source_light_point_native_v1.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

using namespace dh::foundation::frontend;

namespace {
void check(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL " << message << '\n';
        std::exit(1);
    }
}

struct Fixture {
    std::uint32_t type{19};
    std::string class_name{"LightPoint"};
    const char* class_name_field{class_name.c_str()};
    std::shared_ptr<int> object_lease{std::make_shared<int>(7)};
    std::shared_ptr<int> app_lease{std::make_shared<int>(9)};
    dh2::world::CanonicalObjectBorrowV1 object;
    SourceLightPointNativeServicesV1 services;
    unsigned current_calls{};
    unsigned base_init_calls{};
    unsigned refresh_calls{};
    unsigned destroy_calls{};

    Fixture() {
        object.identity = reinterpret_cast<std::uintptr_t>(object_lease.get());
        object.lease = object_lease;
        object.type_f4 = &type;
        object.class_name20 = &class_name_field;
        services.application_owner = app_lease;
        services.application_identity = reinterpret_cast<std::uintptr_t>(app_lease.get());
        services.current_application = [this](std::string& error) {
            ++current_calls; error.clear(); return true;
        };
    }
};
}

int main() {
    Fixture f;
    std::string error;
    std::shared_ptr<dh2::world::NativeSceneLightNodeV113> scene_node;
    check(dh2::world::NativeSceneLightNodeV113::construct(scene_node, error),
          "existing NativeSceneLightNodeV113 source constructor failed");
    f.services.light_base_init_post = [&f](std::uintptr_t object,
        SourceLightPointNativeFieldsV1& fields, dh2::world::NativeLightNodeV113& node,
        std::string& error_out) {
        ++f.base_init_calls;
        const bool same = object == f.object.identity && fields.light_node288 == node.identity && node.identity;
        error_out.clear(); return same;
    };
    f.services.refresh_attachment = [&f](std::uintptr_t object,
        SourceLightPointNativeFieldsV1& fields, dh2::world::NativeLightNodeV113& node,
        std::string& error_out) {
        ++f.refresh_calls;
        const bool same = object == f.object.identity && fields.light_node288 == node.identity && node.identity;
        error_out.clear(); return same;
    };
    f.services.source_objectbase_destroy = [&f](std::uintptr_t app, std::uintptr_t object,
                                                std::string& error_out) {
        ++f.destroy_calls;
        const bool same = app == f.services.application_identity && object == f.object.identity;
        error_out.clear(); return same;
    };
    std::shared_ptr<SourceLightPointNativeV1> point;
    check(SourceLightPointNativeV1::adopt_type19_v1(f.object, f.app_lease,
          f.services.application_identity, f.services, point, error),
          "type-19 canonical receiver and same Application should admit LightPoint C1/C2");
    check(point->objectbase().identity == f.object.identity,
          "LightPoint owner must keep the published ObjectBase identity");
    const auto& c1 = point->fields();
    check(c1.light_node288 == 0 && !c1.radius304_produced && !c1.add_to_scene356,
          "LightBase C1 null node and unproduced fields +304/+356 differ");
    check(c1.attenuation308_ == std::array<float, 3>{0,0,0} &&
          c1.ambient320_ == std::array<float, 3>{0,0,0} &&
          c1.diffuse332_ == std::array<float, 3>{0,0,0} &&
          c1.specular344_ == std::array<float, 3>{0,0,0} &&
          c1.light_base_string360.empty() && c1.attached_to384.empty() &&
          c1.attached_object408.key == 0 && c1.attached_object408.frame == UINT32_MAX &&
          c1.attached_object408.cached == 0 && c1.attached_offset420 == std::array<float, 3>{0,0,0},
          "LightBase/LightPoint C1 zero/string/handle fields differ");

    auto light = scene_node->light();
    light->radius40 = 72.f;
    light->attenuation34 = {0.75f, 0.0125f, 0.000006f};
    light->ambient4 = {0.1f, 0.2f, 0.3f, 1.f};
    light->diffuse14 = {0.4f, 0.5f, 0.6f, 1.f};
    light->specular24 = {0.7f, 0.8f, 0.9f, 1.f};
    const auto borrowed_node = scene_node->borrow();
    check(point->set_light_node(borrowed_node, error),
          "LightBase::setLightNode should retain and copy from the actual native receiver");
    const auto identity = scene_node->identity();
    check(point->node().identity == identity && point->node().light == light &&
          point->fields().light_node288 == identity && point->fields().radius304_produced &&
          point->fields().attenuation308_ == light->attenuation34 &&
          point->fields().ambient320_ == std::array<float,3>{0.1f,0.2f,0.3f} &&
          point->fields().diffuse332_ == std::array<float,3>{0.4f,0.5f,0.6f} &&
          point->fields().specular344_ == std::array<float,3>{0.7f,0.8f,0.9f},
          "setLightNode must use the exact same CLight and source field order");

    const bool init_ok = point->init_post(error);
    if (!init_ok) std::cerr << "InitPost detail: " << error << '\n';
    check(init_ok && f.base_init_calls == 1 && f.refresh_calls == 1 &&
          point->stage() == SourceLightPointNativeStageV1::initialized && light->type58 == 0,
          "LightPoint::InitPost must call base InitPost, type the same CLight, then refresh attachment");
    check(point->destroy_after_object_manager_unpublish(error) && f.destroy_calls == 1 &&
          point->stage() == SourceLightPointNativeStageV1::destroyed,
          "LightPoint destructor must release the retained node before ObjectBase destructor tail");
    check(f.current_calls >= 4, "every source boundary must revalidate the same Application owner");

    std::cout << "PASS real NativeSceneLightNode/CLight field bind, type-19 canonical identity, and LightPoint lifecycle order\n";
}
