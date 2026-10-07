"""One ledger drives the user-visible V checklist. Root owns all status changes."""
from pathlib import Path
import argparse,json
ROOT=Path(__file__).resolve().parents[3]
SOURCE=ROOT/'docs/feature-acceptance-tracker-2026-10-06.json'
TARGET=ROOT/'docs/FEATURE-ACCEPTANCE-TRACKER-2026-10-06.md'
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--id');parser.add_argument('--state',choices=['pending','in_progress','partial','verified','deferred']);parser.add_argument('--done');parser.add_argument('--remaining');parser.add_argument('--evidence',action='append');args=parser.parse_args()
 data=json.loads(SOURCE.read_text())
 if args.id:
  task=next(t for t in data['tasks'] if t['id']==args.id)
  for key in ('state','done','remaining'):
   if getattr(args,key) is not None:task[key]=getattr(args,key)
  if args.evidence:task['evidence']=args.evidence
  if task['state']=='verified' and not task['evidence']:raise ValueError('V requires recorded evidence')
  SOURCE.write_text(json.dumps(data,indent=2)+'\n')
 assert len({t['id'] for t in data['tasks']})==len(data['tasks'])
 overall='; '.join(name+' '+value.replace('_',' ') for name,value in data['overall'].items())
 lines=['# Feature acceptance tracker — 2026-10-06','',data['rule'],'','Overall: **'+overall+'**. No whole-feature completion is inferred from component results.','',
 '**V = verified for the stated scope; — = still open.** Source/component acceptance and live/device acceptance are separate rows. Historical accepted APK evidence is identified explicitly; newer source work is not silently treated as included.','',
 data.get('workers','Root owns integration and acceptance; agents and sessions own delegated components.')+' Only root publishes checklist status.','']
 names={'pending':'Not done','in_progress':'Working','partial':'Partial','verified':'Verified','deferred':'Deferred by user'}
 def cell(s):return s.replace('|','/').replace('\n',' ')
 for area in ('Targeting','Audio','Performance','Act 1'):
  lines+=['## '+area,'','| ID | Done | Subtask | State / owner | Done so far / evidence | Remaining acceptance |','|---|---|---|---|---|---|']
  for t in data['tasks']:
   if t['area']!=area:continue
   for e in t['evidence']:
    if not (ROOT/e).is_file():raise ValueError('Missing evidence file: '+e)
   evidence='; '.join('[evidence]('+str(ROOT/e).replace('\\','/')+')' for e in t['evidence'])
   lines.append('| '+t['id']+' | '+('V' if t['state']=='verified' else '—')+' | '+cell(t['feature'])+' | '+names[t['state']]+' / '+cell(t['owner'])+' | '+cell(t['done'])+' '+evidence+' | '+cell(t['remaining'])+' |')
  lines+=['']
 lines+=['## Whole-feature acceptance','','- Targeting: '+data['acceptance']['targeting'],'- Audio: '+data['acceptance']['audio'],'- Performance: '+data['acceptance']['performance'],'','Update this table after each verified feature; a failed or missing runtime test stays open.']
 TARGET.write_text('\n'.join(lines)+'\n',encoding='utf8')
 print('Tracked',len(data['tasks']),'subtasks;',sum(t['state']=='verified' for t in data['tasks']),'scoped V marks; whole features incomplete')
if __name__=='__main__':main()
