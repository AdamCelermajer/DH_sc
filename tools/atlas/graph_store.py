"""Generation-checked SQLite index for the local Atlas HTTP viewer.

The original graph and source corpus are read-only. The cache lives under the
Atlas tool directory and is rebuilt atomically when the graph generation
changes.
"""
from __future__ import annotations

from collections import deque
import hashlib
import json
import os
from pathlib import Path
import re
import sqlite3
import tempfile
import time
from urllib.parse import quote


SCHEMA_VERSION = 4
MAX_PAGE = 100
MAX_OFFSET = 1_000_000
MAX_ENTITY_EDGES = 1200
MAX_HOP_EDGES = 2400
MAX_HOP_NODES = 1800
MAX_CLOSURE_NODES = 5000
MAX_CLOSURE_EDGES = 7000
MAX_NATIVE_ADDRESSES = 16
RESOURCE_DEPTH = 10
SKIP_RESOURCE_KINDS = {'source_file', 'engine_call', 'helper_definition', 'command',
                       'campaign_script', 'scene_file', 'authored_spawn_candidate', 'source_call'}
RESOURCE_CATEGORIES = {'Actors', 'References', 'Tables', 'Scripts', 'Skills', 'Animations',
                       'Effects', 'Items', 'Audio', 'Assets'}
ID_RE = re.compile(r'^[A-Za-z0-9_.:#@+~-]{1,256}$')
QUERY_KINDS = {'mob':'actor','boss':'actor','actor':'actor','skill':'skill','item':'item',
 'sound':'sound','effect':'effect_set','clip':'animation_clip','anim':'animation_clip',
 'sequence':'animation_sequence','projectile':'projectile','script':'script','function':'engine_function'}


def _identity(path: Path) -> dict:
    st = path.stat()
    return {'size': st.st_size, 'mtime_ns': st.st_mtime_ns,
            'inode': getattr(st, 'st_ino', 0), 'source': str(path.resolve())}


def _sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open('rb') as f:
        while chunk := f.read(1024 * 1024):
            h.update(chunk)
    return h.hexdigest()


class _JSONReader:
    """Incremental JSON value reader; at most one graph record is resident."""
    def __init__(self, stream):
        self.stream = stream
        self.decoder = json.JSONDecoder()
        self.buf = ''
        self.pos = 0
        self.eof = False

    def _fill(self):
        if self.pos:
            self.buf = self.buf[self.pos:]
            self.pos = 0
        chunk = self.stream.read(1024 * 1024)
        if chunk:
            self.buf += chunk
        else:
            self.eof = True

    def ws(self):
        while True:
            while self.pos < len(self.buf) and self.buf[self.pos].isspace():
                self.pos += 1
            if self.pos < len(self.buf) or self.eof:
                return
            self._fill()

    def char(self):
        self.ws()
        if self.pos >= len(self.buf):
            raise ValueError('Unexpected end of graph JSON')
        c = self.buf[self.pos]
        self.pos += 1
        return c

    def value(self):
        self.ws()
        while True:
            try:
                val, end = self.decoder.raw_decode(self.buf, self.pos)
                self.pos = end
                return val
            except json.JSONDecodeError:
                if self.eof:
                    raise
                old = self.pos
                self._fill()
                if self.pos == old and self.eof:
                    raise

    def collection(self, opener, closer):
        if self.char() != opener:
            raise ValueError(f'Expected {opener} in graph JSON')
        self.ws()
        if self.pos < len(self.buf) and self.buf[self.pos] == closer:
            self.pos += 1
            return
        while True:
            yield self.value()
            c = self.char()
            if c == closer:
                return
            if c != ',':
                raise ValueError(f'Expected comma or {closer} in graph JSON')

    def items(self):
        if self.char() != '{':
            raise ValueError('Expected object in graph JSON')
        self.ws()
        if self.pos < len(self.buf) and self.buf[self.pos] == '}':
            self.pos += 1
            return
        while True:
            key = self.value()
            if not isinstance(key, str) or self.char() != ':':
                raise ValueError('Malformed graph JSON object')
            yield key, self.value()
            c = self.char()
            if c == '}':
                return
            if c != ',':
                raise ValueError('Expected comma or } in graph JSON')

    def rows(self):
        if self.char() != '{':
            raise ValueError('Graph root must be a JSON object')
        self.ws()
        if self.pos < len(self.buf) and self.buf[self.pos] == '}':
            return
        while True:
            key = self.value()
            if not isinstance(key, str) or self.char() != ':':
                raise ValueError('Malformed graph JSON property')
            if key in ('nodes', 'edges'):
                tag = key[:-1] if key == 'nodes' else 'edge'
                for row in self.collection('[', ']'):
                    yield tag, row
            elif key == 'profiles':
                for profile_id, profile in self.items():
                    yield 'profile', (profile_id, profile)
            elif key in ('entities_by_id', 'scripts_by_id', 'functions_by_id',
                         'resource_profiles_by_id', 'resource_files_by_id'):
                tag = {'entities_by_id':'native_entity', 'scripts_by_id':'native_script',
                       'functions_by_id':'native_function', 'resource_profiles_by_id':'resource_profile',
                       'resource_files_by_id':'resource_file'}[key]
                for identity, record in self.items():
                    yield tag, (identity, record)
            else:
                self.value()
            c = self.char()
            if c == '}':
                break
            if c != ',':
                raise ValueError('Malformed graph root')


def _norm(value):
    return ' '.join(str(value or '').casefold().split())


def _path_variants(value):
    raw=str(value or '').replace('\\','/').strip().casefold()
    if not raw:return []
    values={raw}
    # Asset ledgers use short Android aliases while the native resolver may
    # name the corresponding logical iPhone URI. Expand only known prefixes.
    queue=[raw]
    while queue:
        current=queue.pop()
        for prefix in ('q:/data/iphone/','q:/','data/iphone/','data/3d/','data/','iphone/'):
            if current.startswith(prefix) and len(current)>len(prefix):
                trimmed=current[len(prefix):]
                if trimmed and trimmed not in values:
                    values.add(trimmed);queue.append(trimmed)
        if current.startswith('3d/'):
            for alias in ('data/3d/'+current[3:], 'q:/data/iphone/data/3d/'+current[3:],
                          'q:/data/iphone/'+current):
                if alias not in values:values.add(alias)
        if current.startswith('data/3d/'):
            tail=current[len('data/'):]
            for alias in ('q:/data/iphone/'+current,'q:/data/iphone/'+tail):
                if alias not in values:values.add(alias)
    return sorted(values)


def _query_variants(value):
    raw=str(value or '').replace('\\','/').strip().casefold()
    variants=_path_variants(raw)
    return list(dict.fromkeys([raw]+variants))[:8]


def _node_row(n):
    data = dict(n)
    aliases = data.get('aliases') or []
    if not isinstance(aliases, list):
        aliases = [aliases]
    display_name = data.get('display_name') or data.get('title')
    fields = data.get('fields')
    if isinstance(fields, dict):
        display_name = display_name or fields.get('display_name') or fields.get('DisplayName')
    name = str(data.get('name') or data['id'])
    field_values=[]
    paths=[]
    fields=data.get('fields')
    if isinstance(fields,dict):
        for key,value in fields.items():
            if isinstance(value,(str,int,float)):
                field_values.extend((str(key),str(value)))
                if isinstance(value,str) and len(value)<=500 and ('/' in value or '\\' in value):
                    paths.extend(_path_variants(value))
            elif isinstance(value,list):
                field_values.extend(str(x) for x in value[:80] if isinstance(x,(str,int,float)))
    for key in ('path','site','source','relative','logical_path'):
        if isinstance(data.get(key),str):paths.extend(_path_variants(data[key]))
    for key in ('logical_paths','paths'):
        if isinstance(data.get(key),list):
            for path in data[key][:100]:paths.extend(_path_variants(path))
    alias_variants=[]
    for alias in aliases:
        alias_variants.append(_norm(alias))
        alias_variants.extend(_norm(p) for p in _path_variants(alias))
    alias_variants=list(dict.fromkeys(a for a in alias_variants if a))
    fields_search=' '.join(field_values)
    search=' '.join([name,str(display_name or ''),str(data.get('id') or ''),
        str(data.get('table') or ''),str(data.get('kind') or ''),str(data.get('source') or ''),
        fields_search,*alias_variants,*paths])
    return (str(data['id']), str(data.get('category') or data.get('kind') or 'Other'), name,
            str(data.get('kind') or ''), str(data.get('table') or ''),
            str(data.get('index')) if data.get('index') is not None else None,
            str(data.get('platform') or ''), str(data.get('source') or ''),
            str(data.get('status') or ''), str(data.get('confidence') or ''),
            str(display_name or ''), json.dumps(aliases, ensure_ascii=False, separators=(',', ':')),
            _norm(name), _norm(display_name), '|'+ '|'.join(alias_variants)+'|',
            search.casefold(), json.dumps(data, ensure_ascii=False, separators=(',', ':')))


class GraphStore:
    def __init__(self, db_path: Path, generation: dict):
        self.db_path = db_path
        self.generation = generation
        self.db = sqlite3.connect(f'file:{db_path.as_posix()}?mode=ro', uri=True,
                                  check_same_thread=False)
        self.db.row_factory = sqlite3.Row
        self.counts = json.loads(self.db.execute("select value from meta where key='counts'").fetchone()[0])
        self.category_order = [r[0] for r in self.db.execute(
            'select category from nodes group by category order by category collate nocase')]
        self.categories = {r['category']: r['n'] for r in self.db.execute(
            'select category,count(*) n from nodes group by category')}
        self.fts = bool(self.db.execute("select 1 from sqlite_master where name='node_fts'").fetchone())

    @classmethod
    def open(cls, ctx, cache_dir=None):
        source = Path(ctx.out) / 'data' / 'graph.json'
        if not source.is_file():
            raise FileNotFoundError(f'Atlas graph is missing: {source}')
        runtime = Path(cache_dir) if cache_dir is not None else Path(__file__).resolve().parent / '.runtime'
        runtime.mkdir(parents=True, exist_ok=True)
        manifest = runtime / 'graph-cache.json'
        identity = _identity(source)
        native_path = Path(ctx.out) / 'data' / 'entity-native.json'
        native_identity = _identity(native_path) if native_path.is_file() else None
        # This side index is small enough to hash on each server launch, which
        # catches an atomic regeneration even if timestamps were preserved.
        native_digest = _sha256(native_path) if native_path.is_file() else None
        resource_path = Path(ctx.out) / 'data' / 'resource-attributions.json'
        resource_identity = _identity(resource_path) if resource_path.is_file() else None
        resource_digest = _sha256(resource_path) if resource_path.is_file() else None
        old = None
        try:
            old = json.loads(manifest.read_text(encoding='utf-8'))
        except (OSError, ValueError):
            pass
        if (old and old.get('identity') == identity and old.get('native_identity') == native_identity
                and old.get('native_sha256') == native_digest and
                old.get('resource_identity') == resource_identity and old.get('resource_sha256') == resource_digest):
            db_path = runtime / old.get('database', '')
            if db_path.parent == runtime and db_path.is_file() and cls._valid_db(
                    db_path, old.get('sha256'), old.get('native_sha256'), old.get('resource_sha256')):
                return cls(db_path, old)
        digest = _sha256(source)
        cache_key = hashlib.sha256((digest + '|' + (native_digest or '') + '|' + (resource_digest or '')).encode('ascii')).hexdigest()
        db_path = runtime / f'graph-{cache_key}.sqlite'
        if not (db_path.is_file() and cls._valid_db(db_path, digest, native_digest, resource_digest)):
            cls._build(source, db_path, digest, native_path if native_digest else None, native_digest,
                       resource_path if resource_digest else None, resource_digest)
        data = {'schema': SCHEMA_VERSION, 'identity': identity, 'sha256': digest,
                'native_identity': native_identity, 'native_sha256': native_digest,
                'resource_identity': resource_identity, 'resource_sha256': resource_digest,
                'cache_key': cache_key, 'database': db_path.name, 'built_at': time.time()}
        tmp = manifest.with_suffix('.tmp')
        tmp.write_text(json.dumps(data, separators=(',', ':')), encoding='utf-8')
        os.replace(tmp, manifest)
        return cls(db_path, data)

    @staticmethod
    def _valid_db(path, digest, native_digest=None, resource_digest=None):
        try:
            with sqlite3.connect(f'file:{path.as_posix()}?mode=ro', uri=True) as db:
                rows = dict(db.execute('select key,value from meta'))
                return (rows.get('schema') == str(SCHEMA_VERSION) and rows.get('sha256') == digest and
                        rows.get('native_sha256') == (native_digest or '') and
                        rows.get('resource_sha256') == (resource_digest or ''))
        except (sqlite3.Error, OSError):
            return False

    @classmethod
    def _build(cls, source, dest, digest, native_path=None, native_digest=None,
               resource_path=None, resource_digest=None):
        fd, temp_name = tempfile.mkstemp(prefix='graph-', suffix='.sqlite.tmp', dir=dest.parent)
        os.close(fd)
        tmp = Path(temp_name)
        try:
            db = sqlite3.connect(tmp)
            db.execute('pragma journal_mode=off')
            db.execute('pragma synchronous=off')
            db.executescript('''
              create table meta(key text primary key,value text not null);
              create table nodes(rowid integer primary key,id text unique not null,category text,name text,kind text,
                table_name text,node_index text,platform text,source text,status text,confidence text,display_name text,
                aliases text,name_norm text,display_norm text,aliases_norm text,search_text text,data text not null);
              create table edges(id text primary key,source text not null,target text not null,kind text,label text,data text not null);
              create index edges_source on edges(source,id);
              create index edges_target on edges(target,id);
              create index nodes_category on nodes(category,name_norm,id);
              create index nodes_name on nodes(name_norm,id);
              create index nodes_display on nodes(display_norm,id);
              create index nodes_kind on nodes(kind);
              create table profiles(id text primary key,data text not null);
              create table native_entities(id text primary key,data text not null);
              create table native_scripts(id text primary key,data text not null);
              create table native_functions(id text primary key,data text not null);
              create table resource_profiles(id text primary key,data text not null);
              create table resource_files(id text primary key,data text not null);
            ''')
            try:
                db.execute("create virtual table node_fts using fts5(search_text, tokenize='unicode61 remove_diacritics 2')")
                has_fts = True
            except sqlite3.OperationalError:
                has_fts = False
            node_count = edge_count = profile_count = 0
            by_category = {}
            with source.open('r', encoding='utf-8') as stream:
                reader = _JSONReader(stream)
                batch = []
                for tag, row in reader.rows():
                    if tag == 'node':
                        values = _node_row(row)
                        batch.append(values)
                        node_count += 1
                        by_category[values[1]] = by_category.get(values[1], 0) + 1
                        if len(batch) >= 500:
                            db.executemany('insert into nodes(id,category,name,kind,table_name,node_index,platform,source,status,confidence,display_name,aliases,name_norm,display_norm,aliases_norm,search_text,data) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)', batch)
                            if has_fts:
                                db.executemany('insert into node_fts(rowid,search_text) select rowid,search_text from nodes where id=?', [(v[0],) for v in batch])
                            batch.clear()
                    elif tag == 'edge':
                        e = dict(row)
                        db.execute('insert into edges values(?,?,?,?,?,?)', (str(e['id']), str(e['from']), str(e['to']),
                            str(e.get('kind') or ''), str(e.get('label') or ''),
                            json.dumps(e, ensure_ascii=False, separators=(',', ':'))))
                        edge_count += 1
                    else:
                        if tag == 'profile':
                            pid, profile = row
                            db.execute('insert into profiles values(?,?)', (str(pid), json.dumps(profile, ensure_ascii=False, separators=(',', ':'))))
                            profile_count += 1
                if batch:
                    db.executemany('insert into nodes(id,category,name,kind,table_name,node_index,platform,source,status,confidence,display_name,aliases,name_norm,display_norm,aliases_norm,search_text,data) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)', batch)
                    if has_fts:
                        db.executemany('insert into node_fts(rowid,search_text) select rowid,search_text from nodes where id=?', [(v[0],) for v in batch])
            if native_path:
                native_tables={'native_entity':'native_entities','native_script':'native_scripts','native_function':'native_functions'}
                with native_path.open('r',encoding='utf-8') as stream:
                    for tag,(identity,record) in _JSONReader(stream).rows():
                        table=native_tables.get(tag)
                        if table:
                            db.execute(f'insert into {table} values(?,?)',(str(identity),
                                json.dumps(record,ensure_ascii=False,separators=(',',':'))))
            if resource_path:
                resource_tables={'resource_profile':'resource_profiles','resource_file':'resource_files'}
                with resource_path.open('r',encoding='utf-8') as stream:
                    for tag,(identity,record) in _JSONReader(stream).rows():
                        table=resource_tables.get(tag)
                        if table:
                            db.execute(f'insert into {table} values(?,?)',(str(identity),
                                json.dumps(record,ensure_ascii=False,separators=(',',':'))))
            counts = {'nodes': node_count, 'edges': edge_count, 'profiles': profile_count,
                      'by_category': by_category, 'scope': 'Indexed graph source; evidence and inference remain on the original records.'}
            db.executemany('insert into meta values(?,?)', [('schema', str(SCHEMA_VERSION)), ('sha256', digest),
                ('native_sha256', native_digest or ''),
                ('resource_sha256', resource_digest or ''),
                ('counts', json.dumps(counts, separators=(',', ':'))), ('fts', str(int(has_fts)))])
            db.commit()
            db.close()
            os.replace(tmp, dest)
        except Exception:
            try:
                tmp.unlink()
            except OSError:
                pass
            raise

    def meta(self):
        return {'counts': {'total': self.counts.get('nodes', 0), 'categories': self.categories},
                'category_order': self.category_order,
                'data_version': self.generation.get('cache_key') or self.generation.get('sha256'),
                'native_version': self.generation.get('native_sha256'),
                'resource_version': self.generation.get('resource_sha256')}

    def _summary(self, row):
        return {'id': row['id'], 'name': row['name'], 'display_name': row['display_name'] or None,
                'category': row['category'], 'kind': row['kind'], 'table': row['table_name'] or None,
                'index': row['node_index'], 'platform': row['platform'] or None,
                'source': row['source'] or None, 'status': row['status'] or None,
                'confidence': row['confidence'] or None, 'aliases': json.loads(row['aliases'])}

    def search(self, q='', category='', offset=0, limit=50):
        q = str(q or '').strip()[:160]
        category = str(category or '').strip()[:80]
        offset = max(0, min(int(offset), MAX_OFFSET))
        limit = max(1, min(int(limit), MAX_PAGE))
        kind_filter = ''
        query = q
        if ':' in q:
            prefix, suffix = q.split(':', 1)
            mapped = QUERY_KINDS.get(prefix.casefold())
            if mapped:
                kind_filter, query = mapped, suffix.strip()
        params = []
        clauses = []
        if category.casefold() in ('all', 'game', 'game entities') or not category:
            clauses.append("category <> 'References'")
        elif category:
            clauses.append('category = ?')
            params.append(category)
        if kind_filter:
            clauses.append('kind = ?')
            params.append(kind_filter)
        if kind_filter and query.isdecimal():
            clauses.append('node_index = ?')
            params.append(query)
        elif query:
            variants = _query_variants(query)
            matches = []
            for variant in variants:
                words = [w for w in re.findall(r'[\w]+', variant, flags=re.UNICODE) if w][:8]
                if words:
                    matches.append('(' + ' '.join('"' + w.replace('"', '') + '"*' for w in words) + ')')
            if self.fts and matches:
                match = ' OR '.join(matches[:8])
                clauses.append('rowid in (select rowid from node_fts where node_fts match ?)')
                params.append(match)
            else:
                clauses.append('search_text like ? escape "\\"')
                escaped = query.casefold().replace('\\', '\\\\').replace('%', '\\%').replace('_', '\\_')
                params.append('%' + escaped + '%')
        where = (' where ' + ' and '.join(clauses)) if clauses else ''
        total = self.db.execute('select count(*) from nodes' + where, params).fetchone()[0]
        rank = ("case when name_norm in ({}) then 0 when display_norm in ({}) then 1 "
                "when {} then 2 when {} then 3 when {} then 4 when {} then 5 else 6 end".format(
                    ','.join('?' for _ in _query_variants(query)),
                    ','.join('?' for _ in _query_variants(query)),
                    ' or '.join('instr(aliases_norm,?)>0' for _ in _query_variants(query)),
                    ' or '.join('name_norm like ?' for _ in _query_variants(query)),
                    ' or '.join('display_norm like ?' for _ in _query_variants(query)),
                    ' or '.join('instr(aliases_norm,?)>0' for _ in _query_variants(query)))) if query else 'category collate nocase'
        default_category = category.casefold() in ('all', 'game', 'game entities') or not category
        priority = ("case when category in ('Actors','Skills','Items','Levels','Campaign','Tables') "
                    "then 0 else 1 end, ") if query and default_category else ''
        order_params = []
        if query:
            norms = _query_variants(query)
            order_params = (norms + norms + ['|' + n + '|' for n in norms] +
                            [n + '%' for n in norms] + [n + '%' for n in norms] +
                            ['|' + n for n in norms])
        rows = self.db.execute('select id,name,display_name,category,kind,table_name,node_index,platform,source,status,confidence,aliases from nodes'
            + where + ' order by ' + priority + rank + ',category collate nocase,name collate nocase,id limit ? offset ?',
            params + order_params + [limit, offset]).fetchall()
        return {'results': [self._summary(r) for r in rows], 'total': total, 'offset': offset,
                'limit': limit, 'next_offset': offset + len(rows) if offset + len(rows) < total else None}

    def entity(self, node_id):
        if not isinstance(node_id, str) or not ID_RE.fullmatch(node_id):
            raise ValueError('Invalid entity ID')
        row = self.db.execute('select data from nodes where id=?', (node_id,)).fetchone()
        if not row:
            return None
        node = json.loads(row[0])
        profile_row = self.db.execute('select data from profiles where id=?', (node_id,)).fetchone()
        profile = json.loads(profile_row[0]) if profile_row else None
        outgoing = [json.loads(r[0]) for r in self.db.execute('select data from edges where source=? order by id limit ?', (node_id, MAX_ENTITY_EDGES + 1))]
        incoming = [json.loads(r[0]) for r in self.db.execute('select data from edges where target=? order by id limit ?', (node_id, MAX_ENTITY_EDGES + 1))]
        truncated = len(outgoing) > MAX_ENTITY_EDGES or len(incoming) > MAX_ENTITY_EDGES
        outgoing = outgoing[:MAX_ENTITY_EDGES]
        incoming = incoming[:MAX_ENTITY_EDGES]
        edges = {e['id']: e for e in outgoing + incoming}
        nodes = {node_id: node}
        refs = set()
        for edge in edges.values():
            other = edge['to'] if edge['from'] == node_id else edge['from']
            obj = self._node(other)
            if obj:
                nodes[other] = obj
                if obj.get('category') == 'References':
                    refs.add(other)
        # Expand exactly one reference intermediary hop, bounded independently.
        hop_used = 0
        for ref_id in refs:
            remaining = MAX_HOP_EDGES - hop_used
            if remaining <= 0 or len(nodes) >= MAX_HOP_NODES:
                truncated = True
                break
            for r in self.db.execute('select data from edges where source=? or target=? order by id limit ?', (ref_id, ref_id, remaining + 1)):
                if hop_used >= MAX_HOP_EDGES or len(nodes) >= MAX_HOP_NODES:
                    truncated = True
                    break
                e = json.loads(r[0]); edges[e['id']] = e; hop_used += 1
                other = e['to'] if e['from'] == ref_id else e['from']
                if other not in nodes:
                    obj = self._node(other)
                    if obj: nodes[other] = obj
        resource = self._resource_closure(node_id, profile)
        result = {'node': node, 'profile': profile, 'nodes': list(nodes.values()), 'edges': list(edges.values()),
                  'incoming': incoming, 'outgoing': outgoing, 'resource_closure': resource,
                  'truncated': truncated}
        native = self._native_entity(node_id)
        if native is not None:
            result['entity_native'] = native
        resource = self._resource_attribution(node_id)
        if resource is not None:
            result['resource_attribution'] = resource
        return result

    def _node(self, node_id):
        row = self.db.execute('select data from nodes where id=?', (node_id,)).fetchone()
        return json.loads(row[0]) if row else None

    def _canonical_entity_candidates(self, node_id):
        node = self._node(node_id)
        if not node:
            return []
        candidates = []
        table = node.get('table')
        index = node.get('index')
        if isinstance(table,str) and table and index is not None:
            candidates.append('entity:'+table+':'+str(index))
        source = node.get('source') if isinstance(node.get('source'),dict) else {}
        raw = source.get('raw') if isinstance(source.get('raw'),dict) else {}
        fields = node.get('fields') if isinstance(node.get('fields'),dict) else {}
        for row_id in (node.get('row_id'),source.get('row_id'),raw.get('row_id'),
                       fields.get('row_id'),fields.get('source.raw.row_id')):
            if isinstance(row_id,str) and row_id:
                candidates.append('entity:'+row_id)
        return list(dict.fromkeys(candidates))

    def _native_entity(self, node_id, script_offset=0, function_offset=0, limit=16):
        candidates = [node_id]
        for prefix in ('table:', 'scene:', 'object:', 'entity:'):
            if node_id.startswith(prefix):
                candidates.append(node_id[len(prefix):])
        for candidate in list(candidates):
            candidates.extend(('table:'+candidate, 'scene:'+candidate, 'entity:'+candidate))
        candidates.extend(self._canonical_entity_candidates(node_id))
        entity = None
        matched_id = None
        for candidate in dict.fromkeys(candidates):
            row = self.db.execute('select data from native_entities where id=?', (candidate,)).fetchone()
            if row:
                entity = json.loads(row[0]); matched_id = candidate; break
        if not entity:
            return None
        native = {key: entity.get(key) for key in ('id','kind','source','script_links','script_ids',
            'call_ids','native_function_ids','association_note','unresolved_script_links','notes',
            'script_association_status','no_script_association_reason')}
        all_scripts = entity.get('script_ids') or []
        all_functions = entity.get('native_function_ids') or []
        limit=max(1,min(int(limit),24))
        script_offset=max(0,min(int(script_offset),10000))
        function_offset=max(0,min(int(function_offset),10000))
        scripts = []
        for script_id in all_scripts[script_offset:script_offset+limit]:
            row = self.db.execute('select data from native_scripts where id=?', (str(script_id),)).fetchone()
            if not row: continue
            script = json.loads(row[0])
            item={key:script.get(key) for key in ('id','name','site','platform','format',
                'status','readiness','call_ids','function_ids')}
            for candidate in ('script:'+str(script_id),'script:ios:'+str(script_id)):
                if self.db.execute('select 1 from nodes where id=?',(candidate,)).fetchone():
                    item['graph_id']=candidate
                    source_edge=self.db.execute("select target from edges where source=? and kind='source_file' order by id limit 1",(candidate,)).fetchone()
                    if source_edge and str(source_edge[0]).startswith('asset:'):
                        item['source_url']='/api/source/'+quote(str(source_edge[0])[6:],safe='')
                    break
            scripts.append(item)
        functions = []
        for function_id in all_functions[function_offset:function_offset+limit]:
            row = self.db.execute('select data from native_functions where id=?', (str(function_id),)).fetchone()
            if not row: continue
            function = json.loads(row[0])
            addresses=[]
            all_addresses=function.get('native_addresses') or []
            for address in all_addresses[:MAX_NATIVE_ADDRESSES]:
                item={key:address.get(key) for key in ('address','mode','binding','handler','signature',
                    'pseudocode_path','source_line','snippet_kind','native_contract_status','port_status')
                      if address.get(key) is not None}
                snippet=address.get('snippet')
                if isinstance(snippet,str): item['snippet']=snippet[:1200]
                item['source_url']='/api/native-source?function_id='+quote(str(function_id),safe='')+'&address_index='+str(len(addresses))+'&full=1'
                addresses.append(item)
            functions.append({key:function.get(key) for key in ('id','name','mode','meaning','status',
                'reason','candidate_note','native_contract_status','port_status','ida_ledger_path')
                if function.get(key) is not None} | {'native_addresses':addresses,
                    'native_addresses_total':len(all_addresses),
                    'native_addresses_truncated':len(all_addresses)>MAX_NATIVE_ADDRESSES})
        native.update({'scripts':scripts,'functions':functions,'matched_entity_id':matched_id,
                       'script_total':len(all_scripts),'function_total':len(all_functions),
                       'script_offset':script_offset,'function_offset':function_offset,'limit':limit,
                       'next_script_offset':script_offset+len(scripts) if script_offset+len(scripts)<len(all_scripts) else None,
                       'next_function_offset':function_offset+len(functions) if function_offset+len(functions)<len(all_functions) else None,
                       'bounded':{'max_page':24,'max_addresses_per_function':MAX_NATIVE_ADDRESSES,
                                  'max_snippet_chars':1200}})
        return native

    def native_page(self, node_id, script_offset=0, function_offset=0, limit=16):
        if not isinstance(node_id,str) or not ID_RE.fullmatch(node_id):
            raise ValueError('Invalid entity ID')
        if not self.db.execute('select 1 from nodes where id=?',(node_id,)).fetchone():
            return None
        native=self._native_entity(node_id,script_offset,function_offset,limit)
        if native is None:
            return {'entity_native':None,'scripts':[],'functions':[],'script_total':0,'function_total':0,
                    'script_offset':max(0,int(script_offset)),'function_offset':max(0,int(function_offset)),
                    'limit':max(1,min(int(limit),24)),'next_script_offset':None,'next_function_offset':None}
        return {'entity_native':native,'scripts':native['scripts'],'functions':native['functions'],
                **{k:native[k] for k in ('script_total','function_total','script_offset','function_offset','limit',
                    'next_script_offset','next_function_offset')}}

    def native_source(self,function_id,address_index=0):
        if not isinstance(function_id,str) or not ID_RE.fullmatch(function_id):
            raise ValueError('Invalid native function ID')
        address_index=int(address_index)
        if address_index<0 or address_index>=MAX_NATIVE_ADDRESSES:
            raise ValueError('Invalid native address index')
        row=self.db.execute('select data from native_functions where id=?',(function_id,)).fetchone()
        if not row:return None
        function=json.loads(row[0]);addresses=function.get('native_addresses') or []
        if address_index>=len(addresses):return None
        address=addresses[address_index]
        return {k:address.get(k) for k in ('address','mode','binding','handler','signature','pseudocode_path',
            'source_line','snippet_kind','snippet','native_contract_status','port_status') if address.get(k) is not None}

    def _resource_attribution(self,node_id):
        candidates=[node_id]
        candidates.extend(self._canonical_entity_candidates(node_id))
        for prefix in ('table:','scene:','object:','entity:','asset:'):
            if node_id.startswith(prefix):
                candidates.append(node_id[len(prefix):])
        for value in list(candidates):
            candidates.extend(('table:'+value,'entity:'+value,'asset:'+value))
        for table,kind in (('resource_profiles','profile'),('resource_files','file')):
            for candidate in dict.fromkeys(candidates):
                row=self.db.execute(f'select data from {table} where id=?',(candidate,)).fetchone()
                if not row:continue
                record=json.loads(row[0]);trimmed={};truncated={}
                for key,value in record.items():
                    if isinstance(value,list) and len(value)>64:
                        trimmed[key]=value[:64];truncated[key]=len(value)-64
                    else:trimmed[key]=value
                return {'kind':kind,'record':trimmed,'truncated_items':truncated or None}
        return None

    def closure(self, node_id, direction='out', node_limit=MAX_CLOSURE_NODES):
        if not isinstance(node_id, str) or not ID_RE.fullmatch(node_id):
            raise ValueError('Invalid entity ID')
        if direction not in ('in', 'out'):
            raise ValueError('Invalid closure direction')
        if not self.db.execute('select 1 from nodes where id=?', (node_id,)).fetchone():
            return None
        nodes = {node_id: self._node(node_id)}
        edges = {}
        queue = deque([node_id])
        truncated = False
        while queue and len(nodes) < min(max(1, int(node_limit)), MAX_CLOSURE_NODES) and len(edges) < MAX_CLOSURE_EDGES:
            current = queue.popleft()
            col = 'source' if direction == 'out' else 'target'
            next_col = 'to' if direction == 'out' else 'from'
            for row in self.db.execute(f'select data from edges where {col}=? order by id limit ?',
                                       (current, MAX_CLOSURE_EDGES - len(edges) + 1)):
                e = json.loads(row[0])
                edges[e['id']] = e
                nxt = e[next_col]
                if nxt not in nodes:
                    if len(nodes) >= min(max(1, int(node_limit)), MAX_CLOSURE_NODES):
                        truncated = True
                        break
                    obj = self._node(nxt)
                    if obj:
                        nodes[nxt] = obj
                        queue.append(nxt)
            if len(edges) >= MAX_CLOSURE_EDGES:
                truncated = bool(queue)
                break
        truncated = truncated or bool(queue)
        return {'nodes': list(nodes.values()), 'edges': list(edges.values()),
                'total': max(0, len(nodes) - 1), 'truncated': truncated,
                'direction': direction}

    def _resource_closure(self, node_id, profile=None):
        nodes = {node_id: self._node(node_id)}
        edges = {}
        queue = deque([(node_id, 0)])
        truncated = False
        while queue and len(nodes) < 1800 and len(edges) < 7000:
            current, depth = queue.popleft()
            if depth >= RESOURCE_DEPTH:
                continue
            source = nodes.get(current) or self._node(current)
            if not source or source.get('category') not in RESOURCE_CATEGORIES:
                continue
            for row in self.db.execute('select data from edges where source=? order by id limit 181', (current,)):
                e = json.loads(row[0])
                if e.get('kind') in SKIP_RESOURCE_KINDS:
                    continue
                if len(edges) >= 7000 or len(nodes) >= 1800:
                    truncated = True
                    break
                edges[e['id']] = e
                target_id = e['to']
                target = nodes.get(target_id) or self._node(target_id)
                if not target:
                    continue
                nodes[target_id] = target
                if target.get('category') in RESOURCE_CATEGORIES and target_id not in {q[0] for q in queue}:
                    queue.append((target_id, depth + 1))
            if truncated:
                break
        # Profiles contain compact, already resolved resource IDs as well as
        # relationships; include those targets even when no graph edge was
        # emitted for the derived profile field.
        profile_added = 0
        if isinstance(profile, dict):
            for key in ('scripts', 'assets', 'audio', 'related_entities'):
                for resource_id in profile.get(key) or []:
                    if len(nodes) >= 1800 or profile_added >= 1200:
                        truncated = True
                        break
                    if isinstance(resource_id, str) and resource_id not in nodes:
                        target = self._node(resource_id)
                        if target:
                            nodes[resource_id] = target
                            profile_added += 1
                if truncated:
                    break
        truncated = truncated or bool(queue)
        return {'nodes': list(nodes.values()), 'edges': list(edges.values()), 'truncated': truncated,
                'max_nodes': 1800, 'max_edges': 7000, 'max_depth': RESOURCE_DEPTH}
