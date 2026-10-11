# SOUNDS report (B090 level-up, B099 NEW QUEST, B104 QUEST COMPLETED)

Branch `fix/sounds` (from v20.1/integrate). Build exit 0 (-Jobs 2); ctest quest_banner_presenter_v1 (extended with cue tests) and runtime_level_up_presentation_v1 pass. Full ctest not run.
Scratch: `.local-inputs/claude-v20.1/fixsounds/` (lvl, quest runs, logs).

## IDA evidence
- Level-up: `Character::LevelUp` 0x3beb88 starts ScriptManager script "PlayerLevelUp" (before PlayAnimFXSet 135). Its compiled script (`port/windows-foundation/reports/encounter-source-scripts.json` id 12) is a single Script_PlaySound, sound id 127 = "LevelUp" (sfx_level_up.wav, 3.43 s).
- New quest / completed: `Quest::SetState` 0x480c78 case 6 enqueues DialogMsg (GLOBAL_QUEST_NEW, DialogStyles QuestMsgDialog); case 0xC enqueues QuestCompletedMsgDialog. No sound call in SetState. The sound is in the HUD movie `dqhud_droid.swf` (original-cache): the QuestMsgDialog show frame handler runs `NativePlaySoundFX("QuestNew")`, the QuestCompleted frame runs `NativePlaySoundFX("QuestComplete")` (string/bytecode order seen in the SWF; also `sfx_quest_panel_up` on hide). NativePlaySoundFX 0x43ae10 -> VoxSoundManager::PlayMenu 0x36bc34 -> Play 0x36b80c (plain emitter). Ids: QuestNew=124 (sfx_quest_new.wav, 5.24 s), QuestComplete=123 (sfx_quest_complete.wav, 6.26 s), LevelUp=127.
## Change
- `RuntimeSessionAudioV1::play_named_ui_sound(name)`: name -> source ordinal -> existing plain script Play path.
- `QuestBannerPresenterV1::on_shown(completed)` fired when a banner becomes the shown one (once per banner, queued ones when promoted); main.cpp plays QuestNew/QuestComplete there (retries while audio output is not ready, for the frame-0 bind banner).
- main.cpp level-up callback plays "LevelUp" (the PlayerLevelUp script content) next to the FX set 135.
## Verification (quiet run, silent)
Log lines: `Level up sound LevelUp ordinal=127 status=submitted frame=143`; `Quest banner sound QuestNew ordinal=166 status=submitted` (frame 124 after accept, and frame 0 after retry); `Quest banner sound QuestComplete ordinal=165 status=submitted frame=249` after the 8th Bog Moth kill. (Ordinal = source table index; the sound-table ids above are from the routing census.) Unit test covers once-per-banner and queued ordering.
## Video / audio
Reference video part1 has an opus audio track. Envelope check: quest complete panel at ~761.2-763.6 s shows a loudness rise/decay (peak ~762.0) consistent with the 6.3 s sfx_quest_complete envelope; level-up at ~769.0-770.5 s shows a rise consistent with sfx_level_up; NEW QUEST at ~171.4-177 s has raised levels. Cross-correlation against the music/ambient mix was inconclusive (score ~0.2, music masks it), so this is timing plausibility only, not a waveform match.
## Observed / inferred / not verified
- Observed: the log lines above in our EXE; SWF strings; IDA SetState/LevelUp.
- Inferred: the NativePlaySoundFX calls belong to the show frames (string order in SWF bytecode, not decompiled ActionScript); sound ids.
- Not verified: audibility (all runs silent); the level-up FX itself still fails in this run (`Level-up actor has no opaque FX manager identity`, FX not part of this bug); exact video waveform alignment; real-game hand test. B104 tested with debug kill/accept, not a natural kill route.
