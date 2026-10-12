"""Read original serialized binding fields for generated map name mismatches."""
import argparse,json,pathlib,struct,zipfile
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=pathlib.Path,required=True);a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[3];reports=root/'port/level-loader/reports'
gold=json.loads((reports/'procedural-modules-original.json').read_text())
paths={module['properties']['dae'] for case in gold['cases'] if case['name'].endswith('019_light_house_03.rule.xml')
       for run in case['runs'] for module in run['modules']};result=[]
with zipfile.ZipFile(a.cache) as archive:
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    entries={entry[len(prefix):].lower():entry for entry in archive.namelist() if entry.startswith(prefix)}
    for path in sorted(paths):
        b=archive.read(entries[path.lower()])
        def w(p):return struct.unpack_from('<I',b,p)[0]
        def s(p):
            at=w(p)
            if not at:return ''
            return b[at:b.index(0,at)].decode()
        r=w(32);geometry={}
        for i in range(w(r+0x68)):
            p=w(r+0x6c)+16*i;mesh=w(p+12)
            geometry['#'+s(p)]=[s(w(mesh+16)+56*j+4) for j in range(w(mesh+12))]
        def node(p):
            for i in range(w(p+64)):
                at=w(p+68)+8*i
                if w(at)!=3:continue
                g=w(at+4);symbols=geometry[s(g+4)]
                bindings=[w(g+16)+60*j for j in range(w(g+12))]
                for index,(symbol,binding) in enumerate(zip(symbols,bindings)):
                    target=s(binding+4)
                    if target!='#'+symbol:
                        fields=[]
                        for offset in range(0,60,4):
                            value=w(binding+offset);text=None
                            if 0<value<len(b):
                                end=b.find(b'\0',value,min(value+256,len(b)))
                                if end>=value:
                                    raw=b[value:end]
                                    if raw and all(32<=c<127 for c in raw):text=raw.decode()
                            fields.append({'offset':offset,'value':value,'string':text})
                        result.append({'asset':path,'node':s(p),'geometry':s(g+4),'primitive':index,
                                       'primitive_symbol':symbol,'binding_target':target,'binding_fields':fields})
            for i in range(w(p+56)):node(w(p+60)+80*i)
        for i in range(w(r+0x98)):
            p=w(r+0x9c)+16*i
            for j in range(w(p+8)):node(w(p+12)+80*j)
report={'scope':__doc__,'asset_count':len(paths),'mismatches':result}
(reports/'generated-material-binding-inputs.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
