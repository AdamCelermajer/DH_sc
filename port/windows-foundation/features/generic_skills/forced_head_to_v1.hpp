#pragma once
// v21.8 w4-grapple: general Cmd_HeadTo(point) owner for a live actor. The Lua form is `target:HeadTo(x, y, z)`
// (Character::_HeadTo 0x3ba71c, source 003ba71c.c): with three numbers it calls v2Controller::Cmd_HeadTo(point), which
// gives the actor a forced heading toward the point that the movement owner follows until it arrives or the command is
// cancelled. This is the engine piece for any skill that drags an actor (GrapplingHook pulls the stunned target to the
// caster); it is not skill-specific.
//
// Port form: one command per actor, stepped once per gameplay frame (advance). Each step moves the actor in the XY plane
// toward the goal by speed * dt, keeps its own Z, and writes the position through the host's move service (the same
// actor-teleport verb the Cyclone carry uses). The command ends when the actor is within arrive_radius of the goal, when
// `may_continue` refuses (dead, not stunned, gone), or when cancel() is called.
//
// Gaps (not recovered from the original, stated in the report): the original steps the actor through its walk locomotion,
// so its pull speed is the authored walk speed; this port uses the speed the caller passes. Collision and floor solving are
// the host move service's concern (no path search: a straight line, as Cmd_HeadTo is a heading, not a route).
#include "../../actor_state.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

struct ForcedHeadToServicesV1 {
    // Current world position of a live actor; false when the actor is absent or dead.
    std::function<bool(ActorId actor, std::array<float, 3>& position)> position_of;
    // Writes the actor position (host teleport verb). False aborts the command with the error.
    std::function<bool(ActorId actor, const std::array<float, 3>& position, std::string& error)> move;
    // Continues the command this frame (the Lua pull runs while the target is Stunned). False ends it without a move.
    std::function<bool(ActorId actor)> may_continue;
};

class ForcedHeadToV1 final {
public:
    explicit ForcedHeadToV1(ForcedHeadToServicesV1 services);

    // Cmd_HeadTo(goal): a new command for the actor replaces its previous one.
    void start(ActorId actor, const std::array<float, 3>& goal, float speed_per_second, float arrive_radius);
    void cancel(ActorId actor);
    bool active(ActorId actor) const noexcept;
    std::vector<ActorId> actors() const;
    std::size_t size() const noexcept { return commands_.size(); }
    void clear() noexcept { commands_.clear(); }

    // Steps every command by dt_ms. Finished commands (arrived, refused, gone) are removed; a move error stops the
    // command and is returned in `error`.
    bool advance(double dt_ms, std::string& error);

    // Last outcome per actor, for diagnostics: "arrived", "refused", "gone", "cancelled" or "" while active.
    const std::string& last_end(ActorId actor) const;

private:
    struct Command {
        std::array<float, 3> goal{};
        float speed = 0.0f;
        float arrive = 0.0f;
    };
    ForcedHeadToServicesV1 services_;
    std::map<ActorId, Command> commands_;
    std::map<ActorId, std::string> last_end_;
};

} // namespace dh::foundation::generic_skills
