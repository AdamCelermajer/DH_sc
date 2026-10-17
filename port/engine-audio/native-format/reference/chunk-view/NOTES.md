# VoxN framing

Adapted from engine branch `e6da25b83086ea01fb1d4cd46ae12da3bbac645f`.
Original evidence: `DecoderNativeCursor::ParseFile`, `0x8738bc`, 2616 bytes;
SHA-256 `db37531fce39bdf9c8d2a98084ddb31360fb81ddae015d34a1474f5d9b294674`.
Corrected five tag words: Segm, Grps, Grpe, Plst, Trsn. Canonical and candidate
API spellings now agree with the actual bytes. This is a bounded port parser;
**zero complete original bodies**, no original execution, decoding or playback.

The eight-byte header exposes the raw read count. The checked layout accepts
the observed count 16, reads declared length/audio base at +16/+20, and exposes
chunks `[24,audio_base-8)`. The remaining eight bytes are uninterpreted. Size,
alignment, overlap and arithmetic checks are added port safeguards. Outputs
remain unchanged on rejection/end. Borrowed payload storage stays immutable
and alive; unknown tags/compression bytes remain raw, with no decoding claim.

Historical framing records identify the branch's twelve-file subset. The gate
checks all seventeen local files, streams hashes, retains at most 1 MiB of
metadata, and tests malformed/truncated/overflow inputs. No assets are bundled.

Cache inspection of all 17 `.vxn` entries found the same `Afmt` record:
format tag `0x0011`, 2 channels, 32,000 Hz. IDA identifies the native path as
`VoxNativeSubDecoderIMAADPCM`; the assets also carry `Segm`, `Plst`, `Stat`,
`Trsn`, `Grps`, and `Grpe` metadata. This establishes the codec family, not a
complete VoxN decoder. No third-party decoder/license is selected in this
checkout. The exact single-nibble predictor/index step and original lookup
tables are now ported and hash-checked in `ima_adpcm_step_v1.hpp`. IDA pins
24-byte segment rows, the first three row fields, and channel-interleaved
16-bit output bounds. No decoded PCM fixture validates the full block/cursor
path, so complete decode and playback remain disconnected. See
`reference/ima-adpcm-step-v1/NOTES.md` and
`reference/vxn-segment-decode/NOTES.md`.

```text
python port/engine-audio/native-format/tests/run_chunk_view_host.py --cache ABSOLUTE_CACHE_FILES --original-elf ABSOLUTE_ORIGINAL_ELF --compiler ABSOLUTE_GXX --build-dir port/level-world/build/vox-n-chunk-view
```
