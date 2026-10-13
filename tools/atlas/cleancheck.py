"""Regenerate into an empty private directory and compare every artifact."""
import hashlib
from pathlib import Path
import tempfile
from types import SimpleNamespace
from common import write_json

def run(ctx):
    from atlas import generate,audit,progress,report,normalize_outputs
    import validation
    runtime=ctx.root/'tools/atlas/.runtime'
    runtime.mkdir(parents=True,exist_ok=True)
    # mkdtemp retains evidence without deleting any source or user directory.
    fresh=Path(tempfile.mkdtemp(prefix='clean-atlas-',dir=runtime))
    testctx=SimpleNamespace(root=ctx.root,inputs=ctx.inputs,out=fresh)
    print('Regenerating every artifact from empty private output.',flush=True)
    generate(testctx)
    check=audit(testctx)
    if not check['passed']:
        print('FAIL clean source audit: '+str(check['errors'][:5]));return 1
    verification=validation.run(testctx)
    import mob_checks
    mob_verification=mob_checks.run(testctx)
    progress(testctx,'COVERAGE VERIFIED; MOB SEMANTICS PARTIAL' if verification['passed'] else 'CHECK FAILURES')
    report(testctx,check)
    normalize_outputs(testctx)
    def digest(folder):
        return {p.relative_to(folder).as_posix():hashlib.sha256(p.read_bytes()).hexdigest()
                for p in folder.rglob('*') if p.is_file()}
    current,rebuilt=digest(ctx.out),digest(fresh)
    changed=sorted(k for k in current.keys()|rebuilt.keys() if current.get(k)!=rebuilt.get(k))
    result={'passed':not changed and verification['passed'] and mob_verification['passed'],'artifacts':len(rebuilt),
            'differing_artifacts':changed,'fresh_output':str(fresh),
            'scope':'All generated files compared byte-for-byte; original inputs and game source read-only.'}
    write_json(runtime/'cleancheck-result.json',result)
    print(('PASS' if result['passed'] else 'FAIL')+f' clean regeneration: {len(rebuilt)} artifacts, {len(changed)} differences')
    for name in changed[:25]:print(name)
    return 0 if result['passed'] else 1
