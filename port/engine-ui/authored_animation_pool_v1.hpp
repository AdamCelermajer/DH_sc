#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace dh2::ui::authored_animation_pool_v1 {

inline constexpr std::size_t playback_context_count = 12;
inline constexpr std::size_t clones_per_style = 8;
inline constexpr std::size_t saturated_clone_slot = clones_per_style - 1;

struct CloneSlot {
    bool created{};
    bool active{};
};

struct StylePool {
    std::array<CloneSlot, clones_per_style> clones{};
};

struct Context {
    bool active{};
    std::uint32_t style_id{};
    std::uint8_t clone_slot{};
};

struct State {
    std::array<Context, playback_context_count> contexts{};
};

// FlashAnimManager allocates a free global context before touching a style's
// clone pool. A saturated style deliberately reuses its eighth clone.
inline bool acquire_at(State& state, StylePool& style, std::uint32_t style_id,
                       std::size_t context_id, std::size_t& clone_id,
                       bool& create_clone) noexcept {
    if (context_id >= state.contexts.size() || state.contexts[context_id].active)
        return false;
    clone_id = saturated_clone_slot;
    for (std::size_t i = 0; i < style.clones.size(); ++i) {
        if (!style.clones[i].active) {
            clone_id = i;
            break;
        }
    }

    auto& clone = style.clones[clone_id];
    create_clone = !clone.created;
    clone.created = true;
    clone.active = true;
    state.contexts[context_id] = {
        true, style_id, static_cast<std::uint8_t>(clone_id)};
    return true;
}

inline bool acquire(State& state, StylePool& style, std::uint32_t style_id,
                    std::size_t& context_id, std::size_t& clone_id,
                    bool& create_clone) noexcept {
    context_id = playback_context_count;
    for (std::size_t i = 0; i < state.contexts.size(); ++i) {
        if (!state.contexts[i].active) {
            context_id = i;
            break;
        }
    }
    if (context_id == playback_context_count) return false;
    return acquire_at(state, style, style_id, context_id, clone_id, create_clone);
}

// Source Update clears both the playback context and the referenced style
// clone's active flag when an auto-stopping animation reaches its final frame.
inline bool stop(State& state, StylePool& style, std::size_t context_id) noexcept {
    if (context_id >= state.contexts.size() || !state.contexts[context_id].active)
        return false;
    const auto clone_id = state.contexts[context_id].clone_slot;
    if (clone_id >= style.clones.size()) return false;
    state.contexts[context_id].active = false;
    style.clones[clone_id].active = false;
    return true;
}

} // namespace dh2::ui::authored_animation_pool_v1
