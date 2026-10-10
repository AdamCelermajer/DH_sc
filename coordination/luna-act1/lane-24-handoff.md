# Lane 24 handoff — chapter presentation and completion

## Source delivery

The assigned source paths already contain connected owners for the lane's
presentation services:

- `native_source_script_ui_v98.inc` binds script UI leaves to the existing
  script execution owner, actual HUD movie slots, campaign `original_ui`, and
  same-world quest/tutorial/cutscene callbacks. Character interaction UI calls
  the real CharacterMenu/HUD callbacks for merchant, cleaner, and merchant
  information. It does not create a parallel menu owner.
- `renderer_campaign_music_v101.inc` binds `PlayLevelMusic`, music state,
  safe-zone music, aggro transitions, and cue playback through the existing
  Application SoundManager and retained current-Level audio owner. Each call
  validates the same World and manager.
- `source_campaign_character_merchant_v114.cpp` fills the actual merchant
  Character's `Inventory37c` from its authored MerchantTable row through the
  existing `LootCreationV8` path. The interaction/UI dispatch is connected in
  `source_campaign_character_interaction_v114.cpp` and the assigned UI include.
- `source_campaign_language_refresh_v109.cpp` traverses the retained
  source-language scene and refreshes existing Character inventories/items;
  `native_process_primary1_binding_v98.inc` connects the front UI language
  callback to it.

No source edit was needed in these assigned implementations: replacing or
duplicating their owners would create competing authorities. Journal and map
menu lifecycle/render/update wiring is owned by the existing native menu
singleton/postmovie/menu action owners, outside this lane's writable paths.
Those owners are present in `native_menu_singletons_v67.inc`,
`native_menu_postmovie_v62.inc`, and root-owned process-menu dispatch.

## Authored exit behavior and limit

IDA's `TriggerZoneExitLevel::Update` at `0x39c33c` is the exit/fast-travel
presentation path. It uses the actual current Level gate, updates activation
count unless the online virtual update branch applies, and unlocks the authored
fast-travel name when a player is activating the zone. It displays the
fast-travel prompt only when the local player touches the zone, the local
player is hosting, the zone's `GameObject::MeetCondition` succeeds, the authored
LevelList ID is not `-1`, and the prompt has not already been shown. The source
also invokes `RequireOnlineUpdate` on this path; its returned value is not tested
by this function. It clears the prompt after the touch/condition branch stops
applying. The prompt uses the zone's authored Level name and entry ID. The
campaign binding in `source_campaign_zones_v83.cpp` routes those
operations through the current zone, condition provider, selected Save's real
fast-travel state, LevelList localization, and original HUD `DisplayFastTravel`
callback.

This function does not itself finish a chapter or authorize a level transition.
Actual serialized Act 1 exit-zone rows and their condition-list contents must
come from the loaded Swamp/Witch Cave source data and the shared condition/event
owners. The source tree does not provide a lane-owned authored row that can be
used to state a fixed quest/condition ID. Do not infer a universal chapter
completion predicate from this zone update. Lane 18 owns condition/event
dispatch; lane 05/root owns actual area-transition execution; root owns the
shared renderer/native app and integrated acceptance.

## Integration boundary

Existing script/audio/merchant/localization connections are source-level only;
their integrated runtime acceptance remains pending. The assigned paths do not
own the journal/map primary menu dispatcher or ExitZone binding site. Root can
consume the existing presentation and transition APIs once those owners are
joined in the shared app. Chapter completion remains blocked on actual authored
Swamp/Witch Cave condition and event data reaching the shared condition/event
runtime, plus the integrated transition/gameplay milestone. No reward, fake
readiness, or completion callback was added.

IDA reference: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039c33c.c`.
