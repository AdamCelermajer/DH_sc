from pathlib import Path
import runpy,sys,json,hashlib
sys.path.insert(0,str(Path(__file__).resolve().parent))
root=Path(__file__).resolve().parents[3]
v=runpy.run_path(str(Path(__file__).with_name('produce_character_world_npc_properties_v1.py')))
rows=[]
for key in v['keys']:
 node=v['xml'][key['room']][key['name']]
 value=node.get('spawn_prob');assert value in (None,'100')
 assert node.get('light_set') is None
 rows.append(dict(**key,spawn_prob_authored=value,spawn_prob_resolved=100,light_set_authored=None))
report=dict(validation='PASS',cache_sha256=v['CACHE'],descriptor_sha256=v['sha'](v['descriptor'].read_bytes()),source_members=v['sources'],records=rows,
 constructor=dict(cached270=-1,threshold274=100,once1395=0,network_mode5=0),
 constructor_addresses=dict(game='38c130',character='3aa1b4',network_base='7fd838',network_derived='825000',network_instance='7fd744',network_GetThis='7fd794'),full_XML_factory=False)
(root/'port/level-world/reference/character-world-npc-object-v1/crypt01-init-final-inputs.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',actors=len(rows),explicit_spawn_prob_100=sum(r['spawn_prob_authored'] is not None for r in rows))))
