"""Preserve actual40-check source and recover the preceding37-check test exactly."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];MODULE=ROOT/'port/script-runtime'
path=MODULE/'tests/script_scalar.cpp';new=MODULE/'tests/script_scalar_source.cpp'
def sha(data):return hashlib.sha256(data).hexdigest()
data=path.read_bytes();assert sha(data)=='18eaf57fb40915f888de6ffc5a515dca0bcd4cde2484a17891168f0bd33a818e'
prefixes=[b'  load(vm,"sixteen=BitOr',b'  state.identity=0xfedcba98;state.calls.clear();dh2_script_value id_arg',b'  assert(dh2_script_vm_call(vm,"FromFixed",&id_arg']
lines=data.splitlines(keepends=True);removed=[line for line in lines if any(line.startswith(p) for p in prefixes)];assert len(removed)==3
recovered=b''.join(line for line in lines if line not in removed)
main=MODULE/'reports/script-scalar-main-linked-host-audit.json';report=json.loads(main.read_text())
assert report['host_audit']['actual_VM_checks']==37 and report['source_sha256']['port\\script-runtime\\tests\\script_scalar.cpp']==sha(data)
assert not new.exists();new.write_bytes(data);path.write_bytes(recovered)
proof=dict(action=__doc__,previous_main_report=dict(path=main.relative_to(ROOT).as_posix(),sha256=sha(main.read_bytes()),executed_checks=37,recorded_source_sha256=sha(data)),historical_mismatch='Prior main report executed37-check binary while recording40-check source SHA; no old report is rewritten.',preserved40_source=dict(path=new.relative_to(ROOT).as_posix(),sha256=sha(data)),recovered37_source=dict(path=path.relative_to(ROOT).as_posix(),sha256=sha(recovered)),removed_lines=[r.decode().rstrip() for r in removed],production_scalar_source_changed=False,gold_changed=False)
(MODULE/'reports/script-scalar-test-source-correction.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof))
