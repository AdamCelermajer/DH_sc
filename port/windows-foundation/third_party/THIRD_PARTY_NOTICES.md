# Third-party notices (Windows foundation port)

## pl_mpeg (MPEG-1 video, MP2 audio, MPEG-PS demuxer)

- File: `port/windows-foundation/third_party/pl_mpeg/pl_mpeg.h` (single header, 141,424 bytes,
  SHA256 `3a8cb30c83c2a1147719c30fe0c8b93da2987aa43140077c575b39aaa75fc2c9`)
- Author: Dominic Szablewski, https://phoboslab.org
- License: MIT (the header carries `SPDX-License-Identifier: MIT`)
- Use: decodes the intro movie `intro_v1.mpg` at runtime (`features/startup/intro_movie_v2.cpp`).
  Only this header is shipped; no other pl_mpeg file is used.

MIT license text (standard wording; the upstream LICENSE file was not in the downloaded header, so
compare with the upstream repository before publishing):

```
Copyright (c) Dominic Szablewski

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Asset-prep tool (not shipped in the game)

- ffmpeg 8.1.2 (Gyan build) converts `intro.mp4` to `intro_v1.mpg` offline
  (`tools/convert_intro_video.ps1`). ffmpeg is not linked or run by the game.
