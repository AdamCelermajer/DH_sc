# SWAMP authored chest and NPC script dependencies

This continues the same source-only inventory. It reads the exact original cache scene/MGP declarations, two source table formats and actual Lua text resources. It runs no native parser, VM, engine leaf, tests or compiler jobs and computes no hashes. All source element attributes, table byte spans, raw numeric operands and Lua text/bytes remain in swamp-object-script-dependencies.json. The compact companion is swamp-object-script-summary.json.

## Exact authored object rows

| Authored object / source module | Actual table descriptor | Base source script request | Actual script owner distinction |
|---|---|---|---|
|_prim_OpenableContainer_2 /obj_4of4_brdwalk_sw_00.mgp|Swamp_Normal_Chest|empty Script length 0; no external name|SAME canonical chest object LuaScript owner|
|_prim_OpenableContainer_1 /obj_4of4_brdwalk_sw_00.mgp|Swamp_Normal_Chest|empty Script length 0; no external name|SAME canonical chest object LuaScript owner|
|_prim_NPC_PriestGood /obj_4of4_brdwalk_sw_00.mgp|WanderingPriest|AI 50 Rene; rene ->data/scripts/ai/rene.luac|SAME actual Character ScriptSession|
|_prim_NPC /obj_4of4_brdwalk_sw_00.mgp|Prisonier2|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_NPC01_001 /obj_4of4_brdwalk_sw_00.mgp|Prisonier|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_ActorPrisoner /obj_4of4_brdwalk_sw_00.mgp|Prisonier2|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_OpenableContainer_001_001 /obj_1of4_brdwalk_nse_00.mgp|Swamp_Normal_Chest|empty Script length 0; no external name|SAME canonical chest object LuaScript owner|
|_prim_OpenableContainer_03 /corner_ruin_ws_00.mgp|Swamp_Normal_Chest|empty Script length 0; no external name|SAME canonical chest object LuaScript owner|
|_prim_NPC_04 /merchantcamp_ruins_swe_00.mgp|NPCmerchantCamp|AI 39 Merchant; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_NPC_05 /merchantcamp_ruins_swe_00.mgp|Swamp_MerchantCamp_NPC_A|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_OpenableContainer /merchantcamp_ruins_swe_00.mgp|SwampCave_Normal_Chest|empty Script length 0; no external name|SAME canonical chest object LuaScript owner|
|_prim_NPC_LizBlood /merchantcamp_ruins_swe_00.mgp|Swamp_MerchantCamp_NPC_C|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_NPC_LizBloodPOST /merchantcamp_ruins_swe_00.mgp|zKill5Lizman_Post|AI 27 InvisibleMan; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|
|_prim_NPC_Eddie /bossroom_ruins_ns_.mgp|WanderingEddie|AI 50 Rene; rene ->data/scripts/ai/rene.luac|SAME actual Character ScriptSession|
|_prim_NPC_Mothgiver /obj_2of4_brdwalk_sw_00.mgp|Swamp_MerchantCamp_NPC_B|AI 70 Villager; __npc__ ->native script_default(0)|SAME actual Character ScriptSession|

## Actual script resources

| Requested URI | Exact source bytes | Stored ZIP bytes | Representation / source-only result |
|---|---:|---:|---|
|data/scripts/ai/_commons.luac|13535|1383|Lua UTF8 source text; SOURCE_LUA_TEXT_INSPECTED|
|data/scripts/ai/rene.luac|9336|1947|Lua UTF8 source text; SOURCE_LUA_TEXT_INSPECTED|

The five authored chests use OpenableContainers row 58 Swamp_Normal_Chest (four placements) and row 55 SwampCave_Normal_Chest (one placement). Both Script fields have literal packed length 0: offsets 4730/4734 and 4643/4647 respectively in game_objects_pyarray.bin. No empty Lua body or replacement resource is fabricated. Existing OpenableContainer init_post still reaches its required load_object_script leaf with the nonnull actual row string and data/scripts/objects/ prefix. GameObject::LoadExternalScript 0x38ef60 first retires any old actual GameObject+0x300 Lua owner through its real virtual D0 and clears that SAME field; it then returns on NULL or first-byte-zero input before any new owner allocation or Lua file loader. The empty-name path never reaches GenericLuaScriptOwner::load. Main must preserve that cleanup/branch order. An empty descriptor does not justify skipping the whole object InitPost, script-owner construction, sound/animation, condition, or loot services.

Ten NPC-template source placements resolve from leading CharacterTable cache rows. Two base AI rows select external rene: WanderingPriest/_prim_NPC_PriestGood and WanderingEddie/_prim_NPC_Eddie. The other eight base AI rows select __npc__, which character_script_selection.cpp maps to native script_default(0). It is not a Lua global function name and not an external __npc__.luac request. Every actual Character ScriptOwner lifecycle loads the existing data/scripts/ai/_commons.luac common resource; external kind 4 additionally uses rene.luac under the same AI prefix. Base current-property mutation is not simulated here.

All ten base CharacterTable Dialogs fields contain raw 0xffffffff (-1). No base indexed dialog row is invented. The actual resolved PropertyState, quest/condition-driven NPC/dialog availability, owner selection and live AI overrides remain Main. All activate_cond/deactivate_cond/static and other authored XML attributes are preserved; ten authored placements are not ten active or rendered NPCs.

There are no direct named Include or CallPyScript lexical call sites in these two source files. Comments and string contents are excluded from that search. This does not prove that aliased, computed, native-issued or future runtime requests are absent. Lua syntax, callback registration, alias tables, actual function reach and engine behavior remain unexecuted. No additional source dependency resource was guessed.

## Existing owner integration Main needs

Use the existing independent cache/resolver to deliver the exact stored file bytes to the existing actual CharacterScriptSessionInput common/external fields and keep their buffer owners alive until the actual session has copied/pinned them. The source CharacterScriptSession already names _commons.luac and normalizes the real AI external name rene to rene.luac. Do not create another session, cache, VM or default ScriptManager to carry these files. Derive the live script selection from the SAME actual resolved PropertyState/AI row, not this static base-row inventory.

Chest object Lua owners and Character ScriptSessions are separate from the ONE application-owned ScriptManager and its common/local compiled PYS resources. The complete 80 binder applies to that manager; object/character Lua buffers must not be installed as its command Data factory. NPC engine callbacks that use world objects, timers, FSM/navigation, combat, aliases, quests, interactions or dialogue need Main genuine owners and leaves. Global scripted sequences already exist in common C00 and local SWAMP L01; publishing source bytes does not execute them or admit rendering.

No new loader interface is necessary for the two resolved resources: the existing cache open_file transport and existing actual session/object owner inputs already express them. The remaining gap is Main supplying the actual owners and functional bodies at the original reached points. This source batch adds an exact resource/row inventory and closes the missing authored-dependency knowledge without copying manager or Lua implementation.

Source-only unresolved resources: none in the bounded explicit roots. Runtime/current-property and indirect dependency paths remain unsupported by this inspection.


## Exact GameObject empty/NULL source boundary

The authentic symbol at 0x38ef60 is GameObject::LoadExternalScript(const char*,const char*), 216 bytes. Its raw source/instructions are in gameobject-loadscript-source.json. At 0x38ef64 it reads the SAME GameObject+0x300 owner. If nonNULL it invokes virtual deleting destructor slot+4 at 0x38ef88 and writes NULL at 0x38ef90 BEFORE inspecting the cached name pointer.

At 0x38ef94 it checks name NULL; otherwise at 0x38ef9c it reads the first signed byte. Either name NULL or first byte 0 returns at 0x38efa8. Both skip LuaScript allocation/C1, virtual GameObject+0xc Lua-representation hook, prefix assignment, LuaScript.Load, and cache/file calls. No new Lua owner or empty script body is constructed.

Only a nonempty name reaches alloc 0x98 and LuaScript C1(false) at 0x38efb4/38efc0, then publishes SAME+300 at 0x38efc4. GameObject virtual+0xc receives that script+0x10 at 0x38efd8; an optional nonNULL prefix is assigned to its CString+0x68 at 0x38effc. LuaScript.Load 0x37b574 runs at 0x38f008. A false Load invokes the new owner's D0 and clears SAME+300 at 0x38f030.

The early path does not define a boolean result. Container InitPost tail-branches at 0x39fa90 and does not inspect a semantic Load boolean. Main's callback bool reports successful real transport/body completion: on NULL/empty it may complete only after required existing-owner teardown/clear and the actual early branch. It must not allocate a replacement owner, load an empty file, or suppress failed real teardown behind unconditional success. A cold C1 +0x300 NULL chest reaches a genuine no-allocation absence branch; no engine binding/cache resource leaf is required there. Nonempty requests still require all actual Main allocation, Lua representation, path, load and conditional cleanup leaves.
