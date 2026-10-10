"""Check unsupported provider saves reject before replacing the durable save."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument('--package', required=True, type=Path)
parser.add_argument('--report', required=True, type=Path)
options = parser.parse_args()
package = options.package.resolve()
save = package / 'gameplay.save'
sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
original = sha(save)
checks = []
with tempfile.TemporaryDirectory(prefix='dh2-save-check-') as directory:
    for name, template, frame in [('controller', 'controller-independent-verification.args', 30),
                                  ('lifecycle', 'spawn-inflight-verification.args', 80)]:
        text = (package / template).read_text().replace('--assets\nassets\n', '--assets\n' + str(package / 'assets') + '\n')
        text += f'\n--save-frame\n{frame}\n--game-save\n{save}\n'
        config = Path(directory) / (name + '.args')
        config.write_text(text)
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', str(config)],
                                cwd=directory, capture_output=True, text=True, timeout=60)
        output = result.stdout + result.stderr
        assert result.returncode == 1, output
        assert 'Campaign lifecycle/controller/animation providers are not persisted' in output, output
        assert sha(save) == original, 'Rejected save modified the existing durable save'
        checks.append({'name': name + '-save-rejection', 'exit_code': 1, 'durable_save_preserved': True})
receipt = {'status': 'expected_runtime_rejections_verified', 'goal_complete': False,
           'executable_sha256': sha(package / 'dh-foundation.exe'), 'save_sha256': original, 'checks': checks}
options.report.resolve().write_text(json.dumps(receipt, indent=2) + '\n')
print(json.dumps({'negative_checks': len(checks), 'durable_save_preserved': True}))
