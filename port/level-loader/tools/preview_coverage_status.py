import json,pathlib
reports=pathlib.Path(__file__).resolve().parents[1]/'reports'
row=json.loads((reports/'fixed-map-preview-coverage.json').read_text())
print(json.dumps({'summary':row['summary'],'swamp':[x for x in row['levels'] if 'SWAMP' in x['name']]}))
