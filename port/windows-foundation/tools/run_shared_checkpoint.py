"""Run packaged native smoke cases, restoring the no-argument startup file exactly."""
import argparse
import json
from pathlib import Path
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, required=True)
parser.add_argument('--moving', action='store_true')
parser.add_argument('--lifecycle', action='store_true')
parser.add_argument('--movement-camera', action='store_true')
parser.add_argument('--controller-admission', action='store_true')
parser.add_argument('--animation-routing', action='store_true')
parser.add_argument('--source-scopes', action='store_true')
parser.add_argument('--floor-motion', action='store_true')
parser.add_argument('--character-menu', action='store_true')
options = parser.parse_args()
package = options.package.resolve()
executable = package / 'dh-foundation.exe'
if not executable.is_file():
    parser.error('Package executable is missing')

completed = []

def run(name, configuration=None):
    arguments = [str(executable)]
    if configuration:
        arguments += ['--startup-config', configuration]
    with (package / (name + '.log')).open('w', encoding='utf-8') as output, \
         (package / (name + '-errors.log')).open('w', encoding='utf-8') as errors:
        result = subprocess.run(arguments, cwd=package, stdout=output, stderr=errors, timeout=60)
    if result.returncode:
        raise RuntimeError(f'{name} exited {result.returncode}; inspect its logs')
    completed.append(name)

run('verification', 'verification.args')
run('resume-verification', 'resume-verification.args')
if options.moving:
    run('moving-verification', 'moving-verification.args')
if options.lifecycle:
    run('spawn-inflight-verification','spawn-inflight-verification.args')
    run('spawn-verification','spawn-verification.args')
if options.movement_camera:
    for name in ['run-verification', 'spawn-camera-transition-verification',
                 'spawn-camera-verification', 'controller-lock-verification']:
        run(name, name + '.args')
if options.controller_admission:
    for name in ['controller-independent-verification','controller-combat-verification']:
        run(name, name + '.args')
if options.animation_routing:
    run('controller-active-verification','controller-active-verification.args')
if options.source_scopes:
    run('spawn-scoped-verification','spawn-scoped-verification.args')
    run('source-floor-verification','source-floor-verification.args')
if options.floor_motion:
    run('floor-boundary-verification','floor-boundary-verification.args')
if options.character_menu:
    run('menu-verification','menu-verification.args')
    run('menu-close-verification','menu-close-verification.args')

startup = package / 'startup.args'
original = startup.read_bytes()
try:
    startup.write_bytes(original + b'\n--fixed-step\n.016\n--frames\n90\n--move-axis\n1,0,0\n--move-frames\n60\n--move-walk\n--capture\nstartup-verification.ppm\n')
    run('startup-verification')
finally:
    startup.write_bytes(original)

print(json.dumps({'package': str(package), 'completed_runs': completed,
                  'scope': 'Native execution only; verify receipts and fidelity separately'}))
