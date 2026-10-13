"""Stage a private portable CPython from the app bundle; never use PATH Python."""
from pathlib import Path
import shutil
import sys
import zipfile

dst = Path(__file__).resolve().parent / '.runtime'
src = Path(sys.executable).parent
dst.mkdir(exist_ok=True)
for pattern in ('python.exe', 'python3.dll', 'python312.dll', 'vcruntime*.dll', 'LICENSE.txt'):
    for p in src.glob(pattern):
        shutil.copy2(p, dst / p.name)
for p in (src / 'DLLs').glob('*'):
    if p.suffix in ('.dll', '.pyd'):
        shutil.copy2(p, dst / p.name)
with zipfile.ZipFile(dst / 'python312.zip', 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted((src / 'Lib').rglob('*')):
        rel = p.relative_to(src / 'Lib')
        if p.is_file() and 'site-packages' not in rel.parts and '__pycache__' not in rel.parts:
            z.write(p, rel.as_posix())
(dst / 'python312._pth').write_text('python312.zip\n.\n..\n', encoding='ascii')
print('Staged private Python under tools/atlas/.runtime (ignored by Git).')
