# Character zonability leaf

The original `Character::IsZonable` body at `0x3a36e4` is fully executed in the ARM oracle. It calls the virtual `Character::IsPlayer` slot at vtable offset `0x28`; a nonzero raw result returns false immediately. Otherwise it calls `Character::IsFaerie`, whose real body at `0x3a3094` calls `GetCharType` and compares the fresh type word with `3`. A nonzero result also returns false. All other characters branch to `GameObject::MeetCondition` at `0x38ab60`, whose complete source body returns true.

`GetCharType` itself reaches the AI table through `GetCharAI`; this leaf audit supplies the resulting type word at the exact call boundary. Player classification is likewise supplied at the original virtual-call boundary. The helper `character_zonability` exposes these two facts as ordered, synchronous providers and retains the full-width `Character` identity across callbacks. Its implementation is host-portable and does not overlay a 32-bit game object.

The host suite tests short-circuit order, raw nonzero words, type `3`, base-condition success, provider errors/exceptions, callback-driven owner replacement, pointer-width retention, and argument guards. The ARM differential executes `IsZonable`, `IsFaerie`, and `MeetCondition` from the pinned ELF and compares each result and normalized top-level predicate order with the portable implementation.

This is only a leaf dependency for CharAI. It does not enroll objects into a zone, set `in_zone`, supply the full zone membership/list producer, run the AI frame, or establish live Ghost pursuit. The exact native path must still supply authentic Player/GetCharType facts and actual zoning owner state.
