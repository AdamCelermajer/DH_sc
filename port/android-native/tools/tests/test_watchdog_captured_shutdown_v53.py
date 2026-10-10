"""Captured 20261009-000556 shutdown failure, using injected APIs/time only.

The retained child metadata and three unresolved observations come from the
receipt with SHA256 14b92a7febbc445e2a8ec7b74f2530b9cf396e8e9dfca7ebb98580f899cd176c.
Successful process metrics are modeled from its last complete telemetry row.
No live emulator, process, JobObject or existing run artifact is touched.
"""
from contextlib import redirect_stdout
import hashlib
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import emulator_watchdog_v36 as watchdog

CHILD = {"pid": 131028, "parent_pid": 45340, "image": "qemu-system-x86_64.exe"}
MEMBERS = [21908, 244172, 45340, 81360, 190036, 77708, 287516, 82868]
MANIFEST = {"job_name": "Local\\DH2-Emulator-46e0403f-7aa4-4daa-bda7-ac414e60ad22",
            "launcher_pid": 220352, "launcher_create_time": 134359671561802228,
            "target_pid": 21908, "target_create_time": 134359671568554720}


def record(pid, parent, image, created, private=1024):
    return {"pid": pid, "parent_pid": parent, "image": image,
            "create_time_filetime": created, "private_bytes": private,
            "working_set_bytes": 1024, "handles": 3, "alive": True}


class CapturedAPI:
    def __init__(self, arm_first=False, clear_after=None):
        self.records = {
            220352: record(220352, 247256, "python.exe", MANIFEST["launcher_create_time"]),
            21908: record(21908, 220352, "emulator.exe", MANIFEST["target_create_time"], 11448320),
            244172: record(244172, 21908, "conhost.exe", 134359671569966624),
            45340: record(45340, 21908, "qemu-system-x86_64.exe", 134359671570552036, 3917328384),
            81360: record(81360, 45340, "netsimd.exe", 134359671587218416),
            190036: record(190036, 45340, "cmd.exe", 134359673614183606),
            77708: record(77708, 45340, "cmd.exe", 134359673614251740),
            287516: record(287516, 190036, "emulator.exe", 134359673614348427, 11427840),
            82868: record(82868, 77708, "emulator.exe", 134359673614385847, 11186176),
        }
        self.arm_first, self.clear_after = arm_first, clear_after
        self.census_calls = 0
        self.terminated = []
        self.closed = []
        self.system = {"commit_total_bytes": 55653199872, "commit_limit_bytes": 100753940480,
                       "physical_available_bytes": 9309515776, "physical_total_bytes": 34181947392}

    def system_snapshot(self):
        return dict(self.system)

    def enumerate_processes(self):
        self.census_calls += 1
        census = {pid: {key: item[key] for key in ("pid", "parent_pid", "image")}
                  for pid, item in self.records.items()}
        if not (self.arm_first and self.census_calls == 1) and not (
                self.clear_after is not None and self.census_calls > self.clear_after):
            census[CHILD["pid"]] = dict(CHILD)
        return census

    def job_process_ids(self, handle):
        return list(MEMBERS)

    def process_snapshot(self, pid, metadata=None):
        if pid == CHILD["pid"]:
            raise watchdog.ProcessAccessDenied(pid)
        return dict(self.records[pid])

    def open_job(self, name):
        if name != MANIFEST["job_name"]:
            raise AssertionError("wrong job")
        return name

    def target_in_job(self, pid, handle, expected_create_time=None):
        return pid in MEMBERS

    def terminate_job(self, handle):
        self.terminated.append(handle)

    def close(self, handle):
        self.closed.append(handle)


class CapturedShutdownTests(unittest.TestCase):
    def setUp(self):
        self.clock = 0.0
        self.sleeps = []
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.folder = Path(self.temp.name)
        self.path = self.folder / "manifest.json"
        self.path.write_text(json.dumps(MANIFEST))
        self.digest = hashlib.sha256(self.path.read_bytes()).hexdigest()
        self.time_patch = patch.object(watchdog.time, "monotonic", side_effect=lambda: self.clock)
        self.sleep_patch = patch.object(watchdog.time, "sleep", side_effect=self.sleep)
        self.time_patch.start()
        self.sleep_patch.start()
        self.addCleanup(self.time_patch.stop)
        self.addCleanup(self.sleep_patch.stop)

    def sleep(self, amount):
        self.sleeps.append(amount)
        self.clock += amount

    def collect(self, api):
        return watchdog.collect_snapshot(api, MANIFEST, MANIFEST["job_name"],
                                         0.0, self.path, self.digest)

    def test_three_captured_unresolved_child_observations_still_fail_closed(self):
        snapshot = self.collect(CapturedAPI())
        self.assertFalse(snapshot["complete"])
        diagnostic = snapshot["access_denied_reconciliations"][0]
        self.assertEqual(diagnostic["previous_census"], CHILD)
        self.assertEqual(len(diagnostic["passes"]), 3)
        for observation in diagnostic["passes"]:
            self.assertTrue(observation["census_present"])
            self.assertFalse(observation["job_member"])
            self.assertEqual(observation["fresh_census"], CHILD)
            self.assertEqual(observation["job_process_ids"], sorted(MEMBERS))
        self.assertEqual(self.sleeps, [0.075, 0.075])
        decision = watchdog.evaluate(snapshot, watchdog.Limits(allow_physical_pressure=True))
        self.assertEqual(decision["action"], "terminate")
        self.assertIn("monitoring_unavailable", decision["reasons"])
        self.assertIn("pid=131028", decision["observations"]["monitoring_error"])

    def test_aborted_sampling_preserves_partial_qemu_and_unknown_total(self):
        snapshot = self.collect(CapturedAPI())
        self.assertEqual(snapshot["qemu_census_count"], 5)
        self.assertIn(CHILD, snapshot["qemu_census_candidates"])
        self.assertFalse(snapshot["qemu_metrics_complete"])
        self.assertIn(45340, [item["pid"] for item in snapshot["qemu"]])
        self.assertNotIn(131028, [item["pid"] for item in snapshot["qemu"]])
        self.assertIsNone(watchdog.evaluate(snapshot, watchdog.Limits())["observations"]["qemu_process_count"])

    def test_later_fourth_census_absence_cannot_rewrite_failed_third_check(self):
        api = CapturedAPI(clear_after=4)
        snapshot = self.collect(api)
        self.assertEqual(api.census_calls, 4)  # Initial census plus three reconciliation checks.
        self.assertNotIn(CHILD["pid"], api.enumerate_processes())
        self.assertFalse(snapshot["complete"])
        self.assertEqual(watchdog.evaluate(snapshot, watchdog.Limits())["action"], "terminate")

    def test_actual_departure_within_bound_keeps_current_job_metrics(self):
        snapshot = self.collect(CapturedAPI(clear_after=2))
        self.assertTrue(snapshot["complete"], snapshot["errors"])
        self.assertTrue(snapshot["qemu_metrics_complete"])
        self.assertEqual(snapshot["job_process_ids"], MEMBERS)
        self.assertIn(CHILD["pid"], snapshot["gone_enumerated_pids"])
        self.assertEqual(len(snapshot["qemu"]), 4)
        self.assertEqual(watchdog.evaluate(snapshot, watchdog.Limits())["action"], "continue")

    def test_98_percent_cutoff_remains_after_verified_child_departure(self):
        api = CapturedAPI(clear_after=2)
        api.system["physical_available_bytes"] = api.system["physical_total_bytes"] // 100
        snapshot = self.collect(api)
        decision = watchdog.evaluate(snapshot, watchdog.Limits(allow_physical_pressure=True))
        self.assertEqual(decision["action"], "terminate")
        self.assertIn("physical_memory_used_percent", decision["reasons"])

    def test_armed_monitor_terminates_only_verified_job_for_captured_failure(self):
        api = CapturedAPI(arm_first=True)
        receipt = self.folder / "receipt.json"
        with redirect_stdout(io.StringIO()):
            status = watchdog.monitor(self.path, self.folder / "telemetry.jsonl", receipt,
                self.folder / "ready.json", watchdog.Limits(allow_physical_pressure=True),
                poll_seconds=0.05, api=api)
        self.assertEqual(status, 2)
        result = json.loads(receipt.read_text())
        self.assertTrue(result["armed"])
        self.assertEqual(result["status"], "terminated_by_guard")
        self.assertEqual(api.terminated, [MANIFEST["job_name"]])
        self.assertEqual(result["last_snapshot"]["access_denied_reconciliations"][0]["previous_census"], CHILD)
        self.assertFalse(result["last_snapshot"]["qemu_metrics_complete"])
        self.assertIsNone(result["last_snapshot"]["decision"]["observations"]["qemu_process_count"])


if __name__ == "__main__":
    unittest.main(verbosity=2)
