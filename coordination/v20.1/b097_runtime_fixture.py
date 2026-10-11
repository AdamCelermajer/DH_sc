from pathlib import Path
import shutil

repo = Path(__file__).resolve().parents[2]
scratch = repo / '.local-inputs/claude-v20.1/fix097b'
source = (repo / '.local-inputs/claude-v20.1/fix096/pkg/production-success.args').read_text()
lines = [line.strip() for line in source.splitlines() if line.strip() and not line.startswith('#')]
base = []
i = 0
remove = {'--save','--game-save','--skills-page-frame','--frames','--capture','--capture-frames'}
while i < len(lines):
    if lines[i] in remove:
        i += 2
        continue
    base.append(lines[i])
    i += 1
for name in ['baseline', 'train', 'reload', 'rank1-cast', 'rank2-cast']:
    run = scratch / name
    run.mkdir(exist_ok=True)
    if name != 'reload' and not (run / 'char.save').exists():
        shutil.copyfile(scratch / 'seed-rank1-longblade.save', run / 'char.save')
    args = base + ['--save', (run / 'char.save').as_posix(), '--game-save', (run/'gp.save').as_posix(),
                   '--frames','1400','--capture',(run/'cap.ppm').as_posix(),
                   '--capture-frames','300,540,720,900,1100,1300']
    if name in ['baseline', 'train', 'reload', 'rank1-cast']:
        args += ['--skills-page-frame','120']
    if name == 'train':
        args += ['--menu-close-frame','900']
    if name in ['rank1-cast', 'reload']:
        args += ['--menu-close-frame','700','--skill-key-frame','1100:2']
    (scratch / (name + '.args')).write_text('\n'.join(args)+'\n')
