import json,pathlib,sys
row=json.loads(pathlib.Path(sys.argv[1]).read_text())
for item in row['functions']:
    if any(x in item['symbol'] for x in sys.argv[2:]):print(json.dumps(item))
