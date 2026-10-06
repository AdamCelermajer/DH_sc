# Loader auxiliary family successor V16

The original first SWAMP MGP now completes fourteen actual authored objects before its first unsupported TriggerZone340ec4: two Openable chests, three Characters, seven Dummy markers, one DestructibleContainer, and one SpawnPoint. The same retained map has nine Module roots and 386 source meshes. This is a constructor/placement checkpoint; the visible APK is unchanged and full rendered SWAMP acceptance remains unfinished.

`canonical_auxiliary_families_v16.hpp/.cpp` is reusable loader wiring for original Dummy3410a4, SpawnPoint340e10, Decor3410fc, and DestructibleContainer340d5c factories. Connect `construct` through `CanonicalLevelClassDispatchInputsV1.remaining`. The factory retains one RuntimeState before each canonical receiver, retains the actual XML lease, and returns the receiver through an aliasing shared ownership lease. It consumes the existing World owner and never creates another Level/manager/Handle registry.

Each typed provider is called before the real constructor. Capture its record weakly to bind later SAME-receiver operations; it owns no receiver until C1 completes. The caller supplies actual initialization, visual, PF, script, sound, interaction, quest and teardown services. Missing reached services stay explicit errors. Providers may not replace the World owner or construct the receiver themselves. Manager Add and original default/override/position order remain with the existing canonical transport. This class does not implement whole Level unload or delegate unsupported types to a generic base. It preserves the caller's output on factory failure.

The native cache probe supplies genuine inherited GameObject SetPosition through those typed providers. Other later providers remain absent explicitly. Character setters use the existing whole character-position owner. The probe verifies every completed journal entry, SAME manager identities, all three Character charpropsname/positions, real Dummy defaults, barrel network owners and placement, SpawnPoint entrypointID/script/placement, and one-shot retained failure behavior. No declaration is filtered to reach the next type. Decor's immutable twenty-one-declaration regression passes, but no Decor occurs in the currently reached MGP prefix.

Layer the independently verified V15 packet first (24 payloads plus manifest) and V16 second (30 payloads plus manifest). V16 PropertyMap CPP hash 0b0de181f990e85e155052fa2893d6195019410b505a4bfa59b20515ad7fee7b supersedes V15 only and retains the authorized named-template correction. Both source snapshots are immutable. The coherent overlay now has 621 manifest-verified files. This successor's manifest lists changed source selections; apply it after same-level-constructor-composition-v4-affaf0d076554c49.zip, preserving previous dependency closure.

Independent cache verification proves all 37 group0 Destructible records and names match the supplied original ZIP byte for byte. The table helper is available to real world providers; the constructor probe does not claim barrel InitPost has completed. Source-frozen V15 and V16 regressions report 208 and 477 checks respectively with explicit external provider fixtures.

Remaining actual boundaries:

- Raw first MGP: TriggerZone340ec4 actual canonical receiver/schema.
- Openable InitPost: MeetCondition provider.
- Positive opened/end animation callbacks: current frozen graph still supplies a no-op. Main is preparing a retained controller/event successor. Do not claim callback execution complete.
- Flag adapter: current OpenableGraph names arguments set,clear although the owner supplies clear,set. The successor must adapt as (flags & ~clear) | set. V16 also supplies clear,set; do not copy the reversed graph callback.
- Whole application composition: actual Lua/Save constructor services, Level Init/Unload, positive character visuals/animation and full SWAMP scene submission remain required.

Build: run `tools/build_module_graph_source.py` with host, sanitizers, x86_64 and arm64-v8a. Test: `tools/run_retained_graph_checks.py host sanitizers x86_64`. Android tests verify AVD DH2_Loader_API37 and target emulator-5590 before each command. Full acceptance requires a later visible APK with complete map, mobs and chests; this handoff does not satisfy that gate.
