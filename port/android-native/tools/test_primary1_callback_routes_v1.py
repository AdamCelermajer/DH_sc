"""Guard the source-audited primary1 callback bridge against SWF string false positives."""
from pathlib import Path
import json
import hashlib
import re

ROOT = Path(__file__).resolve().parents[3]
actions = json.loads((ROOT / "port/engine-ui/reference/character-menu-flow-v1/authored-actions.json").read_text(encoding="utf-8"))
swf = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf"
assert hashlib.sha256(swf.read_bytes()).hexdigest() == actions["original_sha256"], "call-site evidence no longer matches the primary1 SWF"

called = set()

def visit(rows):
    for index, row in enumerate(rows):
        if row.get("op") in ("call_func", "call_method", "call") and index:
            pushed = rows[index - 1]
            values = pushed.get("values", [])
            names = [value.get("text") for value in values if isinstance(value, dict)]
            # The called Native function is the final string operand before
            # CallFunction. Earlier strings can be trace text or arguments.
            if names and isinstance(names[-1], str) and names[-1].startswith("Native"):
                called.add(names[-1])
        if row.get("body"):
            visit(row["body"])

for block in actions["blocks"]:
    visit(block.get("rows", []))

binding = (ROOT / "port/android-native/app/src/main/cpp/native_process_primary1_binding_v98.inc").read_text(encoding="utf-8")
match = re.search(r"application\.native_actions=\{(.*?)\};", binding, re.S)
assert match, "primary1 must explicitly register its audited callback routes"
registered = set(re.findall(r'"(Native[^"]+)"', match.group(1)))

required = {
    "NativeReloadSkills", "NativeChangeRolloverInputBehavior",
    "NativePushMenu", "NativePopMenu", "NativePopAllAbove", "NativePopAllMenus",
    "NativeGetPlayerStats", "NativeGetSkillDetails", "NativeInvGetItemsListForSlot",
    "NativeInvEquipItem", "NativeSaveGame", "NativeStatsAssignPoint",
    "NativeSkillsTrainSkill", "NativeEquipSkill",
}
assert required <= called, f"expected source call sites are absent: {sorted(required - called)}"
assert required <= registered, f"callable routes are not registered: {sorted(required - registered)}"
assert "NativeInvGetHasTwoHandedWeapon(0) is: " not in called

adapter = (ROOT / "port/android-native/app/src/main/cpp/native_character_menu_v4.inc").read_text(encoding="utf-8")
for action in ("NativePushMenu", "NativePopMenu", "NativePopAllAbove", "NativePopAllMenus"):
    assert f'!std::strcmp(name,"{action}")' in adapter, f"missing shared-stack navigation route {action}"
assert "character_panel->dispatch(live,action,projected,e)" in adapter
assert "character_panel->dispatch_source_loading_v98(live,action,projected,e)" in adapter
print(f"PASS: {len(called)} SWF callback names audited; {len(registered)} primary1 routes registered; false-positive trace string excluded")
