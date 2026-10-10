"""Check the original ARM call-set evidence, without executing the original APK.

The paired C++ fixture executes the current BatchNodeCompilerSourceV96. This
test checks its independent Decor/Module expectation against the IDA assembly
and pseudocode export, including both branches into SetVisualObject(NULL).
"""
from pathlib import Path
import re
import struct
import unittest

ROOT = Path(__file__).resolve().parents[3]
EXPORT = ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so"


def branch_target(address, opcode):
    immediate = opcode & 0xFFFFFF
    if immediate & 0x800000:
        immediate -= 0x1000000
    return address + 8 + immediate * 4


class OriginalVisualRetirementArmTest(unittest.TestCase):
    def test_original_family_comparisons_and_retirement_branches(self):
        path = EXPORT / "assembly-functions.asm"
        if not path.is_file():
            self.skipTest("Private original IDA export is unavailable")
        wanted = {
            0x50DF54, 0x50DF58, 0x50DF5C, 0x50DF94, 0x50DFA0,
            0x50DFA8, 0x50DFB4, 0x50DFBC, 0x50DFC0, 0x50DFC8,
            0x50DF24, 0x50DF2C,
        }
        rows = {}
        with path.open(encoding="utf-8") as source:
            for line in source:
                match = re.match(r"^([0-9a-f]{8})  ((?:[0-9a-f]{2} ){3}[0-9a-f]{2})  (.*)", line)
                if match and int(match[1], 16) in wanted:
                    rows[int(match[1], 16)] = (struct.unpack("<I", bytes.fromhex(match[2]))[0], match[3])
                if len(rows) == len(wanted):
                    break
        self.assertEqual(set(rows), wanted)
        self.assertIn('"Module"', rows[0x50DF24][1])
        self.assertIn('"Decor"', rows[0x50DF2C][1])
        # Load the actual CString48 character buffer5c, rather than comparing
        # an unrelated object field or copying the current source predicate.
        self.assertEqual(rows[0x50DF94][0], 0xE595B05C)
        for compare, branch in ((0x50DFA0, 0x50DFA8), (0x50DFB4, 0x50DFBC)):
            self.assertIn("LC_API_STRCMP_0", rows[compare][1])
            self.assertEqual(rows[branch][0] >> 24, 0x0A)  # ARM BEQ
            self.assertEqual(branch_target(branch, rows[branch][0]), 0x50DF54)
        self.assertEqual(rows[0x50DF54][0], 0xE1A00005)  # MOV R0,R5
        self.assertEqual(rows[0x50DF58][0], 0xE3A01000)  # MOV R1,#0
        self.assertIn("GameObject::SetVisualObject", rows[0x50DF5C][1])
        self.assertEqual(rows[0x50DFC0][0], 0xE2866004)  # advance only
        self.assertEqual(branch_target(0x50DFC8, rows[0x50DFC8][0]), 0x50DF6C)

    def test_pseudocode_preserves_the_same_positive_call_set(self):
        path = EXPORT / "pseudocode/0050/0050dc84.c"
        if not path.is_file():
            self.skipTest("Private original IDA pseudocode is unavailable")
        suffix = path.read_text().split("LABEL_29:", 1)[1]
        self.assertRegex(suffix, r'if \( LC_API_STRCMP_0\(v24, "Decor"\) && LC_API_STRCMP_0\(v24, "Module"\) \)')
        self.assertIn("GameObject::SetVisualObject(v22, 0);", suffix.split("else", 1)[1])


if __name__ == "__main__":
    unittest.main()
