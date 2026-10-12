from pathlib import Path
import json,re,struct,zipfile,csv,collections
root=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
out=Path(__file__).parent
cache=Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
association=json.loads((root/'candidate/script-generic-coverage-v60/coverage-v60.json').read_text())
schema_file=root/'script_data_schemas_v52.inc'
schemas={}
for line in schema_file.read_text().splitlines():
 m=re.match(r'\s*\{(\d+),\{(\d+)u,(0x[0-9a-f]+)u,(0x[0-9a-f]+)u,',line)
 if not m:continue
 k,size,factory,reader=m.groups()
 begin=m.end();depth=0;end=begin
 for index in range(begin,len(line)):
  if line[index]=='{':depth+=1
  elif line[index]=='}':depth-=1
  if depth==0:end=index+1;break
 actions=line[begin:end]
 schemas[int(k)]={'original_size':int(size),'factory':factory,'reader':reader,'actions':[tuple(map(int,t)) for t in re.findall(r'\{(\d+),(\d+),(\d+)\}',actions)]}

classes={v['kind']:v['class'] for v in json.loads((root/'candidate/script-command-complete/source/constructor-data-runtime.json').read_text())['runtime_matrix']}
class SourceStop(Exception):pass
class Reader:
 def __init__(self,b):self.b=b;self.at=0
 def take(self,n):
  if n<0 or self.at+n>len(self.b):raise SourceStop('truncated source bytes at'+str(self.at)+' need'+str(n))
  at=self.at;self.at+=n;return at,self.b[at:self.at]
 def word(self,width=4):
  at,b=self.take(width);return at,int.from_bytes(b,'little'),b
 def remaining(self):return len(self.b)-self.at

def commands(b):
 r=Reader(b);result={'source_bytes':len(b),'source_hex':b.hex(),'scripts':[],'reached_kinds':[],'kind_counts':{},'completed_records':0,'status':'SOURCE_DATA_DECODED','native_parser_validated':False,'engine_executed':False}
 reached=[];counts=collections.Counter()
 try:
  _,n,raw=r.word();result['declared_scripts']=n;result['header_hex']=raw.hex()
  if n>8192:raise SourceStop('unsupported source inspection script count'+str(n))
  for si in range(n):
   at,nc,raw=r.word();script={'local_script_index':si,'command_count_offset':at,'declared_commands':nc,'command_count_hex':raw.hex(),'records':[]};result['scripts'].append(script)
   if nc>65536:raise SourceStop('unsupported source inspection command count'+str(nc))
   for ci in range(nc):
    begin=r.at
    if r.remaining()<4:raise SourceStop('truncated command kind at'+str(begin))
    k=int.from_bytes(r.b[begin:begin+4],'little');reached.append(k);counts[k]+=1
    record={'command_index':ci,'source_begin':begin,'kind':k,'actual_class':classes.get(k),'fields':[],'complete':False};script['records'].append(record)
    if k not in schemas:raise SourceStop('unsupported schema for actual reached kind'+str(k)+' at'+str(begin))
    record['schema']=schemas[k];scalars={}
    for offset,width,length_offset in schemas[k]['actions']:
     if width in (1,2,4):
      at,bits,raw=r.word(width);scalars[offset]=bits;field={'original_offset':offset,'source_offset':at,'width':width,'raw_hex':raw.hex(),'raw_unsigned_bits':bits}
     elif width in (0,255):
      if length_offset not in scalars:raise SourceStop('unproduced actual length offset'+str(length_offset))
      count=scalars[length_offset];bytes_count=count if width==0 else count*4
      if count>(1024*1024 if width==0 else 65536):raise SourceStop('unsupported source inspection operand length'+str(count))
      at,raw=r.take(bytes_count);field={'original_offset':offset,'source_offset':at,'representation':'CString_bytes' if width==0 else 'raw_word_array','length_offset':length_offset,'authored_count':count,'raw_hex':raw.hex()}
      if width==0:field['text_display_only']=raw.decode('utf-8','backslashreplace')
      else:field['raw_unsigned_words']=[int.from_bytes(raw[i:i+4],'little') for i in range(0,len(raw),4)]
     else:raise SourceStop('unsupported retained schema width'+str(width))
     record['fields'].append(field)
    record['source_end']=r.at;record['serialized_bytes']=r.at-begin;record['raw_hex']=b[begin:r.at].hex();record['complete']=True;result['completed_records']+=1
  if r.remaining():raise SourceStop('unconsumed authored source tail at'+str(r.at)+' bytes'+str(r.remaining()))
 except SourceStop as e:
  result['status']='SOURCE_DATA_UNSUPPORTED_OR_TRUNCATED';result['stop_reason']=str(e)
 result['consumed_bytes']=r.at;result['remaining_bytes']=r.remaining();result['reached_kinds']=sorted(set(reached));result['kind_counts']={str(k):v for k,v in sorted(counts.items())};return result

def names(b):
 r=Reader(b);result={'source_bytes':len(b),'source_hex':b.hex(),'names':[],'status':'SOURCE_NAMES_DECODED'}
 try:
  _,n,raw=r.word();result['declared_names']=n;result['header_hex']=raw.hex()
  if n>8192:raise SourceStop('unsupported source inspection name count'+str(n))
  for ni in range(n):
   length_at,length,length_raw=r.word()
   if length>1024*1024:raise SourceStop('unsupported source inspection name length'+str(length))
   at,raw=r.take(length);result['names'].append({'local_name_index':ni,'length_offset':length_at,'length_hex':length_raw.hex(),'source_offset':at,'source_length':length,'raw_hex':raw.hex(),'text_display_only':raw.decode('utf-8','backslashreplace')})
  if r.remaining():raise SourceStop('unconsumed name source tail at'+str(r.at))
 except SourceStop as e:result['status']='SOURCE_NAMES_UNSUPPORTED_OR_TRUNCATED';result['stop_reason']=str(e)
 result['consumed_bytes']=r.at;result['remaining_bytes']=r.remaining();return result

pairs=[{'pair_id':'C00','owner_scope':'common_application','command_uri':'data/pydata/scripts_pyscripts.bin','name_uri':'data/pydata/scripts_pyscriptnames.bin','level_identities':[]}]
for i,p in enumerate(association['local_pairs'],1):pairs.append({'pair_id':'L'+str(i).zfill(2),'owner_scope':'local_level','command_uri':p['command_uri'],'name_uri':p['name_uri'],'level_identities':p['level_identities'],'prior_metadata_command_bytes':p['command_bytes'],'prior_metadata_name_bytes':p['name_bytes']})
for i,p in enumerate(association['extra_unmapped_pair_metadata'],1):pairs.append({'pair_id':'U'+str(i).zfill(2),'owner_scope':'unmapped_authored_pair','command_uri':p['uri'],'name_uri':p['uri'].replace('_pyscripts.bin','_pyscriptnames.bin'),'level_identities':[]})
with zipfile.ZipFile(cache) as z:
 entries={}
 for info in z.infolist():
  if not info.is_dir() and info.filename.startswith(prefix):entries.setdefault(info.filename[len(prefix):].replace('\\','/').lower(),[]).append(info)
 for pair in pairs:
  pair['resources']={};pair['status']='SOURCE_PAIR_DECODED'
  for role,decode in [('command',commands),('name',names)]:
   uri=pair[role+'_uri'];found=entries.get(uri.lower(),[])
   resource={'requested_uri':uri,'found':len(found)==1,'candidate_count':len(found)}
   pair['resources'][role]=resource
   if not found:resource['status']='MISSING_AUTHORED_RESOURCE';pair['status']='MISSING_PAIR_RESOURCE';continue
   if len(found)!=1:resource['status']='AMBIGUOUS_CASE_COLLIDING_RESOURCE';pair['status']='UNRESOLVED_PAIR_RESOURCE';continue
   info=found[0];b=z.read(info);resource.update({'authored_entry':info.filename,'source_length':len(b),'stored_zip_length':info.compress_size,'status':'SOURCE_RESOURCE_READ'})
   pair[role+'_decode']=decode(b)
   if 'UNSUPPORTED' in pair[role+'_decode']['status']:pair['status']='UNSUPPORTED_PAIR_DECODE'
  c=pair.get('command_decode',{});n=pair.get('name_decode',{})
  pair['declared_counts_match']=(c.get('declared_scripts')==n.get('declared_names')) if 'declared_scripts' in c and 'declared_names' in n else None
  if pair['declared_counts_match'] is False:pair['status']='AUTHORED_COMMAND_NAME_COUNT_MISMATCH'
  pair['reached_kinds']=c.get('reached_kinds',[])
lookup={p['command_uri']:p for p in pairs}
levels=[]
for old in association['levels']:
 local=[lookup[k] for k in old['command_pairs']]
 levels.append({'identity':old['identity'],'common_pair_id':'C00','local_pair_ids':[p['pair_id'] for p in local],'local_command_uris':old['command_pairs'],'status':('NO_AUTHORED_LOCAL_SCRIPTFILE_IN_RETAINED_METADATA' if not local else 'SOURCE_LOCAL_PAIRS_DECODED' if all(p['status']=='SOURCE_PAIR_DECODED' for p in local) else 'LOCAL_PAIR_SOURCE_INCOMPLETE'),'local_reached_kinds':sorted({k for p in local for k in p['reached_kinds']}),'raw_operand_interpretation':'none','runtime_validated':False})
mapped=[p for p in pairs if p['owner_scope']=='local_level'];all_kinds=sorted({k for p in pairs for k in p['reached_kinds']});mapped_kinds=sorted({k for p in mapped for k in p['reached_kinds']});old_kinds=set(association['known_observed_kinds'])
summary={'level_identities':len(levels),'levels_with_authored_local_pairs':sum(bool(v['local_pair_ids']) for v in levels),'mapped_unique_local_pairs':len(mapped),'source_decoded_mapped_pairs':sum(p['status']=='SOURCE_PAIR_DECODED' for p in mapped),'missing_or_unresolved_pairs':[p['pair_id'] for p in pairs if p['status']!='SOURCE_PAIR_DECODED'],'common_scripts':pairs[0].get('command_decode',{}).get('declared_scripts'),'common_records':pairs[0].get('command_decode',{}).get('completed_records'),'mapped_local_scripts':sum(p.get('command_decode',{}).get('declared_scripts',0) for p in mapped),'mapped_local_records':sum(p.get('command_decode',{}).get('completed_records',0) for p in mapped),'mapped_local_command_bytes':sum(p['resources']['command'].get('source_length',0) for p in mapped),'mapped_local_name_bytes':sum(p['resources']['name'].get('source_length',0) for p in mapped),'reached_kinds_all_selected_pairs':all_kinds,'reached_kinds_mapped_local_pairs':mapped_kinds,'additional_authored_kinds_over_retained_common_swamp':sorted(set(mapped_kinds)-old_kinds),'static_table_slots_not_reached_in_selected_authored_pairs':sorted(set(range(80))-set(all_kinds)),'source_schema_count':len(schemas),'native_or_engine_execution':False,'tests_or_compiler_jobs_run':0,'hashes_computed':False}
report={'status':'SOURCE_DATA_INSPECTION_ONLY_NOT_NATIVE_PARSER_OR_RUNTIME_VALIDATION','cache_path':str(cache),'cache_prefix':prefix,'cache_provenance':'Existing Chapter V36 canonical cache provenance reused; no new hash computed','association_source':'candidate/script-generic-coverage-v60/coverage-v60.json','schema_source':str(schema_file),'summary':summary,'pairs':pairs,'levels':levels,'unsupported_paths':'Missing resources remain missing; unknown schema/width/truncation stops at the exact cursor without guessed record skipping. Unmapped U01 is not assigned to any of the51 identities. No owner, engine effect or resource purpose is inferred from opaque operand bits/strings.'}
(out/'source-script-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
with (out/'level-coverage.csv').open('w',newline='',encoding='utf-8') as f:
 w=csv.writer(f);w.writerow(['level_identity','common_pair','local_pairs','local_command_uris','source_status','exact_local_reached_kinds','native_runtime_validated'])
 for v in levels:w.writerow([v['identity'],v['common_pair_id'],';'.join(v['local_pair_ids']),';'.join(v['local_command_uris']),v['status'],';'.join(map(str,v['local_reached_kinds'])),'false'])
lines=['# All-map authored script resource coverage','',
'This is a pure stored-byte inspection using the retained80 Data schemas. It did not run the native parser, original code, Init/Execute/IsBlocking, tests or compiler jobs. Raw file and operand bytes are retained in source-script-coverage.json; integer values are unsigned bit displays, CString text is display-only, and no opaque meaning is inferred. No new hash or frozen packet was produced.','',
'Common C00 is the application-owned common script resource. Local L01..L38 are authored pair resources associated with distinct level identities; sharing a pair does not merge those level owners. U01 remains authored but unassociated with the retained51 identity scriptFile mapping. MULTIPLAYER_TEST has no authored local reference in that metadata; common ownership remains separate.','',
'## Source inventory','',
'| Pair / owner scope | Authored command URI | Associated identities | Command / name bytes | Scripts / names / complete records | Exact reached kinds | Source status |','|---|---|---|---:|---:|---|---|']
for p in pairs:
 c=p.get('command_decode',{});n=p.get('name_decode',{});resources=p['resources']
 lines.append('|'+p['pair_id']+' /'+p['owner_scope']+'|'+p['command_uri']+'|'+(', '.join(p['level_identities']) or 'none mapped')+'|'+str(resources['command'].get('source_length','missing'))+' /'+str(resources['name'].get('source_length','missing'))+'|'+str(c.get('declared_scripts','unknown'))+' /'+str(n.get('declared_names','unknown'))+' /'+str(c.get('completed_records',0))+'|'+','.join(map(str,p['reached_kinds']))+'|'+p['status']+'|')
lines+=['','## All51 retained level identities','','| Level identity | Common owner resource | Local owner resources | Exact local reached kinds | Source status |','|---|---|---|---|']
for v in levels:lines.append('|'+v['identity']+'|C00; application scope|'+(','.join(v['local_pair_ids']) or 'no authored local scriptFile')+'|'+','.join(map(str,v['local_reached_kinds']))+'|'+v['status']+'|')
lines+=['','## Exact resource and decoding boundaries','',
'Every pair JSON row identifies both requested command/name URI, exact ZIP entry, uncompressed source length and stored ZIP length. Each script/name index, count/header byte span, complete command byte span and ordered field source cursor is retained. CString raw bytes exclude the native sentinel that storage adds; raw arrays preserve every four-byte word. Constructor defaults are not fabricated into authored bytes.','',
'The selected mapped pairs contain'+str(summary['mapped_local_scripts'])+' authored local scripts and'+str(summary['mapped_local_records'])+' complete command records; common adds'+str(summary['common_scripts'])+' scripts and'+str(summary['common_records'])+' records. Additional authored kinds beyond the historical common/SWAMP46 set: '+(','.join(map(str,summary['additional_authored_kinds_over_retained_common_swamp'])) or 'none')+'. Static table slots not reached in these selected resources: '+','.join(map(str,summary['static_table_slots_not_reached_in_selected_authored_pairs']))+'.','',
'No missing file is replaced with empty data. A source-decode success establishes record boundaries under retained schemas, not independent native parser agreement, correct actual Init effects, script scheduling, authored progression, resource readiness or gameplay. The original common/SWAMP receipts remain the historical native/original evidence; the other pairs now have source-data inspection only.','']
(out/'COVERAGE.md').write_text('\n'.join(lines).replace('retained80','retained 80').replace('retained51','retained 51').replace('All51','All 51').replace('contain'+str(summary['mapped_local_scripts']),'contain '+str(summary['mapped_local_scripts'])).replace('and'+str(summary['mapped_local_records']),'and '+str(summary['mapped_local_records'])).replace('adds'+str(summary['common_scripts']),'adds '+str(summary['common_scripts'])).replace('and'+str(summary['common_records']),'and '+str(summary['common_records'])).replace('SWAMP46','SWAMP 46'))
print(json.dumps(summary))
