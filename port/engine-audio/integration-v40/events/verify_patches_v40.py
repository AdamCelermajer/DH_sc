from pathlib import Path
import json,hashlib,subprocess
O=Path(__file__).resolve().parent;R=O.parents[3];V=O/'verify-tree'
j=json.loads((O/'staged-contract.json').read_text())
for e in j['staged_files']:
 p=V/e['path'];assert p.resolve().is_relative_to(V.resolve());p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((O/'source-snapshot'/e['path']).read_bytes())
results=[];before_hashes={}
for group in j['patches']:
 patch=O/'patches'/(group+'.patch');directory=V.relative_to(R).as_posix()
 before_hashes[group]={p:hashlib.sha256((V/p).read_bytes()).hexdigest()for p in j['patches'][group]}
 # Require exact canonical source BEFORE permitting git's mixed-newline
 # compatibility option. Code/space changes do not pass this SHA gate.
 assert all(hashlib.sha256((V/p).read_bytes().replace(b'\r\n',b'\n')).hexdigest()==j['patch_base_normalized_sha256'][group][p]for p in j['patches'][group])
 p=subprocess.run(['git','apply','--ignore-space-change','--check','--directory='+directory,str(patch)],cwd=R,capture_output=True,text=True);assert p.returncode==0,p.stderr
 p=subprocess.run(['git','apply','--ignore-space-change','--directory='+directory,str(patch)],cwd=R,capture_output=True,text=True);assert p.returncode==0,p.stderr
 results.append(dict(patch=group,status='PASS',sha256=hashlib.sha256(patch.read_bytes()).hexdigest()))
assert all((V/e['path']).read_bytes().replace(b'\r\n',b'\n')==(O/'prospective'/e['path']).read_bytes().replace(b'\r\n',b'\n')for e in j['staged_files'])
(O/'patch-verification.json').write_text(json.dumps(dict(scope='Applied only to owned private verify-tree copied from captured source; shared files untouched. Git core.autocrlf rewrites newlines; semantic content compared after only CRLF->LF normalization.',results=results,normalized_staged_contents_match=True,private_tree_patch_base_raw_sha256=before_hashes,private_tree_final_raw_sha256={e['path']:hashlib.sha256((V/e['path']).read_bytes()).hexdigest()for e in j['staged_files']}),indent=2)+'\n')
print('PASS private sequential patches',len(results),'files',len(j['staged_files']))
