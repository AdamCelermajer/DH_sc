# Audit: the `-Woverloaded-virtual` warnings (clean build of `build/tb-warn1`)

Worktree `DH_sc/.worktrees/track-b`, tree state at the audit build.
Toolchain `mingw64/bin/g++` **GCC 16.2.0** (MinGW-W64 UCRT POSIX SEH).
Own build directory `build/tb-warn1` (configured fresh:
`cmake -S port/decomp-reconstruction -B build/tb-warn1 -G Ninja -DCMAKE_BUILD_TYPE=Release`).
Build: **3188/3188 edges, exit 0** (`ninja -k 0`), log `build/tb-warn1-logs/warn1-build.log`.

## Headline

| | count |
|---|---|
| all warnings in the clean build | **492** |
| of those, `-Woverloaded-virtual` | **427** |
| distinct (derived class, function) declarations that produce them | **12** |
| distinct (derived declaration, hidden base virtual) pairs | **13** |
| input translation units that re-emit the loudest pair | **173** |

The expected "489 / 413" is close but not reproducible: the tree is shared and other workers edit it
concurrently, so the only defensible numbers are a build you take yourself. Mine are above.

**The 427 warnings are not 427 problems.** GCC re-emits the same diagnostic in *every* translation unit
that instantiates the class, so one header line (`Character.hpp:178`) accounts for 173 identical
warnings and its neighbour for another 173. Grouping by the declaration collapses 427 → 12.

The same is true of the user's expected breakdown: `ItemInventory` + `CMatching` really do dominate —
346 warnings come from `Character` hiding `ItemInventory` virtuals and 33 from the two `CMatching`
derived classes, i.e. **379 of 427 (89 %)**.

## 1. Breakdown by class

| derived class | warnings | distinct declarations | category |
|---|---|---|---|
| `Character` | 346 | 2 | deliberate hide (ItemInventory) |
| `CMatchingGLLive` | 21 | 1 | **genuine mis-override** |
| `CDynamicAnimationSet` | 20 | 1 | deliberate hide |
| `CMatchingLocal` | 12 | 1 | **genuine mis-override** (missing base decl) |
| `CUserPointerAttribute` | 12 | 1 | deliberate hide (+2 side defects, 1 fixed) |
| `CBillboardSceneNode` | 10 | 1 | deliberate hide |
| `TouchScreenIPhone` | 4 | 4 | deliberate hide |
| `IGUITTFont` | 2 | 1 | deliberate hide |
| **total** | **427** | **12** | |

## 2. Breakdown by category

| category | declarations | warnings |
|---|---|---|
| genuine mis-override (signature says override, dispatch silently reaches the base) | 2 | 33 |
| deliberate hide (the original binary hides it too — proved per case) | 10 | 394 |
| port enum/`int32_t` alias artefact | **0** | **0** |

**The "enum/`int32_t` alias" group is empty in this build.** No user of
`-Woverloaded-virtual` here has an enum parameter on one side and `int`/`int32_t` on the other, so the
documented `IMaterialParameters` "enum mangles as `i`" deviation is *not* what these 413/427 are. The
two nearest cases are original-source mismatches, not port artefacts:

* `CDynamicAnimationSet::remAnimationLibrary(unsigned int)` vs `CAnimationSet::remAnimationLibrary(int)`:
  the binary itself has both spellings (`_ZN6glitch7collada20CDynamicAnimationSet19remAnimationLibraryEj`
  0x62fb2c and `_ZN6glitch7collada13CAnimationSet19remAnimationLibraryEi` 0x65f0d4);
* `IGUITTFont::getCharacterFromPos(char const*, int)` vs `IGUIFont::getCharacterFromPos(wchar_t const*, int)`:
  the binary itself has both (`...CGUITTFont19getCharacterFromPosEPKci` 0x55d340 and `...EPKwi` 0x55d3b8).

So the "group them and say so" instruction is satisfied by *grouping is unnecessary* — there is nothing
to group — and the real reason not to report 400 individual cases is the per-TU re-emission above.

## 3. Genuine mis-overrides (33 warnings, 2 declarations)

### 3.1 `CMatchingGLLive::GenerateNewMemberId()` — 21 warnings — REAL BUG

* hidden base: `c_matching_base.hpp:168` `virtual int CMatching::GenerateNewMemberId()`
* hiding declaration: `c_matching_gl_live.hpp:226`
  `virtual std::int32_t GenerateNewMemberId() const;`

The derived `const` makes this **not an override**. Proof of intent:

* `_ZTV15CMatchingGLLive` (0x992828) dyn **+0x0ac** = `CMatchingGLLive::GenerateNewMemberId()`
  — the demangler prints no `const`; `_ZTV14CMatchingLocal` gives the *same* slot (+0x0ac) to
  `CMatchingLocal::GenerateNewMemberId()`. In the binary the two classes share one slot ⇒ GLLive overrides.
* the recovered symbol table has `_ZN15CMatchingGLLive19GenerateNewMemberIdEv` at **0x81bbf4** (no `K`),
  and `_ZN14CMatchingLocal19GenerateNewMemberIdEv` at 0x80675c.

Consequence in the port, exactly the shape the brief describes: the port emits
`_ZNK15CMatchingGLLive19GenerateNewMemberIdEv` (a symbol the original does not have — rule 10), it does
**not** override `CMatchingLocal::GenerateNewMemberId()` either, so a virtual call on a `CMatchingGLLive`
reached through `CMatching*`/`CMatchingLocal*` runs `CMatchingLocal`'s body (0x80675c) and never
0x81bbf4. It also occupies an extra vtable slot, which shifts everything declared after it in
`CMatchingGLLive`.

Fix exists (drop `const`) but is **withheld**: removing it turns the function into a real override, which
deletes that extra slot from the port's `CMatchingGLLive` vtable. That is a vtable change → integrator.

### 3.2 `CMatchingLocal::IsInRoom()` — 12 warnings — REAL BUG (root cause is the documented vtable defect)

* hidden base: `c_matching_base.hpp:164` `virtual bool CMatching::IsInRoom(int memberIndex)`
* hiding declaration: `c_matching_local.hpp:156` `bool IsInRoom() const;`

Proof of intent + of the second defect:

* `_ZTV14CMatchingLocal` (0x992230) dyn **+0x064** = `CMatchingLocal::IsInRoom()` (non-const) and dyn
  **+0x068** = `CMatchingLocal::IsInRoom(int)`; in `_ZTV9CMatching` both of those slots are *pure*.
  So the binary's base declares **both** `IsInRoom()` and `IsInRoom(int)` as pure virtuals; the port's
  base declares only `IsInRoom(int)` (its +0x64 equivalent is the differently-named `UnknownGate()`).
* symbol table: `_ZN14CMatchingLocal8IsInRoomEv` 0x805cc8 and `_ZN14CMatchingLocal8IsInRoomEi` 0x805c1c —
  **neither is `const`**, and `c_matching_local.hpp:156`'s own comment already quotes the non-const
  mangling `_ZN14CMatchingLocal8IsInRoomEv`, so the `const` contradicts its own evidence note.

So the warning is the visible tip of **two** defects: the base is missing the pure `IsInRoom()` that
belongs at +0x64, and the derived adds a `const` the original does not have. Fixing either one moves or
replaces a vtable slot (adding the base pure virtual renumbers the whole port vtable; dropping `const`
alone would not help because the base still has no `IsInRoom()`), so both are integrator decisions
under the `KNOWN DEFECT` block at the top of `c_matching_base.hpp`.

### 3.3 The most dangerous case in the family is invisible to the warning

`-Woverloaded-virtual` stays silent when the parameter *lists* are identical and only the cv-qualifier
differs, so one line away from warning 3.2 there is a mis-override that no build in this tree reports:

* `c_matching_local.hpp:191` `bool IsInRoom(std::int32_t memberId) const;` — the `const` means it does
  **not** override `CMatching::IsInRoom(int)` (`c_matching_base.hpp:164`, non-const), so it is not even
  virtual (a same-name declaration with a different signature and no `virtual` is a plain member) and
  **every virtual `IsInRoom(idx)` call silently runs the base's `return false` stub** instead of
  `CMatchingLocal`'s body at 0x805c1c (`_ZN14CMatchingLocal8IsInRoomEi`). This is the exact
  "silently call the BASE implementation" defect the brief describes, in the class with a documented
  vtable defect, and GCC reports it at neither level 0/1/2.
* knock-on: `c_matching_gl_live.hpp:201` `virtual bool IsInRoom(std::int32_t memberId);` is correct
  (non-const, as `_ZTV15CMatchingGLLive` dyn +0x068 shows) but it can no longer override
  `CMatchingLocal`'s now-non-virtual version, so in the port `CMatchingGLLive` carries one extra
  `IsInRoom` slot that the binary does not have. Fixing the `const` on either side therefore changes a
  vtable → integrator.

`CMatchingGLLive`'s other `const` room methods (`OpenRoom(bool) const`, `CloseRoom() const`,
`ShowRoom() const`, `HideRoom() const`, `ReceiveInvitation() const`) are *not* mis-overrides: their
`CMatchingLocal` counterparts are non-virtual in the port too, so they are new slots rather than failed
overrides — the same family as the documented order defect, and the same integrator decision.

### 3.4 Same habit, no dispatch consequence (const added to non-const originals)

While classifying 3.2/3.3 I checked every `CMatchingLocal` declaration against the binary's vtable
signature; five more carry a `const` the original does not, and in each case the port's **own comment
above the declaration quotes the non-const mangled name**, which is what makes them provable without a
listing:

| declaration | comment on the same line block | binary vtable slot |
|---|---|---|
| `bool IsRoomClosed() const;` (`:106`) | `0x801b08 _ZN14CMatchingLocal12IsRoomClosedEv` | +0x054 `IsRoomClosed()` |
| `bool IsRoomHidden() const;` (`:108`) | `0x801b14 _ZN14CMatchingLocal12IsRoomHiddenEv` | +0x058 `IsRoomHidden()` |
| `std::int32_t GetMemberMask(std::int32_t) const;` (`:112`) | `0x805664 _ZN14CMatchingLocal13GetMemberMaskEi` | +0x084 `GetMemberMask(int)` |
| `std::uint64_t GetInvitationRoomId() const;` (`:114`) | `0x805694 _ZN14CMatchingLocal19GetInvitationRoomIdEv` | +0x0a0 `GetInvitationRoomId()` |
| `std::string GetMemberName(std::int32_t) const;` (`:208`) | `0x808538 _ZN14CMatchingLocal13GetMemberNameEi` | +0x05c `GetMemberName(int)` |

These are rule-10 symbol mismatches (the port emits the `_ZNK…` form, the original has no such symbol),
but none of them changes dispatch, and they must **not** be fixed piecemeal: `GetMemberMask` has to stay
`const` while `CMatching::MemberBit(int) const` is `const` in the base, and the two `IsInRoom` overloads
must move together with the base declaration that the binary proves at +0x64. Moving them is a vtable
change; that is why none of them is in §5.

## 4. Deliberate hides (394 warnings, 10 declarations)

Every one of these is proved by the binary: the original's vtable keeps the **base** implementation in
the slot, i.e. the original did not override either, and the derived declaration is a separate
(non-virtual or new-slot) member. Nothing here should be "fixed" by changing the signature — that would
invent a vtable the original does not have.

| # | hiding declaration | hidden base virtual | warnings | binary proof |
|---|---|---|---|---|
| 1 | `Character::EquipSlotAuto(unsigned int)` — `Character.hpp:178`, 0x3a9ee8 | `ItemInventory::EquipSlotAuto(unsigned int, Character*)` — `ItemInventory.hpp:64`, 0x401940 | 173 | `_ZTV9Character` ItemInventory sub-vtable (offset-to-top `0xfffffc84` = −0x37c) slot 7 (abs 0x210) = `ItemInventory::EquipSlotAuto(unsigned int, Character*)`; `Character`'s own virtual is a different slot (dyn +0x140 = 0x3a9ee8) |
| 2 | `Character::UnEquipItemFromSlot(unsigned int)` — `Character.hpp:179`, 0x3a9eb0 | `ItemInventory::UnEquipItemFromSlot(unsigned int, int)` — `ItemInventory.hpp:65`, 0x40050c | 173 | same sub-vtable, slot 8 (abs 0x214) = `ItemInventory::UnEquipItemFromSlot(unsigned int, int)` |
| 3 | `CDynamicAnimationSet::remAnimationLibrary(unsigned int)` — `CDynamicAnimationSet.hpp:65`, 0x62fb2c | `CAnimationSet::remAnimationLibrary(CColladaDatabase&)` 0x65f0d0 **and** `remAnimationLibrary(int)` 0x65f0d4 | 20 | `_ZTVN6glitch7collada20CDynamicAnimationSetE` dyn +0x030 = the derived `(CColladaDatabase&)` override, dyn +0x034 = `CAnimationSet::remAnimationLibrary(int)` (base body). The `unsigned int` overload is in no vtable ⇒ plain member |
| 4 | `CUserPointerAttribute::getString(char*)` — `scalar_attributes.hpp:100`, 0x561a70 | `IAttribute::getString() const` — `attributes.hpp:44` | 12 | the char* overload is a **new virtual slot** (dyn +0x10c) that `IAttribute` does not have: `_ZTVN6glitch2io21CUserPointerAttributeE` 280 B = 68 slots vs `_ZTVN6glitch2io10IAttributeE` 276 B = 67; the base's `getString()` slot keeps `IAttribute::getString()` |
| 5 | `CBillboardSceneNode::getMaterial(uint32_t)` — `CBillboardSceneNode.hpp:86`, 0x580b24 | `ISceneNode::getMaterial(uint32_t) const` — `ISceneNode.hpp:167` | 10 | `_ZTVN6glitch5scene19CBillboardSceneNodeE` keeps `ISceneNode::getMaterial(unsigned int) const` in the base slot (dyn +0x0e8); the derived accessor is its own symbol `_ZN6glitch5scene19CBillboardSceneNode11getMaterialEj` and is non-virtual |
| 6 | `TouchScreenIPhone::touchBegan(float,float,double,int,float,float,long)` — `touch_screen_iphone.hpp:24`, 0x33d054 | `TouchScreenBase::touchBegan(Point2D<short> const&, long)` — `touch_screen_base.hpp:49` | 1 | `_ZTV17TouchScreenIPhone` (0x95c428) dyn +0x020..+0x02c hold the four **TouchScreenBase** bodies ⇒ no override |
| 7 | `TouchScreenIPhone::touchMoved(...)` — `:25`, 0x33d124 | `TouchScreenBase::touchMoved(...)` — `:50` | 1 | as above |
| 8 | `TouchScreenIPhone::touchEnded(...)` — `:26`, 0x33ceb4 | `TouchScreenBase::touchEnded(...)` — `:51` | 1 | as above |
| 9 | `TouchScreenIPhone::touchCancelled(...)` — `:27`, 0x33cf84 | `TouchScreenBase::touchCancelled(...)` — `:52` | 1 | as above (the 7-argument function exists: function index 0x33cf84 = `_ZN17TouchScreenIPhone14touchCancelledEffdiffl`, same 208-byte body shape as its three siblings) |
| 10 | `IGUITTFont::getCharacterFromPos(const char*, int) const` — `CGUITTFont.hpp:121`, pure, "K 0x70" | `IGUIFont::getCharacterFromPos(const wchar_t*, int) const` — `IGUIFont.hpp:48` | 2 | `_ZTVN6glitch3gui10CGUITTFontE` (0x9723b8) dyn +0x024 = the `wchar_t` override, dyn **+0x070** = `getCharacterFromPos(char const*, int) const` — a slot the base does not have, which is why the port declares it pure in the intermediate `IGUITTFont` (0x55d340 / 0x55d3b8, and `glitch::gui::IGUITTFont` really exists, vtable 0x972340) |

**The `ItemInventory`/`Character` half of this group is clean.** Only two declarations are involved, and
every *intended* override in `Character` carries the `override` keyword (`CanRangeAttack` ×2,
`EquipItemAuto`, `UnEquipItemAuto`, `EquipItemToSlot`), so a failed override there would have been a
compile error rather than a warning; `ItemObject`, the other `ItemInventory` user, overrides none of its
virtuals but the destructor, and its `ItemInventory` sub-vtable in the binary (address point at abs
0x0fc of `_ZTV10ItemObject`, 0x967408) holds the eight `ItemInventory` base implementations, which the
port matches. There is no
mis-override and no const/return-type mismatch on the `ItemInventory` side at all — only the two
deliberate hides.

**Would `using Base::f;` be correct?** For all ten: **layout-neutral, yes; necessary, no.**A using-declaration never adds, removes or reorders a vtable slot, so it cannot change any ABI — but in
these ten cases the hide is what the original binary does, so a `using` would only restore the base
overload to *name lookup in port code*, and no port code needs it today (the tree compiles without them,
and the qualified `Base::f(...)` calls that the forwarders use are unaffected by hiding). I deliberately
did **not** add them: they would silence 394 of the 427 diagnostics without changing a single observable
behaviour, and silencing a diagnostic that is the only automatic detector of an accidental hide in this
tree is a net loss. If a future reconstruction needs to call, say, `bb->getMaterial(0)` on a const
`CBillboardSceneNode` or `character->EquipSlotAuto(slot, ch)`, adding the one `using` at that point is
safe and correct.

For the two category-3 classes a `using` would be **wrong**: it would paper over a real mis-override
(and `using CMatching::IsInRoom;` in `CMatchingLocal` would additionally mix the base's non-const
`IsInRoom(int)` into the derived overload set next to the derived `const` one).

## 5. What I changed, and why it cannot change any vtable layout

Four one-line edits in `glitch-core-io`, all of the same shape: a `const` the original does not have, on a
member that is **non-virtual and cannot be virtual**.

| file:line | before | after |
|---|---|---|
| `glitch-core-io/include/glitch-core-io/io/scalar_attributes.hpp:100` | `void getString(char* out) const;` | `void getString(char* out);` |
| `glitch-core-io/src/scalar_attributes.cpp:157` | `void CUserPointerAttribute::getString(char* out) const {` | `… getString(char* out) {` |
| `glitch-core-io/include/glitch-core-io/io/typed_attributes.hpp:268` | `void getString(char* buf) const;` | `void getString(char* buf);` |
| `glitch-core-io/src/typed_attributes.cpp:437` | `void CLightAttribute::getString(char* buf) const {` | `… getString(char* buf) {` |

Why this is layout-neutral:

* both classes derive from `glitch::io::IAttribute`, and that base declares **no** `getString(char*)` at
  all — its virtuals end at `getType()`/`getTypeString()` (`attributes.hpp:103-105`, the binary's
  +0x104/+0x108). With no base `getString(char*)` to override, neither member is a vtable slot and the
  cv-qualifier of a non-virtual member changes no vtable, no vtable entry and no class layout.
* the fix only corrects the **mangled symbol** the port emits so that it matches the original, which is
  rule 10: `_ZN6glitch2io21CUserPointerAttribute9getStringEPc` (0x561a70) and
  `_ZN6glitch2io15CLightAttribute9getStringEPc` (0x561dd8) — both non-const in the recovered symbol
  table, in the project's own `glitch-core-io/queue.csv` rows for those two addresses, and in the comment
  the port itself writes above each function. The declaration's `const` contradicted its own evidence note,
  which is what "correcting a signature to match its own header" is meant to cover.

`getString(char*) const` on `CUserPointerAttribute` is also the declaration the warning at #4 points at,
so this fix removes the *defect inside* the deliberate hide (the hide itself stays, correctly).

The last of the four (`CLightAttribute`) does not emit a `-Woverloaded-virtual` warning, because that
class also declares `getString() const override` next to it, which suppresses the diagnostic. It is the
identical defect shape and the identical symbol mismatch, so it is reported and fixed here rather than
left as a trap.

Verification: `tools/trackb/check_tu.sh` on both headers and both sources → `OK` ×4 (the `CUserPointerAttribute`
warning still fires after the change, now naming the non-const signature — as intended, since the
underlying hide is faithful). The emitted symbols were then read back with `nm` on freshly compiled
objects, and they are now the originals:

```
0000000000000a70 T _ZN6glitch2io21CUserPointerAttribute9getStringEPc     (original 0x561a70)
0000000000002950 T _ZN6glitch2io15CLightAttribute9getStringEPc           (original 0x561dd8)
```

### Deliberately NOT changed (all of these are vtable changes → integrator)

1. `CMatchingGLLive::GenerateNewMemberId() const` → non-const (real fix; removes a slot from the port's
   `CMatchingGLLive` vtable).
2. `CMatchingLocal::IsInRoom()` / `IsInRoom(int)` — the `const` and the missing base pure virtuals
   (both renumber or replace slots).
3. `CUserPointerAttribute::getString(char*)` — making it `virtual` reproduces the binary's +0x10c slot
   but **adds** a slot to the port's vtable.
4. No vtable was reordered, and no `using` was added, per the reasoning in §4.
5. Nothing in `link-stubs/`, `reports/`, or any other worker's file was touched.

## 6. Cross-check at `-Woverloaded-virtual=2` (bounded, not a full sweep)

GCC 16 has two levels; the project builds at level 1 (the `-Wall` default, the `[-Woverloaded-virtual=]`
tag in the log). I re-compiled individual TUs out of my own build directory with an explicit level
(`-Woverloaded-virtual=0/1/2`) to see what level 1 misses:

* `net-session-online/src/c_matching_gl_live.cpp` — level 1 = level 2 = the single
  `CMatchingGLLive::GenerateNewMemberId` warning. The `const`-only hides of §3.3 are invisible at **both**
  levels.
* `game-items-save/src/g1-game-items-save-ItemInventory-cf6f6b-0.cpp` — level 1 = level 2 = the two
  `Character`/`ItemInventory` warnings. So the two priority classes are fully covered by the 427.
* `core-platform/src/touch_screen_iphone.cpp` — 4 at both levels (same four).
* `glitch-core-io/src/scalar_attributes.cpp` — 1 at level 1, **5** at level 2: level 2 additionally flags
  `CBoolAttribute`, `CIntAttribute`, `CFloatAttribute` and `CUserPointerAttribute` each declaring
  `setString(const char*)` (a legitimate `override` of the `char*` overload) which hides the **other**
  base overload `IAttribute::setString(const wchar_t*)` (`attributes.hpp:74`). Those four are a
  char/wchar_t spelling pairing that the binary has too, and they are not reported at the project's
  level. I did not run a full level-2 rebuild, so this is a sample, not a total.

## 7. Suite result

All runs are in **my own** build directory `build/tb-warn1`; `build/tb-int` was never built or touched
by me.

| run | tests | result |
|---|---|---|
| before the four edits (binaries built 20:13-20:19) | 824 | **100 % — 824/824 passed** (160.8 s) |
| after the four edits, same tree generation | 825 | 823 passed, 2 failed: `game_center_start_test` (SEGFAULT), `link/library-duplicates` |
| after the four edits, final tree generation (the test list had grown to 827) | 827 | **825 passed, 2 failed**: `game_center_start_test` (SEGFAULT), `a_license_check_test` |

Both final failures are accounted for and neither is caused by the four edits:

* **`game_center_start_test` SEGFAULT — pre-existing.** `net-session-online/src/game_center.cpp` and
  `game_center.hpp` were rewritten by another worker at 20:20:13 / 20:20:17, i.e. after my baseline run
  and before my first post-change run; that test's object in my build dir is stamped 20:25:21, so my
  post-change binary is the first one in this directory to compile the new source. The integrator's own
  shared-tree run lists it too (`build/tb-int/Testing/Temporary/LastTestsFailed.log`, 20:24:23, contains
  exactly `net-session-online/game_center_start_test` and `link/library-duplicates`). And §8's controlled
  A/B reproduces it **with the four edits reverted**.
* **`a_license_check_test` — a flaky test-harness race, not a source defect.** Two tests in the same
  build directory use the same relative path: `net-session-online/tests/a_license_check_test.cpp:55`
  (`std::remove("./iglu.bin")`) and
  `net-session-online/tests/r1n-net-session-online-ALicenseCheck-6e7781-0_test.cpp:15`
  (`kRmsFile = "./iglu.bin"`), and neither has a `WORKING_DIRECTORY` property, so both run with the
  build dir as CWD. Under `ctest -j 8` they clobber each other's licence file. Reproduced directly:
  `ctest -j 8 -R 'license|ALicenseCheck'` failed one of four runs (once failing `a_license_check_test`,
  once `r1n-…-ALicenseCheck-…`), while `ctest -j 1` passed 2/2. Fix belongs in the test properties
  (`RESOURCE_LOCK` or distinct file names); it is not an audit item.
* `link/library-duplicates` failed in the middle run and **passed** in the final one without any edit
  from me: its five duplicated symbols were all game-layer constructors
  (`Decor::Decor(GO_IDS)`, `AnimatedDecor::AnimatedDecor(GO_IDS)`, `Dummy::Dummy(GO_IDS)`,
  `Floor::Floor(GO_IDS)`, `ColBox::ColBox(GO_IDS)`) defined both in new `game-world`/`game-anim` bodies
  and in the regenerated link stubs — the stub table was being edited by other workers at 20:20-20:26.
  No attribute or `getString` symbol ever appeared in that list.
* **every `glitch-core-io` test passes** in the post-change runs — all 78 of them, including
  `scalar_attributes_test` and `typed_attributes_test`, the two that cover the edited code.

## 8. Controlled A/B (proves the two middle-run failures are pre-existing, not mine)

The four edits were reverted, the failing test target was rebuilt in `build/tb-warn1`
(`ninja -C build/tb-warn1 dh2_net_session_online_game_center_start_test`, 131 edges, exit 0) and the two
tests were run again **with the pre-change source restored**:

```
### A/B (my 4-line change REVERTED):
1/2 Test #150: net-session-online/game_center_start_test ...***Exception: SegFault  0.76 sec
2/2 Test #827: link/library-duplicates .....................***Failed    1.07 sec
0% tests passed, 2 tests failed out of 2
```

Both fail identically without the change, so they are pre-existing. The four edits were then restored;
`build/tb-warn1` was rebuilt (exit 0, 943 edges) and the glitch-core-io objects confirm the restored
state (`…/scalar_attributes.cpp.obj` and `…/typed_attributes.cpp.obj` are stamped 20:35:34/35, newer than
the restored headers at 20:35:06), so no object built from the reverted source survives in the tree.

## 9. Post-change warning census

The four edits added no warning and removed none: the census is a property of the declarations, not of
the cv-qualifier. Re-parsing the final incremental build log (20 `-Woverloaded-virtual` warnings from the
TUs that were recompiled) gives the same declarations as the clean build, with the `CUserPointerAttribute`
note now naming the corrected signature:

```
12 warnings  CUserPointerAttribute::getString(char*)   hides IAttribute::getString()   [scalar_attributes.hpp:100]
 4 warnings  Character::UnEquipItemFromSlot(unsigned int)  hides ItemInventory::…
 4 warnings  Character::EquipSlotAuto(unsigned int)        hides ItemInventory::…
```

Census scripts and intermediate data (all outside the worktree):
`dh2-warn-audit/parse_warnings.py` (log → grouped pairs), `levelcheck.py` (per-TU
`-Woverloaded-virtual=0/1/2` comparison), `binq.py` (vtable/symbol dump from
`recovered/native/symbols/libDungeonHunter2.so`), `pairs-final.json`, `report-final.txt`.
