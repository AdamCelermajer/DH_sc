#!/usr/bin/env python3
"""Draw an original BRES quad with an original GLSL pair in host WebGL1."""

import argparse
import base64
import ctypes as c
import hashlib
from html import unescape
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import zipfile

ROOT = Path(__file__).resolve().parent
REPO = ROOT.parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(REPO / 'port/asset-payloads/tools'))
from api import Attribute, Input, Primitive, bind, string  # noqa: E402
sys.path.insert(0, str(REPO / 'port/material-bindings/tools'))
import audit_cache as material_api  # noqa: E402


class TextureView(c.Structure):
    _fields_ = [
        ('bytes', c.c_void_p), ('size', c.c_size_t),
        ('payload', c.c_void_p), ('payload_size', c.c_size_t),
        ('payload_offset', c.c_size_t), ('width', c.c_uint32),
        ('height', c.c_uint32), ('flags', c.c_uint32),
        ('bits_per_pixel', c.c_uint32), ('mipmaps', c.c_uint32),
        ('surfaces', c.c_uint32), ('alpha_mask', c.c_uint32),
        ('tga_descriptor', c.c_uint32), ('kind', c.c_uint32),
        ('format', c.c_uint32),
    ]


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def locate_chrome():
    for executable in ('chrome', 'google-chrome', 'chromium', 'msedge'):
        found = shutil.which(executable)
        if found:
            return Path(found)
    for path in (
        Path(r'C:\Program Files\Google\Chrome\Application\chrome.exe'),
        Path(r'C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe'),
    ):
        if path.is_file():
            return path
    raise FileNotFoundError('Chrome or Edge is required for the WebGL1 check')


def build_shared(output, sources, compiler):
    output.parent.mkdir(parents=True, exist_ok=True)
    cmd = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
           '-shared', '-fPIC', *[str(REPO / source) for source in sources],
           '-o', str(output)]
    if os.name == 'nt':
        cmd += ['-static-libgcc', '-static-libstdc++']
    subprocess.run(cmd, check=True)


def extract_mesh(library, fixture):
    raw = fixture.read_bytes()
    data = Input(bind(library), raw)
    dll = data.dll
    assert dll.dh2_bres_library_count(c.byref(data.view), 7) >= 1
    mesh = data.mesh(0)
    assert mesh.primitives >= 1 and mesh.vertices <= 65535
    primitive = Primitive()
    assert dll.dh2_mesh_primitive(c.byref(mesh), 0, c.byref(primitive)) == 0
    assert primitive.collada_type == 0 and primitive.index_count % 3 == 0
    assert primitive.attributes[0] >= 0
    position = Attribute()
    assert dll.dh2_mesh_attribute(c.byref(mesh), primitive.attributes[0], c.byref(position)) == 0
    assert position.components >= 3
    positions = []
    components = (c.c_float * 16)()
    for i in range(mesh.vertices):
        assert dll.dh2_attribute_read(c.byref(position), i, components)
        positions.append([float(components[j]) for j in range(3)])
    uvs = []
    if primitive.attributes[4] >= 0:
        uv = Attribute()
        assert dll.dh2_mesh_attribute(c.byref(mesh), primitive.attributes[4], c.byref(uv)) == 0
        assert uv.components >= 2
        for i in range(mesh.vertices):
            assert dll.dh2_attribute_read(c.byref(uv), i, components)
            uvs.append([float(components[0]), float(components[1])])
    indices = []
    value = c.c_uint32()
    for i in range(primitive.index_count):
        assert dll.dh2_index_read(c.byref(primitive), i, c.byref(value))
        assert value.value < mesh.vertices
        indices.append(value.value)
    return {'fixture_sha256': sha256(raw), 'mesh_id': string(mesh.id),
            'material': string(primitive.material), 'positions': positions,
            'uvs': uvs, 'indices': indices}


def select_diffuse_texture(library, fixture, material_name, cache_root):
    raw = fixture.read_bytes()
    dll = material_api.bind(library)
    buffer = c.create_string_buffer(raw)
    view = material_api.Bres()
    assert dll.dh2_bres_open(c.byref(view), buffer, len(raw)) == 0
    for i in range(dll.dh2_bres_library_count(c.byref(view), 6)):
        material = material_api.Material()
        assert dll.dh2_material_record(c.byref(material), c.byref(view), i) == 0
        if material_api.decode(material.id) != material_name:
            continue
        for j in range(material.parameter_count):
            parameter = material_api.Parameter()
            assert dll.dh2_material_parameter(c.byref(parameter), c.byref(material), j) == 0
            if parameter.type_code != 11 or 'diffuse' not in material_api.decode(parameter.id).casefold():
                continue
            reference = material_api.ImageRef()
            assert dll.dh2_material_sampler_image(c.byref(reference), c.byref(material), j) == 0
            if reference.index < 0:
                raise ValueError('Diffuse sampler is unbound')
            source_path = material_api.decode(reference.source_path)
            filename = source_path.replace('\\', '/').split('/')[-1]
            texture = cache_root / 'data/3d/textures' / filename
            if not texture.is_file():
                raise FileNotFoundError(f'Diffuse texture not found: {texture}')
            return {'parameter': material_api.decode(parameter.id),
                    'image_index': reference.index, 'image_id': material_api.decode(reference.id),
                    'source_path': source_path, 'path': texture}
        raise ValueError(f'No diffuse image parameter in material {material_name}')
    raise ValueError(f'Primitive material not found in BRES: {material_name}')


def infer_cache_root(fixture):
    for parent in fixture.resolve().parents:
        if (parent / 'data/3d/textures').is_dir():
            return parent
    raise FileNotFoundError('Pass --cache-root for a fixture outside the extracted cache')


def decode_texture(library, path):
    raw = path.read_bytes()
    source = c.create_string_buffer(raw)
    dll = c.CDLL(str(library.resolve()))
    dll.dh2_texture_open.argtypes = [c.POINTER(TextureView), c.c_void_p, c.c_size_t]
    dll.dh2_texture_open.restype = c.c_uint32
    dll.dh2_texture_decode_rgba8.argtypes = [c.c_void_p, c.c_size_t, c.c_size_t,
                                              c.c_void_p, c.c_size_t]
    dll.dh2_texture_decode_rgba8.restype = c.c_uint32
    view = TextureView()
    status = dll.dh2_texture_open(c.byref(view), source, len(raw))
    if status or view.format not in (1, 2):
        raise ValueError(f'Expected supported PVRTC diffuse texture, got status={status}, format={view.format}')
    stride = view.width * 4
    output = c.create_string_buffer(stride * view.height)
    status = dll.dh2_texture_decode_rgba8(output, len(output), stride, source, len(raw))
    if status:
        raise ValueError(f'PVRTC decoder rejected diffuse texture: {status}')
    return {'width': view.width, 'height': view.height,
            'raw_sha256': sha256(raw), 'rgba_sha256': sha256(output.raw),
            'rgba': output.raw}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fixture', type=Path, required=True)
    parser.add_argument('--shaders', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=ROOT / 'build')
    parser.add_argument('--chrome', type=Path)
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--mode', choices=('flat', 'textured'), default='flat')
    parser.add_argument('--cache-root', type=Path,
                        help='Extracted complete cache root; inferred from the fixture when omitted')
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    library = output / ('mesh_decoder.dll' if sys.platform == 'win32' else 'libmesh_decoder.so')
    build_shared(library, ('port/asset-payloads/payloads.cpp',
                           'port/engine-resources/resources.cpp'), args.cxx)
    mesh = extract_mesh(library, args.fixture)
    suffix = '.dll' if sys.platform == 'win32' else '.so'
    texture_report = {}
    if args.mode == 'textured':
        if len(mesh['uvs']) != len(mesh['positions']):
            raise ValueError('The selected primitive lacks a complete UV stream')
        materials_library = output / ('material_bindings' + suffix)
        build_shared(materials_library, ('port/material-bindings/bindings.cpp',
                                         'port/engine-resources/resources.cpp'), args.cxx)
        cache_root = args.cache_root or infer_cache_root(args.fixture)
        choice = select_diffuse_texture(materials_library, args.fixture,
                                        mesh['material'], cache_root)
        texture_library = output / ('texture_decoder' + suffix)
        build_shared(texture_library, ('port/texture-assets/texture.cpp',
                                       'port/texture-assets/decode.cpp'), args.cxx)
        decoded = decode_texture(texture_library, choice['path'])
        texture_report = {'texture_file': str(choice['path'].resolve()),
                          'texture_source_path': choice['source_path'],
                          'texture_image_id': choice['image_id'],
                          'texture_image_index': choice['image_index'],
                          'material_parameter': choice['parameter'],
                          'texture_width': decoded['width'], 'texture_height': decoded['height'],
                          'texture_source_sha256': decoded['raw_sha256'],
                          'texture_rgba_sha256': decoded['rgba_sha256'],
                          'texture_decoder_sha256': sha256(texture_library.read_bytes()),
                          'material_bindings_sha256': sha256(materials_library.read_bytes())}
    with zipfile.ZipFile(args.shaders) as archive:
        vertex_name, fragment_name = (
            ('UnlitOneTextureAndVertexColorVP.glsl', 'UnlitTexturedFP.glsl')
            if args.mode == 'textured' else
            ('UnlitMaterialColorVP.glsl', 'UnlitMaterialColorFP.glsl'))
        vertex_source = archive.read(vertex_name).decode('utf-8')
        fragment_source = archive.read(fragment_name).decode('utf-8')
    payload = {'mode': args.mode, 'positions': mesh['positions'],
               'indices': mesh['indices'], 'uvs': mesh['uvs'],
               'vertex_shader': vertex_source, 'fragment_shader': fragment_source}
    if args.mode == 'textured':
        payload.update(texture_width=decoded['width'], texture_height=decoded['height'],
                       texture_rgba_base64=base64.b64encode(decoded['rgba']).decode('ascii'))
    encoded = base64.b64encode(json.dumps(payload).encode('utf-8')).decode('ascii')
    html = (ROOT / 'preview.html').read_text(encoding='utf-8').replace('@@INPUT_BASE64@@', encoded)
    name = 'preview' if args.mode == 'flat' else 'preview-textured'
    page = output / (name + '.html')
    page.write_text(html, encoding='utf-8')
    chrome = args.chrome or locate_chrome()
    command = [str(chrome), '--headless=new', '--no-sandbox',
               '--disable-crash-reporter', '--disable-breakpad',
               '--disable-extensions', '--no-first-run',
               '--disable-gpu-sandbox', '--enable-webgl', '--use-angle=swiftshader',
               '--enable-unsafe-swiftshader', '--disable-background-networking',
               '--user-data-dir=' + str(output / 'chrome-profile'),
               '--virtual-time-budget=5000', '--dump-dom', page.as_uri()]
    process = subprocess.run(command, capture_output=True, text=True,
                             encoding='utf-8', errors='replace', timeout=90)
    match = re.search(r'<pre id="result"[^>]*data-complete="true"[^>]*>(.*?)</pre>',
                      process.stdout, re.DOTALL)
    if not match:
        raise RuntimeError('Chrome did not return a completed render report: '
                           + process.stderr[-1500:] + process.stdout[-1500:])
    render = json.loads(unescape(match.group(1)))
    if not render.get('ok'):
        raise RuntimeError('WebGL render failed: ' + json.dumps(render))
    png = base64.b64decode(render.pop('png_base64'), validate=True)
    assert png.startswith(b'\x89PNG\r\n\x1a\n')
    image = output / (name + '.png')
    image.write_bytes(png)
    report = {'all_checks_passed': True,
              'scope': 'first BRES mesh/primitive and original GLSL; diagnostic view only',
              'mode': args.mode,
              'fixture': str(args.fixture.resolve()), 'fixture_sha256': mesh['fixture_sha256'],
              'mesh_id': mesh['mesh_id'], 'primitive_material': mesh['material'],
              'positions': mesh['positions'], 'uvs': mesh['uvs'], 'indices': mesh['indices'],
              'vertex_shader': vertex_name, 'fragment_shader': fragment_name,
              'shader_archive_sha256': sha256(args.shaders.read_bytes()),
              'vertex_shader_sha256': sha256(vertex_source.encode('utf-8')),
              'fragment_shader_sha256': sha256(fragment_source.encode('utf-8')),
              'decoder_sha256': sha256(library.read_bytes()), 'preview_png_sha256': sha256(png),
              'browser': str(chrome), **texture_report, **render}
    report_path = output / ('report.json' if args.mode == 'flat' else 'report-textured.json')
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
