"""Read-only inventory of generated room MVX and selected MGP/MVP inputs."""
import hashlib,json,pathlib,zipfile,xml.etree.ElementTree as ET
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader'
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
gold=json.loads((loader/'reports/procedural-layout-original.json').read_text())
prefix='com.gameloft.android.GAND.GloftD2SS/files/';rows=[];missing=[];attributes={};samples={}
with zipfile.ZipFile(cache) as pack:
    keys={name[len(prefix):].lower():name for name in pack.namelist() if name.startswith(prefix)}
    paths=[name[len(prefix):] for name in pack.namelist() if name.startswith(prefix) and
           ('deadend_swpcave_s_00' in name or name.endswith('.mvx'))]
    for case in gold['cases']:
        if not case['name'].startswith('data/'):continue
        for run in case['runs']:
            for tile in run['tiles']:
                uri=(case['folder']+'/mvx/'+tile['block_name']+'.mvx').lower().replace('\\','/')
                item={'definition':case['name'],'seed':run['seed'],'tile':tile['index'],'mvx_uri':uri,
                      'target':case['target'],'folder':case['folder'],'block_name':tile['block_name'],
                      'grid':tile['grid'],'height_bits':tile['height_bits'],'list_element':tile['list_element']}
                key=keys.get(uri)
                if not key:missing.append(uri);item['missing']=True
                else:
                    raw=pack.read(key);node=ET.fromstring(raw);child=node.find('GameObject')
                    item['sha256']=hashlib.sha256(raw).hexdigest();item['attributes']=child.attrib if child is not None else None
                    if child is not None:
                        for name,value in child.attrib.items():attributes.setdefault(name,set()).add(value)
                rows.append(item)
                if len(samples)<2:
                    for kind,extension in (('gameplay','mgp'),('visual','mvp')):
                        filename=tile['list_element'][kind]
                        path=(case['folder']+'/'+extension+'/'+filename).lower().replace('\\','/')
                        if filename and kind not in samples and path in keys:
                            samples[kind]={'uri':path,'xml':pack.read(keys[path]).decode()[:3000]}
report={'scope':__doc__,'tiles':len(rows),'missing':sorted(set(missing)),
        'attribute_values':{key:sorted(values) for key,values in attributes.items()},'rows':rows,
        'original_module_serialization_executed':False}
(loader/'reports/procedural-module-inputs.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'tiles':len(rows),'unique_mvx':len(set(row['mvx_uri'] for row in rows)),
                  'missing':len(report['missing']),'attributes':{key:len(values) for key,values in attributes.items()},
                  'sample':rows[0],'selected_xml_samples':samples,'relevant_cache_paths':paths[:15]}))
