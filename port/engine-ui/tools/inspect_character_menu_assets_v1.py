from zipfile import ZipFile
from pathlib import Path
import struct
out=Path(__file__).resolve().parents[3]/'.local-inputs/character-menu-native-v1-fonts'
out.mkdir(exist_ok=True)
with ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 for n in z.namelist():
  if '/fonts_py' in n:
   raw=z.read(n);(out/Path(n).name).write_bytes(raw)
   print(n,len(raw),raw[:160])
