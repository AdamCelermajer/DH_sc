#!/usr/bin/env python3
"""Build the DH2 Save Editor. One command from a clean checkout:

  py -3 -I tools/build.py --data <game files dir> [--apk-base <dh2-trackb-*.apk>] [--release <version>]

Steps (everything goes to build/, the release to release/<version>/; dh_noam is only read):
  1. host runner of the reconstructed game (dh_noam/source_code/port/decomp-reconstruction, DH2_HOST_RUNNER=ON) in
     build/runner, target dh2_texconv (configured and built only when missing; about 10 minutes the first time);
  2. itemdump/build_itemdump.py -> build/itemdump/dh2_itemdump.exe (links like the runner's own tools);
  3. dh2_itemdump on a copy of the game files (no saves copied) -> build/itemdata-raw.json: the game's own tables and
     the item/bonus texts its ItemInstance code builds, for every language pack;
  4. tools/gen_itemdata.py -> build/out/web/itemdata.js (+ build/out/itemdata.json for the tests);
  5. Windows app: windows/dh2_save_editor_win.cpp (llvm-mingw clang++) + build/out/web -> build/out/windows/;
  6. --apk-base: tools/build_modded_apk.py -> build/out/dh2-trackb-saveeditor.apk;
  7. --release: release/<version>/ with the APK, the Windows app folder and README.txt.
Tools: llvm-mingw (clang++ on PATH or --toolchain), cmake + ninja on PATH, ANDROID_HOME (or dh_noam/source_code/build/sdk), JDK.
"""
import argparse, os, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
EXE = ".exe" if os.name == "nt" else ""


def run(*a, **k):
    print(">", " ".join(str(x) for x in a), flush=True)
    subprocess.check_call([str(x) for x in a], **k)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data", help="game files dir (Android .../files or a copy); only read")
    ap.add_argument("--dh-noam", default=os.path.join(REPO, "..", "dh_noam", "source_code"))
    ap.add_argument("--toolchain", default=r"C:/Users/adamc/Desktop/workspace/dh_adam/source_code/.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin")
    ap.add_argument("--raw", help="reuse an item dump (skips steps 1-3)")
    ap.add_argument("--apk-base", help="Track B APK to inject the editor into")
    ap.add_argument("--release", help="version name of release/<version>")
    a = ap.parse_args()
    dh = os.path.abspath(a.dh_noam)
    build = os.path.join(REPO, "build")
    tc = a.toolchain
    cxx = os.path.join(tc, "x86_64-w64-mingw32-clang++" + EXE) if os.path.isdir(tc) else "x86_64-w64-mingw32-clang++"
    os.environ["PATH"] = tc + os.pathsep + os.environ["PATH"]

    raw = a.raw or os.path.join(build, "itemdata-raw.json")
    if not a.raw:
        if not a.data:
            sys.exit("--data <game files dir> is needed for the item dump (or --raw)")
        runner = os.path.join(build, "runner")
        if not os.path.exists(os.path.join(runner, "build.ninja")):
            run("cmake", "-S", os.path.join(dh, "port", "decomp-reconstruction"), "-B", runner, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
                "-DCMAKE_CXX_FLAGS=-g", "-DCMAKE_C_FLAGS=-g", "-DDH2_HOST_RUNNER=ON", "-DCMAKE_EXE_LINKER_FLAGS=-lpthread",
                "-DCMAKE_C_COMPILER=" + os.path.join(tc, "x86_64-w64-mingw32-clang" + EXE), "-DCMAKE_CXX_COMPILER=" + cxx,
                "-DCMAKE_RC_COMPILER=" + os.path.join(tc, "x86_64-w64-mingw32-windres" + EXE))
        run("ninja", "dh2_texconv", cwd=runner)
        dump = os.path.join(build, "itemdump", "dh2_itemdump.exe")
        run(sys.executable, "-I", os.path.join(REPO, "itemdump", "build_itemdump.py"), runner, dump)
        work = os.path.join(build, "dump-data", "files")
        if not os.path.isdir(work):
            shutil.copytree(a.data, work, ignore=shutil.ignore_patterns("*.savegame", "*.bak", "*.checkpoint"))
        run(dump, "--data", work, "--out", os.path.join(build, "dump-log"), "--dump", raw)

    out = os.path.join(build, "out")
    web = os.path.join(out, "web")
    shutil.rmtree(web, ignore_errors=True)
    shutil.copytree(os.path.join(REPO, "web"), web)
    run(sys.executable, "-I", os.path.join(HERE, "gen_itemdata.py"), raw, os.path.join(web, "itemdata.js"))
    run(sys.executable, "-I", os.path.join(HERE, "gen_itemdata.py"), raw, os.path.join(out, "itemdata.json"))

    win = os.path.join(out, "windows")
    os.makedirs(win, exist_ok=True)
    run(cxx, "-std=c++17", "-O2", "-municode", "-mwindows", "-static", "-o", os.path.join(win, "dh2-save-editor.exe"),
        os.path.join(REPO, "windows", "dh2_save_editor_win.cpp"), "-lws2_32", "-lshell32", "-lole32")
    shutil.rmtree(os.path.join(win, "web"), ignore_errors=True)
    shutil.copytree(web, os.path.join(win, "web"))

    apk = None
    if a.apk_base:
        apk = os.path.join(out, "dh2-trackb-saveeditor.apk")
        run(sys.executable, "-I", os.path.join(HERE, "build_modded_apk.py"), "--base", a.apk_base, "--web", web, "--out", apk, "--dh-noam", dh)

    if a.release:
        rel = os.path.join(REPO, "release", a.release)
        if os.path.exists(rel):
            sys.exit("release folder exists: " + rel)
        os.makedirs(rel)
        if apk:
            base = os.path.splitext(os.path.basename(a.apk_base))[0]
            shutil.copyfile(apk, os.path.join(rel, base + "-saveeditor.apk"))
        appdir = os.path.join(rel, "DH2-Save-Editor-Windows")
        os.makedirs(appdir)
        shutil.copyfile(os.path.join(win, "dh2-save-editor.exe"), os.path.join(appdir, "dh2-save-editor.exe"))
        shutil.copytree(web, os.path.join(appdir, "web"))
        shutil.copyfile(os.path.join(REPO, "README.txt"), os.path.join(rel, "README.txt"))
        print("release:", rel)
    print("done:", out)


if __name__ == "__main__":
    main()
