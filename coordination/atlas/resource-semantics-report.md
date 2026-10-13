# Resource semantics report

## Scope and evidence

This resolver annotates extracted BRES event names and proven sound-label
references. It does not change game behavior. The original consumer evidence is
the Android IDA `CharAI::_OnAnimEvent` at `0x3d4434`, recovered at
`port/level-world/reference/character-melee-animation-event-v1/NOTES.md`, and
the corresponding event dispatch in
`port/level-world/character_melee_animation_event_v1.cpp`. The caller reads
animation step index and count before dispatch. Prefix markers (`ev_`, `an_`,
`fx_`, `sfx_`) route through different source owners. Bare names then pass
state-specific gates: state 5 checks `CanRangeAttack`; states 6, 7, and 13
admit `do_skill`, `do_spell`, and `interact` respectively. The resolver carries
those gates on typed `native_event` targets instead of treating every callback
name as an animation file.

For named sound, `port/engine-audio/reference/event-families-v38/handoff.md`
records the whole original handler path: exact `sfx_` prefix, four-byte prefix
removal, case-sensitive lookup in the 638-name `Arrays::Sounds` table, and
source no-op on an unknown name. `port/engine-audio/reference/source-bindings-v38/NOTES.md`
establishes the distinct source ID, XML UID, event flag and exact sounds.xml
member relationship. Physical files are joined only by the full
`data/sounds/<filename>` path, retaining exact filename case and the Android/iOS
inventory receipts. The owner's flat `.local-inputs/audio-v34/cache` is admitted
as an exact supplemental Android sound corpus: its catalog filename maps to
`data/sounds/<same filename>` without basename searching or case correction.
The Android handler is attached to an iOS event string
only as an explicit inference; iOS marker text does not establish native-ID
equivalence.

Existing visual evidence is reused only as context: the Act 1 source video in
`coordination/v21.7/B155-B156-HANDOFF.md` records relevant gameplay observations
at 18:12–18:15 and 966–1000 seconds. Those frames show effects and transitions,
not the continuous event-dispatch path or a mapping for every marker. This is
internal resource-atlas work, so the visible invariant is that each authored
marker can be traced to an evidenced handler/role or remains visibly unresolved;
the source video is not presented as proof of marker semantics.

## Expected behavior and uncertainties

Keep reference IDs stable, resolve only exact source names and catalog links,
and preserve duplicate event occurrences as separate source sites. The known
`do_skill`, `do_spell`, attack, interaction and prefix families receive typed
native-event targets with their source handler and admission conditions. An
exact source SDD name resolves to its own source row, UID/event members, and
physical URI receipts. A known source row whose exact sound file is absent
remains unresolved with `FILE_MISSING`; known missing SDD members are never
filled from a nearby basename. Unknown names such as `opened`, `closed`, bare
`fx`, and `inccounter` remain unresolved until their exact owner and gate are
proved.

The supplied native proof is Android IDA/source evidence. iOS-only source
strings and package mappings may represent a different version. The resolver
does not infer missing iOS native IDs, change filename case, strip a sound
prefix from an XML value, or infer a generic footstep sound. Exact source SDD
and sound catalog membership do not prove a particular animation or XML object
reaches playback in runtime.

If a reference has neither an explicit platform nor a recognizable source path,
its source platform remains `unknown`. Android native handler/source-row evidence
can still be attached by exact marker or label, but the association is `LIKELY`,
includes an explicit `platform_association`, and keeps
`native_equivalence: not-asserted`; the Android evidence itself remains labeled
as Android.

## Verification

Focused checks are defined in `tools/atlas/resource_semantics_checks.py` and
cover stable IDs/idempotence, state and prefix gates, unknown markers,
one-prefix-only sound lookup, exact SDD membership, full-path Android/iOS
receipts, missing physical members, XML label case behavior, and the explicit
iOS-to-Android inference caveat. Expected failure cases include unknown names,
case-only mismatches, missing physical files, and duplicate exact table labels.

## Results and remaining gaps

Implementation status: isolated resolver and checks only. The atlas orchestration,
graph projection and full corpus regeneration remain with root. No isolated
test result establishes integrated graph correctness or gameplay completion.
Unresolved owner-specific event markers and source SDD rows with absent exact
files remain follow-up work. A no-write dry-run against the saved 95,124-row
reference ledger annotated 1,699 relevant rows: 846 native-event targets and
196 unresolved callback markers (opened 134, fx 38, closed 20, inccounter 4);
all 44 BRES named-sound rows joined exact source SDD labels and complete
physical paths. Exact XML sound attributes produced 569 source-row joins, 567
complete physical paths, two retained `FILE_MISSING` rows, and 44 unknown
labels.
These are isolated resolver counts, not final generated-graph counts.

The synthetic checker passed 22 positive and negative cases, including source
event membership, a two-member random sound event with one missing sample,
Android/iOS union, case-only file mismatch, unknown/case-mismatched labels,
duplicate labels, iOS and unknown-platform inference caveats, and repeated resolution. The
independent `coverage_check(ctx)` is available for root's final generated-ledger
validation; it has not been run against the integrated output yet.
