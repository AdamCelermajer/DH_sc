from pathlib import Path
import hashlib, json, struct, sys
sys.path.insert(0, r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM

root = Path(__file__).resolve().parents[3]
original = root / '.local-inputs/libDungeonHunter2.so'
out = root / 'port/level-loader/reports/root-loading-v50/reference'
out.mkdir(parents=True, exist_ok=True)
md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
with original.open('rb') as f:
    elf = ELFFile(f)
    segments = list(elf.iter_segments())
    syms = list(elf.get_section_by_name('.symtab').iter_symbols())
    def raw(a, n):
        p = next(p for p in segments if p['p_vaddr'] <= a and a+n <= p['p_vaddr']+p['p_filesz'])
        f.seek(p['p_offset']+a-p['p_vaddr'])
        return f.read(n)
    def word(a): return struct.unpack('<I', raw(a, 4))[0]
    addresses = {0x31f55c, 0x59f300, 0x5dad20, 0x5d9e94, 0x5da080, 0x5e828c, 0x5e9f78}
    rows = []
    for s in syms:
        if s.name.startswith('_ZTV') and 'Driver' in s.name and s['st_size'] >= 0xb8:
            at = s['st_value'] + 8 + 0xac
            target = word(at)
            addresses.add(target)
            target_sym = [t.name for t in syms if t['st_value'] == target and t['st_info']['type'] == 'STT_FUNC']
            rows.append({'vtable': s.name, 'address': hex(s['st_value']), 'slot': '0xac', 'word': hex(at), 'target': hex(target), 'symbols': target_sym})
    text = []
    for a in sorted(addresses):
        ss = [s for s in syms if s['st_value'] == a and s['st_info']['type'] == 'STT_FUNC' and s['st_size']]
        if not ss: continue
        s = max(ss, key=lambda s:s['st_size'])
        code = raw(a, s['st_size'])
        text.append('# '+hex(a)+' '+s.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(code,a)))
    (out/'clean-glitch-vtables.json').write_text(json.dumps({'original_sha256': hashlib.sha256(original.read_bytes()).hexdigest(), 'rows': rows}, indent=2))
    (out/'clean-glitch.asm').write_text('\n\n'.join(text)+'\n')
    print(json.dumps(rows, indent=2))
