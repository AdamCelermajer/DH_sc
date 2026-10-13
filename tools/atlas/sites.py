"""Extract exact XML attribute and bounded BRES event reference sites.

Binary event decoding mirrors port/engine-animation/event_track.cpp, using the
serialized offsets (not searching arbitrary BDAE bytes for supposed events).
Other embedded file strings are explicitly string-pool evidence, not events.
"""
from collections import Counter
import html
import re
import struct
from common import source_roots, source_site, stable_id, write_json, read_json

EXT = re.compile(r'\.(?:bdae|wav|vxn|ogg|mp3|luac|lua|pyscript|mlx|mgp|mgx|mvx|mvp|png|dds|tga|xml|lightset_xml|bin)$',re.I)
ATTR = re.compile(r'\b([A-Za-z_][\w:.-]*)\s*=\s*(["\'])(.*?)\2',re.S)
TAG = re.compile(r'<(?![!?/])([\w:.-]+)\b([^<>]*?)>',re.S)
SEMANTIC = re.compile(r'(?:sound|music|fx|effect|animation|animfile|script|projectile|modelfile|template|charactertype|skill)',re.I)

def ref(site,name,kind,rule,context,confidence='PROVEN'):
    return {'id':stable_id('site-ref',site,name,kind),'site':site,'name':name,'kind':kind,
            'rule':rule,'context':context,'confidence':confidence,'status':'TODO','work_status':'TODO',
            'resolves_to':[],'reason':None,'candidates':[], 'port_status':'UNKNOWN','implementation':None}

def kind_for(name,attr=''):
    lower=attr.lower();suffix=name.rsplit('.',1)[-1].lower() if '.' in name else ''
    if suffix in ('wav','vxn','ogg','mp3') or 'sound' in lower or 'music' in lower:return 'sound'
    if suffix in ('lua','luac','pyscript') or 'script' in lower:return 'script'
    if suffix=='mlx':return 'level'
    if suffix in ('png','tga','dds'):return 'texture'
    if 'fx' in lower or 'effect' in lower:return 'effect set'
    if 'anim' in lower:return 'anim'
    if 'projectile' in lower:return 'projectile'
    if 'template' in lower or 'charactertype' in lower:return 'character'
    if 'skill' in lower:return 'skill'
    if suffix in ('mgp','mgx','mvp','mvx','bdae'):return 'model'
    return 'data file'

def xml_refs(ctx,p):
    raw=p.read_bytes();text=raw.decode('utf-8-sig',errors='replace')
    # Strip comments with spaces so source line/offset identities remain exact.
    clean=re.sub(r'<!--[\s\S]*?-->',lambda m:re.sub(r'[^\n]',' ',m[0]),text)
    result=[];base=source_site(ctx,p)
    for ti,t in enumerate(TAG.finditer(clean)):
        tag,body=t.groups()
        for a in ATTR.finditer(body):
            field,_,value=a.groups();value=html.unescape(value)
            if not value or not (EXT.search(value) or SEMANTIC.search(field)):continue
            offset=t.start(2)+a.start();line=text.count('\n',0,offset)+1
            # Includes repeat references separately, even on the same line.
            site=f'{base}:{line}:attribute@{offset}:{field}'
            result.append(ref(site,value,kind_for(value,field),'XML_ATTRIBUTE',
                              {'file':base,'line':line,'byte_offset':len(text[:offset].encode('utf-8')),
                               'character_offset':offset,'element':tag,'element_index':ti,'attribute':field}))
    return result

def bres_events(raw):
    def word(at):
        if at<0 or at+4>len(raw):raise ValueError('word outside BRES')
        return struct.unpack_from('<I',raw,at)[0]
    def span(at,n):
        if n and (at==0 or at<0 or at+n>len(raw)):raise ValueError('event span outside BRES')
    def string(at):
        span(at,1);end=raw.find(b'\0',at,min(len(raw),at+4097))
        if end<0:raise ValueError('event string lacks bounded terminator')
        return raw[at:end].decode('ascii'),at
    if raw[:4]!=b'BRES':raise ValueError('not BRES')
    if len(raw)<60 or raw[4:6]!=b'\xfe\xff' or word(8)!=60 or word(12)!=len(raw):raise ValueError('BRES header mismatch')
    if raw[7]&0x80 or word(20):raise ValueError('BRES already relocated or external')
    root=word(32);span(root,192);record=word(root+0x2c)
    if not record:return []
    span(record,24);typ=word(record);count=word(record+8)
    if typ not in (1,3,4) or word(record+4)!=1 or count>100000 or word(record+16)!=count:raise ValueError('unsupported event record')
    times=word(record+12);groups=word(record+20);width={1:1,3:2,4:4}[typ]
    span(times,count*width);span(groups,count*8);result=[];prior=-(1<<31)
    for i in range(count):
        tick=int.from_bytes(raw[times+i*width:times+(i+1)*width],'little',signed=typ==4)
        if tick<prior:raise ValueError('event times out of order')
        prior=tick;n=word(groups+8*i);ls=word(groups+8*i+4)
        if n>1000000:raise ValueError('event group excessive')
        span(ls,n*4)
        for j in range(n):
            name,offset=string(word(ls+4*j))
            result.append({'name':name,'offset':offset,'key_index':i,'group_index':j,'time_type':typ,
                           'raw_tick':tick,'authored_ms':tick if typ==4 else tick*33.333332061767578125})
    return result

def binary_refs(ctx,p):
    raw=p.read_bytes();base=source_site(ctx,p);result=[];errors=[]
    if p.suffix.lower()=='.bdae':
        try:
            for e in bres_events(raw):
                event=e['name']
                kind='sound' if event.startswith('sfx_') else 'animation event'
                name=event[4:] if kind=='sound' else event
                site=f'{base}:event[{e["key_index"]},{e["group_index"]}]@0x{e["offset"]:x}'
                result.append(ref(site,name,kind,'BRES_EVENT_SFX_PREFIX' if kind=='sound' else 'BRES_EVENT_CALLBACK',
                                  {'file':base,'original_event':event,**e}))
        except (ValueError,UnicodeError) as ex:
            errors.append({'file':base,'reason':str(ex)})
    # Exact file names can also live in mesh/texture/script bindings. Lexical
    # string-pool sites retain their offset and do not pretend to be callbacks.
    for m in re.finditer(rb'[\x20-\x7e]{4,4096}\x00',raw):
        value=m[0][:-1].decode('ascii')
        if not EXT.search(value):continue
        site=f'{base}:string@0x{m.start():x}'
        result.append(ref(site,value,kind_for(value),'BINARY_FILE_STRING',
                          {'file':base,'byte_offset':m.start(),'evidence':'NUL-terminated file string; consumer role requires schema evidence'},'LIKELY'))
    return result,errors

def extract(ctx):
    refs=[];errors=[];counts=Counter();files=Counter()
    for label,folder in source_roots(ctx):
        for p in sorted(folder.rglob('*')):
            if not p.is_file():continue
            if p.suffix.lower() in ('.xml','.mlx','.mgp','.mgx','.mvp','.mvx','.lightset_xml'):
                refs.extend(xml_refs(ctx,p));files['xml']+=1
            elif p.suffix.lower() in ('.bdae',):
                rr,ee=binary_refs(ctx,p);refs.extend(rr);errors.extend(ee);files['bdae']+=1
    for r in refs:counts[r['kind']]+=1
    return refs,{'reference_sites':len(refs),'by_kind':dict(counts),'source_files':dict(files),'event_decode_errors':errors}

def run(ctx,inventory_only=False):
    refs,counts=extract(ctx)
    write_json(ctx.out/'data/references-extra.json',refs)
    write_json(ctx.out/'counts/sites.json',counts)
    return counts

def coverage_check(ctx):
    expected,counts=extract(ctx)
    saved=read_json(ctx.out/'data/references.json',[])
    have={r.get('id') for r in saved}
    errors=['missing XML/BRES reference site: '+r['site'] for r in expected if r['id'] not in have]
    errors += ['BRES event decode failed: '+r['file']+' '+r['reason'] for r in counts['event_decode_errors']]
    return errors
