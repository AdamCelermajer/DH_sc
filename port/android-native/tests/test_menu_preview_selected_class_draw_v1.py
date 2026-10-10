"""Guard the menu's selected-class rendering and empty-slot draw route.

The authored model/class receiver regression covers Mage-vs-Warrior assets.
This source-level check ensures the production MainMenu draw actually submits
that selected receiver and retains its authored idle animation instead of
always drawing the save's old equipped Character model.
"""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


class MenuPreviewSelectedClassDraw(unittest.TestCase):
    def test_main_menu_submits_selected_authored_class_preview(self):
        renderer = (CPP / "model_renderer.cpp").read_text()
        start = renderer.index("  if(menu_background){", renderer.index("void draw(int width,int height)"))
        end = renderer.index("  previous_draw_v35=nullptr;", start)
        menu_branch = renderer[start:end]
        self.assertIn("if(menu_background_character_visible_v137)", menu_branch)
        self.assertIn("if(menu_persona_class>=0)", menu_branch)
        self.assertIn('#include "renderer_front_draw_v87.inc"', menu_branch)
        self.assertIn("else draw_native_menu_character_v123(projection);", menu_branch)

        draw = (CPP / "renderer_front_draw_v87.inc").read_text()
        self.assertIn("menu_background&&menu_persona_class>=0", draw)
        self.assertIn("actor.sample_elapsed=std::chrono::duration_cast<std::chrono::milliseconds>", draw)
        self.assertIn("dh2::objects::sample(resource,ms,error)", draw)

    def test_empty_slot_does_not_admit_old_or_default_actor(self):
        preview = (CPP / "native_menu_preview_v121.cpp").read_text()
        start = preview.index(" bool setup_character(")
        end = preview.index(" template<class F> bool run(", start)
        setup = preview[start:end]
        self.assertIn("if(!exists){", setup)
        empty = setup[setup.index("if(!exists){"):]
        empty = empty[:empty.index("  if(!services_.create_player")]
        self.assertIn("if(character_.identity||character_.owner)", empty)
        self.assertIn("if(!destroy_character(e))return false;", empty)
        self.assertIn('"Menu preview slot setup | requested_slot %d | save_exists 0 | Character none', empty)
        self.assertIn("e.clear();return true;", empty)
        self.assertGreater(setup.index("services_.create_player(slot,!exists"), setup.index("if(!exists){"))


if __name__ == "__main__":
    unittest.main()
