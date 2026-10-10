"""Guest RAM CLI and dry-run coverage; never starts an emulator."""
from pathlib import Path
import sys
import unittest

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import launch_guarded_emulator_v36 as launcher


class GuestMemoryTests(unittest.TestCase):
    def parse(self, *extra):
        return launcher.build_parser().parse_args(
            ["--avd", "Medium_Phone_API_37.0", *extra]
        )

    def test_default_and_dry_run_command_use_guest_memory(self):
        default_args = self.parse()
        self.assertEqual(default_args.guest_memory_mib, 4096)
        default_report = launcher.dry_run_report(default_args, {"qemu": []})
        self.assertTrue(default_report["dry_run"])
        self.assertTrue(default_report["no_emulator_started"])
        default_memory_arg = default_report["command"].index("-memory")
        self.assertEqual(default_report["command"][default_memory_arg + 1], "4096")

        reduced_args = self.parse("--guest-memory-mib", "3072")
        self.assertEqual(reduced_args.guest_memory_mib, 3072)
        report = launcher.dry_run_report(reduced_args, {"qemu": []})
        self.assertEqual(report["guest_memory_mib"], 3072)
        memory_arg = report["command"].index("-memory")
        self.assertEqual(report["command"][memory_arg + 1], "3072")

    def test_guest_memory_rejects_values_outside_bounds(self):
        for value in ("2047", "4097"):
            with self.subTest(value=value), self.assertRaises(SystemExit) as raised:
                self.parse("--guest-memory-mib", value)
            self.assertEqual(raised.exception.code, 2)

    def test_dry_run_reports_selected_guest_memory(self):
        args = self.parse("--guest-memory-mib", "3072")
        report = launcher.dry_run_report(args, {})
        self.assertEqual(report["guest_memory_mib"], 3072)
        memory_arg = report["command"].index("-memory")
        self.assertEqual(report["command"][memory_arg + 1], "3072")


if __name__ == "__main__":
    unittest.main()
