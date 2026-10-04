"""Original camera -> actual original root setters, with observed AS endpoint.

MenuFX/RenderFX receiver projection is explicit; bounds/viewport/point math is
never replaced. Other records retain the frozen original-instruction corpus.
"""
import argparse
import hashlib
import json
from pathlib import Path
import random
import struct
from viewport_differential import Machine, word, words

ROOT = Path(__file__).resolve().parents[1]


class Composed(Machine):
    def hook(self, uc, address, size, unused):
        if address in (0x7a9bac, 0x7a9b30):
            c = self.c
            values = [c.reg(i) for i in (1, 2, 3)] + [word(c, uc.reg_read(c.sp))]
            self.event(4 if address == 0x7a9bac else 5, values)
            c.put(0, self.s)
            uc.reg_write(c.pc, 0x775d38 if address == 0x7a9bac else 0x7755f4)
            return
        super().hook(uc, address, size, unused)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--engine', type=Path, required=True)
    p.add_argument('--prior-gold', type=Path, default=ROOT/'reference/viewport/viewport-gold.bin')
    p.add_argument('--gold', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    assert not a.gold.exists() and not a.report.exists(), 'Preserve existing source evidence'
    sha = lambda raw: hashlib.sha256(raw).hexdigest()
    original_sha = sha(a.engine.read_bytes())
    assert original_sha == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    original = Composed(a.engine, False, {'functions': []})
    rng = random.Random(20261004)
    records, counts, services = [], {}, 0
    prior = a.prior_gold.read_bytes()
    assert struct.unpack_from('<I', prior)[0] == 0x31505756
    at = 8
    for _ in range(struct.unpack_from('<I', prior, 4)[0]):
        il, ol, n = struct.unpack_from('<III', prior, at)
        at += 12
        record = prior[at:at+il+ol+n*40]
        at += len(record)
        op = struct.unpack_from('<I', record)[0]
        if op != 4:
            records.append(words(il, ol, n)+record)
            counts[str(op)] = counts.get(str(op), 0)+1
            services += n
    assert at == len(prior)
    for i in range(160):
        rect, vp = (0., 9600., 0., 6400.), (0, 0, 480, 320)
        bounds = (rng.randrange(-100, 100), rng.randrange(-100, 100), 480, 320)
        state = struct.pack('<4f8ifIQ', *rect, *vp, *bounds, 1., 0, 1)
        camera = struct.pack('<4fQ4i', -100., 640., -200., 800., i % 2,
                             rng.randrange(-100, 100), rng.randrange(-100, 100),
                             rng.randrange(-200, 200), rng.randrange(-200, 200))
        raw = (words(4)+state+camera+struct.pack('<4i', 0, 0, 480, 320)+bytes(8)
               +struct.pack('<4i', i % 4, i % 4, 854 if i % 2 else 1080, 480 if i % 2 else 1920)+words(0))
        output, events = original.run(raw)
        records.append(words(len(raw), len(output), len(events))+raw+output+b''.join(events))
        services += len(events)
        counts['4'] = counts.get('4', 0)+1
    gold = words(0x31505756, len(records))+b''.join(records)
    report = dict(validation='PASS', scope='Prior original root viewport gold plus newly executed actual original camera and both actual root setters; AS publication observed endpoint',
                  original_sha256=original_sha, prior_gold_sha256=sha(prior), gold_sha256=sha(gold),
                  records=len(records), operation_counts=counts, ordered_services=services,
                  new_original_composed_camera_cases=160)
    a.gold.parent.mkdir(parents=True, exist_ok=True)
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.gold.write_bytes(gold)
    a.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
