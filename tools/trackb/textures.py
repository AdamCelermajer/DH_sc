#!/usr/bin/env python3
"""Texture upscale pipeline (improvements work; the game data never changes in the repository).

  py -3 tools/trackb/textures.py extract --data <files dir> --out <folder>
      Decodes every data/3d/textures/*.tga (PVRTC containers, via build/own-v2b/host-runner/dh2_texconv.exe = the game's own
      decoder) and copies res/*.png into <folder>/original/{3d,res}/ as PNG, with manifest.csv (name, source, format, size,
      alpha) and a README for the upscaler. Hand <folder>/original to the upscaler; put results in <folder>/upscaled with the
      SAME file names (PNG, RGBA, any power-of-two size up to 2048).

  py -3 tools/trackb/textures.py pack --folder <folder> --out <files dir of a data copy>
      (the switch, step 2) Converts <folder>/upscaled/3d/*.png back into the game's container (uncompressed RGBA8888 PVR,
      format 0x12, the loader's native path, no decode at load) under data/3d/textures/, and copies upscaled/res/*.png over res/.
      Writes <out>/dh2_textures_manifest.txt with the SHA-256 of every replaced file.
"""
import argparse
import csv
import hashlib
import os
import shutil
import struct
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
TEXCONV = os.path.join(ROOT, 'build', 'own-v2b', 'host-runner', 'dh2_texconv.exe')


def pvr_info(path):
    with open(path, 'rb') as fh:
        b = fh.read(64)
    if b[:4] != b'BTEX':
        return None
    h = struct.unpack('<13I', b[8:60])
    return {'height': h[1], 'width': h[2], 'mips': h[3], 'fmt': h[4] & 0xff}


def extract(data, out):
    from PIL import Image
    orig = os.path.join(out, 'original')
    os.makedirs(os.path.join(orig, '3d'), exist_ok=True)
    os.makedirs(os.path.join(orig, 'res'), exist_ok=True)
    tmp = os.path.join(out, '_tmp')
    os.makedirs(tmp, exist_ok=True)
    rows = []
    texdir = os.path.join(data, 'data', '3d', 'textures')
    names = {'0x18': 'PVRTC 2bpp', '0x19': 'PVRTC 4bpp', '0x12': 'RGBA8888', '0x10': 'RGB565', '0x11': 'RGBA4444'}
    for f in sorted(os.listdir(texdir)):
        if not f.lower().endswith('.tga'):
            continue
        src = os.path.join(texdir, f)
        info = pvr_info(src)
        dec = os.path.join(tmp, f)
        r = subprocess.run([TEXCONV, 'decode', src, dec], capture_output=True, text=True)
        if r.returncode != 0:
            print('FAILED', f, r.stderr.strip())
            continue
        im = Image.open(dec).convert('RGBA')
        alpha = im.getchannel('A').getextrema()
        png = os.path.join(orig, '3d', os.path.splitext(f)[0] + '.png')
        im.save(png, optimize=True)
        fmt = names.get('0x%02x' % info['fmt'], 'pvr 0x%02x' % info['fmt']) if info else 'TGA 32-bit'
        rows.append(['3d/' + os.path.basename(png), 'data/3d/textures/' + f, fmt, '%dx%d' % im.size, 'yes' if alpha[0] < 255 else 'no'])
    resdir = os.path.join(data, 'res')
    for f in sorted(os.listdir(resdir)):
        if f.lower().endswith('.png'):
            src = os.path.join(resdir, f)
            dst = os.path.join(orig, 'res', f)
            shutil.copyfile(src, dst)
            im = Image.open(src)
            rows.append(['res/' + f, 'res/' + f, 'PNG ' + im.mode, '%dx%d' % im.size, 'yes' if 'A' in im.mode else 'no'])
    shutil.rmtree(tmp, ignore_errors=True)
    with open(os.path.join(out, 'manifest.csv'), 'w', newline='', encoding='utf-8') as fh:
        w = csv.writer(fh)
        w.writerow(['file', 'game_path', 'format', 'size', 'alpha'])
        w.writerows(rows)
    with open(os.path.join(out, 'README.txt'), 'w', encoding='utf-8') as fh:
        fh.write(README)
    print('extracted', len(rows), 'images to', orig)


README = """Dungeon Hunter 2 textures for upscaling
========================================

original/3d/   242 world/character/UI textures (decoded from the game's PVRTC containers; what the GPU shows in game)
original/res/  121 PNGs the Java side uses (loading / splash / intro)
manifest.csv   file, game path, original format, size, has alpha

What to hand back (folder "upscaled", same layout, SAME file names):
  upscaled/3d/<name>.png   RGBA PNG. Keep the alpha channel exactly (it is cut-out / glow / blend data), keep the image
                           square-tiling where the original tiles (walls, floors, water, caustics, grass). Any power-of-two
                           size up to 2048x2048 (originals are 64..1024). Do not crop, pad or recentre.
  upscaled/res/<name>.png  same rules.
Leave out any image you did not upscale; the game keeps the original for it.

Then: py -3 tools/trackb/textures.py pack --folder <this folder> --out <a data copy>  (done on the dev side).
"""


def pack(folder, out):
    from PIL import Image
    up = os.path.join(folder, 'upscaled')
    texdir = os.path.join(out, 'data', '3d', 'textures')
    resdir = os.path.join(out, 'res')
    lines = []
    d3 = os.path.join(up, '3d')
    if os.path.isdir(d3):
        for f in sorted(os.listdir(d3)):
            if not f.lower().endswith('.png'):
                continue
            im = Image.open(os.path.join(d3, f)).convert('RGBA')
            w, h = im.size
            if (w & (w - 1)) or (h & (h - 1)):
                print('SKIP (not power of two)', f, im.size)
                continue
            dst = os.path.join(texdir, os.path.splitext(f)[0] + '.tga')
            data = im.tobytes()
            # PVR v2 header: size, height, width, mips, flags (0x12 = RGBA8888 | 0x... no twiddle), data size, bpp, masks, magic, surfaces
            hdr = struct.pack('<13I', 52, h, w, 0, 0x12, len(data), 32, 0xff, 0xff00, 0xff0000, 0xff000000, 0x21525650, 1)
            with open(dst, 'wb') as fh:
                fh.write(b'BTEXpvr\0' + hdr + data)
            lines.append('%s  %s  %dx%d' % (hashlib.sha256(open(dst, 'rb').read()).hexdigest(), os.path.relpath(dst, out).replace(os.sep, '/'), w, h))
    dr = os.path.join(up, 'res')
    if os.path.isdir(dr):
        for f in sorted(os.listdir(dr)):
            if f.lower().endswith('.png'):
                dst = os.path.join(resdir, f)
                shutil.copyfile(os.path.join(dr, f), dst)
                lines.append('%s  %s' % (hashlib.sha256(open(dst, 'rb').read()).hexdigest(), 'res/' + f))
    with open(os.path.join(out, 'dh2_textures_manifest.txt'), 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(lines) + '\n')
    print('packed', len(lines), 'files into', out)


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest='cmd', required=True)
    e = sub.add_parser('extract')
    e.add_argument('--data', required=True)
    e.add_argument('--out', required=True)
    p = sub.add_parser('pack')
    p.add_argument('--folder', required=True)
    p.add_argument('--out', required=True)
    a = ap.parse_args()
    if a.cmd == 'extract':
        extract(a.data, a.out)
    else:
        pack(a.folder, a.out)


if __name__ == '__main__':
    main()
