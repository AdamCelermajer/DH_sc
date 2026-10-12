# Authored chest data and placement restored through real template context

The original first Swamp MGP has `_templateName="OpenableContainer"`, without a
separate `template` attribute. The preceding native property map wrote that
string directly, selecting an empty named map. Both Container factory records
completed but retained empty data_desc and zero positions. That was not valid
authored-placement acceptance.

Main explicitly authorized the modern compatibility correction. The registered
nonempty template-name property now invokes the existing `SetTemplate` path,
which clones actual base descriptors into the selected named map and delivers
the existing LoadTemplate assertion. XML names, values and iteration order are
preserved. No XML key alias, guessed descriptors or default positions are added.
Only a CPP-only PropertyMap leaf changes; frozen interfaces and engine owners
are preserved. This is documented native behavior, not recovered original
setter parity.

The original source test compares both actual Container data_desc values to
their XML and every position component to the authored vector plus the SAME
Module offset. The two source declarations now retain Swamp_Normal_Chest and
their distinct authored positions. Both are registered in the same manager and
Module room. The next unfiltered MGP boundary is Character/_prim_NPC_PriestGood.
No constructor replay occurs after that failure.

The actual SetTemplate assertion9 delivery in this composition test is an
explicit diagnostic fixture. Production callers must supply their actual
Debug/assertion policy; no original assertion mode is invented here. The
Container's actual InitPost still reports absent CheckSpawnProbability. The
new shared RNG and Character family packages are pending separate integration.
NPC charpropsname/char_template are preserved as their own source fields.

Both incoming Config and RoomZone byte87 safety corrections are selected
explicitly. They choose deterministic native state for legacy indeterminate
bytes and are not recovered original constructor zero initialization. Their
included regressions run against the existing canonical owners. The source map
still has nine modules/controllers, 16 floor clones, 19 exits, nine zones and
386 retained render submissions.

Build host, sanitizers, x86_64 and arm64-v8a with
`tools/build_module_graph_source.py`; run `tools/run_named_template_source_checks.py`.
The runner verifies manifests, selected CPP hashes and declared policies, and
targets only loader5590. ARM64 is compiled. The visible APK is unchanged;
authored chest rendering, mobs, full GSLevel lifecycle and complete loader
acceptance remain unfinished.
