from pathlib import Path
import argparse, collections, datetime, json
ROOT = Path(__file__).resolve().parents[1]
PATH = ROOT / 'reports/LOADER-SUBGOALS.json'
STATUSES = {'done','in_progress','pending','dependency_blocked'}
LABELS = {'done':'Done','in_progress':'In progress','pending':'Pending','dependency_blocked':'Dependency blocked'}
def render(data):
    counts = collections.Counter(r['status'] for r in data['subtasks'])
    lines = ['# Generic level loader: subgoals and subtasks', '', f"Updated: {data['updated_at']} | Revision: {data['revision']}", '',
      'First acceptance gate: complete authored Chapter 1 SWAMP map, mobs and chests visibly rendered through the genuine generic runtime loader. The gate is OPEN.', '',
      'Done means only the precise scope in that row has evidence. Data transport, inspection rendering, canonical activation and gameplay acceptance are distinct. Historical receipts are checkpoints; they are not fresh runtime proof.', '',
      'Main owns AI/pathing/targeting/combat/skills/animation/object behavior/loot/inventory/quests/conditions/save and cinematic/dialogue execution. Loader owns original data, preparation, orchestration, resources, event-reference transport and restoration integration. Menu owns loading presentation.', '',
      data.get('device_status','Private emulator is not running. Direct launcher remains disabled; device availability must be verified before a guarded launch.'), '',
      'Subtask counts: ' + ', '.join(f"{LABELS[s]} {counts[s]}" for s in ('done','in_progress','pending','dependency_blocked')) + '.', '',
      '## Subgoals', '', '| ID | Subgoal | Owner | Status |', '|---|---|---|---|']
    for g in data['subgoals']:
        children = [r for r in data['subtasks'] if r['parent']==g['id']]
        statuses = {r['status'] for r in children}
        status = 'done' if statuses=={'done'} else 'dependency_blocked' if statuses=={'dependency_blocked'} else 'in_progress' if ('in_progress' in statuses or 'done' in statuses) else 'pending'
        lines.append(f"| {g['id']} | {g['task']} | {g['owner']} | {LABELS[status]} |")
    for g in data['subgoals']:
        lines += ['', f"## {g['id']}: {g['task']}", '', '| ID | Subtask | Owner | Status | Evidence / completion criterion | Next action or dependency |', '|---|---|---|---|---|---|']
        for r in data['subtasks']:
            if r['parent'] != g['id']: continue
            evidence = '<br>'.join(f'[{Path(e).name}](../{e})' for e in r['evidence'])
            detail = r['acceptance'] + (('<br>'+evidence) if evidence else '')
            clean = lambda value: str(value).replace('|',chr(92)+'|').replace('\n',' ')
            lines.append('| ' + ' | '.join(clean(v) for v in (r['id'],r['task'],r['owner'],LABELS[r['status']],detail,r['next_action'])) + ' |')
    lines += ['', '## Turn updates', '']
    lines += [f"- {h['at']}: {h['note']}" for h in data['history'][-30:]]
    return '\n'.join(lines)+'\n'
def main():
    p=argparse.ArgumentParser(description='Update the loader task table after a turn; completion requires a real evidence path.')
    p.add_argument('--task'); p.add_argument('--status', choices=sorted(STATUSES)); p.add_argument('--evidence', action='append'); p.add_argument('--next'); p.add_argument('--note', required=True)
    args=p.parse_args()
    data=json.loads(PATH.read_text(encoding='utf-8'))
    if args.task:
        matching=[r for r in data['subtasks'] if r['id']==args.task]
        if len(matching)!=1: p.error('Unknown subtask ID')
        row=matching[0]
        if args.status: row['status']=args.status
        if args.evidence is not None: row['evidence']=args.evidence
        if args.next is not None: row['next_action']=args.next
    elif args.status or args.evidence or args.next: p.error('Row changes need --task')
    for row in data['subtasks']:
        if row['status']=='done':
            if not row['evidence']: p.error(f"Done row {row['id']} needs evidence")
            for evidence in row['evidence']:
                if not (ROOT/evidence).is_file(): p.error(f"Missing evidence for {row['id']}: {evidence}")
    data['revision']+=1
    data['updated_at']=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds')
    data['history'].append({'at':data['updated_at'],'note':args.note})
    output=render(data)
    for path,content in [(PATH,json.dumps(data,indent=2)+'\n'),(ROOT/'reports/LOADER-SUBGOALS.md',output)]:
        temporary=path.with_suffix(path.suffix+'.tmp'); temporary.write_text(content,encoding='utf-8'); temporary.replace(path)
    print(json.dumps({'revision':data['revision'],'subtasks':len(data['subtasks']),'counts':dict(collections.Counter(r['status'] for r in data['subtasks']))}))
if __name__=='__main__': main()
