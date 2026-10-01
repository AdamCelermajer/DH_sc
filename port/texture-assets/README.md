# Texture file views (external cache)

This C++17 component classifies and bounds the external texture files in the complete owner-supplied Dungeon Hunter 2 cache. It borrows immutable bytes and returns dimensions, raw metadata and the exact encoded payload span. No pixel decoding, GPU upload, orientation change, material binding or original-engine ABI is claimed. See [`texture.hpp`](texture.hpp) for the small C/C++ interface.

## Verified cache evidence

The complete local ZIP has SHA-256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Its ZIP CRC check passes. The [recorded audit](validation.json) reads every `.tga` and `.png` entry directly from that ZIP, independently checks key header fields and lengths, and calls the compiled C++ parser for each. The results are:

| Container detected from bytes | Files | Notes |
| --- | ---: | --- |
| `BTEXpvr\0` + PVR v2 | 234 | 17 PVRTC 2 bpp, 217 PVRTC 4 bpp; each has one surface and no mipmaps |
| Uncompressed TGA | 8 | 32-bit BGRA pixels; the origin bit is exposed, not applied |
| PNG | 121 | Full chunk boundaries and CRCs checked; compressed pixels remain encoded |

All 234 PVR payload lengths equal both the header's data length and the PVRTC block-size calculation. The raw legacy PVR flag words are `33304` (17 files), `33305` (76) and `537` (141). Their low byte selects pixel types 24 (2 bpp) and 25 (4 bpp). The other bits and the one-bit alpha-mask field are preserved without assigning unverified engine meanings. There are no unsupported formats among these 363 extension-selected cache files. This does not classify texture data embedded inside BRES or other cache containers.

### Header layout used

The `BTEXpvr\0` wrapper is eight bytes, followed by a 52-byte little-endian legacy PVR header. Header fields are height at +4, width at +8, mip count at +12, raw flags at +16, payload length at +20, bits per pixel at +24, alpha mask at +40, `PVR!` tag at +44 and surface count at +48. The payload starts at file offset 60. A standalone PVR v2 file uses the same header at offset 0. The view checks header/tag, positive bounded dimensions, exact payload span, bits per pixel and the minimum PVRTC storage dimensions. Mipmapped or multi-surface PVR files are recognized but returned as unsupported until their layout is separately established.

The eight actual TGA files have image type 2, no color map and 32-bit BGRA data. The parser accepts the pixel span plus an optional 26-byte TGA footer, and exposes the descriptor's origin bits without flipping rows. PNG classification checks the signature, IHDR dimensions and legal color/depth pair, each chunk boundary and CRC, an IDAT and a terminal IEND. PNG pixel decoding remains for a conventional decoder.

The parser caps dimensions at 16,384 on either axis and rejects truncated or extra payload bytes for the supported PVR/TGA forms. It does not allocate storage. `TextureView` pointers remain valid only while the caller holds the original bytes. The tests include malformed lengths, tag, dimensions, formats, CRC and trailing bytes; they confirm that calls do not mutate input data.

## Reproduce

With Python 3.10+, a C++17 compiler and the complete owner-supplied ZIP:

```sh
python port/texture-assets/build.py --cache-zip /path/to/Dungeon-Hunter-2-HD-v1-0-2-cache.zip
```

The command builds a host shared library in the ignored `build/` directory, runs synthetic checks, scans all 363 external texture files and writes `validation.json`. Without `--cache-zip`, it runs the synthetic checks only. The recorded audit used MinGW-w64 GCC 15.2 on Windows. With `--ndk /path/to/android-ndk-r29`, the script also builds an Android 26 ARM64 shared library; this path passed with NDK r29 and produced 16 KiB-aligned load segments ([build evidence](../../reports/new-components-arm64-build.json)). Original-engine upload comparisons and rendered output have not yet been run.

The [material bindings module](../material-bindings/README.md) reads BRES image paths and resolves local effect IDs. Shader parameters, GPU upload and the renderer remain outside these components.
