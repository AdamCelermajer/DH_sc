#!/usr/bin/env python3
"""Convert the compiled Android Java layer to DEX and verify native declarations.

This creates a Java-layer DEX, not a playable APK or an ARM64 game library.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile
import zipfile


def read_native_methods(path):
    """Read native definitions and signatures from a standard little-endian DEX."""
    data = path.read_bytes()
    if not data.startswith(b"dex\n"):
        raise ValueError("Expected a standard DEX file")
    u32 = lambda offset: struct.unpack_from("<I", data, offset)[0]

    def uleb(offset):
        value = shift = 0
        while True:
            byte = data[offset]
            offset += 1
            value |= (byte & 127) << shift
            if not byte & 128:
                return value, offset
            shift += 7
            if shift > 35:
                raise ValueError("Invalid ULEB128")

    strings = []
    for index in range(u32(56)):
        offset = u32(u32(60) + index * 4)
        _, offset = uleb(offset)
        end = data.index(0, offset)
        strings.append(data[offset:end].decode("utf-8", errors="replace"))
    types = [strings[u32(u32(68) + index * 4)] for index in range(u32(64))]
    protos = []
    for index in range(u32(72)):
        offset = u32(76) + index * 12
        return_type = types[u32(offset + 4)]
        params_offset = u32(offset + 8)
        args = []
        if params_offset:
            args = [types[struct.unpack_from("<H", data, params_offset + 4 + i * 2)[0]]
                    for i in range(u32(params_offset))]
        protos.append("(" + "".join(args) + ")" + return_type)
    methods = []
    for index in range(u32(88)):
        class_index, proto_index, name_index = struct.unpack_from("<HHI", data, u32(92) + index * 8)
        methods.append((types[class_index], strings[name_index] + protos[proto_index]))
    native = []
    for index in range(u32(96)):
        offset = u32(u32(100) + index * 32 + 24)
        if not offset:
            continue
        counts = []
        for _ in range(4):
            value, offset = uleb(offset)
            counts.append(value)
        for _ in range(counts[0] + counts[1]):
            _, offset = uleb(offset)
            _, offset = uleb(offset)
        for count in counts[2:]:
            method_index = 0
            for _ in range(count):
                difference, offset = uleb(offset)
                flags, offset = uleb(offset)
                _, offset = uleb(offset)
                method_index += difference
                if flags & 0x100:
                    class_descriptor, method = methods[method_index]
                    native.append({"class_descriptor": class_descriptor, "method": method,
                                   "static": bool(flags & 8)})
    return native


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--android-platform", required=True, type=Path)
    parser.add_argument("--build-tools", required=True, type=Path)
    parser.add_argument("--classes-dir", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--java", default="java")
    parser.add_argument("--min-api", type=int, default=23)
    parser.add_argument("--report", type=Path)
    parser.add_argument("--native-inventory", type=Path,
                        default=Path(__file__).resolve().parents[1] / "reports/java-recovery-native-methods.json")
    args = parser.parse_args()
    platform = args.android_platform.resolve()
    d8 = args.build_tools.resolve() / "lib/d8.jar"
    libraries = [platform / "android.jar", platform / "optional/org.apache.http.legacy.jar"]
    for path in [d8, *libraries, args.native_inventory]:
        if not path.is_file():
            parser.error(f"Missing required input: {path}")
    classes = sorted(args.classes_dir.resolve().rglob("*.class"))
    if not classes:
        parser.error("No compiled classes found; run compile_android_java.py first")
    if args.output.exists() and any(args.output.iterdir()):
        parser.error("DEX output directory must be new or empty")
    args.output.mkdir(parents=True, exist_ok=True)
    command = [args.java, "-cp", str(d8), "com.android.tools.r8.D8", "--release",
               "--min-api", str(args.min_api), "--output", str(args.output.resolve())]
    for library in libraries:
        command.extend(["--lib", str(library)])
    # Windows limits command-line length; D8 accepts one JAR of class files.
    with tempfile.TemporaryDirectory(prefix="dh2-d8-") as temporary:
        class_jar = Path(temporary) / "classes.jar"
        with zipfile.ZipFile(class_jar, "w", compression=zipfile.ZIP_STORED) as bundle:
            for path in classes:
                bundle.write(path, path.relative_to(args.classes_dir.resolve()).as_posix())
        result = subprocess.run(command + [str(class_jar)], capture_output=True, text=True)
    print(result.stdout + result.stderr, end="")
    if result.returncode:
        raise SystemExit(result.returncode)
    dex_files = sorted(args.output.glob("classes*.dex"))
    actual = [row for path in dex_files for row in read_native_methods(path)]
    expected = json.loads(args.native_inventory.read_text())
    contract = lambda row: (row["class_descriptor"], row["method"], row["static"])
    actual_contract = {contract(row) for row in actual}
    expected_contract = {contract(row) for row in expected}
    missing = sorted(expected_contract - actual_contract)
    extra = sorted(actual_contract - expected_contract)
    report = {
        "d8_exit_code": result.returncode, "min_api": args.min_api,
        "input_class_files": len(classes), "dex_files": [
            {"name": path.name, "bytes": path.stat().st_size,
             "sha256": hashlib.sha256(path.read_bytes()).hexdigest()} for path in dex_files],
        "expected_native_declarations": len(expected_contract),
        "rebuilt_native_declarations": len(actual_contract),
        "missing_native_contracts": missing, "unexpected_native_contracts": extra,
        "native_method_contract_matches": not missing and not extra,
        "limitations": ["DEX conversion is not Android runtime or gameplay validation.",
                        "This verifies declared native methods, not native calls into other Java members."]
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    raise SystemExit(0 if not missing and not extra else 1)


if __name__ == "__main__":
    main()
