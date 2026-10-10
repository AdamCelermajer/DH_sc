#pragma once
#include "../../../android-native/app/src/main/cpp/source_process_arrays_v101.hpp"
#include "../../../game-data/combat_events.hpp"
#include "../../../level-world/character_attack_geometry.hpp"
#include <memory>
namespace dh::foundation::effects {
// Authored launch/render plan only. Neither a simulated projectile nor a hit.
// Retaining the original tables keeps the selected row and model URI alive.
struct RuntimeSourceProjectilePlanV1 {
  std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101> tables;
  std::int32_t row{-1}, model{-1};
  std::string name, class_name, model_uri;
  float velocity{}, velocity_damping{}, max_distance{}, max_rotation{};
  std::int32_t timer_ms{}, time_between_hit_ms{};
  std::int32_t expire_fx{-1}, expire_sound{-1}, impact_fx{-1}, impact_sound{-1},
      object_impact_fx{-1}, object_impact_sound{-1};
  bool dedicated{}, disappear_on_hit{}, friendly_fire{}, laser{}, magic{},
      stay_on_floor{}, target_lock{};
  bool requires_model_duration() const noexcept { return timer_ms == -666; }
};
// Caller supplies the original selected row. No fallback/row alias is guessed.
bool runtime_source_projectile_plan_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>,
    std::int32_t row, RuntimeSourceProjectilePlanV1 &, std::string &);
// Reuses original state5 event decision, AFTER caller's actual event admission.
// can_range and projectile must come from real Character/inventory parameters.
// handled=false preserves output; this introduces no clock or deduplication.
bool runtime_source_ranged_event_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>,
    const dh2::data::CombatEventContext &, const char *authored_marker,
    bool &handled, RuntimeSourceProjectilePlanV1 &, std::string &);
enum class RuntimeSourceRangeOriginV1 { cached_property32, current_equipment };
struct RuntimeSourceRangeSelectionV1 {
  RuntimeSourceRangeOriginV1 origin{
      RuntimeSourceRangeOriginV1::cached_property32};
  std::int32_t minimum{}, maximum{}, selected_item{-1}, equipment_type{-1};
  RuntimeSourceProjectilePlanV1 projectile;
};
// Stateless borrow of the caller's actual cached properties, current equipment,
// and decoded ItemTable query projection. No inventory/capability is
// fabricated. Native property32 shortcut does not inspect inventory or
// ItemTable at all. capable=false preserves selection; malformed/missing
// selected row is atomic.
bool runtime_source_range_selection_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>,
    const dh2::character::CombatProperties896 &,
    const dh2::character::CombatInventory16 *,
    const dh2::character::CombatItemRecord164 *, std::uint32_t item_count,
    bool &capable, RuntimeSourceRangeSelectionV1 &, std::string &);
} // namespace dh::foundation::effects
