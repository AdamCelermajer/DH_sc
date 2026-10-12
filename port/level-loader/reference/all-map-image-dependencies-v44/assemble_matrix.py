from pathlib import Path
import json,zipfile,collections,hashlib,struct,shutil,csv
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=p/'reference/all-map-image-dependencies-v44';plan=json.loads((r/'dependency-plan.json').read_text());native=[json.loads(s) for s in (r/'native-asset-identities.jsonl').read_text().splitlines()];by_request={(a['uri'],a['module_selector']):a for a in native};assert len(by_request)==len(native)
cache=Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');z=zipfile.ZipFile(cache);prefix='com.gameloft.android.gand.gloftd2ss/files/';directory={n[len(prefix):].lower().replace('\\','/'):n for n in z.namelist() if n.lower().startswith(prefix) and not n.endswith('/')}
def compatibility_uri(authored):
 key=authored.lower().replace('\\','/')
 if key.startswith('q:/data/iphone/'):key='data/'+key[len('q:/data/iphone/'):]
 elif not key.startswith('data/'):return None
 if any(x in key for x in ['\0',':']) or any(part in ['','..'] for part in key.split('/')):return None
 return key
def classify(s):
 if s['status']=='source_no_image':return s
 canonical=s.get('canonical_compatibility_uri')
 if canonical is None:canonical=compatibility_uri(s['authored_uri'])
 result={**s}
 if canonical is None:result['status']='original_mount_route_required';result['canonical_policy_applied']=False;return result
 result['canonical_compatibility_uri']=canonical;result['canonical_policy']='explicit_supplied_cache_v43_compatibility_only'
 if canonical in directory:result['status']='compatibility_canonical_resolved';result['archive_entry_present']=True;return result
 folder,_,name=canonical.rpartition('/')
 variants=[folder+'/'+prefix+name for prefix in ['pvr2_','pvr_','atc_','etc_','dds_'] if folder+'/'+prefix+name in directory]
 result['status']='archive_entry_absent_for_compatibility_canonical_uri';result['archive_entry_present']=False;result['original_mount_format_route_verified']=False
 result['possible_same_directory_format_variants']=variants;result['variants_are_diagnostic_not_resolved_fallback']=True
 result['route_class']='original_format_route_required' if variants else 'exact_compatibility_entry_absent_no_named_variant'
 return result
def raw_materials(uri):
 # Independent bounded source-byte projection. Used only for valid BRES with
 # no geometry accepted by the current native Scene domain; never a Visual.
 b=z.read(directory[uri]);root=struct.unpack_from('<I',b,32)[0]
 def word(p):
  if p<0 or p+4>len(b):raise ValueError('sampler field outside actual BRES')
  return struct.unpack_from('<I',b,p)[0]
 def text(p):
  if not p or p>=len(b):raise ValueError('actual material/image string missing')
  end=b.find(b'\0',p,min(p+4096,len(b)))
  if end<0:raise ValueError('actual string unbounded')
  return b[p:end].decode()
 materials=[];n=word(root+0x5c);base=word(root+0x60)
 if n>4096:raise ValueError('material catalog outside proven domain')
 for i in range(n):
  q=base+36*i;material={'catalog':i,'id':text(word(q)),'status':'decoded_raw_original_bres_catalog_only','samplers':[]}
  for j in range(word(q+16)):
   param=word(q+20)+24*j
   if word(param+8)!=11:continue
   if word(param+12)!=1 or word(word(param+16))!=1:raise ValueError('unsupported scalar sampler array')
   index=word(word(word(param+20)));s={'parameter_name':text(word(param)),'parameter_record':param,'parameter_order':j,'image_catalog':index,'image_record':0,'image_id':'','authored_uri':''}
   if index==0xffffffff:s['status']='source_no_image'
   else:
    if index>=word(root+0x4c):raise ValueError('image catalog index outside actual BRES')
    image=word(root+0x50)+20*index;s.update(image_record=image,image_id=text(word(image)),authored_uri=text(word(image+8)),status='authored_image_pending_classification')
   material['samplers'].append(classify(s))
  materials.append(material)
 return materials,word(root+0x64),word(root+0x68)
raw_cache={};comparison_count=0
for asset in native:
 uri=asset['uri']
 if uri not in raw_cache:raw_cache[uri]=raw_materials(uri)
 if asset['status']!='decoded_authored_asset_scene':continue
 raw,_,_=raw_cache[uri];assert len(raw)==len(asset['materials'])
 for a,b in zip(raw,asset['materials']):
  assert a['id']==b['id'];assert len(a['samplers'])==len(b['samplers'])
  for x,y in zip(a['samplers'],b['samplers']):
   for key in ['parameter_name','parameter_record','parameter_order','image_catalog','image_record','image_id','authored_uri']:assert x[key]==y[key],(uri,key,x,y)
   comparison_count+=1
asset_rows=[]
for a in native:
 out={**a};out['material_evidence']='actual_native_scene_bres_pair';out['runtime_scene_or_pass_claim']=False;out['instance_scope']='authored_selected_module_subtree' if a['module_selector'] else 'authored_asset_scene_actor_selector_not_applied'
 if a['status']=='unsupported_native_scene_domain' and a.get('error')=='Scene has no visible geometry':
  mats,geometry_count,_=raw_cache[a['uri']];out['materials']=mats;out['instances']=[];out['material_evidence']='independent_bounded_original_bres_catalog_only';out['geometry_catalog_count']=geometry_count;out['status']='nongeometry_or_nonvisual_source';out['emitter_invisible_or_renderer_services_required']=True
 for m in out.get('materials',[]):m['samplers']=[classify(s) for s in m.get('samplers',[])]
 asset_rows.append(out)
by_request={(a['uri'],a['module_selector']):a for a in asset_rows};global_counts=collections.Counter();levels=[];problem_rows=[];per_instance_rows=0;unique_absences={};unique_unresolved={}
def dependency(d,index):
 global per_instance_rows
 uri=d.get('cache_bres_uri');selector=d.get('authored_visual_selector','') if d.get('authored_gametype')=='Module' else ''
 if uri is None:return {'dependency_index':index,**d,'status':'original_bres_asset_route_required','image_rows':[]}
 a=by_request[(uri,selector)];result={'dependency_index':index,**d,'asset_identity_key':[uri,selector],'status':a['status'],'instance_scope':a['instance_scope'],'material_evidence':a['material_evidence'],'image_rows':[]}
 mats={m['catalog']:m for m in a.get('materials',[])}
 if a.get('instances'):
  for inst in a['instances']:
   for slot in inst['material_slots']:
    m=mats[slot['material_catalog']]
    for s in m.get('samplers',[]):
     per_instance_rows+=1;result['image_rows'].append({'instance_index':inst['instance_index'],'node':inst['node'],'node_name':inst['node_name'],'material_slot':slot['slot'],'material_catalog':m['catalog'],'material_id':m['id'],**s})
 else:
  for m in mats.values():
   for s in m.get('samplers',[]):result['image_rows'].append({'instance_index':None,'material_catalog':m['catalog'],'material_id':m['id'],'scope':'source_material_catalog_no_geometry_instance',**s})
 result['bound_materials_without_samplers']=sum(1 for inst in a.get('instances',[]) for slot in inst['material_slots'] if not mats[slot['material_catalog']].get('samplers'))
 return result
for level in plan['levels']:
 row={k:level[k] for k in ['identity','design_index','form','definition','scope','unbound_model_producers','source_issues','selected_seed_or_activation_claim']};row['dependencies']=[dependency(d,i) for i,d in enumerate(level['dependencies'])];counts=collections.Counter();dependency_states=collections.Counter()
 for d in row['dependencies']:
  dependency_states[d['status']]+=1
  for s in d['image_rows']:
   counts[s['status']]+=1;global_counts[s['status']]+=1
   if s['status'] not in ['source_no_image','compatibility_canonical_resolved']:
    problem={'level':level['identity'],'scope':level['scope'],'source_uri':d.get('source_uri'),'authored_name':d.get('authored_name'),'authored_bres_uri':d['authored_bres_uri'],'cache_bres_uri':d['cache_bres_uri'],'module_selector':d.get('authored_visual_selector'),'runtime_failure_inferred':False,**s};problem_rows.append(problem)
    if s['status']=='archive_entry_absent_for_compatibility_canonical_uri':unique_absences[s['canonical_compatibility_uri']]=s
    else:unique_unresolved[s['authored_uri']]=s
 row['counts']={'dependencies':len(row['dependencies']),'image_rows':sum(counts.values()),'image_statuses':dict(counts),'dependency_statuses':dict(dependency_states),'actual_model_provider_required':len(row['unbound_model_producers'])};levels.append(row)
inspections=[]
for index,d in enumerate(plan['inspection_only_models']):
 inspections.append(dependency({**d,'authored_name':d['attributes']['name'],'authored_gametype':d['attributes']['kind'],'authored_visual_selector':''},index))
result={'schema':'all-map-image-dependency-matrix-v44','validation':'PASS_SOURCE_AUDIT','scope':'51 design identities; fixed authored declarations and procedural source potentials only, exact native selected Module subtree where selector provided; actor visual selector/activation/pass/technique sampler use unresolved','claims':{'whole_level_init':False,'actual_actor_rendering':False,'gpu_texture_readiness':False,'selected_seed_or_activate_conditions_evaluated':False,'resource_admission_accounting':False,'original_mount_route_recovered':False,'no_global_basename_fallback':True},'cache_sha256':plan['cache_sha256'],'coverage_sha256':plan['coverage_sha256'],'native_receipt_sha256':hashlib.sha256((r/'native-receipt.json').read_bytes()).hexdigest(),'native_vs_independent_sampler_identity_comparisons':comparison_count,'asset_identity_records':asset_rows,'levels':levels,'inspection_only_swamp_models':inspections,'summary':{'levels':len(levels),'unique_bres':len(plan['unique_bres_uris']),'native_asset_selector_queries':len(native),'dependencies':sum(l['counts']['dependencies'] for l in levels),'image_rows':sum(global_counts.values()),'image_statuses':dict(global_counts),'per_instance_sampler_rows_including_inspections':per_instance_rows,'exact_compatibility_absent_unique_uris':len(unique_absences),'unresolved_lexical_uri_route_unique':len(unique_unresolved),'original_format_route_required_unique':sum(bool(s['possible_same_directory_format_variants']) for s in unique_absences.values()),'model_provider_required':sum(len(l['unbound_model_producers']) for l in levels),'native_raw_sampler_comparisons':comparison_count,'malformed_bres_count':sum(a['status']=='bres_open_rejected' for a in asset_rows),'nongeometry_nonvisual_queries':sum(a['status']=='nongeometry_or_nonvisual_source' for a in asset_rows),'missing_module_selector_queries':sum(a['status']=='source_module_selector_not_found' for a in asset_rows)}}
assert len(levels)==51 and sorted(l['design_index'] for l in levels)==list(range(51));assert sum(l['form']=='fixed' for l in levels)==16;assert not any(l['selected_seed_or_activation_claim'] for l in levels)
for s in unique_absences.values():assert s['canonical_compatibility_uri'] not in directory
for a in asset_rows:
 for m in a.get('materials',[]):
  for s in m.get('samplers',[]):
   if s['status']=='compatibility_canonical_resolved':assert s['canonical_compatibility_uri'] in directory
# Normalize repeated positive evidence instead of duplicating92005 joined rows.
# Every dependency retains its exact source/asset selector; the immutable native
# table retains each instance slot/material/sampler/full URI. No name fallback.
for l in levels:
 for d in l['dependencies']:
  rows=d.pop('image_rows');d['joined_image_row_count']=len(rows)
  d['image_identity_mapping']='asset_identity_key -> instances[].material_slots[].material_catalog -> materials[catalog].samplers[]; nongeometry catalog has instance=null'
for d in inspections:
 rows=d.pop('image_rows');d['joined_image_row_count']=len(rows);d['image_identity_mapping']='asset_identity_key -> instances[].material_slots[].material_catalog -> materials[catalog].samplers[]; nongeometry catalog has instance=null'
(p/'reports/all-map-image-dependencies-v44.json').write_text(json.dumps(result,indent=2)+'\n');(r/'problem-rows.json').write_text(json.dumps(problem_rows,indent=2)+'\n');(r/'exact-archive-absences.json').write_text(json.dumps(list(unique_absences.values()),indent=2)+'\n')
compact={'schema':result['schema'],'validation':result['validation'],'scope':result['scope'],'claims':result['claims'],'summary':result['summary'],'levels':[{k:l[k] for k in ['identity','design_index','form','counts']} for l in levels],'exact_archive_absences':list(unique_absences.values()),'source_domain_queries':[a for a in asset_rows if a['status']!='decoded_authored_asset_scene']}
(p/'reports/all-map-image-dependencies-v44-summary.json').write_text(json.dumps(compact,indent=2)+'\n')
with (p/'reports/all-map-image-dependencies-v44.csv').open('w',newline='') as f:
 fields=['level','form','dependencies','image_rows','resolved','source_no_image','exact_archive_absent','mount_route_required','nongeometry_or_nonvisual','missing_module_selector','model_provider_required'];w=csv.DictWriter(f,fieldnames=fields);w.writeheader()
 for l in levels:
  c=l['counts'];s=c['image_statuses'];d=c['dependency_statuses'];w.writerow(dict(level=l['identity'],form=l['form'],dependencies=c['dependencies'],image_rows=c['image_rows'],resolved=s.get('compatibility_canonical_resolved',0),source_no_image=s.get('source_no_image',0),exact_archive_absent=s.get('archive_entry_absent_for_compatibility_canonical_uri',0),mount_route_required=s.get('original_mount_route_required',0),nongeometry_or_nonvisual=d.get('nongeometry_or_nonvisual_source',0),missing_module_selector=d.get('source_module_selector_not_found',0),model_provider_required=c['actual_model_provider_required']))
print(json.dumps(result['summary'],indent=2));print('absent canonical URIs',list(unique_absences));print('SWAMP',next(l for l in levels if l['identity']=='SWAMP')['counts'])
