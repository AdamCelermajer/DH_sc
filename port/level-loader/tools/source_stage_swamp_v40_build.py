"""Rebuild/run only the isolated source-stage SWAMP test target. No emulator."""
from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[1];b=r.parents[2]/'build/receiver-transport-v5-host'
def lin(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
commands=[['wsl.exe','-e','cmake','--build',lin(b),'--target','dh2_loader_source_stage_swamp_v40','-j1'],['wsl.exe','-e',lin(b/'dh2_loader_source_stage_swamp_v40'),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip','/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world/reference/character-game-design/real-cache-inputs.bin',lin(b.parent/'source-stage-swamp-v40')]]
for command in commands:
 result=subprocess.run(command,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);print(result.stdout)
 if result.returncode:raise SystemExit(result.returncode)
out={'validation':'PASS','commands':commands,'stdout':result.stdout,'source_sha256':hashlib.sha256((r/'tests/source_stage_swamp_v40.cpp').read_bytes()).hexdigest()}
(r/'reports/source-stage-swamp-v40-rerun.json').write_bytes((json.dumps(out,indent=2)+'\n').encode())
