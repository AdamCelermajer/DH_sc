# CharAI owner association

The original 148-byte `CharAI::SetCharacter` writes the supplied nonnull
Character into `CharAI+0x4`, leaving all other fields unchanged. Both source
Character constructor callsites are pinned in the manifest. This is a separate
step after embedded AI construction; the original CharAI constructor leaves its
owner field uninitialized and appends its own identity to the global queue.

This native implementation covers the valid nonnull gameplay branch. It rejects
null before effects instead of reproducing assertion diagnostics or a null
dereference. It does not create or promote an AIS, set `alive+0x48`, force room
membership, replay queue registration or initialize scripts.

The maintained runner checks original ELF bytes and both constructor BL
destinations, executes the actual association function, and verifies every
original byte except the four-byte owner field remains unchanged. Native host
checks retain identities above 4 GiB and verify rejected input preserves state.

The new Android owner can call this bounded source step for each retained
Character/AI projection. That does not reconstruct the entire Character
constructor or establish live AI gameplay.
