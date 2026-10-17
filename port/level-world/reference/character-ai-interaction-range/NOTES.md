# Live interaction range

`character_ai_interaction_range.{hpp,cpp}` maintains the complete original
`CharAI::AI_IsInInteractionRange(GameObject const*) const` caller at `0x3d4f98`
(388 bytes). The manifest pins 12 original function ranges and the primary
GameObject/Character vtable prefixes through virtual slot+94. One new caller
orchestration is claimed; no new complete callee body or native integration is
claimed. Existing snapshot `dh2_ai_range` remains unchanged.

## Source order

1. A null argument captures original AI target+40 once; absent returns false
   without service calls. The selected candidate stays fixed through callbacks.
2. Fresh owner GetTargetPosition returns a borrowed point. Candidate
   GetInteractionSpot returns a **copied** point and may update its cache.
   Read the borrowed owner coordinates only after that second callback.
3. Subtract owner-minus-spot x/y/z; square x then y; add xx+yy; square z and add;
   take sqrtf. Every operation rounds separately to binary32. Distance remains
   fixed through subsequent callbacks. No vertical cutoff or clamping is added.
4. Read the candidate's interaction node+2e8 after distance/sqrt. If nonnull,
   skip radius/property services, use positive-zero radii and threshold80.
   Otherwise query original AI GetMeleeRadius, candidate virtual+94
   GetInteractionRadius, then **fresh owner** GetCharAI and read returned AI
   row+18 (InteractRadius, words6 of the existing 68-byte row).
5. Subtract the first radius then the second radius separately, including both
   positive-zero subtractions on the node branch. Candidate virtual+90
   GetInteractionType receives another fresh owner. Type8 uses remaining
   distance<=positive-zero; every other raw type uses distance<=threshold.
   Threshold/distance are cached before this virtual callback.

The caller adds no handle resolution, Character conversion, state, alive,
visibility or targetability query. It is suitable for non-Character interactive
targets when genuine virtual/property/spot providers exist.

## Interaction spot is a stateful boundary

The service is the exact `GameObject::GetInteractionSpot` call at `0x38b228`
(164 bytes), not a target-position alias. Its original body checks byte+2ec.
On the first call it sets that byte to1, queries visual+2d8 for the named node
`interaction_position` when a visual exists, and writes the result to node+2e8
(including null). A cached node supplies a copied absolute-position point;
otherwise a copied GetTargetPosition is returned. Subsequent calls skip lookup.
The service returns a copied Point plus borrowed ObjectFacts containing the
same candidate identity and live node+2e8. Port validation must not roll back
those cache changes if a later provider fails.

The ARM comparison executes this original body, the real node absolute-position
leaf at `0x597180`, and GetTargetPosition at `0x3935dc`. Visual named-node lookup
is an explicit fixture provider. The compiled host fixture models this service
boundary; a separate maintained full GetInteractionSpot body is not claimed.

## Owning property and virtual boundaries

State/Point reuse the frozen sight views; AiRow reuses enemy-retention's row.
These are header types only; production cpp has no older cpp link dependency.
The melee-radius operation can bind the frozen `get_radius` orchestration;
its subject remains the original AI even after owner mutations. The focused
host test links frozen melee source and routes its generic fallback through
this new caller with independent output storage.

GetCharAI must implement genuine captured global row-base/GetCharAIId behavior,
including fallback8, rather than a guessed ID/modulo. The original comparison
executes its 48-byte body and the real 56-byte GetCharAIId. The compiled source
borrows the selected row and reads words6 after the service returns.

Virtual offsets+90/+94 are measured from the primary vtable address point
(`_ZTV...+8`), not its symbol start. GameObject slots map to GetInteractionType
`0x38ad74` and GetInteractionRadius `0x38ad7c`; Character overrides are
`0x3a47e8` and `0x3a374c`. Vtable bytes are hashed through ELF PT_LOAD mapping.
These callee bodies remain explicit borrowed providers.

## Lifetime, failure and proof bounds

One owning thread retains the original AI, selected candidate, every owner,
borrowed point/ObjectFacts/row and provider context through return, including
retired backing after replacements. Identity keys remain stable. Providers may
change live owner/target/backing facts; same-State reentry into this caller,
output/service overwrite and borrowed destruction are forbidden. Independent
calls may nest. Alignment/known aliases/required row or fact identities are
port guards and do not claim original error branches. Missing, failed or
throwing services preserve already completed source actions with no rollback.

Comparison executes the complete 388-byte original caller plus the listed spot,
position and property leaves. Original-AI melee radius and target virtual
radius/type are observed explicit fixture providers. External fsub/fmul/fadd,
sqrtf and fcmple library imports are modeled IEEE binary32; finite words compare
exactly and NaN outputs compare unordered class, not payload/sign propagation.
This is component proof. Native owner binding, renderer-backed node lookup and
Android gameplay are outside this report.

The first checkpoint misidentified PLT `0x30e9ac` as `fcmplt`. Independent
execution of the relocated stub resolves it to `__aeabi_fcmple`. The corrected
source accepts equality at zero and at the property/node threshold. Its runner
now verifies actual import identities before modeling arithmetic. Historical
interaction reports from the first checkpoint are superseded by this correction.
