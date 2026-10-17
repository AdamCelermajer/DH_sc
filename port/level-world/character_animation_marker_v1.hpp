#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace dh2::character_animation_marker_v1 {

enum class Foot : std::uint8_t { left, right };
enum class Kind : std::uint8_t {
    ignored, script, object_animation, effect_set, sound, footstep
};
enum class Status : std::uint8_t {
    complete, invalid_request, missing_service, dispatch_failed
};
enum class ProviderResult : std::uint8_t {
    dispatched, source_no_match, source_no_launch, failed
};

struct Request {
    std::uintptr_t character{};
    std::string_view marker;
    std::int32_t source_step{};
};

// The renderer owns these callbacks and binds them to the live Character
// Record, ObjectManager, VisualFXManager, and spatial audio queue. The kernel
// only classifies source markers; it creates no VM, clock, resource store, or
// effect/audio authority of its own.
struct Services {
    void* context{};
    bool (*script_event)(void*, std::uintptr_t character,
                         std::string_view name, std::int32_t source_step,
                         std::string& error){};
    ProviderResult (*object_animation)(void*, std::uintptr_t character,
                                       std::string_view target,
                                       std::string_view animation,
                                       std::string& error){};
    // Source Arrays name misses and successful manager calls that launch no
    // effect (for example a source pool cap) are successful no-ops. `failed`
    // means a mapped marker's source backend failed and remains visible.
    ProviderResult (*effect_set)(void*, std::uintptr_t character,
                                 std::string_view source_name,
                                 std::string& error){};
    ProviderResult (*spatial_sound)(void*, std::uintptr_t character,
                                    std::string_view source_name,
                                    std::string& error){};
    bool (*footstep)(void*, std::uintptr_t character, Foot source_foot,
                     std::string& error){};
};

struct Result {
    Status status{Status::invalid_request};
    Kind kind{Kind::script};
};

// Mirrors CharAI::_OnAnimEvent (ELF 0x003d4434) prefix dispatch and the
// AISDefault::OnAnimEvent (0x003dca50) post-script footstep branch. The
// caller supplies the EventTrack's source step; no local timestamp is read.
Result dispatch(const Request&, const Services&, std::string& error);

} // namespace dh2::character_animation_marker_v1
