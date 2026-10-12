from pathlib import Path
import json,struct,subprocess
r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');d=r/'reference/scriptmanager-v52';orig=json.loads((d/'original-v52.json').read_text(encoding='utf-8'));table=json.loads((d/'factory-table.json').read_text())['entries'];schemas={};inits={};unknown=[]
for load in orig['loads']:
 for row in load['records']:
  actions=[];blob=bytes.fromhex(row['data_hex']);ctor=bytes.fromhex(row['data_C1_hex'])
  array_seen=False
  for field in row['reads']:
   if field['offset'] is not None:actions.append({'kind':'scalar','offset':field['offset'],'width':field['width'],'helper':field['helper']})
   elif field['helper']=='0x317454':
    dest=int(field['destination'],16);matches=[off for off in range(4,len(blob)-3,4) if struct.unpack_from('<I',blob,off)[0]==dest];assert len(matches)==1,(row,matches)
    off=matches[0];assert struct.unpack_from('<I',blob,off-4)[0]==field['width'];actions.append({'kind':'cstring','offset':off,'length_offset':off-4,'helper':field['helper']})
   elif row['kind']==0 and field['helper']=='0x459090' and field['width']==4:
    count=struct.unpack_from('<I',blob,16)[0];base=struct.unpack_from('<I',blob,20)[0];dest=int(field['destination'],16);assert base<=dest<base+count*4 and (dest-base)%4==0
    if not array_seen:actions.append({'kind':'int_array','offset':20,'length_offset':16,'helper':field['helper']});array_seen=True
   else:unknown.append((row['kind'],field));raise ValueError(unknown)
  if row['kind']==0 and not array_seen:
   assert struct.unpack_from('<I',blob,16)[0]==0
   pos=next(i for i,a in enumerate(actions) if a['offset']==16);actions.insert(pos+1,{'kind':'int_array','offset':20,'length_offset':16,'helper':'0x459090'})
  default=[]
  for a in actions:
   if a['kind']=='scalar':
    b=ctor[a['offset']:a['offset']+a['width']]
    if b!=b'\xa5'*a['width']:default.append({'offset':a['offset'],'width':a['width'],'bits':int.from_bytes(b,'little')})
   else:assert struct.unpack_from('<I',ctor,a['offset'])[0]==0,(row['kind'],a,ctor.hex())
  schema={'kind':row['kind'],'original_size':row['data_size'],'data_reader':row['data_reader'],'data_factory':table[row['kind']]['data_factory'],'actions':actions,'constructor_defaults':default}
  if row['kind'] in schemas:assert schemas[row['kind']]==schema,(row['kind'],schemas[row['kind']],schema)
  else:schemas[row['kind']]=schema
  inits.setdefault(row['kind'],row['command_init']);assert inits[row['kind']]==row['command_init']
(d/'data-schemas-v52.json').write_bytes((json.dumps(schemas,indent=2)+'\n').encode());(d/'observed-command-init-v52.json').write_bytes(json.dumps(inits,indent=2).encode())
lines=['// Generated only from original executed Data C1/read actions and original static factory addresses.','inline const std::map<std::int32_t,ScriptDataSchemaV52>& original_script_data_schemas_v52(){',' static const std::map<std::int32_t,ScriptDataSchemaV52> schemas{']
for kind,s in sorted(schemas.items()):
 acts=','.join('{'+str(a['offset'])+','+str(a['width'] if a['kind']=='scalar' else (255 if a['kind']=='int_array' else 0))+','+str(a.get('length_offset',0))+'}' for a in s['actions'])
 defaults=','.join('{'+str(a['offset'])+',{'+str(a['width'])+','+str(a['bits'])+'u}}' for a in s['constructor_defaults'])
 lines.append('  {'+str(kind)+',{'+str(s['original_size'])+'u,'+s['data_factory']+'u,'+s['data_reader']+'u,{'+acts+'},{'+defaults+'}}},')
lines+=[' };return schemas;','}'];(r/'script_data_schemas_v52.inc').write_bytes(('\n'.join(lines)+'\n').encode())
factlines=['// Original command factories 0x968c00 / data factories +0x140, exact80 source entries.','inline constexpr std::array<std::uint32_t,80> script_command_factories_v52{{'+','.join(x['command_factory']+'u' for x in table)+'}};'];(r/'script_command_factories_v52.inc').write_bytes(('\n'.join(factlines)+'\n').encode())
# Gold includes real parsed raw fields and strings; ARM pointer/vtable identities
# are normalized, values and constructor-defined source bytes are preserved.
def word(v):return struct.pack('<I',v&0xffffffff)
def text(v):b=v.encode();return word(len(b))+b
parts=[word(870)]
for load in orig['loads']:
 for row in load['records']:
  parts.append(word(row['kind'])+word(row['begin'])+word(row['end'])+word(len(row['reads'])))
  for f in row['reads']:parts.append(word(f['offset'] if f['offset'] is not None else 0xffffffff)+word(f['width'])+bytes.fromhex(f['raw']))
parts.append(word(70));parts.extend(text(x) for x in orig['names']);(d/'data-gold-v52.bin').write_bytes(b''.join(parts))
print(json.dumps({'schemas':len(schemas),'command_init_addresses':sorted(set(inits.values())),'records':870,'gold_bytes':sum(map(len,parts))}))
