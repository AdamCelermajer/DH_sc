#!/usr/bin/env python3
"""Install and exercise the local Irrlicht SWAMP NativeActivity on API 37."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from PIL import Image


PACKAGE = "local.dh2.sourceviewer.irrlichtswamp"
ACTIVITY = f"{PACKAGE}/android.app.NativeActivity"
TAG = "DH2IrrlichtSwamp"
LOG_SINCE = ""
FATAL_MARKERS = (
    "GL_INVALID_OPERATION", "GL_INVALID_ENUM", "Fatal signal",
    "FATAL EXCEPTION", "Tried to set a texture not owned by this driver",
    "Failed to create Irrlicht device",
    "Source Character",
    "SWAMP source Actor session stopped",
)
ASSEMBLY = re.compile(
    r"SWAMP module 0 assembled in Irrlicht r6038: .*?source_draws=(\d+) "
    r"visible_diagnostic_draws=(\d+) omitted_unresolved=(\d+).*?"
    r"vertices=(\d+) indices=(\d+) path_mask=0x([0-9A-Fa-f]+) "
    r"start=\((-?[0-9.]+),(-?[0-9.]+),(-?[0-9.]+)\) "
    r"diffuse_refs=(\d+) diffuse_draws=(\d+) "
    r"AlphaMap_refs=(\d+) AlphaMap_unresolved_refs=(\d+) "
    r"AlphaMap_(cutout|AL_preview|AL)_draws=(\d+)"
)
ROOM_ASSEMBLY = re.compile(
    r"SWAMP module 0 assembled in Irrlicht r6038: .*?source_draws=(\d+) "
    r"visible_scenery_draws=(\d+) room_root_bounds_filtered=(\d+) "
    r"navigation_floor_draws_filtered=(\d+) exit_marker_draws_filtered=(\d+).*?"
    r"vertices=(\d+) indices=(\d+) path_mask=0x([0-9A-Fa-f]+) "
    r"start=\((-?[0-9.]+),(-?[0-9.]+),(-?[0-9.]+)\) "
    r"diffuse_refs=(\d+) diffuse_draws=(\d+) "
    r"AlphaMap_refs=(\d+) AlphaMap_unresolved_refs=(\d+) "
    r"AlphaMap_(cutout|AL_preview|AL)_draws=(\d+)"
)
MOVE = re.compile(
    r"MOVE state=(IDLE|MOVE|BLOCKED|REJECTED) "
    r"(?:animation=(IDLE|WALK) )?"
    r"(?:source_state=(-?\d+) sequence=(-?\d+) clip=(-?\d+) )?"
    r"x=(-?[0-9.]+) y=(-?[0-9.]+) z=(-?[0-9.]+) "
    r"stick_x=(-?[0-9.]+) stick_y=(-?[0-9.]+) path_mask=0x([0-9A-Fa-f]+)"
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def adb(adb_path: Path, serial: str, *args: str) -> str:
    result = subprocess.run([str(adb_path), "-s", serial, *args], text=True,
                            encoding="utf-8", errors="replace",
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            check=False)
    if result.returncode:
        raise RuntimeError(f"adb command failed ({result.returncode}): {args!r}\n{result.stdout}")
    return result.stdout.strip()


def install(adb_path: Path, serial: str, apk: Path) -> None:
    result = subprocess.run([str(adb_path), "-s", serial, "install", "-r", str(apk)],
                            text=True, encoding="utf-8", errors="replace",
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    if result.returncode == 0:
        return
    if "INSTALL_FAILED_UPDATE_INCOMPATIBLE" not in result.stdout:
        raise RuntimeError(f"APK installation failed: {result.stdout}")
    # The diagnostic package has its own identity; replace only a prior local
    # build of that exact package if a different debug key was used.
    adb(adb_path, serial, "uninstall", PACKAGE)
    adb(adb_path, serial, "install", "-r", str(apk))


def screen(adb_path: Path, serial: str, output: Path, label: str) -> dict:
    remote = f"/sdcard/dh2-swamp-irrlicht-{label}.png"
    adb(adb_path, serial, "shell", "screencap", "-p", remote)
    target = output / f"{label}.png"
    adb(adb_path, serial, "pull", remote, str(target))
    with Image.open(target) as source:
        image = source.convert("RGB")
        width, height = image.size
        reference = image.getpixel((width - 8, height // 2))
        region = image.crop((width // 4, height // 8, width * 3 // 4, height * 7 // 8))
        changed = sum(
            sum(abs(a - b) for a, b in zip(pixel, reference)) > 30
            for pixel in region.getdata()
        )
        pixels = list(region.getdata())
        green_texture_pixels = sum(
            1 for r, g, b in pixels
            if g > 45 and g > r * 1.22 and g > b * 1.12
        )
        brown_texture_pixels = sum(
            1 for r, g, b in pixels
            if r > 50 and r > g * 1.18 and g > b * 1.12
        )
    return {"path": str(target), "bytes": target.stat().st_size,
            "sha256": sha256(target), "width": width, "height": height,
            "changed_pixels": changed,
            "swamp_green_pixels": green_texture_pixels,
            "timber_brown_pixels": brown_texture_pixels}


def move_records(log: str) -> list[dict]:
    rows = []
    for line in log.splitlines():
        if TAG not in line:
            continue
        match = MOVE.search(line)
        if not match:
            continue
        state, animation, source_state, sequence, clip, x, y, z, stick_x, stick_y, path_mask = match.groups()
        rows.append({"state": state, "animation": animation,
                     "source_state": int(source_state) if source_state is not None else None,
                     "sequence": int(sequence) if sequence is not None else None,
                     "clip": int(clip) if clip is not None else None,
                     "x": float(x), "y": float(y),
                     "z": float(z), "stick_x": float(stick_x),
                     "stick_y": float(stick_y), "path_mask": int(path_mask, 16)})
        detail = dict(re.findall(r"\b([a-z_]+)=(-?(?:0x[0-9A-Fa-f]+|[0-9]+(?:\.[0-9]+)?))", line))
        integer_fields = ('actor_step', 'world_steps', 'actor_phase', 'body_present',
                          'body_pinned', 'body_service_count', 'floor',
                          'path_boundary_checked', 'path_direction_valid',
                          'source_path_segments', 'source_path_requested')
        float_fields = ('physics_x', 'physics_y', 'desired_x', 'desired_y',
                        'validated_x', 'validated_y', 'owner_delta_x', 'owner_delta_y',
                        'animation_root_delta_x', 'animation_root_delta_y',
                        'desired_rotation', 'current_rotation', 'body_radius',
                        'body_radius_physics')
        for name in integer_fields:
            if name in detail: rows[-1][name] = int(detail[name])
        for name in float_fields:
            if name in detail: rows[-1][name] = float(detail[name])
        if 'source_state_flags' in detail:
            rows[-1]['source_state_flags'] = int(detail['source_state_flags'], 16)
    return rows


def capture_log(adb_path: Path, serial: str, output: Path) -> str:
    pid = adb(adb_path, serial, "shell", "pidof", PACKAGE).split()[0]
    log = app_log(adb_path, serial, pid)
    target = output / "logcat.txt"
    target.write_text(log + "\n", encoding="utf-8")
    return log


def app_log(adb_path: Path, serial: str, pid: str) -> str:
    boundary = ("-T", LOG_SINCE) if LOG_SINCE else ()
    return adb(adb_path, serial, "logcat", "-d", *boundary, "-v", "time", "--pid=" + pid)


def wait_for_assembly(adb_path: Path, serial: str,
                      timeout: float, require_character: bool=False) -> tuple[str, str]:
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        pid_output = adb(adb_path, serial, "shell", "pidof", PACKAGE)
        if not pid_output:
            raise RuntimeError("SWAMP NativeActivity exited before assembling module zero")
        pid = pid_output.split()[0]
        log = app_log(adb_path, serial, pid)
        if (ASSEMBLY.search(log) or ROOM_ASSEMBLY.search(log)) and (not require_character or
                ("PRINCE:" in log and "FIRST_SOURCE_CHARACTER_FRAME:" in log)):
            return pid, log
        time.sleep(0.25)
    raise RuntimeError("SWAMP/Character first-frame readiness did not finish; inspect the saved device log and screen")


def wait_for_mode(adb_path: Path, serial: str, state: str,
                  timeout: float = 4.0) -> list[dict]:
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if not adb(adb_path, serial, "shell", "pidof", PACKAGE):
            raise RuntimeError("SWAMP NativeActivity exited during movement test")
        pid = adb(adb_path, serial, "shell", "pidof", PACKAGE).split()[0]
        log = app_log(adb_path, serial, pid)
        rows = move_records(log)
        if any(row["state"] == state for row in rows):
            return rows
        time.sleep(0.15)
    raise RuntimeError(f"SWAMP NativeActivity did not report MOVE state {state}")


def swipe(adb_path: Path, serial: str, start: tuple[int, int],
          end: tuple[int, int], duration_ms: int,
          output: Path, label: str) -> dict:
    process = subprocess.Popen([
        str(adb_path), "-s", serial, "shell", "input", "swipe",
        str(start[0]), str(start[1]), str(end[0]), str(end[1]), str(duration_ms),
    ], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
        encoding="utf-8", errors="replace")
    try:
        time.sleep(min(0.3, duration_ms / 3000.0))
        # This capture is intentionally taken while the Android swipe command
        # is holding the virtual pointer down, so it records visible feedback.
        held_capture = screen(adb_path, serial, output, f"{label}-held")
        stdout, _ = process.communicate(timeout=duration_ms / 1000.0 + 10.0)
    except Exception:
        process.kill()
        process.wait()
        raise
    if process.returncode:
        raise RuntimeError(f"adb swipe failed: {stdout}")
    return held_capture


def idle_stability(adb_path: Path, serial: str) -> tuple[dict, dict]:
    deadline = time.monotonic() + 3.0
    first = last = None
    while time.monotonic() < deadline:
        pid = adb(adb_path, serial, 'shell', 'pidof', PACKAGE).split()[0]
        records = move_records(app_log(adb_path, serial, pid))
        last_move = max((index for index, row in enumerate(records) if row['state'] == 'MOVE'), default=-1)
        idle = [row for row in records[last_move + 1:] if row['state'] == 'IDLE']
        if len(idle) >= 2:
            first, last = idle[-2:]
            if time.monotonic() >= deadline - 0.25:
                break
        time.sleep(0.15)
    if first is None or last is None:
        raise RuntimeError("two idle source-position reports were not captured after touch release")
    if abs(first["x"] - last["x"]) > 0.03 or abs(first["y"] - last["y"]) > 0.03:
        raise RuntimeError(f"source position drifted after release: {first} -> {last}")
    return first, last


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apk", type=Path, required=True)
    parser.add_argument("--adb", type=Path, required=True)
    parser.add_argument("--serial", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--settle-seconds", type=float, default=1.0)
    parser.add_argument("--startup-timeout-seconds", type=float, default=30.0)
    parser.add_argument("--swipe-ms", type=int, default=900)
    parser.add_argument("--require-prince", action="store_true",
                        help="Require the four-part source-skinned Prince and Idle/Walk touch transitions")
    parser.add_argument("--require-prince-character", action="store_true",
                        help="Also require the authored 116-resource bank and shared Character state path")
    parser.add_argument("--require-source-actor", action="store_true",
                        help="Require scene/root-motion -> one native Step -> actor_runtime ownership and body pinning")
    parser.add_argument("--require-source-room-filter", action="store_true",
                        help="Require exact source root/floor/exit render roles with all navigation input retained")
    parser.add_argument("--require-source-alpha-preview", action="store_true",
                        help="Require the blue-channel AL preview and explicit unresolved AL/AT selection")
    parser.add_argument("--require-source-alpha-selector", action="store_true",
                        help="Require checked source selector-derived AL and explicit profile/render-state limits")
    args = parser.parse_args()
    if args.require_source_actor: args.require_prince_character=True
    if args.require_prince_character: args.require_prince=True

    apk = args.apk.resolve(strict=True)
    adb_path = args.adb.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    built_hash = sha256(apk)
    properties = {
        "android_release": adb(adb_path, args.serial, "shell", "getprop", "ro.build.version.release"),
        "sdk": adb(adb_path, args.serial, "shell", "getprop", "ro.build.version.sdk"),
        "abi": adb(adb_path, args.serial, "shell", "getprop", "ro.product.cpu.abi"),
        "page_size_bytes": adb(adb_path, args.serial, "shell", "getconf", "PAGE_SIZE"),
    }
    if (properties["android_release"], properties["sdk"], properties["abi"]) != ("17", "37", "x86_64"):
        raise RuntimeError(f"expected Android 17/API 37/x86_64, got {properties}")
    if properties["page_size_bytes"] != "16384":
        raise RuntimeError(f"expected the 16 KiB emulator: {properties['page_size_bytes']}")

    install(adb_path, args.serial, apk)
    package_path = adb(adb_path, args.serial, "shell", "pm", "path", PACKAGE)
    base_path = next((line.removeprefix("package:") for line in package_path.splitlines()
                      if line.startswith("package:") and line.endswith("/base.apk")), None)
    if not base_path:
        raise RuntimeError(f"could not find installed base.apk: {package_path}")
    installed = output / "installed-base.apk"
    adb(adb_path, args.serial, "pull", base_path, str(installed))
    installed_hash = sha256(installed)
    if installed_hash != built_hash:
        raise RuntimeError(f"installed APK hash mismatch: built={built_hash} installed={installed_hash}")

    adb(adb_path, args.serial, "shell", "am", "force-stop", PACKAGE)
    global LOG_SINCE
    LOG_SINCE = adb(adb_path, args.serial, "shell", "date", "+%s.%N")
    launch = adb(adb_path, args.serial, "shell", "am", "start", "-W", "-n", ACTIVITY)
    if "Status: ok" not in launch:
        raise RuntimeError(f"NativeActivity launch failed: {launch}")
    time.sleep(max(0.0, args.settle_seconds))
    pid, startup_log = wait_for_assembly(adb_path, args.serial,
                                         max(1.0, args.startup_timeout_seconds),args.require_prince_character)

    initial = screen(adb_path, args.serial, output, "initial")
    if initial["changed_pixels"] < 5000:
        raise RuntimeError(f"SWAMP source screenshot looks blank: {initial['changed_pixels']} changed pixels")
    if (initial["swamp_green_pixels"] < 10000 or
            initial["timber_brown_pixels"] < 1000):
        raise RuntimeError(
            "initial SWAMP screenshot lacks the mapped source diffuse: "
            f"green={initial['swamp_green_pixels']} timber={initial['timber_brown_pixels']}"
        )
    log = capture_log(adb_path, args.serial, output)
    room_assembly = ROOM_ASSEMBLY.search(log)
    assembly = room_assembly or ASSEMBLY.search(log)
    if not assembly:
        raise RuntimeError("source SWAMP module assembly diagnostics are missing")
    if args.require_source_room_filter and not room_assembly:
        raise RuntimeError("source room render-role filtering is absent")
    if room_assembly:
        (source_draws, visible, root_bounds, navigation_floors, exit_markers,
         vertices, indices, mask, start_x, start_y, start_z, diffuse_refs,
         diffuse_draws, alpha_refs, alpha_unresolved_refs,
         alpha_mode, alpha_draws) = room_assembly.groups()
    else:
        (source_draws, visible, omitted, vertices, indices, mask,
         start_x, start_y, start_z, diffuse_refs, diffuse_draws,
         alpha_refs, alpha_unresolved_refs, alpha_mode, alpha_draws) = assembly.groups()
    alpha_count_key = {"AL_preview": "alpha_al_preview_draws", "AL": "alpha_source_al_draws",
                       "cutout": "alpha_cutout_draws"}[alpha_mode]
    geometry = {
        "source_draws": int(source_draws), "vertices": int(vertices),
        "indices": int(indices), "path_mask": int(mask, 16),
        "start": {"x": float(start_x), "y": float(start_y), "z": float(start_z)},
        "diffuse_sampler_refs": int(diffuse_refs),
        "diffuse_draws_mapped": int(diffuse_draws),
        "alpha_map_sampler_refs": int(alpha_refs),
        "alpha_map_unresolved_refs": int(alpha_unresolved_refs),
        alpha_count_key: int(alpha_draws),
    }
    expected = {"source_draws": 54, "vertices": 10816,
                "indices": 13284, "path_mask": 2,
                "diffuse_sampler_refs": 49, "diffuse_draws_mapped": 49,
                "alpha_map_sampler_refs": 22, alpha_count_key: 22}
    if room_assembly:
        geometry.update(visible_scenery_draws=int(visible),
                        room_root_bounds_filtered=int(root_bounds),
                        navigation_floor_draws_filtered=int(navigation_floors),
                        exit_marker_draws_filtered=int(exit_markers),
                        source_navigation_geometry_retained=True)
        expected.update(visible_scenery_draws=49, room_root_bounds_filtered=1,
                        navigation_floor_draws_filtered=2,
                        exit_marker_draws_filtered=2)
    else:
        geometry.update(visible_diagnostic_draws=int(visible),
                        omitted_unresolved_draws=int(omitted))
        expected.update(visible_diagnostic_draws=53, omitted_unresolved_draws=1)
    if {key: geometry[key] for key in expected} != expected:
        raise RuntimeError(f"unexpected SWAMP geometry/assembly summary: {geometry}")
    if (not any(marker in log for marker in ("Irrlicht is confirmed as the game engine family", "Irrlicht is confirmed as the engine family")) or
            "exact customized DH2 Irrlicht fork/revision" not in log):
        raise RuntimeError("source report did not identify the confirmed engine and exact-fork boundary")
    alpha_summary = {"AL_preview": "AlphaMap refs=22 AL-preview draws=22",
                     "AL": "AlphaMap refs=22 source-selector-derived AL draws=22 AT draws=0 selector-unresolved=0",
                     "cutout": "AlphaMap cutout refs=22/draws=22"}[alpha_mode]
    if ("depth-write-off" not in log or
            alpha_summary not in log or
            "ignored lightmap=" not in log.lower()):
        raise RuntimeError("source material limitations were not reported")
    if args.require_source_alpha_preview:
        markers = ("alpha_map_mode=AL-preview", "AT_threshold=0.8_unresolved=1",
                   "source_blend_depth_state=unresolved", "source shader blue mask and fractional alpha")
        if alpha_mode != "AL_preview" or not all(marker in log for marker in markers):
            raise RuntimeError("blue-channel AL preview or unresolved AL/AT selection marker is absent")
        geometry["alpha_preview_scope"] = "Material__11611 blue-channel AL shader output with Irrlicht alpha blending; original AL/AT selection and blend/depth state unresolved"
    if args.require_source_alpha_selector:
        markers = ("alpha_map_policy=source-selector-derived", "AlphaMap_AT_draws=0",
                   "AlphaMap_selector_unresolved_draws=0", "alpha_profile_selection=unresolved",
                   "effect_named_table_ordinals=GLES:2,GLES2:4", "chosen_effect_group=unresolved",
                   "alpha_compiled_ordinal=unresolved", "source_blend_depth_state=unresolved",
                   "AT_threshold=0.8", "source mask uses decoded blue channel",
                   "profile_selectors={GLES:L1_Vc_Al_----_----_----_----;GLES2:L1_Vc_Al_Sp_----_----_----}")
        if alpha_mode != "AL" or not all(marker in log for marker in markers):
            raise RuntimeError("source CurrentTechnique AL selectors or unresolved render-state markers are absent")
        geometry.update(alpha_source_at_draws=0, alpha_selector_unresolved_draws=0,
                        alpha_policy="source-selector-derived",
                        alpha_selectors={"GLES": "L1_Vc_Al_----_----_----_----",
                                         "GLES2": "L1_Vc_Al_Sp_----_----_----"},
                        effect_named_table_ordinals={"GLES": 2, "GLES2": 4},
                        alpha_scope="Both source profiles select AL; blue mask and fractional blending mapped. Runtime profile/effect group/compiled ordinal and original GL blend/depth state unresolved.")
    additive_mapped = "additive_one_one_mapped=2" in log
    if args.require_prince and not additive_mapped:
        raise RuntimeError("the two source ONE/ONE ADD overlay passes are not mapped")
    geometry["source_additive_one_one_passes_mapped"] = 2 if additive_mapped else 0
    prince = re.search(
        r"PRINCE: controllers=(\d+) joints=(\d+) vertices=(\d+) triangles=(\d+) "
        r"parts=(\d+) atlas mapped=(\d+) unmapped diffuse=(\d+) ignored AlphaMaps=(\d+)",
        log,
    )
    prince_record = None
    character_record = None
    if prince:
        keys = ("controllers", "joints", "vertices", "triangles", "parts",
                "mapped_diffuse_parts", "unmapped_diffuse_parts", "ignored_alpha_maps")
        prince_record = dict(zip(keys, map(int, prince.groups())))
        source_binding = "source_visual_binding=owner_helper_graph" in log
        prince_record["source_visual_binding_reported"] = source_binding
        prince_record["placement"] = (
            "source visual owner/helper/graph with fixed first-Idle placement offset; complete Character playback and original camera remain pending"
            if source_binding else
            "development first-Idle bounds anchor; original owner/helper composition pending"
        )
    if args.require_prince:
        expected_prince = {"controllers": 4, "joints": 27, "vertices": 487,
                           "triangles": 586, "parts": 4, "mapped_diffuse_parts": 4,
                           "unmapped_diffuse_parts": 0}
        if not prince_record or any(prince_record[key] != value
                                    for key, value in expected_prince.items()):
            raise RuntimeError(f"source Prince assembly differs: {prince_record}")
        if not prince_record["source_visual_binding_reported"]:
            raise RuntimeError("Prince renderer is missing the source visual owner/helper/graph binding")
    if args.require_prince_character:
        bank = re.search(r"authored_bank_resources=(\d+) registration_occurrences=(\d+) state=(-?\d+) sequence=(-?\d+) clip=(-?\d+)",log)
        if not bank or tuple(map(int,bank.groups()))!=(116,158,3,262,1040):
            raise RuntimeError("Authored Prince bank/source Idle initialization differs")
        if "Character=shared_source_coordinator" not in log or "Irrlicht node identity" not in log:
            raise RuntimeError("Shared source Character or single owner composition marker is missing")
        character_record={"bank_resources":116,"registration_occurrences":158,
            "shared_coordinator_reported":True,"initial_state":3,"idle_sequence":262,
            "walk_sequence":280,"scope":"Original bank/playback/state composition; SWAMP fixed clock/input remain development producers, AI/combat unsupported"}
        prince_record["placement"]="source owner/helper/graph composition with Irrlicht node identity; original camera pending"
    if args.require_source_actor:
        if 'SOURCE_ACTOR_SESSION_READY:' not in log or 'movement owner is SceneBinding source root' not in log:
            raise RuntimeError('Source actor/physics session did not initialize')
        character_record['scope'] = 'Shared bank/Character plus source scene/root-motion, one NativeWorld Step and actor_runtime; fixed20ms development clock/input and Character-owned Stop/pin/unpin services; AI/combat/scripts pending'

    width, height = initial["width"], initial["height"]
    center = (round(width * 0.18), round(height * 0.77))
    radius = min(round(width * 0.18), round(height * 0.24))
    if center[0] >= width or center[1] >= height or radius < 40:
        raise RuntimeError(f"unexpected landscape touch-pad geometry: {width}x{height}")

    captures = {}
    # Source actor: +X then -Y exercises free movement before the separate
    # +Y boundary test. The previous point-mover diagnostic retains +Y.
    y_sign = -1 if args.require_source_actor else 1
    for axis, sign, delta in (("x", 1, (round(radius * 0.65), 0)),
                              ("y", y_sign, (0, -y_sign * round(radius * 0.65)))):
        current_pid = adb(adb_path, args.serial, "shell", "pidof", PACKAGE).split()[0]
        log_before = app_log(adb_path, args.serial, current_pid)
        prior_rows = move_records(log_before)
        prior_idle = next((row for row in reversed(prior_rows) if row["state"] == "IDLE"), None)
        if prior_idle is None:
            raise RuntimeError("no source-coordinate idle position before touch input")
        end = (center[0] + delta[0], center[1] + delta[1])
        held_capture = swipe(adb_path, args.serial, center, end,
                             args.swipe_ms, output, axis)
        rows = wait_for_mode(adb_path, args.serial, "MOVE")
        active = [row for row in rows[len(prior_rows):] if row["state"] == "MOVE"]
        if not active:
                raise RuntimeError(f"no source-coordinate MOVE line was emitted for {sign:+d}{axis}")
        moved = active[-1]
        if args.require_source_actor:
            other = 'y' if axis == 'x' else 'x'
            along = sign * (moved[axis] - prior_idle[axis])
            across = moved[other] - prior_idle[other]
            if along <= 2.5 or along <= abs(across):
                raise RuntimeError(f'Source root motion did not follow requested {sign:+d}{axis}: {prior_idle} -> {moved}')
            if (not moved.get('body_present') or
                    moved['body_service_count'] != prior_idle['body_service_count'] + 1):
                raise RuntimeError('Move focus did not dispatch exactly one source unpin service')
            if not moved.get('path_boundary_checked') or not moved.get('path_direction_valid'):
                raise RuntimeError(f'Moving source actor did not validate its floor/path: {moved}')
        elif axis == "x":
            if moved["x"] <= prior_idle["x"] + 2.5 or abs(moved["y"] - prior_idle["y"]) > 1.0:
                raise RuntimeError(f"right drag did not produce clean source +X motion: {prior_idle} -> {moved}")
        else:
            if moved["y"] <= prior_idle["y"] + 2.5 or abs(moved["x"] - prior_idle["x"]) > 1.0:
                raise RuntimeError(f"up drag did not produce clean source +Y motion: {prior_idle} -> {moved}")
        idle_start, idle_end = idle_stability(adb_path, args.serial)
        if args.require_source_actor and any(
                row['body_service_count'] != moved['body_service_count'] + 2
                for row in (idle_start, idle_end)):
            raise RuntimeError('Move blur did not issue exactly Stop then pin before stable Idle')
        if args.require_prince and (moved["animation"] != "WALK" or
                                   idle_start["animation"] != "IDLE" or
                                   idle_end["animation"] != "IDLE"):
            raise RuntimeError(f"Prince did not select Walk and return to Idle: {moved}, {idle_start}, {idle_end}")
        if args.require_prince_character:
            if (moved["source_state"],moved["sequence"],moved["clip"])!=(4,280,1126):
                raise RuntimeError(f"Source Character Walk selection differs: {moved}")
            if any(row["source_state"]!=3 or row["sequence"]!=262 or row["clip"] not in (1040,1041) for row in (idle_start,idle_end)):
                raise RuntimeError("Release did not return to original source Idle selection")
        captures[f"{axis}_movement"] = {"from": prior_idle, "during": moved,
                                          "idle_start": idle_start, "idle_end": idle_end,
                                          "requested_direction": {"axis": axis, "sign": sign},
                                          "trajectory_scope": "Source root/turning may include lateral displacement; requested-axis movement dominates" if args.require_source_actor else "Development point-mover axis alignment",
                                          "screenshot": held_capture}
        screen(adb_path, args.serial, output, f"after-{axis}-release")

    boundary_capture = None
    if args.require_source_actor:
        prior_rows = move_records(app_log(adb_path, args.serial, pid))
        held = swipe(adb_path, args.serial, center,
                     (center[0], center[1] - round(radius * 0.65)),
                     max(3000, args.swipe_ms), output, 'boundary')
        sweep = [row for row in move_records(app_log(adb_path, args.serial, pid))[len(prior_rows):]
                 if row['state'] == 'MOVE' and row['source_state'] == 4]
        if not sweep or any('validated_x' not in row for row in sweep):
            raise RuntimeError('Source navigation heading diagnostics missing from boundary sweep')
        free = [row for row in sweep if row['desired_y'] > .1 and
                row['validated_y'] > .7 and abs(row['validated_x']) < .3 and
                row['owner_delta_y'] > .1]
        redirected = [row for row in sweep if row['desired_y'] > .1 and
                      row['validated_x'] > .7 and abs(row['validated_y']) < .3 and
                      row['owner_delta_x'] > .1 and row['path_boundary_checked'] and
                      row['path_direction_valid'] and row['floor'] == 0]
        pairs = [(a, b) for a, b in zip(sweep, sweep[1:]) if
                 a in redirected and b in redirected and
                 abs(b['y'] - a['y']) < .5 and b['x'] - a['x'] > 2.5]
        if not free or not pairs:
            raise RuntimeError(f'+Y sweep must prove free +Y followed by source-validated +X boundary sliding: {sweep}')
        idle_start, idle_end = idle_stability(adb_path, args.serial)
        if any(row['source_state'] != 3 or not row['body_pinned'] or
               row['source_path_requested'] or row['source_path_segments']
               for row in (idle_start, idle_end)):
            raise RuntimeError('Boundary input release did not stop, pin and clear its route')
        boundary_capture = {'requested_direction': {'axis': 'y', 'sign': 1},
                            'free_y_sample': free[0],
                            'edge_slide_pair': pairs[0],
                            'idle_start': idle_start, 'idle_end': idle_end,
                            'scope': 'Module-zero boardwalk source PF boundary redirect; environment wall/decor collision bodies are not loaded',
                            'screenshot': held,
                            'released_screenshot': screen(adb_path, args.serial, output, 'after-boundary-release')}

    resume = None
    if args.require_prince:
        before_log = app_log(adb_path, args.serial, pid)
        before_rows = move_records(before_log)
        before_idle = next(row for row in reversed(before_rows)
                           if row["state"] == "IDLE")
        adb(adb_path, args.serial, "shell", "input", "keyevent", "KEYCODE_HOME")
        time.sleep(0.6)
        resume_launch = adb(adb_path, args.serial, "shell", "am", "start", "-W", "-n", ACTIVITY)
        if "Status: ok" not in resume_launch:
            raise RuntimeError(f"NativeActivity resume failed: {resume_launch}")
        time.sleep(1.0)
        resumed_pid = adb(adb_path, args.serial, "shell", "pidof", PACKAGE).split()[0]
        if resumed_pid != pid:
            raise RuntimeError(f"background/resume restarted the source process: {pid} -> {resumed_pid}")
        after_rows = move_records(app_log(adb_path, args.serial, pid))
        if len(after_rows) <= len(before_rows):
            raise RuntimeError("source motion/render loop did not report after resume")
        after_idle = next(row for row in reversed(after_rows)
                          if row["state"] == "IDLE")
        if after_idle["animation"] != "IDLE" or any(
                abs(before_idle[axis] - after_idle[axis]) > 0.03
                for axis in ("x", "y", "z")):
            raise RuntimeError(f"Prince state or position changed across resume: {before_idle} -> {after_idle}")
        resume = {"same_process": True, "before": before_idle, "after": after_idle,
                  "scope": "HOME/background then same-process Activity resume; not process-death save restoration",
                  "screenshot": screen(adb_path, args.serial, output, "resumed")}

    final_log = capture_log(adb_path, args.serial, output)
    final_rows = move_records(final_log)
    if args.require_source_actor:
        if not final_rows or any('actor_step' not in row for row in final_rows):
            raise RuntimeError('Source actor frame/body diagnostics missing')
        for row in final_rows:
            if row.get('body_present') != 1:
                raise RuntimeError(f'Character body ownership is not bound: {row}')
            if row['actor_step'] != row['world_steps'] or row['actor_phase'] != (5 if row['actor_step'] else 0):
                raise RuntimeError(f'Scene/physics/actor frame composition differs: {row}')
            if (row['source_state_flags'], row['body_pinned']) != ((0x2380, 1) if row['source_state'] == 3 else (0x23c1, 0)):
                raise RuntimeError(f'Character flags/body pin differ: {row}')
            if any(abs(row['physics_' + axis] - row[axis] * .01) > .001 for axis in ('x', 'y')):
                raise RuntimeError(f'Physics body is not synchronized with source owner: {row}')
        if any(captures[axis + '_movement']['idle_end']['source_path_segments'] or
               captures[axis + '_movement']['idle_end']['source_path_requested'] for axis in ('x', 'y')):
            raise RuntimeError('Idle release retained a source path request')
    errors = {marker: final_log.count(marker) for marker in FATAL_MARKERS if marker in final_log}
    if errors:
        raise RuntimeError(f"fatal/native/GL error markers found: {errors}")
    primary_screenshots = {
        "initial.png", "x-held.png", "after-x-release.png",
        "y-held.png", "after-y-release.png", "boundary-held.png", "after-boundary-release.png",
    }
    screenshot_records = {
        path.name: {"path": str(path), "bytes": path.stat().st_size,
                    "sha256": sha256(path)}
        for path in sorted(output.glob("*.png"))
        if path.name in primary_screenshots
    }
    report = {
        "scope": "SWAMP module-zero source geometry and source-coordinate touch movement diagnostic; not complete DH2 gameplay",
        "validation": "PASS",
        "source_actor_assertions_required": args.require_source_actor,
        "launch_log_epoch_since": LOG_SINCE,
        "serial": args.serial,
        **properties,
        "renderer": next((line.strip() for line in adb(adb_path, args.serial,
                             "shell", "dumpsys", "SurfaceFlinger").splitlines()
                          if line.strip().startswith("GLES:")), "renderer not reported"),
        "package": PACKAGE,
        "activity": ACTIVITY,
        "launch_status": "ok",
        "pid": pid,
        "touch_pad": {"center": {"x": center[0], "y": center[1]}, "radius": radius},
        "built_apk": {"path": str(apk), "bytes": apk.stat().st_size, "sha256": built_hash},
        "installed_base_apk": {"path": str(installed), "bytes": installed.stat().st_size,
                                "sha256": installed_hash},
        "source_geometry": geometry,
        "source_room_filter_assertions_required": args.require_source_room_filter,
        "source_prince": prince_record,
        "source_prince_character": character_record,
        "prince_motion_assertions_required": args.require_prince,
        "texture_visual_qa": {
            "sample": "initial screenshot central scene region",
            "swamp_green_pixels": initial["swamp_green_pixels"],
            "timber_brown_pixels": initial["timber_brown_pixels"],
            "criteria": "at least 10,000 swamp-green and 1,000 timber-brown pixels, plus 49 diffuse source references mapped to 49 draws",
            "passed": True,
        },
        "startup_log_contains_assembly_before_screenshot": bool(
            ASSEMBLY.search(startup_log) or ROOM_ASSEMBLY.search(startup_log)),
        "movement_assertions": captures,
        "boundary_navigation": boundary_capture,
        "background_resume": resume,
        "move_log_records": final_rows,
        "screenshots": screenshot_records,
        "app_pid_logcat": str(output / "logcat.txt"),
        "fatal_error_markers": errors,
    }
    report_path = output / "irrlicht-swamp-runtime-validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError, KeyError) as error:
        raise SystemExit(f"ERROR: {error}")
