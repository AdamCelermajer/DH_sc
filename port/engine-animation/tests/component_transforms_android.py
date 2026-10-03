"""Replay original real-factory transform corpus in optimized ARM64 instructions."""
import argparse
import json
import struct
import time
from pathlib import Path
from compiled_transforms_differential import sha, word, words
from angle_interpreter_differential import AngleCpu

ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parents[1]


class Reader:
    def __init__(self,path):
        self.raw=path.read_bytes()
        self.at=0

    def read(self,n):
        assert self.at+n<=len(self.raw)
        value=self.raw[self.at:self.at+n]
        self.at+=n
        return value

    def word(self):
        return word(self.read(4),0)

    def block(self):
        return self.read(self.word())


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library',type=Path,required=True)
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    if args.report.exists():
        raise RuntimeError('Refusing to overwrite captured ARM64 proof')
    started=time.monotonic()
    corpus=ROOT/'reference/component-transforms/actual-raw-factories/original-corpus.bin'
    capture=corpus.with_name('original-capture.json')
    original=json.loads(capture.read_bytes())
    assert sha(corpus)==original['corpus_sha256']=='3b8d92629f60c7bf586c1404b272e1e2f9eb65f7a9a4f541aa87c3f29a0d8d19'
    source_paths=[p for module in ('engine-animation','scene-materials','engine-resources','asset-payloads','engine-math')
                  for p in (REPO/'port'/module).glob('*') if p.suffix in ('.cpp','.hpp') or p.name=='CMakeLists.txt']
    source_paths += [Path(__file__),ROOT/'tests/component_transforms.cpp',ROOT/'tests/dynamic_compiled_transforms.cpp',ROOT/'tests/angle_interpreter_differential.py']
    snapshot=lambda:{str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in sorted(source_paths)}
    before=snapshot()
    r=Reader(corpus)
    assert r.word()==0x31544344
    model=r.block()
    count=r.word()
    ids,raws=[],[]
    for i in range(count):
        ids.append(r.word())
        raws.append(r.block())
    cases=r.word()
    counts=dict(targets=0,bindings=0,samples=0,libm=0)
    for case in range(cases):
        mismatch,defaults,nselected=r.word(),r.word(),r.word()
        selected=[r.word() for i in range(nselected)]
        cpu=AngleCpu(args.library,True,{'functions':[]})
        ptrs,sizes,native_ids,selection,model_address,scratch=[cpu.data+x for x in (0x2000,0x3000,0x4000,0x5000,0x10000,0xe000)]
        cpu.uc.mem_write(model_address,model)
        for i,raw in enumerate(raws):
            address=cpu.data+0x100000+i*0x20000
            cpu.uc.mem_write(address,raw)
            cpu.pointer(ptrs+i*8,address)
            cpu.uc.mem_write(sizes+i*4,words([len(raw)]))
            cpu.uc.mem_write(native_ids+i*4,words([ids[i]]))
        cpu.uc.mem_write(selection,words(selected))
        fixture=cpu.invoke('dh2_dynamic_transform_create',[model_address,len(model),count,ptrs,sizes,native_ids,selection,nselected,defaults,mismatch],budget=30000000)
        assert fixture,case
        ntargets=r.word()
        assert cpu.invoke('dh2_dynamic_transform_count',[fixture])==ntargets
        for ti in range(ntargets):
            uri=r.block()
            expected=r.read(12)
            assert cpu.invoke('dh2_dynamic_transform_target',[fixture,ti,scratch,scratch+0x100,4096])==1
            assert bytes(cpu.uc.mem_read(scratch,12))==expected and cpu.string(scratch+0x100)==uri
        for ci in range(nselected):
            expected=r.read(12)
            assert cpu.invoke('dh2_dynamic_transform_clip',[fixture,ci,scratch])==1
            assert bytes(cpu.uc.mem_read(scratch,12))==expected
            for ti in range(ntargets):
                expected=r.read(24)
                assert cpu.invoke('dh2_dynamic_transform_binding',[fixture,ci,ti,scratch])==1
                assert bytes(cpu.uc.mem_read(scratch,24))==expected,(case,ci,ti,'binding')
                counts['bindings']+=1
        nsamples=r.word()
        for i in range(nsamples):
            ci,ti,ms,interpolate,prior=[r.word() for j in range(5)]
            initial=r.read(16)
            key=r.word()
            expected=r.read(16)
            trace=[tuple(struct.unpack('<III',r.read(12))) for j in range(r.word())]
            cpu.uc.mem_write(scratch,initial)
            cpu.uc.mem_write(scratch+0x20,words([prior]))
            cpu.trig=[]
            assert cpu.invoke('dh2_dynamic_transform_sample',[fixture,ci,ti,ms,scratch,4,scratch+0x20,interpolate])==1
            actual=bytes(cpu.uc.mem_read(scratch,16))
            assert actual==expected and word(bytes(cpu.uc.mem_read(scratch+0x20,4)),0)==key,(case,i,ci,ti,ms,'sample',actual.hex(),expected.hex())
            assert [x for x in trace if x[0]!=2]==[x for x in cpu.trig if x[0]!=2],(case,i,'libm')
            counts['samples']+=1
            counts['libm']+=len(trace)
        cpu.invoke('dh2_dynamic_transform_destroy',[fixture])
        counts['targets']+=ntargets
        print(json.dumps(dict(case=case,targets=ntargets,samples=nsamples)),flush=True)
    assert r.at==len(r.raw) and before==snapshot()
    report=dict(validation='PASS',cases=cases,**counts,mismatches=0,source_sha256=before,
                arm64_library_sha256=sha(args.library),original_capture_sha256=sha(capture),gold_sha256=sha(corpus),
                original_sha256=original['original_sha256'],elapsed_seconds=round(time.monotonic()-started,2),
                scope='Actual optimized ARM64 compiler/binding/cursor/sampling replay of real original factories. '
                      'Original corpus supplies resource/post-load assignment/singleton services and selected segments. '
                      'Shared UCRT float libm contracts; no historical Bionic/GPU/renderer/APK/device claim.')
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','scope')}))


if __name__=='__main__':
    main()
