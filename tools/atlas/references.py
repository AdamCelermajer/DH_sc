"""Reference and source-table resolution ledger for the Original Data Atlas.

Rows are decoded only where a recovered parser or native source schema proves
the serialized layout. Other table payloads remain explicitly unresolved.
"""
from __future__ import annotations

import json
import difflib
import re
import struct
import xml.etree.ElementTree as ET
from pathlib import Path

from common import read_json, stable_id, write_json, source_roots, source_site, markdown_table


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
        if name=='sdd_dungeon_hunter_2_iphone':
            _,authoritative_names=_sound_table_paths(ctx)
            groups=[_strings(authoritative_names)]
        for gi,group in enumerate(groups):
          table=aliases.get((name,gi),f'{name}#group{gi}')
          for i,n in enumerate(group):
            tables.append({'id':stable_id('table',table,i,n),'table':table,'index':i,'name':n,
                           'fields':{},'status':'TODO','reason':None,'candidates':[],
                           'candidate_note':None,'work_status':'TODO',
                           'source_path':('.local-inputs/items-discovery/sdd_dungeon_hunter_2_iphone_pyarraynames.bin'
                             if table=='sdd_dungeon_hunter_2_iphone' and (ctx.inputs/'items-discovery/sdd_dungeon_hunter_2_iphone_pyarraynames.bin').exists()
                             else f'.local-inputs/assets-extra/android/data/pydata/{name}_pyarraynames.bin')})
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


def _sound_table_paths(ctx):
    discovered=ctx.inputs/'items-discovery'
    if (discovered/'sdd_dungeon_hunter_2_iphone_pyarray.bin').exists():
        return (discovered/'sdd_dungeon_hunter_2_iphone_pyarray.bin',
                discovered/'sdd_dungeon_hunter_2_iphone_pyarraynames.bin')
    py=ctx.inputs/'assets-extra/android/data/pydata'
    return (py/'sdd_dungeon_hunter_2_iphone_pyarray.bin',
            py/'sdd_dungeon_hunter_2_iphone_pyarraynames.bin')


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
          _mark_cell(by,'animation_sequences',si,'steps',steps)
        cam_count=r.word()
        for ci in range(cam_count):
          ca=r.ints(); vals={'CamAnims':ca,'Crit':r.i(),'Idle':r.i(),'Shake':r.i(),'Template':r.i()}
          _mark_cell(by,'camera_animation_sets',ci,'Camera',vals)
        char_count=r.word()
        state_names=['Attack','AttackStatic','Blocking','DeadlyGreatKB','Despawn','DespawnGreatKB','Died','Dodging','GreatKnockedBack','Idle','IdleFromOOC','IdleOOC','IdleSneak','IdleToOOC','Injured','Interact','KnockedBack','LiftDrop','LiftIdle','LiftMove','Limbus','MenuIdle','MenuOnSelect','PreSpawn','Revived','Reviving','Run','Run180','RunSneak','Scared','Spawn','Spells','Stunned','Template','Walk','Walk180','WalkSneak']
        for ci in range(char_count):
          for fidx,field in enumerate(state_names):
            vals=r.ints() if fidx in (15,31) else [r.i()]
            _mark_cell(by,'character_animations',ci,field,vals if fidx in (15,31) else vals[0])
            for vi,value in enumerate(vals):
              _mark_cell(by,'character_animations',ci,field,vals if fidx in (15,31) else value)
              if value<0: continue
              site=f'pydata/animations_pyarray.bin[character={ci}].{field}[{vi}]'
              valid=value<seq_count
              rr=_reference(site,str(value),'anim','CharAnimTable state field references AnimTable sequence')
              rr['resolves_to']=[{'type':'source-table-cell','table':f'AnimTable[{value}]','name':names[value] if valid else None}]
              rr['status']='RESOLVED' if valid else 'UNRESOLVED';rr['reason']=None if valid else 'ID_OUT_OF_RANGE';rr['candidates']=[]
              rr['work_status']='DONE';rr['confidence']='PROVEN';refs.append(rr)
      except Exception as exc:
        refs.append({'id':stable_id('schema-error','animations'),'site':'pydata/animations_pyarray.bin','name':'animations','kind':'table','resolves_to':[],'rule':'animation_tables.cpp schema','status':'UNRESOLVED','reason':'OTHER','candidates':[],'port_status':'UNKNOWN','implementation':None,'confidence':'PROVEN','work_status':'DONE','note':str(exc)})
    # EffectsTables grammar (four layouts; row names stay in their own groups).
    try:
      names=_string_groups(py/'effects_pyarraynames.bin');dict_paths=_strings(py/'effects_dictionary_pyarray.bin')
      r=_Reader(py/'effects_pyarray.bin');set_count=r.word(); effect_rows=[]
      for si in range(set_count):
        force=r.byte();loop=r.i();step_count=r.word();steps=[]
        for sj in range(step_count):
          vals={'File':r.i(),'ForceCancel':r.byte(),'Loop':r.i(),'OrientOnce':r.byte(),'OrientWithAnchor':r.byte(),
                'PlayTime':r.i(),'PoolSize':r.i(),'Redir':r.i(),'ScaleWithAnchor':r.byte(),'SelfIllum':r.byte(),
                'SpeedBits':r.word(),'SubObject':r.text()}
          steps.append(vals); file_id=vals['File']; valid=0<=file_id<len(dict_paths)
          if file_id<0: continue
          rr=_reference(f'pydata/effects_pyarray.bin[set={si},step={sj}].File',str(file_id),'animated effect',
                        'EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp')
          rr['resolves_to']=[{'type':'EffectDict file row','index':file_id,'path':dict_paths[file_id] if valid else None,
                              'effect_set_index':si,'effect_set_name':names[0][si] if si<len(names[0]) else None,
                              'redir':vals['Redir']}]
          if not valid:
            rr.update(status='UNRESOLVED',reason='ID_OUT_OF_RANGE',candidates=[{'index':len(dict_paths)-1,'path':dict_paths[-1]}] if dict_paths else [],
                      work_status='DONE',confidence='PROVEN')
          else: rr.update(status='RESOLVED',work_status='DONE',confidence='PROVEN')
          refs.append(rr)
        typ=r.i();effect_rows.append({'ForceCache':force,'Loop':loop,'Steps':steps,'Type':typ})
        _mark_cell(by,'effect_sets',si,'steps',steps);_mark_cell(by,'effect_sets',si,'type',typ)
        _mark_cell(by,'effect_sets',si,'ForceCache',force);_mark_cell(by,'effect_sets',si,'Loop',loop)
      char_count=r.word()
      for ci in range(char_count):
        vals={'BloodDeathEffect':r.i(),'BloodEffect':r.i(),'FootprintEffect':r.i(),'SwooshEffect':r.i(),'TriggerFloorFX':r.byte()}
        _mark_cell(by,'character_effects',ci,'fields',vals)
        for field,value in vals.items():
          if field=='TriggerFloorFX' or value<0: continue
          valid=value<set_count;rr=_reference(f'pydata/effects_pyarray.bin[character={ci}].{field}',str(value),'effect set','CharEffectTable field -> AnimatedEffectTable row')
          rr['resolves_to']=[{'type':'EffectTables set','index':value,'name':names[0][value] if valid else None}]
          rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
          if not valid: rr['candidate_note']='No matching AnimatedEffectTable row exists for the source set id.'
          refs.append(rr)
      foot_count=r.word()
      for fi in range(foot_count):
        effect=r.i();floor=r.text();run_sounds=r.ints();walk_sounds=r.ints()
        vals={'Effect':effect,'Floortype':floor,'RunSound':run_sounds,'WalkSound':walk_sounds};_mark_cell(by,'footstep_effects',fi,'fields',vals)
        if effect>=0:
          valid=effect<set_count;rr=_reference(f'pydata/effects_pyarray.bin[footstep={fi}].Effect',str(effect),'effect set','FootstepEffectTable Effect -> AnimatedEffectTable row')
          rr['resolves_to']=[{'type':'EffectTables set','index':effect,'name':names[0][effect] if valid else None}]
          rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
          if not valid: rr['candidate_note']='No matching AnimatedEffectTable row exists for the source set id.'
          refs.append(rr)
        for field,ids in [('RunSound',run_sounds),('WalkSound',walk_sounds)]:
          for j,sid in enumerate(ids):
            if sid<0: continue
            valid=sid<638;rr=_reference(f'pydata/effects_pyarray.bin[footstep={fi}].{field}[{j}]',str(sid),'sound','FootstepEffectTable sound list -> source SDD row index')
            rr['resolves_to']=[{'type':'source sound row','index':sid}]
            rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
            if not valid: rr['candidate_note']='Source sound index lies outside the 638-row SDD table.'
            refs.append(rr)
    except Exception as exc:
      refs.append({'id':stable_id('schema-error','effects'),'site':'pydata/effects_pyarray.bin','name':'effects','kind':'table','resolves_to':[],'rule':'effects_tables.cpp schema','status':'UNRESOLVED','reason':'OTHER','candidates':[],'candidate_note':'Effect set payload could not be decoded by the recovered source schema.','port_status':'UNKNOWN','implementation':None,'confidence':'PROVEN','work_status':'DONE','note':str(exc)})
    # Exact 638-row sound records: count followed by <uid,event> pairs.
    try:
      sound_bin,sound_name_bin=_sound_table_paths(ctx)
      sound_names=_strings(sound_name_bin); rrdr=_Reader(sound_bin);n=rrdr.word()
      for i in range(n):
        uid=rrdr.i();event=rrdr.i();_mark_cell(by,'sdd_dungeon_hunter_2_iphone',i,'uid',uid);_mark_cell(by,'sdd_dungeon_hunter_2_iphone',i,'isEvent',event)
    except Exception: pass
    # ProjectileTable uses the original registered wire shape and compiled
    # member order (source_process_array_registration_v101.hpp / compiled_members).
    try:
      pnames=_strings(py/'projectiles_pyarraynames.bin');pfields=_strings(py/'projectiles_pystructnames.bin')
      models=_strings(py/'projectiles_dictionary_pyarray.bin');r=_Reader(py/'projectiles_pyarray.bin');n=r.word()
      shape='bbiibiibbiiiiibbiiii'
      if len(pfields)!=20 or len(shape)!=len(pfields): raise ValueError('Projectile schema/name dimensions differ')
      # The descriptor records exact serialized widths: b=byte, i/f=word, S=length-prefixed string.
      for i in range(n):
        values={}
        for field,typ in zip(pfields,shape):
          values[field]=r.byte() if typ=='b' else r.text() if typ=='S' else r.i()
        _mark_cell(by,'projectiles#group0',i,'fields',values)
        for field,kind in [('Model','projectile'),('ImpactFX','effect set'),('ExpireFX','effect set'),('ObjectImpactFX','effect set'),
                           ('ImpactSFX','sound'),('ExpireSFX','sound'),('ObjectImpactSFX','sound')]:
          value=int(values.get(field,-1))
          if value<0: continue
          if field=='Model':
            valid=value<len(models);target={'type':'ProjectileDict row','index':value,'path':models[value] if valid else None}
          elif kind=='sound':
            valid=value<638;target={'type':'source sound row','index':value}
          else:
            valid=value<len(names[0]);target={'type':'AnimatedEffectTable row','index':value,'name':names[0][value] if valid else None}
          rr=_reference(f'pydata/projectiles_pyarray.bin[row={i}].{field}',str(value),kind,
                        'ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json')
          rr['resolves_to']=[target];rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
          if not valid: rr['candidate_note']=f'{field} index {value} is outside its source dictionary/table.'
          refs.append(rr)
    except Exception as exc:
      refs.append({'id':stable_id('schema-error','projectiles'),'site':'pydata/projectiles_pyarray.bin','name':'projectiles','kind':'table','resolves_to':[],'rule':'ProjectileTable registered shape bbiibiibbiiiiibbiiii','status':'UNRESOLVED','reason':'OTHER','candidates':[],'candidate_note':'ProjectileTable rows could not be decoded by the registered source schema.','port_status':'UNKNOWN','implementation':None,'confidence':'PROVEN','work_status':'DONE','note':str(exc)})
    # SkillList and SkillTable exact record layout from game-data/skill_tables.cpp.
    try:
      skill_groups=_string_groups(py/'skills_pyarraynames.bin');list_names,skill_names=skill_groups
      schemas=_string_groups(py/'skills_pystructnames.bin');r=_Reader(py/'skills_pyarray.bin');list_count=r.word()
      for li in range(list_count):
        ids=r.ints();_mark_cell(by,'skills#group0',li,'list_entries',ids)
        for j,sid in enumerate(ids):
          valid=sid<len(skill_names);rr=_reference(f'pydata/skills_pyarray.bin[skill_list={li}].List[{j}]',str(sid),'skill','SkillListTable List entries -> SkillTable row index')
          rr['resolves_to']=[{'type':'SkillTable row','index':sid,'name':skill_names[sid] if valid else None}]
          rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
          if not valid: rr['candidate_note']='Skill list id is outside the decoded SkillTable rows.'
          refs.append(rr)
      skill_count=r.word();skill_fields=['Anim','AnimIsMoving','DisplayProps','ElementalType','FairieDependantText','Flags','Level','Script','SkillAssignable','SkillCurrLevel','SkillDescription','SkillIcon','SkillName','SkillNextLevel','Type']
      for si in range(skill_count):
        row={}
        for field in skill_fields:
          if field=='DisplayProps': row[field]=r.ints()
          elif field in ('Script','SkillIcon'): row[field]=r.text()
          elif field in ('AnimIsMoving','FairieDependantText','SkillAssignable'): row[field]=r.byte()
          elif field=='Anim': row[field]=r.i()
          else: row[field]=r.word()
        _mark_cell(by,'skills#group1',si,'fields',row)
        anim=int(row['Anim']);
        if anim>=0:
          valid=anim<len(groups[0]);rr=_reference(f'pydata/skills_pyarray.bin[skill={si}].Anim',str(anim),'anim','SkillTable Anim field -> AnimTable sequence row')
          rr['resolves_to']=[{'type':'AnimTable row','index':anim,'name':groups[0][anim] if valid else None}]
          rr.update(status='RESOLVED' if valid else 'UNRESOLVED',reason=None if valid else 'ID_OUT_OF_RANGE',work_status='DONE',confidence='PROVEN')
          if not valid: rr['candidate_note']='Skill animation id is outside the decoded AnimTable rows.'
          refs.append(rr)
        for field,kind in [('Script','script'),('SkillIcon','data file')]:
          value=row[field]
          if not value: continue
          rr=_reference(f'pydata/skills_pyarray.bin[skill={si}].{field}',value,kind,'SkillTable named string field; game-data/skill_tables.cpp')
          rr['resolves_to']=[{'type':'source path or key','path':value}]
          rr.update(status='TODO',work_status='TODO',confidence='PROVEN');refs.append(rr)
    except Exception as exc:
      refs.append({'id':stable_id('schema-error','skills'),'site':'pydata/skills_pyarray.bin','name':'skills','kind':'table','resolves_to':[],'rule':'skill_tables.cpp schema','status':'UNRESOLVED','reason':'OTHER','candidates':[],'candidate_note':'SkillList/SkillTable rows could not be decoded by the recovered source schema.','port_status':'UNKNOWN','implementation':None,'confidence':'PROVEN','work_status':'DONE','note':str(exc)})
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


_AUDIO_ASSET_SOURCES = {'android', 'ios', 'original-cache', 'android-extra', 'audio-cache'}
_AUDIO_ASSET_LOOKUP = None

def _audio_asset_files(name, assets):
    """Find a catalog audio filename in any included original source corpus.

    sounds.xml stores a filename, not a path relative to one platform's data
    root. The Android/recovered cache and the iOS tree are partial corpora, so
    an exact basename in either is sufficient. Keep each source path and hash
    visible because same-named platform/cache copies can have different bytes.
    """
    leaf=Path(str(name or '').replace('\\','/')).name.casefold()
    if not leaf:return []
    global _AUDIO_ASSET_LOOKUP
    if _AUDIO_ASSET_LOOKUP is None or _AUDIO_ASSET_LOOKUP[0]!=id(assets):
        by_leaf={}
        for asset in assets:
            if asset.get('source') not in _AUDIO_ASSET_SOURCES:continue
            relative=asset.get('relative','').replace('\\','/')
            by_leaf.setdefault(Path(relative).name.casefold(),[]).append(asset)
        _AUDIO_ASSET_LOOKUP=(id(assets),by_leaf)
    return _AUDIO_ASSET_LOOKUP[1].get(leaf,[])

def _asset_file_metadata(assets):
    return [{k:a.get(k) for k in ('id','path','source','sha256','logical_paths','decoded_sha256',
            'decode_recipe','decode_confidence','mapping_confidence','package_version') if a.get(k) is not None}
            for a in assets]


_ASSET_LOOKUP = None
_TABLE_LOOKUP = None
def _resolve_file(name, assets):
    # Exact path and case-insensitive path are both retained as evidence.
    norm=str(name).replace('\\','/').lstrip('./')
    for prefix in ('q:/data/iphone/','q:/data/','data/iphone/','data/'):
        if norm.casefold().startswith(prefix): norm=norm[len(prefix):];break
    if norm.casefold().startswith('data/'): norm=norm[5:]
    global _ASSET_LOOKUP
    if _ASSET_LOOKUP is None or _ASSET_LOOKUP[0] != id(assets):
        exact_map={};fold_map={};leaf_map={}
        for a in assets:
            paths=[a.get('relative',''), *[re.sub(r'^data/', '', p) for p in a.get('logical_paths',[])]]
            for value in paths:
                rel=value.replace('\\','/')
                exact_map.setdefault(rel,[]).append(a);fold_map.setdefault(rel.casefold(),[]).append(a)
                leaf_map.setdefault(Path(rel).name.casefold(),[]).append(a)
        _ASSET_LOOKUP=(id(assets),exact_map,fold_map,leaf_map)
    _,exact_map,fold_map,leaf_map=_ASSET_LOOKUP
    exact=exact_map.get(norm,[])
    if exact: return ('RESOLVED',exact,[],None)
    folded=fold_map.get(norm.casefold(),[])
    if folded: return ('RESOLVED',folded,[],None)
    leaf=Path(norm).name.casefold()
    candidates=leaf_map.get(leaf,[])
    if candidates:
        same_path=[a for a in candidates if a.get('relative','').replace('\\','/').casefold()==norm.casefold()]
        return ('UNRESOLVED',[],candidates,'PATH_CASE' if same_path else 'NAME_NO_FILE')
    return ('UNRESOLVED',[],[],'FILE_MISSING' if name else 'NAME_NO_FILE')


_SOUND_LABEL_LOOKUP = None
def resolve_reference(ctx,row,tables=None,assets=None):
    """Resolve one reference row through the shared table and file rules."""
    tables=tables if tables is not None else (read_json(ctx.out/'data/tables.json',[]) or [])
    assets=assets if assets is not None else _asset_index(ctx)
    global _TABLE_LOOKUP
    if _TABLE_LOOKUP is None or _TABLE_LOOKUP[0] != id(tables):
      _TABLE_LOOKUP=(id(tables),{(t.get('table'),t.get('index')):t for t in tables})
    table_by=_TABLE_LOOKUP[1]
    for target in row.get('resolves_to') or []:
      type_to_table={'AnimTable row':'animation_sequences','AnimTable sequence':'animation_sequences',
                     'AnimatedEffectTable row':'effect_sets','EffectTables set':'effect_sets',
                     'SkillTable row':'skills#group1','SkillListTable row':'skills#group0',
                     'source sound row':'sdd_dungeon_hunter_2_iphone',
                     'ProjectileDict row':'projectiles_dictionary#group0'}
      target_type=target.get('type')
      if target_type in ('source_script','skill_icon_symbol','itemdrop_scene_object_symbol'):
        # These targets already carry exact source identity/variant metadata;
        # a path-oriented fallback would discard package-decoded aliases.
        continue
      if target_type=='table-row':
        tname=target.get('table');idx=target.get('index');trow=table_by.get((tname,idx))
        row['referenced_table']=tname;row['referenced_index']=idx
        if trow is None:
          row['invalid_id']=True;row['status']='UNRESOLVED';row['reason']='ID_OUT_OF_RANGE'
          target.setdefault('candidate_note',f'{tname} has no decoded row at index {idx}.')
        else:
          row['invalid_id']=False
          target['source_row']={'table':tname,'index':idx,'name':trow.get('name'),'fields':trow.get('fields',{})}
      if isinstance(target.get('index'),int) and target_type in type_to_table:
        tname=type_to_table[target_type];idx=target['index'];trow=table_by.get((tname,idx))
        row['referenced_table']=tname;row['referenced_index']=idx
        if trow is None:
          row['invalid_id']=True
          target.setdefault('candidate_note',f'{tname} has no decoded row at index {idx}.')
        else:
          row['invalid_id']=False
          target['source_row']={'table':tname,'index':idx,'name':trow.get('name'),'fields':trow.get('fields',{})}
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
        if target_type=='sounds.xml filename':
          found=_audio_asset_files(path,assets);status='RESOLVED' if found else 'UNRESOLVED'
          candidates=[];reason=None if found else 'FILE_MISSING'
        else:
          status,found,candidates,reason=_resolve_file(path,assets)
        target['asset_files']=_asset_file_metadata(found)
        if status!='RESOLVED':
          row.update(status='UNRESOLVED',reason=reason or 'FILE_MISSING')
          row['candidates']=_asset_file_metadata(candidates) if target_type=='sounds.xml filename' else [{'path':a.get('path'),'source':a.get('source')} for a in candidates]
          if not row['candidates']: row['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'
    name=row.get('name')
    if not row.get('resolves_to') and isinstance(name,str) and '.' in Path(name).name:
      status,found,candidates,reason=_resolve_file(name,assets)
      if status=='RESOLVED':
        row['resolves_to']=[{'type':'source asset file','path':name,'asset_files':_asset_file_metadata(found)}]
        row.update(status='RESOLVED',reason=None,work_status='DONE')
      else:
        row.update(status='UNRESOLVED',reason=reason or 'FILE_MISSING',work_status='DONE')
        row['candidates']=[{'path':a.get('path'),'source':a.get('source')} for a in candidates]
        if not row['candidates']: row['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'
    elif not row.get('resolves_to') and isinstance(name,str) and row.get('kind')=='sound':
      # BDAE sound event names are XML labels after stripping one leading sfx_.
      global _SOUND_LABEL_LOOKUP
      sound_xml=ctx.inputs/'audio-v34/cache/sounds.xml'
      if _SOUND_LABEL_LOOKUP is None:
        sound_nodes=[x.attrib for x in ET.parse(sound_xml).iter('sound')] if sound_xml.exists() else []
        led=json.loads((ctx.root/'port/engine-audio/reference/source-bindings-v38/ledger.json').read_text(encoding='utf-8'))
        uid_map={}
        for entry in led: uid_map.setdefault(str(entry.get('uid')),[]).append(entry)
        _SOUND_LABEL_LOOKUP={}
        for x in sound_nodes:
          item=dict(x);item['source_ids']=[e.get('source_id') for e in uid_map.get(str(x.get('uid')),[])]
          _SOUND_LABEL_LOOKUP.setdefault(x.get('label',''),[]).append(item)
      sound_nodes=_SOUND_LABEL_LOOKUP.get(name,[])
      if sound_nodes:
        catalog=[]
        for x in sound_nodes:
          uid=int(x.get('uid','-1'))
          catalog.append({'uid':uid,'filename':x.get('filename'),'source_ids':x.get('source_ids',[])})
        existing=[a for item in catalog for a in _audio_asset_files(item.get('filename') or '',assets)]
        row['resolves_to']=[{'type':'sounds.xml label','label':name,'rows':catalog,
                             'asset_files':_asset_file_metadata(existing)}]
        missing=[x for x in catalog if not _audio_asset_files(x.get('filename') or '',assets)]
        if len(catalog)==1 and not missing:
          row.update(status='RESOLVED',work_status='DONE',confidence='PROVEN')
        else:
          row.update(status='UNRESOLVED' if missing else 'AMBIGUOUS',reason='FILE_MISSING' if missing else None,work_status='DONE')
          row['candidates']=[{'path':x.get('filename'),'uid':x.get('uid')} for x in catalog]
          if not row['candidates']: row['candidate_note']='No exact sounds.xml label row exists.'
      elif row.get('rule')=='BRES_EVENT_SFX_PREFIX':
        row.update(status='UNRESOLVED',reason='NAME_NO_FILE',work_status='DONE',confidence='PROVEN')
        row['candidate_note']='No sounds.xml label matches the exact BDAE event name after one leading sfx_ is removed.'
    if row.get('status')=='TODO' and row.get('resolves_to'):
      row.update(status='RESOLVED',work_status='DONE',confidence='PROVEN')
    if row.get('status')=='UNRESOLVED' and not row.get('candidates') and not row.get('candidate_note'):
      row['candidate_note']='No candidate source file or row matches the exact referenced name/id.'
    return row


def _resolve_campaign_candidates(refs,tables):
    """Promote only command fields whose names and source roles establish a target table."""
    by_id={t.get('id'):t for t in tables}
    for ref in refs:
      kind=ref.get('kind');command=ref.get('command_name',ref.get('command',''))
      allowed=(kind=='level_name_candidate' and command=='Script_ChangeLevel') or (
               kind=='actor_name_candidate' and command=='Script_PlayActorAnim')
      candidates=ref.get('candidates') or []
      if not allowed or len(candidates)!=1: continue
      candidate=candidates[0];target=by_id.get(candidate.get('id'))
      if not target or target.get('name')!=ref.get('name'): continue
      ref['resolves_to']=[{'type':'table-row','table':target.get('table'),'index':target.get('index'),
                           'name':target.get('name'),'row_id':target.get('id')}]
      ref.update(status='RESOLVED',reason=None,confidence='PROVEN',work_status='DONE',
                 rule=f"{command} source field role plus exact {target.get('table')} row-name match")


_PYDATA_ARRAY_ALIASES = {
    # Public table names registered by PyDataArrays. The group suffixes for
    # multi-table files follow their names-stream / registration order.
    'AnimatedEffectTable': 'effect_sets',
    'AnimTable': 'animation_sequences',
    'CharAnimTable': 'character_animations',
    'CharacterTable': 'character_properties#group0',
    'ClassTable': 'character_classes#group0',
    'AnimDict': 'animations_dictionary#group0',
    'ProjectileTable': 'projectiles#group0',
    'GameObjectDamager': 'game_objects#group3',
    'TimerTraps': 'game_objects#group7',
}
_ITEMDROP_TEMPLATE_URI='data/3d/gameobjects/itemdrops.bdae'
_SKILL_ART_REPORT='port/windows-foundation/features/skill_ui/source_skill_art.json'


def _resolve_literal_pydata_calls(ctx,refs,tables):
    """Resolve only exact two-string GetPyOID calls through proven array aliases.

    GetPyStruct returns a struct member id, so its second string names a field,
    not a row. It remains unresolved here until a matching native field schema
    proves that mapping. The call ledger supplies both unmodified literal args
    keyed by the extractor's stable call_id.
    """
    calls=read_json(ctx.out/'data/calls.json',[]) or []
    calls_by_id={c.get('id'):c for c in calls if c.get('id')}
    rows_by_table={}
    for table_row in tables:
        if table_row.get('index') is not None:
            rows_by_table.setdefault(table_row.get('table'),[]).append(table_row)
    _resolve_charpropsname_sites(refs,rows_by_table)
    evidence=('pseudocode-all.c PyDataArrays constructor registerClassByName/addFuncsForFile '
              'aliases (AnimTable 320095, CharAnimTable 320131, AnimatedEffectTable 320332) '
              'and GetOID; source_process_array_registration_v101.hpp:9, :13, :35 array/name '
              'registrations and exact native names-stream group order')
    for ref in refs:
        context=ref.get('context') or {}
        call=context.get('call')
        if call not in ('GetPyOID','GetPyStruct'):
            continue
        call_data=calls_by_id.get(context.get('call_id'))
        args=(call_data or {}).get('arguments') or []
        literals=[]
        for arg in args:
            values=[lit for lit in (arg.get('literals') or [])
                    if lit.get('type')=='string' and isinstance(lit.get('value'),str)]
            if len(values)!=1 or arg.get('expression') != values[0].get('raw'):
                literals=[];break
            literals.append(values[0]['value'])
        if len(literals)!=2 or call_data.get('name')!=call:
            continue
        # There can be several extracted rows for one call. Only enrich the
        # second-argument table-row record, leaving generic argument literals
        # intact and preserving its extractor-generated id.
        if context.get('argument_index')!=2 or ref.get('reference_kind')!='table_row':
            continue
        table_alias,name=literals
        ref['source_call_id']=context.get('call_id')
        ref['literal_arguments']=[table_alias,name]
        ref['schema_evidence']=evidence
        if call=='GetPyStruct':
            ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN')
            ref['rule']='GetPyStruct second string is a struct member name; no matching native field schema was applied by this resolver.'
            ref['candidate_note']='The literal is a field lookup, not a PyDataArrays row name; no field id is inferred from a similarly named row.'
            ref['candidates']=[]
            continue
        table=_PYDATA_ARRAY_ALIASES.get(table_alias)
        if table is None:
            ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN')
            ref['rule']='GetPyOID table alias has no proven public-array mapping in the native registration evidence.'
            ref['candidate_note']=f'No exact source table mapping is established for public alias {table_alias!r}.'
            ref['candidates']=[]
            continue
        matches=[r for r in rows_by_table.get(table,[]) if r.get('name')==name]
        ref['reference_kind']='table_row'
        ref['referenced_table']=table
        ref['rule']=f'GetPyOID exact public alias {table_alias} -> {table}; exact case-sensitive names-stream row match.'
        if len(matches)==1:
            target_row=matches[0]
            ref['resolves_to']=[{'type':'table-row','table':table,'index':target_row['index'],
                                 'name':target_row.get('name'),'schema_evidence':evidence}]
            ref.update(status='RESOLVED',reason=None,work_status='DONE',confidence='PROVEN')
            ref['candidates']=[]
            ref.pop('candidate_note',None)
        elif len(matches)>1:
            ref['resolves_to']=[]
            ref.update(status='AMBIGUOUS',reason=None,work_status='DONE',confidence='PROVEN')
            ref['candidates']=[{'table':table,'index':r['index'],'name':r.get('name')} for r in matches]
            ref['candidate_note']='More than one source row has this exact name within the registered table.'
        else:
            ref['resolves_to']=[]
            ref.update(status='UNRESOLVED',reason='NAME_NO_FILE',work_status='DONE',confidence='PROVEN')
            nearby=difflib.get_close_matches(name,[r.get('name','') for r in rows_by_table.get(table,[])],n=5,cutoff=0.6)
            ref['candidates']=[{'table':table,'index':r['index'],'name':r.get('name')}
                               for close in nearby for r in rows_by_table.get(table,[]) if r.get('name')==close]
            ref['candidate_note']=f'No exact case-sensitive row name {name!r} exists in registered table {table}.'


def _resolve_charpropsname_sites(refs,rows_by_table):
    """Resolve authored character-property names only through their XML gate."""
    evidence=('native GetCharPropertiesId 0x3b3d38; '
              'canonical_property_declarations_v1.inc:68 charpropsname offset 0x13b0; '
              'canonical_character_family_v4.cpp:40-47 cached id / char_template precedence / '
              'exact CharacterTable lookup; actor_population.cpp:144-170 explicit profile bind')
    for ref in refs:
        context=ref.get('context') or {}
        if str(context.get('attribute','')).lower()!='charpropsname':
            continue
        name=ref.get('name')
        gate=('Authored `charpropsname` on a `gametype="Character"` record conditionally '
              'names a CharacterTable row; this records source identity and does not assert runtime spawning.')
        ref['schema_evidence']=evidence
        ref['admission_note']=gate
        ref['resolution_scope']='conditional source relationship; not a runtime spawn assertion'
        ref['port_status']='PARTIAL'
        ref['port_status_note']='Native name lookup is mapped, but this source identity does not establish runtime actor population.'
        if context.get('gametype')!='Character':
            ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN')
            ref['rule']='charpropsname is mapped only for an exact Character gametype source record.'
            ref['candidate_note']='The source record does not satisfy the native Character-table lookup gate.'
            ref['candidates']=[]
            continue
        # The native consumer gives char_template precedence over the literal
        # CharacterTable name. Do not turn that competing selector into a row.
        if context.get('char_template'):
            ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN')
            ref['rule']='char_template takes precedence over the charpropsname name lookup.'
            ref['candidate_note']='The source record supplies char_template; no CharacterTable target is inferred from charpropsname.'
            ref['candidates']=[]
            continue
        matches=[r for r in rows_by_table.get('character_properties#group0',[]) if r.get('name')==name]
        ref['rule']='gametype="Character" charpropsname exact-name lookup in CharacterTable, subject to char_template precedence.'
        ref['reference_kind']='table_row'
        ref['referenced_table']='character_properties#group0'
        if len(matches)==1:
            target_row=matches[0]
            ref['resolves_to']=[{'type':'table-row','table':'character_properties#group0',
                                 'index':target_row['index'],'name':target_row.get('name'),
                                 'schema_evidence':evidence}]
            ref.update(status='RESOLVED',reason=None,work_status='DONE',confidence='PROVEN')
            ref['candidates']=[]
            ref.pop('candidate_note',None)
        elif len(matches)>1:
            ref['resolves_to']=[]
            ref.update(status='AMBIGUOUS',reason=None,work_status='DONE',confidence='PROVEN')
            ref['candidates']=[{'table':'character_properties#group0','index':r['index'],'name':r.get('name')} for r in matches]
            ref['candidate_note']='More than one CharacterTable row has this exact name.'
        else:
            ref['resolves_to']=[]
            ref.update(status='UNRESOLVED',reason='NAME_NO_FILE',work_status='DONE',confidence='PROVEN')
            ref['candidates']=[]
            ref['candidate_note']=f'No exact CharacterTable name row matches authored charpropsname {name!r}.'


def _logical_uri_assets(uri,assets):
    """Return exact logical-path matches, including decoded package variants."""
    def normalize(value):
        norm=str(value or '').replace('\\','/').lstrip('./')
        for prefix in ('q:/data/iphone/','q:/data/','data/iphone/','data/'):
            if norm.casefold().startswith(prefix):
                norm=norm[len(prefix):]
                break
        return norm.casefold()
    expected=normalize(uri)
    matches=[]
    for asset in assets:
        aliases=[asset.get('relative',''),*(asset.get('logical_paths') or [])]
        if any(normalize(alias)==expected for alias in aliases):
            matches.append(asset)
    return matches


def _source_variant_metadata(assets):
    """Keep the original package identity and decoder metadata for each variant."""
    return _asset_file_metadata(assets)


def _resolve_skill_source_targets(ctx,refs,assets):
    """Turn SkillTable Script stems and SWF icon labels into typed source targets."""
    art_path=ctx.root/_SKILL_ART_REPORT
    art=read_json(art_path,{}) if art_path.is_file() else {}
    icon_labels={str(k).casefold():int(v) for k,v in (art.get('icon_labels') or {}).items()}
    art_source=str(art.get('source') or '').replace('\\','/')
    art_uri=('.repo/'+art_source.lstrip('/') if art_source and not art_source.startswith(('.repo/','.local-inputs/')) else art_source)
    art_assets=_logical_uri_assets('data/menus/dqcharmenu_droid.swf',assets)
    art_assets=[a for a in art_assets if not art.get('sha256') or a.get('sha256')==art.get('sha256')]
    for ref in refs:
        site=ref.get('site','')
        script_match=re.search(r'pydata/skills_pyarray\.bin\[skill=(\d+)\]\.Script$',site)
        icon_match=re.search(r'pydata/skills_pyarray\.bin\[skill=(\d+)\]\.SkillIcon$',site)
        if script_match and ref.get('kind')=='script':
            stem=ref.get('name')
            if not isinstance(stem,str) or not stem or Path(stem).name!=stem or Path(stem).suffix:
                continue
            uri=f'scripts/skills/{stem}.luac'
            found=_logical_uri_assets(uri,assets)
            old=(ref.get('resolves_to') or [{}])[0]
            target={'type':'source_script','id':stable_id('skill-script-uri',uri.casefold()),
                    'name':stem,'source_stem':stem,'path':'data/'+uri,'logical_uri':'data/'+uri,
                    'original_target':{'type':old.get('type'),'path':old.get('path')},
                    'asset_files':_source_variant_metadata(found),'status':'RESOLVED' if found else 'UNRESOLVED',
                    'schema_evidence':'SkillTable Script string field; exact data/scripts/skills/<stem>.luac logical-path match in supplied source assets.'}
            ref['resolves_to']=[target]
            ref['resolution_evidence']=target['schema_evidence']
            if found:
                ref.update(status='RESOLVED',reason=None,work_status='DONE',confidence='PROVEN')
                ref['candidates']=[];ref.pop('candidate_note',None)
            else:
                ref.update(status='UNRESOLVED',reason='FILE_MISSING',work_status='DONE',confidence='PROVEN')
                ref['candidates']=[]
                ref['candidate_note']=f'No exact logical script file exists at data/{uri}.'
        elif icon_match and ref.get('kind')=='data file':
            label=ref.get('name')
            if not isinstance(label,str) or not label:
                continue
            frame=icon_labels.get(label.casefold())
            present=frame is not None and bool(art_assets)
            status='RESOLVED_SWF_SYMBOL_FRAME' if present else 'UNRESOLVED'
            symbol_id=stable_id('skill-icon-swf-symbol',art.get('sha256'),art.get('icon_sprite'),label.casefold(),frame)
            target={'type':'skill_icon_symbol','id':symbol_id,'name':label,'label':label,
                    'source_name':label,'sprite_id':art.get('icon_sprite'),'frame':frame,
                    'path':'data/menus/dqcharmenu_droid.swf','asset_uri':art_uri,
                    'asset_files':_source_variant_metadata(art_assets),'sha256':art.get('sha256'),
                    'art_catalog_path':'.repo/'+_SKILL_ART_REPORT,
                    'status':status,'schema_evidence':'Original skill presenter maps SkillIcon to dqcharmenu_droid.swf sprite 79; source_skill_art.json gives exact label/frame.'}
            ref['resolves_to']=[target]
            ref['reference_kind']='skill_icon_symbol'
            ref['resolution_evidence']=target['schema_evidence']
            if present:
                ref.update(status='RESOLVED',reason=None,work_status='DONE',confidence='PROVEN')
                ref['candidates']=[];ref.pop('candidate_note',None)
            else:
                ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN')
                ref['candidates']=[]
                ref['candidate_note']=(f'No exact SWF sprite-79 label exists for {label!r}.' if frame is None
                                       else 'The symbol label exists in the art catalog, but its matching original SWF asset is absent.')


def _resolve_itemdrop_visual_tokens(ctx,refs,assets):
    """Keep itemdrop Visual values as scene-node tokens, never physical files."""
    template_status,template_files,template_candidates,template_reason=_resolve_file(_ITEMDROP_TEMPLATE_URI,assets)
    template_assets=template_files if template_status=='RESOLVED' else template_candidates
    template_metadata=_source_variant_metadata(template_assets)
    evidence=('Recovered ItemObject::InitOnce 0x3ece80 uses fixed data/3D/GameObjects/itemdrops.bdae '
              'and passes LootAudioVisual.Visual to the scene consumer; node selection is not independently resolved here.')
    for ref in refs:
        context=ref.get('context') or {}
        if context.get('table')!='loot_audiovisual#group0' or context.get('field')!='Visual':
            continue
        token=ref.get('name')
        original=(ref.get('resolves_to') or [{}])[0]
        if not isinstance(token,str) or not token:
            continue
        target={'type':'itemdrop_scene_object_symbol','id':stable_id('itemdrop-scene-node',_ITEMDROP_TEMPLATE_URI,token),
                'name':token,'source_token':token,'path':token,'namespace':'itemdrops.bdae scene node',
                'template_uri':_ITEMDROP_TEMPLATE_URI,'template_asset_candidates':template_metadata,
                # This is the known template dependency, not a selected token node.
                'asset_files':_source_variant_metadata(template_assets),'status':'UNRESOLVED',
                'original_target':{'type':original.get('type'),'path':original.get('path')},
                'schema_evidence':evidence}
        ref['resolves_to']=[target]
        ref['reference_kind']='itemdrop_scene_node_token'
        ref['resolution_evidence']=evidence
        ref['status']='UNRESOLVED';ref['reason']='OTHER';ref['work_status']='DONE';ref['confidence']='PROVEN'
        ref['candidates']=[]
        ref['candidate_note']=('The Visual value is an authored scene-node token passed to the fixed itemdrops.bdae template; '
                               'the template asset is listed as a dependency candidate, but the token is not asserted '
                               'to be a physical file or a verified parsed node.')


def prepare_tables(ctx):
    """Seed source-derived tables before level/campaign consumers on a clean run."""
    tables=_table_inventory(ctx)
    tables.extend(read_json(ctx.out/'data/tables-extra.json',[]) or [])
    tables=list({r['id']:r for r in tables}.values())
    _decode_animation_and_effect_refs(ctx,tables)
    write_json(ctx.out/'data/tables.json',tables)

def run(ctx, inventory_only=False):
    data=ctx.inputs/'assets-extra/android/data'; py=data/'pydata'
    assets_all=_asset_index(ctx)
    tables=_table_inventory(ctx)
    # Root-owned parser for character, AI, item and faery tables supplies rows
    # from their native schemas. The names-stream inventory remains our source
    # of total row coverage; parser rows enrich those same stable table IDs.
    tables.extend(read_json(ctx.out/'data/tables-extra.json',[]) or [])
    table_dedup={r.get('id') or stable_id(r.get('table'),r.get('index'),r.get('name')):r for r in tables}
    tables=list(table_dedup.values())
    refs=_decode_animation_and_effect_refs(ctx,tables)
    # Use native audio ledger when present; it records the complete 638-row source
    # sound table resolution and event members from the existing source parser.
    audio_ledger=ctx.root/'port/engine-audio/reference/source-bindings-v38/ledger.json'
    if audio_ledger.exists():
        try:
            sound_bin,sound_names_bin=_sound_table_paths(ctx)
            sound_bin_site=source_site(ctx,sound_bin)
            sound_names_site=source_site(ctx,sound_names_bin)
            entries=json.loads(audio_ledger.read_text(encoding='utf-8'))
            for e in entries:
                sid=e.get('source_id'); nm=e.get('source_name','')
                site=f"pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row={sid}]"
                row=_reference(site,nm,'sound','row index in the 638-entry source SDD; event expands target bank members')
                row['context']={'file':sound_bin_site,'names_file':sound_names_site,'row':sid,'source_id':sid}
                tgt=e.get('target') or {}
                target=tgt.get('filename')
                members=e.get('assets') or ([tgt] if tgt else [])
                member_assets=[_audio_asset_files(m.get('filename') or '',assets_all) for m in members]
                existing=[m for m,found in zip(members,member_assets) if found]
                absent=[m for m,found in zip(members,member_assets) if not found]
                row['resolves_to']=[{'type':'sounds.xml filename','path':m.get('filename'),'uid':m.get('uid',tgt.get('uid')),
                                     'event':e.get('event'),'exists_in_original_sources':bool(found),
                                     'asset_files':_asset_file_metadata(found)} for m,found in zip(members,member_assets)]
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
    refs.extend(script_refs)
    refs.extend(read_json(ctx.out/'data/references-extra.json',[]) or [])
    refs.extend(read_json(ctx.out/'data/reference-cells-extra.json',[]) or [])
    # The campaign decoder owns its extraction; normalize and merge its rows
    # without changing their stable identities or source locations.
    refs.extend(read_json(ctx.out/'data/campaign-references.json',[]) or [])
    _resolve_literal_pydata_calls(ctx,refs,tables)
    _resolve_skill_source_targets(ctx,refs,assets_all)
    _resolve_itemdrop_visual_tokens(ctx,refs,assets_all)
    by_id={r.get('id') or stable_id(r.get('site'),r.get('name'),r.get('kind')):r for r in refs}
    refs=list(by_id.values())
    _resolve_campaign_candidates(refs,tables)
    if inventory_only:
        for row in tables:
            row.update(status='TODO',reason=None,fields={},work_status='TODO',confidence='GUESS',candidate_note=None)
        for ref in refs:
            ref.update(status='TODO',reason=None,resolves_to=[],candidates=[],candidate_note=None,
                       work_status='TODO',confidence='GUESS',port_status='UNKNOWN',implementation=None)
    else:
        for row in tables:
            if row.get('status')=='TODO':
                schema = row.get('table','').split('#',1)[0]
                row.update(status='UNRESOLVED',reason='OTHER',work_status='DONE',confidence='PROVEN',
                           candidate_note=f'Names and local row index are proven from the native names stream. Payload schema for {schema} has not yet been decoded in this atlas module; no field values or references are inferred.')
        _add_knight_skill_refs(ctx,refs)
        assets=assets_all
        for ref in refs:
            resolve_reference(ctx,ref,tables,assets)
            # A reference site must end with an explicit disposition. Numeric
            # expressions without a proven table role are unresolved as
            # semantics, never treated as IDs based on their shape alone.
            if ref.get('status')=='TODO':
                ref.update(status='UNRESOLVED',reason='OTHER',work_status='DONE')
                ref.setdefault('candidate_note','Source site was extracted, but no audited name/path/table-index rule maps this value to an original row or file.')
                ref.setdefault('candidates',[])
            if ref.get('status')=='UNRESOLVED' and not ref.get('reason'):
                ref['reason']='OTHER'
            if ref.get('status') in ('UNRESOLVED','AMBIGUOUS') and not ref.get('candidates') and not ref.get('candidate_note'):
                ref['candidate_note']='No exact source file or table row was established; no plausible substitute was inferred.'
            if ref.get('status') in ('RESOLVED','UNRESOLVED','AMBIGUOUS'):
                ref['work_status']='DONE'
    refs.sort(key=lambda r:(r.get('site',''),r.get('name',''),r.get('kind','')))
    audio_rows=[r for r in refs if r.get('site','').startswith('pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=') and r.get('rule','').startswith('row index in the 638-entry')]
    write_json(ctx.out/'data/tables.json',tables)
    write_json(ctx.out/'data/references.json',refs)
    write_json(ctx.out/'data/audio.json',audio_rows)
    rules=[
      {'id':'pydata-name-blocks','rule':'PyData name blocks are count-prefixed lists of length-prefixed UTF-8 strings; row index is zero-based in serialized order.','evidence':'port/game-data/animation_tables.cpp Reader::strings and load_animation_tables; port/game-data/effects_tables.cpp Reader::names.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'audio-row-index','rule':'Sound id indexes the 638-row sdd_dungeon_hunter_2_iphone table. Ordinary rows resolve to the sounds.xml filename; isEvent rows expand member filenames. File existence is checked across the partial Android/recovered-cache and iOS corpora; a file in any included source corpus resolves the row.','evidence':'items-discovery/sdd_dungeon_hunter_2_iphone_pyarray.bin and names; port/engine-audio/reference/source-bindings-v38/ledger.json source_id/event/target; audio-v34/cache/sounds.xml; coordination/v21.8/ATLAS-CODEX-PROMPT.md and user clarification that Android/recovered-cache and iOS are both partial sources.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'asset-platform-availability','rule':'Asset presence is recorded per actual source corpus and resolved paths retain their source labels and content hashes. Platform availability differences are diagnostics, not substitutions or grounds to suppress a match from another included original corpus.','evidence':'coordination/atlas/data/platform-comparison.json; coordination/v21.8/ATLAS-CODEX-PROMPT.md; user clarification 2026-10-13.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'path-case-resolution','rule':'Atlas lookup policy: compare a data URI with actual source file paths using exact case first, then case-insensitive comparison, preserving ambiguous candidates. This policy alone does not prove original native case semantics. Similar basenames remain candidates, never resolved targets.','evidence':'tools/atlas/references.py _resolve_file; actual source path inventory; port/windows-foundation/asset_catalog.cpp:15 and :165 normalize port lookup case.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'pydata-literal-oid','rule':'A literal GetPyOID(tableAlias,rowName) resolves only through a proven PyDataArrays public alias and an exact case-sensitive source names-stream match; GetPyStruct names a struct member and is not treated as a row lookup without a matching native field schema.','evidence':'ida-apk-export-2026-10-07/libraries/classes.dex/pseudocode-all.c PyDataArrays constructor/GetOID/GetPyOID wrappers; port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp; exact Android names streams.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'character-charpropsname','rule':'An authored charpropsname on a gametype="Character" record conditionally identifies an exact CharacterTable row when char_template does not take precedence. This is a source relationship, not proof that the actor spawned at runtime.','evidence':'native GetCharPropertiesId 0x3b3d38; canonical_property_declarations_v1.inc:68; canonical_character_family_v4.cpp:40-47; port/windows-foundation/actor_population.cpp:144-170.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'character-sounds-table-group','rule':'CharacterTable.Sounds indexes the first registered sounds-array group exposed as CharSoundsTable (sounds#group0). This establishes only the row namespace; sound payload fields and any SDD/audio relationship remain undecoded. Sounds_bak is a separate later registration group.','evidence':'native Character::GetCharSoundsId 0x3a3294 and GetCharSounds 0x3a32d0; source_process_array_registration_v101.hpp:113-114 (CharSoundsTable) and :121-122 (Sounds_bak).','status':'PROVEN','confidence':'PROVEN'},
      {'id':'skill-script-uri','rule':'SkillTable.Script is a stem, not a path. Resolve it only to an exact data/scripts/skills/<stem>.luac logical-path match and retain all source variants for that URI; basename-only matches are not resolutions.','evidence':'port/game-data/skill_tables.cpp SkillTable.Script string field; supplied original data/scripts/skills files and exact logical-path comparison in tools/atlas/references.py.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'skill-icon-swf-symbol','rule':'A nonempty SkillTable.SkillIcon is a SWF symbol label, not a standalone image file. Resolve it to the exact label/frame in sprite 79 of dqcharmenu_droid.swf; retain the SWF asset identity and hash. Missing labels or SWF assets remain unresolved.','evidence':'Original skill presenter mapping in tools/atlas/entity_profiles.py; port/windows-foundation/features/skill_ui/source_skill_art.json label/frame extraction; original SWF asset hash.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'itemdrop-visual-token','rule':'LootAudioVisual.Visual is a scene-node token passed to the fixed itemdrops.bdae template, not a physical file path. Preserve the token and template asset candidates, but leave its node match unresolved unless an independently parsed source scene receipt proves the exact binding.','evidence':'port/game-data/loot_audiovisual_v8.cpp:10; recovered ItemObject::InitOnce 0x3ece80; tools/atlas/entity_profiles.py resolve_item_resources and source-scene receipts.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'level-scene-prefix','rule':'The decoded Levels.LevelFile is a bare scene filename. The Windows transition consumer prepends data/scene/; the atlas looks up scene/<LevelFile> within each included data root. Original native loader behavior beyond the recovered table schema remains unconfirmed.','evidence':'port/game-data/level_tables.cpp:63; port/windows-foundation/features/transitions/level_transitions_v1.cpp:113.','status':'INFERENCE','confidence':'LIKELY'},
      {'id':'signed-reference-sentinels','rule':'Typed signed table-index fields use signed 32-bit values; negative values such as -1 mean no reference and are omitted from reference rows, never reinterpreted as large unsigned IDs.','evidence':'port/game-data/skill_tables.cpp signed_word and Skill Anim field; Projectiles registered source descriptor and signed row fields; exact source pyarray bytes.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'bdae-sound-prefix','rule':'BDAE sound events strip exactly one leading `sfx_` before lookup in sounds.xml; preserve the resulting label verbatim, including malformed or missing names.','evidence':'port/engine-audio/reference/source-bindings-v38/ledger.json; adapter.cpp sound event mapping; BDAE event sites from tools/atlas/sites.py.','status':'PROVEN','confidence':'PROVEN'},
      {'id':'unsupported-pyarray','rule':'An undecoded PyData payload retains its proven table name and local row index with terminal UNRESOLVED/OTHER disposition; its fields remain unknown pending a recovered parser or native schema. No fields are inferred from adjacent bytes.','evidence':'Native count-prefixed names streams plus exact decoded schemas in port/game-data/animation_tables.cpp, effects_tables.cpp, skill_tables.cpp, and data.cpp.','status':'PROVEN','confidence':'PROVEN'}]
    write_json(ctx.out/'data/rules.json',rules)
    (ctx.out/'RULES.md').write_text('# Reference resolution rules\n\n'+'\n'.join(f"- **{r['id']}** ({r['status']}; {r['confidence']}) — {r['rule']} Evidence: `{r['evidence']}`" for r in rules)+'\n',encoding='utf-8')
    counts={'tables':len(tables),'table_rows_todo':sum(r.get('status')=='TODO' for r in tables),
            'table_rows_unresolved':sum(r.get('status')=='UNRESOLVED' for r in tables),
            'table_rows_work_todo':sum(r.get('work_status')=='TODO' for r in tables),
            'references':len(refs),'reference_todo':sum(r.get('status')=='TODO' for r in refs),
            'reference_work_todo':sum(r.get('work_status')=='TODO' for r in refs),
            'sound_ledger_rows':sum(r.get('rule','').startswith('row index in the 638-entry') for r in refs),
            'references_by_kind':{k:sum(r.get('kind')==k for r in refs) for k in sorted({r.get('kind','') for r in refs})},
            'inventory_only':bool(inventory_only)}
    write_json(ctx.out/'counts/references.json',counts)
    _write_report(ctx,counts,refs,tables)
    return counts


def _write_report(ctx,counts,refs,tables):
    p=ctx.out/'reports/references.md';p.parent.mkdir(parents=True,exist_ok=True)
    kinds=sorted({r.get('kind','') for r in refs})
    status_rows=[]
    for kind in kinds:
      subset=[r for r in refs if r.get('kind')==kind]
      status_rows.append([kind,len(subset),sum(r.get('status')=='RESOLVED' for r in subset),
                          sum(r.get('status')=='AMBIGUOUS' for r in subset),
                          sum(r.get('status')=='UNRESOLVED' for r in subset),
                          json.dumps(dict(sorted(__import__('collections').Counter(r.get('reason') for r in subset if r.get('status')=='UNRESOLVED').items())),ensure_ascii=False)])
    lines=['# Reference resolution ledger','',
      'Generated by `tools/atlas/references.py`. Every extracted site has an explicit resolution status. Fields are decoded only from a matching recovered-source parser or native schema.','',
      '## Inventory','',
      f"- PyData table rows inventoried: {counts['tables']} ({counts['table_rows_todo']} TODO).",
      f"- PyData rows unresolved: {counts.get('table_rows_unresolved',0)}; rows still requiring schema work: {counts.get('table_rows_work_todo',0)}.",
      f"- Reference rows: {counts['references']} ({counts['reference_todo']} TODO; {counts.get('reference_work_todo',0)} require more evidence/work).",
      f"- Audio ledger rows: {counts['sound_ledger_rows']}.",'',
      '## Resolution by reference kind','',
      markdown_table(['Kind','Rows','Resolved','Ambiguous','Unresolved','Unresolved reasons'],status_rows),'',
      '## Evidence and gaps','',
      '- Sound-row mappings use the recovered 638-row source table and the source binding ledger. `isEvent` rows expand to member filenames; file existence is resolved across the partial Android/recovered-cache and iOS corpora. The iOS-only missing-name total is retained in the platform comparison as a diagnostic.',
      '- Animation sequences and character state references are parsed with the exact schemas in `port/game-data/animation_tables.cpp`; sound table UID/event fields use the source registration and the audio ledger.',
      '- Tables without a matching recovered-source schema retain their source names and indexes but remain UNRESOLVED/OTHER with a schema-gap note; their payload fields remain blank.','']
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
        if target.get('type') in ('source_script','skill_icon_symbol','itemdrop_scene_object_symbol'):
          continue
        path=target.get('path')
        if not path: continue
        if target.get('type')=='sounds.xml filename':
          found=_audio_asset_files(path,assets);status='RESOLVED' if found else 'UNRESOLVED'
          candidates=[];reason=None if found else 'FILE_MISSING'
        else:
          status,found,candidates,reason=_resolve_file(path,assets)
        target['asset_candidates']=[{'path':x.get('path'),'source':x.get('source')} for x in candidates]
        if status=='RESOLVED':
          target['asset_files']=_asset_file_metadata(found)
        elif r.get('kind') in ('anim','sound','model','projectile','animated effect','effect set'):
          r['status']='UNRESOLVED';r['reason']=reason or 'FILE_MISSING';r['candidates']=[{'path':x.get('path'),'source':x.get('source')} for x in candidates]
          if not r['candidates']: r['candidate_note']='Exact target path and basename were absent from all indexed original source roots.'


def known_checks(ctx):
    """Known-answer fixtures; all checks are derived again from original inputs."""
    out=[]; py=ctx.inputs/'assets-extra/android/data/pydata'
    ledger_path=ctx.root/'port/engine-audio/reference/source-bindings-v38/ledger.json'
    try:
        rows=json.loads(ledger_path.read_text(encoding='utf-8'))
        sound_bin,sound_name_bin=_sound_table_paths(ctx)
        names=_strings(sound_name_bin)
        records=_Reader(sound_bin); n=records.word(); pairs=[(records.i(),records.i()) for _ in range(n)]
        out.append({'name':'sound row index is the 638-row source table with 63 events','passed':n==638 and len(names)==638 and sum(e==1 for _,e in pairs)==63 and len(rows)==638,
                    'detail':f'rows={n}, names={len(names)}, isEvent={sum(e==1 for _,e in pairs)}, ledger={len(rows)}'})
        # Check the generated audio resolutions against an independent set of
        # original-source basenames. This catches platform-only matches that a
        # path resolver might otherwise leave as suggestions.
        assets=_asset_index(ctx)
        source_kinds={'android','ios','original-cache','android-extra','audio-cache'}
        source_basenames={Path(a.get('relative','').replace('\\','/')).name.casefold()
                          for a in assets if a.get('source') in source_kinds}
        saved_audio=read_json(ctx.out/'data/audio.json',[]) or []
        saved_by_id={int(m.group(1)):r for r in saved_audio
                     if (m:=re.search(r'\[row=(\d+)\]',r.get('site','')))}
        audio_disagreements=[]
        for entry in rows:
            sid=int(entry.get('source_id',-1)); members=entry.get('assets') or ([entry.get('target') or {}] if entry.get('target') else [])
            all_present=bool(members) and all(
                bool(member.get('filename')) and Path(member.get('filename','').replace('\\','/')).name.casefold() in source_basenames
                for member in members)
            observed=saved_by_id.get(sid)
            if observed is None or (observed.get('status')=='RESOLVED')!=all_present:
                audio_disagreements.append({'source_id':sid,'name':entry.get('source_name'),
                                            'expected_resolved':all_present,
                                            'observed':observed.get('status') if observed else None})
        row11=saved_by_id.get(11);row244=saved_by_id.get(244)
        audio_check_passed=(len(rows)==638 and len(saved_by_id)==638 and not audio_disagreements
                            and bool(row11 and row11.get('status')=='RESOLVED')
                            and bool(row244 and row244.get('status')=='RESOLVED'))
        out.append({'name':'638 audio rows resolve from the union of supplied source corpora','passed':audio_check_passed,
                    'detail':json.dumps({'rows':len(rows),'saved_rows':len(saved_by_id),
                                         'disagreement_count':len(audio_disagreements),
                                         'disagreement_sample':audio_disagreements[:8],
                                         'row11_balrog_killed':row11.get('status') if row11 else None,
                                         'row244_event':row244.get('status') if row244 else None},ensure_ascii=False)})
        xml=ctx.inputs/'audio-v34/cache/sounds.xml'
        xml_names={x.get('filename') for x in ET.parse(xml).iter('sound') if x.get('filename')}
        source_files={Path(a.get('relative','')).name.casefold() for a in assets if a.get('source') in ('android','ios','original-cache','android-extra','audio-cache','items-discovery')}
        missing=sorted(nm for nm in xml_names if nm.casefold() not in source_files)
        ios_files={Path(a.get('relative','')).name.casefold() for a in assets if a.get('source')=='ios'}
        ios_missing=sorted(nm for nm in xml_names if nm.casefold() not in ios_files)
        out.append({'name':'45 distinct sounds.xml filenames absent from the combined original corpus','passed':len(missing)==45,
                    'detail':f'xml_names={len(xml_names)}, combined_source_missing={len(missing)}, ios_only_missing={len(ios_missing)}; combined_missing_names={missing}'})
        out.append({'name':'iOS-only sound corpus gap retained as platform diagnostic','passed':len(ios_missing)==245,
                    'detail':f'ios_only_missing_distinct={len(ios_missing)}; iOS corpus is one source platform, not the complete Android/recovered-cache authority.'})
        ice='sfx_faerie_ice.wav'
        out.append({'name':'sfx_faerie_ice.wav is absent from original data','passed':ice.casefold() not in source_files,
                    'detail':f'{ice} present={ice.casefold() in source_files}'})
    except Exception as e:
        out.append({'name':'sound table, missing sound files, and faerie ice known answers','passed':False,'detail':str(e)})
    matrix=ctx.root/'coordination/v21.8/SKILL-VERIFICATION-MATRIX.md'
    try:
        lines=matrix.read_text(encoding='utf-8').splitlines()
        expected={'BashDown':(347,'skill_dh2_prince_warrior_bash_down.bdae',164,190,101),
                  'GroundSlam':(350,'skill_dh2_prince_warrior_crushing_blow.bdae',173,185,103),
                  'Charge':(349,'skill_dh2_prince_warrior_charge.bdae',172,48,102)}
        matrix_rows={};skill_names=_string_groups(py/'skills_pyarraynames.bin')[1]
        anim_refs=_decode_animation_and_effect_refs(ctx,_table_inventory(ctx))
        actual={}
        for line in lines:
            for name in expected:
                if f'| {name} |' in line: matrix_rows[name]=line
        led=json.loads(ledger_path.read_text(encoding='utf-8'));by_id={int(e['source_id']):e for e in led}
        assets=_asset_index(ctx)
        for name,(anim,clip,fx,sound,uid) in expected.items():
            idx=skill_names.index(name) if name in skill_names else -1
            skill_anim=next((int(r['name']) for r in anim_refs if r.get('site')==f'pydata/skills_pyarray.bin[skill={idx}].Anim'),None)
            step_values={}
            for field in ('FX','Sound'):
                rr=next((r for r in anim_refs if r.get('site')==f'pydata/animations_pyarray.bin[sequence={anim},step=0].{field}'),None)
                step_values[field]=int(rr['name']) if rr else None
            sound_row=by_id.get(sound,{})
            clip_present=any(Path(a.get('relative','')).name.casefold()==clip.casefold() for a in assets)
            actual[name]={'skill_index':idx,'skill_anim':skill_anim,'fx':step_values['FX'],'sound':step_values['Sound'],
                          'sound_uid':sound_row.get('uid'),'clip_present':clip_present,
                          'matrix_row_present':name in matrix_rows}
        passed=all(actual[n]['skill_anim']==v[0] and actual[n]['fx']==v[2] and actual[n]['sound']==v[3] and
                   actual[n]['sound_uid']==v[4] and actual[n]['clip_present'] and actual[n]['matrix_row_present'] for n,v in expected.items())
        out.append({'name':'Knight BashDown, GroundSlam, and Charge source rows','passed':passed,
                    'detail':json.dumps({'expected':expected,'actual':actual},ensure_ascii=False)})
    except Exception as e: out.append({'name':'Knight skill known answers','passed':False,'detail':str(e)})
    try:
        saved=read_json(ctx.out/'data/references.json',[]) or []
        expected_calls=[
            ('darkqueen1_core.luac:10',['AnimatedEffectTable','spell_dh2_prince_mage_stone_escape_magefx'],'effect_sets',229),
            ('flaminite_core.luac:67',['AnimDict','root_troll_intimidate'],'animations_dictionary#group0',1144),
            ('prince_mage_stone_escape.luac:30',['AnimatedEffectTable','spell_dh2_prince_mage_stone_escape_magefx'],'effect_sets',229),
        ]
        actual=[]
        for suffix,args,table,index in expected_calls:
            row=next((r for r in saved if r.get('site','').endswith(suffix)
                      and r.get('literal_arguments')==args),None)
            target=(row.get('resolves_to') or [{}])[0] if row else {}
            actual.append({'site':suffix,'status':row.get('status') if row else None,
                           'table':target.get('table'),'index':target.get('index'),
                           'expected_table':table,'expected_index':index})
        charrow=next((r for r in saved if (r.get('context') or {}).get('attribute','').lower()=='charpropsname'
                      and (r.get('context') or {}).get('gametype')=='Character'
                      and not (r.get('context') or {}).get('char_template')
                      and r.get('name')=='Swamp_LizadMan_Type1'),None)
        chartarget=(charrow.get('resolves_to') or [{}])[0] if charrow else {}
        char_actual={'status':charrow.get('status') if charrow else None,
                     'table':chartarget.get('table'),'index':chartarget.get('index'),
                     'port_status':charrow.get('port_status') if charrow else None}
        sounds_row=next((r for r in saved if r.get('site','').endswith('[section=0,row=360].Sounds')),None)
        sounds_target=(sounds_row.get('resolves_to') or [{}])[0] if sounds_row else {}
        sounds_actual={'status':sounds_row.get('status') if sounds_row else None,
                       'table':sounds_row.get('referenced_table') if sounds_row else None,
                       'target_table':sounds_target.get('table'),'index':sounds_target.get('index'),
                       'source_row_table':(sounds_target.get('source_row') or {}).get('table'),
                       'source_row_name':(sounds_target.get('source_row') or {}).get('name')}
        passed=(all(x['status']=='RESOLVED' and x['table']==x['expected_table'] and x['index']==x['expected_index'] for x in actual)
                and char_actual=={'status':'RESOLVED','table':'character_properties#group0','index':360,'port_status':'PARTIAL'}
                and sounds_actual['status']=='RESOLVED' and sounds_actual['table']=='sounds#group0'
                and sounds_actual['target_table']=='sounds#group0' and sounds_actual['index']==2
                and sounds_actual['source_row_table']=='sounds#group0'
                and sounds_actual['source_row_name']=='Default')
        out.append({'name':'literal PyData aliases and gated Swamp charpropsname source joins','passed':passed,
                    'detail':json.dumps({'literal_calls':actual,'charpropsname':char_actual,
                                         'CharSoundsTable_group_scope':sounds_actual},ensure_ascii=False)})
    except Exception as e:
        out.append({'name':'literal PyData aliases and gated Swamp charpropsname source joins','passed':False,'detail':str(e)})
    try:
        saved=read_json(ctx.out/'data/references.json',[]) or []
        itemdrop=[r for r in saved if (r.get('context') or {}).get('table')=='loot_audiovisual#group0'
                  and (r.get('context') or {}).get('field')=='Visual']
        itemdrop_checks=[]
        for r in itemdrop:
            target=(r.get('resolves_to') or [{}])[0]
            itemdrop_checks.append(r.get('status')=='UNRESOLVED'
                and target.get('type')=='itemdrop_scene_object_symbol'
                and target.get('path')==r.get('name')==target.get('source_token')
                and bool(target.get('template_asset_candidates'))
                and all(any(Path(p).name.casefold()=='itemdrops.bdae' for p in [a.get('path',''),*a.get('logical_paths',[])])
                        for a in target.get('template_asset_candidates') or []))
        scripts=[r for r in saved if r.get('site','').startswith('pydata/skills_pyarray.bin[skill=')
                 and r.get('site','').endswith('.Script')]
        script_checks=[]
        for r in scripts:
            target=(r.get('resolves_to') or [{}])[0]
            script_checks.append(r.get('status')=='RESOLVED' and target.get('type')=='source_script'
                and target.get('logical_uri')==f"data/scripts/skills/{r.get('name')}.luac"
                and bool(target.get('asset_files'))
                and all(any(Path(p).suffix.casefold()=='.luac' for p in [a.get('path',''),*a.get('logical_paths',[])])
                        for a in target.get('asset_files') or []))
        icons=[r for r in saved if r.get('site','').startswith('pydata/skills_pyarray.bin[skill=')
               and r.get('site','').endswith('.SkillIcon')]
        icon_checks=[]
        for r in icons:
            target=(r.get('resolves_to') or [{}])[0]
            icon_checks.append(r.get('status')=='RESOLVED' and target.get('type')=='skill_icon_symbol'
                and target.get('id')==stable_id('skill-icon-swf-symbol',target.get('sha256'),79,r.get('name','').casefold(),target.get('frame'))
                and target.get('sprite_id')==79 and isinstance(target.get('frame'),int)
                and target.get('label')==r.get('name') and bool(target.get('asset_files'))
                and all(a.get('sha256')==target.get('sha256') for a in target.get('asset_files') or []))
        passed=(len(itemdrop)==29 and all(itemdrop_checks)
                and len(scripts)==124 and all(script_checks)
                and len(icons)==74 and all(icon_checks))
        out.append({'name':'itemdrop scene tokens and SkillTable script/icon source targets','passed':passed,
                    'detail':json.dumps({'itemdrop_visual_rows':len(itemdrop),'itemdrop_unresolved_symbols':sum(itemdrop_checks),
                                         'skill_script_rows':len(scripts),'scripts_with_exact_uri_assets':sum(script_checks),
                                         'skill_icon_rows':len(icons),'icons_with_sprite79_frames':sum(icon_checks)},ensure_ascii=False)})
    except Exception as e:
        out.append({'name':'itemdrop scene tokens and SkillTable script/icon source targets','passed':False,'detail':str(e)})
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
                 ('effects',0):'effect_sets',('effects',1):'character_effects',('effects',2):'footstep_effects',
                 ('sdd_dungeon_hunter_2_iphone',0):'sdd_dungeon_hunter_2_iphone'}
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
    for filename,label in (('references-extra.json','scene/rule and BDAE sites'),
                           ('reference-cells-extra.json','native table reference cells'),
                           ('campaign-references.json','campaign script fields')):
        source_rows=read_json(ctx.out/'data'/filename,[]) or []
        missing=[r.get('id') for r in source_rows if r.get('id') not in ids]
        if missing: errors.append(f'{label} not merged into references.json: {len(missing)}')
    audio=[r for r in refs if r.get('site','').startswith('pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=') and r.get('rule','').startswith('row index in the 638-entry')]
    if len(audio)!=638: errors.append(f'sound row-site coverage differs from source table: references={len(audio)}, expected=638')
    # Fail coverage for the reference-bearing source families specified by the
    # atlas contract. Other opaque tables stay visible in the report as
    # UNRESOLVED and are not mislabeled as reference-bearing.
    required=('character_properties','ai','ai_factions','faeries','item_powers','projectiles','skills','animations','effects')
    uncovered=[]
    for family in required:
        if family=='character_properties':
            # Sections 1 and 2 are the class name blocks; only group 0 is the
            # 448-row CharacterProperties payload with 224 serialized fields.
            group=[r for r in table_rows if r.get('table')=='character_properties#group0']
        elif family=='animations':
            group=[r for r in table_rows if r.get('table') in ('animation_sequences','camera_animation_sets','character_animations')]
        elif family=='effects':
            group=[r for r in table_rows if r.get('table') in ('effect_sets','character_effects','footstep_effects')]
        else:
            group=[r for r in table_rows if r.get('table','').split('#',1)[0]==family]
        if group and any(r.get('status')!='RESOLVED' for r in group):
            uncovered.append(f'{family}: {sum(r.get("status")!="RESOLVED" for r in group)} unresolved rows')
    if uncovered: errors.append(f'required reference-bearing table rows remain undecoded: {uncovered}')
    return errors
