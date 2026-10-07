"""Refresh source-only receipts after the parent dependency headers are stable."""
import datetime
import hashlib
import json
from pathlib import Path

root = Path(__file__).resolve().parents[4]
packet = Path(__file__).resolve().parent

def entry(path):
    raw = path.read_bytes()
    return {"path": path.relative_to(root).as_posix(), "bytes": len(raw),
            "sha256": hashlib.sha256(raw).hexdigest()}

def write(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")

receipt_path = packet / "host-receipt.json"
receipt = json.loads(receipt_path.read_text())
paths = [root / row["path"] for row in receipt["inputs"]]
paths += [packet / "audio_session_transport_v40.hpp",
          packet.parent / "focus/audio_native_session_v40.hpp",
          packet.parent / "focus/audio_lifecycle_gate_v40.hpp"]
receipt["inputs"] = [entry(path) for path in dict.fromkeys(paths)]
receipt["scope"] = ("53 O2 ASan+UBSan fixture checks over explicit observers and genuine names. "
                    "Runtime/session bridges compile on both strict ABIs. Actual owning session "
                    "lifecycle and production runtime behavior are parent tests, not this fixture.")
receipt["generated_utc"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
write(receipt_path, receipt)

checklist_path = packet / "checklist.json"
checklist = json.loads(checklist_path.read_text())
checklist["production_transport"] = "audio_session_transport_v40 -> owning AudioNativeSessionV40.submit_actual_play"
checklist["direct_runtime_transport"] = "controlled independent fixtures only; bypasses session close acceptance"
write(checklist_path, checklist)

old = json.loads((packet / "manifest.json").read_text())
dependencies = [root / row["path"] for row in old["external_dependencies"]]
dependencies += paths[-2:]
owned = [entry(path) for path in sorted(packet.rglob("*"))
         if path.is_file() and path.name != "manifest.json"
         and not {"build", ".git", "__pycache__"}.intersection(path.relative_to(packet).parts)
         and path.suffix.lower() not in {".o", ".obj", ".so", ".a", ".dll", ".exe", ".class", ".pyc"}]
manifest = {"scope": old["scope"], "generated_utc": receipt["generated_utc"],
            "owned": owned, "external_dependencies": [entry(p) for p in dict.fromkeys(dependencies)]}
write(packet / "manifest.json", manifest)
print("sealed", len(owned), "owned source files", entry(packet / "manifest.json")["sha256"])
