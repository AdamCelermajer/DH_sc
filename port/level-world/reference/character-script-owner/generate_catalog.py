"""Generate native descriptors from frozen executed original registration callers."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
source=ROOT/'reference/character-script-ownership/ownership-probe.json'
assert hashlib.sha256(source.read_bytes()).hexdigest()=='710f6a5639324e049aba1b12ca759a1317d2c71cf84af4ee961f6e7a02c12f92'
report=json.loads(source.read_text());out=[]
for phase,label in ((1,'step1-bind'),(2,'step2-real-character-and-gameobject-registration')):
 out.append('constexpr ScriptBinding24 bindings'+str(phase)+'[]{')
 events=next(x for x in report['cases'] if x['case']==label)['trace']
 for x in events:
  if x['service'] not in ('register_function','register_method'):continue
  out.append(' {'+json.dumps(x['name'])+','+x['function']+'u,'+str(int(x['service']=='register_method'))+'u,'+str(int(phase==2))+'u,0u},')
 out.append('};')
(ROOT/'character_script_owner_bindings.inc').write_text('\n'.join(out)+'\n')
print('Generated35 +265 source binding descriptors')
