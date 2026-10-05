"""Build only this isolated loader APK; no device actions or shared output writes."""
import argparse,hashlib,json,os,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
ap=argparse.ArgumentParser()
ap.add_argument('--fixed-coverage',type=pathlib.Path,default=root/'port/level-loader/reports/fixed-map-level-coverage.json')
ap.add_argument('--procedural-coverage',type=pathlib.Path,default=root/'port/level-loader/reports/procedural-map-host-coverage.json')
options=ap.parse_args()
coverage=options.fixed_coverage
report=json.loads(coverage.read_text())
procedural_coverage=options.procedural_coverage
procedural=json.loads(procedural_coverage.read_text())
assert procedural['cache_sha256']==report['cache_sha256'] and procedural['all_rows_attempted']
maps=[{'identity':row['name'],'definition':row['file'],'label':row['name']+' (fixed)',
       'kind':'fixed'} for row in report['levels'] if row['map_preparation']=='assembled']
assert len(maps)==16
for row in procedural['levels']:
    states={str(run['seed']):run['status'] for run in row['runs']}
    issues=[str(run['seed'])+': '+run['status'].replace('_',' ') for run in row['runs'] if run['status']!='assembled']
    maps.append({'identity':row['identity'],'definition':row['definition'],'kind':'procedural',
                 'label':row['identity']+' (generated'+('; '+', '.join(issues) if issues else '')+')',
                 'preparation_by_seed':states})
assert len(maps)==51 and len({row['identity'] for row in maps})==51
catalog={'cache_sha256':report['cache_sha256'],
         'coverage_sha256':hashlib.sha256(coverage.read_bytes()).hexdigest(),
         'procedural_coverage_sha256':hashlib.sha256(procedural_coverage.read_bytes()).hexdigest(),
         'scope':'Original fixed/generated map inspection; runtime mobs/chests pending; known seed failures explicit',
         'maps':maps}
assets=root/'port/android-native/app/src/main/assets'
assets.mkdir(parents=True,exist_ok=True)
(assets/'loader-map-catalog.json').write_text(json.dumps(catalog,indent=2)+'\n',encoding='utf-8')
java=pathlib.Path(r'C:\Program Files\Android\Android Studio\jbr')
assert (java/'bin/java.exe').is_file(), 'Configured Android Studio JBR missing'
env=os.environ.copy();env['JAVA_HOME']=str(java)
command=['cmd.exe','/c',str(root/'port/android-native/gradlew.bat'),
         'assembleDebug','--no-daemon','--max-workers=2',
         '-Pdh2OriginalCache=C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip']
print('Isolated loader preview build:',root,flush=True)
raise SystemExit(subprocess.call(command,cwd=root/'port/android-native',env=env))
