"""Source audit, not a loader or gameplay claim. Retains authored identities and bytes.

Procedural source candidates are counted once per distinct MGP/MVP URI, never
presented as generated runtime instances. Historical native assembly receipts are
joined by exact Level identity, with their original hashes and seeds.
"""
import argparse, collections, hashlib, json, pathlib, re, struct, zipfile
import xml.etree.ElementTree as ET

CACHE_SHA = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
PREFIX = 'com.gameloft.android.GAND.GloftD2SS/files/'
CONSTRUCTED = {'LevelConfig','Module','Block','Character','OpenableContainer',
 'SoundEmitter','QuestMoveInZone','TriggerZoneExitLevel','TriggerObject','Door',
 'CheckpointZone','AnimatedDecor','TriggerZone','Dummy','SpawnPoint','Decor',
 'DestructibleContainer'}

def sha(b): return hashlib.sha256(b).hexdigest()
def normalized(v): return v.replace('\\','/').lower()
class Reader:
 def __init__(self,b): self.b=b;self.at=0
 def take(self,n):
  if n<0 or self.at+n>len(self.b): raise ValueError('truncated binary at '+str(self.at))
  b=self.b[self.at:self.at+n];self.at+=n;return b
 def word(self): return struct.unpack('<i',self.take(4))[0]
 def byte(self): return self.take(1)[0]
 def text(self): return self.take(self.word()).decode('utf-8')
 def names(self):
  n=self.word()
  if n<0 or n>100000: raise ValueError('invalid array count')
  return [self.text() for _ in range(n)]
 def end(self):
  if self.at!=len(self.b):raise ValueError('unconsumed binary bytes')

class Cache:
 def __init__(self,path):
  with path.open('rb') as f:self.digest=hashlib.file_digest(f,'sha256').hexdigest()
  if self.digest!=CACHE_SHA:raise ValueError('original cache hash differs')
  self.z=zipfile.ZipFile(path);self.entries={};self.basename=collections.defaultdict(list)
  for e in self.z.infolist():
   if e.is_dir():continue
   if not e.filename.startswith(PREFIX):raise ValueError('unexpected cache prefix')
   authored=e.filename[len(PREFIX):];key=normalized(authored)
   if key in self.entries:raise ValueError('case-colliding cache URI')
   self.entries[key]=e;self.basename[pathlib.PurePosixPath(key).name].append(key)
 def raw(self,key):return self.z.read(self.entries[key])
 def resolve(self,authored,folder=None,kind=None):
  key=normalized(authored);candidates=[(key,'case_folded_exact')]
  if folder and not key.startswith('data/'):
   candidates.insert(0,(normalized(folder)+'/'+('mgp' if kind=='gameplay' else 'mvp')+'/'+key,'rule_folder_typed_file'))
  if key.startswith('data/iphone/'):candidates.append((key.replace('data/iphone/','data/',1),'recorded_legacy_iphone_alias'))
  for candidate,method in candidates:
   if candidate in self.entries:return {'authored':authored,'cache_uri':candidate,'method':method}
  base=self.basename[pathlib.PurePosixPath(key).name]
  if len(base)==1:return {'authored':authored,'cache_uri':base[0],'method':'unique_basename_audit_candidate'}
  return {'authored':authored,'cache_uri':None,'method':'unresolved','candidates':base}
 def provenance(self,key):
  b=self.raw(key);return {'cache_uri':key,'cache_authored_case':self.entries[key].filename[len(PREFIX):], 'sha256':sha(b),'bytes':len(b)}
 def names(self,key):
  r=Reader(self.raw(key));out=r.names();r.end();return out

def levels(cache):
 r=Reader(cache.raw('data/pydata/levels_pyarraynames.bin'));travel_names=r.names();names=r.names();r.end()
 r=Reader(cache.raw('data/pydata/levels_pyarray.bin'));travel=[]
 if r.word()!=len(travel_names):raise ValueError('travel count differs')
 for n in travel_names:travel.append({'name':n,'description_id':r.word(),'entrypoint':r.word(),'level':r.text(),'location_type':r.word(),'string_id':r.word()})
 if r.word()!=len(names):raise ValueError('level count differs')
 out=[]
 for index,n in enumerate(names):
  row={'index':index,'name':n,'stable':r.byte(),'description_resource':r.text(),'hub':r.word(),'random':r.byte(),'description_id':r.word(),'file':r.text()}
  for k in ['level_name_id','initial_level_state','map_name_id','max_normal','max_hard','max_nightmare','min_normal','min_hard','min_nightmare']:row[k]=r.word()
  out.append(row)
 r.end();return out,travel

def script_pair(cache,authored):
 stem=normalized(authored)
 if not stem.endswith('.pyscript'):return {'authored':authored,'status':'unsupported_script_path'}
 stem=stem[:-9];keys=[stem+'_pyscriptnames.bin',stem+'_pyscripts.bin']
 out={'authored':authored,'status':'binary_pair_available' if all(k in cache.entries for k in keys) else 'binary_pair_missing','payloads':[cache.provenance(k) for k in keys if k in cache.entries]}
 if keys[0] in cache.entries:
  out['names']=cache.names(keys[0]);out['name_count']=len(out['names'])
 if keys[1] in cache.entries:
  out['declared_script_count']=Reader(cache.raw(keys[1])).word()
  if 'name_count' in out and out['declared_script_count']!=out['name_count']:raise ValueError('script data/name counts differ')
 out['execution_verified']=False;out['script_opcodes_decoded_by_this_audit']=False
 return out

def load_receipt(root,name):
 p=root/name
 if not p.exists():return {},None
 raw=p.read_bytes();return json.loads(raw),{'path':str(p),'sha256':sha(raw)}

def audit(cache,receipt_root):
 table,travel=levels(cache);documents={};issues=[];all_objects=[]
 sources,sp=load_receipt(receipt_root,'procedural-sources-host.json')
 if sources and sources.get('cache_sha256')!=cache.digest:raise ValueError('historical procedural source receipt cache differs')
 def original_tree(tree):
  elem=ET.Element(tree['tag'],tree.get('attributes',{}))
  for child in tree.get('children',[]):elem.append(original_tree(child))
  return elem
 factory_text=(receipt_root.parent/'original_factory_table_v1.inc').read_text()
 factory=dict((n,int(a,16)) for n,a in re.findall(r'\{"([^"]+)", (0x[0-9a-f]+)u\}',factory_text))
 def doc(key):
  if key in documents:return documents[key]
  provenance=cache.provenance(key)
  try:node=ET.fromstring(cache.raw(key));error=None
  except ET.ParseError as e:node=None;error=str(e);issues.append({'cache_uri':key,'kind':'strict_xml_parse_failure','error':error,'native_tinyxml_partial_result_not_inferred':True})
  captured=sources.get('partial_xml_sources',{}).get(key)
  mode='strict_xml'
  if node is None and captured:
   if sources.get('authored_sources_sha256',{}).get(key)!=provenance['sha256']:raise ValueError('historical captured source hash differs')
   node=original_tree(captured['tree']);mode='historical_original_tinyxml_captured_tree'
   issues[-1]['historical_original_xml_error']=captured['original_xml_error'];issues[-1]['historical_original_error_description']=captured['original_error_description'];issues[-1]['tree_reused_from_hash_verified_receipt']=sp
  documents[key]={'provenance':provenance,'node':node,'strict_parse_error':error,'tree_mode':mode};return documents[key]
 fixed,fp=load_receipt(receipt_root,'source-pipeline-fixed-maps-host.json');procedural,pp=load_receipt(receipt_root,'source-pipeline-procedural-maps-host.json')
 for receipt in [fixed,procedural]:
  if receipt and receipt.get('cache_sha256')!=cache.digest:raise ValueError('historical map receipt cache differs')
 fm={v['name']:v for v in fixed.get('levels',[])};pm={v['identity']:v for v in procedural.get('levels',[])}
 global_names=cache.names('data/pydata/scripts_pyscriptnames.bin')
 condition_names=cache.names('data/pydata/v2conditions_pyarraynames.bin')
 quest_names=cache.names('data/pydata/v2quests_pyarraynames.bin')
 result=[]
 for raw in table:
  row={'identity':raw['name'],'design_row':raw,'form':'procedural' if raw['random'] else 'fixed','source_references':[],'scripts':[],'objects':[],'issues':[]}
  row['acceptance']={'map_mobs_chests_visible_current_build':False,'whole_level_init_verified':False,'quest_npc_cinematic_runtime_verified':False,'save_return_verified':False}
  if raw['random']:
   joined=pm.get(raw['name'],{});row['historical_native_map_assembly']={'receipt':pp,'runs':[{'seed':v.get('seed'),'status':v.get('status'),'reason':v.get('reason'),'module_count':v.get('result',{}).get('module_count'),'declaration_types':v.get('result',{}).get('declaration_types')} for v in joined.get('runs',[])]}
  else:
   joined=fm.get(raw['name'],{});row['historical_native_map_assembly']={'receipt':fp,'status':joined.get('map_preparation'),'reason':joined.get('reason'),'module_count':joined.get('result',{}).get('module_count')}
  ref=cache.resolve(raw['file']);row['definition']=ref
  if not ref['cache_uri']:row['issues'].append({'kind':'missing_level_definition'});result.append(row);continue
  definition=doc(ref['cache_uri']);node=definition['node'];row['definition']['provenance']=definition['provenance']
  if node is None:row['issues'].append({'kind':'strict_definition_parse_failure'});result.append(row);continue
  row['definition']['root_tag']=node.tag;row['definition']['tree_mode']=definition['tree_mode']
  def add_objects(source,context):
   source_doc=doc(source)
   if source_doc['node'] is None:return
   for child_index,obj in enumerate(source_doc['node']):
    if obj.tag!='GameObject':continue
    attrs=dict(obj.attrib);kind=attrs.get('gametype','<missing>')
    token={'level':raw['name'],'source_uri':source,'module_context':context,'direct_child_index':child_index,'authored_name':attrs.get('name',''),'authored_gametype':kind,'attributes':attrs,'factory':{'original_registered':kind in factory,'original_address':hex(factory[kind]) if kind in factory else None,'current_loader_constructor_family_present':kind in CONSTRUCTED,'initpost_or_runtime_verified_by_this_audit':False}}
    row['objects'].append(token);all_objects.append(token)
  if not raw['random']:
   add_objects(ref['cache_uri'],None)
   for child_index,obj in enumerate(node):
    if obj.tag!='GameObject' or obj.get('gametype') not in ('Module','Block'):continue
    context={'direct_child_index':child_index,'name':obj.get('name'),'attributes':dict(obj.attrib)}
    for kind in ['mgp','mvp']:
     if not obj.get(kind):row['issues'].append({'kind':'missing_module_file_property','module':context,'property':kind});continue
     source=cache.resolve(obj.get(kind));source.update({'kind':kind,'module_context':context});row['source_references'].append(source)
     if source['cache_uri']:add_objects(source['cache_uri'],context)
     else:row['issues'].append({'kind':'unresolved_module_file','reference':source})
  else:
   folder=node.get('folder');seen=set()
   for list_index,lst in enumerate(node.findall('list')):
    for elem_index,elem in enumerate(lst):
     context={'list_index':list_index,'list_name':lst.get('name'),'element_index':elem_index,'attributes':dict(elem.attrib)}
     for kind in ['gameplay','visual']:
      if not elem.get(kind):continue
      source=cache.resolve(elem.get(kind),folder,kind);source.update({'kind':kind,'rule_candidate_context':context});row['source_references'].append(source)
      if source['cache_uri'] and source['cache_uri'] not in seen:seen.add(source['cache_uri']);add_objects(source['cache_uri'],{'candidate_only':True,'source_uri':source['cache_uri']})
      elif not source['cache_uri']:row['issues'].append({'kind':'unresolved_rule_candidate','reference':source})
   row['procedural_object_count_semantics']='distinct candidate-file authored declarations; not generated instances'
  script_paths=[]
  if node.get('scriptFile'):script_paths.append(node.get('scriptFile'))
  script_paths.extend(o['attributes']['scriptFile'] for o in row['objects'] if o['attributes'].get('scriptFile'))
  row['scripts']=[script_pair(cache,s) for s in dict.fromkeys(script_paths)]
  names=set(global_names)
  for s in row['scripts']:names.update(s.get('names',[]))
  row['script_references']=[];row['condition_references']=[];row['npc_candidates']=[];row['data_references']=[];row['transition_references']=[]
  for o in row['objects']:
   attrs=o['attributes'];base={k:o[k] for k in ['source_uri','module_context','direct_child_index','authored_name','authored_gametype']}
   for k,v in attrs.items():
    if not v:continue
    if k.lower().startswith('script') and k!='scriptFile':row['script_references'].append({**base,'property':k,'value':v,'present_in_local_or_global_name_table':v in names,'execution_verified':False})
    if k in ('activate_cond','deactivate_cond','condition_desc'):row['condition_references'].append({**base,'property':k,'value':v,'present_in_condition_name_table':v in condition_names,'evaluated':False})
    if k in ('charpropsname','char_template','data_desc','questName'):row['data_references'].append({**base,'property':k,'value':v,'typed_runtime_binding_verified':False})
    if k in ('levelname','level_name','levelName','destination','entrypointID'):row['transition_references'].append({**base,'property':k,'value':v})
   if attrs.get('_templateName') in ('NPC','Faery') or 'NPC' in attrs.get('name',''):row['npc_candidates'].append({**base,'attributes':attrs,'classification':'authored_editor_template_or_name; not behavior proof'})
  row['object_counts']=dict(sorted(collections.Counter(o['authored_gametype'] for o in row['objects']).items()))
  row['constructor_family_gaps']=sorted((set(row['object_counts'])&set(factory))-CONSTRUCTED)
  row['unregistered_authored_types']=sorted(set(row['object_counts'])-set(factory))
  row['unregistered_type_policy']='unclassified authored declaration; original native routing unresolved, not presumed a new object constructor'
  if raw['random']:
   joined=pm.get(raw['name'],{});row['historical_native_map_assembly']={'receipt':pp,'runs':[{'seed':v.get('seed'),'status':v.get('status'),'reason':v.get('reason'),'module_count':v.get('result',{}).get('module_count'),'declaration_types':v.get('result',{}).get('declaration_types')} for v in joined.get('runs',[])]}
  else:
   joined=fm.get(raw['name'],{});row['historical_native_map_assembly']={'receipt':fp,'status':joined.get('map_preparation'),'reason':joined.get('reason'),'module_count':joined.get('result',{}).get('module_count')}
  row['acceptance']={'map_mobs_chests_visible_current_build':False,'whole_level_init_verified':False,'quest_npc_cinematic_runtime_verified':False,'save_return_verified':False}
  result.append(row)
 summary={'levels':len(result),'fixed':sum(r['form']=='fixed' for r in result),'procedural':sum(r['form']=='procedural' for r in result),'source_documents':len(documents),'strict_xml_parse_failures':len(issues),'source_reference_failures':sum(len(r['issues']) for r in result),'constructor_family_gap_types':sorted(set(t for r in result for t in r.get('constructor_family_gaps',[]))),'unclassified_authored_types':sorted(set(t for r in result for t in r.get('unregistered_authored_types',[])))}
 out={'schema':'dh2-loader-chapter-coverage-v36','cache_sha256':cache.digest,'claims':'Source audit and explicitly historical receipt joins only; no new native/device execution','level_table_provenance':[cache.provenance('data/pydata/'+n) for n in ['levels_pyarraynames.bin','levels_pyarray.bin','levels_pystructnames.bin']],'design_name_tables':{'global_script_names':global_names,'condition_names':condition_names,'quest_names':quest_names},'design_payloads':[cache.provenance(k) for k in cache.entries if k.startswith('data/pydata/') and any(pathlib.PurePosixPath(k).name.startswith(x) for x in ['v2quests_','v2conditions_','dialogs_','scripts_'])], 'travel':travel,'levels':result,'documents':[{'provenance':v['provenance'],'strict_parse_error':v['strict_parse_error'],'tree_mode':v['tree_mode']} for v in documents.values()],'issues':issues,'summary':summary}
 swamp=next(r for r in result if r['identity']=='SWAMP');swamp2=next(r for r in result if r['identity']=='SWAMP_02')
 counts=swamp['object_counts'].copy();counts.pop('LevelConfig',None);counts.pop('Module',None)
 if sum(counts.values())!=195 or counts.get('Character')!=50 or counts.get('OpenableContainer')!=5:raise ValueError('SWAMP authored-object regression')
 if len(result)!=51 or summary['fixed']!=16 or summary['procedural']!=35:raise ValueError('supplied-level catalog regression')
 if swamp['scripts'][0]['name_count']!=55:raise ValueError('SWAMP script-name regression')
 independent,ip=load_receipt(receipt_root,'swamp-original-object-inventory-v32.json')
 if independent and counts!=independent['types']:raise ValueError('independent SWAMP inventory type counts differ')
 out['validation']={'catalog_51_rows':True,'fixed_16_procedural_35':True,'swamp_module_objects_195_character_50_chests_5':True,'swamp_exact_55_script_names':True,'independent_swamp_inventory':ip,'independent_inventory_equal':bool(independent),'no_new_native_execution':True,'no_device_launch':True}
 out['current_constructor_snapshot']={'types':sorted(CONSTRUCTED),'scope':'Family presence only; neither complete InitPost nor activation inferred','sources':[{'path':str(receipt_root.parent/n),'sha256':sha((receipt_root.parent/n).read_bytes())} for n in ['canonical_auxiliary_families_v16.cpp','canonical_level_class_dispatch_v1.cpp']]}
 out['required_runtime_services']=[{'id':i,'owner':owner,'purpose':purpose,'status':'not_runtime_verified_by_audit'} for i,owner,purpose in [
 ('actual_registry','loader + main','All preserved authored names resolve to same canonical objects and module contexts; never a parallel scene dictionary'),
 ('initpost_activation','loader orchestration + main per-class services','Execute true lifecycle order, conditions, Room membership and active publication after resource preparation'),
 ('script_manager','main','Decode original binary script payloads, register exact local/global IDs, execute nested call/cinematic/NPC/camera/tutorial commands'),
 ('condition_quest','main','Use original ConditionTable and initialized Quest owners for activation, progress, dialogue and returning area gates'),
 ('character_cache_visual','main actor owner + loader asset resolution','Actual character cache13ca/13c8, same application RNG, owner418/current-faery provider; no fake owner/save'),
 ('save_restore','main Save/Profile','Deliver LNAM level/seed/act, LEPT entrypoint, LUSP spawn selection and QEST regular/volatile quest state to canonical owners'),
 ('object_persistence','main save + loader canonical object identity','Prove original checkpoint/container/trap/trigger restoration policy; no assumption that every authored object persists or whole level serialized'),
 ('transition','loader + main world/current GS','Preserve exit destination, entrypoint, fasttravel and activation condition; unload old resources before publishing actual next Level')]]
 out['swamp_return_boundary']={'identities':['SWAMP','SWAMP_02'],'different_design_rows':True,'same_level_name_id':swamp['design_row']['level_name_id']==swamp2['design_row']['level_name_id'],'same_definition':swamp['definition']['cache_uri']==swamp2['definition']['cache_uri'],'same_module_art_possible':True,'no_shared_runtime_or_save_identity_inferred':True,'loader_must_borrow':'actual player save level/seed/current-act and initialized Quest owners; preserve identity, procedural seed, module instance, authored object token','save_format_owned_by_main':True,'save_whole_level_serialization_claim':False}
 out['chapter1_acceptance_cases']=[{'id':'SWAMP-'+str(i+1),'case':case,'status':'pending_runtime_acceptance','owner':owner} for i,(case,owner) in enumerate([
 ('Load complete nine authored module pairs; show map, all active Characters and five authored chest placements','loader + main visual services'),
 ('Original Swamp_Intro, movement/combat tutorials and Faery actor resolve exact authored names after save/new-game selection','loader transport + main script/cinematic execution'),
 ('Camp_Intro and NPC interaction share actual registry, quest states and dialogue runtime','loader registry + main NPC/quest/dialogue'),
 ('tutorial_treasure, chest_tuto, cinematic_Tuto_levelUp and cinematic_Tuto_potionUse preserve original trigger/script identity','loader triggers + main execution/UI'),
 ('Exit, witch cave, checkpoint/reload and return entrypoint retain canonical level/module/object identities','loader transition + main save'),
 ('Quest completion, condition changes, chest/loot state, skills/items/faery state survive supported save reload','main save/quest/loot/skills; loader borrowed restoration'),
 ('Return as SWAMP_02 uses own rule, local scripts, application RNG seed and declarations despite shared art','loader identity/generation + main save'),
 ('SWAMP _prim_ExitLevelZone_toSwamp02: IsAfter_Gothicus2Survivors evaluates through actual Quest/Condition provider before transition to SWAMP_02 entrypoint 0','loader exact exit transport + main condition execution'),
 ('SWAMP_02 _prim_ExitLevelZone_toSwamp returns to SWAMP entrypoint 3 with a01_SWAMP_CAMP fasttravel; do not silently choose initial entrypoint 0','loader exact destination/entrypoint + main saved location')])]
 return out

def markdown(out):
 lines=['# Level and Chapter 1 coverage audit v36','','This is source coverage plus historical map-assembly receipts. It does not prove active objects, visible current-build rendering, cinematics, quests, or persistence.','','| Level identity | Form | Source declarations | Registered classes needing constructors | Unclassified authored types | Script pairs | Historical map assembly | Runtime acceptance |','|---|---|---:|---|---|---|---|---|']
 for r in out['levels']:
  h=r.get('historical_native_map_assembly',{});status=h.get('status') or ', '.join(str(x.get('seed'))+':'+str(x.get('status')) for x in h.get('runs',[]))
  lines.append('| '+r['identity']+' | '+r['form']+' | '+str(sum(r.get('object_counts',{}).values()))+' | '+(', '.join(r.get('constructor_family_gaps',[])) or 'none in source audit')+' | '+(', '.join(r.get('unregistered_authored_types',[])) or 'none')+' | '+', '.join(s['status'] for s in r.get('scripts',[]))+' | '+str(status)+' | pending |')
 lines+=['','## SWAMP acceptance cases','','| ID | Required behavior | Owner | Status |','|---|---|---|---|']
 for c in out['chapter1_acceptance_cases']:lines.append('| '+c['id']+' | '+c['case']+' | '+c['owner']+' | pending |')
 lines+=['','SWAMP is fixed design row 41, hub 0, normal level range 1–4. SWAMP_02 is procedural row 42, hub 1, normal range 20–22. Definitions, scripts, and level-name IDs differ. Shared art never implies a shared campaign or object state. The original save provider must supply level identity, seed, current act, initialized quest owners, and object-state restoration. This audit does not establish that the original game saves an entire scene.','','Strict XML failures remain in JSON with complete source hashes. For fifteen rule files the audit consumes existing original TinyXML captured trees only after checking their source hash against the current original cache; original parser errors remain explicit. Procedural counts represent distinct candidate files, not one generated layout. Script-path .pyscript references resolve to authored *_pyscriptnames.bin and *_pyscripts.bin pairs for inspection; the audit does not execute or decode script opcodes.','','## Runtime services still requiring acceptance','','| Service | Responsibility | Status |','|---|---|---|']
 for s in out['required_runtime_services']:lines.append('| '+s['id']+' | '+s['owner']+': '+s['purpose']+' | pending runtime proof |')
 return '\n'.join(lines)+'\n'

def main():
 p=argparse.ArgumentParser();p.add_argument('--cache',type=pathlib.Path,required=True);p.add_argument('--receipt-root',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args()
 out=audit(Cache(a.cache),a.receipt_root);out['audit_tool_sha256']=sha(pathlib.Path(__file__).read_bytes());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(out,indent=2)+'\n',encoding='utf8');a.output.with_suffix('.md').write_text(markdown(out),encoding='utf8')
 compact={k:out[k] for k in ['schema','cache_sha256','audit_tool_sha256','summary','validation','swamp_return_boundary','chapter1_acceptance_cases','required_runtime_services','current_constructor_snapshot','issues']}
 compact['levels']=[{**{k:r.get(k) for k in ['identity','design_row','form','definition','object_counts','constructor_family_gaps','unregistered_authored_types','historical_native_map_assembly','acceptance','issues']},'script_pairs':[{k:s[k] for k in s if k!='names'} for s in r.get('scripts',[])],'script_reference_count':len(r.get('script_references',[])),'unbound_script_names':sorted(set(v['value'] for v in r.get('script_references',[]) if not v['present_in_local_or_global_name_table']))} for r in out['levels']]
 a.output.with_name(a.output.stem+'-summary.json').write_text(json.dumps(compact,indent=2)+'\n',encoding='utf8');print(json.dumps(out['summary']))
if __name__=='__main__':main()
