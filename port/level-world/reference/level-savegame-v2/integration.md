# LevelSavegame whole backend V2

This succeeds the frozen V1 bundle. The V1 ZIP/manifest were not replaced.
Changed core files: `level_savegame_cache_v1.hpp/.cpp` only. Additive fields are
complete-stream OBJS callback and `storage_lease`; additive method is
`recache_v2(Bytes,error)` over the SAME existing profile index. All consumers of
the cache service struct must rebuild coherently. No actor/visual/layout hooks,
root CMake, application or Level context files were changed.

New level-world TUs: `savegame_stream_v2.cpp`, `savegame_jobs_owner_v2.cpp`,
`level_savegame_objects_v2.cpp`, `level_savegame_writer_v2.cpp`,
`savegame_file_gate_v2.cpp`. Keep the three V1 owner/cache/runtime TUs and the
existing game-data `player_profile_index_v1.cpp` dependency.

## Actual file backend contract

Supply one real application file provider through `SavegameFileServicesV2`.
Its `storage_lease` pins actual callback/FileManager storage, not the whole
Application or Level. Names are exact source-generated filenames from V1.
Bind them to root's genuine Android savefiles directory; cache assets are not
savefiles. Exact original modes: read `rb`; write `w+b` (truncate/read-write).
Read returns actual bytes with a distinct genuine missing-file result.
Write returns actual byte count, seek targets an absolute byte position, and
close clears that same handle. No synthetic file, directory or successful open.

Backup first deletes the previous destination `.bak` (ENOENT is allowed), then
renames primary to `.bak`. Source `backupSavefile34f10c` returns an operation
bool; UpdateJobs ignores that bool. The service bool instead means the whole
backend invocation occurred; required missing services fail and latch the
mutation prefix. Do not use copy-to-backup or a surrogate temporary filename.
Original openSavefile34fee4 flushes matching global jobs before opening.
`SavegameFileGateV2` supplies that cache-read prefix. Create it with the SAME
application-global shared `SavegameJobsOwnerV2` and actual pinned file provider.
`gate.cache_services(files,error)` returns a retained read/OBJS adapter. Use
`bind_objects(actual_context,actual_complete_stream_callback,provider_lease)`
for root's genuine manager/object dispatch. Gate -> Jobs -> FileProvider avoids
a full Application/Level ownership cycle. The job worker uses the raw platform
read callback; its equivalent nested source FlushJobs is a busy no-op.

No checksum exists in the captured Savegame frame/cache/job path. The source
validity mechanism is a first-word -1 marker during writing and a committed
section count afterward. FileSystem's separate alternate/debug/obfuscated
resource paths are captured but not claimed as this plain savefile contract.
Platform permissions/path policy and real disk durability remain root-owned.

## Serialization and delivery

Call `level_savegame_save_all_v2(same_cache,same_fields,object_services,
same_global_jobs,error)` from `LevelSavegameApplicationV1.save_all`. It queues
backup BEFORE running section writers, creates the source frame, recaches SAME
cache, then transfers the owned write stream into the global jobs queue.
Callbacks INFO and OBJS are registered source directory entries. Unknown cached
sections keep source CString-key order and payload bytes. General section
framing is also exposed as `savegame_build_frame_v2` for actual other save
owners; it does not manufacture their writer callbacks.

Frame: uint32 section count; each entry uint32 payload length, four actual tag
bytes, payload. INFO writes same row28, not loaded_row2c. The source serializer
and OBJS callback have independent stream cursor/actual size. `__SaveObjects`
patches count then returns with cursor at payload-start+4; no final end restore.
Consequently its declared section length is4 while trailing object bytes remain
in the stream. This observed source behavior is preserved and oracle-tested.
Do not silently normalize its layout or bound OBJS to the declared four bytes.
The new cache callback receives complete cached bytes and actual section offset.

Jobs coalesce queued jobs with equal filename AND backup flag, retaining active
job and opposite-kind jobs. One update writes one source2048-byte block; first
source stream header is changed to -1, then blocks are delivered, final count is
committed by seek0/write4, and the actual file is closed. `flush(filename)` only
starts when that name is active/pending, then drains the source global queue.
Accepted enqueue is not persistence. Missing platform services latch and cannot
retry the mutation. Explicit `release(error)` closes a retained failed file and
clears jobs without replay. Bulk short-write counts are source-ignored; an
observational counter records them. Only final uint32 commit has the original
integer-writer assertion service. A completed queue must not be reported as
durable success when platform delivery was short. LevelSavegame destruction
does not flush application-global jobs.

## Same-world object services

`LevelSaveObjectsServicesV2` borrows actual ObjectManager sorted iterator, source
checkpoint28/gametype48/room64/disabled81/map-name storage, real virtual
IsCharacter/IsPlayer and Save/Load receivers. No copied actor or identity map.
For every accepted save entry the source network query is fresh. Online players
require actual IsLocallyControlled and disabled81=0. Source type queries are
reread in the later naming branch. Player entries whose map name differs are
serialized as `PlayerCharacter_0`.

Load parses first gametype string but does not use it for lookup. The SECOND
name selects: `PlayerCharacter_0` -> same PlayerManager.GetPlayer(0,true), then
same record Character+660; other names -> same canonical
GetObjectByName(name,room,false,NULL), Handle.GetObject(false). Nullable actual
receiver is valid and still seeks past payload. Real receiver Load virtual14
receives the same complete source stream, and afterward cursor is corrected to
payload end. initializing38 skips the loop. CString wire length includes NUL;
payload length is uint64. Invalid/truncated native spans fail explicitly instead
of reproducing original unsafe stream reads/assert-mode NULL writes.

Whole actor Save/Load virtual bodies are STILL required production providers.
No Character, Item, Container SG body is substituted with a successful empty
serializer. Actual manager iterator/lookup + PlayerManager publication are
root bindings. The new modules compose the whole LevelSavegame orchestration
when these receivers and real platform storage exist; they do not prove live
game persistence by themselves.

## Validation

Original ELF oracle: saveAll8 cases; whole __SaveObjects32 filter/order cases;
whole __LoadObjects6 early/null/player/cursor cases; whole UpdateJobs5 block and
commit cases. Source mutex/allocation/map fixtures and virtual byte/storage/
receiver services are declared in gold reports. Actual original integer writer
executes, including invalid-header and commit branches. Native replay compares
full frames, source query order, full read cursor, each file write offset/size/
byte hash and update counts. Complete cache/job/OBJS roundtrip, interrupted-write
backup recovery, mandatory failure no-retry/close, provider lease and matching
flush gate: PASS449 checks. Current APK-linked isolated executable,11c8219e;
no game lifecycle/input mutation. Strict six touched/new TUs x two ABIs PASS12.
No ASan/UBSan or live Android disk persistence claim is made for this receipt.
