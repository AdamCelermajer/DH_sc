#include "generic_creation_host_v1.hpp"
#include "runtime_creation_source_loader_v1.hpp"

#include "../../../asset_catalog.hpp"
#include "../../../save_store.hpp"
#include "../../pause_ui/source_pause_ui_render_v1.hpp"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::frontend;
using namespace dh::foundation::frontend::creation;
using namespace dh::foundation::frontend::flow;

namespace {
void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

bool same_owner(const std::shared_ptr<CharacterState>& a,
                const std::shared_ptr<CharacterState>& b) {
    return a && b && a.get() == b.get() &&
           !a.owner_before(b) && !b.owner_before(a);
}

std::vector<std::uint8_t> bytes(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    check(bool(stream), "expected isolated profile file is missing: " + path.string());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

struct Scratch final {
    std::filesystem::path path;
    Scratch() {
        const auto tick = std::chrono::steady_clock::now().time_since_epoch().count();
        path = std::filesystem::temp_directory_path() /
            ("dh_frontend_create_cancel_remove_v1_" + std::to_string(tick));
        std::filesystem::create_directories(path);
    }
    ~Scratch() {
        std::error_code ignored;
        std::filesystem::remove_all(path, ignored);
    }
};

struct SlotHost final {
    std::filesystem::path root;
    std::array<std::filesystem::path, 4> paths;
    unsigned create_calls{};
    unsigned assignment_calls{};
    unsigned start_calls{};
    unsigned remove_calls{};
    unsigned selected_change_calls{};
    unsigned receipt_serial{};
    std::shared_ptr<CharacterState> caller_state;

    explicit SlotHost(std::filesystem::path directory,
                      std::shared_ptr<CharacterState> state)
        : root(std::move(directory)), caller_state(std::move(state)) {
        for (int slot = 0; slot != 4; ++slot)
            paths[static_cast<std::size_t>(slot)] = root / ("character-slot-" + std::to_string(slot) + ".save");
    }

    bool inspect(int slot, SlotFact& fact, std::string& error) const {
        if (slot < 0 || slot >= static_cast<int>(paths.size())) {
            error = "slot outside authored 0..3 range";
            return false;
        }
        std::error_code ec;
        const bool exists = std::filesystem::exists(paths[static_cast<std::size_t>(slot)], ec);
        if (ec) { error = ec.message(); return false; }
        fact = {slot, exists, paths[static_cast<std::size_t>(slot)]};
        error.clear();
        return true;
    }

    RuntimeCreationFlowServicesV1 services(const RuntimeCreationSourceV1& source) {
        RuntimeCreationFlowServicesV1 out;
        out.shared_state = caller_state;
        out.source = source;
        out.select_new_profile = [this](const std::string& name, const std::string& class_token,
                                        int& selected, RuntimeCreationRequestV1& request,
                                        std::string& error) {
            ++create_calls;
            selected = -1;
            for (int i = 0; i != static_cast<int>(paths.size()); ++i) {
                std::error_code ec;
                if (!std::filesystem::exists(paths[static_cast<std::size_t>(i)], ec) && !ec) {
                    selected = i;
                    break;
                }
                if (ec) { error = ec.message(); return false; }
            }
            if (selected < 0) { error = "no empty authored profile slot"; return false; }
            request.shared_state = caller_state;
            request.character_id = "frontend-regression-" + std::to_string(++receipt_serial);
            request.player_name = name;
            request.class_token = class_token;
            request.save_path = paths[static_cast<std::size_t>(selected)];
            request.source_timer = 0x12345678u + receipt_serial;
            request.saved_date = 0x87654321u + receipt_serial;
            error.clear();
            return true;
        };
        out.selected_save_path = [this](int slot, std::filesystem::path& path, std::string& error) {
            if (slot < 0 || slot >= static_cast<int>(paths.size())) {
                error = "selected slot outside authored range";
                return false;
            }
            path = paths[static_cast<std::size_t>(slot)];
            error.clear();
            return true;
        };
        out.assign_selected_slot = [this](int slot, int player, std::string& error) {
            if (player != 0 || slot < 0 || slot >= static_cast<int>(paths.size()) ||
                !std::filesystem::exists(paths[static_cast<std::size_t>(slot)])) {
                error = "assignment did not name an existing exact local profile slot";
                return false;
            }
            ++assignment_calls;
            error.clear();
            return true;
        };
        out.start_same_state = [this](const std::shared_ptr<CharacterState>& state,
                                      int difficulty, std::string& error) {
            if (!same_owner(state, caller_state) || difficulty != 0) {
                error = "start did not receive the caller-owned state and selected Normal difficulty";
                return false;
            }
            ++start_calls;
            error.clear();
            return true;
        };
        return out;
    }

    Services navigation() {
        Services out;
        out.inspect_slot = [this](int slot, SlotFact& fact, std::string& error) {
            return inspect(slot, fact, error);
        };
        out.select_slot = [this](int current, int direction, SlotFact& fact, std::string& error) {
            if ((direction != -1 && direction != 1) || current < 0 || current > 3 ||
                current + direction < 0 || current + direction > 3) {
                error = "authored arrow has no adjacent slot";
                return false;
            }
            return inspect(current + direction, fact, error);
        };
        out.remove_selected_slot = [this](const SlotFact& selected, std::string& error) {
            if (selected.id < 0 || selected.id > 3 || !selected.in_use ||
                selected.save_path != paths[static_cast<std::size_t>(selected.id)]) {
                error = "removal was not given the exact occupied selected-slot path";
                return false;
            }
            const auto& exact = paths[static_cast<std::size_t>(selected.id)];
            if (!std::filesystem::exists(exact)) { error = "selected save disappeared"; return false; }
            const auto recovery = exact.string() + ".removed-regression-" + std::to_string(++remove_calls);
            std::error_code ec;
            std::filesystem::rename(exact, recovery, ec);
            if (ec) { error = ec.message(); return false; }
            error.clear();
            return true;
        };
        out.selected_profile_changed = [this](const SlotFact&) { ++selected_change_calls; };
        return out;
    }
};

void check_pause_return_route_contract() {
    using namespace dh::foundation::pause_ui;
    const auto click_authored_button = [](SourcePauseUiFrameV1& frame,
                                          std::string_view suffix) {
        for (const auto& region : frame.art.hit_regions) {
            if (region.button_path.size() < suffix.size() ||
                region.button_path.compare(region.button_path.size() - suffix.size(),
                                           suffix.size(), suffix) != 0 ||
                region.triangles.empty()) continue;
            float x = 0.0f, y = 0.0f;
            for (const auto& vertex : region.triangles) { x += vertex.x; y += vertex.y; }
            x /= static_cast<float>(region.triangles.size());
            y /= static_cast<float>(region.triangles.size());
            const auto hit = source_pause_ui_hit_test_v1(frame, x, y);
            if (hit.hit) return hit;
        }
        return SourcePauseUiHitV1{};
    };
    auto pause = source_pause_ui_frame_v1(SourcePauseSurfaceV1::pause_page);
    const auto request = source_pause_ui_route_v1(
        pause, click_authored_button(pause, "btn_MENU_MAIN_MENU"));
    check(request.kind == SourcePauseRouteKindV1::open_main_menu_confirmation && request.supported,
          "authored pause Main Menu click did not request its confirmation page");
    auto confirmation = source_pause_ui_frame_v1(SourcePauseSurfaceV1::confirmation);
    const auto accepted = source_pause_ui_route_v1(
        confirmation, click_authored_button(confirmation, "btn_yes"));
    check(accepted.kind == SourcePauseRouteKindV1::return_to_main_menu && accepted.supported,
          "authored confirmation Yes did not produce the root-callable return-to-menu route");
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "pass the Android asset root or original-cache asset root");
        AssetCatalog assets(argv[1]);
        dh2::data::LootRandom8V2 caller_random{0x24681357u, 4};
        RuntimeCreationSourceOwnerV1 source_owner;
        std::string error;
        check(load_runtime_creation_source_v1(assets, caller_random, source_owner, error),
              "actual original creation source load failed: " + error);
        check(source_owner.valid() && source_owner.source_hashes.size() == 13,
              "source profile test requires the complete 13-input actual source snapshot");
        check_pause_return_route_contract();

        Scratch scratch;
        auto state = std::make_shared<CharacterState>();
        state->id = "caller-state-before-creation";
        const auto* state_pointer = state.get();
        auto state_owner_witness = state;
        SlotHost slots(scratch.path, state);
        auto creation_services = slots.services(source_owner.source());
        auto host = std::make_shared<GenericCreationFrontendHostV1>(FrontendRuntimeServicesV1{},
                                                                    std::move(creation_services));
        // The host owns the adapter internally; behavior is observed through the real Navigator
        // and GenericCreationFrontendResult contract instead of a parallel create/start fake.
        auto navigation = host->runtime_services().generic_creation;
        check(bool(navigation), "generic creation host did not bind its existing creation adapter");
        auto services = slots.navigation();
        auto bound = navigation;
        auto bound_services = bound->bind_navigation(std::move(services));
        Navigator nav(std::move(bound_services));

        SlotFact slot0;
        check(slots.inspect(0, slot0, error), error);
        check(!slot0.in_use && !std::filesystem::exists(slot0.save_path),
              "isolated test slot0 was not fresh at start");

        // Empty-slot Main -> EnterName -> SelectClass, then Back twice (cancel).
        // Source actions are the sprite510 empty-slot push and sprite93 Accept push;
        // neither Back release may invoke NativeCreateSaveSlot or mutate the slot.
        check(nav.single_player(slot0, error), error);
        check(nav.top() == "menu_EnterName", "empty slot did not enter authored name state");
        check(nav.accept_name("CanceledAttempt", error), error);
        check(nav.select_class(1, error), error);
        check(nav.back(error) && nav.top() == "menu_EnterName", error);
        check(nav.back(error) && nav.top() == "menu_MainMenu", error);
        check(slots.create_calls == 0 && slots.assignment_calls == 0 && slots.start_calls == 0 &&
              !std::filesystem::exists(slot0.save_path) && state->id == "caller-state-before-creation" &&
              nav.current_slot() == 0,
              "Back/cancel mutated the profile slot, caller state, or source operation counts");

        // First actual source-backed creation: Confirm persists/loads into the same state,
        // performs the authored assignment, and stops at StartGame; the source StartGame
        // release assigns again and then calls the same-state start provider.
        check(slots.inspect(0, slot0, error) && nav.single_player(slot0, error), error);
        check(nav.accept_name("Rogue One", error), error);
        check(nav.select_class(1, error), error);
        check(nav.confirm_class(error), error);
        check(nav.top() == "menu_StartGame" && slots.create_calls == 1 &&
              slots.assignment_calls == 1 && slots.start_calls == 0 &&
              std::filesystem::exists(slots.paths[0]) && state->name == "Rogue One" &&
              state->class_id == "RoguePlayerBase" && state.get() == state_pointer &&
              same_owner(state, state_owner_witness),
              "first Confirm did not persist the selected source profile into the same caller state");
        const auto first_profile_bytes = bytes(slots.paths[0]);
        check(!first_profile_bytes.empty(), "source save emitted an empty profile file");
        check(nav.confirm_class(error) && slots.create_calls == 1,
              "repeated class Confirm duplicated the first source profile");
        check(nav.start_game(0, error), error);
        check(slots.start_calls == 1 && slots.assignment_calls == 2 &&
              nav.start_delivered() && state.get() == state_pointer,
              "first authored StartGame release did not deliver on the same state");

        // Source back from StartGame returns to MainMenu without changing the selected path.
        check(nav.back(error) && nav.top() == "menu_MainMenu" && nav.current_slot() == 0,
              "StartGame Back did not return to the same selected Main-menu slot");
        check(nav.begin_remove_selected(error) && nav.erase_confirmation(), error);
        check(nav.resolve_remove_selected(false, error), error);
        check(!nav.erase_confirmation() && std::filesystem::exists(slots.paths[0]) &&
              bytes(slots.paths[0]) == first_profile_bytes && slots.remove_calls == 0 &&
              nav.selected_slot_fact().save_path == slots.paths[0] && nav.selected_slot_fact().in_use,
              "removal refusal changed selected-slot state or profile bytes");

        // Accepted source confirmation uses recoverable same-directory rename. The canonical
        // selected-slot path remains slot0's path and becomes empty; no slot is shifted.
        const auto selected_path = nav.selected_slot_fact().save_path;
        check(nav.begin_remove_selected(error) && nav.resolve_remove_selected(true, error), error);
        check(nav.selected_slot_fact().id == 0 && !nav.selected_slot_fact().in_use &&
              nav.selected_slot_fact().save_path == selected_path &&
              !std::filesystem::exists(selected_path) && slots.remove_calls == 1,
              "accepted removal did not retain the canonical empty slot identity");
        const auto recovery = std::filesystem::path(selected_path.string() + ".removed-regression-1");
        check(std::filesystem::exists(recovery) && bytes(recovery) == first_profile_bytes,
              "accepted removal did not preserve the exact source save in its recovery sibling");
        for (int i = 1; i != 4; ++i)
            check(!std::filesystem::exists(slots.paths[static_cast<std::size_t>(i)]),
                  "remove/create flow accidentally mutated a neighboring slot");

        // Repeat on the same canonical empty path. New selected profile is loaded and started
        // only from that path; the same shared CharacterState owner remains authoritative.
        check(nav.single_player(nav.selected_slot_fact(), error), error);
        check(nav.top() == "menu_EnterName" && nav.accept_name("Knight Two", error), error);
        check(nav.select_class(0, error), error);
        check(nav.confirm_class(error), error);
        check(nav.top() == "menu_StartGame" && slots.create_calls == 2 &&
              slots.assignment_calls == 3 && slots.start_calls == 1 &&
              state->name == "Knight Two" && state->class_id == "KnightPlayerBase" &&
              state.get() == state_pointer && same_owner(state, state_owner_witness) &&
              nav.selected_slot_fact().id == 0 && nav.selected_slot_fact().save_path == selected_path,
              "repeat creation changed slot identity or replaced the caller's profile state");
        check(nav.start_game(0, error), error);
        const auto start_receipt = bound->start_receipt();
        check(start_receipt && start_receipt->valid_for(state_owner_witness, 0) &&
              start_receipt->created_in_this_flow && start_receipt->save_path == selected_path &&
              slots.create_calls == 2 && slots.assignment_calls == 4 && slots.start_calls == 2 &&
              std::filesystem::exists(selected_path) && bytes(recovery) == first_profile_bytes,
              "repeat StartGame did not carry the same selected slot and CharacterState identity");
        FrontendRuntimeResultV1 runtime_result;
        runtime_result.outcome = FrontendRuntimeOutcomeV1::generic_gameplay_started;
        runtime_result.selected_slot = start_receipt->selected_slot;
        runtime_result.generic_start_receipt = start_receipt;
        const auto packaged = host->complete(std::move(runtime_result));
        check(packaged.gameplay_started_for(state_owner_witness, 0) &&
              packaged.launch && packaged.launch->save_path == selected_path &&
              packaged.shared_state.get() == state_pointer,
              "generic host did not package the actual same-state selected-slot start receipt");

        std::cout << "profile_create_cancel_remove_regression_v1: PASS\n"
                  << "pause route: authored Main Menu confirmation Yes -> root return_to_main_menu\n"
                  << "cancel: no slot/state mutation; first save byte-count=" << first_profile_bytes.size() << '\n'
                  << "remove: refusal preserved exact bytes; accepted rename recovery=" << recovery.filename().string() << '\n'
                  << "repeat: slot=0 creates=" << slots.create_calls << " assignments=" << slots.assignment_calls
                  << " starts=" << slots.start_calls << " same-state-owner=1\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "profile_create_cancel_remove_regression_v1: FAIL: " << ex.what() << '\n';
        return 1;
    }
}
