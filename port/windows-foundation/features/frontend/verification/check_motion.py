"""Measure body-region motion separately from a static background sample.

These measurements establish rendered changes only, not original animation parity.
"""
import argparse
import json
from pathlib import Path
import re
import numpy as np
from check_capture import read_ppm


def compare(left, right, region):
    x0, y0, x1, y1 = region
    delta = np.abs(left[y0:y1, x0:x1].astype(np.int16)
                   - right[y0:y1, x0:x1].astype(np.int16))
    return {"changed_fraction": float(np.any(delta > 0, axis=2).mean()),
            "changed_over_8_fraction": float(np.any(delta > 8, axis=2).mean()),
            "mean_channel_change": float(delta.mean())}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("series", nargs="+", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    report = {"scope": "Rendered ROI pixel motion, no original animation parity claim",
              "body_roi_960x540": [300, 175, 680, 410],
              "background_roi_960x540": [600, 100, 700, 160], "series": []}
    for directory in args.series:
        frames = []
        for path in directory.glob("frame-*.ppm"):
            match = re.search(r"-(\d+)ms\.ppm$", path.name)
            if match:
                frames.append((int(match[1]), path))
        frames.sort()
        measurements = []
        previous = None
        for ms, path in frames:
            width, height, pixels = read_ppm(path)
            if (width, height) != (960, 540):
                raise ValueError("Motion ROIs require 960x540 captures")
            current = np.frombuffer(pixels, dtype=np.uint8).reshape(height, width, 3)
            if previous is not None:
                old_ms, old_pixels = previous
                measurements.append({"from_ms": old_ms, "to_ms": ms,
                    "body": compare(old_pixels, current, report["body_roi_960x540"]),
                    "background": compare(old_pixels, current, report["background_roi_960x540"])})
            previous = ms, current
        report["series"].append({"path": str(directory.resolve()),
                                  "frames": len(frames), "changes": measurements})
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
