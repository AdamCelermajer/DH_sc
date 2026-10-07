"""Print captured source routines storing the requested source field offset."""
from pathlib import Path
import sys
source=Path(__file__).resolve().parents[1]/'reference/character-world-target-owner-v1/original-functions.asm'
needle=sys.argv[1] if len(sys.argv)>1 else '#0x80]'
for routine in source.read_text().split('\n# '):
 if any(needle in line and ('strb' in line or 'str ' in line) for line in routine.splitlines()):
  print('# '+routine)
