#include "despawn_after_death_v1.hpp"

namespace dh::foundation::despawn {

namespace {

void note(const Services& services, const std::string& line) {
    if (services.log) services.log(line);
}

std::string name_of(std::uint64_t actor) {
    return std::to_string(actor);
}

} // namespace

const Record* DespawnAfterDeathV1::record(std::uint64_t actor) const noexcept {
    const auto found = records_.find(actor);
    return found == records_.end() ? nullptr : &found->second;
}

bool DespawnAfterDeathV1::track(std::uint64_t actor, bool summoned, bool has_clip, std::uint32_t delay_ms,
                                std::string& error) {
    if (records_.count(actor)) {
        error = "Despawn actor is already tracked";
        return false;
    }
    Record record;
    record.summoned = summoned;
    record.has_clip = has_clip;
    record.delay_ms = delay_ms;
    records_.emplace(actor, record);
    error.clear();
    return true;
}

bool DespawnAfterDeathV1::death_ended(std::uint64_t actor, Services& services, std::string& error) {
    const auto found = records_.find(actor);
    if (found == records_.end() || found->second.phase != Phase::dying) {
        error = "Despawn death end requires a tracked dying actor";
        return false;
    }
    if (!services.release_body) {
        error = "Despawn requires the body release owner";
        return false;
    }
    // Source CSDead::OnEvent event 34: the body is released before the timer starts.
    if (!services.release_body(actor, error)) {
        if (error.empty()) error = "Despawn body release failed";
        return false;
    }
    found->second.phase = Phase::corpse;
    found->second.remaining_ms = found->second.delay_ms;
    note(services, "DESPAWN death end actor=" + name_of(actor) + " body=released delay_ms=" +
                       std::to_string(found->second.delay_ms) + (found->second.has_clip ? " clip=Despawn" : " clip=none"));
    error.clear();
    return true;
}

bool DespawnAfterDeathV1::advance(std::uint32_t elapsed_ms, Services& services, std::string& error) {
    for (auto it = records_.begin(); it != records_.end();) {
        auto& record = it->second;
        if (record.phase != Phase::corpse) {
            ++it;
            continue;
        }
        if (record.remaining_ms > elapsed_ms) {
            record.remaining_ms -= elapsed_ms;
            ++it;
            continue;
        }
        // Source event 46 (Dead -> Despawn). With a clip, the Despawn state plays; without one the actor is hidden.
        record.remaining_ms = 0;
        const auto actor = it->first;
        if (record.has_clip) {
            if (!services.play_clip) {
                error = "Despawn clip owner is unavailable";
                return false;
            }
            if (!services.play_clip(actor, error)) {
                if (error.empty()) error = "Despawn clip start failed";
                return false;
            }
            record.phase = Phase::despawning;
            note(services, "DESPAWN delay expired actor=" + name_of(actor) + " state=Despawn");
            ++it;
            continue;
        }
        if (!services.hide) {
            error = "Despawn hide owner is unavailable";
            return false;
        }
        if (!services.hide(actor, error)) {
            if (error.empty()) error = "Despawn hide failed";
            return false;
        }
        note(services, "DESPAWN delay expired actor=" + name_of(actor) + " clip=none hidden");
        const bool summoned = record.summoned;
        if (summoned) {
            if (!services.release_slot) {
                error = "Despawn slot owner is unavailable";
                return false;
            }
            if (!services.release_slot(actor, error)) {
                if (error.empty()) error = "Despawn slot release failed";
                return false;
            }
            note(services, "DESPAWN complete actor=" + name_of(actor) + " slot=released");
        }
        it = records_.erase(it);
    }
    error.clear();
    return true;
}

bool DespawnAfterDeathV1::poll(Services& services, std::string& error) {
    for (auto it = records_.begin(); it != records_.end();) {
        if (it->second.phase != Phase::despawning) {
            ++it;
            continue;
        }
        bool done = false;
        if (!services.finished) {
            error = "Despawn completion owner is unavailable";
            return false;
        }
        if (!services.finished(it->first, done, error)) {
            if (error.empty()) error = "Despawn completion query failed";
            return false;
        }
        if (!done) {
            ++it;
            continue;
        }
        const auto actor = it->first;
        const bool summoned = it->second.summoned;
        if (summoned) {
            if (!services.release_slot) {
                error = "Despawn slot owner is unavailable";
                return false;
            }
            if (!services.release_slot(actor, error)) {
                if (error.empty()) error = "Despawn slot release failed";
                return false;
            }
        }
        note(services, "DESPAWN complete actor=" + name_of(actor) + " state=Limbus" +
                           (summoned ? " slot=released" : ""));
        it = records_.erase(it);
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::despawn
