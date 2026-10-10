"""Export the retained original generic frontend movie graph and exact bitmaps.

The runtime projects explicit recovered flow state and actual source STOP frames;
this exporter does not replace arbitrary AVM/native callbacks.
"""
from pathlib import Path
import hashlib, importlib.util, json, struct, zlib, types, inspect

ROOT = Path(__file__).resolve().parents[5]
HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('source_swf', ROOT/'port/windows-foundation/tools/export_hud_geometry.py')
swf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(swf)
from shapely.geometry import Polygon
from shapely import constrained_delaunay_triangles
def tessellate_all(contours):
    if len(contours)==1: return swf.triangulate(contours[0])
    polygon=Polygon()
    for contour in contours: polygon=polygon.symmetric_difference(Polygon(contour))
    vertices=[]
    for triangle in constrained_delaunay_triangles(polygon).geoms:
        vertices.extend(list(triangle.exterior.coords)[:3])
    return vertices
# dqmenus exports bitmap8 as MenusGraphics_droid; bitmap1 is splash_final.
# The shared contour parser normalizes all coordinates to the 1024px atlas.
def menu_bitmap_styles(data, at, ident):
    count=data[at]; at+=1
    if count==255: count=struct.unpack_from('<H',data,at)[0]; at+=2
    styles=[]
    for _ in range(count):
        kind=data[at]; at+=1
        if kind not in (0x40,0x41,0x42,0x43): raise ValueError(f'Shape {ident}: unsupported fill {kind}')
        bitmap=struct.unpack_from('<H',data,at)[0]; at+=2
        matrix,at=swf.matrix(data,at)
        if bitmap not in bitmap_dimensions: raise ValueError(f'Shape {ident}: missing source bitmap {bitmap}')
        styles.append(dict(kind=kind,bitmap_id=bitmap,matrix_twips=matrix))
    if data[at]: raise ValueError(f'Shape {ident}: unsupported lines')
    return styles,at+1
swf.read_bitmap_styles=menu_bitmap_styles
source = ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus.swf'
raw = source.read_bytes()
data = raw[:8]+zlib.decompress(raw[8:]) if raw[:3] == b'CWS' else raw
stage, at = swf.rect(data, 8)
at += 4
bitmap_dimensions={struct.unpack_from('<H',tag)[0]:struct.unpack_from('<HH',tag,3) for code,tag in swf.tags(data,at,len(data)) if code in (20,36)}
sprites, shapes, edits, failures, hit_shapes, fonts, clip_labels, stop_frames = {}, {}, {}, {}, {}, {}, {}, {}
hit_color_bytes=4
def solid_styles(tag,at,ident):
    count=tag[at]; at+=1
    if count==255: count=struct.unpack_from('<H',tag,at)[0]; at+=2
    styles=[]
    for _ in range(count):
        if tag[at]!=0: raise ValueError('non-solid hit shape')
        at+=1+hit_color_bytes
        rgba=list(tag[at-hit_color_bytes:at])
        if hit_color_bytes==3:rgba.append(255)
        styles.append({'matrix_twips':[1000000,0,0,1000000,-100000000,-100000000],'rgba':rgba})
    if tag[at]: raise ValueError('hit line style')
    return styles,at+1
parser_source=inspect.getsource(swf.parse_shape).replace('    triangles = []','    triangles = []; paints = []; vertex_bitmap_ids=[]').replace('        if len(fill_contours) != 1:\n            raise ValueError(f"Shape {shape_id}: multiple contours need a hole-aware tessellator")\n        vertices = triangulate(fill_contours[0])','        vertices = tessellate_all(fill_contours)').replace('            triangles.append([x,y,u,v])','            triangles.append([x,y,u,v]); paints.append(styles[fill-1].get("rgba",[255,255,255,255]));vertex_bitmap_ids.append(styles[fill-1].get("bitmap_id",0))').replace('"contours_twips":contours,"triangles":triangles','"contours_twips":contours,"triangles":triangles,"paints":paints,"vertex_bitmap_ids":vertex_bitmap_ids')
hit_globals=dict(swf.parse_shape.__globals__);hit_globals.update(read_bitmap_styles=solid_styles,tessellate_all=tessellate_all)
exec(parser_source,hit_globals);solid_parser=hit_globals['parse_shape']
bitmap_globals=dict(swf.parse_shape.__globals__);bitmap_globals.update(read_bitmap_styles=menu_bitmap_styles,tessellate_all=tessellate_all)
exec(parser_source,bitmap_globals);swf.parse_shape=bitmap_globals['parse_shape']
for code, tag in swf.tags(data, at, len(data)):
    if code == 39:
        ident = struct.unpack_from('<H', tag)[0]
        sprites[ident] = swf.parse_timeline(tag, 4)
        frame=0;clip_labels[ident]={};stop_frames[ident]=[]
        for subcode,subtag in swf.tags(tag,4,len(tag)):
            if subcode==43: clip_labels[ident][subtag.split(b'\0')[0].decode()]=frame
            elif subcode==12 and subtag==b'\x07\x00':stop_frames[ident].append(frame)
            elif subcode==1: frame+=1
    elif code in (2,22,32):
        ident = struct.unpack_from('<H', tag)[0]
        swf.ROLES[ident] = 'shape_'+str(ident)
        try:
            shape=swf.parse_shape(code, tag)
            ids={s['bitmap_id'] for s in shape['fill_records']}
            shape['bitmap_id']=next(iter(ids)) if len(ids)==1 else 0
            # External splash TGA is 2048x1024 despite the placeholder SWF
            # bitmap definition's 1024x1024 dimensions. UVs address real pixels.
            for vertex,bitmap in zip(shape['triangles'],shape['vertex_bitmap_ids']):
                width,height=bitmap_dimensions[bitmap]
                vertex[2]*=1024/width;vertex[3]*=1024/height
            shapes[ident]=shape
        except ValueError as e: failures[ident] = str(e)
        try:
            hit_color_bytes=4 if code==32 else 3
            hit_shapes[ident]=solid_parser(code,tag)
        except ValueError: pass
    elif code == 37:
        rec = swf.parse_edit_text(tag)
        # Variable and optional InitialText terminate the DefineEditText record.
        strings=tag.split(b'\0')
        rec['initial_text']=strings[-2].decode('utf8',errors='replace') if rec['flags'] & 0x8000 else ''
        edits[rec['character']] = rec
    elif code in (48,75):
        ident=struct.unpack_from('<H',tag)[0]; length=tag[4]
        name=tag[5:5+length].decode('utf8').rstrip('\0'); count=struct.unpack_from('<H',tag,5+length)[0]
        metrics=[0,0,0]
        if tag[2]&0x80:
            offsets=7+length; size=4 if tag[2]&8 else 2
            codes=int.from_bytes(tag[offsets+count*size:offsets+(count+1)*size],'little')
            layout=offsets+codes+count*(2 if tag[2]&4 else 1)
            metrics=list(struct.unpack_from('<hhh',tag,layout))
            if code==75: metrics=[v/20 for v in metrics]
        fonts[ident]=dict(name=name,metrics=metrics)
root = swf.parse_timeline(data, at)
# Restore malformed Android keyboard bitmap transforms from the retained
# original generic SWF's corresponding named source components, not a painted
# substitute or atlas-region guess. Exact generic contours/UVs are retained;
# only local authored dimensions are mapped to the Android button envelope.
generic_source=source.with_name('dqmenus.swf')
generic_raw=generic_source.read_bytes()
generic_data=generic_raw[:8]+zlib.decompress(generic_raw[8:]) if generic_raw[:3]==b'CWS' else generic_raw
_,generic_at=swf.rect(generic_data,8);generic_at+=4
generic_shapes={}
bitmap_receipt=[]
for code,tag in swf.tags(generic_data,generic_at,len(generic_data)):
    if code not in (2,22,32): continue
    ident=struct.unpack_from('<H',tag)[0];swf.ROLES[ident]='generic_shape_'+str(ident)
    if ident not in (32,63,73,75,78,80,83,84,87,112,494): continue
    try: generic_shapes[ident]=swf.parse_shape(code,tag)
    except ValueError: pass
generic_rebindings={}
rebindings_report=[]
for target,origin in generic_rebindings.items():
    if target not in shapes or origin not in generic_shapes: continue
    source_shape=generic_shapes[origin];target_bounds=shapes[target]['bounds_twips'];origin_bounds=source_shape['bounds_twips']
    rebindings_report.append(dict(android_shape=target,generic_shape=origin,android_bounds_twips=target_bounds,generic_bounds_twips=origin_bounds,policy='Original generic contour and UV mapped affinely to retained Android authored local envelope; no painted substitute'))
    verts=[]
    for x,y,u,v in source_shape['triangles']:
        x=target_bounds[0]+(x-origin_bounds[0])*(target_bounds[1]-target_bounds[0])/(origin_bounds[1]-origin_bounds[0])
        y=target_bounds[2]+(y-origin_bounds[2])*(target_bounds[3]-target_bounds[2])/(origin_bounds[3]-origin_bounds[2])
        bitmap=source_shape['fill_records'][0]['bitmap_id']
        verts.append([x,y,u*.5 if bitmap==1 else u,v])
    shapes[target]['triangles']=verts
    shapes[target]['paints']=[[255,255,255,255] for _ in verts]
    shapes[target]['bitmap_id']=18 if target in (31,106) else 19 if target==59 else 1
omissions = []
for ident,solid in hit_shapes.items():
    if ident not in shapes:
        solid['bitmap_id']=0
        shapes[ident]=solid
def point(m,x,y): return [(m[0]*x+m[2]*y+m[4])/20,(m[1]*x+m[3]*y+m[5])/20]
def nums(values): return '{'+','.join(swf.cpp_number(float(v)) for v in values)+'}'
def collect_hits(char,m,path,button,hits,depth=0):
    if depth>30: raise ValueError('hit nesting bound')
    if char in shapes or char in hit_shapes:
        if button:
            shape=shapes.get(char,hit_shapes.get(char))
            hits.setdefault(button,[]).extend(point(m,x,y)+[0,0] for x,y,_,_ in shape['triangles'])
        return
    if char not in sprites or not sprites[char]: return
    for dep,p in sorted(sprites[char][0].items()):
        name=p.get('name',str(dep)); child=path+'/'+name
        nextbutton=child if name.startswith('btn') else button
        collect_hits(p['character'],swf.multiply(m,p['matrix']),child,nextbutton,hits,depth+1)
def walk(char,m,path,batches,text,hits,depth=0):
    if depth > 30: raise ValueError('source nesting bound')
    if char in shapes:
        vertices=[point(m,x,y)+[u,v] for x,y,u,v in shapes[char]['triangles']]
        batches.append((path,char,vertices))
        return
    if char in edits:
        r=edits[char]; b=r['bounds_twips']
        corners=[point(m,x,y) for x,y in ((b[0],b[2]),(b[1],b[2]),(b[1],b[3]),(b[0],b[3]))]
        bounds=[min(p[0] for p in corners),max(p[0] for p in corners),min(p[1] for p in corners),max(p[1] for p in corners)]
        text.append((path,char,r,bounds,m)); return
    if char not in sprites:
        omissions.append(dict(path=path,character=char,reason=failures.get(char,'unsupported definition'))); return
    if not sprites[char]: return
    for dep,p in sorted(sprites[char][0].items()):
        name=p.get('name',str(dep)); childpath=path+'/'+name
        if name in ('hitzone','flush_text','btn_ClickPreventer','btn_ClickPreventer2'):
            omissions.append(dict(path=childpath,character=p['character'],reason='input-only branch')); continue
        if p['color'][0][3]==0 and p['color'][1][3]==0: continue
        if p.get('clip_depth') or p['color']!=[[1.]*4,[0.]*4] or p.get('placeobject3_extra_flags'):
            omissions.append(dict(path=childpath,character=p['character'],reason='mask/color/filter unsupported in static contour adapter')); continue
        walk(p['character'],swf.multiply(m,p['matrix']),childpath,batches,text,hits,depth+1)

lines=['// Generated by export_art.py from original dqmenus_droid.swf.','#include "original_art.hpp"','namespace dh::foundation::frontend::art {']
screens=[]
for index,name in enumerate(('menu_MainMenu','menu_SelectClass','menu_EnterName','menu_StartGame')):
    batches=[]; text=[]; hits=[]
    p=swf.placed_path(root,name)
    walk(p['character'],p['matrix'],name,batches,text,hits)
    hit_regions={}
    collect_hits(p['character'],p['matrix'],name,'',hit_regions)
    lines.append('static const ScreenArt art'+str(index)+'{')
    lines.append('{'+','.join('{'+json.dumps(path)+','+str(char)+',{'+','.join(nums(v) for v in vs)+'}}' for path,char,vs in batches)+'},')
    lines.append('{'+','.join('{'+json.dumps(path)+','+str(char)+','+str(r['font'])+','+swf.cpp_number(r['height_twips']/20)+','+nums(bounds)+',{'+','.join(str(c) for c in r['rgba'])+'},'+str(r['layout'].get('align',0))+','+nums(m[:4]+[m[4]/20,m[5]/20])+','+nums([v/20 for v in r['bounds_twips']])+','+nums([r['layout'].get(k,0)/20 for k in ('left_margin','right_margin','indent')])+','+swf.cpp_number(r['layout'].get('leading',0)/20)+','+json.dumps(r['initial_text'])+','+nums(fonts.get(r['font'],{}).get('metrics',[0,0,0]))+','+str(bool(r['flags']&0x4000)).lower()+','+str(bool(r['flags']&0x2000)).lower()+'}' for path,char,r,bounds,m in text)+'},')
    lines.append('{'+','.join(str(shapes[char]['bitmap_id']) for _,char,_ in batches)+'},')
    lines.append('{'+','.join('{'+json.dumps(path)+',{'+','.join(nums(v) for v in vs)+'}}' for path,vs in hit_regions.items())+'},{},{}};')
    screens.append(dict(name=name,character=p['character'],batches=len(batches),triangles=sum(len(b[2])//3 for b in batches),text_fields=len(text)))
lines.extend(['const ScreenArt& original_art(Screen screen) noexcept {switch(screen){case Screen::select_class:return art1;case Screen::enter_name:return art2;case Screen::start_game:return art3;default:return art0;}}','}'])
print('source fonts',fonts)
lines.insert(-1,'const char* source_font_file(std::uint32_t id) noexcept {switch(id){'+''.join('case '+str(i)+':return '+json.dumps('data/arkham_reg.ttf' if f['name']=='Arkham' else 'data/Fontin SmallCaps.ttf' if f['name']=='Fontin SmallCaps' else '')+';' for i,f in fonts.items())+'default:return "";}}')
def table(path):
    data=path.read_bytes(); count=struct.unpack_from('<H',data)[0]; at=2; result=[]
    for _ in range(count):
        size=struct.unpack_from('<H',data,at)[0]; at+=2
        result.append(data[at:at+size].rstrip(b'\0').decode('utf8')); at+=size
    if at!=len(data): raise ValueError('source text trailing bytes')
    return result
labels={}
textroot=source.parents[1]/'text'
for group in ('menu','gameplaymenus','global'):
    keys=table(textroot/(group+'.symbols')); values=table(textroot/(group+'.english'))
    if len(keys)!=len(values): raise ValueError('source text count mismatch')
    labels.update(zip(keys,values))
lines.insert(-1,'std::string source_english_label(const TextField& field) {')
lines.insert(-1,'static const std::pair<const char*,const char*> labels[]{'+','.join('{'+json.dumps(k)+','+json.dumps(v,ensure_ascii=False)+'}' for k,v in labels.items())+'};')
lines.insert(-1,'std::size_t start=0;while(start<field.path.size()){const auto end=field.path.find(\'/\',start);auto key=field.path.substr(start,end-start);if(key.rfind("btn_",0)==0)key.erase(0,4);for(const auto& p:labels)if(key==p.first)return p.second;if(end==std::string::npos)break;start=end+1;}return field.initial_text; }')
(HERE/'original_art_data.cpp').write_text('\n'.join(lines)+'\n',encoding='utf8')
text_sources={str((textroot/(group+suffix)).relative_to(ROOT)):hashlib.sha256((textroot/(group+suffix)).read_bytes()).hexdigest() for group in ('menu','gameplaymenus','global') for suffix in ('.symbols','.english')}
report=dict(source=str(source.relative_to(ROOT)),sha256=hashlib.sha256(raw).hexdigest(),stage_twips=stage,bitmap_definitions={i:dict(asset='assets/source_bitmap_'+str(i)+'.tga',width=dimensions[0],height=dimensions[1]) for i,dimensions in bitmap_dimensions.items()},text_sources=text_sources,policy='Legacy inspection data retains source firstframes; runtime_source_data.cpp retains complete sprite frames, source labels and exact STOP actions for compose.cpp.',screens=screens,omissions=omissions)
report['generic_source']=str(generic_source.relative_to(ROOT));report['generic_sha256']=hashlib.sha256(generic_raw).hexdigest();report['original_keyboard_rebindings']=rebindings_report
report['runtime_policy']='compose.cpp traverses retained source timeline definitions with explicit flow PresentationState visibility, labels, localized/profile text. Shows use settled source tween endpoint. No general AVM execution.'
report['source_stop_frames']={str(i):stop_frames[i] for i in (69,89,408,416,497)}
(HERE/'source_layout.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf8')
print(json.dumps(screens))

# Retained source timeline graph for dynamic source-selected presentation.
def placement_cpp(p):
    m=p['matrix'][:4]+[p['matrix'][4]/20,p['matrix'][5]/20]
    return '{'+json.dumps(p.get('name',''))+','+str(p['character'])+','+nums(m)+','+nums(p['color'][0])+','+nums(p['color'][1])+','+str(p.get('clip_depth',0))+'}'
runtime=['// Generated retained original movie graph.','#include "runtime_source.hpp"','namespace dh::foundation::frontend::art::source {']
def parts_cpp(s):
    colors=s.get('paints',[[255]*4]*len(s['triangles']));ids=s.get('vertex_bitmap_ids',[s['bitmap_id']]*len(s['triangles']));groups=[]
    for vertex,color,bitmap in zip(s['triangles'],colors,ids):
        if not groups or groups[-1][0]!=(color,bitmap):groups.append(((color,bitmap),[]))
        groups[-1][1].append(vertex)
    return '{'+','.join('{'+str(bitmap)+','+nums([v/255 for v in color])+',{'+','.join(nums([x/20,y/20,u,v]) for x,y,u,v in vertices)+'}}' for (color,bitmap),vertices in groups)+'}'
for i,s in shapes.items(): runtime.append('static Shape shape_'+str(i)+'(){return {'+str(i)+','+str(s['bitmap_id'])+','+parts_cpp(s)+'};}')
for i,frames in sprites.items():runtime.append('static Clip clip_'+str(i)+'(){return {{'+','.join('{'+','.join(placement_cpp(p) for _,p in sorted(f.items()))+'}' for f in frames)+'},{'+','.join('{'+json.dumps(label)+','+str(frame)+'}' for label,frame in clip_labels[i].items())+'},{'+','.join(str(f) for f in stop_frames[i])+'}};}')
runtime.append('const Movie& movie(){static const Movie data=[](){Movie data;')
runtime.extend('data.shapes.emplace('+str(i)+',shape_'+str(i)+'());' for i in shapes)
runtime.extend('data.clips.emplace('+str(i)+',clip_'+str(i)+'());' for i in sprites)
runtime.extend('data.fields.emplace('+str(i)+',TextField{"",'+str(i)+','+str(r['font'])+','+swf.cpp_number(r['height_twips']/20)+','+nums([v/20 for v in r['bounds_twips']])+',{'+','.join(str(c) for c in r['rgba'])+'},'+str(r['layout'].get('align',0))+',{1,0,0,1,0,0},'+nums([v/20 for v in r['bounds_twips']])+','+nums([r['layout'].get(k,0)/20 for k in ('left_margin','right_margin','indent')])+','+swf.cpp_number(r['layout'].get('leading',0)/20)+','+json.dumps(r['initial_text'])+','+nums(fonts.get(r['font'],{}).get('metrics',[0,0,0]))+','+str(bool(r['flags']&0x4000)).lower()+','+str(bool(r['flags']&0x2000)).lower()+'});' for i,r in edits.items())
runtime.append('data.roots={'+','.join(placement_cpp(p) for _,p in sorted(root[0].items()) if p.get('name') in ('menu_bg','menu_MainMenu','menu_SelectClass','menu_EnterName','menu_StartGame'))+'};return data;}();return data;} }')
(HERE/'runtime_source_data.cpp').write_text('\n'.join(runtime)+'\n',encoding='utf8')
# Retain exact original generic gold-button bitmap62 as a conventional TGA.
# DecodeLossless2 source bytes are premultiplied ARGB; TGA needs straight BGRA.
for code,tag in swf.tags(generic_data,generic_at,len(generic_data)):
    if code not in (20,36) or tag[2]!=5:continue
    ident=struct.unpack_from('<H',tag)[0]
    width,height=struct.unpack_from('<HH',tag,3);pixels=zlib.decompress(tag[7:]);bgra=bytearray()
    for a,r,g,b in zip(pixels[0::4],pixels[1::4],pixels[2::4],pixels[3::4]):
        if code==20:a=255
        bgra.extend((min(255,b*255//a) if a else 0,min(255,g*255//a) if a else 0,min(255,r*255//a) if a else 0,a))
    header=struct.pack('<BBBHHBHHHHBB',0,0,2,0,0,0,0,0,width,height,32,0x28)
    assets=HERE/'assets';assets.mkdir(exist_ok=True)
    tga=header+bgra;filename='source_bitmap_'+str(ident)+'.tga'
    (assets/filename).write_bytes(tga)
    bitmap_receipt.append(dict(bitmap_id=ident,width=width,height=height,source_tag=code,compressed_payload_sha256=hashlib.sha256(tag).hexdigest(),asset=filename,sha256=hashlib.sha256(tga).hexdigest()))
(HERE/'assets/source_bitmap_receipt.json').write_text(json.dumps(dict(source=str(source.relative_to(ROOT)),source_sha256=hashlib.sha256(raw).hexdigest(),bitmaps=bitmap_receipt),indent=2)+'\n',encoding='utf8')
texture_paths='switch(id){'+''.join('case '+str(i)+':return '+json.dumps('port/windows-foundation/features/frontend/art/assets/source_bitmap_'+str(i)+'.tga')+';' for i in bitmap_dimensions)+'default:return "";}'
runtime.append('namespace dh::foundation::frontend::art { const char* source_texture_file(std::uint32_t id) noexcept {'+texture_paths+'} }')
(HERE/'runtime_source_data.cpp').write_text('\n'.join(runtime)+'\n',encoding='utf8')





