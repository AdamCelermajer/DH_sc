"""Package a local diagnostic checkpoint; preserve the accepted earlier package."""
from pathlib import Path
import hashlib
import json
import shutil
import argparse

workspace = Path(__file__).resolve().parents[3]
build = workspace / '.local-inputs/windows-foundation-build'
source = workspace / '.local-inputs/windows-shared-assets'
parser = argparse.ArgumentParser()
parser.add_argument('--destination', type=Path, default=workspace / '.local-inputs/windows-shared-checkpoint')
parser.add_argument('--retained', action='store_true', help='Supply retained source animation clock policy in package data')
parser.add_argument('--enemy-ai', action='store_true', help='Enable shared enemy navigation and combat in interactive launchers')
parser.add_argument('--population-templates', action='store_true', help='Supply original weighted population choices and extra source combat profiles')
parser.add_argument('--executable', type=Path, default=build / 'dh-foundation.exe', help='Stable built executable to package')
parser.add_argument('--audio-assets', type=Path, help='Original source metadata and WAV asset root')
parser.add_argument('--audio-table', type=Path, help='Original legacy sound table')
parser.add_argument('--source-assets', type=Path, help='Unified original gameplay/frontend asset root')
parser.add_argument('--frontend', action='store_true', help='Start the primary executable in the character frontend; preserve direct gameplay launchers')
parser.add_argument('--animation-only-idle', action='append', default=[], metavar='PROFILE:STATE:VARIANT:PATH', help='Enroll an original noncombat profile in the shared animation clock')
settings = parser.parse_args()
if settings.source_assets:
    source = settings.source_assets.resolve()
if bool(settings.audio_assets) != bool(settings.audio_table):
    parser.error('--audio-assets and --audio-table must be supplied together')
if settings.population_templates and not (settings.retained and settings.enemy_ai):
    parser.error('--population-templates requires --retained and --enemy-ai')
destination = settings.destination.resolve()
if destination.exists() and any(destination.iterdir()):
    parser.error('Destination must be empty; use a new preview directory to preserve released builds')
destination.mkdir(parents=True, exist_ok=True)
shutil.copyfile(settings.executable.resolve(), destination / 'dh-foundation.exe')
asset_folders = ('animations', 'data', 'models', 'original-cache', 'textures')
if settings.source_assets:
    asset_folders = tuple(path.name for path in source.iterdir() if path.is_dir())
for folder in asset_folders:
    for asset in (source / folder).rglob('*'):
        if not asset.is_file():
            continue
        output = destination / 'assets' / asset.relative_to(source)
        output.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(asset, output)
for filename in ('actor-profiles-v2.xml', 'original-melee-bindings.xml', 'development-controller.xml'):
    shutil.copyfile(source / filename, destination / 'assets' / filename)
if (source / 'original-campaign.xml').is_file():
    shutil.copyfile(source / 'original-campaign.xml', destination / 'assets/original-campaign.xml')
shutil.copyfile(workspace / 'port/engine-ui/vendor/freetype-2.3.7-hud/docs/FTL.TXT', destination / 'FreeType-license.txt')

common = [
    '--start-mode', 'swamp',
    '--assets', 'assets', '--level', 'data/scene/001_swamp.mlx',
    '--profiles', 'actor-profiles-v2.xml', '--condition-active', 'RENE_FOLLOW',
    '--model', 'models/prince_modular.bdae', '--template', 'animations/prince_template_anim.bdae',
    '--clip', 'idle=animations/prince_idle_shield.bdae', '--clip', 'walk=animations/prince_walk_slow.bdae',
    '--clip', 'run=animations/prince_walk_1hand.bdae', '--clip-rate', 'walk=1.2999999523162842',
    '--clip-rate', 'run=1.2999999523162842', '--skin', '_default_warrior-mesh-skin',
    '--actor-row', 'KnightPlayerBase', '--fresh-player', '--motion-node', 'auto',
    '--move', '--controller-policy', 'development-controller.xml', '--original-camera', '--hud', '--original-body-bounds',
    '--attach', 'data/3d/characters/prince/weapons/mc_rweapon_longsword_01.bdae@anchor_weapon_right_offset',
    '--combat-bindings', 'original-melee-bindings.xml', '--combat-player', 'KnightPlayerBase', '--combat-seed', '1234',
    '--combat-sequence', 'KnightPlayerBase:AttackStatic:0:0', '--combat-idle', 'KnightPlayerBase:Idle:0:0',
    '--combat-react', 'KnightPlayerBase:Injured:0:0', '--combat-death', 'KnightPlayerBase:Died:0:0',
    '--combat-marker', 'KnightPlayerBase=attack_mainhand',
    '--combat-sequence', 'Swamp_LizadMan_Type1:Attack:0:0', '--combat-idle', 'Swamp_LizadMan_Type1:Idle:0:0',
    '--combat-react', 'Swamp_LizadMan_Type1:Injured:0:0', '--combat-death', 'Swamp_LizadMan_Type1:Died:0:0',
    '--combat-marker', 'Swamp_LizadMan_Type1=attack_mainhand',
    '--equipped-item', 'StartingSuit', '--equipped-item', 'StartingBoots', '--equipped-item', 'StartingGloves',
    '--equipped-item', 'Longsword01', '--combat-main-item', 'Longsword01', '--game-save', 'gameplay.save',
]
if settings.retained:
    common += ['--combat-text', '--source-root-scopes', 'original-cache/data/scene/001_swamp.mlx', '--original-native-bodies', '--original-floor-motion', '--original-heading-rotation', '--original-target-position', '--combat-retained-phase', 'KnightPlayerBase', '--combat-retained-phase', 'Swamp_LizadMan_Type1',
               '--combat-motion-root', 'Swamp_LizadMan_Type1=auto',
               '--combat-locomotion', 'idle:Idle:0:0', '--combat-locomotion', 'walk:Walk:0:0', '--combat-locomotion', 'run:Run:0:0',
               '--original-actor-anchor', 'forward']
world = common + ['--position', '1090.75,-212.202,258']
combat = common + ['--position', '-6752.641,938.285,250', '--combat-auto', '--combat-ai-gate', 'Swamp_LizadMan_Type1=Limbus']
def config(name, arguments):
    (destination / name).write_text('# One argument per line. This file supplies content, not engine map logic.\n' + '\n'.join(arguments) + '\n', encoding='utf-8')
def playable(arguments):
    if not settings.retained:
        return arguments
    result = [value.replace('KnightPlayerBase:AttackStatic:0:0', 'KnightPlayerBase:AttackStatic:0:*')
              .replace('KnightPlayerBase:Attack:0:0', 'KnightPlayerBase:Attack:0:*') for value in arguments]
    return result + ['--combat-source-combo', 'KnightPlayerBase']
config('startup.args', playable(world))
if settings.enemy_ai:
    config('startup.args', playable(world) + ['--enemy-ai'])
    encounter = playable(common + ['--position', '-6852.64,300,255']) + ['--enemy-ai']
    config('enemy-combat.args', encounter)
    config('enemy-approach-verification.args', encounter + ['--fixed-step', '.016', '--frames', '180', '--capture', 'enemy-approach-verification.ppm'])
    config('enemy-fight-verification.args', encounter + ['--fixed-step', '.016', '--frames', '600', '--attack-start-frame', '110', '--attack-frames', '490', '--capture', 'enemy-fight-verification.ppm'])
    for launcher, configuration in [('Play.cmd', 'startup.args'), ('Test-enemy-combat.cmd', 'enemy-combat.args')]:
        (destination / launcher).write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config ' + configuration + '\r\nif errorlevel 1 pause\r\n', encoding='utf-8')
if settings.population_templates:
    # This is supplied validation content, not an engine actor-name branch.
    authored = json.loads((workspace / 'port/windows-foundation/reports/population-source.json').read_text(encoding='utf-8'))
    rows = sorted({name for actor in authored['actors'] if actor.get('template') for name in actor.get('character_choices', [])})
    population_arguments = ['--population-templates', '--enemy-ai']
    for row in rows:
        if row + '=attack_mainhand' in common:
            continue
        population_arguments += ['--combat-sequence', row + ':Attack:0:0', '--combat-idle', row + ':Idle:0:0',
                                 '--combat-react', row + ':Injured:0:0', '--combat-death', row + ':Died:0:0',
                                 '--combat-marker', row + '=attack_mainhand', '--combat-retained-phase', row,
                                 '--combat-motion-root', row + '=auto']
    config('startup.args', playable(world) + population_arguments)
    config('population-verification.args', playable(world) + population_arguments + ['--fixed-step', '.016', '--frames', '120', '--save-frame', '40', '--reload-frame', '80', '--load-frame', '100', '--capture', 'population-verification.ppm'])
    moth_encounter = playable(common + ['--position', '-5848.6,2200,255']) + population_arguments
    config('populated-combat.args', moth_encounter)
    config('populated-combat-verification.args', moth_encounter + ['--fixed-step', '.016', '--frames', '240', '--capture', 'populated-combat-verification.ppm'])
    (destination / 'Test-populated-combat.cmd').write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config populated-combat.args\r\nif errorlevel 1 pause\r\n', encoding='utf-8')
config('menu-verification.args', world + ['--fixed-step', '.016', '--frames', '60', '--profile-click-frame', '5', '--capture', 'menu-verification.ppm'])
config('menu-close-verification.args', world + ['--fixed-step', '.016', '--frames', '60', '--profile-click-frame', '5', '--menu-close-frame', '35', '--capture', 'menu-close-verification.ppm'])
for name, dimensions in [('menu-wide', '1600,900'), ('menu-four-three', '960,720')]:
    config(name + '-verification.args', world + ['--fixed-step', '.016', '--frames', '60',
           '--profile-click-frame', '5', '--resize-at', '25:' + dimensions,
           '--capture', name + '-verification.ppm'])
config('menu-four-three-fresh-verification.args', world + ['--fixed-step', '.016', '--frames', '60',
       '--window-size', '960,720', '--profile-click-frame', '5', '--capture', 'menu-four-three-fresh-verification.ppm'])
config('combat.args', playable(combat))
config('resume.args', playable(world + ['--resume-game']))
moving = [value.replace('KnightPlayerBase:AttackStatic:0:0', 'KnightPlayerBase:Attack:0:0') for value in combat]
if not settings.retained:
    moving += ['--combat-motion-root', 'Swamp_LizadMan_Type1=auto']
config('moving-combat.args', playable(moving))
if settings.retained:
    combo_base = playable([value for value in combat if value != '--combat-auto'])
    for name, held_frames in [('combo-held', 100), ('combo-release', 1), ('combo-accepted-release', 35), ('combo-blocked', 40)]:
        arguments = combo_base + ['--fixed-step', '.016', '--frames', '180',
                                 '--attack-start-frame', '10', '--attack-frames', str(held_frames),
                                 '--capture', name + '-verification.ppm']
        if name == 'combo-blocked':
            arguments += ['--campaign-commands', 'original-campaign.xml',
                          '--campaign-command', 'LizardMan_Intro:1:20',
                          '--campaign-command', 'LizardMan_Intro:8:60']
        config(name + '-verification.args', arguments)
    config('combat-text-verification.args', combo_base + ['--fixed-step', '.016', '--frames', '65',
           '--attack-start-frame', '10', '--attack-frames', '100', '--capture', 'combat-text-verification.ppm'])
    config('combat-text-restore-verification.args', combo_base + ['--fixed-step', '.016', '--frames', '180',
           '--attack-start-frame', '10', '--attack-frames', '100', '--save-frame', '50',
           '--reload-frame', '100', '--load-frame', '130', '--capture', 'combat-text-restore-verification.ppm'])
    config('combat-text-resize-verification.args', combo_base + ['--fixed-step', '.016', '--frames', '65',
           '--attack-start-frame', '10', '--attack-frames', '100', '--resize-at', '40:1600,900',
           '--capture', 'combat-text-resize-verification.ppm'])
config('moving-verification.args', moving + ['--fixed-step', '.016', '--frames', '360', '--attack-start-frame', '60', '--attack-frames', '300', '--save-frame', '210', '--reload-frame', '250', '--load-frame', '300', '--capture', 'moving-verification.ppm'])
config('verification.args', combat + ['--fixed-step', '.016', '--frames', '360', '--attack-start-frame', '60', '--attack-frames', '300', '--save-frame', '210', '--reload-frame', '250', '--load-frame', '300', '--capture', 'verification.ppm'])
config('resume-verification.args', combat + ['--resume-game', '--fixed-step', '.016', '--frames', '30', '--capture', 'resume-verification.ppm'])
config('run-verification.args', world + ['--fixed-step', '.016', '--frames', '90', '--move-axis', '1,0,0', '--move-frames', '60', '--move-run', '--capture', 'run-verification.ppm'])
config('floor-boundary-verification.args', world + ['--fixed-step', '.016', '--frames', '700', '--move-axis', '1,0,0', '--move-frames', '650', '--move-walk', '--capture', 'floor-boundary-verification.ppm'])
if settings.retained and (source / 'original-campaign.xml').is_file():
    spawning = common + ['--position', '-3500,250,255', '--retain-hidden-actors',
        '--lifecycle-spawn', 'Swamp_LizadMan_Type1:Spawn:0:*',
        '--campaign-commands', 'original-campaign.xml',
        '--campaign-command', 'LizardMan_Intro:4:33', '--campaign-command', 'LizardMan_Intro:6:128',
        '--combat-auto', '--combat-ai-gate', 'Swamp_LizadMan_Type1=Limbus']
    config('spawn.args', spawning)
    config('spawn-verification.args', spawning + ['--fixed-step', '.016', '--frames', '400', '--attack-start-frame', '160', '--attack-frames', '240', '--capture', 'spawn-verification.ppm'])
    config('spawn-scoped-verification.args', spawning + ['--campaign-context-object', '_prim_TriggerZone_LizManIntro', '--fixed-step', '.016', '--frames', '400', '--attack-start-frame', '160', '--attack-frames', '240', '--capture', 'spawn-scoped-verification.ppm'])
    config('source-floor-verification.args', world + ['--source-floor-probe', '--fixed-step', '.016', '--frames', '90', '--move-axis', '1,0,0', '--move-frames', '60', '--move-walk', '--capture', 'source-floor-verification.ppm'])
    config('spawn-inflight-verification.args', spawning + ['--fixed-step', '.016', '--frames', '95', '--capture', 'spawn-inflight-verification.ppm'])
    camera_spawning=spawning+['--campaign-command','LizardMan_Intro:2:0','--campaign-command','LizardMan_Intro:9:254']
    config('spawn-camera.args',camera_spawning)
    config('spawn-camera-transition-verification.args',camera_spawning+['--fixed-step','.016','--frames','32','--capture','spawn-camera-transition-verification.ppm'])
    config('spawn-camera-verification.args',camera_spawning+['--fixed-step','.016','--frames','400','--attack-start-frame','160','--attack-frames','240','--capture','spawn-camera-verification.ppm'])
    locked = [v for v in spawning]
    for command in ('LizardMan_Intro:4:33','LizardMan_Intro:6:128'):
        index=locked.index(command);del locked[index-1:index+1]
    config('controller-lock-verification.args',locked+['--campaign-command','LizardMan_Intro:1:0','--campaign-command','LizardMan_Intro:8:60','--fixed-step','.016','--frames','90','--move-axis','1,0,0','--move-frames','60','--move-walk','--attack-start-frame','0','--attack-frames','60','--capture','controller-lock-verification.ppm'])
    admission=['--campaign-commands','original-campaign.xml','--campaign-command','LizardMan_Intro:1:0','--campaign-command','LizardMan_Intro:8:60','--fixed-step','.016','--frames','90','--attack-start-frame','0','--attack-frames','60']
    config('controller-independent-verification.args',common+['--position','-3500,250,255']+admission+['--move-axis','1,0,0','--move-frames','60','--move-walk','--capture','controller-independent-verification.ppm'])
    config('controller-combat-verification.args',combat+admission+['--capture','controller-combat-verification.ppm'])
    active_admission=[value.replace('LizardMan_Intro:1:0','LizardMan_Intro:1:20') for value in admission]
    config('controller-active-verification.args',combat+active_admission+['--capture','controller-active-verification.ppm'])
    (destination / 'spawn-camera-demo.cmd').write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config spawn-camera.args\r\nif errorlevel 1 pause\r\n', encoding='utf-8')
    (destination / 'spawn-demo.cmd').write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config spawn.args\r\nif errorlevel 1 pause\r\n', encoding='utf-8')
for name, configuration in (('combat-demo.cmd', 'combat.args'), ('resume-game.cmd', 'resume.args'), ('moving-combat-demo.cmd', 'moving-combat.args')):
    (destination / name).write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config ' + configuration + '\r\nif errorlevel 1 pause\r\n', encoding='utf-8')

(destination / 'README.txt').write_text('''Dungeon Hunter reconstruction: shared playable diagnostic checkpoint

Double-click dh-foundation.exe for the authored starting world.
combat-demo.cmd starts beside an original Bogwomp with explicitly enabled
stationary diagnostic AI. Press Space to fight. This does not reproduce the
original encounter activation script.
moving-combat-demo.cmd supplies the original moving attack group and enables
the NPC root motion service. Movement follows authored MoveGO and clip data.

WASD: run. Hold Shift: walk. Hold Space: continue the original swing sequence.
Click the portrait or press C to open the original-art character panel.
Escape closes the panel; its back button also closes it. Stats are live.
The stats panel fills the window, including after resize. Equipment, Skills and
Faery pages remain in development; this checkpoint does not complete the menu.
Combat numbers and Miss/Dodge/Block feedback use original results and artwork.
Tab: cycle target. F5: save.
F9: restore. R: reload content while preserving live data. Escape: exit.
resume-game.cmd starts another process and restores gameplay.save.
Keep the assets folder and startup.args beside the executable.

Original models, textures, walk/run and attack phases, source stat/weapon damage,
health/mana/target art, localized labels and original font are connected. The
renderer and shared gameplay code accept supplied data; this area is validation
content. Initial window size follows the authored camera aspect; resize updates
the projection aspect through the recovered driver lifecycle policy.

Component checks and native rendered fights have been exercised. See the
source workspace reports for the final check count and exact receipts.

Incomplete: original campaign spawning/AI, navigation and physical-body fidelity,
combo chaining and integrated blending/cadence, status/DOT/leech,
companion behavior, full HUD/menus/effects/audio, cinematic/campaign coverage and
matched-state visual acceptance. Rene's supplied-model appearance differs from
the reference. The reference is titled v1.0.3; the supplied APK manifest is v1.0.2.

This is progress toward the active reconstruction goal, not full-game acceptance.
''', encoding='utf-8')
if settings.retained:
    with (destination / 'README.txt').open('a', encoding='utf-8') as description:
        description.write('''\nThis package enables retained source animation clocks for combat actors.
Initial attack groups, reaction/death and NPC idle share original two-slot
blending, serialized events, timeline completion and source replay cadence.
Player idle/walk/run now share that owner with combat. Shift selects the exact
original Run state leaf; its distinct root motion determines travel. The source
forward camera anchor is enabled
with the explicit absent-debug-save switch result. Desired movement heading is
separate from visual facing. Original input/attack heading command gates,
synchronous callback reentry and matched-state visual/timing acceptance remain
incomplete. The clean version1 save format
canonicalizes transient actions: saved dead actors keep their saved positions
and terminal death poses; loading does not replay death displacement.

spawn-demo.cmd is a separate lifecycle diagnostic. It replays the TWO serialized
Spawn commands from the original encounter at explicit configured frames using
global context -1; it does not execute the complete encounter script or original
trigger admission. Hidden source visuals remain loaded and disabled, Spawn plays
through the shared owner, and whole completion admits Idle/combat. Registered
player/enemy bodies use shared source rest bounds, native body creation and source
position/filter rules. Spawn removes and recreates actual bodies. Full navigation,
contact dispatch, aggro/sneak and campaign UI/camera/trigger providers remain incomplete. Campaign lifecycle saving/loading/reloading is explicitly
rejected because that state is not yet serialized; ordinary startup/combat saves
remain available. No first-map/name branches are added to gameplay code.
spawn-camera-demo.cmd additionally replays the original SetCameraTarget commands
with their authored one-second transitions, from the live player anchor to the
authored dummy and back. The full trigger/cutscene script remains incomplete.
Original joint-box/rest bounds provide player and enemy radii. Native body
positions follow shared actors and ordinary restore rebuilds them. The native
world is not stepped yet. The retained package uses original floor position
validation and PF radius/obstacle updates; heading/path coordination, actor/decor
collision response and full physics remain incomplete. COMMON material
passes use original opaque/alpha/additive depth and blend state, with native
pixel tests. Lighting values and full renderer/campaign fidelity remain pending.
Root module scopes now use IDs from actual source constructors. The scoped Spawn
verification uses its authored owning module. Full level/trigger activation is
still incomplete. source-floor-verification.args builds original PF floor rooms
with explicit diagnostic geometry admission. Original floor movement replaces
preview sweeps and axis sliding when enabled, so unsupported diagonal steps can
clamp. This does not establish full campaign admission or physics coordination.

Manual player Move now uses the original heading boundary continuation after
sampled root motion, then applies original late Character rotation using shared
actor heading, Euler and turn storage. Move focus uses its recovered prefix.
Idle/attack focus tails, full controller/FSM/path/avoidance and physics frame
remain incomplete. This scoped continuation does not establish visual fidelity.

Retained attack hit markers now apply shared combat damage and bound victim
reaction/death poses inside the source helper, before state-event forwarding.
Legacy marker bindings keep their existing queue. Initial begin-time markers,
full original damage/AIS/FSM reentry and global actor update ordering remain
outside this proof. Callback generation guards protect interrupted poses.

Retained frame callbacks now deliver source named events before actor/root motion.
This corrects hit-versus-displacement ordering; original in-sampler pose/socket
callbacks and complete early-scene/late-actor frame timing remain incomplete.
Own scene target nodes use authored case-insensitive NAME lookup. Their derived
position caches rebuild after reload/restore and are never saved as node tokens.
Current packaged actors prove the absent-node branch; native original boss asset
checks prove a real non-null node. Combat target selection and HUD are separate.

Interactive retained player attacks select the complete original root sequence.
Held commands use source pre/strike/recovery input windows and continuation;
release preserves a continuation already accepted by the source. One animation
owner and action generation span all groups. Original health outcomes are kept.
Restore resets transient combo state and begins a fresh action at the first group;
it does not resume the original mid-swing cursor. Source heading/OOI flags,
sticky targeting, look and pre-attack modifier consumers remain integration gaps.
The first-group verification fixtures preserve earlier regression coverage;
the combo fixtures independently verify the full playable sequence.
''')
files = []
if settings.enemy_ai:
    (destination / 'README.txt').write_text('''Dungeon Hunter 2 reconstruction — V19 playable preview

Play.cmd starts at the authored Swamp starting position with shared enemy AI.
Test-enemy-combat.cmd starts near a bound original enemy so you can test its
approach, attacks and the player's swing sequence immediately. This placement
is a diagnostic; it does not reproduce original encounter triggers.

WASD: run. Shift: walk. Hold Space: attack sequence. Tab: cycle target.
Portrait / C: character panel. Escape: close panel or exit. F5 / F9: save / load.

New: bound enemies acquire targets, approach using the shared floor/navigation
and animation system, and attack using the same authored hit/damage/reaction
system as the player. Authored NPC root movement now works independently of
keyboard input. Static line of sight and original aggro/range data are used.

Still incomplete: full authored mob population and campaign triggers, exact AI
scheduling/search ordering, obstacle detours/dynamic avoidance, full character
pages, frontend character creation, audio/effects enrollment, companions and
cinematics. This preview does not claim full Act 1 or full visual fidelity.
The assets/reference versions differ (recovered 1.0.2, reference titled 1.0.3).

Keep assets and startup.args beside the executable. Previous releases are
preserved separately. The test launchers share this package's gameplay.save.
''', encoding='utf-8')
if settings.population_templates:
    with (destination / 'README.txt').open('a', encoding='utf-8') as description:
        description.write('''
V19 Preview 3 adds original weighted character-template selection. The authored
Swamp validation world now loads 33 visuals instead of 13, including 20 additional
template-based mobs. Original conditions remain enforced: 28 declarations still
remain gated or unsupported. Template selection uses the shared saved RNG.
Test-populated-combat.cmd starts near two original moths for immediate testing.
These extra NPCs use an explicit first authored attack group, as supplied in the
configuration; complete original randomized attack choice/cadence is pending.
Audio is not enrolled in this preview. Save/reload and populated combat have
separate runtime receipts. Diagnostic placement does not prove campaign flow.
''')
if settings.population_templates:
    config('resume.args', playable(world) + population_arguments + ['--resume-game'])
    config('population-resume-verification.args', playable(world) + population_arguments + ['--resume-game', '--fixed-step', '.016', '--frames', '30', '--capture', 'population-resume-verification.ppm'])
if settings.audio_assets:
    for asset in settings.audio_assets.resolve().rglob('*'):
        if not asset.is_file():
            continue
        if asset.suffix.lower() == '.wav':
            original = workspace / '.local-inputs/audio-v34/cache' / asset.name
            if not original.is_file() or hashlib.sha256(original.read_bytes()).digest() != hashlib.sha256(asset.read_bytes()).digest():
                raise ValueError('Audio WAV differs from original cache: ' + str(asset))
        output = destination / 'audio-assets' / asset.relative_to(settings.audio_assets.resolve())
        output.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(asset, output)
    shutil.copyfile(settings.audio_table.resolve(), destination / 'audio-assets/sounds_pyarray.bin')
    audio_arguments = ['--audio', '--audio-assets', 'audio-assets', '--audio-table', 'audio-assets/sounds_pyarray.bin']
    for name in ('startup.args', 'enemy-combat.args', 'populated-combat.args', 'resume.args'):
        path = destination / name
        if path.is_file():
            with path.open('a', encoding='utf-8') as stream:
                stream.write('\n'.join(audio_arguments) + '\n')
    config('audio-verification.args', playable(common + ['--position', '-6852.64,300,255']) + ['--enemy-ai'] + audio_arguments + ['--fixed-step', '.016', '--frames', '420', '--attack-start-frame', '110', '--attack-frames', '490', '--save-frame', '40', '--reload-frame', '80', '--load-frame', '100', '--capture', 'audio-verification.ppm'])
    readme = destination / 'README.txt'
    readme.write_text(readme.read_text(encoding='utf-8').replace('Audio is not enrolled in this preview.', 'Complete global audio remains in development.').replace('audio/effects enrollment', 'complete audio/effects'), encoding='utf-8')
    with readme.open('a', encoding='utf-8') as description:
        description.write('''
V19 Preview 4 enables original combat hit/hurt sounds on primary launchers.
One Windows audio output follows the player/camera and survives save, content
reload and restore. Every packaged WAV matches the original cache bytes.
Swing step-entry sounds, music and complete global audio remain incomplete.
Character schema3 preserves optional quest codec data; older saves load with
unknown quest progress. Full Quest menu enrollment remains in development.
The resume launcher rebuilds the same populated world before restoring.
''')
if settings.animation_only_idle:
    animation_arguments = []
    for choice in settings.animation_only_idle:
        fields = choice.split(':')
        if len(fields) != 4 or not all(fields):
            parser.error('--animation-only-idle requires PROFILE:STATE:VARIANT:PATH')
        animation_arguments += ['--animation-only', fields[0], '--combat-idle', choice]
    for name in ('startup.args', 'enemy-combat.args', 'populated-combat.args', 'resume.args'):
        path = destination / name
        if path.is_file():
            with path.open('a', encoding='utf-8') as stream:
                stream.write('\n'.join(animation_arguments) + '\n')
    verification = destination / 'population-verification.args'
    if verification.is_file():
        (destination / 'companion-verification.args').write_text(verification.read_text(encoding='utf-8') + '\n'.join(animation_arguments + ['--game-save', 'companion-verification.save', '--capture', 'companion-verification.ppm']) + '\n', encoding='utf-8')
    with (destination / 'README.txt').open('a', encoding='utf-8') as description:
        description.write('\nAnimation-only companion profiles use original Idle in the shared session.\nFollowing, attacks, faery abilities and intro placement/cinematics remain incomplete.\n')
if settings.frontend:
    frontend_art = workspace / 'port/windows-foundation/features/frontend/art/assets'
    for asset in frontend_art.rglob('*'):
        if asset.is_file():
            output = destination / 'ui-assets' / asset.relative_to(frontend_art)
            output.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(asset, output)
    startup = destination / 'startup.args'
    direct_arguments = startup.read_text(encoding='utf-8')
    (destination / 'swamp.args').write_text(direct_arguments, encoding='utf-8')
    startup.write_text(direct_arguments.replace('--start-mode\nswamp\n', '--start-mode\nmenu\n', 1) + '--menu-assets\nassets\n--save\ncharacter.save\n', encoding='utf-8')
    (destination / 'Play-swamp.cmd').write_text('@echo off\r\ncd /d "%~dp0"\r\n"%~dp0dh-foundation.exe" --startup-config swamp.args\r\nif errorlevel 1 pause\r\n', encoding='utf-8')
    readme = destination / 'README.txt'
    readme.write_text(readme.read_text(encoding='utf-8').replace('Play.cmd starts at the authored Swamp starting position with shared enemy AI.', 'Play.cmd and the executable start in the original-art character menu.\nPlay-swamp.cmd starts directly at the authored Swamp starting position with shared enemy AI.').replace('frontend character creation, ', ''), encoding='utf-8')
    with readme.open('a', encoding='utf-8') as description:
        description.write('''\nV19 Preview 5 adds menu character creation and selected-profile loading into
the same gameplay window. Warrior combat remains available. Mage and rogue
can be created, rendered and moved; their combat is explicitly incomplete.
Selected-class idle/stance fidelity and the complete opening cinematic remain
in development. Character profiles use character.save; direct gameplay and
diagnostic launchers use gameplay.save. Earlier previews remain preserved.
''')
for path in sorted(destination.rglob('*')):
    if path.is_file() and (path.suffix == '.exe' or 'assets' in path.relative_to(destination).parts or 'audio-assets' in path.relative_to(destination).parts or 'ui-assets' in path.relative_to(destination).parts or path.suffix == '.args'):
        files.append({'path': str(path.relative_to(destination)).replace('\\', '/'), 'bytes': path.stat().st_size, 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()})
receipt = {'package': str(destination), 'goal_complete': False, 'status': 'assembled_runtime_verification_pending', 'files': files}
(destination / 'package-receipt.json').write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'package': str(destination), 'files': len(files), 'executable_sha256': hashlib.sha256((destination / 'dh-foundation.exe').read_bytes()).hexdigest()}))
