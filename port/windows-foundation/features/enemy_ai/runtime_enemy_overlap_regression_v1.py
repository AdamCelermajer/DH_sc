"""Reproduce or gate the authored-body overlap in a same-session gameplay log.

Default mode confirms the supplied known-bad capture still contains overlap.
Pass --require-separated plus a fresh runtime log to require every pair to be
within source Box2D position-solver slop after shared-world stepping.
"""
from __future__ import annotations

import argparse
import math
import pathlib
import re
import sys


ROOT = pathlib.Path(__file__).resolve().parents[4]
DEFAULT_LOG = ROOT / ".local-inputs/v19-frontend-hotfix/profile-projection/mage-incoming-normal.log"
REQUIRED_NAMES = {
    "player": "MagePlayerBase",
    "lizard_a": "_prim_Monster_3_05_313_356_14_33_13_179_13_38",
    "lizard_b": "_prim_LizTemplate_03",
}
# Native body XY/radius values are expressed in game units (native_body.cpp
# multiplies metre-space observations by 100). Box2D 2.0.1's position solver
# returns success at minSeparation >= -1.5 * b2_linearSlop, where the authored
# linear slop is 0.005 m. Preserve that solver tolerance rather than treating
# a contact-slop residual as unresolved gameplay penetration.
GAME_UNITS_PER_METRE = 100.0
BOX2D_LINEAR_SLOP_METRES = 0.005
POSITION_SOLVER_TOLERANCE = 1.5 * BOX2D_LINEAR_SLOP_METRES * GAME_UNITS_PER_METRE


def fail(message: str) -> None:
    raise RuntimeError(message)


def parse(log_path: pathlib.Path) -> tuple[
        dict[str, tuple[str, tuple[float, float, float]]], dict[str, float],
        str, tuple[int, int, int] | None]:
    if not log_path.is_file():
        fail(f"Gameplay log does not exist: {log_path}")
    text = log_path.read_text(encoding="utf-8", errors="replace")
    actors: dict[str, tuple[str, tuple[float, float, float]]] = {}
    bodies: dict[str, float] = {}
    actor_re = re.compile(r"^Actor final id=(\d+) definition=(.*?) HP=.*? position=([-+\d.eE]+),([-+\d.eE]+),([-+\d.eE]+)$", re.M)
    for match in actor_re.finditer(text):
        actor_id, definition = match.group(1), match.group(2)
        name = definition.rsplit("|", 1)[-1] if "|" in definition else definition
        pos = tuple(float(match.group(index)) for index in (3, 4, 5))
        for label, wanted in REQUIRED_NAMES.items():
            if label == "player":
                continue
            if name == wanted:
                if label in actors:
                    fail(f"Ambiguous live actor definition {wanted}")
                actors[label] = (actor_id, pos)
        if definition == "MagePlayerBase":
            if "player" in actors:
                fail("Ambiguous Mage player actor")
            actors["player"] = (actor_id, pos)
    body_re = re.compile(r"^Native body final actor=(\d+) physical=1 radius=([-+\d.eE]+)", re.M)
    for match in body_re.finditer(text):
        bodies[match.group(1)] = float(match.group(2))
    missing = [label for label in REQUIRED_NAMES if label not in actors]
    if missing:
        fail("Capture is missing same-session actor rows: " + ", ".join(missing))
    for label, (actor_id, _) in actors.items():
        if actor_id not in bodies:
            fail(f"Capture is missing actual retained NativeBody radius for {label} actor {actor_id}")
    step_match = re.search(
        r"^Source physical frame final steps=(\d+) imports=(\d+) suppressedRoots=(\d+)",
        text, re.M)
    step_summary = tuple(int(step_match.group(index)) for index in (1, 2, 3)) if step_match else None
    if step_summary is None and "world Step remains unbound" not in text:
        fail("Capture has neither a physical-frame Step summary nor the known unbound-Step marker")
    if step_summary is not None:
        for label, (actor_id, _) in actors.items():
            body_line = re.search(
                rf"^Native body final actor={re.escape(actor_id)} physical=1 .*?sourcePositionMatch=1$",
                text, re.M)
            if not body_line:
                fail(f"Stepped capture does not confirm same-actor body publication for {label}")
    return actors, bodies, text, step_summary


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("log", nargs="?", type=pathlib.Path, default=DEFAULT_LOG)
    parser.add_argument("--require-separated", action="store_true",
                        help="accept when all required body circles are within source solver tolerance")
    args = parser.parse_args()
    try:
        actors, bodies, _, step_summary = parse(args.log)
        if args.require_separated and (step_summary is None or step_summary[0] <= 0):
            fail("Separation acceptance requires a successful same-Session NativeWorld Step")
        labels = tuple(REQUIRED_NAMES)
        overlaps: list[str] = []
        for left_index, left in enumerate(labels):
            left_id, left_position = actors[left]
            for right in labels[left_index + 1:]:
                right_id, right_position = actors[right]
                distance = math.hypot(left_position[0] - right_position[0],
                                      left_position[1] - right_position[1])
                combined_radius = bodies[left_id] + bodies[right_id]
                penetration = combined_radius - distance
                if penetration > POSITION_SOLVER_TOLERANCE:
                    overlaps.append(f"{left}/{right} penetration={penetration:.3f} "
                                    f"distance={distance:.3f} radii={bodies[left_id]:.3f}+{bodies[right_id]:.3f}")
        if args.require_separated:
            if overlaps:
                fail("Source body overlap remains: " + "; ".join(overlaps))
            print("same-session enemy geometry PASS all retained source body circles within "
                  f"Box2D position-solver tolerance ({POSITION_SOLVER_TOLERANCE:.3f} game units)")
        else:
            if not overlaps:
                fail("Known overlap no longer reproduces; inspect current runtime and use --require-separated for acceptance")
            mode = "enabled" if step_summary is not None else "unbound"
            print(f"same-session enemy geometry GAP reproduced; NativeWorld Step {mode}")
            if step_summary is not None:
                print(f"  steps={step_summary[0]} imports={step_summary[1]} suppressedRoots={step_summary[2]}")
            for row in overlaps:
                print("  " + row)
    except Exception as error:  # noqa: BLE001 - standalone diagnostic reports input failures.
        print(f"same-session enemy geometry FAIL: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
