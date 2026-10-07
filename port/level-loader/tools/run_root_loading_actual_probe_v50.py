from pathlib import Path
import json,subprocess
ROOT=Path(__file__).resolve().parents[3]
PRIVATE=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
OUT=ROOT/'port/level-loader/reports/root-loading-v50'
def linux(p):
    s=str(p).replace('\\','/')
    return '/mnt/'+s[0].lower()+s[2:]
def main():
    old=json.loads((PRIVATE/'port/level-loader/reports/native-gslevel-loading-v49-build.json').read_text())['runs']
    compile=list(old[0]['command']);idx=compile.index('-c')
    includes=['-I'+linux(p) for p in (OUT/'compile-layout/port').iterdir() if p.is_dir()]
    compile=compile[:4]+includes+compile[4:]
    compile[compile.index('-c')+1]=linux(OUT/'root-loading-actual-probe.cpp')
    compile[compile.index('-o')+1]=linux(OUT/'root-actual-probe.o')
    compile += ['-ffunction-sections','-fdata-sections']
    physical=compile.copy();physical[physical.index('-c')+1]=linux(ROOT/'port/level-world/physical_world.cpp');physical[physical.index('-o')+1]=linux(OUT/'physical-world-probe.o')
    manager=compile.copy();manager[manager.index('-c')+1]=linux(OUT/'compile-layout/port/level-world/canonical_object_manager_v1.cpp');manager[manager.index('-o')+1]=linux(OUT/'canonical-manager-probe.o')
    link=list(old[1]['command']);link[3]=linux(OUT/'root-actual-probe.o');link[link.index('-o')+1]=linux(OUT/'root-actual-probe');link.insert(4,linux(OUT/'physical-world-probe.o'))
    link.insert(4,linux(OUT/'canonical-manager-probe.o'))
    run=['wsl.exe','-e','/usr/bin/timeout','20s',linux(OUT/'root-actual-probe'),
         '/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip',
         linux(ROOT/'port/level-world/reference/character-game-design/real-cache-inputs.bin'),linux(OUT)]
    rows=[]
    for name,command in [('compile',compile),('physical',physical),('manager',manager),('link',link),('run',run)]:
        result=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=90)
        rows.append({'name':name,'command':command,'exit_code':result.returncode,'output':result.stdout})
        print(name,result.returncode,result.stdout[:4500])
        (OUT/'actual-probe-proof.json').write_text(json.dumps({'scope':'Root facade/source headers + independently frozen loader-host implementation libraries; no root APK/runtime claim; explicit transport fixtures.', 'runs':rows},indent=2))
        if result.returncode:return 1
    return 0
if __name__=='__main__':raise SystemExit(main())
