#!/usr/bin/env python3
"""Make "Track B APK + DH2 Save Editor": inject the editor into an unmodified Track B APK. dh_noam is only read.

usage: py -3 -I tools/build_modded_apk.py --base <dh2-trackb-*.apk> --web <built web folder> --out <apk>
                                          [--dh-noam <dh_noam/source_code>] [--min-sdk 26 --target-sdk 35 --version-code 102 --version-name 1.0.2]

What is added to the base APK (nothing else changes):
  * classes<N+1>.dex: android/src (com.dh2saveeditor.SaveEditorActivity, the file bridge of the WebView UI);
  * assets/dh2editor/: the shared UI (web/ + generated itemdata.js);
  * AndroidManifest.xml: the base manifest with the "DH2 Save Editor" launcher activity (android/editor-activity.xml).
The manifest is rebuilt the way Track B builds it (tools/trackb/java/build_apk.py: the source manifest of
port/android-trackb + package/version/uses-sdk, aapt2 link against the imported res/ whose ids are pinned by public.xml).
As a check, the same link WITHOUT the editor activity must give exactly the base APK's AndroidManifest.xml bytes;
otherwise the script stops (the base APK was built from other sources or flags).
Then zipalign (16 KiB, as Track B) and apksigner with ~/.android/debug.keystore, the key of the Track B APKs, so the
result installs as an update over the installed game and keeps its data.
"""
import argparse, os, shutil, subprocess, sys, zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
PKG = "com.gameloft.android.GAND.GloftD2SS"
EXE = ".exe" if os.name == "nt" else ""
BAT = ".bat" if os.name == "nt" else ""


def run(*a, **k):
    print(">", " ".join(str(x) for x in a), flush=True)
    subprocess.check_call([str(x) for x in a], **k)


def merged_manifest(src, a, editor):
    # tools/trackb/java/build_apk.py: what the Gradle manifest merger does
    m = open(src, encoding="utf-8").read()
    m = m.replace('<manifest xmlns:android="http://schemas.android.com/apk/res/android"',
                  '<manifest xmlns:android="http://schemas.android.com/apk/res/android"\n    package="%s"\n'
                  '    android:versionCode="%d"\n    android:versionName="%s"' % (PKG, a.version_code, a.version_name), 1)
    m = m.replace("</manifest>", '    <uses-sdk android:minSdkVersion="%d" android:targetSdkVersion="%d"/>\n</manifest>' % (a.min_sdk, a.target_sdk))
    if editor:
        frag = open(os.path.join(REPO, "android", "editor-activity.xml"), encoding="utf-8").read()
        if m.count("</application>") != 1:
            sys.exit("unexpected manifest: no single </application>")
        m = m.replace("</application>", frag + "    </application>")
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", required=True)
    ap.add_argument("--web", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--dh-noam", default=os.path.join(REPO, "..", "dh_noam", "source_code"))
    ap.add_argument("--sdk", default=os.environ.get("ANDROID_HOME"))
    ap.add_argument("--keystore", default=os.path.join(os.path.expanduser("~"), ".android", "debug.keystore"))
    ap.add_argument("--min-sdk", type=int, default=26)
    ap.add_argument("--target-sdk", type=int, default=35)
    ap.add_argument("--version-code", type=int, default=102)
    ap.add_argument("--version-name", default="1.0.2")
    a = ap.parse_args()
    dh = os.path.abspath(a.dh_noam)
    sdk = a.sdk or os.path.join(dh, "build", "sdk")
    bt, plat = sdk + "/build-tools/37.0.0", sdk + "/platforms/android-37.0"
    proj = os.path.join(dh, "port", "android-trackb", "app", "src", "main")
    if not os.path.isdir(os.path.join(proj, "res")):
        sys.exit("missing %s/res: run dh_noam's tools/trackb/java/import_resources.py there once (Track B build step)" % proj)
    work = os.path.join(REPO, "build", "apk-work")
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)

    # 1. manifests
    flat = os.path.join(work, "res.zip")
    run(bt + "/aapt2" + EXE, "compile", "--no-crunch", "--dir", os.path.join(proj, "res"), "-o", flat)
    linked = {}
    for name, editor in (("base", False), ("editor", True)):
        mf = os.path.join(work, "AndroidManifest-%s.xml" % name)
        open(mf, "w", encoding="utf-8").write(merged_manifest(os.path.join(proj, "AndroidManifest.xml"), a, editor))
        out = os.path.join(work, "link-%s.apk" % name)
        run(bt + "/aapt2" + EXE, "link", "-o", out, "-I", plat + "/android.jar", "--manifest", mf, "--min-sdk-version", a.min_sdk,
            "--target-sdk-version", a.target_sdk, "--version-code", a.version_code, "--version-name", a.version_name, "--no-auto-version", flat)
        linked[name] = zipfile.ZipFile(out).read("AndroidManifest.xml")
    base = zipfile.ZipFile(a.base)
    if linked["base"] != base.read("AndroidManifest.xml"):
        sys.exit("the rebuilt Track B manifest differs from the base APK's: wrong --dh-noam checkout or sdk/version flags")
    print("base manifest reproduced byte for byte; editor manifest", len(linked["editor"]), "bytes")

    # 2. the editor's dex
    cls = os.path.join(work, "classes")
    os.makedirs(cls)
    srcs = [os.path.join(dp, f) for dp, _, fs in os.walk(os.path.join(REPO, "android", "src")) for f in fs if f.endswith(".java")]
    run("javac", "-nowarn", "-encoding", "UTF-8", "--release", "17", "-cp", plat + "/android.jar", "-d", cls, *srcs)
    dexdir = os.path.join(work, "dex")
    os.makedirs(dexdir)
    classes = [os.path.join(dp, f) for dp, _, fs in os.walk(cls) for f in fs if f.endswith(".class")]
    run(bt + "/d8" + BAT, "--release", "--min-api", a.min_sdk, "--lib", plat + "/android.jar", "--output", dexdir, *classes)
    names = base.namelist()
    n = 2
    while "classes%d.dex" % n in names:
        n += 1
    dexname = "classes%d.dex" % n

    # 3. the APK: every base entry as it was (same compression), the new manifest, dex and UI
    raw = os.path.join(work, "raw.apk")
    with zipfile.ZipFile(raw, "w", zipfile.ZIP_DEFLATED) as zo:
        for i in base.infolist():
            if i.filename.startswith("META-INF/") or i.filename.startswith("assets/dh2editor/"):
                continue
            data = linked["editor"] if i.filename == "AndroidManifest.xml" else base.read(i.filename)
            zi = zipfile.ZipInfo(i.filename, i.date_time)
            zi.compress_type = i.compress_type
            zi.external_attr = i.external_attr
            zo.writestr(zi, data)
        zo.write(os.path.join(dexdir, "classes.dex"), dexname)
        for dp, _, fs in os.walk(a.web):
            for f in sorted(fs):
                p = os.path.join(dp, f)
                zo.write(p, "assets/dh2editor/" + os.path.relpath(p, a.web).replace("\\", "/"))
    aligned = os.path.join(work, "aligned.apk")
    run(bt + "/zipalign" + EXE, "-p", "-f", "16384", raw, aligned)
    os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
    run(bt + "/apksigner" + BAT, "sign", "--ks", a.keystore, "--ks-pass", "pass:android", "--key-pass", "pass:android",
        "--ks-key-alias", "androiddebugkey", "--out", a.out, aligned)
    run(bt + "/apksigner" + BAT, "verify", a.out)
    for f in (raw, aligned):
        os.remove(f)
    idsig = a.out + ".idsig"
    if os.path.exists(idsig):
        os.remove(idsig)   # v4 signature file, only for incremental adb installs
    print("wrote", a.out, os.path.getsize(a.out), "bytes (", dexname, "+ assets/dh2editor )")


if __name__ == "__main__":
    main()
