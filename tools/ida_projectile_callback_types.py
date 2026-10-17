"""Replay the caller-verified static Projectile callback types in IDA."""

import hashlib
from pathlib import Path

import ida_bytes
import ida_nalt
import idc


def apply():
    expected = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
    binary = Path(ida_nalt.get_input_file_path())
    if hashlib.sha256(binary.read_bytes()).hexdigest() != expected:
        raise RuntimeError("Projectile callback addresses belong to a different ELF")
    evidence = (
        "Static callback ABI: R0=Projectile*, R1=CharAISkillScript*. "
        "Verified OnCollision 0x3e55b4 and SpawnProjectile 0x3da4b0; "
        "no hidden this/third argument. Delayed arguments: Projectile+0x3cc "
        "target UserData, Projectile pointer. Concrete check returns 0/1; "
        "generic OnCollision invokes hit immediately only for result 2; "
        "result 0 queues expiry and Projectile::Update 0x3e51b0 invokes "
        "the hit callback on the following update."
    )
    for address, name in ((0x3DAFF4, "_ProjectileCheck"), (0x3DAE7C, "_ProjectileHit")):
        declaration = (
            "int __fastcall " + name
            + "(Projectile *projectile, CharAISkillScript *skill_context);"
        )
        if not idc.SetType(address, declaration):
            raise RuntimeError("Cannot apply callback type at %#x" % address)
        ida_bytes.set_cmt(address, evidence, True)
        print("%#x %s" % (address, idc.get_type(address)))


if __name__ == "__main__":
    apply()
