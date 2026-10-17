"""Build the production native Debug filesystem adapter and exercise real IO."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[3]
NATIVE=ROOT/'port/android-native'
LEVEL=ROOT/'port/level-world'

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',required=True,type=Path)
    parser.add_argument('--cache',required=True,type=Path)
    parser.add_argument('--output',type=Path,default=NATIVE/'build/native-debug-files/host.exe')
    parser.add_argument('--report',type=Path,default=NATIVE/'build/native-debug-files/validation.json')
    args=parser.parse_args();output=args.output.resolve();output.parent.mkdir(parents=True,exist_ok=True)
    sources=[NATIVE/'app/src/main/cpp/native_debug_files.cpp',LEVEL/'debug_switches_runtime.cpp',LEVEL/'debug_switches_persistence.cpp',NATIVE/'tests/native_debug_files_host.cpp']
    command=[str(args.compiler),'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(output)]
    compile_result=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if compile_result.returncode:raise RuntimeError(compile_result.stdout+compile_result.stderr)
    configuration=args.cache.resolve()/'DebugSwitches.savegame';before=sha(configuration)
    assert before=='51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b'
    # Keep the real generated files in a fresh report directory, without deleting
    # previous runs or touching original evidence/app-private production files.
    folder=Path(tempfile.mkdtemp(prefix='real-files-',dir=output.parent))
    result=subprocess.run([str(output),str(configuration),str(folder)],cwd=ROOT,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+result.stderr)
    host=json.loads(result.stdout);assert host['validation']=='PASS' and host['host_cases']==20 and sha(configuration)==before
    dependencies=sources+[NATIVE/'app/src/main/cpp/native_debug_files.hpp',LEVEL/'debug_switches_runtime.hpp',LEVEL/'debug_switches_persistence.hpp',ROOT/'port/persistence/binary.h',Path(__file__).resolve(),NATIVE/'reference/native-debug-files/NOTES.md',NATIVE/'reference/native-debug-files/original-functions.json',LEVEL/'reference/debug-switches-runtime/original-functions.json',LEVEL/'reference/debug-switches-persistence/original-functions.json']
    files={f.relative_to(folder).as_posix():{'bytes':f.stat().st_size,'sha256':sha(f)} for f in sorted(folder.glob('*/DebugSwitches.savegame')) if f.is_file()}
    report={'validation':'PASS','host_report':host,'command':command,'compiler_diagnostics':compile_result.stderr,'source_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in dependencies},'cache_configuration_sha256':before,'original_evidence_unchanged':True,'real_generated_files_directory':str(folder),'real_generated_files':files,'executable_sha256':sha(output),'new_complete_caller_bodies':0,'new_complete_dependency_bodies':0,'android_wired':False,'scope':'Production filesystem dependency adapter over frozen source Debug callers. Real retained binary reads and typed file writes/close; asset installation only when absent, preserves files across backend recreation. Explicit source partial-decode and OS error boundaries; no original OS stream/allocator body claims.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host));print('report:',args.report)

if __name__=='__main__':main()
