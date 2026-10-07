from pathlib import Path
import sys
ref=Path(__file__).resolve().parents[1]/'reference/character-world-target-owner-v1/original-functions.asm'
for chunk in ref.read_text().split('\n# '):
 if not chunk.strip():continue
 name=chunk.splitlines()[0]
 if sys.argv[1]in name:print('# '+chunk)
