from pathlib import Path
import json,zipfile,struct,xml.etree.ElementTree as ET,collections,re
root=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
out=Path(__file__).parent
cache=Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
class Stop(Exception):pass
class R:
 def __init__(self,b,at=0):self.b=b;self.at=at
 def take(self,n):
  if n<0 or self.at+n>len(self.b):raise Stop('source truncated at'+str(self.at))
  p=self.at;self.at+=n;return p,self.b[p:self.at]
 def u(self):p,b=self.take(4);return p,int.from_bytes(b,'little'),b
 def text(self):
  lp,n,lb=self.u();p,b=self.take(n);return {'length_offset':lp,'source_offset':p,'source_length':n,'length_hex':lb.hex(),'raw_hex':b.hex(),'text':b.decode('utf-8','backslashreplace')}
 def strings(self):
  begin,n,raw=self.u();rows=[self.text() for _ in range(n)];return {'begin':begin,'count':n,'count_hex':raw.hex(),'end':self.at,'rows':rows}
def signed(n):return n if n<0x80000000 else n-0x100000000
resources={};objects=[];rows_by_desc={};npc=[];literal_sites=[];lua_files={};unresolved=[];root_requests=[]
with zipfile.ZipFile(cache) as z:
 entries={}
 for i in z.infolist():
  if not i.is_dir() and i.filename.startswith(prefix):entries.setdefault(i.filename[len(prefix):].lower(),[]).append(i)
 def raw(uri):
  found=entries.get(uri.lower(),[])
  if len(found)!=1:unresolved.append({'uri':uri,'status':'MISSING' if not found else 'AMBIGUOUS'});return None
  i=found[0];b=z.read(i);resources[uri]={'requested_uri':uri,'authored_entry':i.filename,'source_length':len(b),'stored_zip_length':i.compress_size};return b
 uris=json.loads((root/'reports/swamp-fixed-sources-host.json').read_text())['uris']
 for uri in uris:
  if not uri.endswith(('.mlx','.mgp')):continue
  b=raw(uri)
  if b is None:continue
  try:tree=ET.fromstring(b)
  except ET.ParseError as e:unresolved.append({'uri':uri,'status':'STRICT_XML_UNSUPPORTED','reason':str(e)});continue
  for index,n in enumerate(tree):
   if n.tag!='GameObject':continue
   attrs=dict(n.attrib);role=attrs.get('gametype')
   if role=='OpenableContainer' or role=='Character' and attrs.get('_templateName')=='NPC':
    objects.append({'source_uri':uri,'direct_child_index':index,'authored_kind':role,'attributes':attrs,'raw_element_xml':ET.tostring(n,encoding='unicode'),'source_owner':'actual canonical authored object; not global ScriptManager'})
 # Retained68-row group offsets are source evidence; decode only that exact
 # group with existing openable_container_owner_v1.cpp's packed field order.
 group_meta=json.loads(Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/reference/openable-container-v1/actual-openable-rows.json').read_text())
 gb=raw('data/pydata/game_objects_pyarray.bin');nb=raw('data/pydata/game_objects_pyarraynames.bin')
 if gb is not None and nb is not None:
  nr=R(nb);groups=[]
  while nr.at<len(nb):groups.append(nr.strings())
  group_names=next((v for v in groups if any(x['text']=='Swamp_Normal_Chest' for x in v['rows'])),None)
  if group_names is None:unresolved.append({'namespace':'OpenableContainers','status':'NAME_GROUP_UNRESOLVED'})
  else:
   r=R(gb,group_meta['offset']);at,count,cr=r.u()
   for index in range(count):
    start=r.at;fields=[]
    for name,width in [('field4',4),('InteractSound',4),('KeepSolid',1),('Loot',4)]:
     at,b=r.take(width);fields.append({'name':name,'source_offset':at,'width':width,'raw_hex':b.hex(),'raw_unsigned_bits':int.from_bytes(b,'little')})
    script=r.text()
    for name in ['field1c','field20','Visual']:
     at,b=r.take(4);fields.append({'name':name,'source_offset':at,'width':4,'raw_hex':b.hex(),'raw_unsigned_bits':int.from_bytes(b,'little')})
    name=group_names['rows'][index]['text'] if index<len(group_names['rows']) else None
    rows_by_desc[name]={'group_index':index,'source_begin':start,'source_end':r.at,'raw_hex':gb[start:r.at].hex(),'fields':fields,'Script':script,'name_source':group_names['rows'][index] if name else None}
   if r.at!=group_meta['end']:unresolved.append({'namespace':'OpenableContainers','status':'RETAINED_GROUP_BOUNDARY_DIFFERS','actual_end':r.at,'retained_end':group_meta['end']})
 # Actual cache CharacterTable leading448x224 packed rows, from data.cpp.
 cb=raw('data/pydata/character_properties_pyarray.bin');cn=raw('data/pydata/character_properties_pyarraynames.bin');cf=raw('data/pydata/character_properties_pystructnames.bin')
 char_rows={};char_meta={}
 if cb is not None and cn is not None and cf is not None:
  nr=R(cn);character_names=nr.strings();fr=R(cf);character_fields=fr.strings();r=R(cb);at,count,cr=r.u();width=len(character_fields['rows'])
  char_meta={'leading_rows':count,'leading_fields':width,'names_end':nr.at,'names_remaining_bytes':len(cn)-nr.at,'fields_end':fr.at,'fields_remaining_bytes':len(cf)-fr.at}
  if width!=224:unresolved.append({'namespace':'CharacterTable','status':'UNSUPPORTED_NON224_SCHEMA','field_count':width})
  else:
   fields={x['text']:index for index,x in enumerate(character_fields['rows'])}
   for index in range(count):
    start,b=r.take(224*4);name=character_names['rows'][index]['text']
    char_rows[name]={'group_index':index,'source_begin':start,'source_end':r.at,'raw_hex':b.hex(),'raw_unsigned_words':list(struct.unpack('<224I',b)),'fields':fields}
   char_meta.update({'leading_records_end':r.at,'remaining_record_bytes_not_decoded':len(cb)-r.at})
 # Actual15-field AiProps schema is retained in game-data/ai.cpp; no AI runs.
 ab=raw('data/pydata/ai_pyarray.bin');an=raw('data/pydata/ai_pyarraynames.bin');af=raw('data/pydata/ai_pystructnames.bin');ai_rows=[]
 if ab is not None and an is not None and af is not None:
  ai_names=R(an).strings();ai_fields=R(af).strings();field_names=[x['text'] for x in ai_fields['rows']];r=R(ab);at,count,cr=r.u()
  expected=['AttackDelay','CombatBeat','CombatMusic','DelayedLoad','Flags','InteractRadius','LeashDistance','MeleeRadius','OnAggroSFX','Script','SelfFX','Trophy','Type','ViewRadius','ViewRadiusNoAggro']
  if field_names!=expected:unresolved.append({'namespace':'AI','status':'UNSUPPORTED_SCHEMA','actual_fields':field_names})
  else:
   for index in range(count):
    begin=r.at;fields=[]
    for field in field_names:
     if field=='Script':script=r.text();fields.append({'name':field,**script})
     else:
      width=1 if field=='DelayedLoad' else 4;at,b=r.take(width);fields.append({'name':field,'source_offset':at,'width':width,'raw_hex':b.hex(),'raw_unsigned_bits':int.from_bytes(b,'little')})
    ai_rows.append({'group_index':index,'name':ai_names['rows'][index]['text'],'source_begin':begin,'source_end':r.at,'raw_hex':ab[begin:r.at].hex(),'fields':fields,'Script':script})
   if r.at!=len(ab):unresolved.append({'namespace':'AI','status':'SOURCE_TAIL_NOT_DECODED','bytes':len(ab)-r.at})
 def lua_path(path,name):
  result=path+name;p=name.find('.lua')
  if p<0:result+='.luac'
  elif name[p:p+5]!='.luac':result+='c'
  return result
 for obj in objects:
  attrs=obj['attributes']
  if obj['authored_kind']=='OpenableContainer':
   desc=attrs.get('data_desc','');row=rows_by_desc.get(desc)
   obj['OpenableContainers_row']=row
   if row is None:unresolved.append({'object':attrs.get('name'),'descriptor':desc,'status':'OPENABLE_ROW_UNRESOLVED'});continue
   name=row['Script']['text'];obj['object_script_request']={'authored_name':name,'prefix':'data/scripts/objects/','empty_source_name':not name,'external_uri':lua_path('data/scripts/objects/',name) if name else None,'owner':'actual object LuaScript owner; separate from common/local ScriptManager'}
   if name:root_requests.append({'owner':'object','object':attrs.get('name'),'uri':lua_path('data/scripts/objects/',name)})
  else:
   name=attrs.get('charpropsname','');row=char_rows.get(name)
   obj['CharacterTable_row']=row
   if row is None:unresolved.append({'object':attrs.get('name'),'charpropsname':name,'status':'CHARACTER_ROW_UNRESOLVED'});continue
   ai_column=row['fields'].get('AI');ai_bits=row['raw_unsigned_words'][ai_column] if ai_column is not None else None
   ai_id=signed(ai_bits) if ai_bits is not None else None
   selected=ai_id if ai_id is not None and 0<=ai_id<len(ai_rows) else 8
   ai=ai_rows[selected] if selected<len(ai_rows) else None
   obj['base_AI_reference']={'raw_bits':ai_bits,'signed_table_index':ai_id,'source_ai_props_fallback8_used':selected!=ai_id,'selected_base_table_index':selected,'actual_resolved_PropertyState_verified':False}
   obj['base_AI_row']=ai
   dialogs=row['fields'].get('Dialogs');obj['base_Dialogs_reference']={'raw_bits':row['raw_unsigned_words'][dialogs] if dialogs is not None else None,'source_dependency_only':True,'runtime_dialog_owner_verified':False}
   root_requests.append({'owner':'character_common','object':attrs.get('name'),'uri':'data/scripts/ai/_commons.luac'})
   if ai:
    script=ai['Script']['text'];obj['character_script_request']={'authored_base_AI_Script':script,'builtin_selection_name':script if script=='__npc__' else None,'source_selected_kind':0 if script=='__npc__' else 4 if script and not script.startswith('__') else None,'external_uri':lua_path('data/scripts/ai/',script) if script and not script.startswith('_') else None,'owner':'actual retained Character ScriptSession; not global/local ScriptManager','base_data_only':True}
    if script and not script.startswith('_'):root_requests.append({'owner':'character_external','object':attrs.get('name'),'uri':lua_path('data/scripts/ai/',script)})
 # Plain .luac cache entries can be Lua source text. Lex source tokens only;
 # do not compile, evaluate or choose a runtime branch. Ignore comments and
 # quoted/long string contents when finding direct named call sites.
 def decode_lua_text(uri,b):
  result={'uri':uri,'source_bytes':len(b),'source_hex':b.hex(),'status':'SOURCE_LUA_TEXT_INSPECTED','literal_include_sites':[],'dynamic_include_sites':[],'literal_CallPyScript_sites':[],'dynamic_CallPyScript_sites':[],'named_function_definitions':[],'engine_executed':False,'conditional_reach_verified':False}
  try:text=b.decode('utf-8')
  except UnicodeDecodeError as e:result['status']='SOURCE_LUA_UNSUPPORTED';result['stop_reason']=str(e);return result
  result['source_text']=text;tokens=[];at=0
  def long_at(pos):
   m=re.match(r'\[(=*)\[',text[pos:])
   if not m:return None
   close=']'+m[1]+']';end=text.find(close,pos+len(m[0]))
   if end<0:raise Stop('unterminated Lua long string/comment at'+str(pos))
   return text[pos+len(m[0]):end],end+len(close)
  def quoted(pos):
   quote=text[pos];i=pos+1;value='';simple={'a':'\a','b':'\b','f':'\f','n':'\n','r':'\r','t':'\t','v':'\v'}
   while i<len(text):
    c=text[i]
    if c==quote:return value,i+1
    if c!='\\':value+=c;i+=1;continue
    i+=1
    if i>=len(text):raise Stop('unterminated Lua escape')
    c=text[i]
    if c.isdigit():
     digits=re.match(r'[0-9]{1,3}',text[i:])[0];n=int(digits)
     if n>255:raise Stop('invalid Lua decimal escape')
     value+=chr(n);i+=len(digits)
    else:value+=simple.get(c,c);i+=1
   raise Stop('unterminated Lua quoted string at'+str(pos))
  try:
   while at<len(text):
    if text[at].isspace():at+=1;continue
    if text.startswith('--',at):
     block=long_at(at+2)
     at=block[1] if block else (text.find('\n',at)+1 if '\n' in text[at:] else len(text));continue
    begin=at
    if text[at] in ('\"',"'"):value,at=quoted(at);tokens.append(('string',value,begin,at));continue
    block=long_at(at)
    if block:value,at=block;tokens.append(('string',value,begin,at));continue
    m=re.match(r'[A-Za-z_][A-Za-z0-9_]*',text[at:])
    if m:at+=len(m[0]);tokens.append(('id',m[0],begin,at));continue
    tokens.append(('symbol',text[at],begin,at+1));at+=1
   for index,t in enumerate(tokens):
    kind,name,begin,end=t
    if name=='function' and index+1<len(tokens) and tokens[index+1][0]=='id':result['named_function_definitions'].append({'source_offset':begin,'following_identifier':tokens[index+1][1]})
    if kind!='id' or name not in ('Include','CallPyScript'):continue
    if index and tokens[index-1][1]=='function':continue
    if index+1>=len(tokens) or tokens[index+1][1]!='(':continue
    argument=tokens[index+2] if index+2<len(tokens) else None
    site={'uri':uri,'call_name':name,'source_offset':begin,'line':text.count('\n',0,begin)+1,'call_style':'method_or_table' if index and tokens[index-1][1] in ('.',':') else 'direct_named','conditional_reach_verified':False}
    literal=argument and argument[0]=='string' and index+3<len(tokens) and tokens[index+3][1] in (')',',')
    target=('literal_include_sites' if name=='Include' else 'literal_CallPyScript_sites') if literal else ('dynamic_include_sites' if name=='Include' else 'dynamic_CallPyScript_sites')
    if literal:site.update({'literal_name':argument[1],'literal_source_offset':argument[2],'raw_source_literal':text[argument[2]:argument[3]]})
    result[target].append(site)
  except Stop as e:result['status']='SOURCE_LUA_UNSUPPORTED';result['stop_reason']=str(e)
  result['source_token_count']=len(tokens);result['consumed_bytes']=len(b)
  return result
 # Pure Lua5.1 bytecode structural inspection. No VM, emulation or instruction
 # execution: only prototype/constant/opcode source decoding and direct literal
 # Include call sites within a basic block. Conditional reach stays unverified.
 def decode_lua(uri,b):
  if not b.startswith(b'\x1bLua'):return decode_lua_text(uri,b)
  result={'uri':uri,'source_bytes':len(b),'source_hex':b.hex(),'status':'SOURCE_LUA_CONSTANTS_INSPECTED','prototypes':[],'literal_include_sites':[],'dynamic_include_sites':[],'engine_executed':False}
  r=R(b)
  try:
   at,h=r.take(12)
   if h[:6]!=b'\x1bLua\x51\x00' or h[6] not in (0,1) or h[7] not in (4,8) or h[8] not in (4,8) or h[9]!=4:raise Stop('unsupported Lua header'+h.hex())
   endian='little' if h[6] else 'big';ints=h[7];sizes=h[8];number_size=h[10]
   def integer(n=ints):return int.from_bytes(r.take(n)[1],endian)
   def string():
    at=r.at;n=integer(sizes)
    if n==0:return {'source_offset':at,'encoded_length':0,'raw_hex':'','text':None}
    p,raw=r.take(n);return {'source_offset':at,'bytes_offset':p,'encoded_length':n,'raw_hex':raw.hex(),'text':raw[:-1].decode('utf-8','backslashreplace'),'trailing_sentinel_hex':raw[-1:].hex()}
   def proto(parent=None,depth=0):
    if depth>32:raise Stop('unsupported deep prototype nesting')
    begin=r.at;source=string();line=integer();lastline=integer();_,flags=r.take(4);ncode=integer()
    if ncode>100000:raise Stop('unsupported large opcode block')
    code_at=r.at;_,code=r.take(ncode*4);instructions=[int.from_bytes(code[i:i+4],endian) for i in range(0,len(code),4)];constants=[]
    for k in range(integer()):
     _,raw=r.take(1);tag=raw[0];v={'tag':tag}
     if tag==0:v['nil']=True
     elif tag==1:v['raw_bool_byte']=r.take(1)[1].hex()
     elif tag==3:v['raw_number_hex']=r.take(number_size)[1].hex()
     elif tag==4:v['string']=string()
     else:raise Stop('unsupported Lua constant tag'+str(tag))
     constants.append(v)
    index=len(result['prototypes']);p={'prototype':index,'parent':parent,'source_begin':begin,'source':source,'line':line,'lastline':lastline,'prototype_flags_hex':flags.hex(),'code_source_offset':code_at,'instruction_words':instructions,'constants':constants};result['prototypes'].append(p)
    regs={}
    for pc,ins in enumerate(instructions):
     op=ins&63;a=(ins>>6)&255;b=(ins>>23)&511;c=(ins>>14)&511;bx=ins>>14
     if op==5:
      kv=constants[bx] if bx<len(constants) else {};regs[a]=('global',kv.get('string',{}).get('text'))
     elif op==1:
      kv=constants[bx] if bx<len(constants) else {};regs[a]=('string',kv['string']['text']) if kv.get('tag')==4 else ('opaque',None)
     elif op==0:regs[a]=regs.get(b,('opaque',None))
     elif op in (28,29):
      if regs.get(a)==('global','Include'):
       argument=regs.get(a+1);site={'uri':uri,'prototype':index,'instruction_index':pc,'source_offset':code_at+pc*4,'source_instruction_word':ins,'conditional_reach_verified':False}
       if b>=2 and argument and argument[0]=='string' and argument[1] is not None:site['literal_name']=argument[1];result['literal_include_sites'].append(site)
       else:result['dynamic_include_sites'].append(site)
      regs.clear()
     elif op in (22,23,24,25,26,27,30,31,32,33,35):regs.clear()
     else:
      regs.pop(a,None)
      if op in (3,11):
       for reg in range(a,b+1 if op==3 else a+2):regs.pop(reg,None)
    for child in range(integer()):proto(index,depth+1)
    nline=integer();r.take(nline*ints)
    for local in range(integer()):string();integer();integer()
    for upvalue in range(integer()):string()
    p['source_end']=r.at
   proto()
   if r.at!=len(b):raise Stop('unconsumed Lua bytes'+str(len(b)-r.at))
  except Stop as e:result['status']='SOURCE_LUA_UNSUPPORTED';result['stop_reason']=str(e)
  result['consumed_bytes']=r.at;return result
 pending=list(dict.fromkeys(v['uri'] for v in root_requests));seen=set()
 while pending:
  uri=pending.pop(0)
  if uri in seen:continue
  if len(seen)>=64:unresolved.append({'status':'UNSUPPORTED_BOUNDED_DEPENDENCY_COUNT'});break
  seen.add(uri);b=raw(uri)
  if b is None:continue
  decoded=decode_lua(uri,b);lua_files[uri]=decoded
  if decoded['status']=='SOURCE_LUA_UNSUPPORTED':unresolved.append({'uri':uri,'status':decoded['status'],'reason':decoded.get('stop_reason')})
  for site in decoded['literal_include_sites']:
   prefix_for_file=uri.rsplit('/',1)[0]+'/';name=site['literal_name'];target=lua_path(prefix_for_file,name) if name else None;site['requested_uri']=target;literal_sites.append(site)
   if target and target not in seen:pending.append(target)
summary={'level':'SWAMP','authored_chests':sum(v['authored_kind']=='OpenableContainer' for v in objects),'authored_NPC_template_objects':sum(v['authored_kind']=='Character' for v in objects),'object_data_descriptors':sorted({v['attributes'].get('data_desc','') for v in objects if v['authored_kind']=='OpenableContainer'}),'nonempty_chest_external_script_names':sum(bool(v.get('object_script_request',{}).get('authored_name')) for v in objects if v['authored_kind']=='OpenableContainer'),'root_requested_lua_resources':sorted({v['uri'] for v in root_requests}),'source_inspected_lua_resources':list(lua_files),'literal_include_sites':len(literal_sites),'literal_CallPyScript_sites':sum(len(v.get('literal_CallPyScript_sites',[])) for v in lua_files.values()),'dynamic_CallPyScript_sites':sum(len(v.get('dynamic_CallPyScript_sites',[])) for v in lua_files.values()),'dynamic_include_sites':sum(len(v['dynamic_include_sites']) for v in lua_files.values()),'unresolved_dependencies':unresolved,'tests_native_engine_or_compiler_jobs_run':0,'hashes_computed':False}
report={'status':'SOURCE_OBJECT_DEPENDENCY_INSPECTION_ONLY','cache_path':str(cache),'summary':summary,'resources':resources,'objects':objects,'character_table_scope':char_meta,'root_requests':root_requests,'literal_include_sites':literal_sites,'lua_sources':lua_files,'data_sources':['port/level-world/character_script_selection.cpp','port/level-world/character_script_owner.cpp','port/level-world/character_script_session.cpp','port/game-data/data.cpp','port/game-data/ai.cpp','port/level-world/openable_container_owner_v1.cpp','reference/openable-container-v1/actual-openable-rows.json retained group offset3031..5007'],'owner_scope':'Actual authored object LuaScript and Character ScriptSession are separate from application common/local ScriptManager; no new VM/registry is created. Base cache AI/Dialogs fields do not prove actual current resolved property state.'}
(out/'swamp-object-script-dependencies.json').write_text(json.dumps(report,indent=2)+'\n')
compact={k:v for k,v in report.items() if k not in ('objects','lua_sources')};compact['objects']=[{k:v for k,v in obj.items() if k not in ('CharacterTable_row','base_AI_row','OpenableContainers_row')} for obj in objects]
(out/'swamp-object-script-summary.json').write_text(json.dumps(compact,indent=2)+'\n')
print(json.dumps(summary))
