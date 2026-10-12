exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightset-discover-v53.py').read().split('for row in rows:')[0])
rows=[x for x in symbols.splitlines() if ('Light' in x and not any(y in x for y in ['LightMap','lightning','Highlight','LightSet'])) or 'LevelConfig' in x and ('Load' in x or 'LevelConfig()' in x)]
(r/'light-symbols.txt').write_text('\n'.join(rows)+'\n');print('\n'.join(rows)[:22000])
more=[{'at':hex(pc+8+w(lit)),'text':txt(pc+8+w(lit))} for pc,lit in [(0x3f01ec,0x3f028c),(0x3f01f8,0x3f0290)]];print(more);(r/'loadxml-literals.json').write_text(json.dumps(more,indent=2)+'\n')
for row in rows:
 bits=row.split();a=int(bits[0],16);n=int(bits[1],16)
 if n and (('LightBase::' in row or 'LightPoint::' in row or 'CLightSceneNode::' in row) and any(x in row for x in ['Load','Init','Declare','LightBase(','LightPoint(','SyncData','GetIds','Refresh','CLightSceneNode(']) or 'LevelConfig' in row and 'Load' in row):
  out=subprocess.check_output([str(tools/'llvm-objdump.exe'),'-d','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+n),str(e)],text=True);(r/('source-'+bits[0]+'.asm')).write_text(out)
import zipfile,xml.etree.ElementTree as ET
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 names=[x for x in z.namelist() if 'light' in x.lower() and not x.endswith('/')];print('\n'.join(names)[:14000]);(r/'light-archive-names.json').write_text(json.dumps(names,indent=2)+'\n')
 for name in names:
  if name.lower().endswith(('.xml','.mlx','.config','.lightset_xml')) and z.getinfo(name).file_size<200000:
   data=z.read(name);(r/('asset-'+Path(name).name)).write_bytes(data)
