"""Stage the reviewed loader dependency migration; never mutate live targets."""
from pathlib import Path
import difflib, hashlib, json, re, zipfile

ROOT = Path(__file__).resolve().parents[3]
PRIVATE = Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
OUT = ROOT / 'port/level-loader/reports/root-loading-v50'
STAGE = OUT / 'stage'
PACKETS = {
    'native-gslevel-loading-v49-d2c6e02d9d55ee73.zip': '46a24ce1eda17c491487e90b7db3cb540d507bb79b928848912182fb67f32df2',
    'loader-provider-binding-v47-84a3e2d2e34cee75.zip': 'c336599fd2516603ca77c6aa375087c7f7009a38b1d35b062fb1e617e3a32e3b',
}

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    for name, expected in PACKETS.items():
        p = PRIVATE / 'port/level-loader/reports' / name
        actual = hashlib.sha256(p.read_bytes()).hexdigest()
        if actual != expected:
            raise RuntimeError(f'Packet mismatch: {name}')
    # Explicit narrow dependency set, not a whole-worktree/vendor overwrite.
    new = ['gslevel_update_v44.hpp', 'gslevel_update_v44_level_borrow.hpp',
           'level_source_loading_update_v46.hpp', 'level_source_loading_v43.hpp',
           'level_source_loading_v43.cpp', 'level_update_loading_v46.hpp',
           'lifecycle_v36.hpp', 'lifecycle_v36.cpp', 'lifecycle_v36_counter_borrow.hpp',
           'native_gslevel_frame_v45.cpp', 'native_gslevel_frame_v45.hpp',
           'native_gslevel_loading_connection_v49.hpp', 'stage_loader_v38_root_file.hpp',
           'stage_loader_v38_init_post.hpp', 'stage_loader_v38_stage10.hpp',
           'stage_loader_v39_file_counter.hpp', 'stage_loader_v46_early.hpp',
           'stage_loader_v46_physical.hpp', 'level_file_walk_v1.cpp',
           'object_entry_v1.cpp', 'object_initialization_v1.cpp',
           'resource_paths_v1.cpp', 'user_properties_v1.cpp', 'visual_transform_v1.cpp']
    borrowed = ['level_constructor_v3.hpp', 'native_level_application_v25.cpp',
                'native_gslevel_runtime_v27.hpp', 'native_gslevel_runtime_v27.cpp']
    migrated = ['retained_level_module_graph_v1.hpp', 'retained_level_module_graph_v1.cpp']
    sources = [(f'port/level-loader/{n}', PRIVATE / 'port/level-loader' / n) for n in new + migrated]
    vendor = PRIVATE / 'port/level-loader/vendor/character-rng-integration-v5-loading'
    sources += [(f'port/level-loader/{n}', vendor / 'port/level-loader' / n) for n in borrowed]
    sources += [(f'port/level-world/{n}', vendor / 'port/level-world' / n)
                for n in ['canonical_object_manager_v1.hpp', 'canonical_object_manager_v1.cpp']]
    # Missing companion parser headers are imported only for this explicit
    # implementation closure; existing API headers are checked, not replaced.
    known={target for target,_ in sources}
    queue=[PRIVATE/'port/level-loader'/n for n in new if n.endswith('.cpp')]
    while queue:
        current=queue.pop()
        for name in re.findall(r'^#include "([^"\n]+)"',current.read_text(),re.M):
            source=(current.parent/name).resolve()
            if not source.is_file() or source.suffix not in ('.hpp','.h','.inc'):continue
            try: relative=source.relative_to(PRIVATE/'port/level-loader')
            except ValueError:continue
            target='port/level-loader/'+relative.as_posix()
            live=ROOT/target
            if live.exists() or target in known:continue
            known.add(target);sources.append((target,source));queue.append(source)
    rows, chunks = [], []
    for target, source in sources:
        if source.name in {'level_file_walk_v1.cpp','object_entry_v1.cpp','object_initialization_v1.cpp',
                           'resource_paths_v1.cpp','user_properties_v1.cpp','visual_transform_v1.cpp'}:
            header=source.with_suffix('.hpp')
            live=ROOT/Path(target).with_suffix('.hpp')
            if live.exists() and live.read_text()!=header.read_text():
                raise RuntimeError(f'Companion implementation header mismatch: {target}')
        data = source.read_bytes()
        if target == 'port/level-loader/retained_level_module_graph_v1.hpp':
            text=data.decode('utf-8')
            needle='    const auto& level()const noexcept{return input_.level;}'
            if needle not in text:raise RuntimeError('Preparation owner borrow anchor changed')
            text=text.replace(needle,needle+'\n    const auto& candidate_owner_v50()const noexcept{return input_.candidate_owner;}')
            data=text.encode('utf-8')
        if target == 'port/level-world/canonical_object_manager_v1.hpp':
            text=data.decode('utf-8')
            text=text.replace('CanonicalObjectManagerServicesV1 services_;',
                'CanonicalObjectManagerServicesV1 services_;\n'
                ' // Native ownership receipt only: invalidated at source mutation prefixes.\n'
                ' // Never substitutes source initialized/count/phase fields.\n'
                ' bool source_c1_fresh_v50_{};')
            text=text.replace('entries_.try_emplace(0);next_key_=1;',
                'entries_.try_emplace(0);next_key_=1;source_c1_fresh_v50_=true;')
            text=text.replace('void begin_frame(std::uint32_t v)noexcept{frame_=v;}',
                'void begin_frame(std::uint32_t v)noexcept{source_c1_fresh_v50_=false;frame_=v;}')
            needle=' using SourceActorBorrowV38=CanonicalObjectBorrowV1;'
            addition='''
 class FreshSourceBorrowV50 {
  friend class CanonicalObjectManagerV1;
  std::shared_ptr<CanonicalObjectManagerV1> owner_;
 public:
  bool same_fresh_producer(const std::shared_ptr<CanonicalObjectManagerV1>& owner)const noexcept{
   return owner_&&owner&&owner_.get()==owner.get()&&!owner_.owner_before(owner)&&!owner.owner_before(owner_)&&owner_->source_c1_fresh_v50_&&owner_->init_phase7c_==0;
  }
 };
 bool borrow_fresh_source_v50(const std::shared_ptr<CanonicalObjectManagerV1>& owner,FreshSourceBorrowV50& out,std::string& error){
  if(!owner||owner.get()!=this||!source_c1_fresh_v50_||init_phase7c_!=0){error="Required authentic unconsumed source ObjectManager C1 producer";return false;}
  FreshSourceBorrowV50 next;next.owner_=owner;out=std::move(next);error.clear();return true;
 }
'''
            if needle not in text:raise RuntimeError('Fresh manager anchor changed')
            text=text.replace(needle,addition+needle)
            data=text.encode('utf-8')
        if target == 'port/level-world/canonical_object_manager_v1.cpp':
            text=data.decode('utf-8')
            text=text.replace('pending_.push_back(actor.identity);',
                'source_c1_fresh_v50_=false;pending_.push_back(actor.identity);')
            text=text.replace('const auto bits=next_key_++;',
                'source_c1_fresh_v50_=false;const auto bits=next_key_++;')
            text=text.replace('auto [it,inserted]=entries_.try_emplace(handle.key);(void)inserted;',
                'auto [it,inserted]=entries_.try_emplace(handle.key);if(inserted)source_c1_fresh_v50_=false;')
            text=text.replace('auto& entry=entries_[out.key];entry.actor=std::move(actor);',
                'source_c1_fresh_v50_=false;auto& entry=entries_[out.key];entry.actor=std::move(actor);')
            data=text.encode('utf-8')
        if target == 'port/level-loader/native_gslevel_runtime_v27.hpp':
            text=data.decode('utf-8')
            needle='const GSLevelFieldsV2<CanonicalLevelContextV1>& fields()const noexcept{return fields_;}'
            if needle not in text:
                raise RuntimeError('Runtime accessor anchor changed')
            text=text.replace(needle,
                'bool owns_globals_v50(const std::shared_ptr<NativeGSLevelGlobalsV27>& actual)const noexcept{\n'
                '  return actual&&globals_.get()==actual.get()&&!globals_.owner_before(actual)&&!actual.owner_before(globals_);\n'
                ' }\n '+needle)
            data=text.encode('utf-8')
        before_path = ROOT / target
        before = before_path.read_bytes() if before_path.exists() else b''
        if data == before:
            continue
        p = STAGE / target
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(data)
        old = before.decode('utf-8').replace('\r\n', '\n')
        after = data.decode('utf-8').replace('\r\n', '\n')
        chunks.extend(difflib.unified_diff(old.splitlines(True), after.splitlines(True),
                      fromfile=f'a/{target}' if before_path.exists() else '/dev/null',
                      tofile=f'b/{target}'))
        rows.append({'path': target, 'source': str(source),
                     'before_sha256': hashlib.sha256(before).hexdigest() if before_path.exists() else None,
                     'after_sha256': hashlib.sha256(data).hexdigest(),
                     'patch_lines': sum(1 for _ in difflib.unified_diff(old.splitlines(), after.splitlines()))})
    (OUT / 'loader-migration.patch').write_text(''.join(chunks), encoding='utf-8', newline='\n')
    (OUT / 'migration-manifest.json').write_text(json.dumps({'packets': PACKETS, 'files': rows}, indent=2), encoding='utf-8')
    print(json.dumps({'files': len(rows), 'rows': [{k:r[k] for k in ('path','patch_lines')} for r in rows]}, indent=2))

if __name__ == '__main__':
    main()
