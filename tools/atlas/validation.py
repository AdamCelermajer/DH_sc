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
    testctx=SimpleNamespace(root=ctx.root,inputs=ctx.inputs,out=fixture,view_root=ctx.out)
    tests=[]
    baseline=audit(testctx)
    tests.append({'name':'unmodified generated atlas passes','passed':baseline['passed'],'errors':baseline['errors'][:5]})
    if not baseline['passed']:
        result={'passed':False,'tests':tests};write_json(ctx.out/'data/validation.json',result);return result
    def test(name,file,mutate,expected):
        p=fixture/'data'/f'{file}.json'
        # The checker only writes its check report. Restore the mutation's
        # authority file and the sole paired-projection mutation, rather than
        # rereading and rewriting the entire large atlas for every probe.
        paths=[p]
        if name=='paired script reference deletion fails':paths.append(fixture/'data/script-references.json')
        originals={path:path.read_bytes() for path in paths}
        try:
            value=read_json(p);mutate(value);write_json(p,value)
            result=audit(testctx)
            matched=[e for e in result['errors'] if expected in e]
            tests.append({'name':name,'passed':not result['passed'] and bool(matched),'observed':matched[:2]})
        finally:
            for original_path,content in originals.items():original_path.write_bytes(content)
    def drop_reason(rr):
        r=next(r for r in rr if r['status']=='UNRESOLVED');r['reason']=None;r.pop('reason_code',None)
    def drop_candidates(rr):
        next(r for r in rr if r['status']=='UNRESOLVED').pop('candidates',None)
    def bad_id(rr):
        r=rr[0];r['referenced_table']='animation_sequences';r['referenced_index']=999999
    def bad_resolve_target(rr):
        r=next(r for r in rr if r.get('status')=='RESOLVED' and r.get('resolves_to'))
        target=next((t for t in r['resolves_to'] if t.get('path')),None)
        if target is None:raise ValueError('No resolved path target is available for this fixture.')
        target['path']='q:/data/iphone/3d/textures/atlas_missing_fixture.bdae'
    def bad_typed_id(rr):
        r=next(r for r in rr if r.get('status')=='RESOLVED' and r.get('resolves_to'))
        r['resolves_to'].append({'type':'table-row','table':'animation_sequences','index':999999})
    def extra_function(rr):
        r=dict(rr[0]);r['id']='fixture-extra-function';r['name']='fixture-extra-function';rr.append(r)
    def extra_reference_and_projection(rr):
        projection_path=fixture/'data'/'script-references.json'
        projection=read_json(projection_path,[])
        projection_ids={r.get('id') for r in projection}
        row=next(r for r in rr if r.get('id') in projection_ids)
        rid=row['id']
        rr[:] = [r for r in rr if r.get('id')!=rid]
        projection[:]=[r for r in projection if r.get('id')!=rid]
        write_json(projection_path,projection)
    def duplicate_table_key(rr):
        row=dict(rr[0]);row['id']='fixture-duplicate-table';rr.append(row)
    def stale_table_key(rr):
        row=dict(rr[0]);row.update(id='fixture-stale-table',table='fixture_table',index=999999);rr.append(row)
    def bad_orphan(rr):
        r=rr[0];r['referrers']=[];r['status']='REFERENCED'
    def fake_referrer(rr):
        r=next(r for r in rr if r.get('status')=='REFERENCED');r['referrers']=['fixture-nonexistent-referrer']
    def invalid_status(rr):
        r=rr[0];r['status']='PASS'
        if 'work_status' in r:r['work_status']='PASS'
    def invalid_work_status(rr):
        r=rr[0];r['work_status']='PASS'
    def drop_event(rr):
        i=next(i for i,r in enumerate(rr) if r.get('rule','').startswith('BRES_EVENT'))
        rr.pop(i)
    test('missing script row fails','scripts',lambda rr:rr.pop(0),'script')
    test('missing mob profile fails','mob-profiles',lambda rr:rr.pop(0),'mob profiles')
    test('missing mob section fails','mob-profiles',lambda rr:rr[0]['sections'].pop('sounds'),'section coverage')
    test('missing CharSounds reference fails','references',lambda rr:rr.__setitem__(slice(None),
         [r for r in rr if not r.get('site','').endswith('[section=0,row=1].Die[0]')]),'CharSounds source reference')
    test('missing function row fails','functions',lambda rr:rr.pop(0),'function')
    test('missing XML/BRES reference site fails','references',drop_event,'reference')
    test('missing row status fails','calls',lambda rr:rr[0].pop('status'),'no status')
    test('unresolved reference without reason fails','references',drop_reason,'reason code')
    test('unresolved reference without candidates fails','references',drop_candidates,'candidates')
    test('unresolved function without candidates fails','functions',drop_candidates,'unresolved without candidates list')
    test('nonexistent referenced id fails','references',bad_id,'does not exist')
    test('nonexistent typed table target fails','references',bad_typed_id,'does not exist')
    test('nonexistent target path fails','references',bad_resolve_target,'target path does not exist')
    test('extra function row fails','functions',extra_function,'functions: source coverage differs')
    test('paired script reference deletion fails','references',extra_reference_and_projection,'references: source coverage differs')
    test('duplicate table source key fails','tables',duplicate_table_key,'duplicate source key')
    test('stale table source key fails','tables',stale_table_key,'stale source table row')
    test('unreferenced file without ORPHAN fails','assets',bad_orphan,'not ORPHAN')
    test('fabricated referrer fails','assets',fake_referrer,'unknown referrer')
    test('unknown row status fails','calls',invalid_status,'invalid status')
    test('unknown work status fails','assets',invalid_work_status,'invalid work_status')
    result={'passed':all(t['passed'] for t in tests),'tests':tests,
            'scope':'Private generated-data copies under tools/atlas/.runtime; original data and game source untouched.'}
    write_json(ctx.out/'data/validation.json',result)
    return result
