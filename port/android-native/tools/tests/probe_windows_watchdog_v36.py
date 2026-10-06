"""Read-only native Windows API probe: system, self, QEMU census; no jobs open."""
from pathlib import Path
import ctypes
import hashlib
import importlib.util
import json
import os
import sys
SOURCE = Path(__file__).resolve().parents[1] / "emulator_watchdog_v36.py"
spec = importlib.util.spec_from_file_location("emulator_watchdog_v36", SOURCE)
wd = importlib.util.module_from_spec(spec); sys.modules[spec.name] = wd; spec.loader.exec_module(wd)
if "--expect-appcontainer-failure" in sys.argv:
    try:
        wd.WindowsAPI()
    except RuntimeError as error:
        assert "AppContainer" in str(error)
        receipt = {"status": "PASS", "scope": "Restricted tool token refuses false host census",
            "watchdog_sha256": hashlib.sha256(SOURCE.read_bytes()).hexdigest(), "diagnostic": str(error)}
        wd.atomic_json(SOURCE.parents[1] / "reports" / "emulator-watchdog-v36" / "appcontainer-failclosed-probe.json", receipt)
        print(json.dumps(receipt)); raise SystemExit(0)
    raise RuntimeError("Expected AppContainer refusal; do not run this branch with a full-host token")
api = wd.WindowsAPI()
current = api.process_snapshot(os.getpid())
snapshot = wd.system_qemu_snapshot(api)
assert current["pid"] == os.getpid() and current["create_time_filetime"] > 0
assert current["private_bytes"] >= 0 and current["handles"] > 0
assert snapshot["system"]["commit_limit_bytes"] > 0 and snapshot["system"]["page_size_bytes"] > 0
assert snapshot["aggregate_qemu_private_bytes"] == sum(p["private_bytes"] for p in snapshot["qemu"])
receipt = {"status": "PASS", "scope": "Read-only system/self/process census; no job/emulator lifecycle call",
    "watchdog_sha256": hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
    "pointer_size": ctypes.sizeof(ctypes.c_void_p),
    "performance_info_size": ctypes.sizeof(api.PerformanceInfo),
    "memory_counters_ex_size": ctypes.sizeof(api.MemoryCounters),
    "process_entry_size": ctypes.sizeof(api.ProcessEntry), "self": current, "preflight": snapshot}
wd.atomic_json(SOURCE.parents[1] / "reports" / "emulator-watchdog-v36" / "native-readonly-probe.json", receipt)
print(json.dumps(receipt))
