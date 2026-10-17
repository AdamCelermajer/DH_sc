"""Replay Application::ComputeDt in the original ARM ELF and compare the host kernel."""
import argparse
import hashlib
import json
import math
import struct
import subprocess
import sys
from pathlib import Path

from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / '../game-data/tests'))
sys.path.insert(0, str(ROOT / 'tests'))
from navigation_differential import Cpu
from aggro_differential import float_bits
from combat_result_differential import floating


class ComputeDtCpu(Cpu):
    def external(self, uc, address, size, unused):
        name = self.imports.get(address)
        if name == '__aeabi_ui2f':
            value = float_bits(self.reg(0) & 0xffffffff)
            self.put(0, value)
        elif name in ('__aeabi_fmul', '__aeabi_fadd', '__aeabi_fsub'):
            a, b = floating(self.reg(0)), floating(self.reg(1))
            value = a * b if name.endswith('fmul') else a + b if name.endswith('fadd') else a - b
            self.put(0, float_bits(value))
        elif name == '__aeabi_f2iz':
            value = floating(self.reg(0))
            if math.isnan(value):
                result = -0x80000000
            elif value >= 2147483648.0:
                result = 0x7fffffff
            elif value <= -2147483648.0:
                result = -0x80000000
            else:
                result = math.trunc(value)
            self.put(0, result & 0xffffffff)
        else:
            return super().external(uc, address, size, unused)
        self.import_calls[name] = self.import_calls.get(name, 0) + 1
        uc.reg_write(self.pc, uc.reg_read(self.lr))

PRODUCER = ROOT / 'reference/character-ai-frame/producers/original-functions.json'
def fword(cpu, address):
    return struct.unpack('<I', cpu.uc.mem_read(address, 4))[0]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--host-test', type=Path, required=True)
    args = parser.parse_args()
    manifest = json.loads(PRODUCER.read_text())
    digest = hashlib.sha256(args.engine.read_bytes()).hexdigest()
    assert digest == manifest['original_sha256'], ('wrong source ELF', digest)

    cpu = ComputeDtCpu(args.engine, False, manifest)
    app = cpu.data + 0x1000
    current_now = 0
    enabled = False

    def hook(uc, address, size, _):
        if enabled and address == 0x60b0cc:
            cpu.put(0, current_now)
            uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))

    cpu.uc.hook_add(UC_HOOK_CODE, hook, begin=0x60b0cc, end=0x60b0cc)
    # Deterministic traces include uint32 timer wrap, multi-second gaps, scale
    # changes, fractional carry, zero dt and binary32 precision boundaries.
    rows = []
    traces = [
        (0, 1., 1., 0., (16, 17, 33, 1000, 2001, 2002)),
        (0, .5, .5, .25, (1, 2, 3, 4, 16, 33, 1000)),
        (0xfffffff0, 1., 1., 0., (0xfffffff8, 0x10, 0x20, 0x7fffffff)),
        (0x7ffffff0, 1.375, .625, .75, (0x7ffffff8, 0x80000010, 0x80001000)),
        (100, 4., .33333334, .875, (101, 200, 500, 10000)),
        (0, -.25, .5, .625, (1, 17, 1000)),
        (0, 1., 1., 0., (0x01000001, 0x01000002, 0x02000001)),
    ]
    for initial, scale_a, scale_b, fraction, times in traces:
        cpu.uc.mem_write(app, bytes(0x200))
        cpu.uc.mem_write(app + 0x88, struct.pack('<I', initial))
        cpu.uc.mem_write(app + 0x94, struct.pack('<f', fraction))
        cpu.uc.mem_write(app + 0x98, struct.pack('<f', scale_b))
        cpu.uc.mem_write(app + 0x9c, struct.pack('<f', scale_a))
        for now in times:
            current_now = now & 0xffffffff
            previous = fword(cpu, app + 0x88)
            previous_fraction = fword(cpu, app + 0x94)
            enabled = True
            cpu.invoke(0x320da4, [app])
            enabled = False
            rows.append((previous, current_now, fword(cpu, app + 0x9c),
                         fword(cpu, app + 0x98), previous_fraction,
                         fword(cpu, app + 0x8c), fword(cpu, app + 0x90),
                         fword(cpu, app + 0x94)))

    stdin = ''.join(f'{last:08x} {now:08x} {a:08x} {b:08x} {rem:08x}\n'
                    for last, now, a, b, rem, _, _, _ in rows)
    result = subprocess.run([str(args.host_test)], input=stdin, text=True,
                            capture_output=True, check=True)
    outputs = [line.split() for line in result.stdout.splitlines()]
    assert len(outputs) == len(rows), (len(outputs), len(rows), result.stderr)
    for index, (row, output) in enumerate(zip(rows, outputs)):
        status, dt, rem, scaled = int(output[0]), int(output[1]), int(output[2], 16), int(output[3])
        expected = (row[5], row[6], row[7])
        assert status == 0 and (dt, scaled, rem) == expected, (index, row, output, expected)
    print(f'PASS: source ELF Application::ComputeDt replay; {len(rows)} cases, '
          'uint32 wrap, long frames, both scales and fractional carry')


if __name__ == '__main__':
    main()
