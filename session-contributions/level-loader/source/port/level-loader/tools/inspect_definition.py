"""Read unchanged cache bytes or the captured native tree for one resource."""
import argparse,json,pathlib,zipfile
ap=argparse.ArgumentParser()
ap.add_argument('uri')
ap.add_argument('--cache',type=pathlib.Path)
ap.add_argument('--native',type=pathlib.Path)
ap.add_argument('--summary',action='store_true')
a=ap.parse_args()
if a.cache:
    with zipfile.ZipFile(a.cache) as z:
        print(z.read('com.gameloft.android.GAND.GloftD2SS/files/'+a.uri).decode('utf-8'))
elif a.native:
    rows=json.loads(a.native.read_text())['documents']
    row=next(r for r in rows if r['uri']==a.uri)
    if a.summary:
        root=row['root'];row={'uri':row['uri'],'xml_error':row['xml_error'],'root_tag':root['tag'],
            'root_attributes':root['attributes'],'child_count':len(root['children']),
            'first_children':root['children'][:3]}
    print(json.dumps(row,indent=2))
else:ap.error('Supply --cache or --native')
