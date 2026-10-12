# Character actual-owner fields and template binding V39

The actual private V5 Character actor now stores the original template cache13ca and CharAI master50/Character418. Existing properties cache13c8 is reused. The two borrows reject an absent or foreign actor alias and unavailable adopted state. This closes field transport; gameplay callbacks and the coherent parent build remain required.

| Subgoal | Subtask | Status | Evidence / boundary |
|---|---|---|---|
| Original field provenance | Whole C1 poisoned13c8/13ca plus real CharAI C1 master store | Finished | V38 original oracle: three poisons; original ELF unchanged |
| Actual owner transport | Reuse13c8; add13ca/master to real actor; fresh guard | Finished | Full candidate implementation ownership test, no archived actor/factory |
| Lifetime | Record alias pins actor and record; facade identity differs actual pointer | Finished | Foreign leases rejected; outer record reset retains fields; final lease reset destroys record and actor |
| Model transport | Live actual cache/master and actual PropertyView references | Finished, bounded | Missing property view preserves output; V38 model adapter thirteen original/native cases |
| Template transport | Real13c8/13ca refs, one existing application channel0 | Finished, bounded | Two actors share RNG; cached selection avoids predicate/RNG; foreign RNG and adopted state fail |
| Template branch coverage | Missing template, direct-name, player, cached | Finished, bounded | Missing template consumes no RNG; direct-name/player require main services; cache returns before services |
| Production integration | Coherent V5 graph rebuild and actual factory callbacks | Pending parent | Actor layout changed: rebuild all dependent sources before using existing V5 archives |
| Main runtime services | Actual IsPlayer, player SG_Load/class, AI_SetMaster, owned Faery save/table | Pending main | No AI effects, player/save override or current equipment created |
| SWAMP gameplay/render proof | Real visuals and later master/activation transitions | Pending | These host tests do not prove rendered or playable SWAMP |

Lease provenance is the existing factory in `canonical_character_family_v4.cpp`: it publishes `actor->canonical(record)`, whose lease pointer is the record. Construct the specific actual-actor alias `std::shared_ptr<const void>(record, record->actor.get())` for field borrow, or `std::shared_ptr<RetainedCharacterActorV1>(record, record->actor.get())` for the template frame. The alias retains the existing record controlblock and points to the real actor. The check compares that pointer to the actor, never to its possibly different facade identity. A raw record lease is rejected. Pointer validation assumes a genuine owning alias; it cannot make a deliberately nonowning/forged shared_ptr safe.

Fresh factory C1 produces template13ca=-1 at3aa354, existing13c8=-1 at3aa34c, and master=NULL through actual CharAI C1 at3cec70. Existing actor adoption remains unavailable because replaying these values would erase source-dependent current state. No authority-transfer method was invented.

`CharacterTemplateFrameV39::bind` receives the actual factory record alias, the existing typed application owner, the actual family's `loot_random` pointer, V35 table borrow, and actual IsPlayer callback. It verifies that family's pointer equals application channel0. Selection reads the current actual template CString1398 and caches in the actor; it uses no private seed/cache, C1 replay, or load-stage scheduling. Call it only where the main source callback reaches the nonplayer template branch of SafeGetCharPropsId3b3d38. The cached prefix returns before IsPlayer, table access and sampling. Binding is a transport preflight and still requires actual owner authority.

The V38 model adapter reads current `PropertyView.resolved[3]`, preserves original model bounds, actual GetCharType/IsFaerie and IsPlayer branches, and reads live master every call. A fresh produced NULL master can use the authored Faery model; a reached nonNULL master requires actual SG_GetCurrentFaerieId(-1) and GetCharFaery. State10 vs state37 does not itself prove ownership state. Main owns these services and all AI/collision/quest effects.

V38's original frozen packet remains unchanged and describes the earlier proposed patch. The valid strengthened patch, current source adoption hashes and ownership tests are recorded in `character-loader-fields-v38-adoption-patch.json`, `character-actual-fields-v39-adopted.json` and `character-actual-binding-v39-verified.json`. Parent owns CMake and aggregate manifest updates. No emulator, Android build, shared root, original cache or original ELF was changed.
