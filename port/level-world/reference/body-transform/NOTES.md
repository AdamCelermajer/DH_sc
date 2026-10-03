Recovered `_ZN6b2Body8SetXFormERK6b2Vec2f` at `0x7e164c`, 460 bytes,
from ELF SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The capture manifest hashes the complete instruction bytes. The differential
audit executes all 115 instructions, with zero skipped kernel blocks.

The world lock byte at `world+0x191d4` returns 1 without changing the body.
Otherwise frozen flags (`body` low halfword bit 2) return 0 unchanged. The kernel
calls imported cosf/sinf, writes the column-major rotation `c,s,-s,c` and position,
rotates local center at `+0x1c/+0x20`, then adds position. Each multiplication and
addition rounds independently to binary32; contraction must remain disabled.
Current center `+0x2c/+0x30` and previous center `+0x24/+0x28` become identical.
Previous angle `+0x34` and current angle `+0x38` receive the supplied bits.

Every shape from `body+0x64`, linked at `shape+8`, synchronizes through original
service `0x7e63d0`, with the SAME new transform (`body+4`) passed twice. Success
commits broadphase (`world+0x191d8`) through `0x7e2ac8`, including an empty list.
First synchronization failure sets frozen flag 2 and zeroes angular/linear
velocity `+0x48/+0x40/+0x44`, then destroys ALL shape proxies from the head through
`0x7e6180`. It returns 0 without commit, retaining the newly written transform.

Native structs use real 64-bit pointers with static layout assertions and a
BodyState pointer shared with physical_controls. Body/world reserved fields and
flags outside the original halfword are malformed native inputs, rejected before
mutation. Return -1 is a native contract error; original success/frozen/failure
returns remain 1/0. The TransformRequest pending marker is owned by the caller.

Shape Synchronize/DestroyProxy and broadphase Commit backends are controlled
fixture callbacks in both CPU audits. The host test supplies the original-verified
trig import results, matching both Unicorn CPUs, because actual platform sinf/cosf
implementations can differ by one ULP. Imported trig implementation parity is not
claimed. Finite kernel arithmetic, infinities and signed zero compare by bits;
arithmetic NaNs compare by class because sign/payload is not CPU-portable.

This reconstructs SetXForm's body writes and ordered proxy/broadphase requests.
It does not reconstruct shape collision, proxy AABBs, broadphase pairs, contacts,
constraint solving, or world stepping. Integration must supply those services
explicitly and must not infer a complete physics engine from these verified writes.
