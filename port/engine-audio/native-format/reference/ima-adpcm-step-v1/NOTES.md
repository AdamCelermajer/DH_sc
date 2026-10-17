# VoxN IMA ADPCM step

`ima_adpcm_step_v1.hpp` ports one nibble transition from the original
`VoxNativeSubDecoderIMAADPCM::DecodeBlock` at `0x887068` (1020 bytes). The
original ELF tables resolve to index VA `0x911a14` and signed step VA
`0x911a24`; the test runner checks their hashes and the function hash against
the pinned ELF SHA. The helper clamps the signed-16 predictor and index 0..88.

This proves only the nibble primitive and its lookup tables. It does not parse
VoxN segments or decode a complete music bank. `DecodeBlock` also establishes
its local framing: one 4-byte seed per channel (signed-16 predictor, index at
byte 2), followed by 4-byte/channel groups; each group yields eight samples
per channel, low nibble first, written with a channel-count sample stride.
The outer segment lengths, seek/remainder behavior and a source-executed bank
test vector remain unresolved, so PCM output and playback stay disabled.

```powershell
python port/engine-audio/native-format/tests/run_ima_adpcm_step_v1_host.py --original-elf outputs/audio-source-trace-ida/libDungeonHunter2.so --compiler g++ --build-dir port/level-world/build/ima-adpcm-step-v1
```
