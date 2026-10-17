#include "../swf_cursor_input.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

using namespace dh2::ui;

namespace {
struct Fixture {
    std::uintptr_t root{0x2000};
    std::uintptr_t context{0x1000};
    std::uintptr_t hit_root{};
    std::uintptr_t hit_target{};
    const char* context_name{};
    const char* release_target{};
};

int invoke(void* raw, SwfInputState288*, const SwfInputRequest64* request,
           SwfInputResponse56* response) {
    auto& fixture = *static_cast<Fixture*>(raw);
    switch (request->operation) {
    case SwfInputOperation::character: {
        static const SwfInputCharacter32 hud{"menu_HUD_0", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 ingame{"menu_Ingame", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 main{"menu_MainMenu", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 character_menu{"menu_CharacterMenu", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 faery_sheet{"menu_FaerySheet", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 inventory_sheet{"menu_InventorySheetMain", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 parent_tab{"btnFaeriesTab", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 faery_button1{"btn_GAMEPLAYMENUS_FAERY_1", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 faery_button{"btn_GAMEPLAYMENUS_FAERY_2", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 inventory_row{"btnInventoryRow_0", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 continue_button{"btn_MENU_CONTINUE", 1, 1, 0, 1, 0, {}};
        static const SwfInputCharacter32 blocker{"btnBlocker", 1, 1, 0, 1, 0, {}};
        if (request->character == 0x7ab1u) response->character = &parent_tab;
        else if (request->character == 0xfae1u) response->character = &faery_button1;
        else if (request->character == 0xfae2u) response->character = &faery_button;
        else if (request->character == 0x1a11u) response->character = &inventory_row;
        else if (request->character == 0xc071u) response->character = &continue_button;
        else if (request->character == 0xb10cu) response->character = &blocker;
        else response->character = std::strcmp(fixture.context_name, hud.name) == 0 ? &hud :
                                    std::strcmp(fixture.context_name, ingame.name) == 0 ? &ingame :
                                    std::strcmp(fixture.context_name, main.name) == 0 ? &main :
                                    std::strcmp(fixture.context_name, faery_sheet.name) == 0 ? &faery_sheet :
                                    std::strcmp(fixture.context_name, inventory_sheet.name) == 0 ? &inventory_sheet :
                                    &character_menu;
        return 1;
    }
    case SwfInputOperation::root_movie:
        response->identity = fixture.root;
        return 1;
    case SwfInputOperation::topmost:
        fixture.hit_root = request->character;
        // The actual dqhud movie has a root-level confirmation blocker over
        // menu_Ingame. Its Continue child is reachable only when the pause
        // modal owns the hit-test scope.
        if (std::strcmp(fixture.context_name, "menu_Ingame") == 0 &&
            request->values[0] > 4500.f && request->values[0] < 5100.f &&
            request->values[1] > 1400.f && request->values[1] < 1900.f) {
            response->identity = request->character == fixture.root ? 0xb10cu : 0xc071u;
            fixture.hit_target = response->identity;
            return 1;
        }
        // Model the authored root display list: the CharacterMenu tab is a
        // lower-depth sibling; the visible gameplay sheet is above it and
        // resolves its own active button in the same root hit query.
        if (request->character == fixture.root && request->values[1] < 800.f &&
            request->values[0] > 6000.f)
            response->identity = 0x7ab1u;
        else if (request->character == fixture.root && request->values[1] >= 800.f &&
                 std::strcmp(fixture.context_name, "menu_InventorySheetMain") == 0)
            response->identity = 0x1a11u;
        else if (request->character == fixture.root && request->values[1] >= 800.f &&
                 std::strcmp(fixture.context_name, "menu_CharacterMenu") != 0)
            response->identity = request->values[0] < 2500.f ? 0xfae1u : 0xfae2u;
        else response->identity = 0;
        fixture.hit_target = response->identity;
        return 1;
    case SwfInputOperation::screen_to_logical:
        response->values[0] = request->values[0];
        response->values[1] = request->values[1];
        return 1;
    case SwfInputOperation::local_position:
        response->values[0] = 0.f;
        response->values[1] = 0.f;
        return 1;
    case SwfInputOperation::can_handle_event:
        response->result = 1;
        return 1;
    case SwfInputOperation::native_event:
        if (request->event && request->event->kind == 6)
            fixture.release_target = request->event->name;
        return 1;
    case SwfInputOperation::as_method:
        return 1;
    case SwfInputOperation::play_animation:
        response->result = 0;
        return 1;
    case SwfInputOperation::retain:
    case SwfInputOperation::drop:
    case SwfInputOperation::publish_raw_cursor:
    case SwfInputOperation::notify_mouse_state:
        return 1;
    default:
        return 0;
    }
}

Fixture resolve_hit_context(const char* context_name) {
    Fixture fixture;
    fixture.context_name = context_name;
    SwfInputState288 state{};
    state.root = fixture.root;
    state.context = fixture.context;
    state.flags = 0x84;
    for (auto& slot : state.slots) slot.enabled = 1;
    const SwfInputServices16 services{&fixture, invoke};
    std::uint32_t selection{};
    const SwfCursor16 cursor{82.f, 70.f, 0.f, 0};
    assert(dh2_ui_swf_update_cursor(&state, &cursor, 0, &selection, &services) == 0);
    return fixture;
}

void verify_root_sibling_click(const char* context_name, float x, float y,
                               std::uintptr_t target, const char* target_name) {
    Fixture fixture;
    fixture.context_name = context_name;
    SwfInputState288 state{};
    state.root = fixture.root;
    state.context = fixture.context;
    state.flags = 0x84;
    for (auto& slot : state.slots) slot.enabled = 1;
    const SwfInputServices16 services{&fixture, invoke};
    std::uint32_t selection{};
    const SwfCursor16 down{x, y, 0.f, 1};
    const SwfCursor16 up{x, y, 0.f, 0};
    assert(dh2_ui_swf_update_cursor(&state, &down, 0, &selection, &services) == 0);
    assert(dh2_ui_swf_update_cursor(&state, &up, 0, &selection, &services) == 0);
    assert(fixture.hit_target == target);
    assert(fixture.release_target &&
           std::strcmp(fixture.release_target, target_name) == 0);
}

void verify_root_inventory_row_click() {
    Fixture fixture;
    fixture.context_name = "menu_InventorySheetMain";
    SwfInputState288 state{};
    state.root = fixture.root;
    state.context = fixture.context;
    state.flags = 0x84;
    for (auto& slot : state.slots) slot.enabled = 1;
    const SwfInputServices16 services{&fixture, invoke};
    std::uint32_t selection{};
    const SwfCursor16 down{163.f, 83.f, 0.f, 1};
    const SwfCursor16 up{163.f, 83.f, 0.f, 0};
    assert(dh2_ui_swf_update_cursor(&state, &down, 0, &selection, &services) == 0);
    assert(dh2_ui_swf_update_cursor(&state, &up, 0, &selection, &services) == 0);
    assert(fixture.hit_root == fixture.root);
    assert(fixture.hit_target == 0x1a11u);
    assert(fixture.release_target &&
           std::strcmp(fixture.release_target, "btnInventoryRow_0") == 0);
}

void verify_pause_continue_click_uses_modal_scope() {
    Fixture fixture;
    fixture.context_name = "menu_Ingame";
    SwfInputState288 state{};
    state.root = fixture.root;
    state.context = fixture.context;
    state.flags = 0x84;
    for (auto& slot : state.slots) slot.enabled = 1;
    const SwfInputServices16 services{&fixture, invoke};
    std::uint32_t selection{};
    // API 37 screenshot tap (1200,274) maps through the fitted viewport to
    // logical (240,81.2), or SWF twips (4800,1624).
    const SwfCursor16 down{240.f, 81.2f, 0.f, 1};
    const SwfCursor16 up{240.f, 81.2f, 0.f, 0};
    assert(dh2_ui_swf_update_cursor(&state, &down, 0, &selection, &services) == 0);
    assert(dh2_ui_swf_update_cursor(&state, &up, 0, &selection, &services) == 0);
    assert(fixture.hit_root == fixture.context);
    assert(fixture.hit_target == 0xc071u);
    assert(fixture.release_target &&
           std::strcmp(fixture.release_target, "btn_MENU_CONTINUE") == 0);
}

void verify_independent_multitouch_cursors() {
    Fixture fixture;
    fixture.context_name = "menu_CharacterMenu";
    SwfInputState288 state{};
    state.root = fixture.root;
    state.context = 0x1000;
    state.flags = 0x84;
    for (auto& slot : state.slots) slot.enabled = 1;
    const SwfInputServices16 services{&fixture, invoke};
    std::uint32_t selection{};
    const SwfCursor16 first{20.f, 30.f, 0.f, 1};
    const SwfCursor16 second{70.f, 90.f, 0.f, 1};
    assert(dh2_ui_swf_update_cursor(&state, &first, 0, &selection, &services) == 0);
    assert(dh2_ui_swf_update_cursor(&state, &second, 1, &selection, &services) == 0);
    assert(state.slots[0].cursor.x == 20.f && state.slots[0].cursor.y == 30.f);
    assert(state.slots[1].cursor.x == 70.f && state.slots[1].cursor.y == 90.f);
    assert(state.slots[2].cursor.buttons == 0 && state.slots[3].cursor.buttons == 0);
    assert(dh2_ui_swf_update_cursor(&state, &second, 4, &selection, &services) == -1);
}
}

int main() {
    const auto hud = resolve_hit_context("menu_HUD_0");
    assert(hud.hit_root == hud.context);
    const auto main = resolve_hit_context("menu_MainMenu");
    assert(main.hit_root == main.root);
    const auto character_menu = resolve_hit_context("menu_CharacterMenu");
    assert(character_menu.hit_root == character_menu.root);
    const auto faery = resolve_hit_context("menu_FaerySheet");
    assert(faery.hit_root == faery.root);
    assert(faery.hit_target == 0xfae1u);
    verify_root_sibling_click("menu_FaerySheet", 87.f, 83.f, 0xfae1u,
                              "btn_GAMEPLAYMENUS_FAERY_1");
    verify_root_sibling_click("menu_FaerySheet", 166.f, 83.f, 0xfae2u,
                              "btn_GAMEPLAYMENUS_FAERY_2");
    verify_root_sibling_click("menu_CharacterSheetStats", 350.f, 15.f, 0x7ab1u,
                              "btnFaeriesTab");
    verify_root_inventory_row_click();
    verify_pause_continue_click_uses_modal_scope();
    verify_independent_multitouch_cursors();
    std::cout << "HUD local scope plus root sibling hit testing routes inventory, Faery and parent-tab clicks\n";
}
