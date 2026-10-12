from pathlib import Path
import json,hashlib,zipfile
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-kernel-v51';reports=b/'reports';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
p=reports/'character-parent-kernel-v51-verified.json';receipt=json.loads(p.read_text());assert receipt['validation'].startswith('PASS') and not receipt['whole_Character_InitPost_success']
for source,h in receipt['selected_sources_unchanged'].items():assert sha(Path(source))==h,source
doc=reports/'character-parent-kernel-v51-handoff.md';s=doc.read_text().replace('Syntax passed','Native verified').replace('Test prepared','Native verified').replace('Test syntax passed; native awaits coherent751','Native verified against coherent751');s+='\nNative result: shared generic original-control cases4 and actual required producer stops2 passed; Character signed16 upper-halfword guard passed. Actual ordered NPC/Faery models enter shared parent conditions/cache/SetPosition and stop at required Device, retaining expected source prefix and RNG count8. Old generic initialization/condition archive objects are excluded. Whole Character/level activation remains false.\n';doc.write_text(s)
(r/'freeze_parent_kernel_v51.py').write_bytes(Path(__file__).read_bytes())
files=[p for p in r.rglob('*') if p.is_file() and '__pycache__' not in p.parts]
for n in ['character-parent-kernel-v51-verified.json','character-parent-kernel-v51-handoff.md','parent_kernel_control_v51-native.log','parent_character_model_v51-native.log','parent_kernel_control_v51.map','parent_character_model_v51.map']:files.append(reports/n)
original=b/'reference/character-parent-init-v48/gameobject-parent-body-original-v48.json';files.append(original)
manifest={str(p.relative_to(b)).replace('\\','/'):sha(p) for p in files};packet=reports/'character-parent-kernel-v51-shared.zip'
with zipfile.ZipFile(packet,'w',zipfile.ZIP_DEFLATED) as z:
 for f in files:z.write(f,str(f.relative_to(b)).replace('\\','/'))
 z.writestr('SHA256SUMS.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(packet) as z:
 for n,h in manifest.items():assert hashlib.sha256(z.read(n)).hexdigest()==h,n
out={'schema':'dh2-character-parent-kernel-v51-frozen','packet':str(packet),'packet_sha256':sha(packet),'members_verified':len(manifest),'shared_original_control':True,'actual_Character_parent_prefix':True,'whole_Character_InitPost_success':False,'selected_graph_written':False,'files':manifest}
(reports/'character-parent-kernel-v51-frozen.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({k:v for k,v in out.items() if k!='files'},indent=2))
