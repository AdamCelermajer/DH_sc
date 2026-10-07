"""Verify original level-table gold in actual sanitized central game-data."""
import argparse,hashlib,json,subprocess
from pathlib import Path
DATA=Path(__file__).resolve().parents[1];ROOT=DATA.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def linux(path):return '/mnt/c/'+str(path.resolve()).replace('\\','/')[3:]
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--build',default='/home/adampalace/dh2-world-build');parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
 if args.output.exists():raise RuntimeError('Preserve previous level-table host proof')
 ref=DATA/'reference/level-tables';arm_path=DATA/'reports/level-tables-arm64-differential.json';arm=json.loads(arm_path.read_text());capture=json.loads((ref/'original-loader-capture.json').read_text());gold=ref/'level-table-fixtures.bin'
 assert arm['validation']=='PASS' and arm['gold_sha256']==sha(gold)==capture['gold_sha256']
 for name,digest in arm['source_sha256'].items():assert sha(ROOT/name)==digest
 for name,digest in capture['assets_sha256'].items():assert sha(ref/name)==digest
 files=[DATA/name for name in ('level_tables.cpp','level_tables.hpp','data.hpp','CMakeLists.txt','tests/level_tables.cpp')]+[Path(__file__)]
 sources={path.relative_to(ROOT).as_posix():sha(path) for path in files};commands=[]
 def run(*command):
  result=subprocess.run(['wsl.exe','--cd',linux(ROOT),*command],capture_output=True,text=True,timeout=60);commands.append(dict(arguments=list(command),returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0 and not result.stderr.strip(),commands[-1];return result.stdout.strip()
 lib=args.build+'/game-data/libdh2_game_data.so';exe=args.build+'/game-data/level_tables_audit'
 def hashes():return {line.split(maxsplit=1)[1]:line.split()[0] for line in run('sha256sum',lib,exe).splitlines()}
 before=hashes();deps=run('ldd',exe);assert lib in deps and 'libasan.so' in deps and 'libubsan.so' in deps and 'not found' not in deps
 compiler=json.loads(run('cat',args.build+'/compile_commands.json'));records=[row for row in compiler if row['file'].endswith('/game-data/level_tables.cpp') or row['file'].endswith('/game-data/tests/level_tables.cpp')];assert len(records)==2 and all('-fsanitize=address,undefined' in row['command'] for row in records)
 audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold),linux(ref)))
 assert audit['validation']=='PASS' and audit['module_library']==lib and audit['record_cases']==212 and audit['truncated_prefix_guards']==11214
 assert before==hashes() and sources=={path.relative_to(ROOT).as_posix():sha(path) for path in files}
 report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=sources,binary_sha256=before,compiler_records=records,dependencies=deps,commands=commands,gold_sha256=sha(gold),arm64_report_sha256=sha(arm_path),original_capture_sha256=sha(ref/'original-loader-capture.json'),cache_asset_sha256=capture['assets_sha256'],sanitizer_findings=0,source_Application_level_selection_verified=False,packaged_APK=False,physical_arm64_verified=False)
 args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host_audit=audit,binaries=before)))
if __name__=='__main__':main()
