"""Read-only localhost viewer for generated atlas files and indexed source assets."""
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import json
import mimetypes
from pathlib import Path
from urllib.parse import unquote, urlparse, parse_qs

from common import physical_path, read_json
from graph_store import GraphStore

MAX_NATIVE_SOURCE_BYTES = 2 * 1024 * 1024
NATIVE_SOURCE_ROOT = Path('ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode')

def source_mime(path):
    if str(path).lower().endswith(('.lua','.luac','.xml','.lightset_xml','.mlx','.mgp','.mvp','.mgx','.mvx','.srt','.strings')):
        return 'text/plain; charset=utf-8'
    return mimetypes.guess_type(str(path))[0] or 'application/octet-stream'


class Handler(SimpleHTTPRequestHandler):
    def __init__(self,*args,ctx=None,assets=None,graph=None,**kwargs):
        self.ctx=ctx;self.assets=assets;self.graph=graph
        super().__init__(*args,directory=str(ctx.out),**kwargs)

    def _json(self, value, status=200):
        payload=json.dumps(value,ensure_ascii=False,separators=(',',':')).encode('utf-8')
        self.send_response(status);self.send_header('Content-Type','application/json; charset=utf-8')
        self.send_header('Content-Length',str(len(payload)));self.send_header('Cache-Control','no-store')
        self.end_headers();self.wfile.write(payload)

    def do_GET(self):
        url=urlparse(self.path)
        if url.path in ('/','/index.html'):
            # Server mode always uses the small editable source template. The
            # generated index.html remains the separate portable full snapshot.
            template=Path(__file__).resolve().parent/'templates'/'explorer.html'
            source=template.read_text(encoding='utf-8').replace('__ATLAS_TITLE__','Dungeon Hunter II · Original Data Atlas')
            payload=source.replace('__ATLAS_GZIP_BASE64__','').encode('utf-8')
            self.send_response(200);self.send_header('Content-Type','text/html; charset=utf-8')
            self.send_header('Content-Length',str(len(payload)));self.send_header('Cache-Control','no-store')
            self.end_headers();self.wfile.write(payload);return
        if url.path=='/api/meta':
            self._json(self.graph.meta());return
        if url.path=='/api/search':
            query=parse_qs(url.query,keep_blank_values=True)
            try:
                offset=int(query.get('offset',['0'])[0]);limit=int(query.get('limit',['50'])[0])
                result=self.graph.search(query.get('q',[''])[0],query.get('category',[''])[0],offset,limit)
            except (ValueError,TypeError,OverflowError):
                self._json({'error':'Invalid search paging parameters'},400);return
            self._json(result);return
        if url.path=='/api/entity':
            node_id=parse_qs(url.query,keep_blank_values=True).get('id',[''])[0]
            try:result=self.graph.entity(node_id)
            except ValueError as error:self._json({'error':str(error)},400);return
            if result is None:self._json({'error':'Unknown entity'},404);return
            self._json(result);return
        if url.path=='/api/native':
            query=parse_qs(url.query,keep_blank_values=True)
            try:
                result=self.graph.native_page(query.get('id',[''])[0],
                    int(query.get('script_offset',['0'])[0]),int(query.get('function_offset',['0'])[0]),
                    int(query.get('limit',['16'])[0]))
            except (ValueError,TypeError,OverflowError) as error:
                self._json({'error':str(error) or 'Invalid native paging parameters'},400);return
            if result is None:self._json({'error':'Unknown entity'},404);return
            self._json(result);return
        if url.path=='/api/native-source':
            query=parse_qs(url.query,keep_blank_values=True)
            try:result=self.graph.native_source(query.get('function_id',[''])[0],query.get('address_index',['0'])[0])
            except (ValueError,TypeError,OverflowError) as error:
                self._json({'error':str(error) or 'Invalid native source reference'},400);return
            if result is None:self._json({'error':'No indexed native pseudocode excerpt'},404);return
            if query.get('full')==['1']:
                try:
                    source_path=result.get('pseudocode_path','')
                    marker='.local-inputs/'
                    if not isinstance(source_path,str) or not source_path.startswith(marker):
                        raise ValueError('Indexed address does not name an individual pseudocode file')
                    relative=Path(source_path[len(marker):])
                    if relative.is_absolute() or '..' in relative.parts or not relative.as_posix().startswith(NATIVE_SOURCE_ROOT.as_posix()+'/'):
                        raise ValueError('Indexed source is outside the approved individual pseudocode corpus')
                    root=(Path(self.ctx.inputs)/NATIVE_SOURCE_ROOT).resolve()
                    path=(Path(self.ctx.inputs)/relative).resolve()
                    if root not in path.parents or not path.is_file():
                        raise ValueError('Indexed individual pseudocode file is unavailable')
                    size=path.stat().st_size
                    if size>MAX_NATIVE_SOURCE_BYTES:
                        raise ValueError('Indexed pseudocode file exceeds the 2 MB preview limit')
                    self.send_response(200);self.send_header('Content-Type','text/plain; charset=utf-8')
                    self.send_header('Content-Length',str(size));self.send_header('Cache-Control','no-store')
                    self.end_headers()
                    with path.open('rb') as source:
                        while chunk:=source.read(64*1024):self.wfile.write(chunk)
                    return
                except (OSError,ValueError) as error:
                    excerpt=result.get('snippet') or '[No indexed excerpt is available.]'
                    message=('Full individual pseudocode unavailable: '+str(error)+'\n\n'
                             'Indexed excerpt for '+str(result.get('address') or 'unknown address')+':\n'+excerpt)
                    payload=message.encode('utf-8')
                    self.send_response(404);self.send_header('Content-Type','text/plain; charset=utf-8')
                    self.send_header('Content-Length',str(len(payload)));self.send_header('Cache-Control','no-store')
                    self.end_headers();self.wfile.write(payload);return
            self._json(result);return
        if url.path=='/api/closure':
            query=parse_qs(url.query,keep_blank_values=True)
            try:result=self.graph.closure(query.get('id',[''])[0],query.get('direction',['out'])[0])
            except ValueError as error:self._json({'error':str(error)},400);return
            if result is None:self._json({'error':'Unknown entity'},404);return
            self._json(result);return
        if url.path.startswith('/api/source/'):
            id=unquote(url.path[len('/api/source/'):]).removeprefix('asset:')
            asset=self.assets.get(id)
            if not asset:self.send_error(404,'No indexed source asset');return
            path=physical_path(self.ctx,asset['path'])
            if not path.is_file():self.send_error(404,'Source file unavailable');return
            if parse_qs(url.query).get('decoded')==['1'] and asset.get('decode_recipe') not in (None,'identity','preserve-raw'):
                import package_index
                try:payload=package_index.read_resource(self.ctx,asset)
                except (ValueError,KeyError):self.send_error(422,'No proven decoder for this resource');return
                logical=(asset.get('logical_paths') or [asset['relative']])[0]
                mime=source_mime(logical)
                self.send_response(200);self.send_header('Content-Type',mime);self.send_header('Content-Length',str(len(payload)))
                self.end_headers();self.wfile.write(payload);return
            mime=source_mime(asset.get('relative',path.name))
            self.send_response(200);self.send_header('Content-Type',mime);self.send_header('Content-Length',str(path.stat().st_size))
            self.send_header('Cache-Control','no-store');self.end_headers()
            with path.open('rb') as source:
                while chunk:=source.read(256*1024):self.wfile.write(chunk)
            return
        if url.path=='/api/document':
            relative=parse_qs(url.query).get('path',[''])[0]
            path=(self.ctx.out/relative).resolve()
            if self.ctx.out.resolve() not in path.parents or path.suffix not in ('.md','.json') or not path.is_file():
                self.send_error(404,'No atlas document');return
            payload=path.read_bytes();self.send_response(200);self.send_header('Content-Type','text/plain; charset=utf-8')
            self.send_header('Content-Length',str(len(payload)));self.end_headers();self.wfile.write(payload);return
        relative=unquote(url.path).lstrip('/') or 'index.html'
        target=(self.ctx.out/relative).resolve()
        if target!=self.ctx.out.resolve() and self.ctx.out.resolve() not in target.parents:
            self.send_error(403,'Outside atlas directory');return
        super().do_GET()

    def do_POST(self):self.send_error(405,'This viewer is read-only')
    def do_PUT(self):self.send_error(405,'This viewer is read-only')
    def do_DELETE(self):self.send_error(405,'This viewer is read-only')


def run(ctx,port=18789):
    assets={a['id']:a for a in read_json(ctx.out/'data/assets.json',[])}
    started=__import__('time').perf_counter()
    graph=GraphStore.open(ctx)
    build_time=__import__('time').perf_counter()-started
    server=ThreadingHTTPServer(('127.0.0.1',port),partial(Handler,ctx=ctx,assets=assets,graph=graph))
    print(f'Atlas graph index ready: {graph.counts.get("nodes",0):,} nodes, '
          f'{graph.counts.get("edges",0):,} edges in {build_time:.2f}s ({graph.db_path.name})',flush=True)
    print(f'Atlas viewer: http://127.0.0.1:{server.server_port}/ (read-only)',flush=True)
    try:server.serve_forever()
    except KeyboardInterrupt:pass
    finally:server.server_close()
    return 0
