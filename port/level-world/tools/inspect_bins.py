"""Print bounded raw bytes/string evidence without guessing a table schema."""
import argparse,re,struct
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('files',nargs='+',type=Path);a=p.parse_args()
for file in a.files:
 raw=file.read_bytes();print(file.name,len(raw));print('first bytes',raw[:192].hex(' '));print('first words',struct.unpack_from('<'+str(min(16,len(raw)//4))+'I',raw))
 strings=[m.group().decode('ascii') for m in re.finditer(rb'[\x20-\x7e]{4,}',raw)]
 print('strings',strings[:22]);print('selected',[s for s in strings if re.search(r'crypt|skeleton|slime|model|animation',s,re.I)][:30])
