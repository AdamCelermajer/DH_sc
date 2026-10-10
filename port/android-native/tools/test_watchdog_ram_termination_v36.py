"""Prompt host-RAM threshold termination without launching an emulator."""
from pathlib import Path
import json
import tempfile
import time
import uuid

from emulator_watchdog_v36 import Limits, monitor


class FakeAPI:
    launcher_pid = 41001
    target_pid = 41002
    launcher_created = 1001
    target_created = 1002

    def __init__(self):
        self.system_samples = 0
        self.process_census_samples = 0
        self.terminated = False

    def open_job(self, _name):
        return object()

    def process_snapshot(self, pid, metadata=None):
        parent = 0 if pid == self.launcher_pid else self.launcher_pid
        return {
            "pid": pid,
            "parent_pid": parent,
            "image": "test-watchdog.exe",
            "create_time_filetime": self.launcher_created if pid == self.launcher_pid else self.target_created,
            "private_bytes": 1,
            "working_set_bytes": 1,
            "handles": 1,
            "alive": True,
        }

    def target_in_job(self, pid, _handle, _created):
        return pid == self.target_pid

    def system_snapshot(self):
        self.system_samples += 1
        # First full sample is below the stop threshold; the next crosses it.
        available = 611 if self.system_samples == 1 else 200
        return {
            "commit_total_bytes": 100,
            "commit_limit_bytes": 200,
            "physical_available_bytes": available,
            "physical_total_bytes": 10000,
            "page_size_bytes": 1,
            "handles": 2,
            "processes": 2,
        }

    def enumerate_processes(self):
        self.process_census_samples += 1
        return {
            self.launcher_pid: {"pid": self.launcher_pid, "parent_pid": 0, "image": "test-watchdog.exe"},
            self.target_pid: {"pid": self.target_pid, "parent_pid": self.launcher_pid, "image": "test-watchdog.exe"},
        }

    def job_process_ids(self, _handle):
        return [self.target_pid]

    def terminate_job(self, _handle):
        self.terminated = True

    def close(self, _handle):
        pass


def main():
    api = FakeAPI()
    with tempfile.TemporaryDirectory(prefix="dh2-watchdog-ram-") as directory:
        root = Path(directory)
        manifest_path = root / "manifest.json"
        manifest = {
            "job_name": "Local\\DH2-Emulator-" + str(uuid.uuid4()),
            "launcher_pid": api.launcher_pid,
            "launcher_create_time": api.launcher_created,
            "target_pid": api.target_pid,
            "target_create_time": api.target_created,
        }
        manifest_path.write_text(json.dumps(manifest), encoding="utf-8")
        start = time.monotonic()
        code = monitor(
            manifest_path,
            root / "telemetry.jsonl",
            root / "receipt.json",
            root / "ready.json",
            Limits(1000, 1000, 1, 100, 30, True, 98),
            poll_seconds=0.05,
            api=api,
        )
        elapsed = time.monotonic() - start
        receipt = json.loads((root / "receipt.json").read_text(encoding="utf-8"))
        assert code == 2, (code, receipt)
        assert api.terminated, "98% host-memory threshold did not terminate the owned job"
        assert api.system_samples == 2, api.system_samples
        assert api.process_census_samples == 1, api.process_census_samples
        assert receipt["status"] == "terminated_by_guard", receipt
        assert "fast_physical_memory_stop" in receipt["last_snapshot"], receipt
        assert receipt["last_snapshot"]["decision"]["observations"]["physical_used_percent"] == 98.0
        assert elapsed < 0.75, f"RAM threshold termination took {elapsed:.3f}s"
    print("PASS watchdog terminates owned job promptly at >=98% host RAM; no emulator launched")


if __name__ == "__main__":
    main()
