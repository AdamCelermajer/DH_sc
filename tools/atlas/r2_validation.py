"""Exercise atlas rejection branches on isolated output copies, never original assets."""
from pathlib import Path
from types import SimpleNamespace
import shutil
import tempfile
from common import read_json, write_json

def run(ctx):
    import r2, r2_natives, r2_staging, r2_questions
    scratch=Path(tempfile.mkdtemp(prefix='r2-rejection-',dir=ctx.root/'tools/atlas/.runtime'))
    test=SimpleNamespace(root=ctx.root,inputs=ctx.inputs,out=scratch/'atlas')
    for rel in ('r2/natives','r2/staging'):
        shutil.copytree(ctx.out/rel,test.out/rel)
    for rel in ('README.md','data/calls.json','r2/package-a-progress.json','r2/package-b-progress.json',
                'r2/package-c-progress.json','r2/initial-inventory.json','r2/questions.json','r2/tracked-size-baseline.json'):
        target=test.out/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ctx.out/rel,target)
    results=[]
    def verify(name,checker,needle=None,reject=False):
        result=checker(test)
        passed=(not result['passed'] and any(needle in e for e in result['errors'])) if reject else result['passed']
        results.append({'name':name,'passed':passed,'expected':'reject' if reject else 'accept',
                        'errors':result.get('errors',[])[:8]})
    verify('valid native package accepted',r2_natives.check)
    verify('valid staging ledger accepted',r2_staging.check)
    verify('valid seven answers accepted',r2_questions.check)
    a=read_json(test.out/'r2/package-a-progress.json')
    chosen=next(row for row in a['page_manifest'] if row['status']=='DONE')
    page=test.out/chosen['path'];raw=page.read_bytes();page.unlink()
    verify('missing native page rejected',r2_natives.check,'contract page set incomplete',True)
    page.write_bytes(raw)
    ap=test.out/'r2/package-a-progress.json';raw=ap.read_bytes();chosen['known_answers']=0;write_json(ap,a)
    verify('DONE native without behavioral cases rejected',r2.semantic_contract_check,'lacks two behavioral known answers',True)
    ap.write_bytes(raw)
    b=read_json(test.out/'r2/package-b-progress.json')
    sp=test.out/b['selection_files'][0];raw=sp.read_bytes();s=read_json(sp)
    s['groups']['invalid_duplicate_fixture']=[next(iter(s['groups'].values()))[0]];write_json(sp,s)
    verify('duplicate copy path across groups rejected',r2_staging.check,'duplicate path across selection',True)
    sp.write_bytes(raw)
    cp=test.out/b['classification_files'][0];raw=cp.read_bytes();c=read_json(cp);c['rows'].pop();write_json(cp,c)
    verify('missing source-site classification rejected',r2_staging.check,'classification coverage mismatch',True)
    cp.write_bytes(raw)
    raw=cp.read_bytes();c=read_json(cp);c['rows'][0]['classification']='UNRESOLVED';c['rows'][0]['reason']='';write_json(cp,c)
    verify('unresolved site without reason rejected',r2_staging.check,'missing classification reason',True)
    cp.write_bytes(raw)
    qp=test.out/'r2/questions.json';raw=qp.read_bytes();q=read_json(qp);q['questions'][0]['claims'][0]['evidence'][0]['sha256']='invalid';write_json(qp,q)
    verify('stale original-source question evidence rejected',r2_questions.check,'source evidence drift',True)
    qp.write_bytes(raw)
    q=read_json(qp);q['questions'][0]['known_answers'][0]['expected']='';write_json(qp,q)
    verify('empty expected result rejected',r2_questions.check,'malformed behavioral case',True)
    qp.write_bytes(raw)
    p=test.out/'r2/package-c-progress.json';raw=p.read_bytes();c=read_json(p);c['rows'].pop(0);write_json(p,c)
    verify('dropped initial dossier row rejected',r2.retention_check,'initial dossier work row missing',True)
    p.write_bytes(raw)
    result={'schema':'atlas-r2-checker-rejection-v1','passed':all(r['passed'] for r in results),
            'tests':results,'source_mutation':False,'fixture_location':'ignored tools/atlas/.runtime scratch'}
    write_json(ctx.out/'r2/validation.json',result)
    return result
