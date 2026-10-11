"""Host regressions for the data-only PyData script decoder."""
import os
from pathlib import Path
import struct
import sys
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
import pydata_scripts as decoder  # noqa: E402
from pydata_scripts import (  # noqa: E402
    DecodeError,
    MAX_ARRAY_ITEMS,
    MAX_COMMANDS_PER_SCRIPT,
    MAX_SCRIPTS,
    MAX_STRING_BYTES,
    decode_script_table,
    parse_programs,
    parse_script_names,
    resolve_script_id,
)


def name_table(*names):
    out = bytearray(struct.pack("<I", len(names)))
    for name in names:
        encoded = name.encode("utf-8")
        out += struct.pack("<I", len(encoded)) + encoded
    return bytes(out)


def program_table(*scripts):
    out = bytearray(struct.pack("<I", len(scripts)))
    for commands in scripts:
        out += struct.pack("<I", len(commands))
        for command in commands:
            out += command
    return bytes(out)


def command(command_id, payload=b""):
    return struct.pack("<I", command_id) + payload


def string(value):
    encoded = value.encode("utf-8")
    return struct.pack("<I", len(encoded)) + encoded


class DecoderTests(unittest.TestCase):
    def test_small_valid_tables_and_case_insensitive_name_resolution(self):
        names = name_table("Common", "LevelOnly")
        programs = program_table(
            [command(12)],
            [command(26, struct.pack("<i", -500))],
        )
        table = decode_script_table(names, programs)
        self.assertEqual(table.names, ("Common", "LevelOnly"))
        self.assertEqual(table.scripts[0].commands[0].name, "WaitDialog")
        self.assertEqual(table.scripts[1].commands[0].payload, (-500,))
        self.assertEqual(resolve_script_id(table.names, "levelonly", start_index=1), 1)
        self.assertEqual(resolve_script_id(table.names, "common", start_index=1), -1)

    def test_character_menu_tutorial_queue_commands(self):
        enqueue = command(74, string("Skills") + string("menu_Character") +
                          struct.pack("<i", 341))
        table = decode_script_table(name_table("TutorialQueue"),
                                    program_table([enqueue, command(75), command(76)]))
        commands = table.scripts[0].commands
        self.assertEqual([item.name for item in commands], [
            "EnqueueCharMenuTutorialMessage", "SkipCharMenuTutorialMessage",
            "SkipAllCharMenuTutorialMessages"])
        self.assertEqual(commands[0].payload, ("Skills", "menu_Character", 341))

    def test_start_dialog_id_uses_one_signed_dialog_index(self):
        decoded = decode_script_table(
            name_table("DialogId"),
            program_table([command(11, struct.pack("<i", -1))]),
        )
        item = decoded.scripts[0].commands[0]
        self.assertEqual(item.name, "StartDialogID")
        self.assertEqual(item.payload, (-1,))

    def test_exec_script_vector_and_non_aligned_bool_layout(self):
        payload = b"\x01" + struct.pack("<iIiiB", 1, 2, -8, 9, 1)
        decoded, consumed = parse_programs(program_table([command(0, payload)]), 1)
        self.assertEqual(consumed, len(program_table([command(0, payload)])))
        self.assertEqual(decoded[0].commands[0].payload, (True, 1, (-8, 9), True))

    def test_rejects_truncated_names_and_program_records(self):
        with self.assertRaises(DecodeError):
            parse_script_names(b"\x01\x00")
        with self.assertRaises(DecodeError):
            parse_script_names(struct.pack("<II", 1, 4) + b"ab")
        with self.assertRaises(DecodeError):
            parse_programs(program_table([struct.pack("<I", 26)]), 1)
        with self.assertRaises(DecodeError):
            parse_programs(program_table([b"\x1a\x00"]), 1)

    def test_rejects_unknown_ids_mismatched_counts_and_trailing_bytes(self):
        with self.assertRaisesRegex(DecodeError, "unsupported command ID"):
            parse_programs(program_table([command(4)]), 1)
        with self.assertRaisesRegex(DecodeError, "does not match"):
            parse_programs(program_table(), 2)
        with self.assertRaisesRegex(DecodeError, "trailing bytes"):
            parse_script_names(name_table("one") + b"\x00")
        with self.assertRaisesRegex(DecodeError, "trailing bytes"):
            parse_programs(program_table() + b"\x00", 0)

    def test_rejects_invalid_boolean_utf8_and_oversized_strings(self):
        set_camera_target = command(8, struct.pack("<iI", 1000, 0) + b"\x02")
        with self.assertRaisesRegex(DecodeError, "invalid bool"):
            parse_programs(program_table([set_camera_target]), 1)
        with self.assertRaisesRegex(DecodeError, "invalid UTF-8"):
            parse_script_names(struct.pack("<II", 1, 1) + b"\xff")
        with self.assertRaisesRegex(DecodeError, "exceeds"):
            parse_script_names(struct.pack("<II", 1, MAX_STRING_BYTES + 1))

    def test_rejects_count_limits_before_allocating(self):
        with self.assertRaisesRegex(DecodeError, "script-name count exceeds"):
            parse_script_names(struct.pack("<I", MAX_SCRIPTS + 1))
        with self.assertRaisesRegex(DecodeError, "command count exceeds"):
            parse_programs(struct.pack("<II", 1, MAX_COMMANDS_PER_SCRIPT + 1), 1)
        exec_payload = b"\x00" + struct.pack("<iI", 0, MAX_ARRAY_ITEMS + 1)
        with self.assertRaisesRegex(DecodeError, "array count exceeds"):
            parse_programs(program_table([command(0, exec_payload)]), 1)

    def test_rejects_aggregate_string_and_table_byte_limits(self):
        original_string_limit = decoder.MAX_TOTAL_STRING_BYTES
        original_table_limit = decoder.MAX_TABLE_BYTES
        try:
            decoder.MAX_TOTAL_STRING_BYTES = 3
            with self.assertRaisesRegex(DecodeError, "table strings exceed"):
                parse_script_names(name_table("ab", "cd"))
            decoder.MAX_TABLE_BYTES = 3
            with self.assertRaisesRegex(DecodeError, "table exceeds"):
                parse_script_names(name_table("one"))
        finally:
            decoder.MAX_TOTAL_STRING_BYTES = original_string_limit
            decoder.MAX_TABLE_BYTES = original_table_limit

    def test_swamp_and_common_cache_goldens(self):
        cache_root = os.environ.get("DH2_CACHE_ROOT")
        if cache_root is None:
            repo = Path(__file__).resolve().parents[3]
            cache_root = repo.parent / "cache" / "files"
        cache_root = Path(cache_root)
        common_dir = cache_root / "data" / "pydata"
        level_dir = common_dir / "scripts"
        files = [
            common_dir / "scripts_pyscriptnames.bin",
            common_dir / "scripts_pyscripts.bin",
            level_dir / "001_swamp_pyscriptnames.bin",
            level_dir / "001_swamp_pyscripts.bin",
        ]
        if not all(path.is_file() for path in files):
            self.skipTest("original cache unavailable; set DH2_CACHE_ROOT to its files directory")

        common = decode_script_table(files[0].read_bytes(), files[1].read_bytes())
        swamp = decode_script_table(files[2].read_bytes(), files[3].read_bytes())
        self.assertEqual(len(common.names), 15)
        self.assertEqual(len(swamp.names), 55)
        self.assertEqual(sum(len(script.commands) for script in common.scripts), 81)
        self.assertEqual(sum(len(script.commands) for script in swamp.scripts), 789)
        self.assertEqual(common.scripts[1].name, "BeginScriptedCutScene")
        self.assertEqual(common.scripts[3].name, "EndScriptedCutScene")

        intro = swamp.scripts[swamp.names.index("LizardMan_Intro")]
        self.assertEqual((intro.table_index, intro.byte_offset, intro.byte_end),
                         (17, 2426, 2656))
        self.assertEqual([cmd.command_id for cmd in intro.commands],
                         [0, 24, 8, 26, 30, 26, 30, 26, 25, 8, 0, 78])
        self.assertEqual([cmd.payload for cmd in intro.commands], [
            (True, 1, (), True),
            ("All",),
            (1000, "_prim_Waypoint_NewCamSpot", False),
            (500,),
            ("_prim_Monster_LizManIntro1",),
            (1500,),
            ("_prim_Monster_LizManIntro2",),
            (2000,),
            ("All",),
            (1000, "LocalPlayer", False),
            (True, 3, (), True),
            ("CombatTuto", 7),
        ])
        self.assertEqual([cmd.byte_offset for cmd in intro.commands],
                         [2430, 2444, 2455, 2493, 2501, 2535,
                          2543, 2577, 2585, 2596, 2620, 2634])
        self.assertEqual([cmd.byte_size for cmd in intro.commands],
                         [14, 11, 38, 8, 34, 8, 34, 8, 11, 24, 14, 22])

        start_room = swamp.scripts[swamp.names.index("enterLocation_StartRoom")]
        self.assertEqual((start_room.table_index, start_room.byte_offset, start_room.byte_end),
                         (45, 17424, 17444))
        self.assertEqual(len(start_room.commands), 1)
        self.assertEqual((start_room.commands[0].command_id,
                          start_room.commands[0].payload),
                         (10, (-1, 3, 1703960)))

        combined_names = common.names + swamp.names
        self.assertEqual(resolve_script_id(combined_names, "lIzArDmAn_InTrO",
                                           start_index=len(common.names)), 32)
        self.assertEqual(resolve_script_id(combined_names, "BeginScriptedCutScene"), 1)


if __name__ == "__main__":
    unittest.main()
