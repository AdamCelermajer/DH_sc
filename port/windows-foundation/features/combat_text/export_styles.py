"""Export original HUD combat text frames and Font512 layout; no invented art."""
import hashlib, importlib.util, json, struct, zlib
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
spec=importlib.util.spec_from_file_location('hud',ROOT/'port/windows-foundation/tools/export_hud_geometry.py')
h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h)
source=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf'
raw=source.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
stage,at=h.rect(data,8);fps=struct.unpack_from('<H',data,at)[0]/256.;at+=4
sprites={};edits={};font=None
for code,tag in h.tags(data,at,len(data)):
    if code==39:
        cid,count=struct.unpack_from('<HH',tag);frames=h.parse_timeline(tag,4)
        if len(frames)!=count:raise ValueError('Source frame count differs')
        sprites[cid]=frames
    elif code==37:
        record=h.parse_edit_text(tag);edits[record['character']]=record
    elif code in (48,75) and struct.unpack_from('<H',tag)[0]==512:
        flags=tag[2];n=tag[4];count=struct.unpack_from('<H',tag,5+n)[0];offset=7+n
        stride=4 if flags&8 else 2;co=int.from_bytes(tag[offset+count*stride:offset+(count+1)*stride],'little')
        code_at=offset+co;cb=2 if flags&4 else 1;layout=code_at+count*cb
        metrics=struct.unpack_from('<hhh',tag,layout);advances=struct.unpack_from('<'+'h'*count,tag,layout+6)
        codes=[int.from_bytes(tag[code_at+i*cb:code_at+(i+1)*cb],'little') for i in range(count)]
        bounds=layout+6+count*2
        for _ in range(count):_,bounds=h.rect(tag,bounds)
        pairs=[];pair_at=bounds+2
        for _ in range(struct.unpack_from('<H',tag,bounds)[0]):
            first=int.from_bytes(tag[pair_at:pair_at+cb],'little');second=int.from_bytes(tag[pair_at+cb:pair_at+2*cb],'little');adjust=struct.unpack_from('<h',tag,pair_at+2*cb)[0];pair_at+=2*cb+2;pairs.append([first,second,adjust/20 if code==75 else adjust])
        unit=20 if code==75 else 1
        font=dict(name=tag[5:5+n].decode().rstrip('\0'),metrics=[x/unit for x in metrics],advances=[[c,a/unit] for c,a in zip(codes,advances)],pairs=pairs)
if font is None:raise ValueError('Missing source Font512')
styles=[]
for placement in h.parse_timeline(data,at)[0].values():
    name=placement.get('name','')
    if not name.startswith('anim_sct_'):continue
    base=list(placement['matrix']);base[4]=int(base[4]/20)*20;base[5]=int(base[5]/20)*20
    output=[];field=None
    for frame in sprites[placement['character']]:
        text=[p for p in frame.values() if p.get('name')=='_text' and p['character'] in edits]
        if len(text)!=1:raise ValueError('Unsupported source combat field graph')
        p=text[0];current=edits[p['character']]
        if current['font']!=512:raise ValueError('Unproven combat font')
        if field is None:field=current
        elif field!=current:raise ValueError('Changing source text definition')
        matrix=h.multiply(base,p['matrix']);matrix[4]/=20;matrix[5]/=20
        output.append(matrix)
    styles.append(dict(name=name,frames=output,font=512,height=field['height_twips']/20,bounds=[x/20 for x in field['bounds_twips']],align=field['layout']['align'],metrics=font['metrics']))
def num(x):return h.cpp_number(x)
lines=['// Generated from original dqhud_droid.swf; regenerate with export_styles.py.']
lines.append('static constexpr const char* sourceCombatFontResource = '+json.dumps('data/'+font['name']+'.ttf')+';')
lines.append('static constexpr float sourceCombatFrameRate = '+num(fps)+';')
lines.append('static const std::vector<SourceFontAdvance> advances512 = {'+','.join('{'+str(c)+','+num(a)+'}' for c,a in font['advances'])+'};')
lines.append('static const std::vector<SourceFontPair> pairs512 = {'+','.join('{'+str(a)+','+str(b)+','+num(c)+'}' for a,b,c in font['pairs'])+'};')
for i,s in enumerate(styles):lines.append('static const std::array<float,6> frames_'+str(i)+'[] = {'+','.join('{'+','.join(num(x) for x in f)+'}' for f in s['frames'])+'};')
lines.append('static const CombatTextStyle sourceStyles[] = {')
for i,s in enumerate(styles):lines.append('{'+json.dumps(s['name'])+',frames_'+str(i)+','+str(len(s['frames']))+',512,'+num(s['height'])+',{'+','.join(num(x) for x in s['bounds'])+'},{'+','.join(num(x) for x in s['metrics'])+'},'+str(s['align'])+'},')
lines.append('};')
(HERE/'style_data.inc').write_text('\n'.join(lines)+'\n')
(HERE/'source_styles.json').write_text(json.dumps(dict(source=str(source),sha256=hashlib.sha256(raw).hexdigest(),fps=fps,styles=styles,font=font,native_color_policy='CombatFlashSwf draw overrides _text cxform with outcome ARGB after goto_frame; no invented alpha fade applied.'),indent=2)+'\n')
print(json.dumps({'styles':len(styles),'frames':sum(len(s['frames']) for s in styles),'font':font['name']}))
