"""Default production wiring and compiled actual authored SpawnPoint traversal.

Run on Linux/WSL with --compile. Outputs live in a fresh temporary directory.
No Gradle, APK, emulator, or save-file operations.
"""
from pathlib import Path
import argparse
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


class Stage34SpawnPointProvider(unittest.TestCase):
    def test_actual_typed_catalog_loan_is_published_after_composition(self):
        source = (CPP / "renderer_character_campaign_v62.inc").read_text()
        compose = source.index("!compose_campaign_noncharacter_v69(")
        owners = source.index("source_world->noncharacter_owners_v105=", compose)
        bag = source.index("auto checkpoint_bag=source_world->stage34_native_v80;", owners)
        provider = source.index("checkpoint_bag->checkpoint.spawn_point=", bag)
        self.assertLess(provider, source.index("!install_campaign_object_loading_inputs_v95(", provider))
        body = source[provider:source.index("//Actual optional upstream", provider)]
        self.assertIn("[borrow=noncharacters.borrow_spawn_point_v80]", body)
        self.assertIn("borrow(actor.identity,out,e)", body)
        self.assertIn("out.owner_before(actor.lease)||actor.lease.owner_before(out)", body)
        self.assertNotIn("make_shared", body)
        typed = (CPP / "renderer_campaign_noncharacter_v69.inc").read_text()
        self.assertIn("owner->borrow_spawn_point(id,out,e)", typed)

    def test_bootstrap_body_forwards_to_live_checkpoint_bag(self):
        source = (CPP / "renderer_source_stage34_v80.inc").read_text()
        start = source.index("if(!native34.checkpoint.spawn_point)")
        body = source[start:source.index("if(!native33.set_use_spawn_point)", start)]
        self.assertIn("scope_world(world,e)", body)
        self.assertIn("auto bag=world->stage34_native_v80;", body)
        self.assertIn("bag->checkpoint.spawn_point", body)
        self.assertIn("return borrow(actor,out,e)", body)
        self.assertNotIn("matching_destination=", source)
        self.assertNotIn("matching_dispatch=", source)
        self.assertNotIn("checkpoint.local_player_hosting=", source)

    def test_checkpoint_tail_calls_the_tested_actual_selector(self):
        source = (ROOT / "port/level-loader/stage_loader_checkpoint_v1.hpp").read_text()
        self.assertIn("source_get_spawn_point_v1(objects,fields.fields->level110", source)
        self.assertIn("services_.make_handle,services_.spawn_point", source)


def compile_and_run():
    output = Path(tempfile.mkdtemp(prefix="dh2-stage34-spawn-"))
    composer = (CPP / "renderer_source_stage34_v80.inc").read_text()
    start = composer.index("if(!native34.checkpoint.spawn_point)")
    forwarding = composer[start:composer.index("if(!native33.set_use_spawn_point)", start)]
    publisher = (CPP / "renderer_character_campaign_v62.inc").read_text()
    start = publisher.index("auto checkpoint_bag=source_world->stage34_native_v80;")
    publication = publisher[start:publisher.index("//Actual optional upstream", start)]
    excerpt = forwarding + '''
 std::shared_ptr<world::CanonicalSpawnPointV15> selected;
 const auto* actor=f.objects->object(record->spawn->base().shared_handle().key);
 check(actor&&native34.checkpoint.spawn_point,"production default forwarding installed");
 check(!native34.checkpoint.spawn_point(*actor,selected,e)&&f.loans==0,"prepublication typed loan remains unavailable");
 const auto publish=[&]()->bool {
''' + publication + '''
  return true;
 };
 check(publish(),"actual production catalog loan publication");
 check(native34.checkpoint.spawn_point(*actor,selected,e)&&selected.get()==record->spawn.get(),"already-assembled body sees late production type13 loan");
 check(!selected.owner_before(record)&&!record.owner_before(selected),"production adaptation retains actual loan control block");
 f.live=false;selected.reset();auto before=f.loans;
 check(!native34.checkpoint.spawn_point(*actor,selected,e)&&!selected&&f.loans==before,"production forwarding rejects retired World scope");
'''
    (output / "production_stage34_spawn.inc").write_text(excerpt)
    world = ROOT / "port/level-world"
    sources = [
        ROOT / "port/level-loader/tests/level_get_spawn_point_v1.cpp",
        world / "canonical_spawn_point_v15.cpp",
        world / "canonical_dummy_owner_v14.cpp",
        world / "canonical_gameobject_base_owner_v1.cpp",
        world / "canonical_property_map_v1.cpp",
        world / "canonical_object_manager_v1.cpp",
        world / "object_manager_language_registry_v1.cpp",
        world / "navigation_objects.cpp",
    ]
    includes = [str(directory) for directory in (ROOT / "port").iterdir() if directory.is_dir()]
    command = ["g++", "-std=c++17", "-O1", "-g", "-ffunction-sections", "-fdata-sections",
               "-DDH2_STAGE34_COMPOSITION_EXCERPTS", "-I" + str(output),
               "-fno-fast-math", "-ffp-contract=off", "-Wl,--gc-sections"]
    command += ["-I" + directory for directory in includes]
    command += [str(source) for source in sources] + ["-o", str(output / "stage34-spawn")]
    compiled = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (output / "compile.log").write_text(compiled.stdout)
    if compiled.returncode:
        print(compiled.stdout)
        raise SystemExit(compiled.returncode)
    result = subprocess.run([str(output / "stage34-spawn")], text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
    (output / "run.log").write_text(result.stdout)
    print(result.stdout, end="")
    print("Focused host output:", output)
    if result.returncode:
        raise SystemExit(result.returncode)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--compile", action="store_true")
    arguments, remaining = parser.parse_known_args()
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(Stage34SpawnPointProvider)
    result = unittest.TextTestRunner().run(suite)
    if not result.wasSuccessful():
        raise SystemExit(1)
    if arguments.compile:
        compile_and_run()
