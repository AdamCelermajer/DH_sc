import collections,json,pathlib,zipfile,xml.etree.ElementTree as ET
root=pathlib.Path(__file__).resolve().parents[1]
capture=json.loads((root/'reference/procedural-functions/symbols-and-strings.json').read_text())
for row in capture['functions']:
    if any(v in row['symbol'] for v in ('LoadRuleFile','LoadListRules','LoadRoomPools','LoadBlocks','LoadFromXml','NextInt','GetInt','Hash')):
        print(json.dumps(row))
with zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    for name in ('data/scene/022_swamp2.rule.xml',):
        raw=z.read(prefix+name)
        print(name+'\n'+raw.decode(errors='replace'))
    counts=collections.Counter()
    for name in z.namelist():
        if name.endswith('.rule.xml'):
            try:doc=ET.fromstring(z.read(name))
            except ET.ParseError:continue
            for e in doc.iter():counts[e.tag]+=1
    print(json.dumps({'rule_element_counts':counts}))
