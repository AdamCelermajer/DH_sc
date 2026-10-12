from pathlib import Path
import subprocess,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/main-stage9-adoption-v54';main=Path('C:/Users/adamc/Desktop/workspace/DH_sc');p=r/'current-root-stage9-source-v54.patch'
check=subprocess.run(['git','apply','--check',str(p)],cwd=main,text=True,capture_output=True);assert check.returncode==0,check.stdout+check.stderr
(r/'git-apply-check-v54.json').write_text(json.dumps({'command':['git','apply','--check',str(p)],'workdir':str(main),'exit_code':check.returncode,'output':check.stdout+check.stderr,'patch_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'root_mutated':False,'line_endings':'normalized LF unified patch; initial CRLF doubling fixed in successor without modifying source'},indent=2)+'\n')
h=r/'HANDOFF.md';s=h.read_text().replace('Apply-check the source patch against current hashes, compile','Read-only `git apply --check` now PASSES against current Main. The first private diff doubled CRLF in context; successor normalizes unified patch to LF (source unchanged). Older V54 ZIP remains immutable and is superseded. Apply the checked source patch against current hashes, compile');h.write_text(s)
exec(Path('C:/Users/adamc/AppData/Local/Temp/dh2-stage9-handoff-v54.py').read_text().split("(r/'renderer-stage9-hook-v54.inc.proposed').write_text")[0])
# Preserve already reviewed binding/HANDOFF and freeze successor manually.
import zipfile,shutil
for script in Path('C:/Users/adamc/AppData/Local/Temp').glob('dh2-stage9-*v54.py'):shutil.copy2(script,r/script.name)
items=[{'path':f.name,'sha256':hashlib.sha256(f.read_bytes()).hexdigest(),'bytes':f.stat().st_size} for f in r.iterdir() if f.is_file()];digest=hashlib.sha256(json.dumps(items,sort_keys=True).encode()).hexdigest()[:16];z=b/'reports'/('main-stage9-adoption-v54-successor-'+digest+'.zip');assert not z.exists()
with zipfile.ZipFile(z,'w',zipfile.ZIP_DEFLATED) as out:
 for item in items:out.write(r/item['path'],item['path'])
 out.writestr('manifest.json',json.dumps({'version':54,'successor':True,'files':items},indent=2)+'\n')
with zipfile.ZipFile(z) as verify:
 for item in items:assert hashlib.sha256(verify.read(item['path'])).hexdigest()==item['sha256']
print(json.dumps({'zip':str(z),'sha256':hashlib.sha256(z.read_bytes()).hexdigest(),'payloads':len(items),'apply_check':'PASS'},indent=2))
