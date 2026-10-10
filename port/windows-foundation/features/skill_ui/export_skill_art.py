"""Read original SWF skill icon states and source interaction placements."""
import importlib.util,json,struct,zlib,sys,types,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
spec=importlib.util.spec_from_file_location('hud_export',ROOT/'port/windows-foundation/tools/export_hud_geometry.py')
swf=importlib.util.module_from_spec(spec);spec.loader.exec_module(swf)
path=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf'
raw=path.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
_,at=swf.rect(data,8);at+=4
sprites={};labels={};shapes={};errors={};shape_tags={}
for code,tag in swf.tags(data,at,len(data)):
 if code==39:
  ident,count=struct.unpack_from('<HH',tag);sprites[ident]=swf.parse_timeline(tag,4);labels[ident]={};frame=0
  for c,t in swf.tags(tag,4,len(tag)):
   if c==43:labels[ident][t.split(b'\0')[0].decode()]=frame
   elif c==1:frame+=1
 elif code in(2,22,32):
  ident=struct.unpack_from('<H',tag)[0];swf.ROLES[ident]=str(ident)
  shape_tags[ident]=(code,tag)
  try:shapes[ident]=swf.parse_shape(code,tag)
  except ValueError as e:errors[ident]=str(e)
root=swf.parse_timeline(data,at)
if '--inspect' in sys.argv:
 for i,l in labels.items():
  if any('bash' in k or 'skill' in k.lower() for k in l) or i==493:print(i,l)
 for i in(493,496):
  print('sprite',i)
  for frame,p in enumerate(sprites[i]):
   if frame<3:print(frame,[(k,v.get('name'),v['character'],v['matrix']) for k,v in p.items()])
 sys.exit()
def solid_styles(tag,at,ident):
 count=tag[at];at+=1
 if count==255:count=struct.unpack_from('<H',tag,at)[0];at+=2
 styles=[]
 for _ in range(count):
  if tag[at]!=0:raise ValueError('non-solid hit style')
  at+=1+color_bytes
  styles.append({'matrix_twips':[1000000,0,0,1000000,-100000000,-100000000]})
 if tag[at]:raise ValueError('unsupported hit line style')
 return styles,at+1
hit_globals=dict(swf.parse_shape.__globals__);hit_globals['read_bitmap_styles']=solid_styles
parse_hit=types.FunctionType(swf.parse_shape.__code__,hit_globals)
def geometry(ident,m,name,frame=0):
 if ident in shapes:
  return [(name,ident,[[(m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20,u,v] for x,y,u,v in shapes[ident]['triangles']])]
 if ident not in sprites:raise ValueError('unsupported icon '+str(ident)+' '+str(errors.get(ident)))
 out=[]
 for depth,p in sorted(sprites[ident][frame].items()):
  if p.get('clip_depth') or p['color']!=[[1.]*4,[0.]*4]:raise ValueError('unsupported icon color/mask')
  out+=geometry(p['character'],swf.multiply(m,p['matrix']),name+'/'+str(depth))
 return out
def hitgeometry(ident,m):
 if ident in shape_tags:
  global color_bytes
  code,tag=shape_tags[ident];color_bytes=4 if code==32 else 3
  try:r=parse_hit(code,tag)
  except ValueError:r=shapes[ident]
  return [[(m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20,0,0] for x,y,_,_ in r['triangles']]
 out=[]
 for _,p in sorted(sprites[ident][0].items()):out+=hitgeometry(p['character'],swf.multiply(m,p['matrix']))
 return out
def nums(v):return '{'+','.join(swf.cpp_number(float(x)) for x in v)+'}'
def batches(v):return '{'+','.join('{'+json.dumps(n)+','+str(i)+',{'+','.join(nums(x) for x in t)+'}}' for n,i,t in v)+'}'
lines=['// Generated from original dqcharmenu_droid.swf; exact bitmap contours/placements.','#include "original_skill_art.hpp"','namespace dh::foundation::skill_ui {']
states=[]
for label,frame in labels[79].items():states.append('{'+json.dumps(label)+','+str(frame)+','+batches(geometry(79,swf.IDENTITY,label,frame))+'}')
lines.append('const std::vector<IconState>& original_skill_icon_states(){static const std::vector<IconState> v{'+','.join(states)+'};return v;}')
panel=swf.placed_path(root,'menu_SkillTreeSheetNew');buttons=swf.placed_path(sprites[panel['character']],'buttons');base=swf.multiply(panel['matrix'],buttons['matrix']);placements=[];zones=[[],[],[]]
for cf in range(3):
 for name,kind in [(f'btn_skill{i}','tree') for i in range(16)]+[(f'skill{i}','drag') for i in range(16)]+[(f'btn_activeskill0{i+1}_drop_all','slot') for i in range(3)]:
  p=next(p for p in sprites[493][cf].values() if p.get('name')==name)
  m=swf.multiply(base,p['matrix']);icon=swf.placed_path(sprites[p['character']],'btimg');im=swf.multiply(m,icon['matrix']);pos=int(name[9:]) if kind=='tree' else int(name[5:]) if kind=='drag' else int(name.split('_drop_all')[0][-1])-1
  placements.append('{'+str(cf)+','+str(pos)+',IconKind::'+kind+','+nums(im[:4]+[im[4]/20,im[5]/20])+','+json.dumps('menu_SkillTreeSheetNew/buttons/'+name+'/btimg')+'}')
  hit=next((x for x in sprites[p['character']][0].values() if x.get('name')=='hitzone'),None)
  if hit is None:hit=next(x for x in sprites[p['character']][0].values() if x['character']==81)
  verts=hitgeometry(hit['character'],swf.multiply(m,hit['matrix']));zones[cf].append('{HitKind::'+('assign' if kind=='slot' else 'select')+','+str(pos)+','+json.dumps(name)+',{'+','.join(nums(v) for v in verts)+'}}')
 p=swf.placed_path(sprites[panel['character']],'btn_add');m=swf.multiply(panel['matrix'],p['matrix']);hit=next(x for x in sprites[p['character']][0].values() if x['character']==477);verts=hitgeometry(477,swf.multiply(m,hit['matrix']));zones[cf].append('{HitKind::train,-1,"btn_add",{'+','.join(nums(v) for v in verts)+'}}')
lines.append('const std::vector<IconPlacement>& original_skill_icon_placements(){static const std::vector<IconPlacement> v{'+','.join(placements)+'};return v;}')
for cf in range(3):lines.append('static const std::vector<HitZone> zones'+str(cf)+'{'+','.join(zones[cf])+'};')
lines.append('const std::vector<HitZone>& original_skill_hit_zones(unsigned cf){switch(cf){case 0:return zones0;case 1:return zones1;case 2:return zones2;default:throw std::out_of_range("Skill source class frame");}}')
lines.insert(2,'#include <stdexcept>');lines.append('}')
Path(__file__).with_name('original_skill_art.cpp').write_text('\n'.join(lines)+'\n')
Path(__file__).with_name('source_skill_art.json').write_text(json.dumps({'source':str(path.relative_to(ROOT)),'sha256':hashlib.sha256(raw).hexdigest(),'icon_sprite':79,'icon_labels':labels[79],'class_sprite':493,'class_labels':labels[493],'placements':len(placements),'hit_zones_per_class':[len(z) for z in zones],'source_as':'character-menu-flow-v1/authored-actions.txt offsets20c3e..20dc0; 21935..2198f'},indent=2))
print('Generated',len(states),'icon states',len(placements),'placements')

