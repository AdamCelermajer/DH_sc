# In-game menu stack v1

This module reconstructs the captured MenuManager/MultiMenuManager stack bodies
and native push/pop routing. It owns native projection records and stack storage;
it does not construct movies or replace MenuBase, World, cursor, HUD, touch,
RenderFX, ActionScript, or animation services. It is intended for the in-game
character/inventory/skills flow. Capturing shared manager branches does not claim
ownership of the separate main-menu/character-selection implementation.

The original is `.local-inputs/libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`captures/native-stack-functions.json` and the four supplemental capture groups
bind addresses, sizes, and assembly to that ELF. The source SWF path table is
decoded directly from its literal pointer arrays; that table is static evidence,
not an executed movie-loading proof.

## Executed source and ordering

| Body | Address | Relevant behavior |
| --- | --- | --- |
| NativePushMenu | `43b1b4` | first value to_xstring, instance, Manager.PushName |
| NativePopMenu | `43b158` | argc zero pops current; otherwise Manager.PopName |
| NativePopAllMenusAbove | `43ac28` | exactly one raw AS tag 3/4, then named-above pop; other signatures ignored |
| NativePopAllMenus | `439dd8` | current pop with all=true |
| Manager.PushObject / PushName | `4317e8` / `431924` | valid menu, duplicate suppression, reset/process touch, multi push, debug, HUD listener registration |
| Manager.PopObject / PopName | `42e208` / `42e2b0` | membership permits popping CURRENT, followed by named HUD listener removal |
| Manager.HideAll | `42d234` | captured registry length, live registry rereads, visible entries SetVisible(false) |
| Manager.GetByName | `42d1f0` | exact case-sensitive first string match |
| Multi.PushObject | `438278` | state lookup, classification, outgoing blur/hide, append, context, incoming show/focus |
| Multi.PopCurrent | `438c14` | global preamble once, blur/hide, remove, resume for EACH pop, including pop-all |
| Multi.PopName | `439270` | above mode stops at named top; direct mode traverses both arrays and removes matching occurrences |
| Multi.IsStateInStack | `437924` | all render state arrays, actual pointer comparison |

The corpus executes the actual GetByName/GetNumMenus, weak-pointer alive paths,
GetState/current-state, contains, and array removal instructions. The SWF
virtual/AS/animation/debug/touch/listener bodies are controlled services on both
sides. Native wrapper string conversion is an explicit external boundary, not a
claimed `as_value::to_xstring` reconstruction.

Push retains the outgoing state receiver across its callbacks. Pop rereads live
top state at the source call sites. Incoming push writes status 1 to its retained
state after GotFocus; pop writes status 3 to the fresh top after GotFocus.
Consequently a synchronous callback may change the top while the original
retained push receiver is still updated. Fifteen original nested manager pushes
and 190 other synchronous mutations exercise this distinction.

Manager duplicate pushes still call IsValidMenu and can register the HUD listener;
they do not execute the multi push. Manager.PopName does not remove an arbitrary
named entry: if that entry is present anywhere, it pops the current entry.
Direct Multi.PopName is exposed separately. Pop-all resumes intermediate entries
rather than hiding every entry without resumption.

Render flag `0x40` suppresses animation operations at the original branches.
Flag `1` invokes SetFocusDefault on push, and saved-focus restoration on resume.
Flag `8` changes source focus-enabled projection. On resume its type test uses
the MANAGER BASE top character, but its write targets the resumed top character.
MenuBase virtual `+3c` is **IsValidMenu `41b3f0`**, reading byte `+7c`, not an
enabled-policy getter. Base GotFocus `41b3e8` and LostFocus `41b3ec` are empty;
the borrowed adapter must still dispatch the genuine concrete virtual method.

Global addresses are `lastOpenMenuID 9f63fc`, `isInGameMenu 9f6400`,
`isBackKeyPressed 9f6401`, `Back_key_Glive 9a5c09`, multiplayer `9a5b59`,
multiplayer IGM `9a5b5a`, and DRM `9f640e`. All source name classifications and
pop remappings are preserved, including MainMenu clearing isInGameMenu and
VerificationLoading setting Back_key_Glive. Debug requests retain original call
site addresses; printf and the platform debug backend remain required services.

## Borrowed service contract

Return zero from `MenuStackServicesV1::invoke` only after delivering the requested
operation. A nonzero return aborts with `-2` at that exact prefix. `request.result`
is the IsValidMenu result or PlayAnimation success result when queried. Services
may synchronously invoke the kernel again. Every record, string, storage array,
character projection, and service context must survive the complete nested call.

| Operation | Genuine required receiver/behavior |
| --- | --- |
| menu_valid | concrete MenuBase virtual `+3c`, IsValidMenu |
| menu_blur / menu_hide / menu_show / menu_focus | MenuBase virtual `+18/+10/+0c/+14` |
| invoke_as | supplied render and supplied menu name, suffix OnHide/OnShow |
| play_animation | supplied character, focus_out/hide/show/focus_in; return actual success |
| render_reset | MenuFX virtual `+24`, **SetFocusDefault `7ac444`** |
| reset_focus / set_focus | genuine controller, false argument, fresh saved focus |
| reset_touch / process_touch | ResetTouch `328f40`, touchscreen ProcessEvents `33c568` |
| debug_load / debug_switch | real Load and GetSwitch("isTracingMenuManager") |
| register_listener / unregister_listener | actual manager listener map and source debug prefix |
| menu_set_visible | concrete SetVisible `4223bc`, false |
| license_check / debug_message | genuine required source services, with original call-site identity |

MenuBase.Show `425450` and Hide `424af4` contain nonempty required localization,
world pause/cursor/HUD/drag/AS/visibility obligations. Those complete captured
bodies are in `captures/lifecycle`; the stack must not accept them as empty
callbacks. The caller also synchronizes direct projection writes (character
visibility/focus flags, render context, saved focus, menu status) with the SAME
retained SWF objects. This module's fields are not a second authoritative movie.

## Ownership, publication, and failure domain

`MenuStackOwnerV1` owns copied names, menu/render projection records, catalogs,
registry, and fixed-capacity outer/per-render occurrence buffers. It is neither
copyable nor movable. Register real retained records before `seal`; registration
after publication is rejected. Borrowed globals/characters/resources/services
must outlive the owner and all nested operations. Duplicate names preserve
first-record lookup. Identities are full `uintptr_t`; host fixtures exceed 32 bits.

Kernel return values are 0 delivered, -1 malformed before mutation, -2 required
service failure, -3 unsupported source-invalid continuation, and -4 exhausted
caller occurrence storage after the preceding source effects. Provision capacity
before publication. The original grows gameswf arrays; automatic allocator/GC
parity is not claimed. Clearing an expired projected weak pointer does not
implement source shared-header refcount ownership.

Direct named removal can revisit allocated stale slots after logical counts
decrease. That traversal is preserved. Some duplicate-removal patterns produce
invalid original negative memcpy extents or invalid resumed indices; the native
kernel stops with -3 instead of reproducing memory corruption. Direct named gold
uses the valid source domain; duplicate push/current/above/all cases remain tested.
NativePush argc=0 is likewise outside the original safe argument domain and is
rejected. Guards and storage/liveness failures are native contract extensions.

## Authentic movie paths and remaining integration

`source-swf-paths.json` records the exact four-path selections from LoadMenu
`431ea4`: index 0 shared, 1 character menu, 2 menus, 3 HUD. The width854 table
selects `dqshared_droid.swf`, `dqcharmenu_droid.swf`, `dqmenus_droid.swf`, and
`dqhud_droid.swf`. Other original width/platform branches remain documented
without assigning unproved platform flags. Multi.LoadSWF `437d68` checks its
cache, then constructs MenuFX, calls the actual loader, and creates its companion
handler. Those factories, resource owners, full MenuBase services, listener map,
world/cursor policy, and source AS conversion remain external required providers.

For in-game integration, use the already retained shared/character movies and
source menu records, register them into this owner, publish once, then route the
native push/pop callbacks into these APIs. Deliver concrete lifecycle calls and
synchronize source projections at callback boundaries. Keep the current root
character-menu reload/AS owner as the authoritative graph.

## Proof and build

`menu-stack-v1-arm64-differential.json`: 4,800 actual original/O2 ARM64 cases,
27,099 ordered state-at-callback comparisons, 190 synchronous mutations, 15
nested pushes, zero mismatches. Twelve operation groups each contain 400 cases.

`menu-stack-v1-host-audit.json`: same 4,800 gold cases, all 27,099 deliberately
failed callback prefixes, ten owned nested/lifetime checks, six malformed/liveness
guards, ASan/UBSan/LSan zero findings. This is projection/service proof; no live
SWF navigation, GPU, packaged instruction, or complete menu ownership claim.

Production sources: `menu_stack_v1.cpp`, `menu_stack_owner_v1.cpp`. C++17, no new
library dependency. Host target: `tests/menu_stack_v1.cpp`, those two sources;
argument: `reference/menu-stack-v1/stack-fixtures.bin`. Compile with
`-fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off`.
Run with `ASAN_OPTIONS=detect_leaks=1`. The O2 ARM64 kernel build command and
actual source/binary/gold/capture hashes are recorded in the proof/freeze receipt.
