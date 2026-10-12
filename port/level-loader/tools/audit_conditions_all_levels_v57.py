from pathlib import Path
import json,zipfile,hashlib,collections,xml.etree.ElementTree as ET
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=b/'reference/condition-list-v55';reports=b/'reports';output=reports/'condition-readiness-v57';output.mkdir(exist_ok=True);cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');inventory=reports/'chapter-coverage-v36.json';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
v=json.loads(inventory.read_text());table=json.loads((ref/'decoded-conditions-v55.json').read_text())['rows'];rows={x['name']:(i,x) for i,x in enumerate(table)};assert len(rows)==len(table)==118
levels=[];unknowns=[];references=[];parse_errors=[];slot_totals=collections.Counter();unique_names=set();type_counts=collections.Counter();other_domains=[]
with zipfile.ZipFile(cache) as z:
 prefix='com.gameloft.android.GAND.GloftD2SS/files/';files=set(z.namelist());document_cache={}
 for name in ['v2conditions_pyarray.bin','v2conditions_pyarraynames.bin']:
  actual=z.read(prefix+'data/pydata/'+name);assert actual==(ref/name).read_bytes(),name+' differs from actual source cache'
 def document(uri):
  if uri not in document_cache:
   name=prefix+uri
   if name not in files:document_cache[uri]=(None,'Missing exact source cache URI')
   else:
    raw=z.read(name)
    try:document_cache[uri]=(ET.fromstring(raw),None)
    except ET.ParseError as e:document_cache[uri]=(None,str(e))
  return document_cache[uri]
 for level in v['levels']:
  objects=level['objects'];counts=collections.Counter();refs=[];issues=[];by_uri={}
  for obj in objects:
   uri=obj['source_uri'];root,error=document(uri)
   if error:
    if uri not in by_uri:issues.append({'source_uri':uri,'error':error});by_uri[uri]=None
   elif uri not in by_uri:
    by_uri[uri]=collections.Counter((x.attrib.get('name',''),x.attrib.get('gametype',''),k,x.attrib[k]) for x in root.iter() for k in ['activate_cond','deactivate_cond'] if k in x.attrib)
   for k,value in obj['attributes'].items():
    if k not in ['activate_cond','deactivate_cond']:
     if 'cond' in k.lower() and value:other_domains.append({'level':level['identity'],'source_uri':uri,'object':obj['authored_name'],'property':k,'value':value,'classification':'separate authored domain; not automatically ConditionData8c/b0'})
     continue
    if by_uri.get(uri) is not None:
     assert by_uri[uri][(obj['authored_name'],obj['authored_gametype'],k,value)]>0,'Inventory authored value differs from original XML: '+str((level['identity'],uri,obj['authored_name'],k,value))
    found=rows.get(value);status='empty' if value=='' else 'Invalid' if value=='Invalid' else 'resolved' if found else 'missing';counts[status]+=1;counts[k]+=1;slot_totals[status]+=1
    item={'level':level['identity'],'form':level['form'],'source_uri':uri,'module_context':obj['module_context'],'direct_child_index':obj['direct_child_index'],'object':obj['authored_name'],'gametype':obj['authored_gametype'],'property':k,'source_offset':'0x8c' if k=='activate_cond' else '0xb0','value':value,'lookup_status':status,'original_XML_attribute_verified':by_uri.get(uri) is not None,'row_index':found[0] if found else None,'stub_ops':[s['op'] for s in found[1]['stubs']] if found else [],'operator_constructor_available':bool(found and all(0<=s['op']<7 for s in found[1]['stubs'])),'original_factory_registered':obj['factory'].get('original_registered',False),'source_choice_or_generation_verified':False,'runtime_Eval_or_activation_verified':False}
    if item['original_factory_registered'] is False:item['authored_route_note']='unclassified declaration; not a proven missing native constructor'
    if found:
     unique_names.add(value);type_counts.update(item['stub_ops'])
     if not item['operator_constructor_available']:unknowns.append(item)
    elif status=='missing':unknowns.append(item)
    references.append(item);refs.append(len(references)-1)
  definition=level.get('definition',{});source_uri=definition.get('cache_uri')
  if source_uri:
   _,error=document(source_uri)
   if error and source_uri not in by_uri:issues.append({'source_uri':source_uri,'error':error})
  # Preserve inherited source-audit issues: procedural generation/native
  # factories, parse failures and authored declarations are distinct domains.
  inherited=level.get('issues',[])
  levels.append({'identity':level['identity'],'form':level['form'],'raw_file':level['design_row'].get('file'),'definition_cache_uri':source_uri,'inventoried_object_count':len(objects),'counts':dict(counts),'reference_indices':refs,'condition_name_table_and_constructor_readiness':'ready for source transport' if not counts['missing'] and not any(not references[i]['operator_constructor_available'] for i in refs if references[i]['lookup_status']=='resolved') else 'explicit gap','original_source_parse_issues':issues,'inherited_definition_issue_count':len(inherited),'source_issues':inherited,'generated_or_selected_objects_verified':False,'native_qualified_parent_bound_on_this_level':False,'rendered_or_playable':False})
  parse_errors.extend({'level':level['identity'],**x} for x in issues)
miss_groups=collections.Counter(x['value'] for x in unknowns)
summary={'schema':'dh2-all-level-condition-readiness-v57','source_cache_sha256':sha(cache),'inventory_sha256':sha(inventory),'condition_data_sha256':sha(ref/'v2conditions_pyarray.bin'),'condition_names_sha256':sha(ref/'v2conditions_pyarraynames.bin'),'level_count':len(levels),'forms':dict(collections.Counter(x['form'] for x in levels)),'declared_slot_counts':dict(slot_totals),'original_XML_verified_declared_slots':sum(x['original_XML_attribute_verified'] for x in references),'inventory_only_slots_in_unparsed_original_XML':sum(not x['original_XML_attribute_verified'] for x in references),'referenced_unique_condition_names':len(unique_names),'used_constructor_op_counts':dict(type_counts),'explicit_condition_gaps':len(unknowns),'unique_exact_missing_name_counts':dict(miss_groups),'original_XML_parse_or_missing_issues':len(parse_errors),'other_condition_domains':len(other_domains),'source_transport_only':True,'all_level_runtime_Eval_or_rendering_claim':False}
assert len(levels)==51 and summary['source_cache_sha256']==v['cache_sha256']
(output/'all-level-condition-readiness.json').write_text(json.dumps({'summary':summary,'levels':levels,'references':references,'explicit_gaps':unknowns,'XML_issues':parse_errors,'other_domains':other_domains},indent=2)+'\n')
md='# All51 level condition readiness — source transport only\n\n'+f"Checked all51 supplied level identities against actual ZIP XML attributes and the real118-row Conditions table. {len(references)} authored activate/deactivate slots reference {len(unique_names)} distinct names; {len(unknowns)} explicit name/operator gaps. Original source parse/missing issues remain separate. This audit does not choose procedural objects, execute Eval, or prove rendering/playability.\n\n"
md+='| Level | Form | Slots | Resolved | Empty/Invalid | Missing | Source XML issues |\n|---|---|---:|---:|---:|---:|---:|\n'
for l in levels:
 c=l['counts'];md+=f"| {l['identity']} | {l['form']} | {sum(c.get(k,0) for k in ['resolved','empty','Invalid','missing'])} | {c.get('resolved',0)} | {c.get('empty',0)+c.get('Invalid',0)} | {c.get('missing',0)} | {len(l['original_source_parse_issues'])} |\n"
if unknowns:
 md+='\nExplicit gaps:\n\n'
 for x in unknowns:md+=f"- {x['level']} / {x['source_uri']} / {x['object']} / {x['property']} = `{x['value']}`: {x['lookup_status']}, ops{x['stub_ops']}.\n"
md+='\nAll seven source constructors are adopted in selected766 and the actual50 SWAMP condition-field probe passed (parent receipt condition-list-v57-selected-verified.json). The actual cache uses quest discriminator types0/1/2. Empty/Invalid values follow the original early-return branch; missing names follow the original table-miss branch rather than creating a condition. Current ObjectBase TestEnableCondition only calls its evaluator when compiled_a8 is nonnull and tested_ac is false; a miss must preserve that original branch, not invent a condition or treat a source miss as a missing constructor. Actual gameplay effects remain main-owned. Conditional attributes outside activate_cond/deactivate_cond are retained as separate domains, without claiming they route through ConditionData. Per-level inherited source issues and module/declaration provenance remain in the machine-readable table.\n'
md+='\nExact missing-name counts:\n\n'+''.join(f'- `{name}`: {count} authored references.\n' for name,count in miss_groups.items())
md+=f"\nOriginal XML attribute verification: {summary['original_XML_verified_declared_slots']} slots checked against exact original XML; {summary['inventory_only_slots_in_unparsed_original_XML']} slots remain inventory-only where strict source parsing fails.15 source parse/missing issue occurrences are retained; the inherited broader audit issues are not discarded. These do not automatically prove a runtime map failure because procedural selection and original XML parser behavior are separate prerequisites. SWAMP and SWAMP_02 have zero condition-name or constructor-op misses.\n"
(output/'all-level-condition-readiness.md').write_text(md);(b/'tools/audit_conditions_all_levels_v57.py').write_bytes(Path(__file__).read_bytes());print(json.dumps(summary,indent=2));print('explicit gaps',[(x['level'],x['object'],x['value']) for x in unknowns][:30])
