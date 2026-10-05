import argparse,json,pathlib,sys
ap=argparse.ArgumentParser();ap.add_argument('needle');ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True);a=ap.parse_args()
sys.path.insert(0,str(a.dependency_root))
from elftools.elf.elffile import ELFFile
with a.engine.open('rb') as f:
    e=ELFFile(f)
    for s in e.get_section_by_name('.dynsym').iter_symbols():
        if a.needle in s.name:print(json.dumps({'name':s.name,'address':hex(s['st_value']),'size':s['st_size']}))
