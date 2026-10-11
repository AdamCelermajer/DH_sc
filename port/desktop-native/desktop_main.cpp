#include "android/asset_manager.h"
#include "gameplay_audio_sdl.hpp"
#include "model_renderer.hpp"
#include "original_ui_session.hpp"
#include "../android-native/app/src/main/cpp/menu_start_commit_gate_v1.hpp"
#include "../game-data/campaign_profile_files_v1.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace {
struct Options {
    std::filesystem::path assets;
    std::filesystem::path runtime;
    std::filesystem::path capture;
    std::filesystem::path initial_capture;
    std::uint64_t frames = 0;
    std::int32_t level_row = -1;
    bool movement_attack_scenario = false;
    bool fixed_cultist_scenario = false;
    bool fixed_cultist_kill_scenario = false;
};

Options parse(int argc, char** argv) {
    Options options;
#ifdef DH2_DEFAULT_ASSET_DIR
    options.assets = std::filesystem::u8path(DH2_DEFAULT_ASSET_DIR);
#else
    options.assets = std::filesystem::current_path() / "port/android-native/app/src/main/assets";
#endif
    bool frames_supplied = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        auto value = [&](const char* name) -> std::string {
            if (++i >= argc) throw std::runtime_error(std::string("Missing value for ") + name);
            return argv[i];
        };
        if (arg == "--assets") options.assets = value("--assets");
        else if (arg == "--runtime-dir") options.runtime = value("--runtime-dir");
        else if (arg == "--level-row") {
            const auto row = value("--level-row");
            std::size_t parsed = 0;
            const auto number = std::stoll(row, &parsed);
            if (parsed != row.size() || number < 0 ||
                number > std::numeric_limits<std::int32_t>::max())
                throw std::runtime_error("--level-row must be a nonnegative 32-bit integer");
            options.level_row = static_cast<std::int32_t>(number);
            if (options.level_row != 23 && options.level_row != 41)
                throw std::runtime_error("--level-row currently supports direct Crypt 23 or fresh-save SWAMP 41");
        }
        else if (arg == "--capture") options.capture = value("--capture");
        else if (arg == "--capture-initial") options.initial_capture = value("--capture-initial");
        else if (arg == "--scenario") {
            const auto scenario = value("--scenario");
            if (scenario == "move-attack") options.movement_attack_scenario = true;
            else if (scenario == "fixed-cultist07") options.fixed_cultist_scenario = true;
            else if (scenario == "fixed-cultist07-kill") options.fixed_cultist_kill_scenario = true;
            else throw std::runtime_error("Unknown scenario: " + scenario);
        }
        else if (arg == "--frames") {
            const auto count = value("--frames");
            std::size_t parsed = 0;
            options.frames = std::stoull(count, &parsed);
            if (parsed != count.size() || options.frames == 0)
                throw std::runtime_error("--frames must be a positive integer");
            frames_supplied = true;
        }
        else if (arg == "--help") {
            std::cout << "DH2 desktop runtime\n"
                         "  --assets DIR       Android app asset tree\n"
                         "  --runtime-dir DIR  save/config output (default per-user AppData)\n"
                         "  --level-row N      skip the menu and start source row 23 or 41 directly\n"
                         "  --frames N         exit after N rendered UI/gameplay frames\n"
                         "  --capture FILE.ppm save the last framebuffer as a PPM image\n"
                         "  --capture-initial FILE.ppm save the initial frame (for gameplay scenarios)\n"
                         "  --scenario move-attack steer with normal movement toward a source enemy, then attack\n"
                         "  --scenario fixed-cultist07 route to Crypt enemy 4; attack when nearest and in melee range\n"
                         "  --scenario fixed-cultist07-kill route to Crypt enemy 4; repeat normal attacks until death or frame bound\n"
                         "Controls: click/touch authored menus; type for character names; WASD move, Space attack, mouse drag orbit, wheel zoom, Esc quit.\n";
            std::exit(0);
        } else throw std::runtime_error("Unknown argument: " + arg);
    }
    const auto scenario_count = unsigned(options.movement_attack_scenario) +
        unsigned(options.fixed_cultist_scenario) + unsigned(options.fixed_cultist_kill_scenario);
    if (scenario_count > 1) throw std::runtime_error("Select only one gameplay scenario per run");
    if (options.movement_attack_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (!frames_supplied) options.frames = 300;
        if (options.frames < 4) throw std::runtime_error("move-attack requires at least four gameplay frames");
    } else if (options.fixed_cultist_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (options.level_row != 23) throw std::runtime_error("fixed-cultist07 requires Crypt row 23");
        if (!frames_supplied) options.frames = 2400;
        if (options.frames < 4) throw std::runtime_error("fixed-cultist07 requires at least four gameplay frames");
    } else if (options.fixed_cultist_kill_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (options.level_row != 23) throw std::runtime_error("fixed-cultist07-kill requires Crypt row 23");
        if (!frames_supplied) options.frames = 6000;
        if (options.frames < 4) throw std::runtime_error("fixed-cultist07-kill requires at least four gameplay frames");
    } else if (!options.initial_capture.empty()) {
        throw std::runtime_error("--capture-initial requires a gameplay scenario");
    }
    return options;
}

void update_movement(const std::array<bool, 4>& keys) {
    const float x = (keys[3] ? 1.0f : 0.0f) - (keys[2] ? 1.0f : 0.0f);
    const float y = (keys[0] ? 1.0f : 0.0f) - (keys[1] ? 1.0f : 0.0f);
    const float scale = x != 0 && y != 0 ? 0.70710678f : 1.0f;
    model_renderer::move_axis(x * scale, y * scale);
}

void print_vitals() {
    const auto values = model_renderer::player_vitals();
    std::cout << "HP " << values[0] / 256.0f << '/' << values[1] / 256.0f
              << "  MP " << values[2] / 256.0f << '/' << values[3] / 256.0f
              << (values[4] ? "  defeated" : "") << '\n';
}

void print_vitals_snapshot(const char* label, const std::array<int, 7>& values) {
    std::cout << label << " HP " << values[0] / 256.0f << '/' << values[1] / 256.0f
              << " (" << (values[0] - values[1]) / 256.0f << " from max)"
              << " MP " << values[2] / 256.0f << '/' << values[3] / 256.0f
              << " state " << values[6] << (values[4] ? " defeated" : " alive") << '\n';
}

struct CombatReceiptSnapshot {
    std::array<int, 7> vitals{};
    model_renderer::UiPlayerStatsReadV1 stats;
    std::int32_t gold{};
    std::map<std::int32_t, std::int64_t> inventory_by_item;
};

bool read_combat_receipt(CombatReceiptSnapshot& output, std::string& error) {
    std::uintptr_t player = 0;
    if (!model_renderer::ui_player_identity(player, error)) return false;
    CombatReceiptSnapshot candidate;
    candidate.vitals = model_renderer::player_vitals();
    if (!model_renderer::ui_player_stats(player, candidate.stats, error) ||
        !model_renderer::ui_player_inventory_gold(player, candidate.gold, error)) return false;
    for (std::int32_t slot = 0; slot <= 9; ++slot) {
        std::vector<model_renderer::UiInventoryItemReadV1> items;
        if (!model_renderer::ui_player_inventory_slot(player, slot, items, error)) return false;
        for (const auto& item : items) candidate.inventory_by_item[item.id] += item.quantity;
    }
    output = std::move(candidate);
    error.clear();
    return true;
}

void print_combat_receipt(const char* label, const CombatReceiptSnapshot& value) {
    std::cout << label << " HP " << value.vitals[0] / 256.0f
              << " | XP " << value.stats.xp << " | level " << value.stats.level
              << " | gold " << value.gold << " | inventory item IDs "
              << value.inventory_by_item.size() << " (UI slots 0..9)\n";
}

void print_combat_receipt_delta(const CombatReceiptSnapshot& before,
                                const CombatReceiptSnapshot& after) {
    auto items = after.inventory_by_item;
    for (const auto& [id, quantity] : before.inventory_by_item) items[id] -= quantity;
    for (auto it = items.begin(); it != items.end();) {
        if (it->second == 0) it = items.erase(it);
        else ++it;
    }
    std::cout << "Combat receipt delta | HP "
              << (after.vitals[0] - before.vitals[0]) / 256.0f
              << " | XP " << (after.stats.xp - before.stats.xp)
              << " | level " << before.stats.level << " -> " << after.stats.level
              << " | gold " << (after.gold - before.gold) << " | item deltas";
    if (items.empty()) std::cout << " none";
    for (const auto& [id, quantity] : items) std::cout << ' ' << id << ':' << quantity;
    std::cout << " (UI slots 0..9)\n";
}

bool steer_toward_nearest_enemy() {
    model_renderer::GameplayPoseSnapshotV1 pose;
    std::string error;
    if (!model_renderer::gameplay_pose_snapshot(pose, error))
        throw std::runtime_error("Shared gameplay pose snapshot failed: " + error);
    model_renderer::GameplayRouteSnapshotV1 route;
    if (!model_renderer::gameplay_route_snapshot(pose.enemy_position, route, error))
        throw std::runtime_error("Selected-floor route query unavailable: " + error);
    std::cout << "Scenario selected-floor route | sewn " << route.graph_sewn
              << " | nodes " << route.graph_nodes << " | edges " << route.graph_edges
              << " | status " << route.query_status << " | found " << route.path_found
              << " | node pair " << route.source_node << " -> " << route.target_node
              << " | waypoints " << route.waypoints.size()
              << " | start " << pose.player_position[0] << ',' << pose.player_position[1]
              << ',' << pose.player_position[2] << " | enemy " << pose.enemy_position[0]
              << ',' << pose.enemy_position[1] << ',' << pose.enemy_position[2] << '\n';
    if (route.query_status != 0 || !route.path_found || route.waypoints.empty())
        throw std::runtime_error("Selected-floor route is unavailable; refusing direct steering fallback");
    const auto& waypoint = route.waypoints.front();
    const float dx = waypoint[0] - pose.player_position[0];
    const float dy = waypoint[1] - pose.player_position[1];
    const float distance = std::hypot(dx, dy);
    if (!std::isfinite(distance))
        throw std::runtime_error("Selected-floor route produced a non-finite waypoint");
    if (distance <= 48.0f) {
        model_renderer::move_axis(0.0f, 0.0f);
        return true;
    }
    std::cout << "Scenario steering toward source enemy " << pose.enemy_index
              << " at " << pose.enemy_position[0] << ',' << pose.enemy_position[1]
              << " HP " << pose.enemy_hp << '/' << pose.enemy_max_hp
              << " via waypoint " << waypoint[0] << ',' << waypoint[1] << '\n';
    // move_axis accepts Android MovementControl screen axes. Invert its
    // recovered fixed +45-degree basis for this selected-floor world vector.
    constexpr float inverse_sqrt_two = 0.7071067811865475f;
    model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                              (dy - dx) * inverse_sqrt_two / distance);
    return true;
}

bool steer_toward_fixed_position(const std::array<float, 3>& target,
                                 std::uint32_t& graph_nodes,
                                 std::uint32_t& graph_edges,
                                 std::uint32_t& query_status,
                                 bool& path_found,
                                 std::uint32_t& waypoint_count) {
    model_renderer::GameplayPoseSnapshotV1 pose;
    std::string error;
    if (!model_renderer::gameplay_pose_snapshot(pose, error))
        throw std::runtime_error("Shared fixed-target pose snapshot failed: " + error);
    model_renderer::GameplayRouteSnapshotV1 route;
    if (!model_renderer::gameplay_route_snapshot(target, route, error))
        throw std::runtime_error("Fixed-target selected-floor route query unavailable: " + error);
    graph_nodes = route.graph_nodes;
    graph_edges = route.graph_edges;
    query_status = route.query_status;
    path_found = route.path_found;
    waypoint_count = static_cast<std::uint32_t>(route.waypoints.size());
    std::cout << "Fixed target route | sewn " << route.graph_sewn
              << " | nodes " << route.graph_nodes << " | edges " << route.graph_edges
              << " | status " << route.query_status << " | found " << route.path_found
              << " | node pair " << route.source_node << " -> " << route.target_node
              << " | waypoints " << route.waypoints.size()
              << " | start " << pose.player_position[0] << ',' << pose.player_position[1]
              << ',' << pose.player_position[2] << " | target " << target[0] << ','
              << target[1] << ',' << target[2] << '\n';
    if (route.query_status != 0 || !route.path_found || route.waypoints.empty())
        return false;
    const auto& waypoint = route.waypoints.front();
    const float dx = waypoint[0] - pose.player_position[0];
    const float dy = waypoint[1] - pose.player_position[1];
    const float distance = std::hypot(dx, dy);
    if (!std::isfinite(distance))
        throw std::runtime_error("Fixed-target route produced a non-finite waypoint");
    if (distance <= 48.0f) {
        model_renderer::move_axis(0.0f, 0.0f);
        return true;
    }
    constexpr float inverse_sqrt_two = 0.7071067811865475f;
    model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                              (dy - dx) * inverse_sqrt_two / distance);
    return true;
}

struct PlayerUiFrame {
    dh2::android_ui::OriginalUiSession* session{};
    int width = 1;
    int height = 1;
};

bool draw_player_ui(void* context, const std::int32_t* sheet, std::size_t count,
                    std::uintptr_t character, std::string& error) {
    auto& frame = *static_cast<PlayerUiFrame*>(context);
    return frame.session->render_player(frame.width, frame.height, sheet, count, character, error);
}

bool dispatch_touch(dh2::android_ui::OriginalUiSession& session, SDL_Window* window,
                   int x, int y, int action, std::string& error) {
    int logical_width = 1, logical_height = 1, drawable_width = 1, drawable_height = 1;
    SDL_GetWindowSize(window, &logical_width, &logical_height);
    SDL_GetWindowSizeInPixels(window, &drawable_width, &drawable_height);
    const float sx = float(drawable_width) / std::max(1, logical_width);
    const float sy = float(drawable_height) / std::max(1, logical_height);
    return session.touch(float(x) * sx, float(y) * sy, action, error);
}

bool dispatch_finger_touch(dh2::android_ui::OriginalUiSession& session, SDL_Window* window,
                           float normalized_x, float normalized_y, int action,
                           std::string& error) {
    int width = 1, height = 1;
    SDL_GetWindowSize(window, &width, &height);
    const auto x = static_cast<int>(std::lround(std::clamp(normalized_x, 0.0f, 1.0f) * width));
    const auto y = static_cast<int>(std::lround(std::clamp(normalized_y, 0.0f, 1.0f) * height));
    return dispatch_touch(session, window, x, y, action, error);
}

bool desktop_difficulty_count(void* context, std::uint32_t* count, std::string& error) {
    if (!context || !count) {
        error = "Desktop NativeCreateSaveSlot difficulty service is unavailable";
        return false;
    }
    return static_cast<dh2::android_ui::OriginalUiSession*>(context)
        ->game_difficulty_count(*count, error);
}

struct FrontRuntimeContext {
    AAssetManager* assets{};
    dh2::android_ui::OriginalUiSession* ui{};
};

void bind_desktop_front_runtime(dh2::android_ui::OriginalUiSession& ui,
                                FrontRuntimeContext& context) {
    dh2::android_ui::FrontRuntimeServices services{};
    services.context = &context;
    services.create_save_slot = [](void* raw, const std::string& name,
            const std::string& character, std::int32_t& slot,
            bool& publish_result, std::string& error) {
        auto& owner = *static_cast<FrontRuntimeContext*>(raw);
        if (!owner.assets || !owner.ui) {
            error = "Desktop NativeCreateSaveSlot owners are unavailable";
            return false;
        }
        return model_renderer::create_menu_save_slot(owner.assets, name, character,
            slot, publish_result, error, owner.ui, desktop_difficulty_count);
    };
    services.assign_save_slot = [](void*, std::int32_t slot, std::int32_t ordinal,
                                   std::string& error) {
        return model_renderer::assign_menu_save_slot(slot, ordinal, error);
    };
    services.eabi_integer = [](void*, double number, std::int32_t& value,
                               std::string& error) {
        if (!std::isfinite(number) || number < double(INT32_MIN) ||
            number > double(INT32_MAX)) {
            error = "Menu integer outside native conversion boundary";
            return false;
        }
        value = static_cast<std::int32_t>(number);
        error.clear();
        return true;
    };
    services.request_start_game = [](void*, bool numeric, std::int32_t requested,
            std::int32_t& slot, std::string& error) {
        return model_renderer::request_menu_start(numeric, requested, slot, error);
    };
    services.request_continue_game = [](void*, std::int32_t& slot, std::string& error) {
        return model_renderer::request_menu_continue(slot, error);
    };
    services.change_preview_slot = [](void*, std::int32_t slot, bool force,
                                       std::string& error) {
        return model_renderer::select_menu_preview_slot(slot, force, error);
    };
    services.debug_load = [](void* raw, std::string& error) {
        auto* owner = static_cast<FrontRuntimeContext*>(raw);
        return owner && owner->assets && model_renderer::menu_debug_load(owner->assets, error);
    };
    services.debug_query = [](void* raw, const char* key, std::string& error) {
        auto* owner = static_cast<FrontRuntimeContext*>(raw);
        return owner && owner->assets && model_renderer::menu_debug_query(owner->assets, key, error);
    };
    services.save_game = [](void*, std::uintptr_t character, std::string& error) {
        return model_renderer::ui_player_save_game(character, error);
    };
    services.camera_zoom_lock = [](void*, bool locked) {
        model_renderer::camera_zoom_lock(locked);
    };
    ui.bind_front_runtime(services);
}

int game_swf_ascii_code(unsigned char value) noexcept {
    if (value >= 'a' && value <= 'z') return 65 + value - 'a';
    if (value >= 'A' && value <= 'Z') return 65 + value - 'A';
    if (value >= '0' && value <= '9') return value;
    switch (value) {
        case ' ': return 32;
        case ',': case '<': return 188;
        case '.': case '>': return 190;
        case ';': case ':': return 186;
        case '\'': case '"': return 222;
        case '/': case '?': return 191;
        case '`': case '~': return 192;
        case '-': case '_': return 189;
        case '=': case '+': return 187;
        case '[': case '{': return 219;
        case ']': case '}': return 221;
        case '\\': case '|': return 220;
        case '!': return 49;
        case '@': return 50;
        case '#': return 51;
        case '$': return 52;
        case '%': return 53;
        case '^': return 54;
        case '&': return 55;
        case '*': return 56;
        case '(': return 57;
        case ')': return 48;
        default: return -1;
    }
}

bool game_swf_ascii_shift(unsigned char value) noexcept {
    return (value >= 'A' && value <= 'Z') ||
        std::string_view("!@#$%^&*()_+{}|:\"<>?~").find(char(value)) != std::string_view::npos;
}

int game_swf_special_key(SDL_Keycode key) noexcept {
    switch (key) {
        case SDLK_BACKSPACE: return 8;
        case SDLK_TAB: return 9;
        case SDLK_RETURN: case SDLK_KP_ENTER: return 13;
        case SDLK_ESCAPE: return 27;
        case SDLK_PAGEUP: return 34;
        case SDLK_PAGEDOWN: return 33;
        case SDLK_HOME: return 36;
        case SDLK_END: return 35;
        case SDLK_LEFT: return 37;
        case SDLK_UP: return 38;
        case SDLK_RIGHT: return 39;
        case SDLK_DOWN: return 40;
        case SDLK_DELETE: return 46;
        case SDLK_LSHIFT: case SDLK_RSHIFT: return 16;
        default: return -1;
    }
}

bool send_menu_text(dh2::android_ui::OriginalUiSession& ui,
                    const char* text, std::string& error) {
    if (!text) return true;
    for (const auto* cursor = reinterpret_cast<const unsigned char*>(text); *cursor; ++cursor) {
        const int code = game_swf_ascii_code(*cursor);
        if (code < 0) continue;
        const bool shift = game_swf_ascii_shift(*cursor);
        if (shift && !ui.menu_key_event(16, true, error)) return false;
        if (!ui.menu_key_event(static_cast<std::uint32_t>(code), true, error) ||
            !ui.menu_key_event(static_cast<std::uint32_t>(code), false, error)) return false;
        if (shift && !ui.menu_key_event(16, false, error)) return false;
    }
    return true;
}

bool start_from_authored_menu(const dh2::android_ui::NativeStartGameIntentV1& request,
        AAssetManager* assets, const std::filesystem::path& runtime,
        dh2::android_ui::OriginalUiSession& ui, bool& gameplay, std::string& error) {
    if (request.slot < 0 || !assets) {
        error = "Authored NativeStartGame returned an invalid slot or asset owner";
        return false;
    }
    ui.deactivate();
    const auto startup = model_renderer::start_menu_game(request.slot, assets, -1,
        request.has_numeric_difficulty, request.requested_difficulty);
    const bool world_loaded = startup.rfind("Crypt |", 0) == 0 ||
                              startup.rfind("SWAMP |", 0) == 0;
    std::string attach_error;
    const bool hud_attached = world_loaded && ui.attach_player(runtime.string(), attach_error);
    model_renderer::bind_gameplay_ui_session(hud_attached ? &ui : nullptr);
    if (dh2::android_ui::menu_start_commit_gate_v1(world_loaded, hud_attached)) {
        gameplay = true;
        std::cout << "Authored NativeStartGame | slot " << request.slot << " | " << startup << '\n';
        error.clear();
        return true;
    }
    if (world_loaded) model_renderer::deactivate();
    std::string recovery_error;
    if (!ui.resume_front_after_start_failure(recovery_error)) {
        error = "NativeStartGame failed: " +
            (attach_error.empty() ? startup : attach_error) +
            "; authored menu recovery failed: " + recovery_error;
    } else {
        error = "NativeStartGame failed: " +
            (attach_error.empty() ? startup : attach_error);
    }
    return false;
}

std::int32_t select_or_create_source_profile(
        AAssetManager* assets, const std::filesystem::path& runtime,
        dh2::android_ui::OriginalUiSession& original_ui) {
    std::string error;
    for (std::uint32_t candidate = 0; candidate < 4; ++candidate) {
        bool exists = false;
        if (!dh2::data::campaign_profile_exists_v1(runtime.string(), candidate, exists, error))
            throw std::runtime_error("Source campaign profile lookup failed: " + error);
        if (!exists) continue;
        if (!model_renderer::assign_menu_save_slot(static_cast<std::int32_t>(candidate), 0, error))
            throw std::runtime_error("Source campaign profile assignment failed: " + error);
        std::cout << "Using existing source campaign slot " << candidate << '\n';
        return static_cast<std::int32_t>(candidate);
    }

    std::int32_t slot = -1;
    bool published = false;
    if (!model_renderer::create_menu_save_slot(
            assets, "Desktop Knight", "KnightPlayerBase", slot, published, error,
            &original_ui, desktop_difficulty_count))
        throw std::runtime_error("Source profile creation failed: " + error);
    if (!published || slot < 0)
        throw std::runtime_error("NativeCreateSaveSlot did not publish a canonical campaign profile");
    if (!model_renderer::assign_menu_save_slot(slot, 0, error))
        throw std::runtime_error("Source campaign profile assignment failed: " + error);
    std::cout << "Created source campaign profile in slot " << slot << '\n';
    return slot;
}

void save_capture(const std::filesystem::path& path, int width, int height) {
    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[0] != 0 || viewport[1] != 0 || viewport[2] != width || viewport[3] != height)
        throw std::runtime_error("GLES capture viewport does not match the framebuffer dimensions");
    const auto prior_error = glGetError();
    if (prior_error != GL_NO_ERROR)
        throw std::runtime_error("GLES reported an error before framebuffer readback: " + std::to_string(prior_error));
    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width) * height * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glFinish();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    const auto read_error = glGetError();
    if (read_error != GL_NO_ERROR)
        throw std::runtime_error("GLES framebuffer readback failed with error " + std::to_string(read_error));
    if (!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    if (!output) throw std::runtime_error("Could not open capture path: " + path.string());
    output << "P6\n" << width << ' ' << height << "\n255\n";
    std::vector<std::uint8_t> rgb(static_cast<std::size_t>(width) * 3);
    for (int y = height - 1; y >= 0; --y) {
        const auto* row = pixels.data() + static_cast<std::size_t>(y) * width * 4;
        for (int x = 0; x < width; ++x) {
            rgb[static_cast<std::size_t>(x) * 3 + 0] = row[static_cast<std::size_t>(x) * 4 + 0];
            rgb[static_cast<std::size_t>(x) * 3 + 1] = row[static_cast<std::size_t>(x) * 4 + 1];
            rgb[static_cast<std::size_t>(x) * 3 + 2] = row[static_cast<std::size_t>(x) * 4 + 2];
        }
        output.write(reinterpret_cast<const char*>(rgb.data()), static_cast<std::streamsize>(rgb.size()));
    }
    if (!output) throw std::runtime_error("Framebuffer capture write failed: " + path.string());
}
}

int main(int argc, char** argv) {
    try {
        auto options = parse(argc, argv);
        const bool any_fixed_cultist_scenario =
            options.fixed_cultist_scenario || options.fixed_cultist_kill_scenario;
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
        if (options.runtime.empty()) {
            char* preference = SDL_GetPrefPath("Gameloft", "Dungeon Hunter II");
            if (!preference)
                throw std::runtime_error(std::string("SDL_GetPrefPath: ") + SDL_GetError());
            options.runtime = std::filesystem::u8path(preference);
            SDL_free(preference);
        } else options.runtime = std::filesystem::absolute(options.runtime);
        std::filesystem::create_directories(options.runtime);
        SDL_Window* window = SDL_CreateWindow("Dungeon Hunter II | shared native runtime",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!window) throw std::runtime_error(std::string("SDL_CreateWindow: ") + SDL_GetError());
        SDL_SysWMinfo window_info{};
        SDL_VERSION(&window_info.version);
        if (!SDL_GetWindowWMInfo(window, &window_info) || window_info.subsystem != SDL_SYSWM_WINDOWS)
            throw std::runtime_error(std::string("SDL Windows window handle: ") + SDL_GetError());
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        EGLint egl_major = 0, egl_minor = 0;
        if (display == EGL_NO_DISPLAY || !eglInitialize(display, &egl_major, &egl_minor))
            throw std::runtime_error("ANGLE eglInitialize failed");
        if (!eglBindAPI(EGL_OPENGL_ES_API)) throw std::runtime_error("ANGLE eglBindAPI failed");
        const EGLint config_attributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
        };
        EGLConfig egl_config{};
        EGLint config_count = 0;
        if (!eglChooseConfig(display, config_attributes, &egl_config, 1, &config_count) || config_count != 1)
            throw std::runtime_error("ANGLE has no compatible RGBA8/GLES2 window config");
        const EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        EGLContext context = eglCreateContext(display, egl_config, EGL_NO_CONTEXT, context_attributes);
        if (context == EGL_NO_CONTEXT) throw std::runtime_error("ANGLE GLES2 context creation failed");
        EGLSurface surface = eglCreateWindowSurface(display, egl_config,
            window_info.info.win.window, nullptr);
        if (surface == EGL_NO_SURFACE) throw std::runtime_error("ANGLE SDL window surface creation failed");
        if (!eglMakeCurrent(display, surface, surface, context))
            throw std::runtime_error("ANGLE eglMakeCurrent failed");
        eglSwapInterval(display, 1);

        std::unique_ptr<AAssetManager, decltype(&AAssetManager_delete)> assets(
            AAssetManager_createFromFilesystem(options.assets.string().c_str()),
            &AAssetManager_delete);
        if (!assets) throw std::runtime_error("Could not open asset directory: " + options.assets.string());
        dh2::desktop_native::GameplayAudioSdl gameplay_audio;
        std::string audio_error;
        if (!gameplay_audio.open(audio_error))
            SDL_Log("DH2 gameplay audio unavailable; continuing without output: %s", audio_error.c_str());

        model_renderer::runtime_directory(options.runtime.string());
        model_renderer::reset_context();
        dh2::android_ui::OriginalUiSession original_ui;
        std::string ui_error;
        if (!original_ui.initialize(assets.get(), ui_error))
            throw std::runtime_error("Shared UI asset services failed: " + ui_error);
        const auto design_settings = original_ui.design_settings_table();
        model_renderer::bind_xp_design_settings(design_settings.data, design_settings.size);
        model_renderer::bind_item_text_services(&original_ui,
            [](void* context, const dh2::data::ItemTable& items,
               const dh2::data::CharacterTable& characters) {
                return static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->item_text_services(items, characters);
            });
        FrontRuntimeContext front_runtime{assets.get(), &original_ui};
        bind_desktop_front_runtime(original_ui, front_runtime);
        int initial_width = 1, initial_height = 1;
        bool gameplay = false;
        if (options.level_row >= 0) {
            // Keep the explicit direct row path for bounded development runs.
            // The default launch below enters the actual authored main menu.
            if (!original_ui.load_front_screen(options.runtime.string(), "loading", ui_error))
                throw std::runtime_error("Shared loading UI setup failed: " + ui_error);
            SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
            if (!original_ui.render(initial_width, initial_height, ui_error))
                throw std::runtime_error("Shared loading UI frame failed: " + ui_error);
            const auto slot = select_or_create_source_profile(assets.get(), options.runtime, original_ui);
            const auto debug_level_row = options.level_row == 23 ? 23 : -1;
            const auto loaded = model_renderer::start_menu_game(
                slot, assets.get(), debug_level_row, false, 0);
            const auto expected_prefix = options.level_row == 23 ? "Crypt |" : "SWAMP |";
            if (loaded.rfind(expected_prefix, 0) != 0)
                throw std::runtime_error("Canonical source-menu row " + std::to_string(options.level_row) +
                                         " resolved a different level: " + loaded);
            std::cout << loaded << '\n';
            if (!original_ui.attach_player(options.runtime.string(), ui_error))
                throw std::runtime_error("Shared player HUD attach failed: " + ui_error);
            model_renderer::bind_gameplay_ui_session(&original_ui);
            gameplay = true;
        } else {
            if (!original_ui.load_front_screen(options.runtime.string(), "main", ui_error))
                throw std::runtime_error("Authored main menu setup failed: " + ui_error);
            std::cout << "Original main menu | click/touch authored controls | Esc quits\n";
        }
        SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
        glViewport(0, 0, initial_width, initial_height);
        if (gameplay) model_renderer::drawable_target_changed(initial_width, initial_height);
        const auto* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        std::cout << "Renderer: " << (renderer ? renderer : "unknown")
                  << " | GLES: " << (version ? version : "unknown") << '\n';
        std::cout << "Authored menu: mouse/touch and name typing | Gameplay: WASD move, Space attack nearest reachable enemy, drag orbit, wheel zoom | Esc quit\n";

        std::array<bool, 4> keys{};
        bool running = true, dragging = false, finger_down = false;
        SDL_TouchID active_touch = 0;
        SDL_FingerID active_finger = 0;
        float finger_x = 0.0f, finger_y = 0.0f;
        bool text_input_started = false;
        std::uint64_t rendered_frames = 0;
        int mouse_x = 0, mouse_y = 0;
        auto last_vitals = SDL_GetTicks64();
        std::array<int, 7> scenario_initial_vitals{};
        std::int32_t scenario_target_index = -1;
        std::int32_t scenario_target_initial_hp = 0;
        std::string scenario_attack_result;
        bool scenario_started = false, scenario_attack_sent = false;
        std::uint64_t fixed_attack_frame = 0;
        std::uint64_t fixed_next_attack_frame = 0;
        std::uint64_t fixed_target_dead_frame = 0;
        std::uintptr_t fixed_target_identity = 0;
        std::uint32_t fixed_attack_count = 0;
        bool fixed_kill_complete = false;
        std::string fixed_kill_failure;
        CombatReceiptSnapshot fixed_receipt_before;
        bool fixed_receipt_before_ready = false;
        std::uint32_t fixed_route_nodes = 0, fixed_route_edges = 0,
                      fixed_route_status = 0, fixed_route_waypoints = 0;
        bool fixed_route_found = false;
        constexpr std::int32_t fixed_target_index = 4;
        constexpr std::array<float, 3> fixed_target_position{-1234.43f, -369.638f, 198.437f};
        auto sync_text_input = [&] {
            const bool should_start = !gameplay && original_ui.menu_text_input_active();
            if (should_start == text_input_started) return;
            if (should_start) SDL_StartTextInput();
            else SDL_StopTextInput();
            text_input_started = should_start;
        };
        auto return_to_authored_menu = [&] {
            if (!gameplay || !original_ui.consume_gameplay_main_menu_request()) return;
            std::string transition_error;
            if (!model_renderer::unload_game_to_menu(transition_error)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not return to authored menu: %s",
                             transition_error.c_str());
                return;
            }
            if (!original_ui.load_front_screen(options.runtime.string(), "main", transition_error))
                throw std::runtime_error("Authored main menu reload failed: " + transition_error);
            gameplay = false;
            keys.fill(false);
            model_renderer::stop_movement();
            std::cout << "Returned to authored main menu\n";
        };
        auto pump_front_audio = [&] {
            for (auto cue = original_ui.consume_menu_sound(); !cue.empty();
                 cue = original_ui.consume_menu_sound()) {
                std::string audio_error;
                if (!gameplay_audio.play_menu_sound(assets.get(), cue, audio_error))
                    SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Authored menu sound unavailable (%s): %s",
                                cue.c_str(), audio_error.c_str());
            }
            for (auto command = original_ui.consume_menu_audio(); !command.empty();
                 command = original_ui.consume_menu_audio()) {
                std::string audio_error;
                if (!gameplay_audio.apply_menu_audio_command(command, audio_error))
                    SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Authored menu audio command rejected (%s): %s",
                                command.c_str(), audio_error.c_str());
            }
        };
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = 1, height = 1;
                    SDL_GetWindowSizeInPixels(window, &width, &height);
                    glViewport(0, 0, width, height);
                    if (gameplay) model_renderer::drawable_target_changed(width, height);
                } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    if (dragging && !dispatch_touch(original_ui, window, mouse_x, mouse_y, 3, ui_error))
                        std::cerr << "HUD touch release on focus loss failed: " << ui_error << '\n';
                    if (finger_down && !dispatch_finger_touch(original_ui, window, finger_x, finger_y, 3, ui_error))
                        std::cerr << "Authored touch cancel on focus loss failed: " << ui_error << '\n';
                    dragging = false;
                    finger_down = false;
                    keys.fill(false);
                    if (gameplay) model_renderer::stop_movement();
                } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE &&
                        (!original_ui.menu_text_input_active() || gameplay)) {
                        running = false;
                    } else if (!gameplay && original_ui.menu_text_input_active()) {
                        const int code = game_swf_special_key(event.key.keysym.sym);
                        if (code >= 0 && !original_ui.menu_key_event(static_cast<std::uint32_t>(code), true, ui_error))
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu key-down failed: %s", ui_error.c_str());
                    } else if (gameplay) {
                        if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                            std::cout << model_renderer::player_attack() << '\n';
                        update_movement(keys);
                    }
                } else if (event.type == SDL_KEYUP) {
                    if (!gameplay && original_ui.menu_text_input_active()) {
                        const int code = game_swf_special_key(event.key.keysym.sym);
                        if (code >= 0 && !original_ui.menu_key_event(static_cast<std::uint32_t>(code), false, ui_error))
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu key-up failed: %s", ui_error.c_str());
                    } else if (gameplay) {
                        if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = false;
                        update_movement(keys);
                        if (!keys[0] && !keys[1] && !keys[2] && !keys[3]) model_renderer::stop_movement();
                    }
                } else if (event.type == SDL_TEXTINPUT) {
                    if (!gameplay && original_ui.menu_text_input_active() &&
                        !send_menu_text(original_ui, event.text.text, ui_error))
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu text input failed: %s", ui_error.c_str());
                } else if (event.type == SDL_FINGERDOWN && !finger_down) {
                    finger_down = true; active_touch = event.tfinger.touchId;
                    active_finger = event.tfinger.fingerId;
                    finger_x = event.tfinger.x; finger_y = event.tfinger.y;
                    if (!dispatch_finger_touch(original_ui, window, finger_x, finger_y, 0, ui_error))
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Authored touch down failed: %s", ui_error.c_str());
                } else if (event.type == SDL_FINGERMOTION && finger_down &&
                           event.tfinger.touchId == active_touch && event.tfinger.fingerId == active_finger) {
                    finger_x = event.tfinger.x; finger_y = event.tfinger.y;
                    if (!dispatch_finger_touch(original_ui, window, finger_x, finger_y, 2, ui_error))
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Authored touch move failed: %s", ui_error.c_str());
                } else if (event.type == SDL_FINGERUP && finger_down &&
                           event.tfinger.touchId == active_touch && event.tfinger.fingerId == active_finger) {
                    finger_x = event.tfinger.x; finger_y = event.tfinger.y;
                    if (!dispatch_finger_touch(original_ui, window, finger_x, finger_y, 1, ui_error))
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Authored touch up failed: %s", ui_error.c_str());
                    finger_down = false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.which != SDL_TOUCH_MOUSEID &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    dragging = true; mouse_x = event.button.x; mouse_y = event.button.y;
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 0, ui_error))
                        std::cerr << "Authored UI touch down failed: " << ui_error << '\n';
                } else if (event.type == SDL_MOUSEBUTTONUP && event.button.which != SDL_TOUCH_MOUSEID &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 1, ui_error))
                        std::cerr << "Authored UI touch up failed: " << ui_error << '\n';
                    dragging = false;
                } else if (event.type == SDL_MOUSEMOTION && event.motion.which != SDL_TOUCH_MOUSEID &&
                           (!gameplay || dragging)) {
                    if (!dispatch_touch(original_ui, window, event.motion.x, event.motion.y, 2, ui_error))
                        std::cerr << "Authored UI pointer move failed: " << ui_error << '\n';
                    if (gameplay && dragging && original_ui.camera_pan_allowed())
                        model_renderer::orbit(float(event.motion.x - mouse_x), float(event.motion.y - mouse_y), 1.0f);
                    mouse_x = event.motion.x; mouse_y = event.motion.y;
                } else if (event.type == SDL_MOUSEWHEEL) {
                    if (gameplay) model_renderer::orbit(0.0f, 0.0f, event.wheel.y > 0 ? 0.92f : 1.08f);
                }
            }
            return_to_authored_menu();
            sync_text_input();
            if (options.movement_attack_scenario && scenario_started && !scenario_attack_sent) {
                if (rendered_frames >= options.frames / 2) {
                    keys.fill(false);
                    update_movement(keys);
                    model_renderer::stop_movement();
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (!model_renderer::gameplay_pose_snapshot(pose, ui_error))
                        throw std::runtime_error("Shared attack pose snapshot failed: " + ui_error);
                    std::cout << "Scenario attack candidate " << pose.enemy_index << " at "
                              << pose.enemy_position[0] << ',' << pose.enemy_position[1]
                              << " HP " << pose.enemy_hp << '/' << pose.enemy_max_hp << '\n';
                    scenario_target_index = pose.enemy_index;
                    scenario_target_initial_hp = pose.enemy_hp;
                    scenario_attack_result = model_renderer::player_attack();
                    std::cout << "Scenario nearest-enemy attack result: " << scenario_attack_result << '\n';
                    scenario_attack_sent = true;
                } else if (rendered_frames && rendered_frames % 30 == 0) {
                    steer_toward_nearest_enemy();
                }
            }
            if (options.fixed_cultist_scenario && scenario_started && !scenario_attack_sent &&
                rendered_frames && rendered_frames % 30 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(fixed_target_index, target_state, ui_error))
                    throw std::runtime_error("Fixed Crypt target snapshot failed: " + ui_error);
                if (target_state.name != "_prim_tmp_cultist07")
                    throw std::runtime_error("Fixed Crypt target identity changed: " + target_state.name);
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (!model_renderer::gameplay_pose_snapshot(pose, ui_error))
                    throw std::runtime_error("Fixed Crypt target pose failed: " + ui_error);
                const float target_distance = std::hypot(
                    pose.player_position[0] - fixed_target_position[0],
                    pose.player_position[1] - fixed_target_position[1]);
                if (pose.enemy_index == fixed_target_index &&
                    pose.enemy_in_melee_range) {
                    keys.fill(false);
                    update_movement(keys);
                    model_renderer::stop_movement();
                    scenario_target_index = fixed_target_index;
                    scenario_target_initial_hp = target_state.hp;
                    scenario_attack_result = model_renderer::player_attack();
                    scenario_attack_sent = true;
                    fixed_attack_frame = rendered_frames;
                    std::cout << "Fixed Crypt attack | index " << fixed_target_index
                              << " | name " << target_state.name << " | player-target XY "
                              << target_distance << " | HP " << target_state.hp << '/'
                              << target_state.max_hp << " | result " << scenario_attack_result << '\n';
                } else {
                    if (rendered_frames % 300 == 0)
                        std::cout << "Fixed Crypt approach progress | nearest " << pose.enemy_index
                                  << " | target XY " << target_distance << " | target HP "
                                  << target_state.hp << '/' << target_state.max_hp
                                  << " | live melee reach " << pose.enemy_in_melee_range << '\n';
                    if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                            fixed_route_edges, fixed_route_status, fixed_route_found,
                            fixed_route_waypoints)) {
                        std::cout << "Fixed Crypt route blocked | nearest " << pose.enemy_index
                                  << " | target XY " << target_distance << " | graph nodes "
                                  << fixed_route_nodes << " | edges " << fixed_route_edges
                                  << " | status " << fixed_route_status << " | found "
                                  << fixed_route_found << " | waypoints " << fixed_route_waypoints << '\n';
                        throw std::runtime_error("No selected-floor path to fixed Crypt target");
                    }
                }
            }
            if (options.fixed_cultist_kill_scenario && scenario_started &&
                !fixed_kill_complete && fixed_kill_failure.empty() &&
                rendered_frames && rendered_frames % 15 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(
                        fixed_target_index, target_state, ui_error)) {
                    fixed_kill_failure = "Fixed Crypt target snapshot failed: " + ui_error;
                } else if (target_state.name != "_prim_tmp_cultist07") {
                    fixed_kill_failure = "Fixed Crypt target identity changed: " + target_state.name;
                } else if (target_state.dead || target_state.hp <= 0) {
                    if (!fixed_target_dead_frame) {
                        fixed_target_dead_frame = rendered_frames + 1;
                        keys.fill(false);
                        update_movement(keys);
                        model_renderer::stop_movement();
                        std::cout << "Fixed Crypt target death observed | " << target_state.name
                                  << " | HP " << target_state.hp << '/' << target_state.max_hp
                                  << " | attack attempts " << fixed_attack_count << '\n';
                    }
                } else {
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (!model_renderer::gameplay_pose_snapshot(pose, ui_error)) {
                        fixed_kill_failure = "Fixed Crypt target pose failed: " + ui_error;
                    } else if (pose.enemy_index != fixed_target_index) {
                        if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                                fixed_route_edges, fixed_route_status, fixed_route_found,
                                fixed_route_waypoints))
                            fixed_kill_failure = "No selected-floor path to fixed Crypt target";
                    } else {
                        const float target_distance = std::hypot(
                            pose.player_position[0] - fixed_target_position[0],
                            pose.player_position[1] - fixed_target_position[1]);
                        if (!pose.enemy_in_melee_range) {
                            if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                                    fixed_route_edges, fixed_route_status, fixed_route_found,
                                    fixed_route_waypoints))
                                fixed_kill_failure = "No selected-floor path to fixed Crypt target";
                        } else if (!fixed_target_dead_frame &&
                                   rendered_frames >= fixed_next_attack_frame) {
                            keys.fill(false);
                            update_movement(keys);
                            model_renderer::stop_movement();
                            // The SDL Space handler calls this same source-owned
                            // entry; cooldown and animation acceptance stay live.
                            const auto attack_result = model_renderer::player_attack();
                            std::cout << "Fixed Crypt normal attack | attempt "
                                      << (fixed_attack_count + 1) << " | candidate " << pose.enemy_index
                                      << " | target XY " << target_distance << " | HP "
                                      << target_state.hp << '/' << target_state.max_hp
                                      << " | result " << attack_result << '\n';
                            if (attack_result == "Attacking") {
                                std::uintptr_t player = 0;
                                model_renderer::UiEnemyHudProjectionV1 hud_target;
                                if (!model_renderer::ui_player_identity(player, ui_error) ||
                                    !model_renderer::ui_player_enemy_hud_projection(
                                        player, hud_target, ui_error)) {
                                    fixed_kill_failure = "Player attack target projection failed: " + ui_error;
                                } else if (!hud_target.character_target.present ||
                                    !hud_target.character_target.debug_name ||
                                    std::string_view(hud_target.character_target.debug_name) !=
                                        "_prim_tmp_cultist07") {
                                    fixed_kill_failure = "Default Player attack selected a different target";
                                } else if (fixed_target_identity &&
                                    fixed_target_identity != hud_target.character_target.identity) {
                                    fixed_kill_failure = "Fixed Crypt target Character identity changed";
                                } else {
                                    fixed_target_identity = hud_target.character_target.identity;
                                    scenario_target_index = fixed_target_index;
                                    if (fixed_attack_count == 0) {
                                        scenario_target_initial_hp = target_state.hp;
                                        fixed_attack_frame = rendered_frames;
                                        scenario_attack_result = attack_result;
                                    }
                                    ++fixed_attack_count;
                                    scenario_attack_sent = true;
                                    fixed_next_attack_frame = rendered_frames + 60;
                                    std::cout << "Fixed Crypt selected target identity "
                                              << fixed_target_identity << " | Player AI target "
                                              << (hud_target.ai_target.debug_name ?
                                                  hud_target.ai_target.debug_name : "<none>") << '\n';
                                }
                            } else if (attack_result == "Attack is cooling down" ||
                                       attack_result == "Attack is already in progress") {
                                fixed_next_attack_frame = rendered_frames + 15;
                            } else {
                                fixed_kill_failure = "Normal Player attack failed: " + attack_result;
                            }
                        }
                        if (rendered_frames % 300 == 0)
                            std::cout << "Fixed Crypt kill progress | nearest " << pose.enemy_index
                                      << " | target XY " << target_distance << " | target HP "
                                      << target_state.hp << '/' << target_state.max_hp
                                      << " | live melee reach " << pose.enemy_in_melee_range
                                      << " | attacks " << fixed_attack_count << '\n';
                    }
                }
            }
            int width = 1, height = 1;
            SDL_GetWindowSizeInPixels(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (gameplay) {
                model_renderer::draw(width, height);
                if (!model_renderer::active())
                    throw std::runtime_error("Shared renderer deactivated during gameplay frame; frame rejected");
                PlayerUiFrame player_ui{&original_ui, width, height};
                if (!model_renderer::with_player_status_sheet(&player_ui, draw_player_ui, ui_error))
                    throw std::runtime_error("Shared player HUD frame failed: " + ui_error);
            } else if (!original_ui.render(width, height, ui_error)) {
                throw std::runtime_error("Authored main menu frame failed: " + ui_error);
            }
            dh2::android_ui::NativeStartGameIntentV1 launch_request;
            if (!gameplay && original_ui.consume_launch_request(launch_request)) {
                if (!start_from_authored_menu(launch_request, assets.get(), options.runtime,
                                              original_ui, gameplay, ui_error))
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", ui_error.c_str());
                keys.fill(false);
                model_renderer::stop_movement();
                sync_text_input();
            }
            pump_front_audio();
            gameplay_audio.pump(assets.get());
            if ((options.movement_attack_scenario || options.fixed_cultist_scenario ||
                 options.fixed_cultist_kill_scenario) && scenario_attack_sent &&
                (rendered_frames + 1) % 30 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(
                        scenario_target_index, target_state, ui_error)) {
                    if (options.fixed_cultist_kill_scenario)
                        fixed_kill_failure = "Shared exact-target HP snapshot failed: " + ui_error;
                    else
                        throw std::runtime_error("Shared exact-target HP snapshot failed: " + ui_error);
                } else {
                    std::cout << "Scenario target HP | " << target_state.name << ' '
                              << target_state.hp << '/' << target_state.max_hp
                              << " | delta " << target_state.hp - scenario_target_initial_hp
                              << " | dead " << target_state.dead << '\n';
                    if (options.fixed_cultist_kill_scenario && target_state.dead &&
                        !fixed_target_dead_frame)
                        fixed_target_dead_frame = rendered_frames + 1;
                }
            }
            if (gameplay && (options.movement_attack_scenario || options.fixed_cultist_scenario ||
                             options.fixed_cultist_kill_scenario) && !scenario_started) {
                scenario_initial_vitals = model_renderer::player_vitals();
                print_vitals_snapshot("Scenario initial", scenario_initial_vitals);
                model_renderer::move_axis(0.0f, 0.0f);
                if (any_fixed_cultist_scenario) {
                    model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                    if (!model_renderer::gameplay_enemy_state_snapshot(fixed_target_index, target_state, ui_error))
                        throw std::runtime_error("Fixed Crypt target snapshot failed: " + ui_error);
                    std::cout << "Fixed Crypt target baseline | index " << fixed_target_index
                              << " | name " << target_state.name << " | HP " << target_state.hp
                              << '/' << target_state.max_hp << " | position "
                              << fixed_target_position[0] << ',' << fixed_target_position[1]
                              << ',' << fixed_target_position[2] << '\n';
                    if (target_state.name != "_prim_tmp_cultist07")
                        throw std::runtime_error("Fixed Crypt target identity differs from expected candidate 4");
                    if (options.fixed_cultist_kill_scenario) {
                        if (!read_combat_receipt(fixed_receipt_before, ui_error))
                            throw std::runtime_error("Initial Player XP/inventory snapshot failed: " + ui_error);
                        fixed_receipt_before_ready = true;
                        print_combat_receipt("Fixed Crypt player receipt before", fixed_receipt_before);
                    }
                    if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                            fixed_route_edges, fixed_route_status, fixed_route_found,
                            fixed_route_waypoints))
                        throw std::runtime_error("No selected-floor path to fixed Crypt target");
                } else if (!steer_toward_nearest_enemy()) {
                    throw std::runtime_error("No nearest source enemy is available for move-attack");
                }
                scenario_started = true;
                if (!options.initial_capture.empty()) save_capture(options.initial_capture, width, height);
            }
            bool capture_saved_this_frame = false;
            if (options.frames && rendered_frames + 1 == options.frames && !options.capture.empty()) {
                save_capture(options.capture, width, height);
                capture_saved_this_frame = true;
            }
            const auto completed_frame = rendered_frames + 1;
            const bool fixed_attack_settled = options.fixed_cultist_scenario &&
                scenario_attack_sent && completed_frame >= fixed_attack_frame + 120 &&
                (!options.frames || completed_frame < options.frames);
            const bool fixed_kill_dead_settled = options.fixed_cultist_kill_scenario &&
                fixed_target_dead_frame && completed_frame >= fixed_target_dead_frame + 120;
            if (!capture_saved_this_frame && !options.capture.empty() &&
                (fixed_attack_settled || fixed_kill_dead_settled ||
                 (options.fixed_cultist_kill_scenario && !fixed_kill_failure.empty()))) {
                save_capture(options.capture, width, height);
                capture_saved_this_frame = true;
            }
            if (fixed_kill_dead_settled) {
                fixed_kill_complete = true;
                std::cout << "Fixed Crypt kill scenario completed its 120-frame post-death observation\n";
                running = false;
            }
            if (options.fixed_cultist_kill_scenario && !fixed_kill_failure.empty()) {
                std::cout << "Fixed Crypt kill scenario stopped on source failure: "
                          << fixed_kill_failure << '\n';
                running = false;
            }
            eglSwapBuffers(display, surface);
            ++rendered_frames;
            if (options.frames && rendered_frames >= options.frames) running = false;
            if (options.fixed_cultist_scenario && scenario_attack_sent &&
                rendered_frames >= fixed_attack_frame + 120) {
                running = false;
            }
            const auto now = SDL_GetTicks64();
            if (gameplay && now - last_vitals >= 1000) { print_vitals(); last_vitals = now; }
        }

        model_renderer::stop_movement();
        if (options.fixed_cultist_kill_scenario && gameplay) {
            if (!fixed_receipt_before_ready)
                throw std::runtime_error("Fixed Crypt kill scenario did not capture its initial Player receipt");
            CombatReceiptSnapshot receipt_after;
            if (!read_combat_receipt(receipt_after, ui_error))
                throw std::runtime_error("Final Player XP/inventory snapshot failed: " + ui_error);
            print_combat_receipt("Fixed Crypt player receipt after", receipt_after);
            print_combat_receipt_delta(fixed_receipt_before, receipt_after);
            model_renderer::GameplayEnemyStateSnapshotV1 target_state;
            if (!model_renderer::gameplay_enemy_state_snapshot(
                    fixed_target_index, target_state, ui_error))
                throw std::runtime_error("Final fixed Crypt target snapshot failed: " + ui_error);
            std::cout << "Fixed Crypt kill result | " << target_state.name << ' '
                      << target_state.hp << '/' << target_state.max_hp
                      << " | dead " << target_state.dead
                      << " | attacks " << fixed_attack_count
                      << " | selected identity " << fixed_target_identity << '\n';
            if (!fixed_kill_failure.empty())
                throw std::runtime_error(fixed_kill_failure);
            if (!fixed_kill_complete || !target_state.dead || target_state.hp > 0)
                throw std::runtime_error("fixed-cultist07-kill reached its frame bound before verified death");
            if (fixed_attack_count == 0 || !fixed_target_identity)
                throw std::runtime_error("fixed-cultist07-kill completed without a verified normal attack");
        }
        if ((options.movement_attack_scenario || options.fixed_cultist_scenario) && gameplay) {
            if (!scenario_attack_sent) {
                if (options.fixed_cultist_scenario) {
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (model_renderer::gameplay_pose_snapshot(pose, ui_error))
                        std::cout << "Fixed Crypt scenario ended before attack | nearest " << pose.enemy_index
                                  << " | target XY " << std::hypot(pose.player_position[0] - fixed_target_position[0],
                                                                      pose.player_position[1] - fixed_target_position[1])
                                  << " | graph nodes " << fixed_route_nodes << " | edges " << fixed_route_edges
                                  << " | status " << fixed_route_status << " | found " << fixed_route_found
                                  << " | waypoints " << fixed_route_waypoints << '\n';
                    throw std::runtime_error("fixed-cultist07 reached frame bound without candidate 4 becoming nearest");
                }
                throw std::runtime_error("move-attack scenario ended before its nearest-enemy attack");
            }
            const auto final_vitals = model_renderer::player_vitals();
            print_vitals_snapshot("Scenario final", final_vitals);
            std::cout << "Scenario HP delta " << (final_vitals[0] - scenario_initial_vitals[0]) / 256.0f
                      << " | MP delta " << (final_vitals[2] - scenario_initial_vitals[2]) / 256.0f << '\n';
            model_renderer::GameplayEnemyStateSnapshotV1 target_state;
            if (!model_renderer::gameplay_enemy_state_snapshot(
                    scenario_target_index, target_state, ui_error))
                throw std::runtime_error("Final shared exact-target HP snapshot failed: " + ui_error);
            std::cout << "Scenario target final | " << target_state.name << ' '
                      << target_state.hp << '/' << target_state.max_hp
                      << " | delta " << target_state.hp - scenario_target_initial_hp
                      << " | dead " << target_state.dead
                      << " | attack " << scenario_attack_result << '\n';
        }
        model_renderer::deactivate();
        original_ui.deactivate();
        pump_front_audio();
        gameplay_audio.pump(assets.get());
        gameplay_audio.close();
        SDL_StopTextInput();
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "DH2 desktop runtime failed: " << error.what() << '\n';
        SDL_Quit();
        return 1;
    }
}
