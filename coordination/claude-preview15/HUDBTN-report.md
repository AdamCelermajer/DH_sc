# HUDBTN (I025): PC HUD skill buttons use original button art and real cooldown

Status: IN PROGRESS (skeleton 2026-10-10).

## Evidence (investigation)
- User screenshot: `.local-inputs/claude-preview15/user-shots/hud-skill-circles.png`: plain gold-ringed circles with no art and no cooldown.
- Android/native reference with original art: `.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png` (bottom row of five gold/grey rings centred horizontally, pitch about 56 stage px, radius about 24 stage px; captions Lv 1 / Empty / Locked / x 5).
- Reference video (640x360 landscape, not 720p): `.local-inputs/claude-preview15/hudbtn/ref/full420.png` shows the original phone HUD: portrait and HP top-left, potion top-right, Faery and skill buttons on the right, attack bottom-right, joystick bottom-left.
- SWF `dqhud_droid.swf` (dump script `.local-inputs/claude-preview15/hudbtn/dump_buttons.py`, probe `probe2.py`):
  - btn_skill sprite 353 (23 frames) leaves at frame 0: shape 167 (direct), shape 168 via sprite 169, btimg sprite 245 (icon), Grey sprite 249 -> shape 248 (solid), hitzone 352 -> shape 351, CoolDown sprite 350 (101 frames).
  - CoolDown sprite 350: frame 0 empty; frame s >= 1 is the single identity-placed shape 249+s (solid fills 250..349).
  - btn_spell sprite 398 has CoolDown child 350 at depth 7; btn_potion sprite 121 has NO CoolDown child (no potion cooldown in the original).
- IDA `InfoHUDManager::FastUpdate` 0x41e064 (pseudocode-all.c lines ~206031-206300): per skill button v26 (0..2), slot = AS SlotId, cooldown frame = GotoFrame((int)(v125[slot]*100.0) - 1) clamped at 0; v125[slot] = CharAISkillScript::GetCooldown(skill in SG_GetSkillInSlot(slot)). Spell button (this+300) uses GetCooldown of the spell script (this+1160).
- `CharAISkillScript::GetCooldown` 0x... = 1 - elapsed/total (CharTimers::TMR_Start sets elapsed=0 at start), so the value is the REMAINING fraction: 1.0 right after a cast, 0 when ready. Frame 0 = ready (no overlay), frame 99 = full overlay right after a cast.
- Faery/Celest spell cooldown in the PC port: HottyCooldownClockV1 spell_ready_at_ms with fixed 5000 ms (hotty_cast_v1.cpp:424, celest_cast_v1.cpp:236). Skill cooldown: RuntimeSkillCastCoordinatorV1 skill_ready_at_ms_ (no total stored yet).
- Original layout (for the PC stage): bottom row centred horizontally, centre y about 274 stage px, pitch 56, radius 24 (from the Android reference). The current PC layout (centres 162..350, radius 15) is not the original.
