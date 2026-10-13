"""Shared atlas I/O; paths always point at read-only original sources."""
import hashlib
import json
from pathlib import Path
from types import SimpleNamespace

ROOT = Path(__file__).resolve().parents[2]

def context(inputs=None):
    if inputs:
        source = Path(inputs).resolve()
    else:
        candidates = [ROOT / '.local-inputs', ROOT.parents[2] / '.local-inputs']
        source = next((p for p in candidates if (p / 'assets-extra/android/data').is_dir()), None)
        if source is None:
            raise RuntimeError('Original data unavailable; use --inputs <read-only .local-inputs directory>.')
    return SimpleNamespace(root=ROOT, inputs=source, out=ROOT / 'coordination/atlas')

def write_json(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + '\n', encoding='utf-8')

def read_json(path, default=None):
    path = Path(path)
    return json.loads(path.read_text(encoding='utf-8')) if path.exists() else default

def stable_id(*parts):
    return hashlib.sha256('|'.join(map(str, parts)).encode('utf-8')).hexdigest()[:20]

def source_roots(ctx):
    candidates = [('android',ctx.inputs / 'assets-extra/android/data'),
                  ('ios',ctx.inputs / 'assets-extra/ios/data'),
                  ('original-cache',ctx.inputs / 'assets-extra/android/original-cache/data'),
                  ('android-extra',ctx.inputs / 'assets-extra/android/3d'),
                  ('audio-cache',ctx.inputs / 'audio-v34/cache')]
    return [(label,p) for label,p in candidates if p.is_dir()]

def source_site(ctx, path):
    return '.local-inputs/' + Path(path).relative_to(ctx.inputs).as_posix()

def markdown_table(headers, rows):
    def cell(v):
        if isinstance(v, (list, dict)): v=json.dumps(v,ensure_ascii=False)
        return str(v).replace('|','\\|').replace('\n','<br>')
    return '\n'.join(['| ' + ' | '.join(headers) + ' |', '| ' + ' | '.join(['---']*len(headers)) + ' |'] +
                     ['| ' + ' | '.join(cell(v) for v in row) + ' |' for row in rows]) + '\n'
