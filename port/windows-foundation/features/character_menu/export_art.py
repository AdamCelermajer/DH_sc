"""Export a bounded original character-menu artwork/layout subset."""
import sys,struct,zlib,json,hashlib,importlib.util,types
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
spec=importlib.util.spec_from_file_location('hud_export',ROOT/'port/windows-foundation/tools/export_hud_geometry.py')
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
if '--inspect' in sys.argv:
    print('stage',stage,'shapes',len(shapes),'unsupported',len(failures))
    for ident,frames in [(0,root)]+[(i,sprites[i]) for i in(267,328,439,496)]:
        if not frames:continue
        named=[(p.get('name'),p.get('character'),p['matrix']) for p in frames[0].values() if p.get('name')]
        if named:print(ident,named)
    print('failures',failures)
    sys.exit(0)

def transform(m,x,y):return ((m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20)
def vertices(record,m):
    return [[*transform(m,x,y),u,v] for x,y,u,v in record['triangles']]
excluded={'hitzone','flush_text','btn_ClickPreventer','btn_ClickPreventer2','btn_Dragging',
    # P16 map: the legend popup is shown only after Show legend; exported separately as art4_legend.
    'LegendPopup'}
skipped=[];mask_applications=[];source_solids=[]
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
def walk(char,m,path,art,text,depth=0,active_tab='stats',has_stat_points=True):
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
    frame=0
    if char==265:
        # CharacterMenu tab buttons reveal their red TabIcon during the source
        # highlight tween. Freeze at its fully-visible authored frame (32).
        active_buttons={'stats':'btnCharacterSheet','equipment':'btnInventoryTab','skills':'btnSkillTreeTab','faery':'btnFaeriesTab','map':'btnMapTab'}
        if path.endswith('/'+active_buttons[active_tab]):frame=32
    if char==316 and not has_stat_points:
        # Original CharacterSheetNew.Init/point update calls deactivated on all
        # four training buttons when NativeGetPlayerStats.Stat_Points is zero.
        frame=labels[316]['deactivated']
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
    if char==654:
        # P16 map: LegendPopup idles off-screen at frame 0 and slides in over 'show' (frame 1) .. 'hide'-1 (frame 13);
        # the legend is shown at its settled frame.
        frame=labels[654]['hide']-1
    masks=[]
    for dep,p in sorted(sprites[char][frame].items()):
        masks=[active for active in masks if dep<=active[0]]
        name=p.get('name',str(dep))
        if name in excluded:continue
        if p['color'][0][3]==0 and p['color'][1][3]==0:continue
        if p.get('clip_depth'):
            try:mask=mask_vertices(p['character'],swf.multiply(m,p['matrix']))
            except ValueError as e:mask=[];skipped.append({'path':path+'/'+name,'reason':str(e)})
            masks.append((p['clip_depth'],mask));continue
        first_art=len(art);first_text=len(text)
        first_solid=len(source_solids)
        # sprite496's per-skill Grey child is a source solid-fill overlay
        # with the authored alpha multiplier 102/256. Preserve that exact
        # source color transform as a MenuSolidBatch instead of discarding it.
        # No other color/filter branch is admitted by this exception.
        skill_grey=(name=='Grey' and '/buttons/skill' in path and
                    p['color']==[[1.0,1.0,1.0,102/256],[0.0,0.0,0.0,0.0]] and
                    not p.get('placeobject3_extra_flags'))
        if skill_grey:
            walk(p['character'],swf.multiply(m,p['matrix']),path+'/'+name,art,text,depth+1,active_tab,has_stat_points)
            for index in range(first_solid,len(source_solids)):
                role,ident,verts,rgba,after=source_solids[index]
                rgba=[rgba[channel]*p['color'][0][channel]+p['color'][1][channel] for channel in range(4)]
                source_solids[index]=(role,ident,verts,rgba,after)
            continue
        if p['color']!=[[1.]*4,[0.]*4] or p.get('placeobject3_extra_flags'):
            skipped.append({'path':path+'/'+name,'reason':'mask/color/filter branch excluded'});continue
        walk(p['character'],swf.multiply(m,p['matrix']),path+'/'+name,art,text,depth+1,active_tab,has_stat_points)
        for _,mask in masks:
            before=sum(len(verts) for _,_,verts in art[first_art:])
            for i in range(first_art,len(art)):
                role,ident,verts=art[i];art[i]=(role,ident,clipped(verts,mask))
            mask_applications.append({'path':path+'/'+name,'mask_vertices':len(mask),'input_vertices':before,
                                      'output_vertices':sum(len(verts) for _,_,verts in art[first_art:])})
        if masks and len(text)>first_text:
            skipped.append({'path':path+'/'+name,'reason':'masked text requires actual glyph clip sink; excluded'});del text[first_text:]
def placement(name):return swf.placed_path(root,name)
tabs=placement('menu_CharacterMenu');panels=[placement(n) for n in('menu_CharacterSheetNew','menu_InventorySheetMain','menu_SkillTreeSheetNew')]+[None]
# P16 map: index 4 = the Map tab page (menu_MapSheet sprite 655); its RenderMap rectangle is drawn by the host.
panels.append(placement('menu_MapSheet'))
def numbers(v):return '{'+','.join(swf.cpp_number(float(n)) for n in v)+'}'
lines=['// Generated original dqcharmenu contours and source-selected CharacterMenu tab / stat-point states.','#include "character_menu.hpp"','namespace dh::foundation::character_menu {']
def emit_block(art_name,art,text,first_solid):
    # One MenuArt variant: contours, text fields and source solid batches (verbatim emission).
    lines.append(f'static const MenuArt {art_name}{{')
    lines.append('{'+','.join('{'+json.dumps(path)+','+str(char)+',{'+','.join(numbers(v) for v in verts)+'}}' for path,char,verts in art if verts)+'},')
    lines.append('{'+','.join('{'+json.dumps(path)+','+str(char)+','+str(rec['font'])+','+swf.cpp_number(rec['height_twips']/20)+','+numbers(bounds)+',{'+','.join(str(c) for c in rec['rgba'])+'},'+str(rec['layout'].get('align',0))+','+numbers(m[:4]+[m[4]/20,m[5]/20])+','+numbers([v/20 for v in rec['bounds_twips']])+','+numbers([rec['layout'].get(key,0)/20 for key in('left_margin','right_margin','indent')])+','+swf.cpp_number(rec['layout'].get('leading',0)/20)+'}' for path,char,rec,bounds,m in text)+'}')
    lines[-1]+=','
    lines.append('{'+','.join('{{'+json.dumps(role)+','+str(ident)+',{'+','.join(numbers(v) for v in verts)+'}},'+numbers(color)+','+json.dumps(after)+'}' for role,ident,verts,color,after in source_solids[first_solid:])+'}')
    lines.append('};')

for index,panel in enumerate(panels):
  variants=[(True,'art'+str(index))] if index else [(True,'art0'),(False,'art0_no_points')]
  for has_stat_points,art_name in variants:
    art=[];text=[]
    first_solid=len(source_solids)
    selected=[tabs] if panel is None else [tabs,panel]
    # Actual CharacterMenu.ChangeToStats source59545/59559 pushes BOTH these
    # source sheets; the second is the right-hand default statistics page.
    if index==0:selected.append(placement('menu_CharacterSheetStats'))
    for p in selected:walk(p['character'],p['matrix'],p.get('name','panel'),art,text,0,('stats','equipment','skills','faery','map')[index],has_stat_points)
    emit_block(art_name,art,text,first_solid)
    if index==4:
        # P16 map legend popup (LegendPopup inside menu_MapSheet), shown only while legend is on.
        legend_placed=swf.placed_path(sprites[panel['character']],'LegendPopup')
        legend_m=swf.multiply(panel['matrix'],legend_placed['matrix']);legend_art=[];legend_text=[];legend_first=len(source_solids)
        walk(legend_placed['character'],legend_m,'menu_MapSheet/LegendPopup',legend_art,legend_text,0,'map',True)
        emit_block('art4_legend',legend_art,legend_text,legend_first)
lines.append('const MenuArt& original_menu_art(Tab tab,bool has_stat_points){switch(tab){case Tab::equipment:return art1;case Tab::skills:return art2;case Tab::faery:return art3;case Tab::map:return art4;default:return has_stat_points?art0:art0_no_points;}}')
lines.append('const MenuArt& original_map_legend_art(){return art4_legend;}')
tabs_child=swf.placed_path(sprites[tabs['character']],'CharacterMenuTabs')
tab_matrix=swf.multiply(tabs['matrix'],tabs_child['matrix']);zones=[]
for name,action in [('btnCharacterSheet','stats'),('btnInventoryTab','equipment'),('btnSkillTreeTab','skills'),('btnFaeriesTab','faery'),('btnMapTab','map'),('btnBack','close')]:
    button=swf.placed_path(sprites[tabs_child['character']],name)
    m=swf.multiply(tab_matrix,button['matrix']);child_name='btimg' if action=='close' else 'hitzone'
    hit=swf.placed_path(sprites[button['character']],child_name)
    hit_m=swf.multiply(m,hit['matrix']);zone_art=[];zone_text=[]
    if hit['character']==264:
        leaf=sprites[264][0][1]
        zone_art=[(name,263,vertices(hit_shapes[263],swf.multiply(hit_m,leaf['matrix'])))]
    else:walk(hit['character'],hit_m,name,zone_art,zone_text)
    if not zone_art:raise ValueError('source hit contour missing '+name)
    zone_vertices=[v for _,_,verts in zone_art for v in verts]
    zones.append('{Action::'+action+','+json.dumps(name)+',{'+','.join(numbers(v) for v in zone_vertices)+'}}')
# P16 map controls (Show legend / Reset zoom, sprite 623 placed in menu_MapSheet). Hit contours
# come from the same walk as the visible button; the presenter accepts them only on the Map tab.
mapsheet=placement('menu_MapSheet')
for name,action in [('btn_Legend','map_legend'),('btn_ResetZoom','map_reset_zoom')]:
    button=swf.placed_path(sprites[mapsheet['character']],name)
    m=swf.multiply(mapsheet['matrix'],button['matrix']);zone_art=[];zone_text=[]
    walk(button['character'],m,name,zone_art,zone_text)
    if not zone_art:raise ValueError('source map control contour missing '+name)
    zone_vertices=[v for _,_,verts in zone_art for v in verts]
    zones.append('{Action::'+action+','+json.dumps(name)+',{'+','.join(numbers(v) for v in zone_vertices)+'}}')
lines.append('const std::vector<MenuHitZone>& original_menu_hit_zones(){static const std::vector<MenuHitZone> zones{'+','.join(zones)+'};return zones;}')
lines.append('}')
Path(__file__).with_name('original_art.cpp').write_text('\n'.join(lines)+'\n')
Path(__file__).with_name('source_layout.json').write_text(json.dumps({'source':str(path.relative_to(ROOT)),'sha256':hashlib.sha256(raw).hexdigest(),'frame_policy':'actual source defaultStats328+386; selected main tab icon sampled at frame32 for Stats/Inventory/Skills/Faery (full source alpha); Faery page art/content comes from its independent provider; source stat-training sprite316 frame10 when resolved Stat_Points property148 is zero; otherwise idle frame0; detail/resistance named frames retained; other AS dynamics unavailable','viewport':'actualFlashCamera mode0 independentaxisstretch; nofitletterbox','mask_applications':mask_applications,'excluded':skipped},indent=2))
