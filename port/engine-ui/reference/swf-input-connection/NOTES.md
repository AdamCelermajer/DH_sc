# Original RenderFX event and HUD input connection

## Frozen event dispatcher

`swf_event_dispatch.hpp/.cpp` implements the complete original RenderFX::SendEvent 0x7abf34. SwfEvent48 widens the original two pointers, retaining every other source payload word/flag byte. Source Event40 offsets are character +0, name +4, kind +8, value0 +c, X/Y +10/+14, value1 +18, buttons +1c, cursor +20, consumed +24, second flag +25, padding +26. Native offsets after the two pointers shift by eight bytes. Unknown words and flag are data, not invented policy.

The source mutable native receiver (`renderer+fc`, virtual +0) executes first. Only then is consumed reloaded. A nonzero consumed byte suppresses all ActionScript callbacks and selection writes. Native handler may change kind, name, target or recursively dispatch the same event. There is no once-only guard. Required service failure returns -2, keeping delivered prefixes. Native malformed pointers/null initial name return -1 before delivery. Caller must retain valid C strings, records and callback receivers; native caller validation is not a claim that original accepted malformed pointers.

| Kind | Source method |
|---|---|
| 0 | on_focus_in |
| 1 | on_focus_out |
| 2 | on_clicked |
| 3 | none |
| 4 | onPress |
| 5 | none |
| 6 | onRelease |
| 7 | onReleaseOutside |
| 8 | onRollOver |
| 9 | onRollOut |
| 10 | onDragOver |
| 11 | onDragOut |

Other unsigned kind words do nothing after native delivery. The no-argument AS invocation's source boolean is ignored. A missing receiver or method is therefore different from a missing native backend. Kind 6 additionally reloads event name after the synchronous AS call and writes the actual borrowed selection word: btnDelete -> 0x13, btn_GAMEPLAYMENUS_ACCEPT/REFUSE -> 1, btn_Legend -> 0x14, btn_deadzone -> 0x0c. These are direct stores, not invented notification callbacks.

Services24 has context, mandatory native_event, and conditionally required as_method. `dh2_ui_swf_send_event(Event48*, uint32_t*source_selection_global, Services24*)` returns 0/-1/-2. as_method reports delivered execution, including genuine source absence; it must not accept an unimplemented bridge. Callbacks must respect native receiver/storage lifetime through their synchronous return.

## Genuine retained core method bridge

`swf_event_core.hpp/.cpp` composes actual GameSWF character/environment/call_method with the complete original InvokeASCallback 0x7abe0c gates. `swf_event_method(lease,character,name,source_invoked,error)` requires exact retained graph lease and the facade's core Scope when callbacks use its services. Null character is delivered source false. Sprite input selects its own environment; non-sprite input rereads the actual weak parent for presence, sprite classification, then environment receiver. Missing/non-sprite/expired parent returns source false. Once selected, source retains INPUT character, obtains the selected sprite's environment, and calls the method with INPUT character as `this`, zero arguments. The return is converted/discarded through genuine GameSWF call_method; source true means the wrapper executed, not that a named method existed. A missing required environment is an explicit native provider failure.

The lease pins the exact graph through reentry and release; retaining the outer facade alone is insufficient. Borrowed renderer/native/game provider context must remain alive too. No arbitrary global registration, GUI actions, current-menu receiver or ownership is supplied here.

## Proof

`swf-event-dispatch-arm64-differential.json`: 2,400 complete original SendEvent executions versus O2 ARM64; 5,174 ordered native/AS services and actual global stores, zero mismatches. Includes all kinds, consumed bytes, both null/non-null character identities, unknown payload/IEEE bits copied exactly, callback mutation and 400 genuine recursive original/native dispatch executions.

`method-probe.json`: 32 original complete InvokeASCallback cases, actual weak-pointer/refcount routines, caller virtual class/environment and downstream AS execution services. Verifies sprite/parent/expired gates and INPUT-this versus selected environment. It does not claim original AS runtime execution.

`swf-event-dispatch-host-audit-v2.json`: actual private UI DSO sanitizer composition, 2,400 original gold cases, 4,377 ordered native/AS requests, 400 original-bound reentry cases, 32 corresponding genuine core method gates, 14 actual AS native fixture callbacks, 16 malformed/failure/ownership checks. ASan/UBSan/LSan zero findings. Host compares final selection word exactly; original/O2 proof additionally compares all write instructions/order. Generic callback bodies are fixtures, not game-action acceptance. The test genuinely retires expired parents from the GameSWF heap before weak-parent checks, rather than treating release of an external strong pointer as destruction.

Private UI DSO SHA ead53a5d0d239ea15871acd766f554c2def6ddb5b8e319999225c94fb62b0deb; no central library/APK changes were made. Central integration: add swf_event_dispatch.cpp and swf_event_core.cpp to engine UI; test tests/swf_event_dispatch.cpp links actual UI and uses `reference/swf-input-connection/dispatch-gold.bin` as its sole argument. The original/optimized build script is tools/build_swf_event_dispatch_oracle.ps1. Host runner is tools/swf_event_dispatch_host.py with --ui-library, --build-directory and a new --report path; existing reports are preserved.

## Full input prerequisites, not supplied by dispatch

Complete UpdateCursor 0x7ac924, UpdateInput 0x7ac4bc and Update 0x7ad68c are captured. UpdateCursor uses four owned 0x28-byte slots; rejects cursor indices >3. It stores raw X/Y and cursor ID on original root +48/+4c/+50, maps screen coordinates through original ScreenToLogical 0x773dc0, conditionally updates a retained cursor graphic and checks slot-enabled/context gates. It notifies root mouse state with converted logical integers and buttons ZERO at 0x7acb54. Original raw notification 0x774128 writes only fields; it does not dispatch immediate mouse move.

The later cursor body performs true sprite virtual topmost hit test (+68) with logical coordinates multiplied by 20, retain/drop of candidate/focus/pressed objects, focus transitions, local-coordinate GetLocalPosition, native CanHandleEvent (+8), mutable event delivery/consumption, and authored focus/press/release animation operations. Flags at renderer+f8 and per-slot enabled/retained fields determine these branches. Native state at renderer+fc is required; MenuFX.OnEvent 0x7adf20 selects the current state for the event character (0x7addb0) and invokes state virtual +2c; CanHandleEvent 0x7adee0 invokes its +34 or source true only when no states exist. A fake current state is not allowed.

UpdateInput delivers native event kind3 before subsequent navigation; consumed suppresses handling. Its real focus search uses complete object transforms and weighted direction distances, not button rectangles. Update(dt_ms,realtime) converts unsigned milliseconds through the original helper and /1000 then calls root::advance(float,bool), followed by pending animation completion events for four slots.

Original root::advance 0x775304 lacks the stock upstream mouse-drag/hit/generate_mouse_button_events prefix. Stock root::notify_mouse_state also introduces immediate MOUSE_MOVE. Thus stock notify+advance is not the recovered RenderFX cursor/event path. No full source input/frame or live Android parity is claimed. Required next backends are original cursor slot/flag producers, current MenuFX state/native game receiver, actual object hit testing/local transforms, focus and PlayAnim/animation-completion ownership, source root advancement/listener/GC policy, and Input/Application routing. The exact source globals NativeUsePotion 0x43d2c8, NativeAwayFromHud 0x43ab98, NativeRefreshHudManager 0x43a810 are captured separately; registering names without their real receiver/property/item/menu services is unsupported.

The current world milestone displays the player status subtree. That subtree is not the containing original controls/menu button tree. This module must not route arbitrary named buttons to game actions before the complete source receiver and cursor policy are connected.
