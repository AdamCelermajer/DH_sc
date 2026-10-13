"""Ensure repeated package lookups cannot hide changed source-map bytes."""
import package_index


def run(ctx):
    tests=[]
    raw=b'data/file000001.dat:data/a.xml\n'
    tsv=b'data/file000001.dat\tdata/a.xml\n'
    expected={'data/file000001.dat':'data/a.xml'}
    first=package_index._validated_mapping(raw,tsv)
    tests.append({'name':'exact matching original maps are admitted','passed':first==expected})
    for name,new_raw,new_tsv in (
        ('same-size pack-map mutation is rejected',raw.replace(b'a.xml',b'b.xml'),tsv),
        ('same-size transcription mutation is rejected',raw,tsv.replace(b'a.xml',b'b.xml'))):
        try:
            package_index._validated_mapping(new_raw,new_tsv)
            rejected=False
        except ValueError:
            rejected=True
        tests.append({'name':name,'passed':rejected})
    first=package_index._read_mapping(ctx)
    second=package_index._read_mapping(ctx)
    first.clear()
    tests.append({'name':'callers cannot alter the validated package map',
                  'passed':len(second)==6145 and package_index._read_mapping(ctx)==second})
    return {'passed':all(t['passed'] for t in tests),'tests':tests}
