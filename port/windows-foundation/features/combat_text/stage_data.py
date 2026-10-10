"""Stage only exact source combat design/localization data in this feature."""
import argparse,hashlib,json,zipfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True);a=p.parse_args();root=Path(__file__).resolve().parent/'assets';rows=[]
with zipfile.ZipFile(a.cache) as z:
    index={n.split('/files/',1)[1].lower():n for n in z.namelist() if '/files/' in n}
    uris=['data/pydata/'+n for n in ['common_text_pycst.bin','design_pycst.bin','common_text_pyarray.bin','common_text_pyarraynames.bin','common_text_pystructnames.bin']]
    uris+=['data/text/ingame.english']
    for uri in uris:
        raw=z.read(index[uri]);path=root/uri
        if path.exists() and path.read_bytes()!=raw:raise ValueError('Conflicting source staging')
        path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw);rows.append(dict(uri=uri,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
(root/'source-manifest.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps({'staged':len(rows)}))
