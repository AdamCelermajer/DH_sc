"""Bounded access-denied recovery requires fresh census and job proof."""
from pathlib import Path
import sys
import unittest

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import emulator_watchdog_v36 as watchdog


PID = 144016
QEMU = {"pid": PID, "parent_pid": 100, "image": "qemu-system-x86_64.exe"}


class FakeAPI:
    def __init__(self, census=None, job_pids=()):
        self.census = census or {}
        self.job_pids = list(job_pids)
        self.census_calls = 0
        self.job_calls = 0

    def process_snapshot(self, pid, metadata=None):
        raise watchdog.ProcessAccessDenied(pid)

    def enumerate_processes(self):
        self.census_calls += 1
        return self.census

    def job_process_ids(self, handle):
        self.job_calls += 1
        return self.job_pids


class AccessDeniedReconciliationTests(unittest.TestCase):
    def test_absent_from_fresh_census_and_named_job_is_departed(self):
        api = FakeAPI()
        with self.assertRaises(watchdog.ProcessGone) as caught:
            watchdog.process_snapshot_reconciled(api, PID, QEMU, "owned-job")
        self.assertEqual(
            caught.exception.reason,
            "AccessDenied_absent_from_fresh_census_and_named_job",
        )
        self.assertEqual(api.census_calls, 1)
        self.assertEqual(api.job_calls, 1)

    def test_pid_still_in_named_job_is_fatal_even_when_census_lacks_it(self):
        api = FakeAPI(job_pids=[PID])
        with self.assertRaises(RuntimeError) as caught:
            watchdog.process_snapshot_reconciled(api, PID, QEMU, "owned-job")
        self.assertIn("census_present=False", str(caught.exception))
        self.assertIn("job_member=True", str(caught.exception))

    def test_inaccessible_live_qemu_is_fatal(self):
        api = FakeAPI(census={PID: QEMU}, job_pids=[PID])
        with self.assertRaises(RuntimeError) as caught:
            watchdog.process_snapshot_reconciled(api, PID, QEMU, "owned-job")
        self.assertIn("job_member=True", str(caught.exception))
        self.assertIn("qemu-system-x86_64.exe", str(caught.exception))

    def test_same_image_pid_outside_job_remains_unresolved_and_fatal(self):
        api = FakeAPI(census={PID: QEMU})
        with self.assertRaises(RuntimeError) as caught:
            watchdog.process_snapshot_reconciled(api, PID, QEMU, "owned-job")
        self.assertIn("census_present=True", str(caught.exception))
        self.assertIn("job_member=False", str(caught.exception))
        self.assertIn("qemu-system-x86_64.exe", str(caught.exception))


if __name__ == "__main__":
    unittest.main(verbosity=2)
