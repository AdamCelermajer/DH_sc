#include "runtime_skills_text_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <string_view>

namespace dh::foundation::generic_skills {
namespace {
std::int32_t signed_word(std::uint32_t bits) {
    std::int32_t value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

std::int32_t arm_asr8(std::int32_t value) {
    if (value >= 0) return value / 256;
    return -1 - static_cast<std::int32_t>((~static_cast<std::uint32_t>(value)) >> 8);
}

void append_faery_oid_offset(std::int32_t& oid, std::int32_t offset) {
    const auto bits = static_cast<std::uint32_t>(oid) + static_cast<std::uint32_t>(offset);
    std::memcpy(&oid, &bits, sizeof(oid));
}

struct AuditedSkillInfo {
    const char* script;
    const char* class_token; // ClassTable name passed to SetTempProps
};

// B147: every player skill script whose recovered OnSkillInfo_(slvl) body is
// exactly `SetTempProps(<ID>, slvl);` (optionally followed by an unrelated
// `return cooldown;` / commented line, which the text path never reads). The
// token is the ClassTable name that the script's own
// `local <ID> = GetPyOID("ClassTable", "...")` binds. Generated from the 72
// original data/scripts/skills/prince_*.lua chunks and cross-checked against
// them by runtime_skills_text_v1_tests (third argument). Deliberately absent
// (their OnSkillInfo_ depends on live state this projection has no source
// binding for, so they stay explicitly unavailable):
//   prince_warrior_charge          if (HasShield()) CLASS_ID_SHIELD else CLASS_ID
//   prince_paladin_energy_balance  GetData().BUFF_ID (current faery element)
// prince_mage_magi_shielding is handled separately below: its body first
// does `if (slvl>1000) then slvl=skill_level; end`.
constexpr AuditedSkillInfo kAuditedSkillInfo[] = {
    {"prince_archer_bowmastery", "Skill_Archer_BowMastery"},
    {"prince_archer_exploding_arrows", "Skill_Archer_ExplodingArrows"},
    {"prince_archer_multihit", "Skill_Archer_MultiHitArrow"},
    {"prince_archer_pin_shot", "Skill_Archer_PinShot"},
    {"prince_archer_poison_arrows", "Skill_Archer_PoisonArrows"},
    {"prince_archer_precision", "Skill_Archer_Precision"},
    {"prince_archer_sidestep", "Skill_Archer_SideStep"},
    {"prince_archer_volley", "Skill_Archer_Volley"},
    {"prince_assassin_assassinate", "Skill_Assassin_Assassinate"},
    {"prince_assassin_assassination", "Skill_Assassin_Assassination"},
    {"prince_assassin_blade_deflect", "Skill_Assassin_BladeDeflect"},
    {"prince_assassin_blindness", "Skill_Assassin_Blindness"},
    {"prince_assassin_contaminate", "Skill_Assassin_Contaminate"},
    {"prince_assassin_grapling_hook", "Skill_Assassin_GrapplingHook"},
    {"prince_assassin_poison_sting", "Skill_Assassin_PoisonSting"},
    {"prince_assassin_treasure_hunter", "Skill_Assassin_TreasureHunter"},
    {"prince_berseeker_berserk", "Skill_Berserk_Berserk_Damage"},
    {"prince_berseeker_killing_frenzy", "Skill_Berserk_KillingFrenzy"},
    {"prince_berseeker_killing_streak", "Skill_Berserk_KillingStreak"},
    {"prince_berseeker_ravager", "Skill_Berserk_Ravager"},
    {"prince_berseeker_survival", "Skill_Berserk_Survival"},
    {"prince_berseeker_toughness", "Skill_Berserk_Toughness"},
    {"prince_berseeker_warcry", "Skill_Berserk_Intimidation"},
    {"prince_berseeker_whirling_drift", "Skill_Berserk_WhirlingDrift"},
    {"prince_illusionist_disciples", "Skill_Illusionist_Disciples"},
    {"prince_illusionist_eclipse", "Skill_Illusionist_Eclipse"},
    {"prince_illusionist_figments", "Skill_Illusionist_Figments"},
    {"prince_illusionist_mesmerize", "Skill_Illusionist_Mesmerize"},
    {"prince_illusionist_mirror", "Skill_Illusionist_Mirror"},
    {"prince_illusionist_petrify", "Skill_Illusionist_Petrify"},
    {"prince_illusionist_terror", "Skill_Illusionist_Terror"},
    {"prince_illusionist_timecontrol", "Skill_Illusionist_TimeControl"},
    {"prince_mage_chain_lightning", "Skill_Chain_Lightning"},
    {"prince_mage_coldray", "Skill_Mage_ColdRay"},
    {"prince_mage_cyclone", "Skill_Cyclone"},
    {"prince_mage_elemental_mastery", "Skill_Mage_ElementalMastery"},
    {"prince_mage_faeryblood", "Skill_Mage_FaeryBlood"},
    {"prince_mage_staffmaster", "Skill_Mage_StaffMaster"},
    {"prince_mage_stone_escape", "Skill_Mage_StoneEscape"},
    {"prince_necromancer_carrioneater", "Skill_Necromancer_CarrionEater"},
    {"prince_necromancer_deathclaw", "Skill_Necromancer_DeathClaw"},
    {"prince_necromancer_fireskullrain", "Skill_Necromancer_FireSkullRain"},
    {"prince_necromancer_leecher", "Skill_Necromancer_Leecher"},
    {"prince_necromancer_pestilence", "Skill_Necromancer_Pestilence"},
    {"prince_necromancer_putrid", "Skill_Necromancer_Putrid"},
    {"prince_necromancer_rise", "Skill_Necromancer_Rise"},
    {"prince_necromancer_wrath", "Skill_Necromancer_Wrath"},
    {"prince_paladin_conversion", "Skill_Paladin_Conversion"},
    {"prince_paladin_divine_judgement", "Skill_Paladin_DivineJudgementAura"},
    {"prince_paladin_guarding_aura", "Skill_Paladin_GuardingAura"},
    {"prince_paladin_holy_weapon", "Skill_Paladin_HolyWeapon"},
    {"prince_paladin_offensive_aura", "Skill_Paladin_OffensiveAura"},
    {"prince_paladin_purifying_aura", "Skill_Paladin_PurifyingAura"},
    {"prince_paladin_shield", "Skill_Paladin_GuardianShield"},
    {"prince_rogue_acrobat", "Skill_Rogue_Acrobat"},
    {"prince_rogue_ambidextrous", "Skill_Rogue_Ambidextrous"},
    {"prince_rogue_combomaster", "Skill_Rogue_ComboMaster"},
    {"prince_rogue_findweakness", "Skill_Rogue_FindWeakness"},
    {"prince_rogue_jump_kick", "Skill_Rogue_JumpKick"},
    {"prince_rogue_quickness", "Skill_Rogue_Quickness"},
    {"prince_rogue_roundhouse", "Skill_Rogue_Roundhouse"},
    {"prince_rogue_vicious_strike", "Skill_Rogue_ViciousStrike"},
    {"prince_warrior_bashdown", "Skill_Warrior_BashDown"},
    {"prince_warrior_combat_master", "Skill_Warrior_CombatMaster"},
    {"prince_warrior_fast_metabolism", "Skill_Warrior_FastMetabolism"},
    {"prince_warrior_ground_slam", "Skill_Warrior_GroundSlam"},
    {"prince_warrior_hardiness", "Skill_Warrior_Hardiness"},
    {"prince_warrior_leadership", "Skill_Warrior_Leadership"},
    {"prince_warrior_potion_addict", "Skill_Warrior_PotionAddict"},
};

constexpr const char* kMagiShieldingScript = "prince_mage_magi_shielding";
constexpr const char* kMagiShieldingClass = "Skill_Mage_MagisShielding";

// Returns the SetTempProps class token for an audited script, or null. For
// Magi's Shielding ranks above 1000 the script substitutes its cached
// skill_level, which this pure projection does not own, so those ranks are
// reported as unaudited (null).
const char* audited_skill_info_class(std::string_view script, std::uint32_t rank) {
    for (const auto& entry : kAuditedSkillInfo)
        if (script == entry.script) return entry.class_token;
    if (script == kMagiShieldingScript && rank <= 1000u) return kMagiShieldingClass;
    return nullptr;
}
} // namespace

const char* audited_skill_info_class_v1(std::string_view script, std::uint32_t rank) {
    return audited_skill_info_class(script, rank);
}

struct RuntimeSkillsTextProviderV1::State {
    const dh2::data::CharacterTable* characters{};
    dh2::data::SkillTables::Borrow skills;
    character_menu::MenuLocalization* localization{};
    const CharacterState* profile{};
    int name_word = -1;
    int description_word = -1;
    RuntimeSkillsDetailsBindingsV1 details;
    dh2::ui::HudTextV1* hud_text{};
    dh2::ui::HudTextEnvironmentV1 hud_environment{};

    bool get_text(const CharacterState& same_state, int table_id,
                  std::optional<std::string>& name,
                  std::optional<std::string>& description,
                  std::string& error) const {
        error.clear();
        if (&same_state != profile)
            return fail(error, "Generic skill text callback was given a different CharacterState owner");
        if (!characters || !localization || !skills ||
            table_id < 0 || static_cast<std::size_t>(table_id) >= skills.skills().size())
            return fail(error, "Generic skill text callback has no matching original SkillTable row");
        const auto class_row = std::find(characters->names.begin(), characters->names.end(), profile->class_id);
        if (class_row == characters->names.end())
            return fail(error, "Skill text CharacterState.class_id is absent from original CharacterTable");

        const auto& row = skills.skills()[static_cast<std::size_t>(table_id)];
        auto source_oid = [&](int word, std::optional<std::string>& output) {
            if (word < 0 || static_cast<std::size_t>(word) >= std::size(row.scalar.words))
                return fail(error, "Original SkillTable static text word is outside scalar row");
            std::int32_t oid{};
            std::memcpy(&oid, &row.scalar.words[word], sizeof(oid));
            if (oid < 0) {
                output = std::string{};
                return true;
            }
            std::string localized;
            if (!localization->string_id(oid, localized, error)) return false;
            output = std::move(localized);
            return true;
        };

        std::optional<std::string> next_name, next_description;
        if (!source_oid(name_word, next_name) || !source_oid(description_word, next_description))
            return false;
        name = std::move(next_name);
        description = std::move(next_description);
        error.clear();
        return true;
    }

    // NativeGetSkillDetails (0x445a10, port/engine-ui/hud_initialization_v1.cpp
    // details()) decides the Current/Next strings in this order:
    //   1. character level < SkillTable RequiredLevel: Current =
    //      GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL(RequiredLevel), Next empty. No
    //      script is consulted.
    //   2. otherwise rank == 0: Current = GAMEPLAYMENUS_NEEDS_SKILL_POINTS
    //      (static); rank > 0: Current = the row's current-level OID, parsed
    //      with the ordered display_props of AI_SkillInfo(rank) (script).
    //   3. Next = GAMEPLAYMENUS_MAX_SKILL_LEVEL at the CharacterDesign cap,
    //      else GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING when
    //      CanIncrementSkill rejects, else the next-level OID parsed with the
    //      display_props of AI_SkillInfo(rank + 1) (script).
    // Only the script-dependent parts need an audited OnSkillInfo; the static
    // guard strings do not (B147: they were previously hidden together with the
    // whole panel for every unaudited script, and for every locked skill).
    bool get_level_text(const CharacterState& same_state,
                        const character_menu::SkillPageCurrentNextRequestV1& request,
                        character_menu::SkillPageCurrentNextTextV1& output,
                        std::string& error) const {
        error.clear();
        character_menu::SkillPageCurrentNextTextV1 next;
        next.state = request.state;
        next.skill_list_id = request.skill_list_id;
        next.class_skill_position = request.class_skill_position;
        next.skill_table_id = request.skill_table_id;
        next.saved_rank = request.saved_rank;
        next.character_level = request.character_level;
        next.available = false;
        if (&same_state != profile || request.state != profile)
            return fail(error, "Skill details query was given a different CharacterState owner");
        if (!skills || request.skill_list_id < 0 || request.class_skill_position < 0 ||
            request.skill_table_id < 0 ||
            static_cast<std::size_t>(request.skill_list_id) >= skills.lists().size() ||
            static_cast<std::size_t>(request.class_skill_position) >=
                skills.lists()[static_cast<std::size_t>(request.skill_list_id)].size() ||
            skills.lists()[static_cast<std::size_t>(request.skill_list_id)]
                [static_cast<std::size_t>(request.class_skill_position)] != request.skill_table_id ||
            static_cast<std::size_t>(request.skill_table_id) >= skills.skills().size())
            return fail(error, "Skill details request does not identify the same source SkillList row");
        const auto& skill = skills.skills()[static_cast<std::size_t>(request.skill_table_id)];
        const bool faery_dependent = (skill.scalar.words[6] & 0xffu) != 0;
        if (!details.classes || !details.property_rules || !details.current_resolved ||
            !details.max_skill_level || !details.can_increment || !hud_text || !localization)
            return fail(error, "Audited SkillDetails source tables/property/text providers are incomplete");
        if (request.character_level != same_state.stats.level)
            return fail(error, "Skill details request level differs from the same CharacterState");
        if (request.saved_rank > std::numeric_limits<std::uint16_t>::max())
            return fail(error, "Skill details saved rank is outside the source u16 field");

        const auto required_level = signed_word(skill.scalar.words[8]);
        const auto current_oid = signed_word(skill.scalar.words[12]);
        const auto next_oid = signed_word(skill.scalar.words[17]);
        const auto parse_template = [&](const std::string& input,
                                       const std::vector<dh2::ui::HudTextVariantV1>& arguments,
                                       std::string& text) {
            bool changed = false;
            const auto* values = arguments.empty() ? nullptr : arguments.data();
            return hud_text->parse_ex(input.c_str(), values, arguments.size(),
                                      hud_environment, text, changed, error);
        };
        const auto parse_id = [&](std::int32_t oid,
                                  const std::vector<dh2::ui::HudTextVariantV1>& arguments,
                                  std::string& text) {
            if (oid < 0) { text.clear(); return true; }
            std::string input;
            if (!localization->string_id(oid, input, error)) return false;
            return parse_template(input, arguments, text);
        };
        // Source `named("StrID", key)`: constant lookup, then the raw getString by
        // integer id, then parseEx. (MenuLocalization::symbol() would pre-parse the
        // template as plain text and reject its ^d level argument.)
        const auto parse_symbol = [&](const char* symbol,
                                      const std::vector<dh2::ui::HudTextVariantV1>& arguments,
                                      std::string& text) {
            std::int32_t id{};
            if (!localization->constant("StrID", symbol, id, error)) return false;
            std::string input;
            if (!localization->string_id(id, input, error)) return false;
            return parse_template(input, arguments, text);
        };
        const auto apply_faery_offset = [&](std::int32_t base, std::int32_t& adjusted,
                                            bool& supported) {
            adjusted = base;
            supported = true;
            if (!faery_dependent || base == -1) return true;
            if (!details.faery_oid_offset) {
                supported = false;
                next.unavailable_reason = "Faery-dependent text left blank without same-owner source offset";
                return true;
            }
            std::int32_t offset{};
            if (!details.faery_oid_offset(same_state, offset, error)) {
                if (error.empty()) error = "Same-owner source faery OID offset provider failed";
                return false;
            }
            append_faery_oid_offset(adjusted, offset);
            return true;
        };
        const auto numeric_argument = [](std::int32_t integer, float number) {
            return dh2::ui::HudTextVariantV1{number, integer, nullptr};
        };

        // Step 1: the RequiredLevel lock is decided from the same character
        // level before any script or property sheet is needed.
        if (static_cast<std::int32_t>(request.character_level) < required_level) {
            std::vector<dh2::ui::HudTextVariantV1> required_level_arg{
                numeric_argument(required_level, static_cast<float>(required_level))};
            if (!parse_symbol("GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL", required_level_arg,
                              next.current_level)) return false;
            next.available = true;
            output = std::move(next);
            error.clear();
            return true;
        }

        // The script-dependent display_props of AI_SkillInfo(rank) and
        // (rank + 1). They are evaluated lazily, only when a displayed string
        // actually consumes them, and only for audited OnSkillInfo bodies.
        bool arguments_loaded = false, arguments_supported = false;
        std::vector<dh2::ui::HudTextVariantV1> current_args, next_args;
        const auto load_arguments = [&]() {
            if (arguments_loaded) return true;
            arguments_loaded = true;
            const auto* class_token = audited_skill_info_class(skill.script, request.saved_rank + 1u);
            if (!class_token) {
                next.unavailable_reason = "OnSkillInfo implementation has not been source-audited for this script";
                return true;
            }
            dh2::data::PropertySheet current_resolved{};
            if (!details.current_resolved(same_state, current_resolved, error)) {
                if (error.empty()) error = "Same-player resolved-property snapshot unavailable";
                return false;
            }
            if (arm_asr8(current_resolved[19]) != static_cast<std::int32_t>(request.character_level))
                return fail(error, "Same-player resolved Character level differs from the Skills page level");
            // NativeGetSkillDetails calls OnSkillInfo at rank then rank+1. The
            // audited bodies do only SetTempProps(CLASS_ID, slvl); the second
            // call may be rank 65536, still representable in the signed sheet.
            SkillTempPropertiesV1 current_properties, next_properties;
            if (!evaluate_skill_temp_properties_v1(*details.classes, *details.property_rules,
                    current_resolved, class_token, request.saved_rank, current_properties, error)) return false;
            if (!evaluate_skill_temp_properties_v1(*details.classes, *details.property_rules,
                    current_resolved, class_token, request.saved_rank + 1u, next_properties, error)) return false;
            const auto display_arguments = [&](const dh2::data::PropertySheet& properties,
                                               std::vector<dh2::ui::HudTextVariantV1>& arguments) {
                std::vector<dh2::ui::HudTextVariantV1> fresh;
                fresh.reserve(skill.display_props.size());
                for (const auto property : skill.display_props) {
                    if (property < 0 || property >= static_cast<std::int32_t>(properties.size()))
                        return fail(error, "SkillTable display property id is outside source property sheet");
                    const auto fixed = properties[static_cast<std::size_t>(property)];
                    fresh.push_back(numeric_argument(arm_asr8(fixed),
                                                      static_cast<float>(fixed) * (1.0f / 256.0f)));
                }
                arguments = std::move(fresh);
                return true;
            };
            if (!display_arguments(current_properties.properties, current_args) ||
                !display_arguments(next_properties.properties, next_args)) return false;
            arguments_supported = true;
            return true;
        };

        // Step 2: Current level.
        bool produced_text = false;
        if (request.saved_rank == 0) {
            if (!parse_symbol("GAMEPLAYMENUS_NEEDS_SKILL_POINTS", {}, next.current_level)) return false;
            produced_text = true;
        } else {
            std::int32_t adjusted_oid{};
            bool supported = true;
            if (!apply_faery_offset(current_oid, adjusted_oid, supported)) return false;
            if (supported && adjusted_oid >= 0) {
                if (!load_arguments()) return false;
                if (arguments_supported) {
                    if (!parse_id(adjusted_oid, current_args, next.current_level)) return false;
                    produced_text = true;
                }
            }
        }

        // Step 3: Next level.
        std::int32_t max_skill_level{};
        if (!details.max_skill_level(same_state, max_skill_level, error)) {
            if (error.empty()) error = "Source CharacterDesign skill cap provider failed";
            return false;
        }
        if (max_skill_level <= static_cast<std::int32_t>(request.saved_rank)) {
            if (!parse_symbol("GAMEPLAYMENUS_MAX_SKILL_LEVEL", {}, next.next_level)) return false;
            produced_text = true;
        } else {
            bool can_increment = false;
            if (!details.can_increment(same_state, request.class_skill_position,
                                      can_increment, error)) {
                if (error.empty()) error = "Source CanIncrementSkill provider failed";
                return false;
            }
            if (!can_increment) {
                if (!parse_symbol("GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING", {},
                                  next.next_level)) return false;
                produced_text = true;
            } else {
                std::int32_t adjusted_oid{};
                bool supported = true;
                if (!apply_faery_offset(next_oid, adjusted_oid, supported)) return false;
                if (supported && adjusted_oid >= 0) {
                    if (!load_arguments()) return false;
                    if (arguments_supported) {
                        if (!parse_id(adjusted_oid, next_args, next.next_level)) return false;
                        produced_text = true;
                    }
                }
            }
        }
        // Nothing source-backed to show (unaudited script, no static branch):
        // keep the whole panel unavailable rather than showing bare headings.
        next.available = produced_text;
        output = std::move(next);
        error.clear();
        return true;
    }

private:
    static bool fail(std::string& error, const char* message) {
        error = message;
        return false;
    }
};

bool RuntimeSkillsTextProviderV1::bind(
    const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow skills,
    character_menu::MenuLocalization& localization,
    const CharacterState& same_state,
    ServicesV1& services,
    std::string& error) {
    error.clear();
    if (state_) {
        error = "Runtime Skills text provider can only be bound once";
        return false;
    }
    if (!skills || skills.skills().empty() || characters.names.size() != characters.rows.size()) {
        error = "Runtime Skills text requires actual CharacterTable and SkillTables owners";
        return false;
    }
    if (std::find(characters.names.begin(), characters.names.end(), same_state.class_id) == characters.names.end()) {
        error = "Runtime Skills text requires the same CharacterState's original CharacterTable class row";
        return false;
    }
    for (const char* field : {"SkillName", "SkillDescription", "SkillCurrLevel",
                              "SkillNextLevel", "FairieDependantText"}) {
        if (skills.skill_field(field) < 0) {
            error = std::string("Original SkillTable schema is missing field ") + field;
            return false;
        }
    }
    if (!localization.bind_profile(&same_state, error)) return false;

    auto next = std::make_shared<State>();
    next->characters = &characters;
    next->skills = std::move(skills);
    next->localization = &localization;
    next->profile = &same_state;
    // These are recovered source SkillTable scalar offsets from the actual
    // 76-byte record: SkillName at +0x40, SkillDescription at +0x34.
    next->name_word = 16;
    next->description_word = 13;
    if (!services.localized_text) {
        services.localized_text = [next](const CharacterState& actual, int table_id,
                std::optional<std::string>& name, std::optional<std::string>& description,
                std::string& message) {
            return next->get_text(actual, table_id, name, description, message);
        };
    }
    state_ = std::move(next);
    error.clear();
    return true;
}

bool RuntimeSkillsTextProviderV1::bind_details(
    const RuntimeSkillsDetailsBindingsV1& bindings,
    ServicesV1& services,
    std::string& error) {
    error.clear();
    if (!state_ || !state_->skills || !state_->localization || !state_->profile) {
        error = "Source SkillDetails producer must follow same-owner static Skills text binding";
        return false;
    }
    if (state_->details.classes || services.skill_level_text) {
        error = "Runtime Skills source details provider is already bound";
        return false;
    }
    if (!bindings.classes || !bindings.property_rules || !bindings.current_resolved ||
        !bindings.max_skill_level || !bindings.can_increment) {
        error = "Runtime Skills source details requires exact class/property/cap/increment providers";
        return false;
    }
    dh2::ui::HudTextV1* same_text = nullptr;
    dh2::ui::HudTextEnvironmentV1 same_environment;
    if (!state_->localization->borrow_text(same_text, same_environment, error)) return false;
    if (!same_text) { error = "Source SkillDetails requires the existing HudText owner"; return false; }
    state_->details = bindings;
    state_->hud_text = same_text;
    state_->hud_environment = std::move(same_environment);
    auto source = state_;
    services.skill_level_text = [source](const CharacterState& actual,
            const character_menu::SkillPageCurrentNextRequestV1& request,
            character_menu::SkillPageCurrentNextTextV1& output, std::string& message) {
        return source->get_level_text(actual, request, output, message);
    };
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
