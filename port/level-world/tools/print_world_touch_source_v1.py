from pathlib import Path
import sys
p=Path(__file__).resolve().parents[1]/'reference/world-touch-target-v1/original-source.asm'
text=p.read_text()
for address in sys.argv[1:]:
 start=text.index('# '+address+' ');end=text.find('\n# ',start+2);print(text[start:end if end>=0 else len(text)])
