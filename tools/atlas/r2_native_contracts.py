"""Source-derived round-2 native contracts and reproducible package outputs."""
from collections import Counter, defaultdict
import bisect
import hashlib
import json
import re
from pathlib import Path

import scripts as lua_scripts
from common import read_json, write_json

BASE = Path('r2/natives')
IDA = 'ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so'
QUESTION_MAP = {
    'Q1': {'F_ApplyResult'},
    'Q2': {'SkillCombatRoll__', 'F_CalculateResult', 'CFCalcPush', 'CFCalcStun'},
    'Q3': {'ApplyBuff'},
    'Q7': {'ApplyBuff', 'ApplyPropClass'},
    'Q4': {'CreateBuff'},
    'Q5': {'RemoveBuff'}, 'Q6': {'SetStunState'},
}
T = chr(96)

def _code(value):
    """Keep source expressions on one Markdown row; raw ledger stays exact."""
    value=str(value).replace('\r','\\r').replace('\n','\\n').replace('\t','\\t').replace('|','\\|')
    fence=T*(max((len(m.group()) for m in re.finditer(T+'+',value)),default=0)+1)
    return fence+value+fence

def _native_index(ctx):
    path = ctx.inputs / IDA / 'pseudocode-all.c'
    if not path.is_file():
        return {}, defaultdict(list), defaultdict(list)
    source = path.read_text(encoding='utf-8', errors='replace')
    lines = source.splitlines()
    line_starts = [0]
    line_starts.extend(m.start() + 1 for m in re.finditer('\n', source))
    header_matches = list(re.finditer(r'/\* FUNCTION ([0-9a-fA-F]+) ([^*]+) \*/', source))
    headers = [(m.group(1).lower(), m.group(2).strip(),
                bisect.bisect_right(line_starts, m.start()))
               for m in header_matches]
    by_address, by_handler = {}, defaultdict(list)
    for i, (address, mangled, start) in enumerate(headers):
        end = headers[i + 1][2] - 1 if i + 1 < len(headers) else len(lines)
        body = lines[start:end]
        declaration = '\n'.join(body).split('{', 1)[0]
        match = re.search(r'\b([A-Za-z_][A-Za-z_0-9]*(?:::[A-Za-z_][A-Za-z_0-9]*)+)\s*\(', declaration)
        handler = match.group(1) if match else None
        record = {'address': '0x' + address, 'handler': handler, 'start': start + 1,
                  'end': end, 'lines': body, 'mangled': mangled}
        by_address[address] = record
        if handler:
            by_handler[handler].append(record)
    bindings = defaultdict(list)
    pattern = re.compile(r'Binder::bind(Function|Method)\s*\([^;]*?"([^"\\]+)"\s*,\s*\(void\s*\(\*[^;]*?\)\s*([A-Za-z_][A-Za-z_0-9:]*)(?:\s*,\s*[^;]*?)?\s*\);', re.S)
    header_positions = [(m.start(), m.group(2).strip()) for m in header_matches]
    header_offsets = [x[0] for x in header_positions]
    for match in pattern.finditer(source):
        owner = 'unknown'
        owner_index = bisect.bisect_right(header_offsets, match.start()) - 1
        if owner_index >= 0:
            owner = header_positions[owner_index][1]
        bindings[match.group(2)].append({'kind': match.group(1), 'public': match.group(2),
             'handler': match.group(3), 'owner': owner,
             'line': bisect.bisect_right(line_starts, match.start())})
    return by_address, by_handler, bindings

def _extra_contracts(ctx):
    """Load independently curated semantic contracts without treating them as generated facts."""
    result = {}
    for filename in ('r2_native_contracts_local.json', 'r2_extra_native_contracts.json'):
        path = ctx.root / 'tools/atlas' / filename
        if not path.is_file():
            continue
        payload = read_json(path, {}) or {}
        result.update({str(item.get('name')): item for item in payload.get('contracts') or []
                       if item.get('name')})
    return result

def _facts(record):
    if not record:
        return {'arguments': [], 'conditions': [], 'calls': [], 'returns': []}
    arguments, conditions, calls, returns = [], [], [], []
    for offset, line in enumerate(record['lines']):
        number = record['start'] + offset
        if 'Arguments::operator[]' in line:
            index = re.search(r'Arguments::operator\[\]\([^,\n]+,\s*(\d+)\)', line)
            readers = sorted(set(re.findall(r'Value::(get(?:Number|Boolean|String|UserData|Pointer|Integer|Int|Type))\s*\(', line)))
            arguments.append({'index': int(index.group(1)) if index else None,
                              'readers': readers, 'line': number})
        if re.search(r'\b(if|switch|while|for)\s*\(', line):
            conditions.append(number)
        calls.extend({'callee': name, 'line': number} for name in re.findall(
            r'\b([A-Za-z_][A-Za-z_0-9]*(?:::[A-Za-z_][A-Za-z_0-9]*)+)\s*\(', line)
            if not name.startswith('sfc::script::lua::Value::get'))
        if re.search(r'\breturn\b', line):
            returns.append(number)
    return {'arguments': arguments, 'conditions': conditions, 'calls': calls, 'returns': returns}

def refine(ctx, scope):
    by_address, by_handler, bindings = _native_index(ctx)
    extra = _extra_contracts(ctx)
    rows = []
    for row in scope.get('native_rows') or []:
        registered = []
        aliases = {row.get('name'), str(row.get('name') or '').split(':')[-1].split('.')[-1]}
        for public in sorted(x for x in aliases if x):
            for binding in bindings.get(public, []):
                addr = next((x['address'] for x in by_handler.get(binding['handler'], [])), None)
                registered.append({**binding, 'address': addr})
        addresses = {str(x['address']).lower().replace('0x', '').zfill(8)
                     for x in registered if x.get('address')}
        if not addresses:
            addresses.update(str(x.get('address')).lower().replace('0x', '').zfill(8)
                             for x in row.get('atlas_address_candidates') or [] if x.get('address'))
        bodies = []
        for address in sorted(addresses):
            record = by_address.get(address)
            if not record:
                continue
            rel = f'{IDA}/pseudocode/{address[:4]}/{address}.c'
            path = ctx.inputs / rel
            exists = path.is_file()
            bodies.append({'address': record['address'], 'handler': record.get('handler'),
                'signature': re.sub(r'\s+', ' ', '\n'.join(record['lines']).split('{', 1)[0]).strip(),
                'source': '.local-inputs/' + rel if exists else '.local-inputs/' + IDA + '/pseudocode-all.c',
                'all_source': '.local-inputs/' + IDA + '/pseudocode-all.c',
                'line_start': record['start'], 'line_end': record['end'],
                'sha256': hashlib.sha256(path.read_bytes()).hexdigest() if exists else None,
                'facts': _facts(record)})
        hits = []
        for hit in row.get('current_port_identifier_matches') or []:
            if not isinstance(hit, str) or ':' not in hit:
                continue
            rel, value = hit.rsplit(':', 1)
            try:
                number = int(value)
            except ValueError:
                continue
            port_path = ctx.root / rel
            source_lines = port_path.read_text(encoding='utf-8', errors='replace').splitlines() if port_path.is_file() else []
            if 1 <= number <= len(source_lines):
                hits.append({'path': rel, 'line': number, 'source_line': source_lines[number - 1].strip()[:240]})
        public_base = str(row.get('name') or '').split(':')[-1].split('.')[-1]
        proof = lua_scripts._port_proof(ctx.root, public_base)
        partial_reasons = getattr(lua_scripts, '_PORT_PARTIAL_REASON', {})
        status = 'PARTIAL' if public_base in partial_reasons else 'PORTED' if proof else 'PARTIAL' if hits else 'MISSING'
        reason = (partial_reasons[public_base] if public_base in partial_reasons else
            'Exact callback and registration path evidence exists in frozen production source; not runtime verification.'
            if proof else 'Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.'
            if hits else 'No exact production-source identifier hit or curated callback/binding proof was found.')
        refs = [q for q, names in QUESTION_MAP.items() if public_base in names]
        manual_contract = extra.get(row.get('name')) or extra.get(public_base)
        if manual_contract:
            status = manual_contract.get('port_status') or status
            reason = manual_contract.get('port_difference') or reason
            proof = manual_contract.get('port_evidence') or proof
        rows.append({**row, 'state': 'UNRESOLVED', 'manual_contract': manual_contract, 'contract': {
            'registrations': registered, 'registration_status': 'FOUND' if registered else 'NOT_FOUND_AFTER_BINDER_SCAN',
            'bodies': bodies, 'body_status': 'FOUND' if bodies else 'NOT_FOUND_BY_ADDRESS',
            'port_status': status, 'port_reason': reason, 'port_evidence': proof or hits,
            'root_question_refs': refs}})
    return {'schema': 'atlas-r2-native-scope-refinement-v1',
        'step0_scope_sha256': hashlib.sha256((ctx.out / BASE / 'scope.json').read_bytes()).hexdigest(),
        'source_method': 'Exact Binder registration scan plus address-identified recovered handler body. Body text is not copied into generated output.',
        'rows': rows,
        'counts': {'rows': len(rows), 'registration_found': sum(bool(r['contract']['registrations']) for r in rows),
          'body_found': sum(bool(r['contract']['bodies']) for r in rows),
          'port_status': dict(Counter(r['contract']['port_status'] for r in rows))}}

def _questions(ctx, row):
    path = ctx.out / Path('r2/questions.json')
    if not path.is_file():
        return []
    public_base = str(row.get('name') or '').split(':')[-1].split('.')[-1]
    # Curated known answers attach only to their public Lua API, never to a
    # different native merely because it happens to be called by the handler.
    wanted = [q for q, names in QUESTION_MAP.items() if public_base in names]
    payload = read_json(path, {}) or {}
    return [item for item in payload.get('questions') or []
            if str(item.get('id')) in {q.removeprefix('Q') for q in wanted}]

def _answers(row, question_sections):
    contract = row.get('contract') or {}
    answers = []
    manual = row.get('manual_contract') or {}
    for case in manual.get('known_answers') or []:
        if case.get('confidence') != 'PROVEN':
            continue
        evidence = [(item.get('source') or item.get('path'), item.get('line'))
                    for item in case.get('evidence') or []]
        if case.get('given') and case.get('expected') and evidence:
            answers.append({'question': f'Given: {case["given"]}',
                            'answer': case['expected'], 'evidence': evidence})
        if len(answers) >= 3:
            return answers
    for question in question_sections:
        evidence = []
        for claim in question.get('claims') or []:
            if claim.get('confidence') != 'PROVEN':
                continue
            for item in claim.get('evidence') or []:
                ref = (item.get('path'), item.get('line'))
                if ref not in evidence:
                    evidence.append(ref)
        for case in question.get('known_answers') or []:
            if case.get('confidence') != 'PROVEN':
                continue
            answers.append({'question': f'Given: {case.get("given")}',
                'answer': case.get('expected') or '',
                'evidence': evidence})
            if len(answers) >= 3:
                return answers
    return answers[:3]

def _render(row, question_sections):
    contract = row.get('contract') or {}
    manual = row.get('manual_contract') or {}
    binds, bodies = contract.get('registrations') or [], contract.get('bodies') or []
    answers = _answers(row, question_sections)
    gaps = []
    if not binds and manual.get('status') not in ('NOT-NEEDED', 'UNRESOLVED'):
        gaps.append('Exact Binder registration search was performed, but no binding was found; classify all call spellings, Lua table fields, wrapper traces, and source strings before treating this as a native.')
    if not bodies and manual.get('status') not in ('NOT-NEEDED', 'UNRESOLVED'):
        gaps.append('Address-based native body search was performed, but no body was linked. This source gap is concrete and semantics remain unknown.')
    if len(answers) < 2 and (binds or bodies):
        gaps.append(f'Only {len(answers)} behavior/testcase answer(s) are curated so far. The available handler/callers still require semantic audit; this is TODO work, not a resolved unknown.')
    if not (row.get('all_original_call_sites') or []):
        gaps.append('The all-script Lua call ledger contains no matching caller; native behavior may be engine-only or the public-name alias remains unresolved.')
    # Runtime/visual absence is an evidence boundary, not a source-contract gap.
    if manual.get('status') == 'NOT-NEEDED' and manual.get('unresolved_reasons'):
        status = 'NOT-NEEDED'
    elif manual.get('status') == 'DONE' and len(answers) >= 2:
        status = 'DONE'
    elif manual.get('status') == 'UNRESOLVED' and manual.get('unresolved_reasons'):
        status = 'UNRESOLVED'
    elif binds and bodies and len(answers) >= 2:
        status = 'DONE'
    elif manual.get('status') == 'NOT-NEEDED':
        status = 'NOT-NEEDED'
    elif not binds and not bodies:
        status = 'UNRESOLVED'
    else:
        status = 'TODO'
    out = [f'# {row.get("name")}', '',
       f'Contract status: **{status}**. Static IDA/Lua/port source investigation only. Frozen port status: **{manual.get("port_status") or contract.get("port_status", "UNKNOWN")}**.', '']
    if row.get('rank'):
        out += [f'Engine functions to implement next ranking: **#{row["rank"]}**.', '']
    out += ['## Binding and handler', '']
    if manual.get('binding'):
        item = manual['binding']
        out += ['| Public Lua name | Binding | Handler | Registration owner | Evidence |',
                '| --- | --- | --- | --- | --- |',
                f'| {T}{item.get("public_name") or row.get("name")}{T} | bind{item.get("kind", "?")} | {T}{item.get("handler", "unknown")}{T} | {T}{item.get("registration_owner", "unknown")}{T} | {item.get("source", "source unavailable")}:{item.get("line", "?")} |']
    elif binds:
        out += ['| Public Lua name | Binding | Handler | Registration owner | Evidence |',
                '| --- | --- | --- | --- | --- |']
        unique = sorted({(x.get('public'),x.get('kind'),x.get('handler'),x.get('owner'),x.get('line')) for x in binds})
        for name,kind,handler,owner,line in unique:
            out.append(f'| {T}{name}{T} | bind{kind} | {T}{handler}{T} | {T}{owner}{T} | .local-inputs/{IDA}/pseudocode-all.c:{line} |')
    else:
        if manual.get('status') == 'NOT-NEEDED':
            out.append('**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.')
        else:
            out.append('**NOT-FOUND:** no exact Binder function/method registration was located.')
            for site in (row.get('scope_source_sites') or [])[:12]:
                out.append(f'- Source spelling: {_code(site.get("expression"))} at {site.get("source")}:{site.get("line")}.')
            if not row.get('scope_source_sites'):
                out.append('No scoped Lua site; this row remains because it is in the ranked inventory.')
    for body in bodies:
        out += ['', f'Address-identified callback: {T}{body.get("handler")}{T} at {T}{body["address"]}{T}; source {T}{body["source"]}{T}, unified lines {body["line_start"]}-{body["line_end"]}.',
          f'Recovered declaration: {T}{body.get("signature")}{T}. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.']
    out += ['', '## Lua argument mapping and units', '',
       'bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.']
    if manual.get('lua_signature'):
        out.append('**PROVEN Lua signature/call shape:** ' + T + manual['lua_signature'] + T + '. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.')
    mapping = defaultdict(lambda: {'readers': set(), 'lines': []})
    for body in bodies:
        for item in body['facts']['arguments']:
            if item['index'] is not None:
                mapping[item['index'] + 1]['readers'].update(item['readers'])
                mapping[item['index'] + 1]['lines'].append(item['line'])
    manual_arguments = manual.get('arguments') or []
    if manual_arguments:
        for item in manual_arguments:
            evidence = ', '.join(f'{T}{x.get("source") or x.get("path")}:{x.get("line")}{T}' for x in item.get('evidence') or [])
            label = f'Lua argument {item.get("index")}: {item.get("type", "type unresolved")}'
            if item.get('unit'):
                label += f'; unit/scale {item["unit"]}'
            out.append(f'- {label}. Evidence: {evidence or "none provided"}.')
    else:
        for number, item in sorted(mapping.items()):
            readers = ', '.join(sorted(item['readers'])) or 'getter not adjacent in decompilation'
            out.append(f'- Lua argument {number}: {readers}; handler lines {", ".join(map(str, sorted(set(item["lines"]))))}.')
    if not mapping and not manual_arguments and not manual.get('lua_signature'):
        if manual and 'arguments' in manual:
            body_refs = ', '.join(f'{T}{item["source"]}:1{T}' for item in bodies)
            out.append('**PROVEN:** No Lua arguments are inspected by this audited callback; extra arguments are ignored. Evidence: ' + body_refs + '.')
        else:
            out.append('No explicit operator[] index was extracted. This does not prove a zero-argument callback.')
    out += ['', '## Recovered behavior, gates, state, timers, events and return', '']
    manual_sections = any(manual.get(key) for key in ('behavior', 'gates', 'state_timers_events', 'return'))
    if manual_sections:
        for heading, field in (('Behavior', 'behavior'), ('Gates', 'gates'),
                               ('State, timers and events', 'state_timers_events'), ('Return', 'return')):
            values = manual.get(field) or []
            if isinstance(values, str):
                values = [values]
            if values:
                out.append(f'**{heading}:**')
                for value in values:
                    if isinstance(value, dict):
                        text = value.get('text') or value.get('claim') or str(value)
                        refs = value.get('evidence') or []
                    else:
                        text, refs = str(value), []
                    evidence = ', '.join(f'{T}{x.get("source") or x.get("path")}:{x.get("line")}{T}' for x in refs)
                    label = '' if text.startswith(('PROVEN:', 'LIKELY:', 'GUESS:')) else str(value.get('confidence') or 'PROVEN') + ': ' if isinstance(value, dict) else 'PROVEN: '
                    out.append(f'- {label}{text}' + (f' Evidence: {evidence}.' if evidence else ''))
    elif question_sections:
        for question in question_sections:
            for claim in question.get('claims') or []:
                if claim.get('confidence') == 'PROVEN':
                    evidence = ', '.join(f'{T}{x.get("path")}:{x.get("line")}{T}' for x in claim.get('evidence') or [])
                    out.append(f'- **PROVEN:** {claim.get("text")} Evidence: {evidence}.')
    elif bodies:
        for body in bodies:
            facts = body['facts']
            out += [f'### {T}{body.get("handler") or "handler unresolved"}{T} at {T}{body["address"]}{T}', '',
              f'C handler signature: {T}{body.get("signature") or "signature unavailable"}{T}. Address-identified source: {T}{body["source"]}{T}; unified pseudocode-all lines {body["line_start"]}-{body["line_end"]}; individual body SHA-256 {T}{body.get("sha256") or "unavailable"}{T}. Generated output references, but does not reproduce, the native body.',
              f'- Explicit branch/loop statements: {len(facts["conditions"])}; line references: {", ".join(map(str, facts["conditions"])) or "none"}.',
              '- Qualified calls: ' + (', '.join(f'{T}{x}{T}' for x in sorted({z['callee'] for z in facts['calls']})) or 'none extracted') + '.',
              f'- Return statements detected: {len(facts["returns"])}; exact values and failure cases remain unresolved unless answered below.', '']
    else:
        out.append('No native body was linked; behavior, gates, state, timers, events and return contract are not guessed.')
    out += ['', '## Lua wrapper/helper traces', '']
    traces = list(row.get('wrapper_traces') or [])
    manual_traces = manual.get('wrapper_traces') or []
    wrapper_calls = row.get('calls_through_lua_wrappers') or []
    if traces or wrapper_calls or manual_traces:
        for item in traces:
            out.append('Proven source chain: ' + ' -> '.join(f'{T}{x}{T}' for x in item.get('wrapper_path') or []) + '.')
        for item in wrapper_calls:
            out.append(f'Wrapper call: {T}{item.get("source")}:{item.get("line")}{T} {_code(item.get("expression"))}; chain ' + ' -> '.join(f'{T}{x}{T}' for x in item.get('via') or []) + '.')
        for item in manual_traces:
            refs = ', '.join(f'{T}{x.get("source") or x.get("path")}:{x.get("line")}{T}' for x in item.get('evidence') or [])
            out.append(f'- {item.get("text", "Lua wrapper")}'+(f' Evidence: {refs}.' if refs else '.'))
    else:
        out.append('No helper-to-native trace was proven. Missing trace does not prove direct-only use.')
    out += ['', '## Source-backed behavior cases', '']
    if question_sections:
        for q in question_sections:
            if q.get('title'):
                out.append(f'### {q.get("title")}')
                out.append('')
            for claim in q.get('claims') or []:
                if claim.get('confidence') == 'PROVEN':
                    refs = ', '.join(f'{T}{x.get("path")}:{x.get("line")}{T}' for x in claim.get('evidence') or [])
                    out.append(f'- **PROVEN:** {claim.get("text")} Evidence: {refs}.')
            out.append('')
    elif not answers:
        out.append('No source-backed native result case is asserted: ' + ('the source binding/body is genuinely NOT-FOUND after the specific attempts below.' if status == 'UNRESOLVED' else 'the available source still needs semantic cases; this row remains TODO.'))
    else:
        out.append('PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.')
    out += ['', '## Current frozen port source', '',
       f'**{manual.get("port_status") or contract.get("port_status", "UNKNOWN")}:** {manual.get("port_difference") or contract.get("port_reason", "No current port evidence.")}']
    port_evidence = manual.get('port_evidence') or contract.get('port_evidence') or []
    for item in port_evidence[:12]:
        path, line = item.get('path') or item.get('source'), item.get('line')
        if path:
            detail = item.get('text') or item.get('source_line')
            out.append(f'- Source evidence: {T}{path}{T}' + (f':{line}' if line else '') +
                       (f' — {T}{detail}{T}' if detail else '') + '.')
    port_deltas = {
      'ApplyBuff': 'Original ApplyBuff is registered as both global function and object method and uses the caller-supplied pointer/null/bool branches before recalculating the bound receiver. Frozen production code found here is a selected skill-owner path; it does not prove a general VM binding. The current timed-buff owner replaces matching class instances, which is not identical to the source per-class instance/capacity contract.',
      'CreateBuff': 'Original Character::_CreateBuff creates/reset sheets and schedules a timer for nonzero duration; no Idle admission test appears in this body. Frozen production source exposes selected timed-buff owner paths, not the full original arbitrary-capacity Lua constructor path.',
      'RemoveBuff': 'Original RemoveBuff deletes within its bound receiver and its optional handle only selects among that receiver class instances. The current timed-buff owner erases every instance with the same class name; that is broader than source deletion when multiple instances exist.',
      'SetStunState': 'Original SM_SetStunState performs boss/animation-row/timer/state-event gates. The frozen port routes selected effects through native-effect/FSM services; this does not establish general Lua callback parity or exact source admission for every actor.',
    }
    public_base = str(row.get('name') or '').split(':')[-1].split('.')[-1]
    manual_delta = manual.get('port_difference')
    if manual_delta:
        label = 'Concrete behavioral difference' if (manual.get('port_status') or contract.get('port_status')) in ('PARTIAL', 'MISSING') else 'Port comparison'
        out += ['', f'**{label}:** ' + str(manual_delta)]
    elif public_base in port_deltas:
        out += ['', '**Concrete behavioral difference:** ' + port_deltas[public_base]]
    out += ['', 'Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.', '',
            '## Known answers', '']
    if answers:
        for i, answer in enumerate(answers, 1):
            evidence = ', '.join(f'{T}{p}{T}' + (f':{n}' if n else '') for p,n in answer['evidence'])
            out.append(f'{i}. **PROVEN:** {answer["question"]} {answer["answer"]} Evidence: {evidence}.')
    else:
        out.append('No answer is promoted from a generic name or guessed semantics.')
    sites = row.get('all_original_call_sites') or []
    ledger_label = 'Scoped exact original call ledger (supplement excludes outside-scope calls)' if manual.get('supplement') else 'Global exact-name/function-ID ledger, including calls outside the selected roots'
    out += ['', '## Original Lua call sites', '',
       f'{ledger_label}: **{len(sites)}** sites. Each occurrence is preserved below.', '']
    if sites:
        out += ['| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |',
                '| --- | --- | --- | --- |']
        for site in sites:
            membership = ', '.join(site.get('source_membership') or []) or 'outside scoped roots'
            args = [{'index': a.get('index'), 'expression': a.get('expression'),
                     'literal_tokens': [x.get('raw') for x in a.get('literals') or []]}
                    for a in site.get('arguments') or []]
            out.append(f'| {_code(str(site.get("source"))+":"+str(site.get("line")))} | {membership} | {_code(site.get("expression"))} | {_code(json.dumps(args, ensure_ascii=False, separators=(",", ":")))} |')
    else:
        out.append('No indexed Lua callsite was found.')
    traces = row.get('wrapper_traces') or []
    related = sorted(set((row.get('atlas_handler_candidates') or []) +
        [x.get('wrapper_path', [])[-1] for x in traces if x.get('wrapper_path')]))
    out += ['', '## Related natives', '', ', '.join(f'{T}{x}{T}' for x in related) if related else 'No exact related name established.', '',
      '## Unresolved reasons', '']
    gaps.extend(str(x) for x in manual.get('unresolved_reasons') or [])
    out += [f'- **{status}:** {gap}' for gap in gaps]
    out += ['', '## Evidence boundary', '',
       'This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.', '']
    return status, '\n'.join(out)

def run(ctx):
    scope = read_json(ctx.out / BASE / 'scope.json', {}) or {}
    refined = refine(ctx, scope)
    write_json(ctx.out / BASE / 'scope-refined.json', refined)
    page_root = ctx.out / BASE
    page_root.mkdir(parents=True, exist_ok=True)
    questions = {row['name']: _questions(ctx, row) for row in refined['rows']}
    counts, manifest = Counter(), []
    index = ['# Native contract index', '',
      'One page per ranked native or scoped candidate. Contract completeness and port status are separate; neither means runtime verified.', '',
      '| Rank | Native | Contract | Port | Global calls | Binding | Page |', '| ---: | --- | --- | --- | ---: | --- | --- |']
    for row in refined['rows']:
        filename = re.sub(r'[^A-Za-z0-9_.-]+', '_', row.get('name') or 'unknown') + '.md'
        status, text = _render(row, questions.get(row['name'], []))
        (page_root / filename).write_text(text, encoding='utf-8', newline='\n')
        contract = row['contract']
        calls = len(row.get('all_original_call_sites') or [])
        index.append(f'| {row.get("rank") or ""} | {T}{row["name"]}{T} | **{status}** | **{contract["port_status"]}** | {calls} | {contract["registration_status"]} | [spec]({filename}) |')
        counts[status] += 1
        manifest.append({'name': row['name'], 'path': (BASE / filename).as_posix(),
          'status': status, 'sha256': hashlib.sha256(text.encode()).hexdigest(),
          'global_call_sites': calls, 'known_answers': len(_answers(row, questions.get(row['name'], [])))})
    (ctx.out / BASE / 'INDEX.md').write_text('\n'.join(index) + '\n', encoding='utf-8', newline='\n')
    supplements = []
    extras = _extra_contracts(ctx)
    for name, manual in sorted(extras.items()):
        if not manual.get('supplement'):
            continue
        calls = [item for item in scope.get('scoped_source_call_ledger') or [] if item.get('name') == name]
        row = {'name': name, 'manual_contract': manual, 'all_original_call_sites': calls,
               'contract': {'port_status': manual.get('port_status'), 'registrations': [], 'bodies': []}}
        state, text = _render(row, [])
        (page_root / (name + '.md')).write_text(text, encoding='utf-8', newline='\n')
        supplements.append({'name': name, 'status': state, 'page': name+'.md',
                            'call_sites': len(calls), 'scope': 'Audited Lua helper dependency supplement, outside sealed 271 native rows.'})
    write_json(ctx.out / BASE / 'helper-supplement.json', {'schema': 'atlas-r2-native-helper-supplement-v1', 'rows': supplements})
    if supplements:
        with (ctx.out / BASE / 'INDEX.md').open('a', encoding='utf-8') as target:
            target.write('\n## Audited Lua helper dependencies (outside sealed 271 rows)\n\n')
            for item in supplements:
                target.write(f'- [**{item["name"]}**]({item["page"]}): {item["status"]}; {item["call_sites"]} scoped original calls.\n')
    progress = {'schema': 'atlas-r2-package-progress-v1', 'package': 'A-native-contract-specs',
       'phase': 'CONTRACT_SPEC_GENERATION', 'total': len(manifest),
       'processed': counts['DONE'] + counts['UNRESOLVED'] + counts['NOT-NEEDED'],
       'status_counts': {'DONE': counts['DONE'], 'UNRESOLVED': counts['UNRESOLVED'],
                         'TODO': counts['TODO'], 'NOT-NEEDED': counts['NOT-NEEDED']},
       'expected_contract_pages': len(manifest),
       'scope_files': ['r2/natives/scope.json', 'r2/natives/scope-refined.json', 'r2/natives/INDEX.md'],
       'counts': refined['counts'], 'page_manifest': manifest,
       'contract_status': 'Source contracts generated; unknown behavior remains explicitly unresolved.'}
    write_json(ctx.out / Path('r2/package-a-progress.json'), progress)
    return progress
