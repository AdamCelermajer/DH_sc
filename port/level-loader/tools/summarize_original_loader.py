import json,pathlib,sys
rows=json.loads((pathlib.Path(__file__).resolve().parents[1]/'reference/loader-functions/symbols-and-strings.json').read_text())['functions']
for r in rows:
    if (sys.argv[1] in r['symbol']) if len(sys.argv)>1 else ('Module' in r['symbol'] or 'LoadFromXML' in r['symbol']):
        print(json.dumps(r))
