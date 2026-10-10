"""Execute bounded original-menu and combat-text checks on an exact package."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
from PIL import Image, ImageChops

parser = argparse.ArgumentParser()
parser.add_argument('--package', required=True, type=Path)
parser.add_argument('--report', required=True, type=Path)
options = parser.parse_args()
package = options.package.resolve()
executable = package / 'dh-foundation.exe'
checks = []
cases = [('menu-wide', (1600, 900)), ('menu-four-three', (960, 720)),
         ('menu-four-three-fresh', (960, 720)),
         ('combat-text', None), ('combat-text-restore', None), ('combat-text-resize', (1600, 900))]
for name, dimensions in cases:
    case = name + '-verification'
    with (package / (case + '.log')).open('w', encoding='utf-8') as output:
        result = subprocess.run([str(executable), '--startup-config', case + '.args'],
                                cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=60)
    content = (package / (case + '.log')).read_text(encoding='utf-8')
    assert result.returncode == 0 and 'clean shutdown' in content, content
    assert 'Combat presentation diagnostic:' not in content, content
    capture = package / (case + '.ppm')
    with capture.open('rb') as image:
        assert image.readline().strip() == b'P6', case
        size = tuple(map(int, image.readline().split()))
    if dimensions:
        assert size == dimensions, (case, size, dimensions)
        if name != 'menu-four-three-fresh':
            assert 'Window resized frame=' in content, case
    if name.startswith('menu-'):
        assert 'Character menu final opened=1 drawn=55 open=1' in content, content
    else:
        assert re.search(r'Combat text frame=26 style=anim_sct_crit text=32 ', content), content
        assert 'Combat text final results=' in content, content
        if name != 'combat-text-restore':
            assert re.search(r'Combat text frame=59 style=anim_sct_block text=Miss ', content), content
            assert 'Combat text final results=2 labels=2 drawnFrames=39 active=2' in content, content
        else:
            for expected in ['Saved live checkpoint frame=50', 'Content unloaded and reloaded at frame=100',
                             'Restored live checkpoint frame=130']:
                assert expected in content, (case, expected)
            labels = [int(frame) for frame in re.findall(r'Combat text frame=(\d+)', content)]
            assert labels and not any(frame >= 130 for frame in labels), labels
            assert re.search(r'Combat text final .* active=0;', content), content
    checks.append({'case': case, 'exit_code': 0, 'capture_dimensions': size,
                   'capture_sha256': hashlib.sha256(capture.read_bytes()).hexdigest()})

resized = Image.open(package / 'menu-four-three-verification.ppm').convert('RGB')
fresh = Image.open(package / 'menu-four-three-fresh-verification.ppm').convert('RGB')
# Same source Strength field at the same final viewport. Reusing a texture from
# the previous width clips these glyphs even when height stays unchanged.
region = (120, 310, 410, 365)
assert ImageChops.difference(resized.crop(region), fresh.crop(region)).getbbox() is None, 'Resize reused stale menu glyph pixels'

receipt = {'status': 'packaged_menu_and_combat_text_subset_verified', 'goal_complete': False,
           'executable_sha256': hashlib.sha256(executable.read_bytes()).hexdigest(), 'checks': checks,
           'resize_glyph_pixels_match_fresh_viewport': {'region': region, 'dimensions': [960, 720]},
           'limits': ['Full equipment, skills, faery and other menu pages remain incomplete.',
                      'Combat result positions snapshot before motion, earlier than the full original F_ApplyResult text call; projection uses the later current camera.',
                      'Source visibility/state recovery and matched original reference inspection remain separate from these runtime checks.']}
options.report.resolve().write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'ui_text_cases': len(checks), 'goal_complete': False}))
