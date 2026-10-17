import importlib.util
from pathlib import Path
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / 'menu_ui_runtime_smoke.py'
SPEC = importlib.util.spec_from_file_location('menu_ui_runtime_smoke', SCRIPT)
SMOKE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(SMOKE)


class MenuTouchDiagnosticTest(unittest.TestCase):
    def test_tags_down_failure_and_scrubs_path_and_pointer(self):
        line = ('W/DH2Native: Native UI touch failure | action=DOWN | '
                r'Menu touch failed: input C:\Users\private\cache\menu.swf at 0x1234abcd')
        self.assertEqual(SMOKE.safe_menu_touch_failure(line), {
            'action': 'DOWN',
            'detail': 'input [path] at [address]',
        })

    def test_tags_up_failure(self):
        line = 'W/DH2Native: Native UI touch failure | action=UP | Menu touch failed: SWF core busy'
        self.assertEqual(SMOKE.safe_menu_touch_failure(line), {
            'action': 'UP', 'detail': 'SWF core busy',
        })

    def test_untagged_or_unrelated_failure_is_not_misreported(self):
        self.assertIsNone(SMOKE.safe_menu_touch_failure('Menu touch failed: SWF core busy'))
        self.assertIsNone(SMOKE.safe_menu_touch_failure('Native UI touch failure | action=DOWN | Other failure'))


if __name__ == '__main__':
    unittest.main()
