"""Summarize factory inventory without confusing MGX links with runtime types."""
import collections,json,pathlib
root=pathlib.Path(__file__).resolve().parents[1]
receipt=root/'reports/original-object-factories.json'
r=json.loads(receipt.read_text());rows=r['unregistered_declarations']
summary={'unregistered_by_tag':dict(collections.Counter(x['tag'] for x in rows)),
         'unregistered_by_root':dict(collections.Counter(x['root_tag'] for x in rows)),
         'unregistered_by_extension':dict(collections.Counter(pathlib.PurePosixPath(x['uri']).suffix for x in rows)),
         'unregistered_unique_documents':len({x['uri'] for x in rows}),
         'unregistered_mgp_documents':dict(collections.Counter(x['uri'] for x in rows if x['uri'].lower().endswith('.mgp'))),
         'samples':rows[:2]}
print(json.dumps(summary,indent=2))
