"""Replay original routing observations through compiled ARM64 instructions.

AI consumers/FSM remain explicit synchronous services. This proves routing,
gate decisions and preserved event/payload identity, not full AI behavior.
"""
import argparse, hashlib, json, struct, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tests'))
from body_transform_differential import Cpu

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    if args.report.exists():
        raise RuntimeError('Refusing to overwrite native routing evidence')
    reference = ROOT / 'reference/animation-event-routing/native-reference'
    original = json.loads((reference / 'probe.json').read_bytes())
    assert original['validation'] == 'PASS' and original['native_comparisons'] == 0
    assert original['script_sha256'] == sha(ROOT / 'tests/animation_event_routing_discovery.py')
    assert original['manifest_sha256'] == sha(reference / 'original-functions.json')
    assert original['original_sha256'] == sha(ROOT / '../../.local-inputs/libDungeonHunter2.so')
    raw = (reference / 'routing-reference.bin').read_bytes()
    assert hashlib.sha256(raw).hexdigest() == original['reference_sha256']
    cpu = Cpu(args.library, True, {'functions': []})
    facts, services = cpu.data + 0x1000, cpu.data + 0x2000
    context = 0xf123456789abcdef
    callbacks, event, payload, state, accepted = [], 0, 0, 0, 0

    def callback(uc, address, size, unused):
        assert cpu.reg(0) == context
        kind, actual_event, actual_payload = struct.unpack('<IIQ', uc.mem_read(cpu.reg(1), 16))
        assert (actual_event, actual_payload) == (event, payload)
        callbacks.append(kind)
        cpu.put(0, (state if kind == 1 else accepted) & 0xffffffff)
        uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))

    cpu.imports[cpu.callback + 32] = 'body_callback'
    cpu.body_callback = callback
    cpu.uc.mem_write(services, struct.pack('<QQ', context, cpu.callback + 32))
    magic, count = struct.unpack_from('<II', raw)
    assert magic == 0x31524541 and count == original['cases']
    offset, total_callbacks = 8, 0
    for index in range(count):
        event, state, locked, blocked, forced, accepted, old_payload, length = struct.unpack_from('<8I', raw, offset)
        offset += 32
        expected = list(struct.unpack_from('<' + 'I' * length, raw, offset))
        offset += length * 4
        payload = 0x100000000 + old_payload if old_payload else 0
        cpu.uc.mem_write(facts, struct.pack('<4IQ', event, blocked, locked, forced, payload))
        callbacks.clear()
        assert cpu.invoke('dh2_character_animation_event_route', [facts, services]) == 1
        assert callbacks == expected, (index, callbacks, expected)
        total_callbacks += len(callbacks)
    assert offset == len(raw)
    callbacks.clear()
    rejected = 0
    guards = [(0, services), (facts, 0)]
    cpu.uc.mem_write(services + 32, struct.pack('<QQ', context, 0))
    guards.append((facts, services + 32))
    for f, s in guards:
        assert cpu.invoke('dh2_character_animation_event_route', [f, s]) & 0xffffffff == 0xffffffff
        assert not callbacks
        rejected += 1
    for fields in ((0x21, 0, 0, 0), (0x28, 0, 0, 0), (0x27, 2, 0, 0),
                   (0x27, 0, 2, 0), (0x27, 0, 0, 2)):
        cpu.uc.mem_write(facts, struct.pack('<4IQ', *fields, 0))
        assert cpu.invoke('dh2_character_animation_event_route', [facts, services]) & 0xffffffff == 0xffffffff
        assert not callbacks
        rejected += 1
    sources = [ROOT / name for name in ('character_animation_events.cpp', 'character_animation_events.hpp',
               'tests/character_animation_events.cpp', 'tests/animation_event_routing_discovery.py')]
    sources.append(Path(__file__))
    report = dict(validation='PASS', actual_compiled_arm64_instructions_executed=True,
                  original_sha256=original['original_sha256'], reference_sha256=original['reference_sha256'],
                  original_probe_sha256=sha(reference / 'probe.json'), library_sha256=sha(args.library),
                  source_sha256={str(p.relative_to(ROOT)).replace('\\', '/'): sha(p) for p in sources},
                  comparisons=count, ordered_callbacks=total_callbacks, mismatches=0,
                  atomic_rejections=rejected, payload_and_context_above_4gib=True,
                  imported_services=cpu.import_calls, scope=__doc__)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))

if __name__ == '__main__':
    main()
