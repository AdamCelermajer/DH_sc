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
from common import context, read_json, write_json, stable_id, source_roots, source_site, markdown_table

MODULES = ('scripts', 'sites', 'references', 'views')
VALID_REASONS = {'FILE_MISSING', 'NAME_NO_FILE', 'PATH_CASE', 'ID_OUT_OF_RANGE', 'NOT_IN_CACHE', 'OTHER'}

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
    for label, folder in source_roots(ctx):
        for p in sorted(folder.rglob('*')):
            if not p.is_file(): continue
            path = source_site(ctx, p)
            stat = p.stat()
            prev = old.get(path, {})
            # Rehash at regen/check to detect same-size mutations as well.
            sha = hashlib.sha256(p.read_bytes()).hexdigest()
            result.append({'id': stable_id('asset',path), 'path':path,
                           'source':label, 'relative':p.relative_to(folder).as_posix(),
                           'size':stat.st_size, 'sha256':sha,
                           'status':'TODO' if inventory_only else 'ORPHAN',
                           'work_status':'TODO' if inventory_only else 'DONE',
                           'reason':'' if inventory_only else 'No extracted consumer in the supplied corpus; not proof of unused original content.',
                           'referrers':[]})
    write_json(ctx.out / 'data/assets.json', result)
    return {'asset_files':len(result), 'by_source':dict(Counter(r['source'] for r in result))}

def link_assets(ctx):
    assets=rows(ctx,'assets')
    by_path={r['path'].lower():r for r in assets}
    by_suffix=defaultdict(list)
    for a in assets:
        by_suffix[a['relative'].lower()].append(a)
        by_suffix[Path(a['relative']).name.lower()].append(a)
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
                value=r.get(key,[])
                for v in value if isinstance(value,list) else [value]:
                    if isinstance(v,dict):v=v.get('path',v.get('site',''))
                    add(v,str(rid))
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
    for name in MODULES:
        mod=importlib.import_module(name)
        counts=mod.run(ctx,inventory_only=inventory_only)
        if counts is not None:write_json(ctx.out/'counts'/f'{name}.json',counts)

def source_manifest(ctx):
    files=rows(ctx,'assets')
    manifest={'corpora':[{'label':s,'path':source_site(ctx,p),'files':sum(a['source']==s for a in files)} for s,p in source_roots(ctx)],
              'asset_digest':hashlib.sha256('\n'.join(a['path']+' '+a['sha256'] for a in files).encode()).hexdigest(),
              'missing_input_paths':[str(p) for p in [ctx.inputs/'original-cache'] if not p.exists()],
              'cache_path_note':'The supplied supplemental cache is .local-inputs/assets-extra/android/original-cache/data. Original recovered audio is also inventoried from audio-v34/cache.',
              'source_commit':read_json(ctx.out/'data/baseline.json',{}).get('source_commit','64afbdc5'),
              'runtime':'Private CPython 3.12 under tools/atlas/.runtime, bootstrapped from Codex app dependency bundle; never global Python.'}
    write_json(ctx.out/'data/source-manifest.json',manifest)

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
            if not r.get('status'):errors.append(f'{name}/{loc}: no status')
            if r.get('status')=='TODO' or r.get('work_status')=='TODO':errors.append(f'{name}/{loc}: TODO')
            if r.get('status')=='UNRESOLVED' or r.get('work_status')=='UNRESOLVED':
                reason=r.get('reason_code',r.get('reason',''))
                if name=='references':
                    if reason not in VALID_REASONS:errors.append(f'{name}/{loc}: invalid/missing reason code')
                    if 'candidates' not in r:errors.append(f'{name}/{loc}: missing candidates list (empty is permitted only with candidate_note)')
                    elif not r['candidates'] and not r.get('candidate_note'):errors.append(f'{name}/{loc}: empty candidates without explanation')
                elif not reason:errors.append(f'{name}/{loc}: unresolved without reason')
            if r.get('status')=='NOT-NEEDED' and not r.get('reason'):errors.append(f'{name}/{loc}: NOT-NEEDED without reason')
    script_ids={r['id'] for r in sets['scripts']}
    function_ids={r['id'] for r in sets['functions']}
    for c in sets['calls']:
        if c.get('script_id') not in script_ids:errors.append(f'call {c["id"]}: script row absent')
        if c.get('engine_candidate',True) and c.get('function_id') not in function_ids:errors.append(f'call {c["id"]}: function row absent')
    for r in sets['references']:
        # `invalid_id` is only set for an evidenced table-index interpretation.
        # A numeric constant without a proven table role is never an ID here.
        if r.get('invalid_id'):errors.append(f'reference {r["id"]}: referenced table id does not exist')
        if r.get('status') in ('RESOLVED','AMBIGUOUS') and not r.get('resolves_to'):errors.append(f'reference {r["id"]}: no targets')
    for a in sets['assets']:
        if not a.get('referrers') and a.get('status')!='ORPHAN':errors.append(f'asset {a["path"]}: no referrer and not ORPHAN')
        if a.get('status')=='ORPHAN' and not a.get('reason'):errors.append(f'asset {a["path"]}: ORPHAN without explanation')
    if include_sources:
        expected={source_site(ctx,p):p for _,folder in source_roots(ctx) for p in folder.rglob('*') if p.is_file()}
        actual={r['path']:r for r in sets['assets']}
        for path in expected.keys()-actual.keys():errors.append('missing asset row: '+path)
        for path in actual.keys()-expected.keys():errors.append('stale asset row: '+path)
        for path,p in expected.items():
            a=actual.get(path)
            if a and hashlib.sha256(p.read_bytes()).hexdigest()!=a['sha256']:errors.append('input drift: '+path)
        # Independent re-extraction covers new/deleted scripts/functions/sites;
        # do not validate a dataset only against its own saved row counts.
        for name in ('scripts','sites','references','views'):
            mod=importlib.import_module(name)
            fn=getattr(mod,'coverage_check',None)
            if fn:
                result=fn(ctx)
                if isinstance(result,list):errors.extend(result)
                elif result:errors.append(str(result))
            else:errors.append(name+': independent source coverage_check(ctx) missing')
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
        if f.get('port_status')=='PORTED':continue
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
         'Commands (PowerShell, from this worktree):','',
         '```powershell\n./tools/atlas/regen.ps1\n./tools/atlas/atlas.ps1 check\n./tools/atlas/atlas.ps1 find swampking\n./tools/atlas/atlas.ps1 find sound:517\n```','',
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
    # Typed queries disambiguate numeric row indexes across unrelated tables.
    for name in ('references','assets','functions','scripts','calls','tables','levels','actors'):
        for r in rows(ctx,name):
            searchable=json.dumps(r,ensure_ascii=False).lower()
            if q in searchable:found.append({'category':name,**r})
    print(json.dumps({'query':query,'matches':len(found),'rows':found},ensure_ascii=False,indent=2))
    return 0 if found else 1

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('command',choices=('inventory','regen','check','find'))
    ap.add_argument('query',nargs='?')
    ap.add_argument('--inputs')
    a=ap.parse_args();ctx=context(a.inputs)
    if a.command=='find':
        if a.query is None:ap.error('find needs a name, id, or typed id')
        return find(ctx,a.query)
    ctx.out.mkdir(parents=True,exist_ok=True)
    if a.command in ('inventory','regen'):
        inventory_only=a.command=='inventory'
        counts=asset_inventory(ctx,inventory_only)
        write_json(ctx.out/'counts/assets.json',counts)
        run_modules(ctx,inventory_only)
        if not inventory_only:link_assets(ctx)
        source_manifest(ctx)
        p=progress(ctx,'INVENTORY TODO' if inventory_only else 'MAPPED')
        print(f'Processed {p["not_TODO"]:,}/{p["total"]:,} rows ({p["percent"]:.2f}%)')
        if inventory_only:return 0
    check=audit(ctx)
    progress(ctx,'VERIFIED' if check['passed'] else 'CHECK FAILURES')
    report(ctx,check)
    for k,v in check['coverage'].items():print(f'{k}: {v["total"]:,} '+json.dumps(v['statuses'],sort_keys=True))
    for c in check['known_answers']:print(('PASS' if c.get('passed') else 'FAIL')+' '+c.get('name','?'))
    for e in check['errors'][:40]:print('ERROR '+e)
    print('atlas check: '+('PASS' if check['passed'] else 'FAIL')+f' ({len(check["errors"])} errors)')
    return 0 if check['passed'] else 1

if __name__=='__main__':
    raise SystemExit(main())
