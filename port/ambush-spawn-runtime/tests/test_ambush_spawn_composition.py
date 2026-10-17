import copy
import sys
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
PORT = HERE.parents[1]
sys.path.insert(0, str(PORT / "actor-spawn-runtime"))
sys.path.insert(0, str(PORT / "ambush-spawn-runtime"))

from actor_spawn_runtime import CharacterTemplate  # noqa: E402
from ambush_spawn_composition import (  # noqa: E402
    CompositionError,
    EXPECTED_REQUESTS,
    EXPECTED_TEMPLATE,
    compose_projection,
)


def fixture():
    loaded = [row[0] for row in EXPECTED_REQUESTS if row[3] is not None]
    loaded.extend(f"source_character_{index:02}" for index in range(22))
    records = {"_prim_tmp_infected05": 6, "_prim_tmp_infected07": 8,
               "_prim_tmp_infected06": 7, "_prim_tmp_infected16": 19}
    requests = []
    for name, registry_result, event_type, source_record in EXPECTED_REQUESTS:
        actor = None
        if source_record is not None:
            actor = {
                "module": "_module_infectedvillage_01_001",
                "source_record": records[name],
                "template": EXPECTED_TEMPLATE,
                "template_data_class": "Charater_Templates",
                "editor_template_name": "MonsterCommonType1",
                "ai_state": "Limbus",
                "auto_spawn": 0,
                "lifecycle": "spawn_requested",
            }
        requests.append({
            "name": name,
            "registry_result": registry_result,
            "event_type": event_type,
            "event_command_id": 30,
            "source_actor": actor,
        })
    projection = {
        "source_actor_count": 26,
        "runtime_object_count": 26,
        "activation_status": "activated",
        "request_count": 5,
        "loaded_character_names": loaded,
        "requests": requests,
        "registry_lookup_misses": 1,
        "last_lookup_miss": "_prim_tmp_infected17",
        "missing_name_still_unregistered": True,
        "actors_created": 0,
        "render_objects_created": 0,
    }
    return (
        projection,
        (CharacterTemplate(EXPECTED_TEMPLATE, (0, 1)),),
        ("CommonType1_base", "CommonType1_variant"),
        {"Limbus": 0, "Spawn": 1},
    )


class AmbushSpawnCompositionTest(unittest.TestCase):
    def test_composes_four_state_requests_and_one_still_missing_lookup(self):
        projection, templates, properties, states = fixture()
        result = compose_projection(
            projection, templates, properties, states,
            registered_state_ids=tuple(range(20)),
        )
        present = [row for row in result["ordered_requests"]
                   if row["registry_result"] == "requested"]
        self.assertEqual(len(present), 4)
        self.assertTrue(all(row["spawn_runtime"]["status"] ==
                            "direct_state_change" for row in present))
        self.assertTrue(all(row["spawn_runtime"]["requested_state_id"] == 1
                            for row in present))
        self.assertTrue(all(row["spawn_runtime"]["state_changed"] is True
                            for row in present))
        self.assertFalse(result["ordered_requests"][2]["spawn_runtime"]["state_changed"])
        self.assertEqual(result["unresolved_lookup"]["name"], "_prim_tmp_infected17")
        self.assertEqual(result["template"]["property_names"],
                         ["CommonType1_base", "CommonType1_variant"])
        self.assertFalse(result["template"]["alternative_choice_performed"])

    def test_rejects_reordered_or_mutated_script_request(self):
        projection, templates, properties, states = fixture()
        projection["requests"][0]["name"], projection["requests"][1]["name"] = (
            projection["requests"][1]["name"], projection["requests"][0]["name"]
        )
        with self.assertRaises(CompositionError):
            compose_projection(projection, templates, properties, states)

    def test_rejects_invented_source_actor_for_unresolved_name(self):
        projection, templates, properties, states = fixture()
        projection["loaded_character_names"].append("_prim_tmp_infected17")
        projection["source_actor_count"] = 27
        projection["runtime_object_count"] = 27
        with self.assertRaises(CompositionError):
            compose_projection(projection, templates, properties, states)

    def test_rejects_template_mismatch_on_source_row(self):
        projection, templates, properties, states = fixture()
        projection["requests"][0]["source_actor"]["template"] = "OtherTemplate"
        with self.assertRaises(CompositionError):
            compose_projection(projection, templates, properties, states)


if __name__ == "__main__":
    unittest.main()
