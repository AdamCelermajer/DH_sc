"""Execute the original ItemInventory selectors used by saved skill slots."""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TESTS = ROOT / "port/game-data/tests"
sys.path.insert(0, str(TESTS))
from items_differential_v4_original import Original  # noqa: E402

ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
SKILL_GETTER = 0x3FC6A0
EQUIPMENT_GETTER = 0x3FC6A8
SKILL_BYTES = bytes.fromhex("0000a0e31eff2fe1")
EQUIPMENT_BYTES = bytes.fromhex("000051e3030000ba011041e2010051e30000a0831eff2f81de02d0e11eff2fe1")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--elf", type=Path, required=True)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    elf = args.elf.resolve()
    raw_sha = hashlib.sha256(elf.read_bytes()).hexdigest()
    assert raw_sha == ELF_SHA256, raw_sha

    original = Original(elf, {"functions": []})
    assert bytes(original.uc.mem_read(SKILL_GETTER, len(SKILL_BYTES))) == SKILL_BYTES
    assert bytes(original.uc.mem_read(EQUIPMENT_GETTER, len(EQUIPMENT_BYTES))) == EQUIPMENT_BYTES

    owner = original.data + 0x9000
    selectors = [-9, -1, 0, 1, 2, 3, 99, 0x7FFFFFFF]
    comparisons = []
    for selected_byte in (0, 1, 0xFF):
        original.uc.mem_write(owner, bytes(0x40))
        original.uc.mem_write(owner + 0x2E, bytes([selected_byte]))
        selected_signed = selected_byte if selected_byte < 0x80 else selected_byte - 0x100
        for selector in selectors:
            current_skill_set = original.invoke(
                SKILL_GETTER, [owner, selector & 0xFFFFFFFF], budget=64
            )
            expected_equipment = (
                selected_signed if selector < 0 or 1 <= selector <= 2 else 0
            )
            current_equipment_set = original.invoke(
                EQUIPMENT_GETTER, [owner, selector & 0xFFFFFFFF], budget=64
            )
            if current_equipment_set >= 0x80000000:
                current_equipment_set -= 0x100000000
            assert current_skill_set == 0
            assert current_equipment_set == expected_equipment, (
                selected_byte, selector, current_equipment_set, expected_equipment
            )
            comparisons.append({
                "selected_byte": selected_byte,
                "selector": selector,
                "current_skill_set": current_skill_set,
                "current_equipment_set": current_equipment_set,
                "expected_equipment_set": expected_equipment,
            })

    report = {
        "validation": "PASS",
        "original_elf_sha256": raw_sha,
        "original_executed_function_count": 2,
        "comparisons": len(comparisons),
        "get_current_skill_set": {
            "symbol": "_ZN13ItemInventory18GetCurrentSkillSetEi",
            "address": hex(SKILL_GETTER),
            "size": len(SKILL_BYTES),
            "bytes": SKILL_BYTES.hex(),
            "result": "constant zero; ignores this and selector",
        },
        "get_current_equipment_set": {
            "symbol": "_ZNK13ItemInventory18GetCurrentEquipSetEi",
            "address": hex(EQUIPMENT_GETTER),
            "size": len(EQUIPMENT_BYTES),
            "bytes": EQUIPMENT_BYTES.hex(),
            "result": "selected signed byte for negative or selectors 1..2; zero for selector 0 or >2",
        },
        "cases": comparisons,
        "saved_map_effect": "GetCurrentSkillSet(-1) is zero, so source SG_SetSkillInSlot/SG_GetSkillInSlot/SG_HasSkillSlots use saved map zero regardless of selected equipment set.",
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "comparisons": len(comparisons),
                      "report": args.report.resolve().as_posix()}))


if __name__ == "__main__":
    main()
