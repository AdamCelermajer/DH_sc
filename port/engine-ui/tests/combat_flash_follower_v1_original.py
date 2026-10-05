"""Original IsFollower/GetCharType/GetCharAIId over original decoded cache rows.
Compares source type rows to native host parser/provider receipt, not a fake all-NPC policy.
"""
from pathlib import Path
import sys,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_target_providers_differential import QueryOracle,words
o=QueryOracle();c=o.old;owner=o.owners[0]
host_path=ROOT/'port/android-native/reports/combat-flash-inputs-v1-host.json';host=json.loads(host_path.read_text());native=json.loads(host['commands'][-1]['stdout'])
assert o.types==native['types'],(o.types,native['types'])
rows=[]
for aid in [*range(len(o.types)),-1,len(o.types),-2147483648,2147483647]:
 c.uc.mem_write(owner+0xffc,words(aid));actual=c.invoke(0x3a307c,[owner]);index=aid if 0<=aid<len(o.types)else 8;assert actual==int(native['types'][index]==2),(aid,actual)
 rows.append(dict(ai_id=aid,type=o.types[index],follower=actual))
report=dict(validation='PASS',cases=len(rows),original_instructions_executed=True,all_actual_cache_rows=True,mismatches=0,scope=__doc__,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),host_receipt_sha256=hashlib.sha256(host_path.read_bytes()).hexdigest(),rows=rows)
(ROOT/'port/android-native/reports/combat-flash-follower-v1-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k!='rows'}))
