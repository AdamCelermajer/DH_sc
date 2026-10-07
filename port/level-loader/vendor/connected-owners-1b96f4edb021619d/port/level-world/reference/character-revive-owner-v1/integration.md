# Whole Character Revive source service

Use `dh2_character_revive_v1` at InitPost's3a59ac request. It borrows the SAME
CombatActorState: `dead` is Character1449 and `low_health_armed` is cue1448,
as independently established by existing health/application differential tests.
This is not a second alive/HP/death owner. Borrow actual ObjectBase remote118,
network110 and114 from the canonical base owner. Do not substitute invented
flags or a detached network snapshot. NPC state owner's constructor combat
fields already own the source110 value; generic base integration must reuse it.

If dead, the kernel first delivers genuine Character.RaiseEvent(3,null), then
writes cue1448=1/dead1449=0, invokes genuine _InitHpMp, clears remote118, and
queries the actual Application session byte5. Nonzero resets110=-1 and114=0.
Requested physical initialization occurs before the IsPlayer query. Offline
players reread Application byte5 and use original respawn1474/floor-height/
SetPosition(true). All paths finish at genuine CharAI.UpdateAllSkills.

Services are synchronous required endpoints. Zero delivery means the whole
helper actually ran; nonzero aborts with reached prefix retained. Request entries:

|Entry|Required whole helper|
|---|---|
|3a4d5c|same Character.RaiseEvent; argument0=3/null payload|
|3b3a70|same _InitHpMp; existing `dh2_character_script_init_vitals` with real Debug|
|7fd794|actual Application owner/session byte5; return live byte word|
|3b4088|same InitPhysicalObject; existing source NPC body/physical helpers|
|3a49f0|actual Character.IsPlayer query over same properties/AI/catalog|
|525508|actual source world height; localXYZ input with resulting Z replacement|
|393db4|same GameObject.SetPosition(payload,true)|
|3d8894|WHOLE UpdateAllSkills; actual arrays/FSM predicates, no empty assumption|

The unused incoming GameObject target is retained in the public signature.
`result.calls/last_entry/completed` distinguish success from a retained partial
prefix. The containing retained InitPost owner must keep its failure guard;
never recreate it to turn called1394's early return into fake completion.

Original3a59ac312-byte receipt288cases, replayed in native O1/O2 ASAN+UBSAN:
6528checks each, ordered source requests/direct fields/position submission match.
Includes raw dead255, physical255, and different first/second Application values
to prove genuine reload semantics. Every mandatory service failure is tested.
Helper bodies are named fixture boundaries; complete gameplay initialization is
not claimed. Strict ARM64/x86_64 compilation also passed.
