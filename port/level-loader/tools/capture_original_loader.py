"""Capture symbol-bounded original functions and their PC-relative strings."""
import argparse,hashlib,json,pathlib,re,struct,subprocess,sys
ap=argparse.ArgumentParser()
ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--objdump',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True)
ap.add_argument('--only-prefix',action='append',default=[],help='Capture only these original symbol prefixes')
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
from elftools.elf.elffile import ELFFile
raw=a.engine.read_bytes()
assert hashlib.sha256(raw).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
elf=ELFFile(a.engine.open('rb'))
def read(addr,n):
    for seg in elf.iter_segments():
        if seg['p_vaddr']<=addr and addr+n<=seg['p_vaddr']+seg['p_filesz']:
            start=seg['p_offset']+addr-seg['p_vaddr'];return raw[start:start+n]
    raise ValueError(hex(addr))
def string(addr):
    try:
        b=read(addr,256).split(b'\0')[0]
        return b.decode('ascii') if b and all(c in (9,10,13) or 32<=c<=126 for c in b) else None
    except (ValueError,UnicodeError):return None
names=subprocess.check_output([str(a.objdump),'--syms','--demangle',str(a.engine)],text=True)
targets=[]
for s in elf.get_section_by_name('.dynsym').iter_symbols():
    n=s.name
    if a.only_prefix:
        if s['st_size'] and s['st_info']['type']=='STT_FUNC' and any(n.startswith(p) for p in a.only_prefix):
            targets.append((s['st_value'],s['st_size'],n))
        continue
    if s['st_size'] and s['st_info']['type']=='STT_FUNC' and (n.startswith(('_ZN6Module','_ZNK6Module')) or 'LoadFromBuffer' in n or
        n.startswith('_ZN13ObjectManager11LoadFromXML') or
        n.startswith(('_ZN12VisualObject','_ZN10GameObject','_ZN5Block')) and any(p in n for p in ('DeclareProperties','InitPre','InitPost','SetRotation','SetScale','SetScaling','SetPosition','SyncRotation','SyncScaling','SetVisualObject','_UpdateRotation','_UpdatePosition','_UpdateScale')) or
        n.startswith('_ZN12VisualObjectC1') or n=='_ZN12VisualObject4SyncEv' or
        n in ('_ZN12AssetManager13loadSceneNodeEPKcS1_bi','_ZN12SceneManager9LoadSceneEPKcS1_bb') or
        n.startswith('_ZN14UserProperties') or n=='_ZN7PFFloor12_LoadNavMeshEPN6glitch5scene14IMeshSceneNodeE' or
        n in ('_ZN13RootSceneNode21ResetPositionFromFileEv','_ZN7PFWorld8LoadRoomEPN6glitch5scene10ISceneNodeEjPKc','_ZN6PFRoom10_LoadFloorEPN6glitch5scene14IMeshSceneNodeEPKc') or
        n.startswith('_ZN5Level') and any(p in n for p in ('Load','Init','Prepare','Create','Clean'))):
        targets.append((s['st_value'],s['st_size'],n))
a.out.mkdir(parents=True,exist_ok=True);rows=[]
for addr,size,name in sorted(targets):
    asm=subprocess.check_output([str(a.objdump),'--disassemble','--demangle',f'--start-address={addr}',f'--stop-address={addr+size}',str(a.engine)],text=True)
    refs=[];loads={}
    for line in asm.splitlines():
        m=re.search(r'^\s*([0-9a-f]+):.*\bldr\s+(r\d+), \[pc,.*@ 0x([0-9a-f]+)',line)
        if m:loads[m[2]]=struct.unpack('<I',read(int(m[3],16),4))[0]
        m=re.search(r'^\s*([0-9a-f]+):.*\badd\s+(r\d+), pc, (r\d+)\b',line)
        if m and m[3] in loads:
            dest=(int(m[1],16)+8+loads[m[3]])&0xffffffff;s=string(dest)
            if s is not None:refs.append({'instruction':m[1],'address':hex(dest),'value':s})
    file=f'{addr:08x}.asm';(a.out/file).write_text(asm)
    rows.append({'address':hex(addr),'size':size,'symbol':name,'capture':file,'pc_relative_strings':refs})
statics=[]
for address,length in ((0x956a44,16),(0x956a54,32),(0x99b07c,80)):
    values=struct.unpack('<'+'I'*(length//4),read(address,length))
    statics.append({'address':hex(address),'words':[{'value':hex(p),'string':string(p)} for p in values]})
report={'engine_sha256':hashlib.sha256(raw).hexdigest(),'functions':rows,'static_data':statics}
(a.out/'symbols-and-strings.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'functions':len(rows),'static_data':statics},indent=2))
