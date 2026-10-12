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
#include <string>

namespace dh2::native::gameplay_scenario_v1 {

enum class Mode : std::uint8_t { none, move_kill, move_defend };

struct Driver {
    Mode mode = Mode::none;
    std::uint64_t frames = 0;
    std::int32_t last_hp = -1;
    std::int32_t last_items = -1;
    bool died = false, damaged = false, revived = false;
    unsigned kills = 0, attacks = 0, pickups = 0, taps = 0;
    std::int32_t target = -1;

    static Mode parse(const std::string& name) {
        if (name == "move-kill") return Mode::move_kill;
        if (name == "move-defend") return Mode::move_defend;
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
        if (died && !revived) {
            const auto phase = frames % 120;
            if (phase == 60 || phase == 66) {
                tap(float(width) / 2.0f, float(height) / 2.0f, phase == 60 ? 0 : 1);
                if (phase == 66) ++taps;
            }
        }
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
        if (pose.enemy_in_melee_range) {
            model_renderer::stop_movement();
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
