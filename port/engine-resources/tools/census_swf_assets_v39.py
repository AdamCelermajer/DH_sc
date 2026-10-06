"""Bounded original UI catalog descriptors; no pixels, GL or device access."""
from pathlib import Path
import hashlib,json,re,subprocess,tempfile,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/swf-resource-v39'
catalog=root/'port/android-native/app/src/main/cpp/original_ui_asset_catalog.inc'
exe=root/'port/engine-resources/reports/resource-budget-v37/asset-census'
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
declared=re.findall(r'\{"([^"]+)","([^"]+)","([^"]+)","([^"]+)",(\d+)u\}',catalog.read_text())
cache=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
rows=[];read_bytes=0
with zipfile.ZipFile(cache) as z,tempfile.TemporaryDirectory(dir=out) as temporary:
 stage=Path(temporary);assert stage.resolve().is_relative_to(out.resolve())
 entries={p.filename.lower():p for p in z.infolist()}
 for key,uri,asset,digest,length in declared:
  if not key.endswith('.tga'):continue
  member=entries['com.gameloft.android.gand.gloftd2ss/files/'+uri.lower()]
  assert member.file_size==int(length) and member.file_size<=32*1024**2
  if read_bytes+member.file_size>64*1024**2:raise RuntimeError('UI descriptor census read cap would be exceeded')
  raw=z.read(member);read_bytes+=len(raw);assert hashlib.sha256(raw).hexdigest()==digest
  path=stage/'asset.bin';path.write_bytes(raw)
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',unix(exe),unix(path)],capture_output=True,text=True,timeout=10)
  if p.returncode:raise RuntimeError(uri+': '+p.stderr)
  row=json.loads(p.stdout);row.pop('input');assert row['kind']=='texture';row.update(uri=uri,sha256=digest);rows.append(row)
rgba=sum(r['decoded_rgba_bytes'] for r in rows)
targets=[]
for w,h in [(960,540),(1280,720),(1920,1080),(2712,1220),(3200,1440)]:
 # Both UI owners can retain targets. Three storages per owner; two are textures.
 targets.append({'width':w,'height':h,'one_owner_target_gpu_bytes':w*h*12,'both_owner_target_gpu_bytes':w*h*24,'both_owner_target_texture_bytes':w*h*16,'single_catalog_plus_both_targets_gpu_bytes':rgba+w*h*24,'single_catalog_plus_both_targets_texture_bytes':rgba+w*h*16,'duplicated_catalog_plus_both_targets_gpu_bytes':2*rgba+w*h*24,'duplicated_catalog_plus_both_targets_texture_bytes':2*rgba+w*h*16,'default_gpu256_texture128_static_duplicated_fit':2*rgba+w*h*24<=256*1024**2 and 2*rgba+w*h*16<=128*1024**2})
result={'status':'PASS','scope':__doc__,'bounded_encoded_bytes_read':read_bytes,'all_catalog_variant_textures':len(rows),'all_catalog_rgba_base_bytes':rgba,'largest_decoded_image_bytes':max(r['decoded_rgba_bytes'] for r in rows),'default_cpu_individual64_and_gpu_individual128_fit':max(r['decoded_rgba_bytes'] for r in rows)<=64*1024**2,'duplicated_retained_plus_one_decode_transient_cpu_envelope_bytes':2*rgba+max(r['decoded_rgba_bytes'] for r in rows),'representative_targets':targets,'rows':rows,'catalog_sha256':hashlib.sha256(catalog.read_bytes()).hexdigest(),'census_binary_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limits':'All catalog variants, single and duplicated front/gameplay copies. Not actual selected exports, generated glyphs, vertex caches, model/effect textures, driver residency or simultaneous resize peaks. Target dimensions are representative, not observed device dimensions.'}
(out/'ui-asset-census.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['status','all_catalog_variant_textures','all_catalog_rgba_base_bytes','largest_decoded_image_bytes','bounded_encoded_bytes_read','representative_targets']}))
