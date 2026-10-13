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

def check(ctx):
    import atlas
    base = atlas.audit(ctx,write_result=False)
    tests = [{'name':'original source audit and round-1 known answers','passed':base['passed']}]
    errors = list(base['errors'])
    for letter,name in PACKAGES.items():
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
    result = {'passed':not errors,'tests':tests,'errors':errors,'known_answers':base['known_answers']}
    write_json(ctx.out/'r2/check.json',result)
    return result

def check_command(ctx):
    result = check(ctx)
    progress(ctx,'CHECK PASSED; EXPLICIT SEMANTIC GAPS REMAIN' if result['passed'] else 'CHECK FAILURES')
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
    import r2_act2,r2_natives,r2_staging,r2_questions
    r2_act2.enumerate_scope(source_ctx)
    r2_natives.run(source_ctx)
    r2_staging.run(source_ctx)
    r2_act2.run(source_ctx)
    r2_questions.run(source_ctx)
    for p in (source_ctx.out/'r2').rglob('*'):
        if not p.is_file():continue
        if p.stat().st_size>MAX_GENERATED_BYTES:raise ValueError('Generated file exceeds 40 MB: '+str(p))
        dest = ctx.out/'r2'/p.relative_to(source_ctx.out/'r2')
        dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dest)
    size_recommendation(ctx)
    return check_command(ctx)
