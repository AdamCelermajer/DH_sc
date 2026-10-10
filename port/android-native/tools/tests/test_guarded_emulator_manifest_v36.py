"""The guarded launcher publishes each fresh manifest in one atomic write."""
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import launch_guarded_emulator_v36 as launcher


class ManifestPublicationTests(unittest.TestCase):
    def test_physical_pressure_is_in_the_first_and_only_manifest_replace(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            sdk = root / "sdk"
            executable = sdk / "emulator" / "emulator.exe"
            executable.parent.mkdir(parents=True)
            executable.write_bytes(b"test executable placeholder")
            registry = root / "registry"
            manifest_writes = []

            class FakeJob:
                name = r"Local\DH2-Emulator-manifest-regression"

                def __init__(self, _limit):
                    pass

                def create_suspended(self, *_args):
                    return {"pid": 1001, "create_time_filetime": 2002}

                def limits(self):
                    return {"job_memory_limit": 15 * launcher.GIB}

                def resume(self):
                    pass

                def members(self):
                    return []

                def terminate(self):
                    pass

                def close(self):
                    pass

            class FakeWatchdog:
                def poll(self):
                    return None

                def wait(self, timeout=None):
                    return 0

            def start_watchdog(command, **_kwargs):
                ready_path = Path(command[command.index("--ready") + 1])
                receipt_path = Path(command[command.index("--receipt") + 1])
                ready_path.write_text(
                    json.dumps({"verified": True, "job_name": FakeJob.name}),
                    encoding="utf8",
                )
                receipt_path.write_text(
                    json.dumps({"status": "job_completed"}), encoding="utf8"
                )
                return FakeWatchdog()

            real_atomic = launcher.atomic

            def record_atomic(path, value):
                if Path(path).name.endswith(".manifest.json"):
                    manifest_writes.append((Path(path), dict(value)))
                return real_atomic(path, value)

            snapshot = {
                "qemu": [],
                "system": {
                    "commit_limit_bytes": 100 * launcher.GIB,
                    "commit_total_bytes": 10 * launcher.GIB,
                    "physical_available_bytes": 0,
                    "physical_total_bytes": 32 * launcher.GIB,
                    "page_size_bytes": 4096,
                    "handles": 10,
                    "processes": 10,
                },
            }

            with (
                patch.object(launcher, "REGISTRY", registry),
                patch.object(launcher, "MemoryJob", FakeJob),
                patch.object(launcher, "CreateMutex", return_value=11),
                patch.object(launcher, "Wait", return_value=0),
                patch.object(launcher, "ReleaseMutex"),
                patch.object(launcher, "Close"),
                patch.object(launcher, "registry_live", return_value=None),
                patch.object(launcher, "system_qemu_snapshot", return_value=snapshot),
                patch.object(launcher, "free_port"),
                patch.object(launcher, "creation_time", return_value=3003),
                patch.object(
                    launcher.subprocess,
                    "run",
                    return_value=type("Result", (), {"stdout": b"Medium_Phone_API_37.0\n"})(),
                ),
                patch.object(launcher.subprocess, "Popen", side_effect=start_watchdog),
                patch.object(launcher, "atomic", side_effect=record_atomic),
                patch.dict(os.environ, {"ANDROID_HOME": str(sdk)}),
                patch.object(
                    sys,
                    "argv",
                    [
                        "launch_guarded_emulator_v36.py",
                        "--avd",
                        "Medium_Phone_API_37.0",
                        "--hard-memory-gib",
                        "15",
                        "--soft-memory-gib",
                        "14",
                        "--allow-guest-paging",
                        "--allow-physical-pressure",
                    ],
                ),
            ):
                self.assertEqual(launcher.main(), 0)

            self.assertEqual(len(manifest_writes), 1)
            manifest_path, written_record = manifest_writes[0]
            self.assertTrue(written_record["allow_physical_pressure"])
            saved_manifest = json.loads(manifest_path.read_text(encoding="utf8"))
            self.assertTrue(saved_manifest["allow_physical_pressure"])


if __name__ == "__main__":
    unittest.main()
