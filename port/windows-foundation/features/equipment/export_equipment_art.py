"""Export original inventory slot/button placements and item-icon timelines.

Read the character menu exporter's parsing prefix only: its publication path
is not executed, and generated files remain in the equipment feature.
"""
from pathlib import Path
import json,sys
directory=Path(__file__).resolve().parent
parser=directory.parent/'character_menu/export_art.py'
scope={'__file__':str(parser)}
exec(parser.read_text().split("if '--inspect' in sys.argv:")[0],scope)
sprites,labels=scope['sprites'],scope['labels']
if '--inspect' in sys.argv:
    print('icon labels',labels.get(424));print('fill labels',labels.get(414))
    print('settled slots',[(p.get('name'),p['matrix']) for p in sprites[435][-1].values() if p.get('name')])
    for ident in (435,439):
        print(ident,'labels',labels.get(ident), 'frames',len(sprites[ident]))
        print([(p.get('name'),p.get('character'),p['matrix']) for p in sprites[ident][0].values() if p.get('name')])
    for p in sprites[435][0].values():
        if p.get('name'):
            ident=p['character'];print(p['name'],ident,'labels',labels.get(ident),[(c.get('name'),c['character']) for c in sprites.get(ident,[{}])[0].values()])
    sys.exit(0)
swf=scope['swf'];shapes=scope['shapes'];edits=scope['edits'];skipped=[]
def walk(ident,m,path,art,text,category,depth=0):
    if depth>20:raise ValueError('equipment nesting limit')
    if ident in shapes:
        verts=[[(m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20,u,v] for x,y,u,v in shapes[ident]['triangles']]
        art.append((path,ident,verts));return
    if ident in edits:
        rec=edits[ident];bounds=rec['bounds_twips']
        corners=[[(m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20] for x,y in ((bounds[0],bounds[2]),(bounds[1],bounds[2]),(bounds[1],bounds[3]),(bounds[0],bounds[3]))]
        text.append((path,ident,rec,[min(v[0] for v in corners),max(v[0] for v in corners),min(v[1] for v in corners),max(v[1] for v in corners)],m));return
    if ident not in sprites:raise ValueError('unsupported original equipment shape '+str(ident))
    frame=labels[424][category] if ident==424 else 0
    for _,p in sorted(sprites[ident][frame].items()):
        name=p.get('name','leaf'+str(p['character']))
        if p['color'][0][3]==0 and p['color'][1][3]==0:continue
        if p.get('clip_depth') or p['color']!=[[1.]*4,[0.]*4]:
            skipped.append({'path':path+'/'+name,'reason':'source color/mask unsupported'});continue
        walk(p['character'],swf.multiply(m,p['matrix']),path+'/'+name,art,text,category,depth+1)
def numbers(v):return '{'+','.join(swf.cpp_number(float(n)) for n in v)+'}'
def batches(art):return '{'+','.join('{'+json.dumps(path)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}}' for path,ident,verts in art)+'}'
def fields(text):return '{'+','.join('{'+json.dumps(path)+','+str(ident)+','+str(rec['font'])+','+swf.cpp_number(rec['height_twips']/20)+','+numbers(bounds)+',{'+','.join(str(c) for c in rec['rgba'])+'},'+str(rec['layout'].get('align',0))+','+numbers(m[:4]+[m[4]/20,m[5]/20])+','+numbers([v/20 for v in rec['bounds_twips']])+','+numbers([rec['layout'].get(key,0)/20 for key in ('left_margin','right_margin','indent')])+','+swf.cpp_number(rec['layout'].get('leading',0)/20)+'}' for path,ident,rec,bounds,m in text)+'}'
panel=swf.placed_path(scope['root'],'menu_InventorySheetMain');anim=swf.placed_path(sprites[439],'inv_anim')
parent=swf.multiply(panel['matrix'],anim['matrix'])
mapping=[('btn_torso','torso'),('btn_main_hand','main_hand'),('btn_off_hand','off_hand'),('btn_feet','feet'),('btn_hands','hands'),('btn_ring1','ring'),('btn_ring2','ring'),('btn_waist','waist'),('btn_head','head')]
lines=['// Generated original SWF inventory category icons, item text and contours.','#include "equipment_menu.hpp"','namespace dh::foundation::equipment_menu {','const std::vector<SlotArt>& original_slot_art(){static const std::vector<SlotArt> slots{']
evidence=[]
for slot,(button,category) in enumerate(mapping):
    p=swf.placed_path(sprites[435],button);m=swf.multiply(parent,p['matrix']);path='menu_InventorySheetMain/inv_anim/'+button
    art=[];text=[];walk(p['character'],m,path,art,text,category)
    # Source MovieClip onRelease tests its visible vector artwork, not a guessed
    # rectangular cell. Original text object is not converted to a hit rectangle.
    vertices=[v for _,_,verts in art for v in verts]
    lines.append('{'+str(slot)+','+json.dumps(path)+','+json.dumps(category)+',{'+batches(art)+','+fields(text)+',{}},{'+','.join(numbers(v) for v in vertices)+'}},')
    evidence.append({'slot':slot,'button':button,'category':category,'matrix_twips':m,'shapes':[ident for _,ident,_ in art],'text_fields':[ident for _,ident,_,_,_ in text]})
lines+=['};return slots;}','}']
(directory/'original_equipment_art.cpp').write_text('\n'.join(lines)+'\n')
(directory/'source_equipment_layout.json').write_text(json.dumps({'source':str(scope['path'].relative_to(scope['ROOT'])),'sha256':scope['hashlib'].sha256(scope['raw']).hexdigest(),'mapping_source':'authored-actions.txt Init439 InvSlotId assignments; btimg gotoAndStop source category labels424','slots':evidence,'excluded':skipped},indent=2))
print('exported9source slots; excluded',len(skipped))
