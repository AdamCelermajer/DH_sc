#pragma once
// v21.8 mage: host-owned Pre/Use arms for skills whose Lua is not a plain target-list SkillCombatRoll loop.
//
// ChainLightning, Cyclone and StoneEscape spawn skill projectiles and finish their work in `delayed` callbacks or
// timers (see prince_mage_*.luac). The cast coordinator still owns everything it already owns for every skill
// (SkillTable admission, HasMana/UseMana, the Skill6 sequence, LookAt, Post/ClearTarget). For a profile row with
// `external_owner` it hands the authored OnPreSkill_/OnSkill_ bodies to the owner supplied by the host, passing the
// same facts the Lua reads (target list, SnS_Level class projection, population order).
#include "runtime_skill_target_query_v1.hpp"
#include "runtime_skill_use_profile_v1.hpp"
#include "../../combat_session.hpp"
#include "../../../game-data/properties.hpp"

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

struct RuntimeSkillExternalContextV1 {
    ActorId caster = invalid_actor_id;
    std::uint64_t generation = 0;
    const RuntimeSkillUseProfileV1* profile = nullptr;
    std::string skill_name;
    std::uint32_t rank = 0;
    std::uint32_t row_mask = 0;
    std::int32_t element = -1, category = -1;
    // Use only: the resolved sheet after `SetProp(SnS_Level)` + `ApplyPropClass(CLASS_ID)`. Its SnS_* fields are what the
    // Lua reads with GetProp (min/max damage, TempProp1/2, ...), and it is the F_SkillAttack attacker formula sheet.
    const dh2::data::PropertySheet* formula_sheet = nullptr;
    const dh2::data::CharacterTable* characters = nullptr;
    const dh2::data::ClassTables* classes = nullptr;
    const dh2::data::PropertyRules* rules = nullptr;
    // Pre: the OnPre TargetListSearch order. Use: the Use-time list under the profile's `use_targets` mode.
    const std::vector<ActorId>* ordered_character_targets = nullptr;
    // The same population order / source facts the generic Pre query consumed (for owner-side TargetListSearch).
    const std::vector<ActorId>* population_order = nullptr;
    const std::vector<SkillTargetSourceFactsV1>* target_facts = nullptr;
    std::optional<bool> non_character_attackable_objects_absent;
    // Use only, synchronous: `SetProp(SnS_Level)` + `ApplyPropClass(<class token>)` for another ClassTable row of the same
    // skill (Cyclone_Proximity), evaluated against the live caster sheet exactly like the primary formula sheet.
    std::function<bool(const std::string& class_token, dh2::data::PropertySheet& output, std::string& error)> class_projection;
};

class RuntimeSkillExternalOwnerV1 {
public:
    virtual ~RuntimeSkillExternalOwnerV1() = default;
    virtual bool handles(const RuntimeSkillUseProfileV1&) const = 0;
    // OnPreSkill_ body after the generic target query, LookAt and UseMana (Focus Pre).
    virtual bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) = 0;
    // OnSkill_ body at the retained state-6 do_skill marker.
    virtual bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) = 0;
};

} // namespace dh::foundation::generic_skills
