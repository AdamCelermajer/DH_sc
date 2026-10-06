# Lethal skill failure: current installed V40

The user reproduced a black frame when a skill killed a monster. The same
running emulator process (PID3740) recorded the following ordered prefix:

- Learned Bash, original clip1234, one real acquired target.
- Authored `do_skill` animation event delivered.
- `_prim_tmp_cultist05`, identity0x100000004, resolved HP1177 ->0.
- Source application status−2, missing original Hit service8.
- Native frame catch deactivated HUD and model renderer, producing the blank
  dark frame. This is an exception boundary, not evidence of an OS/process crash.

Full process-only log is `before-fix-game-logcat.txt`; the captured image is
`../emulator-v40/lethal-before.png`. Current installed checkpoint SHA256 is
75dc51c6d9f11c0a047b3f28be10112f942abc982bef719d1308142083d16051.

Hit8 is the shared `hit_controller_kill` continuation. The live backend has no
published production kill owner. Surviving enemies avoid this branch, which
explains why the previous nonlethal cast rendered correctly. This finding
applies to all skill/DoT paths that reach this shared HitFor death continuation;
it is not a Bash-specific texture or effect transform fix.

Root integrated the proof-backed frontal target heap correction and missing
fresh-only NPC/player C1 Kill metadata. These additions prepare actual borrowed
receivers; they do not complete Kill. The exact further contract is recorded
in `port/level-world/reference/ranged-attack-v41/lethal-skill-adoption-audit-v42.md`.

Required production order: actual published GS current-Level -> positive
DropLoot when its genuine gate150 is zero -> contributor event4/counters/trophy
-> credited-killer XP -> immediate quest events -> victim event2 and original
death animation/target cleanup. The demo retains a genuine unactivated Level C1
with phase0 and loot gate0; it cannot impersonate the GS current-Level. No loot
skip, manufactured phase38, fake XP award, forced kill or early independent death
animation is used to bypass those required owners.

The native frame catcher now forwards the real failure to the visible Android
error surface. This is diagnostic improvement only: lethal gameplay acceptance
remains open until the whole ordered production path runs on the emulator.
