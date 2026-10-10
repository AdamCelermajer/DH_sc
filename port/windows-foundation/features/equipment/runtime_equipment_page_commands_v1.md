# Runtime Equipment page command handoff V1

`RuntimeEquipmentPageV1::release` returns `RuntimeEquipmentPageReleaseV1`. For original Details `drop`, `auto_equip`, and `transmute` hits, the release now carries a one-shot `PendingCommand` with the exact selected inventory instance ID and selected source slot. These are requests; the page does not remove items, award value, or transmute them. The `SourceCompositionV1` callback path retains the same typed request until the root calls `take_source_pending_command`; another release is rejected while that command or a render receipt remains unconsumed.

`request_auto_equip` can be consumed through `RuntimeEquipmentBindingV1::auto_equip(instance_id, error)`. That entry reaches the existing `EquipmentAdapter::auto_equip` and frozen `dh2_equipment_auto_v3` kernel. The kernel reads the actual ItemTable source row's `word26` slotting and type. For types other than bow/staff (4/5), it applies the live option flags: slotting 1 plus flag1320 becomes dual-hand `-3`; slotting `-4` plus flag1324 becomes one-hand slot 1. Direct slots 0..8 equip there, with slot 2 first checking and possibly clearing the two-hander at slot 1. Slotting `-3` chooses hand slots 1 then 2; `-2` chooses armor slots 5 then 6; both pick the first free slot and return no-op if both are occupied. Slotting `-4` clears slot 2 then equips slot 1. Other slotting values return source result 0. The adapter then runs its existing gear/property/visual transaction and queues the same-session render receipt; it does not invent an item power or an equipment slot.

Drop still needs the root's actual world-item owner and policy; transmute still needs its source owner. Both remain typed, non-mutating requests until those owners are connected. No UI action is silently treated as success for either operation.

Verification:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/equipment/run_runtime_equipment_binding_v1_tests.ps1
```

The strict LLVM-MinGW integration test passes the source-composition release path for all three requests, asserts the preserved instance/slot, confirms unconsumed requests block overwrite, verifies drop/transmute leave the shared state alone, and consumes auto-equip through the existing source kernel with a same-Scene receipt.
