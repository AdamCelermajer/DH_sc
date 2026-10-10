"""Shutdown census races use injected APIs and time; no live processes/jobs."""
from pathlib import Path
import hashlib
import json
import sys
import tempfile
import unittest
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import emulator_watchdog_v36 as watchdog

PID = 180060
QEMU = {"pid": PID, "parent_pid": 52580, "image": "qemu-system-x86_64.exe"}


class FakeAPI:
    def __init__(self, censuses, members=((),)):
        self.censuses = list(censuses)
        self.members = list(members)
        self.census_calls = self.job_calls = 0

    def process_snapshot(self, pid, metadata=None):
        raise watchdog.ProcessAccessDenied(pid)

    def enumerate_processes(self):
        value = self.censuses[min(self.census_calls, len(self.censuses) - 1)]
        self.census_calls += 1
        if isinstance(value, Exception):
            raise value
        return value

    def job_process_ids(self, handle):
        value = self.members[min(self.job_calls, len(self.members) - 1)]
        self.job_calls += 1
        if isinstance(value, Exception):
            raise value
        return list(value)


def metric(pid, image, parent, created):
    return {"pid": pid, "parent_pid": parent, "image": image,
            "create_time_filetime": created, "private_bytes": 1024,
            "working_set_bytes": 1024, "handles": 3, "alive": True}


class CollectAPI(FakeAPI):
    def __init__(self, persistent=False):
        owner = metric(100, "python.exe", 1, 1000)
        target = metric(10, "emulator.exe", 100, 2000)
        self.records = {100: owner, 10: target}
        present = dict(self.records)
        present[PID] = QEMU
        super().__init__([present, present, present if persistent else self.records], ([10],))

    def system_snapshot(self):
        return {"commit_total_bytes": 8 * watchdog.GIB,
                "commit_limit_bytes": 64 * watchdog.GIB,
                "physical_available_bytes": 16 * watchdog.GIB,
                "physical_total_bytes": 32 * watchdog.GIB}

    def process_snapshot(self, pid, metadata=None):
        if pid == PID:
            raise watchdog.ProcessAccessDenied(pid)
        return dict(self.records[pid])


class ShutdownReconciliationTests(unittest.TestCase):
    def setUp(self):
        self.clock = 0.0
        self.sleeps = []
        self.state = {"deadline": None, "diagnostics": []}
        self.time_patch = patch.object(watchdog.time, "monotonic", side_effect=lambda: self.clock)
        self.sleep_patch = patch.object(watchdog.time, "sleep", side_effect=self.sleep)
        self.time_patch.start()
        self.sleep_patch.start()
        self.addCleanup(self.time_patch.stop)
        self.addCleanup(self.sleep_patch.stop)

    def sleep(self, amount):
        self.sleeps.append(amount)
        self.clock += amount

    def reconcile(self, api, metadata=QEMU):
        return watchdog.process_snapshot_reconciled(api, PID, metadata, "owned-job", self.state)

    def require_fatal(self, api, metadata=QEMU):
        with self.assertRaises(RuntimeError) as caught:
            self.reconcile(api, metadata)
        self.assertIn("Unresolved access denied", str(caught.exception))
        return str(caught.exception)

    def test_candidate_disappears_after_first_yield_with_both_authorities(self):
        api = FakeAPI([{PID: QEMU}, {}])
        with self.assertRaises(watchdog.ProcessGone) as caught:
            self.reconcile(api)
        self.assertEqual(self.sleeps, [0.075])
        self.assertEqual(api.census_calls, 2)
        self.assertEqual(api.job_calls, 2)
        self.assertEqual(caught.exception.details["reconciliation_passes"], 2)
        diagnostic = self.state["diagnostics"][0]
        self.assertEqual(diagnostic["previous_census"], QEMU)
        self.assertEqual(diagnostic["passes"][0]["fresh_census"], QEMU)
        self.assertFalse(diagnostic["passes"][-1]["census_present"])
        self.assertFalse(diagnostic["passes"][-1]["job_member"])

    def test_candidate_disappears_after_second_yield_within_total_budget(self):
        api = FakeAPI([{PID: QEMU}, {PID: QEMU}, {}])
        with self.assertRaises(watchdog.ProcessGone):
            self.reconcile(api)
        self.assertEqual(api.census_calls, 3)
        self.assertEqual(self.sleeps, [0.075, 0.075])
        self.assertLessEqual(sum(self.sleeps), 0.15)

    def test_persistent_outside_job_qemu_remains_fatal_and_diagnostic(self):
        api = FakeAPI([{PID: QEMU}])
        message = self.require_fatal(api)
        self.assertIn("census_present=True", message)
        self.assertIn("job_member=False", message)
        self.assertEqual(api.census_calls, 3)
        self.assertLessEqual(sum(self.sleeps), 0.15)
        self.assertEqual(self.state["diagnostics"][0]["outcome"], "unresolved")

    def test_listed_job_pid_never_qualifies_for_wait(self):
        self.require_fatal(FakeAPI([{PID: QEMU}], ([PID],)))
        self.assertEqual(self.sleeps, [])

    def test_census_absence_does_not_hide_still_listed_job_pid(self):
        self.require_fatal(FakeAPI([{}], ([PID],)))
        self.assertEqual(self.sleeps, [])

    def test_candidate_joining_job_during_wait_is_fatal(self):
        api = FakeAPI([{PID: QEMU}], ((), [PID]))
        self.require_fatal(api)
        self.assertEqual(self.sleeps, [0.075])
        self.assertEqual(api.job_calls, 2)

    def test_image_change_never_qualifies_for_wait(self):
        reused = dict(QEMU, image="python.exe")
        self.require_fatal(FakeAPI([{PID: reused}]))
        self.assertEqual(self.sleeps, [])

    def test_parent_change_never_qualifies_for_wait(self):
        reused = dict(QEMU, parent_pid=999)
        self.require_fatal(FakeAPI([{PID: reused}]))
        self.assertEqual(self.sleeps, [])

    def test_parent_change_after_first_yield_is_fatal(self):
        reused = dict(QEMU, parent_pid=999)
        self.require_fatal(FakeAPI([{PID: QEMU}, {PID: reused}]))
        self.assertEqual(self.sleeps, [0.075])

    def test_missing_original_metadata_does_not_qualify_for_wait(self):
        self.require_fatal(FakeAPI([{PID: QEMU}]), metadata=None)
        self.assertEqual(self.sleeps, [])

    def test_failed_census_keeps_failure_diagnostic(self):
        self.require_fatal(FakeAPI([RuntimeError("census unavailable")]))
        self.assertEqual(self.sleeps, [])
        self.assertIn("census unavailable", self.state["diagnostics"][0]["reconciliation_error"])

    def test_failed_job_query_keeps_failure_diagnostic(self):
        self.require_fatal(FakeAPI([{PID: QEMU}], (RuntimeError("job unavailable"),)))
        self.assertEqual(self.sleeps, [])
        self.assertIn("job unavailable", self.state["diagnostics"][0]["reconciliation_error"])

    def test_oversleep_cannot_reset_deadline(self):
        def oversleep(amount):
            self.sleeps.append(amount)
            self.clock += 0.21
        with patch.object(watchdog.time, "sleep", side_effect=oversleep):
            self.require_fatal(FakeAPI([{PID: QEMU}]))
        self.assertEqual(self.sleeps, [0.075])
        self.assertTrue(self.state["diagnostics"][0]["wait_budget_exhausted"])

    def test_multiple_candidates_share_one_snapshot_wait_budget(self):
        with self.assertRaises(watchdog.ProcessGone):
            self.reconcile(FakeAPI([{PID: QEMU}, {}]))
        self.require_fatal(FakeAPI([{PID: QEMU}]))
        self.assertEqual(self.sleeps, [0.075, 0.075])
        self.assertLessEqual(sum(self.sleeps), 0.15)

    def collect(self, persistent):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "manifest.json"
            manifest = {"launcher_pid": 100, "launcher_create_time": 1000,
                        "target_pid": 10, "target_create_time": 2000}
            path.write_text(json.dumps(manifest))
            digest = hashlib.sha256(path.read_bytes()).hexdigest()
            return watchdog.collect_snapshot(CollectAPI(persistent), manifest,
                "owned-job", 0.0, path, digest)

    def test_departed_candidate_keeps_final_job_metrics_and_limits(self):
        snapshot = self.collect(False)
        self.assertTrue(snapshot["complete"], snapshot["errors"])
        self.assertEqual(snapshot["gone_enumerated_pids"], [PID])
        self.assertEqual(snapshot["job_process_ids"], [10])
        self.assertEqual([record["pid"] for record in snapshot["qemu"]], [10])
        self.assertEqual(watchdog.evaluate(snapshot, watchdog.Limits())["action"], "continue")
        snapshot["job_members"][0]["private_bytes"] = 6 * watchdog.GIB
        self.assertIn("owned_job_private_limit", watchdog.evaluate(snapshot, watchdog.Limits())["reasons"])

    def test_unresolved_candidate_keeps_metadata_in_failed_snapshot(self):
        snapshot = self.collect(True)
        self.assertFalse(snapshot["complete"])
        self.assertEqual(snapshot["owner"]["pid"], 100)
        self.assertEqual(snapshot["access_denied_reconciliations"][0]["previous_census"], QEMU)
        decision = watchdog.evaluate(snapshot, watchdog.Limits())
        self.assertEqual(decision["action"], "terminate")
        self.assertIn("monitoring_unavailable", decision["reasons"])
        self.assertNotIn("launcher_missing_or_exited", decision["reasons"])


if __name__ == "__main__":
    unittest.main(verbosity=2)
