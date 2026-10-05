"""Bind the current isolated source-preparation milestone to files and builds."""
import hashlib,json,pathlib
ROOT=pathlib.Path(__file__).resolve().parents[3]
LOADER=ROOT/'port/level-loader';BUILD=ROOT.parent/'build'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
sources=sorted(p for p in LOADER.rglob('*') if p.is_file() and
    'reports' not in p.relative_to(LOADER).parts and '__pycache__' not in p.parts)
sources += [ROOT/'port/asset-payloads'/n for n in ('zip_asset_pack_v1.cpp','zip_asset_pack_v1.hpp')]
binaries={}
for platform in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    directory=BUILD/platform
    for name in ('libdh2_level_loader.a','libdh2_loader_xml_reference.a','libdh2_loader_cache.a',
                 'dh2_loader_fixed_sources_probe','dh2_loader_xml_ownership'):
        path=directory/name
        if path.exists():binaries[path.relative_to(BUILD).as_posix()]=sha(path)
receipts={}
for name in ('xml-original-comparison.json','resource-path-original.json','resource-path-differential.json',
             'swamp-fixed-sources-host.json','swamp-fixed-sources-sanitizers.json','fixed-sources-level-coverage.json'):
    path=LOADER/'reports'/name;receipts[name]=sha(path)
report={'scope':'Isolated source-preparation checkpoint; not a finished loader, APK or integration handoff.',
    'source_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in sources},
    'binary_sha256':binaries,'receipt_sha256':receipts,'map_render_verified':False,
    'runtime_factory_abi_agreed':False,'integrated_APK_built':False,'full_loader_verified':False}
(LOADER/'reports/source-preparation-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),'full_loader_verified':False}))
