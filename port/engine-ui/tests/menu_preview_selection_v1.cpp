#include "../menu_preview_selection_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ui = dh2::ui;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct Fixture {
    std::int32_t* slot{};
    std::vector<std::string> calls;
    bool fail_destroy{};
    bool fail_setup{};
    bool fail_camera{};

    static bool destroy(void* raw, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        self.calls.emplace_back("destroy");
        if (self.fail_destroy) { error = "destroy"; return false; }
        return true;
    }
    static bool setup(void* raw, std::int32_t slot, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        check(self.slot && *self.slot == slot,
              "SetupCharacter did not read the freshly published MainMenu slot");
        self.calls.emplace_back("setup:" + std::to_string(slot));
        if (self.fail_setup) { error = "setup"; return false; }
        return true;
    }
    static bool camera(void* raw, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        self.calls.emplace_back("camera");
        if (self.fail_camera) { error = "camera"; return false; }
        return true;
    }
    ui::MenuPreviewSelectionServicesV1 services() {
        return {this, destroy, setup, camera};
    }
};
} // namespace

int main() {
    try {
        std::string error;
        ui::MenuPreviewSelectionResultV1 result;

        {
            std::int32_t slot = -1;
            Fixture f; f.slot = &slot;
            check(ui::change_menu_preview_slot_v1(&slot, 2, false,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::complete && slot == 2 &&
                      f.calls == std::vector<std::string>{"setup:2", "camera"} &&
                      !result.destroy_attempted && result.slot_stored,
                  "first preview selection did not follow the source no-old-avatar path");
        }
        {
            std::int32_t slot = 1;
            Fixture f; f.slot = &slot;
            check(ui::change_menu_preview_slot_v1(&slot, 1, false,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::complete && f.calls.empty() &&
                      !result.changed && !result.slot_stored,
                  "equal unforced preview selection was not a source no-op");
            check(ui::change_menu_preview_slot_v1(&slot, 1, true,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::complete &&
                      f.calls == std::vector<std::string>{"destroy", "setup:1", "camera"},
                  "forced same-slot preview did not rebuild source owners");
        }
        {
            std::int32_t slot = -1;
            Fixture f; f.slot = &slot;
            check(ui::change_menu_preview_slot_v1(&slot, 4, true,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::complete && slot == 4 &&
                      f.calls == std::vector<std::string>{"setup:4", "camera"},
                  "force on an uninitialized MainMenu slot incorrectly destroyed an avatar");
        }
        {
            std::int32_t slot = 3;
            Fixture f; f.slot = &slot; f.fail_destroy = true;
            check(ui::change_menu_preview_slot_v1(&slot, 0, false,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::destroy_failed && slot == 3 &&
                      f.calls == std::vector<std::string>{"destroy"} &&
                      !result.slot_stored,
                  "destroy failure crossed the source slot-store boundary");
        }
        {
            std::int32_t slot = 3;
            Fixture f; f.slot = &slot; f.fail_setup = true;
            check(ui::change_menu_preview_slot_v1(&slot, 0, false,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::setup_failed && slot == 0 &&
                      f.calls == std::vector<std::string>{"destroy", "setup:0"} &&
                      result.slot_stored && !result.camera_attempted,
                  "SetupCharacter failure rolled back or crossed the camera prefix");
        }
        {
            std::int32_t slot = 3;
            Fixture f; f.slot = &slot; f.fail_camera = true;
            check(ui::change_menu_preview_slot_v1(&slot, 0, false,
                      f.services(), &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::camera_failed && slot == 0 &&
                      f.calls == std::vector<std::string>{"destroy", "setup:0", "camera"} &&
                      result.slot_stored && result.camera_attempted,
                  "camera failure did not preserve the reached source prefix");
        }
        {
            std::int32_t slot = 3;
            Fixture f; f.slot = &slot;
            auto services = f.services(); services.destroy_character = nullptr;
            check(ui::change_menu_preview_slot_v1(&slot, 0, false,
                      services, &result, error) ==
                      ui::MenuPreviewSelectionStatusV1::destroy_failed && slot == 3 &&
                      f.calls.empty() && !result.slot_stored,
                  "missing avatar teardown owner advanced the selected slot");
        }
        std::cout << "menu preview selection host checks passed: 7 source-order cases\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
