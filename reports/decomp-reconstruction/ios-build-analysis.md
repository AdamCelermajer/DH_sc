# iOS build analysis: what the iPad 1.0.0 IPA can and cannot do for Track B

Date: 2026-10-16. Author: analysis agent for the integrator. Inputs: the IPA the owner shared by Google Drive link
(file id `1AlRBN40TUpTBX19EiryV-_69CBJW598L`), the Android HD v1.0.2 APK and complete cache ZIP in the project root,
the Android IDA database (`libDungeonHunter2.so.i64`), and the Track B ledgers.

Nothing from the IPA is in git. Everything extracted lives in the session scratchpad only (paths at the end).
No code in the IPA was executed. The scripts that read it were run with `py -3 -I` from a different folder.

## Short answer

The IPA is a clean, **decrypted** armv7 build of the same game and the same code base, but it is the **original
1.0.0 iPad release (2010)** and it is **stripped**. Track B's Android binary is *richer* in names than the iOS one
(Android keeps its full `.symtab`, iOS keeps 180 defined symbols). So the iOS binary does **not** unlock a blocker by
giving names, RTTI or layouts. Its real contributions are (1) a second, older, simpler reading of the same functions,
(2) 239 sound effect files the Android cache is missing, and (3) a cross-check corpus for data tables and scripts.
Value for Track B itself is low to moderate. Value for Track A (playable port) is higher because of the sound files.

## 1. What the file is

| item | value |
|---|---|
| file name | `Dungeon_Hunter_2_HD_1.0.0_ios_3.2.ipa` |
| size | 567,704,020 bytes |
| SHA-256 | `1c0ef56b5c288a1e4be9d3a7944212552c459b802f7cced82e4ca5ffd12db89a` |
| MD5 | `e0f579de79ef14e7f423d6e5f03ebc1d` |
| zip entries | 8,069 (7,811 files) |
| bundle | `Payload/dungeonhunter2iPad.app`, id `com.gameloft.dungeonhunter2iPad` |
| version | CFBundleVersion 1.0.0, minimum iOS 3.2, built against the iOS 4.2 SDK, binary dated 2010-12-04 |
| data version | `data/version.txt` = `1.0.0c` (Android cache: `1.0.0B`; Android APK is HD v1.0.2) |
| main binary | `dungeonhunter2iPad`, 7,940,368 bytes, SHA-256 `81f61e897c79329342888c0dc60ed5905104f87bcda3d0970e183542c295e111` |
| architecture | single slice, ARM (cputype 12) subtype 9 = **armv7**, not fat; no armv6, no arm64 |
| encryption | `LC_ENCRYPTION_INFO` present with **cryptid = 0**: the code is decrypted and readable |
| segments | `__TEXT` 7.0 MB of code, `__DATA` 0.3 MB, `__LINKEDIT` 0.17 MB; no `__DWARF` |
| linked frameworks | Foundation, UIKit, OpenGLES, QuartzCore, CoreGraphics, SystemConfiguration, OpenAL, AddressBook(UI), CFNetwork, GameKit (weak), MediaPlayer (weak), AudioToolbox (weak), libxml2, libstdc++, libgcc_s |
| compiler/STL | GCC with libstdc++ (the Android build uses STLport) |

Provenance warning. The bundle contains a cracker marker file, a cracktro image and two copies of a 43 MB cracktro
audio file, and an `iTunesMetadata.plist` holding another person's purchase metadata. The binary is decrypted
because it was cracked, not dumped from a device the project owns. I did not copy any of that metadata into this
report. See the risks section.

## 2. Symbols and debug information

- **Symbol table:** 901 entries. 721 are imports from system libraries. 180 are defined, and none are local.
  The defined ones are almost all template and inline instantiations that the linker could not strip: Box2D inlines
  (`b2Mul`, `b2Vec2::Normalize`, contact destructors), `std::vector<item>`, `std::vector<channel>`, `std::list`,
  `slim::XmlBase`, `DataPacket`, `GLXPlayerHttp`, `GLXPlayerMPBase`, `Thread`, `_LiveFeed`, `TrophyInfo::~TrophyInfo`.
  There is no `Character`, `CharAI`, `Level` or `glitch::*` function name in it.
- **Debug info:** none. No DWARF, no line tables, no dSYM in the IPA.
- **RTTI:** only 8 typeinfo/typeinfo-name pairs and 4 vtables (`DataPacket`, `Thread`, `b2Contact`, `b2NullContact`,
  `b2PairCallback`, GLXPlayer observers). The game was built without RTTI, so **there is no RTTI route to vtables**
  for `Character`, `GameObject`, `Zone` and the rest.
- **Objective-C:** 58 class and protocol names (`EAGLView`, `GlitchGLView`, `Platform_iPhone`, `GLLive`, the Facebook
  classes, `BTControllerImpl`, `Reachability`). This is platform glue and is useless for Android.
- **Function count:** IDA found 24,137 procedures, all named `sub_xxxxxx` apart from about 180. Track B has 25,218
  in-scope functions, so the two builds are the same size class.
- **Compare with Android:** `symbols.csv` has 115,555 rows: 82,983 `.symtab` and 32,572 `.dynsym`, with 59,814 named
  functions. That includes 952 `Character::`, 377 `CharAI::`, 298 `Level::`, 270 `RenderFX`, 3,833 `glitch::scene`
  names. The Android native debug export has DWARF for 32 library CUs only and no gameplay CU. iOS adds nothing here.
- **Strings:** 14,464 unique strings. 6,236 are also in the Android binary (which has 55,979). The iOS string pool
  keeps full-path `__FILE__` strings for 127 source files; 114 of them are also in the Android binary (Android has 167
  paths, 31 of them STLport). Only 13 paths are iOS-only, and they are headers or tinyxml files, plus
  `MenuCharMenu_Inv.cpp`, `MenuMainMenu.cpp`, `MenuMerchant.cpp` and `PlayerSavegame.cpp`. The `ASSERT(%s) FAILED: %s:%d`
  macro (`gAssertLevel`: 1 prints, 2 traps) and the game-level assert texts (`faeryId >= 0 ...`, `Arrays::AIFactionTable::size`,
  `Arrays::AnimTable::members[...].StepsSize`, `Make sure to call CharAI::SetCharacter ...`, `meshes.size() == 1`) are in
  **both** binaries. My first pass wrongly counted them as iOS-only because the Android paths use backslashes.
- **Same source tree, confirmed:** the iOS paths read `.../project_iphone/../sources/Game/Objects/Characters/AI/CharAI.cpp`.
  The Android paths read `..\..\project_vs2005\Game/..\..\sources\Game\...`. The `sources/` layout is identical, which
  confirms the per-file groupings Track B already uses (`Character_Initialization`, `Character_Formulas`, `CharAI_Queries`,
  `Level_Players`, `ItemInventory_Loots`, `PFWorld_PathFinding`, `PlayerSavegame_LoadSave`).
- **Per-file string grouping.** The iOS `__cstring` pool is laid out per translation unit, so the strings that follow
  each path string belong to that file (for example the `Character_Faery.cpp` run holds `faery_charged`,
  `cinematic_Tuto_faery`, the faery asserts and `dh2_settings.savegame`). The Android pool is laid out the same way,
  so this adds little.

## 3. Side-by-side findings on sampled functions

I decompiled the iOS binary with the REA/IDA tools (Hex-Rays works on it) and compared against the Android database.
Because the iOS binary has no names, every function had to be found through a string anchor (a `__FILE__` path or an
assert text) followed by cross-references. That works only for functions that touch such a string. REA also repeats
its payload twice, so each decompile cost about 25k tokens; I stopped at three in-depth samples rather than ten.
Two more ledger rows were read only on the ledger side. The sample is small, so treat the verdicts as indicative.

| Android function (ledger status) | iOS counterpart | what the comparison showed |
|---|---|---|
| `PFWorld::FindPath` 0x52db48 (reconstructed; used as a probe of the version gap) | `sub_201584` | iOS is 31 lines, Android is 110. iOS has the same `FindPath` profiling scope string and the same call chain (drop path, copy goal into PFObject +0x40/+0x44/+0x48, `_SearchGraph`, `_SmoothPath`, `_CalcWaypointVec`) but **none** of the `PF_ProfileSearches` debug switch, the 10-sample deque of search times, or `DebugSwitches`. Android 1.0.2 added debug and timing code on top of 1.0.0. Readable, but it only shows the core, which Track B already has. |
| `Zone::IsInside` 0x3970c8 (**partial**: colzone branch missing) | `sub_B97F8` (0x270 bytes) | Same function, same assert (`go`, Zone.cpp line 152). The iOS colzone branch is **fully visible**: sweep a segment from the other object's position offset by +/-100.0 along the constant vector (`Vec3f_K`, three floats), through vtable +0xb0 of the colzone node and vtable +0x0c of a manager held at device +0x1c then +0x2c. That is exactly what the Android listing and the ledger note already describe; the Android decompile at 0x3970c8 has the same branch and only adds the `DisplayColZoneWhenColliding` debug draw. So the blocker is **not information, it is tooling** (the `Vec3f_K` header and the scene manager accessors). iOS does not unblock it. |
| `rnd::Rule::LoadFromXml` 0x490bdc (**partial**: not rebuilt past the null check) | `sub_1CE134` (0x9cc bytes, 587 decompiled lines) | iOS decompile is complete and readable and confirms the ledger description point by point: `name` attribute, `#list` / `#list[N]` selecting a list and a count, otherwise a comma separated block list; `exit` string; `id` hashed into the rule id; a child loop that creates a rule per child and ANDs the results; a final pass that clamps the room pool sizes from two fields. It also fixes the three assert line numbers (315, 322, 333, 337 in `rndRule.cpp`). Useful as a **second opinion** for the first draft, but the Track B blocker is "TinyXML element access is not declared", which iOS does not change. |
| `ObjectBase::TestDisableCondition` / `TestEnableCondition` (partial, local-player gate) | not located | No string anchor. The blocker is include and header ordering (`PlayerManager.hpp` vs `Character.hpp`), not missing facts. |
| `ObjectManager::Serialize/LoadGameObjectNetData` (partial, hosting branch) | not located | No string anchor. The net layer differs most between the builds (iOS has GKSession/Bluetooth and `GLXPlayerMP*`), so a straight read-across is unlikely. |

Results in plain terms:

- **No blocker was resolved by the iOS build.** Of the partial rows I read, the stated blockers are missing headers,
  undeclared layouts, undeclared TinyXML accessors, undeclared device file-system helpers. Those are Track B tree
  problems and a second binary does not answer them.
- **Class layouts do not transfer.** In `Zone::IsInside` the colzone node is at `this + 636` on iOS and `this + 900`
  on Android, the position floats are at object offset `0xD4` versus `0x160`, and the AABB is at `+160..+180` versus
  `+300..+320`. The 140-plus byte shift comes from libstdc++ versus STLport string and container sizes plus members added
  in 1.0.1 and 1.0.2. Field **order and kinds** are usable as a hint; **offsets are not**.
- **Hex-Rays on armv7 iOS output is readable but not better than the Android output.** The Android database has names,
  typed callees and Android-only type libraries. iOS output is `sub_` soup with NEON intrinsics.
- **The iOS build compiles in the version differences.** 1.0.0 lacks `DebugSwitches`, the PF profiling switch and
  the colzone debug draw. Any Android function that matches iOS exactly is a function with no debug additions.

## 4. Assets inventory

Everything in `data/` is stored as `fileNNNNNN.dat` (6,147 files, 490.9 MB). There are two index files:
`file00000-plantext.dat` is a **plain-text map** of 6,145 lines in the form `data/fileNNNNNN.dat:<original path>`,
left behind by the packer, so no key is needed to recover real names. `file000000.dat` is the encrypted copy of the
same index. The only obfuscation I found on the payloads is a **header tweak**: byte `i` of the first four bytes has
`i + 1` added (`BRES` is stored as `CTHW`, `<?xm` as `=A{q`, `BTEX` as `CVH\`). Undoing that gives the real files. All
6,145 mapped files exist; 0 are missing.

Mapped game data (6,145 files):

| type | files | bytes | notes |
|---|---:|---:|---|
| `.zip` (`data/sounds/sounds.zip`) | 1 | 292.4 MB | 494 `.wav` + 17 `.vxn`, stored under `_build_iphone/data/sounds/` |
| `.bdae` (BRES) | 2,901 | 143.6 MB | same BRES container as Android (2,904 there) |
| `.tga` (BTEX wrapper, PVRTC) | 233 | 43.5 MB | Android has 242 (234 BTEXpvr + 8 plain) |
| `.mgp` / `.mvp` / `.mlx` / `.mvx` / `.mgx` (module XML) | 629 / 598 / 400 / 253 / 250 | 4.6 MB | identical to Android |
| `.luac` (actually Lua **source**) | 219 | 0.9 MB | AI, skill, level, object scripts; identical to Android |
| `.bin` (pydata arrays and constants) | 215 | 1.9 MB | 208 identical, 7 differ |
| localisation (`.english` `.french` `.german` `.italian` `.spanish` `.japanese` `.korean` `.sc` `.symbols`) | 37 each | about 1.4 MB | many differ from Android (see below) |
| `.swf` menus | 7 | 1.1 MB | **tiny** compared with Android (see below) |
| `.xml`, `.lightset_xml`, `.tweaker_xml`, `.txt`, `.fnt`, `.pak`, `.state` | 39, 40, 2, 22, 1, 1, 1 | 0.2 MB | `shaders.pak` holds 32 GLSL files and 2 config files |
| fonts `.ttf` | 7 | 16 MB | includes a CJK font (`wqy-zenhei`) |

Outside `data/` (not game data for our purpose): about 215 MB of root files (three 38 MB intro `.mov` files, two 43 MB `.aif`
files that are cracker or title audio, an `.m4v`, `oconf.bar`, three effect `.bdae`), `html/` (1,562 files, 35.6 MB:
promo web pages, 1,026 JPEG), `GLIVE_RES/` (50 PNG), 8 trailer subtitle `.srt` files, and the lproj stubs.

Comparison with the Android complete cache (6,833 files, 253 directories, v1.0.2):

- **Paths:** 6,143 of the 6,145 iOS paths exist in the Android cache. The 2 iOS-only paths are `data/shaders.pak`
  and `data/sounds/sounds.zip` (Android keeps shaders as a loose `shaders.pak` at the cache root and sounds as loose files).
  The other 690 Android-only entries are `.nomedia` markers, 122 `res/*` UI PNGs, savegames, `intro_*.mp4`, and the like.
- **Byte-identical:** all 2,130 module files, all 219 Lua scripts, all 39 `.xml`, 40 `.lightset_xml`, 208 of 215 pydata
  `.bin`, the fonts, and 29 of 34 shader files.
- **`.bdae`: 1,750 of 2,901 differ but with the same size.** The only differing bytes are generated ASCII ids
  (for example a `302961746_..._material` vs `288847108_..._material` string), i.e. the same models re-exported. No geometry changes.
- **`.tga`: 156 of 233 differ at the same size.** These are PVRTC payloads that were re-encoded. Same dimensions and wrapper.
- **Text:** all 37 German and French files, 36 Spanish, 36 Italian, 8 Korean, 4 Japanese and 2 English files differ.
  This is the 1.0.0 to 1.0.2 localisation fix pass. It gives a **before-and-after** for translations and for string tables
  (`menu.sc` / `menu.symbols` differ in size), useful only if a specific text bug is chased.
- **pydata constants:** 7 small `.bin` files differ at identical size: `design_pyarray`, `game_objects_pycst`,
  `specs_pycst`, `engine_core_pycst`, `character_properties_pycst`, `common_pyarray`, plus `common_text_pycst` (size
  change). These are the only gameplay-constant differences between 1.0.0 and 1.0.2 I found; they are a tuning diff.
- **SWF menus:** `dqshared.swf` is 65 KB on iOS and 5.6 MB on Android; `dqcharmenu`, `dqhud` and `dqmenus` are
  0.27 to 0.29 MB versus 10 MB. The Android files embed their bitmaps; the iOS files look texture-split. Same menu logic,
  different packaging.
- **Sound effects: the main gap-filler.** The Android `sounds.xml` references 470 distinct `.wav` names. The Android cache
  holds only 197 of them, so **273 are missing**. The iOS `sounds.zip` contains **239 of those 273** (about 32 MB of
  `sfx_*.wav` for monsters, bosses, chests, vases and so on). 34 stay missing everywhere
  (`m_title_intro`, `m_title_loop`, `m_cutscene_01`, `m_boss_hell_king`, `m_combat_big_enemy`, `sfx_balrog_idle`,
  `sfx_checkpoint_activated`, `sfx_assassin_bomb_throw`, and so on). The iOS pack also has 58 `.wav` files that the
  Android `sounds.xml` never names.

## 5. How it helps Track B, ranked

| rank | use | value for Track B | effort | note |
|---:|---|---|---|---|
| 1 | **Second-opinion decompile of partial rows that already have a string anchor** (`rnd::*`, `Zone::*`, `Level::*`, `LuaManager`), read only to cross-check the first draft | low to medium | 1 to 2 h per function plus REA cost | Confirmed on `Zone::IsInside` and `rnd::Rule::LoadFromXml`. It will rarely reveal anything the Android listing does not. Use it as a tie-breaker when the Android Hex-Rays output is ambiguous. |
| 2 | **Older-version baseline for behaviour questions.** When the Android function has extra debug, profiling or GameCenter code, the iOS 1.0.0 body shows the minimal original (31 lines vs 110 for `FindPath`) | low | low | Helps decide what is core and what is a 1.0.2 add-on, for example which branches can be left out of a clean reconstruction. |
| 3 | **Data and script cross-check.** 208 pydata `.bin`, 219 Lua scripts and 2,130 module files are identical, so they add no new tests. The 7 differing pydata files show exactly which constants changed in 1.0.2 | low | trivial | A natural regression pair for the `pydata-accessors` readers. |
| 4 | **Real bodies for the roughly 100 iPhone/GameCenter/Bluetooth ledger rows** (`TouchScreenIPhone`, `CBluetoothReliable`, `GameCenter`, `Application::InitIPhone`) | low | medium | Those rows are skipped or partial because they are not an Android target. The 1.0.0 net layer is a different generation than the one in 1.0.2, so it is only a loose guide. |
| 5 | **Confirm the source layout** (`sources/Core`, `Game`, `Data`, `Modules`, `Utils`, `Online`) and per-file string grouping | low | trivial | Already known from the Android paths. 13 extra paths only. |
| 6 | **Name hints for `sub_` functions via assert text** | negligible | n/a | The Android binary already has the names. |

Not helpful (checked, so nobody repeats it): RTTI-driven vtable recovery (no RTTI); layout recovery (offsets differ by 140+
bytes); DWARF-like type hints (none); new type names (none beyond Android); signature or header recovery (none).

Where the build is actually more valuable is **outside Track B**. For Track A and for runtime validation the 239 sound
files and the clean `1.0.0` models and textures matter more (rank them above everything in the table).

## 6. Risks and limits

- **Provenance and licensing.** This is a cracked copy (decrypted, cracker marker file and artwork, and purchase metadata
  of a third party). The game is Gameloft's. Keep the IPA, the extracted tree, the decompile database and the sound files
  **out of git** and out of any shared artifact. Do not commit the iOS strings or the plain-text index either, because the
  index lists every asset path. Treat the whole bundle like the Android cache: private, local, no redistribution.
  If the project may not rely on pirated binaries at all, delete the copy and use only the Android material.
- **Version mismatch.** iOS is 1.0.0 (2010, iOS SDK 4.2). Android is HD v1.0.2. Behaviour, constants (7 pydata files),
  text, SWF packaging, debug code and the whole net layer differ. A match is evidence; a mismatch is not proof of an error.
- **Stripped.** No names, no RTTI, no DWARF. Every function lookup needs a string anchor, and only a minority of
  functions contain one.
- **Different toolchain.** libstdc++ against STLport and a different compiler give different inlining, class sizes
  and offsets. Never copy an iOS offset into a Track B header.
- **Not encrypted, but not complete either.** The code is readable (cryptid 0), but only the armv7 slice exists, so there
  is no arm64 comparison.
- **Tool cost.** The REA decompile tools return the same payload twice. A 587-line function cost about 25k tokens.
  Prefer `procedure_pseudo_code` on small functions, or an IDA instance driven directly.
- **Sample size.** Three functions decompiled on iOS, two more checked on the ledger side. I did not sweep the 1,000 partial
  rows, so a pleasant surprise elsewhere cannot be ruled out.

## 7. Recommended next steps

1. **Decide the licensing question first.** If the answer is "private research only", keep the files where they are and
   do nothing else with them in the repository.
2. **Hand the 239 missing sound effects to Track A** (and to whoever owns the Android cache): copy
   `sounds.zip` entries named in `data/sounds/sounds.xml` into the local cache, and run `tools/audit_missing_cache_paths.py`
   style checks against `sounds.xml`. This is the clearest win and costs minutes.
3. **Use iOS only as a tie-breaker.** When a partial row's Android listing is ambiguous, locate the iOS twin by its `__FILE__`
   string with `xrefs`, decompile just that function, and write the finding into the ledger note. Start with the `rnd::*`
   rows (`LoadRuleFile`, `LoadRoomPools`, `LoadListRules`, `Step`, `TrySpawn`) and `Level::LoadFile`, which have clean string anchors.
4. **Build a string-to-function anchor table once** (a script that lists each game `__FILE__`/assert string and the iOS function
   that references it) if step 3 is wanted at scale. About 270 anchors, one afternoon.
5. **Diff the 7 pydata files** field by field against the `pydata-accessors` readers, to record which design constants changed
   between 1.0.0 and 1.0.2.
6. **Do not** plan any Track B milestone on the iOS build. The remaining 1,007 partial and 2,068 skipped rows are dominated by
   missing headers, layout requests and compiler-generated destructors, which a second binary does not fix.

## Where the working files are (not in git)

Scratchpad: `C:/Users/NACWOR~1/AppData/Local/Temp/claude/C--Users-nacworkstation-Documents-Projects-Dungeon-Hunter-2-Recomp-DH-sc/92aea28a-441d-487f-b829-7d37b39f798b/scratchpad/`

- `ios-build/dh2.ipa` the downloaded file
- `ios-x/` the unpacked IPA (untouched)
- `ios-assets/` decoded game data under the original `data/...` paths (483 MB)
- `ios-inventory.json`, `content-diffs.json`, `diff.json` the inventories and the byte-diff list against the Android cache
- `groups.json` iOS strings grouped by source file; `strs.txt` raw strings; `srcpaths.txt` the 127 source paths
- `ios-scripts/` the Python used (Mach-O header parser, symbol dump, string compare, asset decoder, cache comparison)
- IDA database of the iOS binary: under `C:/Users/nacworkstation/.codex/rea/private-work/` (created by the REA tool, outside the repo)

## Experiment: compiler-difference recovery

Date: 2026-10-16. Question: does the iOS armv7 build give recoverable code that the Android binary lacks or makes ambiguous?
Method: capstone over both raw binaries (no decompiler), read as data only. iOS is ARM-mode (178 of 179 defined symbols are ARM),
so 18,354 functions were found by prologue and BL-target scan; Android has 31,018 symbol-bounded functions (mostly ARM mode,
1,106 Thumb). Per function: string refs (literal pool, movw/movt and pc-relative resolution), callees (with import names),
immediates and size. Matching phases: A shared rare strings (mutual best, Jaccard), V vtable slot alignment (vote from already
matched slots), P call-graph propagation (aligned callee sequences, unique callers), E neighbour scoring (size, constants,
import calls, call count), K rare 32-bit constants, L the same scoring with looser thresholds.

### Match coverage

| tier | pairs | note |
|---|---:|---|
| A string anchors | 988 | assert, source-path and format strings |
| V vtable | 123 | slot index vote |
| P callee/caller propagation | 1,945 | |
| E neighbour score >= 0.62 | 820 | |
| K rare constants | 24 | |
| **high confidence (A+V+P+E+K)** | **3,900** | |
| L low confidence (score >= 0.50) | 724 | separate; use as a hint only |

- High confidence covers 12.6% of all Android functions and **16.6% of the 20,661 non-trivial ones (>= 12 instructions)**; with L, 22%
  of the non-trivial ones outside STL and boost templates. 25% of the iOS functions found are matched.
- By ledger status (non-trivial rows, high confidence): partial **242 of 941 (25.7%)**, skipped 170 of 1,315 (12.9%), reconstructed 1,956 of 11,731.
- Precision, measured where it can be: among P and E pairs whose two sides both reference strings, 91% (156/172) and 94% (72/77) share a
  string (some misses are the same string with a different path prefix, so true precision is a little higher); L is 79%. Tier A has no
  independent check (the 70 iOS symbols that also exist on Android are small Box2D and DataPacket inlines and none was matched). Tier V is unverified (3 checkable).
- Ceiling: iOS is not a superset. Android keeps about 13k weak template and STL instantiations that iOS inlined or took from libstdc++
  instead of STLport, and the 1.0.0 net layer differs, so coverage much above 40% of non-trivial functions is unlikely with this method.
- Compiler differences seen: iOS emits inline VFP (vcmpe, vmul, vadd) where Android calls `__aeabi_f*` soft-float helpers; iOS 1.0.0 still has
  `assert` (279 functions call `__assert_rtn`, mostly boost `intrusive_ptr::operator->` with `px != 0`) and debug logging that Android compiled out;
  iOS leaves tiny accessors as standalone functions.

### Inlining-gap hits

- `CBatchMesh::getSegmentCenter` (0x57e90c, 89 insns, one caller `registerTransparentBatches`): **no iOS twin found**. Its caller and that
  caller's parent are both unmatched, so nothing anchors it. Not a hit.
- `CBatchSceneNode::addVisibleSegments<...>`: **4 of 4 found** as standalone iOS functions, all called from the iOS twin of
  `GOBatchSceneNode::updateSegmentVisibility` (iOS 0x1a7e44, tier P). Mapping by float-op profile: `SUniverseIntersector` = 0x1a8840 (no float code),
  `SBoxIntersector` = 0x1a907c (6 vcmp), `SFrustumBoxIntersector3` = 0x1a89d4 (12 vcmpe, 9 vmul), `SFrustumBoxIntersector` = 0x1a8c9c (24 vcmpe, 18 vmul).
  Universe and Box are firm; the two frustum ones are medium (call order and sizes disagree). Android has these standalone too (skipped in the ledger
  for other reasons); the gain is that the plane tests are inline VFP, not `__aeabi_fcmp*` chains.
- `android-link-missing.csv`, "original-not-reconstructed" (60) plus "original-partial-row" (7) = 67 rows: **29 matched** (A16 P9 E2 L2), 9 more have a
  plausible neighbour-scored twin, **24 none**, 5 tiny. Twins include `fillLeaderBoard` (iOS 0xb55b4), `DBG_TraceSceneNode` (0xbf9bc),
  `IVideoDriver::drawFullScreenQuad` (0x56f464), `IVideoDriver::setMaterial` (0x572318) and the 12 `Console::_setMenu*` debug menus (tier A). For
  `CTextureManager::getTexture` and `CResFileManager::get` the iOS body is larger (92 vs 271, 173 vs 554 insns): an older, more verbose build, not a cleaner one.
- Reverse scan (unmatched iOS functions called by a matched function whose Android partner has fewer callees): 847 candidates, 573 with >= 15 insns. Most are
  outlined string or vector code or unmatched neighbours, not real "Android-inlined" helpers. Clear classes: boost `intrusive_ptr` accessors (11) and a few
  GameSWF and Utils helpers (for example 0x303bec, 0x16ba3c). Not checked one by one; `helpers.py` lists them.

### Tie-breakers (10 partial rows with a named unknown)

| # | Android row | iOS twin (tier) | unknown named in the ledger | resolved? | evidence |
|--:|---|---|---|---|---|
| 1 | `Module::InitPost` 0x388b20 | 0xa7c74 (A) | callee of virtual +0x34 on the node | no | iOS makes the same virtual call at +0x34 before `sprintf`; no names or RTTI |
| 2 | `Zone::IsInside` 0x3970c8 | 0xb97f8 (P) | colzone branch | partial | iOS shows the branch plainly (VCALL +0xb0 triangle selector, then +0xc collision point) and has no `DisplayColZoneWhenColliding` block, so that is a 1.0.2 add-on; layout requests stay open |
| 3 | `CMatching::Terminate` 0x7ff5a0 | 0x651680 (E) | vtable +0x3c call, slot +0x10 | no | different order, extra GameCenter calls, vcalls +0x64/+0x20/+0xa4 unnamed |
| 4 | `CMatchingBluetooth::GenerateBluetoothDeviceName` 0x803cbc | 0x67dc8c (A) | table `CSWTCH_280` (bin attribute sizes) | no | iOS `GetMaxBufferSize` reads `this + idx*36 + 0x150` (per instance), no static table; the Android table can be read from `.rodata` directly |
| 5 | `CSignInGLLive::LoginLobby` 0x8242b4 | 0x67ad58 (A) | Game Center gate `dword_A3387C` | partial | iOS has the `mpSendLobbyLoginWithGameCenter` branch with GetGGI, UID, strlen: shape confirmed, gate flag still unnamed |
| 6 | `GLXPlayerMPBase::setErrMsg` 0x84abd8 | 0x29b5bc (P) | aux type behind vtable +0x60/+0x64 | no | same offsets, then `new[]`, memset, memcpy; type still anonymous |
| 7 | `rnd::Path::Impl::OneStep` 0x48fd64 | 0x1d2e80 (V) | `DIRECTION_OPPOSITES`, `sDirections`, link table | partial | same Step, TrySpawn, virtual-recursion skeleton, but 1,157 insns (inlined string debug code) vs 360; tables not separable cheaply |
| 8 | `glitch::res::File::Init` 0x69a40c | 0x576aec (P) | roles of header +0x2c, +0x38 | no | line for line the same (277 vs 278 insns, same compare chain) |
| 9 | `LiftableObject::InitFinal` 0x3eeec8 | 0x26d1cc (E) | PODecor vs PhysicalObject (isStatic arg) | no | iOS calls an older 4-bool PhysicalObject ctor; nothing about the retype |
| 10 | `detail::constructCompatibilityTable` 0x670a60 | 0x3661c (K) | does word 276 (+0x450) belong to the table or to `vHalf` | partial (favourable) | iOS builds the same 92 rows and masks, its zero fill stops at +0x450 and no mask writes +0x450: the table proper ends at word 275 |

Score: **0 yes, 4 partial, 6 no.** Where iOS adds anything it confirms structure; it never supplies a name, a table value or a layout.

### Verdict

Not worth a real pass for Track B. The blockers in these rows are anonymous vtable slots, struct layouts and missing headers; the iOS binary is
stripped, has no RTTI, and shows the same unnamed slots at the same offsets. What it does carry is older, simpler bodies (Zone, compat table),
which helps decide what is a 1.0.2 add-on, as section 5 already said.

Cheap things worth doing instead:
1. Add the iOS twin address to the ledger notes of the 242 high-confidence partial matches (a script over `matches.csv`; about half a day; no REA tokens), so a reviewer can open the iOS body when a row is genuinely stuck.
2. For float-heavy rows where Android shows `__aeabi_f*` chains (frustum and intersector code, `Zone`, camera), use the inline-VFP iOS body as a readable cross-check.
3. Skip the full pass. Rough cost if wanted anyway: scripted traces are about 3k tokens per function, so the 242 partial matches are about 0.7M tokens plus review; REA decompiles of both sides are about 25k tokens each, over 10M tokens, for an expected yield of no definitive answers and roughly four confirmations in ten.

### Reusable matcher (not in git)

`C:/Users/NACWOR~1/AppData/Local/Temp/claude/C--Users-nacworkstation-Documents-Projects-Dungeon-Hunter-2-Recomp-DH-sc/92aea28a-441d-487f-b829-7d37b39f798b/scratchpad/matcher/`
- `extract.py ios|android OUT.pkl` per-function features (reads `ios-x/` and `android-so/libDungeonHunter2.so` from the scratchpad)
- `match.py and.pkl ios.pkl m.pkl` the matcher; `matches.csv` is its output (4,624 pairs: android_addr, symbol, ios_addr, tier)
- `cover.py` coverage by ledger status, `prec.py` precision proxy, `twin.py ADDR` iOS twin through matched neighbours, `trace.py and|ios ADDR` compact call, string and vcall trace with partner names, `disasm_pair.py ANDROID IOS` plain disassembly, `helpers.py` iOS-only helper scan, `linkmissing.py`, `tie.py`
- Run with `py -3 -I` from inside the folder. Results vary by a few pairs between runs (set ordering).
