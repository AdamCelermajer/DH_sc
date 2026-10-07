from pathlib import Path
import json,hashlib,sys
O=Path(__file__).resolve().parent;R=O.parents[3]
j=json.loads((O/'staged-contract.json').read_text());group=sys.argv[1]if len(sys.argv)>1 else '01-active-transport'
assert group in j['patches'];bad=[]
for path,expected in j['patch_base_normalized_sha256'][group].items():
 raw=(R/path).read_bytes();canonical=raw.replace(b'\r\n',b'\n')
 if hashlib.sha256(canonical).hexdigest()!=expected:bad.append(path)
print(json.dumps(dict(patch=group,status='REVIEW_BASE_DRIFT'if bad else'PASS',changed_semantic_source=bad,newline_policy='Only CRLF->LF is normalized; no whitespace/code differences accepted')))
sys.exit(2 if bad else 0)
