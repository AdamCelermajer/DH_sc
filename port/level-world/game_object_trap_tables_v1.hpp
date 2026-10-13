#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::game_object_trap_tables_v1 {

struct Bytes { const std::uint8_t* data{}; std::size_t size{}; };

// Structs::TimerTrap row (Arrays::TimerTraps) from game_objects_pyarray. The
// original rows are 32 bytes; TimerTrap::GetDamagerId (0x39d59c) reads the
// `Damage` word, GetDelay (0x39d650) `Delay`, GetHurtChance (0x39d5d0)
// `HurtChance`, GetSound (0x39d61c) `Sound`, GetVisual (0x39d568) `Visual` (a
// GameObjectDict index) and GetScript (0x39d524) `Script`.
struct TimerTrapRow {
    std::string name;
    std::int32_t damage{-1};
    std::int32_t delay{0};
    std::int32_t hurt_chance{0};
    std::string script;
    std::int32_t sound{-1};
    std::int32_t visual{-1};
};

// Structs::GameObjectDamager row (Arrays::GameObjectDamager): the 24-byte rows
// Character::F_GOAttack (0x3b061c) reads. `damage` is the Character immunity
// timer index (negative = none), `min`/`max` the whole damage range.
struct DamagerRow {
    std::string name;
    std::int32_t damage{-1};
    std::int32_t immunity_delay{0};
    std::int32_t max{0};
    std::int32_t min{0};
    std::int32_t type{-1};
};

// Validates the embedded PyData arrays (the original game_objects_pyarraynames
// and pystructnames streams) and retains the TimerTraps and GameObjectDamager
// arrays plus the GameObjectDict visual paths.
class Tables {
public:
    bool load(Bytes game_objects, Bytes game_object_names,
              Bytes game_object_fields, Bytes visual_dictionary,
              Bytes visual_dictionary_names, std::string& error);
    const TimerTrapRow* timer_trap(std::int32_t id) const noexcept;
    const TimerTrapRow* find_timer_trap(std::string_view name,
                                        std::int32_t* id = nullptr) const noexcept;
    const DamagerRow* damager(std::int32_t id) const noexcept;
    const DamagerRow* find_damager(std::string_view name,
                                   std::int32_t* id = nullptr) const noexcept;
    const std::string* visual_path(std::int32_t visual_id) const noexcept;
    std::size_t timer_trap_count() const noexcept { return timer_traps_.size(); }
    std::size_t damager_count() const noexcept { return damagers_.size(); }

private:
    std::vector<TimerTrapRow> timer_traps_;
    std::vector<DamagerRow> damagers_;
    std::vector<std::string> visual_paths_;
};

} // namespace dh2::game_object_trap_tables_v1
