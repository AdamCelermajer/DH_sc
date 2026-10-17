"""Compare composed CanUpdate/culling against their nested original ARM calls."""
from __future__ import annotations
import argparse
import hashlib
import json
import random
import struct
import subprocess
import sys
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
sys.path.insert(0, str(MODULE / "tests"))
from run_object_update_culling_host import OriginalCpu, bits, verify_evidence
from unicorn import UC_HOOK_CODE

BASE = 0x02000000
CHARACTER, VISUAL, NODE, ALT_NODE = [BASE+x for x in (0x1000,0x7000,0x8000,0x9000)]
ONLINE, PLAYER, MANAGER, LEVEL = [BASE+x for x in (0xa000,0xb000,0xc000,0xd000)]
CAMERA, CAMERA_ROOT, FRUSTUM = [BASE+x for x in (0xe000,0xf000,0x10000)]
VTABLE, CAMERA_VTABLE, DEAD_STUB, FRUSTUM_STUB = [BASE+x for x in (0x11000,0x12000,0x13000,0x13010)]
SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"

def fixtures():
    rows = []
    def add(name, **changes):
        row = dict(phase=1,online=0,remote_word=0xffffffff,remote_byte=0,
                   local=0x7002,dead=0,respawn=0,visible=1,node_cull=1,
                   gate=0,level=1,mutation=0,aabb=[bits(-1.)]*3+[bits(1.)]*3,
                   planes=[0]*24)
        row.update(changes)
        rows.append((name,row))
    add("inside")
    add("outside",planes=[0,0,0,bits(1.)]*6)
    add("outside_gate_bypasses",gate=1,planes=[0,0,0,bits(1.)]*6)
    add("null_level",level=0)
    for phase in (0,2,3,128,255): add(f"phase_{phase}",phase=phase)
    add("local_player",local=0x7001)
    add("node_culling_disabled",node_cull=0)
    add("hidden_dead",visible=0,dead=0x80000000)
    add("hidden_dead_respawns",visible=0,dead=1,respawn=0x80000000)
    add("online_remote",online=255,remote_byte=255,dead=1,visible=0)
    add("online_nonremote",online=128,remote_byte=0)
    add("online_nonremote_outside",online=1,remote_byte=0,planes=[0,0,0,bits(1.)]*6)
    add("remote_word_selects_one",online=1,remote_word=0)
    for mutation in (1,2,4,8,16,2|4|8):
        add(f"callback_mutation_{mutation}",mutation=mutation)
    add("aabb_captured_before_callback",mutation=2,planes=[bits(1.),0,0,0]*6)
    add("phase_write_after_callback",mutation=4,planes=[0,0,0,bits(1.)]*6)
    for word in (0,0x80000000,0x7fc01234,0x7f800000,0xff800000):
        add(f"boundary_{word:x}",planes=[0,0,0,word]*6)
    rng=random.Random(202610052)
    edge=(0,0x80000000,1,0x80000001,0x7f800000,0xff800000,0x7fc01234)
    for index in range(96):
        pick=lambda: rng.choice(edge) if index%3==0 else bits(rng.uniform(-10,10))
        add(f"generated_{index}",phase=rng.choice((0,1,1,1,2,255)),
            online=rng.choice((0,0,1,255)),remote_word=rng.choice((0xffffffff,0)),
            remote_byte=rng.choice((0,1,255)),visible=rng.choice((0,1,255)),
            dead=rng.choice((0,1)),respawn=rng.choice((0,1)),gate=rng.choice((0,1)),
            level=rng.choice((0,1)),mutation=rng.choice((0,0,1,2,4,8)),
            aabb=[pick() for _ in range(6)],planes=[pick() for _ in range(24)])
    return rows

def original_cases(path, rows):
    manifest = json.loads((MODULE / "reference/object-update-culling/original-functions.json").read_bytes())
    eligibility = json.loads((MODULE / "reference/character-update-eligibility/original-functions.json").read_bytes())
    manifest["functions"] += eligibility["functions"]
    cpu=OriginalCpu(path,False,manifest)
    assert hashlib.sha256(path.read_bytes()).hexdigest()==SHA
    application=cpu.symbols["_ZN9SingletonI11ApplicationE6s_instE"]
    # The recovered Character address point uses the exact ObjectBase leaf.
    assert struct.unpack("<I",cpu.uc.mem_read(0x965f38+0x54,4))[0]==0x33dd10
    cpu.pointer(application+0x40,MANAGER)
    calls=[]
    current=[None]
    coverage=set()
    def byte(at,value): cpu.uc.mem_write(at,bytes([value&255]))
    def ret(value): cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def hook(uc,at,size,unused):
        if at in cpu.address_owner: coverage.add(at)
        x=current[0]
        if x is None:return
        if at==0x7fd794:
            calls.append(0)
            if x["mutation"]&1:byte(CHARACTER+0x86,1)
            ret(ONLINE)
        elif at==0x33dd10:
            assert cpu.reg(0)==CHARACTER
            calls.append(1) # Execute the original remote leaf, without stubbing.
        elif at==0x36e478:
            assert (cpu.reg(0),cpu.reg(1),cpu.reg(2))==(MANAGER,0,1)
            calls.append(2);ret(PLAYER)
        elif at==DEAD_STUB:
            assert cpu.reg(0)==CHARACTER
            calls.append(4);ret(x["dead"])
        elif at==0x3a5248:
            assert cpu.reg(0)==CHARACTER
            calls.append(5);ret(x["respawn"])
        elif at==0x31f594:
            assert cpu.reg(0)==application
            calls.append(6)
            if x["mutation"]&2:cpu.pointer(CHARACTER+0x12c,bits(100.))
            if x["mutation"]&4:byte(CHARACTER+0x86,77)
            ret(LEVEL if x["level"] else 0)
        elif at==FRUSTUM_STUB:
            assert cpu.reg(0)==CAMERA_ROOT
            calls.append(7)
            if x["mutation"]&8:cpu.pointer(VISUAL+8,ALT_NODE)
            ret(FRUSTUM)
    handle=cpu.uc.hook_add(UC_HOOK_CODE,hook)
    records=[]
    for name,x in rows:
        cpu.uc.mem_write(BASE,bytes(0x15000))
        current[0]=x;calls.clear()
        cpu.pointer(CHARACTER,VTABLE)
        cpu.pointer(VTABLE+0x34,DEAD_STUB);cpu.pointer(VTABLE+0x54,0x33dd10)
        cpu.pointer(CHARACTER+0x2d8,VISUAL);cpu.pointer(CHARACTER+0x418,0x7001)
        cpu.pointer(VISUAL+8,NODE);cpu.pointer(NODE+0x118,x["node_cull"])
        byte(NODE+0x200,9);byte(ALT_NODE+0x200,9)
        byte(CHARACTER+0x80,x["visible"]);byte(CHARACTER+0x86,x["phase"])
        byte(CHARACTER+0x2fc,0);byte(CHARACTER+0x1480,x["gate"])
        cpu.pointer(CHARACTER+0x110,x["remote_word"]);byte(CHARACTER+0x118,x["remote_byte"])
        cpu.uc.mem_write(CHARACTER+0x12c,struct.pack("<6I",*x["aabb"]))
        byte(ONLINE+5,x["online"]);cpu.pointer(PLAYER+0x660,x["local"])
        cpu.pointer(LEVEL+0x128,CAMERA);cpu.pointer(CAMERA+8,CAMERA_ROOT)
        cpu.pointer(CAMERA_ROOT,CAMERA_VTABLE);cpu.pointer(CAMERA_VTABLE+0x144,FRUSTUM_STUB)
        cpu.uc.mem_write(FRUSTUM+12,struct.pack("<24I",*x["planes"]))
        value=cpu.invoke("_ZN9Character9CanUpdateEv",[CHARACTER])
        records.append({"name":name,"can_update":value,
            "phase":cpu.uc.mem_read(CHARACTER+0x86,1)[0],
            "node_flag":cpu.uc.mem_read(NODE+0x200,1)[0],
            "alternate_flag":cpu.uc.mem_read(ALT_NODE+0x200,1)[0],"calls":list(calls)})
    cpu.uc.hook_del(handle)
    return records,sorted(coverage)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf",type=Path,required=True)
    parser.add_argument("--compiler",required=True)
    parser.add_argument("--build-dir",type=Path,default=MODULE/"build/character-culling-runtime")
    args=parser.parse_args()
    imports=verify_evidence(args.original_elf.resolve())
    sources=[MODULE/name for name in ("character_culling_runtime.cpp","character_update_eligibility.cpp","object_update_culling.cpp","tests/character_culling_runtime.cpp")]
    inputs=sources+[MODULE/name for name in ("character_culling_runtime.hpp","character_update_eligibility.hpp","object_update_culling.hpp","tests/run_character_culling_runtime_host.py","tests/run_object_update_culling_host.py","reference/object-update-culling/original-functions.json","reference/character-update-eligibility/original-functions.json")]
    before={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    args.build_dir=args.build_dir.resolve();args.build_dir.mkdir(parents=True,exist_ok=True)
    exe=args.build_dir/"host.exe"
    command=[args.compiler,"-std=c++17","-O1","-Wall","-Wextra","-Werror","-pedantic","-fno-fast-math","-ffp-contract=off",*map(str,sources),"-o",str(exe)]
    subprocess.run(command,check=True)
    rows=fixtures();expected,coverage=original_cases(args.original_elf.resolve(),rows)
    keys=("phase","online","remote_word","remote_byte","local","dead","respawn","visible","node_cull","gate","level","mutation")
    stdin="\n".join(" ".join(map(str,[*[x[k] for k in keys],*x["aabb"],*x["planes"]])) for _,x in rows)+"\n"
    output=subprocess.check_output([str(exe)],input=stdin.encode()).decode()
    actual=[json.loads(line) for line in output.splitlines()]
    assert len(actual)==len(expected)
    for wanted,got in zip(expected,actual):
        assert {k:v for k,v in wanted.items() if k!="name"}==got,(wanted,got)
    guards=json.loads(subprocess.check_output([str(exe),"--guards"]).decode())
    assert guards["validation"]=="PASS"
    after={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    if before!=after:raise ValueError("source changed during compilation/test")
    report={"validation":"PASS","original_arm_cases":len(rows),"mismatches":0,
        "scope":"Composed existing CanUpdate, TestCullingBeforeUpdate and IsRemotelyUpdated bodies; original nested culling/remote calls execute without stubs. Online, local-player, dead/respawn and camera providers remain external. No new original body credit or native integration.",
        "original_elf_sha256":SHA,"compile_command":command,"protocol_guards":guards,
        "source_sha256":before,
        "executable_sha256":hashlib.sha256(exe.read_bytes()).hexdigest(),
        "imports_verified_from_relocated_plt":imports,"executed_original_instruction_addresses":[hex(at) for at in coverage],"cases":expected}
    (args.build_dir/"validation.json").write_text(json.dumps(report,indent=2)+"\n",encoding="utf-8")
    print(json.dumps({"validation":"PASS","original_arm_cases":len(rows),"protocol_guards":guards["checks"],"mismatches":0}))
if __name__=="__main__":main()
