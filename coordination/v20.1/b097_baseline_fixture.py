from pathlib import Path
import shutil
import json

repo = Path(__file__).resolve().parents[2]
scratch = repo / '.local-inputs/claude-v20.1/fix097b'
run = scratch / 'baseline'
shutil.copyfile(scratch / 'train/char.save', run / 'char.save')
text = (scratch / 'baseline.args').read_text().replace('--frames\n1400', '--frames\n600')
text += '\n--campaign-click\n300:8:80\n'
(scratch / 'baseline.args').write_text(text)
real = Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix097b')
jobs = [{'name':'B097-accepted-baseline','exe':str(repo / '.local-inputs/windows-source-clock-v20/dh-foundation.exe'),
         'args':['--startup-config',str(real / 'baseline.args')], 'cwd':str(real / 'baseline'),
         'log':str(real/'baseline/run.log'),'timeoutSec':75}]
(scratch/'baseline-jobs.json').write_text(json.dumps(jobs,indent=2))
