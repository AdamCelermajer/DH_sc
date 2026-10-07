from pathlib import Path
import json,struct
root=Path(__file__).resolve().parents[1];data=json.loads((root/'reference/canonical-object-factory-v1/property-declarations-original.json').read_text())
out=['// Exact source declaration stream captured by probe_canonical_property_declarations_v1.py.']
for c in data['classes']:
 out.append('if (kind=="'+c['kind']+'"'+(' || kind=="Player"'if c['kind']=='Character'else'')+') {')
 for d in c['declarations']:
  k=d.get('kind');v=d.get('default');table=d.get('descriptor_vtable');raw=d.get('descriptor_default_raw')
  if k=='bool':value='std::uint8_t{'+str(v)+'}'
  elif k=='string':value='std::string{'+json.dumps(v)+'}'
  elif k=='vector3':value='*source_.position_rotation_default'if d['name']in('position','rotation')else'std::array<float,3>{{1.0f,1.0f,1.0f}}'
  elif table=='0x95c608':value='std::uint8_t{'+str(raw[0]&255)+'}'
  elif table=='0x95c5c8':value='std::string{}'
  elif table=='0x964708':value='std::int32_t{'+str(struct.unpack('<i',struct.pack('<I',raw[0]))[0])+'}'
  elif table=='0x964898':value='float{'+str(struct.unpack('<f',struct.pack('<I',raw[0]))[0])+'f}'
  elif table=='0x965eb0':value='std::array<std::int32_t,2>{{'+','.join(str(struct.unpack('<i',struct.pack('<I',a))[0])for a in raw)+'}}'
  else:raise ValueError(d)
  if d['name']=='static':value='static_default'
  out.append(' add('+json.dumps(d['name'])+','+d['offset']+','+value+');')
 out.append('}')
(root/'canonical_property_declarations_v1.inc').write_text('\n'.join(out)+'\n')
print('generated exact five source class declaration streams')
