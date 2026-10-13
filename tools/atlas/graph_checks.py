"""Focused, low-copy checks for the universal atlas graph.

This module reads the generated graph once and runs coverage_check against a
small in-memory fixture. It does not regenerate or mutate atlas outputs.
"""
from collections import defaultdict, deque
from copy import deepcopy
from pathlib import Path
from types import SimpleNamespace

import graph
from common import read_json


def _node_matches(data, name, kind=None):
    q = name.casefold()
    return [n for n in data.get('nodes', [])
            if (kind is None or n.get('kind') == kind)
            and (n.get('name', '').casefold() == q
                 or q in [str(a).casefold() for a in n.get('aliases', [])])]


def _reachable(start, adjacency):
    seen, todo = {start}, deque([start])
    while todo:
        current = todo.popleft()
        for target in adjacency[current]:
            if target not in seen:
                seen.add(target)
                todo.append(target)
    return seen


def _resource_target_ids(target, asset_paths, by_id, table_by_key):
    """Mirror graph.add_item_resources target conversion for explicit links."""
    ids = set()
    paths = [target.get('uri'), target.get('asset_uri')]
    # A template_asset is one concrete platform variant. Requiring every file
    # with the same logical template URI would invent cross-variant identity.
    if not target.get('template_asset'):
        paths.append(target.get('template_uri'))
    for path in paths:
        if not path:
            continue
        wanted = str(path).replace('\\', '/').casefold().removeprefix('data/').lstrip('/')
        ids.update(asset_paths.get(wanted, ()))
    for asset in target.get('assets') or []:
        if 'asset:' + str(asset.get('id')) in by_id:
            ids.add('asset:' + str(asset['id']))
    template_asset = target.get('template_asset') or {}
    if template_asset.get('id') and 'asset:' + str(template_asset['id']) in by_id:
        ids.add('asset:' + str(template_asset['id']))
    for model in target.get('models') or []:
        model_row = model.get('model') or {}
        model_id = model_row.get('id')
        if model_id and 'table:' + str(model_id) in by_id:
            ids.add('table:' + str(model_id))
        elif model_row.get('table') is not None and model_row.get('index') is not None:
            match = table_by_key.get((model_row['table'], model_row['index']))
            if match:
                ids.add(match)
        asset_id = (model.get('asset') or {}).get('id')
        if asset_id and 'asset:' + str(asset_id) in by_id:
            ids.add('asset:' + str(asset_id))
    return ids


def _item_resource_graph_check(data, links, asset_paths, by_id, table_by_key):
    if not links or 'derived_edges' not in links:
        return {'name': 'all explicit item resource links reach graph targets', 'passed': False,
                'observed': ['item-resource-links.json is missing or incomplete'], 'expected_links': 0}
    edges_by_source_id = defaultdict(list)
    for edge in data.get('edges', []):
        if edge.get('source_edge_id'):
            edges_by_source_id[edge['source_edge_id']].append(edge)
    expected, missing = 0, []
    for source_edge in links['derived_edges']:
        target = source_edge.get('target') or {}
        targets = _resource_target_ids(target, asset_paths, by_id, table_by_key)
        # Unresolved candidates without a corresponding indexed asset/model do
        # not have a graph endpoint to require.
        if not targets:
            continue
        expected += 1
        source = source_edge.get('source') or {}
        source_id = table_by_key.get((source.get('table'), source.get('index')))
        emitted_targets = {edge.get('to') for edge in edges_by_source_id.get(source_edge.get('id'), [])
                           if edge.get('from') == source_id}
        if not source_id or not targets.issubset(emitted_targets):
            missing.append(f"{source.get('table')}[{source.get('index')}].{source_edge.get('field')} -> {sorted(targets)[:3]}")
    return {'name': 'all explicit item resource links reach graph targets', 'passed': not missing,
            'observed': missing[:12], 'expected_links': expected,
            'missing_links': len(missing)}


def _has(reachable_ids, nodes, kind=None, category=None, name=None, suffix=None):
    for node_id in reachable_ids:
        node = nodes[node_id]
        if kind is not None and node.get('kind') != kind:
            continue
        if category is not None and node.get('category') != category:
            continue
        if name is not None and node.get('name', '').casefold() != name.casefold():
            continue
        paths = [node.get('path', '')] + list(node.get('aliases') or [])
        if suffix is not None and not any(str(p).casefold().endswith(suffix.casefold()) for p in paths):
            continue
        return True
    return False


def _coverage_fixture_checks():
    """Exercise graph.coverage_check without copying large generated datasets."""
    base = {
        'data/graph.json': {
            'nodes': [{'id': 'table:owner'}, {'id': 'ref:r'}, {'id': 'table:target'}],
            'edges': [{'id': 'e1', 'from': 'table:owner', 'to': 'ref:r', 'kind': 'reference_site'},
                      {'id': 'e2', 'from': 'ref:r', 'to': 'table:target', 'kind': 'resolution'}],
            'profiles': {'table:owner': {'kind': 'actor'}},
        },
        'data/tables.json': [{'id': 'owner', 'table': 'character_properties#group0'},
                             {'id': 'target', 'table': 'fixture_table'}],
        'data/assets.json': [], 'data/scripts.json': [], 'data/functions.json': [],
        'data/references.json': [{'id': 'r', 'status': 'RESOLVED',
                                  'resolves_to': [{'type': 'table-row', 'table': 'fixture_table', 'index': 0}]}],
        'data/packed-sites.json': {'script_records': [], 'script_calls': []},
    }
    cases = [
        ('valid small graph passes', None, None, None),
        ('duplicate node IDs rejected', 'nodes', lambda x: x.append({'id': 'table:owner'}), 'duplicate'),
        ('missing source node rejected', 'nodes', lambda x: x.remove({'id': 'table:target'}), 'missing tables'),
        ('dangling relationship rejected', 'edges', lambda x: x[0].update(to='table:absent'), 'dangling'),
        ('duplicate relationship IDs rejected', 'edges', lambda x: x.append(deepcopy(x[0])), 'duplicate'),
        ('reference without source owner rejected', 'edges', lambda x: x.pop(0), 'no source owner'),
        ('resolved reference without target rejected', 'edges', lambda x: x.pop(1), 'no target'),
        ('actor without profile rejected', 'profiles', lambda x: x.clear(), 'missing entity profile'),
    ]
    original_reader = graph.read_json
    output = []
    try:
        for name, key, mutate, expected in cases:
            current = deepcopy(base)
            if key is not None:
                fixture = current['data/graph.json'] if key in ('nodes', 'edges', 'profiles') else current
                mutate(fixture[key] if fixture is current['data/graph.json'] else current[key])

            def fixture_read(path, default=None, _current=current):
                suffix = Path(path).as_posix().split('/data/', 1)[-1]
                return deepcopy(_current.get('data/' + suffix, default))

            graph.read_json = fixture_read
            errors = graph.coverage_check(SimpleNamespace(out=Path('fixture')))
            passed = (not errors) if expected is None else any(expected in e for e in errors)
            output.append({'name': name, 'passed': passed, 'observed': errors[:3]})
    finally:
        graph.read_json = original_reader
    return output


def run(ctx):
    """Run focused graph path and rejection checks; return a report dict."""
    data = read_json(ctx.out / 'data/graph.json', {})
    tests = _coverage_fixture_checks()
    nodes = {n['id']: n for n in data.get('nodes', [])}
    adjacency = defaultdict(list)
    table_by_key = {}
    asset_paths = defaultdict(set)
    for edge in data.get('edges', []):
        adjacency[edge.get('from')].append(edge.get('to'))
    for node in nodes.values():
        if node.get('table') is not None and node.get('index') is not None:
            table_by_key[(node['table'], node['index'])] = node['id']
        if node['id'].startswith('asset:'):
            for value in (node.get('path', ''), *(node.get('aliases') or [])):
                normalized = str(value).replace('\\', '/').casefold().removeprefix('data/').lstrip('/')
                if normalized:
                    asset_paths[normalized].add(node['id'])

    def path_test(label, query, kind, requirements):
        matches = _node_matches(data, query, kind)
        if len(matches) != 1:
            tests.append({'name': label, 'passed': False,
                          'observed': [f'expected one {kind} root for {query!r}; found {len(matches)}']})
            return
        reached = _reachable(matches[0]['id'], adjacency)
        missing = [description for description, predicate in requirements if not predicate(reached)]
        outgoing = any(e.get('from') == matches[0]['id'] for e in data.get('edges', []))
        incoming = any(e.get('to') == matches[0]['id'] for e in data.get('edges', []))
        if not outgoing:
            missing.append('forward dependencies')
        if not incoming:
            missing.append('reverse users')
        tests.append({'name': label, 'passed': not missing,
                      'observed': [] if not missing else ['missing reachable: ' + ', '.join(missing)]})

    path_test('Mud Lord reaches authored script, animation, effects, audio and loot',
              'Mud Lord', 'actor', [
                  ('swampking_core script', lambda r: _has(r, nodes, kind='script', name='swampking_core.luac')),
                  ('animation sequence', lambda r: _has(r, nodes, kind='animation_sequence')),
                  ('effect set', lambda r: _has(r, nodes, kind='effect_set')),
                  ('sound row', lambda r: _has(r, nodes, kind='sound')),
                  ('loot item', lambda r: _has(r, nodes, kind='item')),
              ])

    for skill in ('BashDown', 'GroundSlam', 'Charge'):
        path_test(f'Knight skill {skill} reaches script, animation, effect and audio', skill, 'skill', [
            ('skill script', lambda r: _has(r, nodes, kind='script')),
            ('animation sequence', lambda r: _has(r, nodes, kind='animation_sequence')),
            ('effect set', lambda r: _has(r, nodes, kind='effect_set')),
            ('sound row', lambda r: _has(r, nodes, kind='sound')),
        ])

    path_test('Bow item reaches audiovisual data, sound and model asset', 'Bow03', 'item', [
        ('loot audiovisual row', lambda r: any(nodes[i].get('table') == 'loot_audiovisual#group0' for i in r)),
        ('sound row', lambda r: _has(r, nodes, kind='sound')),
        ('3D asset', lambda r: _has(r, nodes, kind='source_asset', suffix='.bdae')),
    ])
    path_test('Potion0 reaches audiovisual data, sound and visual asset', 'Potion0', 'item', [
        ('loot audiovisual row', lambda r: any(nodes[i].get('table') == 'loot_audiovisual#group0' for i in r)),
        ('sound row', lambda r: _has(r, nodes, kind='sound')),
        ('source asset', lambda r: _has(r, nodes, kind='source_asset')),
    ])

    path_test('Bow01 reaches its authored weapon model and audiovisual data', 'Bow01', 'item', [
        ('weapon model BDAE', lambda r: _has(r, nodes, kind='source_asset', name='mc_lweapon_bow_01.bdae')),
        ('loot audiovisual row', lambda r: any(nodes[i].get('table') == 'loot_audiovisual#group0' for i in r)),
        ('sound row', lambda r: _has(r, nodes, kind='sound')),
    ])
    links = read_json(ctx.out / 'data/item-resource-links.json', {})
    tests.append(_item_resource_graph_check(data, links, asset_paths, nodes, table_by_key))

    return {'passed': bool(data) and all(t['passed'] for t in tests),
            'tests': tests,
            'scope': 'Read-only focused graph checks; graph is loaded once and failure fixtures are in memory.'}
