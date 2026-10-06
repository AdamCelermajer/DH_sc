import pathlib,subprocess,json,hashlib
draft=pathlib.Path(__file__).parent
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=root/'port/level-loader/reports'
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
fixtures=reports/'connected-source-fixtures.zip'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
rows=[]
for build in ('connected-owner-host','connected-owner-sanitizers'):
 base=root.parent/'build'/build
 for name,args in [('dh2_loader_connected_source_probe',[linux(cache),linux(fixtures)]),('loader/dh2_loader_canonical_source_adapter_probe',[])]:
  probe=base/name
  result=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe),*args],capture_output=True,timeout=60)
  assert result.returncode==0,(build,name,result.stdout,result.stderr)
  assert not result.stderr,result.stderr
  data=json.loads(result.stdout);assert data['validation']=='PASS'
  if 'composition_checks' in data:
   assert data['composition_checks']==12 and len(data['additional_required_classes'])==13
   assert data['first_failure']['factory_address']==0x340ca8
  else:assert data['adapter_checks']==14
  rows.append({'build':build,'probe':name,'probe_sha256':sha(probe),'result':data})
  print(json.dumps({'build':build,'probe':name,'checks':data.get('composition_checks',data.get('adapter_checks'))}),flush=True)
receipt={'validation':'PASS','scope':'Canonical cached file delivery and original SWAMP first failure; no live world construction','cases':rows,
 'cache_sha256':sha(cache),'fixtures_sha256':sha(fixtures),
 'connected_archive_sha256':'29bdb6fb795260087928f5523bb7c6c15ffec39918e4e2a87cefb2d08b0c924a',
 'supplement_sha256':'f94ddc5a5ba35ff8c72951b4cc691c27ef49ac988ac04a306489c221b51f34ea',
 'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in [root/'port/level-loader/canonical_cached_file_v1.hpp',root/'port/level-loader/canonical_cached_file_v1.cpp',root/'port/level-loader/tests/canonical_connected_source_probe.cpp',root/'port/level-loader/tests/canonical_source_adapter_probe.cpp',root/'port/level-loader/tests/cmake-connected-owner/CMakeLists.txt']},
 'second_owner_target_created':False,'full_loader_verified':False}
(reports/'canonical-connected-source-host.json').write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
