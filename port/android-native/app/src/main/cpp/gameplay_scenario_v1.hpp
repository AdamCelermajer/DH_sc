#pragma once
// Development-only gameplay driver shared by debug Android builds. It uses the
// same public renderer queries and Player inputs as the desktop scenarios
// (selected-floor route, move_axis, player_attack, HUD touch) and owns no
// gameplay state of its own.
#include "model_renderer.hpp"
#include <android/log.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace model_renderer {
// Defined in model_renderer.cpp (development probe, not part of the public header).
bool script_controllers_blocked() noexcept;
}

namespace dh2::native::gameplay_scenario_v1 {

// move_approach walks the Player up to the nearest enemy and stops in melee
// range, leaving every attack, dialog tap and potion to the real touch UI.
// observe only logs. fail_frame throws from the frame path once, proving a failed
// frame reaches the player as a visible error instead of a silent freeze.
enum class Mode : std::uint8_t { none, move_kill, move_defend, move_approach, observe, fail_frame };

struct Driver {
    Mode mode = Mode::none;
    std::uint64_t frames = 0;
    std::int32_t last_hp = -1;
    std::int32_t last_items = -1;
    bool died = false, damaged = false, revived = false;
    unsigned kills = 0, attacks = 0, pickups = 0, taps = 0;
    std::int32_t target = -1;
    bool arrived = false;

    static Mode parse(const std::string& name) {
        if (name == "move-kill") return Mode::move_kill;
        if (name == "move-defend") return Mode::move_defend;
        if (name == "move-approach") return Mode::move_approach;
        if (name == "observe") return Mode::observe;
        if (name == "fail-frame") return Mode::fail_frame;
        return Mode::none;
    }

    template <class... Values>
    static void log(const char* format, Values... values) {
        __android_log_print(ANDROID_LOG_INFO, "DH2Scenario", format, values...);
    }

    static void steer(const std::array<float, 3>& player, const std::array<float, 3>& goal) {
        model_renderer::GameplayRouteSnapshotV1 route;
        std::string error;
        std::array<float, 3> waypoint = goal;
        if (model_renderer::gameplay_route_snapshot(goal, route, error) && route.path_found &&
            !route.waypoints.empty()) {
            std::size_t next = 0;
            while (next + 1 < route.waypoints.size() &&
                   std::hypot(route.waypoints[next][0] - player[0],
                              route.waypoints[next][1] - player[1]) <= 48.0f)
                ++next;
            waypoint = route.waypoints[next];
        }
        const float dx = waypoint[0] - player[0], dy = waypoint[1] - player[1];
        const float distance = std::hypot(dx, dy);
        if (!std::isfinite(distance) || distance <= 1.0f) {
            model_renderer::move_axis(0.0f, 0.0f);
            return;
        }
        // Invert move_axis's recovered fixed +45-degree screen basis.
        constexpr float inverse_sqrt_two = 0.7071067811865475f;
        model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                                  (dy - dx) * inverse_sqrt_two / distance);
    }

    // Called once per rendered gameplay frame. tap(x, y, action) delivers a
    // HUD/menu touch through the retained gameplay UI session.
    template <class Tap>
    void step(int width, int height, Tap tap) {
        if (mode == Mode::none) return;
        ++frames;
        if (mode == Mode::fail_frame) {
            if (frames == 120) throw std::runtime_error("Scenario fail-frame: deliberate exception on the frame path");
            return;
        }
        if (died && !revived) {
            const auto phase = frames % 120;
            if (phase == 60 || phase == 66) {
                tap(float(width) / 2.0f, float(height) / 2.0f, phase == 60 ? 0 : 1);
                if (phase == 66) ++taps;
            }
        }
        if ((mode == Mode::move_approach || mode == Mode::observe) && frames % 60 == 0) {
            // Heartbeat: proves the render thread keeps calling the frame path.
            model_renderer::GameplayPoseSnapshotV1 beat;
            std::string beat_error;
            if (model_renderer::gameplay_pose_snapshot(beat, beat_error))
                log("Scenario heartbeat | frame %llu | Player %.0f %.0f HP %d/%d | enemy %d HP %d/%d melee %d | controllers blocked %d",
                    static_cast<unsigned long long>(frames), beat.player_position[0],
                    beat.player_position[1], beat.player_hp, beat.player_max_hp, beat.enemy_index,
                    beat.enemy_hp, beat.enemy_max_hp, int(beat.enemy_in_melee_range),
                    int(model_renderer::script_controllers_blocked()));
            else
                log("Scenario heartbeat | frame %llu | Player snapshot unavailable | %s",
                    static_cast<unsigned long long>(frames), beat_error.c_str());
        }
        if (mode == Mode::observe) return;
        if (frames % 15) return;
        model_renderer::GameplayPoseSnapshotV1 pose;
        std::string error;
        if (!model_renderer::gameplay_pose_snapshot(pose, error)) {
            if (!died) log("Scenario Player died | %s", error.c_str());
            died = true;
            return;
        }
        if (died && !revived && pose.player_hp > 0) {
            revived = true;
            log("Scenario revive verified | Player HP %d/%d | taps %u", pose.player_hp,
                pose.player_max_hp, taps);
        }
        if (last_hp > 0 && pose.player_hp < last_hp) {
            if (!damaged) log("Scenario damage verified | Player HP %d -> %d", last_hp, pose.player_hp);
            damaged = true;
        }
        last_hp = pose.player_hp;
        if (target >= 0) {
            model_renderer::GameplayEnemyStateSnapshotV1 state;
            if (model_renderer::gameplay_enemy_state_snapshot(target, state, error) &&
                (state.dead || state.hp <= 0)) {
                ++kills;
                log("Scenario kill verified | %s | kills %u", state.name.c_str(), kills);
                target = -1;
            }
        }
        if (mode == Mode::move_kill && kills > 0) {
            model_renderer::GameplayWorldItemSnapshotV1 items;
            if (model_renderer::gameplay_world_item_snapshot(items, error) && items.count > 0) {
                if (last_items >= 0 && items.count < last_items)
                    log("Scenario pickup verified | world items %d -> %d | pickups %u",
                        last_items, items.count, ++pickups);
                last_items = items.count;
                if (items.distance > 20.0f && pickups < 3) {
                    steer(pose.player_position, items.nearest);
                    return;
                }
            }
        }
        if (pose.enemy_index < 0 || pose.enemy_hp <= 0) return;
        if (mode == Mode::move_approach && (arrived || model_renderer::script_controllers_blocked()))
            return;
        if (pose.enemy_in_melee_range) {
            model_renderer::stop_movement();
            if (mode == Mode::move_approach) {
                arrived = true;
                log("Scenario arrived | enemy %d HP %d/%d | Player %.0f %.0f", pose.enemy_index,
                    pose.enemy_hp, pose.enemy_max_hp, pose.player_position[0], pose.player_position[1]);
            }
            if (mode == Mode::move_kill) {
                target = pose.enemy_index;
                const auto result = model_renderer::player_attack(-1);
                if (++attacks % 8 == 1) log("Scenario attack | enemy %d HP %d/%d | %s",
                    pose.enemy_index, pose.enemy_hp, pose.enemy_max_hp, result.c_str());
            }
            return;
        }
        steer(pose.player_position, pose.enemy_position);
    }
};

}  // namespace dh2::native::gameplay_scenario_v1
