"""Original-data atlas orchestrator, coverage auditor and lookup CLI.

All generated files are confined to coordination/atlas. Original inputs and game
source are read-only. No runtime playability is inferred from this static atlas.
"""
import argparse
from collections import Counter, defaultdict
import difflib
import hashlib
import importlib
import json
from pathlib import Path
import re
import sys
from common import context, read_json, write_json, stable_id, source_roots, source_files, source_site, markdown_table

MODULES = ('scripts', 'sites', 'table_sites', 'campaign', 'references', 'views')
VALID_REASONS = {'FILE_MISSING', 'NAME_NO_FILE', 'PATH_CASE', 'ID_OUT_OF_RANGE', 'NOT_IN_CACHE', 'OTHER'}

# These values are emitted by the category extractors. TODO is included for
# the explicit inventory checkpoint; it is still rejected below for final
# audit outputs. work_status is intentionally optional on legacy row schemas.
STATUS_ENUMS = {
    'scripts': {'TODO', 'DONE', 'UNRESOLVED'},
    'functions': {'TODO', 'DONE', 'UNRESOLVED'},
    'calls': {'TODO', 'DONE', 'UNRESOLVED'},
    'references': {'TODO', 'RESOLVED', 'AMBIGUOUS', 'UNRESOLVED', 'NOT-NEEDED'},
    'assets': {'TODO', 'REFERENCED', 'ORPHAN'},
    'levels': {'TODO', 'DONE', 'RESOLVED', 'INFERENCE', 'UNRESOLVED'},
    'actors': {'TODO', 'RESOLVED', 'UNRESOLVED'},
    'bosses': {'TODO', 'PROVEN', 'UNRESOLVED'},
    'tables': {'TODO', 'RESOLVED', 'UNRESOLVED'},
}
WORK_STATUS_ENUMS = {
    'scripts': {'TODO', 'DONE', 'UNRESOLVED'},
    'functions': {'TODO', 'DONE', 'UNRESOLVED'},
    'calls': {'TODO', 'DONE', 'UNRESOLVED'},
    'references': {'TODO', 'DONE', 'UNRESOLVED'},
    'assets': {'TODO', 'DONE'},
    'levels': {'TODO', 'DONE', 'UNRESOLVED'},
    'actors': {'TODO', 'DONE'},
    'bosses': {'TODO', 'DONE'},
    'tables': {'TODO', 'DONE'},
}

def rows(ctx, name):
    value = read_json(ctx.out / 'data' / (name + '.json'), [])
    if isinstance(value, dict):
        for k in ('rows', name, 'references', 'assets'):
            if isinstance(value.get(k), list): return value[k]
        return []
    return value

def asset_inventory(ctx, inventory_only=False):
    old = {r['path']: r for r in rows(ctx, 'assets')}
    result = []
    folders=dict(source_roots(ctx))
    for label, p in source_files(ctx):
            path = source_site(ctx, p)
            stat = p.stat()
            prev = old.get(path, {})
            # Rehash at regen/check to detect same-size mutations as well.
            sha = hashlib.sha256(p.read_bytes()).hexdigest()
            result.append({'id': stable_id('asset',path), 'path':path,
                           'source':label, 'relative':p.relative_to(folders[label]).as_posix() if label in folders else p.name,
                           'size':stat.st_size, 'sha256':sha,
                           'status':'TODO' if inventory_only else 'ORPHAN',
                           'work_status':'TODO' if inventory_only else 'DONE',
                           'reason':'' if inventory_only else 'No extracted consumer in the supplied corpus; not proof of unused original content.',
                           'referrers':[]})
    import package_index
    if (ctx.inputs/'claude-preview15/ipa/index_map.tsv').is_file():
        updates={r['path']:r for r in package_index.enrich(ctx,result)}
        for row in result:
            if row['path'] in updates:
                row.update(updates[row['path']])
    write_json(ctx.out / 'data/assets.json', result)
    return {'asset_files':len(result), 'by_source':dict(Counter(r['source'] for r in result))}

def link_assets(ctx):
    assets=rows(ctx,'assets')
    by_path={r['path'].lower():r for r in assets}
    by_suffix=defaultdict(list)
    for a in assets:
        by_suffix[a['relative'].lower()].append(a)
        by_suffix[Path(a['relative']).name.lower()].append(a)
        for alias in a.get('logical_paths',[]):
            by_suffix[re.sub(r'^data/', '', alias.lower())].append(a)
    def match(value):
        if not isinstance(value,str): return []
        value=value.replace('\\','/').lower().split(':line')[0]
        if value in by_path: return [by_path[value]]
        # A target may append the original line or a BRES marker offset.
        no_loc=re.sub(r':(?:\d+|0x[0-9a-f]+)(?:.*)?$','',value)
        if no_loc in by_path:return [by_path[no_loc]]
        if '/data/' in value:value=value.split('/data/',1)[1]
        elif value.startswith('data/'):value=value[5:]
        return by_suffix.get(value,[])
    def add(value,referrer):
        for a in match(value):
            if referrer not in a['referrers']:a['referrers'].append(referrer)
    def add_targets(value,referrer):
        if isinstance(value,str):add(value,referrer)
        elif isinstance(value,list):
            for v in value:add_targets(v,referrer)
        elif isinstance(value,dict):
            for key,v in value.items():
                if key in ('path','file','source_file','site','asset_files','members','resolves_to'):
                    add_targets(v,referrer)
    for name in ('references','scripts','levels','actors','tables'):
        for r in rows(ctx,name):
            rid=r.get('id',r.get('site',name))
            # Source files are also consumed by the decoder/inventory that owns
            # the row. That consumer is recorded distinctly from asset use.
            site=r.get('site',r.get('path',''))
            add(site,'source:'+str(rid))
            for value in (r.get('context',{}).get('file',''),r.get('context',{}).get('script','')):
                add(value,'source:'+str(rid))
            for key in ('resolves_to','source_file','file','path','source_path'):
                add_targets(r.get(key,[]),str(rid))
    for a in assets:
        a['referrers']=sorted(a['referrers'])
        a['status']='REFERENCED' if a['referrers'] else 'ORPHAN'
        a['work_status']='DONE'
        a['reason']='' if a['referrers'] else 'No extracted consumer in the supplied corpus; not proof of unused original content.'
    write_json(ctx.out/'data/assets.json',assets)

def progress(ctx,phase):
    categories={}
    for p in sorted((ctx.out/'counts').glob('*.json')):
        categories[p.stem]=read_json(p,{})
    totals={}
    for name in ('scripts','functions','calls','references','assets','levels','actors','bosses','tables'):
        rr=rows(ctx,name)
        states=Counter(r.get('work_status',r.get('status','')) for r in rr)
        todo=sum(v for k,v in states.items() if k=='TODO' or not k)
        totals[name]={'total':len(rr),'not_TODO':len(rr)-todo,'TODO':todo,'states':dict(states)}
    total=sum(r['total'] for r in totals.values())
    done=sum(r['not_TODO'] for r in totals.values())
    data={'phase':phase,'categories':categories,'rows':totals,'total':total,'not_TODO':done,
          'percent':round(100*done/total,4) if total else 0,
          'definition':'Processed = DONE, UNRESOLVED(reason), or NOT-NEEDED(reason); resolved != ported != runtime verified.'}
    write_json(ctx.out/'data/progress.json',data)
    doc=['# Original Data Atlas progress','',f'Phase: **{phase}**. Processed rows: **{done:,}/{total:,} ({data["percent"]:.2f}%)**.',
         '', '**Mob/boss semantics remain PARTIAL.** Joined profiles cover every character row, but unresolved Lua expressions, native contracts and runtime conditions remain. '
         'Resume with the named profile gaps in [mob profiles](general/mobs/index.md); root owns further source investigation.',
         '',data['definition'],'',markdown_table(['Category','Rows','Processed','TODO','States'],
         [[k,v['total'],v['not_TODO'],v['TODO'],v['states']] for k,v in totals.items()]),
         '\n## Inventory category counts\n']
    for k,v in categories.items():doc += [f'### {k}','', '```json',json.dumps(v,indent=2),'```','']
    doc += ['## Resume','', 'Run `powershell -NoProfile -ExecutionPolicy Bypass -File tools/atlas/regen.ps1` from this worktree.',
            'Use `atlas.ps1 check` to validate current files. Read the category reports for evidence and unresolved ownership.',
            'The inventory checkpoint preserves TODO before resolution. Later regeneration replaces the generated rows deterministically.','']
    (ctx.out/'PROGRESS.md').write_text('\n'.join(doc),encoding='utf-8')
    return data

def run_modules(ctx, inventory_only=False):
    # Level source rows feed campaign decoding. Render a preliminary source view
    # before reference resolution, then regenerate final views with the ledger.
    sequence=('scripts','sites','table_sites','views','campaign','references','views')
    for name in sequence:
        mod=importlib.import_module(name)
        if name=='table_sites':
            counts=mod.run(ctx,inventory_only=inventory_only)
            if counts is not None:write_json(ctx.out/'counts'/f'{name.replace("_","-")}.json',counts)
            importlib.import_module('references').prepare_tables(ctx)
            continue
        counts=mod.run(ctx,inventory_only=inventory_only)
        if counts is not None and name!='views':write_json(ctx.out/'counts'/f'{name.replace("_","-")}.json',counts)
        if name=='references' and not inventory_only:
            import mob_sounds
            mob_sounds.integrate(ctx)
            import enrichment
            enrichment.run(ctx)
            # The final views consume reference outcomes; sync the projections
            # before that pass, not only after all modules have finished.
            sync_projections(ctx)
    if not inventory_only:
        # views regenerates the campaign extractor projection; restore enriched
        # ledger rows after that pass, including the new typed operand sites.
        enriched=[r for r in rows(ctx,'references') if r.get('scene_id') and r.get('command_index') is not None]
        write_json(ctx.out/'data/campaign-references.json',enriched)
        sync_projections(ctx)

def source_manifest(ctx):
    files=rows(ctx,'assets')
    native=ctx.inputs/'ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c'
    manifest={'corpora':[{'label':s,'path':source_site(ctx,p),'files':sum(a['source']==s for a in files)} for s,p in source_roots(ctx)],
              'asset_digest':hashlib.sha256('\n'.join(a['path']+' '+a['sha256'] for a in files).encode()).hexdigest(),
              'cache_path_note':'Owner clarified both partial Android/recovered and iOS caches must be used; assets present in either can resolve. Supplemental cache: assets-extra/android/original-cache/data.',
              'audio_authority':['.local-inputs/assets-extra/ios/data/sounds','.local-inputs/audio-v34/cache'],
              'sound_table_authority':'.local-inputs/items-discovery/sdd_dungeon_hunter_2_iphone_pyarray.bin',
              'native_authority':'Android APK/ELF IDA export. No iPhone binary is assumed.',
              'native_evidence_files':[{'path':source_site(ctx,native),'size':native.stat().st_size,
                                       'sha256':hashlib.sha256(native.read_bytes()).hexdigest()}] if native.is_file() else [],
              'additional_binary_found':{'path':'.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/dungeonhunter2iPad','platform':'iPad','size':7940368,'magic':'cefaedfe','use':'discovery reported to owner; not used as native semantic evidence'},
              'source_commit':read_json(ctx.out/'data/baseline.json',{}).get('source_commit','64afbdc5'),
              'runtime':'Private CPython 3.12 under tools/atlas/.runtime, bootstrapped from Codex app dependency bundle; never global Python.'}
    write_json(ctx.out/'data/source-manifest.json',manifest)

def finalize_unresolved_rows(ctx):
    """Give terminal non-reference rows the shared unresolved evidence shape."""
    for name in ('scripts','functions','calls','assets','levels','actors','bosses','tables'):
        path=ctx.out/'data'/f'{name}.json'
        rr=read_json(path,[]) or []
        changed=False
        for row in rr:
            if row.get('status')!='UNRESOLVED' and row.get('work_status')!='UNRESOLVED':continue
            if not isinstance(row.get('candidates'),list):
                row['candidates']=[];changed=True
            if not row.get('candidate_note'):
                note=row.get('reason') or row.get('reason_code')
                if note:
                    row['candidate_note']=str(note);changed=True
        if changed:write_json(path,rr)

def platform_compare(ctx):
    import platform_views
    views=platform_views.compare_assets(rows(ctx,'assets'))
    write_json(ctx.out/'data/platform-logical-uris.json',views['logical_uri_view'])
    write_json(ctx.out/'data/platform-source-paths.json',views['path_view'])
    groups=defaultdict(list)
    spellings={}
    by_hash=defaultdict(list)
    for a in rows(ctx,'assets'):
        relative=a['relative'].replace('\\','/')
        if a['source']=='android-extra':relative='3d/'+relative
        if a['source']=='audio-cache' and Path(relative).suffix.lower() in ('.wav','.vxn'):
            relative='sounds/'+Path(relative).name
        spellings[a['id']]=relative
        by_hash[a['sha256']].append(a)
        groups[relative.casefold()].append(a)
    result=[]
    for path,aa in sorted(groups.items()):
        android=[a for a in aa if a['source'] in ('android','android-extra','original-cache','audio-cache','port-runtime-original-cache')]
        ios=[a for a in aa if a['source'].startswith('ios')]
        if not android and not ios:continue
        record={'id':stable_id('platform-path',path),'path':path,'format':Path(path).suffix or 'extensionless',
                'availability':'BOTH' if android and ios else 'ANDROID_OR_RECOVERED_ONLY' if android else 'IOS_ONLY',
                'different_content':len({a['sha256'] for a in aa})>1,
                'case_variants':sorted({spellings[a['id']] for a in aa}),
                'files':[{'path':a['path'],'source':a['source'],'sha256':a['sha256']} for a in aa],
                'status':'DONE','confidence':'PROVEN'}
        result.append(record)
    write_json(ctx.out/'data/platform-comparison.json',result)
    aliases=[]
    for sha,aa in sorted(by_hash.items()):
        if not any(a['source']=='ios' for a in aa) or not any(a['source'] in ('android','android-extra','original-cache','audio-cache') for a in aa):continue
        names={spellings[a['id']].casefold() for a in aa}
        if len(names)>1:
            aliases.append({'sha256':sha,'files':[{'path':a['path'],'source':a['source'],'canonical_path':spellings[a['id']]} for a in aa],
                'status':'DONE','confidence':'PROVEN','rule':'Equal SHA-256 bytes under different normalized names; no semantic equivalence inferred for differing bytes.'})
    write_json(ctx.out/'data/platform-name-aliases.json',aliases)
    write_json(ctx.out/'counts/platforms.json',{'paths':len(result),'availability':dict(Counter(r['availability'] for r in result)),
        'differing_content':sum(r['different_content'] for r in result),
        'case_variant_paths':sum(len(r['case_variants'])>1 for r in result),
        'identical_content_different_name_groups':len(aliases),
        'logical_uri_groups':len(views['logical_uri_view']),
        'logical_uri_availability':dict(Counter(r['availability'] for r in views['logical_uri_view'])),
        'logical_by_format':{ext:dict(Counter(r['availability'] for r in views['logical_uri_view']
            if (Path(r['key']).suffix or 'extensionless')==ext)) for ext in sorted({Path(r['key']).suffix or 'extensionless'
            for r in views['logical_uri_view']})},
        'by_format':{ext:dict(Counter(r['availability'] for r in result if r['format']==ext)) for ext in sorted({r['format'] for r in result})},
        'policy':'Use both supplied partial caches, as clarified by the owner; record the platform of every resolution.'})

def sync_projections(ctx):
    ledger={r['id']:r for r in rows(ctx,'references')}
    # Extractor projections share the ledger's reference-site IDs and outcomes.
    for name in ('script-references','references-extra','reference-cells-extra','campaign-references'):
        rr=rows(ctx,name)
        for r in rr:
            mapped=ledger.get(r['id'])
            if mapped:
                for field in ('status','work_status','reason','reason_code','candidate_note','candidates','resolves_to','port_status','implementation','confidence','symbolic_trace','derived_edges','semantic_resolution','resolution_evidence','file_resolution','reference_kind','referenced_table','referenced_index','rule'):
                    if field in mapped:r[field]=mapped[field]
        write_json(ctx.out/'data'/f'{name}.json',rr)
    counts=read_json(ctx.out/'counts/scripts.json',{})
    rr=rows(ctx,'script-references')
    if counts:
        counts['script_references'].update(todo=sum(r.get('status')=='TODO' or r.get('work_status')=='TODO' for r in rr),
            statuses=dict(Counter(r['status'] for r in rr)))
        write_json(ctx.out/'counts/scripts.json',counts)

def normalize_outputs(ctx):
    for p in ctx.out.rglob('*'):
        if p.is_file() and p.suffix in ('.json','.md'):
            content=p.read_bytes()
            content=content.replace(b'\r\n',b'\n')
            if p.suffix=='.md':
                content=('\n'.join(line.expandtabs(4).rstrip() for line in content.decode('utf-8').splitlines()).rstrip()+'\n').encode('utf-8')
            if content!=p.read_bytes():p.write_bytes(content)

def known_checks(ctx):
    # Category modules supply checks based on their source decoders, not merely
    # counts copied from the handoff. The checker refuses a missing test suite.
    checks=[]
    for name in ('scripts','references'):
        mod=importlib.import_module(name)
        fn=getattr(mod,'known_checks',None)
        if fn:
            result=fn(ctx)
            if isinstance(result,dict): result=[result]
            checks.extend(result)
        else:
            checks.append({'name':name+' known-answer suite','passed':False,'detail':'Module has no known_checks(ctx).'})
    return checks

def audit(ctx, include_sources=True):
    errors=[]
    coverage={}
    sets={name:rows(ctx,name) for name in ('scripts','functions','calls','references','assets','levels','actors','bosses','tables')}
    for name,rr in sets.items():
        ids=[r.get('id') for r in rr]
        if not rr:errors.append(f'{name}: no rows')
        if any(x is None for x in ids):errors.append(f'{name}: a row has no stable ID')
        if len(ids)!=len(set(ids)):errors.append(f'{name}: duplicate IDs')
        statuses=Counter(r.get('status','') for r in rr)
        coverage[name]={'total':len(rr),'statuses':dict(statuses)}
        for r in rr:
            loc=str(r.get('id','?'))
            status=r.get('status')
            if 'confidence' in r and r['confidence'] not in ('PROVEN','LIKELY','GUESS'):
                errors.append(f'{name}/{loc}: invalid confidence {r["confidence"]!r}')
            if not status:errors.append(f'{name}/{loc}: no status')
            elif not isinstance(status,str) or status not in STATUS_ENUMS[name]:
                errors.append(f'{name}/{loc}: invalid status {status!r}')
            if 'work_status' in r:
                work_status=r.get('work_status')
                if not isinstance(work_status,str) or work_status not in WORK_STATUS_ENUMS[name]:
                    errors.append(f'{name}/{loc}: invalid work_status {work_status!r}')
            if r.get('status')=='TODO' or r.get('work_status')=='TODO':errors.append(f'{name}/{loc}: TODO')
            if r.get('status')=='UNRESOLVED' or r.get('work_status')=='UNRESOLVED':
                reason=r.get('reason_code',r.get('reason',''))
                if name=='references':
                    if reason not in VALID_REASONS:errors.append(f'{name}/{loc}: invalid/missing reason code')
                    if 'candidates' not in r:errors.append(f'{name}/{loc}: missing candidates list (empty is permitted only with candidate_note)')
                    elif not r['candidates'] and not r.get('candidate_note'):errors.append(f'{name}/{loc}: empty candidates without explanation')
                elif not reason:errors.append(f'{name}/{loc}: unresolved without reason')
                if name!='references':
                    if not isinstance(r.get('candidates'),list):errors.append(f'{name}/{loc}: unresolved without candidates list')
                    if not r.get('candidate_note'):errors.append(f'{name}/{loc}: unresolved without candidate note')
            if r.get('status')=='NOT-NEEDED' and not r.get('reason'):errors.append(f'{name}/{loc}: NOT-NEEDED without reason')
    script_ids={r['id'] for r in sets['scripts']}
    function_ids={r['id'] for r in sets['functions']}
    for c in sets['calls']:
        if c.get('script_id') not in script_ids:errors.append(f'call {c["id"]}: script row absent')
        if c.get('engine_candidate',True) and c.get('function_id') not in function_ids:errors.append(f'call {c["id"]}: function row absent')
    table_indexes={(t.get('table'),t.get('index')) for t in sets['tables']}
    duplicate_table_keys=[key for key,count in Counter((t.get('table'),t.get('index')) for t in sets['tables']).items() if count>1]
    for table,index in duplicate_table_keys:
        errors.append(f'table row duplicate source key: {table}[{index}]')
    # Compare table rows with independently reconstructed source names and
    # schema-backed rows; saved rows alone cannot establish target existence.
    if include_sources:
        import references as reference_sources
        import table_sites as table_sources
        expected_tables=reference_sources._table_inventory(ctx)
        expected_tables.extend(table_sources.extract(ctx)[0])
        expected_ids={t.get('id') for t in expected_tables}
        actual_ids={t.get('id') for t in sets['tables']}
        for tid in sorted(expected_ids-actual_ids):errors.append(f'missing source table row: {tid}')
        for tid in sorted(actual_ids-expected_ids):errors.append(f'stale source table row: {tid}')
        expected_keys={(t.get('table'),t.get('index')) for t in expected_tables}
        for table,index in sorted(expected_keys-table_indexes,key=lambda x:(str(x[0]),str(x[1]))):
            errors.append(f'missing source table key: {table}[{index}]')
        for table,index in sorted(table_indexes-expected_keys,key=lambda x:(str(x[0]),str(x[1]))):
            errors.append(f'stale source table key: {table}[{index}]')
    target_table_types={
        'AnimTable row':'animation_sequences','AnimTable sequence':'animation_sequences',
        'EffectsTables row':'effect_sets','sdd sound row':'sdd_dungeon_hunter_2_iphone',
        'AnimatedEffectTable row':'effect_sets','EffectTables set':'effect_sets',
        'SkillTable row':'skills#group1','SkillListTable row':'skills#group0',
        'source sound row':'sdd_dungeon_hunter_2_iphone',
        'ProjectileDict row':'projectiles_dictionary#group0',
    }
    assets_by_relative=defaultdict(list)
    asset_paths={a.get('path') for a in sets['assets']}
    audio_basenames={Path(a.get('relative','')).name.casefold() for a in sets['assets']
                     if Path(a.get('relative','')).suffix.casefold() in ('.wav','.vxn','.ogg','.mp3')}
    for asset in sets['assets']:
        assets_by_relative[asset.get('relative','').replace('\\','/').casefold()].append(asset)
        for alias in asset.get('logical_paths',[]):
            assets_by_relative[re.sub(r'^data/', '', alias.replace('\\','/').casefold())].append(asset)
    def source_path_exists(value):
        if not isinstance(value,str) or not value:return False
        norm=value.replace('\\','/').lstrip('./')
        for prefix in ('q:/data/iphone/','q:/data/','data/iphone/','data/'):
            if norm.casefold().startswith(prefix):norm=norm[len(prefix):];break
        if norm.casefold().startswith('data/'):norm=norm[5:]
        return bool(assets_by_relative.get(norm.casefold()))
    for r in sets['references']:
        # `invalid_id` is only set for an evidenced table-index interpretation.
        # A numeric constant without a proven table role is never an ID here.
        if r.get('invalid_id'):errors.append(f'reference {r["id"]}: referenced table id does not exist')
        target_table=r.get('referenced_table');target_index=r.get('referenced_index')
        if target_table and isinstance(target_index,int):
            if (target_table,target_index) not in table_indexes:
                errors.append(f'reference {r["id"]}: {target_table}[{target_index}] does not exist')
        for target in r.get('resolves_to') or []:
            if not isinstance(target,dict):continue
            target_type=target.get('type')
            table=target.get('table');index=target.get('index')
            mapped_table=target_table_types.get(target_type)
            if index is None and isinstance(target.get('id_or_path'),int):index=target['id_or_path']
            table_expr=re.search(r'(?:AnimTable|EffectsTables|sdd_dungeon_hunter_2_iphone)\[(\d+)\]',str(table or ''))
            if target_type=='table-row':mapped_table=table
            elif table_expr:
                mapped_table=('animation_sequences' if 'AnimTable' in str(table) else
                              'effect_sets' if 'EffectsTables' in str(table) else
                              'sdd_dungeon_hunter_2_iphone')
                index=int(table_expr.group(1))
            if mapped_table and isinstance(index,int) and (mapped_table,index) not in table_indexes:
                errors.append(f'reference {r["id"]}: {mapped_table}[{index}] does not exist')
            file_target_types={'source asset file','file','EffectDict file row','ProjectileDict row','sounds.xml filename'}
            path=(target.get('path') or target.get('filename')) if target_type in file_target_types else None
            # Missing file targets are valid unresolved evidence. A row claiming
            # resolution, or an explicit positive source-cache assertion, must
            # point to at least one source asset.
            path_like=(isinstance(path,str) and ('.' in Path(path).name or '/' in path or '\\' in path))
            if path_like and (r.get('status')=='RESOLVED' or target.get('exists_in_source_cache') is True):
                exists=(Path(path).name.casefold() in audio_basenames
                        if target_type=='sounds.xml filename' else source_path_exists(path))
                if not exists:
                    errors.append(f'reference {r["id"]}: resolved target path does not exist: {path}')
            for linked_asset in target.get('asset_files') or []:
                if not isinstance(linked_asset,dict) or linked_asset.get('path') not in asset_paths:
                    errors.append(f'reference {r["id"]}: target links a nonexistent source asset')
        if r.get('status') in ('RESOLVED','AMBIGUOUS') and not r.get('resolves_to'):errors.append(f'reference {r["id"]}: no targets')
    for a in sets['assets']:
        if not a.get('referrers') and a.get('status')!='ORPHAN':errors.append(f'asset {a["path"]}: no referrer and not ORPHAN')
        if a.get('referrers') and a.get('status')=='ORPHAN':errors.append(f'asset {a["path"]}: ORPHAN has referrers')
        if a.get('status')=='ORPHAN' and not a.get('reason'):errors.append(f'asset {a["path"]}: ORPHAN without explanation')
        if not isinstance(a.get('referrers'),list):errors.append(f'asset {a.get("path")}: referrers is not a list')
    valid_referrers=set()
    for name in ('references','scripts','levels','actors','tables'):
        for row in sets[name]:
            rid=row.get('id',row.get('site',name))
            valid_referrers.add(str(rid))
            valid_referrers.add('source:'+str(rid))
    for a in sets['assets']:
        for referrer in a.get('referrers') or []:
            if not isinstance(referrer,str) or referrer not in valid_referrers:
                errors.append(f'asset {a.get("path")}: unknown referrer {referrer}')
    if include_sources:
        manifest=read_json(ctx.out/'data/source-manifest.json',{})
        for evidence in manifest.get('native_evidence_files',[]):
            path=ctx.inputs/evidence['path'].removeprefix('.local-inputs/')
            if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest()!=evidence['sha256']:
                errors.append('native evidence input drift: '+evidence['path'])
        expected={source_site(ctx,p):p for _,p in source_files(ctx)}
        folders=dict(source_roots(ctx))
        expected_asset_relatives={source_site(ctx,p):(p.relative_to(folders[label]).as_posix() if label in folders else p.name)
                                  for label,p in source_files(ctx)}
        actual={r['path']:r for r in sets['assets']}
        for path in expected.keys()-actual.keys():errors.append('missing asset row: '+path)
        for path in actual.keys()-expected.keys():errors.append('stale asset row: '+path)
        for path,p in expected.items():
            a=actual.get(path)
            if a and hashlib.sha256(p.read_bytes()).hexdigest()!=a['sha256']:errors.append('input drift: '+path)
            if a and a.get('relative')!=expected_asset_relatives[path]:errors.append('asset relative path drift: '+path)
        # Independent re-extraction covers new/deleted scripts/functions/sites;
        # do not validate a dataset only against its own saved row counts.
        for name in ('scripts','sites','table_sites','campaign','references','views'):
            mod=importlib.import_module(name)
            fn=getattr(mod,'coverage_check',None)
            if fn:
                result=fn(ctx)
                if isinstance(result,list):errors.extend(result)
                elif result:errors.append(str(result))
            else:errors.append(name+': independent source coverage_check(ctx) missing')
        import coverage_extra
        errors.extend(coverage_extra.coverage_check(ctx))
        import mobs, mob_sounds
        errors.extend(mob_sounds.coverage_check(ctx))
        errors.extend(mobs.coverage_check(ctx))
        import graph
        errors.extend(graph.coverage_check(ctx))
    checks=known_checks(ctx)
    for c in checks:
        if not c.get('passed'):errors.append('known answer failed: '+c.get('name','?')+' '+str(c.get('detail','')))
    report={'passed':not errors,'errors':errors,'coverage':coverage,'known_answers':checks}
    write_json(ctx.out/'data/check.json',report)
    return report

def report(ctx, check):
    refs=rows(ctx,'references'); funcs=rows(ctx,'functions'); calls=rows(ctx,'calls')
    func_by_id={r['id']:r for r in funcs}; script_by_id={r['id']:r for r in rows(ctx,'scripts')}
    impacts=defaultdict(lambda:{'calls':0,'scripts':set(),'bosses':set()})
    for c in calls:
        f=func_by_id.get(c.get('function_id'),{})
        if not c.get('engine_candidate',True) or c.get('port_status',f.get('port_status'))=='PORTED':continue
        imp=impacts[f.get('name',str(c.get('function_id')))];imp['calls']+=1
        s=script_by_id.get(c.get('script_id'),{})
        name=s.get('name',str(c.get('script_id')));imp['scripts'].add(name)
        if s.get('is_boss') or re.search('swampking|witch|madruk|darkqueen|edwardboss|flaminite|roottroll|sandworm|winddragon|balrog|assassin|emissary|resurrectinggod|miniboss',name,re.I):imp['bosses'].add(name)
    ranking=sorted(impacts.items(),key=lambda kv:(-len(kv[1]['bosses']),-len(kv[1]['scripts']),-kv[1]['calls'],kv[0]))
    p=read_json(ctx.out/'data/progress.json',{})
    doc=['# Original Data Atlas','',
         'Generated from the supplied Android, iOS and Android original-cache corpus and the frozen port source. '
         'This is a static data and source atlas. Port status records source coverage; it does not certify gameplay or audibility.','',
         f'Coverage check: **{"PASS" if check["passed"] else "FAIL"}**. Processed: **{p.get("not_TODO",0):,}/{p.get("total",0):,} ({p.get("percent",0):.2f}%)**.','',
         'Processed means every inventoried row has a terminal disposition. Explicitly unresolved references and native contracts remain follow-up work.','',
         'Commands (PowerShell, from this worktree):','',
         '```powershell\n./tools/atlas/regen.ps1\n. ./tools/atlas/use.ps1\natlas check\natlas serve\natlas show "Mud Lord"\natlas show skill:BashDown\natlas show item:Potion0\natlas mob Witch --json\natlas find sound:517\natlas cleancheck\n```','',
         '[Open the searchable atlas](index.html). Run `atlas serve` and open `http://127.0.0.1:18789/` for source inspection and audio playback. '
         'Search by original name, English display name, URI or typed ID; Dependencies and Used by navigate in both directions.','',
         '[Mob, boss and actor profiles](general/mobs/index.md) join every CharacterProperties row to its available scripts, animation/effect/projectile dependencies, sound producers and loot graph. '
         '**Semantic coverage remains partial:** dynamic Lua bindings, native contracts, encounter placement and runtime conditions are explicit gaps. All inventoried Android table payload rows are decoded; packed iOS table payloads retain their own evidence limits. The processed percentage is not a claim that every mob is fully understood.','',
         'The wrapper stages an ignored portable interpreter from the Codex dependency bundle on first use. '
         'Use `--inputs <directory>` to select another read-only input corpus. No global Python or original data is committed.','',
         '## Coverage','',markdown_table(['Category','Total','Statuses'],[[k,v['total'],v['statuses']] for k,v in check['coverage'].items()]),
         '## Reference resolution and port status','',markdown_table(['Kind','Total','Resolved','Ambiguous','Unresolved','Reasons','Port status'],[
          [kind,len(rr),sum(r['status']=='RESOLVED' for r in rr),sum(r['status']=='AMBIGUOUS' for r in rr),sum(r['status']=='UNRESOLVED' for r in rr),
           dict(Counter(r.get('reason_code',r.get('reason','')) for r in rr if r['status']=='UNRESOLVED')),
           dict(Counter(r.get('port_status','UNKNOWN') for r in rr))]
          for kind in sorted({r.get('kind','other') for r in refs}) for rr in [[r for r in refs if r.get('kind','other')==kind]]]),
         '## Asset consumers and orphans','',markdown_table(['Corpus','Files','Referenced','ORPHAN'],[
          [source,len(rr),sum(r['status']=='REFERENCED' for r in rr),sum(r['status']=='ORPHAN' for r in rr)]
          for source in sorted({a['source'] for a in rows(ctx,'assets')}) for rr in [[a for a in rows(ctx,'assets') if a['source']==source]]]),
         'ORPHAN means no extracted consumer in this supplied corpus. It does not prove an asset was unused by the original game.','',
         '## Engine functions to implement next','',
         'Ranked by distinct boss/custom scripts blocked, then all affected scripts, then call sites. PARTIAL remains a readiness gap.', '',
         markdown_table(['Function','Port status','Boss/custom scripts','All scripts','Call sites'],[
          [name,next((f.get('port_status','UNKNOWN') for f in funcs if f.get('name')==name),'UNKNOWN'),sorted(imp['bosses']),len(imp['scripts']),imp['calls']] for name,imp in ranking]),
         '## Known-answer checks','',markdown_table(['Check','Passed','Evidence/result'],[[c.get('name'),c.get('passed'),c.get('detail','')] for c in check['known_answers']]),
         '## Platform comparison','',markdown_table(['Logical resource format','Platform availability'],[[k,v] for k,v in read_json(ctx.out/'counts/platforms.json',{}).get('logical_by_format',{}).items()]),
         'Both supplied partial caches are used under the owner’s clarification. Each resolved target retains its source platform. '
         'The iOS-only audio subset has 245 distinct missing catalog filenames; combining it with the recovered Android audio leaves the required 45.', '',
         '## Checker failure-branch validation','',
         markdown_table(['Test','Passed'],[[t['name'],t['passed']] for t in read_json(ctx.out/'data/validation.json',{}).get('tests',[])]),
         '## Act gaps','', 'Act allocation and boundaries are **INFERENCE pending owner confirmation**. See [generated act views](general/README.md), '
         '[resolution rules](RULES.md), [progress](PROGRESS.md) and [evidence reports](reports/).','']
    # views supplies source-derived rankings; include them without hand-edited rows.
    gaps=read_json(ctx.out/'data/act-gaps.json',{})
    if gaps:
        doc += ['```json',json.dumps(gaps,ensure_ascii=False,indent=2),'```','']
    else:doc += ['Act-specific gap rankings are not generated yet.','']
    if check['errors']:doc += ['## Remaining check failures',''] + ['- '+e for e in check['errors'][:100]] + ['']
    (ctx.out/'README.md').write_text('\n'.join(doc),encoding='utf-8')

def find(ctx, query):
    q=query.lower(); found=[]
    typed=re.fullmatch(r'(sound|anim|effect|projectile|model|script|level|character|skill|table):(.+)',q)
    kind,term=typed.groups() if typed else (None,q)
    numeric=int(term) if re.fullmatch(r'-?\d+',term) else None
    def values(v):
        if isinstance(v,str):yield v
        elif isinstance(v,list):
            for x in v:yield from values(x)
        elif isinstance(v,dict):
            for k,x in v.items():
                if k not in ('id','sha256','bytes','size','byte_offset','character_offset','literal_offset'):
                    yield from values(x)
    def accepts(category,r):
        rk=str(r.get('reference_kind',r.get('kind',''))).lower()
        table=str(r.get('table',r.get('source_table',''))).lower()
        targets=[t for t in r.get('resolves_to',[]) if isinstance(t,dict)]
        target_tables=[str(r.get('referenced_table','')).lower()]+[str(t.get('source_row',{}).get('table',t.get('table',''))).lower() for t in targets]
        table_prefixes={'sound':('sdd_','sounds#'),'anim':('animation','character_animations'),
                        'effect':('effect','character_effects','footstep_effects'),
                        'projectile':('projectiles',),'model':('character_models_dictionary',),
                        'character':('character_properties','character_templates'),
                        'skill':('skills#',),'level':('levels#',)}
        target_kind=any(t.startswith(table_prefixes.get(kind,())) for t in target_tables) if kind else False
        if kind and category=='references' and not (rk.startswith(kind) or kind=='effect' and 'effect' in rk or target_kind):return False
        if kind and category=='tables' and kind not in table and not (kind=='sound' and 'sdd_' in table):return False
        if kind and category not in ('references','tables',kind+'s'):return False
        if numeric is not None:
            if str(r.get('name',''))==term or r.get('index')==numeric or r.get('source_id')==numeric:return True
            if category=='references':
                if r.get('referenced_index')==numeric:return True
                if any(t.get('index')==numeric or t.get('source_row',{}).get('index')==numeric for t in targets):return True
                if re.search(r'\[(?:row=)?'+re.escape(term)+r'\]',r.get('site','')):return True
                return any(re.search(r'\['+re.escape(term)+r'\]',v) for v in values(r.get('resolves_to',[])))
            return False
        return any(term in v.lower() for v in values(r))
    # Typed IDs disambiguate original row indexes from catalog UIDs; plain IDs
    # show all relevant source tables without accidental hash/offset matches.
    for name in ('references','assets','functions','scripts','calls','tables','levels','actors'):
        for r in rows(ctx,name):
            if accepts(name,r):found.append({'category':name,**r})
    print(json.dumps({'query':query,'matches':len(found),'rows':found},ensure_ascii=False,indent=2))
    return 0 if found else 1

def generate(ctx, inventory_only=False):
    """Generate source projections without depending on previous output."""
    ctx.out.mkdir(parents=True,exist_ok=True)
    (ctx.out/'.gitattributes').write_text('.gitattributes text eol=lf\n*.md text eol=lf\n*.json text eol=lf\n*.html text eol=lf\n',encoding='utf-8',newline='\n')
    write_json(ctx.out/'data/baseline.json',{'source_commit':'64afbdc5','branch':'v21.8-atlas','game_code_read_only':True})
    template=ctx.root/'tools/atlas/templates/atlas-method.md'
    (ctx.out/'reports').mkdir(parents=True,exist_ok=True)
    (ctx.out/'reports/atlas-method.md').write_text(template.read_text(encoding='utf-8'),encoding='utf-8',newline='\n')
    for template in sorted((ctx.root/'tools/atlas/templates').glob('*.md')):
        if template.name=='atlas-method.md':continue
        (ctx.out/template.name).write_text(template.read_text(encoding='utf-8'), encoding='utf-8', newline='\n')
    old_count=ctx.out/'counts/table_sites.json'
    if old_count.is_file():old_count.unlink()
    write_json(ctx.out/'counts/assets.json',asset_inventory(ctx,inventory_only))
    run_modules(ctx,inventory_only)
    if not inventory_only:
        finalize_unresolved_rows(ctx)
        link_assets(ctx);sync_projections(ctx)
        import mobs
        mobs.run(ctx)
        import graph
        graph.run(ctx)
    platform_compare(ctx)
    source_manifest(ctx)
    return progress(ctx,'INVENTORY TODO' if inventory_only else 'MAPPED')

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('command',choices=('inventory','regen','check','find','mob','show','serve','selftest','cleancheck'))
    ap.add_argument('query',nargs='?')
    ap.add_argument('--inputs')
    ap.add_argument('--port',type=int,default=18789)
    ap.add_argument('--json', action='store_true', help='Expand joined mob tables, references and calls as JSON')
    a=ap.parse_args();ctx=context(a.inputs)
    if a.command=='serve':
        import explorer_server
        return explorer_server.run(ctx,a.port)
    if a.command=='show':
        if a.query is None:ap.error('show needs a name, alias, typed ID or kind:index')
        import graph
        return graph.lookup(ctx,a.query,a.json)
    if a.command=='cleancheck':
        import cleancheck
        return cleancheck.run(ctx)
    if a.command=='selftest':
        import validation
        result=validation.run(ctx)
        for test in result['tests']:print(('PASS' if test['passed'] else 'FAIL')+' '+test['name'])
        return 0 if result['passed'] else 1
    if a.command=='find':
        if a.query is None:ap.error('find needs a name, id, or typed id')
        return find(ctx,a.query)
    if a.command=='mob':
        if a.query is None:ap.error('mob needs an actor name or CharacterProperties row index')
        import mobs
        return mobs.lookup(ctx, a.query, a.json)
    ctx.out.mkdir(parents=True,exist_ok=True)
    if a.command in ('inventory','regen'):
        inventory_only=a.command=='inventory'
        p=generate(ctx,inventory_only)
        print(f'Processed {p["not_TODO"]:,}/{p["total"]:,} rows ({p["percent"]:.2f}%)')
        if inventory_only:return 0
    check=audit(ctx)
    if a.command=='regen' and check['passed']:
        import semantic_checks
        semantic_verification=semantic_checks.run(ctx)
        write_json(ctx.out/'data/semantic-verification.json',semantic_verification)
        if not semantic_verification['passed']:
            check['passed']=False
            check['errors'].append('source semantic lookup checks failed; see data/semantic-verification.json')
            write_json(ctx.out/'data/check.json',check)
        import graph_checks
        graph_verification=graph_checks.run(ctx)
        write_json(ctx.out/'data/graph-verification.json',graph_verification)
        if not graph_verification['passed']:
            check['passed']=False
            check['errors'].append('universal graph lookup checks failed; see data/graph-verification.json')
            write_json(ctx.out/'data/check.json',check)
        import mob_checks
        mob_verification=mob_checks.run(ctx)
        if not mob_verification['passed']:
            check['passed']=False
            check['errors'].append('joined mob lookup focused checks failed; see data/mob-verification.json')
            write_json(ctx.out/'data/check.json',check)
        import validation
        verification=validation.run(ctx)
        if not verification['passed']:
            check['passed']=False
            check['errors'].append('checker rejection-branch selftests failed; see data/validation.json')
            write_json(ctx.out/'data/check.json',check)
    progress(ctx,'COVERAGE VERIFIED; MOB SEMANTICS PARTIAL' if check['passed'] else 'CHECK FAILURES')
    report(ctx,check)
    normalize_outputs(ctx)
    for k,v in check['coverage'].items():print(f'{k}: {v["total"]:,} '+json.dumps(v['statuses'],sort_keys=True))
    for c in check['known_answers']:print(('PASS' if c.get('passed') else 'FAIL')+' '+c.get('name','?'))
    for e in check['errors'][:40]:print('ERROR '+e)
    print('atlas check: '+('PASS' if check['passed'] else 'FAIL')+f' ({len(check["errors"])} errors)')
    return 0 if check['passed'] else 1

if __name__=='__main__':
    raise SystemExit(main())
