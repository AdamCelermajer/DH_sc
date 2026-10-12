from pathlib import Path
import hashlib,json,re,zipfile,struct
r=Path(__file__).resolve().parent.parent
cache=r/'port/level-loader/vendor/connected-owners-1b96f4edb021619d/reference/openable-container-v1/cache'
header=(r/'port/level-world/tests/openable_container_real_cache_fixture_v1.hpp').read_text()
proof={'scope':'exact byte extraction from original cache matching frozen native fixture; offsets refer to this hashed asset only','groups':[]}
outdir=r/'port/level-world/reference/openable-container-data-v11';outdir.mkdir(exist_ok=True)
for label,filename in [('records','game_objects_pyarray.bin'),('names','game_objects_pyarraynames.bin')]:
 values=re.search(r'source_openable_'+label+r'\[\]\s*=\s*\{([^}]*)\}',header,re.S).group(1)
 data=bytes(int(v) for v in re.findall(r'\d+',values));whole=(cache/filename).read_bytes()
 start=whole.find(data);assert start>=0 and whole.find(data,start+1)==-1
 assert struct.unpack_from('<I',data)[0]==68
 target=outdir/f'openable-containers-group5-{label}.bin';target.write_bytes(data)
 proof['groups'].append({'source':str((cache/filename).relative_to(r)).replace('\\','/'),'source_sha256':hashlib.sha256(whole).hexdigest(),'source_size':len(whole),'group_index':5,'start_inclusive':start,'end_exclusive':start+len(data),'size':len(data),'count':68,'extracted':str(target.relative_to(r)).replace('\\','/'),'sha256':hashlib.sha256(data).hexdigest(),'count_word_included':True})
(outdir/'group-extraction-proof.json').write_text(json.dumps(proof,indent=2))
files=[]
for p in ['port/game-data/game_object_dictionary_v11.hpp','port/game-data/game_object_dictionary_v11.cpp','port/level-world/openable_container_data_connection_v11.hpp','port/level-world/openable_container_data_connection_v11.cpp','port/level-world/openable_container_owner_v1.hpp','port/level-world/openable_container_owner_v1.cpp','port/level-world/tests/openable_container_data_connection_v11.cpp','port/level-world/tests/openable_container_real_cache_fixture_v1.hpp','port/level-world/reports/openable-container-data-v11-handoff.md','port/level-world/reference/inventory-gathering-v11/chest-binding-original.asm','port/level-world/reports/android-native-owner-tests/openable-container-data-v11/receipt.json','.local-inputs/freeze_openable_data_v11.py']:
 files.append(r/p)
files+=list(outdir.glob('*'))
files += [cache/n for n in ['game_objects_pyarray.bin','game_objects_pyarraynames.bin','game_objects_dictionary_pyarray.bin','game_objects_dictionary_pyarraynames.bin']]
entries=[{'path':p.relative_to(r).as_posix(),'size':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sorted(files)]
body=json.dumps({'version':11,'scope':'separate immutable chest table/dictionary provider; no gathering/Item shared-layout successors','entries':entries},indent=2).encode()
tag=hashlib.sha256(body).hexdigest()[:16];out=r/'port/level-world/reports'/f'openable-container-data-v11-handoff-{tag}.zip'
with zipfile.ZipFile(out,'w',zipfile.ZIP_DEFLATED) as z:
 z.writestr('openable-container-data-v11-manifest.json',body)
 for p in files:z.write(p,p.relative_to(r).as_posix())
with zipfile.ZipFile(out) as z:
 for e in entries:assert hashlib.sha256(z.read(e['path'])).hexdigest()==e['sha256']
receipt={'status':'PASS','path':str(out),'sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'entries':len(entries),'individual_hashes_verified':True,'extraction':proof}
(r/'port/level-world/reports/openable-container-data-v11-freeze.json').write_text(json.dumps(receipt,indent=2));print(json.dumps(receipt,indent=2))
