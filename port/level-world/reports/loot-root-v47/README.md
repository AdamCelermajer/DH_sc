# V47 Item frame, pickup leaves and original UseOOI integration

Status: source subsystem staged; no live gameplay acceptance. Root owns all shared renderer/CMake/GS integration. No shared production globals or private loader files changed.

## Delivered implementation

- loot_source_fields_v47: missing Character14e8 SAME Save association with lifetime pin; byte80 only available after actual source producer; exact registered StateInfo borrow; corrected NAME30 IsPlayer; actual StringManager localization/typed508ef4 auto-transmute formatting, source tutorial byte2a, saved difficulty singleton, real Gear transmute mutations/trophy dispatch.
- loot_item_frame_source_v47: same Item speed3b0 and actual retained VisualObject scaling120; inherits the exact V5 frame policy. NULL auxiliary/default boundary paths need no invented global. Positive auxiliary/boundary require real policy provider.
- loot_item_target_source_v47: extends existing TargetServices only for actual canonical pooled Item. Inherited GameObject.IsDead3400ac is original literal0; GetTargetPosition3935dc selects node180/visible80/cached184 or raw160. Positive184 is explicitly unavailable in current base storage, never replaced by visual position.
- character_use_ooi_v47: whole Cmd4057fc, Ctrl3ad690, Force3ad5ac, Use3ad614. Forced/global/lock gates, remote-first, NULL held request, source idle/moving state queries/reload, same current target408, SetTarget(mode0), sole412 store after success. Network virtual24/dead/virtual90 twice, client/factory/message fields50/52/54 and original requested virtual50 tail remain ordered. Actual online receivers are mandatory; offline never invents packets.
- renderer_loot_root_providers_v47.inc: actual registered canonical actors, player/NPC bodies, StateOwnerInfo, Gear/property/OOI, source UI/text/settings/difficulty/FX owners. Retains callback contexts, including status and tooltip; unknown/missing physical peers fail explicitly.
- renderer_use_ooi_v47.inc: exact HudAttackServicesV46.use_ooi endpoint on same current controller; same player target and skill_state.heading_enabled. Pins Item145 and root actor lifetimes without new registries.

## Root compile and integration

Add four TUs to level-world CMake:
loot_source_fields_v47.cpp, loot_item_frame_source_v47.cpp, loot_item_target_source_v47.cpp, character_use_ooi_v47.cpp.

Include their headers at model_renderer TU scope. Include renderer_loot_root_providers_v47.inc and renderer_use_ooi_v47.inc after V44 connection and all preceding typed declarations. Additive current full renderer compiled for both ABIs; no model_renderer/CMake edit was made by this packet.

Retain RendererLootRootProvidersV47 and RendererUseOoiV47 beside the one RendererCanonicalLootConnectionV44. Populate source contracts from actual UI/localization/item-text owners and preserve their leases. Call rootProviders.connect(providers, canonicalServices, error), then construct/connect V44 and call rootProviders.bind_connection(connection). V44 must still run precache exclusively at actual published Level state29; do NOT call its initialize_final for these late Items.

Bind UseOOI only after the same Item owner exists. ControllerUseOoiFieldsV47 borrows:
- controller identity reinterpret_cast<uintptr_t>(&prince_runtime.controller)
- receiver lease actual world/player runtime lifetime
- forced9 &prince_controller_forced
- locked8 &prince_state.controller_locked
- networkA &t.attack_controller.network_enabled
- global_blocked &controller_global_blocked
- actorC &t.attack_controller.character
- controllable4 &t.attack_controller.controllable

These flags are the existing raw-word source byte projections (0..255), not new storage. Existing network_enabled zero is a development constructor projection, not a proven full v2Controller C1. Supply genuine positive network services if this later becomes enabled. The retained root endpoint forwards all network contexts; direct held invokes RendererUseOoiV47::held(owner, SAMEcontroller378, original_requested, error). NULL requested must remain NULL.

## Minimal real publication hooks and blockers

Character14e8: add one shared LootPlayerFieldAssociationV47 to existing player_canonical metadata, constructed with actual player identity. In initialize_player_skills, the current root already constructs t.save and calls set_character(id). Immediately after that executed allocation/SetCharacter prefix, call fields.source_save_store_3b36d8(t.save,error). This publishes that SAME pointer, does not allocate/reload/init a profile and does not mark InitPost complete. Restore reuses this association; no another SaveLoad owner. Borrow is genuine NULL before publication.

Player80: no genuine constructor store exists. Current root canonical player facet has no byte80 provider. Call source_visible_store(actual_value) only from actual PropertyMap/SetVisible38b0f0 write. World actor search.visible and on-screen visibility are not this byte; neither1 nor0 may be forced to make Item contact pass.

Decor PO.owner8: current BodyOwner does not retain canonical GameObject association. Decor DTO/body owners are not published canonical objects. Root/loader must publish actual receiver/owner8 + visible80 + canonical Handle, and provide source.decor_peer/decor_fields. Physics invokes both custom tests before category filtering, so missing decor fields can be reached during Item allocation/Step. Do not filter away those source calls or return true.

GS/current Level: demo world currently has no actual published GS Level; V44 has no C1 fallback. Loader owns stage29/GS publication and should compose its frozen V45/V46 frame bridge. Source phase0 current published Level is valid; unpublished demo C1 is not. No phase38/count/InitFinal forcing.

Full pickup continuation: source.pickup_remaining supplies actual gameplay tutorial manager, positive tooltip/glow, controller network packet, stat/tracking hooks. Existing V23 owns normal transfer/currency/potions/despawn; V47 supplies text/transmute/Save/difficulty/local/FX leaves. Positive tutorial starting, online delivery, and stat tail are not silently bypassed. Item equipment remains the actual Gear/menu action owner, not copied starting gear. Potion pickup does not directly heal.

Whole death->loot->XP->quest acceptance is pending root's actual Hit8/Level/PM/Save/GS composition and live test. This source packet alone does not prove that gameplay sequence.

## Proof scope

compile.json: strict portable implementation + test both arm64-v8a/x86_64.
renderer-compile.json: additive current full model renderer both ABIs, including both staged adapter includes.
host.json: bounded ASan/UBSan actual cache/pool/BDAE/body/animation graph + new source regressions. External network/state/debug facts are declared fixtures; older immutable support DSOs are declared, current source modules listed in receipt are rebuilt.

Actual positives include145 Item cache, source late Spawn with no InitFinal/PF InitObject, real localized ItemInstance, visual/body/visibility/despawn/reuse, source100th insert failure prefix, target service extension, actual visual scaling, actual varargs defaults, Save identity/lifetime, corrected NAME semantics, source state gates, network integer truncation and failures after message/set-target mutations. Missing cached184 rejects explicitly.

No device, emulator, APK build/install, FPS or visual pixel claim is made.

