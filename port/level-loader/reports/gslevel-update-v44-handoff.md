# GS.Update V44 complete source envelope and actual C1 bridge

PASS:1012 exact original ARM cases match the native contract; six missing/null/foreign/failed/reentry guards pass normally and under ASAN/UBSAN. A separate executable constructs the real V5 C1 from original cache tables/scripts and proves state130/byte145 borrow pointers alias its actual fields; exact Level.Load stores0, GS.loading2 becomes3, non-source cells and SAME Lua44/Save_ec owners remain unchanged. Missing actual Level.Update then stops3. GS entry/application/menu transports are explicit fixtures; original GS Ctor publication and complete Level.Init are not claimed.

| Original branch | Native effect and source address |
| --- | --- |
| Application.byteec nonzero | Skip the whole GS body/menu, source386650..386664. Must borrow actual Application byte; no inferred offline/false value. |
| GS.loading38=1 | Store2 at38676c; reread Level field34; do not call Load yet. |
| GS.loading38=2 | Call exact Level.Load3ef218 at38677c (only store real130=0), then GS.loading=3 at386784. |
| GS.loading38=3 | Call actual Level.Update(false)386798; reread GS.field34 and its real130; setGS.loading4 only when real130==38 at3867ac. |
| GS.loading38=4 | Clear original GOT2dfc word3866b4 and GOT2fd0 byte3866bc; Debug.load3866c4; GetSwitch Lua_DumpCalls3866e4; iftrue setbyte1 at3867bc, reload3867c4 and SetSwitch(false)3867e8; then actual Level.Update(false)38670c. |
| Other loading words | Source switch default goes directly to real menu gate; no invented body/state normalization. |
| Menu gate after body | Reread actual GS.field34. Null updates Menu; nonnull byte145nonzero skips; otherwise signed32 state130>1 && <=26 skips; outside updates. Original386710..386748. |

The contract is gslevel_update_v44.hpp (template over the existing GSLevelFieldsV2). Its read borrow helper and exact Level.Load provider are gslevel_update_v44_level_borrow.hpp, validated against complete actual C1 with typed uint32 state130 and uint8 byte145. It owns no new current-Level/global/GameSM state or shadow phase/readiness. Fields active3c are preserved; original Update does not read/write them.

Minimal integration: retain this frame owner beside the existing GS runtime and supply a lifetime pin for THAT actual GS fields reference; same actual Application.byteec and Debug globals; actual DebugSwitches owner/load/get/set; actual MenuManager instance/Update(true); complete C1 read borrow; exactLevel.Load helper; actual Level.Update(false) backend. If kept inside the GS runtime, do not create a GS->frame->GS strong self-cycle: parent must use a scoped owner/lifetime arrangement or retain the frame externally. Current NativeGSRuntime fields are const-exposed; a narrow owner-internal binding/borrow is needed. No selected graph files were edited by this agent.

Level.Update(false) is not always a loading tick. At source130!=38 it calls _LoadProcess; at38 it reaches gameplay Update. GS loading4 must therefore route to main's genuine whole Level/frame owner. Calling SourceLoadingV43.tick at38 instead would not reproduce the gameplay body. The six source callback services are required only when reached; null original unsafe dereferences become explicit native required errors without a successful state tail. Provider failures retain original completed prefix, and failed/nonreentrant calls do not replay.

Source body386630 has no network/GetOnline/GameSM stateId6f call. Those may be needed for outer state selection or deeper Level bodies, but are separate unproven outer integration here. Main owns state-machine selection, network/menu/application producers and actual gameplay/frame services.

## Logical identity and raw filename audit

LevelSourceRequestV1 is the modern loader-owned source transaction (level_preparation_v1.hpp7), carrying logicalidentity/definition/kind/seed. The original GS body has no such request. Original GS.name18 is read3861b8 and passed as C1 arg r1 at3861e0/386200; C1 copies that CString into Level.name_f8 via3f3158/3f31b4/3f31b8/3f31f4. C1 scans Arrays::LevelList row.file lowercased substring within name_f8 at3f3330..3f336c, producing row3c, proceduralbytee8 and difficulty40. It does not compare logical catalog name to rawfilename.

Keep modern identity as catalogrow.NAME (SWAMP distinctSWAMP_02); GSname18/C1name_f8 is selectedrawrow.FILE spelling; definition is separately resolved cacheURI. SWAMP_02 original row42 file022_swamp2.rule.xml is procedural1, while firstSWAMP uses001_swamp.mlx. Legacy NativeGS tests set request.identity=worlds/+filename, so their identity==C1name guard covers only that legacy request domain. NativeLevelConnectionV25.construct_allocated equality guard atline41 is modern policy, not originalGS evidence. Parent should separate raw source filename from logical identity rather than rename/drop the latter. When a prepared description is claimed, validate its catalog row/filename/type/seed provenance explicitly. Seed equality is also modern preparation-consistency policy: the standalone fixedSWAMP source probe request defaultsseed0 while authenticC1 fixture arguments seed114=7; this discrepancy is explicit and is not a production prepared-generator proof. Actual C1 arguments remain source-authoritative.

| Subgoal | Status |
| --- | --- |
| Full original GS.Update branch order/field rereads | Finished:1012 original-gold cases |
| Native required-service/lifetime/failure safeguards | Finished:six normal+sanitizer cases |
| Actual complete V5 C1 pointer/Level.Load bridge | Finished:separate cache-backed native executable |
| Bind into retained actual GS runtime | Parent in progress; no duplicate current global |
| Actual Application/Debug/Menu/wholeLevel frame providers | Required main services |
| Whole Level initialization/gameplay | Unfinished; false readiness preserved |
