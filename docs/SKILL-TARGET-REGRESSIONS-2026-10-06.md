# Open gameplay regressions, 2026-10-06

User reports on the currently visible development build:

1. Skill animation/effect remains misaligned.
2. Targeting disappears after the skill. Acceptance follows each original skill:
   BashDown and GroundSlam Post explicitly clear current and last targets;
   Charge keeps its target only in melee range. A held attack must reacquire
   through the actual controller after that cleanup. Released controls must
   not acquire or retain an invalid or dead actor.
3. A skill killing a mob can produce a black screen. Captured earlier lethal
   Bash logs reached HP zero and failed at the required original Hit service 8,
   the unfinished death/loot/XP path.

These are open runtime bugs. Historical transform arithmetic and marker tests
do not establish that the full cast-to-target gameplay chain works.

Acceptance requires actual cast-begin/impact/end traces and visible inspection
of target identity, controller state, marker, target HP, player facing and effect
anchor. Cover surviving targets, moving targets, multiple headings/cameras,
repeat casts, real death, removal and target swaps. Distinguish a disappearing
marker from a cleared AI target; do not keep dead or invalid targets artificially.

The menus agent owns the original character tabs and campaign writer. Root
owns alignment/target-control integration; the loot agent and root own the
positive lethal-hit, reward and interaction sequence. Performance is deferred.
Only root marks the corresponding tracker rows V after the named scope passes.
