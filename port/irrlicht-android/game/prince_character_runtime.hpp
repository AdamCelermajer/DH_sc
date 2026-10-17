#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::irrlicht_game {
class PrinceActor;

using PrinceAssetReader = bool (*)(void* context, const char* path,
                                   std::vector<std::uint8_t>* output,
                                   std::string* error);

// Borrowed native body calls consumed by the recovered Character source FSM.
// The caller binds only after creating the real body, and unbinds before its
// owning world/body is torn down. False reports a service error; the session
// stops further ticks without rolling back the partially executed transition.
struct PrinceCharacterBodyServices {
    void* context = nullptr;
    bool (*stop)(void*, std::string&) = nullptr;
    bool (*pin)(void*, std::string&) = nullptr;
    bool (*unpin)(void*, std::string&) = nullptr;
};

// Host-side composition of the recovered Character state/timer coordinator
// and two-slot source animation playback for the four-mesh Prince rig.
// SWAMP remains an explicitly bounded owner-position/input producer.
class PrinceCharacterRuntime {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    PrinceCharacterRuntime();
    ~PrinceCharacterRuntime();
    PrinceCharacterRuntime(PrinceCharacterRuntime&&) noexcept;
    PrinceCharacterRuntime& operator=(PrinceCharacterRuntime&&) noexcept;
    PrinceCharacterRuntime(const PrinceCharacterRuntime&) = delete;
    PrinceCharacterRuntime& operator=(const PrinceCharacterRuntime&) = delete;

    bool load(PrinceActor& actor, PrinceAssetReader reader, void* reader_context,
              std::string& error);
    // The PrinceCharacterRuntime wrapper is a borrowed session dependency.
    // Keep this object at the same address and do not reload it until
    // unbind_body_services() or the borrowing session has shut down.
    bool bind_body_services(const PrinceCharacterBodyServices& services,
                            std::string& error);
    void unbind_body_services();
    bool set_input(float x, float y, bool accepted, std::string& error);
    bool request_move(std::string& error);
    bool scene_phase(std::uint32_t absolute_ms, std::string& error);
    bool update_timers(std::uint32_t dt_ms, std::string& error);
    bool update_state(std::uint32_t dt_ms, std::string& error);
    bool animator_phase(std::string& error);
    bool update_pose(const float owner_position[3], std::string& error);

    std::int32_t state_id() const;
    std::uint32_t state_flags() const;
    bool copy_resolved_properties(std::array<std::int32_t, 224>& output) const;
    std::int32_t sequence_id() const;
    std::int32_t idle_sequence_id() const;
    std::int32_t walk_sequence_id() const;
    std::uint32_t registered_resource_count() const;
    std::uint32_t registration_occurrence_count() const;
    std::uint32_t timeline_less_resource_count() const;
    std::int32_t clip_id() const;
    std::int32_t engine_clip_id() const;
    float timeline_speed() const;
    std::uint32_t idle_common_update_calls() const;
    std::uint32_t external_state_events() const;
    bool body_present() const;
    bool body_services_bound() const;
    bool ready() const;
};
} // namespace dh2::irrlicht_game
