"""Whole original ItemAudioVisualTable4ba648 reader on genuine29-row cache."""
import sys,hashlib,json
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from loot_entry_selection_v8_original import EntryOriginal,W
old=EntryOriginal();cache=R/'.local-inputs/loot-world-v8/cache';old.blob=(cache/'loot_audiovisual_pyarray.bin').read_bytes();old.cursor=0;old.invoke(0x4ba648,[old.stream],budget=10000000);assert old.cursor==len(old.blob)
count=old.word(0x9a65f0);rows=old.word(0x9a65f4);out=b'LAV8'+W(count)
for i in range(count):
 p=rows+i*20;size=old.word(p+12);text=bytes(old.uc.mem_read(old.word(p+16),size));out+=W(old.word(p+4),old.word(p+8),size)+text
ref=R/'port/game-data/reference/loot-audiovisual-v8';ref.mkdir(parents=True,exist_ok=True);(ref/'fixtures.bin').write_bytes(out)
report=dict(validation='PASS',original_rows=count,original_sha256=hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(out).hexdigest(),input_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in cache.glob('*.bin')},scope=__doc__)
(ref/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
