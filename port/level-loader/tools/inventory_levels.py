"""Independent canonical-cache level inventory, not a native gameplay loader.

Retains complete XML trees/attributes and records unresolved references. Legacy
data/iphone aliases and unique-basename candidates are evidence, not an assertion
that the original engine performed those substitutions.
"""
import argparse, collections, hashlib, json, pathlib, struct, zipfile
import xml.etree.ElementTree as ET

EXPECTED = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
PREFIX = 'com.gameloft.android.GAND.GloftD2SS/files/'

class Reader:
    def __init__(self, raw): self.raw, self.at = raw, 0
    def word(self):
        value = struct.unpack_from('<i', self.raw, self.at)[0]; self.at += 4; return value
    def byte(self): value = self.raw[self.at]; self.at += 1; return value
    def text(self):
        size = self.word(); assert 0 <= size <= len(self.raw)-self.at
        value = self.raw[self.at:self.at+size].decode('utf-8'); self.at += size; return value
    def strings(self):
        count = self.word(); assert 0 <= count <= 10000
        return [self.text() for _ in range(count)]
    def end(self): assert self.at == len(self.raw), (self.at, len(self.raw))

def tree(node):
    return dict(tag=node.tag, attributes=dict(node.attrib), text=node.text or '',
                children=[tree(child) for child in node])

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('--cache', type=pathlib.Path, required=True)
    ap.add_argument('--output', type=pathlib.Path, required=True); a = ap.parse_args()
    with a.cache.open('rb') as f: digest = hashlib.file_digest(f, 'sha256').hexdigest()
    assert digest == EXPECTED, 'Canonical archive identity differs'
    with zipfile.ZipFile(a.cache) as z:
        entries = {}
        for e in z.infolist():
            if e.is_dir(): continue
            assert e.filename.startswith(PREFIX), e.filename
            key = e.filename[len(PREFIX):].replace('\\', '/').lower()
            assert key not in entries, key
            entries[key] = e
        by_name = collections.defaultdict(list)
        for key in entries: by_name[pathlib.PurePosixPath(key).name].append(key)
        def read_name(name):
            candidates = by_name[name.lower()]; assert len(candidates) == 1, (name, candidates)
            return z.read(entries[candidates[0]])
        names = Reader(read_name('levels_pyarraynames.bin'))
        travel_names, level_names = names.strings(), names.strings(); names.end()
        data = Reader(read_name('levels_pyarray.bin')); travel = []
        assert data.word() == len(travel_names)
        for name in travel_names:
            travel.append(dict(name=name, description_id=data.word(), entry_point=data.word(),
                               level=data.text(), location_type=data.word(), string_id=data.word()))
        assert data.word() == len(level_names); levels = []
        for index, name in enumerate(level_names):
            row = dict(index=index, name=name, stable=data.byte(), description_resource=data.text(),
                       hub=data.word(), random=data.byte(), description_id=data.word(), file=data.text())
            keys = ['level_name_id', 'initial_level_state', 'map_name_id', 'max_normal',
                    'max_hard', 'max_nightmare', 'min_normal', 'min_hard', 'min_nightmare']
            row.update({k: data.word() for k in keys}); levels.append(row)
        data.end()
        refs, documents, issues = [], {}, []
        def resolve(uri, source):
            key = uri.replace('\\', '/').lower()
            if key in entries: return key
            legacy = key.replace('data/iphone/', 'data/', 1)
            if legacy != key and legacy in entries:
                refs.append(dict(source=source, authored=uri, resolved=legacy,
                                 method='candidate_legacy_iphone_prefix_alias'))
                return legacy
            candidates = by_name[pathlib.PurePosixPath(key).name]
            if len(candidates) == 1:
                refs.append(dict(source=source, authored=uri, resolved=candidates[0],
                                 method='candidate_unique_basename'))
                return candidates[0]
            issues.append(dict(source=source, authored=uri, candidates=candidates,
                               issue='missing' if not candidates else 'ambiguous'))
            return None
        def document(key):
            if key in documents: return documents[key]
            raw = z.read(entries[key])
            try: root = ET.fromstring(raw)
            except ET.ParseError as error:
                issues.append(dict(source=key, issue='XML parse failure', error=str(error))); return None
            doc = dict(uri=key, sha256=hashlib.sha256(raw).hexdigest(), bytes=len(raw), root=tree(root))
            documents[key] = doc
            for node in root.iter():
                for attribute in ('mgp', 'mvp', 'gameplay', 'visual'):
                    value = node.get(attribute)
                    if value:
                        linked = resolve(value, key)
                        if linked: document(linked)
            return doc
        for level in levels:
            key = resolve(level['file'], 'levels/'+level['name'])
            level['definition_uri'] = key
            doc = document(key) if key else None
            level['definition_parsed'] = bool(doc)
            level['factory_support'] = 'not_implemented'
            level['native_render_verified'] = False
        objects = collections.Counter(); tags = collections.Counter(); attributes = collections.Counter()
        for doc in documents.values():
            def visit(node):
                tags[node['tag']] += 1; attributes.update(node['attributes'].keys())
                if node['tag'] == 'GameObject': objects[node['attributes'].get('gametype', '<missing>')] += 1
                for child in node['children']: visit(child)
            visit(doc['root'])
        out = dict(cache_sha256=digest, cache_files=len(entries), travel=travel, levels=levels,
                   documents=list(documents.values()), reference_alias_candidates=refs, issues=issues,
                   object_types=dict(objects), xml_tags=dict(tags),
                   attribute_occurrences=dict(attributes), procedural_execution_verified=False,
                   original_elf_behavior_verified=False, native_loader_complete=False)
        a.output.parent.mkdir(parents=True, exist_ok=True)
        a.output.write_text(json.dumps(out, indent=2)+'\n', encoding='utf-8')
        print(json.dumps(dict(levels=len(levels), definitions_parsed=sum(x['definition_parsed'] for x in levels),
                              documents=len(documents), object_types=dict(objects),
                              issues=len(issues), alias_candidates=len(refs), output=str(a.output))))

if __name__ == '__main__': main()
