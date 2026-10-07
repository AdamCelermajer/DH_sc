"""Fixture-only watchdog tests. Never launch, monitor or terminate an emulator."""
from pathlib import Path
import importlib.util
import json
import hashlib
import sys
import tempfile
import unittest
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / "emulator_watchdog_v36.py"
spec = importlib.util.spec_from_file_location("emulator_watchdog_v36", SOURCE)
wd = importlib.util.module_from_spec(spec); sys.modules[spec.name] = wd; spec.loader.exec_module(wd)


def process(pid, private=wd.GIB, created=None, image="qemu-system-x86_64.exe", parent=100):
    return {"pid": pid, "parent_pid": parent, "image": image,
            "create_time_filetime": created or pid * 100, "private_bytes": private,
            "working_set_bytes": private // 8, "handles": 35, "alive": True}


def snapshot():
    return {"complete": True, "errors": [], "manifest_unchanged": True,
        "expected_owner": {"pid": 100, "create_time_filetime": 1000},
        "owner": process(100, created=1000, image="python.exe", parent=1),
        "system": {"commit_total_bytes": 12*wd.GIB, "commit_limit_bytes": 64*wd.GIB,
                   "physical_available_bytes": 32*wd.GIB},
        "job_process_ids": [10], "job_members": [process(10)],
        "qemu": [process(10), process(20)], "elapsed_seconds": 0.5}


class PureDecisionTests(unittest.TestCase):
    def test_defaults_and_good_snapshot(self):
        result = wd.evaluate(snapshot(), wd.Limits())
        self.assertEqual(result["action"], "continue")
        self.assertEqual(result["observations"]["owned_private_bytes"], wd.GIB)
        self.assertEqual(result["observations"]["aggregate_qemu_private_bytes"], 2*wd.GIB)
        self.assertEqual(wd.Limits().timeout_seconds, 1200)

    def test_private_commit_is_not_working_set(self):
        value = snapshot(); value["job_members"][0].update(private_bytes=58*wd.GIB, working_set_bytes=wd.GIB)
        result = wd.evaluate(value, wd.Limits())
        self.assertIn("owned_job_private_limit", result["reasons"])
        value = snapshot(); value["job_members"][0]["working_set_bytes"] = 100*wd.GIB
        self.assertEqual(wd.evaluate(value, wd.Limits())["action"], "continue")

    def test_requested_paging_preserves_commit_identity_and_job_stops(self):
        value=snapshot();value["system"]["physical_available_bytes"]=1
        limits=wd.Limits(allow_physical_pressure=True)
        self.assertEqual(wd.evaluate(value,limits)["action"],"continue")
        value["system"]["commit_total_bytes"]=49*wd.GIB
        self.assertIn("system_commit_headroom",wd.evaluate(value,limits)["reasons"])
        value=snapshot();value["system"]["physical_available_bytes"]=1
        value["job_members"][0]["private_bytes"]=5*wd.GIB
        self.assertIn("owned_job_private_limit",wd.evaluate(value,limits)["reasons"])
        value=snapshot();value["owner"]=None
        self.assertEqual(wd.evaluate(value,limits)["action"],"terminate")
        with self.assertRaises(ValueError):wd.Limits(allow_physical_pressure=1).validate()

    def test_independent_all_qemu_aggregate(self):
        value = snapshot(); value["qemu"][1]["private_bytes"] = 22*wd.GIB
        result = wd.evaluate(value, wd.Limits())
        self.assertIn("aggregate_qemu_private_limit", result["reasons"])
        self.assertNotIn("owned_job_private_limit", result["reasons"])

    def test_threshold_boundaries(self):
        for delta, action in ((-1, "continue"), (0, "terminate"), (1, "terminate")):
            value = snapshot(); value["job_members"][0]["private_bytes"] = 5*wd.GIB+delta
            self.assertEqual(wd.evaluate(value, wd.Limits())["action"], action)
        value = snapshot(); value["system"]["commit_total_bytes"] = 48*wd.GIB
        value["system"]["physical_available_bytes"] = 6*wd.GIB
        self.assertEqual(wd.evaluate(value, wd.Limits())["action"], "continue")
        value["system"]["commit_total_bytes"] += 1
        self.assertIn("system_commit_headroom", wd.evaluate(value, wd.Limits())["reasons"])
        value["system"]["physical_available_bytes"] -= 1
        self.assertIn("physical_available_floor", wd.evaluate(value, wd.Limits())["reasons"])

    def test_owner_death_reuse_and_timeout(self):
        for owner in (None, dict(process(100, created=1001), alive=False), process(100, created=1001)):
            value = snapshot(); value["owner"] = owner
            self.assertEqual(wd.evaluate(value, wd.Limits())["action"], "terminate")
        value = snapshot(); value["elapsed_seconds"] = 1200
        self.assertIn("monitor_timeout", wd.evaluate(value, wd.Limits())["reasons"])

    def test_fail_closed_for_missing_or_invalid_metrics(self):
        cases = []
        value = snapshot(); value["complete"] = False; cases.append(value)
        value = snapshot(); value["errors"] = ["Access denied"]; cases.append(value)
        value = snapshot(); del value["system"]["commit_limit_bytes"]; cases.append(value)
        value = snapshot(); value["job_members"][0]["private_bytes"] = None; cases.append(value)
        value = snapshot(); value["job_members"][0]["handles"] = -1; cases.append(value)
        value = snapshot(); value["job_members"] = []; cases.append(value)
        value = snapshot(); value["manifest_unchanged"] = False; cases.append(value)
        value = snapshot(); value["elapsed_seconds"] = float("nan"); cases.append(value)
        value = snapshot(); value["qemu"].append(dict(value["qemu"][0], create_time_filetime=12345)); cases.append(value)
        for value in cases:
            self.assertEqual(wd.evaluate(value, wd.Limits())["action"], "terminate")

    def test_job_empty_is_complete_with_owner_alive(self):
        value = snapshot(); value["job_members"] = []; value["job_process_ids"] = []
        self.assertEqual(wd.evaluate(value, wd.Limits())["action"], "complete")

    def test_images_and_limits(self):
        for name in ("QEMU-SYSTEM-X86_64.EXE", "qemu-system-aarch64.exe", "emulator.exe", "emulator-headless.exe"):
            self.assertTrue(wd.is_emulator_image(name))
        for name in ("adb.exe", "python.exe", "qemu-not-a-binary.txt", "myqemu.exe"):
            self.assertFalse(wd.is_emulator_image(name))
        for limits in (wd.Limits(soft_job_bytes=0), wd.Limits(timeout_seconds=float("nan")), wd.Limits(soft_job_bytes=True)):
            with self.assertRaises(ValueError): wd.evaluate(snapshot(), limits)


class FakeAPI:
    def __init__(self, failure=None):
        self.failure = failure; self.cycle = 0; self.terminated = []; self.closed = []; self.opened = []
        self.job = "fixture-job-handle"
        self.records = {100: process(100, created=1000, image="python.exe", parent=1),
            10: process(10, created=2000), 20: process(20, created=3000, parent=999),
            # A stale descendant PID with creation predating launcher must not
            # be admitted into the exact launcher tree.
            30: process(30, created=900, image="python.exe")}

    def open_job(self, name): self.opened.append(name); return self.job
    def close(self, handle): self.closed.append(handle)
    def terminate_job(self, handle): self.terminated.append(handle)
    def target_in_job(self, pid, handle, expected_create_time=None):
        if self.failure == "membership": return False
        return pid == 10
    def process_snapshot(self, pid, metadata=None):
        if self.failure == "owner_reuse" and self.cycle >= 2 and pid == 100:
            return dict(self.records[pid], create_time_filetime=9000)
        if self.failure == "owner_exit" and self.cycle >= 2 and pid == 100:
            raise wd.ProcessGone(pid)
        value = dict(self.records[pid])
        if self.failure == "target_identity" and pid == 10:
            value["create_time_filetime"] = 9999
        if self.failure == "memory" and self.cycle >= 2 and pid == 10:
            value["private_bytes"] = 58*wd.GIB
        return value
    def job_process_ids(self, handle):
        return [] if self.cycle >= 2 and not self.failure else [10]
    def enumerate_processes(self): return self.records
    def system_snapshot(self):
        self.cycle += 1
        if self.failure == "metrics" and self.cycle >= 2:
            raise OSError("Explicit fixture monitoring access failure")
        return snapshot()["system"]


class ProtocolTests(unittest.TestCase):
    def setUp(self):
        self.fixture_root = SOURCE.parents[1] / "reports" / "emulator-watchdog-v36" / "fixture-work"
        self.fixture_root.mkdir(parents=True, exist_ok=True)
        self.temporary = tempfile.TemporaryDirectory(dir=self.fixture_root); self.root = Path(self.temporary.name)
        if not self.root.resolve().is_relative_to(self.fixture_root.resolve()):
            raise RuntimeError("Fixture cleanup path escaped owned report directory")
        self.manifest = self.root / "manifest.json"; self.telemetry = self.root / "telemetry.jsonl"
        self.receipt = self.root / "receipt.json"; self.ready = self.root / "ready.json"
        self.data = {"job_name": "Local\\DH2-Emulator-12345678-1234-4234-8234-123456789abc",
                     "launcher_pid": 100, "launcher_create_time": 1000,
                     "target_pid": 10, "target_create_time": 2000}
        wd.atomic_json(self.manifest, self.data)

    def tearDown(self): self.temporary.cleanup()

    def run_monitor(self, api):
        with patch.object(wd.time, "sleep", return_value=None), patch("builtins.print"):
            return wd.monitor(self.manifest, self.telemetry, self.receipt, self.ready, wd.Limits(), api=api)

    def test_ready_and_clean_completion(self):
        api = FakeAPI(); self.assertEqual(self.run_monitor(api), 0)
        ready = json.loads(self.ready.read_text())
        self.assertTrue(ready["verified"]); self.assertEqual(ready["job_name"], self.data["job_name"])
        self.assertEqual(api.terminated, []); self.assertEqual(api.closed, [api.job])
        first = json.loads(self.telemetry.read_text().splitlines()[0])
        self.assertEqual([p["pid"] for p in first["job_members"]], [10])
        self.assertEqual(sorted(p["pid"] for p in first["qemu"]), [10,20])
        self.assertEqual(sorted(p["pid"] for p in first["owned_process_tree"]), [10,100])

    def test_only_verified_job_terminated_for_all_runtime_failures(self):
        for failure in ("memory", "metrics", "owner_reuse", "owner_exit"):
            with self.subTest(failure=failure):
                self.ready.unlink(missing_ok=True)
                api = FakeAPI(failure); self.assertEqual(self.run_monitor(api), 2)
                self.assertEqual(api.terminated, [api.job])
                self.assertEqual(api.closed, [api.job])
                receipt = json.loads(self.receipt.read_text())
                self.assertTrue(receipt["termination_succeeded"])
                # The fixture unrelated QEMU20 is observed but no PID cleanup
                # API exists or is invoked. The named job contains only PID10.
                if failure != "metrics":
                    self.assertEqual(receipt["last_snapshot"]["job_process_ids"], [10])

    def test_unverified_target_never_arms_or_terminates_job(self):
        for failure in ("membership", "target_identity"):
            api = FakeAPI(failure); self.assertEqual(self.run_monitor(api), 3)
            self.assertFalse(self.ready.exists()); self.assertEqual(api.terminated, [])
            self.assertEqual(json.loads(self.receipt.read_text())["status"], "bootstrap_failed")

    def test_unavailable_telemetry_fails_closed_after_verification(self):
        self.telemetry.mkdir()
        api = FakeAPI(); self.assertEqual(self.run_monitor(api), 2)
        self.assertEqual(api.terminated, [api.job]); self.assertFalse(self.ready.exists())

    def test_manifest_namespace_and_creation_identity(self):
        for update in ({"job_name": "Local\\UnrelatedJob"}, {"target_pid": True}, {"launcher_create_time": 0}):
            wd.atomic_json(self.manifest, dict(self.data, **update))
            with self.assertRaises(ValueError): wd.read_manifest(self.manifest)

    def test_preflight_is_read_only_and_aggregates_private_usage(self):
        api = FakeAPI(); result = wd.system_qemu_snapshot(api)
        self.assertEqual(result["aggregate_qemu_private_bytes"], 2*wd.GIB)
        self.assertEqual(api.opened, []); self.assertEqual(api.terminated, [])

    def test_bounded_telemetry_rotation(self):
        bounded = wd.BoundedTelemetry(self.telemetry, 65536)
        for _ in range(200): bounded.append({"value": "x"*1000})
        for path in (self.telemetry, self.telemetry.with_name(self.telemetry.name+".1")):
            self.assertLessEqual(path.stat().st_size, 65536)
            for line in path.read_text().splitlines(): self.assertIsInstance(json.loads(line), dict)


if __name__ == "__main__":
    program = unittest.main(verbosity=2, exit=False)
    wd.atomic_json(SOURCE.parents[1] / "reports" / "emulator-watchdog-v36" / "fixture-receipt.json", {
        "status": "PASS" if program.result.wasSuccessful() else "FAIL",
        "tests": program.result.testsRun, "failures": len(program.result.failures),
        "errors": len(program.result.errors), "live_emulator_operations": False,
        "watchdog_sha256": hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
        "test_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        "scope": "Pure decisions and injected fake-job readiness/termination/telemetry protocol"})
    raise SystemExit(0 if program.result.wasSuccessful() else 1)
