# B004 independent target-retention review

Reviewed `auto_target_marker_v1.{hpp,cpp}`, its six-assertion test and runner, the
owner's `auto-target-marker-v1-report.json`, and the recovered target/marker
handoffs. No production source was changed.

## Evidence

- **Original visual:** inspected the supplied v1.0.3 local-video contact sheet
  `.local-inputs/reference-video/dh2-act1/target-retention-sequence-281-291.png`,
  sampled from 281.0 through 290.5 seconds every 0.5 seconds. A living Bogwomp
  carries the red ring and name/HP while the player moves through different
  sword poses and MISS/damage feedback appears. The marker later changes after
  the first enemy is down. This is direct visual evidence of presentation
  continuity between sampled poses; the frames do not reveal Space/touch input,
  so they do not prove a release event or exact target-state transition.
- **Current frozen build reproduction:** directly inspected
  `.local-inputs/v19-frontend-hotfix/pc-skill-space/target-release.png` and
  `target-auto-release-preview.png`. The same Rogue/Bogwomp scene shows the
  red marker/name/health after explicit Tab selection and no marker/name/health
  after implicit Space attack/release. The corresponding logs report the same
  living Bogwomp HP `16.7969/47.5`, while player `target_id` is respectively
  `7118915781085668844` and `0`. This confirms the adapted PC runtime has
  separate explicit and implicit target outcomes in frozen build DA4C; it does
  not establish source-correct expected state or original touch semantics.
- **Original logic:** the source-bound ELF is
  `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
  `CharacterTargetMarkerV28::update` consumes `Character+0x40c` (`last_target`)
  first and falls back to `Character+0x14a4` (OOI) when last-target is null or
  the player. Null/self at both inputs hides the marker. Its interaction-type
  virtual chooses one of nine owned marker slots. A negative interaction type
  leaves the prior marker selection unchanged; the marker code has no alive
  filter. The recovered `Character::Update` caller reaches
  `UpdateObjectOfInterest` at `0x3ac0b4`; the OOI updater at `0x3abb9c` validates
  the existing OOI and refreshes its candidate query on a 500 ms timer.
  `Ctrl_Click` at `0x3addc8` is an explicit sticky-target producer.
  `_ClearNonStickyTarget` at `0x3d8d70` clears current and last target only when
  sticky is false; HUD `Update` at `0x41a780` has a held-attack/no-OOI path that
  sends `Cmd_Attack(null)` and clears sticky. These source paths make current
  target, preferred/last target, OOI and rendered ring distinct values.
- **Skill rule:** recovered BashDown/GroundSlam Post callbacks explicitly call
  `ClearTarget`; Charge clears only when its target is outside melee range.
  `ClearTarget` uses SetTarget(null,false) then SyncLastTarget. The ring policy
  must not write combat target state back or override those authored clears.

## Review and verification

The helper implements the exact candidate precedence and source enum without
mutating combat state. Its tests cover live last-target precedence, null and
self last-target fallback, last-target without OOI, null/self hide, and an
unresolved opaque source identity that must pass through without an invented
registry/alive filter. I independently reran the final working tree:

```powershell
& port/windows-foundation/features/combat/run_auto_target_marker_v1_tests.ps1
```

Result: `PASS original target-marker candidate precedence, null/self fallback, and unfiltered source identity`
(strict optimized C++17 build; exit code 0). The final helper runner passed.

This is helper-level evidence only. No actual last-target/OOI provider is wired
to the Windows foundation marker caller by this helper, and it does not test
same-frame Session clear ordering, source interaction dispatch, the world ring
or HUD in the normal executable, death/anchor update, or a release-to-next-
attack integrated sequence. B004 remains open until the lead/root verify those
normal-path states and capture the visible ring/HUD on an isolated profile.
