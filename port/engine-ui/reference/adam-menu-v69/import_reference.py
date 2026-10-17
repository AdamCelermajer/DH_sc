"""Import the reviewed v69 frontend source overlay without changing Adam's checkout.

Original game assets are deliberately not extracted by this source importer.
Existing shared files are compared and retained; the manifest names differences.
"""
from __future__ import annotations
import argparse, hashlib, json, os, re, subprocess
from pathlib import Path

COMMIT = '791e961b12233100b303038c961666834f4beb9d'
OVERLAY = 'session-contributions/menu-launch/verified-v69/'
UI = 'port/engine-ui/'

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adam-repo',required=True,type=Path)
    parser.add_argument('--repo',required=True,type=Path)
    parser.add_argument('--apply',action='store_true')
    args=parser.parse_args();adam=args.adam_repo.resolve();root=args.repo.resolve()
    def git(*parts,input=None,env=None):
        return subprocess.check_output(['git',*parts],cwd=adam,input=input,env=env)
    records={}
    for line in git('ls-tree','-r',COMMIT,'port/engine-ui','port/game-data','port/android-native',
                    'port/asset-payloads','port/scene-materials',OVERLAY).decode().splitlines():
        left,path=line.split('\t');mode,kind,oid=left.split()
        if kind=='blob':records[path]=oid
    blobs={}
    def origin(path):return OVERLAY+path if OVERLAY+path in records else path
    def read(path):
        source=origin(path)
        if source not in blobs:blobs[source]=git('show',COMMIT+':'+source)
        return blobs[source]
    selected=set()
    def add(path):
        if origin(path) not in records:raise RuntimeError('Missing pinned source: '+path)
        if path in selected:return
        selected.add(path)
        if Path(path).suffix in {'.cpp','.hpp','.h','.inc','.cmake'}:
            body=read(path).decode(errors='replace')
            for include in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"',body,re.M):
                local=(Path(path).parent/include).as_posix()
                # Normalize ../ references without allowing import outside port.
                local=os.path.normpath(local).replace('\\','/')
                if local.startswith('port/') and origin(local) in records:add(local)
                elif origin(UI+include) in records:add(UI+include)
    cmake=read(UI+'CMakeLists.txt').decode()
    library=re.search(r'add_library\(dh2_engine_ui SHARED(.*?)\)',cmake,re.S).group(1)
    for name in re.findall(r'\b[\w/-]+\.cpp\b',library):add(UI+name)
    for name in ['gameswf_sources.cmake','gameswf_font_overlay_v1.cmake',
                 'gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake',
                 'gameswf_text_property_overlay_v1.cmake','freetype237-hud.cmake']:
        add(UI+name)
        # Replacement TU paths are not C++ includes.
        for path in re.findall(r'\$\{CMAKE_CURRENT_LIST_DIR\}/([^"\s]+)',read(UI+name).decode()):
            if origin(UI+path) in records:add(UI+path)
    native='port/android-native/app/src/main/cpp/'
    for name in ['original_ui_session.cpp','original_ui_session.hpp','swf_gpu.cpp','swf_gpu.hpp',
                 'original_ui_assets.cpp','original_ui_assets.hpp','original_ui_asset_catalog.inc',
                 'original_menu_sound_data.hpp','menu_frame_clock.hpp','authored_shader_program.cpp']:add(native+name)
    add('port/android-native/app/src/main/java/com/example/dh2/FrontAudio.java')
    for path in ['port/asset-payloads/sha256.cpp','port/asset-payloads/sha256.hpp',
                 'port/scene-materials/swf_texture.cpp','port/scene-materials/swf_texture.hpp',
                 'port/scene-materials/shader_sources.cpp','port/scene-materials/shader_sources.hpp']:add(path)
    for name in ['class_preview_setup.cpp','class_preview_setup.hpp','campaign_profile_files_v1.cpp',
                 'campaign_profile_files_v1.hpp','menu_profile_metadata_v1.cpp','menu_profile_metadata_v1.hpp']:
        add('port/game-data/'+name)
    # Our adapted OriginalUiAssets uses the exact URI catalog and fails on
    # unknown required resources; it does not instantiate another full cache.
    selected.discard(native+'original_cache_assets_v1.hpp')
    # Selected upstream implementations and their macro/relative include trees.
    # Exclude examples, compiled binaries, tests, reports and historical caches.
    for path in records:
        if path.startswith(UI+'vendor/gameswf1714/') or path.startswith(UI+'vendor/freetype-2.3.7-hud/'):
            relative=path.removeprefix(UI+'vendor/')
            if any(x in relative.split('/') for x in ['test','tests','examples','docs','doc','contrib']):continue
            name=Path(path).name.lower()
            if Path(path).suffix.lower() in {'.c','.cpp','.h','.hpp','.inc'} or any(x in name for x in ['license','copyright','readme','notice','copying']):
                selected.add(path)
    # Materialize missing source blobs together; never checkout/reset the clone.
    source_paths=sorted({origin(p) for p in selected});oids=[records[p] for p in source_paths]
    env=os.environ.copy();env['GIT_NO_LAZY_FETCH']='1'
    check=git('cat-file','--batch-check',input=('\n'.join(oids)+'\n').encode(),env=env).decode().splitlines()
    missing=sorted({line.split()[0] for line in check if line.endswith(' missing')})
    if missing:
        print(f'Fetching {len(missing)} pinned source blobs',flush=True)
        fetched=subprocess.run(['git','-c','fetch.negotiationAlgorithm=noop','fetch','origin',
                       '--no-tags','--no-write-fetch-head','--recurse-submodules=no','--filter=blob:none','--stdin'],cwd=adam,
                       input=('\n'.join(missing)+'\n').encode(),stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        if fetched.returncode:raise RuntimeError(fetched.stderr.decode(errors='replace')[-4000:])
    pending=[];manifest=[]
    for path in sorted(selected):
        source=origin(path);raw=read(path);target=(root/path).resolve()
        if not target.is_relative_to(root):raise RuntimeError('Import path escapes repository')
        digest=hashlib.sha256(raw).hexdigest();state='new'
        if target.exists():state='same' if target.read_bytes()==raw else 'existing-different'
        if state=='new':pending.append((target,raw))
        manifest.append({'path':path,'upstream_path':source,'git_blob':records[source],
                         'upstream_sha256':digest,'action':state})
    report={'commit':COMMIT,'overlay':OVERLAY,'scope':'Frontend source only; existing shared owners retained; no original assets, APKs, reports or save fixtures copied.',
            'files':manifest,'selected_ui_translation_units':re.findall(r'\b[\w/-]+\.cpp\b',library)}
    dest=Path(__file__).with_name('import-manifest.json');dest.write_text(json.dumps(report,indent=2)+'\n')
    if args.apply:
        for target,raw in pending:target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
    print(json.dumps({'files':len(manifest),'new':len(pending),'existing_different':[x['path'] for x in manifest if x['action']=='existing-different'],'applied':args.apply}))
if __name__=='__main__':main()
