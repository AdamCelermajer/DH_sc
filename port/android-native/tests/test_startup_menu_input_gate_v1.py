"""Regression for the source main-menu movie input gate."""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
EVENTS = (CPP / "native_menu_application_event_v120.inc").read_text(encoding="utf-8")
STARTUP = (CPP / "native_process_startup_v119.inc").read_text(encoding="utf-8")
IDA = ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so"


class StartupMenuInputGateTests(unittest.TestCase):
    def test_slot_two_uses_main_menu_show_gate_not_animation_preference(self):
        gate = EVENTS.split("if(slot==2){", 1)[1].split("\n  dh2::ui::SwfMovie* movie", 1)[0]
        self.assertIn("!source_main_menu_input_enabled_v120", gate)
        self.assertIn("!source_initial_splash_input_allowed_v120(native,receipt.identity)", gate)
        self.assertNotIn("source_animation_scaling_enabled_v99", gate)
        self.assertIn("bool source_main_menu_input_enabled_v120{};", EVENTS)

    def test_splash_exception_requires_the_active_top_state_in_same_render(self):
        exception = EVENTS.split("bool source_initial_splash_input_allowed_v120", 1)[1].split("struct NativeMenuCurrentEventScopeV120", 1)[0]
        self.assertIn("native.shared_stack_v27->render(movie)", exception)
        self.assertIn("render->states[render->count-1]", exception)
        self.assertIn('!std::strcmp(active->name,"menu_splash")', exception)

    def test_main_menu_show_publishes_gate_only_after_its_real_display_hook(self):
        hook = STARTUP.index('movie.menu_display_callback("_root.menu_bg.RenderedBG"')
        publish = STARTUP.index("source_main_menu_input_enabled_v120=true;", hook)
        close = STARTUP.index("bool model_renderer::native_process_derived_menu_focus_v119", publish)
        admission = STARTUP.rfind("if(!p->scoped_movie(owner->fields", 0, hook)
        self.assertLess(hook, publish)
        self.assertNotEqual(admission, -1)
        self.assertIn("return false;", STARTUP[admission:publish])
        self.assertLess(publish, close)

    def test_ida_byte_is_written_by_main_show_and_gates_slot_two(self):
        listing = (IDA / "full-listing.asm").read_text(encoding="utf-8", errors="replace")
        self.assertIn("byte_99F7D7     % 1", listing)
        self.assertIn("MenuMainMenu::Show(void)+220", listing)
        on_event = (IDA / "pseudocode/0043/00430ad4.c").read_text(encoding="utf-8", errors="replace")
        self.assertIn("else if ( !byte_99F7D7 )", on_event)
        main_show = (IDA / "assembly-functions.asm").read_text(encoding="utf-8", errors="replace")
        self.assertIn("0042c84c", main_show)
        self.assertIn("STRB            R2, [R3,#(byte_99F7D7 - 0x99F72C)]", main_show)

    def test_authored_continue_button_reaches_main_menu_action(self):
        actions = ROOT / ".local-inputs/publication/checkpoint/session-contributions/menu-launch/evidence/current/dqmenus_droid-actions.json"
        source = actions.read_text(encoding="utf-8")
        self.assertIn('"btn_Continue"', source)
        self.assertIn('"NativeGoToMainMenu"', source)


if __name__ == "__main__":
    unittest.main()
