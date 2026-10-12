from pathlib import Path
import zipfile,json,collections
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
zp=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
rows=[];configs=[]
with zipfile.ZipFile(zp) as z:
 for info in z.infolist():
  s=info.filename.lower()
  if not s.endswith('/') and any(t in s for t in ['.config','obfusc','filemap','texture','settings']) and not any(s.endswith(x) for x in ['.tga','.png','.bdae']):configs.append([info.filename,info.file_size])
  if info.file_size>2_000_000 or not any(s.endswith(x) for x in ['.xml','.config','.ini','.txt','.cfg','.dat']):continue
  b=z.read(info).lower()
  if any(t in b for t in [b'pvr2',b'pvrtc',b'atc_',b'textureformat',b'obfusc']):
   rows.append({'name':info.filename,'size':info.file_size,'matches':[{'needle':t.decode(),'contexts':[b[max(0,p-60):p+100].decode(errors='replace') for p in [b.find(t)] if p>=0]} for t in [b'pvr2',b'pvrtc',b'atc_',b'textureformat',b'obfusc'] if t in b]})
report={'textual_matches':rows,'potential_config_entries':configs}
(out/'supplied-cache-policy-candidates.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2)[:15000])
