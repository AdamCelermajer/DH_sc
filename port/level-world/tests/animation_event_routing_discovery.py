"""Execute original Character/CharAI animation-event routing, not native parity.

Controller/global gates and state metadata are explicit inputs. State-specific
Attack/Skill/Move consumers, OnEndOfAnim virtual and FSM are service fixtures.
The original dispatch, simple sequence consumers and state getters execute.
"""
import argparse, hashlib, itertools, json, struct, sys
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tests'))
from body_transform_differential import Cpu

ADDRESSES = (0x3a4d5c, 0x3cbb34, 0x3c01ac, 0x3d3aec, 0x3d3ae4,
             0x3d3d30, 0x3d3d4c, 0x3d3ff8, 0x3d4204,
             0x3d3e44, 0x3d3d68, 0x3d4044, 0x3d3dd4, 0x3d4120)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    if (args.output / 'probe.json').exists():
        raise RuntimeError('Refusing to replace original routing observations')
    binary = args.engine.read_bytes()
    assert sha(binary) == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    functions, disassembly = [], []
    with args.engine.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = list(elf.get_section_by_name('.symtab').iter_symbols())
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for address in ADDRESSES:
            symbol = next(s for s in symbols if s['st_value'] == address and s['st_size'])
            segment = next(s for s in segments if s['p_vaddr'] <= address < s['p_vaddr'] + s['p_filesz'])
            stream.seek(segment['p_offset'] + address - segment['p_vaddr'])
            raw = stream.read(symbol['st_size'])
            functions.append(dict(original_symbol=symbol.name, elf_address=hex(address),
                                  size=len(raw), sha256=sha(raw)))
            disassembly.append('\n# ' + symbol.name)
            disassembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}'
                               for i in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, address))
    manifest = dict(original_sha256=sha(binary), functions=functions)
    (args.output / 'original-functions.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (args.output / 'original-functions.asm').write_text('\n'.join(disassembly) + '\n')
    cpu = Cpu(args.engine, False, manifest)
    character = cpu.data + 0x1000
    ai = character + 0x3c8
    controller = cpu.data + 0x4000
    ai_vtable = cpu.data + 0x5000
    state_info = cpu.data + 0x6000
    virtual_end = cpu.data + 0x7000
    cpu.uc.mem_write(virtual_end, bytes.fromhex('1eff2fe1'))
    trace, records = [], []
    blocked = accepted = event = payload = 0
    handlers = {0x3d3e44: 'attack-end', 0x3d3d68: 'skill-end',
                0x3d4044: 'attack-begin', 0x3d3dd4: 'skill-begin',
                0x3d4120: 'move-begin'}

    def returned(value=0):
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    def hook(uc, address, size, unused):
        if address == 0x3cbd18:
            uc.mem_write(cpu.reg(3), bytes((blocked,)))
        elif address in handlers:
            assert cpu.reg(0) == ai
            trace.append(handlers[address])
            returned(accepted)
        elif address == virtual_end:
            assert cpu.reg(0) == ai
            trace.append('end-virtual')
            returned(accepted)  # This virtual result must not veto FSM forwarding.
        elif address == 0x3c5684:
            assert (cpu.reg(0), cpu.reg(1), cpu.reg(2)) == (character + 0x4fc, event, payload)
            trace.append('state-event')
            returned()
        elif address == 0x3c01ac:
            trace.append('state-getter')  # Actual original getter executes.

    cpu.uc.hook_add(UC_HOOK_CODE, hook)
    states = (-1, 3, 4, 5, 6, 7, 12)
    for event, state, locked, blocked, forced, accepted, payload in itertools.product(
            range(0x22, 0x28), states, (0, 1), (0, 1), (0, 1), (0, 1), (0, 0xf1234567)):
        cpu.uc.mem_write(character, bytes(0x1800))
        cpu.uc.mem_write(ai_vtable, bytes(0x100))
        cpu.pointer(ai, ai_vtable)
        cpu.pointer(ai + 4, character)
        cpu.pointer(ai_vtable + 0x98, virtual_end)
        cpu.pointer(character + 0x378, controller)
        cpu.uc.mem_write(controller + 8, bytes((locked, forced)))
        cpu.pointer(character + 0x51c, state_info)
        cpu.pointer(state_info, state & 0xffffffff)
        trace.clear()
        cpu.invoke(0x3a4d5c, [character, event, payload])
        gated = not forced and bool(locked or blocked)
        # 22/23 take their source direct switch branch, preceding generic gates.
        if event in (0x22, 0x23):
            expected = ['end-virtual', 'state-event']
        elif gated:
            expected = ['state-event']
        elif event in (0x24, 0x25):
            expected = ['state-getter', 'state-event']
        else:
            consumer = None
            if event == 0x27:
                consumer = 'attack-end' if state == 5 else 'skill-end' if state in (6, 7) else None
            else:
                consumer = {4: 'move-begin', 5: 'attack-begin', 6: 'skill-begin', 7: 'skill-begin'}.get(state)
            expected = ['state-getter'] + ([consumer] if consumer else [])
            if not consumer or accepted:
                expected.append('state-event')
        assert trace == expected, (event, state, locked, blocked, forced, accepted, trace, expected)
        records.append(dict(event=event, state=state, locked=locked, blocked=blocked,
                            forced=forced, service_return=accepted, payload=payload, trace=trace.copy()))
    report = dict(validation='PASS', original_instructions_executed=True,
                  original_sha256=sha(binary), script_sha256=sha(Path(__file__).read_bytes()),
                  manifest_sha256=sha((args.output / 'original-functions.json').read_bytes()),
                  cases=len(records), mismatches=0, native_comparisons=0,
                  scope=__doc__, records=records)
    services = ('end-virtual', 'state-getter', 'attack-end', 'skill-end',
                'attack-begin', 'skill-begin', 'move-begin', 'state-event')
    reference = struct.pack('<II', 0x31524541, len(records))
    for record in records:
        reference += struct.pack('<8I', record['event'], record['state'] & 0xffffffff,
                                 record['locked'], record['blocked'], record['forced'],
                                 record['service_return'], record['payload'], len(record['trace']))
        reference += b''.join(struct.pack('<I', services.index(name)) for name in record['trace'])
    (args.output / 'routing-reference.bin').write_bytes(reference)
    report['reference_sha256'] = sha(reference)
    (args.output / 'probe.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'records'}))

if __name__ == '__main__':
    main()
