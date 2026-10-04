"""Fail-closed receipt guards, independent of builds/devices/current sources."""
import importlib.util,json,pathlib,tempfile
R=pathlib.Path(__file__).resolve().parents[3]
def module(name):
    spec=importlib.util.spec_from_file_location(name,R/'port/engine-ui/tools'/f'{name}.py')
    value=importlib.util.module_from_spec(spec);spec.loader.exec_module(value);return value
font=module('verify_font_overlay_stage');hud=module('verify_connected_hud_stage')
checks=rejects=0
def reject(call):
    global checks,rejects
    try:call()
    except (AssertionError,ValueError):checks+=1;rejects+=1;return
    raise AssertionError('invalid receipt input accepted')
old=b'prefix\r\r\nsuffix\r\n'
now=('prefix\n'+hud.GETTER+'suffix\n').encode()
hud.verify_addition(old,now,hud.GETTER);checks+=1
reject(lambda:hud.verify_addition(old,now.replace(b'suffix',b'changed gameplay'),hud.GETTER))
reject(lambda:hud.verify_addition(old,now.replace(b'sheet.data()',b'nullptr'),hud.GETTER))
reject(lambda:hud.verify_addition(old,now+hud.GETTER.encode(),hud.GETTER))
stock={f'core{i}.cpp.o':str(i) for i in range(94)};stock['gameswf_font.cpp.o']='stock'
overlay=dict(stock);overlay['gameswf_font.cpp.o']='overlay'
assert font.check_members(stock,overlay)==['gameswf_font.cpp.o'];checks+=1
bad=dict(overlay);bad['core0.cpp.o']='unknown change';reject(lambda:font.check_members(stock,bad))
bad=dict(overlay);bad['extra.cpp.o']='new';reject(lambda:font.check_members(stock,bad))
reject(lambda:font.check_members(stock,stock))
with tempfile.TemporaryDirectory() as directory:
    file=pathlib.Path(directory)/'bad.a';file.write_bytes(b'!<arch>\nshort')
    reject(lambda:font.archive_members(file))
    values={'long_original_source_name.cpp.o':b'abc','gameswf_font.cpp.o':b'four'}
    file.write_bytes(font.paired_archive(values))
    assert font.archive_members(file,True)==values;checks+=1
print(json.dumps({'validation':'PASS','checks':checks,'deliberate_rejections':rejects,'mismatches':0}))
