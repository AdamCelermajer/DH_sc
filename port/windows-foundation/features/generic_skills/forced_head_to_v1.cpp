#include "forced_head_to_v1.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace dh::foundation::generic_skills {

namespace {
const std::string kEmpty;
}

ForcedHeadToV1::ForcedHeadToV1(ForcedHeadToServicesV1 services) : services_(std::move(services)) {}

void ForcedHeadToV1::start(ActorId actor, const std::array<float, 3>& goal, float speed_per_second, float arrive_radius) {
    Command command;
    command.goal = goal;
    command.speed = speed_per_second;
    command.arrive = arrive_radius;
    commands_[actor] = command;
    last_end_[actor].clear();
}

void ForcedHeadToV1::cancel(ActorId actor) {
    if (commands_.erase(actor) != 0) last_end_[actor] = "cancelled";
}

bool ForcedHeadToV1::active(ActorId actor) const noexcept { return commands_.count(actor) != 0; }

std::vector<ActorId> ForcedHeadToV1::actors() const {
    std::vector<ActorId> out;
    for (const auto& entry : commands_) out.push_back(entry.first);
    return out;
}

const std::string& ForcedHeadToV1::last_end(ActorId actor) const {
    const auto found = last_end_.find(actor);
    return found == last_end_.end() ? kEmpty : found->second;
}

bool ForcedHeadToV1::advance(double dt_ms, std::string& error) {
    error.clear();
    if (dt_ms <= 0.0) return true;
    for (auto it = commands_.begin(); it != commands_.end();) {
        const ActorId actor = it->first;
        const Command command = it->second;
        std::array<float, 3> position{};
        if (!services_.position_of || !services_.position_of(actor, position)) {
            last_end_[actor] = "gone";
            it = commands_.erase(it);
            continue;
        }
        if (services_.may_continue && !services_.may_continue(actor)) {
            last_end_[actor] = "refused";
            it = commands_.erase(it);
            continue;
        }
        const double dx = static_cast<double>(command.goal[0]) - position[0];
        const double dy = static_cast<double>(command.goal[1]) - position[1];
        const double distance = std::hypot(dx, dy);
        if (distance <= static_cast<double>(command.arrive)) {
            last_end_[actor] = "arrived";
            it = commands_.erase(it);
            continue;
        }
        const double step = std::min(distance, static_cast<double>(command.speed) * dt_ms / 1000.0);
        std::array<float, 3> next = position;
        next[0] = static_cast<float>(position[0] + dx / distance * step);
        next[1] = static_cast<float>(position[1] + dy / distance * step);
        std::string move_error;
        if (!services_.move || !services_.move(actor, next, move_error)) {
            error = move_error.empty() ? "forced HeadTo move service is missing" : move_error;
            last_end_[actor] = "gone";
            commands_.erase(it);
            return false;
        }
        ++it;
    }
    return true;
}

} // namespace dh::foundation::generic_skills
