# V56 source-order LightPoint successor adoption checklist

This is a UNIQUE source-only proposal. FrozenV53/V54/V55 and root/selected source are untouched. No build, WSL, sanitizer, oracle, emulator or native provider was launched/implemented. The original PASS reports are consumed as provenance, not rerun.

The earlierV55 ordering proposal used node.owner.reset() as its possible native-drop boundary. This successor supersedes that proposal at this boundary: owner is a passive runtime pin; a distinct unique MainLightActorNodeReferenceV56 capability invokes Main's actual actor node reference drop. A shared_ptr decrement is never accepted as evidence of that body running.

Required adoption changes form one unit: interface header; same retained GO19 class successor header/source snippets; factory/service types usingV56; Main native construction/scene lookup returning the exact native-reference capability plus passive pin; Main's existing ObjectBase continuation; Main preflight and barrier in its existing Flush/Clear path. These .proposed files are not a compiled patch and should not be installed piecemeal. The current constructor/factory allows absent providers in source tests; adoption must reject unsupported runtime publication before InitPost and supply a destruction lease for every created candidate, including failed/no-node candidates. Never throw after retaining an unarmed native reference or allow normal shared_ptr destruction as cancellation.

Binding provenance: Main must identify the native drop function/body and actual receiver/refcount ownership in its runtime provider. Native reference token owns exactly actor120's intrusive reference; parent graph, lightset and cache pins are independent. Passive owner pin and its captured service callbacks must not duplicate that actor reference or drop it later. The provider must tolerate final native node deletion on the source drop, and caller cannot dereference that node afterwards. No loader fake/native renderer implementation is permitted.

Same-owner checks compare runtime shared ownership/control block AND runtime identity, generation and actual Application/Level/ObjectManager/SceneManager/embedded LightSetManager stamps. Actor, node and CLight identities must match. Main validates an owning-thread barrier token and synchronous native release capability. A source-only interface cannot prove a malicious/no-op provider is genuine: Main's actual runtime binding/proof is an explicit acceptance requirement.

Main sequence: retain barrier in Main beyond all actor destructors; preflight/arm EVERY retained GO19 candidate with the same lease; if any arm fails, abort before ObjectManager.Flush and retain actors/journals/pins; execute actual ObjectManager.Flush3496b8 at LevelDtor3f91c8; destructor releases180, invokes genuine native actor node drop, clears120, releases168, then reached ObjectBase continuation; continue actual SceneManager virtual68/clear354468; verify auto-list logical length0/no retired actor readers on the same generation; only then release loader journals and Main's barrier. ResetLightSets alone preserves raw automatic-list borrows and is not an alternative. Do not detach individual raw actor entries as guessed source destructor behavior. Physical vector capacity reuse is allowed.

The destructor's terminate-on-invalid guard is a proposed fail-closed integration invariant, not recovered source error behavior. Returning from an invalid C++ destructor would automatically release members and lose the pins, so it cannot be treated as a recoverable false result. Expected provider/quiescence errors must be rejected in preflight before Flush. Main's genuine drop/base callbacks are noexcept and infallible once armed; an actual failing/async provider cannot be adopted through this interface. Main must decide whether to accept this strict native-destructor contract or keep the current candidate entirely pending. No unchecked fallback is offered.

Future admitted tests, using the genuine Main native provider (do not run during RAM hold):

| Case | Required observation |
|---|---|
| Source node absent; actual base provider |180 freed; no native drop;120 stays0;168 freed; same-actor base continuation once |
| Source node present, ref2 independent root1 |180 free BEFORE actual drop; at drop120 still original node; native ref2->1; clear120 AFTER drop returns;168 then base; passive reset not a second drop |
| Final native actor reference |Drop can invalidate/delete node; no node dereference afterwards; real remaining parent/cache pins accounted by Main |
| Alias passive runtime pins/callback captures |Copies cannot duplicate actor120 reference or postpone actual native drop; callback token consumes once |
| Foreign runtime/control block/generation/manager/actor/node/light |arm returnsfalse before ANY field mutation; no Flush, no drop/base, all pins/barrier retained |
| Missing drop/base capability or false quiescence |Reject before publication/Flush; no permissive default success callback |
| Actor reference exists while120null |Reject stale/abandoned capability before mutation |
| Repeat same arm |Idempotent same lease; different lease rejected |
| Partial InitPost failure after actual node construction/root/auto |Same original GO19/source reference retained; Main preflight/Flush/Clear owns cleanup; no journal-only reset |
| Raw auto-list retirement interval |Quiescence forbids _RegisterAutomacticLights/readers after actor deletion until actual SceneManager.clear resets manager vector; ResetLightSets still preserves list |
| Base continuation/source field ownership |Main consumes same canonical actor ObjectBase fields in reached source order once; no double release by remaining class members; no ARM body call on host class layout |
| Repeated load/destroy/transition |Each generation owns unique node/reference tokens; old barrier released only after clear proof; no old GO19 pointer readable |

Known open integration points: Main's genuine provider classes; constructor/publication admission; failed-candidate disposal; whole ObjectBase field parity; native-light/final-node/GPU behavior. This proposal demonstrates precise source ordering and checked boundaries only. No GPU/full InitPost or completed integration claim.
