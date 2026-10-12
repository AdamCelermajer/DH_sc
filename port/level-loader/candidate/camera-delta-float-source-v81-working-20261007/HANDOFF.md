# Same camera-delta fields and direct typed consumer

Targeted source-only paired delta against current Root. No Root writes, tests,
compiler/build/runtime/hash/freeze or independent device jobs.

Original complete state34 stores __aeabi_fsub results in SAME Level19c/1a0/1a4.
They are float values, not native pointers. Split only these three existing
constructor fields into typed floats; C1 zeroing and all unrelated fields stay.
The existing source_campaign_runtime_v61.cpp update_camera_fog consumer now
reads those values directly instead of integer-converting/bit-punning them.
All existing camera/fog calculation/order, config and Scene methods stay intact.

Adopt camera-delta-float-source-v81.patch together with the complete state34
and same-RetainedCharacter Stage80 restore contract. Do not copy the full review
snapshots over newer Root changes. All affected owner/consumer headers require
coherent rebuild at the BIG integrated milestone. This fixes type consumption;
actual camera/environment/restore runtime acceptance remains unverified.
