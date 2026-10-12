"""Build the exact live worktree-removal manifest from preserved source refs."""
from datetime import datetime, timezone
import json
from pathlib import Path
import subprocess

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc').resolve()
preserved = json.loads((root/'coordination/v21.3/WORKTREE-PRESERVATION.json').read_text())
by_path = {str(Path(row['path']).resolve()).lower(): row for row in preserved['worktrees']}
manifest = dict(schemaVersion=1, primaryRoot=str(root), scopeReviewed=True, releaseAccepted=True,
                capturedAtUtc=datetime.now(timezone.utc).isoformat(), expectedWorktrees=[], removals=[], holds=[])
assert 'Root accepts v21.3' in (root/'coordination/v21.3/FINAL-REVIEW.md').read_text()
listing = subprocess.check_output(['git', '-C', str(root), 'worktree', 'list', '--porcelain'], text=True)
for block in listing.strip().split('\n\n'):
    values = dict(line.split(' ', 1) for line in block.splitlines() if ' ' in line)
    path = Path(values['worktree']).resolve()
    if path == root:
        continue
    saved = by_path[str(path).lower()]
    assert saved['head'] == values['HEAD'], 'Live HEAD changed'
    item = dict(path=str(path), head=values['HEAD'])
    manifest['expectedWorktrees'].append(item)
    if path.parent.name in ('b117-chest-crash', 'b118-continue-crash'):
        manifest['holds'].append(dict(**item, reason='App archive queued; remove only through archive_worktree'))
    else:
        manifest['removals'].append(dict(**item, preservationRef=saved['preservationRef'],
                                         ownerSnapshotComplete=True, localInputsPreserved=True))
output = root/'.local-inputs/v21.3/root/worktree-removal-manifest.json'
output.write_text(json.dumps(manifest, indent=2)+'\n')
print(json.dumps(dict(removals=len(manifest['removals']), holds=len(manifest['holds']), output=str(output))))
