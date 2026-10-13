"""Exercise the atlas checker's required rejection branches on private copies."""
from pathlib import Path
from types import SimpleNamespace
import shutil
from common import read_json,write_json

def run(ctx):
    from atlas import audit
    fixture=ctx.root/'tools/atlas/.runtime/check-fixture'
    (fixture/'data').mkdir(parents=True,exist_ok=True)
    for p in (ctx.out/'data').glob('*.json'):shutil.copy2(p,fixture/'data'/p.name)
    testctx=SimpleNamespace(root=ctx.root,inputs=ctx.inputs,out=fixture)
    tests=[]
    baseline=audit(testctx)
    tests.append({'name':'unmodified generated atlas passes','passed':baseline['passed'],'errors':baseline['errors'][:5]})
    if not baseline['passed']:
        result={'passed':False,'tests':tests};write_json(ctx.out/'data/validation.json',result);return result
    def test(name,file,mutate,expected):
        p=fixture/'data'/f'{file}.json';original=p.read_bytes()
        try:
            value=read_json(p);mutate(value);write_json(p,value)
            result=audit(testctx)
            matched=[e for e in result['errors'] if expected in e]
            tests.append({'name':name,'passed':not result['passed'] and bool(matched),'observed':matched[:2]})
        finally:p.write_bytes(original)
    def drop_reason(rr):
        r=next(r for r in rr if r['status']=='UNRESOLVED');r['reason']=None;r.pop('reason_code',None)
    def drop_candidates(rr):
        next(r for r in rr if r['status']=='UNRESOLVED').pop('candidates',None)
    def bad_id(rr):
        r=rr[0];r['referenced_table']='animation_sequences';r['referenced_index']=999999
    def bad_orphan(rr):
        r=rr[0];r['referrers']=[];r['status']='REFERENCED'
    def drop_event(rr):
        i=next(i for i,r in enumerate(rr) if r.get('rule','').startswith('BRES_EVENT'))
        rr.pop(i)
    test('missing script row fails','scripts',lambda rr:rr.pop(0),'script')
    test('missing function row fails','functions',lambda rr:rr.pop(0),'function')
    test('missing XML/BRES reference site fails','references',drop_event,'reference')
    test('missing row status fails','calls',lambda rr:rr[0].pop('status'),'no status')
    test('unresolved reference without reason fails','references',drop_reason,'reason code')
    test('unresolved reference without candidates fails','references',drop_candidates,'candidates')
    test('nonexistent referenced id fails','references',bad_id,'does not exist')
    test('unreferenced file without ORPHAN fails','assets',bad_orphan,'not ORPHAN')
    result={'passed':all(t['passed'] for t in tests),'tests':tests,
            'scope':'Private generated-data copies under tools/atlas/.runtime; original data and game source untouched.'}
    write_json(ctx.out/'data/validation.json',result)
    return result
