#include "menu_preview_selection_v1.hpp"

namespace dh2::ui {

MenuPreviewSelectionStatusV1 change_menu_preview_slot_v1(
    std::int32_t* slot, std::int32_t requested_slot, bool force,
    const MenuPreviewSelectionServicesV1& services,
    MenuPreviewSelectionResultV1* result, std::string& error) noexcept {
    error.clear();
    if (!result) {
        error = "Menu preview selection result is required";
        return MenuPreviewSelectionStatusV1::invalid_argument;
    }
    *result = {};
    if (!slot || !services.setup_character || !services.create_avatar_camera) {
        error = "MainMenu slot and preview setup/camera owners are required";
        result->status = MenuPreviewSelectionStatusV1::invalid_argument;
        return result->status;
    }

    result->old_slot = *slot;
    if (*slot == requested_slot && !force) {
        result->status = MenuPreviewSelectionStatusV1::complete;
        return result->status;
    }

    if (*slot != -1) {
        if (!services.destroy_character) {
            error = "Avatar-only MainMenu Character teardown owner is unavailable";
            result->status = MenuPreviewSelectionStatusV1::destroy_failed;
            return result->status;
        }
        result->destroy_attempted = true;
        if (!services.destroy_character(services.context, error)) {
            if (error.empty()) error = "MainMenu preview Character teardown failed";
            result->status = MenuPreviewSelectionStatusV1::destroy_failed;
            return result->status;
        }
    }

    *slot = requested_slot;
    result->changed = true;
    result->slot_stored = true;
    result->setup_attempted = true;
    if (!services.setup_character(services.context, *slot, error)) {
        if (error.empty()) error = "MainMenu preview Character setup failed";
        result->status = MenuPreviewSelectionStatusV1::setup_failed;
        return result->status;
    }

    result->camera_attempted = true;
    if (!services.create_avatar_camera(services.context, error)) {
        if (error.empty()) error = "MainMenu avatar camera setup failed";
        result->status = MenuPreviewSelectionStatusV1::camera_failed;
        return result->status;
    }
    result->status = MenuPreviewSelectionStatusV1::complete;
    return result->status;
}

} // namespace dh2::ui
