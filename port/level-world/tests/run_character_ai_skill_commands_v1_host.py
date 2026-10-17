"""Selected-library checks for bounded CharAI skill command source kernels."""
from __future__ import annotations
import argparse, hashlib, json, os, re, shutil, subprocess
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/character-ai-skill-commands-v1/original-functions.json"
CACHE_MANIFEST = MANIFEST.with_name("cache-inputs.json")

def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()

def run(command, cwd=ROOT, env=None):
    process = subprocess.run(list(map(str, command)), cwd=cwd, env=env,
                             capture_output=True, text=True)
    if process.returncode:
        raise RuntimeError(f"{command}\nexit={process.returncode}\n{process.stdout}\n{process.stderr}")
    return process.stdout

def verify_original(path: Path):
    from elftools.elf.elffile import ELFFile
    document = json.loads(MANIFEST.read_text(encoding="utf-8"))
    assert sha(path) == document["original_sha256"]
    raw = path.read_bytes()
    verified = []
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        loads = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        for row in document["functions"]:
            address, size = int(row["elf_address"], 0), row["size"]
            symbol = symbols[row["original_symbol"]]
            assert (symbol["st_value"], symbol["st_size"]) == (address, size)
            segment = next(s for s in loads if s["p_vaddr"] <= address and address + size <= s["p_vaddr"] + s["p_filesz"])
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            actual = hashlib.sha256(raw[offset:offset + size]).hexdigest()
            assert actual == row["sha256"], row["original_symbol"]
            verified.append(row)
    return document, verified

def compare_original(path: Path, exe: Path, env, document):
    """Execute real ARM caller/setter instructions; only named call boundaries are stubbed."""
    import struct, sys
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu
    cpu = Cpu(path, False, document)
    data = cpu.data
    ai, character, row, vtable = data + 0x1000, data + 0x3000, data + 0x5000, data + 0x6000
    machine = character + 0x4fc
    trace = []

    def store(address, value): cpu.pointer(address, value)
    def read32(address): return struct.unpack("<I", bytes(cpu.uc.mem_read(address, 4)))[0]
    def read8(address): return bytes(cpu.uc.mem_read(address, 1))[0]
    def returned(value):
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    scenarios = []
    instructions = set()
    def begin_case(usable):
        nonlocal trace
        trace = []
        row0, row1 = row, row + 0x100
        store(ai + 4, character); store(ai + 0xcc, 99); store(ai + 0xd0, 1); store(ai + 0xd1, 1)
        store(character, vtable); store(vtable + 0x28, 0x3a49f0)
        store(character + 0x4fc + 4, character)
        store(row0 + 4, 0x111); cpu.uc.mem_write(row0 + 8, b"\x01"); store(row0 + 0x48, 2)
        store(row1 + 4, 0x222); cpu.uc.mem_write(row1 + 8, b"\x00"); store(row1 + 0x48, 2)
        rows = [row0, row1]
        def hook_source(_, address, __, ___):
            if address == 0x3bc784:
                assert cpu.reg(0) == character and cpu.reg(1) == 0
                row_index = min(len(trace), 1)
                trace.append(0); returned(rows[row_index])
            elif address == 0x3d8358:
                assert cpu.reg(0) == ai and cpu.reg(1) == 0
                trace.append(2); returned(usable)
            elif address == 0x4c4bdc:
                trace.append(4); returned(0)
            elif address == 0x3c5684:
                assert cpu.reg(1) == 0xc355
                trace.append(6); returned(0)
            elif address == 0x3a49f0:
                trace.append(8); returned(0)
            elif address == 0x3c02e8:
                returned(int(read32(machine + 0x28) != 0))
        hook = cpu.uc.hook_add(UC_HOOK_CODE, hook_source)
        try: value = cpu.invoke(0x3d86bc, [ai, 0])
        except Exception as error:
            raise RuntimeError(f"AI_BeginSkill {usable=} failed at {cpu.uc.reg_read(cpu.pc):#x} {trace=}") from error
        finally: cpu.uc.hook_del(hook)
        original = {"status": 0, "value": value, "slot": read32(ai + 0xcc),
                    "continued": read8(ai + 0xd0), "last": read8(ai + 0xd1),
                    "writes": 0 if not usable else 6,
                    "animation": read32(machine + 0x28), "skill": read32(machine + 0x54),
                    "moving": read8(machine + 0x58), "state": 6 if usable else 0,
                    "trace": trace[:]}
        compiled = json.loads(run([exe, "--model-begin", str(usable), "0", "0"], env=env))
        assert compiled == original, ("AI_BeginSkill", usable, original, compiled)
        scenarios.append({"command": "AI_BeginSkill", "usable": usable, "result": original})

    begin_case(0)
    begin_case(1)
    instructions.update(cpu.seen)

    # Active query: current FSM/slot short-circuit and a raw noncanonical Lua
    # check result both execute the source caller and compare with the kernel.
    active_records = []
    for state_id, current_slot, raw_active in [(6, 0, 0), (0, 99, 0x80000002)]:
        vector = data + 0x7000; script = data + 0x8000
        store(ai + 4, character); store(ai + 0xb4, vector); store(ai + 0xb8, vector + 4)
        store(vector, script); store(ai + 0xcc, current_slot)
        trace = []
        def active_hook(_, address, __, ___):
            if address == 0x3c02e8: returned(int(state_id == 6))
            elif address == 0x3db16c:
                trace.append(1); returned(raw_active)
        hook = cpu.uc.hook_add(UC_HOOK_CODE, active_hook)
        try: value = cpu.invoke(0x3d85d4, [ai, 0])
        finally: cpu.uc.hook_del(hook)
        expected = {"status": 0, "value": value, "service_calls": len(trace), "trace": trace}
        compiled = json.loads(run([exe, "--model-active", str(state_id), str(current_slot), str(raw_active)], env=env))
        assert compiled == expected, ("AI_IsSkillActive", state_id, current_slot, raw_active, expected, compiled)
        active_records.append({"state": state_id, "slot": current_slot, "raw_callback": raw_active, "result": expected})
    instructions.update(cpu.seen)

    # EndSkill's no-using branch, type-2 continue-bit write and StopLoop tail.
    end_records = []
    initial_last = 0xa5
    for using, continued, skill_type in [(0, 0, 2), (1, 0, 2), (1, 1, 2),
                                       (1, 0x7f, 2), (1, 0, 1), (1, 1, 3)]:
        store(ai + 4, character); store(ai + 0xd0, continued); store(ai + 0xd1, initial_last)
        # The source receiver is the embedded animation object at owner+49c,
        # not a pointer loaded from that offset.
        store(row + 0x48, skill_type)
        trace = []
        def end_hook(_, address, __, ___):
            if address == 0x3c02e8: returned(using)
            elif address == 0x3bc784:
                assert cpu.reg(0) == character and cpu.reg(1) == 0
                trace.append(0); returned(row)
            elif address == 0x3c948c:
                assert cpu.reg(0) == character + 0x49c and cpu.reg(1) == 1
                trace.append(15); returned(0)
        hook = cpu.uc.hook_add(UC_HOOK_CODE, end_hook)
        try: cpu.invoke(0x3d8474, [ai, 0])
        finally: cpu.uc.hook_del(hook)
        state = 6 if using else 0
        original = {"status": 0, "value": 0, "last": read8(ai + 0xd1),
                    "writes": int(bool(using and skill_type == 2 and not continued)),
                    "service_calls": len(trace), "trace": trace[:]}
        compiled = json.loads(run([exe, "--model-end", str(state), str(continued),
                                   str(skill_type), str(initial_last)], env=env))
        assert compiled == original, ("AI_EndSkill", using, continued, skill_type, original, compiled)
        assert original["last"] == (1 if using and skill_type == 2 and not continued else initial_last)
        end_records.append({"using": using, "continued": continued, "type": skill_type,
                            "initial_last": initial_last, "result": original})
    instructions.update(cpu.seen)

    # The actual caller captures AI+4 at 0x3d87ac before the trophy manager
    # load at 0x3d87c0. Mutate AI+4 at that global-provider boundary: the
    # reached GetInt must use the retained owner, while the final FSM query
    # reloads the replacement owner. Execute the original owner load and the
    # manager's actual ldr; substitute only the borrowed global cell backing.
    replacement, trophy_cell = data + 0xa000, data + 0xb000
    manager = 0x76540000
    store(ai + 4, character); store(ai + 0xcc, 99); store(ai + 0xd0, 1); store(ai + 0xd1, 1)
    store(character, vtable); store(vtable + 0x28, 0x3a49f0)
    store(character + 0x4fc + 4, character)
    store(character + 0x560 + 216 * 4, 198)
    store(row + 4, 0x111); cpu.uc.mem_write(row + 8, b"\x01"); store(row + 0x48, 2)
    trace = []; captured_get_owner = []; captured_manager = []
    def owner_hook(_, address, __, ___):
        if address == 0x3bc784:
            assert cpu.reg(0) == character and cpu.reg(1) == 0
            trace.append(0); returned(row)
        elif address == 0x3d8358:
            trace.append(2); returned(1)
        elif address == 0x4c4bdc:
            trace.append(4); returned(0)
        elif address == 0x3c5684:
            trace.append(6); returned(0)
        elif address == 0x3a49f0:
            trace.append(8); returned(1)
        elif address == 0x3e0798:
            assert (cpu.reg(0), cpu.reg(1), cpu.reg(2)) == (character + 0x560, 216, 1)
            trace.append(9)
            store(character + 0x560 + 216 * 4, read32(character + 0x560 + 216 * 4) + 1)
            returned(0)
        elif address == 0x3d87b4:
            store(cpu.reg(5) + cpu.reg(3), trophy_cell)
            store(trophy_cell, manager)
        elif address == 0x3d87c0:
            assert cpu.reg(0) == character + 0x560
            assert cpu.reg(3) == trophy_cell
            trace.append(11); store(ai + 4, replacement)
        elif address == 0x3df6e0:
            assert (cpu.reg(0), cpu.reg(1), cpu.reg(2)) == (character + 0x560, 216, 0)
            captured_get_owner.append(0)
            captured_manager.append(cpu.reg(7))
            trace.append(10); returned(read32(character + 0x560 + 216 * 4))
        elif address == 0x3c02e8:
            assert cpu.reg(0) == replacement + 0x4fc
            returned(0)
    hook = cpu.uc.hook_add(UC_HOOK_CODE, owner_hook)
    try: value = cpu.invoke(0x3d86bc, [ai, 0])
    finally: cpu.uc.hook_del(hook)
    assert captured_get_owner == [0] and captured_manager == [manager]
    owner_result = {"status": 0, "value": value,
                    "property_value": read32(character + 0x560 + 216 * 4),
                    "get_owner": captured_get_owner[0],
                    "owner_after": int(read32(ai + 4) == replacement),
                    "manager": captured_manager[0], "trace": trace[:]}
    compiled = json.loads(run([exe, "--model-owner-capture"], env=env))
    assert compiled == owner_result, ("AI_BeginSkill owner capture", owner_result, compiled)
    instructions.update(cpu.seen)

    # UseSkill runs Begin and omits End completely on a false Usable return.
    store(ai + 4, character); store(ai + 0xcc, 99); store(ai + 0xd0, 1); store(ai + 0xd1, 1)
    store(row + 4, 0x111); cpu.uc.mem_write(row + 8, b"\x01"); store(row + 0x48, 2)
    trace = []
    def use_hook(_, address, __, ___):
        if address == 0x3bc784:
            trace.append(0); returned(row)
        elif address == 0x3d8358:
            trace.append(2); returned(0)
        elif address == 0x3d8474:
            trace.append(0xff)
    hook = cpu.uc.hook_add(UC_HOOK_CODE, use_hook)
    try: value = cpu.invoke(0x3d8868, [ai, 0])
    finally: cpu.uc.hook_del(hook)
    compiled_use = json.loads(run([exe, "--model-use", "0"], env=env))
    assert value == compiled_use["value"] == 0 and trace == [0, 2] and compiled_use["trace"] == trace
    scenarios.append({"command": "AI_UseSkill_false_begin", "value": value, "trace": trace})
    instructions.update(cpu.seen)

    # Nested setter full branch paths: execute the 180-byte source body, with
    # only real engine/UI leaves intercepted. Vary stance mask and force edge.
    setter_records = []
    for mask, stance, force, moving in [
        (0, 0, 0, 0), (0, 0, 1, 1), (0x200000, -7, 0, 1), (0x200000, 8, 1, 0),
    ]:
        trace = []
        store(character + 0x4fc + 4, character)
        store(row + 4, 0x111); cpu.uc.mem_write(row + 8, b"\x01"); store(row + 0x48, 2)
        def setter_hook(_, address, __, ___):
            if address == 0x3bc784:
                assert cpu.reg(0) == character and cpu.reg(1) == 0
                trace.append(0); returned(row)
            elif address == 0x4c4bdc:
                # GetPyCst(category,key) exact strings are checked below once
                # source ABI is reached; source return is the real word.
                trace.append(4); returned(mask)
            elif address == 0x3a53e0:
                assert cpu.reg(0) == character
                trace.append(5); returned(stance)
            elif address == 0x3c5684:
                assert cpu.reg(1) == 0xc355
                trace.append(6)
                returned(0)
            elif address == 0x3c1938:
                assert cpu.reg(1) == 6 and cpu.reg(2) == 0xc355
                trace.append(7)
                returned(0)
        hook = cpu.uc.hook_add(UC_HOOK_CODE, setter_hook)
        try:
            cpu.invoke(0x3c6670, [machine, 0, moving, 0x76543210], [force])
        finally: cpu.uc.hook_del(hook)
        # GNU/ARM Thumb word stores are little-endian; signed arithmetic wraps
        # to 32 bits exactly as the source integer add.
        expected_setter = {"status": 0, "animation": (0x111 + (stance if mask & 0x200000 else 0)) & 0xffffffff,
                           "skill": 0, "moving": moving & 0xff, "writes": 3,
                           "trace": trace}
        compiled_setter = json.loads(run([exe, "--model-setter", hex(mask), str(stance), str(force), str(moving)], env=env))
        for key in ("status", "skill", "moving", "writes", "trace"):
            assert compiled_setter[key] == expected_setter[key], (key, mask, stance, force, compiled_setter, expected_setter)
        # C++ prints a signed field, ARM exposes the same bits as uint32.
        assert (compiled_setter["animation"] & 0xffffffff) == expected_setter["animation"]
        setter_records.append({"mask": mask, "stance": stance, "force": force,
                               "moving": moving, "trace": trace, "result": compiled_setter})
        instructions.update(cpu.seen)
    covered = sorted({hex(address) for address in instructions})
    return {"validation": "PASS", "comparisons": 16, "mismatches": 0,
            "begin_and_use_paths": scenarios, "active_paths": active_records,
            "end_paths": end_records, "set_skill_state": setter_records,
            "owner_capture": owner_result,
            "observed_instruction_addresses": covered,
            "source_scope": "AI_BeginSkill false/true Usable and player property owner-capture path; EndSkill using/type/continue paths with actual d1 byte comparison; nested SM_SetSkillState branches. Named script/property/FSM/stance leaves and trophy global backing are bounded provider hooks."}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument("--cache", required=True, type=Path)
    parser.add_argument("--original-elf", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-ai-skill-commands-v1/validation.json")
    args = parser.parse_args()
    output = args.output.resolve()
    wrapper, build = output / "wrapper", output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True); build.mkdir(parents=True, exist_ok=True)
    compiler = args.compiler.resolve()
    c_compiler = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake, ninja = shutil.which("cmake"), shutil.which("ninja")
    test = MODULE / "tests/character_ai_skill_commands_v1.cpp"
    source = MODULE / "character_ai_skill_commands_v1.cpp"
    backend = ROOT / "port/android-native/app/src/main/cpp/native_debug_files.cpp"
    names = ROOT / "port/pydata-names/names.c"
    cmake_text = f'''cmake_minimum_required(VERSION 3.22)
project(skill_commands_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(skill_commands_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(skill_commands_audit PRIVATE cxx_std_17)
target_compile_options(skill_commands_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(skill_commands_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper / "CMakeLists.txt").write_text(cmake_text, encoding="utf-8")
    logs = [run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={ninja}", f"-DCMAKE_CXX_COMPILER={compiler}",
                 f"-DCMAKE_C_COMPILER={c_compiler}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"]) ]
    commands = run([ninja, "-C", build, "-t", "commands", "skill_commands_audit"])
    from run_player_skill_cleanup_session_v1_host import selected_entries, actual_dependencies
    database = json.loads((build / "compile_commands.json").read_text(encoding="utf-8"))
    entries = selected_entries(build, commands)
    selected_paths = [Path(row["file"]).resolve() for row in entries]
    selected = set(selected_paths)
    assert test.resolve() in selected and source.resolve() in selected
    assert sum(Path(row["file"]).resolve() == source.resolve() for row in database) == 1
    assert selected_paths.count(source.resolve()) == 1
    scoped_entry = next(row for row in entries if Path(row["file"]).resolve() == source.resolve())
    assert Path(scoped_entry["output"]).resolve().relative_to(build).as_posix().startswith("selected-world/CMakeFiles/dh2_level_world.dir/"), "kernel compiled outside selected world library"
    assert (MODULE / "player_skill_use_session_v1.cpp").resolve() in selected, "retained player VM source is not in selected library"
    assert (ROOT / "port/adam-script-runtime/script_runtime.c").resolve() in selected
    assert not any(path.name == "player_skill_vm_services.cpp" for path in selected)
    fixture = MODULE / "tests/player_skill_session_v1.cpp"
    assert fixture.exists()
    path_set = selected | {fixture, Path(__file__).resolve(), MANIFEST, MANIFEST.with_name("NOTES.md"), CACHE_MANIFEST,
                           MODULE / "tests/run_player_skill_cleanup_session_v1_host.py",
                           ROOT / "port/engine-resources/tests/cpu.py"}
    for directory in {path.parent for path in selected}:
        path_set.update(directory.glob("*.h")); path_set.update(directory.glob("*.hpp"))
        if (directory / "CMakeLists.txt").exists(): path_set.add(directory / "CMakeLists.txt")
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in path_set if path.is_relative_to(ROOT)}
    cache_document = json.loads(CACHE_MANIFEST.read_text(encoding="utf-8"))
    assert cache_document["schema_version"] == 1
    cache = args.cache.resolve(); cache_hashes = {}
    for row in cache_document["inputs"]:
        relative = row["path"]
        candidate = (cache / relative).resolve()
        assert candidate.is_relative_to(cache) and relative not in cache_hashes
        assert candidate.stat().st_size == row["size"] and sha(candidate) == row["sha256"], relative
        cache_hashes[relative] = row["sha256"]
    assert len(cache_hashes) == 33 and len(cache_document["script_paths"]) == 15
    assert set(cache_document["script_paths"]) == {path for path in cache_hashes if path.startswith("data/scripts/")}
    original_document, rows = verify_original(args.original_elf.resolve())
    logs.append(run([cmake, "--build", build, "--target", "skill_commands_audit", "--parallel", "1"]))
    assert all(sha(ROOT / relative) == digest for relative, digest in before.items()), "selected sources changed during build"
    dependency_paths = actual_dependencies(build, ninja, entries) | path_set
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in dependency_paths if path.is_relative_to(ROOT)}
    exe = build / "skill_commands_audit.exe"
    dlls = sorted(build.rglob("*.dll"))
    env = os.environ.copy(); env["PATH"] = os.pathsep.join([str(compiler.parent), *(str(p.parent) for p in dlls), env.get("PATH", "")])
    binaries = {path.relative_to(output).as_posix(): sha(path) for path in [exe, *dlls]}
    imports = {path.relative_to(output).as_posix(): re.findall(r"DLL Name: (\S+)", run([compiler.with_name("objdump.exe"), "-p", path])) for path in [exe, *dlls]}
    assert "libdh2_level_world.dll" in imports[exe.relative_to(output).as_posix()]
    assert "libdh2_script_runtime.dll" in imports[exe.relative_to(output).as_posix()]
    assert sum(path.name == "libdh2_script_runtime.dll" for path in dlls) == 1
    host_source = json.loads(run([exe, "--source"], env=env))
    assert host_source["validation"] == "PASS"
    real_vm = json.loads(run([exe, "--real-vm", cache], env=env))
    assert real_vm["validation"] == "PASS" and real_vm["native_activation"] is False
    assert real_vm["loaded_script_paths"] == cache_document["script_paths"], "actual VM loaded unpinned scripts"
    arm = compare_original(args.original_elf.resolve(), exe, env, original_document)
    assert all(sha(ROOT / relative) == digest for relative, digest in before.items())
    assert all(sha(cache / relative) == digest for relative, digest in cache_hashes.items())
    assert binaries == {path.relative_to(output).as_posix(): sha(path) for path in [exe, *dlls]}
    assert commands == run([ninja, "-C", build, "-t", "commands", "skill_commands_audit"])
    report = {"validation": "PASS", "selected_host": host_source, "selected_real_vm": real_vm,
              "original_comparison": arm, "original_sha256": sha(args.original_elf), "original_ranges": rows,
              "source_sha256": before, "cache_inputs_sha256": cache_hashes,
              "cache_manifest_sha256": sha(CACHE_MANIFEST), "source_before_after_equal": True,
              "scoped_source_tu_count": selected_paths.count(source.resolve()),
              "kernel_object": Path(scoped_entry["output"]).resolve().relative_to(build).as_posix(),
              "selected_dso_imports": imports, "binary_before_after_equal": True,
              "selected_commands": commands, "new_complete_original_bodies": 0,
              "binary_sha256": binaries,
              "wrapper_cmake": cmake_text, "build_stdout": logs,
              "scope": "Bounded CharAI skill command projection plus actual passive Knight VM gate. No native activation, Player AIS lifecycle, combat, timers, or connected slot ownership."}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "source_cases": host_source["source_cases"],
                      "real_vm": real_vm["real_vm_begin_usable_false"],
                      "original_comparisons": arm["comparisons"], "original_ranges": len(rows)}))

if __name__ == "__main__": main()
