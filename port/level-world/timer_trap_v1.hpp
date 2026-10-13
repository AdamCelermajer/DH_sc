#pragma once

#include <cstdint>
#include <set>

namespace dh2::timer_trap_v1 {

using Identity = std::uintptr_t;

// Port of the source TimerTrap / TriggerTrap pair (libDungeonHunter2.so):
//   TimerTrap::Create          0x39d724  (spawned by GameObject::_SummonTimerTrap 0x390c50)
//   TimerTrap::InitPost        0x39d978  -> TriggerTrap::InitPost 0x39dee0
//   TimerTrap::SpecificUpdate  0x39d688  (TriggerTrap::SpecificUpdate 0x39f2e8 for the base)
//   TriggerTrap::Update        0x39eedc  (ZoneEx::Update 0x3980a8 prunes the contact set)
//   TriggerTrap::TransferVictims 0x39f168, ::CanTrigger 0x39ea9c, ::Activate 0x39e198
//   TriggerTrap::__Callback    0x39e738  (animation end), ::__EventCallback 0x39ec78
//   TriggerTrap::_Remove       0x39dd18
//
// The object keeps three GameObject sets: the ZoneEx contact set (+0x388, the
// objects whose position is inside the zone), the victim set (+0x3c8, rolled
// this frame) and the hit set (+0x3e0, already struck during this activation).
// Everything that lives in the renderer (the zone geometry, the Lua object
// script, the BDAE timeline, damage application and the ObjectManager row) is
// reached through Services; a missing provider is a required-service failure.
struct Config {
    // TimerTrap::GetDamagerId / GetDelay / GetHurtChance (row) / GetSound.
    std::int32_t damager_id{-1};
    std::int32_t delay_ms{0};
    std::int32_t row_hurt_chance{0};
    std::int32_t sound{-1};
    // LuaScript at +0x300 and VisualObject at +0x2d8 exist.
    bool has_script{false};
    bool has_visual{false};
};

struct State {
    std::int32_t trap_id{-1};        // +0x3c0
    bool active{false};              // +0x3c4
    bool delete_requested{false};    // +0x400
    bool idle_started{false};        // +0x401
    std::int32_t timer_ms{0};        // +0x404
    std::int32_t hurt_override{-1};  // +0x3fc (Create's third argument)
    Identity owner{0};               // +0x3f8
    std::set<Identity> contacts;     // +0x388
    std::set<Identity> victims;      // +0x3c8
    std::set<Identity> hit;          // +0x3e0
};

struct Services {
    void* context{};
    // Application::GetDt (milliseconds).
    std::int32_t (*dt_ms)(void*){};
    // ObjectHandle::operator Character*: whether `object` is a live Character.
    bool (*is_character)(void*, Identity object){};
    // TriggerTrap::CanTrigger: the script's OnTrigger(object) verdict (true when
    // there is no script or the script does not return a boolean false).
    bool (*can_trigger)(void*, Identity object, bool& allowed){};
    // The script's OnDamageRoll(victim) (has_script), reported failure stops.
    bool (*script_damage_roll)(void*, Identity victim){};
    // Character::F_GOAttack(damager, hurt chance) + F_ApplyResult (no script).
    bool (*go_attack)(void*, Identity victim, std::int32_t damager_id,
                      std::int32_t hurt_chance){};
    // The script's OnAnimEvent(name).
    bool (*script_anim_event)(void*, const char* name){};
    // VisualObject controller slot 8: starts a named clip; true when the BDAE
    // has it (a missing clip leaves the current one running).
    bool (*start_clip)(void*, const char* name, bool loop){};
    // VoxSoundManager::Play3D at the trap position (sound id >= 0).
    bool (*play_sound)(void*, std::int32_t sound){};
    // GameObject::SetVisible(false) then ObjectBase::Delete.
    bool (*delete_self)(void*){};
};

class Trap {
public:
    Trap() = default;
    Trap(const Config& config, std::int32_t trap_id, Identity owner,
         std::int32_t hurt_override) noexcept;

    // TriggerTrap::InitPost visual tail + TimerTrap::InitPost: the first timer.
    bool init_post(const Services&);
    // GetHurtChance: Create's override unless it is -1.
    std::int32_t hurt_chance() const noexcept;
    // ZoneEx::OnCollisionBegins / OnCollisionEnds / Update's prune: the object's
    // membership in the contact set follows Zone::IsInside.
    void observe(Identity object, bool inside);
    // TriggerTrap::Update.
    bool update(const Services&);
    // TimerTrap::SpecificUpdate.
    bool specific_update(const Services&);
    // TriggerTrap::TransferVictims.
    bool transfer_victims(const Services&);
    // TriggerTrap::Activate.
    bool activate(const Services&);
    // The BDAE controller's animation-end callback / triggered-event callback.
    bool clip_ended(const Services&);
    bool clip_event(const Services&, const char* name);
    // TriggerTrap::_Remove.
    bool remove(const Services&);

    const Config& config() const noexcept { return config_; }
    const State& state() const noexcept { return state_; }
    State& state() noexcept { return state_; }

private:
    Config config_{};
    State state_{};
};

} // namespace dh2::timer_trap_v1
