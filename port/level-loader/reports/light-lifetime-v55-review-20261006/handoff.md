# Light teardown and 51-definition coverage review V55

This packet consumes the already completed original ARM PASS proofs. No build, WSL, sanitizer, Unicorn or device job was launched in this continuation. The replay script is provided for later admitted validation; it has not been rerun here. Renderer/GPU behavior, full InitPost and whole-level readiness remain unverified.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Existing V53 and V54 archives and canonical source files are untouched. Source-order changes are proposals for Main adoption.

## Reached original bodies and limits

| Proof | Cases | Observed behavior | Limit |
|---|---:|---|---|
| LightPoint C1 40bdd8 -> D1 40bd1c -> LightBase D2 40ab34 -> ObjectBase D2 33e998 | node absent / present | CString180 released first; genuine IReferenceCounted.drop31d584 decreases node ref2 to1, zeroes actor120; CString168 then ObjectBase fields released | Independent root pin remains; final node deletion and renderer/GPU are not reached |
| LightSetManager.ResetLightSets40cbc0 | empty, one, six, all25 slots | All25 CLights refs released and slots cleared; field1a0 cleared; four Names and automatic vector190/194/198 preserved | Independent CLight ref1 remains; no final native/GPU free |
| LightSetManager.operator=3541d4 and vector assignment | fresh-layout empty, fresh-layout retained light, self-assignment | Non-self updates existing four Names, releases old CLight refs, resets auto length while reusing buffer, copies1a0; self preserves state | Explicit initialized source layout; fresh constructor/driver not supplied |

SceneManager424/428/42c is the SAME manager automatic vector: embedded LightSetManager starts at SceneManager294 and has vector190/194/198. It holds borrowed actor pointers. Actor destruction does not remove this borrow. ResetLightSets also leaves it unchanged. A standalone actor release without quiescence and later actual SceneManager clear can leave a stale actor address for `_RegisterAutomacticLights353bac`.

## Precise Main cleanup adoption

Attach these checks to Main's one existing Level destruction path. Do not add a second Level.Unload owner or replace driver/cache cleanup with loader ledger resets. Level.Unload3f0690 saves players and tears down network/menu state; it is not the light destruction path.

1. Retain the SAME current Level/Application/ObjectManager/SceneManager/embedded LightSetManager owners and their generation. Establish owning-thread quiescence covering renderer traversal, automatic registration, RefreshAttachment, scheduled updates and in-flight callbacks. No automatic-list reader may run between steps2 and3.
2. Reach Main's actual ObjectManager.Flush3496b8 at original LevelDtor3f91c8. This destroys the actual GO19 receiver through its normal deleting destructor. Release only the actor's genuine intrusive node reference, separately from parent graph/node/resource/light-set pins. The supplied V53 node.owner must represent this exact actor reference, not a cache pin or no-op deleter. Captured callbacks/services must not retain a second actor pin that postpones this drop.
3. Continue the existing source LevelDtor dispatch at3f91ec, SceneManager virtual68 -> actual SceneManager.clear354468. It drops retained scene nodes44c, calls genuine CSceneManager.clear589888, constructs and assigns a fresh embedded LightSetManager then destroys the temporary, calls CResFileManager.unloadAll659c3c and material/texture/driver cleanup, drops node438 and clears28c. ResetLightSets alone is insufficient because the automatic borrow list remains.
4. On the same retained owner/generation, establish the old automatic-list logical length is zero, no scheduled/renderer reader can borrow old GO19 identities, and the actual flush/clear bodies completed. Physical vector allocation may remain; capacity reuse is source behavior. Four Name fields remain the same receiver fields, assigned default contents by manager assignment; do not replace Main's Names owner with a parallel owner.
5. Only then release loader XML/factory/transport journals and reopen transition polling. Preserve real outstanding parent/resource/encoded/decoded/GPU owner pins and charges until their actual release. If a binding/body/postcondition fails, keep the old owner pins and quiescence barrier; report the exact failure. Do not claim rollback or reset caches/ledgers unconditionally.

V53's implicit C++ member destructor drops node_ before strings_, contrary to the reached source order. `canonical_light_point_destructor_v55.inc.proposed` is the small explicit ordering proposal: attachedTo CString180 -> node actor reference -> dae CString168. It requires Main's actual node-pin deleter and quiescence. It does not supply native node construction/destruction, parent graph removal, automatic removal, driver behavior or full ObjectBase destructor parity. Main must validate source ordering using real provider pins after adopting it. The proposed hooks header describes existing-body bindings only and is not an implemented teardown controller.

## All51 bounded coverage

The matrix executes original TinyXML for every supplied definition and inventories original filename edges against exact V52 cache paths. It does not execute every generated GO4/config publication, selected pipeline, stage9 ingestion or light InitPost.

| Inventory | Count | Meaning |
|---|---:|---|
| Both consumed light_set and fixed_light_set attrs present | 45 | Authored filename availability; original mode selects one alternative |
| Regular consumed attr only | 1 | RANDOMFORALEX; absent fixed filename requires real generated config InitPost default |
| No consumed filename attrs | 5 | Default may arise only through actual config creation/InitPost/publication |
| Missing consumed filename routes | 0 | Exact cache availability, not runtime readiness |
| Direct fixed-definition LevelConfig candidates | 15 | Eligibility, actual InitPost/publication, pipeline and providers still required |
| Procedural definitions | 35 | Need generated GO4/LevelConfig owner from the actual procedural path |
| Fixed debug definition with no direct config | 1 | MULTIPLAYER_TEST; no invented GO4/config |
| Original raw TinyXML parser errors | 6 | Explicit source-prefix diagnostics; no silent readiness |
| Colon light_set_str metadata | 2 | ICY_HUB/MACHINE_ROOMS_01 unmapped descriptor ignored by source PropertyMap; not resource failures |

Six raw rule parser errors: GOTHICUS_CRYPT_02, QUESTTEST, ROOT_DEN_01, SECRET_BASEMENT_01, VOID_MAZE_01, VOID_MAZE_02. The five definitions without consumed filename attrs are MULTIPLAYER_TEST, QUESTTEST, VOID_MAZE_01, VOID_MAZE_02, VOID_MAZE_03. Source InitPost defaults both fields to data/3D/Light/default.lightset_xml, conditional on reaching actual config InitPost; fixed/regular never falls back to the other authored mode.

The per-definition table preserves SWAMP and SWAMP_02 as separate identities. Main still owns actual native-light creation, root attachment, automatic registry, CLight type writer, RefreshAttachment, light-set driver and source runtime cleanup.

## Reproduction after runtime admission

Use the provided replay_original_bodies_v55.py with `--loader` pointing at this private port/level-loader, `--engine` at the exact ELF, `--python-site` at the retained Unicorn/pyelftools dependency site, and `--out` at a NEW scratch directory. It executes the copied original class definitions and writes fresh proof reports without modifying frozen outputs. The current dependency site is C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages. Use the already configured Python runtime. This command is subject to Main's restored process admission.

The lightweight packet verification below checks the preserved report invariants and table bounds. It consumes archived PASS results; it is not a substitute for a fresh original execution or runtime integration proof.

