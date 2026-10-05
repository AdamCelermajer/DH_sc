# Whole in-game menu actions v1

This additive facade composes the frozen MenuStackOwnerV1 and stack kernel with
the four complete original native entry points. The menu records, retained
movies, global flags, and lifecycle services are the same objects supplied by the
caller. No separate menu model or movie graph is created. Main-menu/character
selection ownership is outside this integration scope.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Original caller bodies are in
`reference/character-menu-flow-v1/native-stack-functions.{json,asm}`; this receipt
binds their unchanged bytes. New `values` and `tu-string` captures provide the
actual original tag producers and conversion bodies.

## API and entry semantics

`MenuStackActionsV1` retains a caller game owner and fresh instance provider that
returns the genuine SAME `MenuStackOwnerV1`. Required lifecycle delivery uses the
frozen `MenuStackServicesV1`. The direct `dispatch(name,CharacterMenuCallV1&,error)`
uses the root bridge's reached **debug_text** conversion, without touching the AS
result. A raw overload accepts original tag plus stable argument token and an
explicit to_xstring provider. Tokens retain full native64 width and are never
interpreted as pointers by the facade.

| Entry | Original | Complete order |
| --- | --- | --- |
| NativePushMenu | `43b1b4` | convert argument zero via to_xstring, GetInstance, Manager.PushName |
| NativePopMenu | `43b158` | GetInstance FIRST; argc zero pops current; otherwise convert argument zero and Manager.PopName |
| NativePopAllAbove | `43ac28` | argc exactly one AND original raw tag 3/4; conversion, GetInstance, direct named-above pop; every other signature is ignored |
| NativePopAllMenus | `439dd8` | GetInstance, PopCurrent(true); no argument access/conversion |

Extra arguments are ignored. None of these bodies performs number or boolean
conversion. Push has no original zero-argument guard and is unsafe in that domain;
the facade rejects it before providers. No entry writes the AS result, including
misses, ignored signatures, nested calls, and required-service failure prefixes.

The source order of GetInstance versus conversion matters: either provider can
invoke source menu transitions synchronously. The proof executes actual nested
Manager.Push instructions from each boundary. Per-call game/provider pins and
owned converted strings keep caller data alive through nested dispatch.

`dh2_menu_stack_action_v1` is the native64 C projection for op 0..3. Its callbacks
are synchronous instance and text providers. Return 0 delivered, -1 malformed,
-2 required provider failure, or frozen stack -3/-4 continuation errors. Ignored
AllAbove signatures do not require any provider. Caller data/lifecycle must remain
valid through all callbacks; fixed occurrence budget remains the frozen stack's
explicit allocation boundary. The C++ facade maps failures to descriptive errors.

## Actual original tags and to_xstring

These are original packed tag bytes, not upstream r1714 enum ordinals:

| Tag | Original evidence |
| --- | --- |
| 1 boolean | set_bool `797230`, store at `797248` |
| 2 number | set_double `797488`, store at `7974b0` |
| 3 as_string-backed string | set_string(as_string*) `7971fc`, stores object +8 and char bytes +4 |
| 4 owned tu_string | set_string(char*) `797350`, store at `79738c` |
| 5 object / null object | constructor `7babb0`, store at `7babc8` |
| 6 property | property constructor `769540`, store at `769558` |

Therefore AllAbove accepts TWO STRING REPRESENTATIONS, not number/string.
Root bridge semantic kinds map number to 2, string to 4, object/null to 5,
deferred property to 6. Upstream has no direct original as_string tag3
representation; the raw overload preserves that source tag when a caller has it.
No object or property is silently promoted to an eligible string before the guard.

Whole original to_xstring `796f5c` checks raw tag5. Nonobjects call
to_tu_string `420a84`, then return its inline/heap bytes. Object/null instead calls
snprintf `30e244`, destination shared static buffer `a2ca7c`, limit16, exact format
`0x%p` at `90a098`, pointer from original value+4. Thus ordinary to_string is not
an equivalent object conversion. `xstring-contract.json` records the decoded
literal addresses; this facade requires the correct debug_text provider.

Pointer text depends on original libc formatting and source32 object identity.
Formatting a native64 GameSWF pointer is a portability adaptation, not exact
original pointer-text parity. Full to_tu_string/property/string allocation is
captured evidence and an explicit borrowed conversion boundary here; it is not
claimed implemented by the action facade. The original/O2 cases compare actual
raw guard behavior and wrapper ordering while conversion is a controlled service.

## Authored in-game connections

The existing original `dqcharmenu_droid.swf` action evidence has resource SHA256
`43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0`.
`character-menu-flow-v1/authored-actions.txt` records actual tab navigation:
PopAllAbove("menu_CharacterMenu") followed by Push of the selected sheet;
confirmation actions PopMenu with zero args, and close actions PopAllMenus then
NativeBackToHud. The tutorial also pops above the parent before pushing its
returned tutorial-menu name. The facade implements the four stack entries;
NativeBackToHud/sound/input and caller reload/query operations remain distinct
required source components, not successful no-ops supplied here.

## Still required lifecycle/resource providers

The frozen stack's Request48 carries menu/render/character identities, text,
value, and result. Providers must connect the SAME retained records:

- MenuBase.IsValidMenu `41b3f0`; Show `425450`; Hide `424af4`; concrete GotFocus
  virtual+14 and LostFocus+18; SetVisible `4223bc`.
- RenderFX InvokeAS `7ad7e8`, PlayAnimation `7aba04` and real success return;
  MenuFX.SetFocusDefault `7ac444`, ResetFocus `7ac410`, SetFocus `7ac228`.
- Direct source context/visibility/focus/status/saved-focus projection writes
  synchronized with the actual retained SWF graph at provider boundaries.
- Touch reset/process, genuine debug switch/load/messages, listener map,
  conditional license service and the real MenuBase world/cursor/HUD/localization
  obligations. Show/Hide are nonempty bodies; they are not proved empty callbacks.
- Authentic shared/character SWF resource construction and MenuBase catalog
  records before owner publication; those source paths are in frozen stack notes.

## Proof and integration

Original/O2 ARM64: 960 whole entry-plus-stack cases, 8,225 ordered state-at-service
comparisons, 74 actual nested manager pushes from conversion/instance, 220 ignored
signatures, 960 original AS result sentinels preserved, zero mismatches. Ordering
counts include convert→instance, instance→convert, instance only, and no providers.

Host ASan/UBSan/LSan: same 960 gold cases and every 8,225 deliberately failed
provider prefix; direct CharacterMenuCall facade has nested dispatch, stable
string/manager ownership, property result/NaN preservation, and conversion errors.
Fourteen facade checks, eight guards, zero sanitizer findings. The copied test
fixture prefix is exact frozen stack fixture code with its unused main removed.

Add only `menu_stack_actions_v1.cpp` to production already containing the two
frozen stack TUs. Host target `tests/menu_stack_actions_v1.cpp` plus those three
TUs, C++17, no added library. Argument:
`reference/menu-stack-actions-v1/action-fixtures.bin`. Its direct bridge overload
requires current `CharacterMenuCallV1::debug_text`; no bridge/source changes are
owned by this module. No packaged instruction or live whole-menu navigation claim.
