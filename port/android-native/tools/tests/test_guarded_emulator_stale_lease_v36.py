"""Stale guarded leases are skipped only when their named job is absent."""
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import launch_guarded_emulator_v36 as launcher


class StaleLeaseTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.registry = Path(self.temp.name)
        self.record = {
            "launcher_pid": 258512,
            "launcher_create_time": 1234,
            "job_name": r"Local\DH2-Emulator-stale-test",
        }
        (self.registry / "stale.manifest.json").write_text(
            json.dumps(self.record), encoding="utf8"
        )

    def tearDown(self):
        self.temp.cleanup()

    @staticmethod
    def access_denied():
        error = OSError("access denied reading recycled PID")
        error.winerror = 5
        return error

    def test_recycled_inaccessible_pid_skips_only_when_named_job_is_absent(self):
        with (
            patch.object(launcher, "REGISTRY", self.registry),
            patch.object(launcher, "creation_time", side_effect=self.access_denied()),
            patch.object(launcher, "OpenJob", return_value=None) as open_job,
            patch.object(launcher.C, "get_last_error", return_value=2),
        ):
            self.assertIsNone(launcher.registry_live())
        open_job.assert_called_once_with(4, False, self.record["job_name"])

    def test_existing_named_job_keeps_inaccessible_pid_lease_live(self):
        with (
            patch.object(launcher, "REGISTRY", self.registry),
            patch.object(launcher, "creation_time", side_effect=self.access_denied()),
            patch.object(launcher, "OpenJob", return_value=123),
            patch.object(launcher, "Close") as close,
        ):
            self.assertEqual(launcher.registry_live(), self.record)
        close.assert_called_once_with(123)

    def test_unverifiable_named_job_fails_closed(self):
        with (
            patch.object(launcher, "REGISTRY", self.registry),
            patch.object(launcher, "creation_time", side_effect=self.access_denied()),
            patch.object(launcher, "OpenJob", return_value=None),
            patch.object(launcher.C, "get_last_error", return_value=5),
            self.assertRaises(OSError),
        ):
            launcher.registry_live()


if __name__ == "__main__":
    unittest.main()
