#!/usr/bin/env python3
"""Build an isolated Android smoke APK from the pinned Irrlicht OGL-ES r6038 tree."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import zipfile


HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
UPSTREAM = HERE / "upstream"
MANIFEST = HERE / "upstream-source-manifest.json"
NOTICE_DIR = HERE / "notices"
BUILD = HERE / "build"
STAGE = BUILD / "stage-r6038"
DEFAULT_BUILD = BUILD
NDK_DEFAULT = REPO.parent / "emulator-test" / "sdk" / "ndk" / "29.0.14206865"
SDK_DEFAULT = REPO.parent / "emulator-test" / "sdk"
ABIS = (("arm64-v8a", "aarch64", "AArch64"),
        ("x86_64", "x86_64", "Advanced Micro Devices X86-64"))
EXPECTED_CURATED_TREE_SHA256 = "d60c411e0186ca75339601219a44dee8211fe08dfbc92ef78d06329a42c17374"
EXPECTED_CURATED_FILE_COUNT = 876
EXPECTED_CURATED_BYTES = 13062914
REQUIRED_LICENSES = (
    "doc/aesGladman.txt", "doc/bzip2-license.txt", "doc/irrlicht-license.txt",
    "doc/jpglib-license.txt", "doc/libpng-license.txt",
    "source/Irrlicht/aesGladman/Readme.txt", "source/Irrlicht/bzip2/LICENSE",
    "source/Irrlicht/bzip2/README", "source/Irrlicht/jpeglib/README",
    "source/Irrlicht/libpng/LICENSE", "source/Irrlicht/libpng/README",
    "source/Irrlicht/zlib/README", "source/Irrlicht/zlib/zlib.h",
)
UPSTREAM_NOTICE_SOURCES = {
    "Irrlicht-LICENSE.txt": "doc/irrlicht-license.txt",
    "IJG-LICENSE.txt": "doc/jpglib-license.txt",
    "IJG-README.txt": "source/Irrlicht/jpeglib/README",
    "libpng-LICENSE.txt": "doc/libpng-license.txt",
    "libpng-UPSTREAM-LICENSE.txt": "source/Irrlicht/libpng/LICENSE",
    "aesGladman-LICENSE.txt": "doc/aesGladman.txt",
    "aesGladman-README.txt": "source/Irrlicht/aesGladman/Readme.txt",
    "bzip2-LICENSE.txt": "doc/bzip2-license.txt",
    "bzip2-UPSTREAM-LICENSE.txt": "source/Irrlicht/bzip2/LICENSE",
    "zlib-README.txt": "source/Irrlicht/zlib/README",
}
SMOKE_MEDIA = (
    "media/irrlichtlogo3.png", "media/dwarf.x", "media/dwarf.jpg",
    "media/axe.jpg", "media/fonthaettenschweiler.bmp", "media/bigfont.png",
)
NATIVE_APP_GLUE_NOTICE = NOTICE_DIR / "native_app_glue-NOTICE.txt"
APACHE2_LICENSE = NOTICE_DIR / "Apache-2.0.txt"
NOTICE_HASHES = {
    "native_app_glue-NOTICE.txt": "396e2d75715e8b4d1029ded55ae92832c8ab9c58148969653d34931248739b95",
    "Apache-2.0.txt": "bb28c48e3e078166e91cfc2b6db7ffebb8a0973b9e23b3df060561292d8d69ec",
}


def android_make_sources() -> set[str]:
    makefile = UPSTREAM / "source/Irrlicht/Android/jni/Android.mk"
    text = makefile.read_text(encoding="utf-8")
    match = re.search(r"(?ms)^LOCAL_SRC_FILES\s*:=\s*\\?\s*\n(.*?)(?=\n\s*\n|\Z)", text)
    if not match:
        raise RuntimeError("could not locate Irrlicht Android.mk LOCAL_SRC_FILES")
    names = set(re.findall(r"\b[\w./-]+\.(?:c|cc|cpp|cxx)\b", match.group(1)))
    if not names:
        raise RuntimeError("Irrlicht Android.mk has no source entries")
    return {f"source/Irrlicht/{name}" for name in names}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def run(command: list[str | Path], *, cwd: Path | None = None,
        log_path: Path | None = None) -> str:
    args = [str(arg) for arg in command]
    result = subprocess.run(args, cwd=cwd, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    output = result.stdout or ""
    if log_path:
        log_path.parent.mkdir(parents=True, exist_ok=True)
        log_path.write_text(output, encoding="utf-8", errors="replace")
    if result.returncode:
        tail = "\n".join(output.splitlines()[-100:])
        raise RuntimeError(f"command failed ({result.returncode}): {args!r}\n{tail}")
    return output


def verify_source() -> dict:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    records = manifest["files"]
    canonical = json.dumps(records, sort_keys=True, separators=(",", ":")).encode()
    if hashlib.sha256(canonical).hexdigest() != manifest["tree_manifest_sha256"]:
        raise RuntimeError("upstream source manifest canonical hash mismatch")
    if manifest.get("schema") != "dh2.irrlicht-svn-curated-snapshot.v1":
        raise RuntimeError("expected the curated Irrlicht r6038 source manifest")
    if manifest["tree_manifest_sha256"] != EXPECTED_CURATED_TREE_SHA256:
        raise RuntimeError("curated Irrlicht source manifest differs from the reviewed snapshot")
    if manifest.get("file_count") != EXPECTED_CURATED_FILE_COUNT:
        raise RuntimeError("curated Irrlicht source file count mismatch")
    if manifest.get("bytes_total") != EXPECTED_CURATED_BYTES:
        raise RuntimeError("curated Irrlicht source byte count mismatch")
    if manifest.get("curation", {}).get("policy_version") != "android-engine-source-only-v1":
        raise RuntimeError("unexpected Irrlicht source curation policy")
    seen: set[str] = set()
    for record in records:
        relative = record["path"]
        if Path(relative).is_absolute() or ".." in Path(relative).parts or "\\" in relative:
            raise RuntimeError(f"unsafe upstream manifest path: {relative!r}")
        path = UPSTREAM.joinpath(*Path(relative).parts)
        seen.add(relative)
        if not path.is_file() or path.stat().st_size != record["bytes"]:
            raise RuntimeError(f"upstream source missing or size mismatch: {relative}")
        if sha256(path) != record["sha256"]:
            raise RuntimeError(f"upstream source hash mismatch: {relative}")
    if len(records) != manifest["file_count"] or sum(row["bytes"] for row in records) != manifest["bytes_total"]:
        raise RuntimeError("curated Irrlicht manifest file/byte totals mismatch")
    actual = {path.relative_to(UPSTREAM).as_posix()
              for path in UPSTREAM.rglob("*") if path.is_file()}
    if actual != seen:
        raise RuntimeError("upstream tree file set differs from pinned manifest")
    required = set(REQUIRED_LICENSES) | set(SMOKE_MEDIA) | {
        "source/Irrlicht/Android/jni/Android.mk",
        "source/Irrlicht/Android/jni/Application.mk",
        "examples/01.HelloWorld_Android/main.cpp",
        "include/irrlicht.h",
    }
    if not required <= seen:
        raise RuntimeError(f"curated source is missing required files: {sorted(required - seen)}")
    shader_paths = {path for path in seen if path.startswith("media/Shaders/")}
    if len(shader_paths) != 22:
        raise RuntimeError(f"expected all 22 upstream shaders; found {len(shader_paths)}")
    compile_sources = android_make_sources()
    if len(compile_sources) != 324 or not compile_sources <= seen:
        missing = sorted(compile_sources - seen)
        raise RuntimeError(f"Android.mk source coverage mismatch ({len(compile_sources)} entries): {missing}")
    if any(Path(path).suffix.lower() in {".exe", ".dll", ".so", ".a", ".lib", ".o", ".obj", ".apk", ".zip"}
           for path in seen):
        raise RuntimeError("curated Irrlicht snapshot unexpectedly includes a prebuilt artifact")
    for path in (NATIVE_APP_GLUE_NOTICE, APACHE2_LICENSE):
        expected_hash = NOTICE_HASHES[path.name]
        if not path.is_file() or sha256(path) != expected_hash:
            raise RuntimeError(f"checked Android native_app_glue notice/license differs: {path}")
    return manifest


def reset_build_directory() -> None:
    resolved = BUILD.resolve()
    default = DEFAULT_BUILD.resolve()
    if resolved == default:
        if resolved.parent != HERE.resolve() or resolved.name != "build":
            raise RuntimeError(f"refusing to reset unexpected build path: {resolved}")
    elif resolved.parent != default or not resolved.name.startswith("variant-"):
        raise RuntimeError(f"refusing to reset unexpected build path: {resolved}")
    if BUILD.exists():
        shutil.rmtree(BUILD)
    BUILD.mkdir()


def make_build_stage(manifest: dict) -> None:
    if STAGE.exists():
        raise RuntimeError(f"staging path unexpectedly exists after build reset: {STAGE}")
    STAGE.mkdir(parents=True)
    for record in manifest["files"]:
        relative = record["path"]
        source = UPSTREAM.joinpath(*Path(relative).parts)
        target = STAGE.joinpath(*Path(relative).parts)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
    example_source = STAGE / "examples/01.HelloWorld_Android/main.cpp"
    example_bytes = example_source.read_bytes()
    window_size = b"param.WindowSize = dimension2d<u32>(300,300);"
    if example_bytes.count(window_size) != 1:
        raise RuntimeError("expected the upstream Android example's 300x300 test window")
    example_source.write_bytes(example_bytes.replace(
        window_size, b"param.WindowSize = dimension2d<u32>(0,0);", 1))
    device_source = STAGE / "source/Irrlicht/Android/CIrrDeviceAndroid.cpp"
    original = device_source.read_bytes()
    old = b"ALooper_pollAll("
    count = original.count(old)
    if count != 2:
        raise RuntimeError(f"expected two upstream ALooper_pollAll calls; found {count}")
    device_source.write_bytes(original.replace(old, b"ALooper_pollOnce("))
    texture_source = STAGE / "source/Irrlicht/COpenGLCoreTexture.h"
    texture_bytes = texture_source.read_bytes()
    desktop_mipmap_hack = (
        b"\t\t\tglEnable(GL_TEXTURE_2D);\t// Hack some ATI cards need this glEnable "
        b"according to https://www.khronos.org/opengl/wiki/Common_Mistakes"
    )
    if texture_bytes.count(desktop_mipmap_hack) != 1:
        raise RuntimeError("expected one upstream desktop mipmap workaround")
    texture_source.write_bytes(texture_bytes.replace(
        desktop_mipmap_hack,
        b"\t\t\t#ifndef IRR_COMPILE_GLES2_COMMON\n"
        b"\t\t\tglEnable(GL_TEXTURE_2D);\t// Desktop/ES1 driver workaround only.\n"
        b"\t\t\t#endif",
        1,
    ))
    driver_source = STAGE / "source/Irrlicht/COGLES2Driver.cpp"
    driver_bytes = driver_source.read_bytes()
    bgra_extension_branch = (
        b"\t\t\tif (queryGLESFeature(COGLESCoreExtensionHandler::IRR_GL_IMG_texture_format_BGRA8888) ||\r\n"
        b"\t\t\t\tqueryGLESFeature(COGLESCoreExtensionHandler::IRR_GL_EXT_texture_format_BGRA8888) ||\r\n"
        b"\t\t\t\tqueryGLESFeature(COGLESCoreExtensionHandler::IRR_GL_APPLE_texture_format_BGRA8888))\r\n"
        b"\t\t\t{\r\n"
        b"\t\t\t\tpixelFormat = GL_BGRA;\r\n"
        b"\t\t\t}\r\n"
        b"\t\t\telse\r\n"
        b"\t\t\t{\r\n"
        b"\t\t\t\tpixelFormat = GL_RGBA;\r\n"
        b"\t\t\t\t*converter = CColorConverter::convert_A8R8G8B8toA8B8G8R8;\r\n"
        b"\t\t\t}\r\n"
    )
    if driver_bytes.count(bgra_extension_branch) != 1:
        raise RuntimeError("expected one GLES2 BGRA extension format-selection branch")
    driver_source.write_bytes(driver_bytes.replace(
        bgra_extension_branch,
        b"\t\t\tpixelFormat = GL_RGBA;\n"
        b"\t\t\t*converter = CColorConverter::convert_A8R8G8B8toA8B8G8R8;\n",
        1,
    ))
    makefile = STAGE / "source/Irrlicht/Android/jni/Android.mk"
    makefile_bytes = makefile.read_bytes()
    flag_line = b"LOCAL_CFLAGS := -Wall -pipe -fno-exceptions -fno-rtti -fstrict-aliasing"
    if makefile_bytes.count(flag_line) != 1:
        raise RuntimeError("expected one Irrlicht LOCAL_CFLAGS line in upstream Android.mk")
    makefile.write_bytes(makefile_bytes.replace(
        flag_line, flag_line + b"\nLOCAL_CONLYFLAGS += -std=gnu89", 1))
    makefile = STAGE / "source/Irrlicht/Android/jni/Android.mk"
    makefile_bytes = makefile.read_bytes()
    copy_rule = (b"all: $(IRRLICHT_LIB_PATH)\r\n"
                 b"$(IRRLICHT_LIB_PATH) : $(TARGET_OUT)/$(IRRLICHT_LIB_NAME)\r\n"
                 b"\tcp $< $@\r\n")
    if makefile_bytes.count(copy_rule) != 1:
        raise RuntimeError("expected the upstream Irrlicht Android.mk archive copy rule")
    makefile.write_bytes(makefile_bytes.replace(copy_rule, b"", 1))
    for relative in (
        "source/Irrlicht/Android/jni/Application.mk",
        "examples/01.HelloWorld_Android/jni/Application.mk",
    ):
        app_mk = STAGE / relative
        app_mk_text = app_mk.read_text(encoding="utf-8")
        if re.search(r"(?m)^APP_STL\s*[:?+]?=", app_mk_text):
            raise RuntimeError(f"upstream unexpectedly sets APP_STL in {relative}")
        app_mk.write_text(app_mk_text.rstrip() + "\nAPP_STL := c++_static\n",
                          encoding="utf-8", newline="\n")
    (STAGE / "lib/Android").mkdir(parents=True, exist_ok=True)


def ndk_build(ndk_build_cmd: Path, project: Path, abi: str, *, output_root: Path,
              linker_flags: str = "") -> str:
    ndk_out = output_root / "obj" / abi
    libs_out = output_root / "native"
    ndk_out.mkdir(parents=True, exist_ok=True)
    libs_out.mkdir(parents=True, exist_ok=True)
    args: list[str | Path] = [ndk_build_cmd, "-C", project,
        "-j4",
        "APP_PLATFORM=android-35", f"APP_ABI={abi}",
        f"NDK_OUT={ndk_out.as_posix()}", f"NDK_LIBS_OUT={libs_out.as_posix()}"]
    if linker_flags:
        args.append(f"APP_LDFLAGS={linker_flags}")
    log_path = output_root / "logs" / f"{abi}-{project.parent.name}-ndk-build.log"
    # ndk-build.cmd is a batch script; invoke it through cmd.exe on Windows.
    cmdline = subprocess.list2cmdline([str(arg) for arg in args])
    output = run(["cmd.exe", "/d", "/c", cmdline],
                 cwd=project, log_path=log_path)
    return output


def verify_elf(readelf: Path, path: Path, expected_machine: str) -> dict:
    header = run([readelf, "-h", path])
    if expected_machine not in header:
        raise RuntimeError(f"unexpected ELF machine for {path}: expected {expected_machine}")
    segments = run([readelf, "-lW", path])
    aligns = []
    for line in segments.splitlines():
        fields = line.split()
        if fields and fields[0] == "LOAD":
            aligns.append(int(fields[-1], 16))
    if not aligns:
        raise RuntimeError(f"no PT_LOAD segments found in {path}")
    if min(aligns) < 16384:
        raise RuntimeError(f"PT_LOAD below 16 KiB in {path}: {aligns}")
    return {"path": path.relative_to(HERE).as_posix(), "bytes": path.stat().st_size,
            "sha256": sha256(path), "machine": expected_machine,
            "pt_load_alignments": aligns}


def map_short_build_drive() -> str:
    """Give old ndk-build's `ar` command short object paths on Windows."""
    for letter in "ZYXWVUTSRQPONMLKJIHGFED":
        if not Path(f"{letter}:/").exists():
            result = subprocess.run(["subst", f"{letter}:", str(BUILD)],
                                    text=True, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, check=False)
            if result.returncode:
                raise RuntimeError(f"could not map temporary build drive {letter}: {result.stdout}")
            return letter
    raise RuntimeError("no free drive letter for the temporary short NDK output path")


def unmap_short_build_drive(letter: str) -> None:
    result = subprocess.run(["subst", f"{letter}:", "/d"], text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            check=False)
    if result.returncode:
        raise RuntimeError(f"could not remove temporary build drive {letter}: {result.stdout}")


def make_apk(sdk: Path, native_libs: dict[str, Path], source_manifest: dict) -> dict:
    build_tools = sdk / "build-tools/37.0.0"
    platform_candidates = [sdk / f"platforms/{name}/android.jar"
                           for name in ("android-37.0.0", "android-37.0", "android-37")]
    platform = next((candidate for candidate in platform_candidates if candidate.is_file()),
                    platform_candidates[0])
    if not platform.is_file() or not build_tools.is_dir():
        raise FileNotFoundError("Android API 37 platform and Build Tools 37.0.0 are required")
    android_manifest = HERE / "smoke/AndroidManifest.xml"
    assets = BUILD / "apk-assets"
    if assets.exists():
        shutil.rmtree(assets)
    media_assets = assets / "media"
    shader_assets = media_assets / "Shaders"
    shader_assets.mkdir(parents=True)
    manifest_paths = {record["path"] for record in source_manifest["files"]}
    asset_paths = set(SMOKE_MEDIA) | {path for path in manifest_paths
                                     if path.startswith("media/Shaders/")}
    if not asset_paths <= manifest_paths:
        raise RuntimeError("APK media/shader assets are not all covered by the source manifest")
    for relative in sorted(asset_paths):
        source = UPSTREAM.joinpath(*Path(relative).parts)
        target = media_assets.joinpath(*Path(relative).parts[1:])
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
    notices_root = assets / "third-party-notices"
    notices_root.mkdir(parents=True, exist_ok=True)
    for source, target_name in (
        (NATIVE_APP_GLUE_NOTICE, "native_app_glue-NOTICE.txt"),
        (APACHE2_LICENSE, "Apache-2.0-LICENSE.txt"),
    ):
        if sha256(source) != NOTICE_HASHES[source.name]:
            raise RuntimeError(f"checked Android legal asset changed: {source}")
        shutil.copy2(source, notices_root / target_name)
    upstream_notice_assets: set[str] = set()
    for target_name, relative in UPSTREAM_NOTICE_SOURCES.items():
        if relative not in manifest_paths:
            raise RuntimeError(f"required upstream notice is not in the curated manifest: {relative}")
        source = UPSTREAM.joinpath(*Path(relative).parts)
        record = next(row for row in source_manifest["files"] if row["path"] == relative)
        if source.stat().st_size != record["bytes"] or sha256(source) != record["sha256"]:
            raise RuntimeError(f"upstream notice differs from the curated manifest: {relative}")
        shutil.copy2(source, notices_root / target_name)
        upstream_notice_assets.add(f"assets/third-party-notices/{target_name}")
    zlib_relative = "source/Irrlicht/zlib/zlib.h"
    zlib_record = next(row for row in source_manifest["files"]
                       if row["path"] == zlib_relative)
    zlib_header = UPSTREAM / zlib_relative
    zlib_bytes = zlib_header.read_bytes()
    if len(zlib_bytes) != zlib_record["bytes"] or sha256(zlib_header) != zlib_record["sha256"]:
        raise RuntimeError("upstream zlib license source differs from the curated manifest")
    if not zlib_bytes.startswith(b"/*") or b"*/" not in zlib_bytes:
        raise RuntimeError("expected zlib.h opening notice block")
    zlib_notice = notices_root / "zlib-LICENSE.txt"
    zlib_notice.write_bytes(zlib_bytes[:zlib_bytes.index(b"*/") + 2])
    upstream_notice_assets.add("assets/third-party-notices/zlib-LICENSE.txt")
    notice_index = notices_root / "NOTICE.txt"
    notice_index.write_text(
        "Third-party notices for the standalone Irrlicht Android smoke package.\n\n"
        "This package includes Irrlicht OGL-ES SVN r6038 and its bundled AES, "
        "bzip2, JPEG, libpng, and zlib sources, plus Android NDK native_app_glue. "
        "The applicable upstream notices, native_app_glue attribution, and "
        "Apache License 2.0 are included in this directory.\n",
        encoding="utf-8")
    base = BUILD / "base.apk"
    run([build_tools / "aapt2.exe", "link", "--manifest", android_manifest,
         "-I", platform, "--min-sdk-version", "26", "--target-sdk-version", "37",
         "-A", assets, "-o", base], log_path=BUILD / "logs/aapt2-link.log")
    unsigned = BUILD / "irrlicht-ogles-r6038-unsigned.apk"
    with zipfile.ZipFile(base, "r") as source, zipfile.ZipFile(unsigned, "w") as target:
        required_notices = {
            "assets/third-party-notices/native_app_glue-NOTICE.txt",
            "assets/third-party-notices/Apache-2.0-LICENSE.txt",
            *upstream_notice_assets,
            "assets/third-party-notices/NOTICE.txt",
        }
        if not required_notices <= set(source.namelist()):
            raise RuntimeError("standalone APK asset staging is missing native_app_glue Apache notices")
        for item in source.infolist():
            target.writestr(item, source.read(item.filename))
        for abi, library in native_libs.items():
            target.write(library, f"lib/{abi}/{library.name}", compress_type=zipfile.ZIP_STORED)
    aligned = BUILD / "irrlicht-ogles-r6038-aligned.apk"
    run([build_tools / "zipalign.exe", "-f", "-P", "16", "4", unsigned, aligned],
        log_path=BUILD / "logs/zipalign.log")
    java_home = Path(os.environ.get("JAVA_HOME", "C:/Program Files/Java/jdk-23"))
    keytool = java_home / "bin/keytool.exe"
    if not keytool.is_file():
        found = shutil.which("keytool")
        if not found:
            raise FileNotFoundError("JDK keytool is required to sign the smoke APK")
        keytool = Path(found)
    keystore = BUILD / "smoke-debug.jks"
    if not keystore.is_file():
        run([keytool, "-genkeypair", "-keystore", keystore, "-storepass", "android",
             "-keypass", "android", "-alias", "debug", "-keyalg", "RSA", "-keysize",
             "2048", "-validity", "3650", "-dname", "CN=Irrlicht r6038 local smoke"],
            log_path=BUILD / "logs/keytool.log")
    signed = BUILD / "irrlicht-ogles-r6038-debug.apk"
    run([build_tools / "apksigner.bat", "sign", "--ks", keystore,
         "--ks-key-alias", "debug", "--ks-pass", "pass:android",
         "--key-pass", "pass:android", "--out", signed, aligned],
        log_path=BUILD / "logs/apksigner.log")
    run([build_tools / "apksigner.bat", "verify", "--verbose", signed],
        log_path=BUILD / "logs/apksigner-verify.log")
    run([build_tools / "zipalign.exe", "-c", "-P", "16", "4", signed],
        log_path=BUILD / "logs/zipalign-verify.log")
    return {"path": signed.relative_to(HERE).as_posix(), "bytes": signed.stat().st_size,
            "sha256": sha256(signed), "min_sdk": 26, "target_sdk": 37,
            "zipalign_page_size": 16384, "signed": True,
            "asset_file_count": sum(1 for path in assets.rglob("*") if path.is_file()),
            "notice_assets": sorted(required_notices)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--ndk", type=Path, default=NDK_DEFAULT)
    parser.add_argument("--sdk", type=Path, default=SDK_DEFAULT)
    parser.add_argument("--no-apk", action="store_true",
                        help="build/verify native libraries only; skip APK packaging")
    parser.add_argument("--package-only", action="store_true",
                        help="package existing build/native outputs without rebuilding")
    parser.add_argument("--verify-source-only", action="store_true",
                        help="verify the curated source and exit without creating build outputs")
    parser.add_argument("--build-dir", type=Path,
                        help="isolated build output under port/irrlicht-android/build/variant-*")
    args = parser.parse_args()
    if args.package_only and args.no_apk:
        parser.error("--package-only cannot be combined with --no-apk")
    if args.verify_source_only and (args.package_only or args.no_apk):
        parser.error("--verify-source-only cannot be combined with build/package options")
    manifest = verify_source()
    if args.verify_source_only:
        print(json.dumps({
            "source": {key: manifest[key] for key in (
                "upstream", "svn_revision", "git_svn_commit", "git_svn_id",
                "upstream_version", "file_count", "bytes_total", "tree_manifest_sha256")},
            "curation": manifest["curation"],
            "android_mk_source_count": len(android_make_sources()),
            "shader_count": 22,
            "apk_build_started": False,
            "emulator_used": False,
        }, indent=2))
        return 0
    if args.build_dir is not None:
        requested = args.build_dir
        if not requested.is_absolute():
            requested = REPO / requested
        resolved = requested.resolve()
        if resolved.parent != DEFAULT_BUILD.resolve() or not resolved.name.startswith("variant-"):
            parser.error("--build-dir must be a direct variant-* child of port/irrlicht-android/build")
        global BUILD, STAGE
        BUILD = resolved
        STAGE = BUILD / "stage-r6038"
    if not args.package_only:
        reset_build_directory()
        make_build_stage(manifest)

    ndk = args.ndk.resolve()
    ndk_build_cmd = ndk / "ndk-build.cmd"
    llvm_bin = ndk / "toolchains/llvm/prebuilt/windows-x86_64/bin"
    readelf = llvm_bin / "llvm-readelf.exe"
    if not ndk_build_cmd.is_file() or not readelf.is_file():
        raise FileNotFoundError(f"NDK r29 tools missing under {ndk}")
    libraries: dict[str, Path] = {}
    native_report = {}
    if args.package_only:
        for abi, _, machine in ABIS:
            static_copy = BUILD / "static" / abi / "libIrrlicht.a"
            library = BUILD / "native" / abi / "libHelloWorldMobile.so"
            if not static_copy.is_file() or not library.is_file():
                raise FileNotFoundError(f"missing existing native outputs for {abi}; build first")
            libraries[abi] = library
            native_report[abi] = {
                "static_library": {"path": static_copy.relative_to(HERE).as_posix(),
                                   "bytes": static_copy.stat().st_size,
                                   "sha256": sha256(static_copy)},
                "shared_library": verify_elf(readelf, library, machine),
                "ndk_target": "android-35",
            }
    else:
        mapped_drive = map_short_build_drive()
        try:
            short_output_root = Path(f"{mapped_drive}:/")
            for abi, _, machine in ABIS:
                ndk_build(ndk_build_cmd, STAGE / "source/Irrlicht/Android", abi,
                          output_root=short_output_root)
                # Collect the archive from NDK_OUT; the upstream custom copy rule
                # uses Unix cp, which Windows ndk-build cannot execute.
                static_lib = short_output_root / "obj" / abi / "local" / abi / "libIrrlicht.a"
                if not static_lib.is_file():
                    raise RuntimeError(f"Irrlicht static archive missing after NDK build: {static_lib}")
                static_copy = BUILD / "static" / abi / "libIrrlicht.a"
                static_copy.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(static_lib, static_copy)
                shutil.copyfile(static_lib, STAGE / "lib/Android/libIrrlicht.a")
                ndk_build(ndk_build_cmd, STAGE / "examples/01.HelloWorld_Android", abi,
                          output_root=short_output_root,
                          linker_flags="-Wl,-z,max-page-size=16384 -Wl,-z,common-page-size=16384")
                library = BUILD / "native" / abi / "libHelloWorldMobile.so"
                if not library.is_file():
                    raise RuntimeError(f"NativeActivity shared library missing: {library}")
                libraries[abi] = library
                native_report[abi] = {
                    "static_library": {"path": static_copy.relative_to(HERE).as_posix(),
                                       "bytes": static_copy.stat().st_size,
                                       "sha256": sha256(static_copy)},
                    "shared_library": verify_elf(readelf, library, machine),
                    "ndk_target": "android-35",
                }
        finally:
            unmap_short_build_drive(mapped_drive)

    apk_report = None if args.no_apk else make_apk(args.sdk.resolve(), libraries, manifest)
    report = {
        "source": {key: manifest[key] for key in (
            "upstream", "svn_revision", "git_svn_commit", "git_svn_id",
            "upstream_version", "file_count", "bytes_total", "tree_manifest_sha256")},
        "curation": manifest["curation"],
        "ndk": ndk.as_posix(), "ndk_revision": "29.0.14206865",
        "native_api": 35, "apk_target_api": 37, "min_api": 26,
        "source_patches": [
            {"file": "examples/01.HelloWorld_Android/main.cpp",
             "change": "Use WindowSize 0x0 in the generated smoke build so Irrlicht selects the full Android surface instead of the upstream sample's 300x300 test window.",
             "upstream_tree_modified": False},
            {"file": "source/Irrlicht/Android/CIrrDeviceAndroid.cpp",
             "change": "Replace two ALooper_pollAll calls with ALooper_pollOnce for NDK r29/API35; the NDK headers mark pollAll unavailable.",
             "upstream_tree_modified": False},
            {"file": "source/Irrlicht/COpenGLCoreTexture.h",
             "change": "Skip the fixed-function glEnable(GL_TEXTURE_2D) ATI mipmap workaround in the generated GLES2 staging copy; retain it for desktop GL and GLES1.",
             "upstream_tree_modified": False},
            {"file": "source/Irrlicht/COGLES2Driver.cpp",
             "change": "Force A8R8G8B8 sources through the existing RGBA byte-order converter in GLES2 instead of selecting the advertised BGRA extension format; SwiftShader rejected that format for mipmap generation.",
             "upstream_tree_modified": False},
            {"file": "source/Irrlicht/Android/jni/Android.mk",
             "change": "Add LOCAL_CONLYFLAGS += -std=gnu89 for bundled legacy zlib C sources; C++ language mode is unchanged.",
             "upstream_tree_modified": False}],
        "build_workarounds": [
            "Remove the upstream Android.mk custom all/cp copy rule from the staging copy because Windows ndk-build cannot execute Unix cp. Collect libIrrlicht.a directly from NDK_OUT and stage it at lib/Android for the upstream example's prebuilt-static-library input.",
            "Set APP_STL := c++_static in the staged Irrlicht and HelloWorld Application.mk files; the upstream files omit APP_STL and NDK r29's legacy system STL cannot satisfy sized-delete symbols emitted by the current compiler.",
            "Build with ndk-build -j4 to parallelize compilation on Windows."],
        "compatibility_flags": [],
        "ndk_object_output_drive": "temporary subst mapping removed after build",
        "abi": native_report, "apk": apk_report,
        "emulator_used": False,
    }
    (BUILD / "build-report.json").write_text(json.dumps(report, indent=2) + "\n",
                                             encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as error:
        print(f"ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
