# B034: player deaths must not enter enemy reward dispatch

## Evidence

- **Visual reference:** `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`, original v1.0.3 recording. I inspected the 4:22–4:33 sequence at one frame per second. At 4:22 the gameplay HUD is visible and `8 EXP` appears over the encounter; subsequent frames show enemy combat and bodies. This is direct visual evidence of the ordinary enemy-reward presentation. The sampled sequence does not show the player dying, so it does not establish the player-death transition or directly prove absence of player loot/XP.
- **Reproduction:** frozen executable `AAC57D8F9B211955E8BCD40B2BC9FFDD603C380E876A470791A924AAE01391E6`, `.local-inputs/v19-frontend-hotfix/next-preview-integration/same-process-death-restart.log:306`. The selected Rogue begins at HP=1. Production binds it under `UINT64_MAX` (`main.cpp` combat-player setup); `invalid_actor_id` is actually zero. At frame 14 the source death-reward observer reports that max-valued identity as victim, `xpRecipients=1`, XP `1→2`, and six spawned items. Nearby frame-14 damage lines identify that actor as the target and `dead=1`; End34 later removes its physical body. This reproduces the unwanted reward side effects on player death.
- **Recovered source logic:** original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; `port/level-world/reference/character-kill/NOTES.md` records full `Character::Kill 0x3a5b18` and `Ctrl_Kill 0x3ad528` captures. Ctrl_Kill checks IsDead, invokes Kill, then raises event2. Kill writes dead and HP=0, queries IsPlayer through the receiver's virtual `+28`, and for `player && !force` increments death property25, runs local death-count trophy checks and online/local-player handling, then returns. That branch precedes Level+0x150, DropLoot (`0x3a5ae4`), killer/contributor credit, DistributeXP (`0x3bf828`) and quest delivery. For NPC force0, Level+0x150 gates DropLoot before contributor and credited-killer XP; this must remain intact. The exact source entry and downstream reward order are detailed in `port/level-world/reference/character-kill-live-v21/integration.md` and `port/level-world/reference/ranged-attack-v41/lethal-skill-adoption-audit-v42.md`.
- **Actual implementation gap:** `RuntimeSessionDeathRewardsV1::after_update` dispatches generic applied lethal DamageEvents. `RuntimeDeathRewardsV1::consume_events` previously treated every dead `ActorState` as a reward victim. `PlayableActorWorld::traits(id)->is_player` is the existing role classification in this adapter. It must be checked before resolving reward owners, consuming loot RNG, publishing items, or distributing XP.

## Expected behavior and implementation

Player death still follows the existing death/body path, but the generic session reward adapter skips its loot/XP route. Ordinary NPC deaths continue to use authored loot, the source RNG, XP formulas, and the existing once-only lifetime receipt. The minimal reusable change is an early player-victim admission check in the shared `RuntimeDeathRewardsV1` event consumer.

Uncertainty: the generic session adapter has no `force` input. This change preserves the normal player-death path (`force=false`) used by the recorded runtime. A forced player Kill (`force=true`) is not represented by this event adapter and remains outside this component's evidence.

## Focused verification

The focused runtime test retains a positive NPC death (one XP/loot route) and its duplicate no-op, then kills a player bound at production's exact `UINT64_MAX` ActorId while another player is alive. First and repeated player-death delivery leave XP, gold, inventory/equipment, loot RNG and spawned item count unchanged; SaveStore encode/decode retains the same character values. The full runner passes with the original loot tables, power resources and design settings.

## Status

Implementation is in the shared death-event consumer and its focused test. The original recording provides enemy-death context only. Normal executable integration and visual verification on the fixed build remain the integration lead's responsibility.
