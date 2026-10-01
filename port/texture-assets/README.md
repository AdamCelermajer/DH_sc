# Texture file views (external cache)

This C++17 component classifies and bounds the external texture files in the complete owner-supplied Dungeon Hunter 2 cache. It borrows immutable bytes and returns dimensions, raw metadata and the exact encoded payload span. It also decodes the cache's PVRTC1 2bpp and 4bpp textures to caller-owned RGBA8 buffers. GPU upload, orientation changes, material binding and the original-engine ABI remain unimplemented. See [`texture.hpp`](texture.hpp) for the small C/C++ interface.

## Verified cache evidence

The complete local ZIP has SHA-256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Its ZIP CRC check passes. The [recorded audit](validation.json) reads every `.tga` and `.png` entry directly from that ZIP, independently checks key header fields and lengths, and calls the compiled C++ parser for each. The results are:

| Container detected from bytes | Files | Notes |
| --- | ---: | --- |
| `BTEXpvr\0` + PVR v2 | 234 | 17 PVRTC 2 bpp, 217 PVRTC 4 bpp; each has one surface and no mipmaps |
| Uncompressed TGA | 8 | 32-bit BGRA pixels; the origin bit is exposed, not applied |
| PNG | 121 | Full chunk boundaries and CRCs checked; compressed pixels remain encoded |

All 234 PVR payload lengths equal both the header's data length and the PVRTC block-size calculation. The raw legacy PVR flag words are `33304` (17 files), `33305` (76) and `537` (141). Their low byte selects pixel types 24 (2 bpp) and 25 (4 bpp). The other bits and the one-bit alpha-mask field are preserved without assigning unverified engine meanings. All 234 BTEX textures are square power-of-two images, 64–1024 pixels on each side. There are no unsupported formats among these 363 extension-selected cache files. This does not classify texture data embedded inside BRES or other cache containers.

### Header layout used

The `BTEXpvr\0` wrapper is eight bytes, followed by a 52-byte little-endian legacy PVR header. Header fields are height at +4, width at +8, mip count at +12, raw flags at +16, payload length at +20, bits per pixel at +24, alpha mask at +40, `PVR!` tag at +44 and surface count at +48. The payload starts at file offset 60. A standalone PVR v2 file uses the same header at offset 0. The view checks header/tag, positive bounded dimensions, exact payload span, bits per pixel and the minimum PVRTC storage dimensions. Mipmapped or multi-surface PVR files are recognized but returned as unsupported until their layout is separately established.

The eight actual TGA files have image type 2, no color map and 32-bit BGRA data. The parser accepts the pixel span plus an optional 26-byte TGA footer, and exposes the descriptor's origin bits without flipping rows. PNG classification checks the signature, IHDR dimensions and legal color/depth pair, each chunk boundary and CRC, an IDAT and a terminal IEND. PNG pixel decoding remains for a conventional decoder.

The parser caps dimensions at 16,384 on either axis and rejects truncated or extra payload bytes for the supported PVR/TGA forms. It does not allocate storage. `TextureView` pointers remain valid only while the caller holds the original bytes. The tests include malformed lengths, tag, dimensions, formats, CRC and trailing bytes; they confirm that calls do not mutate input data.

## PVRTC1 pixel decoding

[`decode.cpp`](decode.cpp) implements the PVRTC1 2bpp and 4bpp word layout, reflected Morton ordering, endpoint expansion, bilinear color reconstruction, direct and interpolated 2bpp modulation, and 4bpp punch-through alpha. It follows the [Khronos Data Format specification](https://github.com/KhronosGroup/DataFormat/blob/main/pvrtc.txt). The API calls `open` again before decoding and accepts a caller-owned output buffer and row stride. It checks the full output span and rejects source/output overlap. Unsupported non-power-of-two or sub-minimum images return `unsupported_format`; too-small or overlapping output returns `invalid_output`. Rows are emitted in encoded order without a vertical flip. No GPU or original-engine texture upload result has been compared.

The [pixel audit](pixel-validation.json) decoded all 234 real BTEX textures, including 17 2bpp and 217 4bpp images. Synthetic tests exercise color endpoints, alpha punch-through, all 2bpp modulation directions, word ordering, malformed input, stride padding and output canaries. The real 2bpp files use direct and checkerboard modulation; horizontal and vertical modes occur only in the synthetic tests. In a separate [reference comparison](reference-validation.json), every RGBA byte of all 234 textures matched the official Imagination [PowerVR Native_SDK decoder](https://github.com/powervr-graphics/Native_SDK/blob/master/framework/PVRCore/texture/PVRTDecompress.cpp). The aggregate decoded-pixel digest is `83d5637bf464996fb70751ad5e12d9a7afa5f52e45c69b492ec75f958c60cdb8`. The comparison verifies this decoder against that software reference, not against original game rendering.

## Reproduce

With Python 3.10+, a C++17 compiler and the complete owner-supplied ZIP:

```sh
python port/texture-assets/build.py --cache-zip /path/to/Dungeon-Hunter-2-HD-v1-0-2-cache.zip
# Or use --cache-root /path/to/extracted/cache for the pixel audit.
```

The command builds a host shared library in the ignored `build/` directory and runs parser and pixel fixtures. With `--cache-zip`, it scans all 363 external textures for `validation.json` and decodes the 234 PVRTC images for `pixel-validation.json`. With `--cache-root`, it runs the pixel audit against an extracted complete cache. The recorded host run used MinGW-w64 GCC 15.2 on Windows. With `--ndk /path/to/android-ndk-r29`, the script also builds an Android 26 ARM64 shared library and checks its ELF headers and every load segment. The revised decoder passed with NDK r29: both PT_LOAD segments have 16 KiB alignment and congruent file/virtual offsets ([build evidence](arm64-build-validation.json)). This is a cross-build check; Android execution and GPU upload have not been tested.

To export one supported texture after building:

```sh
python port/texture-assets/export_png.py /path/to/texture.tga /path/to/texture.png
```

The exporter uses Python's standard library and limits output to 16,777,216 pixels by default. For an independent oracle replay, obtain `PVRTDecompress.cpp` and `.h` from the linked PowerVR Native_SDK source. The recorded `.cpp` SHA-256 is `74559c5a4b8161aafce1ebe984bf8060896feaa81276b614491dbee755d6e4c7`. Build them together with [`tests/reference_wrapper.cpp`](tests/reference_wrapper.cpp) as a temporary shared library, then run [`tests/compare_reference.py`](tests/compare_reference.py) with `--library`, `--reference-library` and `--cache-source`; Windows MinGW builds may also need `--dependency-dir` pointing at the compiler's runtime DLL directory. The comparator checks the reference decoder's consumed-byte return before comparing pixels. The SDK files and oracle DLL are deliberately not vendored in this repository.

The [material bindings module](../material-bindings/README.md) reads BRES image paths and resolves local effect IDs. Shader parameters, GPU upload and the renderer remain outside these components.
