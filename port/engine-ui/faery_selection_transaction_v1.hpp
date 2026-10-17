#pragma once

#include <cstdint>
#include <string>

namespace dh2::ui {

// Source NativeHUDSetActiveFaery spans Character::ChangeFaery and the active
// Level's PlaceFaeryAndFollowers. `prepare` must validate both owners and the
// placement request without changing Save, skills, or world state. `commit`
// owns the coordinated mutation; it must leave both owners unchanged on
// failure. The opaque plan is private to that canonical provider.
struct FaerySelectionTransactionServicesV1 {
    void* context{};
    bool (*prepare)(void*, std::uintptr_t character, std::uint32_t faery_id,
                    std::uintptr_t& plan, std::string&){};
    bool (*commit)(void*, std::uintptr_t plan, std::string&){};
    void (*discard)(void*, std::uintptr_t plan){};
};

inline bool transact_faery_selection_v1(
        std::uintptr_t character, std::uint32_t faery_id,
        const FaerySelectionTransactionServicesV1& services,
        std::string& error) {
    if (!character || !services.prepare || !services.commit || !services.discard) {
        error = "Faery selection requires canonical Save/skill and active Level placement transaction owners";
        return false;
    }
    std::uintptr_t plan = 0;
    if (!services.prepare(services.context, character, faery_id, plan, error)) {
        if (plan) services.discard(services.context, plan);
        if (error.empty()) error = "Faery selection preparation was rejected";
        return false;
    }
    if (!plan) {
        error = "Faery selection preparation returned no transaction plan";
        return false;
    }
    if (!services.commit(services.context, plan, error)) {
        services.discard(services.context, plan);
        if (error.empty()) error = "Faery selection transaction commit was rejected";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh2::ui
