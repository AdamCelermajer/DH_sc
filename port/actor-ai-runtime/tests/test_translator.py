from pathlib import Path
import sys
import unittest

HERE = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(HERE))
from actor_ai_runtime import StateIds, translate_spawn_command


ACTOR = {
    "name": "_prim_tmp_infected05",
    "source_record": 6,
    "ai_state": "Limbus",
    "auto_spawn": "0",
    "template_name": "InfectedVillage_CommonType1",
}
MODELS = [
    {"character_table_id": 258, "character_name": "InfectedVillager", "model_key": "infected"},
    {"character_table_id": 252, "character_name": "InfectedBurned", "model_key": "burned"},
]
STATE_IDS = StateIds(limbus=0, spawn=1)


class TranslatorTests(unittest.TestCase):
    def test_explicit_spawn_request_is_translated_without_animation(self):
        row = translate_spawn_command(
            {"name": "SpawnCharacter", "payload": [ACTOR["name"]]},
            [ACTOR], MODELS, STATE_IDS)
        self.assertEqual(row["status"], "translated_explicit_request")
        self.assertEqual(row["authored_initial_state"], {"name": "Limbus", "id": 0})
        self.assertEqual(row["state_request"]["name"], "Spawn")
        self.assertEqual(row["state_request"]["id"], 1)
        self.assertIsNone(row["animation_request"])
        self.assertEqual([m["character_table_id"] for m in row["model_choices"]], [258, 252])
        self.assertFalse(row["model_choice_selected"])
        self.assertFalse(row["script_or_trigger_executed"])

    def test_unresolved_lookup_fails_closed(self):
        row = translate_spawn_command(
            {"name": "SpawnCharacter", "payload": ["_prim_tmp_infected17"]},
            [], MODELS, STATE_IDS)
        self.assertEqual(row["status"], "unresolved_static_character")
        self.assertIsNone(row["state_request"])
        self.assertIsNone(row["animation_request"])

    def test_other_commands_do_not_invent_ai_transitions(self):
        for name in ("MoveActor", "PutCharacterInIdle", "Attack", "Idle"):
            with self.subTest(name=name):
                row = translate_spawn_command(
                    {"name": name, "payload": [ACTOR["name"]]},
                    [ACTOR], MODELS, STATE_IDS)
                self.assertEqual(row["status"], "unsupported_command")
                self.assertIsNone(row["state_request"])
                self.assertIsNone(row["animation_request"])

    def test_wrong_initial_state_or_template_is_rejected(self):
        for field, value in (("ai_state", "Idle"),
                             ("template_name", "OtherTemplate"),
                             ("auto_spawn", "1")):
            with self.subTest(field=field):
                source = dict(ACTOR)
                source[field] = value
                row = translate_spawn_command(
                    {"name": "SpawnCharacter", "payload": [ACTOR["name"]]},
                    [source], MODELS, STATE_IDS)
                self.assertEqual(row["status"], "source_record_not_supported")
                self.assertIsNone(row["state_request"])

    def test_malformed_command_and_missing_model_choices_are_rejected(self):
        malformed = translate_spawn_command(
            {"name": "SpawnCharacter", "payload": []}, [ACTOR], MODELS, STATE_IDS)
        self.assertEqual(malformed["status"], "malformed_command")
        empty_models = translate_spawn_command(
            {"name": "SpawnCharacter", "payload": [ACTOR["name"]]}, [ACTOR], [], STATE_IDS)
        self.assertEqual(empty_models["status"], "model_choices_unresolved")


if __name__ == "__main__":
    unittest.main()
