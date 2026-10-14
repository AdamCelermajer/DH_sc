#!/usr/bin/env python3
"""One command, three builds of the current source tree: Windows x64, Linux x86-64 (Fedora 40 container), Android arm64.

    py -3 tools/build/build.py all            # everything that is available
    py -3 tools/build/build.py windows linux  # chosen targets
    py -3 tools/build/build.py all --test     # also run the Windows ctest suite first
    py -3 tools/build/build.py serve          # serve the dist folder over Tailscale for downloads

Results land in <out_dir>/<version>/ and are mirrored (stable names) in <out_dir>/latest/.
The game data (assets, audio-assets, ui-assets, args files) comes from the package directory in config.json; the
build never modifies it. Saves (*.gsave*) and old executables are never packaged.
"""
import argparse, datetime, hashlib, json, os, shutil, string, subprocess, sys, time, zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
CONFIG_PATH = Path(__file__).with_name("config.json")
LOCAL_CONFIG_PATH = Path(__file__).with_name("config.local.json")  # optional per-machine overrides, not committed


def load_config():
    cfg = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
    if LOCAL_CONFIG_PATH.exists():
        cfg.update(json.loads(LOCAL_CONFIG_PATH.read_text(encoding="utf-8")))
    for key in ("toolchain_bin", "cmake_bin", "package_dir", "out_dir", "build_root"):
        cfg[key] = str((REPO / cfg[key]).resolve()) if not os.path.isabs(cfg[key]) else cfg[key]
    return cfg


def run(cmd, cwd=None, env=None, log=None, check=True):
    print("  $", " ".join(str(c) for c in cmd)[:240], flush=True)
    out = open(log, "w", encoding="utf-8", errors="replace") if log else None
    try:
        proc = subprocess.run([str(c) for c in cmd], cwd=cwd, env=env, stdout=out or None, stderr=subprocess.STDOUT if out else None)
    finally:
        if out: out.close()
    if check and proc.returncode != 0:
        if log:
            tail = Path(log).read_text(encoding="utf-8", errors="replace").splitlines()[-25:]
            print("\n".join("    | " + l for l in tail))
        raise SystemExit(f"FAILED ({proc.returncode}): {cmd[0]} ... see {log or 'output above'}")
    return proc.returncode


def version_stamp():
    def git(*a):
        return subprocess.run(["git", "-C", str(REPO), *a], capture_output=True, text=True).stdout.strip()
    commit = git("rev-parse", "--short=8", "HEAD") or "nogit"
    dirty = "-dirty" if git("status", "--porcelain", "--untracked-files=no") else ""
    return datetime.datetime.now().strftime("%Y%m%d-%H%M") + "-" + commit + dirty


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def data_files(package_dir):
    """Every file of the data package except saves, old executables and launcher scripts of other platforms."""
    skip_names = {"dh-foundation.exe", "package-receipt.json", "Play.cmd", "Play-swamp.cmd", "README.txt"}  # per-platform launchers/readme are added by the target
    for path in sorted(Path(package_dir).rglob("*")):
        if not path.is_file():
            continue
        rel = path.relative_to(package_dir)
        name = path.name
        if name in skip_names and len(rel.parts) == 1:
            continue
        if ".gsave" in name or name.endswith((".args.bak",)) or name.startswith("w5-") or name.endswith("-package.json"):
            continue
        yield path, rel.as_posix()


def make_zip(zip_path, top, package_dir, extra, unix_exec=()):
    """extra: list of (source path, archive name inside top, executable?). Streams files; nothing is copied to disk."""
    zip_path = Path(zip_path)
    zip_path.parent.mkdir(parents=True, exist_ok=True)
    if zip_path.exists():
        zip_path.unlink()
    count = 0
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED, compresslevel=6) as z:
        def add(src, arc, executable):
            nonlocal count
            zi = zipfile.ZipInfo.from_file(str(src), f"{top}/{arc}")
            zi.create_system = 3  # Unix: lets `unzip` honour the executable bit
            zi.external_attr = (0o100000 | (0o755 if executable else 0o644)) << 16
            with open(src, "rb") as fh:
                z.writestr(zi, fh.read(), zipfile.ZIP_DEFLATED, 6)
            count += 1
        for src, arc in data_files(package_dir):
            add(src, arc, False)
        for src, arc, executable in extra:
            add(src, arc, executable)
    return count


def free_drive_letter():
    used = subprocess.run(["subst"], capture_output=True, text=True).stdout
    for letter in "ZYXWVUTSRQPONM":
        if not Path(f"{letter}:/").exists() and f"{letter}:\\" not in used:
            return letter
    raise SystemExit("no free drive letter for the short source path")


# ---------------------------------------------------------------------------------------------------------------- Windows
def build_windows(cfg, version, dist, args):
    print("== windows x64 (llvm-mingw clang, Release)")
    build_dir = Path(cfg["build_root"]) / "windows"
    build_dir.mkdir(parents=True, exist_ok=True)
    cmake = Path(cfg["cmake_bin"]) / "cmake.exe"
    ninja = Path(cfg["cmake_bin"]) / "ninja.exe"
    tc = Path(cfg["toolchain_bin"])
    fwd = lambda p: Path(p).as_posix()  # CMake cache values must not contain backslashes
    env = dict(os.environ, PATH=f"{tc};{cfg['cmake_bin']};{os.environ['PATH']}")
    letter = free_drive_letter()
    run(["subst", f"{letter}:", str(REPO)])  # short source path: the tree is deep and MAX_PATH bites
    try:
        src = f"{letter}:/port/windows-foundation"
        if not (build_dir / "CMakeCache.txt").exists():
            run([cmake, "-S", src, "-B", fwd(build_dir), "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release", f"-DCMAKE_MAKE_PROGRAM={fwd(ninja)}",
                 f"-DCMAKE_CXX_COMPILER={fwd(tc / 'clang++.exe')}", f"-DCMAKE_C_COMPILER={fwd(tc / 'clang.exe')}"], env=env, log=build_dir / "configure.log")
        target = [] if args.test else ["--target", "dh-foundation"]
        run([cmake, "--build", build_dir, *target], env=env, log=build_dir / "build.log")
        if args.test:
            print("  running ctest ...")
            run(["ctest", "--test-dir", build_dir, "-j4"], env=env, log=build_dir / "ctest.log")
    finally:
        run(["subst", f"{letter}:", "/D"], check=False)
    exe = build_dir / "dh-foundation.exe"
    extra = [(exe, "dh-foundation.exe", True)]
    for name in ("Play.cmd", "Play-swamp.cmd", "README.txt"):
        p = Path(cfg["package_dir"]) / name
        if p.exists():
            extra.append((p, name, False))
    out = dist / f"dh2-windows-x64-{version}.zip"
    make_zip(out, "dungeon-hunter-2-windows", cfg["package_dir"], extra)
    return out


# ---------------------------------------------------------------------------------------------------------------- Linux
DOCKERFILE = """FROM {image}
RUN dnf install -y gcc-c++ cmake ninja-build SDL2-devel mesa-libGL-devel mesa-libGLU-devel pkgconf-pkg-config binutils && dnf clean all
"""

LINUX_STEPS = r"""set -e
if [ ! -f /build/CMakeCache.txt ]; then
  cmake -S /src/port/windows-foundation -B /build -G Ninja -DCMAKE_BUILD_TYPE=Release > /build/configure.log 2>&1 || { tail -30 /build/configure.log; exit 1; }
fi
cmake --build /build --target dh-foundation -j"$(nproc)" > /build/build.log 2>&1 || { grep -E "error|FAILED" /build/build.log | head -30; exit 1; }
cp /build/dh-foundation /out/dh-foundation
objdump -T /build/dh-foundation | grep -o "GLIBC_[0-9.]*" | sort -V | uniq | tail -1
"""


def docker_ready():
    return subprocess.run(["docker", "info"], capture_output=True).returncode == 0


def start_docker_desktop(cfg):
    exe = Path(os.environ.get("LOCALAPPDATA", "")) / "Programs" / "DockerDesktop" / "Docker Desktop.exe"
    if not exe.exists():
        exe = Path("C:/Program Files/Docker/Docker/Docker Desktop.exe")
    if not exe.exists():
        raise SystemExit("Docker Desktop is not installed (needed for the Linux build)")
    print("  starting Docker Desktop ...", flush=True)
    subprocess.Popen([str(exe)])
    for _ in range(60):
        time.sleep(5)
        if docker_ready():
            return
    raise SystemExit("Docker did not come up in 5 minutes")


def build_linux(cfg, version, dist, args):
    print(f"== linux x86-64 ({cfg['docker_image']} container, SDL2 + OpenGL)")
    if not docker_ready():
        start_docker_desktop(cfg)
    image = "dh2-build-" + cfg["docker_image"].replace(":", "")
    if subprocess.run(["docker", "image", "inspect", image], capture_output=True).returncode != 0:
        print("  building the build image (once) ...", flush=True)
        subprocess.run(["docker", "build", "-t", image, "-"], input=DOCKERFILE.format(image=cfg["docker_image"]).encode(), check=True)
    out_dir = Path(cfg["build_root"]) / "linux-out"
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "dh-foundation").unlink(missing_ok=True)
    script = Path(cfg["build_root"]) / "linux-steps.sh"
    script.write_text(LINUX_STEPS, newline="\n")
    env = dict(os.environ, MSYS_NO_PATHCONV="1")
    volume = "dh2-linux-build-" + cfg["docker_image"].replace(":", "")  # named volume: incremental builds, fast ext4
    run(["docker", "run", "--rm", "-v", f"{REPO}:/src:ro", "-v", f"{volume}:/build", "-v", f"{out_dir}:/out",
         "-v", f"{script}:/steps.sh:ro", image, "bash", "/steps.sh"], env=env)
    binary = out_dir / "dh-foundation"
    if not binary.exists():
        raise SystemExit("Linux build produced no binary")
    tmp = Path(cfg["build_root"]) / "linux-launchers"
    tmp.mkdir(parents=True, exist_ok=True)
    (tmp / "play.sh").write_text('#!/bin/sh\ncd "$(dirname "$0")" || exit 1\nexec ./dh-foundation --startup-config startup.args\n', newline="\n")
    (tmp / "play-swamp.sh").write_text('#!/bin/sh\ncd "$(dirname "$0")" || exit 1\nexec ./dh-foundation --startup-config swamp.args\n', newline="\n")
    (tmp / "README-LINUX.txt").write_text(
        "Dungeon Hunter II reconstruction - Linux x86-64 (built in a Fedora container, SDL2 + OpenGL)\n\n"
        "Run: ./play.sh (main menu) or ./play-swamp.sh. If not executable: chmod +x play.sh play-swamp.sh dh-foundation\n"
        "Needs SDL2 and a working OpenGL driver: sudo dnf install SDL2 mesa-dri-drivers\n"
        "Saves are written to this folder.\n", newline="\n")
    extra = [(binary, "dh-foundation", True), (tmp / "play.sh", "play.sh", True), (tmp / "play-swamp.sh", "play-swamp.sh", True),
             (tmp / "README-LINUX.txt", "README-LINUX.txt", False)]
    out = dist / f"dh2-linux-x86_64-{version}.zip"
    make_zip(out, "dungeon-hunter-2-linux", cfg["package_dir"], extra)
    return out


# ---------------------------------------------------------------------------------------------------------------- Android
def build_android(cfg, version, dist, args):
    print("== android arm64-v8a (NDK + Gradle)")
    script = REPO / cfg["android_build_script"]
    if not script.exists():
        print(f"  not available yet: {cfg['android_build_script']} does not exist (the Android port is still being built)")
        return None
    out_dir = dist / "android-tmp"
    out_dir.mkdir(parents=True, exist_ok=True)
    # Contract with the Android project: `build.ps1 -Abi arm64-v8a -PackageDir <dir> -OutDir <dir> -Version <v>` leaves
    # dh2-android-arm64.apk in OutDir and exits 0.
    run(["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", script, "-Abi", "arm64-v8a",
         "-PackageDir", cfg["package_dir"], "-OutDir", out_dir, "-Version", version], log=Path(cfg["build_root"]) / "android.log")
    apk = out_dir / "dh2-android-arm64.apk"
    if not apk.exists():
        raise SystemExit("Android build script finished without dh2-android-arm64.apk")
    final = dist / f"dh2-android-arm64-{version}.apk"
    shutil.move(str(apk), final)
    shutil.rmtree(out_dir, ignore_errors=True)
    return final


# ---------------------------------------------------------------------------------------------------------------- serve
def tailscale_ip():
    for exe in ("tailscale", "C:/Program Files/Tailscale/tailscale.exe"):
        try:
            return subprocess.run([exe, "ip", "-4"], capture_output=True, text=True, check=True).stdout.split()[0]
        except Exception:
            continue
    return None


def serve(cfg, port):
    root = Path(cfg["out_dir"]) / "latest"
    root.mkdir(parents=True, exist_ok=True)
    ip = tailscale_ip()
    if not ip:
        raise SystemExit("Tailscale address not found; is Tailscale running?")
    print(f"serving {root} on http://{ip}:{port}/  (Ctrl+C to stop)")
    for f in sorted(root.iterdir()):
        print(f"  http://{ip}:{port}/{f.name}")
    os.chdir(root)
    from http.server import HTTPServer, SimpleHTTPRequestHandler
    HTTPServer((ip, port), SimpleHTTPRequestHandler).serve_forever()


# ---------------------------------------------------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("targets", nargs="+", choices=["windows", "linux", "android", "all", "serve"])
    ap.add_argument("--test", action="store_true", help="build and run the whole Windows ctest suite as well")
    ap.add_argument("--port", type=int, default=8765, help="port for `serve`")
    args = ap.parse_args()
    cfg = load_config()
    if "serve" in args.targets:
        serve(cfg, args.port)
        return
    targets = ["windows", "linux", "android"] if "all" in args.targets else args.targets
    version = version_stamp()
    dist = Path(cfg["out_dir"]) / version
    dist.mkdir(parents=True, exist_ok=True)
    print(f"version {version}  ->  {dist}")
    builders = {"windows": build_windows, "linux": build_linux, "android": build_android}
    results, started = {}, time.time()
    for name in targets:
        t0 = time.time()
        try:
            out = builders[name](cfg, version, dist, args)
            results[name] = {"file": out.name if out else None, "status": "ok" if out else "skipped",
                             "seconds": round(time.time() - t0)}
            if out:
                results[name].update(bytes=out.stat().st_size, sha256=sha256(out))
        except SystemExit as exc:
            results[name] = {"file": None, "status": "FAILED", "error": str(exc), "seconds": round(time.time() - t0)}
            print("  " + str(exc))
    latest = Path(cfg["out_dir"]) / "latest"
    latest.mkdir(parents=True, exist_ok=True)
    for name, info in results.items():
        if info.get("file"):
            stable = info["file"].replace(f"-{version}", "")
            shutil.copy2(dist / info["file"], latest / stable)
    (dist / "BUILD-INFO.json").write_text(json.dumps({"version": version, "targets": results}, indent=2), encoding="utf-8")
    print("\n== summary")
    for name, info in results.items():
        size = f"{info['bytes'] / 1048576:.0f} MB" if info.get("bytes") else ""
        print(f"  {name:8} {info['status']:8} {info.get('file') or info.get('error', '')} {size} ({info['seconds']} s)")
    print(f"  folder: {dist}\n  stable copies: {latest}")
    if any(i["status"] == "FAILED" for i in results.values()):
        sys.exit(1)


if __name__ == "__main__":
    main()
