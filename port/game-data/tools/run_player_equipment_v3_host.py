"""Build/replay the isolated equipment source caller, without central mutation."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--output',type=Path,default=ROOT/'port/game-data/reports/player-equipment-v3-host-audit.json');a=ap.parse_args()
 names=['port/game-data/player_equipment_v3.hpp','port/game-data/player_equipment_v3.cpp','port/game-data/tests/player_equipment_v3.cpp'];before={p:sha(ROOT/p) for p in names};exe='.local-inputs/player-equipment-v3/host-audit';gold='port/game-data/reference/player-equipment-v3/fixtures.bin';wslroot='/mnt/'+ROOT.drive[0].lower()+'/'+ROOT.as_posix()[3:]
 prefix='cd '+shlex.quote(wslroot)+' && ';build=prefix+shlex.join(['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',names[1],names[2],'-o',exe]);subprocess.run(['wsl','-e','bash','-lc',build],check=True)
 command=prefix+'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.join([exe,gold]);r=subprocess.run(['wsl','-e','bash','-lc',command],capture_output=True,text=True);out=ROOT/'.local-inputs/player-equipment-v3';(out/'host.stdout').write_text(r.stdout);(out/'host.stderr').write_text(r.stderr);r.check_returncode();audit=json.loads(r.stdout);assert not r.stderr and audit['validation']=='PASS' and audit['mismatches']==0;assert before=={p:sha(ROOT/p) for p in names}
 report={'validation':'PASS','host_audit':audit,'source_sha256':before,'script_sha256':sha(__file__),'executable_sha256':sha(ROOT/exe),'gold_sha256':sha(ROOT/gold),'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'sanitizers':['address','undefined','leak'],'sanitizer_findings':0,'commands':[build,command],'scope':'Exact borrowed source equipment graph and ordered native providers. Storage/effect providers are explicit controlled fixtures; no live equipment visuals, campaign or complete property recalculation claim.'};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
