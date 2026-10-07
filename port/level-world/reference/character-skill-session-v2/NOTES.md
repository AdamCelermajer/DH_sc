# Retained player skill Session V2

This batch replaces the earlier skill-text audit's active-VM fixture with one retained native player authority. `CharacterPlayerSkillsV2` owns `CharacterScriptSessionV2`, its privately owned `ScriptOwnerV2`/VM/aliases/timers/property bindings, and the genuine `CharacterSkillOwner`. It borrows the actual caller's NativeFSM and Debug services and pins immutable game design/skill/faery tables plus the mutable saved-skill owner. No V1 owner is instantiated alongside it. V1 files and receipts remain unchanged.

Original authority is `.local-inputs/libDungeonHunter2.so`, SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The source cache archive is the previously identified HD v1.0.2 archive. `cache-inputs.json` binds the 133 extracted exact source inputs individually; the extraction tool records the known archive hash, rather than claiming to hash the entire archive on each extraction.

## Source additions and ordering

`AISPlayerIPhone`'s captured vtable at `0x966cd0`, slot `0xcc`, selects `AISPlayer::InitVCB(0x3dd884)`. This calls the complete `AISDefault::InitVCB(0x3dc7d8)`: clear flags, test `OnTargetHit`, write `0x800`, test `OnTargetMissed`, write the accumulated `0x1000`. It then captures those flags before testing `OnKill`, and writes the captured base OR `0x400`. Alias membership does not require a callable global. The V2 helper writes the owned session's authoritative flags at both initial and later InitVCB call sites. The vtable's Init/Post/Final/Terminate remain the captured genuine empty Default functions; their dispatch is reused from the existing native kernel.

The eight versioned owner/session/service/info headers and sources are normalized copies of their frozen predecessors. `versioning-map.json` records exact predecessor hashes, normalized base hashes and reversible changes. Helpers/catalogue exports are reused from the existing World library. Changes are the actual player InitVCB path, four original native descriptor bindings, and the persistent backing of those bindings. This equivalence receipt is not itself behavior proof.

The original descriptors now install ClearProps `0x3b82e4`, SetProp `0x3b7eac`, ApplyPropClass `0x3babdc` and GetCurrentSkillInfo__ `0x3b8f9c` at their original registration deliveries, on the same owned VM. Property writers reuse frozen `character_skill_properties_v1`: temporary defaults/class mode1 are real decoded class/rule inputs; SetProp's Boolean argument does not select the temporary sheet. Normal uncached class application, external sheet resolution and recomputation still require genuine providers.

`_GetCurrentSkillInfo` returns no result with no argument. Numeric tag3 bypasses the unsigned precheck; the complete source then obtains `Value::getNumber`, performs signed f2iz, evaluates `GetCharSkill`, and reads `SG_GetSkillLevel` by **saved-row index**, not dictionary ID. `GetCharSkillListId(0x3bc5c0)` reads resolved property28 and falls back to list3. `GetCharSkill(0x3bc784)` consumes that ordered list's dictionary ID. `SG_GetSkillLevel(0x3bbed0 -> 0x4668dc)` returns -1 for null Savegame or reads unsigned16 at SavedSkill+4. These original helper instructions execute in the new numeric differential, with actual projected source GOT tables. Unsafe original assertions followed by out-of-bounds access are not emulated as success; the native checked boundary fails without output mutation.

The nonnumeric wrapper uses the first conversion for unsigned list-range checking, then freshly obtains the signed number and re-reads the live selected list. Nil/Boolean conversion is directly supported. String/native identity conversion remains an explicit required original conversion provider; there is no address truncation or guessed parsing.

`CharacterPlayerSkillsV2::initialize(final)` follows the genuine LoadNInit active guard, advances the real owner through script loading/OnInit/VCB/publication, then composes the original InitProcess kernel: required vitals -> actual configure skills/spells -> actual UpdateAllSkills -> selected Post -> optional Final. The caller still enforces upstream Character.Update eligibility. The initialization descriptor is copied; its context, Debug descriptor and NativeFSM remain borrowed. A failed required prefix does not report ready. This is not a whole Character.Update implementation.

Real skill setup executes the original path/DeclareSkill/load/reset choreography through `CharacterSkillSessionServicesV2`, then restores the captured path and refreshes the owned callback flags. Info uses the same actual active identity and owned Instance32 records, fresh SetSkill/OnSkillInfo alias lookup, the frozen one-call return observer and this same Session's timer store. No second VM, timer cursor or detached property model is used. File loaded-set semantics, alias refresh, owned close ordering and active guard are preserved. Arbitrary active replacement/hot reload/termination providers remain unsupported as documented by the original Session API; this batch does not manufacture them.

## Proof and boundaries

- Original/O2 ARM64: 256 player VCB cases/768 alias deliveries, including synchronous flag mutation at each lookup; 768 full numeric CurrentSkill wrapper/helper cases, including missing argument, fallback indices, null Savegame, NaN and unsigned16 levels. Zero mismatches. Alias membership and ReturnValues.pushInteger are logical services in this instruction proof.
- Sanitizer source replay: all 768 numeric cases and the 32 VCB cases without logical lookup mutation use real owned alias maps. The other 224 mutation cases are explicitly ARM64 logical-service proof only. Eleven native atomic/index guards pass.
- Actual retained player proof: three authored Knight/Mage/Rogue player sessions publish the selected source IPhone AIS, initialize real source vitals and load/update 19 real skill instances. Eighteen reached ordered native writer registrations and owned VCB flags are checked. The source initialized Savegame rows start at level0; this is a real native producer, **not** a replacement current-level callback. Fresh-campaign grants are a separate producer.
- Forty-two frozen original class-sheet records (9,408 words/84 localized strings) are replayed through real registered Knight/Mage skill instances and same-session Info. Some old records have other actor backgrounds; mode1 temporary class results are compared unchanged, rather than claiming those skills exist in Rogue's authored list. Rogue's actual authored initialization/update is independently exercised.
- Live saved-row mutation is observed by the same VM binding. A controlled returned timer ID proves same-owner source timer lookup/fraction; it does not claim actual authored cooldown creation. Two busy reentry and two required conversion/index failures pass. Input table facades and saved external references may die while the owners retain them; a genuine Lua finalizer calls GetCurrentSkillInfo__ during close and the saved backing releases afterward.
- ASan/UBSan/LSan: zero findings. Final host report pre/post-binds sources, conservative project-header superset, actual cache/gold, private coherent main120 dependencies and frozen formatting-v1/runtime DSOs. It records actual dynamic loader paths and executable/library hashes.
- NDK arm64-v8a/x86_64: fourteen translation units pass syntax readiness. Parent owns central linkage/build/package/device proof.

Actual Buff creation/application/removal, gameplay skill check/pre/use/post, normal property recomputation, whole caller startup eligibility and real authored cooldown creation remain required services. The audit's absent Debug file, Idle FSM projection and text-only Debug delivery are explicit fixtures. No full gameplay, frame, GPU, Android package or campaign-parity claim is made.

## Central integration

Append these **seven** sources to World (do not compile them into the test executables):

```
character_script_owner_v2.cpp
character_script_session_v2.cpp
character_skills_session_v2.cpp
character_skill_info_session_v2.cpp
character_script_player_vcb_v2.cpp
character_current_skill_v2.cpp
character_player_skills_v2.cpp
```

The frozen formatting-v1 sources and runtime first-return API must already be centrally linked. Keep the runtime replacement TU rule from that receipt: `script_runtime_return_v1.c` replaces `script_runtime.c`, never both. The V2 owner does not redefine existing helper/catalogue C exports. Standard C++17, `-fno-fast-math -ffp-contract=off`, existing World/Data/UI/Runtime dependencies suffice.

Two linked host targets:

```
character_skill_session_v2_audit
  tests/character_skill_session_v2.cpp
  reference/character-skill-session-v2/source-gold.bin

character_player_skills_v2_audit
  tests/character_player_skills_v2.cpp
  reference/character-game-design/real-cache-inputs.bin
  ../android-native/app/src/main/assets
  ../../.local-inputs/character-skill-session-v2/cache
  ../engine-ui/reference/hud-formatting-v1/skill-class-gold.bin
```

Run extraction once with `tools/extract_character_skill_session_v2.py` if those audit-only cache inputs are absent; it refuses mismatched existing bytes and never stages APK assets. The private reproduction runner is `tests/character_skill_session_v2_host.py`; its commands and exact dependency paths are in the host receipt. Original/O2 reproduction is `tools/build_character_skill_session_v2_oracle.ps1`, then `tests/character_skill_session_v2_differential.py --library .local-inputs/character-skill-session-v2/libskill_session_v2_oracle.so`. Only parent runs the main-linked adaptation/build.
