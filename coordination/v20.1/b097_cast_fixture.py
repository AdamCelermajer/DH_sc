from pathlib import Path
import shutil

repo = Path(__file__).resolve().parents[2]
scratch = repo / '.local-inputs/claude-v20.1/fix097b'
source = (scratch / 'rank1-cast.args').read_text()
lines = [line.strip() for line in source.splitlines() if line.strip() and not line.startswith('#')]
base = []
i = 0
remove = {'--save','--game-save','--skills-page-frame','--frames','--capture','--capture-frames',
          '--menu-close-frame','--skill-key-frame','--position'}
while i < len(lines):
    if lines[i] in remove:
        i += 2
        continue
    base.append(lines[i])
    i += 1
for name, seed in [('normal-rank1', 'seed-rank1-longblade.save'),
                   ('normal-rank2', 'train/char.save'),
                   ('normal-target-rank1', 'seed-rank1-longblade.save'),
                   ('normal-target-rank2', 'train/char.save')]:
    run = scratch / name
    run.mkdir(exist_ok=True)
    if not (run / 'char.save').exists():
        shutil.copyfile(scratch / seed, run / 'char.save')
    # B083 reviewer's proven authored Bogwomp target fixture, reused with
    # our private profiles and save owners. This is test placement only.
    position = '-6752.641,938.285,250' if 'target' in name else '1090.75,-212.202,258'
    args = base + ['--save', (run/'char.save').as_posix(), '--game-save',(run/'gp.save').as_posix(),
                   '--frames','650','--capture',(run/'cap.ppm').as_posix(),
                   '--capture-frames','180,250,300,360,420,540','--position',position]
    (scratch/(name+'.args')).write_text('\n'.join(args)+'\n')
