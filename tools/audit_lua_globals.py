#!/usr/bin/env python3
"""Inventory compiled Lua global reads/writes; never execute the game scripts."""
import argparse
from collections import Counter,defaultdict
import csv
import hashlib
import json
from pathlib import Path
import re
import subprocess

REPO=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser()
    p.add_argument('--compiler',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    manifest_path=REPO/'recovered/scripts/manifest.json'
    manifest=json.loads(manifest_path.read_text())
    overrides={x['original']:x for x in manifest['overrides']}
    reads=Counter();writes=Counter();rows=[];use=defaultdict(list)
    pattern=re.compile(r'^\s*\d+\s+\[[^\]]+\]\s+(GETGLOBAL|SETGLOBAL)\s+[^;]+;\s*(\S+)\s*$')
    for row in manifest['files']:
        assert sha(REPO/row['path'])==row['sha256']
        selected=overrides.get(row['path'],row);path=REPO/selected['path']
        assert sha(path)==selected['sha256']
        result=subprocess.run([str(a.compiler),'-l','-p',str(path)],capture_output=True,check=True)
        local_reads=Counter();local_writes=Counter()
        for line in result.stdout.decode('utf-8','strict').splitlines():
            match=pattern.match(line)
            if match:
                (local_reads if match[1]=='GETGLOBAL'else local_writes)[match[2]]+=1
            elif re.search(r'\b(?:GETGLOBAL|SETGLOBAL)\b',line):
                raise AssertionError('Unparsed global instruction: '+line)
        reads.update(local_reads);writes.update(local_writes)
        for name in local_reads:use[name].append(row['cache_path'])
        rows.append({'original':row['path'],'compiled_source':selected['path'],
            'source_sha256':selected['sha256'],'global_reads':dict(sorted(local_reads.items())),
            'global_writes':dict(sorted(local_writes.items()))})
    symbols=REPO/'recovered/native/symbols/libDungeonHunter2.so/symbols.csv'
    candidates=defaultdict(list)
    with symbols.open(encoding='utf-8')as stream:
        for row in csv.DictReader(stream):
            name=row['demangled']
            if row['table']!='.dynsym' or 'sfc::script::lua::Arguments const&'not in name:
                continue
            match=re.search(r'::_([A-Za-z][A-Za-z0-9_]*)\(',name)
            if match and match[1]in reads:
                candidates[match[1]].append({'demangled':name,'symbol':row['name'],
                    'elf_address':f"0x{int(row['address']):08x}",'bytes':int(row['size'])})
    version=subprocess.run([str(a.compiler),'-v'],capture_output=True,check=True)
    result={'complete_game':False,'scripts_executed':False,'registration_verified':False,
        'scope':'Compiler-emitted global reads/writes, including nested functions; candidate original Lua bridge methods from matching names/signatures only',
        'files':len(rows),'distinct_global_reads':len(reads),'distinct_global_writes':len(writes),
        'read_instructions':sum(reads.values()),'write_instructions':sum(writes.values()),
        'candidate_bridge_names':len(candidates),'compiler_sha256':sha(a.compiler.resolve()),
        'compiler_version':(version.stdout+version.stderr).decode().strip(),
        'manifest_sha256':sha(manifest_path),'symbol_index_sha256':sha(symbols),
        'test_sha256':sha(Path(__file__)),'global_reads':dict(sorted(reads.items())),
        'global_writes':dict(sorted(writes.items())),
        'candidate_native_bridge':{name:{'symbols':values,'read_instructions':reads[name],
            'referenced_by':use[name]}for name,values in sorted(candidates.items())},'per_file':rows}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k not in ('global_reads','global_writes','candidate_native_bridge','per_file')}))
if __name__=='__main__':main()
