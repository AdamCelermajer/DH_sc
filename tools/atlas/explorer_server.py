"""Read-only localhost viewer for generated atlas files and indexed source assets."""
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import json
import mimetypes
from urllib.parse import unquote, urlparse, parse_qs

from common import physical_path, read_json

def source_mime(path):
    if str(path).lower().endswith(('.lua','.luac','.xml','.lightset_xml','.mlx','.mgp','.mvp','.mgx','.mvx','.srt','.strings')):
        return 'text/plain; charset=utf-8'
    return mimetypes.guess_type(str(path))[0] or 'application/octet-stream'


class Handler(SimpleHTTPRequestHandler):
    def __init__(self,*args,ctx=None,assets=None,**kwargs):
        self.ctx=ctx;self.assets=assets
        super().__init__(*args,directory=str(ctx.out),**kwargs)

    def do_GET(self):
        url=urlparse(self.path)
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
    server=ThreadingHTTPServer(('127.0.0.1',port),partial(Handler,ctx=ctx,assets=assets))
    print(f'Atlas viewer: http://127.0.0.1:{server.server_port}/ (read-only)',flush=True)
    try:server.serve_forever()
    except KeyboardInterrupt:pass
    finally:server.server_close()
    return 0
