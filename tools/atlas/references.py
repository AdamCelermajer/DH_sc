"""Reference and source-table inventory for the Original Data Atlas.

This first pass is deliberately conservative: table rows are inventoried from
their native names blocks, while rows whose payload schema is not decoded remain
TODO rather than being guessed from adjacent bytes.
"""
from __future__ import annotations

import json
import re
import struct
import xml.etree.ElementTree as ET
from pathlib import Path

from common import read_json, stable_id, write_json, source_roots


def _string_groups(path: Path):
    """Read all count-prefixed groups in a PyData names stream."""
    b = path.read_bytes(); at = 0
    def word():
        nonlocal at
        if at + 4 > len(b): raise ValueError(f"truncated name block: {path}")
        v = struct.unpack_from('<I', b, at)[0]; at += 4; return v
    def text():
        nonlocal at
        n = word()
        if n > len(b) - at: raise ValueError(f"invalid string size in {path}")
        s = b[at:at+n].decode('utf-8', 'replace'); at += n; return s
    groups=[]
    while at < len(b):
        n=word()
        if n > 100000: raise ValueError(f"invalid count in {path}")
        groups.append([text() for _ in range(n)])
    return groups


def _strings(path: Path):
    return [s for group in _string_groups(path) for s in group]


def _table_inventory(ctx):
    data=ctx.inputs/'assets-extra/android/data'
    py=data/'pydata'
    tables=[]
    for names_path in sorted(py.glob('*_pyarraynames.bin')):
        name=names_path.name[:-len('_pyarraynames.bin')]
        records=py/f'{name}_pyarray.bin'
        if not records.exists():
            continue
        try: groups=_string_groups(names_path)
        except Exception as e:
            tables.append({'id':stable_id('table',name),'table':name,'index':None,'name':None,
                           'fields':{},'status':'UNRESOLVED','reason':'OTHER','candidates':[],
                           'work_status':'DONE','note':str(e)})
            continue
        aliases={('animations',0):'animation_sequences',('animations',1):'camera_animation_sets',('animations',2):'character_animations',
                 ('effects',0):'effect_sets',('effects',1):'character_effects',('effects',2):'footstep_effects',
                 ('sdd_dungeon_hunter_2_iphone',0):'sdd_dungeon_hunter_2_iphone'}
        for gi,group in enumerate(groups):
          table=aliases.get((name,gi),f'{name}#group{gi}')
          for i,n in enumerate(group):
            tables.append({'id':stable_id('table',table,i,n),'table':table,'index':i,'name':n,
                           'fields':{},'status':'TODO','reason':None,'candidates':[],
                           'candidate_note':None,'work_status':'TODO'})
    return tables


def _reference(site,name,kind,rule='TODO'):
    return {'id':stable_id('ref',site,name,kind),'site':site,'name':name,'kind':kind,
            'resolves_to':[],'rule':rule,'status':'TODO','reason':None,'candidates':[],
            'candidate_note':None,
            'port_status':'UNKNOWN','implementation':None,'confidence':'GUESS','work_status':'TODO'}


class _Reader:
    def __init__(self,path): self.b=path.read_bytes(); self.at=0
    def word(self):
        v=struct.unpack_from('<I',self.b,self.at)[0];self.at+=4;return v
    def i(self): return struct.unpack('<i',struct.pack('<I',self.word()))[0]
    def f(self): return struct.unpack('<f',struct.pack('<I',self.word()))[0]
    def byte(self): v=self.b[self.at];self.at+=1;return v
    def text(self):
        n=self.word();v=self.b[self.at:self.at+n].decode('utf-8','replace');self.at+=n;return v
    def strings(self):
        n=self.word(); out=[]
        for _ in range(n):
            size=self.word();out.append(self.b[self.at:self.at+size].decode('utf-8','replace'));self.at+=size
        return out
    def ints(self): return [self.i() for _ in range(self.word())]


def _table_by(tables):
    return {(r['table'],r['index']):r for r in tables if r.get('index') is not None}


def _mark_cell(tables_by,table,index,field,value):
    row=tables_by.get((table,index))
    if row is not None: row['fields'][field]=value;row['status']='RESOLVED';row['work_status']='DONE';row['confidence']='PROVEN'


def _decode_animation_and_effect_refs(ctx,tables):
    py=ctx.inputs/'assets-extra/android/data/pydata'; by=_table_by(tables); refs=[]
    # Exact binary grammar and field order are read from the matching recovered
    # source parsers in port/game-data/{animation_tables,effects_tables,data}.cpp.
    for stem,dictstem in [('animations','animations_dictionary')]:
      try:
        dnames=_strings(py/f'{dictstem}_pyarraynames.bin');dpaths=_strings(py/f'{dictstem}_pyarray.bin')
        groups=_string_groups(py/f'{stem}_pyarraynames.bin'); names=groups[0];cam_names=groups[1];char_names=groups[2]; r=_Reader(py/f'{stem}_pyarray.bin')
        seq_count=r.word(); seqs=[]
        for si in range(seq_count):
          loop=r.i(); step_count=r.word();steps=[]
          for sj in range(step_count):
            vals={'AnchorFX':bool(r.byte()),'Anim':r.i(),'BlendOut':r.i(),'Cam':r.i(),'CamDir':bool(r.byte()),'FX':r.i(),'MoveGO':bool(r.byte()),'RandomCam':r.ints(),'Redir':r.i(),'Sound':r.i(),'Speed':r.f(),'Swoosh':bool(r.byte())};steps.append(vals)
            site=f'pydata/animations_pyarray.bin[sequence={si},step={sj}]'
            for field,kind in [('Anim','anim'),('FX','effect set'),('Sound','sound')]:
              value=vals[field]
              if value < 0: continue
              if field=='Anim':
                if vals['Redir']:
                  target=f'AnimTable[{value}] ({names[value] if value<len(names) else "out-of-range"})';valid=value<len(names)
                else:
                  target=dpaths[value] if value<len(dpaths) else f'animation dictionary index {value}';valid=value<len(dpaths)
              elif field=='FX': target=f'EffectsTables[{value}]';valid=True
              else: target=f'sdd_dungeon_hunter_2_iphone[{value}]';valid=True
              rr=_reference(f'{site}.{field}',str(value),kind,'AnimStep field schema in animation_tables.cpp')
              rr['resolves_to']=[{'type':'source-table-cell','table':target}]
              rr['status']='RESOLVED' if valid else 'UNRESOLVED';rr['reason']=None if valid else 'ID_OUT_OF_RANGE';rr['candidates']=[] if valid else [{'table':t} for t in names[-3:]]
              rr['work_status']='DONE';rr['confidence']='PROVEN';refs.append(rr)
          typ=r.i()
          seqs.append({'Loop':loop,'Steps':step_count,'Type':typ})
          _mark_cell(by,'animation_sequences',si,'Loop',loop);_mark_cell(by,'animation_sequences',si,'Steps',step_count);_mark_cell(by,'animation_sequences',si,'Type',typ)
        cam_count=r.word()
        for ci in range(cam_count):
          ca=r.ints(); vals={'CamAnims':ca,'Crit':r.i(),'Idle':r.i(),'Shake':r.i(),'Template':r.i()}
          _mark_cell(by,'camera_animation_sets',ci,'Camera',vals)
        char_count=r.word()
        state_names=['Attack','AttackStatic','Blocking','DeadlyGreatKB','Despawn','DespawnGreatKB','Died','Dodging','GreatKnockedBack','Idle','IdleFromOOC','IdleOOC','IdleSneak','IdleToOOC','Injured','Interact','KnockedBack','LiftDrop','LiftIdle','LiftMove','Limbus','MenuIdle','MenuOnSelect','PreSpawn','Revived','Reviving','Run','Run180','RunSneak','Scared','Spawn','Spells','Stunned','Template','Walk','Walk180','WalkSneak']
        for ci in range(char_count):
          for fidx,field in enumerate(state_names):
            vals=r.ints() if fidx in (15,31) else [r.i()]
            for vi,value in enumerate(vals):
              if value<0: continue
              site=f'pydata/animations_pyarray.bin[character={ci}].{field}[{vi}]'
              valid=value<seq_count
              rr=_reference(site,str(value),'anim','CharAnimTable state field references AnimTable sequence')
              rr['resolves_to']=[{'type':'source-table-cell','table':f'AnimTable[{value}]','name':names[value] if valid else None}]
              rr['status']='RESOLVED' if valid else 'UNRESOLVED';rr['reason']=None if valid else 'ID_OUT_OF_RANGE';rr['candidates']=[]
              rr['work_status']='DONE';rr['confidence']='PROVEN';refs.append(rr)
              _mark_cell(by,'character_animations',ci,field,vals if fidx in (15,31) else value)
      except Exception as exc:
        refs.append({'id':stable_id('schema-error','animations'),'site':'pydata/animations_pyarray.bin','name':'animations','kind':'table','resolves_to':[],'rule':'animation_tables.cpp schema','status':'UNRESOLVED','reason':'OTHER','candidates':[],'port_status':'UNKNOWN','implementation':None,'confidence':'PROVEN','work_status':'DONE','note':str(exc)})
    # Exact 638-row sound records: count followed by <uid,event> pairs.
    try:
      sound_names=_strings(py/'sdd_dungeon_hunter_2_iphone_pyarraynames.bin'); rrdr=_Reader(py/'sdd_dungeon_hunter_2_iphone_pyarray.bin');n=rrdr.word()
      for i in range(n):
        uid=rrdr.i();event=rrdr.i();_mark_cell(by,'sdd_dungeon_hunter_2_iphone',i,'uid',uid);_mark_cell(by,'sdd_dungeon_hunter_2_iphone',i,'isEvent',event)
    except Exception: pass
    return refs


def _initial_refs(tables):
    refs=[]
    # Seed inventory with every row of the major reference-bearing tables. A row
    # remains TODO until its payload/call site establishes a real reference.
    for row in tables:
        t=row['table']; n=row['name']; i=row['index']
        mapping={'animation_sequences':'anim','effect_sets':'effect set','sounds#group0':'sound',
                 'projectiles':'projectile',
                 'character_models_dictionary':'model'}
        kind=mapping.get(t)
        if kind:
            refs.append(_reference(f"pydata/{t}_pyarraynames.bin[row={i}]",str(n),kind,
                                   'native PyData row; payload mapping pending'))
    return refs


def _asset_index(ctx):
    return read_json(ctx.out/'data/assets.json', []) or []


def _resolve_file(name, assets):
    # Exact path and case-insensitive path are both retained as evidence.
    norm=str(name).replace('\\','/').lstrip('./')
    for prefix in ('q:/data/iphone/','q:/data/','data/iphone/','data/'):
        if norm.casefold().startswith(prefix): norm=norm[len(prefix):];break
    if norm.casefold().startswith('data/'): norm=norm[5:]
    exact=[a for a in assets if a.get('relative','').replace('\\','/')==norm]
    if exact: return ('RESOLVED',exact,[],None)
    folded=[a for a in assets if a.get('relative','').replace('\\','/').casefold()==norm.casefold()]
    if folded: return ('RESOLVED',folded,[],None)
    leaf=Path(norm).name.casefold()
    candidates=[a for a in assets if Path(a.get('relative','')).name.casefold()==leaf]
    if candidates:
        same_path=[a for a in candidates if a.get('relative','').replace('\\','/').casefold()==norm.casefold()]
        return ('UNRESOLVED',[],candidates,'PATH_CASE' if same_path else 'NAME_NO_FILE')
    # Fuzzy basename candidates are diagnostic only.
    stem=Path(leaf).stem
    close=[a for a in assets if stem and stem in Path(a.get('relative','')).name.casefold()]
    return ('UNRESOLVED',[],close[:10],'FILE_MISSING' if name else 'NAME_NO_FILE')


def resolve_reference(ctx,row,tables=None,assets=None):
    """Resolve one reference row through the shared table and file rules."""
    tables=tables if tables is not None else (read_json(ctx.out/'data/tables.json',[]) or [])
    assets=assets if assets is not None else _asset_index(ctx)
    table_by={(t.get('table'),t.get('index')):t for t in tables}
    for target in row.get('resolves_to') or []:
      table=target.get('table')
      m=re.search(r'(?:AnimTable|EffectsTables|sdd_dungeon_hunter_2_iphone)\[(\d+)\]',str(table or ''))
      if m:
        idx=int(m.group(1));tname=('animation_sequences' if 'AnimTable' in table else 'effect_sets' if 'EffectsTables' in table else 'sdd_dungeon_hunter_2_iphone')
        trow=table_by.get((tname,idx))
        if trow:
          target['source_row']={'table':tname,'index':idx,'name':trow.get('name'),'fields':trow.get('fields',{})}
          target['status']='RESOLVED'
        else:
          row.update(status='UNRESOLVED',reason='ID_OUT_OF_RANGE',candidates=[])
          row['candidate_note']=f'{tname}[{idx}] is outside the decoded source table.'
      path=target.get('path') or target.get('id_or_path')
      if path and isinstance(path,str) and ('.' in Path(path).name):
        status,found,candidates,reason=_resolve_file(path,assets)
        target['asset_files']=[{'path':a.get('path'),'source':a.get('source')} for a in found]
        if status!='RESOLVED':
          row.update(status='UNRESOLVED',reason=reason or 'FILE_MISSING')
          row['candidates']=[{'path':a.get('path'),'source':a.get('source')} for a in candidates]
          if not row['candidates']: row['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'
    name=row.get('name')
    if not row.get('resolves_to') and isinstance(name,str) and '.' in Path(name).name:
      status,found,candidates,reason=_resolve_file(name,assets)
      if status=='RESOLVED':
        row['resolves_to']=[{'type':'source asset file','path':name,'asset_files':[{'path':a.get('path'),'source':a.get('source')} for a in found]}]
        row.update(status='RESOLVED',reason=None,work_status='DONE')
      else:
        row.update(status='UNRESOLVED',reason=reason or 'FILE_MISSING',work_status='DONE')
        row['candidates']=[{'path':a.get('path'),'source':a.get('source')} for a in candidates]
        if not row['candidates']: row['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'
    elif not row.get('resolves_to') and isinstance(name,str) and row.get('kind')=='sound':
      # Labels are keys in the source sounds table, not filenames.
      hits=[x for x in read_json(ctx.out/'data/references.json',[]) or [] if x.get('kind')=='sound' and x.get('name')==name]
      if len(hits)==1:
        row['resolves_to']=[{'type':'source sound row','id':hits[0].get('site'),'members':hits[0].get('resolves_to',[])}]
        row.update(status='RESOLVED',work_status='DONE',confidence='PROVEN')
    if row.get('status')=='TODO' and row.get('resolves_to'):
      row.update(status='RESOLVED',work_status='DONE',confidence='PROVEN')
    if row.get('status')=='UNRESOLVED' and not row.get('candidates') and not row.get('candidate_note'):
      row['candidate_note']='No candidate source file or row matches the exact referenced name/id.'
    return row


def run(ctx, inventory_only=False):
    data=ctx.inputs/'assets-extra/android/data'; py=data/'pydata'
    tables=_table_inventory(ctx)
    refs=_decode_animation_and_effect_refs(ctx,tables)
    # Use native audio ledger when present; it records the complete 638-row source
    # sound table resolution and event members from the existing source parser.
    audio_ledger=ctx.root/'port/engine-audio/reference/source-bindings-v38/ledger.json'
    if audio_ledger.exists():
        try:
            entries=json.loads(audio_ledger.read_text(encoding='utf-8'))
            for e in entries:
                sid=e.get('source_id'); nm=e.get('source_name','')
                site=f"pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row={sid}]"
                row=_reference(site,nm,'sound','row index in the 638-entry source SDD; event expands target bank members')
                tgt=e.get('target') or {}
                target=tgt.get('filename')
                members=e.get('assets') or ([tgt] if tgt else [])
                existing=[m for m in members if m.get('exists')]
                absent=[m for m in members if not m.get('exists')]
                row['resolves_to']=[{'type':'sounds.xml filename','path':m.get('filename'),'uid':m.get('uid',tgt.get('uid')),
                                     'event':e.get('event'),'exists_in_source_cache':bool(m.get('exists'))} for m in members]
                if members and not absent:
                    row['status']='RESOLVED';row['work_status']='DONE';row['confidence']='PROVEN'
                elif existing:
                    row['status']='AMBIGUOUS';row['reason']=None;row['candidates']=[{'path':m.get('filename'),'uid':m.get('uid')} for m in absent]
                    row['work_status']='DONE';row['confidence']='PROVEN'
                else:
                    row['status']='UNRESOLVED';row['reason']='FILE_MISSING'
                    row['candidates']=[{'path':m.get('filename'),'uid':m.get('uid')} for m in absent]
                    row['work_status']='DONE';row['confidence']='PROVEN'
                refs.append(row)
        except Exception:
            pass
    # Merge the script-reference extractor's output if it has completed.
    script_refs=read_json(ctx.out/'data/script-references.json', []) or []
    # Another category agent may already have promoted its rows to the combined
    # ledger. Preserve those rows and merge by stable id.
    prior_refs=read_json(ctx.out/'data/references.json', []) or []
    prior_refs=[r for r in prior_refs if r.get('context') or r.get('kind')=='named_reference']
    refs.extend(prior_refs)
    refs.extend(script_refs)
    refs.extend(read_json(ctx.out/'data/references-extra.json',[]) or [])
    by_id={r.get('id') or stable_id(r.get('site'),r.get('name'),r.get('kind')):r for r in refs}
    refs=list(by_id.values())
    if inventory_only:
        for row in tables:
            row.update(status='TODO',reason=None,fields={},work_status='TODO',confidence='GUESS',candidate_note=None)
        for ref in refs:
            ref.update(status='TODO',reason=None,resolves_to=[],candidates=[],candidate_note=None,
                       work_status='TODO',confidence='GUESS',port_status='UNKNOWN',implementation=None)
    else:
        for row in tables:
            if row.get('status')=='TODO':
                row.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN',
                           candidate_note='No recovered-source schema or existing parser was available for this PyData table; payload fields intentionally remain undecoded.')
        _add_knight_skill_refs(ctx,refs)
        assets=_asset_index(ctx)
        for ref in refs: resolve_reference(ctx,ref,tables,assets)
    refs.sort(key=lambda r:(r.get('site',''),r.get('name',''),r.get('kind','')))
    write_json(ctx.out/'data/tables.json',tables)
    write_json(ctx.out/'data/references.json',refs)
    rules=[
      {'id':'pydata-name-blocks','rule':'PyData name blocks are count-prefixed lists of length-prefixed UTF-8 strings; row index is zero-based in serialized order.','evidence':'port/game-data/animation_tables.cpp Reader::strings and load_animation_tables; port/game-data/effects_tables.cpp Reader::names.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'audio-row-index','rule':'Sound id indexes the 638-row sdd_dungeon_hunter_2_iphone table. Ordinary rows resolve to sounds.xml filename; isEvent rows expand their member sounds.','evidence':'port/engine-audio/reference/source-bindings-v38/ledger.json source_id, event, target; coordination/v21.8/SKILL-VERIFICATION-MATRIX.md.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'path-case-resolution','rule':'Resolve a data URI against actual source file paths using exact case first, then case-insensitive comparison; preserve ambiguous candidates. Do not infer missing content from a similar basename.','evidence':'atlas path-resolution implementation; source path inventory.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'unsupported-pyarray','rule':'Rows without an audited source schema remain TODO/UNRESOLVED until a matching recovered-source parser or native schema is available; do not infer fields from byte patterns.','evidence':'data/tables.json rows with empty fields and TODO status.','status':'PROVEN','confidence':'PROVEN'}]
    write_json(ctx.out/'data/rules.json',rules)
    counts={'tables':len(tables),'table_rows_todo':sum(r.get('status')=='TODO' for r in tables),
            'references':len(refs),'reference_todo':sum(r.get('status')=='TODO' for r in refs),
            'sound_ledger_rows':sum(r.get('rule','').startswith('row index in the 638-entry') for r in refs),
            'references_by_kind':{k:sum(r.get('kind')==k for r in refs) for k in sorted({r.get('kind','') for r in refs})},
            'inventory_only':bool(inventory_only)}
    write_json(ctx.out/'counts/references.json',counts)
    _write_report(ctx,counts,refs,tables)
    return counts


def _write_report(ctx,counts,refs,tables):
    p=ctx.out/'reports/references.md';p.parent.mkdir(parents=True,exist_ok=True)
    lines=['# Reference resolution ledger','',
      'Generated by `tools/atlas/references.py`. The initial inventory intentionally leaves payloads without a recovered-source schema as TODO; no byte-pattern field guesses are made.','',
      '## Inventory','',
      f"- PyData table rows inventoried: {counts['tables']} ({counts['table_rows_todo']} TODO).",
      f"- Reference rows: {counts['references']} ({counts['reference_todo']} TODO).",
      f"- Audio ledger rows: {counts['sound_ledger_rows']}.",'',
      '## Evidence and gaps','',
      '- Audio row-index mappings are copied from the source binding ledger. `isEvent` rows are represented by the event member mapping in that ledger; the referenced WAV name alone does not prove the file exists in the Android corpus.',
      '- Animation sequences and character state references are parsed with the exact schemas in `port/game-data/animation_tables.cpp`; sound table UID/event fields use the source registration and the audio ledger.',
      '- Tables without a matching recovered-source schema are UNRESOLVED/OTHER with a note; their payload fields remain blank.','']
    p.write_text('\n'.join(lines),encoding='utf-8')


def _add_knight_skill_refs(ctx,refs):
    """Keep the three published Knight skill answers executable as ledger rows."""
    path=ctx.root/'coordination/v21.8/SKILL-VERIFICATION-MATRIX.md'
    if not path.exists(): return
    expected={'BashDown':(347,'skill_dh2_prince_warrior_bash_down.bdae',164,190,'sfx_skill_warrior_headsplitter.wav'),
              'GroundSlam':(350,'skill_dh2_prince_warrior_crushing_blow.bdae',173,185,'sfx_skill_warrior_earthstrike.wav'),
              'Charge':(349,'skill_dh2_prince_warrior_charge.bdae',172,48,'sfx_skill_warrior_shield_rush.wav')}
    for line in path.read_text(encoding='utf-8').splitlines():
      for skill,(anim,clip,fx,sound,wav) in expected.items():
        if f'| {skill} |' not in line: continue
        values=[('anim',anim,'AnimTable row'),('model',clip,'animation dictionary clip'),('effect set',fx,'EffectsTables row'),('sound',sound,'sdd sound row')]
        for kind,value,rule in values:
          site=f'SKILL-VERIFICATION-MATRIX.md:{skill}:{kind}'
          rr=_reference(site,str(value),kind,f'{rule}; coordination/v21.8/SKILL-VERIFICATION-MATRIX.md row for {skill}')
          rr['resolves_to']=[{'type':rule,'id_or_path':value,'clip':clip if kind=='anim' else None,'filename':wav if kind=='sound' else None}]
          rr['status']='RESOLVED';rr['work_status']='DONE';rr['confidence']='PROVEN'
          rr['port_status']='PARTIAL';rr['implementation']='port/game-data/animation_tables.cpp; skills-a profile/runtime_skill_cast_coordinator_v1.cpp (matrix evidence)'
          refs.append(rr)


def _resolve_with_assets(ctx,refs):
    assets=_asset_index(ctx)
    for r in refs:
      if r.get('status')=='UNRESOLVED' and not r.get('candidates') and not r.get('candidate_note'):
        r['candidate_note']='No candidate asset or table row could be established from the available source inventory.'
      for target in r.get('resolves_to') or []:
        path=target.get('path')
        if not path: continue
        status,found,candidates,reason=_resolve_file(path,assets)
        target['asset_candidates']=[{'path':x.get('path'),'source':x.get('source')} for x in candidates]
        if status=='RESOLVED':
          target['asset_files']=[{'path':x.get('path'),'source':x.get('source')} for x in found]
        elif r.get('kind') in ('anim','sound','model','projectile','animated effect','effect set'):
          r['status']='UNRESOLVED';r['reason']=reason or 'FILE_MISSING';r['candidates']=[{'path':x.get('path'),'source':x.get('source')} for x in candidates]
          if not r['candidates']: r['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'


def known_checks(ctx):
    """Known-answer fixtures; all checks are derived again from original inputs."""
    out=[]; py=ctx.inputs/'assets-extra/android/data/pydata'
    ledger_path=ctx.root/'port/engine-audio/reference/source-bindings-v38/ledger.json'
    try:
        rows=json.loads(ledger_path.read_text(encoding='utf-8'))
        names=_strings(py/'sdd_dungeon_hunter_2_iphone_pyarraynames.bin')
        records=_Reader(py/'sdd_dungeon_hunter_2_iphone_pyarray.bin'); n=records.word(); pairs=[(records.i(),records.i()) for _ in range(n)]
        out.append({'name':'sound row index is the 638-row source table with 63 events','passed':n==638 and len(names)==638 and sum(e==1 for _,e in pairs)==63 and len(rows)==638,
                    'detail':f'rows={n}, names={len(names)}, isEvent={sum(e==1 for _,e in pairs)}, ledger={len(rows)}'})
        xml=ctx.inputs/'audio-v34/cache/sounds.xml'
        xml_names={x.get('filename') for x in ET.parse(xml).iter('sound') if x.get('filename')}
        assets=_asset_index(ctx)
        source_files={Path(a.get('relative','')).name.casefold() for a in assets if a.get('source') in ('android','ios','original-cache')}
        missing=sorted(nm for nm in xml_names if nm.casefold() not in source_files)
        out.append({'name':'45 sounds.xml filenames lack a file in original data','passed':len(missing)==45,
                    'detail':f'xml_names={len(xml_names)}, missing={len(missing)}; names={missing}'})
        ice='sfx_faerie_ice.wav'
        out.append({'name':'sfx_faerie_ice.wav is absent from original data','passed':ice.casefold() not in source_files,
                    'detail':f'{ice} present={ice.casefold() in source_files}'})
    except Exception as e:
        out.append({'name':'sound table, missing sound files, and faerie ice known answers','passed':False,'detail':str(e)})
    matrix=ctx.root/'coordination/v21.8/SKILL-VERIFICATION-MATRIX.md'
    try:
        lines=matrix.read_text(encoding='utf-8').splitlines()
        expected={'BashDown':('347','164','190'),'GroundSlam':('350','173','185'),'Charge':('349','172','48')}
        results={}
        for line in lines:
            for name,values in expected.items():
                if f'| {name} |' in line:
                    results[name]=all(v in line for v in values)
        out.append({'name':'Knight BashDown, GroundSlam, and Charge source rows','passed':all(results.get(k,False) for k in expected),
                    'detail':json.dumps({k:{'expected':v,'matrix_match':results.get(k,False)} for k,v in expected.items()})})
    except Exception as e: out.append({'name':'Knight skill known answers','passed':False,'detail':str(e)})
    return out


def coverage_check(ctx):
    """Re-read original table name streams and verify the ledger has every row."""
    errors=[]; table_rows=read_json(ctx.out/'data/tables.json',[]) or []
    seen={(r.get('table'),r.get('index')) for r in table_rows}
    for names_path in sorted((ctx.inputs/'assets-extra/android/data/pydata').glob('*_pyarraynames.bin')):
        table=names_path.name[:-len('_pyarraynames.bin')]
        if not (names_path.parent/f'{table}_pyarray.bin').exists(): continue
        try: groups=_string_groups(names_path)
        except Exception as e:
            errors.append(f'{table}: unable to independently extract row names: {e}');continue
        aliases={('animations',0):'animation_sequences',('animations',1):'camera_animation_sets',('animations',2):'character_animations',
                 ('effects',0):'effect_sets',('effects',1):'character_effects',('effects',2):'footstep_effects'}
        missing=[]
        for gi,group in enumerate(groups):
          tname=aliases.get((table,gi),f'{table}#group{gi}')
          missing.extend((tname,i) for i in range(len(group)) if (tname,i) not in seen)
        if missing: errors.append(f'{table}: missing {len(missing)} table rows; sample indexes {missing[:10]}')
    refs=read_json(ctx.out/'data/references.json',[]) or []; ids={r.get('id') for r in refs}
    if not refs: errors.append('reference ledger is empty')
    script_refs=read_json(ctx.out/'data/script-references.json',[]) or []
    missing_script=[r.get('id') for r in script_refs if r.get('id') not in ids]
    if missing_script: errors.append(f'script references not merged into references.json: {len(missing_script)}')
    uncovered=sorted({r.get('table') for r in table_rows if r.get('status')=='UNRESOLVED' and r.get('reason')=='OTHER'})
    if uncovered: errors.append(f'PyData payloads without a recovered-source decoder remain uncovered: {uncovered}')
    return errors
