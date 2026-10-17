"""Original Player AIS startup/skills/timers composition on API37/16KiB.

The shell fixtures invoke the unchanged skill framework, not a full activation
or combat lifecycle. Optional --saved-skills checks the integrated saved owner,
eight Knight updates and full two-result checks. --full-update additionally
requires all13 Knight/faery callbacks, one source InitProcess, retained owners,
real MP spending and exact reached regeneration. Profile/grants/activation and
the complete Player AI frame remain unfinished. No compensating heal is used.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import zipfile
from emulator_smoke import inspect, launch_fresh

PACKAGE = 'com.example.dh2'
BAD = re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|World load failed|Model draw GL error|Native Player AIS failure retained|Native Player AI timer failed')

AIS_INITIAL = re.compile(r'Native Player AIS initialized \| AI (\S+) \| AIS (\S+) \| active (\S+) \| pending (\S+) \| phase (\d+) \| VM (\S+) \| init mask ([0-9a-f]+) \| HP (-?\d+) / (-?\d+) \| MP (-?\d+) / (-?\d+) \| timer33 (\d+) \| timer34 (\d+)')
AIS_RETAINED = re.compile(r'Native Player AIS retained \| initialized (\d+) \| AI (\S+) \| AIS (\S+) \| active (\S+) \| pending (\S+) \| phase (\d+) \| timer33 (\d+) \| timer34 (\d+)')
TIMER_SNAPSHOT = re.compile(r'Native Player AI timer snapshot \| phase (initial|restore) \| event (33|34) \| slot (\d+) \| duration (\d+) \| repeat (-?\d+) \| elapsed (\d+) \| active (\d+) \| paused (\d+) \| ref (\d+) \| delivered (\d+) \| HP (-?\d+) / (-?\d+) \| MP (-?\d+) / (-?\d+)')
TIMER_DELIVERED = re.compile(r'Native Player AI timer delivered \| event (33|34) \| slot (\d+) \| count (\d+) \| combat (\d+) \| remote (\d+) \| HP rate (\d+) \| MP rate (\d+) \| HP (-?\d+) / (-?\d+) \| MP (-?\d+) / (-?\d+) \| DoT attacks (\d+) \| HP before (-?\d+) \| MP before (-?\d+)')

def source_startup(text):
    rows = AIS_INITIAL.findall(text)
    assert len(rows) == 1, 'Player source initialization was missing or replayed'
    ai, ais, active, pending, phase, vm, mask, hp, max_hp, mp, max_mp, timer33, timer34 = rows[0]
    assert int(ai,16) and int(ais,16) and int(vm,16), rows[0]
    assert active == pending == ais and int(phase) == 7 and int(mask,16) == 31, rows[0]
    # The authored Knight source initial-vitals pass truncates once. The old
    # preparation path performed another fill; that extra write is not allowed.
    assert tuple(map(int,(hp,max_hp,mp,max_mp))) == (42264,42265,6975,6976), rows[0]
    assert (int(timer33),int(timer34))==(0,1), 'fresh source repeat timer order differs'
    return {'AI':ai,'AIS':ais,'VM':vm,'phase':7,'init_phase_mask':31,
            'HP':int(hp),'max_HP':int(max_hp),'MP':int(mp),'max_MP':int(max_mp),
            'timer33':int(timer33),'timer34':int(timer34),'initialization_count':1}

def source_timer_receipts(text, startup, retained=False):
    snapshots = []
    for match in TIMER_SNAPSHOT.finditer(text):
        row = match.groups()
        item = dict(zip(('phase','event','slot','duration_ms','repeat','elapsed_ms','active','paused','ref','delivered','HP','max_HP','MP','max_MP'),row))
        item.update({key:int(value,16) if key=='event' else int(value) for key,value in item.items() if key!='phase'})
        item['event_hex'] = hex(item['event'])
        item['log_offset'] = match.start()
        snapshots.append(item)
    assert len(snapshots)>=2 and [r['event'] for r in snapshots[:2]]==[0x33,0x34], 'source timer snapshots missing'
    assert len(snapshots)%2==0, 'incomplete timer snapshot pair'
    prior = {}
    for offset in range(0,len(snapshots),2):
        pair=snapshots[offset:offset+2]
        assert [r['event'] for r in pair]==[0x33,0x34] and pair[0]['phase']==pair[1]['phase']
        assert pair[0]['HP']==pair[1]['HP'] and pair[0]['MP']==pair[1]['MP'], 'snapshot pair saw different properties'
        for row in pair:
            event=row['event']
            assert row['slot']==startup['timer'+format(event,'x')] and row['duration_ms']==(3000 if event==0x33 else 1000)
            assert (row['repeat'],row['active'],row['paused'],row['ref'])==(-1,1,0,0), row
            assert 0<=row['elapsed_ms']<row['duration_ms'], row
            assert (row['max_HP'],row['max_MP'])==(startup['max_HP'],startup['max_MP'])
            row['accumulated_source_timer_ms']=row['delivered']*row['duration_ms']+row['elapsed_ms']
            if event in prior:
                old=prior[event]
                assert row['phase']=='restore' and row['duration_ms']==old['duration_ms'], 'timer schedule replaced'
                assert row['delivered']>=old['delivered'] and row['accumulated_source_timer_ms']>=old['accumulated_source_timer_ms'], 'source timer reset on restore'
            else:
                assert row['phase']=='initial' and row['delivered']==row['elapsed_ms']==0, row
                assert (row['HP'],row['MP'])==(startup['HP'],startup['MP']), 'initial timer snapshot differs from source InitProcess vitals'
            prior[event]=row
        # Both repeat timers consume the same actual Coordinator dt stream.
        # This is a source timer invariant, not an assertion about wall time.
        assert pair[0]['accumulated_source_timer_ms']==pair[1]['accumulated_source_timer_ms'], pair
    deliveries=[]
    previous33=0
    for match in TIMER_DELIVERED.finditer(text):
        keys=('event','slot','count','combat','remote','HP_rate','MP_rate','HP','max_HP','MP','max_MP','dot_attacks','HP_before','MP_before')
        row={key:int(value,16) if key=='event' else int(value) for key,value in zip(keys,match.groups())}
        row['event_hex'] = hex(row['event'])
        row['log_offset']=match.start()
        assert row['slot']==startup['timer'+format(row['event'],'x')] and row['dot_attacks']==0, row
        assert (row['max_HP'],row['max_MP'])==(startup['max_HP'],startup['max_MP']), row
        if row['event']==0x33:
            assert row['count']==previous33+1, 'reached source regeneration count skipped/reset'
            previous33=row['count']
            assert (row['combat'],row['remote'],row['HP_rate'],row['MP_rate'])==(0,0,1760,741), row
            assert row['HP']==min(row['HP_before']+row['HP_rate'],row['max_HP'])
            assert row['MP']==min(row['MP_before']+row['MP_rate'],row['max_MP']), 'source RegenMP amount/cap differs'
        else:
            assert row['HP_rate']==row['MP_rate']==0 and row['HP']==row['HP_before'] and row['MP']==row['MP_before'], 'zero DoT event changed vitals'
        deliveries.append(row)
    for row in snapshots:
        if row['event']==0x33:
            reached=[r['count'] for r in deliveries if r['event']==0x33 and r['log_offset']<row['log_offset']]
            assert row['delivered']==(reached[-1] if reached else 0), 'snapshot source count differs from reached33 receipts'
    if retained:
        assert sum(row['phase']=='restore' for row in snapshots)>=6, 'all recreation timer snapshots missing'
        assert previous33>0 and prior[0x33]['accumulated_source_timer_ms']>0
    return snapshots,deliveries

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb', required=True)
    p.add_argument('--serial', required=True)
    p.add_argument('--apk', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--saved-skills', action='store_true')
    p.add_argument('--full-update', action='store_true')
    p.add_argument('--mana-faeries', action='store_true', help='verify saved faeries and MP callbacks at an explicit unfinished update provider')
    p.add_argument('--scalar-dictionary', action='store_true', help='verify same-AIS GetInt/SetInt and nonzero dictionary retention across reload/rotation')
    p.add_argument('--buffs', action='store_true', help='verify authored Celest resistance buff, same owned sheet/groups across reload/rotation')
    p.add_argument('--update-boundary', default='GetInt')
    p.add_argument('--completed-updates', type=int, default=8)
    a = p.parse_args()
    assert not (a.full_update or a.mana_faeries) or a.saved_skills, 'new owner checks require --saved-skills'
    assert not (a.full_update and a.mana_faeries), 'select complete updates or a bounded update checkpoint'
    assert not a.buffs or a.full_update, 'buff retention requires complete authored updates'
    assert not a.scalar_dictionary or a.full_update or a.mana_faeries, 'scalar retention checks require the current player/save integration'
    assert 8 <= a.completed_updates < 13
    assert a.serial.startswith('emulator-'), 'emulator only'
    out = a.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    report = {'validation': 'FAIL', 'scope': 'Live source Player AIS split load/InitProcess, same skill/property/save/buff owners, original timer regeneration and framework cooldown shell fixture. Profile/starter grants, Player AI frame, skill activation and complete combat loop remain unfinished.',
              'apk_sha256': hashlib.sha256(a.apk.read_bytes()).hexdigest(),
              'libraries': inspect(a.apk), 'serial': a.serial}
    transcript = []
    pid = since = last_logs = ''
    prior_rotation = None
    frozen_mp = None
    startup = None
    def adb(*args):
        r = subprocess.run([a.adb, '-s', a.serial, *args], capture_output=True, text=True, timeout=45)
        transcript.append({'args': list(args), 'exit_code': r.returncode,
                           'stdout': r.stdout if args[0] != 'logcat' else '[stored separately]', 'stderr': r.stderr})
        assert r.returncode == 0, (args, r.stdout, r.stderr)
        return r.stdout.strip()
    def logs():
        nonlocal last_logs
        assert adb('shell', 'pidof', PACKAGE) == pid, 'process changed or exited'
        last_logs = adb('logcat', '-d', '-T', since, '--pid=' + pid, '-v', 'brief')
        assert not BAD.search(last_logs), 'native failure in logs'
        return last_logs
    def wait(predicate, label, seconds=35):
        deadline = time.monotonic() + seconds
        while time.monotonic() < deadline:
            text = logs()
            if predicate(text): return text
            time.sleep(.2)
        raise AssertionError(label + ' not observed')
    def command(action, *extras):
        return adb('shell', 'am', 'broadcast', '-a', 'com.example.dh2.DEBUG_' + action, '-p', PACKAGE, *extras)
    def screenshot(name):
        raw = subprocess.check_output([a.adb, '-s', a.serial, 'exec-out', 'screencap', '-p'], timeout=30)
        assert raw.startswith(b'\x89PNG')
        path = out / (name + '.png')
        path.write_bytes(raw)
        return {'file': path.name, 'sha256': hashlib.sha256(raw).hexdigest()}
    def freeze():
        prior=logs().count('Animation time command applied | time 0')
        command('ANIMATION_TIME','--ei','time_ms','0')
        return wait(lambda t:t.count('Animation time command applied | time 0')==prior+1,'freeze source clocks')
    def resume():
        prior=logs().count('Animation time command applied | time -1')
        command('ANIMATION_TIME','--ei','time_ms','-1')
        return wait(lambda t:t.count('Animation time command applied | time -1')==prior+1,'resume source clocks')
    def mana_probe(amount, has=None, used=None, before=None, after=None):
        nonlocal frozen_mp
        prior=len(re.findall(r'Native Player mana probe \|',logs()))
        command('PLAYER_MANA','--ei','raw_amount',str(amount))
        text=wait(lambda t:len(re.findall(r'Native Player mana probe \|',t))==prior+1,'original frozen mana probe')
        rows=re.findall(r'Native Player mana probe \| amount (\d+) \| has (\d+) \| used (\d+) \| MP before (-?\d+) \| MP after (-?\d+) \| exempt14f0 (\d+) \| options (\d+)',text)
        row=tuple(map(int,rows[-1]))
        assert row[0]==amount and row[5:]==(0,0),row
        for actual,expected in zip(row[1:5],(has,used,before,after)):
            assert expected is None or actual==expected,row
        if amount==0:
            assert row[1:3]==(1,1) and row[3]==row[4], 'zero probe mutated source MP'
        frozen_mp=row[4]
        report.setdefault('mana_observations' if amount==0 else 'mana_debits',[]).append(
            {'amount':amount,'has':row[1],'used':row[2],'MP_before':row[3],'MP_after':row[4]})
        return text
    def observe_frozen_mana():
        return mana_probe(0)
    def retained_ais(text):
        if not a.full_update:return
        assert source_startup(text)==startup, 'source InitProcess receipt changed'
        rows=AIS_RETAINED.findall(text)
        assert rows,'retained AIS receipt missing'
        for row in rows:
            assert int(row[0])==1 and row[1]==startup['AI'] and row[2]==row[3]==row[4]==startup['AIS'],row
            assert tuple(map(int,row[5:]))==(7,startup['timer33'],startup['timer34']),row
        report['AIS_retained']={'same_AI':startup['AI'],'same_AIS':startup['AIS'],'active_equals_pending':True,
                               'phase':7,'initialization_count':1,'recreations':len(rows)}
    def skill_check(slot, usable, active, mana=None):
        prior = len(re.findall(r'Native Player skill check probe \|', logs()))
        command('PLAYER_SKILL_CHECK', '--ei', 'skill_slot', str(slot))
        text = wait(lambda t: len(re.findall(r'Native Player skill check probe \|', t)) == prior + 1,
                    'two-result source skill check')
        rows = re.findall(r'Native Player skill check probe \| script (\S+) \| slot (\d+) \| saved level (\d+) \| usable (\d+) \| active (\d+) \| usable returns (\d+) \| active returns (\d+) \| MP (\d+) \| SnS_Level (-?\d+) \| temp ManaCost (-?\d+)', text)
        assert rows, 'source skill check did not report both result vectors'
        row = rows[-1]
        assert tuple(map(int, row[1:7])) == (slot, 0, usable, active, 2, 2), row
        expected=frozen_mp if mana is None else mana
        assert expected is not None and int(row[7]) == expected, 'HasMana/check changed the frozen live Knight MP'
        report.setdefault('skill_checks', []).append({'script': row[0], 'slot': slot,
            'saved_level': 0, 'usable': usable, 'active': active, 'return_counts': [2, 2],
            'MP': int(row[7]), 'SnS_Level': int(row[8]), 'temporary_mana_cost': int(row[9])})
        return text
    def scalar_probe(write, value, observed, before, after):
        prior = len(re.findall(r'Native Player scalar probe \|', logs()))
        command('PLAYER_SCALAR', '--ei', 'raw_value', str(value), '--ez', 'write', str(write).lower())
        text = wait(lambda t: len(re.findall(r'Native Player scalar probe \|', t)) == prior + 1, 'original script dictionary')
        rows = re.findall(r'Native Player scalar probe \| write (\d+) \| input (-?\d+) \| observed (-?\d+) \| entries before (\d+) \| entries after (\d+) \| AIS (\S+) \| VM (\S+)', text)
        row = rows[-1]
        assert tuple(map(int,row[:5])) == (int(write),value,observed,before,after), row
        receipts = report.setdefault('scalar_dictionary',[])
        if startup is not None:
            assert (row[5],row[6])==(startup['AIS'],startup['VM']), 'scalar callbacks used a different AIS/VM from source InitProcess'
        assert not receipts or (row[5],row[6]) == (receipts[0]['AIS'],receipts[0]['VM']), 'LuaScript dictionary owner changed'
        receipts.append({'write':write,'input':value,'observed':observed,'entries_before':before,'entries_after':after,'AIS':row[5],'VM':row[6]})
        return text
    def buff_snapshots(text, retained=False):
        rows=re.findall(r'Native Player buff snapshot \| phase (\S+) \| count (\d+) \| groups (\d+) \| id (-?\d+) \| instance (\S+) \| timer (-?\d+) \| strength (\d+) \| sheet ([0-9a-f]+)',text)
        assert rows and rows[0][0]=='initial', 'authored buff was not published'
        assert all(row[1:4]==rows[0][1:4] and row[4:]==rows[0][4:] for row in rows), 'buff instance/sheet changed during recreation'
        assert tuple(map(int,(rows[0][1],rows[0][2],rows[0][5],rows[0][6])))==(1,1,-1,1),rows[0]
        properties=re.findall(r'Native Player buff property \| phase initial \| name (\S+) \| index (\d+) \| buff (-?\d+) \| resolved (-?\d+)',text)
        assert properties and any(int(row[2]) and int(row[3]) for row in properties), 'authored resistance never reached live properties'
        if retained: assert sum(row[0]=='restore' for row in rows)>=3, 'buff retention across all recreations missing'
        report['authored_buff']={'count':1,'groups':1,'class_id':int(rows[0][3]),'instance':rows[0][4],
            'timer':-1,'strength':1,'sheet_fnv1a64_le_words':rows[0][7],
            'initial_resistance_properties':[{'name':r[0],'index':int(r[1]),'buff':int(r[2]),'resolved':int(r[3])} for r in properties],
            'retained_recreations':sum(row[0]=='restore' for row in rows),'activation_or_unlock_claim':False}
    try:
        report['api'] = int(adb('shell', 'getprop', 'ro.build.version.sdk'))
        report['page_size'] = int(adb('shell', 'getconf', 'PAGE_SIZE'))
        report['abi'] = adb('shell', 'getprop', 'ro.product.cpu.abi')
        assert report['api'] == 37 and report['page_size'] == 16384 and report['abi'] == 'x86_64'
        with zipfile.ZipFile(a.apk) as z:
            manifest = json.loads(z.read('assets/player-skills-provenance.json'))
            for path, receipt in manifest['files'].items():
                raw = z.read('assets/' + path)
                assert len(raw) == receipt['bytes'] and hashlib.sha256(raw).hexdigest() == receipt['sha256'], path
            report['packaged_original_resources_verified'] = len(manifest['files'])
        assert 'Success' in adb('install', '-r', str(a.apk.resolve()))
        prior_rotation = adb('shell', 'cmd', 'window', 'user-rotation').split()
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '0')
        since = adb('shell', 'date', '+%s.%N')
        launch_fresh(adb, '--es', 'world', 'crypt01.dwld', '--ez', 'enemy_ai', 'false')
        pid = adb('shell', 'pidof', PACKAGE)
        # Each nonnull script has DeclareSkill(name,index), then the no-args
        # DeclareSkill() reset after loading: 26 real calls for 13 instances.
        callbacks = 13 if a.full_update else a.completed_updates if a.mana_faeries else 8 if a.saved_skills else 0
        boundary = None if a.full_update else a.update_boundary if a.mana_faeries else 'GetCurrentSpellInfo' if a.saved_skills else 'GetCurrentSkillInfo__'
        update_message = ('Native Player skill update complete | attempt 1 | callbacks 13 | skill slots 16 | faery slots 5' if a.full_update else
                          f'Native Player skill update blocked | attempt 1 | callbacks {callbacks} | VM status -5')
        text = wait(lambda t: 'Native Player skill preparation | skills 16 | faeries 5 | loaded paths 15 | declarations 26' in t and
                    update_message in t and 'Native actor frame |' in t, 'real Player preparation/update boundary')
        if boundary: assert f'unresolved native Player skill provider: {boundary}' in text
        else:
            assert 'Native Player skill update blocked |' not in text
        if a.full_update or a.mana_faeries:
            assert 'Native Player saved faeries ready | difficulty 0 | selected 0 | level 0 | rows 5 5 5' in text
            assert text.count('Native Player saved faeries ready |') == 1
        if a.saved_skills:
            assert 'rows 16 | slot0 level 0 | source _InitSkills; starter grant/profile load pending' in text
            assert text.count('Native Player saved skills ready |') == 1
        assert text.count('Native Player skill preparation |') == 1
        if a.full_update:
            startup=source_startup(text)
            snapshots,deliveries=source_timer_receipts(text,startup)
            report['source_startup']=startup
            report['source_timers']={'snapshots':snapshots,'deliveries':deliveries,
                'cadence_scope':'Original duration/repeat, actual delivered counts and retained elapsed fields on one Coordinator; no exact wall-clock timing claim.'}
        report['initial'] = {'source_skill_slots': 16, 'source_faery_slots': 5, 'nonnull_instances': 13,
                             'source_paths': 15, 'source_declaration_calls': 26, 'completed_knight_updates': min(callbacks,8),
                             'completed_faery_updates': max(0,callbacks-8),
                             'update_boundary': boundary, 'saved_skill_rows': 16 if a.saved_skills else 0,
                             'update_vm_status': 0 if a.full_update else -5, 'full_skill_update_complete': a.full_update}
        report['screenshots'] = [screenshot('player-skills-initial')]
        if a.buffs: buff_snapshots(text)
        freeze()
        observe_frozen_mana()
        report['initial_frozen_MP']=frozen_mp
        if a.scalar_dictionary:
            scalar_probe(False,0,0,1,2)  # Source missing read inserts one entry.
            scalar_probe(True,7312,7312,2,2)
        if a.saved_skills:
            skill_check(0, 1, 0)
            assert report['skill_checks'][-1]['temporary_mana_cost'] == 1024
            skill_check(7, 0, 1)  # Original passive check selects the second result.
        command('PLAYER_SKILL_COOLDOWN', '--ei', 'delay_ms', '12000')
        text = wait(lambda t: 'Player skill cooldown command applied | Original skill cooldown callback armed' in t, 'source cooldown fixture')
        match = re.search(r'Native Player skill cooldown probe \| script prince_warrior_bashdown \| slot0 field18 (\d+) \| timer (\d+) \| duration 12000', text)
        assert match and match[1] == match[2], 'Lua/native timer identities differ'
        timer_id = int(match[1])
        report['cooldown_fixture'] = {'script': 'prince_warrior_bashdown', 'slot': 0, 'field18': timer_id,
                                      'native_timer_id': timer_id, 'duration_ms': 12000, 'full_skill_use': False}
        # Authored Bashdown has no cooldown: its check is HasMana,false. This
        # deliberately armed framework timer must not add a new check gate.
        if a.saved_skills: skill_check(0, 1, 0)
        command('RELOAD_WORLD')
        text = wait(lambda t: 'World reload command applied |' in t and 'Native Player skills retained |' in t, 'retained world reload')
        expected_blocked = 0 if a.full_update else 1
        assert text.count('Native Player skill preparation |') == 1 and text.count('Native Player skill update blocked |') == expected_blocked
        if a.scalar_dictionary: scalar_probe(False,0,7312,2,2)
        first_vm = re.search(r'Native Player skills retained \| VM (\S+) \| paths 15 \| update attempts (\d+) \| timer callbacks 0', text)
        assert first_vm, 'VM/paths/update attempt not retained'
        if a.full_update:
            assert first_vm[1]==startup['VM'],'source VM replaced after startup'
            retained_ais(text)
            source_timer_receipts(text,startup)
        assert int(first_vm[2]) == 1, 'UpdateAllSkills must be delivered by the single source InitProcess, not renderer frames'
        # Inspection remains frozen across reload; ensure cooldown survives recreation.
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '1')
        text = wait(lambda t: len(re.findall(r'Native Player skills retained \|', t)) >= 2, 'retained rotation')
        restores = re.findall(r'Native Player skills retained \| VM (\S+) \| paths 15 \| update attempts (\d+)', text)
        assert restores and {row[0] for row in restores} == {first_vm[1]}, 'Player VM replaced on graphics restore'
        if a.full_update: assert all(int(row[1]) == 1 for row in restores), 'reload replayed an UpdateAllSkills caller'
        assert text.count('Native Player skill preparation |') == 1 and text.count('Native Player skill update blocked |') == expected_blocked
        retained_ais(text)
        report['restore'] = {'same_process': pid, 'same_vm': first_vm[1], 'reload_and_rotation': True, 'preparation_count': 1,
                             'blocked_update_attempts': expected_blocked, 'observed_update_attempts': [int(row[1]) for row in restores]}
        freeze()
        observe_frozen_mana()
        if a.scalar_dictionary:
            scalar_probe(False,0,7312,2,2)
            scalar_probe(True,-37,-37,2,2)
        if a.saved_skills:
            assert text.count('Native Player saved skills ready |') == 1
            skill_check(0, 1, 0)
        resume()
        text = wait(lambda t: re.search(r'Native Player skill timer callback \| timer ' + str(timer_id) + r' \| callbacks 1 \| slot0 field18 -1', t), 'original cooldown expiry', 35)
        report['cooldown_expiry'] = {'callback_count': 1, 'slot0_field18': -1, 'same_native_timer_id': timer_id}
        freeze()
        observe_frozen_mana()
        if a.saved_skills: skill_check(0, 1, 0)
        command('PLAYER_SKILL_COOLDOWN', '--ei', 'delay_ms', '300')
        resume()
        text = wait(lambda t: re.search(r'Native Player skill timer callback \| timer \d+ \| callbacks 2 \| slot0 field18 -1', t), 'cooldown rearm/expiry')
        assert text.count('Player skill cooldown command applied | Original skill cooldown callback armed') == 2
        report['cooldown_rearm_verified'] = True
        if a.full_update or a.mana_faeries:
            freeze()
            observe_frozen_mana()
            before=frozen_mp
            assert before>=2048,'Knight source MP insufficient for debit/check fixture'
            mana_probe(1024,1,1,before,before-1024)
            remaining=frozen_mp
            mana_probe(8000,0,0,remaining,remaining)
            skill_check(0,1,0,remaining)
            mana_probe(remaining,1,1,remaining,0)
            skill_check(0,0,0,0)
            mana_probe(1,0,0,0,0)
            before_reload=logs()
            snapshot_count=len(TIMER_SNAPSHOT.findall(before_reload))
            command('RELOAD_WORLD')
            text = wait(lambda t: t.count('World reload command applied |') == 2, 'retained MP backing before resumed frames')
            retained_ais(text)
            if a.full_update:
                snapshots,deliveries=source_timer_receipts(text,startup)
                assert len(snapshots)==snapshot_count+2,'zero-MP reload snapshot missing or duplicated'
                restored=snapshots[-2:]
                assert all(row['phase']=='restore' and row['MP']==0 for row in restored), 'reload healed/drained source MP before frames'
                baseline=restored[0]
                target_count=baseline['delivered']+1
                # Reload now preserves inspection time instead of unfreezing it.
                resume()
                def reached_regen(t):
                    return any(row[0]=='33' and int(row[2])==target_count for row in TIMER_DELIVERED.findall(t))
                text=wait(reached_regen,'first resumed source33 regeneration',35)
                snapshots,deliveries=source_timer_receipts(text,startup)
                first=next(row for row in deliveries if row['event']==0x33 and row['count']==target_count)
                assert first['log_offset']>baseline['log_offset'] and first['MP_before']==0 and first['MP']==741, 'first resumed source gain differs'
                freeze()
                text=observe_frozen_mana()
                snapshots,deliveries=source_timer_receipts(text,startup,True)
                resumed=[row for row in deliveries if row['event']==0x33 and row['count']>=target_count]
                assert resumed and frozen_mp==resumed[-1]['MP'], 'MP changed outside reached source33 while frozen'
                assert frozen_mp==min(len(resumed)*741,startup['max_MP']), 'source33 rate/count/cap composition differs'
                skill_check(0,int(frozen_mp>=1024),0,frozen_mp)
                report['post_reload_source_regeneration']={'MP_at_restore':0,'baseline_count':baseline['delivered'],
                    'first_count':first['count'],'first_MP_before':0,'first_MP_after':741,'raw_MP_rate':741,
                    'reached_ticks':len(resumed),'frozen_MP':frozen_mp,'max_MP':startup['max_MP'],
                    'same_timer_slot':startup['timer33'],'source_duration_ms':baseline['duration_ms'],
                    'same_VM':startup['VM'],'initialization_count':1,'no_compensating_heal':True}
                report['source_timers']['snapshots']=snapshots
                report['source_timers']['deliveries']=deliveries
            else:
                freeze()
                observe_frozen_mana()
                skill_check(0,0,0,0)
            assert text.count('Native Player skill update blocked |') == expected_blocked
            assert text.count('Native Player saved faeries ready |') == 1
            report['zero_MP_at_reload_before_resumed_frames'] = True
            if a.scalar_dictionary:
                scalar_probe(False,0,-37,2,2)
                assert report['scalar_dictionary'][0]['VM'] == first_vm[1]
                report['scalar_dictionary_retained_on_reload_and_rotation'] = True
        report['screenshots'].append(screenshot('player-skills-restored'))
        if a.buffs: buff_snapshots(logs(),True)
        if a.full_update:
            text=logs()
            retained_ais(text)
            snapshots,deliveries=source_timer_receipts(text,startup,True)
            report['source_timers']['snapshots']=snapshots
            report['source_timers']['deliveries']=deliveries
        report['validation'] = 'PASS'
    except Exception as exc:
        report['error'] = str(exc)
        raise
    finally:
        (out / 'player-skills.log').write_text(last_logs, encoding='utf-8')
        (out / 'adb-transcript.json').write_text(json.dumps(transcript, indent=2) + '\n')
        report['log_sha256'] = hashlib.sha256(last_logs.encode()).hexdigest()
        (out / 'player-skills-smoke.json').write_text(json.dumps(report, indent=2) + '\n')
        if prior_rotation:
            if prior_rotation[0] == 'lock': adb('shell', 'cmd', 'window', 'user-rotation', 'lock', prior_rotation[1])
            else: adb('shell', 'cmd', 'window', 'user-rotation', 'free')
    print(json.dumps({'validation': report['validation'], 'resources': report['packaged_original_resources_verified'],
                      'cooldown_callbacks': 2, 'player_skill_use': False}))

if __name__ == '__main__': main()
