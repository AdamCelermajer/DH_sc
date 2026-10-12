"""Record which arithmetic primitives are undefined original ELF dependencies."""
import hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];SOURCE=ROOT/'.local-inputs/libDungeonHunter2.so'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
expected={'__aeabi_f2iz','__aeabi_idiv','__aeabi_i2f','__aeabi_ui2f','__aeabi_fmul'}
with SOURCE.open('rb') as stream:
    elf=ELFFile(stream);rows=[dict(name=s.name,address=s['st_value'],size=s['st_size'],section=s['st_shndx']) for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name in expected]
assert {r['name'] for r in rows}==expected and all(r['section']=='SHN_UNDEF' for r in rows)
(HERE/'undefined-arithmetic-imports.json').write_text(json.dumps(dict(original_sha256=sha(SOURCE),script_sha256=sha(Path(__file__)),imports=rows),indent=2)+'\n')
print(json.dumps(dict(undefined_imports=len(rows),original_sha256=sha(SOURCE))))
