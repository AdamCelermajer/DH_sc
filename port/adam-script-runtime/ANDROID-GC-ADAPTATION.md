# Android weak-table GC adaptation

The imported Lua 5.1.4 core remains attributed to Adam's pinned import. This local change affects `lua/lgc.c` only; the vendor reference is unchanged.

The first native monster initialization APK (`f5a4fbd391db5f099b2c3a0339a2776bb8705b2ae6817e329a0862959b59494d`) aborted on API37 during native closure registration. Android FORTIFY rejected `strchr` in `traversetable`: the Lua string payload follows a union subobject, whose compiler-visible extent does not describe the trailing allocation.

The replacement scans no farther than `TString.len` and stops at the first NUL, retaining the original `strchr` interpretation of weak modes `k` and `v`, including embedded NULs. FORTIFY remains enabled. This is a platform adaptation, not a newly reconstructed original game function.

`tests/run_weak_mode_gc_host.py` validates eight weak-mode cases and 8,000 native closure registrations with real GC. The corrected APK `46e372a41d900666ed17899bb62674f3bf68c1b35212c2251f974359406465ab` runs both original Ghost OnInit callbacks and survives reload/rotation on API37 with 16 KiB pages. Whole original VM parity is not claimed.

Evidence: [current checkpoint](../../docs/NATIVE-MONSTER-INITIALIZATION-CHECKPOINT-2026-10-04.md).
