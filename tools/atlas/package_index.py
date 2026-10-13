"""Read-only inventory and conservative decoder for the iPad 1.0.0 data packs.

The iOS package's `fileNNNNNN.dat` names are transport names.  The package's
plantext map associates them with logical game paths. Those associations are
useful lookup aliases, but are marked LIKELY: this repository has no recovered
iOS loader code proving how the shipped runtime admits every mapped path.

No source archive is modified and no extracted payload is written by this
module. `read_resource` returns bytes in memory. It applies the observed
first-four-byte offset only when the expected type validates successfully.
"""
from __future__ import annotations

import csv
import hashlib
import io
import json
import posixpath
from pathlib import Path, PurePosixPath
import re
import struct
import zipfile
import xml.etree.ElementTree as ET

_IPA_REL = Path('claude-preview15/ipa')
_APP_REL = Path('Payload/dungeonhunter2iPad.app')
_DATA_REL = Path('data')
_MAPPING = 'file00000-plantext.dat'
_SOUNDS_ZIP_LOGICAL = 'data/sounds/sounds.zip'
_SOUNDS_ZIP_FIRST = b'QM\x06\x08'
_BRES_IOS_MAGIC = b'CTHW'
_BRES_MAGIC = b'BRES'
_HEADER_OFFSETS = (1, 2, 3, 4)
_XML_SUFFIXES = ('.xml', '.lightset_xml', '.mlx', '.mgp', '.mvp', '.mgx', '.mvx')
_NAME_STREAM_SUFFIXES = ('_pyarraynames.bin', '_pystructnames.bin')
_LUA_TOKEN = re.compile(r'(?im)(^\s*--|\b(function|local|return|if|then|end|include|require)\b|\w+\s*\()')
_LUA_SOURCE_START = re.compile(
    r'(?is)^\s*(?:--|(?:function|local|return|if|for|while|repeat|break|include|require|dofile)\b|'
    r'[A-Za-z_]\w*(?:\s*\.\s*[A-Za-z_]\w*)*\s*=)')


def _ipa_root(ctx) -> Path:
    """Resolve the original IPA evidence directory from the shared input root."""
    return Path(ctx.inputs) / _IPA_REL


def _app_root(ctx) -> Path:
    return _ipa_root(ctx) / 'extracted' / _APP_REL


def _data_root(ctx) -> Path:
    return _app_root(ctx) / _DATA_REL


def _android_pair_root(ctx) -> Path | None:
    """Return a retained Android source tree used only for exact byte checks."""
    candidates = (
        Path(ctx.inputs) / 'windows-source-clock-v19-preview-13/assets/original-cache/data',
        Path(ctx.inputs) / 'assets-extra/android/original-cache/data',
        Path(ctx.inputs) / 'assets-extra/android/data',
    )
    return next((path for path in candidates if path.is_dir()), None)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _staged_ios_receipts(ctx) -> dict[str, dict]:
    """Read the existing staged-iOS hash manifest as optional evidence only."""
    manifest_path = Path(ctx.inputs) / 'assets-extra/ios/MANIFEST.json'
    if not manifest_path.is_file():
        return {}
    manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
    receipts = {}
    for item in manifest.get('files', []):
        source_path = str(item.get('path', '')).replace('\\', '/')
        prefix = 'assets-extra/ios/'
        if source_path.startswith(prefix):
            receipts[source_path[len(prefix):]] = item
    return receipts


def _safe_relative(value: str) -> str:
    """Return a normalized package-relative path or reject traversal."""
    p = PurePosixPath(str(value).replace('\\', '/'))
    if p.is_absolute() or not p.parts or any(part in ('', '.', '..') for part in p.parts):
        raise ValueError(f'invalid package-relative path: {value!r}')
    return p.as_posix()


def _read_mapping(ctx) -> dict[str, str]:
    """Read the embedded pack map and cross-check the retained TSV transcription."""
    data = _data_root(ctx)
    plan = data / Path(*_MAPPING.split('/'))
    raw = plan.read_bytes()
    mapping: dict[str, str] = {}
    for line_no, line in enumerate(raw.splitlines(), 1):
        try:
            physical, logical = line.decode('utf-8').split(':', 1)
        except (UnicodeDecodeError, ValueError) as exc:
            raise ValueError(f'malformed embedded plantext line {line_no}') from exc
        physical = _safe_relative(physical)
        logical = _safe_relative(logical)
        if not re.fullmatch(r'data/file\d{6}\.dat', physical):
            raise ValueError(f'non-pack physical path at map line {line_no}: {physical}')
        if physical in mapping:
            raise ValueError(f'duplicate physical pack in embedded map: {physical}')
        mapping[physical] = logical

    # index_map.tsv is a convenient sorted transcription used by the original
    # IPA investigation scripts; require exact agreement with the in-package
    # map before using it as evidence.
    tsv = _ipa_root(ctx) / 'index_map.tsv'
    with tsv.open('r', encoding='utf-8', newline='') as stream:
        rows = list(csv.reader(stream, delimiter='\t'))
    transcribed = {}
    for row in rows:
        if len(row) != 2:
            raise ValueError('index_map.tsv row must contain exactly two columns')
        physical, logical = (_safe_relative(item) for item in row)
        if physical in transcribed:
            raise ValueError(f'duplicate physical pack in index_map.tsv: {physical}')
        transcribed[physical] = logical
    if mapping != transcribed:
        raise ValueError('index_map.tsv does not exactly match the embedded plantext map')
    return mapping


def _subtract_header(raw: bytes) -> bytes | None:
    """Undo the observed byte offsets on the first four package bytes only."""
    if len(raw) < 4:
        return None
    return bytes((raw[i] - _HEADER_OFFSETS[i]) & 0xff for i in range(4)) + raw[4:]


def _is_xml(raw: bytes) -> bool:
    if not raw.lstrip().startswith((b'<?xml', b'<')):
        return False
    try:
        ET.fromstring(raw)
    except (ET.ParseError, ValueError):
        return False
    return True


def _name_groups(raw: bytes) -> list[list[str]] | None:
    """Parse only bounded count/length groups from a names stream.

    The output deliberately carries no table IDs or group semantics.  The
    byte grammar mirrors the existing atlas names reader (u32 count followed
    by u32-length-prefixed Latin-1 strings), but this package reader validates
    complete consumption before accepting it.
    """
    if not raw:
        return None
    groups: list[list[str]] = []
    pos = 0
    total_names = 0
    while pos < len(raw):
        if pos + 4 > len(raw):
            return None
        count = struct.unpack_from('<I', raw, pos)[0]
        pos += 4
        if count > 100_000 or total_names + count > 1_000_000:
            return None
        names = []
        for _ in range(count):
            if pos + 4 > len(raw):
                return None
            size = struct.unpack_from('<I', raw, pos)[0]
            pos += 4
            if size > 100_000 or pos + size > len(raw):
                return None
            names.append(raw[pos:pos + size].decode('latin1').rstrip('\0'))
            pos += size
        total_names += count
        groups.append(names)
        if len(groups) > 100_000:
            return None
    return groups or None


def _lua_kind(raw: bytes) -> str | None:
    """Recognize strict UTF-8 Lua-like source or a coherent Lua 5.1 header."""
    if raw.startswith(b'\x1bLua'):
        if len(raw) < 12:
            return None
        version, fmt, endian, int_size, size_t, instruction, number, integral = raw[4:12]
        if (version == 0x51 and fmt == 0 and endian in (0, 1)
                and int_size == 4 and size_t in (4, 8) and instruction == 4
                and number in (4, 8) and integral in (0, 1)):
            return 'bytecode'
        return None
    if b'\x00' in raw:
        return None
    try:
        text = raw.decode('utf-8')
    except UnicodeDecodeError:
        return None
    return 'plaintext' if _LUA_SOURCE_START.search(text) and _LUA_TOKEN.search(text) else None


def _swf_valid(raw: bytes) -> bool:
    """Validate an uncompressed SWF header, declared size, and tag boundaries."""
    if len(raw) < 12 or raw[:3] != b'FWS' or not 1 <= raw[3] <= 50:
        return False
    if struct.unpack_from('<I', raw, 4)[0] != len(raw):
        return False
    nbits = raw[8] >> 3
    pos = 8 + (5 + 4 * nbits + 7) // 8 + 4  # RECT, frame rate, frame count
    if pos > len(raw):
        return False
    while pos + 2 <= len(raw):
        header = struct.unpack_from('<H', raw, pos)[0]
        pos += 2
        code, length = header >> 6, header & 0x3f
        if length == 0x3f:
            if pos + 4 > len(raw):
                return False
            length = struct.unpack_from('<I', raw, pos)[0]
            pos += 4
        if pos + length > len(raw):
            return False
        pos += length
        if code == 0:
            return length == 0 and pos == len(raw)
    return False


def _classify(logical_paths: list[str], relative: str, raw: bytes) -> tuple[str, str, str, bytes | None]:
    """Classify and optionally decode using expected-type validation gates."""
    logical = logical_paths[0] if logical_paths else ''
    suffix = PurePosixPath(logical or relative).suffix.lower()
    if logical == _SOUNDS_ZIP_LOGICAL:
        if raw[:4] == _SOUNDS_ZIP_FIRST:
            decoded = _subtract_header(raw)
            if decoded and decoded[:4] == b'PK\x03\x04':
                try:
                    with zipfile.ZipFile(io.BytesIO(decoded), 'r') as archive:
                        archive.infolist()
                    return ('ZIP archive (iOS four-byte header offset)',
                            'subtract-first-four-bytes(+1,+2,+3,+4):zip-header', 'LIKELY', decoded)
                except (OSError, zipfile.BadZipFile):
                    return 'sounds.zip with invalid ZIP directory', 'unresolved', 'UNRESOLVED', None
        if raw[:4] == b'PK\x03\x04':
            return 'ZIP archive', 'identity', 'DIRECT_SIGNATURE', raw
        return 'unknown bytes at sounds.zip logical path', 'unresolved', 'UNRESOLVED', None
    if suffix == '.bdae':
        if raw[:4] == _BRES_IOS_MAGIC and raw[4:8] == b'\xfe\xff\x00\x00':
            decoded = _subtract_header(raw)
            if decoded and decoded[:8] == b'BRES\xfe\xff\x00\x00':
                return 'BDAE/BRES (iOS four-byte header offset)', 'subtract-first-four-bytes(+1,+2,+3,+4):bres-header', 'LIKELY', decoded
            return 'BDAE extension; header failed expected BRES check', 'unresolved', 'UNRESOLVED', None
        if raw[:4] == _BRES_MAGIC and raw[4:8] == b'\xfe\xff\x00\x00':
            return 'BDAE/BRES', 'identity', 'DIRECT_SIGNATURE', raw
        return 'BDAE extension; unrecognized header', 'unresolved', 'UNRESOLVED', None
    if logical.lower().endswith(_NAME_STREAM_SUFFIXES):
        if _name_groups(raw) is not None:
            return ('PyData names-only count/length groups (group semantics unresolved)',
                    'identity:names-only-groups', 'CONTENT_VALIDATED', raw)
        decoded = _subtract_header(raw)
        if decoded is not None and _name_groups(decoded) is not None:
            return ('PyData names-only count/length groups (first four bytes offset; group semantics unresolved)',
                    'subtract-first-four-bytes(+1,+2,+3,+4):names-only-groups', 'LIKELY', decoded)
        return 'PyData names stream; bounded group parse failed', 'preserve-raw', 'UNRESOLVED', None
    if suffix in _XML_SUFFIXES:
        if _is_xml(raw):
            return 'XML text', 'identity', 'DIRECT_SIGNATURE', raw
        decoded = _subtract_header(raw)
        if decoded and _is_xml(decoded):
            return ('XML text (first four bytes offset; XML parse valid)',
                    'subtract-first-four-bytes(+1,+2,+3,+4):xml-et-parse', 'LIKELY', decoded)
        return 'XML extension; XML parse failed', 'preserve-raw', 'UNRESOLVED', None
    if suffix == '.luac':
        direct = _lua_kind(raw)
        if direct:
            return f'Lua {direct} (direct signature/content)', 'identity', 'DIRECT_SIGNATURE', raw
        decoded = _subtract_header(raw)
        kind = _lua_kind(decoded) if decoded else None
        if kind:
            return (f'Lua {kind} (first four bytes offset; payload validated)',
                    'subtract-first-four-bytes(+1,+2,+3,+4):lua-content-validate', 'LIKELY', decoded)
        return 'Lua extension; text/bytecode validation failed', 'preserve-raw', 'UNRESOLVED', None
    if suffix == '.swf':
        if _swf_valid(raw):
            return f'SWF FWS version {raw[3]} (direct signature/tags valid)', 'identity', 'DIRECT_SIGNATURE', raw
        decoded = _subtract_header(raw)
        if decoded and _swf_valid(decoded):
            return (f'SWF FWS version {decoded[3]} (first four bytes offset; size/tags valid)',
                    'subtract-first-four-bytes(+1,+2,+3,+4):swf-structure-validate', 'LIKELY', decoded)
        return 'SWF extension; header/size/tag validation failed', 'preserve-raw', 'UNRESOLVED', None
    if suffix in ('.ttf',) and raw[:4] in (b'\x00\x01\x00\x00', b'OTTO'):
        return 'TrueType/OpenType font', 'identity', 'DIRECT_SIGNATURE', raw
    if suffix:
        return f'{suffix[1:].upper()} payload (raw; decoder not established)', 'preserve-raw', 'UNRESOLVED', None
    return 'unknown raw package data', 'preserve-raw', 'UNRESOLVED', None


def _inventory_rows(ctx) -> tuple[list[dict], dict[str, str]]:
    data = _data_root(ctx)
    if not data.is_dir():
        raise FileNotFoundError(f'iPad package data directory not found: {data}')
    mapping = _read_mapping(ctx)
    by_physical = mapping
    by_logical = {logical: physical for physical, logical in mapping.items()}
    if len(by_logical) != len(mapping):
        raise ValueError('duplicate logical path in iPad plantext map')

    mapped_files = {data / Path(*physical.split('/')[1:]) for physical in by_physical}
    missing = sorted(str(p.relative_to(data)).replace('\\', '/') for p in mapped_files if not p.is_file())
    if missing:
        raise FileNotFoundError(f'{len(missing)} mapped iOS pack files are missing; first: {missing[0]}')

    rows = []
    staged_receipts = _staged_ios_receipts(ctx)
    android_root = _android_pair_root(ctx)
    for file in sorted(data.rglob('*')):
        if not file.is_file():
            continue
        relative = file.relative_to(data).as_posix()
        physical = f'data/{relative}'
        logical = by_physical.get(physical)
        raw = file.read_bytes()
        logical_paths = [logical] if logical else []
        fmt, recipe, decode_confidence, decoded = _classify(logical_paths, relative, raw)
        name_groups = (_name_groups(decoded) if decoded is not None
                       and logical and logical.lower().endswith(_NAME_STREAM_SUFFIXES) else None)
        decode_crosscheck = None
        if decoded is not None and logical:
            receipt = staged_receipts.get(logical)
            if (receipt and receipt.get('sha256') == _sha(decoded)
                    and receipt.get('bytes') == len(decoded)):
                decode_confidence = 'CROSSCHECKED'
                decode_crosscheck = 'Exact decoded SHA-256 and byte-count match to assets-extra/ios/MANIFEST.json.'
        if decoded is not None and logical and android_root is not None:
            android_path = android_root / Path(*logical.split('/')[1:])
            if android_path.is_file() and android_path.stat().st_size == len(decoded):
                if _sha(android_path.read_bytes()) == _sha(decoded):
                    decode_confidence = 'CROSSCHECKED'
                    decode_crosscheck = 'Exact decoded SHA-256 match: ' + android_path.relative_to(Path(ctx.inputs)).as_posix()
        source_path = '.local-inputs/' + (_IPA_REL / 'extracted' / _APP_REL / _DATA_REL).as_posix() + '/' + relative
        aliases = []
        if logical:
            aliases.append({
                'logical_path': logical,
                'confidence': 'LIKELY',
                'evidence': 'Embedded iPad file00000-plantext.dat, cross-checked byte-for-byte with index_map.tsv; runtime path admission not recovered.',
                'package_version': 'Dungeon Hunter 2 HD iPad CFBundleVersion 1.0.0',
            })
        rows.append({
            'path': source_path,
            'source': 'ios-package',
            'relative': relative,
            'package_relative': f'data/{relative}',
            'logical_paths': logical_paths,
            'metadata_aliases': aliases,
            'mapping_confidence': 'LIKELY' if logical else 'UNMAPPED',
            'format': fmt,
            'decode_recipe': recipe,
            'decode_confidence': decode_confidence,
            'decode_crosscheck': decode_crosscheck,
            'bytes': len(raw),
            'sha256': _sha(raw),
            'decoded_bytes': len(decoded) if decoded is not None else None,
            'decoded_sha256': _sha(decoded) if decoded is not None else None,
            'name_group_count': len(name_groups) if name_groups is not None else None,
            'name_count': sum(map(len, name_groups)) if name_groups is not None else None,
            'decoded_semantics': ('names-only; group and ID meaning unresolved'
                                  if name_groups is not None else None),
            'raw_magic_hex': raw[:8].hex(),
            'package_version': 'Dungeon Hunter 2 HD iPad CFBundleVersion 1.0.0',
        })
    return rows, by_logical


def inventory(ctx) -> list[dict]:
    """Return a SHA-256 inventory for all files in app/data, with map aliases.

    The embedded plantext map covers 6,145 numbered packs.  Additional files
    such as fonts and loose sounds catalogs remain present as source-file rows;
    an unmapped numbered pack is retained with no invented logical name.
    """
    return _inventory_rows(ctx)[0]


def enrich(ctx, assets: list[dict]) -> list[dict]:
    """Return metadata updates for app/data assets already present in `assets`.

    Updates are keyed by source path and carry the logical aliases, raw hash,
    format, iPad build identity, and exact decode recipe.  The caller decides
    how these updates merge into its shared asset schema.
    """
    package_rows, _ = _inventory_rows(ctx)
    package_by_relative = {row['relative'].casefold(): row for row in package_rows}
    updates = []
    for asset in assets:
        if not isinstance(asset, dict):
            continue
        relative = asset.get('relative')
        source = str(asset.get('source', '')).lower()
        package_relative = asset.get('package_relative')
        if package_relative:
            rel = _safe_relative(package_relative)
            if rel.startswith('data/'):
                rel = rel[5:]
        elif source in ('ios-package', 'ios_package', 'ios') and relative:
            rel = _safe_relative(str(relative))
            if rel.startswith('data/'):
                rel = rel[5:]
        else:
            # A passed iOS path can be recognized without interpreting any
            # other source's basename as a package member.
            path = str(asset.get('path', '')).replace('\\', '/')
            marker = '/dungeonhunter2iPad.app/data/'
            if marker not in path:
                continue
            rel = _safe_relative(path.split(marker, 1)[1])
        row = package_by_relative.get(rel.casefold())
        if not row:
            continue
        updates.append({key: row[key] for key in (
            'path', 'logical_paths', 'metadata_aliases', 'mapping_confidence',
            'format', 'decode_recipe', 'decode_confidence', 'decode_crosscheck', 'bytes', 'sha256',
            'decoded_bytes', 'decoded_sha256', 'raw_magic_hex',
            'name_group_count', 'name_count', 'decoded_semantics',
            'package_version', 'package_relative')})
    return updates


def _source_file(ctx, asset: dict) -> Path:
    """Resolve an inventory row to a package file while enforcing data-root scope."""
    package_relative = asset.get('package_relative')
    if not package_relative:
        path = str(asset.get('path', '')).replace('\\', '/')
        marker = '/dungeonhunter2iPad.app/data/'
        if marker not in path:
            raise ValueError('asset does not identify an iPad app/data resource')
        package_relative = 'data/' + path.split(marker, 1)[1]
    package_relative = _safe_relative(package_relative)
    if not package_relative.startswith('data/'):
        raise ValueError('resource path must remain under app/data')
    target = (_app_root(ctx) / Path(*package_relative.split('/'))).resolve()
    root = _data_root(ctx).resolve()
    if target != root and root not in target.parents:
        raise ValueError('resource path escapes app/data')
    if not target.is_file():
        raise FileNotFoundError(target)
    logical_paths = list(asset.get('logical_paths') or ())
    if logical_paths:
        package_key = PurePosixPath(*package_relative.split('/')[1:]).as_posix()
        physical_key = f'data/{package_key}'
        expected = _read_mapping(ctx).get(physical_key)
        if expected is None or logical_paths != [expected]:
            raise ValueError('asset logical alias does not match the embedded iOS pack map')
    return target


def _paired_source_audit(ctx, mapping: dict[str, str]) -> dict:
    """Compare available Android source pairs without treating content as equal by name."""
    root = _android_pair_root(ctx)
    if root is None:
        return {'available': False, 'pairs_found': 0}
    ios_data = _data_root(ctx)
    result = {
        'available': True,
        'source_root': root.relative_to(Path(ctx.inputs)).as_posix(),
        'pairs_found': 0,
        'same_size_pairs': 0,
        'exact_source_bytes': 0,
        'offset_header_and_body_equal': 0,
        'offset_header_body_differs': 0,
        'other_same_size_pairs': 0,
        'different_size_pairs': 0,
    }
    for physical, logical in mapping.items():
        ios_path = ios_data / Path(*physical.split('/')[1:])
        android_path = root / Path(*logical.split('/')[1:])
        if not ios_path.is_file() or not android_path.is_file():
            continue
        result['pairs_found'] += 1
        ios_raw = ios_path.read_bytes()
        android_raw = android_path.read_bytes()
        if len(ios_raw) != len(android_raw):
            result['different_size_pairs'] += 1
            continue
        result['same_size_pairs'] += 1
        if ios_raw == android_raw:
            result['exact_source_bytes'] += 1
        elif (_subtract_header(ios_raw) or b'')[:4] == android_raw[:4]:
            if ios_raw[4:] == android_raw[4:]:
                result['offset_header_and_body_equal'] += 1
            else:
                result['offset_header_body_differs'] += 1
        else:
            result['other_same_size_pairs'] += 1
    return result


def read_resource(ctx, asset: dict, member: str | None = None) -> bytes:
    """Return source bytes in memory, applying only established exact recipes.

    `member` is supported for the documented `data/sounds/sounds.zip` pack.
    It returns a member's original bytes without writing an unpacked copy.
    """
    path = _source_file(ctx, asset)
    raw = path.read_bytes()
    logical = set(asset.get('logical_paths') or ())
    package_relative = str(asset.get('package_relative') or '').replace('\\', '/')
    if _SOUNDS_ZIP_LOGICAL in logical or package_relative == 'data/file000787.dat':
        logical.add(_SOUNDS_ZIP_LOGICAL)
    relative = str(asset.get('relative') or Path(package_relative).name)
    fmt, recipe, confidence, decoded = _classify(sorted(logical), relative, raw)
    if decoded is None:
        raise ValueError(f'resource decoder declined payload ({recipe}; {confidence})')
    if member is not None:
        if _SOUNDS_ZIP_LOGICAL not in logical:
            raise ValueError('member selection is supported only for the identified sounds.zip resource')
        requested = _safe_relative(member)
        with zipfile.ZipFile(io.BytesIO(decoded), 'r') as archive:
            matches = [item for item in archive.infolist()
                       if item.filename.replace('\\', '/') == requested
                       or (len(PurePosixPath(requested).parts) == 1
                           and posixpath.basename(item.filename.replace('\\', '/')) == requested)]
            if len(matches) != 1:
                raise KeyError(f'expected exactly one sounds.zip member named {requested!r}, found {len(matches)}')
            return archive.read(matches[0])
    return decoded


def read_name_groups(ctx, asset: dict) -> list[list[str]]:
    """Return validated name strings by stored group, without assigning IDs."""
    logical_paths = list(asset.get('logical_paths') or ())
    if not any(path.lower().endswith(_NAME_STREAM_SUFFIXES) for path in logical_paths):
        raise ValueError('name-group reader supports only the two PyData names-stream suffixes')
    decoded = read_resource(ctx, asset)
    groups = _name_groups(decoded)
    if groups is None:
        raise ValueError('bounded PyData names-group parser declined the payload')
    return groups


def validate(ctx) -> dict:
    """Validate the map, bounded decoders, Android source pairs and WAV receipts."""
    rows, by_logical = _inventory_rows(ctx)
    mapped = [row for row in rows if row['logical_paths']]
    numbered = [row for row in rows if re.fullmatch(r'file\d{6}\.dat', row['relative'])]
    if len(mapped) != 6145 or len(by_logical) != 6145:
        raise AssertionError(f'expected 6145 unique mapped logical resources, got {len(mapped)}')
    if len(numbered) != 6146:
        raise AssertionError(f'expected 6146 numbered data packs including unmapped file000000.dat, got {len(numbered)}')
    unknown = [row for row in numbered if not row['logical_paths']]
    if [row['relative'] for row in unknown] != ['file000000.dat']:
        raise AssertionError('unmapped numbered pack set changed; refusing to invent aliases')

    decoder_counts = {}
    for suffix in ('.xml', '.lightset_xml', '.mlx', '.mgp', '.mvp', '.mgx', '.mvx', '.luac', '.swf', '.bin'):
        selected_rows = [row for row in mapped if row['logical_paths'][0].lower().endswith(suffix)]
        decoded_rows = [row for row in selected_rows if row['decoded_sha256'] is not None]
        decoder_counts[suffix] = {'mapped': len(selected_rows), 'validated': len(decoded_rows),
                                  'unresolved': len(selected_rows) - len(decoded_rows)}
    if decoder_counts['.xml'] != {'mapped': 39, 'validated': 24, 'unresolved': 15}:
        raise AssertionError(f'XML validation count changed unexpectedly: {decoder_counts[".xml"]}')
    if decoder_counts['.luac'] != {'mapped': 219, 'validated': 217, 'unresolved': 2}:
        raise AssertionError(f'Lua validation count changed unexpectedly: {decoder_counts[".luac"]}')
    if decoder_counts['.swf'] != {'mapped': 7, 'validated': 7, 'unresolved': 0}:
        raise AssertionError(f'SWF validation count changed unexpectedly: {decoder_counts[".swf"]}')
    xml_family_expected = {
        '.lightset_xml': {'mapped': 40, 'validated': 40, 'unresolved': 0},
        '.mlx': {'mapped': 400, 'validated': 400, 'unresolved': 0},
        '.mgp': {'mapped': 629, 'validated': 628, 'unresolved': 1},
        '.mvp': {'mapped': 598, 'validated': 596, 'unresolved': 2},
        '.mgx': {'mapped': 250, 'validated': 250, 'unresolved': 0},
        '.mvx': {'mapped': 253, 'validated': 253, 'unresolved': 0},
    }
    for suffix, expected in xml_family_expected.items():
        if decoder_counts[suffix] != expected:
            raise AssertionError(f'{suffix} XML parse counts changed: {decoder_counts[suffix]}')
    if sum(decoder_counts[suffix]['validated'] for suffix in xml_family_expected) + decoder_counts['.xml']['validated'] != 2191:
        raise AssertionError('XML-family validation total changed unexpectedly')

    name_rows = [row for row in mapped if row['logical_paths'][0].lower().endswith(_NAME_STREAM_SUFFIXES)]
    name_stream_counts = {}
    for suffix in _NAME_STREAM_SUFFIXES:
        selected = [row for row in name_rows if row['logical_paths'][0].lower().endswith(suffix)]
        name_stream_counts[suffix] = {
            'mapped': len(selected),
            'validated': sum(row['name_group_count'] is not None for row in selected),
            'groups': sum(row['name_group_count'] or 0 for row in selected),
            'names': sum(row['name_count'] or 0 for row in selected),
            'unresolved': sum(row['name_group_count'] is None for row in selected),
        }
    if name_stream_counts['_pyarraynames.bin'] != {
            'mapped': 35, 'validated': 35, 'groups': 71, 'names': 8863, 'unresolved': 0}:
        raise AssertionError(f'pyarray names stream inventory changed: {name_stream_counts}')
    if name_stream_counts['_pystructnames.bin'] != {
            'mapped': 34, 'validated': 31, 'groups': 343, 'names': 11705, 'unresolved': 3}:
        raise AssertionError(f'pystruct names stream inventory changed: {name_stream_counts}')
    pyarray_rows = [row for row in mapped if row['logical_paths'][0].lower().endswith('_pyarray.bin')]
    if any(row['decoded_sha256'] is not None for row in pyarray_rows):
        raise AssertionError('pyarray.bin payload was decoded without its source schema')

    # Break known-good decoded candidates in memory and ensure their typed
    # validators reject them. The source files themselves are never edited.
    xml_row = next(row for row in mapped if row['logical_paths'][0].lower().endswith('.xml')
                   and row['decode_confidence'] == 'LIKELY')
    lua_row = next(row for row in mapped if row['logical_paths'][0].lower().endswith('.luac')
                   and row['decode_confidence'] == 'LIKELY')
    swf_row = next(row for row in mapped if row['logical_paths'][0].lower().endswith('.swf'))
    zip_row = next(row for row in mapped if _SOUNDS_ZIP_LOGICAL in row['logical_paths'])
    xml_broken = _source_file(ctx, xml_row).read_bytes()[:-1]
    lua_broken = bytearray(_source_file(ctx, lua_row).read_bytes())
    lua_broken[min(len(lua_broken) - 1, 100)] = 0xff
    swf_broken = _source_file(ctx, swf_row).read_bytes()[:-1]
    zip_broken = b'\x00\x00\x00\x00' + _source_file(ctx, zip_row).read_bytes()[4:]
    invalid_cases = {
        'xml_parse': _classify(xml_row['logical_paths'], xml_row['relative'], xml_broken)[3] is None,
        'lua_utf8_or_bytecode': _classify(lua_row['logical_paths'], lua_row['relative'], bytes(lua_broken))[3] is None,
        'swf_size_or_tag_bounds': _classify(swf_row['logical_paths'], swf_row['relative'], swf_broken)[3] is None,
        'zip_signature': _classify(zip_row['logical_paths'], zip_row['relative'], zip_broken)[3] is None,
        'direct_plaintext_xml': _classify(['data/test.xml'], 'test.xml', b'<root/>')[3] == b'<root/>',
    }
    name_row = next(row for row in name_rows if row['name_group_count'] is not None)
    parsed_name_groups = read_name_groups(ctx, name_row)
    if (len(parsed_name_groups) != name_row['name_group_count']
            or sum(map(len, parsed_name_groups)) != name_row['name_count']):
        raise AssertionError('public names-only reader differs from inventory metadata')
    name_broken = bytearray(read_resource(ctx, name_row))
    if len(name_broken) >= 4:
        name_broken[:4] = struct.pack('<I', 0xffffffff)
    invalid_cases['name_group_bound'] = _classify(
        name_row['logical_paths'], name_row['relative'], bytes(name_broken))[3] is None
    if not all(invalid_cases.values()):
        raise AssertionError(f'corrupt typed payload unexpectedly decoded: {invalid_cases}')

    plan = _ipa_root(ctx) / 'ios_sound_plan.txt'
    selected = [line.strip() for line in plan.read_text(encoding='utf-8').splitlines() if line.strip()]
    wav_names = [name for name in selected if name.lower().endswith('.wav')]
    if len(wav_names) != 239:
        raise AssertionError(f'expected 239 staged WAV names, found {len(wav_names)}')
    manifest_path = Path(ctx.inputs) / 'assets-extra/ios/MANIFEST.json'
    manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
    staged = {item['path'].split('/')[-1]: item for item in manifest.get('files', [])
              if item.get('path', '').startswith('assets-extra/ios/data/sounds/')}
    if any(name not in staged for name in wav_names):
        missing = [name for name in wav_names if name not in staged]
        raise AssertionError(f'{len(missing)} planned WAVs have no staged iOS receipt')

    zip_row = next(row for row in rows if _SOUNDS_ZIP_LOGICAL in row['logical_paths'])
    zip_path = _source_file(ctx, zip_row)
    zip_raw = zip_path.read_bytes()
    if zip_raw[:4] == _SOUNDS_ZIP_FIRST:
        zip_raw = b'PK\x03\x04' + zip_raw[4:]
    elif zip_raw[:4] != b'PK\x03\x04':
        raise AssertionError('sounds.zip has an unrecognized first header')
    checked = 0
    with zipfile.ZipFile(io.BytesIO(zip_raw), 'r') as archive:
        members_by_basename = {}
        for item in archive.infolist():
            members_by_basename.setdefault(posixpath.basename(item.filename.replace('\\', '/')), []).append(item)
        for name in wav_names:
            matches = members_by_basename.get(name, [])
            if len(matches) != 1:
                raise AssertionError(f'expected exactly one sounds.zip member named {name}, found {len(matches)}')
            payload = archive.read(matches[0])
            receipt = staged[name]
            digest = _sha(payload)
            if digest != receipt['sha256'] or len(payload) != receipt['bytes']:
                raise AssertionError(f'sounds.zip receipt mismatch for {name}')
            checked += 1

    bdae_receipts = {}
    staged_bdae = {item['path'].removeprefix('assets-extra/ios/'): item
                   for item in manifest.get('files', [])
                   if item.get('path', '').startswith('assets-extra/ios/data/3d/')
                   and item.get('path', '').endswith('.bdae')}
    for logical in ('data/3d/interface/level_up.bdae',
                    'data/3d/camera/minimapcameras.bdae',
                    'data/3d/camera/minimapcamerashud.bdae'):
        row = next(item for item in mapped if logical in item['logical_paths'])
        receipt = staged_bdae.get(logical)
        if (receipt is None or receipt.get('sha256') != row.get('decoded_sha256')
                or receipt.get('bytes') != row.get('decoded_bytes')
                or row.get('decode_confidence') != 'CROSSCHECKED'):
            raise AssertionError(f'staged iOS BDAE receipt mismatch for {logical}')
        bdae_receipts[logical] = receipt['sha256']

    # A fabricated or corrupt mapping fails closed, and traversal is rejected.
    bogus = dict(zip_row, package_relative='data/file999999.dat')
    try:
        read_resource(ctx, bogus)
    except (FileNotFoundError, ValueError):
        pass
    else:
        raise AssertionError('unknown pack mapping unexpectedly resolved')
    forged = dict(zip_row, logical_paths=['data/sounds/not-the-package-member.zip'])
    try:
        read_resource(ctx, forged)
    except ValueError:
        pass
    else:
        raise AssertionError('forged logical alias unexpectedly resolved')
    try:
        _safe_relative('data/../../outside.dat')
    except ValueError:
        pass
    else:
        raise AssertionError('path traversal mapping unexpectedly accepted')

    return {
        'app_data_files': len(rows),
        'numbered_packs': len(numbered),
        'mapped_logical_paths': len(mapped),
        'unmapped_numbered_packs': [row['relative'] for row in unknown],
        'staged_wav_hashes_verified': checked,
        'staged_bdae_hashes_verified': len(bdae_receipts),
        'typed_decoder_counts': decoder_counts,
        'names_only_stream_counts': name_stream_counts,
        'pyarray_payloads_left_raw': len(pyarray_rows),
        'android_source_pairs': _paired_source_audit(
            ctx, {physical: logical for logical, physical in by_logical.items()}),
        'corrupt_typed_payloads_rejected': invalid_cases,
        'mapping_source': 'embedded file00000-plantext.dat exactly matches index_map.tsv',
        'read_only': True,
    }
