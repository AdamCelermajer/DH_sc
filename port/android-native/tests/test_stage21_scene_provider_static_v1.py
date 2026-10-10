"""Static regression for the Stage21 Scene.update provider connection.

Checks production enrollment, the composed-provider publication, and source
ordering. Does not execute native callbacks or claim runtime/asset validation.
Run directly with Python; no build or emulator is needed.
"""
from pathlib import Path
import re
import unittest


CPP = Path(__file__).resolve().parents[1] / "app/src/main/cpp"


class Stage21SceneProviderStaticTest(unittest.TestCase):
    def test_room_and_no_room_enrollment_preserve_the_public_bag(self):
        boundary = (CPP / "renderer_source_boundary_v61.inc").read_text()
        for helper, filename in (
            ("install_campaign_player_light_tweakers_v90", "renderer_player_light_tweaker_v90.inc"),
            ("install_campaign_no_room_primitive_v89", "renderer_campaign_no_room_v89.inc"),
        ):
            with self.subTest(enrollment=helper):
                self.assertIn(f"{helper}(result,error)", boundary)
                source = (CPP / filename).read_text()
                # Both default-room and no-room enrollment reuse an existing
                # bag, or allocate one, then publish it to the SAME World.
                self.assertRegex(source, r"auto bag=world->stage34_native_v80\?world->stage34_native_v80:std::make_shared<")
                self.assertIn("world->stage34_native_v80=std::move(bag)", source)

    def test_composed_scene_update_reaches_the_public_bag_without_replacement(self):
        source = (CPP / "renderer_source_stage34_v80.inc").read_text()
        compose = source.split("bool compose_source_stage33_stage34_v80", 1)[1].split(
            "bool bind_source_stage33_stage34_v80", 1
        )[0]
        provider = compose.index("if(!native34.scene_virtual60)")
        update = compose.index("roots->source_update_v102(delta,optimized,transport,e)", provider)
        publication = re.search(
            r"if\(source_world->stage34_native_v80\s*&&\s*"
            r"!source_world->stage34_native_v80->scene_virtual60\)\s*"
            r"source_world->stage34_native_v80->scene_virtual60\s*=\s*"
            r"native34.scene_virtual60\s*;",
            compose,
        )
        self.assertIsNotNone(publication, "Stage21 reads the public bag, not the private Stage34 service copy")
        self.assertLess(update, publication.start())
        self.assertLess(publication.end(), compose.index("out34=std::move(native34)"))

    def test_disabled_batching_still_updates_the_same_scene_first(self):
        source = (CPP / "renderer_source_batching_v96.inc").read_text()
        stage = source.split("Step stage(std::uint32_t phase", 1)[1].split(
            "static bool bind_source_batching_stages_v96", 1
        )[0]
        scene = stage.index("if(phase==21){auto bag=source->stage34_native_v80;")
        update = stage.index("bag->scene_virtual60(roots,0.0f,false,e)", scene)
        debug = stage.index('debug("IsDisablingCompiledBatching",disabled,e)', update)
        disabled = stage.index("if(disabled){", debug)
        compiler = stage.index("release_level_batch_compiler_v96(receiver,e)", disabled)
        self.assertLess(scene, update)
        self.assertLess(update, debug)
        self.assertLess(debug, disabled)
        self.assertLess(disabled, compiler)
        self.assertIn("return Step::complete;", stage[disabled:compiler])


if __name__ == "__main__":
    unittest.main()
