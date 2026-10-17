import struct
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from actor_spawn_runtime import (
    CharacterTemplate,
    DecodeError,
    apply_spawn_character_request,
    choose_template_property,
    parse_character_templates,
    parse_integer_constants,
    parse_name_file,
    parse_name_tables,
)


def names_file(names):
    return struct.pack("<I", len(names)) + b"".join(
        struct.pack("<I", len(value)) + value for value in names
    )


class CharacterTemplateReaderTest(unittest.TestCase):
    def test_reads_variable_template_lists_and_keeps_alternatives(self):
        # Correct row encoding: template counts 2 and 1, followed by IDs 0, 1, 1.
        array = struct.pack("<IIiiIi", 2, 2, 0, 1, 1, 1)
        templates = parse_character_templates(
            array,
            names_file((b"one", b"two")),
            names_file((b"base", b"variant")),
        )
        self.assertEqual(templates, (
            CharacterTemplate("one", (0, 1)),
            CharacterTemplate("two", (1,)),
        ))
        chosen = choose_template_property(templates[0], 1, ("base", "variant"))
        self.assertEqual(chosen["property_name"], "variant")
        self.assertFalse(chosen["random_choice_made"])
        self.assertEqual(choose_template_property(templates[0], 2, ("base", "variant"))["status"],
                         "alternative_out_of_range")

    def test_rejects_count_mismatch_bad_index_and_trailing_data(self):
        with self.assertRaises(DecodeError):
            parse_character_templates(
                struct.pack("<IIi", 1, 1, 2),
                names_file((b"one",)),
                names_file((b"base", b"variant")),
            )
        with self.assertRaises(DecodeError):
            parse_character_templates(
                struct.pack("<IIi", 1, 1, 2),
                names_file((b"one",)),
                names_file((b"base",)),
            )
        with self.assertRaises(DecodeError):
            parse_character_templates(
                struct.pack("<II", 1, 0) + b"x",
                names_file((b"one",)),
                names_file((b"base",)),
            )

    def test_decodes_signed_integer_constants(self):
        raw = (struct.pack("<I", 1) + struct.pack("<I", 8) + b"AIStates" +
               struct.pack("<I", 2) + struct.pack("<I", 6) + b"Limbus" +
               struct.pack("<i", 0) + struct.pack("<I", 5) + b"Spawn" +
               struct.pack("<i", 1))
        self.assertEqual(parse_integer_constants(raw), {"AIStates": {"Limbus": 0, "Spawn": 1}})
        with self.assertRaises(DecodeError):
            parse_name_file(struct.pack("<II", 1, 3) + b"x")

    def test_decodes_concatenated_name_tables(self):
        raw = names_file((b"first",)) + names_file((b"second", b"third"))
        self.assertEqual(parse_name_tables(raw), (("first",), ("second", "third")))


class SpawnTransitionTest(unittest.TestCase):
    def test_only_exact_existing_character_transitions_to_spawn(self):
        actor = "_prim_tmp_infected05"
        states = {"Limbus": 0, "Spawn": 1}
        accepted = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": [actor]}, [actor], states, 0,
            registered_state_ids=(0, 1),
        )
        self.assertEqual((accepted["from_state_id"], accepted["to_state_id"]), (0, 1))
        self.assertTrue(accepted["state_changed"])
        self.assertFalse(accepted["actor_created"])
        self.assertFalse(accepted["animation_requested"])

        miss = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": ["_prim_tmp_infected17"]},
            [actor], states, 0,
        )
        self.assertEqual(miss["status"], "actor_lookup_miss_or_ambiguous")
        self.assertEqual(miss["to_state_id"], 0)

        duplicate = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": [actor]}, [actor, actor], states, 0
        )
        self.assertEqual(duplicate["status"], "actor_lookup_miss_or_ambiguous")

        unverified = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": [actor]}, [actor], states, 0
        )
        self.assertEqual(unverified["status"], "state_registration_unverified")
        self.assertEqual(unverified["requested_state_id"], 1)
        self.assertIsNone(unverified["state_changed"])

        unavailable = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": [actor]}, [actor], states, 0,
            registered_state_ids=(0,),
        )
        self.assertEqual(unavailable["status"], "spawn_state_not_registered")
        self.assertEqual(unavailable["to_state_id"], 0)

    def test_bad_or_repeat_request_does_not_claim_a_new_transition(self):
        states = {"Limbus": 0, "Spawn": 1}
        bad = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": []}, ["actor"], states, 0
        )
        self.assertEqual(bad["status"], "malformed_command")
        repeated = apply_spawn_character_request(
            {"name": "SpawnCharacter", "payload": ["actor"]}, ["actor"], states, 1,
            registered_state_ids=(1,),
        )
        self.assertEqual(repeated["status"], "direct_state_change")
        self.assertFalse(repeated["state_changed"])


if __name__ == "__main__":
    unittest.main()
