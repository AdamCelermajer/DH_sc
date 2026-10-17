#include "character_menu_reload_v1.hpp"

namespace {
using namespace dh2::ui;

bool aligned(const void* pointer, std::size_t alignment) {
    return pointer && reinterpret_cast<std::uintptr_t>(pointer) % alignment == 0;
}

bool overlaps(const void* left, const void* right) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < sizeof(MenuReloadResult16V1)
                  : a - b < sizeof(MenuReloadResult16V1);
}
}

extern "C" int dh2_character_menu_reload_v1(
    dh2::ui::MenuReloadResult16V1* out, std::uintptr_t character,
    const dh2::ui::MenuReloadServices16V1* services) {
    using namespace dh2::ui;
    if (!aligned(out, alignof(MenuReloadResult16V1)) || !character ||
        !aligned(services, alignof(MenuReloadServices16V1)) ||
        !services->invoke || overlaps(out, services)) {
        return -1;
    }

    *out = {};
    MenuReloadResponse16V1 response{};
    const auto send = [&](MenuReloadServiceV1 service,
                          std::uint32_t argument = 0,
                          std::uintptr_t subject = 0,
                          const char* path = nullptr,
                          const char* callback = nullptr) {
        out->phase = static_cast<std::uint32_t>(service) + 1;
        ++out->calls;
        response = {};
        const MenuReloadRequest32V1 request{
            static_cast<std::uint32_t>(service), argument,
            subject ? subject : character, path, callback};
        return services->invoke(services->context, &request, &response) == 0 &&
               response.reserved == 0;
    };

    if (!send(reload_remove_buffs_v1) ||
        !send(reload_saved_skills_v1, 0x20) ||
        !send(reload_skill_instances_v1) ||
        !send(reload_update_skills_v1) ||
        !send(reload_recalculate_v1, 1) ||
        !send(reload_check_items_v1) ||
        !send(reload_saved_level_v1)) {
        return -2;
    }

    if (response.value > 11) {
        // The source rereads saved class on each comparison.
        constexpr std::int32_t classes[]{263, 325, 290};
        for (const auto wanted : classes) {
            if (!send(reload_saved_class_v1)) return -2;
            if (response.value == wanted) {
                out->specialization = 1;
                break;
            }
        }
    }

    if (!send(reload_menu_fx_v1) || !response.identity) return -2;
    const auto menu_fx = response.identity;
    if (!send(reload_spec_prompt_v1, out->specialization, menu_fx,
              "_root.menu_CharacterMenu", "IsSpecTime")) {
        return -2;
    }
    out->phase = 11;
    return 1;
}
