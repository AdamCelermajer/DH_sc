"""Seven source-audited answers, regenerated with exact evidence lines and hashes."""
import hashlib
from pathlib import Path
from common import read_json, write_json

IDA = '.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/'

def body(address):
    return IDA+f'pseudocode/{address[:4]}/{address}.c'

def evidence(path, anchor):
    return {'path':path,'anchor':anchor}

def claim(text, *proof, confidence='PROVEN'):
    return {'text':text,'confidence':confidence,'evidence':list(proof)}

def case(given, expected):
    return {'given':given,'expected':expected,'confidence':'PROVEN',
            'verification':'Source-derived contract expectation; no original-game or port runtime execution claimed.'}

def contracts():
    return [
      {'id':1,'title':'Player and defender combat-result delivery',
       'claims':[
         claim('Character::F_ApplyResult 0x3b10b4 dispatches first to attacker CharAI at Character+968, then defender CharAI at the same offset, both virtual slot +180 with the same (attacker, defender, result). Result mask 0x20000000 suppresses both. Online authority can return before this tail.',
               evidence(body('003b10b4'),'a2[242] + 180'),evidence(body('003b10b4'),'a3[242] + 180'),evidence(body('003b10b4'),'0x20000000')),
         claim('CharAI::OnCombatResults 0x3d0da4 forwards to its selected AIS at +28, virtual slot +180. Exported CharAI and AISDefault vtables map those slots to 0x3d0da4 and 0x3dc9a8 respectively. This is not intrinsically a monster-only event.',
               evidence(body('003d0da4'),'this + 28'),evidence(IDA+'vtables-and-rtti.json','_ZTV10AISDefault')),
         claim('AISDefault::InitVCB 0x3dc7d8 records 0x800 for OnTargetHit and 0x1000 for OnTargetMissed. OnCombatResults 0x3dc9a8 sends missed when outcome byte+24 has either low bit set; otherwise hit, subject to callback registration.',
               evidence(body('003dc7d8'),'OnTargetHit'),evidence(body('003dc9a8'),'a4[24] & 3')),
         claim('StealAIEvent is defined in skills/_commons.luac. It installs an AIEventProxy through AddToVFTable and invokes stored listeners while temporarily changing SKILL_ID. It is not a missing native registration named StealAIEvent.',
               evidence('.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac','function StealAIEvent'),evidence('.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac','AddToVFTable(eventName')),
         claim('The frozen port has character_combat_results_pair_v1, which calls both borrowed AIS contexts. Static existence of this low-level pair does not establish that the Windows player-skill production path supplies and invokes both contexts.',
               evidence('port/level-world/character_combat_results_ai_v1.cpp','int character_combat_results_pair_v1'))],
       'known_answers':[case('Both selected AIS contexts register OnTargetHit; outcome low bits are zero; suppression mask absent.','Two ordered OnTargetHit deliveries, attacker context first, defender context second, each receiving the same actor pair.'),
                        case('Result flags contain 0x20000000.','Neither CharAI receives this F_ApplyResult tail callback.'),
                        case('Outcome low bit 1 is set; selected AIS registers both callbacks.','OnTargetMissed is delivered instead of OnTargetHit.')],
       'remaining':'Native delivery is answered. Production player passive owner/VM integration and actual gameplay execution remain separate work.'},
      {'id':2,'title':'SnS target-chance units and comparisons',
       'claims':[
         claim('The original _F_CalculateResult 0x3b2638 requests Random(100), then shifts the random integer left by 8 before the chance helper. Random uses seed modulo the positive bound, so the comparison domain is integers 0..99 multiplied by 256.',
               evidence(body('003b2638'),'v12 << 8, 2'),evidence(body('003af6d8'),'Random::s_seed % this'),
               evidence('port/level-world/reference/character-dot-attack/original-functions.asm','003b2948: mov      r0, #0x64')),
         claim('Mode 2 stun and push use resolved SnS chance properties 184 and 183. If the raw chance is positive, the effective threshold is chance + 5*attacker_level_raw - defender_resistance_raw - 5*defender_level_raw; strict threshold > random_raw succeeds. Stun resistance is 138; push resistance is 136.',
               evidence(body('003b0fa0'),'PROPS_Get((int)a1 + 1376, 184)'),evidence(body('003b0d78'),'PROPS_Get((int)a1 + 1376, 183)'),evidence(body('003b0fa0'),'return Property > a3;')),
         claim('Values such as 80+5L and 185+4L are chance percentage points represented in fixed256, not an 8-bit probability divided by 256. A base above 100 can still fail after defender resistance and level adjustment. A successful roll is also separate from state admission, including the boss rejection in SM_SetStunState.',
               evidence(body('003b2638'),'CF__CalcStun'),evidence(body('003b0fa0'),'5 * v17'),evidence(body('003c5ffc'),'Character::IsBoss')),
         claim('SkillCombatRoll(object) is a Lua wrapper passing SKILL_ID to SkillCombatRoll__(SKILL_ID, object), whose registered handler is Character::_SkillCombatRoll 0x3b9fbc; it runs F_SkillAttack and then F_ApplyResult.',
               evidence('.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac','return SkillCombatRoll__(SKILL_ID, object);'),evidence(body('003b9fbc'),'Character::F_SkillAttack'))],
       'known_answers':[case('Mode2 stun; equal levels, zero resistance, chance85*256; supplied random84*256.','Chance succeeds; random85*256 fails because comparison is strict.'),
                        case('Mode2 push; equal levels, chance185*256, resistance100*256.','Effective threshold is85*256; random85*256 fails despite the base exceeding100.'),
                        case('Mode2 stun; chance0, supplied random0.','No positive base bonus is applied and 0>0 is false.')],
       'remaining':'This answers the native roll units. It does not assign certainty to a specific visible proc without the actor properties, skill flags and state-admission gates.'},
      {'id':3,'title':'Class formula types 5 and 6',
       'claims':[
         claim('Type5 dispatches to _ScaleWithPercentage 0x3e2c78: destination S[d] becomes arithmetic_shift_right_8(low32(S[a]*S[d])). Type6 dispatches to _ScaleWithPercentageForBuff 0x3e2c10: scale S[a] multiplies S[d], or owner resolved[d] when the caller enables buff mode.',
               evidence(body('003e2c78'),'v7 >> 8'),evidence(body('003e2c10'),'a1 + 2708'),evidence(body('003e2e20'),'case 6:')),
         claim('Type6 then falls through to _AddValue 0x3e2bdc, adding raw Param1 (the scale property index). This unusual addition is actual original behavior: assembly calls 0x3e2c10 at0x3e2ef4 and 0x3e2bdc at0x3e2f04. Do not remove it as an assumed bug.',
               evidence(body('003e2e20'),'goto LABEL_10;'),evidence(body('003e2bdc'),'Property + *(_DWORD *)(a3 + 12)'),
               evidence('port/game-data/reference/classes/reference/original-functions.asm','003e2f04: bl       #0x3e2bdc')),
         claim('For valid property indices the frozen dh2_class_formula cases5/6 match these formulas, including low32 multiplication, arithmetic right shift and the raw Param1 addition. Its buff argument denotes the owner sheet for the type6 destination operand; reversing multiplication order is equivalent.',
               evidence('port/game-data/class_tables.cpp','case 5:'),evidence('port/game-data/class_tables.cpp','case 6:')),
         claim('A resulting buff-sheet Max_HP delta is not the final actor Max_HP. RecalcProperty type&4 accumulates the base, saved, gear and set buff-sheet values. A smaller buff delta alone does not prove Toughness shrinks final HP.',
               evidence(body('003dfe60'),'if ( (Type & 4) != 0 )'),evidence('port/game-data/properties.cpp','if(t&4)'))],
       'known_answers':[case('Type5, destination raw25600, scale raw384.','Raw result38400.'),
                        case('Type6, scale property190 contains384; owner destination raw25600; buff mode enabled.','Raw buff destination38590:38400+190.'),
                        case('Type6, same scale but owner destination raw51200.','Raw buff destination76990:76800+190.')],
       'remaining':'Kernel formula comparison is answered; passives must still apply and combine the correct sheets through their production owner. No runtime HP claim is made.'},
      {'id':4,'title':'Buff capacity, reuse and per-property combination',
       'claims':[
         claim('PROPS_AddBuff 0x3e232c maps capacity<=0 to128. Replacement/reuse scans run only when current count equals requested capacity. Lower unsigned strength entries qualify and their strength is immediately rewritten; equal-strength permanent entries qualify; timed ties compare elapsed time through TMR_TimeLeft. This is not a general FIFO eviction policy.',
               evidence(body('003e232c'),'a4 = 128;'),evidence(body('003e232c'),'v47 >= a5'),evidence(body('003db344'),'*a3 = *(_DWORD *)(v6 + 16);'),
               evidence('port/level-world/reference/character-buffs/NOTES.md','compare **elapsed**, not remaining duration')),
         claim('At full capacity, if no entry qualifies, allocation is refused and nullptr returned. A qualifying entry is reused after stopping its prior timer; nonzero duration starts a one-shot event54 timer, zero duration is permanent. ResetSheet initializes defaults before SnS_Level receives strength<<8.',
               evidence(body('003e232c'),'return nullptr;'),evidence(body('003e232c'),'a3, 0, 54, v59'),evidence(body('003e232c'),'a5 << 8')),
         claim('Combination depends on each property type from AAA_TYPES_DONT_DELETE, not a universal buff-sum rule. Type&4 accumulates; type&2 takes base then saved then gear, then first ascending declaration with a set value (last set instance in that group); type&1 favors the highest descending declaration group, then gear/saved/base. Type&32 uses base+saved; type&16 uses base. Defaults identify unset values.',
               evidence(body('003dfe60'),'if ( (Type & 2) != 0 )'),evidence(body('003dfe60'),'if ( (Type & 1) != 0 )'),evidence(body('003deed8'),'return 16;'),evidence('port/game-data/properties.cpp','auto group=')),
         claim('The native buff kernel and property resolver are recovered, but Windows RuntimeTimedBuffOwnerV1 currently replaces every matching class instance on application and exposes no arbitrary native capacity selection. Its source is a partial production adaptation of the original buff-map contract.',
               evidence('port/windows-foundation/features/generic_skills/runtime_timed_buff_v1.cpp','it = buffs.instances.erase(it);'))],
       'known_answers':[case('Capacity1; existing strength5; request strength4.','No eligible reusable instance; AddBuff returns null instead of evicting the stronger instance.'),
                        case('Capacity1; one permanent strength5; request strength5 duration1000.','Same instance is reused, old timer is stopped, defaults reset, then a new event54 timer starts.'),
                        case('Type&4 property default0, base100*256, two set buff values10*256 and20*256.','Resolved value130*256; for a nonaccumulating type this expectation is not applicable.')],
       'remaining':'Exact observed gameplay stacking, visuals and production arbitrary-capacity integration remain unverified. The handoff FIFO shorthand is superseded by the original scan rules.'},
      {'id':5,'title':'RemoveBuff receiver and foreign handles',
       'claims':[
         claim('Character::_RemoveBuff 0x3b842c calls PROPS_DelBuff on the bound receiver at Character+1376. The optional pointer selects an instance inside that receiver\'s class declaration; it never changes the receiver to the pointer\'s owner.',
               evidence(body('003b842c'),'((char *)a3 + 1376)'),evidence(body('003b56bc'),'"RemoveBuff"')),
         claim('The first argument must be numeric and less than ClassTable size. With one argument deletion uses nullptr; with a second pointer-tag argument the pointer is supplied to PROPS_DelBuff 0x3e101c. Invalid first types/IDs or unsupported second types do not enter deletion.',
               evidence(body('003b842c'),'this < Arrays::ClassTable::size'),evidence(body('003b842c'),'v9 + 116')),
         claim('PROPS_DelBuff removes a sole instance regardless of the supplied pointer; for multiple instances it requires a nonnull exact pointer match. Thus receiverA:RemoveBuff(class, receiverB_handle) can delete A\'s sole instance, but does not delete B\'s buff.',
               evidence(body('003e101c'),'if ( this == 1 )'),evidence(body('003e101c'),'else if ( a3 )'))],
       'known_answers':[case('A has one instance of classC; B has its own; call A:RemoveBuff(C, B_handle).','A\'s declaration is deleted; B remains untouched.'),
                        case('A has two instances; call A:RemoveBuff(C, B_handle), where handle is not in A.','No matching instance is deleted.'),
                        case('A has one instance; call A:RemoveBuff(C) with valid numeric class.','A\'s sole instance/declaration is removed and properties recalculated.')],
       'remaining':'The native receiver rule is answered. A general production per-instance handle path still needs to preserve this behavior.'},
      {'id':6,'title':'CharAnimTable stun lookup and admission',
       'claims':[
         claim('SM_SetStunState 0x3c5ffc rejects bosses first. It obtains GetCharAnimTableId 0x3a3228 (invalid authored ID falls back to17), then validates the returned row against CharAnimTable size. It reads the Stunned field at byte140 of the160-byte native row, not an arbitrary hardcoded animation.',
               evidence(body('003c5ffc'),'Arrays::CharAnimTable::members + 160 * v10 + 140'),evidence(body('003a3228'),'17'),
               evidence('port/level-world/reference/character-native-effects/NOTES.md','Stunned+8c/Scared+7c')),
         claim('Only when pending bit2 is absent does it start timer43 for the supplied unsigned integer milliseconds; reapplying while pending does not restart that timer. The timer response is ignored by this source caller, then pending bit2 is set.',
               evidence(body('003c5ffc'),'this + 11'),evidence(body('003c5ffc'),'a2, 0, 43, nullptr')),
         claim('It queries AnimStancedAnim.SL__LIST_IPHONE mask0x200. If present, GetAnimStance is added to the base Stunned sequence; otherwise modifier0. The selected sequence is stored at machine+40. Force enters state9 through _SetState; otherwise event50012 is raised. Requested mode adds flag0x800 after that transition/event.',
               evidence(body('003c5ffc'),'& 0x200'),evidence(body('003c5ffc'),'9, 50012'),evidence(body('003c5ffc'),'| 0x800'))],
       'known_answers':[case('Boss actor, force=true, duration3000.','No stun timer, override or state transition is performed.'),
                        case('Nonboss valid row, Stunned sequence50, stance7, stanced mask0x200, pending bit clear.','Override57; timer43 starts once with3000ms; normal mode raises50012.'),
                        case('Same actor receives another stun while pending bit2 is already set.','Existing stun timer is retained; animation override/event selection still executes.')],
       'remaining':'Native lookup is answered. Whether the Windows production state machine actually plays and admits each actor\'s authored sequence requires separate integration and visual verification.'},
      {'id':7,'title':'ApplyBuff binding and pointer branch',
       'claims':[
         claim('Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine.',
               evidence(body('003b56bc'),'"ApplyBuff"')),
         claim('A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch.',
               evidence(body('003babdc'),'CharProperties::PROPS_ApplyClassToSheet'),evidence(body('003df314'),'_LoadClass(this, a3, a2, 1)'),evidence(body('003babdc'),'RecalcProperties')),
         claim('The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check.',
               evidence(body('003babdc'),'this > Arrays::ClassTable::size'),evidence(body('003e2e20'),'a3 < Arrays::ClassTable::size'))],
       'known_answers':[case('ApplyBuff(validClass, liveBuffSheetPointer).','Class rows execute on that sheet in buff mode, then receiver properties recalculate.'),
                        case('ApplyBuff(validClass, nullPointer).','Pointer branch performs no class application or recalculation.'),
                        case('ApplyPropClass(validClass, true).','Boolean branch applies the class through PROPS_ApplyClass, not the pointer-sheet buff branch.')],
       'remaining':'Registration and behavior branch are answered. A pointer association alone does not guarantee that every Windows skill owner applies an equivalent buff.'}
    ]

def _proof(ctx, item):
    path = item['path']
    file = ctx.inputs/path[len('.local-inputs/'):] if path.startswith('.local-inputs/') else ctx.root/path
    if not file.is_file():raise ValueError('Missing question evidence: '+path)
    raw = file.read_bytes()
    lines = raw.decode('utf-8',errors='replace').splitlines()
    matches = [i+1 for i,line in enumerate(lines) if item['anchor'] in line]
    if not matches:raise ValueError('Question source anchor absent: '+path+' '+item['anchor'])
    return {**item,'line':matches[0],'matching_lines':matches,'sha256':hashlib.sha256(raw).hexdigest()}

def run(ctx):
    rows = contracts()
    for row in rows:
        row['state']='DONE'
        row['attempts']=['Read exact individual IDA bodies and callers.',
                         'Compare recovered source/assembly and frozen port implementation; registration names traced where applicable.']
        for c in row['claims']:c['evidence']=[_proof(ctx,e) for e in c['evidence']]
    write_json(ctx.out/'r2/questions.json',{'schema':'atlas-r2-open-questions-v1','questions':rows})
    text = ['# Seven native questions: original-source answers','',
            'These are source contracts and proposed known-answer cases. They do not certify runtime implementation, gameplay or visuals. Confidence applies per claim; source bodies and their callers take precedence over shorthand in earlier handoffs.','']
    for row in rows:
        text.extend([f'## Q{row["id"]}. {row["title"]}',''])
        for c in row['claims']:
            refs='; '.join(f'`{e["path"]}:{e["line"]}`' for e in c['evidence'])
            text.extend([f'**{c["confidence"]}:** {c["text"]} Evidence: {refs}.',''])
        text.extend(['Known answers (source-derived; runtime tests still required):',''])
        for c in row['known_answers']:text.append(f'- **{c["confidence"]}:** {c["given"]} → {c["expected"]}')
        text.extend(['',row['remaining'],''])
    dest=ctx.out/'r2/natives/QUESTIONS.md';dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_text('\n'.join(text),encoding='utf-8',newline='\n')
    return rows

def check(ctx):
    data=read_json(ctx.out/'r2/questions.json',{})
    rows=data.get('questions',[]);errors=[]
    if sorted(q.get('id',0) for q in rows)!=list(range(1,8)):errors.append('All seven open questions must be present exactly once')
    for q in rows:
        if q.get('state') not in ('DONE','UNRESOLVED','NOT-NEEDED'):errors.append(f'Q{q.get("id")}: no terminal disposition')
        if not q.get('attempts'):errors.append(f'Q{q.get("id")}: missing investigation attempts')
        if not q.get('claims') or len(q.get('known_answers',[]))<2:errors.append(f'Q{q.get("id")}: answer/cases missing')
        for case in q.get('known_answers',[]):
            if not case.get('given') or not case.get('expected') or case.get('confidence') not in ('PROVEN','LIKELY','GUESS'):
                errors.append(f'Q{q.get("id")}: malformed behavioral case')
        for c in q.get('claims',[]):
            if c.get('confidence') not in ('PROVEN','LIKELY','GUESS') or not c.get('evidence'):
                errors.append(f'Q{q.get("id")}: claim confidence/evidence missing')
            for e in c.get('evidence',[]):
                try:
                    current=_proof(ctx,e)
                    if any(current[k]!=e.get(k) for k in ('line','matching_lines','sha256')):errors.append(f'Q{q.get("id")}: source evidence drift: '+e['path'])
                except ValueError as ex:errors.append(str(ex))
    if not (ctx.out/'r2/natives/QUESTIONS.md').is_file():errors.append('Question answer page missing')
    return {'passed':not errors,'errors':errors,'tests':[{'name':'seven source-audited questions and hash-bound evidence','passed':not errors}]}
