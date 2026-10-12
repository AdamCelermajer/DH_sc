"""Preserve each extra checkout's exact committed HEAD before cleanup."""
import argparse
import json
from pathlib import Path
import re
import subprocess

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc').resolve()
allowed = [Path('C:/Users/adamc/Desktop/workspace/DH_wt').resolve(),
           Path('C:/Users/adamc/.codex/worktrees').resolve()]
parser = argparse.ArgumentParser()
parser.add_argument('--output', type=Path, required=True)
options = parser.parse_args()

def git(*args):
    return subprocess.check_output(['git', '-C', str(root), *args], text=True).strip()

records = []
for block in git('worktree', 'list', '--porcelain').split('\n\n'):
    values = dict(line.split(' ', 1) for line in block.splitlines() if ' ' in line)
    path = Path(values['worktree']).resolve()
    if path == root:
        continue
    if path != Path('C:/DHqa').resolve() and not any(path.is_relative_to(parent) for parent in allowed):
        raise ValueError('Checkout outside the named cleanup roots: ' + str(path))
    head = values['HEAD']
    if not re.fullmatch('[0-9a-f]{40}', head):
        raise ValueError('Invalid HEAD: ' + str(path))
    status = subprocess.check_output(['git', '-C', str(path), 'status', '--porcelain=v1',
                                     '--untracked-files=all', '--ignored=no'], text=True)
    if status:
        raise ValueError('Commit every tracked/untracked source change first: ' + str(path))
    label = path.parent.name if path.name == 'DH_sc' else path.name
    if not re.fullmatch('[a-zA-Z0-9._-]+', label):
        raise ValueError('Unsupported checkout label')
    ref = 'refs/heads/codex/archive-wt-20261012-' + label.lower()
    existing = subprocess.run(['git', '-C', str(root), 'rev-parse', '--verify', ref],
                              text=True, capture_output=True)
    if existing.returncode == 0:
        if existing.stdout.strip() != head:
            raise ValueError('Preserving conflicting existing archive ref: ' + ref)
    else:
        git('update-ref', ref, head, '0' * 40)
    records.append(dict(path=str(path), head=head, preservationRef=ref,
                        originalBranch=values.get('branch'), label=label))
if len({row['preservationRef'] for row in records}) != len(records):
    raise ValueError('Duplicate archive label')
options.output.parent.mkdir(parents=True, exist_ok=True)
options.output.write_text(json.dumps(dict(primaryRoot=str(root), worktrees=records), indent=2)+'\n')
print(json.dumps(dict(preserved_heads=len(records), output=str(options.output))))
