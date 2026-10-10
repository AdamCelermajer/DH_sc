import json
import pathlib
import re
import struct
import sys


root = pathlib.Path(sys.argv[1]).resolve()
scene_report = json.loads((root / "reports/fidelity-video-audit-v18.json").read_text(encoding="utf-8"))
entries = scene_report["population"]["inventory"]
boss = next(row for row in entries if row.get("name") == "_prim_Boss_SwampKing_01")
props = boss["properties"]
assert boss["module"] == "bossroom_ruins_ns__8"
assert props["charpropsname"] == "SwampKing"
assert props["activate_cond"] == "IsBefore_Swamp_Escape"
assert props["auto_spawn"] == "0"
assert boss["diagnostic_spawn_runtime"] == "skipped"

def read_native_names(path):
    data = pathlib.Path(path).read_bytes()
    count, = struct.unpack_from("<I", data, 0)
    offset, names = 4, []
    for _ in range(count):
        length, = struct.unpack_from("<I", data, offset)
        offset += 4
        names.append(data[offset : offset + length].decode("utf-8"))
        offset += length
    assert offset == len(data)
    return names

condition_dir = root / "port/windows-foundation/assets/original-cache/data/pydata"
condition_names = read_native_names(condition_dir / "v2conditions_pyarraynames.bin")
condition_rows = (condition_dir / "v2conditions_pyarray.bin").read_bytes()
count, = struct.unpack_from("<I", condition_rows, 0)
offset, decoded_conditions = 4, {}
for row_index, name in enumerate(condition_names):
    stub_count, = struct.unpack_from("<I", condition_rows, offset)
    offset += 4
    stubs = []
    for _ in range(stub_count):
        stubs.append(struct.unpack_from("<iii", condition_rows, offset))
        offset += 12
    row_type, = struct.unpack_from("<i", condition_rows, offset)
    offset += 4
    decoded_conditions[name] = (row_index, stubs, row_type)
assert len(condition_names) == count and offset == len(condition_rows)
assert decoded_conditions["IsBefore_Swamp_Escape"] == (71, [(1, 50, 13)], 0)
quest_names = read_native_names(root / ".local-inputs/runtime-source-projectile-v1/assets/pydata/v2quests_pyarraynames.bin")
assert quest_names[50] == "Swamp_Escape"
constants = json.loads((root / ".local-inputs/publication/checkpoint/port/script-runtime/reference/design-bindings/cache-constant-inventory.json").read_text(encoding="utf-8"))
quest_state_constants = constants["all_constant_values"]["v2QuestState"][0]["values"]
assert quest_state_constants["PostClosed"] == 13 and quest_state_constants["Count"] == 14
plan_source = (root / "port/windows-foundation/features/bosses/runtime_boss_encounter_plan_v1.cpp").read_text(encoding="utf-8")
assert '{"IsBefore_Swamp_Escape", 50, 1, 13}' in plan_source
native_conditions = (root / "port/level-world/native_conditions_runtime_v69.cpp").read_text(encoding="utf-8")
assert "truth=stub.argument_c>*state.state0" in native_conditions
assert "case 1:" in plan_source and "predicate.argument_c > actual_quest_state" in plan_source
native_enable = (root / "port/level-world/object_enable_condition_v2.cpp").read_text(encoding="utf-8")
assert "if(character&&(!save||!byte14))return true;" in native_enable
assert "if(!*b.tested_ac&&*b.condition_a8)" in native_enable
assert "object_test_enable_condition_v2(fields,services,mark,enabled,e)" in (root / "port/windows-foundation/features/encounters/encounter_services.cpp").read_text(encoding="utf-8")
print("actual v2Conditions row to Quest 50 to PostClosed=13 predicate and ObjectBase enable caller: PASS")

scripts = root / ".local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts"
core = (scripts / "ai/swampking_core.luac").read_text(encoding="utf-8")
assert re.search(r"local\s+phase2_initPct\s*=\s*75", core)
assert re.search(r"if\(SK_PHASE_ID\s*==\s*1\s+and\s+HPPercentCurr\s*<=\s*phase2_initPct", core)
assert re.search(r"if\(HPPercentCurr\s*<=\s*25\s+and\s*\(not\s+weakAnim_flag\)\)", core)
assert re.search(r"local\s+ATTACK_DELAY\s*=\s*1500", core)
assert re.search(r"local\s+GOTODIVE_DELAY\s*=\s*15000", core)
assert re.search(r"local\s+INDIVEPATTERN_DELAY\s*=\s*8500", core)
ready = core[core.index("function isAttackReady") : core.index("-- Attack timer reset function", core.index("function isAttackReady"))] if core.index("-- Attack timer reset function") > core.index("function isAttackReady") else core[core.index("function isAttackReady") : core.index("function RegisterAnims")]
assert "HasTarget()" in ready
assert "SK_STATE == ST_IDLE" in ready and 'GetPyCst("AIStates", "Skill")' in ready
assert "not canDiveNow" in ready
assert re.search(r"if\s*\(state == GetPyCst\(\"AIStates\", \"Idle\"\)\)\s*then\s*allowed = true", core)
assert "if (allowed) then" in core[core.index("function swampking_OnTargetInRangedRange") : core.index("function swampking_OnTargetInCloseRange")]
ready_source = core[core.index("function isAttackReady") : core.index("-- Attack timer reset function", core.index("function isAttackReady"))] if core.index("-- Attack timer reset function") > core.index("function isAttackReady") else core[core.index("function isAttackReady") : core.index("function RegisterAnims")]
assert "SK_STATE == ST_IDLE" in ready_source and "GetPyCst(\"AIStates\", \"Skill\")" in ready_source
assert "not canDiveNow" in ready_source
assert "HasTarget()" in ready_source and "LookAtTarget()" in ready_source
assert ready_source.index("if (HasTarget())") < ready_source.index("if (SK_STATE == ST_IDLE")
assert '"Idle": 3' in (root / ".local-inputs/publication/checkpoint/port/script-runtime/reference/design-bindings/cache-constant-inventory.json").read_text(encoding="utf-8")
assert '"Skill": 6' in (root / ".local-inputs/publication/checkpoint/port/script-runtime/reference/design-bindings/cache-constant-inventory.json").read_text(encoding="utf-8")
assert "StartTimerCB(500, false, GoToDivePattern)" in core
assert "SetSwampkingInEmergeState" in core
assert "StartTimerCB(DIVE_DELAY, false, SetSwampkingInEmergeState)" in core
assert "StartTimerCB(3000, false, TargetAgain)" in core
assert "StartTimerCB(500, false, DiveAttackDecision)" in core
assert "DoSkill(3);" in core
assert "if (diveCounter < 2) then" in core and "diveCounter = 0;" in core
assert "DoSkill(1);" in core and "DoSkill(0);" in core and "DoSkill(2);" in core
phase2 = core[core.index("function Do_MeleeAttackPhase2") : core.index("-- Spawn point dive function")]
assert "prob_pct < normalMeleeAttack2_prob[1]" in phase2
assert "prob_pct < (normalMeleeAttack2_prob[1] + normalMeleeAttack2_prob[2]) and prob_pct > normalMeleeAttack2_prob[1]" in phase2
assert re.search(
    r"elseif\s*\(prob_pct < \(normalMeleeAttack2_prob\[1\] \+ normalMeleeAttack2_prob\[2\]\) and prob_pct > normalMeleeAttack2_prob\[1\]\)\s*then\s*DoSkill\(1\);\s*end\s*end\s*end",
    phase2,
) # The source branch ends without a catch-all attack for the remaining rolls.

skill_expectations = {
    "swamp_king_attack_1.luac": ("SwampKingSkill5PCT", 300.0, "SkillCombatRoll(target)"),
    "swamp_king_attack_2.luac": ("SwampKingSkill30PCT", 200.0, "SkillCombatRoll(target)"),
    "swamp_king_attack_3.luac": ("SwampKingSkillPoisonAttack", 400.0, "fx_swampking_attack_03_poisonspit"),
    "swamp_king_attack_4.luac": ("SwampKingSkillAOEPushback", 400.0, "fx_swampking_blue_shockwave"),
}
for name, expected in skill_expectations.items():
    text = (scripts / "skills" / name).read_text(encoding="utf-8")
    class_id, range_value, effect = expected
    assert class_id in text and effect in text, name
    assert re.search(rf"local\s+RANGE\s*=\s*{range_value:.1f}", text), name
spit = (scripts / "skills/swampking_atk_spit.luac").read_text(encoding="utf-8")
assert "SwampKingSkillPoisonAttackAOE" in spit
assert re.search(r"local\s+AOE_RANGE\s*=\s*250\.0", spit)
assert "GetProp(GetPyStruct(\"CharacterProperties\", \"RangeMaxDistance\"))" in spit
assert "SummonTimerTrap(TIMERTRAP_ID, TIMERTRAP_DAM_ID" in spit
class_table = json.loads((root / ".local-inputs/class-tables-parsed.json").read_text(encoding="utf-8"))
actual_classes = {row["name"] for row in class_table["rows"]}
assert {
    "SwampKingSkill5PCT",
    "SwampKingSkill30PCT",
    "SwampKingSkillAOEPushback",
    "SwampKingSkillPoisonAttack",
    "SwampKingSkillPoisonAttackAOE",
} <= actual_classes
print("authored boss MGP row, AI script transitions/gates, and five skill-source rows: PASS")
