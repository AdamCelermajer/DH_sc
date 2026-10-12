#!/usr/bin/env python3
"""One serial loader milestone. Plan by default; never configures/builds/installs.
Controlled fixtures and actual runtime/device gates have distinct result classes.
Provider commands are explicit argv vectors, never shell strings or PATH guesses.
"""
import argparse
import datetime
import json
import os
from pathlib import Path
import subprocess
import sys
import uuid


def read_json(path):
    with Path(path).open(encoding="utf-8-sig") as stream:
        return json.load(stream)


def tail(path):
    with path.open("rb") as stream:
        stream.seek(0, 2)
        stream.seek(max(0, stream.tell() - 2 * 1024 * 1024))
        return stream.read().decode("utf-8", errors="replace")


def receipt_from_output(text):
    for line in reversed(text.splitlines()):
        try:
            value = json.loads(line)
        except ValueError:
            continue
        if isinstance(value, dict):
            return value
    return None


def expand(value, values):
    for name, replacement in values.items():
        value = value.replace("{" + name + "}", str(replacement))
    if "{" in value or "}" in value:
        raise ValueError("Unbound argument placeholder: " + value)
    return value


def rendering_receipt(receipt, identity):
    metrics = receipt.get("metrics", {})
    if metrics.get("level_identity") != identity:
        raise ValueError("Wrong canonical level identity")
    entries = receipt.get("render_accounting")
    if not isinstance(entries, list) or not entries:
        raise ValueError("Missing per-source mob/chest renderer accounting")
    identifiers = set()
    for entry in entries:
        key = entry.get("source_id")
        if not key or key in identifiers:
            raise ValueError("Missing/duplicate authored source identity")
        identifiers.add(key)
        if not entry.get("source_path") or not isinstance(entry.get("authored_name"), str):
            raise ValueError("Renderer record lost authored provenance")
        if entry.get("kind") not in ("Character", "OpenableContainer"):
            raise ValueError("Unexpected renderer accounting family")
        if entry.get("eligible") is True:
            if entry.get("submitted") is not True or not entry.get("draw_submission_id"):
                raise ValueError("Eligible mob/chest lacks actual draw submission")
            if entry.get("same_native_receiver") is not True or entry.get("placement_matches_source") is not True:
                raise ValueError("Submitted actor lost receiver or authored placement")
        elif entry.get("eligible") is False:
            if not entry.get("source_eligibility_reason"):
                raise ValueError("Ineligible actor has no original-state reason")
        else:
            raise ValueError("Actor eligibility is unknown")
    if identity == "SWAMP":
        if not any(e["kind"] == "Character" and e.get("role") == "mob" and e.get("eligible") is True for e in entries):
            raise ValueError("Fresh SWAMP has no eligible mob actual draw")
        if not any(e["kind"] == "OpenableContainer" and e.get("eligible") is True for e in entries):
            raise ValueError("Fresh SWAMP has no eligible chest actual draw")
        if sum(e["kind"] == "Character" for e in entries) != 50:
            raise ValueError("Fixed SWAMP must account for all50 authored Characters")
        if sum(e["kind"] == "OpenableContainer" for e in entries) != 5:
            raise ValueError("Fixed SWAMP must account for all5 authored chests")
    expected = receipt.get("authored_actor_ids")
    if not isinstance(expected, list) or set(expected) != identifiers or len(expected) != len(identifiers):
        raise ValueError("Renderer accounting does not match source inventory IDs")


def validate_runtime(case, receipt, run_id, integration_id, configured):
    if not isinstance(receipt, dict):
        raise ValueError("Provider produced no structured receipt")
    for key, expected in (("schema", 1), ("run_id", run_id), ("case_id", case["id"]), ("integration_id", integration_id), ("validation", "PASS")):
        if receipt.get(key) != expected:
            raise ValueError("Missing/mismatched current runtime receipt field: " + key)
    if receipt.get("genuine_providers") is not True or receipt.get("fixture_providers") != []:
        raise ValueError("Controlled providers cannot satisfy a runtime gate")
    for key, expected in case.get("runtime_metrics", {}).items():
        if receipt.get("metrics", {}).get(key) != expected:
            raise ValueError("Runtime invariant absent/mismatched: " + key)
    if case.get("render_identity"):
        rendering_receipt(receipt, case["render_identity"])
    if case.get("device"):
        if receipt.get("device_id") != configured.get("isolated_device_id") or receipt.get("package_id") != configured.get("package_id"):
            raise ValueError("Device/package does not match explicit isolated binding")
        screenshots = receipt.get("screenshots", [])
        if not screenshots or any(not Path(item).is_file() for item in screenshots):
            raise ValueError("Current device evidence missing")
    if case.get("generated"):
        if receipt.get("original_generator_and_serializer") is not True:
            raise ValueError("Inspection XML/layout cannot satisfy native Generate gate")
        if not receipt.get("generated_stream_provenance") or receipt.get("metrics", {}).get("generator_restarts") != 0:
            raise ValueError("Missing original generated-stream provenance or generator rerolled")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", required=True)
    parser.add_argument("--execute", action="store_true", help="Run already-built controlled fixtures serially")
    parser.add_argument("--runtime", action="store_true", help="Also run explicitly bound genuine Main runtime/device providers")
    args = parser.parse_args()
    suite = read_json(Path(__file__).with_name("suite.json"))
    config = read_json(args.config)
    if config.get("schema") != 1:
        raise ValueError("Configuration schema must be1")
    if not args.execute:
        print(json.dumps({"status": "NOT_RUN", "serial": True, "cases": suite["cases"], "runtime_providers": list(config.get("runtime_providers", {})), "note": "No compile/build/emulator/process is started in plan mode"}, indent=2))
        return 0
    integration_id = config.get("integration_id")
    if not integration_id:
        raise ValueError("Explicit coherent integration_id is required")
    output_root = Path(config["output_root"])
    if not output_root.is_absolute():
        raise ValueError("output_root must be an explicit absolute path")
    output_root.mkdir(parents=True, exist_ok=True)
    lock = output_root / ".loader-milestone.lock"
    try:
        lock_fd = os.open(str(lock), os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o600)
    except FileExistsError:
        raise ValueError("Another or interrupted milestone owns the lock; inspect it manually, no automatic retry/removal")
    run_id = str(uuid.uuid4())
    os.write(lock_fd, json.dumps({"pid": os.getpid(), "run_id": run_id}).encode())
    os.close(lock_fd)
    results = []
    run_dir = output_root / run_id
    try:
        run_dir.mkdir()
        private_save = run_dir / "private-save"
        private_save.mkdir()
        try:
            build = read_json(config["build_receipt"]) if config.get("build_receipt") else None
        except (OSError, ValueError):
            build = None
        build_ok = bool(build and build.get("validation") == "PASS" and build.get("integration_id") == integration_id and build.get("coherent_source_batches") == suite["coherent_source_batches"])
        stopped = False
        for case in suite["cases"]:
            result = {"case_id": case["id"], "category": case["category"], "required": case.get("required", True), "coverage": case["coverage"]}
            results.append(result)
            if stopped:
                result.update(status="BLOCKED_BY_PREVIOUS_FAILURE", reason="Safe serial suite stopped; no retry or additional world/device action")
                continue
            controlled = case["category"] == "controlled_fixture"
            binding = config.get("executables", {}).get(case["id"]) if controlled else config.get("runtime_providers", {}).get(case["id"])
            if controlled and not build_ok:
                result.update(status="UNAVAILABLE_BUILD_PROVENANCE", reason="One successful coherent milestone build receipt is required; historical binaries do not qualify")
                continue
            if not controlled and not args.runtime:
                result.update(status="UNAVAILABLE_PROVIDER", reason="Actual Main runtime/device gate is pending; --runtime and explicit genuine provider binding required")
                continue
            if not binding:
                result.update(status="UNAVAILABLE_PROVIDER", reason=case.get("unavailable_reason", "No explicitly bound already-built executable"))
                continue
            if not controlled and (binding.get("genuine") is not True or (case.get("device") and (not binding.get("isolated_device_id") or not binding.get("package_id")))):
                result.update(status="UNAVAILABLE_PROVIDER", reason="Genuine provider and explicit isolated device/package declaration required")
                continue
            executable = Path(binding["executable"])
            if not executable.is_absolute() or not executable.is_file():
                result.update(status="UNAVAILABLE_PROVIDER", reason="Explicit absolute executable is absent")
                continue
            case_dir = run_dir / case["id"]
            case_dir.mkdir()
            receipt_path = case_dir / "provider-receipt.json"
            values = dict(config.get("paths", {}), run_id=run_id, case_id=case["id"], integration_id=integration_id, receipt=receipt_path, output=case_dir, private_save=private_save)
            try:
                arguments = [expand(value, values) for value in binding.get("arguments", case.get("arguments", []))]
            except ValueError as error:
                result.update(status="UNAVAILABLE_INPUT", reason=str(error))
                continue
            missing = [name for name in case.get("required_paths", []) if name not in values or not Path(values[name]).exists()]
            if missing:
                result.update(status="UNAVAILABLE_INPUT", reason="Missing required original input(s): " + ",".join(missing))
                continue
            if controlled and case["id"] not in build.get("case_ids", []):
                result.update(status="UNAVAILABLE_BUILD_PROVENANCE", reason="Build receipt omits this fixture")
                continue
            stdout_path, stderr_path = case_dir / "stdout.txt", case_dir / "stderr.txt"
            environment = dict(os.environ, DH2_MILESTONE_RUN_ID=run_id, DH2_MILESTONE_CASE_ID=case["id"], DH2_MILESTONE_RECEIPT=str(receipt_path))
            try:
                with stdout_path.open("wb") as out, stderr_path.open("wb") as err:
                    process = subprocess.Popen([str(executable)] + arguments, stdout=out, stderr=err, shell=False, env=environment, cwd=str(case_dir))
                    try:
                        code = process.wait(timeout=int(binding.get("timeout_seconds", case.get("timeout_seconds", 180))))
                    except subprocess.TimeoutExpired:
                        process.terminate()
                        try:
                            process.wait(timeout=5)
                        except subprocess.TimeoutExpired:
                            process.kill()
                            process.wait()
                        raise ValueError("Timed out; only this owned runner process stopped. Inspect its retained runtime/device prefix before any retry")
                text = tail(stdout_path)
                result.update(exit_code=code, stdout=str(stdout_path), stderr=str(stderr_path))
                if code != 0:
                    raise ValueError("Executable exit status is " + str(code))
                if controlled:
                    receipt = receipt_from_output(text)
                    if case.get("pass_marker") and case["pass_marker"] not in text:
                        raise ValueError("Expected fixture PASS marker absent")
                    for key, expected in case.get("fixture_metrics", {}).items():
                        if not receipt or receipt.get(key) != expected:
                            raise ValueError("Fixture metric absent/mismatched: " + key)
                    result.update(status="PASS_CONTROLLED_FIXTURE", fixture_receipt=receipt, actual_runtime_gate=False)
                else:
                    receipt = read_json(receipt_path)
                    validate_runtime(case, receipt, run_id, integration_id, binding)
                    result.update(status="PASS_RUNTIME_GATE", receipt=str(receipt_path))
            except (OSError, ValueError) as error:
                result.update(status="FAIL", reason=str(error))
                stopped = True
        controlled = [row for row in results if row["category"] == "controlled_fixture"]
        runtime = [row for row in results if row["category"] != "controlled_fixture" and row["required"]]
        fixture_pass = all(row["status"] == "PASS_CONTROLLED_FIXTURE" for row in controlled)
        runtime_pass = all(row["status"] == "PASS_RUNTIME_GATE" for row in runtime)
        status = "FAIL" if stopped else "PASS_MILESTONE_GATES" if fixture_pass and runtime_pass else "INCOMPLETE_UNAVAILABLE_GATES"
        summary = {"schema": 1, "run_id": run_id, "integration_id": integration_id, "created_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(), "status": status, "serial": True, "automatic_retries": 0, "controlled_fixture_pass": fixture_pass, "actual_runtime_gates_pass": runtime_pass, "whole_gameplay_verified": False, "results": results}
        destination = run_dir / "milestone-result.json"
        destination.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
        print(str(destination))
        return 1 if stopped else 0 if fixture_pass and runtime_pass else 2
    finally:
        # This process created the exclusive lock. No foreign/stale lock is removed.
        lock.unlink()


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, KeyError) as error:
        print("Milestone unavailable: " + str(error), file=sys.stderr)
        sys.exit(2)