# XP renderer binding V44

NEW renderer_character_progression_bindings_v44.inc composes CharacterProgressionWorldV23 on SAME player/NPC fields. Include after renderer_combat_text_v1.inc (the actual text callbacks), source player-manager helpers and current renderer runtime; headers character_progression_world_v23.hpp, character_kill_level_events_v23.hpp, player_xp_text_v1.hpp, player_save_write_owner_v1.hpp. No global renderer/CMake or deployment edits were made.

## Concrete root API

Retain RendererCharacterProgressionBindingsV44 beside real Kill runtime. Constructor takes PlayerSkillsRuntime& plus RendererProgressionProvidersV44; bind(error) constructs one progression receiver; owner() returns the actual CharacterProgressionWorldV23 for RendererKillProvidersV43.progression. Call release before PM/VM/profile teardown. It does not publish players, change PM6c4, add XP independently of Kill or create an extra award ledger.

Providers require a lease pinning real callback storage, actual DesignSettingsProjection176, real KillLevelProviderV23, exact Character+14e8 Save getter, actual Application/SavegameManager current difficulty getter, PlayerSaveWriteServicesV1 and whole LevelUp presentation callback. Negative award assertion remains a separate required source policy only if reached. Level118 is mode and must not replace difficulty. NULL Save14e8 is a genuine source possibility; nonNULL must equal same retained Skills Save.

The player actor borrows actual object PropertyState, session PropertyView, same Gear/skills binding, source selected actor row, actual sourceIsPlayer, localnetwork query, remote query and source position160. NPC actor borrows same session PropertyView and raw remote118 byte; victim Save staysNULL, not a generated profile. PlayerGameplayBinding currently chooses KnightPlayerBase in current renderer; that source row must be corrected by genuine selected-class initialization before claiming multi-class progression.

## Completed provider branches

Debug loads actual DebugSwitches each source query. Constants use actual CharacterDesign keys requested by the source core. Regeneration calls real dh2_character_skill_regen_v6(-1) for HP/MP over same sheets; it does not forcibly clamp a current value down to a reduced maximum. SG_Save uses same PlayerSaveLoadOwnerV1/Save identity and PlayerSaveWriteOwnerV1. Statistics performs actual GetPlayerByCharacter before the proven empty IncreaseStat. XP text binds original anim_sct_xp, actual ScrollingCombatText.XPColor, localized StrID.GAMEPLAYMENUS_REWARD_XP, source formatter and same retained combat scrolling queue/follower position+height.

## Positive save and presentation boundaries

Current renderer_character_mutations_v4.inc save callback explicitly returns required-false for campaign writer operations. No completed production file writer was discovered behind that binding. Correct profileNULL/disabled early branches of PlayerSaveWriteOwnerV1 still return before service effects; a slot−1 is not itself a save-success policy. For positive offline profile, actual named save_all writes/job delivery remains mandatory. Online branches additionally require hosting flags, synchronize, section setup/cache, checkpoint deletion and volatile quest packing. V44 exposes these exact source services rather than returningtrue.

LevelUp is original3beb88, **2320 bytes,62 direct calls**, captured in levelup-sourcecalls-v44.json. After property/class/HP/MP/Save it executes actual localized status deque, ScriptManager GetIDFromName/StartScript, anchored FX87 via495f04, local trophy thresholds, MenuManager callback/cache, difficulty/tutorial/campaign flags and updateJob_thread::Start2. The current V44 level_presentation callback must implement that whole source branch. Missing callback fails after the real source property/Save prefix and before carry/tails; this packet does not advertise a fake complete level-up UI. No invented tutorial flag, menu switch or achievement unlock is supplied.

## Proof

Private currentrenderer injected adapter strictly compiled ARM64+x86_64. New test player_progression_v44_host.cpp executes current native progression and actual native RegenHP/MP against genuine bundled character/class rows. O1/O2 ASAN+UBSAN pass22444 checks each, including3200 original ARM math cases, below-threshold XP, threshold property prefix, one-level increment (not recursive giant-award leveling), carry clamp, actual cache stat/skill point grants and localized XP request shape. Explicit save/presentation/text/debug callbacks are fixtures; no positive disk/MenuManager/live level-up acceptance is claimed. The original V1 receipts remain unmodified.

Current whole lethal production remains unpublished until PM AddCharacter/count/currentGS/loot/XP/save/presentation providers are genuinely present. A compiled progression owner with sourcecount0 does not prove XP awards or fix a lethal skill by itself.
