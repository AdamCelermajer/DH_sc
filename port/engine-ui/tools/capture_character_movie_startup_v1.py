"""Capture source platform version and player startup used by authored menus."""
from pathlib import Path
import hashlib,json,struct,sys,re
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[3]
SRC=ROOT/'.local-inputs/libDungeonHunter2.so'
expected='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert hashlib.sha256(SRC.read_bytes()).hexdigest()==expected
OUT=ROOT/'port/engine-ui/reference/character-movie-startup-v1'
OUT.mkdir(parents=True,exist_ok=True)
with SRC.open('rb') as stream:
 elf=ELFFile(stream)
 symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']}
 selected={a for a,s in symbols.items() if 'as_global_get_version' in s.name or 'get_gameswf_version' in s.name or s.name.startswith(('_ZN7gameswf6playerC1E','_ZN7gameswf6playerC2E')) or s.name in ('_GLOBAL__I_.._.._src_gameswf_gameswf_player.cpp','_ZN7gameswf6player11action_initEv')}
 segments=[(s['p_vaddr'],s['p_vaddr']+s['p_filesz'],s['p_offset']) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
 def read(a,n):
  for lo,hi,off in segments:
   if lo<=a and a+n<=hi:stream.seek(off+a-lo);return stream.read(n)
  raise ValueError((a,n))
 rows=[];assembly=[]
 related=[{'symbol':s.name,'address':hex(s['st_value']),'size':s['st_size']} for s in elf.get_section_by_name('.symtab').iter_symbols() if 'gameswf_version' in s.name or ('GLOBAL' in s.name and 'player' in s.name)]
 for address in sorted(selected):
  symbol=symbols[address];raw=read(address,symbol['st_size']);calls={};literals={};strings=[]
  assembly.append('\n# '+symbol.name+' '+hex(address))
  for ins in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address):
   assembly.append(f'{ins.address:08x}: {ins.mnemonic:8} {ins.op_str}')
   match=re.fullmatch(r'(\w+), \[pc, #(-?0x[0-9a-f]+)\]',ins.op_str)
   if ins.mnemonic=='ldr' and match:
    reg,off=match.groups();literals[reg]=struct.unpack('<I',read(ins.address+8+int(off,0),4))[0]
   match=re.fullmatch(r'(\w+), pc, (\w+)',ins.op_str)
   if ins.mnemonic=='add' and match and match[2] in literals:
    a=(ins.address+8+literals[match[2]])&0xffffffff
    try:
     data=read(a,160).split(b'\0',1)[0]
     if data and all(32<=c<127 for c in data):strings.append({'pc':hex(ins.address),'address':hex(a),'text':data.decode()})
    except ValueError:pass
   if ins.mnemonic in ('bl','b','blx') and ins.op_str.startswith('#'):
    target=int(ins.op_str[1:],0)
    if target in symbols:calls[hex(target)]=symbols[target].name
  rows.append({'symbol':symbol.name,'address':hex(address),'size':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'direct_calls':calls,'literal_strings':strings})
 (OUT/'startup-functions.json').write_text(json.dumps({'original_sha256':expected,'functions':rows,'related_symbols':related},indent=2)+'\n')
 (OUT/'startup-functions.asm').write_text('\n'.join(assembly)+'\n')
 tree=ROOT/'.local-inputs/ui-layout-discovery/dqcharmenu_droid-tree.json'
 movie=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf'
 definitions={r['id']:r for r in json.loads(tree.read_text()) if r['tag']==39}
 placements=[r for r in json.loads(tree.read_text()) if r['depth']==0 and r['tag'] in (26,70) and r.get('instance_name','').startswith('menu_')]
 catalog=[]
 for p in placements:
  d=definitions[p['character_id']]
  catalog.append({'name':p['instance_name'],'id':p['character_id'],'depth':p['placement_depth'],'frames':d['frames'],'placement_offset':p['offset'],'definition_offset':d['offset']})
 assert len({r['name'] for r in catalog})==len(catalog) and any(r['name']=='menu_CharacterMenu' for r in catalog)
 metadata={'original_movie_sha256':hashlib.sha256(movie.read_bytes()).hexdigest(),'tree_sha256':hashlib.sha256(tree.read_bytes()).hexdigest(),'root_menu_placements':catalog}
 (OUT/'root-menu-catalog.json').write_text(json.dumps(metadata,indent=2)+'\n')
 header=['// Generated from the original character-menu placements; do not hand-edit.','static constexpr CharacterMenuScreenDefinitionV1 source_screens[]{']
 header += [' {'+json.dumps(r['name'])+','+','.join(str(r[k]) for k in ('id','depth','frames'))+'},' for r in catalog]
 header.append('};')
 (OUT/'root_menu_catalog_v1.inc').write_text('\n'.join(header)+'\n')
 flow=json.loads((ROOT/'port/engine-ui/reference/character-menu-flow-v1/authored-flow-index.json').read_text())
 assert flow['resource_sha256']==metadata['original_movie_sha256']
 native_names=sorted(s for s in flow['strings'] if s.startswith('Native') and s!='NativeGetStringFromSymbol')
 (OUT/'native-callback-names.json').write_text(json.dumps({'original_movie_sha256':metadata['original_movie_sha256'],'names':native_names},indent=2)+'\n')
 (OUT/'native_callback_names_v1.inc').write_text('static constexpr const char* source_native_callbacks[]{\n'+''.join(' '+json.dumps(s)+',\n' for s in native_names)+'};\n')
 print(json.dumps({'validation':'PASS','complete_functions':len(rows),'root_menu_placements':catalog}))
