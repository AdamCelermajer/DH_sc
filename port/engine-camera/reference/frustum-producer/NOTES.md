# Frustum plane producer

Implements bytes `0x5826c0..0x5828ff` of `SViewFrustum::setFrom` (576 of its 580 bytes). The 4-byte tail branch at `0x582900` enters `recalculateBoundingBox` at `0x5823d4`; that intersection/bounds routine is explicitly outside this slice.

The matrix is 16 raw binary32 words; outputs are six source-order `[a,b,c,d]` planes. Plane bases are `m3-m2`, `m2`, `m3+m0`, `m3-m0`, `m3+m1`, `m3-m1`, with matching coefficient indices spaced by four. Each plane is normalized by `-(1/sqrt(a*a+b*b+c*c))`, including coefficient `d`; the sign-bit toggle and operation order are literal source behavior.

External `fadd/fsub/fmul/fdiv/sqrtf` imports are modeled IEEE binary32 helpers, not historical Bionic libm. Run `python port/engine-camera/tests/run_frustum_host.py --original-elf ../test_strategy/libDungeonHunter2.so`; the host differential covers ordinary, degenerate, sign, overflow/underflow, infinity, NaN, and random matrices. No camera ownership, frustum intersection, culling classification, or native wiring is claimed.
