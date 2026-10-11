import json, os, re, shutil, hashlib, sys
os.chdir('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs')
src = 'windows-source-clock-v19-preview-16'
dst = 'windows-source-clock-v19-preview-16-1'
exe = 'C:/Users/adamc/Desktop/workspace/DH_wt/build-p161int/dh-foundation.exe'
commit = sys.argv[1]
assert not os.path.exists(dst)
shutil.copytree(src, dst)
shutil.copy2(exe, dst + '/dh-foundation.exe')

# B076 new decor assets (device-folder files, same relative names under assets/data/3d)
extra = 'assets-extra/android/data/3d/'
new = ['animateddecors/swamp_tikitorches.bdae', 'animateddecors/swamp_watercaustics.bdae',
       'animateddecors/swamp_caveentrance_effect.bdae', 'animateddecors/swamp_cave_entrance_troll.bdae',
       'animateddecors/swamp_rayolight_dust.bdae', 'props/swamp/prop_swamp_corpses.bdae']
for rel in new:
    d = dst + '/assets/data/3d/' + rel
    os.makedirs(os.path.dirname(d), exist_ok=True)
    shutil.copy2(extra + rel, d)

def sha(p):
    h = hashlib.sha256()
    with open(p, 'rb') as f:
        for b in iter(lambda: f.read(1 << 20), b''):
            h.update(b)
    return h.hexdigest().upper()

exe_sha = sha(dst + '/dh-foundation.exe')
exe_bytes = os.path.getsize(dst + '/dh-foundation.exe')

fixes = [
 "B069 Space on a chest/barrel out of reach now walks the hero to the object and interacts on arrival (original AI_IsInInteractionRange rule); previously Space silently did nothing between 200 and ~400 units",
 "B070 tutorials are one-shot and persist across relaunches (dh2_settings.tutorials next to the --save profile, original dh2_settings.savegame rule)",
 "B071 scripted FakeMoths from the MothIntro cutscene now leave the Spawn state (full idle state change like SM_SetIdleState): targetable, killable",
 "B072 Map page draws the original _minimap/_floor silhouettes, parchment region and tint, icon clipping",
 "B073 Quest Journal: header 'Quest Journal', icons and clip masks, side-quest bullet, current quest selected on open, MAKE ACTIVE keeps the selection",
 "B074 attack range: no divergence found against the original (reach = Param5 + MeleeRadius for both parties); no code change",
 "B075 attack ring under a targeted unbroken destructible (vase/barrel); walk-over breaking not reproduced (original contact handler is empty)",
 "B076 hero point light (PlayerLight), specular, swamp ambient decor (tiki torches, water caustics, cave entrance, corpses), clear colour from the level config",
 "B077 Rene and Celeste follow the hero after the opening (general follower controller from the AI row script)"]
openl = [
 "NOT in 16.1: B068 cinematics (input lock during cutscenes, SKIP hit-test, lizard ambush placement, camera/animations, caption speaker name, tutorial panel centring, New Quest banner timing, troll/cage scene) - in progress, planned as 16.2",
 "B077: Rene does not shoot (ranged attack not implemented); stop distances from the formula, not measured against the video",
 "B076: dust-mote particle clouds, torch bone breathing, specular on alpha-blended ranges and actor lighting not done",
 "B072: zoom/pan with mouse wheel, drag and keys unit tested only (no input in headless runs); orientation kept from IDA; zoom range 1-4 is a PC adaptation",
 "B073: MAKE ACTIVE and tab-row clicks not exercised in the real EXE",
 "B075: mouse targeting of vases not implemented; chest ring art missing",
 "B071: moth wrong animation and following inferred to come from the stuck Spawn state, not compared frame by frame with the video",
 "B070: Linux/Android hosts must set tutorial_settings_path; New Game does not reset the tutorial flags (as in the original)",
 "Carried from Preview 16: respawn of LizTemplate_03 unverified against the original; witch zone dialogue, kill objective progress, ambush after the opening, helm after in-game equip, red X, continue after SKIP not independently verified; level-up white column is a placeholder; no audio verification"]

n = {
 'preview': '16.1', 'status': "Preview 16.1 (B069-B077 fixes) for the user's hand testing; merged build verified by ctest, not yet independently verified (verifier report pending); B068 cinematics not included",
 'platform': 'Windows x64', 'base_package': src, 'exe_sha256': exe_sha,
 'exe_source_commit': commit + ' (branch p16.1/integrate: windows-foundation 461ae9fc + fix/b069..b077 merges)',
 'fixes': fixes, 'open': openl,
 'added_files': ['assets/data/3d/' + r for r in new]}
json.dump(n, open(dst + '/release-notes.json', 'w'), indent=2)
r = {'preview': '16.1', 'utc': '2026-10-11', 'exe_sha256': exe_sha, 'exe_bytes': exe_bytes,
     'source_commit': commit + ' branch p16.1/integrate (DH_wt/p161int, build DH_wt/build-p161int)',
     'base_package': src + ' (copied; folder not modified)',
     'ctest': '142 of 143 pass; session_skill_binding fails (known worktree junction issue, allowed)',
     'status': 'for hand testing; independent verification pending'}
json.dump(r, open(dst + '/package-receipt.json', 'w'), indent=2)
t = open(dst + '/README.txt', encoding='utf-8').read()
t = t.replace('V19 Preview 16', 'V19 Preview 16.1')
t = re.sub(r'Preview 16 for hand testing[^\n]*\nThis is the RC3 build[^\n]*\n',
           'Preview 16.1 for hand testing: Preview 16 plus fixes B069-B077 (see release-notes.json). Cinematic fixes (B068) are not included.\n', t)
open(dst + '/README.txt', 'w', encoding='utf-8').write(t)

files = []
skip = {'manifest.json', 'package-receipt.json', 'release-notes.json'}
for root, _, fs in os.walk(dst):
    for f in fs:
        p = os.path.join(root, f)
        rel = os.path.relpath(p, dst).replace('\\', '/')
        if rel in skip:
            continue
        files.append({'path': rel, 'bytes': os.path.getsize(p), 'sha256': sha(p)})
files.sort(key=lambda x: x['path'])
json.dump({'preview': '16.1', 'files': files}, open(dst + '/manifest.json', 'w'), indent=2)
print('ok', exe_sha, len(files))
