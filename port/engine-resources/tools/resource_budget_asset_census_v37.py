"""Bounded real asset descriptors; no pixels/GL/emulator or whole ZIP inflate."""
from pathlib import Path
import argparse,collections,hashlib,json,subprocess,tempfile,zipfile,xml.etree.ElementTree as ET
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/resource-budget-v37';out.mkdir(parents=True,exist_ok=True)
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=Path,default=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));ap.add_argument('--loader-packet',type=Path,default=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reports\character-template-assets-v35-4bc16d0ffbce3834.zip'));args=ap.parse_args()
sources=['port/engine-resources/tests/resource_budget_asset_census_v37.cpp','port/engine-resources/resource_budget_v37.cpp','port/engine-resources/resources.cpp','port/asset-payloads/payloads.cpp','port/engine-textures/textures.cpp'];exe=out/'asset-census'
command=['g++','-std=c++17','-O2','-Wall','-Wextra','-Werror','-ffunction-sections','-fdata-sections',*sources,'-Wl,--gc-sections','-pthread','-o',unix(exe)]
p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=40)
if p.returncode:raise RuntimeError(p.stdout+p.stderr)
with zipfile.ZipFile(args.loader_packet) as loader:
 name='port/android-native/app/src/main/assets/loader-objects/SWAMP.xml';assert loader.getinfo(name).file_size<65536
 xml=loader.read(name);decl=ET.fromstring(xml);occurrences=collections.Counter(e.attrib['asset'].lower() for e in decl.findall('Entity'))
 blocked=[e.attrib for e in decl.findall('Blocked')];assert sum(occurrences.values())==54 and len(occurrences)==10
 rows=[];metadata=[]
 with tempfile.TemporaryDirectory(dir=out) as temporary,zipfile.ZipFile(args.cache) as cache:
  stage=Path(temporary);assert stage.resolve().is_relative_to(out.resolve())
  prefix='com.gameloft.android.GAND.GloftD2SS/files/';entries={i.filename[len(prefix):].lower():i for i in cache.infolist() if i.filename.lower().startswith(prefix.lower()) and not i.is_dir()}
  selections=[('swamp54',uri,count) for uri,count in occurrences.items()]
  selections.append(('swamp54','data/3d/modules/swamp/swamp.bdae',1))
  selections += [('crypt',uri,1) for uri in ['data/3d/modules/crypt/crypt.bdae','data/3d/characters/prince/prince_modular.bdae','data/3d/characters/skeleton/skeleton.bdae']]
  selections += [('target',r['uri'].lower(),1) for r in json.loads((root/'port/level-world/reports/target-art-v28/assets.json').read_text())]
  selections += [('fire',uri,1) for uri in ['data/3d/interface/spell_fire_hurt.bdae','data/3d/interface/spell_fire_prince_casting.bdae']]
  total_read=0
  def inspect(family,uri,count):
   entry=entries.get(uri.lower())
   if entry is None:raise RuntimeError('Exact census URI missing: '+uri)
   if entry.file_size>64*1024**2:raise RuntimeError('Single encoded census asset exceeds read bound')
   if sum(m['bytes'] for m in metadata)+entry.file_size>128*1024**2:raise RuntimeError('Census total encoded reads would exceed 128MiB')
   path=stage/(str(len(metadata))+'.asset');data=cache.read(entry);path.write_bytes(data)
   run=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',unix(exe),unix(path)],capture_output=True,text=True,timeout=10)
   if run.returncode:raise RuntimeError(uri+': '+run.stderr)
   row=json.loads(run.stdout);row.pop('input');row.update(family=family,uri=uri,instances=count,sha256=hashlib.sha256(data).hexdigest());rows.append(row)
   metadata.append({'uri':uri,'bytes':entry.file_size,'crc32':entry.CRC,'sha256':row['sha256']})
   path.unlink();return row
  for family,uri,count in selections:
   row=inspect(family,uri,count);total_read+=row['encoded_bytes']
   if total_read>128*1024**2:raise RuntimeError('Bounded census total reads exceeded 128MiB')
  images=collections.defaultdict(set)
  for row in rows:
   for image in row.get('images',[]):
    # Same scene-materials/scene.cpp Reader::image basename used by renderer
    # upload() and original-cache texture lookup; not ZIP URI normalization.
    key='data/3d/textures/'+image.replace('\\','/').rsplit('/',1)[-1].lower()
    if key in entries:images[key].add(row['family'])
    else:raise RuntimeError('Exact declared image URI missing: '+image)
  for uri,families in sorted(images.items()):
   row=inspect('texture',uri,1);row['referenced_by']=sorted(families);total_read+=row['encoded_bytes']
   if total_read>128*1024**2:raise RuntimeError('Bounded census total reads exceeded 128MiB')
 summaries={}
 for family in ['swamp54','crypt','target','fire']:
  models=[r for r in rows if r['family']==family];textures=[r for r in rows if family in r.get('referenced_by',[])]
  sums={key:sum(r.get(key,0) for r in models) for key in ['encoded_bytes','static_vbo_bytes','static_ebo_bytes','max_particle_vbo_bytes','max_particle_ebo_bytes']}
  sums.update(unique_models=len(models),entity_occurrences=54 if family=='swamp54' else sum(r['instances'] for r in models),module_occurrences=1 if family=='swamp54' else 0,unique_textures=len(textures),decoded_rgba_bytes=sum(r['decoded_rgba_bytes'] for r in textures),full_mip_rgba_bytes=sum(r['full_mip_rgba_bytes'] for r in textures),texture_encoded_bytes=sum(r['encoded_bytes'] for r in textures))
  sums['occurrence_weighted_static_gpu_bytes']=sum(r['instances']*(r.get('static_vbo_bytes',0)+r.get('static_ebo_bytes',0)) for r in models)
  sums['shared_descriptor_gpu_bytes']=sum(sums[k] for k in ['static_vbo_bytes','static_ebo_bytes','max_particle_vbo_bytes','max_particle_ebo_bytes','full_mip_rgba_bytes'])
  sums['default_gpu256_texture128_fx64_fit']=sums['shared_descriptor_gpu_bytes']<=256*1024**2 and sums['full_mip_rgba_bytes']<=128*1024**2 and (family not in ['target','fire'] or sum(sums[k] for k in ['static_vbo_bytes','static_ebo_bytes','max_particle_vbo_bytes','max_particle_ebo_bytes'])<=64*1024**2)
  summaries[family]=sums
 result={'status':'PASS','scope':__doc__,'limits':'Engineering defaults, descriptor envelope only; shader/reflection, bone state, occurrence-specific allocations and simultaneous FX concurrency not fully censused','canonical_swamp_scope':'49 characters +5 chests, 10 unique model assets; constructed inspection prefix, whole activation unverified','blocked':blocked,'loader_packet_sha256':hashlib.sha256(args.loader_packet.read_bytes()).hexdigest(),'loader_xml_sha256':hashlib.sha256(xml).hexdigest(),'archive_bytes':args.cache.stat().st_size,'bounded_encoded_bytes_read':total_read,'rows':rows,'families':summaries,'sources_sha256':{s:hashlib.sha256((root/s).read_bytes()).hexdigest() for s in sources},'compile':command}
 (out/'asset-census.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':'PASS','families':summaries,'assets':len(rows),'encoded_bytes_read':total_read}))
