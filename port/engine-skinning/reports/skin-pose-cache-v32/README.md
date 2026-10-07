# V32 source handoff

Completed: retained SkinPoseCacheV32, synchronous equipment/weapon draw views,
numeric transactional Player sampler, Resource-owned sampler/primitive caches.
No gameplay clock, event, AI, physics, script or offscreen animation shortcut.

Validation: strict O2 Android compile PASS 10 across both ABIs; standalone
ASan/UBSan PASS 28,291 checks with 172 actual modules and an actual anchored
sword; 128 bit-exact pose frames; 32 whole-object parity frames; 1,200 frozen
cache hits and zero postwarm storage growth. Earlier O1/O2 receipt remains
separate. Non-sanitized CPU workload measured approximately 1.10x skin and
1.03x numeric-pose improvement; no device FPS claim.

Cache borrows must be rebound each time the immutable Skin/rest owner moves.
Pointer identity in bind invalidates cached pose on relocation; objects::sample
calls bind every time. Restoring root NPC records into new group resources
clears old per-actor caches. In-place stream mutations require reset+bind.
Successful deformation revisions are globally unique across cache owners.

Root owns live renderer, culling, GPU/UI instrumentation and final emulator
measurements. This agent reviewed owner/revision/culling lifetime ordering and
found no actionable stale-upload regression. No SceneBinding optimization was
implemented. Full detail and precise limits: NOTES.md and hashed receipts.

Player startup/XP work was separately frozen as unfinished and unpublished at
port/level-world/reports/player-initialization-v29/handoff.zip. Its pending
source producers and untested additions are listed in UNPUBLISHED.md.
