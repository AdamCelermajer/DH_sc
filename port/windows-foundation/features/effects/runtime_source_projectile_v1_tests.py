import pathlib,subprocess,json,os,hashlib,zipfile
root=pathlib.Path(__file__).resolve().parents[4];feature=root/'port/windows-foundation/features/effects';build=root/'.local-inputs/runtime-source-projectile-v1';bin=root/'.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin';compiler=bin/'clang++.exe'
# Recreate test-only inputs from the supplied original cache. Never modify it.
archive=pathlib.Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert hashlib.sha256(archive.read_bytes()).hexdigest()=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
asset_root=build/'assets';asset_root.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(archive) as original:
 prefix='com.gameloft.android.GAND.GloftD2SS/files/data/'
 selected={'3d/projectiles/elemental_bolt_fire.bdae':'data/3D/projectiles/elemental_bolt_fire.bdae','3d/characters/prince/animations/prince_ranged_attack.bdae':'data/3D/characters/prince/animations/prince_ranged_attack.bdae','scripts/skills/npc_fireball.luac':'npc_fireball.luac'}
 for name in original.namelist():
  if not name.startswith(prefix):continue
  uri=name[len(prefix):]
  relative=uri if uri.startswith('pydata/') and uri.endswith('.bin') else selected.get(uri.lower())
  if relative:
   destination=asset_root/relative;destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(original.read(name))
sources=[feature/'runtime_source_projectile_v1.cpp',feature/'runtime_source_projectile_v1_tests.cpp',root/'port/game-data/combat_events.cpp',root/'port/level-world/character_attack_geometry.cpp',root/'port/game-data/data.cpp',root/'port/game-data/animation_tables.cpp',root/'port/game-data/skill_tables.cpp',root/'port/game-data/items.cpp',root/'port/game-data/class_tables.cpp',root/'port/game-data/properties.cpp',root/'port/windows-foundation/original_actor_properties.cpp',root/'port/windows-foundation/asset_catalog.cpp',root/'port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp',root/'port/windows-foundation/animation_markers.cpp',root/'port/engine-animation/event_track.cpp',root/'port/engine-animation/events.cpp',root/'port/engine-resources/resources.cpp']
flags=['-std=c++17','-Wall','-Wextra','-Werror','-pedantic','-O2','-fno-fast-math','-ffp-contract=off']
exe=build/'runtime-source-projectile-v1-tests.exe';r=subprocess.run([str(compiler),*flags,*map(str,sources),'-o',str(exe)],capture_output=True,text=True);receipt={'compile_exit':r.returncode,'compile_stderr':r.stderr};print(r.stdout+r.stderr,end='')
if r.returncode:raise SystemExit(r.returncode)
env=dict(os.environ);env['PATH']=str(bin)+os.pathsep+env.get('PATH','');r=subprocess.run([str(exe),str(build/'assets')],capture_output=True,text=True,env=env);print(r.stdout+r.stderr,end='');receipt.update(runtime_exit=r.returncode,runtime_output=r.stdout+r.stderr)
report=feature/'runtime-source-projectile-v1-report.json';data=json.loads(report.read_text());data['implementation']='Authored immutable source plan, current source property/equipment selection through native dh2_attack_range_parameters, and original state5 ranged decision. No native-owner callback facade, World/projectile owner, motion/collision/damage result implementation added.';data['isolated_validation']=receipt;data['production_validation']='NOT INTEGRATED: normal original animation-event caller, plan-to-source mesh draw, launch transform, qualified GameObject motion/PF, delayed callback/result and visual acceptance remain root/feature work.';data['status']='verified_range_selection_plan_api_runtime_unverified' if not r.returncode else 'actual_asset_plan_test_failed';data['assets'].update({str(p.relative_to(root)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in (build/'assets').rglob('*') if p.is_file() and ('projectile' in p.name or p.name in ['npc_fireball.luac','elemental_bolt_fire.bdae','prince_ranged_attack.bdae'])});report.write_text(json.dumps(data,indent=2)+'\n');(build/'test-result.json').write_text(json.dumps(receipt,indent=2)+'\n');raise SystemExit(r.returncode)
