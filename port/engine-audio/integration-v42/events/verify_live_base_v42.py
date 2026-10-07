"""Read-only exact source gate before parent applies the V42 producer diff."""
from pathlib import Path
import hashlib,json,sys
O=Path(__file__).resolve().parent;R=O.parents[3]
contract=json.loads((O/'staged-contract.json').read_text())
changed=[]
for row in contract['files']:
 raw=(R/row['path']).read_bytes().replace(b'\r\n',b'\n')
 if hashlib.sha256(raw).hexdigest()!=row['base_canonical_sha256']:changed.append(row['path'])
print(json.dumps(dict(status='FAIL'if changed else'PASS',semantic_drift=changed)))
sys.exit(2 if changed else 0)
