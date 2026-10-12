"""Freeze current checkpoint sources/evidence; this is a bounded integration slice.

Requires the existing isolated reconstruction baseline and original cache/ELF
for rebuilding. Does not publish, merge, include campaign state or move devices.
"""
import hashlib, json, pathlib, zipfile

root = pathlib.Path(__file__).resolve().parents[1]; worktree = root.parents[1]
assert worktree == pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports = root/'reports'; checkpoint_path = reports/'source-pipeline-checkpoint.json'
checkpoint = json.loads(checkpoint_path.read_text())
assert checkpoint['validation'] == 'PASS' and not checkpoint['full_loader_verified']
def sha(path):
    with path.open('rb') as stream: return hashlib.file_digest(stream,'sha256').hexdigest()
files = {}
for relative, expected in checkpoint['source_sha256'].items():
    path = worktree/relative
    assert sha(path) == expected, relative
    files[relative] = path
for name, expected in checkpoint['receipt_sha256'].items():
    path = reports/name
    assert sha(path) == expected, name
    files[path.relative_to(worktree).as_posix()] = path
preview_path = reports/'source-pipeline-preview.json'
assert sha(preview_path) == checkpoint['preview_receipt_sha256']
preview = json.loads(preview_path.read_text())
for name, expected in preview['screenshots'].items():
    path = reports/name
    assert sha(path) == expected, name
    files[path.relative_to(worktree).as_posix()] = path
for name in ('source-pipeline-checkpoint.json','source-pipeline-preview.json',
             'source-pipeline-preview-logcat.txt','source-pipeline-swamp-declarations.json',
             'canonical-level-inventory.json','preview-apk.json'):
    path = reports/name; files[path.relative_to(worktree).as_posix()] = path
# Include all loader verification sources, even those not rerun this milestone.
# Original test receipts outside this slice remain historical in the worktree.
for folder in ('tests','tools'):
    for path in (root/folder).glob('*'):
        if path.is_file() and path.suffix in ('.py','.cpp','.hpp','.h'):
            files[path.relative_to(worktree).as_posix()] = path
for path in root.glob('*.md'):
    files[path.relative_to(worktree).as_posix()] = path
manifest = {'scope':__doc__,'checkpoint_sha256':sha(checkpoint_path),
            'apk_sha256':checkpoint['apk_sha256'],
            'files_sha256':{key:sha(path) for key,path in sorted(files.items())},
            'requires_existing_private_baseline':True,
            'runtime_factory_provider_available':False,'full_loader_verified':False}
encoded = (json.dumps(manifest,indent=2)+'\n').encode()
name = 'source-pipeline-handoff-'+sha(checkpoint_path)[:16]+'.zip'
target = reports/name
assert not target.exists(), 'Existing frozen handoff must not be overwritten: '+name
with zipfile.ZipFile(target,'x',compression=zipfile.ZIP_DEFLATED,compresslevel=6) as archive:
    for relative,path in sorted(files.items()):
        info = zipfile.ZipInfo(relative,date_time=(2026,10,5,0,0,0))
        info.compress_type = zipfile.ZIP_DEFLATED
        archive.writestr(info,path.read_bytes())
    archive.writestr('HANDOFF-MANIFEST.json',encoded)
with zipfile.ZipFile(target) as archive:
    for relative, expected in manifest['files_sha256'].items():
        assert hashlib.sha256(archive.read(relative)).hexdigest() == expected, relative
receipt = {'archive':str(target),'archive_sha256':sha(target),
           'checkpoint_sha256':sha(checkpoint_path),'files':len(files),
           'full_loader_verified':False}
(reports/(name+'.json')).write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(receipt))
