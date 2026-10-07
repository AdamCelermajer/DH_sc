# V40 shared gameplay audio integration

This is a staged integration packet, not an installed runtime or audible acceptance result. Root owns shared application edits, CMake, build/link, the emulator and device checks. New independent runtime/control helpers, captured prospective diffs and bounded CPU/source proofs are provided here. Earlier V32/V34/V38/V39 handoffs remain frozen.

The runtime reads the exact registered generated 638-row source table/names and original-selected `data/sounds/sounds.xml`. It preserves actual Play3D gates, event selection/history and load-before-output ordering. Actual manager identity, event time, output readiness and source command authority are required. Queued commands and started receipts are distinct; neither alone proves audible output. Missing exact clips remain explicit failures.

## Required root construction order

1. Retain the actual World/GS, Vox manager, source assets, random provider and receiver services in one lifetime lease. Construct one `AudioNativeSessionV40` on the gameplay producer. Bind actual Play3D gate callbacks; use the real current Level identity and reached load phase. Never assign phase38 to enable audio.
2. Initialize the exact source catalog before output opens. The session publishes its completed data initialization through a fresh lifecycle epoch. Shared Java focus and native activity/window state control permission. Control opens, timestamps, pauses, resumes and closes the real AAudio stream on its own thread. No callback performs asset IO or gameplay command production.
3. Complete the source manager initialization and actual Level listener update. Select the real Level+e4 listener row, preserving its integer distances and rolloff, and supply the completed UpdateListener vectors. Publish a synchronous immutable `AudioSourceCommandAuthorityV40` with matching manager, actual current Level and World/listener epochs. Its booleans mean completed source operations. They are not readiness requests.
4. `audio_source_command_v40` starts from proved original fresh emitter fields, applies the group's actual position type and paired distance overrides, and performs recovered Q14 spatial conversion. Supply current source general fields after all reached setters, actual emitter/group/settings gain and pitch after the original modifier/RNG prefix, and actual native music initial state when needed. The adapter does not invent settings, random modifiers, actor velocity, an actor for target0, or a visual-to-audio scale.
5. Establish the actual authored-event monotonic time scope, including scheduler lag and nested callbacks, before binding the typed producer transport to the owning session's `submit_actual_play`. A direct runtime endpoint bypasses session close exclusion and belongs only in independent controlled fixtures. Apply the events diffs in their documented order against verified base hashes. Existing selection/state/RNG/position prefixes stay in their owners. The one runtime executes source gates once. Ordinary/named/combat/target events share this endpoint.
6. Poll runtime receipts on the gameplay producer. Expose exact missing asset, provider, source gate, queue rejection and native control-required results. Keep started voices separate from queued plays and audible acceptance. `load_actual_xml_uid` is original LoadSound loading only; prove the caller's unit conversion before binding it. Do not play or randomly choose an event to implement precache.
7. On teardown, exclude new producer submissions, request output close independently of the Java GL queue, then complete session shutdown on the producer. Successful real stream close and control join precede sole-consumer stop/drain and pin release. Failed or timed-out close retains the entire owner and provider lease; do not create a replacement session or free World/assets. Muted output advances silence without consuming queued commands.

## Loader and gameplay prerequisites still open

The shared app still needs a successful genuine source Level `_LoadProcess`/GS transition and matching current-Level identity; a fabricated phase is insufficient. Actual Level listener selection/update, current camera/actor vectors, Vox settings/general setters and source modifier ownership must be connected to the same World epoch. Music Play/initial state and event precache have their own original owners and must not be synthesized through gameplay Play3D.

Player/NPC full melee requires the retained source animator/AI/inventory/FX/common result pipeline and distinct Character+408 look-at authority. Development HP writes and diagnostic NPC Attack/Walk/Dead scheduling do not establish those owners. Item drop/pickup diffs are held until the genuine Item145 pool and WorldLoot lifecycle are installed. Container diffs are held until the actual factory, interaction/state/timeline/quest/loot/Lua receivers exist. Projectile impacts need real ranged creation/update/collision ownership. Detailed per-category prerequisites are in [events/handoff.md](events/handoff.md).

The source archive audit found no exact copy of 283 unique missing XML filenames in 212 plausible archives. Chest source33->XML162 and the eight exact drop/pickup clips remain absent. Normal sword fallback samples are present; elemental mage-staff override IDs478..482 are five direct sounds, not generic sword overrides. No substitution is included.

## Review and validation

- [events/handoff.md](events/handoff.md): actual producer call sites, four narrow prospective diffs, held owners, category checklist and syntax/patch receipts.
- [focus/README.md](focus/README.md): one shared front/native focus request, epoch mailbox, control-thread output and Java/native staged changes.
- [runtime/default-source-fields/NOTES.md](runtime/default-source-fields/NOTES.md): original constructor/Reset3D/GetGain/GetPitch/general initializer evidence.
- [runtime/host.json](runtime/host.json): O1/O2 ASan/UBSan real-source CPU decode, event scheduling, required failure and lifetime fixtures. Host inherited dense units use a misleading-indentation suppression; production NDK checks do not.
- [runtime/strict-compile.json](runtime/strict-compile.json): runtime/clock/source command strict ARM64 and x86_64 compilation with O2, Wall/Wextra/Werror and controlled floating point.

Root must perform the final link/build and actual AAudio/World/lifecycle checks. All-category audible acceptance remains open: sword swing, skills, monsters, chest, item and projectile categories need reached source events plus actual output, and missing clips remain unavailable.
