# Preview 13 verification: play (verifier `play`)

Verdicts: B043 APPROVE. B044 APPROVE (Swamp path). B040 follow-up logging REJECT (stop-on-menu-return line missing). Regression B037, B004, B038, B041, B039 PASS. Hit behaviour (B035 sanity) PASS by visual check.

## Setup
- WIP EXE: `.local-inputs/windows-source-clock-v19-preview-13-candidate/dh-foundation.exe`, SHA256 `521533F4...5A0B` (matches brief; the `AA4442AD` in I-report is stale).
- Baseline: `.local-inputs/windows-source-clock-v19-preview-12/dh-foundation.exe`, SHA256 `1D43942E...2672`.
- Own folder: `.local-inputs/claude-preview13/verify-play/` (`wip/`, `p12/` package copies with hard-linked assets, `saves/`, `logs/`, `shots/`, `scripts/clickrun.ps1`). Nothing written in the candidate or baseline folders. No `dh-foundation.exe` was running before my runs. All processes I started were stopped by me or exited.
- Setup notes: args must sit inside the package folder (relative `assets`), and saves must be relative (absolute `--save` paths fail the menu action). `--capture` with absolute paths fails with "Cannot write capture" after the run; logs stay valid, so I used relative capture names.

## (1) B043 potion after Celest/Faery MP spend
Args: `startup.args` + `--save <copy>.save --menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER --fixed-step .016 --skill-key-frame 30:4 --skill-key-frame 60:5 --frames 80` (Mage: key 4 at 30, key 5 at 150, `--frames 160`; Knight: key 2 at 30, key 5 at 60).

| Run | P12 | WIP |
|---|---|---|
| Rogue Faery then potion | `Source Faery key=4 frame=30 ... MP=30.25`; `Source potion key=5 frame=60 result=0 consumed=0 quantity=0->0 ... Source PropertyAdd could not synchronize live actor vital` | `Source Faery key=4 frame=30 ... MP=30.25`; `Source potion key=5 frame=60 result=1 consumed=1 quantity=5->4 HPraw=37580->37580 MPraw=7744->10304`; 0 sync errors |
| Mage Faery then potion | same failure (`result=0`, sync error) | `Source Faery key=4 frame=30 ... MP=68.5`; `Source potion key=5 frame=150 result=1 consumed=1 quantity=5->4 MPraw=17536->20096`; 0 sync errors |
| Knight BashDown then potion (regression) | `result=1 consumed=1 quantity=5->4 MPraw=5440->6976` | identical |

Result: PASS. Potion consumes one, restores MP, and produces no sync error. HP was already full, so the HP restore is not observable in these runs. Captures were not written (path issue above).

## (2) B044 Rogue Swamp, fresh-player args
Args: C-worker `c-skills/rogue/keys.args` with asset paths made relative.
- P12: `Source player melee bank class=RoguePlayerBase ... off=-1` then `Foundation error: Combat initialization: Selected source phase has no damage marker: RoguePlayerBase/attack_offhand` (exit 1).
- WIP: same bank line, no marker error, reaches gameplay. JumpKick runs (`Source skill key=2 ... skill=JumpKick MP=30.25`).
- Attack/kill (WIP, `b044-kill.args`: Rogue at `-6752.641,938.285,250`, `--combat-ai-gate Swamp_LizadMan_Type1=Limbus`, Space held 30:260, 300 frames): Rogue hits land with attacker `18446744073709551615` (player), e.g. `Damage frame=46 ... removed=16.5 dead=0`; lizard `7118915781085668844` killed at frame 175 (`removed=0.597 dead=1`) and a second kill at frame 289 (`dead=1`). XP: `xpRecipients=1 xp=8`, then `xp=16`.
- Result: PASS for the Swamp path. The "one hand, no offhand" trigger is what I ran; the dual-dagger r1 case is covered by the I-worker.

## (3) B040 level-music logging (WIP)
- Real-time run, Swamp, 30 s (`rt30.args`, `--frames 1800`): exactly one line `Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000 ordinal=211`. Shutdown: `Audio final dispatched=10 startedVoices=63`, `WinMM pump: underruns=0 ...`, `Rendered frames=1800; clean shutdown`, exit 0.
- Return to menu (`b040.args`, real OS clicks on the window at pause frame 60: Main Menu at client (594,505), then Yes at (444,412)). Pause opens, confirm dialog "Return to main menu?" appears (captured), Yes confirms at frame 474-477 (`Pause menu confirmed main menu frame=474`). Logs: `Pause main-menu save prefix`, then the gameplay teardown (`Source FX final`, `Audio final`, `Rendered frames=474; clean shutdown`), then stderr `Audio return-to-menu diagnostic: Audio host is unavailable`. No `return-stop` line is logged. The frontend (Player Warrior, Level 1 slot) is shown afterwards (screenshot), and the process keeps running there, which is expected for the menu.
- Result: start-once PASS. Stop-on-menu-return logging REJECT: the teardown (audio summary and shutdown) runs before the return-to-menu block in `main.cpp` (source-order inference), so `RuntimeSessionAudioV1::on_return_to_menu` finds `host_` null and never logs `return-stop`. The fix is to call `on_return_to_menu` before the teardown, or to keep the host alive until then.
- Preview 12 A/B for this path: not established (P12 has no level-music log lines; my clicks hit a different pause button in P12).
- Two WIP processes from the return runs were stopped by me (PIDs 267500, 95036) after the screenshot. I did not kill any user process.

## (4) Regression (WIP vs P12 Knight Swamp at `-6752.641,938.285,250`)
- B037 PASS: skill mid-swing. `Source skill key=2 ... skill=BashDown ... diagnostic=` (empty) at frame 50, hit 84, Post 131. Combo boundary frames are identical in WIP and P12 (basic swing gen1 30-43, next basic swing gen2 at 132).
- B004 PASS: target marker after skill and release. `--space-key-interval 30:14 --skill-key-frame 100:2 --frames 200`. Target ring and name/HP frame shown on the Bogwomp. WIP and P12 PNGs are byte-identical (md5 `dbe212e7...`). Viewed.
- B038 PASS, visual weak: XP grows on lizard kills (`xp=8` then `xp=16`, `xpRecipients=1`). Top-left XP bar is empty at frame 2 and shows a green fill at frame 300. Estimate only, no mid-run capture. Level-up not observed.
- B041 PASS: `Source player step FX ... sequence=470 step=1 occurrence=6 dispatched=1` at frame 43, sequence 471 `dispatched=1` at 74, sequence 472 `dispatched=1` at 106. Log output is identical in WIP and P12. Trail image not checked.
- B039 PASS: 30.4 s real-time run above: `WinMM pump: underruns=0 refills=2855 renderedSeconds=30.45 wallSeconds=30.39 maxGapMs=33.98 gapsOver40ms=0`.

## (5) Hit behaviour (no knockback on clean hit)
- Knight hit by the Bogwomp at frame 14 (`Damage frame=14 attacker=7118915781085668844 target=18446744073709551615 ... removed=8.35`). Captures at frame 13 and frame 17 (`shots/wip-knight-hit-13.png`, `-17.png`): hero at the same screen position, ring and sword unchanged. The floor sigil and wall stones are at the same screen positions, so the camera did not move either. Only the "8" damage number appears.
- Rogue hit at frame 14 shows the same (`shots/hit-cap-13.png`, `-17.png`).
- Limits: a 4-frame gap, screen-space only. Per-frame hero position is not logged (`Motion provenance` prints once at spawn). No push-bearing hit (0x98) was run, so the comparison is clean hits only.

## Not verified
- B043 HP restore (HP was full). B044 dual-dagger path (I-worker only). B040 Preview 12 A/B for return-to-menu. Swoosh trail image. Exact hit result flags (not in log).
