#include "timer_trap_v1.hpp"

#include <vector>

namespace dh2::timer_trap_v1 {

Trap::Trap(const Config& config, std::int32_t trap_id, Identity owner,
           std::int32_t hurt_override) noexcept
    : config_(config) {
    state_.trap_id = trap_id;
    state_.owner = owner;
    state_.hurt_override = hurt_override;
}

std::int32_t Trap::hurt_chance() const noexcept {
    // TimerTrap::GetHurtChance 0x39d5d0: the +0x3fc word unless it is -1.
    return state_.hurt_override != -1 ? state_.hurt_override : config_.row_hurt_chance;
}

bool Trap::init_post(const Services& services) {
    // TriggerTrap::InitPost 0x39dee0, after MeetCondition: the controller
    // callbacks are installed and "spawn" is requested once; a BDAE without it
    // (the steam clouds) goes straight to the looping "idle" and marks the
    // spawn animation as consumed. TimerTrap::InitPost then arms the timer.
    if (config_.has_visual) {
        if (!services.start_clip) return false;
        if (!services.start_clip(services.context, "spawn", false)) {
            state_.idle_started = true;
            services.start_clip(services.context, "idle", true);
        }
    }
    state_.timer_ms = config_.delay_ms;
    return true;
}

void Trap::observe(Identity object, bool inside) {
    // ZoneEx::OnCollisionBegins inserts an object Zone::IsInside accepts;
    // OnCollisionEnds and ZoneEx::Update erase one it rejects.
    if (inside) state_.contacts.insert(object);
    else state_.contacts.erase(object);
}

bool Trap::transfer_victims(const Services& services) {
    // An object already struck this activation, or already rolled this frame,
    // is skipped; the rest need the script's OnTrigger approval.
    for (const auto object : state_.contacts) {
        if (state_.hit.count(object) || state_.victims.count(object)) continue;
        bool allowed = true;
        if (!services.can_trigger || !services.can_trigger(services.context, object, allowed))
            return false;
        if (allowed) state_.victims.insert(object);
    }
    return true;
}

bool Trap::activate(const Services& services) {
    if (config_.has_visual) {
        state_.active = true;
        if (!services.start_clip) return false;
        services.start_clip(services.context, "activate", false);
    } else if (!state_.hit.empty()) {
        state_.hit.clear();
    }
    if (state_.trap_id >= 0 && config_.sound >= 0 && services.play_sound)
        return services.play_sound(services.context, config_.sound);
    return true;
}

bool Trap::specific_update(const Services& services) {
    if (state_.trap_id >= 0) {
        if (!services.dt_ms) return false;
        state_.timer_ms -= services.dt_ms(services.context);
        if (state_.timer_ms <= 0) {
            if (!state_.active) {
                state_.timer_ms = config_.delay_ms;
                if (!activate(services)) return false;
            } else {
                state_.timer_ms = 0;
            }
        }
    }
    return state_.active ? transfer_victims(services) : true;
}

bool Trap::update(const Services& services) {
    if (!specific_update(services)) return false;
    if (state_.trap_id >= 0) {
        // The script's OnDamageRoll / F_GOAttack run per rolled victim.
        const std::vector<Identity> rolled(state_.victims.begin(), state_.victims.end());
        for (const auto victim : rolled) {
            if (!services.is_character || !services.is_character(services.context, victim))
                continue;
            if (config_.has_script) {
                if (!services.script_damage_roll ||
                    !services.script_damage_roll(services.context, victim))
                    return false;
            } else {
                if (!services.go_attack ||
                    !services.go_attack(services.context, victim, config_.damager_id,
                                        hurt_chance()))
                    return false;
            }
        }
    }
    for (const auto victim : state_.victims) state_.hit.insert(victim);
    state_.victims.clear();
    return true;
}

bool Trap::clip_ended(const Services& services) {
    if (state_.delete_requested)
        return services.delete_self ? services.delete_self(services.context) : false;
    if (state_.active) {
        state_.active = false;
        state_.hit.clear();
        if (config_.has_visual && services.start_clip)
            services.start_clip(services.context, "idle", true);
    } else if (!state_.idle_started) {
        state_.idle_started = true;
        if (config_.has_visual && services.start_clip)
            services.start_clip(services.context, "idle", true);
    }
    return true;
}

bool Trap::clip_event(const Services& services, const char* name) {
    if (!config_.has_script || !name) return true;
    return services.script_anim_event ? services.script_anim_event(services.context, name)
                                      : false;
}

bool Trap::remove(const Services& services) {
    state_.delete_requested = true;
    if (!config_.has_visual || !services.start_clip ||
        !services.start_clip(services.context, "remove", false))
        return services.delete_self ? services.delete_self(services.context) : false;
    return true;
}

} // namespace dh2::timer_trap_v1
