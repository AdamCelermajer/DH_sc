"""OpenProcess denial remains fail-closed, with one bounded race retry."""
import ctypes as C
import importlib.util
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / "emulator_watchdog_v36.py"
spec = importlib.util.spec_from_file_location("watchdog_openprocess_retry_v50", SOURCE)
wd = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = wd
spec.loader.exec_module(wd)


class OpenProcessRetryTests(unittest.TestCase):
    def api(self, outcomes):
        api = object.__new__(wd.WindowsAPI)
        calls = []

        def open_process(access, inherit, pid):
            calls.append((access, inherit, pid))
            outcome = outcomes[min(len(calls) - 1, len(outcomes) - 1)]
            if isinstance(outcome, tuple) and outcome[0] == "error":
                C.set_last_error(outcome[1])
                return None
            return outcome

        api.open_process = open_process
        return api, calls

    def test_one_access_denied_race_retries_once(self):
        api, calls = self.api([("error", 5), 1234])
        with patch.object(wd.time, "sleep") as sleep:
            self.assertEqual(api.open_process_limited(4321), 1234)
        self.assertEqual(calls, [(0x1000, False, 4321)] * 2)
        sleep.assert_called_once_with(0.025)

    def test_persistent_access_denied_still_fails_closed_with_pid(self):
        api, calls = self.api([("error", 5)])
        with patch.object(wd.time, "sleep"):
            with self.assertRaises(OSError) as caught:
                api.open_process_limited(4321)
        self.assertEqual(len(calls), 2)
        self.assertIn("pid=4321", str(caught.exception))
        self.assertEqual(caught.exception.errno, 5)

    def test_invalid_parameter_remains_process_gone_for_authoritative_reconcile(self):
        api, calls = self.api([("error", 87)])
        with patch.object(wd.time, "sleep"):
            with self.assertRaises(wd.ProcessGone) as caught:
                api.open_process_limited(4321)
        self.assertEqual(len(calls), 1)
        self.assertEqual(caught.exception.reason, "OpenProcess_ERROR_INVALID_PARAMETER")


if __name__ == "__main__":
    unittest.main(verbosity=2)
