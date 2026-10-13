"""Round-2 orchestration. Original inputs and tracked round-1 outputs are read-only."""
from collections import Counter
import hashlib
import importlib
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
from types import SimpleNamespace
from common import read_json, write_json, markdown_table

MAX_GENERATED_BYTES = 40_000_000
PACKAGES = {'A': 'r2_natives', 'B': 'r2_staging', 'C': 'r2_act2'}
INITIAL_SCOPE_COMMIT = 'a0fd3078'

def initial_inventory(ctx):
    """Keep the initial work identity compact and immutable across source repairs."""
    dest=ctx.out/'r2/initial-inventory.json'
    if dest.is_file():return read_json(dest)
    def frozen(relative):
        payload=subprocess.run(['git','show',INITIAL_SCOPE_COMMIT+':coordination/atlas/'+relative],
                               cwd=ctx.root,capture_output=True,check=True).stdout
        return json.loads(payload)
    native=frozen('r2/natives/scope.json')
    act2=frozen('r2/package-c-progress.json')
    staging=frozen('r2/staging/scope-index.json')
    b=[]
    for rel in staging['files']:
        shard=frozen(rel)
        b.extend({'act':shard['act'],'reference_id':row['reference_id'],'state':'TODO'} for row in shard['rows'])
    value={'schema':'atlas-r2-initial-inventory-v1','initial_commit':INITIAL_SCOPE_COMMIT,
           'packages':{'A':[{'name':r['name'],'state':'TODO'} for r in native['native_rows']],
                       'B':b,'C':act2['rows']},
           'questions':[{'id':i,'state':'TODO'} for i in range(1,8)],
           'rule':'Never drop initial work rows after a source repair; retain a terminal reason for retirements.'}
    write_json(dest,value)
    return value

def package_progress(ctx, letter):
    candidates = [ctx.out/'r2'/f'package-{letter.lower()}-progress.json',
                  ctx.out/'r2'/{'A':'natives','B':'staging','C':'act2'}[letter]/f'package-{letter.lower()}-progress.json']
    return next((read_json(p) for p in candidates if p.is_file()), {})

def progress(ctx, phase='ENUMERATING'):
    root = ctx.out/'r2'
    root.mkdir(parents=True, exist_ok=True)
    packages = {k: package_progress(ctx,k) for k in PACKAGES}
    questions = read_json(root/'questions.json', {})
    rows = []
    total = processed = 0
    for letter, data in packages.items():
        n, p = data.get('total',0), data.get('processed',0)
        counts = data.get('status_counts', {'TODO':n})
        rows.append([letter, n, p, f'{100*p/n:.2f}%' if n else 'enumeration pending', counts])
        total += n; processed += p
    qrows = questions.get('questions',[])
    qp = sum(q.get('state','TODO') != 'TODO' for q in qrows)
    rows.append(['Open questions',7,qp,f'{100*qp/7:.2f}%',dict(Counter(q.get('state','TODO') for q in qrows)) or {'TODO':7}])
    total += 7; processed += qp
    result = {'schema':'atlas-r2-progress-v1','phase':phase,'packages':packages,
              'total':total,'processed':processed,'percent':100*processed/total if total else 0,
              'questions_total':7,'questions_processed':qp}
    write_json(root/'progress.json',result)
    text = ['# Original Data Atlas round 2 progress','',f'Phase: **{phase}**.',
            '',f'Processed: **{processed:,}/{total:,} ({result["percent"]:.2f}%)**.',
            '', 'Processed means DONE, UNRESOLVED(reason), or NOT-NEEDED(reason). It does not mean that unknown contracts are resolved or that content runs in the port.',
            '',markdown_table(['Package','Rows','Processed','Percent','States'],rows),
            'Package B counts act/reference pairs; its package ledger also records unique source-site coverage. Shared player resources can belong to both act scopes.',
            '', 'Act boundaries are INFERENCE. Follow the source transition frontier and package scope files rather than assuming the entire reachable closure is narrative Act 2.',
            '', 'Resume with TODO rows first, then the explicit unresolved reasons. Root owns integration, validation, commits and follow-up investigation. Package output and source evidence remain separate from gameplay implementation and runtime/visual verification.',
            '', 'Regenerate from original sources: `./tools/atlas/regen.ps1 --round2`. Check: `./tools/atlas/atlas.ps1 check --round2`. Source regeneration builds the round-1 dependency atlas in an ignored scratch directory and writes only round-2 results to tracked output.',
            '']
    (root/'PROGRESS-R2.md').write_text('\n'.join(text),encoding='utf-8',newline='\n')
    return result

def size_recommendation(ctx):
    tracked = subprocess.run(['git','ls-files','coordination/atlas'],cwd=ctx.root,
                             capture_output=True,text=True,check=True).stdout.splitlines()
    large = []
    for rel in tracked:
        p = ctx.root/rel
        if p.is_file() and p.stat().st_size > MAX_GENERATED_BYTES:
            large.append({'path':rel,'bytes':p.stat().st_size,
                          'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
    large.sort(key=lambda r:(-r['bytes'],r['path']))
    write_json(ctx.out/'r2/tracked-size-baseline.json',{'limit_bytes':MAX_GENERATED_BYTES,'oversized_round1_files':large})
    text = ['# Generated-file tracking recommendation','',
            'Preserve the existing round-1 files for this round. A later repository cleanup should stop tracking the regenerable files below, keep source generators and compact checks/manifests, and use ignored runtime output or release artifacts for full snapshots.',
            '',markdown_table(['Existing generated file','Bytes','Action'],[[r['path'],r['bytes'],'Untrack in a separate reviewed cleanup; regenerate locally'] for r in large]),
            'Shard JSON by source category/act and stable IDs, or compress each section, with a manifest listing hash, row count and schema. Keep every tracked generated file below 40,000,000 bytes. Avoid embedding the complete graph into tracked HTML; the local viewer already queries a persistent ignored SQLite cache.',
            '', 'Removing files from the current tree does not remove oversized blobs from Git history. Before a later GitHub push, the repository owner must choose a separate history-cleanup or artifact-storage migration and preserve a recoverable local backup. This round does not rewrite history or push.', '']
    (ctx.out/'r2/TRACKED-FILE-SIZES.md').write_text('\n'.join(text),encoding='utf-8',newline='\n')
    return large

def seal_step0(ctx):
    packages = {k: package_progress(ctx,k) for k in PACKAGES}
    if any(not d or not d.get('total') for d in packages.values()):
        raise ValueError('All three package enumerations are required before Step 0 is sealed')
    if any(d.get('processed',0) or any(k!='TODO' and v for k,v in d.get('status_counts',{}).items()) for d in packages.values()):
        raise ValueError('Step 0 requires every enumerated row to start TODO')
    scope_manifest = []
    for p in sorted((ctx.out/'r2').rglob('*.json')):
        rel = p.relative_to(ctx.out/'r2').as_posix()
        if p.name in ('scope.json','scope-index.json') or '/scope/' in '/'+rel:
            scope_manifest.append({'path':rel,'bytes':p.stat().st_size,
                                   'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
    write_json(ctx.out/'r2/STEP0.json',{'schema':'atlas-r2-step0-v1','packages':packages,'initial_scope_manifest':scope_manifest,
               'open_questions':[{'id':i,'state':'TODO'} for i in range(1,8)],
               'scope_policy':'Frozen initial enumeration; expansions require explicit source evidence and new TODO rows.'})
    size_recommendation(ctx)
    return progress(ctx,'STEP 0 ENUMERATED; ALL ROWS TODO')

def retention_check(ctx):
    """Original work rows survive repaired-source expansions and retirements."""
    initial=initial_inventory(ctx)['packages']
    current={k:package_progress(ctx,k) for k in PACKAGES}
    errors=[]
    a={r['name']:r for r in current['A'].get('page_manifest',[])}
    for row in initial['A']:
        if row['name'] not in a:errors.append('initial native missing: '+row['name'])
    b={}
    for rel in current['B'].get('classification_files',[]):
        shard=read_json(ctx.out/rel,{})
        for row in shard.get('rows',[]):
            key=(shard.get('act'),row.get('reference_id'))
            if key in b:errors.append('duplicate staging identity: '+str(key))
            b[key]=row
    for row in initial['B']:
        if (row['act'],row['reference_id']) not in b:
            errors.append('initial reference missing: '+row['act']+'/'+row['reference_id'])
    c={(r.get('row_type'),r.get('id')):r for r in current['C'].get('rows',[])}
    for row in initial['C']:
        if (row.get('row_type'),row.get('id')) not in c:
            errors.append('initial dossier work row missing: '+str(row.get('id')))
    for row in c.values():
        if row.get('state') in ('NOT-NEEDED','UNRESOLVED') and not row.get('reason'):
            errors.append('dossier terminal disposition lacks reason: '+str(row.get('id')))
    baseline=read_json(ctx.out/'r2/tracked-size-baseline.json',{})
    for row in baseline.get('oversized_round1_files',[]):
        path=ctx.root/row['path']
        if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest()!=row['sha256']:
            errors.append('old oversized generated file changed: '+row['path'])
    return {'name':'immutable initial identities and unchanged round-1 oversized files','passed':not errors,'errors':errors}

def report(ctx):
    root=ctx.out/'r2'
    packages={k:package_progress(ctx,k) for k in PACKAGES}
    rows=[[k,d.get('total',0),d.get('processed',0),d.get('status_counts',{})] for k,d in packages.items()]
    b=packages['B'];metrics=read_json(root/'staging/before-after-metrics.json',{})
    selections=[]
    for rel in b.get('selection_files',[]):
        data=read_json(ctx.out/rel,{})
        groups=data.get('groups',{})
        count=sum(len(g) if isinstance(g,list) else len(g.get('files',[]))
                  for g in (groups.values() if isinstance(groups,dict) else groups))
        selections.append([f'[{Path(rel).name}]({Path(rel).relative_to("r2").as_posix()})',count])
    gap_counts=Counter()
    for rel in b.get('classification_files',[]):
        for row in read_json(ctx.out/rel,{}).get('rows',[]):
            if row.get('classification') in ('UNRESOLVED','AMBIGUOUS','ABSENT_ALL_CORPORA','PORT_RULE_ABSENT'):
                gap_counts[row.get('reason_code') or row.get('reason') or 'unspecified']+=1
    questions=read_json(root/'questions.json',{}).get('questions',[])
    coverage=read_json(root/'act2/coverage.json',{}).get('counts',{})
    blockers=read_json(root/'act2/blocker-reach.json',{}).get('blockers',[])[:12]
    text=['# Original Data Atlas round 2','',
          'Source contracts, a staging ledger, and per-scene reconstruction dossiers. Original Android and iOS inputs are complementary partial corpora. There is no iPhone binary or verified iOS native ID mapping. Native contracts use the Android library; platform/version uncertainty remains explicit.',
          '',markdown_table(['Package','Work rows','Processed','States'],rows),
          'Processed includes explicit UNRESOLVED and NOT-NEEDED dispositions. TODO means investigation remains. Contract completion does not establish Windows gameplay parity.',
          '', '[Native contract index](natives/INDEX.md) · [Staging classifications and selections](staging/CLASSIFICATION-AND-SELECTIONS.md) · [Level dossiers](act2/dossier.md) · [Progress](PROGRESS-R2.md)',
          '',f'Package B covers {b.get("act_reference_pairs",0):,} act/reference pairs, {b.get("unique_reference_sites",0):,} distinct source IDs, and {b.get("shared_reference_sites_across_acts",0):,} IDs shared across act labels. Typed non-file keys retain a staging-only disposition; their table/resource dependencies must still be followed.',
          '',markdown_table(['Act label','Before source unresolved','After source unresolved','Ready reference pairs','Already staged pairs'],
             [[act,metrics.get('before_source_unresolved_by_act',{}).get(act,0),metrics.get('after_source_unresolved_by_act',{}).get(act,0),metrics.get('staging_ready_by_act',{}).get(act,0),metrics.get('already_staged_by_act',{}).get(act,0)] for act in ('act_1','act_2_inferred')]),
          'Before/after counts concern source classification, not successfully loaded game content. Selected physical paths are deduplicated across groups and base destinations.',
          '',markdown_table(['Selection JSON','Physical files'],selections),
          'Selections were checked by the original staging script, stopping before its first target-directory creation. Packed iOS .dat candidates need materialization/version review and are not offered as ready-to-copy files. [Preflight receipt](staging/preflight.json).',
          '',markdown_table(['Unresolved reason','Reference pairs'],gap_counts.most_common()),
          'Detailed per-kind metrics and referrer chains: [metrics](staging/before-after-metrics.json), [top 50 references](staging/top50-blocking-references.json). The global audio catalog checks preserve 45 absent sounds.xml filenames and absent sfx_faerie_ice.wav; these differ from scoped pair counts.',
          '', '## Seven source questions','']
    for q in questions:
        text += [f'{q["id"]}. **{q["title"]}** — {q["state"]}. '+ ' '.join(c.get('text','') for c in q.get('claims',[])[:1]),'']
    text += ['[Full answers, source addresses, evidence and behavioral cases](natives/QUESTIONS.md).',
             '', '## Readiness and remaining gaps','',
             f'The source-transition closure contains {coverage.get("scenes",0)} scenes and {coverage.get("source_edges",0)} edges. It includes later content and Act 1 backlinks; narrative act boundaries are INFERENCE. {coverage.get("unique_entity_rows",0)} distinct character rows have source-linked dossiers. Repairs to malformed source rule text are logged; original bytes were not changed.',
             '',markdown_table(['Highest-reaching blocker','Affected scenes'],[[r.get('blocker'),r.get('blocked_scene_count')] for r in blockers]),
             'The conservative playable-support metric is '+str(coverage.get('verified_playable_fraction',{}))+'. Its denominator counts required per-level capabilities. Zero verified support here means this static investigation supplied no integrated runtime/visual proof; it does not assert every existing port feature fails.',
             '', '[Coverage and denominator](act2/coverage.md) · [Implementation ranking](act2/blocker-reach.md) · [Scope expansions and retained frontier records](act2/scope-expansion.json)',
             '', '## Rebuild and verification','',
             'Run `./tools/atlas/regen.ps1 --round2` from the atlas2 worktree. It rebuilds source indexes in ignored scratch storage, then regenerates round-2 artifacts. Run `./tools/atlas/atlas.ps1 check --round2` for scope, contract, classification, staging, evidence and original known-answer checks. A remaining TODO makes the final check fail.',
             '', '[Check receipt](check.json) · [Immutable initial scope](STEP0.json) · [Tracked-file-size recommendation](TRACKED-FILE-SIZES.md)',
             '', 'All new generated files must stay below 40,000,000 bytes. Existing oversized round-1 outputs are preserved. A separate cleanup should untrack or shard regenerable snapshots and address historical Git blobs before a future push. This round does not push or rewrite history.','']
    (root/'README.md').write_text('\n'.join(text),encoding='utf-8',newline='\n')

def check(ctx):
    import atlas
    base = atlas.audit(ctx,write_result=False)
    tests = [{'name':'original source audit and round-1 known answers','passed':base['passed']}]
    errors = list(base['errors'])
    for letter,name in PACKAGES.items():
        current = package_progress(ctx,letter)
        if (not current or current.get('processed',0)!=current.get('total',-1)
                or current.get('status_counts',{}).get('TODO',0)):
            errors.append(f'package {letter} still has unprocessed TODO scope rows')
        result = importlib.import_module(name).check(ctx)
        tests.append({'name':f'package {letter} coverage and evidence','passed':result.get('passed',False)})
        errors.extend(f'{letter}: {e}' for e in result.get('errors',[]))
        if not result.get('passed',False) and not result.get('errors'):
            errors.append(f'package {letter} check failed')
    import r2_questions
    result = r2_questions.check(ctx)
    tests.extend(result.get('tests',[])); errors.extend(result.get('errors',[]))
    oversize = [p.relative_to(ctx.root).as_posix() for p in (ctx.out/'r2').rglob('*')
                if p.is_file() and p.stat().st_size>MAX_GENERATED_BYTES]
    tests.append({'name':'all new generated files below 40 MB','passed':not oversize})
    errors.extend('oversized new generated file: '+p for p in oversize)
    retained=retention_check(ctx)
    tests.append({'name':retained['name'],'passed':retained['passed']});errors.extend(retained['errors'])
    result = {'passed':not errors,'tests':tests,'errors':errors,'known_answers':base['known_answers']}
    write_json(ctx.out/'r2/check.json',result)
    return result

def check_command(ctx):
    result = check(ctx)
    progress(ctx,'CHECK PASSED; EXPLICIT SEMANTIC GAPS REMAIN' if result['passed'] else 'CHECK FAILURES')
    report(ctx)
    for t in result['tests']:print(('PASS' if t['passed'] else 'FAIL')+' '+t['name'],flush=True)
    for e in result['errors'][:40]:print('ERROR '+e,flush=True)
    return 0 if result['passed'] else 1

def regenerate(ctx):
    import atlas
    scratch_root = ctx.root/'tools/atlas/.runtime'
    scratch_root.mkdir(parents=True,exist_ok=True)
    scratch = Path(tempfile.mkdtemp(prefix='r2-source-regen-',dir=scratch_root))
    source_ctx = SimpleNamespace(root=ctx.root,inputs=ctx.inputs,out=scratch/'atlas')
    print('Rebuilding source dependency atlas in ignored scratch: '+str(source_ctx.out),flush=True)
    atlas.generate(source_ctx)
    audited = atlas.audit(source_ctx)
    if not audited['passed']:raise RuntimeError('Scratch source atlas audit failed: '+str(audited['errors'][:10]))
    # Original ranking is a requested frozen input, not a mutable priority list.
    shutil.copy2(ctx.out/'README.md',source_ctx.out/'README.md')
    initial_inventory(ctx)
    # Frozen work identity is a generated historical input; current content
    # still rebuilds from source. Keep the initial Act 2 scope for retirement
    # dispositions when a malformed rule is recovered by the new parser.
    for rel in ('STEP0.json','initial-inventory.json','act2/scope.json','act2/scope.md'):
        source=ctx.out/'r2'/rel
        if source.is_file():
            target=source_ctx.out/'r2'/rel
            target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source,target)
    import r2_act2,r2_natives,r2_staging,r2_questions
    r2_act2.enumerate_scope(source_ctx)
    r2_questions.run(source_ctx)
    r2_natives.run(source_ctx)
    r2_staging.run(source_ctx)
    r2_act2.run(source_ctx)
    for p in (source_ctx.out/'r2').rglob('*'):
        if not p.is_file():continue
        if p.stat().st_size>MAX_GENERATED_BYTES:raise ValueError('Generated file exceeds 40 MB: '+str(p))
        dest = ctx.out/'r2'/p.relative_to(source_ctx.out/'r2')
        dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dest)
    size_recommendation(ctx)
    return check_command(ctx)
