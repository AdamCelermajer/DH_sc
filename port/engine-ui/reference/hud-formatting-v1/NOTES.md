# Whole HUD localization and skill-info producer v1

This batch supplies the previously explicit integer-string, `parseEx`, skill-info,
temporary property and VarArgs services of `NativeGetSkillDetails`. It preserves
the frozen HUD initialization/manager, localization, ScriptOwner and runtime
sources. It does not itself select a player, publish an active script, or draw.

The authority is the supplied ARM32 ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`
and canonical cache ZIP SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Captured complete functions and helper scopes are in the adjacent manifests/ASM;
`literals-and-imports.json` binds the actual strings and external imports.

## Source and API

* `StringManager::parseEx` 0x509aec is implemented in
  `hud_text_format_v1.*`. It appends to output and implements all fifteen source
  directives, exact numeric conversion/rounding/grouping, missing arguments,
  pipe flag and final current-pack UTF punctuation processing. Locale constants,
  raw integer string, application version/title and current pack are required
  services. The changed result means a plain pipe was encountered, not that
  formatting occurred. Service failure retains an already appended prefix.
* `getString` 0x508e1c/0x508edc and index getter 0x5088c4 are backed by the
  owned `HudTextV1` cache. It owns the original common_text 9-by-37 metadata and
  loaded text bytes; callers retain file/constant/debug providers. Negative
  integer IDs return a separate null result. No symbol-echo fallback exists.
  `HudTextV1::parse_ex` composes that same cache, preventing a second independent
  pack clock. Compatibility `native_string` remains the frozen text-only domain.
* `AI_SkillInfo` 0x3d7e88 and `CharAISkillScript::GetInfo` 0x3daca8 are implemented
  in `character_skill_info_v1.*`. The order is fresh active query, actual
  `SetSkill(script, argument_index)`, fresh owner/active query, actual
  **OnSkillInfo(level)**, first-return numeric tag check, then fresh timer lookup
  and `1 - elapsed / duration`. Source Lua error preserves the source fraction
  behavior. Required provider failure is distinct. Valid indices and retained
  instance/vector/owner lifetimes are mandatory; unsafe original OOB loads are
  not successful native branches.
* `character_skill_info_session_v1.*` resolves real retained private sessions
  through a required Character identity provider. It uses public owner lookup,
  fresh aliases, the new single-call observer, and genuine timer store queries.
  It is compile-linked here; the composition audit below does not claim a
  complete live CharacterScriptSession publication/ownership test.
* `character_skill_properties_v1.*` supplies the source skill-script
  `ClearProps` 0x3b82e4, `SetProp` 0x3b7eac and `ApplyPropClass` 0x3babdc paths.
  **SetProp's third Boolean is ignored**; `SnS_Level` (property172/type8) updates
  owner.resolved. `ClearProps(true)` resets the shared temporary sheet from the
  actual defaults; `ApplyPropClass(class,true)` evaluates source mode1 into it
  using live owner.resolved, not a snapshot. Actual `_LoadClass` is 0x3e2e20.
  Mode0 and external-sheet `RecalcProperties(true)` remain mandatory caller
  backends, with the source mutation prefix preserved on failure.
* `CharacterHudSkillTextV1` is one NativeGetSkillDetails invocation's owned
  VarArgs and returned-string frame. Compose its `query` with the existing owned
  HUD/AS services; it handles constant/string/arguments/skill/property/parse
  operations only. Keep the frame through the complete invocation. Nested
  invocations use distinct frames. Foreign/expired argument identities fail.

Numeric parseEx reads Variant word0 as float; `^s` reads its string member.
The integer member is retained even where that directive does not read it.
The port imposes explicit 1MiB text/65536-argument bounds; malformed bounded
inputs reject atomically. Returned strings are borrowed from the owning frame.

## Evidence and reproduction

`hud-text-format-v1-arm64-differential.json`: 1,087 complete original-vs-O2
ARM64 format cases. Localized defaults, libc/AEABI and application providers are
declared services. `character-skill-info-v1-arm64-differential.json`: 640 complete
original-vs-O2 SkillInfo cases with 1,325 ordered service requests, including
source synchronous owner/active mutations and nonfinite timer arithmetic.

`skill-class-original.json` and `skill-class-gold.bin` execute original reset,
class evaluator, source setters/getters and parseEx against actual cache rows.
The real native audit loads original skills `_commons`, StaffMaster and
Hardiness scripts, genuine decoded Character448/Class260/Skills127, real design
and constant lookup, the same native temporary property writers, and actual
owned common_text/text files. Across levels0..20 it compares 42 full sheets
(9,408 words), 84 localized Current/Next strings and 84 actual Lua calls.
The two opened text leases are closed before providers leave scope. Six
ownership/required-failure checks are separate from successful source cases.
The active Character/script publication in this audit is an explicit retained
VM fixture. No normal uncached class, Buff/FX/gameplay or campaign-owner success
is inferred. This proves the reached true-temporary skill path, not every Lua
wrapper argument branch or complete live skill system.

`hud-formatting-v1-host-audit.json` binds private source-linked sanitizer DSOs,
the replacement runtime, coherent main120 dependency snapshot, conservative
project-header superset and exact cache/gold inputs before and after execution.
The source closure is conservative; system headers are represented by compiler
version. All four audits pass ASan/UBSan/LSan with zero findings. Invoke:

```
python port/engine-ui/tests/hud_formatting_v1_host.py
```

Its `--build`, `--runtime`, `--snapshot` options select the private WSL build,
replacement runtime build and coherent transitive dependency snapshot. The
report records exact commands and binary hashes. NDK readiness is a separate
14-translation-unit syntax check, not packaged linkage/device behavior.

## Central integration

UI production: `hud_text_format_v1.cpp`, `hud_text_v1.cpp`.
World production: `character_skill_info_v1.cpp`,
`character_skill_info_session_v1.cpp`, `character_skill_properties_v1.cpp`,
`character_hud_skill_text_v1.cpp`.
Runtime: compile `script_runtime_return_v1.c` **instead of** `script_runtime.c`.
The new TU includes that unchanged frozen core and adds one symbol; do not link
both translation units. Keep all other runtime sources unchanged.

Four host executables link the production DSOs, without compiling these new
production files into the executables:

```
hud_text_format_v1_audit reference/hud-formatting-v1/format-gold.bin
character_skill_info_v1_audit ../level-world/reference/character-skill-info-v1/skill-info-gold.bin
hud_skill_text_v1_audit reference/hud-formatting-v1/skill-class-gold.bin ../android-native/app/src/main/assets <extracted-original-skill-script-directory>
script_first_return_v1_audit
```

Test source paths are recorded in the freeze receipt. Scripts are the three
exact cache members `data/scripts/skills/{_commons,prince_mage_staffmaster,
prince_warrior_hardiness}.luac` (plaintext despite the extension). The production
callback does not depend on the test extraction directory.
