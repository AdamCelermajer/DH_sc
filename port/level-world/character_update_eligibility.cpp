#include "character_update_eligibility.hpp"

#include <limits>

namespace dh2::character_update_eligibility {
namespace {
struct Range { std::uintptr_t begin, end; };

template<class T> bool range(const T* pointer, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignof(T) ||
        begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T)) return false;
    out = {begin, begin + sizeof(T)};
    return true;
}

bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }

struct ActiveFrame { const Character* character; ActiveFrame* previous; };
thread_local ActiveFrame* active_frames = nullptr;

struct ActiveGuard {
    ActiveFrame frame;
    explicit ActiveGuard(const Character* character)
        : frame{character, active_frames} { active_frames = &frame; }
    ~ActiveGuard() { active_frames = frame.previous; }
};

bool currently_active(const Character* character) {
    for (auto* frame = active_frames; frame; frame = frame->previous)
        if (frame->character == character) return true;
    return false;
}

Status query(const Services& services, Character* character,
             std::uintptr_t character_identity, Result& result,
             Operation operation, std::uintptr_t subject,
             std::uintptr_t argument, std::uint32_t first,
             std::uint32_t second, Response& response) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, character_identity, subject, argument,
                          first, second};
    response = {};
    ++result.service_calls;
    try {
        return services.invoke(services.context, character, &request, &response)
            ? Status::service_failed : Status::complete;
    } catch (...) {
        return Status::service_failed;
    }
}

bool valid_visual(const Visual* visual, Range character_range,
                  Range services_range, Range out_range,
                  Range& visual_range, Range& node_range) {
    // Validate and exclude the Visual address before reading any of its
    // fields. Then validate/exclude the node address before reading node data.
    if (!range(visual, visual_range) ||
        overlaps(character_range, visual_range) ||
        overlaps(services_range, visual_range) ||
        overlaps(out_range, visual_range) || !visual->identity)
        return false;
    const auto* const root = visual->root;
    if (!range(root, node_range) ||
        overlaps(character_range, node_range) ||
        overlaps(services_range, node_range) ||
        overlaps(out_range, node_range) ||
        overlaps(visual_range, node_range) || !root->identity)
        return false;
    return true;
}
}

Status evaluate(Character* character, const Services* services, Result* out) {
    Range character_range{}, services_range{}, out_range{}, visual_range{},
          node_range{};
    if (!range(character, character_range) ||
        !range(services, services_range) || !range(out, out_range) ||
        overlaps(character_range, services_range) ||
        overlaps(character_range, out_range) ||
        overlaps(services_range, out_range) || !character->identity)
        return Status::invalid_argument;

    if (currently_active(character)) return Status::reentrant_call;
    ActiveGuard active_guard(character);

    const Services bound = *services;
    const auto character_identity = character->identity;
    auto* const captured_visual = character->visual_2d8;
    if (captured_visual) {
        if (!valid_visual(captured_visual, character_range, services_range,
                          out_range, visual_range, node_range))
            return Status::invalid_source_fact;
    }
    *out = {};
    out->captured_visual = captured_visual ? captured_visual->identity : 0;

    // r6 captures Character+0x2d8 once; the first scene-node byte is cleared
    // before GetOnline and remains an observable source effect on later error.
    if (captured_visual) {
        captured_visual->root->update_flag_200 = 0;
        ++out->scene_flag_writes;
    }

    Response response{};
    auto status = query(bound, character, character_identity, *out,
                        Operation::get_online_byte,
                        0, 0, 0, 0, response);
    if (status != Status::complete) return status;
    // GetOnline is a pointer query followed by source LDRB [result+5], so a
    // provider must return the exact byte domain rather than a wider bool word.
    if (response.raw > 0xffu) return Status::invalid_source_fact;
    const bool online = response.raw != 0;

    bool remotely_updated = false;
    if (online) {
        status = query(bound, character, character_identity, *out,
                       Operation::is_remotely_updated,
                       character_identity, 0, 0, 0, response);
        if (status != Status::complete) return status;
        remotely_updated = response.raw != 0;
    }

    // In the offline/non-remotely-updated arm branch, test the captured Visual
    // but reread Character+0x2d8 after GetLocalPlayer returns.
    if (!online || !remotely_updated) {
        if (captured_visual) {
            const auto captured_field_418 = character->field_418;
            status = query(bound, character, character_identity, *out,
                           Operation::local_player_character,
                           character_identity, 0, 0, 1, response);
            if (status != Status::complete) return status;
            if (captured_field_418 != response.identity) {
                auto* const fresh_visual = character->visual_2d8;
                Range fresh_visual_range{}, fresh_node_range{};
                if (!valid_visual(fresh_visual, character_range,
                                  services_range, out_range,
                                  fresh_visual_range, fresh_node_range))
                    return Status::invalid_source_fact;
                if (fresh_visual->root->culling_word_118 != 0 ||
                    character->culling_field_2fc != 0) {
                    status = query(bound, character, character_identity, *out,
                                   Operation::test_culling_before_update,
                                   character_identity, 0x12c, 0, 0,
                                   response);
                    if (status != Status::complete) return status;
                    if (response.raw == 0 && character->culling_gate_1480 == 0) {
                        out->outcome = Outcome::culled;
                        out->can_update = 0;
                        return Status::complete;
                    }
                }
            }
        }
        // Both the offline path and an online object not handled remotely
        // converge on the source +0x34 IsDead gate. Remote-updated objects
        // bypass this gate and go directly to the final +0x80 mark/return.
        status = query(bound, character, character_identity, *out,
                       Operation::is_dead,
                       character_identity, 0, 0, 0, response);
        if (status != Status::complete) return status;
        if (response.raw != 0 && character->current_visibility_80 == 0) {
            status = query(bound, character, character_identity, *out,
                           Operation::can_respawn,
                           character_identity, 0, 0, 0, response);
            if (status != Status::complete) return status;
            if (response.raw == 0) {
                out->outcome = Outcome::dead_cannot_respawn;
                out->can_update = 0;
                return Status::complete;
            }
        }
    }

    // The final mark still targets the Visual captured at function entry, with
    // its root pointer reloaded at this exact point. Character+0x80 is fresh.
    if (captured_visual && character->current_visibility_80 != 0) {
        Range current_visual_range{}, current_root_range{};
        if (!valid_visual(captured_visual, character_range, services_range,
                          out_range, current_visual_range, current_root_range))
            return Status::invalid_source_fact;
        auto* const captured_visual_root = captured_visual->root;
        captured_visual_root->update_flag_200 = 1;
        ++out->scene_flag_writes;
    }
    out->outcome = Outcome::allowed;
    out->can_update = 1;
    return Status::complete;
}

}  // namespace dh2::character_update_eligibility
