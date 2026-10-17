"""Actual selected frontend/text/data DSO gate with declared AS/service fixtures.

No candidate staging, original asset copying, ARM replay, APK or native live claim.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

ROOT=Path(__file__).resolve().parents[3]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    for name in ('cache','build','compiler','report'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();a.build.mkdir(parents=True,exist_ok=True)
    env=os.environ.copy();env['PATH']=str(a.compiler.parent)+os.pathsep+env.get('PATH','')
    logs=[]
    def run(command):
        r=subprocess.run([str(x)for x in command],cwd=ROOT,env=env,capture_output=True,text=True)
        logs.append({'command':[str(x)for x in command],'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
        (a.build/'gate-commands.json').write_text(json.dumps(logs,indent=2)+'\n')
        if r.returncode:raise RuntimeError((r.stdout+r.stderr)[-5000:])
        return r.stdout
    wrapper=ROOT/'port/engine-ui/tests/menu-frontend-v69/CMakeLists.txt'
    production=ROOT/'port/engine-ui/menu_frontend_v69.cmake'
    native=ROOT/'port/android-native/app/src/main/cpp/CMakeLists.txt'
    assert 'menu_frontend_v69.cmake' in native.read_text(), 'Production native selection is mandatory'
    run(['cmake','-S',wrapper.parent,'-B',a.build,'-G','Ninja','-DCMAKE_BUILD_TYPE=Release',
         '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DCMAKE_CXX_COMPILER='+str(a.compiler),
         '-DCMAKE_C_COMPILER='+str(a.compiler.with_name('gcc.exe'))])
    database=json.loads((a.build/'compile_commands.json').read_text())
    selected=[r for r in database if any('CMakeFiles/'+t+'.dir' in r['command'].replace('\\','/') for t in
        ('dh2_engine_ui','dh2_gameswf_core','dh2_inventory_text_v1','dh2_game_data','dh2_freetype237','dh2_menu_host_zlib','menu_frontend_v69_host'))]
    menu=[r for r in selected if 'CMakeFiles/dh2_engine_ui.dir' in r['command'].replace('\\','/')]
    text=[r for r in selected if 'CMakeFiles/dh2_inventory_text_v1.dir' in r['command'].replace('\\','/')]
    assert len(text)==5
    for name in ('localization.cpp','hud_text_v1.cpp','hud_text_format_v1.cpp','item_text_owner_v5.cpp','item_text_varargs_v5.cpp'):
        assert not any(Path(r['file']).name==name for r in menu), 'Duplicate text owner'
    assert sum(Path(r['file']).name=='swf_menu_launch_v1.cpp'for r in menu)==1
    def inputs():
        paths={Path(r['file']).resolve()for r in selected}
        # Actual compiler header dependency closure from this configured build.
        for line in run(['ninja','-C',a.build,'-t','deps']).splitlines():
            if line.startswith('    '):
                p=Path(line.strip()).resolve()
                if p.is_file()and p.is_relative_to(ROOT):paths.add(p)
        paths.update((wrapper,production,native,ROOT/'port/game-data/CMakeLists.txt',
                      ROOT/'port/engine-ui/inventory_text_v1.cmake',Path(__file__).resolve()))
        paths.update((ROOT/'port/engine-ui'/name) for name in ('gameswf_sources.cmake','gameswf_font_overlay_v1.cmake',
                    'gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake',
                    'gameswf_text_property_overlay_v1.cmake','freetype237-hud.cmake'))
        return {p.relative_to(ROOT).as_posix():digest(p)for p in sorted(paths)}
    before=inputs()
    (a.build/'source-before.json').write_text(json.dumps(before,indent=2)+'\n')
    run(['cmake','--build',a.build,'--clean-first','--parallel','3','--target','menu_frontend_v69_host'])
    after=inputs();(a.build/'source-after.json').write_text(json.dumps(after,indent=2)+'\n')
    changed=[p for p in sorted(before.keys()|after.keys())if before.get(p)!=after.get(p)]
    assert before==after, 'Selected sources/headers changed during clean rebuild: '+str(changed)
    ui=a.build/'libdh2_engine_ui.dll';text_dso=a.build/'libdh2_inventory_text_v1.dll';data=a.build/'game-data/libdh2_game_data.dll'
    for p in (ui,text_dso,data):assert p.is_file()
    env['PATH']=str(ui.parent)+os.pathsep+str(data.parent)+os.pathsep+env['PATH']
    exe=a.build/'menu_frontend_v69_host.exe';result=json.loads(run([exe,a.cache]))
    assert result['validation']=='PASS'and result['mismatches']==0
    def imports(p):return re.findall(r'DLL Name:\s*([^\s]+)',run([a.compiler.with_name('objdump.exe'),'-p',p]))
    dependencies={p.name:imports(p)for p in (exe,ui,text_dso)}
    assert 'libdh2_engine_ui.dll'in dependencies[exe.name]and 'libdh2_inventory_text_v1.dll'in dependencies[ui.name]
    assert 'libdh2_game_data.dll'in dependencies[ui.name]and 'libdh2_game_data.dll'in dependencies[text_dso.name]
    ref=ROOT/'port/engine-ui/reference/adam-menu-v69'
    report={'validation':'PASS','scope':__doc__,'upstream_commit':'791e961b12233100b303038c961666834f4beb9d',
            'source_before_after_equal':True,'source_sha256':before,'selected_commands':selected,
            'selected_menu_TUs':len(menu),'selected_text_TUs':len(text),'test':result,
            'libraries':{p.name:{'sha256':digest(p),'bytes':p.stat().st_size}for p in (ui,text_dso,data)},
            'executable_sha256':digest(exe),'dll_dependencies':dependencies,
            'cache_sha256':{('design'+s):digest(a.cache/('design'+s))for s in ('_pyarray.bin','_pyarraynames.bin','_pystructnames.bin')},
            'reference_sha256':{p.name:digest(p)for p in ref.iterdir()if p.is_file()and p.name!='commit-whitelist.json'}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','test':result,'menu_TUs':len(menu),'text_TUs':len(text),'guarded_inputs':len(before),'report':str(a.report)}))
if __name__=='__main__':main()
