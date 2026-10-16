#!/usr/bin/env python3
"""One command: libdh2.so (arm64-v8a Release) -> two signed APKs (and optionally install them).

usage: py -3 tools/trackb/android/build_release.py [--skip-native] [--only origdex|source] [--install [--serial S]] [--apk ORIG.apk]

Outputs (all under build/own-ab, never tracked):
  build/own-ab/out/dh2-trackb-origdex.apk   original v1.0.2 classes.dex + resources + our libdh2.so (+ DataSetup launcher dex)
  build/own-ab/out/dh2-trackb-source.apk    fully source-built: port/android-trackb Java + our libdh2.so + resources imported from the APK
  build/own-ab/out/libdh2-unstripped.so     for symbolising (llvm-addr2line)
Steps: cmake/ninja dh2 in build/own-ab-arm64 (Release, no DH2_BOOT_DEBUG, abort stubs OFF) -> llvm-strip -> libStormGLOFT stub
       -> tools/trackb/java/build_apk.py (source APK) -> original APK repack (manifest from the same build) -> zipalign 16K -> apksigner (debug key).
Prerequisite: your own Dungeon-Hunter-2-HD-v1-0-2.apk (--apk or the default path) for resources; res/ is imported once by
tools/trackb/java/import_resources.py (this script runs it when port/android-trackb/app/src/main/res is missing).
"""
import argparse, os, shutil, subprocess, sys, zipfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
SDK = os.environ.get("ANDROID_HOME", r"C:/Users/nacworkstation/Documents/Codex/2026-10-09/find/work/tools/android-sdk")
NDK = SDK + "/ndk/29.0.14206865"
NDKB = NDK + "/toolchains/llvm/prebuilt/windows-x86_64/bin"
BT = SDK + "/build-tools/37.0.0"
PLAT = SDK + "/platforms/android-37.0"
ADB = SDK + "/platform-tools/adb" + (".exe" if os.name == "nt" else "")
EXE = ".exe" if os.name == "nt" else ""
BAT = ".bat" if os.name == "nt" else ""
PKG = "com.gameloft.android.GAND.GloftD2SS"
OUTROOT = os.path.join(ROOT, "build", "own-ab")
OUT = os.path.join(OUTROOT, "out")
BUILD = os.path.join(ROOT, "build", "own-ab-arm64")
PROJ = os.path.join(ROOT, "port", "android-trackb", "app", "src", "main")
DEFAULT_APK = r"C:/Users/nacworkstation/Documents/Projects/Dungeon Hunter 2 Recomp/Dungeon-Hunter-2-HD-v1-0-2.apk"


def run(*a, **k):
    print(">", " ".join(str(x) for x in a), flush=True)
    subprocess.check_call([str(x) for x in a], **k)


def keystore():
    for ks in (os.path.join(OUTROOT, "debug.keystore"), os.path.join(os.path.expanduser("~"), ".android", "debug.keystore")):
        if os.path.exists(ks):
            return ks
    ks = os.path.join(OUTROOT, "debug.keystore")
    run("keytool", "-genkeypair", "-keystore", ks, "-storepass", "android", "-keypass", "android", "-alias", "androiddebugkey",
        "-keyalg", "RSA", "-keysize", "2048", "-validity", "10000", "-dname", "CN=Android Debug,O=Android,C=US")
    return ks


def sign(raw, final):
    aligned = final + ".aligned"
    if os.path.exists(aligned):
        os.remove(aligned)
    if os.path.exists(final + ".idsig"):
        os.remove(final + ".idsig")
    run(BT + "/zipalign" + EXE, "-p", "-f", "16384", raw, aligned)
    run(BT + "/apksigner" + BAT, "sign", "--ks", keystore(), "--ks-pass", "pass:android", "--key-pass", "pass:android",
        "--ks-key-alias", "androiddebugkey", "--out", final, aligned)
    os.remove(aligned)
    run(BT + "/apksigner" + BAT, "verify", final)


def build_native():
    if not os.path.exists(os.path.join(BUILD, "build.ninja")):
        run("cmake", "-S", os.path.join(ROOT, "port", "decomp-reconstruction"), "-B", BUILD, "-G", "Ninja",
            "-DCMAKE_TOOLCHAIN_FILE=" + NDK + "/build/cmake/android.toolchain.cmake", "-DANDROID_ABI=arm64-v8a",
            "-DANDROID_PLATFORM=android-26", "-DCMAKE_BUILD_TYPE=Release", "-DDH2_BOOT_DEBUG=OFF", "-DDH2_ANDROID_ABORT_STUBS=OFF")
    run("ninja", "dh2", cwd=BUILD)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--skip-native", action="store_true", help="reuse build/own-ab-arm64/libdh2.so")
    ap.add_argument("--only", choices=("origdex", "source"))
    ap.add_argument("--apk", default=DEFAULT_APK, help="your original Dungeon-Hunter-2-HD-v1-0-2.apk")
    ap.add_argument("--install", action="store_true", help="adb install -r -g the origdex APK (the source one with --only source)")
    ap.add_argument("--serial")
    ap.add_argument("--min-sdk", type=int, default=26)
    ap.add_argument("--target-sdk", type=int, default=28)
    a = ap.parse_args()
    os.makedirs(OUT, exist_ok=True)

    # 1. native library
    if not a.skip_native:
        build_native()
    so = os.path.join(BUILD, "libdh2.so")
    if not os.path.exists(so):
        sys.exit("missing " + so)
    shutil.copyfile(so, os.path.join(OUT, "libdh2-unstripped.so"))
    stripped = os.path.join(OUTROOT, "libDungeonHunter2.so")
    run(NDKB + "/llvm-strip" + EXE, "--strip-unneeded", "-o", stripped, so)
    storm = os.path.join(OUTROOT, "libStormGLOFT.so")
    run(NDKB + "/aarch64-linux-android26-clang" + (".cmd" if os.name == "nt" else ""), "-shared", "-llog", "-Wl,-z,max-page-size=16384",
        "-o", storm, os.path.join(ROOT, "tools", "trackb", "boot", "storm_stub.c"))

    # 2. source APK (resources imported once from the user's APK; libs placed in the git-ignored jniLibs)
    if not os.path.isdir(os.path.join(PROJ, "res")):
        run(sys.executable, os.path.join(ROOT, "tools", "trackb", "java", "import_resources.py"), "--apk", a.apk, "--so", so)
    jl = os.path.join(PROJ, "jniLibs", "arm64-v8a")
    os.makedirs(jl, exist_ok=True)
    shutil.copyfile(stripped, os.path.join(jl, "libDungeonHunter2.so"))
    shutil.copyfile(storm, os.path.join(jl, "libStormGLOFT.so"))
    jout = os.path.join(OUTROOT, "java-out")
    run(sys.executable, os.path.join(ROOT, "tools", "trackb", "java", "build_apk.py"), "--out", jout,
        "--min-sdk", a.min_sdk, "--target-sdk", a.target_sdk)
    if a.only != "origdex":
        sign(os.path.join(jout, "dh2-java-raw.apk"), os.path.join(OUT, "dh2-trackb-source.apk"))   # same key as the origdex APK

    # 3. original-dex APK: the original APK's dex/resources, the manifest compiled from the same source manifest (ids are
    #    pinned identical to the original's resources.arsc), our libs, and DataSetup (the first-run data unpacker) as classes2.dex
    if a.only != "source":
        d8dir = os.path.join(OUTROOT, "datasetup-dex")
        shutil.rmtree(d8dir, ignore_errors=True)
        os.makedirs(d8dir)
        cls = os.path.join(jout, "classes")
        ds = os.path.join(cls, "com", "gameloft", "android", "GAND", "GloftD2SS")
        ds_classes = [os.path.join(ds, f) for f in os.listdir(ds) if f == "DataSetup.class" or f.startswith("DataSetup$")]
        run(BT + "/d8" + BAT, "--min-api", a.min_sdk, "--lib", PLAT + "/android.jar", "--classpath", os.path.join(jout, "classes.jar"),
            "--output", d8dir, *ds_classes)
        manifest = zipfile.ZipFile(os.path.join(jout, "base.apk")).read("AndroidManifest.xml")
        raw = os.path.join(OUTROOT, "dh2-origdex-raw.apk")
        zi = zipfile.ZipFile(a.apk)
        with zipfile.ZipFile(raw, "w", zipfile.ZIP_DEFLATED) as zo:
            for i in zi.infolist():
                n = i.filename
                if n.startswith("META-INF/") or n.startswith("lib/") or n == "AndroidManifest.xml":
                    continue
                zo.writestr(n, zi.read(n), zipfile.ZIP_STORED if n == "resources.arsc" else zipfile.ZIP_DEFLATED)
            zo.writestr("AndroidManifest.xml", manifest)
            zo.write(os.path.join(d8dir, "classes.dex"), "classes2.dex")
            zo.write(stripped, "lib/arm64-v8a/libDungeonHunter2.so", compress_type=zipfile.ZIP_STORED)
            zo.write(storm, "lib/arm64-v8a/libStormGLOFT.so", compress_type=zipfile.ZIP_STORED)
        sign(raw, os.path.join(OUT, "dh2-trackb-origdex.apk"))

    for f in sorted(os.listdir(OUT)):
        print("%10d  %s" % (os.path.getsize(os.path.join(OUT, f)), os.path.join("build/own-ab/out", f)))

    # 4. optional install
    if a.install:
        which = "dh2-trackb-source.apk" if a.only == "source" else "dh2-trackb-origdex.apk"
        s = ["-s", a.serial] if a.serial else []
        run(ADB, *s, "install", "-r", "-g", os.path.join(OUT, which))
        # Android 10+ refuses TelephonyManager.getDeviceId to unprivileged apps; the original dex does not catch that
        # (the source build does), so the origdex build needs this app-op once.
        run(ADB, *s, "shell", "appops", "set", PKG, "USE_ICC_AUTH_WITH_DEVICE_IDENTIFIER", "allow")
        print("installed", which, "- data: see docs/ANDROID-RUN.md (copy the cache zip to the device, first start unpacks it)")


if __name__ == "__main__":
    main()
