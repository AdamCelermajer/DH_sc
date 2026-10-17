"""Selected V4 and text DSOs + borrowed query gate using genuine local cache.

Only controlled Debug transport/English pack selection are host fixtures.
No copied cache/APK/ELF, native renderer, gameplay/profile/loot activation claim.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

ROOT=Path(__file__).resolve().parents[3]
REF=ROOT/"port/game-data/reference/adam-791e961-inventory-binding"
TARGETS=("inventory_binding_text_queries_v1","inventory_binding_format_gold_v1","inventory_binding_varargs_gold_v1")
def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    for name in ("cache","build","compiler","report"):
        ap.add_argument("--"+name,type=Path,required=True)
    args=ap.parse_args();args.build.mkdir(parents=True,exist_ok=True)
    assert "player_equipment_queries_live_v1.cpp" in (ROOT/"port/game-data/CMakeLists.txt").read_text(), "Query must be centrally selected; no candidate staging in this gate"
    env=os.environ.copy();env["PATH"]=str(args.compiler.parent)+os.pathsep+env.get("PATH","")
    logs=[]
    def run(command):
        result=subprocess.run([str(x)for x in command],cwd=ROOT,env=env,capture_output=True,text=True)
        logs.append({"command":[str(x)for x in command],"returncode":result.returncode,"stdout":result.stdout,"stderr":result.stderr})
        (args.build/"gate-command-outputs.json").write_text(json.dumps(logs,indent=2)+"\n")
        if result.returncode:
            raise RuntimeError(result.stderr or result.stdout)
        return result.stdout
    cmake=ROOT/"port/engine-ui/tests/inventory-binding-v1/CMakeLists.txt"
    run(["cmake","-S",cmake.parent,"-B",args.build,"-G","Ninja","-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON","-DCMAKE_CXX_COMPILER="+str(args.compiler),
         "-DCMAKE_C_COMPILER="+str(args.compiler.with_name("gcc.exe"))])
    database=json.loads((args.build/"compile_commands.json").read_text())
    names=("dh2_game_data","dh2_inventory_text_v1",*TARGETS)
    reached=[row for row in database if any("CMakeFiles/"+name+".dir" in row["command"].replace("\\","/")for name in names)]
    selected=[row for row in reached if "CMakeFiles/dh2_game_data.dir" in row["command"].replace("\\","/")]
    assert sum(Path(row["file"]).name=="player_equipment_queries_live_v1.cpp"for row in selected)==1
    pending=[Path(row["file"]).resolve()for row in reached];sources=set()
    while pending:
        path=pending.pop()
        if path in sources:
            continue
        path.relative_to(ROOT);sources.add(path)
        for include in re.findall(r'^\s*#\s*include\s*"([^\"]+)"',path.read_text(),re.MULTILINE):
            dep=(path.parent/include).resolve()
            if dep.is_file():
                pending.append(dep)
    sources.update((Path(__file__).resolve(),cmake,ROOT/"port/game-data/CMakeLists.txt",ROOT/"port/engine-ui/inventory_text_v1.cmake"))
    before={p.relative_to(ROOT).as_posix():digest(p)for p in sorted(sources)}
    run(["cmake","--build",args.build,"--parallel","1","--target",*TARGETS])
    library=args.build/"game-data/libdh2_game_data.dll";text_library=args.build/"libdh2_inventory_text_v1.dll"
    assert library.is_file()and text_library.is_file(),"Gate requires both selected shared libraries"
    env["PATH"]=str(library.parent)+os.pathsep+str(text_library.parent)+os.pathsep+env["PATH"]
    objdump=args.compiler.with_name("objdump.exe")
    def dependencies(path):
        return re.findall(r'DLL Name:\s*([^\s]+)',run([objdump,"-p",path]))
    text_dependencies=dependencies(text_library)
    assert "libdh2_game_data.dll"in text_dependencies,"Text DSO must use the existing game-data DSO"
    cases=( (TARGETS[0],[args.cache,REF/"query-fixtures.bin"]),
            (TARGETS[1],[REF/"format-gold.bin"]), (TARGETS[2],[REF/"varargs-fixtures.bin"]) )
    tests=[];text_files=[]
    for name,inputs in cases:
        command=[args.build/(name+".exe"),*inputs];result=json.loads(run(command))
        assert result["validation"]=="PASS"and result["mismatches"]==0
        imported=dependencies(command[0])
        assert "libdh2_inventory_text_v1.dll"in imported,"Tests must execute the selected text DSO"
        text_files.extend(result.pop("text_files",[]))
        tests.append({"command":[str(x)for x in command],"result":result,
                      "executable_sha256":digest(command[0]),"dll_dependencies":imported})
    after={p.relative_to(ROOT).as_posix():digest(p)for p in sources};assert before==after,"Source changed during gate"
    cache_inputs=[args.cache/(prefix+suffix)for prefix in ("loot_table","character_properties","item_powers","common_text")
                  for suffix in ("_pyarray.bin","_pyarraynames.bin","_pystructnames.bin")]
    cache_inputs.extend([args.cache/"common_text_pycst.bin",args.cache/"fonts_pycst.bin"])
    cache_inputs.extend(args.cache.parent/name for name in text_files)
    report={"validation":"PASS","upstream_commit":"791e961b12233100b303038c961666834f4beb9d",
            "scope":__doc__,"source_before_after_equal":True,"source_sha256":before,
            "candidate_query_staged": "player_equipment_queries_live_v1.cpp"not in (ROOT/"port/game-data/CMakeLists.txt").read_text(),
            "selected_library":{"path":str(library),"sha256":digest(library)},"selected_commands":selected,
            "selected_text_library":{"path":str(text_library),"sha256":digest(text_library),"dll_dependencies":text_dependencies},
            "text_commands":[row for row in reached if "CMakeFiles/dh2_inventory_text_v1.dir"in row["command"].replace("\\","/")],
            "tests":tests,"reference_sha256":{p.name:digest(p)for p in REF.iterdir()if p.is_file()},
            "cache_sha256":{p.relative_to(args.cache.parent).as_posix():digest(p)for p in cache_inputs}}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+"\n")
    print(json.dumps({"validation":"PASS","tests":[t["result"]for t in tests],"report":str(args.report)}))
if __name__=="__main__":
    main()
