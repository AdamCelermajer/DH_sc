"""Reproduce or gate the authored-body overlap in a same-session gameplay log.

Default mode confirms the supplied known-bad capture still contains overlap.
Pass --require-separated plus a fresh runtime log to use it as the eventual
acceptance check after the shared source physics/contact owner is connected.
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


def fail(message: str) -> None:
    raise RuntimeError(message)


def parse(log_path: pathlib.Path) -> tuple[dict[str, tuple[str, tuple[float, float, float]]], dict[str, float], str]:
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
    if "world Step remains unbound" not in text:
        fail("Capture does not disclose the known native-world Step boundary")
    return actors, bodies, text


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("log", nargs="?", type=pathlib.Path, default=DEFAULT_LOG)
    parser.add_argument("--require-separated", action="store_true",
                        help="accept only when all required body circles are separated")
    args = parser.parse_args()
    try:
        actors, bodies, _ = parse(args.log)
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
                if penetration > 0.0:
                    overlaps.append(f"{left}/{right} penetration={penetration:.3f} "
                                    f"distance={distance:.3f} radii={bodies[left_id]:.3f}+{bodies[right_id]:.3f}")
        if args.require_separated:
            if overlaps:
                fail("Source body overlap remains: " + "; ".join(overlaps))
            print("same-session enemy geometry PASS all retained source body circles separated")
        else:
            if not overlaps:
                fail("Known overlap no longer reproduces; inspect current runtime and use --require-separated for acceptance")
            print("same-session enemy geometry GAP reproduced; native world Step is unbound")
            for row in overlaps:
                print("  " + row)
    except Exception as error:  # noqa: BLE001 - standalone diagnostic reports input failures.
        print(f"same-session enemy geometry FAIL: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
