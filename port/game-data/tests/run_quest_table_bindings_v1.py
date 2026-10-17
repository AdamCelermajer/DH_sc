"""Fresh original QuestTable readers against the actual selected game-data DLL.

The existing quest-data decoder is reused unchanged. The stream, allocation,
disposal and libc memory leaves are declared in the original helper. This gate
does not claim Quest child execution, Android compilation or live gameplay.
"""
from __future__ import annotations
import argparse, hashlib, json, os, subprocess
from pathlib import Path
from quest_table_bindings_v1_original import Original

ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data'
PIN=MODULE/'reference/quest-table-bindings-v1/original-functions.json'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--original',type=Path,required=True)
    p.add_argument('--cache',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--compiler',required=True)
    p.add_argument('--library',type=Path,required=True)
    args=p.parse_args()
    pins=json.loads(PIN.read_text())
    assert sha(args.original)==pins['original_sha256']
    for name,pin in pins['cache'].items():
        path=args.cache/name
        assert path.stat().st_size==pin['bytes'] and sha(path)==pin['sha256'],name
    decoder=pins['reused_decoder']
    assert sha(ROOT/decoder['path'])==decoder['sha256']
    assert sha(ROOT/'port/quest-data/quests.h')==decoder['header_sha256']
    prior=pins['prior_original_reference']
    assert sha(ROOT/prior['path'])==prior['sha256']
    gold=json.loads((ROOT/prior['path']).read_text())
    packed=(args.cache/'v2quests_pyarray.bin').read_bytes()
    names=(args.cache/'v2quests_pyarraynames.bin').read_bytes()
    original=Original(args.original,packed,names)
    expected=[original.record(i) for i in range(original.count)]
    assert original.count==64 and expected==gold['rows']
    # Original array readers retain allocation headers even for zero lists.
    list_presence=[[bool(original.word(original.pointer+i*0x11c+0x18+k*8))
                    for k in range(5)] for i in range(original.count)]
    assert all(all(row) for row in list_presence)
    args.output.mkdir(parents=True,exist_ok=True)
    library=args.library.resolve()
    command_file=next(parent/'compile_commands.json' for parent in library.parents
                      if (parent/'compile_commands.json').is_file())
    selected=[entry for entry in json.loads(command_file.read_text())
              if Path(entry['file']).name in ('quest_table_bindings_v1.cpp','quests.c')]
    assert sum(Path(e['file']).name=='quest_table_bindings_v1.cpp' for e in selected)==1
    assert sum(Path(e['file']).name=='quests.c' for e in selected)==1
    exe=args.output/'host.exe'
    command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',
             str(MODULE/'tests/quest_table_bindings_v1_host.cpp'),str(library),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True)
    assert build.returncode==0,build.stderr
    dlls=sorted(library.parent.parent.rglob('*.dll'))
    env=os.environ.copy()
    env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),
                                str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(args.cache.resolve())],capture_output=True,text=True,env=env)
    assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout)
    assert len(actual['rows'])==len(expected)
    mismatches=[dict(index=i,expected=e,actual=n)
                for i,(e,n) in enumerate(zip(expected,actual['rows'])) if e!=n]
    assert actual['packed_truncations']==len(packed)
    assert actual['names_truncations']==len(names)
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    sources=['quest_table_bindings_v1.cpp','quest_table_bindings_v1.hpp',
             'quest_runtime_fields_v1.hpp','tests/quest_table_bindings_v1_host.cpp',
             'tests/quest_table_bindings_v1_original.py','tests/run_quest_table_bindings_v1.py',
             'reference/quest-table-bindings-v1/original-functions.json']
    source_hashes={s:sha(MODULE/s) for s in sources}
    source_hashes.update({s:sha(ROOT/s) for s in [decoder['path'],'port/quest-data/quests.h',
                                               'port/engine-math/tests/differential.py']})
    report=dict(status='PASS' if not mismatches else 'FAIL',
                arm_row_comparisons=len(expected),counts=gold['counts'],
                executed_pinned_words=len(original.words),original_entries=dict(original.entries),
                original_boundary_calls=dict(original.calls),original_import_calls=dict(original.machine.import_calls),
                original_stream_consumption=dict(packed=len(packed),names=original.cursor),
                original_nonnull_list_controls=sum(sum(row) for row in list_presence),
                native_checks=actual['native_checks'],packed_truncations=actual['packed_truncations'],
                names_truncations=actual['names_truncations'],mismatches=mismatches[:10],
                original_sha256=pins['original_sha256'],source_sha256=source_hashes,
                selected_commands=selected,selected_library=str(library),
                selected_library_sha256=sha(library),
                binary_sha256={path.name:sha(path) for path in [exe,*dlls]},
                reused_decoder_attribution=decoder['attribution'],scope=pins['scope'],
                malformed_preserve=True,retained_generation_lifetime=True,
                android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items()
                     if k not in ('source_sha256','binary_sha256','selected_commands','mismatches')},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)

if __name__=='__main__':raise SystemExit(main())
