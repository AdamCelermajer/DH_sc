# Actual actor body assignment in checkpoints

Investigation before implementation. Frozen development executable
3F31F063DA7692B57FC291E4B9EB58FC837DD0BB38241E1CC7D83F8C4BBDE161
normal Rogue kill evidence is in
`.local-inputs/v19-frontend-hotfix/death-end-main/rogue-kill.log/.png`.
Root inspected that render and exact log/hash: the authored Lizard death
finishes at serial134, physicalBefore1 becomes physicalAfter0, and its retained
corpse remains. This proves current assignment and visible corpse, not an
original-video physical detach frame. Original CSDead OnEvent3c4c3c handles
End34 with SetPhysicalObject(null,false); HP0 entry does not remove it.
See the existing seeded-animation-completion investigation for footage limits
and complete caller/animator/source registration evidence.

Current save defect: main reconstructs a body for every source body plan after
R/F9, even when this exact completed source lifecycle already removed it.
GameSave actors preserve HP/properties/identity but have no physical assignment
fact. Their action/target/animation clock are intentionally normalized; none
can prove absence. Original physical2dc is transient pointer ownership. Preserve
the actual presence as a typed value rather than serialize a pointer, infer
from HP, emit another End34, or restore the native object graph.

Minimal modern implementation: append optional ActorState physical presence.
Unknown denotes no host assignment witness; known true/false is published only
after successful actual body creation/removal (including configured no-physics).
It is independent of player/NPC/class/map, HP and render visibility. GameSavev3
appends the optional facts in existing actor order after unchanged actor payload
and v2 neutral objects; v1/v2 bytes/read/write remain unchanged and their actors
get unknown. Capture automatically chooses v3 when any fact is known. Reject
attempts to write known facts in v1/v2 instead of silently dropping them.
The strict existing world restore publishes the value with the same actor roster,
properties and RNG atomically. No extra world/actor/health owner is introduced.

Main assembly retains the saved optional value before creating the corresponding
pool entry. For false, remove the new actual physical receiver before any Step,
update or render; retain the existing PF/pool identity and publish actual false.
For true/unknown, follow actual configured creation and publish its actual result.
End34 publishes false only after successful removal. Unknown old checkpoints
use existing configured initialization, without a claim they saved absence.

Verification defined: real actor/property fixtures, true/false/unknown disk
roundtrip, v1/v2 compatibility, unsupported version/fact combination, malformed
wire/checksum/truncation and wrong roster must preserve caller outputs/World/RNG.
Same normal source kill -> F5 -> R -> F9 must keep completed corpse physical0,
HP/XP/RNG/transform and no End34/reward replay. Before death completion its body
must remain present. Original mid-death animation-cursor persistence is still
outside the current normalized-pose checkpoint contract and remains a distinct
gap; this presence extension does not claim to preserve unfinished animation
time or complete native OBJS save parity.

Serializer and isolated restore checks pass. Root inspected the new test,
runner, exact PASS log and private executable
CD65336171F121D5533463F87459E023F26290435DDA25D4A85984D493E71F45
at `.local-inputs/game-save-physical-presence-test/`. It uses real authored
Knight/Lizard sheets in one PlayableActorWorld and models the actual host's
published absent-body fact; it does not execute the body-removal producer.
True/false/unknown disk roundtrip, legacy v1/v2 byte roundtrip/no HP inference,
strict same-roster restore, wrong roster, unsupported version, checksum and
truncation preserve output/World/RNG/Character. Known facts cannot be written in
old versions and the rejected write preserves the existing file. Main producer
and normal kill/F5/R/F9 consumption are handed to lead for coherent ABI rebuild
and runtime verification. No new source-body assignment inferred by save code.

Normal183DE development run exposed a second gate before success: F5/R/F9 at
340/380/400 all reject because the existing checkpoint guard correctly treats
all animation callbacks as volatile. The End34 consumer itself is stateless:
actual body absence now belongs to the saved ActorState witness. Add an explicit
reconstructible-notification binding with a read-only checkpoint validator that
checks those saved facts against current actual body assignments. Default legacy
notification registration stays volatile; no other lifecycle/controller guard
is relaxed. Detach drops this callback and requires fresh registration after
new actor leases/pool assembly before updating again; it never replays an End.

Teardown ordering matters: normal F9 currently clears bodies before detach,
which would invalidate that validator. Add an admitted detach overload: validate
while the existing pool is still live, then invoke caller teardown with the old
Session lease still valid, then detach/invalidate leases without revalidating
the deliberately removed transient pool. Reentry/update/replacement during this
teardown is rejected. Failed teardown preserves its reached prefix and does not
publish a successful detach; it is not retried as an undone operation. The
callback may release external resources, not replace/destroy the Session/World.
Test legacy rejection, dirty-validator rejection, typed admission, old-lease
teardown order, failure prefix, fresh-binding requirement and restored no-replay.

Root's notification/teardown implementation is now source-terminal. The typed
binding validates before publishing; rejected or throwing validators preserve
the prior consumer. Checkpoint revalidates actual host facts, while legacy
registration revokes typed status. Ordinary campaign/controller and active
source-program checkpoint guards remain unchanged. Admitted teardown checks
before resource release, prevents frame/replace/detach/notification reentry,
and drops the consumer only after successful cleanup. A failed cleanup retains
the attached lease and reached external prefix, requiring caller recovery.
After restore, neither legacy registration nor clearing callbacks unlocks
update; a fresh validated typed consumer is required. Direct numeric delivery
also rejects during validation/teardown or before that fresh binding.
Strict C++17 syntax and whitespace checks pass; existing aggregate missing-field
warnings are suppressed for the syntax check. The dedicated private regression
and normal production kill/save/restore test are still pending at this receipt.

Subsequent independent regression receipt: root inspected the test source,
fresh private runner and PASS log at
`.local-inputs/session-reconstructible-notification-test/run.log` (SHA256
3A5276D81D4E585C1EC13E46C28DBDF14E17D73EED4C4E8476C71D189D941079).
Executable SHA256 is
91C3A7181767D8A0A3EB42B4E14D1DFDB80E24D23F422E0E5612AD0B3E740C82.
The actual retained Lizard Died clip produces End34, and the modeled consumer
publishes absent physical presence before v3 capture/disk restore. Tests cover
legacy/dirty/throwing validator rejection, preservation of the prior consumer,
validation before teardown, old lease validity during teardown, rejection of
frame/checkpoint/detach/bind/notification/mode reentry, failed teardown staying
attached, successful lease retirement, fresh binding required despite clear or
legacy registration, and 120 restored frames with no End34 replay. The fixture
models the physical-presence producer; it does not own the production body pool.
Main coherent build and normal kill/F5/R/F9 pool verification are now handed to
the integration lead and remain pending. No release acceptance is implied.

Production verification now passes on frozen executable SHA256
9C40A52EC493EDE7EC371898AF00AB06641BF35797360B07A41D43523D1CC663
under `.local-inputs/v19-frontend-hotfix/physical-presence-restore/`. Root
independently inspected main's producer/validator/rebuild/teardown consumers,
the executable hash, normal 480-frame log, final PNG and receipt.json. The log
SHA256 is 4306B495189337677570894F6CCDEE81A9F14C66EFD5B58F3A4AB9500EFCBB64.
Actual source kills at frames81/220 award XP8/16, and genuine End34 at serials
134/273 removes each actual body (physicalBefore1/physicalAfter0) while keeping
the existing retained visual. F5 at340 saves HP114.129 and RNG8509664/163;
content reload at380 and F9 at400 each rehydrate both saved bodies as absent.
The final frame480 has both physical0, exactly two rewards/two End34 entries,
zero checkpoint rejections and a clean shutdown. Both original corpse models
and the equipped Rogue remain visible in the captured scene. This proves the
normal host producer, v3 storage, teardown and silent restoration path for
completed death; unfinished death cursor and original visual parity are still
separate gaps. Root independently counted all94 passed/zero failed tests in
LastTest.log after the coherent154-task build and focused12-case verification.
Preview9 remains the accepted release; this receipt closes the reproduced
save rejection/body resurrection development gate, not all preview gates.
