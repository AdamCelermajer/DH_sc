"""Read-only original Stage1 callback inventory and direct native dependencies."""
import hashlib,json,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
ENGINE=ROOT/'.local-inputs/libDungeonHunter2.so'
REG=ROOT/'port/script-runtime/reference/game-bindings/registration-names.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
rows=[r for r in json.loads(REG.read_text())['bindings'] if r['caller']=='0x37b5a0']
assert len(rows)==33
subprocess.run([sys.executable,str(ROOT/'port/level-world/tools/capture_reference.py'),str(ENGINE),
 *[r['callback'] for r in rows],'--output',str(HERE/'catalog')],check=True)
with ENGINE.open('rb') as f:
 elf=ELFFile(f);segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 names={s['st_value']:s.name for s in symbols if s['st_size']}
 def raw(a,n):
  seg=next(s for s in segments if s['p_vaddr']<=a<s['p_vaddr']+s['p_filesz'])
  f.seek(seg['p_offset']+a-seg['p_vaddr']);return f.read(n)
 for row in rows:
  a=int(row['callback'],16);sym=next(s for s in symbols if s['st_value']==a and s['st_size'])
  row['symbol']=sym.name;row['size']=sym['st_size'];row['sha256']=hashlib.sha256(raw(a,sym['st_size'])).hexdigest()
  calls=[]
  for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw(a,sym['st_size']),a):
   if i.mnemonic in ('bl','b') and i.op_str.startswith('#0x'):
    target=int(i.op_str[1:],16)
    if not(a<=target<a+sym['st_size']):calls.append(dict(instruction=hex(i.address),target=hex(target),symbol=names.get(target)))
  row['direct_calls']=calls
 setters=[dict(symbol=s.name,address=hex(s['st_value']),size=s['st_size']) for s in symbols if s['st_size'] and any(t in s.name for t in ('SetBool','GetBool','SetString','GetString')) and any(t in s.name for t in ('LuaScript','LuaManager','GameObject','Character','script'))]
report=dict(original_sha256=sha(ENGINE),registration_evidence=str(REG.relative_to(ROOT)).replace('\\','/'),registration_evidence_sha256=sha(REG),script_sha256=sha(Path(__file__)),base_globals=33,callbacks=rows,other_typed_owner_symbols=setters,scope='Captured original registrations/direct-call dependencies; indirect services and deeper implementations are not inferred as accepted globals.')
(HERE/'catalog.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(base_globals=33,other_typed_owner_symbols=len(setters))))
