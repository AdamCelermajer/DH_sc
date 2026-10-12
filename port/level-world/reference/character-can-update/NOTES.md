# Character CanUpdate eligibility

`dh2_character_can_update` reconstructs the complete 316-byte
`Character::CanUpdate()` at `0x3a52a4`. It is a borrowed eligibility coordinator,
not a Character frame, visibility producer or script-startup shortcut.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The source function SHA256 is recorded in `original-functions.json`; the source
assembly and its dependencies were copied from the retained-frame discovery.
Only CanUpdate executes in this differential corpus. Other captured dependency
bodies are not claimed as native providers by this module.

## Source ordering

1. Capture owner visual+0x2d8. When nonnull, read its node+8 and clear byte+0x200.
2. Get the current online receiver (`0x7fd794`), read raw byte+5. When nonzero,
   call current owner virtual+0x54 (`IsRemotelyUpdated` for Character). A nonzero
   result goes directly to step 5, bypassing culling and IsDead.
3. Otherwise, if the **captured visual** exists, capture owner+0x418 before
   calling `GetPlayer(index0,true)` (`0x36e478`). Compare the captured identity
   with the returned Player+0x660. If unequal, reload owner visual+0x2d8 and its
   current node, then read node word+0x118. A nonzero word, or otherwise nonzero
   owner byte+0x2fc, calls `TestCullingBeforeUpdate(owner,owner+0x12c)`
   (`0x33de90`). False rejects unless the subsequently loaded owner byte+0x1480
   is nonzero. A true result or that interaction override proceeds.
4. Call current owner virtual+0x34 (Character `IsDead`). Nonzero plus owner
   enabled byte+0x80 zero calls `CanRespawn` (`0x3a5248`); false rejects.
5. If the original captured visual still exists and the subsequently loaded
   enabled byte+0x80 is nonzero, reload the node through that captured visual and
   set byte+0x200 to 1. Return true. Otherwise return true with the cleared byte.

Thus a dead remotely updated object may be accepted without IsDead delivery;
a failed culling result may be accepted through the interaction override;
source enabled byte zero does not generally mean rejection. The implementation
preserves these branches rather than converting eligibility into a single
derived predicate. Raw byte values use nonzero tests, including 255.

## Borrowed API and errors

`character_can_update.hpp` supplies native 64-bit typed projections:

| Projection | Source fields |
|---|---|
| `CanUpdateOwner40` | stable identity; live visual pointer; owner+418 player link; AABB identity; raw bytes80/2fc/1480 |
| `CanUpdateVisual8` | current node pointer corresponding to visual+8 |
| `CanUpdateNode8` | word118 and mutable byte200 |
| `CanUpdateServices24` | synchronous typed queries and availability bitmap |

The caller retains the owner, every captured/reloaded visual and node, and the
provider context across the call and any synchronous nested call. Replacing a
live visual does not invalidate the previously captured visual. Replacing the
captured visual's node changes the final source store destination. No reference
count, model allocation, or renderer node is silently created here.

The online result is the genuine raw byte5. Player result.identity is the actual
returned Player+660 identity, including null; it is not the Player object itself.
The culling request's subject is the caller's actual AABB identity. Other results
use word !=0. Upstream App/online/Player/camera/AABB/CanRespawn property ownership
and actual class virtual dispatch remain required provider dependencies. This
module supplies their exact call order, not accepted defaults.

Return values are 0 complete, 1 malformed projection/response, 2 reached service
unavailable, 3 delivery failed. The separate output contains the source boolean
only on complete delivery. Prior source effects, such as clearing node200,
remain when a provider fails; no rollback is claimed. Entry alignment is checked
before projection dereference, and newly loaded visual/node pointers are checked
before dereference. Reached provider failure must stop the enclosing actor frame.

## Proof

Original CanUpdate instructions versus separately compiled Android ARM64 O2:
**4,752 cases / 13,833 ordered services / zero mismatches**. The matrix covers
null and present visuals, online0/1/255, remote status, player match/miss,
culling word and force byte, true/false culling, interaction override, dead/
living, respawn true/false and enabled0/1/255. Service traces include transitional
node bytes, live/captured visual topology, player identity and raw flags.

Additional cases mutate live visual during online/Player delivery, mutate player
link after its source capture, clear culling during Player delivery, change
interaction during culling, enable during IsDead, replace the captured visual's
node during IsDead, and synchronously execute a nested CanUpdate during culling.
The nested original instructions and native helper run before the outer source
continues. No generic queue or deferred callback model is used.

Isolated host replay uses the same original-generated binary corpus under
AddressSanitizer, UndefinedBehaviorSanitizer and LeakSanitizer. It passes all
4,752 comparisons and 13,833 services, plus **23 malformed/unavailable/delivery
checks**, with zero findings. These include unaligned initial/reloaded node or
visual projections, invalid raw online response, each required query unavailable
or failing, and output-preservation/prefix-store checks. This is not a complete
Application, Character frame, property recomputation, visibility or dead-respawn
backend proof. The new helper was not yet linked into the central world DSO.

Reports:
`reports/character-can-update-arm64-differential.json` and
`reports/character-can-update-host-audit.json`, relative to level-world.
They bind source, original/corpus/O2 library and sanitized executable hashes.

From repository root, with the pinned Python and existing oracle PYTHONPATH:

```
powershell -File port/level-world/tools/build_character_can_update_oracle.ps1
python port/level-world/tests/character_can_update_differential.py
python port/level-world/tests/character_can_update_host.py
```

Suggested future central audit target: `character_can_update_audit`, compiling
`tests/character_can_update.cpp` and linking the world DSO. Its only argument is
`port/level-world/reference/character-can-update/can-update-fixtures.bin`.
The standalone host script intentionally changes no central CMake or DSO.

## Startup boundary

CanUpdate is before script-load/publication work in Character.Update. It does
not require a published active AIS merely to test eligibility. The earlier
retained-frame plan's published-active Idle branch applies after real private
VM Init/publication; it must not be substituted for the first eligible Update
of a delayed-load actor. The separate source startup worker is recovering the
actual Crypt delayed-load producer and first eligible frame initialization.
Initial Idle selection by itself does not prove that initialization happened.

The independent startup evidence in `../character-idle-startup/NOTES.md` shows
Crypt AI rows40/68 carry delayed_load=1: InitPost skips LoadScriptProcess at
`0x3b5010` and InitScriptProcess at `0x3b54c8`. The load-state stage selects
Idle3 while active AIS remains null. The first eligible Character.Update,
after CanUpdate at `0x3abf60`, excludes current0/12/2, interaction byte1480 and
already-active cases at `0x3ac34c..380`; the remaining genuine manager/UI/queue
branch calls `LoadNInitScriptProcess(true)` at `0x3ac404`, before controller,
timers and AI. That startup discovery is static source/capture evidence, not a
complete Character.Update execution claim. This eligibility kernel preserves
its predecessor position; it neither performs nor bypasses delayed startup.
