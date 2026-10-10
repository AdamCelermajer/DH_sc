"""Verify the packaged inputs and scoped runtime receipts, without accepting fidelity."""
from pathlib import Path
import hashlib
import json
import argparse
import re

workspace = Path(__file__).resolve().parents[3]
parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, default=workspace / '.local-inputs/windows-shared-checkpoint')
parser.add_argument('--report', type=Path, default=workspace / 'port/windows-foundation/reports/packaged-shared-runtime.json')
parser.add_argument('--moving', action='store_true', help='Also check the authored moving attack diagnostic')
parser.add_argument('--retained', action='store_true', help='Verify actual retained source clock cadence')
parser.add_argument('--retained-locomotion', action='store_true', help='Verify explicit retained locomotion and forward anchor diagnostic')
parser.add_argument('--lifecycle', action='store_true', help='Also verify serialized Spawn command replay and lifecycle rendered diagnostics')
parser.add_argument('--movement-camera', action='store_true', help='Verify source Run, camera transitions and initial controller input locks')
parser.add_argument('--controller-admission', action='store_true', help='Verify shared player/AI attack admission and locks without Spawn lifecycle')
parser.add_argument('--animation-routing', action='store_true', help='Verify genuine named40/closure34 source notifications and active markers under locks')
parser.add_argument('--source-body-radius', action='store_true', help='Verify live player radius bound from original rest joint/body definitions')
parser.add_argument('--native-bodies', action='store_true', help='Verify actual native body/position backing, lifecycle removal and restore rebuild')
parser.add_argument('--source-scopes', action='store_true', help='Verify live constructor scopes and module-scoped source Spawn replay')
parser.add_argument('--floor-motion', action='store_true', help='Verify late original PF initialization and shared source floor motion')
parser.add_argument('--heading-rotation', action='store_true', help='Verify original manual heading and late player Move rotation continuation')
parser.add_argument('--synchronous-hits', action='store_true', help='Verify hit markers delivered inside retained source callbacks')
parser.add_argument('--own-target-position', action='store_true', help='Verify authored own scene-node lookup and transient cache publication')
parser.add_argument('--character-menu', action='store_true', help='Verify portrait-input opening and modal close of the original-art development panel')
options = parser.parse_args()
if options.heading_rotation and not options.floor_motion:
    parser.error('--heading-rotation requires --floor-motion')
if options.retained_locomotion and not options.retained:
    parser.error('--retained-locomotion requires --retained')
package = options.package.resolve()
spawn_final_hit = ('Damage frame=295' if options.synchronous_hits else 'Damage frame=296')+' attacker=18446744073709551615'
startup_position = 'Actor final position=1324.6,-198.633,255 grounded=1' if options.heading_rotation else ('Actor final position=1138.32,-199.164,255 grounded=1' if options.floor_motion else ('Actor final position=1272.13,-199.164,255 grounded=1' if options.retained_locomotion else 'Actor final position=1279.79,-201.095,255 grounded=1'))
startup_motion = ['Actor camera anchor final=1580.6,-198.633,255', 'sourceXY=276.028 worldXY=248.425'] if options.heading_rotation else (['Actor camera anchor final=1319.34,-18.1449,255', 'sourceXY=276.028 worldXY=59.1969'] if options.floor_motion else ['Actor camera anchor final=1453.15,-18.1449,255', 'sourceXY=276.028 worldXY=193.002'])
receipt = json.loads((package / 'package-receipt.json').read_text(encoding='utf-8'))
for entry in receipt['files']:
    path = package / entry['path']
    assert path.stat().st_size == entry['bytes'], entry['path']
    assert hashlib.sha256(path.read_bytes()).hexdigest() == entry['sha256'], entry['path']

def runtime(name, required):
    content = (package / name).read_text(encoding='utf-8-sig')
    for needle in required:
        assert needle in content, (name, needle)
    return {'log': str((package / name).relative_to(workspace)).replace('\\', '/'), 'required_observations': required}

checks = [
    runtime('verification.log', [('Damage frame=13' if options.retained else 'Damage frame=11')+' attacker=7118915781085668844',
            ('Damage frame=76' if options.retained else 'Damage frame=74')+' attacker=18446744073709551615',
            ('Damage frame=192' if options.retained else 'Damage frame=180')+' attacker=18446744073709551615', 'removed=14.0039 dead=1',
            'Saved live checkpoint frame=210 HP=157.457 RNG=2808983/38',
            'Content unloaded and reloaded at frame=250',
            'Restored live checkpoint frame=300 HP=157.457 RNG=2808983/38',
            'Rendered frames=360; clean shutdown', '938.285,255 grounded=1']),
    runtime('resume-verification.log', ['Restored live checkpoint frame=0 HP=157.457 RNG=2808983/38',
            'Rendered frames=30; clean shutdown', 'HP=0/47.5 target=0 action=5']),
    runtime('startup-verification.log', ['Rendered frames=90; clean shutdown',
            startup_position, 'HP=165.098/165.098 target=0 action=0'] +
            (startup_motion if options.retained_locomotion else [])),
]
if options.moving:
    moving_observations = [
        'Rendered frames=360; clean shutdown',
        'Content unloaded and reloaded at frame=250',
        'Restored live checkpoint frame=300 HP=157.457 RNG=2808983/38']
    if options.retained:
        moving_observations += ['Damage frame=78 attacker=18446744073709551615',
            'Damage frame=204 attacker=18446744073709551615',
            ('sourceXY=519.183 worldXY=467.262' if options.synchronous_hits else ('sourceXY=519.174 worldXY=467.256' if options.retained_locomotion else 'sourceXY=519.153 worldXY=467.236')),
            ('sourceXY=425.828 worldXY=383.246' if options.synchronous_hits else ('sourceXY=425.828 worldXY=383.244' if options.retained_locomotion else 'sourceXY=443.059 worldXY=398.753')),
            'Retained source animation clock actor=18446744073709551615']
    else:
        moving_observations += ['Actor final position=-6852.87,937.209,255 grounded=1',
            'sourceXY=621.495 worldXY=559.345',
            'Motion final actor=7118915781085668844 enabledSegments=70 disabledSegments=0 sourceXY=0 worldXY=0']
    checks.append(runtime('moving-verification.log', moving_observations))
if options.lifecycle:
    checks.append(runtime('spawn-inflight-verification.log', [
        'Source command frame=33 script=LizardMan_Intro index=4 kind=30',
        'Rendered frames=95; clean shutdown',
        'name=_prim_Monster_LizManIntro1 state=1 enabled=1 physical=0 collisions=1',
        'name=_prim_Monster_LizManIntro2 state=17 enabled=0 physical=0 collisions=0']))
    inflight=(package/'spawn-inflight-verification.log').read_text(encoding='utf-8-sig')
    assert 'Damage frame=' not in inflight and 'Lifecycle whole sequence finished' not in inflight
    checks.append(runtime('spawn-verification.log', [
        'Source command frame=33 script=LizardMan_Intro index=4 kind=30',
        'Source command frame=128 script=LizardMan_Intro index=6 kind=30',
        'Lifecycle whole sequence finished actor=11259224883678223927 state=3',
        'Lifecycle whole sequence finished actor=4660011647479645029 state=3',
        'Damage frame=131 attacker=11259224883678223927',
        spawn_final_hit, 'removed=14.0039 dead=1',
        'Population final enabled=15 deferredInitially=7 loaded=20',
        'Rendered frames=400; clean shutdown']))
baseline = workspace / '.local-inputs/windows-foundation-preview/dh-foundation.exe'
if options.animation_routing:
    checks.append(runtime('controller-active-verification.log', [
        'Rendered frames=90; clean shutdown',
        'Source animation frame=43 actor=7118915781085668844 event=40 service=3 name=attack_mainhand lag=2 controllerBlocked=1',
        'Source animation frame=43 actor=7118915781085668844 event=40 service=0 name=attack_mainhand lag=2 controllerBlocked=1',
        'Damage frame=43 attacker=7118915781085668844',
        'Source command frame=20 script=LizardMan_Intro index=1 kind=24',
        'Source command frame=60 script=LizardMan_Intro index=8 kind=25']))
    notifications=[
        'Source animation frame=13 actor=7118915781085668844 event=40 service=3 name=attack_mainhand lag=11',
        'Source animation frame=71 actor=7118915781085668844 event=34 service=0',
        'Source animation frame=71 actor=7118915781085668844 event=34 service=7']
    runtime('verification.log',notifications)
    checks[0]['required_observations'].extend(notifications)
if options.controller_admission:
    checks.append(runtime('controller-independent-verification.log', [
        'Rendered frames=90; clean shutdown', 'Actor final position=-3500,250,255 grounded=1',
        'Source command frame=0 script=LizardMan_Intro index=1 kind=24',
        'Source command frame=60 script=LizardMan_Intro index=8 kind=25']))
    independent=(package/'controller-independent-verification.log').read_text(encoding='utf-8-sig')
    assert 'Damage frame=' not in independent and 'Lifecycle diagnostic uses' not in independent
    checks.append(runtime('controller-combat-verification.log', [
        'Rendered frames=90; clean shutdown',
        'Damage frame=73 attacker=7118915781085668844',
        'Actor final position=-6752.64,938.285,255 grounded=1',
        'HP=157.457/165.098 target=0']))
    content=(package/'controller-combat-verification.log').read_text(encoding='utf-8-sig')
    damage=re.findall(r'Damage frame=(\d+) attacker=(\d+)',content)
    assert damage and all(int(frame)>=60 and int(actor)!=18446744073709551615 for frame,actor in damage)
    assert 'Lifecycle diagnostic uses' not in content
if options.movement_camera:
    checks.append(runtime('run-verification.log', [
        'Rendered frames=90; clean shutdown',
        ('Actor final position=1573.77,-203.944,255 grounded=1' if options.heading_rotation else ('Actor final position=1211.08,-199.58,255 grounded=1' if options.floor_motion else 'Actor final position=1576.17,-199.58,255 grounded=1')),
        ('sourceXY=888.992 worldXY=518.686' if options.heading_rotation else ('sourceXY=888.992 worldXY=144.493' if options.floor_motion else 'sourceXY=888.992 worldXY=509.59'))]))
    checks.append(runtime('spawn-camera-transition-verification.log', [
        'Rendered frames=32; clean shutdown',
        'target=10098129968780931155 remaining=488',
        'anchor=-3634.73,283.167,255.367 damping=0']))
    checks.append(runtime('spawn-camera-verification.log', [
        'Rendered frames=400; clean shutdown',
        'target=18446744073709551615 remaining=-8',
        'anchor=-3500,250,255 damping=1',
        spawn_final_hit]))
    checks.append(runtime('controller-lock-verification.log', [
        'Rendered frames=90; clean shutdown',
        'Actor final position=-3500,250,255 grounded=1',
        'Source command frame=0 script=LizardMan_Intro index=1 kind=24',
        'Source command frame=60 script=LizardMan_Intro index=8 kind=25']))
    assert 'Damage frame=' not in (package/'controller-lock-verification.log').read_text(encoding='utf-8-sig')
assert hashlib.sha256(baseline.read_bytes()).hexdigest() == '13ffc4ceb4c80a6b84882fc2d92fa800f509cb8d074767637993f517fbdb3bde'
if options.source_scopes:
    checks.append(runtime('source-floor-verification.log', [
        'Source floor diagnostic rooms=9 floors=16; explicit geometry admission',
        'Source floor final actor=18446744073709551615 hit=1 height=255',
        'Rendered frames=90; clean shutdown']))
    checks.append(runtime('spawn-scoped-verification.log', [
        'Serialized command replay context=1;',
        'Lifecycle whole sequence finished actor=11259224883678223927 state=3',
        'Lifecycle whole sequence finished actor=4660011647479645029 state=3',
        'Damage frame=131 attacker=11259224883678223927',
        spawn_final_hit,
        'Rendered frames=400; clean shutdown']))
    for check in checks:
        check['required_observations'].append('Source root constructor scopes modules=9; full Level initialization remains incomplete')
        runtime(Path(check['log']).name, check['required_observations'])
if options.source_body_radius:
    for check in checks:
        observation='Source movement body radius=113.7 from original joint bounds/body definition'
        runtime(Path(check['log']).name,[observation])
        check['required_observations'].append(observation)
if options.native_bodies:
    observation='Native body final actor=18446744073709551615 physical=1 radius=113.7 pfUser='+('18446744073709551615' if options.floor_motion else '0')
    for check in checks:
        content=(workspace/check['log']).read_text(encoding='utf-8-sig')
        assert observation in content
        bodies=[line for line in content.splitlines() if line.startswith('Native body final actor=')]
        assert len(bodies)>=3
        for line in bodies:
            actor=re.search(r'actor=(\d+)',line).group(1)
            assert re.search(r'pfUser=(\d+)',line).group(1)==(actor if options.floor_motion else '0')
            if ' physical=1 ' in line:assert 'sourcePositionMatch=1' in line
        check['required_observations'].append(observation)
    runtime('spawn-inflight-verification.log',['Native body final actor=11259224883678223927 physical=0',
        'Native body final actor=4660011647479645029 physical=0'])
    runtime('spawn-verification.log',['Native body final actor=11259224883678223927 physical=1',
        'Native body final actor=4660011647479645029 physical=1'])
if options.floor_motion:
    checks.append(runtime('floor-boundary-verification.log', [
        'Rendered frames=700; clean shutdown',
        'Source floor final actor=18446744073709551615 hit=1 height=255',
        ('Source PF motion final actor=18446744073709551615 solves=712 accepted=118 clamped=542' if options.heading_rotation else 'Source PF motion final actor=18446744073709551615 solves=712 accepted=19 clamped=641')]))
    for check in checks:
        observations=['Original floor motion bound actors=', 'Navigation final actor=18446744073709551615 pfUser=18446744073709551615 radius=113.7']
        runtime(Path(check['log']).name,observations)
        check['required_observations'].extend(observations)
if options.character_menu:
    checks.append(runtime('menu-verification.log', ['Character menu opened frame=5 via profile input', 'Character menu final opened=1 drawn=55 open=1', 'Rendered frames=60; clean shutdown', 'Actor final position=1090.75,-212.202,255 grounded=1']))
    checks.append(runtime('menu-close-verification.log', ['Character menu opened frame=5 via profile input', 'Character menu final opened=1 drawn=30 open=0', 'Rendered frames=60; clean shutdown', 'Actor final position=1090.75,-212.202,255 grounded=1']))
if options.own_target_position:
    for check in checks:
        content=(workspace/check['log']).read_text(encoding='utf-8-sig')
        actors=set(re.findall(r'^Actor final id=(\d+)',content,re.M))
        nodes=re.findall(r'^Source own target final actor=(\d+) nodePresent=([01]) cache=([^\n]+)',content,re.M)
        assert actors and {item[0] for item in nodes}==actors, (check['log'],'own node records differ from same actor registry')
        counts=re.search(r'Source own target cache final nodeQueries=(\d+) writes=(\d+)',content)
        assert counts and counts.group(1)==counts.group(2)
        bindings=re.findall(r'Source own target nodes bound=(\d+) absent=(\d+)',content)
        assert bindings and all(int(present)+int(absent)==len(actors) for present,absent in bindings)
        for actor,present,cache in nodes:
            if present=='0':assert cache.strip()=='0,0,0', (check['log'],actor,'null own node changed constructed cache')
        if any(present=='1' for _,present,_ in nodes):assert int(counts.group(1))>0
        check['required_observations'].extend(['Source own target nodes bound=', 'Source own target cache final nodeQueries='])
if options.synchronous_hits:
    for check in checks:
        content=(workspace/check['log']).read_text(encoding='utf-8-sig')
        hits=re.findall(r'Damage frame=(\d+) attacker=(\d+).*?marker=(\S+)', content)
        for frame,actor,name in hits:
            expected=rf'Source animation frame={frame} actor={actor} event=40 service=3 name={re.escape(name)}[^\n]* synchronousHit=1'
            assert re.search(expected,content), (check['log'], 'hit was not delivered inside source helper', frame,actor,name)
        if hits:
            check['required_observations'].append('synchronousHit=1')
if options.heading_rotation:
    detailed = {
        'startup-verification.log': ['Source manual heading final checks=60 slides=44 lateRotations=60', 'Source PF motion final actor=18446744073709551615 solves=91 accepted=60 clamped=0'],
        'source-floor-verification.log': ['Source manual heading final checks=60 slides=44 lateRotations=60', 'Source PF motion final actor=18446744073709551615 solves=91 accepted=60 clamped=0'],
        'run-verification.log': ['Source manual heading final checks=60 slides=45 lateRotations=60'],
        'floor-boundary-verification.log': ['Source manual heading final checks=650 slides=100 lateRotations=650'],
    }
    for check in checks:
        name = Path(check['log']).name
        observations = ['Source manual heading final checks='] + detailed.get(name, [])
        runtime(name, observations)
        check['required_observations'].extend(observations)
output = {
    'status': 'diagnostic_packaged_runtime_verified_fidelity_incomplete',
    'goal_complete': False,
    'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
    'package_files_verified': len(receipt['files']),
    'retained_source_clock': options.retained,
    'retained_player_locomotion': options.retained_locomotion,
    'source_spawn_command_replay': options.lifecycle,
    'source_run_camera_initial_input_lock': options.movement_camera,
    'shared_controller_attack_admission': options.controller_admission,
    'original_named_animation_routing': options.animation_routing,
    'source_player_body_radius': options.source_body_radius,
    'native_actor_bodies_before_initfinal': options.native_bodies,
    'live_root_constructor_scopes': options.source_scopes,
    'original_pf_floor_motion': options.floor_motion,
    'original_manual_heading_late_rotation': options.heading_rotation,
    'synchronous_source_hit_callbacks': options.synchronous_hits,
    'authored_own_target_node_cache': options.own_target_position,
    'portrait_character_menu_development_panel': options.character_menu,
    'baseline_executable_preserved': True,
    'runtime_checks': checks,
    'limits': ['Explicit diagnostic stationary AI; original encounter activation remains unverified',
              'Lifecycle replay uses explicitly scheduled serialized Spawn commands in global context-1; full original trigger/module/cutscene/UI flow and campaign persistence remain incomplete',
              ('Player locomotion shares source pose ownership; original controller/attack heading command gates and outer combo chaining remain incomplete' if options.retained_locomotion else 'Initial full phase groups and MoveGO motion service are connected; outer combo chaining and player locomotion handoff remain incomplete'),
              ('Manual player Move heading and late rotation are live; full focus/controller/path/physics coordination, dynamic collisions and continuous reference fidelity remain incomplete' if options.heading_rotation else 'Original floor position/radius/obstacle leaves are live when enabled; heading/path/physics coordination, dynamic collisions and continuous reference fidelity remain incomplete'),
              ('Retained hit effects are synchronous during source advancement; complete original damage/AIS/FSM reentry, seed-time event handling and global actor chronology remain incomplete' if options.synchronous_hits else 'Damage application remains queued after source event delivery'),
              'Companion behavior, status/DOT/leech, full UI/effects/audio/campaign remain incomplete'],
}
if options.character_menu:
    output['limits'].append('Original-art character panel is a development subset: equipment/skills mutations, full AS visibility/reflow, complete masks/art/typography and reference parity remain incomplete')
options.report.resolve().write_text(json.dumps(output, indent=2) + '\n', encoding='utf-8')
receipt['status'] = output['status']
receipt['goal_complete'] = False
receipt['runtime_receipts_verified'] = len(checks)
receipt['runtime_report'] = str(options.report.resolve())
(package / 'package-receipt.json').write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'package_files_verified': len(receipt['files']), 'runtime_receipts': len(checks), 'goal_complete': False}))
