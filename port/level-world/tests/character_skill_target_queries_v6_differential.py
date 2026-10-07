"""Original predicate corpus replay against V6 direct live-array borrowing."""
from pathlib import Path
import sys,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_target_providers_differential import QueryOracle,cases
lib=ROOT/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
o=QueryOracle(lib)
# Only the invoked entry name is redirected. ABI/layout, original instruction
# execution, live mutation, nesting and required policy services are unchanged.
o.new.symbols['dh2_character_target_query']=o.new.symbols['dh2_character_skill_target_query_v6']
records=[]
for i,row in enumerate(cases(o.types)):
 expected=o.execute(row);actual=o.execute(row,True)
 assert expected==actual,(i,row,expected,actual)
 records.append(dict(input=row,result=expected))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
gold=ROOT/'port/level-world/reference/character-skill-combat-v6/target-query-gold-v6.json'
gold.write_text(json.dumps(records,indent=2)+'\n')
report=dict(validation='PASS',cases=len(records),ordered_callbacks=sum(len(x['result'][1])for x in records),mismatches=0,original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),optimized_sha256=sha(lib),gold_sha256=sha(gold),source_sha256=sha(ROOT/'port/level-world/character_skill_target_queries_v6.cpp'),header_sha256=sha(ROOT/'port/level-world/character_skill_target_queries_v6.hpp'),script_sha256=sha(Path(__file__)),scope=__doc__)
(ROOT/'port/level-world/reports/character-skill-target-queries-v6-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
