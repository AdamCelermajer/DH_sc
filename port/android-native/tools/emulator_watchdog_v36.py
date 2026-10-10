"""Independent Windows memory guard. Only a verified named Job Object is killed.

The launcher creates a fresh job and suspended target, writes its immutable
manifest, starts this process OUTSIDE that job, and waits for --ready before
resuming the target. No emulator, ADB or unrelated process is launched here.
All byte limits use private committed memory, never working-set residency.
"""
from __future__ import annotations
import argparse
import ctypes as C
from ctypes import wintypes as W
from dataclasses import asdict, dataclass
import hashlib
import json
import math
import os
from pathlib import Path
import time
import uuid

GIB = 1024**3
MIB = 1024**2
JOB_OBJECT_QUERY = 0x0004
JOB_OBJECT_TERMINATE = 0x0008
JOB_OBJECT_BASIC_PROCESS_ID_LIST = 3
MAX_JOB_PROCESSES = 4096
MEMBERSHIP_DIAGNOSTIC_PID_LIMIT = 64
EXIT_DRAIN_DELAY_SECONDS = 0.075
EXIT_DRAIN_WAIT_BUDGET_SECONDS = 0.15
ACCESS_DENIED_CENSUS_DELAY_SECONDS = 0.075
ACCESS_DENIED_CENSUS_WAIT_BUDGET_SECONDS = 0.15
FAST_PHYSICAL_MEMORY_POLL_SECONDS = 0.1
QEMU_IMAGES = {"emulator.exe", "emulator-headless.exe"}


@dataclass(frozen=True)
class Limits:
    soft_job_bytes: int = 5 * GIB
    aggregate_qemu_bytes: int = 12 * GIB
    min_commit_headroom_bytes: int = 16 * GIB
    min_physical_available_bytes: int = 6 * GIB
    timeout_seconds: float = 1200.0
    allow_physical_pressure: bool = False
    max_physical_used_percent: int = 98

    def validate(self):
        if not isinstance(self.allow_physical_pressure, bool):
            raise ValueError("Physical-pressure policy must be explicit boolean")
        if (isinstance(self.max_physical_used_percent, bool)
                or not isinstance(self.max_physical_used_percent, int)
                or not 1 <= self.max_physical_used_percent <= 100):
            raise ValueError("Physical-memory stop percentage must be an integer from 1 to 100")
        for name in ("soft_job_bytes", "aggregate_qemu_bytes",
                     "min_commit_headroom_bytes", "min_physical_available_bytes"):
            if isinstance(getattr(self, name), bool) or not isinstance(getattr(self, name), int) or getattr(self, name) <= 0:
                raise ValueError(f"Invalid positive limit: {name}")
        if not math.isfinite(self.timeout_seconds) or self.timeout_seconds <= 0:
            raise ValueError("Invalid positive timeout")


def is_emulator_image(image: str) -> bool:
    name = Path(image.replace("\\", "/")).name.casefold()
    return name in QEMU_IMAGES or (name.startswith("qemu") and name.endswith(".exe"))


def physical_memory_used_percent(system: dict) -> float:
    available = system["physical_available_bytes"]
    total = system["physical_total_bytes"]
    if (isinstance(available, bool) or not isinstance(available, int) or available < 0 or
            isinstance(total, bool) or not isinstance(total, int) or total <= 0 or available > total):
        raise ValueError("Invalid physical-memory counters")
    return 100.0 * (total - available) / total


def evaluate(snapshot: dict, limits: Limits) -> dict:
    """Pure, fail-closed decision. It performs no OS operation or cleanup."""
    limits.validate()
    reasons = []
    observations = {}
    try:
        if snapshot.get("complete") is not True or snapshot.get("errors"):
            reasons.append("monitoring_unavailable")
        expected = snapshot["expected_owner"]
        owner = snapshot.get("owner")
        if not owner or not owner.get("alive"):
            reasons.append("launcher_missing_or_exited")
        elif (owner["pid"], owner["create_time_filetime"]) != (
                expected["pid"], expected["create_time_filetime"]):
            reasons.append("launcher_identity_reused")
        if snapshot.get("manifest_unchanged") is not True:
            reasons.append("manifest_changed_or_unavailable")
        system = snapshot["system"]
        commit = system["commit_total_bytes"]
        limit = system["commit_limit_bytes"]
        physical = system["physical_available_bytes"]
        physical_total = system["physical_total_bytes"]
        if any(isinstance(x, bool) or not isinstance(x, int) or x < 0
               for x in (commit, limit, physical, physical_total)) or not limit or not physical_total or physical > physical_total:
            raise ValueError("Invalid system byte counters")
        physical_used_percent = physical_memory_used_percent(system)
        members = snapshot["job_members"]
        qemu = snapshot["qemu"]
        def total(records):
            ids = set()
            result = 0
            for record in records:
                identity = (record["pid"], record["create_time_filetime"])
                if any(isinstance(x, bool) or not isinstance(x, int) or x <= 0 for x in identity):
                    raise ValueError("Invalid process creation identity")
                if record["pid"] in ids:
                    raise ValueError("Duplicate process identity")
                ids.add(record["pid"])
                value = record["private_bytes"]
                if isinstance(value, bool) or not isinstance(value, int) or value < 0:
                    raise ValueError("Invalid process PrivateUsage")
                handles = record.get("handles")
                if isinstance(handles, bool) or not isinstance(handles, int) or handles < 0:
                    raise ValueError("Missing process handle count")
                result += value
            return result
        owned = total(members)
        aggregate = total(qemu)
        if sorted(p["pid"] for p in members) != sorted(snapshot["job_process_ids"]):
            raise ValueError("Incomplete named-job process metrics")
        elapsed = snapshot["elapsed_seconds"]
        if not isinstance(elapsed, (float, int)) or not math.isfinite(elapsed) or elapsed < 0:
            raise ValueError("Invalid elapsed monotonic clock")
        headroom = limit - commit
        observations = {
            "owned_private_bytes": owned,
            "aggregate_qemu_private_bytes": aggregate,
            "commit_headroom_bytes": headroom,
            "physical_available_bytes": physical,
            "physical_total_bytes": physical_total,
            "physical_used_percent": physical_used_percent,
            "job_process_count": len(members),
            "qemu_process_count": len(qemu),
            "owned_handles": sum(p["handles"] for p in members),
            "elapsed_seconds": elapsed,
        }
        if owned >= limits.soft_job_bytes:
            reasons.append("owned_job_private_limit")
        if aggregate >= limits.aggregate_qemu_bytes:
            reasons.append("aggregate_qemu_private_limit")
        if headroom < limits.min_commit_headroom_bytes:
            reasons.append("system_commit_headroom")
        if physical < limits.min_physical_available_bytes and not limits.allow_physical_pressure:
            reasons.append("physical_available_floor")
        if physical_used_percent >= limits.max_physical_used_percent:
            reasons.append("physical_memory_used_percent")
        if elapsed >= limits.timeout_seconds:
            reasons.append("monitor_timeout")
        if not reasons and not members:
            return {"action": "complete", "reasons": [], "observations": observations}
    except (KeyError, TypeError, ValueError, OverflowError) as error:
        reasons.append("monitoring_unavailable")
        observations["invalid_snapshot"] = str(error)[:300]
    if snapshot.get("complete") is not True:
        # Failed collection cannot establish a zero-QEMU total. Preserve its
        # primary error alongside the secondary missing-job-metrics symptom.
        errors = snapshot.get("errors")
        if isinstance(errors, list) and errors:
            observations["monitoring_error"] = str(errors[0])[:500]
        observations["qemu_metrics_complete"] = False
        observations["qemu_process_count"] = None
    return {"action": "terminate" if reasons else "continue",
            "reasons": list(dict.fromkeys(reasons)), "observations": observations}


class ProcessGone(Exception):
    def __init__(self, pid, reason="unavailable_or_exited", **details):
        super().__init__(pid)
        self.pid, self.reason, self.details = pid, reason, details

    def diagnostic(self):
        return {"pid": self.pid, "reason": self.reason, **self.details}


class ProcessAccessDenied(OSError):
    """Persistent query denial, kept distinct for bounded census/job recheck."""
    def __init__(self, pid):
        super().__init__(5, f"Windows API failed: OpenProcess(query limited, pid={pid})")
        self.pid = pid


class WindowsAPI:
    """Read-only process/system APIs, plus named-job query/termination only."""
    def __init__(self):
        if os.name != "nt":
            raise RuntimeError("Windows memory monitoring unavailable on this platform")
        self.kernel = C.WinDLL("kernel32", use_last_error=True)
        self.psapi = C.WinDLL("psapi", use_last_error=True)
        size = C.c_size_t
        class PerformanceInfo(C.Structure):
            _fields_ = [("cb", W.DWORD)] + [(name, size) for name in (
                "CommitTotal", "CommitLimit", "CommitPeak", "PhysicalTotal",
                "PhysicalAvailable", "SystemCache", "KernelTotal", "KernelPaged",
                "KernelNonpaged", "PageSize")] + [(name, W.DWORD) for name in (
                "HandleCount", "ProcessCount", "ThreadCount")]
        class MemoryCounters(C.Structure):
            _fields_ = [("cb", W.DWORD), ("PageFaultCount", W.DWORD)] + [
                (name, size) for name in (
                    "PeakWorkingSetSize", "WorkingSetSize", "QuotaPeakPagedPoolUsage",
                    "QuotaPagedPoolUsage", "QuotaPeakNonPagedPoolUsage",
                    "QuotaNonPagedPoolUsage", "PagefileUsage", "PeakPagefileUsage",
                    "PrivateUsage")]
        class ProcessEntry(C.Structure):
            _fields_ = [("dwSize", W.DWORD), ("cntUsage", W.DWORD),
                ("th32ProcessID", W.DWORD), ("th32DefaultHeapID", size),
                ("th32ModuleID", W.DWORD), ("cntThreads", W.DWORD),
                ("th32ParentProcessID", W.DWORD), ("pcPriClassBase", W.LONG),
                ("dwFlags", W.DWORD), ("szExeFile", W.WCHAR * 260)]
        self.PerformanceInfo, self.MemoryCounters, self.ProcessEntry = PerformanceInfo, MemoryCounters, ProcessEntry
        def bind(library, name, arguments, result=W.BOOL):
            fn = getattr(library, name); fn.argtypes = arguments; fn.restype = result; return fn
        self.open_process = bind(self.kernel, "OpenProcess", [W.DWORD, W.BOOL, W.DWORD], W.HANDLE)
        self.close_handle = bind(self.kernel, "CloseHandle", [W.HANDLE])
        self.get_times = bind(self.kernel, "GetProcessTimes", [W.HANDLE] + [C.POINTER(W.FILETIME)] * 4)
        self.get_exit = bind(self.kernel, "GetExitCodeProcess", [W.HANDLE, C.POINTER(W.DWORD)])
        self.get_handles = bind(self.kernel, "GetProcessHandleCount", [W.HANDLE, C.POINTER(W.DWORD)])
        self.get_memory = bind(self.psapi, "GetProcessMemoryInfo", [W.HANDLE, C.POINTER(MemoryCounters), W.DWORD])
        self.get_image = bind(self.kernel, "QueryFullProcessImageNameW", [W.HANDLE, W.DWORD, W.LPWSTR, C.POINTER(W.DWORD)])
        self.get_performance = bind(self.psapi, "GetPerformanceInfo", [C.POINTER(PerformanceInfo), W.DWORD])
        self.toolhelp = bind(self.kernel, "CreateToolhelp32Snapshot", [W.DWORD, W.DWORD], W.HANDLE)
        self.process_first = bind(self.kernel, "Process32FirstW", [W.HANDLE, C.POINTER(ProcessEntry)])
        self.process_next = bind(self.kernel, "Process32NextW", [W.HANDLE, C.POINTER(ProcessEntry)])
        self.open_named_job = bind(self.kernel, "OpenJobObjectW", [W.DWORD, W.BOOL, W.LPCWSTR], W.HANDLE)
        self.query_job = bind(self.kernel, "QueryInformationJobObject", [W.HANDLE, C.c_int, W.LPVOID, W.DWORD, C.POINTER(W.DWORD)])
        self.is_in_job = bind(self.kernel, "IsProcessInJob", [W.HANDLE, W.HANDLE, C.POINTER(W.BOOL)])
        self.kill_job = bind(self.kernel, "TerminateJobObject", [W.HANDLE, W.UINT])
        # An AppContainer can see global memory but only its process namespace.
        # A zero-QEMU result there is not a complete host census. Refuse arming
        # rather than silently omitting another session's emulator processes.
        advapi = C.WinDLL("advapi32", use_last_error=True)
        current_process = bind(self.kernel, "GetCurrentProcess", [], W.HANDLE)
        open_token = bind(advapi, "OpenProcessToken", [W.HANDLE, W.DWORD, C.POINTER(W.HANDLE)])
        token_information = bind(advapi, "GetTokenInformation", [W.HANDLE, C.c_int, W.LPVOID, W.DWORD, C.POINTER(W.DWORD)])
        token = W.HANDLE()
        if not open_token(current_process(), 0x0008, C.byref(token)):
            raise self.error("OpenProcessToken(host census visibility)")
        try:
            appcontainer = W.DWORD(); returned = W.DWORD()
            if not token_information(token, 29, C.byref(appcontainer), C.sizeof(appcontainer), C.byref(returned)):
                raise self.error("GetTokenInformation(TokenIsAppContainer)")
            if appcontainer.value:
                raise RuntimeError("Host process census unavailable from AppContainer; run the guard outside the tool sandbox")
        finally:
            self.close(token)

    @staticmethod
    def error(operation):
        return OSError(C.get_last_error(), f"Windows API failed: {operation}")

    def close(self, handle):
        if handle:
            self.close_handle(handle)

    def system_snapshot(self):
        info = self.PerformanceInfo(); info.cb = C.sizeof(info)
        if not self.get_performance(C.byref(info), C.sizeof(info)):
            raise self.error("GetPerformanceInfo")
        if not info.PageSize:
            raise RuntimeError("System PageSize unavailable")
        return {"commit_total_bytes": int(info.CommitTotal * info.PageSize),
                "commit_limit_bytes": int(info.CommitLimit * info.PageSize),
                "physical_available_bytes": int(info.PhysicalAvailable * info.PageSize),
                "physical_total_bytes": int(info.PhysicalTotal * info.PageSize),
                "page_size_bytes": int(info.PageSize), "handles": int(info.HandleCount),
                "processes": int(info.ProcessCount)}

    def enumerate_processes(self):
        handle = self.toolhelp(0x00000002, 0)
        if not handle or handle == C.c_void_p(-1).value:
            raise self.error("CreateToolhelp32Snapshot")
        try:
            record = self.ProcessEntry(); record.dwSize = C.sizeof(record)
            if not self.process_first(handle, C.byref(record)):
                raise self.error("Process32FirstW")
            result = {}
            while True:
                result[int(record.th32ProcessID)] = {"pid": int(record.th32ProcessID),
                    "parent_pid": int(record.th32ParentProcessID), "image": record.szExeFile}
                if len(result) > 32768:
                    raise RuntimeError("Process enumeration exceeded bounded capacity")
                if not self.process_next(handle, C.byref(record)):
                    if C.get_last_error() != 18:  # ERROR_NO_MORE_FILES
                        raise self.error("Process32NextW")
                    return result
        finally:
            self.close(handle)

    def process_snapshot(self, pid, metadata=None):
        # Query metadata only; no memory writes, suspension, or VM inspection.
        handle = self.open_process_limited(pid)
        try:
            code = W.DWORD()
            if not self.get_exit(handle, C.byref(code)):
                raise self.error("GetExitCodeProcess")
            created, exited, kernel, user = (W.FILETIME() for _ in range(4))
            if not self.get_times(handle, C.byref(created), C.byref(exited), C.byref(kernel), C.byref(user)):
                raise self.error("GetProcessTimes")
            created_time = (int(created.dwHighDateTime) << 32) | int(created.dwLowDateTime)
            if code.value != 259:
                # Preserve what the same still-open query HANDLE actually
                # reported. This diagnostic is not permission to omit a listed
                # member: the final job query must still exclude its PID.
                raise ProcessGone(pid, "GetExitCodeProcess_exited", exit_code=int(code.value),
                    create_time_filetime=created_time,
                    exit_time_filetime=(int(exited.dwHighDateTime) << 32) | int(exited.dwLowDateTime))
            memory = self.MemoryCounters(); memory.cb = C.sizeof(memory)
            if not self.get_memory(handle, C.byref(memory), C.sizeof(memory)):
                raise self.error("GetProcessMemoryInfo(PrivateUsage)")
            handles = W.DWORD()
            if not self.get_handles(handle, C.byref(handles)):
                raise self.error("GetProcessHandleCount")
            name = C.create_unicode_buffer(32768); name_size = W.DWORD(len(name))
            if not self.get_image(handle, 0, name, C.byref(name_size)):
                raise self.error("QueryFullProcessImageNameW")
            result = dict(metadata or {"pid": pid, "parent_pid": None, "image": ""})
            result.update(pid=pid, image=Path(name.value.replace("\\", "/")).name,
                create_time_filetime=created_time,
                private_bytes=int(memory.PrivateUsage), working_set_bytes=int(memory.WorkingSetSize),
                handles=int(handles.value), alive=True)
            return result
        finally:
            self.close(handle)

    def open_process_limited(self, pid):
        """Open for query, retrying one transient access-denied race only.

        Access denied is never treated as evidence that a process exited. A
        second denial raises an explicit unresolved error; only the caller's
        fresh census/job reconciliation can establish departure. The watchdog
        fails closed when it cannot account for a live job/QEMU process.
        """
        for attempt in range(2):
            handle = self.open_process(0x1000, False, pid)
            if handle:
                return handle
            code = C.get_last_error()
            if code == 87:
                # This is NOT creation/exit proof. Only a later authoritative
                # job query excluding this PID can reconcile its missing data.
                raise ProcessGone(pid, "OpenProcess_ERROR_INVALID_PARAMETER")
            if code == 5 and attempt == 0:
                # A process can exit and its PID be recycled between the
                # Toolhelp census and OpenProcess. Give that transition one
                # short interval to settle; persistent denial remains fatal.
                time.sleep(0.025)
                continue
            if code == 5:
                raise ProcessAccessDenied(pid)
            raise self.error(f"OpenProcess(query limited, pid={pid})")
        raise self.error(f"OpenProcess(query limited, pid={pid})")

    def open_job(self, name):
        handle = self.open_named_job(JOB_OBJECT_QUERY | JOB_OBJECT_TERMINATE, False, name)
        if not handle:
            raise self.error("OpenJobObjectW(named query/terminate)")
        return handle

    def target_in_job(self, pid, handle, expected_create_time=None):
        process = self.open_process(0x1000, False, pid)
        if not process:
            raise self.error("OpenProcess(target membership)")
        try:
            if expected_create_time is not None:
                created, exited, kernel, user = (W.FILETIME() for _ in range(4))
                if not self.get_times(process, C.byref(created), C.byref(exited), C.byref(kernel), C.byref(user)):
                    raise self.error("GetProcessTimes(membership identity)")
                if ((int(created.dwHighDateTime) << 32) | int(created.dwLowDateTime)) != expected_create_time:
                    raise RuntimeError("Process PID reused during membership verification")
            answer = W.BOOL()
            if not self.is_in_job(process, handle, C.byref(answer)):
                raise self.error("IsProcessInJob")
            return bool(answer.value)
        finally:
            self.close(process)

    def job_process_ids(self, handle):
        # BasicProcessIdList is 3. ExtendedLimitInformation is 9, never decoded
        # as a PID list. ULONG_PTR entries follow the two DWORD counts.
        capacity = 32
        while capacity <= MAX_JOB_PROCESSES:
            data = C.create_string_buffer(8 + capacity * C.sizeof(C.c_size_t))
            assigned = W.DWORD.from_buffer(data, 0)
            present = W.DWORD.from_buffer(data, 4)
            ok = self.query_job(handle, JOB_OBJECT_BASIC_PROCESS_ID_LIST,
                                data, C.sizeof(data), None)
            if assigned.value > MAX_JOB_PROCESSES:
                raise RuntimeError("Named job exceeds bounded 4096-process capacity")
            if ok and present.value == assigned.value and present.value <= capacity:
                array = (C.c_size_t * present.value).from_buffer(data, 8)
                return [int(pid) for pid in array]
            if not ok and C.get_last_error() != 234:
                raise self.error("QueryInformationJobObject(BasicProcessIdList)")
            capacity = max(capacity * 2, int(assigned.value))
        raise RuntimeError("Named job PID list unavailable within bounded capacity")

    def terminate_job(self, handle):
        if not self.kill_job(handle, 0xD236):
            raise self.error("TerminateJobObject(verified owned job)")


def read_manifest(path: Path):
    raw = manifest_bytes(path)
    if len(raw) > 65536:
        raise ValueError("Guard manifest exceeds 64 KiB")
    value = json.loads(raw)
    name = value["job_name"]
    prefix = "Local\\DH2-Emulator-"
    if not isinstance(name, str) or not name.startswith(prefix):
        raise ValueError("Job name is outside fresh DH2 guard namespace")
    suffix = name[len(prefix):]
    if suffix.startswith("{") and suffix.endswith("}"):
        suffix = suffix[1:-1]
    if str(uuid.UUID(suffix)) != suffix.lower():
        raise ValueError("Job name requires canonical UUID")
    for key in ("launcher_pid", "launcher_create_time", "target_pid", "target_create_time"):
        if isinstance(value[key], bool) or not isinstance(value[key], int) or value[key] <= 0:
            raise ValueError(f"Invalid manifest identity: {key}")
    if value["launcher_pid"] == value["target_pid"]:
        raise ValueError("Guard launcher must be outside the target job")
    return value, hashlib.sha256(raw).hexdigest()


def manifest_bytes(path: Path):
    with path.open("rb") as stream:
        value = stream.read(65537)
    if len(value) > 65536:
        raise ValueError("Guard manifest exceeds 64 KiB")
    return value


def system_qemu_snapshot(api=None):
    """Reusable preflight: system counters and every QEMU/emulator private usage.

    No job opens and no termination. A missing active process metric is an error,
    while an already-exited enumerated process is omitted explicitly.
    """
    api = api or WindowsAPI()
    processes = api.enumerate_processes()
    result = {"system": api.system_snapshot(), "qemu": [], "gone_pids": []}
    for pid, metadata in processes.items():
        if is_emulator_image(metadata["image"]):
            try:
                record = api.process_snapshot(pid, metadata)
                if is_emulator_image(record["image"]):
                    result["qemu"].append(record)
            except ProcessGone:
                result["gone_pids"].append(pid)
    result["aggregate_qemu_private_bytes"] = sum(p["private_bytes"] for p in result["qemu"])
    return result


def process_snapshot_reconciled(api, pid, metadata, job_handle, reconciliation=None):
    """Reconcile persistent access denial only with fresh census and job proof.

    Access denied never implies exit by itself. A PID is treated as departed
    only when both a new Toolhelp census and a new authoritative named-job
    query exclude it. An unlisted same-image/parent emulator gets at most two
    short census yields, sharing one deadline for the entire snapshot. This
    never substitutes zero/stale metrics for a process still present anywhere.
    Any failed or conflicting observation remains fatal.
    """
    try:
        return api.process_snapshot(pid, metadata)
    except ProcessAccessDenied as error:
        state = reconciliation if reconciliation is not None else {
            "deadline": None, "diagnostics": []}
        original = dict(metadata or {})
        diagnostic = {"pid": pid, "previous_census": original, "passes": [],
                      "wait_seconds": 0.0, "outcome": "unresolved"}
        if len(state["diagnostics"]) < MEMBERSHIP_DIAGNOSTIC_PID_LIMIT:
            state["diagnostics"].append(diagnostic)
        for attempt in range(3):
            try:
                fresh_processes = api.enumerate_processes()
                fresh_members = set(api.job_process_ids(job_handle))
            except Exception as verify_error:
                diagnostic["reconciliation_error"] = f"{type(verify_error).__name__}: {verify_error}"[:300]
                raise RuntimeError(
                    f"Unresolved access denied for pid={pid}; fresh reconciliation failed: "
                    f"{type(verify_error).__name__}: {verify_error}"
                ) from error
            census_present = pid in fresh_processes
            job_member = pid in fresh_members
            fresh = fresh_processes.get(pid)
            diagnostic["passes"].append({"pass": attempt + 1,
                "census_present": census_present, "job_member": job_member,
                "fresh_census": dict(fresh) if fresh is not None else None,
                "job_process_count": len(fresh_members),
                "job_process_ids": sorted(fresh_members)[:MEMBERSHIP_DIAGNOSTIC_PID_LIMIT]})
            if not census_present and not job_member:
                diagnostic["outcome"] = "absent_from_fresh_census_and_named_job"
                raise ProcessGone(
                    pid,
                    "AccessDenied_absent_from_fresh_census_and_named_job",
                    census_present=False, job_member=False,
                    previous_image=original.get("image"),
                    reconciliation_passes=attempt + 1,
                    reconciliation_wait_seconds=diagnostic["wait_seconds"],
                ) from error
            fresh_image = fresh.get("image") if fresh is not None else None
            # Parent/image metadata is NOT creation identity or ownership proof.
            # It only narrows eligibility to wait; absence from BOTH authorities
            # remains the sole condition that permits omission of this PID.
            matching_candidate = (not job_member and fresh is not None and
                isinstance(original.get("image"), str) and
                is_emulator_image(original["image"]) and
                fresh_image == original["image"] and
                isinstance(original.get("parent_pid"), int) and
                fresh.get("parent_pid") == original["parent_pid"])
            if not matching_candidate or attempt == 2:
                break
            now = time.monotonic()
            if state["deadline"] is None:
                state["deadline"] = now + ACCESS_DENIED_CENSUS_WAIT_BUDGET_SECONDS
            wait = min(ACCESS_DENIED_CENSUS_DELAY_SECONDS,
                       max(0.0, state["deadline"] - now))
            if wait <= 0:
                diagnostic["wait_budget_exhausted"] = True
                break
            started_wait = time.monotonic()
            time.sleep(wait)
            diagnostic["wait_seconds"] += max(0.0, time.monotonic() - started_wait)
        raise RuntimeError(
            f"Unresolved access denied for pid={pid}; census_present={census_present}; "
            f"job_member={job_member}; fresh_image={fresh_image!r}; "
            f"reconciliation_passes={len(diagnostic['passes'])}"
        ) from error


def collect_snapshot(api, manifest, job_handle, start, manifest_path, manifest_digest):
    value = {"wall_time_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        "elapsed_seconds": time.monotonic() - start,
        "expected_owner": {"pid": manifest["launcher_pid"], "create_time_filetime": manifest["launcher_create_time"]},
        "complete": True, "errors": [], "job_members": [], "qemu": [], "owned_process_tree": [],
        "job_process_ids": [], "manifest_unchanged": False,
        "job_membership_passes": [], "access_denied_reconciliations": [],
        "qemu_metrics_complete": False, "qemu_census_count": None,
        "qemu_census_candidates": []}
    access_denied_state = {"deadline": None,
                          "diagnostics": value["access_denied_reconciliations"]}
    try:
        value["manifest_unchanged"] = hashlib.sha256(manifest_bytes(manifest_path)).hexdigest() == manifest_digest
        value["system"] = api.system_snapshot()
        processes = api.enumerate_processes()
        qemu_candidates = {pid: p for pid, p in processes.items() if is_emulator_image(p["image"])}
        value["qemu_census_count"] = len(qemu_candidates)
        value["qemu_census_candidates"] = [qemu_candidates[pid]
            for pid in sorted(qemu_candidates)[:MEMBERSHIP_DIAGNOSTIC_PID_LIMIT]]
        # Reuse the existing census: bounded image counts identify a host-wide
        # process surge without another process, memory probe or command line.
        image_counts = {}
        for process in processes.values():
            image = process["image"].casefold()
            image_counts[image] = image_counts.get(image, 0) + 1
        value["host_process_images"] = [
            {"image": image, "count": count}
            for image, count in sorted(image_counts.items(), key=lambda item: (-item[1], item[0]))[:16]
        ]
        value["host_process_census_count"] = len(processes)
        members = api.job_process_ids(job_handle)
        value["job_process_ids"] = members
        selected = set(members) | {manifest["launcher_pid"]}
        selected.update(qemu_candidates)
        # Snapshot launcher descendants, with creation ordering checked below.
        tree = {manifest["launcher_pid"]}
        for _ in range(64):
            children = {pid for pid, p in processes.items() if p["parent_pid"] in tree}
            if children <= tree:
                break
            tree.update(children)
        else:
            raise RuntimeError("Process tree exceeds bounded 64-level depth")
        selected.update(tree)
        metrics = {}
        gone = set()
        # Publish owner telemetry before any child can fail. A child sampling
        # error must not fabricate a missing/exited launcher diagnosis.
        owner_pid = manifest["launcher_pid"]
        for pid in [owner_pid, *sorted(selected - {owner_pid})]:
            try:
                metrics[pid] = process_snapshot_reconciled(
                    api, pid, processes.get(pid), job_handle, access_denied_state
                )
                if pid == owner_pid:
                    value["owner"] = metrics[pid]
                if is_emulator_image(metrics[pid]["image"]):
                    # These are current partial observations, never a complete
                    # aggregate until the entire collection succeeds below.
                    value["qemu"].append(metrics[pid])
            except ProcessGone:
                gone.add(pid)
        value["qemu"] = [p for p in metrics.values() if is_emulator_image(p["image"])]
        value["job_members"] = [metrics[pid] for pid in members if pid in metrics]
        # Fresh actual membership wins over parent-PID guesses. An exited child
        # may still appear in one JobObject PID query. Reconcile at most three
        # sampling passes. A refreshed subset is safe when EVERY final member
        # has identity-valid metrics from THIS pass; departing sampled members
        # do not require list equality. A new unmeasured member requires another
        # pass. Never suppress access failures, PID reuse or missing live metrics.
        final_members = api.job_process_ids(job_handle)
        seen_members = set(members) | set(final_members)
        value["job_membership_requeries"] = 0
        value["job_exit_drain_wait_seconds"] = 0.0
        drain_deadline = None
        for attempt in range(3):
            cap = MEMBERSHIP_DIAGNOSTIC_PID_LIMIT
            queried = set(final_members)
            diagnostic = {"pass": attempt + 1, "queried_count": len(queried),
                "queried_pids": sorted(queried)[:cap], "sampled_count": 0,
                "sampled_identities": [], "process_gone_count": 0,
                "process_gone_pids": [], "process_gone_diagnostics": [], "accepted": False,
                "pid_list_limit": cap}
            value["job_membership_passes"].append(diagnostic)
            value["job_process_ids"] = final_members
            value["job_members"] = [metrics[pid] for pid in final_members if pid in metrics]
            sampled = {}
            for pid in final_members:
                try:
                    current = process_snapshot_reconciled(
                        api, pid, processes.get(pid), job_handle, access_denied_state
                    )
                except ProcessGone as error:
                    previous = metrics.get(pid)
                    gone_creation = error.details.get("create_time_filetime")
                    if previous and gone_creation is not None and gone_creation != previous["create_time_filetime"]:
                        diagnostic["identity_error"] = {"pid": pid,
                            "previous_create_time": previous["create_time_filetime"],
                            "exited_create_time": gone_creation}
                        raise RuntimeError("Process PID reused before exited metric reconciliation")
                    gone.add(pid)
                    diagnostic["process_gone_count"] += 1
                    if len(diagnostic["process_gone_pids"]) < cap:
                        diagnostic["process_gone_pids"].append(pid)
                        diagnostic["process_gone_diagnostics"].append(error.diagnostic())
                    continue
                except Exception as error:
                    diagnostic["metric_error"] = {"pid": pid,
                        "error": f"{type(error).__name__}: {error}"[:300]}
                    raise
                created = current.get("create_time_filetime")
                if (current.get("pid") != pid or current.get("alive") is not True or
                        isinstance(created, bool) or not isinstance(created, int) or created <= 0):
                    diagnostic["identity_error"] = {"pid": pid, "returned_pid": current.get("pid"),
                        "returned_create_time": created}
                    raise RuntimeError("Job metric returned invalid live process identity")
                if pid in gone:
                    diagnostic["identity_error"] = {"pid": pid, "returned_after_ProcessGone": True}
                    raise RuntimeError("Exited process PID returned live during job reconciliation")
                previous = metrics.get(pid)
                if previous and current["create_time_filetime"] != previous["create_time_filetime"]:
                    diagnostic["identity_error"] = {"pid": pid,
                        "previous_create_time": previous["create_time_filetime"],
                        "current_create_time": current["create_time_filetime"]}
                    raise RuntimeError("Process PID reused during job metric reconciliation")
                sampled[pid] = current
                metrics[pid] = current
                diagnostic["sampled_count"] += 1
                if len(diagnostic["sampled_identities"]) < cap:
                    diagnostic["sampled_identities"].append({"pid": pid, "create_time_filetime": created})
            refreshed = api.job_process_ids(job_handle)
            seen_members.update(refreshed)
            value["job_membership_requeries"] += 1
            final_set = set(refreshed)
            missing = final_set - sampled.keys()
            diagnostic.update(refreshed_count=len(final_set), refreshed_pids=sorted(final_set)[:cap],
                departed_count=len(queried-final_set), departed_pids=sorted(queried-final_set)[:cap],
                new_count=len(final_set-queried), new_pids=sorted(final_set-queried)[:cap],
                missing_current_count=len(missing), missing_current_pids=sorted(missing)[:cap],
                accepted=not missing)
            if not missing:
                final_members = refreshed
                break
            final_members = refreshed
            if attempt < 2 and missing <= gone:
                # Job PID notifications can lag a process exit. Busy immediate
                # retries cannot give that kernel cleanup a chance to finish.
                # Yield ONLY for missing PIDs observed as ProcessGone; retain
                # all three passes, final membership checks and fail-closed
                # policy. New unsampled arrivals never qualify for this drain.
                now = time.monotonic()
                if drain_deadline is None:
                    drain_deadline = now + EXIT_DRAIN_WAIT_BUDGET_SECONDS
                wait = min(EXIT_DRAIN_DELAY_SECONDS, max(0.0, drain_deadline-now))
                if wait > 0:
                    diagnostic["exit_drain_wait_requested_seconds"] = wait
                    started_wait = time.monotonic()
                    time.sleep(wait)
                    observed_wait = max(0.0, time.monotonic()-started_wait)
                    diagnostic["exit_drain_wait_observed_seconds"] = observed_wait
                    value["job_exit_drain_wait_seconds"] += observed_wait
        else:
            value["job_process_ids"] = final_members
            value["job_members"] = [metrics[pid] for pid in final_members if pid in metrics and pid not in gone]
            raise RuntimeError("Named-job membership/metrics did not stabilize within three passes")
        value["job_process_ids"] = final_members
        # Publish current-pass records only, never older metrics merely because
        # an integer PID happened to appear in a previous census/pass.
        value["job_members"] = [sampled[pid] for pid in final_members]
        departed_members = seen_members - set(final_members)
        value["qemu"] = [p for pid, p in metrics.items()
                         if pid not in gone and pid not in departed_members and is_emulator_image(p["image"])]
        value["job_reconciled_exited_pids"] = sorted((gone & seen_members) - set(final_members))
        value["job_reconciled_departed_pids"] = sorted(departed_members)
        # A parent PID alone is not lineage: exclude impossible creation order.
        admitted = {manifest["launcher_pid"]}
        for _ in range(64):
            changed = False
            for pid in tree - admitted:
                child = metrics.get(pid) if pid not in gone and pid not in departed_members else None
                if not child:
                    continue
                parent = metrics.get(child["parent_pid"])
                if parent and parent["pid"] in admitted and child["create_time_filetime"] >= parent["create_time_filetime"]:
                    admitted.add(pid); changed = True
            if not changed:
                break
        value["owned_process_tree"] = [metrics[pid] for pid in sorted(admitted) if pid in metrics]
        value["gone_enumerated_pids"] = sorted(gone)
        value["qemu_metrics_complete"] = True
    except Exception as error:
        value["complete"] = False
        value["errors"].append(f"{type(error).__name__}: {error}"[:500])
    return value


def atomic_json(path: Path, value: dict):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + f".{os.getpid()}.tmp")
    with temporary.open("w", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2); stream.write("\n"); stream.flush(); os.fsync(stream.fileno())
    os.replace(temporary, path)


class BoundedTelemetry:
    def __init__(self, path: Path, max_bytes=8*MIB):
        if max_bytes < 65536 or max_bytes > 64*MIB:
            raise ValueError("Telemetry bound must be between 64 KiB and 64 MiB")
        self.path, self.max_bytes = path, max_bytes
        path.parent.mkdir(parents=True, exist_ok=True)

    def append(self, value):
        data = (json.dumps(value, separators=(",", ":")) + "\n").encode("utf-8")
        if len(data) > self.max_bytes:
            # Preserve the decision and counts when enormous tree telemetry
            # cannot fit one bounded record. Full receipt still has diagnostics.
            data = (json.dumps({"telemetry_truncated": True,
                "decision": value.get("decision"), "elapsed_seconds": value.get("elapsed_seconds")}) + "\n").encode()
        current = self.path.stat().st_size if self.path.exists() else 0
        if current + len(data) > self.max_bytes:
            os.replace(self.path, self.path.with_name(self.path.name + ".1"))
        with self.path.open("ab") as stream:
            stream.write(data); stream.flush()


def monitor(manifest_path, telemetry_path, receipt_path, ready_path, limits,
            poll_seconds=2.0, telemetry_bytes=8*MIB, api=None):
    """Own a pinned, verified named-job handle; injectable for fixture tests."""
    limits.validate()
    if not math.isfinite(poll_seconds) or not 0.05 <= poll_seconds <= 10:
        raise ValueError("Poll interval must be between 0.05 and 10 seconds")
    manifest_path, telemetry_path, receipt_path, ready_path = map(Path,
        (manifest_path, telemetry_path, receipt_path, ready_path))
    if len({str(p.resolve()).casefold() for p in (manifest_path, telemetry_path, receipt_path, ready_path)}) != 4:
        raise ValueError("Manifest, telemetry, receipt and ready paths must be distinct")
    handle = None; verified = False; armed = False; latest = None
    result = {"schema": "dh2-emulator-watchdog-v36", "status": "bootstrap_failed",
        "verified": False, "limits": asdict(limits), "termination_attempted": False,
        "termination_succeeded": False, "watchdog_pid": os.getpid()}
    start = time.monotonic()
    try:
        manifest, digest = read_manifest(manifest_path)
        result.update(job_name=manifest["job_name"], manifest_sha256=digest,
                      launcher_pid=manifest["launcher_pid"], target_pid=manifest["target_pid"])
        api = api or WindowsAPI()
        handle = api.open_job(manifest["job_name"])
        owner = api.process_snapshot(manifest["launcher_pid"])
        target = api.process_snapshot(manifest["target_pid"])
        if owner["create_time_filetime"] != manifest["launcher_create_time"]:
            raise RuntimeError("Launcher creation identity mismatch before verification")
        if target["create_time_filetime"] != manifest["target_create_time"]:
            raise RuntimeError("Target creation identity mismatch before verification")
        if not api.target_in_job(manifest["target_pid"], handle, manifest["target_create_time"]):
            raise RuntimeError("Manifest target is not in supplied named job")
        if api.target_in_job(manifest["launcher_pid"], handle, manifest["launcher_create_time"]):
            raise RuntimeError("Guard launcher is unexpectedly inside target job")
        verified = True; result["verified"] = True
        telemetry = BoundedTelemetry(telemetry_path, telemetry_bytes)
        while True:
            latest = collect_snapshot(api, manifest, handle, start, manifest_path, digest)
            decision = evaluate(latest, limits); latest["decision"] = decision
            telemetry.append(latest)
            if decision["action"] == "terminate":
                result["status"] = "terminated_by_guard"
                break
            if decision["action"] == "complete":
                result["status"] = "job_completed"
                return_code = 0
                break
            if not armed:
                # Require a complete first monitored snapshot while target is
                # suspended. Missing startup target membership never arms.
                if manifest["target_pid"] not in latest["job_process_ids"]:
                    raise RuntimeError("Suspended target disappeared before watchdog readiness")
                ready = {"verified": True, "job_name": manifest["job_name"],
                    "launcher_pid": manifest["launcher_pid"], "launcher_create_time": manifest["launcher_create_time"],
                    "target_pid": manifest["target_pid"], "target_create_time": manifest["target_create_time"],
                    "watchdog_pid": os.getpid(), "manifest_sha256": digest}
                atomic_json(receipt_path, dict(result, status="armed", first_snapshot=latest))
                atomic_json(ready_path, ready)
                armed = True
            # Full telemetry includes a process-tree/member census and stays
            # at the configured cadence. Check the cheap host RAM counters
            # between those samples so a short spike cannot hide for a whole
            # census interval before the unchanged hard percentage cutoff.
            poll_deadline = time.monotonic() + poll_seconds
            while True:
                remaining = poll_deadline - time.monotonic()
                if remaining <= 0:
                    break
                time.sleep(min(FAST_PHYSICAL_MEMORY_POLL_SECONDS, remaining))
                fast_system = api.system_snapshot()
                fast_used = physical_memory_used_percent(fast_system)
                if fast_used >= limits.max_physical_used_percent:
                    fast_observation = {
                        "physical_available_bytes": fast_system["physical_available_bytes"],
                        "physical_total_bytes": fast_system["physical_total_bytes"],
                        "physical_used_percent": fast_used,
                        "max_physical_used_percent": limits.max_physical_used_percent,
                    }
                    latest["fast_physical_memory_stop"] = {
                        "wall_time_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
                        "elapsed_seconds": time.monotonic() - start,
                        "observations": fast_observation,
                    }
                    latest["decision"] = {"action": "terminate",
                        "reasons": ["physical_memory_used_percent"],
                        "observations": fast_observation}
                    telemetry.append(latest)
                    result["status"] = "terminated_by_guard"
                    break
            if result["status"] == "terminated_by_guard":
                break
    except BaseException as error:
        result["status"] = "monitoring_failed" if verified else "bootstrap_failed"
        result["error"] = f"{type(error).__name__}: {error}"[:700]
    finally:
        if verified and result["status"] != "job_completed":
            result["termination_attempted"] = True
            try:
                api.terminate_job(handle)
                result["termination_succeeded"] = True
            except Exception as error:
                result["termination_error"] = f"{type(error).__name__}: {error}"[:500]
        result["armed"] = armed
        result["elapsed_seconds"] = time.monotonic() - start
        result["last_snapshot"] = latest
        if handle:
            api.close(handle)
        # Failure to emit a receipt must not delay termination above.
        try:
            atomic_json(receipt_path, result)
        except Exception as error:
            result["receipt_error"] = str(error)[:300]
        print(json.dumps({key: result.get(key) for key in (
            "status", "verified", "armed", "termination_attempted", "termination_succeeded", "error", "receipt_error")}))
    return 0 if result["status"] == "job_completed" else 2 if result["termination_succeeded"] else 3


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--telemetry", type=Path, required=True)
    parser.add_argument("--receipt", type=Path, required=True)
    parser.add_argument("--ready", type=Path, required=True)
    parser.add_argument("--poll-seconds", type=float, default=2.0)
    parser.add_argument("--soft-job-gib", type=float, default=5.0)
    parser.add_argument("--aggregate-qemu-gib", type=float, default=12.0)
    parser.add_argument("--min-commit-headroom-gib", type=float, default=16.0)
    parser.add_argument("--min-physical-available-gib", type=float, default=6.0)
    parser.add_argument("--timeout-seconds", type=float, default=1200.0)
    parser.add_argument("--allow-physical-pressure", action="store_true")
    parser.add_argument("--max-physical-used-percent", type=int, default=98)
    parser.add_argument("--telemetry-limit-bytes", type=int, default=8*MIB)
    args = parser.parse_args()
    amounts = (args.soft_job_gib, args.aggregate_qemu_gib,
               args.min_commit_headroom_gib, args.min_physical_available_gib)
    if any(not math.isfinite(x) or x <= 0 for x in amounts):
        parser.error("GiB thresholds must be finite and positive")
    limits = Limits(*(int(x*GIB) for x in amounts), args.timeout_seconds,
                     args.allow_physical_pressure, args.max_physical_used_percent)
    return monitor(args.manifest, args.telemetry, args.receipt, args.ready, limits,
                   args.poll_seconds, args.telemetry_limit_bytes)


if __name__ == "__main__":
    raise SystemExit(main())
