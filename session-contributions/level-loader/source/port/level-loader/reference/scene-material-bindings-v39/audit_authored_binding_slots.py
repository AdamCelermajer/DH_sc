import zipfile,struct,json,hashlib,time
from pathlib import Path
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\scene-material-bindings-v39')
z=zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
rows=[];errors=[];totals={'assets':0,'scene_instances':0,'binding_slots':0,'catalog_URI_index_agreement':0,'primitive_symbol_mismatches':0,'missing_binding_slots':0};gold=[];started=time.monotonic()
for name in z.namelist():
 if not name.lower().endswith('.bdae'):continue
 b=z.read(name)
 if b[:4]!=b'BRES':continue
 try:
  w=lambda p:struct.unpack_from('<I',b,p)[0]
  text=lambda p:b[p:b.index(b'\0',p)].decode('utf-8') if p else ''
  r=w(32);materials=[text(w(w(r+0x60)+i*36)) for i in range(w(r+0x5c))]
  geometries={text(w(w(r+0x6c)+i*16)):w(r+0x6c)+i*16 for i in range(w(r+0x68))}
  controllers={text(w(w(r+0x74)+i*12+4)):w(r+0x74)+i*12 for i in range(w(r+0x70))}
  instances=[]
  def node(p):
   for i in range(w(p+64)):
    a=w(p+68)+8*i;tag=w(a)
    if tag not in (2,3):continue
    g=w(a+4);uri=text(w(g+4));geometry_uri=uri
    if tag==2:
     c=controllers[uri[1:]]
     if w(c)!=0:continue
     geometry_uri=text(w(w(c+8)+112))
    geom=geometries[geometry_uri[1:]]
    if w(geom+8)!=0:continue
    mesh=w(geom+12);bindings=[]
    for j in range(w(g+12)):
     q=w(g+16)+60*j;target=text(w(q+4));idx=w(q+8);external=w(q)
     bindings.append({'slot':j,'external_file_offset':external,'target_URI':target,'catalog_index8':idx,'record_offset':q})
     if external==0:assert idx<len(materials) and materials[idx]==target[1:],(name,target,idx,materials)
    primitives=[text(w(w(mesh+16)+56*k+4)) for k in range(w(mesh+12))]
    instances.append({'node':text(w(p)),'instance_offset':g,'tag':tag,'geometry':geometry_uri,'bindings':bindings,'primitive_symbols':primitives})
   for i in range(w(p+56)):node(w(p+60)+80*i)
  for i in range(w(r+152)):
   vs=w(r+156)+16*i
   for j in range(w(vs+8)):node(w(vs+12)+80*j)
  totals['assets']+=1;totals['scene_instances']+=len(instances)
  for a in instances:
   totals['binding_slots']+=len(a['bindings']);totals['catalog_URI_index_agreement']+=sum(x['external_file_offset']==0 for x in a['bindings'])
   for k,symbol in enumerate(a['primitive_symbols']):
    if k>=len(a['bindings']):totals['missing_binding_slots']+=1;continue
    bind=a['bindings'][k]
    if symbol!=materials[bind['catalog_index8']]:totals['primitive_symbol_mismatches']+=1
  if name.lower().endswith('/go_chest_swamp.bdae') or name.lower().endswith('/swamp.bdae') or name.lower().endswith('/crypt.bdae'):
   rows.append({'asset':name,'sha256':hashlib.sha256(b).hexdigest(),'materials':materials,'instances':instances})
  if instances:gold.append((name,materials,instances))
 except Exception as e:errors.append({'asset':name,'error':str(e)})
report={'validation':'PASS' if not errors else 'partial','scope':'Exact authored BRES SInstanceMaterial records, catalog URI/index agreement, source binding slot vs primitive material-symbol evidence; no original GPU material construction/pass registration',**totals,'errors':errors,'selected_assets':rows,'elapsed_seconds':time.monotonic()-started}
(out/'authored-bres-bindings.json').write_text(json.dumps(report,indent=2)+'\n')
W=lambda n:struct.pack('<I',n);S=lambda x:W(len(x.encode()))+x.encode()
payload=b'SCM9'+W(len(gold))
for name,materials,instances in gold:
 payload+=S(name)+W(len(materials))+b''.join(S(x) for x in materials)+W(len(instances))
 for a in instances:
  payload+=S(a['node'])+W(len(a['bindings']))+b''.join(W(x['catalog_index8']) for x in a['bindings'])+W(len(a['primitive_symbols']))+b''.join(S(x) for x in a['primitive_symbols'])
(out/'authored-binding-slot-gold.bin').write_bytes(payload)
print(json.dumps({k:v for k,v in report.items() if k not in ('selected_assets','errors')},indent=2));print('errors',errors[:5]);print('gold_bytes',len(payload))
