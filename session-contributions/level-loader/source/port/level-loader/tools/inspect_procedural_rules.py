"""Inspect captured original rule readers and retained authored rule examples."""
import argparse,json,pathlib
ap=argparse.ArgumentParser();ap.add_argument('--example',default='swamp2-procedural-sources.json');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1]
functions=json.loads((root/'reference/procedural-functions/symbols-and-strings.json').read_text())['functions']
for f in functions:
    n=f['symbol']
    if any(s in n for s in ('LoadListRules','LoadRuleFile','LoadFromXml')) or ('C1' in n and any(s in n for s in ('RootRule','ListRule','RoomPool','RPElem','ListElem','RandomGenerator','4Rule','4Path','ForceBlock','EndPath'))):
        print(json.dumps(f))
source=json.loads((root/'reports'/a.example).read_text())
print(json.dumps({'identity':source['identity'],'rule':source['rule']},indent=2))
