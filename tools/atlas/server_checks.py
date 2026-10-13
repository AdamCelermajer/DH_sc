"""Focused read-only viewer protocol checks against an explicitly started server."""
import hashlib
from urllib.request import Request,urlopen
from urllib.error import HTTPError
from common import read_json

def run(ctx,base='http://127.0.0.1:18789'):
    tests=[]
    def request(path,method='GET'):
        try:
            with urlopen(Request(base+path,method=method),timeout=20) as response:
                return response.status,response.headers,response.read()
        except HTTPError as error:return error.code,error.headers,error.read()
    assets=read_json(ctx.out/'data/assets.json',[])
    wave=next(a for a in assets if a['path'].endswith('/sfx_skill_warrior_headsplitter.wav'))
    status,headers,body=request('/api/source/'+wave['id'])
    tests.append({'name':'indexed WAV serves exact original bytes','passed':status==200 and
        hashlib.sha256(body).hexdigest()==wave['sha256'] and headers.get_content_type() in
        ('audio/wav','audio/x-wav','audio/wave','audio/vnd.wave')})
    script=next(a for a in assets if a['source']=='android' and a['path'].endswith('/swampking_core.luac'))
    status,headers,body=request('/api/source/'+script['id'])
    tests.append({'name':'original Lua source is readable text','passed':status==200 and
        hashlib.sha256(body).hexdigest()==script['sha256'] and headers.get_content_type()=='text/plain'})
    packed=next(a for a in assets if a['source']=='ios-packed' and
        'data/scripts/skills/prince_archer_sidestep.luac' in a.get('logical_paths',[]))
    status,headers,body=request('/api/source/'+packed['id']+'?decoded=1')
    tests.append({'name':'iPad Lua applies only the recorded decoder','passed':status==200 and
        hashlib.sha256(body).hexdigest()==packed['decoded_sha256'] and headers.get_content_type()=='text/plain'})
    for name,path,method,expected in [
        ('unknown asset rejected','/api/source/unknown','GET',404),
        ('source document escape rejected','/api/document?path=../../AGENTS.md','GET',404),
        ('viewer writes rejected','/api/source/'+wave['id'],'POST',405)]:
        status,_,_=request(path,method)
        tests.append({'name':name,'passed':status==expected})
    return {'passed':all(t['passed'] for t in tests),'tests':tests,
            'scope':'Local viewer protocol and source identity; no gameplay or audibility claim.'}
