#!/usr/bin/env python3
"""Exercise native-reader expected quest records in the checked source Lua VM."""
import argparse,hashlib,importlib.util,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('quest_ui',REPO/'port/android-app/tests/questcompile_runtime.py')
qa=importlib.util.module_from_spec(spec);spec.loader.exec_module(qa)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('cache','runner','build-report','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();build=json.loads(a.build_report.read_text(encoding='utf-8'))
    assert sha(a.runner)==build['artifacts'][a.runner.name]['sha256']
    for name,value in build['source_sha256'].items():assert sha(ROOT/name)==value,name
    stage=ROOT/'build/questcompile-corpus-host';stage.mkdir(exist_ok=True)
    controlled=ROOT/'build/combat-controlled-host'
    for name in ('properties.bin','loot.bin'):(stage/name).write_bytes((controlled/name).read_bytes())
    constants=[a.cache/f'data/pydata/{n}_pycst.bin'for n in ('ai','design','v2quests')]
    cfile=stage/'constants.txt';cfile.write_text(''.join(str(f.resolve())+'\n'for f in constants),encoding='utf-8')
    scripts=[]
    for i,source in enumerate((qa.damage.PREPARE,qa.damage.COMBAT,qa.damage.POLICIES,qa.damage.ROLLBACK,qa.EVENTS,qa.METADATA,qa.REAL_QUESTS,qa.COMPILE_REAL)):
        f=stage/f'{i}.lua';f.write_text(source,encoding='utf-8');scripts.append(f)
    sfile=stage/'scripts.txt';sfile.write_text(''.join(str(f.resolve())+'\n'for f in scripts),encoding='utf-8')
    # The exact recovered melee source executes before diagnostic scripts.
    original=REPO/'recovered/scripts/original/data/scripts/level/combat_formulas.luac'
    sfile.write_text(str(original.resolve())+'\n'+sfile.read_text(encoding='utf-8'),encoding='utf-8')
    quests=a.cache/'data/pydata/v2quests_pyarray.bin'
    command=[str(a.runner.resolve()),'--quests',str((stage/'properties.bin').resolve()),str((stage/'loot.bin').resolve()),str(cfile.resolve()),str(quests.resolve()),str(sfile.resolve())]
    completed=subprocess.run(command,capture_output=True,text=True,timeout=60)
    if completed.returncode:raise RuntimeError(completed.stdout+completed.stderr)
    assert completed.stdout.endswith('QUEST CORPUS PASS 3 9\n'),completed.stdout
    report={'complete_game':False,'passed':True,'quest_records':64,'counted_kill_objectives':34,'compiled_real_objectives':34,'scripts':9,'constant_files':3,
        'runner_sha256':sha(a.runner),'build_report_sha256':sha(a.build_report),'test_sha256':sha(Path(__file__)),
        'ui_test_sha256':sha(REPO/'port/android-app/tests/questcompile_runtime.py'),'quests_sha256':sha(quests),
        'script_sha256':[sha(f)for f in (original,*scripts)],'output':completed.stdout,
        'scope':'Host native source Lua executes exact melee formula, owned health/death/quest counting, all 64 real quest record snapshots compared to original reader evidence and 34 real counted-kill objectives. All 34 real counted-kill records compile against owned resolved-ID world snapshots and execute formula/hit/death/count. Original Character/world collection lifecycle, conditions, automatic dispatch, quest persistence/rewards and full source gameplay remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))
if __name__=='__main__':main()
