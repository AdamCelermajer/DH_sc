#!/usr/bin/env python3
"""Verifies the Preview 15 intro movie against ffmpeg (stdlib only).

Checks, for each window capture (PPM written by --boot-capture):
  * the picture: the movie (1024x576, aspect-fit into the window) is compared with the
    ffmpeg frames of intro_v1.mpg around the expected frame index (time * 24);
    prints the best matching frame index and PSNR (expected: best index within 2 frames of time * 24; the picture is driven by the audible soundtrack clock, which is ~1-2 frames behind nominal because of the platform queue);
  * optionally the soundtrack: the sample count of the .mpg audio (ffmpeg s16le 48 kHz
    stereo) against the duration the EXE logged (--log, the "soundtrack_duration=" value).

Usage:
  py -3 verify_intro_movie.py --ffmpeg <ffmpeg.exe> --movie <intro_v1.mpg> \
      --capture 6=<m-6.ppm> --capture 15=<m-15.ppm> [--log <log.txt>] [--work <dir>]
Exit code 0 when every capture matches (PSNR >= 30 dB at the best index, within 1 frame of the
expected index) and the soundtrack duration agrees with ffmpeg to 20 ms.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

MOVIE_W, MOVIE_H = 1024, 576
FPS = 24


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    tokens, i = [], 0
    while len(tokens) < 4:
        while data[i:i + 1] in (b" ", b"\t", b"\r", b"\n"):
            i += 1
        j = i
        while data[j:j + 1] not in (b" ", b"\t", b"\r", b"\n"):
            j += 1
        tokens.append(data[i:j])
        i = j
    i += 1
    magic, w, h, _maxv = tokens
    if magic != b"P6":
        raise SystemExit("not a P6 PPM: " + path)
    w, h = int(w), int(h)
    return w, h, data[i:i + w * h * 3]


def ffmpeg_frames(ffmpeg, movie, indices, work):
    """Decodes the given frame indices with ffmpeg as rgb24 (1024x576)."""
    expr = "+".join("eq(n\\,%d)" % k for k in indices)
    out = os.path.join(work, "ref-frames.rgb")
    cmd = [ffmpeg, "-v", "error", "-y", "-i", movie, "-vf", "select=" + expr,
           "-vsync", "0", "-f", "rawvideo", "-pix_fmt", "rgb24", out]
    subprocess.run(cmd, check=True)
    size = MOVIE_W * MOVIE_H * 3
    with open(out, "rb") as f:
        raw = f.read()
    if len(raw) != size * len(indices):
        raise SystemExit("ffmpeg returned %d frames for %d indices" % (len(raw) // size, len(indices)))
    return {k: raw[n * size:(n + 1) * size] for n, k in enumerate(indices)}


def window_to_movie(win_w, win_h, win_pixels, flip):
    """Samples the aspect-fit movie rectangle of the window on a 4-pixel grid (movie coordinates)."""
    scale = min(win_w / MOVIE_W, win_h / MOVIE_H)
    dw, dh = MOVIE_W * scale, MOVIE_H * scale
    ox, oy = (win_w - dw) / 2.0, (win_h - dh) / 2.0
    out = bytearray()
    for my in range(0, MOVIE_H, 4):
        wy = int((my + 0.5) * scale + oy)
        if flip:
            wy = win_h - 1 - wy
        row = wy * win_w
        for mx in range(0, MOVIE_W, 4):
            wx = int((mx + 0.5) * scale + ox)
            p = (row + wx) * 3
            out += win_pixels[p:p + 3]
    return bytes(out)


def psnr_grid(sample, ref):
    """PSNR between a 4-pixel-grid sample and the same grid of the reference frame."""
    ref_grid = bytearray()
    for my in range(0, MOVIE_H, 4):
        base = my * MOVIE_W * 3
        for mx in range(0, MOVIE_W, 4):
            p = base + mx * 3
            ref_grid += ref[p:p + 3]
    total = 0
    for a, b in zip(sample, ref_grid):
        d = a - b
        total += d * d
    mse = total / float(len(sample))
    return 99.0 if mse == 0 else 10.0 * __import__("math").log10(255.0 * 255.0 / mse)


def audio_seconds(ffmpeg, movie, work):
    out = os.path.join(work, "audio.s16le")
    subprocess.run([ffmpeg, "-v", "error", "-y", "-i", movie, "-map", "0:a:0", "-f", "s16le",
                    "-ac", "2", "-ar", "48000", out], check=True)
    return os.path.getsize(out) / 4.0 / 48000.0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--ffmpeg", required=True)
    ap.add_argument("--movie", required=True)
    ap.add_argument("--capture", action="append", default=[], help="T=<ppm>: T = boot time of the capture")
    ap.add_argument("--log", help="boot log containing soundtrack_duration=")
    ap.add_argument("--work", help="scratch directory (default: temp)")
    args = ap.parse_args()
    work = args.work or tempfile.mkdtemp(prefix="intro-verify-")
    os.makedirs(work, exist_ok=True)

    ok = True
    captures = []
    for spec in args.capture:
        t, path = spec.split("=", 1)
        w, h, pixels = read_ppm(path)
        expected = int(float(t) * FPS)
        captures.append((float(t), path, w, h, pixels, expected))
    indices = sorted({k + d for (_, _, _, _, _, k) in captures for d in range(-3, 4) if k + d >= 0})
    refs = ffmpeg_frames(args.ffmpeg, args.movie, indices, work) if indices else {}
    for t, path, w, h, pixels, expected in captures:
        best = None
        for flip in (False, True):
            sample = window_to_movie(w, h, pixels, flip)
            for k in indices:
                if abs(k - expected) > 3:
                    continue
                score = psnr_grid(sample, refs[k])
                if best is None or score > best[0]:
                    best = (score, k, flip)
        score, k, flip = best
        good = score >= 30.0 and abs(k - expected) <= 2
        ok = ok and good
        print("capture t=%.1f %s window=%dx%d expected_frame=%d best_frame=%d psnr=%.2f dB flipped=%s %s"
              % (t, os.path.basename(path), w, h, expected, k, score, flip, "OK" if good else "MISMATCH"))

    if args.log:
        text = open(args.log, encoding="utf-8", errors="replace").read()
        m = re.findall(r"soundtrack_duration=([0-9.]+)", text)
        if not m:
            print("audio: no soundtrack_duration in log")
            ok = False
        else:
            logged = float(m[-1])
            ff = audio_seconds(args.ffmpeg, args.movie, work)
            same = abs(logged - ff) <= 0.02
            ok = ok and same
            print("audio: ffmpeg=%.4f s logged=%.4f s %s" % (ff, logged, "OK" if same else "MISMATCH"))
    print("intro movie verification: %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
