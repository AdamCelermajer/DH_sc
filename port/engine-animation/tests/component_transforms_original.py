"""Original-only dynamic union/default/typed-sampling corpus for real component clips.

Explicit fixtures: immutable caller-selected resources, C++ singleton guards,
preallocated vectors and selected segment. Compilation, binding, defaults,
cursor search and typed values execute the original ARM32 instructions.
"""
import argparse
import ctypes
import json
import struct
import time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from compiled_transforms_differential import Cpu, block, database, i32, nodes, relocate, sha, u32, word, words
from animation_blend_differential import bits, floating

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
COMPONENT_IDS = [1029, 1028, 1027, 984, 1022, 1009, 1014, 1077, 1078, 1079]


class AngleCpu(Cpu):
    def __init__(self,*args):
        super().__init__(*args)
        self.cosf = self.crt.cosf
        self.cosf.argtypes = [ctypes.c_float]
        self.cosf.restype = ctypes.c_float

    def external(self,uc,address,size,user):
        if self.imports.get(address) != 'cosf':
            return super().external(uc,address,size,user)
        value = self.reg(0)
        output = bits(self.cosf(floating(value)))
        self.trig.append((3,value,output))
        self.put(0,output)
        self.import_calls['cosf'] = self.import_calls.get('cosf',0)+1
        uc.reg_write(self.pc,uc.reg_read(self.lr))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--engine', type=Path, default=REPO / '.local-inputs/libDungeonHunter2.so')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    if (args.output / 'original-corpus.bin').exists():
        raise RuntimeError('Refusing to overwrite captured original corpus')
    started = time.monotonic()
    manifest_path = ROOT / 'reference/component-transforms/original-functions.json'
    manifest = json.loads(manifest_path.read_bytes())
    assert sha(args.engine) == manifest['original_sha256']
    assets = REPO / 'port/android-native/app/src/main/assets'
    bank_path = assets / 'data/prince-animation-bank.json'
    bank = json.loads(bank_path.read_bytes())
    resources = {row['clip_id']: row for row in bank['resources']}
    ids = [1111] + COMPONENT_IDS
    raws = [(assets / resources[cid]['asset']).read_bytes() for cid in ids]
    for cid, raw in zip(ids, raws):
        import hashlib
        assert hashlib.sha256(raw).hexdigest() == resources[cid]['sha256']
    model = (assets / 'models/prince_modular.bdae').read_bytes()
    authored = nodes(model)
    component_uris = set()
    for raw in raws:
        root = word(raw, 32)
        for index in range(word(raw, root+36)):
            record = word(raw, root+40) + index*32
            channel = word(raw, record+16)
            if word(raw, channel+8) in (2, 3, 4, 7, 8, 9):
                name = word(raw, channel+4)
                component_uris.add(raw[name:raw.index(0, name)].decode())
    cases = [(1, 0, list(range(len(ids)))), (1, 0, list(range(1, len(ids)))+[0]),
             (1, -1, list(range(1, len(ids))))]
    gold = words([0x31544344]) + block(model) + words([len(ids)])
    gold += b''.join(words([cid])+block(raw) for cid, raw in zip(ids, raws)) + words([len(cases)])
    summaries = []
    for case, (mismatch, default_index, order) in enumerate(cases):
        old = AngleCpu(args.engine, False, manifest)
        images, dbs = [], []
        for ci, raw in enumerate(raws):
            image = old.data+0x100000+ci*0x20000
            images.append(image)
            dbs.append(database(old, relocate(old, raw, image), old.data+0x2000+ci*0x400))
        old.uc.mem_write(0x9f7110, words([1]))
        old.pointer(0x9f7570, old.invoke(0x670a60, []))
        dyn, animator = old.data+0xc000, old.data+0xc200
        scratch, cursors, factory = old.data+0xe000, dyn+0x400, old.data+0x700000
        factory_count, selected_data = 0, 0
        singleton_services = {}

        def ret(value=0):
            old.put(0, value)
            old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))

        def services(uc, address, size, user):
            nonlocal factory_count
            if address == 0x611ae0:
                factory_count += 1
            elif address == 0x65f364:
                ret(selected_data)
            elif address in (0x30e76c,0x30ea3c,0x30e304):
                singleton_services[hex(address)] = singleton_services.get(hex(address),0)+1
                if address == 0x30ea3c:
                    uc.mem_write(old.reg(0),words([1]))
                ret(1 if address == 0x30e76c else 0)

        old.uc.hook_add(UC_HOOK_CODE, services)
        # Populate raw SAnimation runtime interpreters separately from the
        # union's first-seen extended track. Actual factory instructions choose
        # each class; the raw resource initialization assignment is a fixture.
        raw_factories = []
        for ci,raw in enumerate(raws):
            root = word(raw,32)
            for ai in range(word(raw,root+36)):
                offset = word(raw,root+40)+ai*32
                track = images[ci]+offset
                instance = old.invoke(0x611ae0,[track])
                assert instance
                old.pointer(track+20,instance)
                vtable = word(bytes(old.uc.mem_read(instance,4)),0)
                raw_factories.append(dict(clip_id=ids[ci],animation=ai,
                    type=word(raw,word(raw,offset+16)+8),vtable=hex(vtable)))
        old.uc.mem_write(dyn, bytes(0x100))
        old.invoke(0x3648c4, [dyn])
        old.invoke(0x62db98, [dyn, mismatch])
        base = old.data+0x500000
        for field, stride, capacity in ((0x18,4,256),(0x24,8,32),(0x30,12,4096),
                                      (0x40,4,32),(0x4c,4,32),(0x58,4,32),(0x74,16,256)):
            old.uc.mem_write(base, bytes(stride*capacity))
            old.pointer(dyn+field, base)
            old.pointer(dyn+field+4, base)
            old.pointer(dyn+field+8, base+stride*capacity)
            base += 0x20000
        for ci in order:
            old.invoke(0x62e8a8, [dyn, dbs[ci]])
        if default_index != -1:
            old.invoke(0x62fc90, [dyn, dbs[default_index]])
        old.invoke(0x62f61c, [dyn], budget=3000000)
        read = lambda pointer: word(bytes(old.uc.mem_read(pointer, 4)), 0)
        n, channels, bindingbase, extended = read(dyn+0x3c), read(dyn+0x74), read(dyn+0x30), read(dyn+0x18)
        gold += words([mismatch,u32(default_index),len(order)])+words(order)+words([n])
        targets, allbindings, notes = [], [], []
        for ti in range(n):
            channel = channels+ti*16
            uri, kind = old.string(read(channel+4)).decode(), read(channel+8)
            instance = read(extended+ti*4)
            width = old.invoke(read(read(instance)+8),[instance])//4
            node = next((i for i,(name,_) in enumerate(authored) if name == uri), 0xffffffff)
            targets.append((uri,kind,width))
            gold += block(uri.encode())+words([kind,node,width])
        for clip, ci in enumerate(order):
            gold += words([ids[ci], old.invoke(0x65f07c,[dyn,clip]), old.invoke(0x65f098,[dyn,clip])])
            bindings = []
            for ti, (uri, kind, width) in enumerate(targets):
                mode, dp, track = struct.unpack('<III', old.uc.mem_read(bindingbase+(clip*n+ti)*12,12))
                value = bytes(old.uc.mem_read(dp,width*4)) if dp else bytes(width*4)
                gold += words([mode,int(bool(dp))])+value+bytes(16-len(value))
                bindings.append((mode,dp,track))
                if track and uri in component_uris:
                    source_kind = read(read(track+16)+8)
                    if source_kind != kind:
                        notes.append(dict(clip_id=ids[ci],uri=uri,union_type=kind,accessor_type=source_kind))
            allbindings.append(bindings)
        old.uc.mem_write(animator, bytes(0x100))
        old.pointer(animator+0x24,dyn)
        old.pointer(animator+0x40,cursors)
        records = []
        for clip,ci in enumerate(order):
            old.uc.mem_write(animator+0x4c, words([clip*n,clip]))
            raw = raws[ci]
            lib = word(raw, word(raw,32)+48)
            segmentbase = word(raw,lib+4)
            ranges = [(i32(word(raw,segmentbase+i*24)),i32(word(raw,segmentbase+i*24+4))) for i in range(word(raw,lib))]
            queries = sorted(set([0,33,199]+[x for pair in ranges for x in pair]))
            for ti,(uri,kind,width) in enumerate(targets):
                if uri not in component_uris and uri != 'Bip01-node':
                    continue
                mode,dp,track = allbindings[clip][ti]
                for interpolate in (0,1):
                    for ms in queries:
                        segment = 0
                        while segment+1<len(ranges) and ms>=ranges[segment][1]:
                            segment += 1
                        seg = segmentbase+segment*24
                        data_offset = word(raw,seg+12 if word(raw,seg+8)==0 else seg+20)
                        selected_data = images[ci]+data_offset
                        for index in range(word(raw,data_offset)):
                            slot = data_offset+8+index*8
                            old.pointer(images[ci]+slot,images[ci]+slot+i32(word(raw,slot)))
                        for prior in (0,7):
                            initial = words([0x80000000,0x7fc05678,0x7f800000,0x3f800000])
                            old.uc.mem_write(scratch,initial)
                            old.uc.mem_write(cursors+ti*4,words([prior]))
                            old.uc.mem_write(animator+12,words([0 if interpolate else 1]))
                            old.trig = []
                            old.invoke(0x65f7b4,[animator,ti,u32(ms),scratch])
                            expected = bytes(old.uc.mem_read(scratch,16))
                            key = read(cursors+ti*4)
                            records.append(words([clip,ti,u32(ms),interpolate,prior])+initial+words([key])+expected+
                                           words([len(old.trig)])+b''.join(words(x) for x in old.trig))
        gold += words([len(records)])+b''.join(records)
        summaries.append(dict(order=[ids[i] for i in order],targets=n,samples=len(records),
                              cross_type_bindings=notes,original_import_calls=old.import_calls,
                              actual_factory_calls=factory_count,singleton_services=singleton_services,
                              raw_factory_results=raw_factories))
        print(json.dumps({'case':case,'targets':n,'samples':len(records),'cross_type_bindings':len(notes)}),flush=True)
    corpus = args.output / 'original-corpus.bin'
    corpus.write_bytes(gold)
    report = dict(validation='ORIGINAL_CAPTURED',original_sha256=sha(args.engine),manifest_sha256=sha(manifest_path),
                  script_sha256=sha(Path(__file__)),bank_sha256=sha(bank_path),corpus_sha256=sha(corpus),
                  component_uris=sorted(component_uris),cases=summaries,elapsed_seconds=round(time.monotonic()-started,2),
                  scope='Actual original dynamic compiler and sampling. No native parity or live renderer claim.')
    (args.output / 'original-capture.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({key:value for key,value in report.items() if key not in ('cases','component_uris')}))


if __name__ == '__main__':
    main()
