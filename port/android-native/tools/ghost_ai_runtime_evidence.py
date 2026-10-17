"""Validate observed Ghost AI acquisition and body pursuit in device logs.

Path requests and compiled exports alone cannot establish live movement.
Input records must come from the native owner's actual source counters and
body positions. Each lifecycle epoch is validated separately by its caller.
"""
import json
import math

NAMES = ('_prim_Monster_SURPRISE_01', '_prim_Monster_SURPRISE_02')
MARKER = 'GhostAI frame | '


def vector(record, key):
    value = record[key]
    assert isinstance(value, list) and len(value) == 3, (key, value)
    assert all(isinstance(x, (float, int)) and not isinstance(x, bool) and math.isfinite(x) for x in value)
    return value


def distance(first, second):
    return math.sqrt(sum((a-b)**2 for a, b in zip(first, second)))


def validate_pursuit(logs):
    records = []
    for line in logs.splitlines():
        if MARKER not in line:
            continue
        record = json.loads(line.split(MARKER, 1)[1])
        if record.get('name') in NAMES:
            records.append(record)
    groups = {name: [r for r in records if r['name'] == name] for name in NAMES}
    assert all(groups.values()), 'both distinct source Ghosts must be observed'
    result = {}
    all_actor_ids, all_ai_ids, all_ais_ids = set(), set(), set()
    for name, rows in groups.items():
        identity_keys = ('actor_id', 'character_id', 'ai_id', 'ais_id', 'player_id')
        identities = {key: rows[0][key] for key in identity_keys}
        for key, identity in identities.items():
            assert isinstance(identity, int) and not isinstance(identity, bool) and identity > 0, (key, identity)
        assert identities['character_id'] != identities['player_id']
        all_actor_ids.add(identities['actor_id']); all_ai_ids.add(identities['ai_id']); all_ais_ids.add(identities['ais_id'])
        assert all(all(r[key] == value for key, value in identities.items()) for r in rows), 'owner identity changed within epoch'
        assert all(b['frame_index'] > a['frame_index'] for a, b in zip(rows, rows[1:])), 'duplicate or reversed actor frames'
        def successful(row):
            return row['status'] == 0 and row['frame_status'] == 0 and row['frame_skip'] == 0 and row['boundary'] == ''
        acquired = [r for r in rows if successful(r) and r['source_search_started'] and
                    r['candidates'] > 0 and r['events'] > 0 and r['script_callbacks'] > 0 and
                    r['set_target_calls'] > 0 and r['head_to_calls'] > 0 and
                    r['target_id'] == identities['player_id'] and r['path_count'] > 0]
        assert acquired, name + ': no complete source search/Lua/target/controller acquisition'
        first = acquired[0]
        later = [r for r in rows if r['frame_index'] >= first['frame_index']]
        moving, displacements = [], []
        for row in later:
            before, after = vector(row, 'pos_before'), vector(row, 'pos_after')
            player, endpoint = vector(row, 'player_position'), vector(row, 'desired_endpoint')
            delta = distance(before, after)
            assert row['target_id'] in (0, identities['player_id']), 'unexpected target in pursuit interval'
            if delta <= 0.001:
                continue
            assert successful(row), name + ': movement observed after an unresolved or failed AI frame'
            assert row['target_id'] == identities['player_id'] and row['path_count'] > 0
            assert distance(endpoint, player) <= 1.0, 'route endpoint is not the observed target position'
            if distance(after, player) + 0.001 < distance(before, player):
                moving.append(row['frame_index']); displacements.append(delta)
        assert len(moving) >= 3 and sum(displacements) > 1.0, name + ': no sustained body pursuit'
        result[name] = {'identities': identities, 'acquisition_frame': first['frame_index'],
                        'observed_frames': len(rows), 'body_motion_frames_toward_player': moving,
                        'summed_displacement_toward_player': sum(displacements),
                        'source_search_lua_target_path_and_body_motion_observed': True}
    assert len(all_actor_ids) == len(all_ai_ids) == len(all_ais_ids) == 2, 'actor/AI/AIS sessions are shared'
    assert groups[NAMES[0]][0]['player_id'] == groups[NAMES[1]][0]['player_id'], 'different player ownership'
    return {'validation': 'PASS', 'actors': result,
            'scope': 'Two independent Ghost source acquisition/Lua/controller chains followed by actual body movement toward the same player in one observed lifecycle epoch; no attack, full-level, saved-game or complete-AI claim.'}
