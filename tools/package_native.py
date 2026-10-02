#!/usr/bin/env python3
"""Package large exact assembly and ELF inventories with deterministic headers."""
import argparse
import gzip
import hashlib
import json
import tarfile
from pathlib import Path


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo-root', type=Path, default=Path(__file__).resolve().parents[1])
    a = p.parse_args()
    root = a.repo_root.resolve()
    output = root / 'recovered/native/bundles'
    output.mkdir(parents=True, exist_ok=True)
    bundles = []
    for category in ['assembly', 'symbols']:
        paths = sorted(x for x in (root / 'recovered/native' / category).rglob('*') if x.is_file())
        target = output / (category + '.tar.gz')
        with target.open('wb') as raw, gzip.GzipFile(filename='', mode='wb', fileobj=raw, mtime=0, compresslevel=6) as gz:
            with tarfile.open(fileobj=gz, mode='w') as tar:
                for path in paths:
                    info = tar.gettarinfo(str(path), arcname=path.relative_to(root).as_posix())
                    info.uid = info.gid = 0
                    info.uname = info.gname = ''
                    info.mtime = 0
                    info.mode = 0o644
                    with path.open('rb') as f: tar.addfile(info, f)
        row = {'path': target.relative_to(root).as_posix(), 'files': len(paths),
               'uncompressed_file_bytes': sum(x.stat().st_size for x in paths),
               'archive_bytes': target.stat().st_size, 'sha256': hashlib.sha256(target.read_bytes()).hexdigest()}
        bundles.append(row)
        print(f'{target.name}: {row["files"]} files, {row["archive_bytes"]} bytes')
    (output / 'manifest.json').write_text(json.dumps({'schema_version': 1, 'bundles': bundles}, indent=2) + '\n')


if __name__ == '__main__':
    main()
