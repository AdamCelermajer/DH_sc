"""Local Comfy MCP + Blender MCP texture lab; no desktop input automation."""
import argparse, asyncio, hashlib, json, os, shutil, socket, sys, uuid
from pathlib import Path

ROOT=Path(__file__).resolve().parent

def recipe_data(path):
    cfg=json.loads(path.read_text())
    for field in ('atlas','source_obj','output'):
        if cfg.get(field):cfg[field]=str((path.resolve().parent/Path(cfg[field])).resolve())
    return cfg

def blender(recipe,stage):
    module=str(ROOT/'blender_runtime.py');config=str(recipe.resolve())
    code=f"exec(compile(open({module!r},encoding='utf-8').read(),{module!r},'exec'))\nresult=run({config!r},{stage!r})"
    with socket.create_connection(('127.0.0.1',9876),timeout=10) as connection:
        connection.settimeout(600)
        connection.sendall(json.dumps({'type':'execute','code':code,'strict_json':True}).encode()+b'\0')
        data=b''
        while b'\0' not in data:
            chunk=connection.recv(65536)
            if not chunk:raise RuntimeError('Blender MCP closed before returning a result')
            data+=chunk
    result=json.loads(data.split(b'\0')[0])
    if result.get('status')!='ok':raise RuntimeError(result)
    return result['result']

def unpack(result):
    if result.is_error:raise RuntimeError(result.model_dump())
    for content in result.content:
        if content.type=='text':
            try:return json.loads(content.text)
            except json.JSONDecodeError:pass
    raise RuntimeError('MCP returned no structured JSON result')

async def enhance(recipe):
    from mcp import ClientSession,StdioServerParameters
    from mcp.client.stdio import stdio_client
    cfg=recipe_data(recipe);out=Path(cfg['output']);manifest=json.loads((out/'capture.json').read_text())
    mcp_home=Path(os.environ.get('TEXTURE_LAB_MCP_HOME',str(Path.home()/'.local/share/comfy-mcp')))
    shared=Path(os.environ.get('TEXTURE_LAB_COMFY_SHARED',str(Path(os.environ['LOCALAPPDATA'])/'Comfy-Desktop/ComfyUI-Shared')))
    env={**os.environ,'COMFY_BIN':str(mcp_home/'venv/Scripts/comfy.exe'),'COMFY_LOCAL_URL':os.environ.get('COMFY_LOCAL_URL','http://127.0.0.1:8188'),'PYTHONUTF8':'1'}
    completed=[]
    async with stdio_client(StdioServerParameters(command=str(mcp_home/'venv/Scripts/comfy-mcp.exe'),env=env)) as (read,write):
        async with ClientSession(read,write) as session:
            await session.initialize();await session.call_tool('server_info',{})
            for i,view in enumerate(manifest['views']):
                state_path=out/f'view-{i}-job.json'
                if state_path.exists():state=json.loads(state_path.read_text())
                else:
                    image_name=f"texture-lab-{cfg['name']}-{i}.png";shutil.copy2(view['reference'],shared/'input'/image_name)
                    graph=json.loads((ROOT/'klein-template.api.json').read_text())
                    graph['1']['inputs']['image']=image_name;graph['5']['inputs']['text']=cfg['prompt']
                    size=cfg.get('resolution',1024)
                    for node in ('10','11'):graph[node]['inputs'].update(width=size,height=size)
                    graph['12']['inputs']['noise_seed']=cfg.get('seed',424242)+i
                    graph['17']['inputs']['filename_prefix']='texture-lab/'+cfg['name']+f'/view-{i}'
                    workflow=out/f'view-{i}.api.json';workflow.write_text(json.dumps(graph,indent=2))
                    unpack(await session.call_tool('validate_workflow',{'workflow_path':str(workflow)}))
                    state=unpack(await session.call_tool('run_workflow',{'workflow_path':str(workflow),'wait':False}))
                    state_path.write_text(json.dumps(state,indent=2))
                prompt_id=state['prompt_id']
                while True:
                    status=unpack(await session.call_tool('job',{'action':'wait','prompt_id':prompt_id,'timeout_seconds':25}))
                    (out/f'view-{i}-status.json').write_text(json.dumps(status,indent=2))
                    if status['status']=='completed':break
                    if status['status'] in ('failed','error','cancelled'):raise RuntimeError(status)
                    print(json.dumps({'view':i,'prompt_id':prompt_id,'status':status['status']}),flush=True)
                fetched=unpack(await session.call_tool('fetch_outputs',{'prompt_id':prompt_id,'out_dir':str(out/f'view-{i}-output')}))
                path=Path(fetched['files'][0]['path'])
                completed.append({'path':str(path),'prompt_id':prompt_id,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
                (out/'enhanced.json').write_text(json.dumps({'views':completed},indent=2))
                print(json.dumps({'view':i,'status':'completed','path':str(path)}),flush=True)
    return {'views':completed}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('stage',choices=['capture','enhance','project','bake','finish','all'])
    parser.add_argument('recipe',type=Path)
    parser.add_argument('--new-run',action='store_true',help='Create a unique run without editing the reusable recipe')
    args=parser.parse_args()
    if args.new_run:
        if args.stage not in ('capture','all'):parser.error('--new-run applies to capture or all')
        cfg=recipe_data(args.recipe);suffix=uuid.uuid4().hex[:8]
        cfg['name']+='-'+suffix;cfg['output']+='-'+suffix
        output=Path(cfg['output']);output.mkdir(parents=True,exist_ok=False)
        args.recipe=output/'recipe.resolved.json';args.recipe.write_text(json.dumps(cfg,indent=2))
    stages=['capture','enhance','project','bake'] if args.stage=='all' else [args.stage]
    for stage in stages:
        print(json.dumps({'stage':stage,'result':asyncio.run(enhance(args.recipe)) if stage=='enhance' else blender(args.recipe,stage)}),flush=True)

if __name__=='__main__':main()
