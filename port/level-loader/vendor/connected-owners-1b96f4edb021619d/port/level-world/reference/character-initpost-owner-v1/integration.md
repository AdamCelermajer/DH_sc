# Retained NPC InitPost integration

`CharacterNpcInitPostOwnerV1` ports the complete **NPC** control-flow path of
Character::InitPost3b4d60. Original ELF/function hashes and the full2164-byte
capture are beside this file. Player/local-equipment/highlight paths remain a
required separate implementation; IsPlayer=true fails explicitly.

Construction pipeline:

1. Construct `RetainedCharacterActorV1(identity,worldPin,actualCatalogName)`.
2. Canonical factory writes `*canonical(...).class_name20=entry.name`; use the
   resulting `properties()` borrow for actual shared PropertyMap declaration,
   template/default/XML writes. Source static84 is ctor0. Other bools are not
   readable until their producers write them.
3. Canonical Add and property overrides must finish before allocating the
   ScriptCharacterObject with committed name/position and the SAME props/life.
4. Construct graph, then `construct_script`. This allocates the private owner;
   **it does not load Lua**. Invoke `load_script()` only at source3cf1f0.
5. `init_post_fields(borrow,error)` borrows those SAME retained fields,
   including `session.owner().lifecycle().delayed` for Character3ec. Construct
   one retained `CharacterNpcInitPostOwnerV1` from the borrow and actual services.
   Keep it through failure; do not create a replacement to bypass its guard.

The caller composes each request at its exact source entry. `subject` identifies
the same Character; manager/debug receivers come from its pinned World context.
An explicit successful provider means the whole helper actually completed.

| Entry | Required behavior/response |
|---|---|
|38bd64|CheckSpawnProbability; SAME source RNG, return signed value|
|337888 /337a88|actual Debug.load then GetSwitch(`isTracingChar_Init`)|
|34aca0|same ObjectManager GetByName(text,room,false,null); return handle identity|
|33fdc0 /33fee4|actual Handle.GetObject(false), then AsGameObject; response identity|
|405540|actual same controller Cmd_MoveTo(payload)|
|3b3d38|SafeGetCharPropsId; writes borrowed halfword13c8|
|3df2a4 /3e0810|actual same properties LoadBaseProperties(id), Recalc(true)|
|3a54d4|GetCharModelName; optional stable response CString|
|38be5c|WHOLE GameObject.InitPost including visual/physical/scene services|
|38ab60|MeetCondition signed result|
|ffffff40|virtual+40 on actual receiver, reached only when conditionfalse|
|33ddb4|ObjectBase.Delete after preceding virtual+40|
|3bc4d0|SG_Load(2); actual null Save branch may be genuinely empty|
|3a2fec|actual AI index/table selection; return captured real `AiProps*` in `ai`|
|3a49f0|actual receiver IsPlayer query; NPC false required for this domain|
|3cf1f0|same CharAI LoadScriptProcess; retained `load_script`, then publish actual activeAIS|
|495430|GrabAnimFX(wrapped offset+capturedAI.self_fx,Character); identity result|
|3b4738|whole RegisterCharacterFXTable via actual cache/preload owner|
|3c9f4c|whole same CharAnimator.SetAnimationSet|
|3b3b00|whole Character._InitSounds; no blanket empty success|
|3a58f4|SetInitialPosition(payload currentXYZ); actual floor result writes1450|
|393db4|SetPosition(payload initialXYZ,true), same Scene/PF/body|
|470a54|actual nonnull VisualObject.ApplyMeshBox|
|3a59ac|whole Revive(null,true), including real RaiseEvent/physical prefix|
|3b3a70|whole _InitHpMp, after Revive's separate first pass|
|3ce7c0|same InitScriptProcess(false), only when live delayed3ec==0|
|3d37d0|actual AddToGroup(Character)|

Pure stores use existing verified scale kernel, live resolved fade properties,
and raw same rotation words. SelfFX identity is stored only after actual Grab
returns. Initial-position and source visual fields remain helper-owned writes.
The called1394 byte is published BEFORE RNG. A failed provider preserves all
prior stores/services, and the retained native failure guard prevents treating
the original already-called early return as completion on a retry.

The generic GameObject visual lane is owned by root; collision/physical/body
helpers already exist but must be composed at their original calls. No fake
AssetManager, save, sound, FX factory, AI group or Lua success is installed.

Verification: both ABI production compile; O1/O2 ASAN+UBSAN host117 checks,
36 ordered positive requests and failure at every request, skip/conditionfalse/
delayed/waypoint/player branches. The helper endpoints are explicit fixtures in
this host audit, **not original differential or full game initialization proof**.
The separate collision original receipt executes36 whole ARM wrapper/filter
cases. Reproduce with `tools/run_character_npc_initpost_owner_v1_host.py` and
`tools/run_retained_character_actor_v1_host.py` in the existing WSL environment.
