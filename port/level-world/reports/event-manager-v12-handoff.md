# Retained source EventManager V12

One new production TU: `port/level-world/event_manager_owner_v12.cpp` in the existing level-world target. No existing shared classes, Inventory/Item sources, renderer, CMake, globals or app lifecycle were changed.

## Exact method census

| Original method | Address | Source behavior |
|---|---:|---|
| C2 / C1 |33805c /3380bc|48-byte native base; empty receiver map8, pending-event list20 and delayed-detach list28. Tree flag8=0/rootc=0/count18=0. Self-linked sentinels produced; unrelated padding/field4 not fabricated.|
| Attach(type,receiver,priority) |338da0|Reject duplicate receiver identity within that type (return0). Otherwise append ReceiverInfo, priority stored unchanged, byte10=0, return1. No priority sort.|
| Detach(type,receiver) |33811c|Remove first matching receiver node20; return1, otherwise0. Empty type-map node remains.|
| DelayedDetach |3382a4|Find the actual receiver registration and append map/list-node references to delayed list node16. Returns1/0.|
| DropDelayedDetach |3383f4|Remove the scheduled receiver nodes in order, then clear delayed nodes.|
| Raise(event) |338ebc|Call event virtual8 getEventID; copy that type's ReceiverInfo list, then invoke snapshot receiver virtual8 with SAME event and SAME manager. Stop ONLY on result exactly1. Clean snapshot nodes after dispatch.|
| RaiseAsync(event) |339090|A single unconditional `b338ebc`: immediate synchronous Raise. No copy/enqueue operation.|
| Update(double) |33900c|Ignore double argument. For each pending pointer: Raise → pointed event virtual4 deleting destructor → unlink/free node12. Drain to empty, including additions during dispatch, then DropDelayedDetach.|
| Flush |3384ac|Clear pending list nodes, receiver map/list nodes, then delayed list nodes. It does NOT call pointed event deleting destructors.|
| D1 / D2 |3384f8 /338590|Reset vtable, clear pending nodes, delayed nodes, pending nodes again, then map/list nodes. No handler destructor or event deleting destructor.|
| D0 |338574|D1, then release enclosing allocation. The embedded Level base must not separately free its parent's allocation.|

IEvent virtual8 is `IEvent::getEventID31d8dc`: `ldr r0,[r0,#4]; bx lr`. QuestEvent's vtable selects that SAME leaf. Thus a typed event adapter must borrow the real event-ID field4, rather than infer a class tag. The original Objective handlers can mutate fields of the SAME event; source const-reference does not imply cloning. GameEventManager (`479cf4/479d08` three-pointer-vector constructor) is a distinct subsystem and is not this EventManager base.

## Actual Level composition

Embed ONE `events::EventManagerOwnerV12` into the retained Level graph at its EventManager C2 step. Construct it with the actual retained base identity supplied by the Level owner, not a fabricated global ID. The parent owns allocation/publication/destruction and the SAME current-Level/GSLevel slot. This owner neither publishes a Level nor changes loot gate150.

```cpp
events::EventBorrowV12 event {
    actual_payload_identity, actual_payload_context,
    actual_get_event_id_field4, actual_containing_world_lease
};
events::EventReceiverV12 receiver {
    actual_receiver_identity, actual_receiver_context,
    actual_on_event_virtual8, actual_receiver_lease
};
bool inserted;
same_level.events.attach(actual_event_id, receiver, actual_priority, inserted, error);
same_level.events.raise_async(event, error); // dispatches before returning
```

Callbacks use `bool` for required backend availability/failure and a separate signed32 source handler result; failure is not source result0. Handler context remains the actual receiver. The owner passed to the handler exposes its original base identity via `identity()`. Payload context must refer to the actual event fields; no second mutable quest state or event clone. Attach metadata is one retained map/list authority. Receiver snapshots pin actual native leases through synchronous dispatch, so detach/Flush during a callback preserve the snapshot membership. The caller owns the normal single-threaded engine invocation contract; this module adds no threading system.

The existing `LootPickupQuestServicesV10` comment described339090 as copying/owning an async stack event. That comment is contradicted by the original whole function. Bind its callback to synchronous dispatch of the SAME stack event through this owner. Do not enqueue or clone it on that address. Actual Objective registration, Quest/GameEventManager construction, source constants, positive handler logic and source Level frame invocation remain required providers; this module does not accept no-op quest receivers.

Delayed registrations remain attached through synchronous Raise. Deliver the actual source EventManager Update when the Level's original frame path reaches it; do not automatically drop them at the end of Raise.

## Pending ownership boundary

The constructor genuinely owns an empty pending20 list. No actual enqueue/clone producer has been recovered, and RaiseAsync is conclusively not one. `import_pending_from_required_producer_v12` is explicitly a typed source-storage adapter requiring a real producer callback, plus real getEventID/deleting-destructor providers. An absent producer fails clearly before any insertion. This is not a recovered engine enqueue function and must not be wired to quest RaiseAsync.

The positive native/original tests inject storage through explicitly declared fixture producers only. They verify the complete dispatch/delete/pop path and show that Flush/destruction free only list nodes. `PendingEventV12.event.lifetime` must pin a containing native arena/World rather than act as a second owning event deleter: its `deleting_destructor` callback is the source event-delete site. Flush/destruction do not call that callback. Pending processing preserves required-failure prefix and refuses replay after failed handler/deleting-destructor delivery; Flush deliberately clears the entire graph and resets that failure latch.

## Clearly labeled modern safety corrections

Original DelayedDetach can append the SAME raw registration node twice; Drop then double-frees it. Detach followed by later Drop likewise leaves a dangling scheduled raw node. The native owner uses host registration tokens, coalesces duplicate scheduling, and ignores tokens already removed by immediate Detach. A newly reattached receiver has a different token and is not removed by an old schedule. These repair legacy indeterminate lifetime behavior and are not claimed as source parity in those invalid cases.

Destructive queued Update reentry and Flush during queued Update are rejected before destructive mutation. Synchronous nested Raise and source-safe Flush during Raise are supported. Required provider failures latch to prevent duplicate delivery. These are bounded modern native-host guards; source normal-path order is preserved.

## Verification

Original ARM oracle executes1610 constructor/Attach/Detach/map-tree/list/DelayedDetach/Drop/Raise/RaiseAsync/Flush/Update/D1 sequences. It compares insertion order, signed priorities, byte10, exact return flags, delayed count, event mutations and dispatched identities; extreme type/priority values included. Three positive pending lifetime cases execute Update, Flush and D1. Source allocator and abstract event/receiver virtuals are declared fixtures. Source native map rebalance/traversal and receiver snapshot allocations execute. Undefined duplicate/dangling delayed-node cases are excluded from source parity and checked separately as modern repairs.

Current native owner passes33,012 checks against those original snapshots, plus detach/Flush during snapshot delivery, pending dispatch/delete/pop, non-deleting Flush/destruction, required missing-producer/failing-handler/failing-delete prefixes and modern delayed-node safety. Strict ARM64 owner/test syntax PASS. Native receipt links APK11c8219e but compiles this new owner inline; it is isolated native evidence, not a live Level/quest success claim. Root still supplies the actual Level constructor/current-Level lifecycle and actual registered quest receivers.

Evidence: `reference/event-manager-v12/{symbols.json,base-original.asm,original.asm,payload-symbols.json,original-audit.json,fixtures.bin}`; native receipt `reports/android-native-owner-tests/event-manager-owner-v12/receipt.json`; syntax receipt `reports/event-manager-v12-arm64-syntax.json`. Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
