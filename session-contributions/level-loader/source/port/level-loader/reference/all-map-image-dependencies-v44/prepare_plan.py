from pathlib import Path
import json,hashlib,zipfile,xml.etree.ElementTree as ET,collections,shutil
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');out=p/'reference/all-map-image-dependencies-v44';out.mkdir(parents=True,exist_ok=True)
source=p/'reports/chapter-coverage-v36.json';coverage=json.loads(source.read_text());cache=Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');z=zipfile.ZipFile(cache);prefix='com.gameloft.android.gand.gloftd2ss/files/'
directory={n[len(prefix):].lower().replace('\\','/'):n for n in z.namelist() if n.lower().startswith(prefix) and not n.endswith('/')};assert len(directory)==6833
with cache.open('rb') as f:sha=hashlib.file_digest(f,'sha256').hexdigest()
assert sha==coverage['cache_sha256'];provenance={d['provenance']['cache_uri']:d['provenance'] for d in coverage['documents']}
partial_path=p/'reports/procedural-sources-host.json';partial=json.loads(partial_path.read_text());all_assets=set();levels=[]
def normalized(s):return s.replace('\\','/').lower()
def resolve_asset(s):
 key=normalized(s)
 if key in directory:return key,'case_folded_exact'
 if key.startswith('data/iphone/'):
  alias='data/'+key[len('data/iphone/'):]
  if alias in directory:return alias,'explicit_level_source_iphone_alias'
 return None,'original_asset_route_required'
def tree(node):
 e=ET.Element(node['tag'],node.get('attributes',{}))
 for c in node.get('children',[]):e.append(tree(c))
 return e
def xml(key):
 raw=z.read(directory[key]);got=hashlib.sha256(raw).hexdigest()
 if key in provenance:assert got==provenance[key]['sha256']
 try:return ET.fromstring(raw),'strict_xml',got
 except ET.ParseError:
  captured=partial.get('partial_xml_sources',{}).get(key)
  if not captured:return None,'strict_xml_unavailable',got
  assert partial['authored_sources_sha256'][key]==got
  return tree(captured['tree']),'hash_verified_original_tinyxml_capture',got
for level in coverage['levels']:
 deps=[];issues=[];unbound=[];seen=set();identity=level['identity'];scope='procedural_source_candidate' if level['form']=='procedural' else 'fixed_authored_declaration'
 def add(asset,context,method):
  if not asset:return
  key,route=resolve_asset(asset);entry={'authored_bres_uri':asset,'cache_bres_uri':key,'bres_resolution':route,'scope':scope,'producer':method,**context}
  token=json.dumps(entry,sort_keys=True)
  if token in seen:return
  seen.add(token);deps.append(entry)
  if key:all_assets.add(key)
 for obj in level['objects']:
  attrs=obj['attributes'];ctx={k:obj.get(k) for k in ['source_uri','module_context','direct_child_index','authored_name','authored_gametype']};ctx['authored_visual_selector']=attrs.get('xrefobject','')
  for k in ['dae','skybox']:
   if attrs.get(k):add(attrs[k],{**ctx,'property':k},'authored_typed_visual_property')
  if obj['authored_gametype'] in ['Character','OpenableContainer','DestructibleContainer','Door'] and not attrs.get('dae'):
   unbound.append({**ctx,'scope':scope,'status':'actual_model_provider_required','authored_model_inputs':{k:attrs[k] for k in ['charpropsname','char_template','data_desc','_templateName'] if k in attrs},'runtime_visual_missing_inferred':False})
 if level['form']=='procedural':
  definition=level['definition'].get('cache_uri');node,mode,digest=xml(definition) if definition in directory else (None,'definition_unavailable',None)
  if node is None:issues.append({'status':mode,'source_uri':definition})
  else:
   folder=node.get('folder','');sky=node.get('skybox')
   if sky:add(sky,{'source_uri':definition,'source_sha256':digest,'source_tree_mode':mode,'property':'skybox','authored_gametype':'LevelConfig','authored_name':'rule_config','authored_visual_selector':''},'authored_rule_skybox')
   contexts={json.dumps(r['rule_candidate_context'],sort_keys=True):r['rule_candidate_context'] for r in level['source_references'] if r.get('rule_candidate_context')}
   for ctx in contexts.values():
    # Source project_procedural_module_v1 uses tile.block name. Its list
    # element.block_name is compared to that SAME block name before selection.
    block=ctx['attributes'].get('name','');mvx=normalized(folder+'/mvx/'+block+'.mvx')
    if mvx not in directory:issues.append({'status':'optional_mvx_absent','rule_candidate_context':ctx,'mvx_uri':mvx,'runtime_failure_inferred':False});continue
    mvnode,mvmode,mvsha=xml(mvx)
    first=next((n for n in list(mvnode) if n.tag=='GameObject'),None) if mvnode is not None and mvnode.tag=='Module' else None
    if first is None:issues.append({'status':'first_module_gameobject_unavailable','rule_candidate_context':ctx,'mvx_uri':mvx});continue
    add(first.get('dae'),{'source_uri':mvx,'source_sha256':mvsha,'source_tree_mode':mvmode,'authored_name':first.get('name',''),'authored_gametype':'Module','authored_visual_selector':first.get('xrefobject',''),'rule_candidate_context':ctx,'property':'dae'},'source_generated_module_first_mvx_gameobject_property')
 levels.append({'identity':identity,'design_index':level['design_row']['index'],'form':level['form'],'definition':level['definition'],'dependencies':deps,'unbound_model_producers':unbound,'source_issues':issues,'scope':scope,'selected_seed_or_activation_claim':False})
inspection_file=p/'reports/character-swamp-model-v40/character-swamp-model-v40.xml';inspections=[]
if inspection_file.exists():
 for entity in ET.fromstring(inspection_file.read_text()):
  if entity.tag!='Entity' or not entity.get('asset'):continue
  key,route=resolve_asset(entity.get('asset'));inspections.append({'identity':'SWAMP','scope':'isolated_real_factory_model_inspection_only','actual_production_visual_claim':False,'authored_bres_uri':entity.get('asset'),'cache_bres_uri':key,'bres_resolution':route,'attributes':dict(entity.attrib)})
  if key:all_assets.add(key)
plan={'schema':'all-map-image-dependency-plan-v44','scope':'source potential dependencies; procedural rule candidates and fixed authored declarations, no actual pass/activation/render inference','cache_sha256':sha,'cache_files':len(directory),'coverage_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'coverage_path':str(source),'procedural_captured_xml_receipt_sha256':hashlib.sha256(partial_path.read_bytes()).hexdigest(),'source_producers_sha256':{f:hashlib.sha256((p/f).read_bytes()).hexdigest() for f in ['procedural_modules_v1.cpp','procedural_layout_v1.cpp']},'levels':levels,'inspection_only_models':inspections,'inspection_receipt_sha256':hashlib.sha256(inspection_file.read_bytes()).hexdigest(),'unique_bres_uris':sorted(all_assets)}
(out/'dependency-plan.json').write_text(json.dumps(plan,indent=2)+'\n');(out/'bres-uris.txt').write_text('\n'.join(sorted(all_assets))+'\n');shutil.copy(__file__,out/'prepare_plan.py')
queries={(d['cache_bres_uri'],d['authored_visual_selector'] if d.get('authored_gametype')=='Module' else '') for l in levels for d in l['dependencies'] if d['cache_bres_uri']}
queries.update((i['cache_bres_uri'],'') for i in inspections if i['cache_bres_uri'])
with (out/'bres-inputs.txt').open('w',newline='\n') as f:f.write('\n'.join(uri+'\t'+selector for uri,selector in sorted(queries))+'\n')
print('native asset/selected-module queries',len(queries))
print(json.dumps({'levels':len(levels),'dependencies':sum(len(l['dependencies']) for l in levels),'unique_bres':len(all_assets),'unbound_model_producers':sum(len(l['unbound_model_producers']) for l in levels),'source_issues':sum(len(l['source_issues']) for l in levels),'inspection_only_assets':len(inspections),'swamp_dependencies':len(next(l for l in levels if l['identity']=='SWAMP')['dependencies'])}))
