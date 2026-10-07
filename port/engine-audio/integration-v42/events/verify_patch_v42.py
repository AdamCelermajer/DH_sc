"""Apply only to a private captured tree, after exact canonical SHA gates."""
from pathlib import Path
import hashlib,json,shutil,subprocess
O=Path(__file__).resolve().parent;R=O.parents[3];V=O/'verify-tree'
contract=json.loads((O/'staged-contract.json').read_text())
for row in contract['files']:
 src=O/'source-snapshot'/row['path'];raw=src.read_bytes()
 assert hashlib.sha256(raw).hexdigest()==row['base_sha256']
 assert hashlib.sha256(raw.replace(b'\r\n',b'\n')).hexdigest()==row['base_canonical_sha256']
 dst=V/row['path'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,dst)
patch=O/'active-application-producers-v42.patch'
for mode in ('--check',''):
 cmd=['git','-c','core.autocrlf=false','apply','--ignore-space-change']+([mode]if mode else[])+[str(patch)]
 subprocess.run(cmd,cwd=V,check=True,capture_output=True)
for row in contract['files']:
 assert (V/row['path']).read_bytes().replace(b'\r\n',b'\n')==(O/'prospective'/row['path']).read_bytes()
(O/'patch-receipt.json').write_text(json.dumps(dict(status='PASS',scope='Private copied tree only. Exact canonical SHA guard before mixed-newline git compatibility. Final normalized contents equal prospective.',files=len(contract['files']),patch_sha256=hashlib.sha256(patch.read_bytes()).hexdigest()),indent=2)+'\n')
print('PASS private patch',len(contract['files']),'files')
