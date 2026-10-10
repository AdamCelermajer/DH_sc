"""Export a bounded original character-menu artwork/layout subset."""
import sys,struct,zlib,json,hashlib,importlib.util,types
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
spec=importlib.util.spec_from_file_location('hud_export',Path(__file__).with_name('source_swf_geometry.py'))
swf=importlib.util.module_from_spec(spec);spec.loader.exec_module(swf)
path=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf'
raw=path.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
stage,at=swf.rect(data,8);at+=4
sprites={};shapes={};edits={};failures={};labels={}
hit_shapes={}
def solid_hit_styles(tag,at,ident):
    count=tag[at];at+=1
    if count==255:count=struct.unpack_from('<H',tag,at)[0];at+=2
    styles=[]
    for _ in range(count):
        if tag[at]!=0:raise ValueError('hit-only shape must have solid fill')
        color=list(tag[at+1:at+1+hit_color_bytes]);color+=[255]*(4-len(color))
        at+=1+hit_color_bytes
        # Texture mapping is unused/discarded for hit-only contours. A broad
        # coordinate embedding lets the existing contour tessellator run.
        styles.append({'matrix_twips':[1000000,0,0,1000000,-100000000,-100000000],'rgba':color})
    if tag[at]:raise ValueError('hit-only line style not supported')
    return styles,at+1
hit_globals=dict(swf.parse_shape.__globals__);hit_globals['read_bitmap_styles']=solid_hit_styles
parse_solid_hit=types.FunctionType(swf.parse_shape.__code__,hit_globals)
for code,tag in swf.tags(data,at,len(data)):
    if code==39:
        ident,count=struct.unpack_from('<HH',tag);sprites[ident]=swf.parse_timeline(tag,4)
        frame=0;labels[ident]={}
        for subcode,subtag in swf.tags(tag,4,len(tag)):
            if subcode==43:labels[ident][subtag.split(b'\0')[0].decode()]=frame
            elif subcode==1:frame+=1
    elif code in(2,22,32):
        ident=struct.unpack_from('<H',tag)[0];swf.ROLES[ident]=f'shape_{ident}'
        try:shapes[ident]=swf.parse_shape(code,tag)
        except ValueError as e:failures[ident]=str(e)
        if ident in(82,263) or ident in failures and 'unsupported fill 0' in failures[ident]:
            hit_color_bytes=4 if code==32 else 3
            try:hit_shapes[ident]=parse_solid_hit(code,tag)
            except ValueError:pass
    elif code==37:
        rec=swf.parse_edit_text(tag);edits[rec['character']]=rec
root=swf.parse_timeline(data,at)
faery_image_shape=shapes.get(538)
faery_image_source={}
if faery_image_shape:
    us=[v[2] for v in faery_image_shape['triangles']];vs=[v[3] for v in faery_image_shape['triangles']]
    faery_image_source={'shape_id':538,'fill_records':faery_image_shape['fill_records'],
        'u_range':[min(us),max(us)],'v_range':[min(vs),max(vs)],
        'atlas_alias':'SWF bitmap id1 -> menus/MenusGraphics_droid.tga'}
if '--inspect' in sys.argv:
    print('stage',stage,'shapes',len(shapes),'unsupported',len(failures))
    for ident,frames in [(0,root)]+[(i,sprites[i]) for i in(267,328,439,496,543)]:
        if not frames:continue
        named=[(p.get('name'),p.get('character'),p['matrix']) for p in frames[0].values() if p.get('name')]
        if named:print(ident,named)
    print('faery-image frames',len(sprites.get(543,[])),'labels',labels.get(543,{}))
    print('failures',failures)
    sys.exit(0)

def transform(m,x,y):return ((m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20)
def vertices(record,m):
    return [[*transform(m,x,y),u,v] for x,y,u,v in record['triangles']]
excluded={'hitzone','flush_text','btn_ClickPreventer','btn_ClickPreventer2','btn_Dragging','img_Faery','buttons','btimg'}
skipped=[];mask_applications=[];source_solids=[]
faery_frame=None
button_frame=None;element_frame=None;button_insert_at=None;image_insert_at=None
def mask_vertices(char,m,depth=0):
    if depth>30:raise ValueError('mask nesting bound')
    if char in shapes:return vertices(shapes[char],m)
    if char in hit_shapes:return vertices(hit_shapes[char],m)
    if char not in sprites or not sprites[char]:raise ValueError('unavailable mask definition '+str(char))
    out=[]
    for _,p in sorted(sprites[char][0].items()):out+=mask_vertices(p['character'],swf.multiply(m,p['matrix']),depth+1)
    return out
def edge(a,b,p):return (b[0]-a[0])*(p[1]-a[1])-(b[1]-a[1])*(p[0]-a[0])
def clip_triangle(poly,clip):
    direction=1 if edge(clip[0],clip[1],clip[2])>=0 else -1
    for a,b in zip(clip,clip[1:]+clip[:1]):
        if not poly:break
        result=[];previous=poly[-1];pv=edge(a,b,previous)*direction
        for current in poly:
            cv=edge(a,b,current)*direction
            if (cv>=-1e-6)!=(pv>=-1e-6):
                fraction=pv/(pv-cv)
                result.append([previous[i]+fraction*(current[i]-previous[i]) for i in range(4)])
            if cv>=-1e-6:result.append(current)
            previous=current;pv=cv
        poly=result
    return poly
def clipped(vertices,mask):
    out=[]
    for i in range(0,len(vertices),3):
        for j in range(0,len(mask),3):
            poly=clip_triangle(vertices[i:i+3],mask[j:j+3])
            for k in range(1,len(poly)-1):
                if abs(edge(poly[0],poly[k],poly[k+1]))>1e-6:out+=poly[:1]+poly[k:k+2]
    return out
def walk(char,m,path,art,text,depth=0):
    global button_insert_at,image_insert_at
    if depth>30:raise ValueError('menu nesting bound')
    if char in shapes:
        art.append((path,char,vertices(shapes[char],m)));return
    if char in edits:
        rec=edits[char];bounds=rec['bounds_twips'];corners=[transform(m,x,y) for x,y in((bounds[0],bounds[2]),(bounds[1],bounds[2]),(bounds[1],bounds[3]),(bounds[0],bounds[3]))]
        text.append((path,char,rec,[min(p[0] for p in corners),max(p[0] for p in corners),min(p[1] for p in corners),max(p[1] for p in corners)],m));return
    if char in hit_shapes and len(hit_shapes[char]['fill_records'])==1:
        record=hit_shapes[char]
        # Source solid fill bytes and contour, not an invented UI rectangle.
        source_solids.append((path,char,vertices(record,m),[c/255 for c in record['fill_records'][0]['rgba']],art[-1][0] if art else ''))
        return
    if char not in sprites:
        skipped.append({'path':path,'character':char,'reason':failures.get(char,'unsupported definition tag')});return
    if not sprites[char]:return
    frame=faery_frame if char==543 and faery_frame is not None else button_frame if char==536 and button_frame is not None else element_frame if char==534 and element_frame is not None else 0
    if char==262:
        # Actual menu_CharacterMenu Init actions58083..58299 assign these
        # named TabIcon frames; preserved labels from this original movie.
        tab_labels={'btnCharacterSheet':'CharacterSheet','btnInventoryTab':'Inventory','btnSkillTreeTab':'Skills',
                    'btnFaeriesTab':'Faery','btnQuestLogTab':'LogMap','btnMapTab':'Map'}
        for button,label in tab_labels.items():
            if '/'+button+'/' in path:frame=labels[262][label]
    if char==277:
        icon_labels={'btnDefenceTab':'Defence','btnMagicTab':'Magic','btnOffenseTab':'Offence',
                     'btnRecoveryTab':'Recovery','btnStatisticsTab':'Stats'}
        for button,label in icon_labels.items():
            if '/'+button+'/' in path:frame=labels[277][label]
    if char==347:
        for name in ('Fire','Water','Lightning','Earth','Air'):
            if path.endswith('/'+name):frame=labels[347][name]
    masks=[]
    for dep,p in sorted(sprites[char][frame].items()):
        masks=[active for active in masks if dep<=active[0]]
        name=p.get('name',str(dep))
        if char==547 and name=='buttons' and button_insert_at is None:button_insert_at=len(art)
        if char==547 and name=='img_Faery' and image_insert_at is None:image_insert_at=len(art)
        if name in excluded:continue
        if p['color'][0][3]==0 and p['color'][1][3]==0:continue
        if p.get('clip_depth'):
            try:mask=mask_vertices(p['character'],swf.multiply(m,p['matrix']))
            except ValueError as e:mask=[];skipped.append({'path':path+'/'+name,'reason':str(e)})
            masks.append((p['clip_depth'],mask));continue
        if p['color']!=[[1.]*4,[0.]*4] or p.get('placeobject3_extra_flags'):
            skipped.append({'path':path+'/'+name,'reason':'mask/color/filter branch excluded'});continue
        first_art=len(art);first_text=len(text)
        walk(p['character'],swf.multiply(m,p['matrix']),path+'/'+name,art,text,depth+1)
        for _,mask in masks:
            before=sum(len(verts) for _,_,verts in art[first_art:])
            for i in range(first_art,len(art)):
                role,ident,verts=art[i];art[i]=(role,ident,clipped(verts,mask))
            mask_applications.append({'path':path+'/'+name,'mask_vertices':len(mask),'input_vertices':before,
                                      'output_vertices':sum(len(verts) for _,_,verts in art[first_art:])})
        if masks and len(text)>first_text:
            skipped.append({'path':path+'/'+name,'reason':'masked text requires actual glyph clip sink; excluded'});del text[first_text:]
def placement(name):return swf.placed_path(root,name)
panel=placement('menu_FaerySheet')
def numbers(v):return '{'+','.join(swf.cpp_number(float(n)) for n in v)+'}'
def hit_walk(char,m,path,out,depth=0):
    if depth>30:raise ValueError('faery hit contour nesting bound')
    if char in hit_shapes:
        out.append((path,char,vertices(hit_shapes[char],m)));return
    if char in shapes:
        out.append((path,char,vertices(shapes[char],m)));return
    if char not in sprites or not sprites[char]:raise ValueError('missing authored faery hit contour '+str(char))
    for _,p in sorted(sprites[char][0].items()):
        hit_walk(p['character'],swf.multiply(m,p['matrix']),path+'/'+str(p.get('name','child')),out,depth+1)
art=[];text=[]
walk(panel['character'],panel['matrix'],panel['name'],art,text)
img_placement=swf.placed_path(sprites[panel['character']],'img_Faery')
faery_images=[]
for source_name in ('Celest','Rocky','Wetty','Windy','Hotty'):
    faery_frame=labels[543][source_name]
    image_art=[];image_text=[]
    walk(img_placement['character'],swf.multiply(panel['matrix'],img_placement['matrix']),
        'menu_FaerySheet/img_Faery/'+source_name,image_art,image_text)
    if not image_art:raise ValueError('missing actual img_Faery frame '+source_name)
    faery_images.append(image_art)
faery_frame=None

buttons=swf.placed_path(sprites[panel['character']],'buttons')
button_sprite=sprites[buttons['character']]
button_visuals=[];button_solids=[]
button_icon_names=('Lightning','Nature','Ice','Wind','Fire')
button_state_frames=(labels[536]['idle'],labels[536]['Focused'],16) # source gotoAndStop(17) is frame index 16
for slot in range(5):
    button=swf.placed_path(button_sprite,f'btn_GAMEPLAYMENUS_FAERY_{slot+1}')
    button_m=swf.multiply(swf.multiply(panel['matrix'],buttons['matrix']),button['matrix'])
    variants=[];solid_variants=[]
    for state_frame in button_state_frames:
        button_frame=state_frame;element_frame=labels[534][button_icon_names[slot]]
        state_art=[];state_text=[];solid_start=len(source_solids)
        walk(button['character'],button_m,f'menu_FaerySheet/buttons/slot{slot}/{state_frame}',state_art,state_text)
        if not state_art:raise ValueError(f'missing source faery button frame {slot}/{state_frame}')
        variants.append(state_art);solid_variants.append(source_solids[solid_start:])
    button_visuals.append(variants)
    button_solids.append(solid_variants)
button_frame=None;element_frame=None

menu=placement('menu_CharacterMenu')
tabs_child=swf.placed_path(sprites[menu['character']],'CharacterMenuTabs')
tab=swf.placed_path(sprites[tabs_child['character']],'btnFaeriesTab')
tab_matrix=swf.multiply(swf.multiply(menu['matrix'],tabs_child['matrix']),tab['matrix'])
hit=swf.placed_path(sprites[tab['character']],'hitzone')
hit_m=swf.multiply(tab_matrix,hit['matrix']);zone_art=[];zone_text=[]
if hit['character']==264:
    leaf=sprites[264][0][1]
    zone_art=[('btnFaeriesTab',263,vertices(hit_shapes[263],swf.multiply(hit_m,leaf['matrix'])))]
else:hit_walk(hit['character'],hit_m,'btnFaeriesTab',zone_art)
if not zone_art:raise ValueError('source Faery tab hit contour missing')
zone_vertices=[v for _,_,verts in zone_art for v in verts]

offsets=[]
for slot in range(1,6):
    button=swf.placed_path(button_sprite,f'btn_GAMEPLAYMENUS_FAERY_{slot}')
    button_m=swf.multiply(swf.multiply(panel['matrix'],buttons['matrix']),button['matrix'])
    hit=swf.placed_path(sprites[button['character']],'hitzone')
    hit_m=swf.multiply(button_m,hit['matrix']);hit_art=[];hit_text=[]
    hit_walk(hit['character'],hit_m,f'faery_{slot}',hit_art)
    if not hit_art:raise ValueError(f'source faery slot {slot} hit contour missing')
    offsets.append(hit_art)

lines=['// Generated from the exact original dqcharmenu_droid.swf faery page.','#include "faery_menu.hpp"','#include <cmath>','namespace dh::foundation::faery_menu {']
lines.append('static const SourceArt art{')
lines.append('{'+','.join('{'+json.dumps(role)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}}' for role,ident,verts in art if verts)+'},')
lines.append('{'+','.join('{'+json.dumps(p)+','+str(char)+','+str(rec['font'])+','+swf.cpp_number(rec['height_twips']/20)+','+numbers(bounds)+',{'+','.join(str(c) for c in rec['rgba'])+'},'+str(rec['layout'].get('align',0))+','+numbers(m[:4]+[m[4]/20,m[5]/20])+','+numbers([v/20 for v in rec['bounds_twips']])+','+numbers([rec['layout'].get(key,0)/20 for key in('left_margin','right_margin','indent')])+','+swf.cpp_number(rec['layout'].get('leading',0)/20)+'}' for p,char,rec,bounds,m in text)+'},')
slot_vertices=[[v for _,_,verts in group for v in verts] for group in offsets]
lines.append('{{'+','.join('{'+','.join(numbers(v) for v in verts)+'}' for verts in slot_vertices)+'}}')
lines.append('};')
lines.append('const SourceArt& original_faery_art(){return art;}')
lines.append('namespace { float edge(const HudGeometryVertex& a,const HudGeometryVertex& b,float x,float y){return (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);} bool contains(const std::vector<HudGeometryVertex>& t,float x,float y){for(std::size_t i=0;i+2<t.size();i+=3){const auto& a=t[i];const auto& b=t[i+1];const auto& c=t[i+2];const float area=edge(a,b,c.x,c.y);if(std::abs(area)<1e-6f)continue;const float aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return true;}return false;} }')
lines.append('int slot_at(float x,float y) noexcept{if(!std::isfinite(x)||!std::isfinite(y))return -1;const auto& slots=original_faery_art().slots;for(unsigned i=0;i<slots.size();++i)if(contains(slots[i],x,y))return static_cast<int>(i);return -1;}')
for i,group in enumerate(faery_images):
    lines.append('static const std::vector<HudGeometryBatch> image'+str(i)+'{'+','.join('{'+json.dumps(role)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}}' for role,ident,verts in group if verts)+'};')
for slot,variants in enumerate(button_visuals):
    for state,group in enumerate(variants):
        lines.append('static const std::vector<HudGeometryBatch> button'+str(slot)+'_'+str(state)+'{'+','.join('{'+json.dumps(role)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}}' for role,ident,verts in group if verts)+'};')
for slot,variants in enumerate(button_solids):
    for state,group in enumerate(variants):
        lines.append('static const std::vector<SolidArt> button_solid'+str(slot)+'_'+str(state)+'{'+','.join('{{'+json.dumps(role)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}},{'+','.join(swf.cpp_number(float(c)) for c in rgba)+'},'+json.dumps(after)+'}' for role,ident,verts,rgba,after in group)+'};')
lines.append('const std::vector<HudGeometryBatch>& original_faery_image(unsigned slot){switch(slot){')
for i in range(5):lines.append('case '+str(i)+':return image'+str(i)+';')
lines.append('default:throw std::out_of_range("faery source image slot");}}')
lines.append('const std::vector<HudGeometryBatch>& original_faery_button(unsigned slot,ButtonVisual state){switch(slot){')
for slot in range(5):lines.append('case '+str(slot)+':switch(state){case ButtonVisual::idle:return button'+str(slot)+'_0;case ButtonVisual::focused:return button'+str(slot)+'_1;case ButtonVisual::locked:return button'+str(slot)+'_2;}break;')
lines.append('default:break;}throw std::out_of_range("faery source button slot/state");}')
lines.append('const std::vector<SolidArt>& original_faery_button_solids(unsigned slot,ButtonVisual state){switch(slot){')
for slot in range(5):lines.append('case '+str(slot)+':switch(state){case ButtonVisual::idle:return button_solid'+str(slot)+'_0;case ButtonVisual::focused:return button_solid'+str(slot)+'_1;case ButtonVisual::locked:return button_solid'+str(slot)+'_2;}break;')
lines.append('default:break;}throw std::out_of_range("faery source solid button slot/state");}')
lines.append('std::size_t original_faery_buttons_insert_at() noexcept{return '+str(button_insert_at or 0)+';}')
lines.append('std::size_t original_faery_image_insert_at() noexcept{return '+str(image_insert_at or 0)+';}')
lines.append('const std::vector<HudGeometryVertex>& original_faery_tab_hit(){static const std::vector<HudGeometryVertex> hit{'+','.join(numbers(v) for v in zone_vertices)+'};return hit;}')
lines.append('}')
Path(__file__).with_name('original_art.cpp').write_text('\n'.join(lines)+'\n')
Path(__file__).with_name('source_layout.json').write_text(json.dumps({'source':str(path.relative_to(ROOT)),'sha256':hashlib.sha256(raw).hexdigest(),'stage':[480,320],'page':{'name':panel['name'],'character':panel['character'],'matrix':panel['matrix'],'static_bitmap_batches':len(art),'button_insert_at':button_insert_at,'image_insert_at':image_insert_at,'edit_text_fields':[{'path':p,'character_id':char,'font_id':rec['font'],'height':rec['height_twips']/20,'bounds':bounds,'rgba':rec['rgba'],'align':rec['layout'].get('align',0),'matrix':m[:4]+[m[4]/20,m[5]/20],'local_bounds':[v/20 for v in rec['bounds_twips']]} for p,char,rec,bounds,m in text]},'faery_tab_hit_vertices':len(zone_vertices),'faery_slot_hit_vertices':[sum(len(v[2]) for v in group) for group in offsets],'dynamic_projection':{'button_names':[f'btn_GAMEPLAYMENUS_FAERY_{i}' for i in range(1,6)],'authored_element_icon_path':'buttons/btn_GAMEPLAYMENUS_FAERY_n/FaeryElementImage','text_fields':['faery_desc/text','faery_spell/text','FaeryNameText/text','menu_title/txt_title']},'faery_images':{'sprite_character_id':543,'named_frames':{name:{'frame':labels[543][name],'bitmap_batches':len(group),'shape_ids':[ident for _,ident,verts in group]} for name,group in zip(('Celest','Rocky','Wetty','Windy','Hotty'),faery_images)}},'button_states':{'sprite_character_id':536,'state_frames':{'idle':labels[536]['idle'],'focused':labels[536]['Focused'],'locked_gotoAndStop_17_frame':16},'icon_names':button_icon_names,'variants':[{'slot':i,'icon':button_icon_names[i],'frames':[{'index':button_state_frames[state],'shape_ids':[ident for _,ident,verts in group if verts],'solid_overlays':[{'shape_id':sid,'rgba':rgba,'after_bitmap_role':after,'vertex_count':len(verts)} for _,sid,verts,rgba,after in button_solids[i][state]]} for state,group in enumerate(variants)]} for i,variants in enumerate(button_visuals)]},'common_composition':{'CharacterMenu_background_shape':90,'page_shape':497,'side_trim_shape':546,'CharacterSheetNew_retained':False},'faery_image_source':faery_image_source,'excluded':skipped,'mask_applications':mask_applications},indent=2))
print('generated original faery art',len(art),'bitmap batches,',len(text),'fields,',sum(sum(len(v[2]) for v in group) for group in offsets),'slot hit vertices; unsupported branches',len(skipped))
