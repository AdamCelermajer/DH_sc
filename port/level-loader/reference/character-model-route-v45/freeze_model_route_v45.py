from pathlib import Path
import hashlib,json,zipfile
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/character-model-route-v45';reports=base/'reports'
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  while b:=f.read(1024*1024):h.update(b)
 return h.hexdigest()
receipt=json.loads((reports/'character-model-route-v45-verified.json').read_text())
assert receipt['validation']=='PASS bounded ordered actual model-stage route' and not receipt['whole_InitPost_success']
assert receipt['no_archived_actor_factory'] and not receipt['constructor_model_template_preselection']
for p,h in receipt['sources'].items():assert sha(Path(p))==h,p
for p,h in receipt['selected_sources_unchanged'].items():assert sha(Path(p))==h,p
cache_sha=sha(Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
receipt.update(native_runner_sha256=sha(ref/'build_character_model_route_v45.py'),original_cache_sha256=cache_sha,original_elf_proof_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',design_fixture_sha256=sha(base/'tests/native-ctor-v4/design.bin'),ordered_stop_entry='0x38be5c',app_random0_calls=8,owned_faery_original_table_model33=True,original_dae_transport_readback=True)
(reports/'character-model-route-v45-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
doc='''# Character model route V45: native prefix verified

The candidate removes the actual family model request's AI-only shortcut. Every reached 3a54d4 request delegates to the main provider. The new stateless route checks the actual factory record, live actor/property/cache pointers, current error-free NPC InitPost request and pinned model dictionary before invoking the original-proved V38 model kernel. No selected V5/common source changed.

| Subtask | Status |
|---|---|
| Existing LoadBase3df2a4 / Recalc3e0810 and original order audit | Finished V41 |
| Actual family3a54d4 routing candidate | Native verified |
| Same record/property/cache/dictionary ownership checks | Native verified |
| Existing NPC request as readiness witness; failed request guard | Native verified |
| Actual NPC and NULL-master DefaultFairy model/scale prefix | Native verified |
| NonNULL-master Faery using genuine save getter and original table | Native verified with explicit domain callbacks |
| Missing order/dictionary/type/backing/saved services | Native verified required-service stops |
| Required whole GameObject InitPost38be5c | Explicit pending boundary |
| Whole Character/level activation and rendered/playable SWAMP | Pending main providers/renderer |

Original ordering remains SafeProps -> LoadBaseProperties -> Recalc(true) -> GetCharModelName -> nonNULL model290 copy -> source scale -> whole GameObject.InitPost. Positive tests use genuine canonical factory records, actual property declarations/defaults/overrides, existing actual base/recalc providers and the same application spawn RNG. They stop honestly at missing whole GameObject InitPost38be5c after model/scale production. Whole InitPost never succeeds in this proof. No model/template row is selected in construction.

DefaultFairy resolves actual ModelFile34 to Celeste through its current NULL-master branch. A later nonNULL master requires saved-current(-1) and Faery-model services. The native test uses genuine retained PlayerSavegameV1.current_faery and original FaeryTables. Master assignment, master/save association, difficulty1 and DEFAULT Faery list are explicit test-domain producers; they do not prove main SG/GetCharFaeryListId or player equipment integration. DebugSwitches and offline/network callbacks are also explicit fixtures.

The ownership route keeps a weak factory to avoid record/service/factory cycles. The model dictionary is pinned independently by the actual table owner. An invalid original model returns NULL before type lookup and preserves the authored model290 string. Failed model prefixes cannot replay through initialize, and a stale direct request after owner failure is rejected.

The first native attempt correctly stopped before compilation because main's active PlayerSavegame header evolved beyond coherent V5. The matching genuine pre-extension source was recovered from .local-inputs/publication/checkpoint/port/game-data: source ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43 and header014acd3b821b4ab99b7fbd2072a8df4132465d783917e3b9592e58e8aeda0f6f. An exact copy is pinned under the new candidate's genuine-save-snapshot. Current main source was untouched and no mixed Save layouts were linked.

The serial runner builds full candidate actor/family source against parent-confirmed coherent V5 archives. The link map rejects archived actor/factory objects; selected source hashes are unchanged. --native --coherent-ready requires explicit parent READY. Reproduction needs the known original cache ZIP, source-checked design fixture and coherent V5 dependencies; it launches no emulator/game.

Original provenance accompanies this packet: V38 whole model-name original fixtures and V41 whole IsPlayer fixtures. Those proofs validate branch semantics separately; explicit callbacks in this ordered native test do not imply the whole original Init RNG sequence or complete gameplay services.

Parent owns review/adoption/CMake and the genuine stage10 service composition. Candidate files are under reference/character-model-route-v45; adoption.patch changes only the family model dispatch branch. New model route files require main's actual reached providers.
'''
(reports/'character-model-route-v45-handoff.md').write_text(doc)
prepared=json.loads((reports/'character-model-route-v45-prepared.json').read_text());prepared.update(status='Native bounded prefix PASS; whole InitPost pending',native_run=True,native_runner_sha256=sha(ref/'build_character_model_route_v45.py'),test_source_sha256=sha(ref/'character_model_route_v45_test.cpp'))
(reports/'character-model-route-v45-prepared.json').write_text(json.dumps(prepared,indent=2)+'\n')
files=list(ref.rglob('*'))
files=[p for p in files if p.is_file() and '__pycache__' not in p.parts]
for n in ['character-model-route-v45-prepared.json','character-model-route-v45-verified.json','character-model-route-v45-source-coherence.json','character-model-route-v45-handoff.md','character-model-route-v45-native.log','character-model-route-v45-build.log','character-model-route-v45.map']:
 files.append(reports/n)
for n in ['character-loader-fields-original-v38.json','model-name-original-gold-v38.tsv','0x3a54d4.asm','0x3aeac0.asm','0x3bb98c.asm']:
 files.append(base/'reference/character-loader-fields-v38'/n)
for n in ['character-is-player-original-v41.json','character-is-player-original-gold-v41.tsv']:
 files.append(base/'reference/character-init-model-v41'/n)
files.append(reports/'character-init-model-v41-dependencies.json')
files.append(reports/'character-is-player-v41-verified.json')
manifest={str(p.relative_to(base)).replace('\\','/'):sha(p) for p in files}
(ref/'freeze_model_route_v45.py').write_bytes(Path(__file__).read_bytes())
manifest[str((ref/'freeze_model_route_v45.py').relative_to(base)).replace('\\','/')]=sha(ref/'freeze_model_route_v45.py');files.append(ref/'freeze_model_route_v45.py')
packet=reports/'character-model-route-v45-candidate.zip'
with zipfile.ZipFile(packet,'w',zipfile.ZIP_DEFLATED) as z:
 for p in files:z.write(p,str(p.relative_to(base)).replace('\\','/'))
 z.writestr('SHA256SUMS.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(packet) as z:
 for n,h in manifest.items():assert hashlib.sha256(z.read(n)).hexdigest()==h,n
freeze={'schema':'dh2-character-model-route-v45-frozen','packet':str(packet),'packet_sha256':sha(packet),'packet_members_verified':len(manifest),'files':manifest,'native_prefix_pass':True,'whole_InitPost_success':False,'rendered':False,'playable':False,'selected_graph_modified':False}
(reports/'character-model-route-v45-frozen.json').write_text(json.dumps(freeze,indent=2)+'\n');print(json.dumps({k:v for k,v in freeze.items() if k!='files'},indent=2))
