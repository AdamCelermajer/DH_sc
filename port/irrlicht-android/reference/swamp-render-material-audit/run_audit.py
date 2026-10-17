#!/usr/bin/env python3
"""Reproduce the SWAMP AlphaMap shader/UV/channel audit without exporting source shader text."""
from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import zipfile


HERE = Path(__file__).resolve().parent
REPO = HERE.parents[3]
SMOKE = REPO / 'port/irrlicht-android/swamp-smoke'

UV_SOURCES = [
    HERE / 'uv_probe.cpp',
    REPO / 'port/world-data/world.cpp',
    REPO / 'port/world-data/world_scene.cpp',
    REPO / 'port/android-app/scene_buffers.cpp',
    REPO / 'port/skin-payloads/skin.cpp',
    REPO / 'port/animation-pose/pose.cpp',
    REPO / 'port/animation-values/values.cpp',
    REPO / 'port/animation-timeline/timeline.cpp',
    REPO / 'port/animation-mixing/mixing.cpp',
    REPO / 'port/animation-layers/layers.cpp',
    REPO / 'port/scene-draw/draw.cpp',
    REPO / 'port/scene-payloads/scene.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/engine-math/math.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
]

SHADER_PREFIX = (
    'dh2-reconstruction/recovered/assets/source-data/'
    'com.gameloft.android.GAND.GloftD2SS/files/'
    'shaders.pak.contents/'
)
SHADER_MEMBERS = {
    'vertex': SHADER_PREFIX + 'GL_Diffuse_L1_iPhone_VS.glsl',
    'fragment': SHADER_PREFIX + 'GL_Diffuse_L1_iPhone_FS.glsl',
}
PRE_CORRECTION_SOURCE_SHA256 = {
    'port/irrlicht-android/swamp-smoke/main.cpp':
        '8a90798f21703a95f4e0f737305a962d8e79d4170e3310079e972c4e41867d77',
    'port/irrlicht-android/swamp-smoke/alpha_map_policy.hpp':
        '1a6daaea5e3ca9405d89f103d084adfa37565393c69f9c04c9a29097ff0d07ed',
    'port/irrlicht-android/swamp-smoke/tests/alpha_map_policy.cpp':
        'd44a6336791d62f384d6ddb141ee12a777c17376f33a43c6d1b3b8cef4330a37',
    'port/irrlicht-android/swamp-smoke/tests/run_host.py':
        '15867b362ad6d06989569bded03d34622bb2edd809bdcf8496b256b189a85c8e',
    'port/android-app/swamp_render_policy.hpp':
        'ffacf497790a24265f88850fbdd6dfac86876c2a453379bc6c2983dbcfb4fe6f',
    'port/irrlicht-android/swamp-smoke/README.md':
        'c495b3116877b1896b3df5eadaa984be9853174e53e382809fd78baa24843db6',
}


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(command: list[str | Path], *, cwd: Path = REPO) -> str:
    result = subprocess.run([str(item) for item in command], cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-12000:])
    return result.stdout


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


def decode_texture(library: Path, source: bytes) -> tuple[TextureView, bytes]:
    decoder = c.CDLL(str(library.resolve()))
    decoder.dh2_texture_open.argtypes = [c.POINTER(TextureView), c.c_void_p, c.c_size_t]
    decoder.dh2_texture_open.restype = c.c_uint32
    decoder.dh2_texture_decode_rgba8.argtypes = [
        c.c_void_p, c.c_size_t, c.c_size_t, c.c_void_p, c.c_size_t]
    decoder.dh2_texture_decode_rgba8.restype = c.c_uint32
    source_buffer = c.create_string_buffer(source)
    view = TextureView()
    if decoder.dh2_texture_open(c.byref(view), source_buffer, len(source)) != 0:
        raise RuntimeError('AlphaMap texture header rejected')
    if view.width != 1024 or view.height != 1024 or view.format != 1:
        raise RuntimeError('AlphaMap no longer resolves to 1024x1024 PVRTC2')
    output = c.create_string_buffer(view.width * view.height * 4)
    if decoder.dh2_texture_decode_rgba8(
            output, len(output), view.width * 4, source_buffer, len(source)) != 0:
        raise RuntimeError('AlphaMap PVRTC decode failed')
    return view, output.raw


def alpha_channel_stats(rgba: bytes) -> dict:
    if len(rgba) != 1024 * 1024 * 4:
        raise RuntimeError('unexpected decoded AlphaMap pixel count')
    channel_zeros = [0, 0, 0, 0]
    blue_alpha_diff_hist = [0] * 256
    blue_red_diff_hist = [0] * 256
    blue_fractional = 0
    blue_threshold_discard = 0
    b_zero_a_nonzero = 0
    b_nonzero_a_zero = 0
    for pos in range(0, len(rgba), 4):
        red, green, blue, alpha = rgba[pos:pos + 4]
        channel_zeros[0] += red == 0
        channel_zeros[1] += green == 0
        channel_zeros[2] += blue == 0
        channel_zeros[3] += alpha == 0
        blue_alpha_diff_hist[abs(blue - alpha)] += 1
        blue_red_diff_hist[abs(blue - red)] += 1
        blue_fractional += 0 < blue < 255
        # The source AT comparison is strict < 0.8 on normalized 8-bit data.
        # 204/255 equals 0.8, so integer values 0..203 are discarded.
        blue_threshold_discard += blue < 204
        b_zero_a_nonzero += blue == 0 and alpha != 0
        b_nonzero_a_zero += blue != 0 and alpha == 0

    def diff_summary(histogram: list[int]) -> dict:
        different = sum(histogram[1:])
        total = sum(histogram)
        rank = (total * 95 + 99) // 100
        cumulative = 0
        percentile = 0
        for amount, count in enumerate(histogram):
            cumulative += count
            if cumulative >= rank:
                percentile = amount
                break
        return {
            'different_texels': different,
            'max_abs_difference': max(
                (i for i, count in enumerate(histogram) if count), default=0),
            'mean_abs_difference': sum(i * count for i, count in enumerate(histogram)) / total,
            'p95_abs_difference': percentile,
        }

    pixels = len(rgba) // 4
    return {
        'pixels': pixels,
        'decoded_channel_zero_counts': dict(zip('RGBA', channel_zeros)),
        'blue_fractional_alpha_texels_1_to_254': blue_fractional,
        'blue_alpha_channel_difference': diff_summary(blue_alpha_diff_hist),
        'blue_red_channel_difference': diff_summary(blue_red_diff_hist),
        'blue_zero_alpha_nonzero': b_zero_a_nonzero,
        'blue_nonzero_alpha_zero': b_nonzero_a_zero,
        'AT_lt_0_8_discard_count_if_this_variant_is_selected': blue_threshold_discard,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', required=True, type=Path,
                        help='Extracted owner-supplied cache root')
    parser.add_argument('--recovery-archive', required=True, type=Path,
                        help='Owner-supplied recovery archive that contains the GLSL members')
    parser.add_argument('--output', type=Path,
                        default=REPO / 'port/irrlicht-android/build/swamp-render-material-audit')
    parser.add_argument('--report', type=Path,
                        default=HERE / 'validation.json',
                        help='Derived audit JSON path; original shader text is never included')
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--texture-library', type=Path,
                        help='Built port/texture-assets host library; built on demand when omitted')
    args = parser.parse_args()
    cache = args.cache.resolve(strict=True)
    archive_path = args.recovery_archive.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f'C++ compiler not found: {args.cxx}')

    uv_executable = output / ('swamp-material-uv-probe.exe' if sys.platform == 'win32'
                              else 'swamp-material-uv-probe')
    run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
         '-fno-exceptions', '-fno-rtti', *UV_SOURCES, '-o', uv_executable])
    uv_output = run([uv_executable, cache])
    uv_report = json.loads(uv_output)
    if uv_report.get('draw_counts') != {'Material__11610': 25, 'Material__11611': 22}:
        raise RuntimeError('Selected source-material draw counts changed')
    if any(item.get('uv_attribute_id') != 3 or item.get('uv_components') != 2
           or item.get('uv_data_type') != 6 for item in uv_report['draws']):
        raise RuntimeError('One or more alpha-material source UV streams changed')

    if args.texture_library:
        texture_library = args.texture_library.resolve(strict=True)
    else:
        suffix = 'dh2_texture_assets.dll' if sys.platform == 'win32' else 'libdh2_texture_assets.so'
        texture_library = REPO / 'port/texture-assets/build' / suffix
        if not texture_library.is_file():
            run([sys.executable, REPO / 'port/texture-assets/build.py',
                 '--cache-root', cache])
        texture_library = texture_library.resolve(strict=True)

    shader_records = {}
    with zipfile.ZipFile(archive_path) as source_zip:
        bad_entry = source_zip.testzip()
        if bad_entry:
            raise RuntimeError(f'recovery archive CRC failed at {bad_entry}')
        for stage, member in SHADER_MEMBERS.items():
            data = source_zip.read(member)
            shader_records[stage] = {
                'archive_member': member,
                'bytes': len(data),
                'sha256': sha256(data),
            }
            text = data.decode('utf-8')
            if stage == 'vertex':
                checks = {
                    'diffuse_specular_uv_from_texcoord0': 'vTexCoord0.xy = texcoord0.xy' in text,
                    'lightmap_uv_from_texcoord1': 'vTexCoord0.zw = texcoord1.xy' in text,
                }
            else:
                checks = {
                    'alpha_sampler_blue_channel': 'texture2D(AlphaSampler, vTexCoord0.xy).z' in text,
                    'AT_strict_discard_threshold_0_8': 'if (Diffuse.w < 0.8) discard' in text,
                    'AL_variant_does_not_use_that_AT_discard': '#ifdef AT' in text,
                    'alpha_sampler_shares_uv0': 'AlphaSampler, vTexCoord0.xy' in text,
                }
            if not all(checks.values()):
                raise RuntimeError(f'Original shader contract assertion failed: {stage}')
            shader_records[stage]['contract_checks'] = checks

    alpha_path = cache / 'data/3d/textures/pvr2_env_swamp_alpha.tga'
    alpha_bytes = alpha_path.read_bytes()
    texture_view, decoded_alpha = decode_texture(texture_library, alpha_bytes)
    current_sources = [
        REPO / 'port/irrlicht-android/swamp-smoke/main.cpp',
        REPO / 'port/irrlicht-android/swamp-smoke/alpha_map_policy.hpp',
        REPO / 'port/irrlicht-android/swamp-smoke/tests/alpha_map_policy.cpp',
        REPO / 'port/irrlicht-android/swamp-smoke/tests/run_host.py',
        REPO / 'port/android-app/swamp_render_policy.hpp',
        REPO / 'port/irrlicht-android/swamp-smoke/README.md',
    ]
    source_hashes = {path.relative_to(REPO).as_posix(): sha256(path.read_bytes())
                     for path in current_sources if path.is_file()}
    input_paths = [
        'data/3d/modules/swamp/swamp.bdae',
        'data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae',
        'data/3d/textures/env_swamp.tga',
        'data/3d/textures/pvr2_env_swamp_alpha.tga',
        'data/3d/textures/env_swamp_spec.tga',
    ]
    inputs = {}
    for relative in input_paths:
        data = (cache / relative).read_bytes()
        inputs[relative] = {'bytes': len(data), 'sha256': sha256(data)}

    report = {
        'pass': True,
        'scope': 'Audits source AlphaMap channel/UV and Irrlicht material mismatch. This does not identify the serialized CurrentTechnique variant or original GL blend/depth state.',
        'source_archive': {
            'sha256': sha256(archive_path.read_bytes()),
            'shader_members': shader_records,
            'shader_text_copied_into_repository': False,
        },
        'source_cache_inputs': inputs,
        'material_draws_and_uvs': uv_report,
        'decoded_alpha_map': {
            'source_path': 'data/3d/textures/pvr2_env_swamp_alpha.tga',
            'bytes': len(alpha_bytes),
            'sha256': sha256(alpha_bytes),
            'width': texture_view.width,
            'height': texture_view.height,
            'format_code': texture_view.format,
            'channel_stats': alpha_channel_stats(decoded_alpha),
        },
        'material_contract': {
            'Material__11610': 'Diffuse+Specular; AlphaMap unbound; retains existing opaque diffuse path',
            'Material__11611': 'Diffuse+Specular+AlphaMap BRES references; 22 visible draws; alpha behavior changed only for this resolved paired-sampler material',
            'serialized_alpha_ref': 0.0,
            'serialized_diffuse_color': [0.8, 0.8, 0.8, 0.1],
            'object_alpha': 1.0,
            'CurrentTechnique': 'type-code 20 on both source materials; the local checked reader retains opaque raw records and cannot decode the selected AL-vs-AT variant',
        },
        'renderer_mismatch': {
            'current_app_alpha_channel': 'decoded PVRTC alpha byte',
            'source_fragment_alpha_channel': 'decoded PVRTC blue byte',
            'current_irrlicht_material': 'EMT_TRANSPARENT_ALPHA_CHANNEL_REF over an EMT_SOLID base renderer',
            'irrlicht_alpha_ref': 0.0,
            'irrlicht_effective_blending': False,
            'consequence': 'With strict Color.a < uAlphaRef and alpha ref zero, transparent texels are not discarded; fractional texels are also written without source AL blending.',
        },
        'port_preview_policy': {
            'Material__11611': 'Map the recovered AL fractional-output branch as a diagnostic preview, using B-channel mask, Irrlicht SRC_ALPHA/ONE_MINUS_SRC_ALPHA blending and EZW_AUTO (Irrlicht suppresses depth writes for the transparent pass by default). AL-vs-AT selection and exact original blend/depth state are unresolved.',
            'AT_branch': 'Its source shader threshold is exactly 0.8 if selected; do not apply it without proving the material technique selection.',
            'Material__11610': 'Unchanged opaque diffuse mapping because its AlphaMap image reference is absent.',
        },
        'pre_correction_source_sha256': PRE_CORRECTION_SOURCE_SHA256,
        'source_sha256_when_regenerated': source_hashes,
        'host_tools': {
            'uv_probe_source': (HERE / 'uv_probe.cpp').relative_to(REPO).as_posix(),
            'uv_probe_executable_sha256': sha256(uv_executable.read_bytes()),
            'texture_decoder': texture_library.relative_to(REPO).as_posix(),
            'texture_decoder_sha256': sha256(texture_library.read_bytes()),
        },
    }
    report_path = args.report.resolve()
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'PASS: source shader member, SWAMP UV and AlphaMap channel audit; report={report_path}')


if __name__ == '__main__':
    main()
