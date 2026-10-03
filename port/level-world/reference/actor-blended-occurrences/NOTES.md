# Occurrence-aware Prince two-slot binding

The additive `BlendedPlayback::compile_dynamic(bank, RegistrationSet, scene,
binding, error, mismatch)` keeps the existing static and explicit unique-order
overloads. It compiles every source library occurrence under its engine index
and copies the refreshed dictionary lookup plus engine-to-dictionary metadata.
The compiled resource/event storage is owned; the registration may die before
playback. The explicit canonical `ClipBank` remains alive and immutable.

`engine_index(dictionary_id)` resolves the source first-resource occurrence.
`dictionary_id(engine_index)` reports that occurrence's original dictionary ID.
`current_clip()` and observer events retain requested dictionary IDs;
`current_engine_clip()` and the timeline use engine occurrence indices.
Root and pose sampling use the same selected compiled record and retained
per-target key cursor. No second dictionary lookup is used for root sampling.

The bounded binding requires a fresh detached coordinator, complete refreshed
map, one canonical bank Player per resource identity, and a matching registered
default. Every bank Player must occur in the registration. An alias dictionary
key may point to an existing canonical bank Player; if its dictionary key is
also present in the bank, its pointer must match that bank entry exactly.
Distinct Player objects containing equal bytes are not accepted as a substitute
for the source shared resource identity. Validation/compilation occurs in a
temporary coordinator and commits only on success.

The actual producer evidence is
`engine-animation/reference/prince-registration/probe.json`, `load-probe.json`,
and the separately executed `animation-registration` instruction corpus.
Prince registers 158 occurrences backed by 116 resources. Library zero is the
template dictionary1111; both constructor slots select engine0 with loop1 and
zero cursors. The empty dictionary1138 resource remains engine155 with raw
INT_MAX/INT_MIN bounds. It is retained without manufacturing playable bounds.

The original `PlayClip` at0x47680c saves the incoming slot's engine selection
through0x65f114 before invoking0x3674ac. That setter resolves dictionary identity
through0x3660f4 and selects the engine index through0x65f8c8. The comparison
at0x4768b4 uses those engine indices. The native same-engine replay compares
`compiled_clip`, including dictionary aliases, and preserves the original
extra-time jump after SetClip has cleared ended. Requested dictionary metadata
is updated independently.

`tests/actor_blended_occurrences_reference.py` executes72 actual original
PlayClip cases, including differing dictionary aliases, prior engine0/2/7,
ended0/1, and extra0/17/-17/INT_MAX. Its fixture supplies resolved resource
indices, timeline bounds, event installation and root NewAnim services. It
executes the actual blend, selection, timeline clip and jump routines. It is
not an original complete frame or asset ownership oracle. Gold and complete
instruction-service records are saved here as `playclip-fixtures.bin` and
`original-playclip.json`.

`tests/actor_blended_occurrences_host.py` builds its new test against genuine
sanitized production shared libraries and binds source, executable, library,
asset and evidence hashes. The full source bank verifies158 bounds, seventeen
source first-index selections,131 scene frames,102 root-history checks, two
authored callbacks carrying dictionary955 metadata from both slots, copied
registration lifetime, synthetic alias occurrence159 mapping to first engine2,
eleven atomic compile rejections, and the source Blend-before-missing-map
failure order. Event frames execute the caller's explicit animator phase after
the scene phase; a pending completion correctly defers selection until that
phase. World Step is outside this test.

The same acceptance also regenerates the old static/dynamic capture bytes and
requires their original-bound hashes to remain exact. Existing original-derived
scheduler48, selection72 and control2160 corpora replay through current native
libraries. Report: `reports/actor-blended-occurrences-host-audit.json`.

The refreshed independent full-bank DSO proof is
`engine-animation/reports/prince-bank-current-integration-host.json`:26,228
raw samples, all158 bindings/bounds and sample-after-Player-destruction checks.
It binds current animation sources and the exact staged cache assets. Prior
historical reports remain unchanged. These are bounded source/composite native
proofs; no whole original-frame, gameplay FSM, GPU or APK differential is claimed.

Production freeze:

* actor_blended_playback.hpp SHA256
  `6bfe572044fa203f877acdd500fe48e2a3eef41886a6fd0a673694b02bef3bf2`
* actor_blended_playback.cpp SHA256
  `139ebade8b14e07771c1a9a34d4996fa9940dcf99b05e291db3a07f1056cfa3b`

Reproduction on this Windows/WSL workspace:

```
python port/level-world/tests/actor_blended_occurrences_reference.py
python port/engine-animation/tests/prince_bank_integration.py --output port/engine-animation/reports/prince-bank-current-integration-host.json
python port/level-world/tests/actor_blended_occurrences_host.py
```

The host dependencies must first be built in the supplied sanitizer build
directory; the host runner compiles its thin acceptance executable itself.
The parent owns CMake, renderer initialization, dictionary bank lifetime and
explicit restored-state reselection. This handoff changes none of those files.
