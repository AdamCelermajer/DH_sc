"""New receipt after typed same-authority Save continuation fields; preserve v1."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, struct
root = Path(__file__).resolve().parents[3]
out = root / '.local-inputs/player-save-load-owner-v2-host'
def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()
historical_path = root / 'port/game-data/reports/player-save-load-owner-v1-host-audit.json'
old = json.loads(historical_path.read_text())
assert old['validation'] == 'PASS'
report = dict(old)
report['recorded_utc'] = datetime.now(timezone.utc).isoformat()
report['historical_v1_receipt_sha256'] = digest(historical_path)
report['change'] = 'Same sole Save/profile authority gains typed source default-constructor continuation fields (+c=false,+178=0), read accessors and Save writer friendship; Load bodies unchanged.'
report['native'] = {}
for mode in ('SAN', 'O2'):
    result = json.loads((out / f'result-{mode}.json').read_text())
    assert result == old['native'][mode]['result']
    report['native'][mode] = dict(result=result, result_sha256=digest(out/f'result-{mode}.json'), executable_sha256=digest(out/f'load-{mode}'))
names = list(old['source_hashes']) + ['port/game-data/tools/build_player_save_load_owner_v2.sh', 'port/game-data/tools/audit_player_save_load_owner_v2.py']
report['source_hashes'] = {name: digest(root/name) for name in names}
obj = out/'load-owner-arm64.o'; raw = obj.read_bytes()
assert raw[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H', raw, 18)[0] == 183
report['Android_compile'] = dict(sha256=digest(obj), ELF64_AArch64=True, scope='Compile only; not linked or run on Android')
path = root/'port/game-data/reports/player-save-load-owner-v2-host-audit.json'
path.write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps(dict(validation='PASS', report=str(path), historical_v1_preserved=True)))
