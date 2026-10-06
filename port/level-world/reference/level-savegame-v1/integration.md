# LevelSavegame V1 integration

Production TUs for `level-world`: `level_savegame_owner_v1.cpp`,
`level_savegame_cache_v1.cpp`, `level_savegame_runtime_v1.cpp`. Existing dependency:
`game-data/player_profile_index_v1.cpp` (already linked in game-data).
No canonical Level layout or Character layout dependency was added.

Allocate `LevelSavegameRuntimeV1(application_services)` at the actual outer
Level C1 allocation point. Its owner is then unconstructed and not ready.
After allocation, reread the SAME Level's seed114, difficulty40, row3c and
mode118. Invoke `runtime.owner().construct({level_identity, seed, difficulty,
row, mode, false}, error)` on that same allocation. The outer source C1 alone
selects this branch when row != -1; this owner does not invent a selected row.
Do not cache the request across the preceding outer allocation callback.

`LevelSavegameApplicationV1.files.read_file` borrows the actual Application
FileManager and supplies actual whole-file bytes. `found=false` means a genuine
file-open miss. A service failure is distinct from that miss. C1 attempts the
source filename and then `.bak` when the primary is missing, <=3 bytes or has
the source -1 corruption marker. A missing/invalid backup gives a genuine empty
directory; no empty profile bytes are manufactured. Other malformed section
files fail the existing source-derived safe profile index. This bounded safety
refusal is not a claim of original undefined/truncated stream behavior.

The exact Level C1 directory registration is INFO then OBJS. INFO Load reads
one source little-endian int32 into the SAME loaded_row2c; INFO Save writes the
SAME row28 (it does not write the loaded row). Constructor publication occurs
after the nested cache constructor, and initializing38 is cleared only after
both registrations. Failed constructor prefixes cannot replay. The same
Savegame lease owns filename, immutable cache and section bindings.

`files.load_objects` is required only for a nonempty OBJS section after the
initializing38 guard. It must implement the captured whole `__LoadObjects`
against SAME Application ObjectManager/PlayerManager and same object load
receivers; it must not instantiate copied characters. The captured source uses
two strings, room int32 and payload uint64, resolves the actual receiver, calls
its load virtual when present, then seeks to the payload end even when the
object is missing. This whole receiver/stream service remains unimplemented.

Save first observes the source nullable cache and inhibit_save39. Then it
requires actual Network byte5. Online saves require SAME PlayerManager hosting
and byte719; non-host or nonzero719 returns without saveAll. Offline or host
with zero719 reaches the mandatory `save_all` backend. That backend must provide
the captured whole Savegame section serialization/global jobs/file delivery,
INFO row28 and OBJS source ObjectManager tree order. It is not supplied by this
handoff; there is no blanket successful Save callback. Global job lifetime is
separate: source Savegame D1 does not call FlushJobs. Release destroys the owned
cache/directory and clears the owner publication. No PlayerSavegame is cloned
or reset. Profile snapshots may remain retained by explicit native read leases.

Checkpoint load/save, Exists/Delete, backup promotion and whole Savegame job
writer are captured source boundaries, not connected production claims. Their
network/hosting and filename mutations must be ported before exposing them.
Global initialization4618d8 has been captured but no LevelSavegame singleton
was inferred from it. Constructor global lookups include real source vtable,
stack guard and callback addresses; runtime application owners remain explicit.

Validation: whole original LevelSavegame C1 plus source filename functions,
64 cases, with declared allocation/CString/libc/nested-cache/section fixtures.
Nested Savegame file/cache is not accepted by that oracle. Native test executes
the actual new cache/index composition and replays all64 golden filenames,
backup/corruption/short-file branches, INFO, required OBJS, constructor failures,
online save gates, destruction and missing saveAll: PASS569 checks against
current APK libraries11c8219e. Strict three TUs x two ABIs PASS.
Only an isolated executable was run on5554; app inputs/lifecycle were untouched.
