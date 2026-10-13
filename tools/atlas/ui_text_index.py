"""Read-only atlas for original UI movies, fonts, textures, and localized text.

The index keeps SWF character IDs in their owning movie namespace. Numeric
StringManager IDs are decoded only with the shipped StringConfig constants and
the common_text sheet table; unresolved IDs remain explicit gaps.
"""
from __future__ import annotations

from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import struct
import zlib

from common import stable_id


_CACHE_REL = Path('port/android-native/app/src/main/assets/original-cache')
_STRING_TABLES = {
    ('character_properties#group0', 'Name'): 'actor-name',
    ('skills#group1', 'SkillName'): 'skill-name',
    ('skills#group1', 'SkillDescription'): 'skill-description',
    ('loot_table#group3', 'Name'): 'item-name',
}
_TABLE_ROW_TABLES = {item[0] for item in _STRING_TABLES} | {'faeries#group1'}
_FONT_EXTENSIONS = {'.ttf', '.otf', '.fnt'}
_FONT_DEFINITION_TAGS = {10, 48, 75, 91}  # DefineFont/2/3/4.
_FONT_REFERENCE_TAGS = {13, 62, 73, 88}   # Font info, zones, and names.
_BITMAP_TAGS = {6, 20, 21, 35, 36, 90}     # DefineBits/JPEG/Lossless family.
_SHAPE_TAGS = {2, 22, 32, 83, 46, 84}     # Shape and morph-shape definitions.


def _node_id(kind: str, *parts) -> str:
    return f'{kind}:{stable_id("ui-text-index", kind, *parts)}'


def _node(nodes, kind: str, name: str, source: str, **extra) -> str:
    identity = extra.pop('id', None) or _node_id(kind, source, extra.get('character_id', ''), name)
    nodes.setdefault(identity, {'id': identity, 'name': name, 'kind': kind, 'source': source, **extra})
    return identity


def _edge(edges, source_id: str, target_id: str, kind: str, evidence: str = ''):
    key = (source_id, target_id, kind, evidence)
    edges.setdefault(key, {'id': _node_id('edge', *key), 'source_id': source_id,
                           'target_id': target_id, 'kind': kind, 'evidence': evidence})


def _cache_root(ctx, assets) -> Path:
    """Find the indexed cache from an asset path or the repository's staged root."""
    for row in assets or ():
        path = str(row.get('path', '')).replace('\\', '/')
        marker = '/original-cache/'
        if marker not in ('/' + path.lstrip('/')):
            continue
        normalized = '/' + path.lstrip('/')
        before = normalized.split(marker, 1)[0]
        if before.startswith('/.local-inputs/'):
            candidate = Path(ctx.inputs) / before[len('/.local-inputs/'):].replace('/', '\\') / 'original-cache'
        elif before.startswith('/.repo/'):
            candidate = Path(ctx.root) / before[len('/.repo/'):].replace('/', '\\') / 'original-cache'
        else:
            candidate = Path(path)
            for parent in candidate.parents:
                if parent.name == 'original-cache':
                    candidate = parent
                    break
        if (candidate / 'index.json').is_file() and (candidate / 'text-index.json').is_file():
            return candidate
    candidate = Path(ctx.root) / _CACHE_REL
    if (candidate / 'index.json').is_file() and (candidate / 'text-index.json').is_file():
        return candidate
    raise FileNotFoundError('original-cache index.json and text-index.json are unavailable')


def _manifest(path: Path) -> dict:
    value = json.loads(path.read_text(encoding='utf-8'))
    if value.get('version') != 1 or not isinstance(value.get('resources'), dict):
        raise ValueError(f'unsupported original-cache index: {path.name}')
    return value


def _resource_path(cache: Path, record: dict) -> Path:
    rel = PurePosixPath(str(record.get('asset', '')).replace('\\', '/'))
    if rel.is_absolute() or not rel.parts or any(part in ('', '.', '..') for part in rel.parts):
        raise ValueError('invalid indexed asset path')
    target = cache.parent.joinpath(*rel.parts).resolve()
    root = cache.parent.resolve()
    if target != root and root not in target.parents:
        raise ValueError('indexed path escaped original-cache parent')
    return target


def _verified_bytes(cache: Path, record: dict) -> bytes:
    raw = _resource_path(cache, record).read_bytes()
    if record.get('bytes') != len(raw):
        raise ValueError('indexed resource byte length mismatch')
    if record.get('sha256') != hashlib.sha256(raw).hexdigest():
        raise ValueError('indexed resource SHA-256 mismatch')
    return raw


def _cstring(data: bytes, at: int) -> tuple[str, int]:
    end = data.find(b'\0', at)
    if end < 0:
        raise ValueError('unterminated SWF string')
    return data[at:end].decode('utf-8', 'strict'), end + 1


def _tags(data: bytes, start: int = 0, *, base_offset: int = 0):
    at = start
    rows = []
    ended = False
    ordinal = 0
    while at < len(data):
        tag_offset = base_offset + at
        if at + 2 > len(data):
            raise ValueError('truncated SWF tag header')
        header = struct.unpack_from('<H', data, at)[0]
        at += 2
        code, length = header >> 6, header & 0x3f
        if length == 0x3f:
            if at + 4 > len(data):
                raise ValueError('truncated SWF extended tag length')
            length = struct.unpack_from('<I', data, at)[0]
            at += 4
        end = at + length
        if end > len(data):
            raise ValueError(f'SWF tag {code} exceeds declared file length')
        payload = data[at:end]
        rows.append((code, payload, base_offset + at, tag_offset, ordinal))
        ordinal += 1
        at = end
        if code == 0:
            if length != 0:
                raise ValueError('SWF End tag has a payload')
            ended = True
            break
    if not ended:
        raise ValueError('SWF timeline has no End tag')
    if at != len(data):
        raise ValueError('bytes follow SWF End tag')
    return rows


def _swf(raw: bytes):
    if len(raw) < 12 or raw[:3] not in (b'FWS', b'CWS'):
        raise ValueError('unsupported or truncated SWF header')
    version = raw[3]
    declared = struct.unpack_from('<I', raw, 4)[0]
    if declared < 12 or declared > 128 * 1024 * 1024:
        raise ValueError('SWF declared file length outside bounds')
    if raw[:3] == b'FWS':
        if declared != len(raw):
            raise ValueError('FWS declared length differs from bytes')
        body = raw[8:]
    else:
        inflater = zlib.decompressobj()
        body = inflater.decompress(raw[8:], declared) + inflater.flush()
        if not inflater.eof or inflater.unused_data or inflater.unconsumed_tail:
            raise ValueError('CWS compressed stream is incomplete or has trailing data')
        if len(body) + 8 != declared:
            raise ValueError('CWS declared length differs from decompressed bytes')
    nbits = body[0] >> 3
    offset = (5 + 4 * nbits + 7) // 8 + 4
    if offset > len(body):
        raise ValueError('truncated SWF frame header')
    return version, _tags(body, offset, base_offset=8)


_AVM1_NAMES = {
    0x00: 'End', 0x04: 'NextFrame', 0x05: 'PreviousFrame', 0x06: 'Play',
    0x07: 'Stop', 0x08: 'ToggleQuality', 0x09: 'StopSounds',
    0x0A: 'Add', 0x0B: 'Subtract', 0x0C: 'Multiply', 0x0D: 'Divide',
    0x0E: 'Equals', 0x0F: 'Less', 0x10: 'And', 0x11: 'Or', 0x12: 'Not',
    0x13: 'StringEquals', 0x14: 'StringLength', 0x15: 'StringExtract',
    0x17: 'Pop', 0x18: 'ToInteger', 0x1C: 'GetVariable', 0x1D: 'SetVariable',
    0x20: 'SetTarget2', 0x21: 'StringAdd', 0x22: 'GetProperty',
    0x23: 'SetProperty', 0x24: 'CloneSprite', 0x25: 'RemoveSprite',
    0x26: 'Trace', 0x27: 'StartDrag', 0x28: 'EndDrag', 0x29: 'StringLess',
    0x2A: 'Throw', 0x2B: 'Cast', 0x2C: 'Implements', 0x30: 'RandomNumber',
    0x31: 'MBStringLength', 0x32: 'CharToAscii', 0x33: 'AsciiToChar',
    0x34: 'GetTime', 0x35: 'MBStringExtract', 0x36: 'MBCharToAscii',
    0x37: 'MBAsciiToChar', 0x3A: 'Delete', 0x3B: 'Delete2', 0x3C: 'DefineLocal',
    0x3D: 'CallFunction', 0x3E: 'Return', 0x3F: 'Modulo', 0x40: 'NewObject',
    0x41: 'DefineLocal2', 0x42: 'InitArray', 0x43: 'InitObject', 0x44: 'TypeOf',
    0x45: 'TargetPath', 0x46: 'Enumerate', 0x47: 'Add2', 0x48: 'Less2',
    0x49: 'Equals2', 0x4A: 'ToNumber', 0x4B: 'ToString', 0x4C: 'PushDuplicate',
    0x4D: 'StackSwap', 0x4E: 'GetMember', 0x4F: 'SetMember', 0x50: 'Increment',
    0x51: 'Decrement', 0x52: 'CallMethod', 0x53: 'NewMethod', 0x54: 'InstanceOf',
    0x55: 'Enumerate2', 0x60: 'BitAnd', 0x61: 'BitOr', 0x62: 'BitXor',
    0x63: 'BitLShift', 0x64: 'BitRShift', 0x65: 'BitURShift', 0x66: 'StrictEquals',
    0x67: 'Greater', 0x68: 'StringGreater', 0x69: 'Extends',
    0x81: 'GotoFrame', 0x83: 'GetURL', 0x87: 'StoreRegister',
    0x88: 'ConstantPool', 0x8A: 'WaitForFrame', 0x8B: 'SetTarget',
    0x8C: 'GoToLabel', 0x8D: 'WaitForFrame2', 0x8E: 'DefineFunction2',
    0x8F: 'Try', 0x94: 'With', 0x96: 'Push', 0x99: 'Jump', 0x9A: 'GetURL2',
    0x9B: 'DefineFunction', 0x9D: 'If', 0x9E: 'Call', 0x9F: 'GotoFrame2',
}


def _avm1_cstring(data: bytes, at: int):
    end = data.find(b'\0', at)
    if end < 0:
        raise ValueError('unterminated AVM1 string operand')
    if end - at > 1024 * 1024:
        raise ValueError('AVM1 string operand exceeds bound')
    return data[at:end].decode('utf-8', 'strict'), end + 1


def _avm1_inventory(raw: bytes):
    """Count AVM1 opcodes and extract bounded authored strings, without execution."""
    at, counts, records, ended = 0, Counter(), 0, False
    constant_pool, push_strings, function_names = [], [], []
    literal_bytes = 0

    def keep_string(target, value):
        nonlocal literal_bytes
        if len(target) >= 16384:
            raise ValueError('AVM1 extracted string count exceeds bound')
        literal_bytes += len(value.encode('utf-8'))
        if literal_bytes > 8 * 1024 * 1024:
            raise ValueError('AVM1 extracted strings exceed per-block byte bound')
        target.append(value)
    while at < len(raw):
        if records >= 1_000_000:
            raise ValueError('AVM1 action record count exceeds bound')
        opcode = raw[at]
        at += 1
        size = 0
        if opcode >= 0x80:
            if at + 2 > len(raw):
                raise ValueError('truncated AVM1 long action length')
            size = struct.unpack_from('<H', raw, at)[0]
            at += 2
        if size > len(raw) - at:
            raise ValueError('AVM1 action payload exceeds block')
        operand = raw[at:at + size]
        if opcode == 0x88:  # ActionConstantPool.
            if len(operand) < 2:
                raise ValueError('truncated AVM1 ActionConstantPool count')
            pool_count = struct.unpack_from('<H', operand, 0)[0]
            if pool_count > 16384:
                raise ValueError('AVM1 constant pool count exceeds bound')
            pos = 2
            for _ in range(pool_count):
                value, pos = _avm1_cstring(operand, pos)
                keep_string(constant_pool, value)
            if pos != len(operand):
                raise ValueError('bytes follow AVM1 ActionConstantPool entries')
        elif opcode == 0x96:  # ActionPush; retain only typed string operands.
            pos = 0
            while pos < len(operand):
                value_type = operand[pos]
                pos += 1
                if value_type == 0:
                    value, pos = _avm1_cstring(operand, pos)
                    keep_string(push_strings, value)
                else:
                    widths = {1: 4, 2: 0, 3: 0, 4: 1, 5: 1, 6: 8,
                              7: 4, 8: 1, 9: 2}
                    if value_type not in widths:
                        raise ValueError(f'unknown AVM1 ActionPush value type {value_type}')
                    width = widths[value_type]
                    if width > len(operand) - pos:
                        raise ValueError('truncated AVM1 ActionPush operand')
                    pos += width
        elif opcode in (0x9B, 0x8E):  # DefineFunction / DefineFunction2.
            name, pos = _avm1_cstring(operand, 0)
            keep_string(function_names, name)
            if pos + 2 > len(operand):
                raise ValueError('truncated AVM1 function parameter count')
            parameter_count = struct.unpack_from('<H', operand, pos)[0]
            pos += 2
            if parameter_count > 16384:
                raise ValueError('AVM1 function parameter count exceeds bound')
            if opcode == 0x8E:
                if pos + 3 > len(operand):
                    raise ValueError('truncated AVM1 DefineFunction2 header')
                pos += 3  # Register count and flags.
            for _ in range(parameter_count):
                if opcode == 0x8E:
                    if pos >= len(operand):
                        raise ValueError('truncated AVM1 DefineFunction2 parameter register')
                    pos += 1
                _, pos = _avm1_cstring(operand, pos)
            if pos + 2 > len(operand):
                raise ValueError('truncated AVM1 function code size')
            code_size = struct.unpack_from('<H', operand, pos)[0]
            pos += 2
            if pos != len(operand):
                raise ValueError('bytes follow AVM1 function code-size field')
            # Function bytecode follows this declaration action in the same
            # action stream, rather than inside its declared action payload.
            # The normal opcode walk inventories those bytes in place.
            if code_size > len(raw) - at:
                raise ValueError('AVM1 function body exceeds enclosing action block')
        at += size
        counts[opcode] += 1
        records += 1
        if opcode == 0:
            ended = True
            if at != len(raw):
                raise ValueError('bytes follow AVM1 ActionEnd')
            break
    if not ended:
        raise ValueError('AVM1 action block has no ActionEnd')
    return {'record_count': records,
            'opcode_counts': {f'0x{op:02x}': count for op, count in sorted(counts.items())},
            'opcode_names': {f'0x{op:02x}': _AVM1_NAMES[op] for op in counts if op in _AVM1_NAMES},
            'constant_pool_strings': list(dict.fromkeys(constant_pool)),
            'push_string_literals': list(dict.fromkeys(push_strings)),
            'declared_function_names': list(dict.fromkeys(function_names)),
            'unhandled_operand_payloads': sum(count for op, count in counts.items()
                                                if op >= 0x80 and op not in (0x88, 0x8e, 0x96, 0x9b))}


def _font_or_texture(tag: int) -> str | None:
    if tag in _FONT_DEFINITION_TAGS:
        return 'swf-font'
    if tag in _BITMAP_TAGS:
        return 'swf-texture'
    if tag in _SHAPE_TAGS:
        return 'swf-shape'
    if tag == 39:
        return 'swf-sprite'
    if tag in (7, 34):
        return 'swf-button'
    if tag == 37:
        return 'swf-edit-text'
    return None


def _id_and_name(tag: int, payload: bytes):
    if tag in _FONT_DEFINITION_TAGS | _BITMAP_TAGS | _SHAPE_TAGS | {39, 7, 34, 37}:
        if len(payload) < 2:
            raise ValueError(f'SWF definition tag {tag} lacks CharacterID')
        return struct.unpack_from('<H', payload, 0)[0], None
    if tag in (56, 76):  # ExportAssets or SymbolClass
        if len(payload) < 2:
            raise ValueError('truncated SWF export table')
        count = struct.unpack_from('<H', payload, 0)[0]
        at, result = 2, []
        if count > 65535:
            raise ValueError('SWF export count outside range')
        for _ in range(count):
            if at + 2 > len(payload):
                raise ValueError('truncated SWF export CharacterID')
            character_id = struct.unpack_from('<H', payload, at)[0]
            name, at = _cstring(payload, at + 2)
            result.append((character_id, name))
        if at != len(payload):
            raise ValueError('SWF export table has trailing bytes')
        return None, result
    return None, None


def _parse_swf(raw: bytes, uri: str, nodes, edges, aliases, gaps):
    version, top = _swf(raw)
    movie_id = _node(nodes, 'swf-movie', uri, uri, id=_node_id('swf-movie', uri),
                     version=version, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
    nodes[movie_id].update({'version': version, 'bytes': len(raw),
                            'sha256': hashlib.sha256(raw).hexdigest()})
    symbols = {}
    exports = []
    imports = []
    placements = []
    script_edges = []

    def visit(tags, owner='timeline'):
        nonlocal exports, imports
        frame_index = 0
        for code, payload, payload_offset, tag_offset, tag_ordinal in tags:
            try:
                char_id, names = _id_and_name(code, payload)
            except (ValueError, UnicodeDecodeError, struct.error) as exc:
                char_id, names = None, None
                gaps.append({'source': uri, 'kind': 'swf-symbol-parse',
                             'tag_code': code, 'tag_ordinal': tag_ordinal,
                             'offset': tag_offset, 'reason': str(exc)})
            kind = _font_or_texture(code)
            if char_id is not None and kind:
                identity = _node_id(kind, uri, char_id)
                symbols[char_id] = _node(nodes, kind, f'{uri}#{char_id}', uri, id=identity,
                                         character_id=char_id, tag_code=code)
                _edge(edges, movie_id, identity, 'defines', f'SWF tag {code}')
            if names:
                exports.extend(names)
            if code in (12, 59):  # DoAction / DoInitAction.
                action_offset = 2 if code == 59 else 0
                if len(payload) < action_offset:
                    raise ValueError(f'truncated SWF DoInitAction tag {tag_ordinal}')
                if code == 59 and len(payload) < 2:
                    raise ValueError(f'DoInitAction lacks SpriteID at tag {tag_ordinal}')
                action_data = payload[action_offset:]
                action_sha = hashlib.sha256(action_data).hexdigest()
                timeline_owner_id = None if owner == 'timeline' else int(owner.split(':', 1)[1])
                target_sprite_id = struct.unpack_from('<H', payload, 0)[0] if code == 59 else None
                owner_id = target_sprite_id if code == 59 else timeline_owner_id
                identity = _node_id('swf-action-block', uri, code, owner_id,
                                    frame_index, tag_ordinal, tag_offset)
                try:
                    inventory = _avm1_inventory(action_data)
                    status = 'indexed-opcodes'
                except ValueError as exc:
                    inventory = {'record_count': None, 'opcode_counts': {}, 'opcode_names': {},
                                 'constant_pool_strings': [], 'push_string_literals': [],
                                 'declared_function_names': [], 'unhandled_operand_payloads': None}
                    status = 'malformed-opaque'
                    gaps.append({'source': uri, 'kind': 'swf-action-block',
                                 'tag_ordinal': tag_ordinal, 'offset': tag_offset,
                                 'reason': str(exc)})
                node = {'id': identity, 'name': f'{uri}#tag{tag_ordinal}-action@{tag_offset}',
                        'kind': 'swf-action-block', 'source': uri,
                        'tag_code': code, 'tag_ordinal': tag_ordinal,
                        'tag_offset': tag_offset, 'payload_offset': payload_offset + action_offset,
                        'decoded_swf_offset': payload_offset + action_offset,
                        'source_sha256': hashlib.sha256(raw).hexdigest(),
                        'timeline_owner_character_id': timeline_owner_id,
                        'target_sprite_id': target_sprite_id,
                        'frame_index': frame_index, 'byte_count': len(action_data),
                        'sha256': action_sha, 'inventory_status': status, **inventory}
                nodes.setdefault(identity, node)
                for literal in inventory.get('constant_pool_strings', ()):
                    aliases.append({'source_id': identity, 'alias': literal,
                                    'kind': 'avm1-constant-pool-string', 'namespace': uri,
                                    'tag_ordinal': tag_ordinal, 'offset': tag_offset,
                                    'evidence': 'Validated ActionConstantPool string operand'})
                for literal in inventory.get('push_string_literals', ()):
                    aliases.append({'source_id': identity, 'alias': literal,
                                    'kind': 'avm1-push-string', 'namespace': uri,
                                    'tag_ordinal': tag_ordinal, 'offset': tag_offset,
                                    'evidence': 'Validated type-0 ActionPush string operand'})
                for function_name in inventory.get('declared_function_names', ()):
                    aliases.append({'source_id': identity, 'alias': function_name,
                                    'kind': 'avm1-function-declaration', 'namespace': uri,
                                    'tag_ordinal': tag_ordinal, 'offset': tag_offset,
                                    'evidence': 'Validated DefineFunction/DefineFunction2 declaration name'})
                script_edges.append((owner_id, identity, 'contains-script', code,
                                     tag_ordinal, tag_offset))
            if code in (72, 82):  # DoABCDefine / DoABC.
                abc_name = None
                abc_data = payload
                if code == 82 and len(payload) >= 5:
                    try:
                        abc_name, name_end = _cstring(payload, 4)
                        abc_data = payload[name_end:]
                    except (ValueError, UnicodeDecodeError):
                        abc_name = None
                identity = _node_id('swf-avm2-block', uri, code, tag_ordinal, tag_offset)
                nodes.setdefault(identity, {'id': identity,
                    'name': f'{uri}#tag{tag_ordinal}-abc@{tag_offset}',
                    'kind': 'swf-avm2-block', 'source': uri, 'tag_code': code,
                    'tag_ordinal': tag_ordinal, 'tag_offset': tag_offset,
                    'payload_offset': payload_offset, 'decoded_swf_offset': payload_offset,
                    'source_sha256': hashlib.sha256(raw).hexdigest(),
                    'byte_count': len(payload), 'sha256': hashlib.sha256(payload).hexdigest(),
                    'abc_byte_count': len(abc_data), 'abc_sha256': hashlib.sha256(abc_data).hexdigest(),
                    'abc_name': abc_name, 'timeline_owner_character_id':
                    None if owner == 'timeline' else int(owner.split(':', 1)[1]),
                    'inventory_status': 'opaque-bytecode'})
                owner_id = None if owner == 'timeline' else int(owner.split(':', 1)[1])
                script_edges.append((owner_id, identity, 'contains-script', code,
                                     tag_ordinal, tag_offset))

            carrier_reason = None
            carrier_character_id = None
            if code in (7, 34):  # Button action payload grammar not decoded here.
                carrier_reason = 'Button action carrier retained opaque; action-record offset/conditions are not decoded.'
                carrier_character_id = struct.unpack_from('<H', payload, 0)[0] if len(payload) >= 2 else None
            elif code == 26 and payload and payload[0] & 0x80:
                carrier_reason = 'PlaceObject2 ClipActions carrier retained opaque; nested event-action offsets are not decoded.'
                if len(payload) >= 5 and payload[0] & 0x02:
                    carrier_character_id = struct.unpack_from('<H', payload, 3)[0]
            elif code == 70 and len(payload) >= 2 and payload[1] & 0x80:
                carrier_reason = 'PlaceObject3 ClipActions carrier retained opaque; nested event-action offsets are not decoded.'
                if len(payload) >= 6 and payload[0] & 0x02:
                    carrier_character_id = struct.unpack_from('<H', payload, 4)[0]
            if carrier_reason:
                identity = _node_id('swf-script-carrier', uri, code, tag_ordinal, tag_offset)
                nodes.setdefault(identity, {'id': identity,
                    'name': f'{uri}#tag{tag_ordinal}-carrier@{tag_offset}',
                    'kind': 'swf-script-carrier', 'source': uri, 'tag_code': code,
                    'tag_ordinal': tag_ordinal, 'tag_offset': tag_offset,
                    'decoded_swf_offset': payload_offset, 'byte_count': len(payload),
                    'source_sha256': hashlib.sha256(raw).hexdigest(),
                    'sha256': hashlib.sha256(payload).hexdigest(),
                    'timeline_owner_character_id': None if owner == 'timeline'
                    else int(owner.split(':', 1)[1]),
                    'character_id': carrier_character_id,
                    'inventory_status': 'opaque-carrier'})
                owner_id = None if owner == 'timeline' else int(owner.split(':', 1)[1])
                script_edges.append((owner_id, identity, 'contains-script-carrier', code,
                                     tag_ordinal, tag_offset))
                gaps.append({'source': uri, 'kind': 'swf-script-carrier',
                             'tag_ordinal': tag_ordinal, 'offset': tag_offset,
                             'reason': carrier_reason})
            try:
                if code == 4 and len(payload) >= 4:
                    placements.append((owner, struct.unpack_from('<H', payload, 2)[0], code))
                elif code == 26 and len(payload) >= 3 and payload[0] & 0x02:
                    if len(payload) < 5:
                        raise ValueError('truncated SWF PlaceObject2 CharacterID')
                    placements.append((owner, struct.unpack_from('<H', payload, 3)[0], code))
                elif code == 70 and len(payload) >= 4 and payload[0] & 0x02:
                    if len(payload) < 6:
                        raise ValueError('truncated SWF PlaceObject3 CharacterID')
                    placements.append((owner, struct.unpack_from('<H', payload, 4)[0], code))
                if code in _FONT_REFERENCE_TAGS:
                    if len(payload) < 2:
                        raise ValueError(f'truncated SWF font reference tag {code}')
                    font_id = struct.unpack_from('<H', payload, 0)[0]
                    placements.append((owner, font_id, code))
            except (ValueError, struct.error) as exc:
                gaps.append({'source': uri, 'kind': 'swf-reference-parse',
                             'tag_code': code, 'tag_ordinal': tag_ordinal,
                             'offset': tag_offset, 'reason': str(exc)})
            if code in (57, 71):  # ImportAssets/ImportAssets2.
                try:
                    if len(payload) < 3:
                        raise ValueError('truncated SWF import table')
                    if code == 71:
                        if len(payload) < 5:
                            raise ValueError('truncated SWF ImportAssets2 header')
                        at = 2  # Reserved UI16, then URLString and DownloadNow.
                        _, at = _cstring(payload, at)
                        if at >= len(payload):
                            raise ValueError('truncated SWF ImportAssets2 DownloadNow')
                        at += 1
                    else:
                        _, at = _cstring(payload, 0)
                    if at + 2 > len(payload):
                        raise ValueError('truncated SWF import count')
                    count = struct.unpack_from('<H', payload, at)[0]
                    at += 2
                    for _ in range(count):
                        if at + 2 > len(payload):
                            raise ValueError('truncated SWF imported CharacterID')
                        imported_id = struct.unpack_from('<H', payload, at)[0]
                        imported_name, at = _cstring(payload, at + 2)
                        imports.append((imported_id, imported_name))
                    if at != len(payload):
                        raise ValueError('SWF import table has trailing bytes')
                except (ValueError, UnicodeDecodeError, struct.error) as exc:
                    gaps.append({'source': uri, 'kind': 'swf-import-parse',
                                 'tag_code': code, 'tag_ordinal': tag_ordinal,
                                 'offset': tag_offset, 'reason': str(exc)})
            if code == 39:  # DefineSprite contains a nested timeline.
                try:
                    if len(payload) < 4:
                        raise ValueError('truncated SWF DefineSprite')
                    nested_id, frames = struct.unpack_from('<HH', payload, 0)
                    nested_tags = _tags(payload[4:], base_offset=payload_offset + 4)
                    visit(nested_tags, f'sprite:{nested_id}')
                except (ValueError, struct.error) as exc:
                    gaps.append({'source': uri, 'kind': 'swf-nested-timeline-parse',
                                 'tag_code': code, 'tag_ordinal': tag_ordinal,
                                 'offset': tag_offset, 'reason': str(exc)})
            if code == 1:  # ShowFrame starts the following timeline frame.
                frame_index += 1
    visit(top)

    for owner_id, target_id, edge_kind, code, ordinal, offset in script_edges:
        source_id = movie_id
        if owner_id is not None:
            source_id = symbols.get(owner_id) or _node(
                nodes, 'swf-symbol', f'{uri}#{owner_id}', uri,
                id=_node_id('swf-symbol', uri, owner_id), character_id=owner_id)
            symbols[owner_id] = source_id
        _edge(edges, source_id, target_id, edge_kind,
              f'SWF tag {code}; ordinal {ordinal}; decoded offset {offset}')

    for character_id, name in exports:
        target = symbols.get(character_id)
        if not target:
            target = _node(nodes, 'swf-symbol', f'{uri}#{character_id}', uri,
                           id=_node_id('swf-symbol', uri, character_id), character_id=character_id)
            symbols[character_id] = target
        aliases.append({'source_id': target, 'alias': name, 'kind': 'swf-export',
                        'namespace': uri, 'evidence': 'ExportAssets/SymbolClass CharacterID association'})
        _edge(edges, movie_id, target, 'exports', name)
    for owner, character_id, code in placements:
        target = symbols.get(character_id)
        if not target:
            gaps.append({'source': uri, 'kind': 'swf-symbol-reference', 'character_id': character_id,
                         'tag_code': code, 'reason': 'Referenced CharacterID has no definition/export in this SWF.'})
            continue
        source_id = movie_id if owner == 'timeline' else symbols.get(int(owner.split(':', 1)[1]), movie_id)
        _edge(edges, source_id, target, 'font-reference' if code in _FONT_REFERENCE_TAGS else 'places-character',
              f'SWF tag {code}; CharacterID {character_id}')
    for imported_id, name in imports:
        target = _node(nodes, 'swf-import', f'{uri}#{imported_id}', uri,
                       id=_node_id('swf-import', uri, imported_id), character_id=imported_id,
                       imported_name=name)
        _edge(edges, movie_id, target, 'imports', 'ImportAssets name retained without path inference')
        gaps.append({'source': uri, 'kind': 'external-swf-import', 'name': name,
                     'reason': 'SWF import name is retained; no exact indexed resource URI match was proven.'})
    return {'version': version, 'tags': len(top), 'symbols': len(symbols), 'exports': len(exports),
            'imports': len(imports)}


def _decode_pycst(raw: bytes) -> dict[str, dict[str, int]]:
    at = 0
    def word():
        nonlocal at
        if at + 4 > len(raw):
            raise ValueError('truncated PyDataConstants word')
        value = struct.unpack_from('<I', raw, at)[0]
        at += 4
        return value
    def name():
        nonlocal at
        size = word()
        if size > 255 or size > len(raw) - at:
            raise ValueError('PyDataConstants name outside source reader bound')
        value = raw[at:at + size].split(b'\0', 1)[0].decode('utf-8', 'strict')
        at += size
        return value
    groups = {}
    count = word()
    if count > 65536:
        raise ValueError('PyDataConstants group count outside bound')
    for _ in range(count):
        group = name()
        entries = word()
        if entries > 100000:
            raise ValueError('PyDataConstants entry count outside bound')
        values = groups.setdefault(group, {})
        for _ in range(entries):
            key, value = name(), struct.unpack('<i', struct.pack('<I', word()))[0]
            values[key] = value
    if at != len(raw):
        raise ValueError('PyDataConstants has trailing bytes')
    return groups


def _read_locale(raw: bytes) -> list[str]:
    if len(raw) < 2:
        raise ValueError('truncated localization count')
    count = struct.unpack_from('<H', raw, 0)[0]
    if count > 32767:
        raise ValueError('localization count exceeds source signed16 domain')
    at, values = 2, []
    for _ in range(count):
        if at + 2 > len(raw):
            raise ValueError('truncated localization string length')
        size = struct.unpack_from('<H', raw, at)[0]
        at += 2
        if size > 1024 * 1024 or size > len(raw) - at:
            raise ValueError('localization text exceeds source bounds')
        text = raw[at:at + size]
        if b'\0' in text:
            raise ValueError('localization string has embedded NUL')
        values.append(text.decode('utf-8', 'strict'))
        at += size
    if at != len(raw):
        raise ValueError('localization file has trailing bytes')
    return values


def _common_text_sheets_bytes(raw: bytes) -> list[list[dict]]:
    at = 0
    def word():
        nonlocal at
        if at + 4 > len(raw):
            raise ValueError('truncated common_text PyData')
        value = struct.unpack_from('<I', raw, at)[0]
        at += 4
        return value
    def text():
        nonlocal at
        size = word()
        if size > 1 << 20 or size > len(raw) - at:
            raise ValueError('common_text filename outside bounds')
        value = raw[at:at + size].decode('utf-8', 'strict')
        at += size
        return value
    packs = word()
    if packs != 9:
        raise ValueError(f'expected nine common_text packs, found {packs}')
    result = []
    for _ in range(packs):
        sheets = word()
        if sheets != 37:
            raise ValueError(f'expected 37 common_text sheets, found {sheets}')
        result.append([{'filename': text(), 'name': text()} for _ in range(sheets)])
    if at != len(raw):
        raise ValueError('common_text PyData has trailing bytes')
    return result


def _uid_alias(uid, table_id, table_node_id, table_name, field, sheets, symbol_records, english_records,
               shifts, nodes, edges, aliases, gaps):
    if not isinstance(uid, int) or uid < 0 or uid > 0xffffffff:
        return
    # The signed Item/Skill source fields use -1 for absent text; the atlas
    # table projection may surface the same word as unsigned 0xffffffff.
    if uid == 0xffffffff:
        return
    pack_shift, pack_mask, string_shift, string_mask = shifts
    sheet, ordinal = (uid >> pack_shift) & pack_mask, (uid >> string_shift) & string_mask
    if not sheets or sheet >= len(sheets[0]):
        gaps.append({'source': table_id, 'kind': 'localization-uid', 'uid': uid,
                     'reason': f'StringConfig decodes sheet {sheet}, outside the 37 authored sheets.'})
        return
    sheet_meta = sheets[0][sheet]  # CommonText pack order; IDs encode sheet, not language.
    stem = re.sub(r'\.(?:english|symbols)$', '', Path(sheet_meta['filename']).name, flags=re.IGNORECASE)
    source = f'data/text/{stem}.symbols'
    symbols = symbol_records.get(source)
    if symbols is None or ordinal >= len(symbols):
        gaps.append({'source': table_id, 'kind': 'localization-uid', 'uid': uid,
                     'reason': f'No symbol row at exact sheet={sheet}, ordinal={ordinal} in {source}.'})
        return
    symbol = symbols[ordinal]
    language = english_records.get(f'data/text/{stem}.english')
    if language is None or ordinal >= len(language):
        gaps.append({'source': table_id, 'kind': 'localization-uid', 'uid': uid,
                     'reason': f'English text row absent at exact sheet={sheet}, ordinal={ordinal}.'})
        return
    text = language[ordinal]
    source_id = _node_id('localization-row', source, ordinal)
    _node(nodes, 'localization-row', text, source, id=source_id, ordinal=ordinal,
          symbol=symbol, language='english')
    _edge(edges, table_node_id, source_id, 'localizes-field', f'{table_name}.{field}; UID {uid}')
    aliases.append({'source_id': source_id, 'table_id': table_id, 'table_name': table_name,
                    'table_node_id': table_node_id, 'table_field': field, 'alias': symbol, 'kind': field,
                    'namespace': source, 'uid': uid, 'sheet': sheet, 'ordinal': ordinal,
                    'human_text': text,
                    'evidence': ('StringConfig packed-ID decode + common_text sheet metadata + '
                                 'same-ordinal symbols/english rows; CharacterProperties.Name is the '
                                 'name ID consumed by the eligible-target enemy HUD StringManager path')
                    if field == 'actor-name' else
                    'StringConfig packed-ID decode + common_text sheet metadata + same-ordinal symbols/english rows'})
    if text:
        aliases.append({'source_id': source_id, 'table_id': table_id, 'table_node_id': table_node_id,
                        'table_name': table_name, 'table_field': field, 'alias': text, 'kind': f'{field}-english',
                        'namespace': source, 'uid': uid, 'sheet': sheet, 'ordinal': ordinal,
                        'human_text': text,
                        'evidence': 'Exact same-ordinal English row in indexed locale resource'})


def build(ctx, assets, tables):
    """Build typed UI/text nodes and exact source-backed aliases without writes."""
    cache = _cache_root(ctx, assets)
    indexes = {name: _manifest(cache / name) for name in
               ('index.json', 'text-index.json', 'texture-index.json')}
    if len({manifest.get('cache_sha256') for manifest in indexes.values()}) != 1:
        raise ValueError('adjacent original-cache indexes identify different cache snapshots')
    nodes, edges, aliases, gaps = {}, {}, [], []
    swf_reports, records = {}, {}

    # Resource inventory: provenance and paths come from the adjacent source indexes.
    resource_records = {}
    for index_name, manifest in indexes.items():
        for uri, record in manifest['resources'].items():
            resource_records.setdefault(uri, record)
    for uri, record in sorted(resource_records.items()):
        path = _resource_path(cache, record)
        suffix = path.suffix.casefold()
        if uri.startswith('data/text/'):
            kind = 'localization-file'
        elif suffix == '.swf':
            kind = 'swf-movie'
        elif suffix in _FONT_EXTENSIONS:
            kind = 'font-resource'
        elif uri in indexes['texture-index.json']['resources']:
            kind = 'texture-resource'
        else:
            continue
        identity = _node_id(kind, uri)
        _node(nodes, kind, uri, uri, id=identity, bytes=record.get('bytes'),
              sha256=record.get('sha256'), indexed_asset=record.get('asset'))
        try:
            raw = _verified_bytes(cache, record)
        except Exception as exc:
            gaps.append({'source': uri, 'kind': kind, 'reason': str(exc)})
            continue
        if kind == 'localization-file':
            try:
                records[uri] = _read_locale(raw)
            except Exception as exc:
                gaps.append({'source': uri, 'kind': kind, 'reason': str(exc)})
        elif kind == 'swf-movie':
            try:
                swf_reports[uri] = _parse_swf(raw, uri, nodes, edges, aliases, gaps)
            except Exception as exc:
                gaps.append({'source': uri, 'kind': 'swf-parse', 'reason': str(exc)})

    # The iPad package reader validates the exact packed resource map and
    # decodes only established first-four-byte SWF recipes in memory. Keep
    # these resources under a distinct iOS namespace even when their logical
    # path matches an Android movie.
    ios_swfs = 0
    try:
        import package_index
        ios_assets = [row for row in assets or () if row.get('source') == 'ios-packed']
        # The generator has validated the inventory already. Every selected
        # SWF is still read and hash-checked below before it is parsed.
        required=('format','package_relative','logical_paths','decoded_sha256','package_version')
        updates=(ios_assets if all(all(key in a for key in required) for a in ios_assets)
                 else package_index.enrich(ctx, ios_assets))
        assets_by_path = {row.get('path'): row for row in ios_assets}
        for update in updates:
            if not str(update.get('format', '')).startswith('SWF'):
                continue
            if not update.get('logical_paths'):
                gaps.append({'source': str(update.get('path')), 'kind': 'ios-swf-package',
                             'reason': 'Validated SWF resource has no exact logical path alias.'})
                continue
            logicals = [value for value in update['logical_paths']
                        if str(value).endswith('.swf')]
            if len(logicals) != 1:
                gaps.append({'source': str(update.get('path')), 'kind': 'ios-swf-package',
                             'reason': 'Expected one exact indexed menu SWF logical path.'})
                continue
            asset = assets_by_path.get(update.get('path'))
            if asset is None:
                continue
            try:
                raw = package_index.read_resource(ctx, {**asset, **update})
                if hashlib.sha256(raw).hexdigest() != update.get('decoded_sha256'):
                    raise ValueError('decoded iOS SWF SHA-256 differs from validated package receipt')
                uri = 'ios7/' + logicals[0]
                swf_reports[uri] = _parse_swf(raw, uri, nodes, edges, aliases, gaps)
                ios_swfs += 1
                for node in nodes.values():
                    if node.get('source') == uri:
                        node['package_version'] = update.get('package_version')
                        node['package_path'] = update.get('path')
                        node['package_container_sha256'] = update.get('sha256')
                        node['package_decoded_sha256'] = update.get('decoded_sha256')
                        node['package_mapping_confidence'] = update.get('mapping_confidence')
                        node['package_decode_evidence'] = (update.get('decode_crosscheck')
                            or 'Embedded iPad map plus package_index FWS size/tag validation; own decoded SHA-256 retained.')
            except Exception as exc:
                gaps.append({'source': str(update.get('path')), 'kind': 'ios-swf-parse',
                             'logical_path': logicals[0], 'reason': str(exc)})
    except Exception as exc:
        gaps.append({'source': 'ios-package', 'kind': 'ios-swf-inventory', 'reason': str(exc)})

    # Native font files are nodes; their use by SWF character definitions is
    # represented by each movie's DefineFont character IDs above.
    for uri, record in sorted(indexes['texture-index.json']['resources'].items()):
        identity = _node_id('texture-resource', uri)
        _node(nodes, 'texture-resource', uri, uri, id=identity,
              bytes=record.get('bytes'), sha256=record.get('sha256'),
              indexed_asset=record.get('asset'))
        try:
            _verified_bytes(cache, record)
        except Exception as exc:
            gaps.append({'source': uri, 'kind': 'texture-resource', 'reason': str(exc)})

    # Source StringManager ID decoding uses the same constants and authored
    # sheet table as HudTextV1::id and Localization::id.
    try:
        constants_record = resource_records['data/pydata/common_text_pycst.bin']
        sheets_record = resource_records['data/pydata/common_text_pyarray.bin']
        constants = _decode_pycst(_verified_bytes(cache, constants_record))
        config = constants['StringConfig']
        shifts = (config['PackIDShift'], config['PackIDMask'],
                  config['StrIDShift'], config['StrIDMask'])
        if any(not isinstance(value, int) or value < 0 for value in shifts):
            raise ValueError('negative StringConfig shift/mask')
        sheets = _common_text_sheets_bytes(_verified_bytes(cache, sheets_record))
    except Exception as exc:
        sheets, shifts = [], None
        gaps.append({'source': 'data/pydata/common_text_pycst.bin', 'kind': 'localization-id-map',
                     'reason': str(exc)})

    symbols, english = {}, {}
    for uri, strings in records.items():
        if uri.endswith('.symbols'):
            symbols[uri] = strings
        elif uri.endswith('.english'):
            english[uri] = strings

    # Every original locale and symbol row remains addressable, including
    # empty and currently unreferenced strings. The row identity is scoped to
    # its exact source URI and ordinal; equal text/UIDs never merge namespaces.
    for uri, strings in sorted(records.items()):
        suffix = PurePosixPath(uri).suffix.lstrip('.').casefold()
        language = suffix if suffix and suffix != 'symbols' else None
        stem = re.sub(r'\.[^.]+$', '', uri)
        symbol_rows = records.get(stem + '.symbols', ())
        english_rows = records.get(stem + '.english', ())
        for ordinal, value in enumerate(strings):
            identity = _node_id('localization-row', uri, ordinal)
            extra = {'ordinal': ordinal, 'language': language, 'text': value}
            if uri.endswith('.symbols'):
                extra['symbol'] = value
                if ordinal < len(english_rows):
                    extra['english_text'] = english_rows[ordinal]
            elif uri.endswith('.english') and ordinal < len(symbol_rows):
                extra['symbol'] = symbol_rows[ordinal]
            _node(nodes, 'localization-row', value, uri, id=identity, **extra)
            if uri.endswith('.symbols') and value:
                aliases.append({'source_id': identity, 'alias': value,
                                'kind': 'localization-symbol', 'namespace': uri,
                                'ordinal': ordinal, 'human_text': english_rows[ordinal]
                                if ordinal < len(english_rows) else None,
                                'evidence': 'Exact symbol row in indexed source resource'})
    if shifts is not None:
        for row in tables or ():
            table = row.get('table')
            table_id = row.get('id')
            if not table_id or table not in _TABLE_ROW_TABLES:
                continue
            table_node_id = f'table-row:{table_id}'
            table_name = str(row.get('name') or '')
            _node(nodes, 'table-row', table_name, str(row.get('source_file') or table),
                  id=table_node_id, table=table, index=row.get('index'), table_id=table_id)
            fields = row.get('fields') or {}
            if isinstance(fields.get('fields'), dict):
                fields = fields['fields']
            for field, alias_kind in _STRING_TABLES.items():
                if table != field[0] or field[1] not in fields:
                    continue
                _uid_alias(fields[field[1]], table_id, table_node_id, table_name, alias_kind,
                           sheets, symbols, english, shifts, nodes, edges, aliases, gaps)
            # Source table identifiers remain searchable without claiming
            # that their internal names are player-facing English text.
            if row.get('name'):
                aliases.append({'source_id': table_node_id, 'table_id': table_id,
                                'alias': table_name,
                                'kind': 'table-identifier', 'namespace': table,
                                'table_name': str(row['name']),
                                'evidence': 'Exact source PyData row name'})

    coverage = {
        'resources_by_kind': dict(sorted(Counter(n['kind'] for n in nodes.values()).items())),
        'swf_movies': len(swf_reports), 'swf_reports': swf_reports,
        'ios7_swf_movies': ios_swfs,
        'script_inventory': {
            'avm1_action_blocks': sum(n['kind'] == 'swf-action-block' for n in nodes.values()),
            'avm1_by_tag': dict(sorted(Counter(str(n['tag_code']) for n in nodes.values()
                                                if n['kind'] == 'swf-action-block').items())),
            'avm1_opcode_indexed': sum(n['kind'] == 'swf-action-block'
                                       and n.get('inventory_status') == 'indexed-opcodes'
                                       for n in nodes.values()),
            'avm1_malformed_opaque': sum(n['kind'] == 'swf-action-block'
                                         and n.get('inventory_status') == 'malformed-opaque'
                                         for n in nodes.values()),
            'avm1_records': sum(n.get('record_count') or 0 for n in nodes.values()
                                if n['kind'] == 'swf-action-block'),
            'avm2_bytecode_blocks': sum(n['kind'] == 'swf-avm2-block' for n in nodes.values()),
            'avm2_by_tag': dict(sorted(Counter(str(n['tag_code']) for n in nodes.values()
                                                if n['kind'] == 'swf-avm2-block').items())),
            'opaque_script_carriers': sum(n['kind'] == 'swf-script-carrier' for n in nodes.values()),
            'opaque_carriers_by_tag': dict(sorted(Counter(str(n['tag_code']) for n in nodes.values()
                                                           if n['kind'] == 'swf-script-carrier').items())),
        },
        'localization_files': sum(uri.startswith('data/text/') for uri in records),
        'localization_rows': sum(len(values) for values in records.values()),
        'aliases': len(aliases), 'gaps': len(gaps),
        'source_consumer': ('HudTextV1::id decodes StringConfig then indexes common_text sheet/ordinal; '
                            'InfoHUDManager FastUpdate 41e064 reads the eligible target name ID through '
                            'StringManager and writes enemy.enemy_name.text; Faery page text is authored '
                            'GAMEPLAYMENUS_FAERY_* symbol lookup, so Faery table Name/Description are '
                            'retained as rows but not asserted as that page display source.')
    }
    return {'nodes': sorted(nodes.values(), key=lambda row: row['id']),
            'edges': sorted(edges.values(), key=lambda row: row['id']),
            'aliases': sorted(aliases, key=lambda row: (row['namespace'], row['alias'], row['source_id'])),
            'coverage': coverage, 'gaps': gaps}
