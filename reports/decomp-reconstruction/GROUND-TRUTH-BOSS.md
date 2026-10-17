# Ground truth for the swamp-king burrow question (owner GT2, 2026-10-17)

Question: does the original 1.0.2 swamp serpent dive underground when the hero fights it at range (~900 units)?

Result: **not answered**. The original game could be run (see tools/trackb/boot/orig_emulator.md) up to the loading screen of the Boglands with the data cache's WOLF save,
but the process dies on entering gameplay, so the boss (witch cave / swamp) was never reached. No dive timeline was recorded.

What was established
- Original armeabi-v7a APK runs on x86 Android 9 (google_apis_playstore, libndk_translation): intro movie, title, main menu (Start game / Options / Info), slot WOLF Warrior L1 Act 1,
  Start game > Single player > level loads to 100 % ("touch the screen to continue"). Screens: build/own-gt2/boss/s8.png (title), s15.png (menu), s24.png (loading).
- Gameplay entry fails: 32-bit address space exhausted inside ndk_translation (VmSize 4.17 GB with 250 MB RSS) -> `code_pool.cc:46 CHECK failed: start_address_`. Other images/modes: see orig_emulator.md.
- The old reference "ro.product.cpu.abilist32 empty" of FIRST-BOOT.md is true only for API 33+/37 images; API 28 and API 30 x86 images do translate armeabi-v7a.

Not done: mage character (new game dies in character select, same address-space limit), fight, screenshots every 2 s, logcat of a fight.
Next candidates: an old emulator build with qemu-system-armel plus system-images;android-25;google_apis;armeabi-v7a (slow); a rooted API 30 image whose translator is patched so the interpreter FPE
disappears (then /proc/<pid>/maps shows what eats the address space); or the glitch-in-Unicorn chain for the timeline (CHAINS.md, Extension 3, "Open").

## GT3 result (2026-10-17): the original was played into the swamp-king fight and the boss DOES dive underground at range
Route A worked (see "GT3" in tools/trackb/boot/orig_emulator.md: the abort was a leaked-thread-stack address-space exhaustion in the game's own updateJob_thread::Start, fixed by a detached-thread patch
of a copy of the apk; x86 playstore API 28 image, ndk_translation, -memory 4096). Warrior WOLF level 1 (full HP 310, 5 potions), level 001_swamp, hero starting at the boss module's door.
Setup of every fight: hero at (-18800,11245) after the intro cut scene, boss spawn (-19536,11679): distance ~855 units (the "about 900 units at range" case), hero stands still (no attack, potions only).
Evidence: build/own-gt3/rec (about 2.6 screenshots per second, file name = HHMMSS_ms, 1280x720) + tools/trackb/boot/gt3/boss.py and tl.py; times below are host wall clock (emulator runs near real time).
Boss visibility = bright green head/neck at the top screen edge and the "Повелитель грязи" name plate; hero hits = HP bar drops.

Fight 2 (08:21): cut scene shows the boss from 08:21:15.4; control returns 08:21:21 (boss mid roar); spit hit 08:21:26.1 (-94 HP); head gone 08:21:31.8 (= 16.4 s after the boss first appears);
hit 08:21:32.3 (-98); boss never seen at the top again, hits keep coming from outside the view (08:21:52 -105).
Fight 3 (08:23): boss first visible 08:23:42.0; control 08:23:48.7 (roar); spit hit 08:23:51.5 (-101); head sinks 08:23:58.0 and is gone at 08:23:58.9 (= 16.9 s after first visible); crystals/emerge strike
at 08:24:02.2 (-112 HP, 3.3 s after the dive, boss green column shown for 1 s at the top edge); then hidden again, hit pairs 5.8-5.9 s apart (08:24:07.9, 08:24:29.0 / :34.9 / :40.7, 08:25:03 / :12.7) with
gaps of 20-22 s between pairs.
Fight 4 (08:31): boss visible from 08:31:23.9; control 08:31:36.5; spit hit 08:31:37.4 (-105); head gone 08:31:39.9 (= 16.0 s after first visible); big hit 08:31:42.6 (-118, 2.7 s after the dive);
not visible at the top for the next 70 s, hit pairs 08:31:48.9 (-111), 08:32:05 (-111), 08:32:31 / :37 (-101), ...
Fight 1 (08:14, started with a long potion-tutorial cut scene, hero HP already 107): boss visible 08:14:08 - 08:15:30 with only 0.4 s head dips every ~3 s (spit animation), NO dive in 80 s although the hero stayed in range.
Reading: in 3 of 4 fights the first dive came 16.0-16.9 s after the boss first appeared in the intro cut scene (15 s dive tick + ~1 s), independent of when the player got control (T0+10.2 / +3.4 s),
and its first contact with the hero was one spit/emerge hit 0.5-3.3 s later. The dive at range therefore exists in the original; it happens at the FIRST 15 s tick in three runs; the run that had
a 40 s tutorial cut scene (boss in view, hero low HP) did not dive at all, i.e. the dive depends on the cycle phase at the tick, as the recon analysis (CHAINS.md Extension 3) predicted, but
the phase in the original's intro cut scene lets the first tick land on an Idle gap (the reconstruction's phase made the tick fall inside intimidate/spit). Candidate to test: the original boss is
activated when the cut scene starts showing it (not when control returns), and its first cycle starts one 1.5 s grid later than ours.
After the dive the boss resurfaces at another BossSpawnPoint (not visible in the top band) and keeps spitting from there (hit pairs 5.8 s apart, 20 s gap = next dive/resurface).
Caveats: n = 4 fights, one hero level, boss name plate/visibility measured from screenshots (not from the engine state); the exact intro-cut-scene length depends on how fast the dialogue is tapped.
