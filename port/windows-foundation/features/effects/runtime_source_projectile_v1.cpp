#include "runtime_source_projectile_v1.hpp"
#include <cstring>
namespace dh::foundation::effects {
namespace {
std::int32_t signed_word(std::uint32_t word) {
  std::int32_t value;
  std::memcpy(&value, &word, 4);
  return value;
}
float float_word(std::uint32_t word) {
  float value;
  std::memcpy(&value, &word, 4);
  return value;
}
bool fail(std::string &error, const char *message) {
  error = message;
  return false;
}
} // namespace
bool runtime_source_projectile_plan_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101> tables,
    std::int32_t selected, RuntimeSourceProjectilePlanV1 &output,
    std::string &error) {
  using Value = dh2::android_ui::ProcessArrayValueV101;
  const auto *group = tables ? tables->group("ProjectileTable") : nullptr;
  if (!tables || !tables->ready() || !group || !group->records_loaded ||
      !group->names_loaded || group->declared_rows != group->rows.size() ||
      selected < 0 ||
      static_cast<std::size_t>(selected) >= group->rows.size() ||
      static_cast<std::size_t>(selected) >= group->names.size())
    return fail(error, "Original ProjectileTable selected row unavailable");
  const auto &fields = group->rows[static_cast<std::size_t>(selected)].fields;
  constexpr const char *shape = "bbiibiibbiiiiibbiiii";
  if (fields.size() != 20)
    return fail(error, "Original ProjectileTable scalar shape changed");
  for (unsigned i = 0; i < 20; ++i) {
    const bool byte = shape[i] == 'b';
    if (fields[i].kind != (byte ? Value::Kind::byte : Value::Kind::word) ||
        (byte && fields[i].bits > 255))
      return fail(error, "Original ProjectileTable scalar kind changed");
  }
  RuntimeSourceProjectilePlanV1 plan;
  plan.tables = std::move(tables);
  plan.row = selected;
  plan.name = group->names[static_cast<std::size_t>(selected)];
  plan.dedicated = fields[0].bits != 0;
  plan.disappear_on_hit = fields[1].bits != 0;
  plan.expire_fx = signed_word(fields[2].bits);
  plan.expire_sound = signed_word(fields[3].bits);
  plan.friendly_fire = fields[4].bits != 0;
  plan.impact_fx = signed_word(fields[5].bits);
  plan.impact_sound = signed_word(fields[6].bits);
  plan.laser = fields[7].bits != 0;
  plan.magic = fields[8].bits != 0;
  plan.max_distance = float_word(fields[9].bits);
  plan.max_rotation = float_word(fields[10].bits);
  plan.model = signed_word(fields[11].bits);
  plan.object_impact_fx = signed_word(fields[12].bits);
  plan.object_impact_sound = signed_word(fields[13].bits);
  plan.stay_on_floor = fields[14].bits != 0;
  plan.target_lock = fields[15].bits != 0;
  plan.time_between_hit_ms = signed_word(fields[16].bits);
  plan.timer_ms = signed_word(fields[17].bits);
  plan.velocity = float_word(fields[18].bits);
  plan.velocity_damping = float_word(fields[19].bits);
  plan.class_name = plan.laser ? "LaserTypeProjectile" : "Projectile";
  if (plan.model >= 0) {
    const auto *models = plan.tables->group("ProjectileDict");
    if (!models || !models->records_loaded ||
        models->declared_rows != models->rows.size() ||
        static_cast<std::size_t>(plan.model) >= models->rows.size())
      return fail(error, "Original ProjectileDict model row unavailable");
    const auto &model =
        models->rows[static_cast<std::size_t>(plan.model)].fields;
    if (model.size() != 1 || model[0].kind != Value::Kind::string ||
        model[0].text.empty())
      return fail(error, "Original ProjectileDict model URI unavailable");
    plan.model_uri = model[0].text;
  }
  output = std::move(plan);
  error.clear();
  return true;
}
bool runtime_source_ranged_event_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101> tables,
    const dh2::data::CombatEventContext &context, const char *marker,
    bool &handled, RuntimeSourceProjectilePlanV1 &output, std::string &error) {
  handled = false;
  dh2::data::CombatEventAction action;
  if (dh2_combat_event_route(&action, &context, marker))
    return fail(error, "Malformed original ranged animation-event context");
  if (action.kind != dh2::data::CombatEventKind::projectile) {
    error.clear();
    return true;
  }
  if (!runtime_source_projectile_plan_v1(std::move(tables),
                                         action.sequence_step, output, error))
    return false;
  handled = true;
  return true;
}
bool runtime_source_range_selection_v1(
    std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101> tables,
    const dh2::character::CombatProperties896 &properties,
    const dh2::character::CombatInventory16 *inventory,
    const dh2::character::CombatItemRecord164 *items, std::uint32_t count,
    bool &capable, RuntimeSourceRangeSelectionV1 &output, std::string &error) {
  capable = false;
  std::int32_t parameters[3]{};
  const int result = dh2_attack_range_parameters(parameters, &properties,
                                                 inventory, items, count);
  if (result < 0)
    return fail(error,
                "Malformed current source range property/equipment loan");
  if (!result) {
    error.clear();
    return true;
  }
  RuntimeSourceRangeSelectionV1 next;
  next.minimum = parameters[0];
  next.maximum = parameters[1];
  if (properties.words[32] == -1) {
    // Reached only after the native query validated this selected item chain.
    next.origin = RuntimeSourceRangeOriginV1::current_equipment;
    next.selected_item =
        (*inventory->sets[inventory->current_set].main_hand)->item_id;
    next.equipment_type = items[next.selected_item].words[22];
  }
  if (!runtime_source_projectile_plan_v1(std::move(tables), parameters[2],
                                         next.projectile, error))
    return false;
  output = std::move(next);
  capable = true;
  error.clear();
  return true;
}
} // namespace dh::foundation::effects
