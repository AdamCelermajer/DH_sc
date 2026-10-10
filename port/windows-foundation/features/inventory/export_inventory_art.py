"""Extract original equipped-category frames; writes inventory-owned outputs only."""
from pathlib import Path
import json,hashlib
ROOT=Path(__file__).resolve().parents[4]
p=ROOT/'port/windows-foundation/features/character_menu/export_art.py'
env={'__file__':str(p)}
# Reuse read-only original SWF parser definitions, before its output generator.
exec(p.read_text().split('tabs=placement')[0],env)
swf=env['swf'];sprites=env['sprites'];labels=env['labels']
# Preserve the native RenderCharacterPane callback locations as typed data.
# Character symbol 217 uses a bitmap fill the bounded art tessellator does not
# export, so recover its authored RECT directly from the source shape tag.
source_shape_bounds={}
for code,tag in swf.tags(env['data'],env['at'],len(env['data'])):
 if code in (2,22,32):
  ident=int.from_bytes(tag[:2],'little')
  source_shape_bounds[ident]=swf.rect(tag,2)[0]
def pane_manifest(name,after_role,before_role):
 root_matches=[(depth,p) for depth,p in env['root'][0].items() if p.get('name')==name]
 if len(root_matches)!=1:raise ValueError('Missing/ambiguous root pane owner '+name)
 root_depth,root_pane=root_matches[0]
 parent_frames=sprites[root_pane['character']]
 child_depth,pane=next((depth,p) for depth,p in parent_frames[0].items() if p.get('name')=='avatarpane')
 if pane.get('clip_depth') is not None:raise ValueError(name+' avatarpane unexpectedly has clipDepth')
 pane_frames=sprites[pane['character']]
 if len(pane_frames)!=1 or len(pane_frames[0])!=1:raise ValueError(name+' avatarpane symbol is not the expected single source shape')
 shape_depth,shape=next(iter(pane_frames[0].items()))
 shape_id=shape['character']
 if shape_id not in source_shape_bounds:raise ValueError('Missing authored pane bounds '+str(shape_id))
 bounds=source_shape_bounds[shape_id]
 matrix=swf.multiply(swf.multiply(root_pane['matrix'],pane['matrix']),shape['matrix'])
 corners=[(matrix[0]*x+matrix[2]*y+matrix[4],matrix[1]*x+matrix[3]*y+matrix[5])
          for x,y in ((bounds[0],bounds[2]),(bounds[1],bounds[2]),(bounds[1],bounds[3]),(bounds[0],bounds[3]))]
 box=[min(x for x,y in corners),min(y for x,y in corners),max(x for x,y in corners),max(y for x,y in corners)]
 matrix_px=matrix[:4]+[matrix[4]/20,matrix[5]/20]
 box_px=[v/20 for v in box]
 return {'path':'_root.'+name+'.avatarpane','root_depth':root_depth,'child_depth':child_depth,
         'shape_id':shape_id,'shape_depth':shape_depth,'shape_bounds_twips':bounds,
         'matrix':matrix_px,'bounds':box_px,'after_role':after_role,'before_role':before_role}
source_panes=[
 pane_manifest('menu_InventorySheetMain','', 'menu_InventorySheetMain/3/'),
 pane_manifest('menu_InventorySheetDetails','menu_InventorySheetDetails/btn_AutoEquip/','menu_InventorySheetDetails/237/')]
panel=env['placement']('menu_InventorySheetMain')
anim=swf.placed_path(sprites[panel['character']],'inv_anim')
parent=swf.multiply(panel['matrix'],anim['matrix'])
slots=[('btn_torso','torso',0),('btn_main_hand','main_hand',1),('btn_off_hand','off_hand',2),
 ('btn_feet','feet',3),('btn_hands','hands',4),('btn_ring1','ring',5),('btn_ring2','ring',6),
 ('btn_waist','waist',7),('btn_head','head',8),('btn_potions','potions',9)]
def numbers(v):return '{'+','.join(swf.cpp_number(float(n))for n in v)+'}'
def batches(art):return '{'+','.join('{'+json.dumps(path)+','+str(char)+',{'+','.join(numbers(v)for v in verts)+'}}'for path,char,verts in art)+'}'
def fields(text):
 return '{'+','.join('{'+json.dumps(path)+','+str(char)+','+str(rec['font'])+','+swf.cpp_number(rec['height_twips']/20)+','+numbers(bounds)+',{'+','.join(str(c)for c in rec['rgba'])+'},'+str(rec['layout'].get('align',0))+','+numbers(m[:4]+[m[4]/20,m[5]/20])+','+numbers([v/20 for v in rec['bounds_twips']])+','+numbers([rec['layout'].get(k,0)/20 for k in('left_margin','right_margin','indent')])+','+swf.cpp_number(rec['layout'].get('leading',0)/20)+'}'for path,char,rec,bounds,m in text)+'}'
def solids(records):return '{'+','.join('{{'+json.dumps(path)+','+str(char)+',{'+','.join(numbers(v)for v in verts)+'}},'+numbers(rgba)+','+json.dumps(after)+'}'for path,char,verts,rgba,after in records)+'}'
lines=['// Original dqcharmenu source contours; do not edit generated data.','#include "inventory_menu.hpp"','namespace dh::foundation::inventory {','const std::vector<SlotArt>& original_inventory_slots(){static const std::vector<SlotArt> slots{']
manifest=[]
for name,label,slot in slots:
 button=swf.placed_path(sprites[435],name);m=swf.multiply(parent,button['matrix']);path='menu_InventorySheetMain/inv_anim/'+name
 icon=swf.placed_path(sprites[button['character']],'btimg');im=swf.multiply(m,icon['matrix']);art=[];text=[]
 old=sprites[424];sprites[424]=[old[labels[424][label]]]
 env['walk'](424,im,path+'/btimg',art,text)
 sprites[424]=old
 ta=[];tt=[];tz=swf.placed_path(sprites[button['character']],'txtZone')
 env['walk'](tz['character'],swf.multiply(m,tz['matrix']),path+'/txtZone',ta,tt)
 # Original bitmap button contour defines hit region; not a guessed rectangle.
 border=next(v for v in sprites[button['character']][0].values()if v['character'] in (398,429))
 ha=[];ht=[];env['walk'](border['character'],swf.multiply(m,border['matrix']),path+'/border',ha,ht)
 hit=[vertex for _,_,verts in ha for vertex in verts]
 fills=[]
 fill=swf.placed_path(sprites[button['character']],'btfill');fm=swf.multiply(m,fill['matrix'])
 original=sprites[414]
 for frame in range(5):
  sprites[414]=[original[frame]];fa=[];ft=[];env['walk'](414,fm,path+'/btfill',fa,ft);fills.append(batches(fa))
 sprites[414]=original
 lines.append('{'+str(slot)+','+json.dumps(path)+','+batches(art)+','+fields(tt)+',{'+','.join(numbers(v)for v in hit)+'},{{'+','.join(fills)+'}}},')
 manifest.append({'button':name,'source_slot':slot,'icon_label':label,'icon_frame':labels[424][label], 'icon_shapes':[c for _,c,_ in art], 'text_fields':[p for p,_,_,_,_ in tt]})
lines+=['};return slots;}']
details=env['placement']('menu_InventorySheetDetails');dm=details['matrix'];dp='menu_InventorySheetDetails'
# Static list placements are data-driven; hide template rows from panel export.
old_list=sprites[111];sprites[111]=[{}];pa=[];pt=[];first_solid=len(env['source_solids'])
env['walk'](456,dm,dp,pa,pt);panel_solids=env['source_solids'][first_solid:];sprites[111]=old_list
# sprite456 depth212 contains sprite449. Authored actions select sprite449
# labels Idle (frame0) for ordinary items and disabled (frame23) for equipped
# items. Its three dynamic text fields move in that disabled frame; exporting
# only sprite456's initial display-list state loses the source placements.
transmute_path=dp+'/btn_GAMEPLAYMENUS_TRANSMUTE2'
pt=[record for record in pt if not record[0].startswith(transmute_path+'/')]
transmute_states=[]
transmute_placement=swf.placed_path(sprites[456],'btn_GAMEPLAYMENUS_TRANSMUTE2')
transmute_matrix=swf.multiply(dm,transmute_placement['matrix'])
old_transmute=sprites[449]
for state,frame in [('idle',0),('disabled',23)]:
 sprites[449]=[old_transmute[frame]];ta=[];tt=[];first_solid=len(env['source_solids'])
 env['walk'](449,transmute_matrix,transmute_path,ta,tt)
 transmute_states.append('{'+batches(ta)+','+fields(tt)+','+solids(env['source_solids'][first_solid:])+'}')
sprites[449]=old_transmute
rows=[]
list_p=swf.placed_path(sprites[456],'list');lm=swf.multiply(dm,list_p['matrix'])
for name,relative in [('btn_pre3',-4),('btn_pre2',-3),('btn_pre1',-2),('btn_pre0',-1),('btn_0',0),('btn_post0',1),('btn_post1',2),('btn_post2',3)]:
 row=swf.placed_path([old_list[3]],name);rm=swf.multiply(lm,row['matrix']);rp=dp+'/list/'+name;variants=[]
 original=sprites[108]
 for frame in (0,1):
  sprites[108]=[original[frame]];ra=[];rt=[];first_solid=len(env['source_solids']);env['walk'](108,rm,rp,ra,rt);variants.append('{'+batches(ra)+','+fields(rt)+','+solids(env['source_solids'][first_solid:])+'}')
 sprites[108]=original
 hit=env['vertices'](env['shapes'][102],rm)
 rows.append('{'+str(relative)+','+','.join(variants)+',{'+','.join(numbers(v)for v in hit)+'}}')
actions=[]
for name,action in [('btn_left','previous'),('btn_right','next'),('btn_EquipItem','equip'),('btn_Unequip','unequip'),('btn_Drop','drop'),('btn_GAMEPLAYMENUS_TRANSMUTE2','transmute'),('btn_AutoEquip','auto_equip')]:
 pp=swf.placed_path(sprites[456],name);aa=[];tt=[];env['walk'](pp['character'],swf.multiply(dm,pp['matrix']),dp+'/'+name,aa,tt)
 hit=[v for _,_,verts in aa for v in verts]
 actions.append('{DetailAction::'+action+','+json.dumps(dp+'/'+name)+',{'+','.join(numbers(v)for v in hit)+'}}')
lines.append('const DetailArt& original_inventory_details(){static const DetailArt art{{'+batches(pa)+','+fields(pt)+','+solids(panel_solids)+'},{'+','.join(rows)+'},{'+','.join(actions)+'},{'+','.join(transmute_states)+'}};return art;}')
array_init=lambda values:'{{'+','.join(swf.cpp_number(float(v)) for v in values)+'}}'
lines.append('const std::vector<SourceCharacterPaneV1>& original_inventory_character_panes_v1(){static const std::vector<SourceCharacterPaneV1> panes{'+','.join('{'+json.dumps(p['path'])+','+str(p['root_depth'])+'u,'+str(p['child_depth'])+'u,'+json.dumps(p['after_role'])+','+json.dumps(p['before_role'])+','+array_init(p['matrix'])+','+array_init(p['bounds'])+'}' for p in source_panes)+'};return panes;}')
lines.append('}')
lines[1]='#include "inventory_details.hpp"'
Path(__file__).with_name('original_inventory_art.cpp').write_text('\n'.join(lines)+'\n')
Path(__file__).with_name('source_inventory_layout.json').write_text(json.dumps({'source':str(env['path'].relative_to(ROOT)),'sha256':hashlib.sha256(env['raw']).hexdigest(),'slots':manifest,'character_panes':source_panes,'detail_transmute':{'sprite456_frame':0,'display_list_depth':212,'child_sprite':449,'states':[{'label':'idle','frame':0,'fields':['ButtonName/text','ValueText/value','ValueBox/value']},{'label':'disabled','frame':23,'fields':['ButtonName/text','ValueText/value','ValueBox/value']}],'policy':'Only selected item equipped state chooses sprite449 disabled; ordinary selected item uses idle. ButtonName, ValueText and ValueBox remain separate source fields; absent ItemTransmuteValueString is blank.'},'policy':'Settled inv_anim frame12 equals originalframe0. Categoryicons sprite424 label assignments. Original ItemTable IconName empty; no item-specific icons invented. Original colored fills exposed only for genuine ItemColor provider.'},indent=2)+'\n')
print('exported10 original categoryicons,50 sourcecolor fill states,10 slot textfields and authored hitcontours')
