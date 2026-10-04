# _ZN7gameswf14player_contextC1Ev
0076ce38: push     {r4, r5, r6, lr}
0076ce3c: ldr      r5, [pc, #0x44]
0076ce40: mov      r4, r0
0076ce44: bl       #0x759c04
0076ce48: ldr      r2, [pc, #0x3c]
0076ce4c: add      r5, pc, r5
0076ce50: mov      r3, #0
0076ce54: ldr      r2, [r5, r2]
0076ce58: str      r3, [r4, #0x28]
0076ce5c: str      r3, [r4, #0xc]
0076ce60: add      r2, r2, #8
0076ce64: str      r2, [r4]
0076ce68: str      r3, [r4, #0x10]
0076ce6c: str      r3, [r4, #0x14]
0076ce70: str      r3, [r4, #0x18]
0076ce74: str      r3, [r4, #0x1c]
0076ce78: strb     r3, [r4, #0x20]
0076ce7c: str      r3, [r4, #0x24]
0076ce80: mov      r0, r4
0076ce84: pop      {r4, r5, r6, pc}
0076ce88: eoreq    r7, r2, r4, asr #24
0076ce8c: andeq    r4, r0, r0, ror #24

# _ZN7gameswf14player_contextC2Ev
0076ce90: push     {r4, r5, r6, lr}
0076ce94: ldr      r5, [pc, #0x44]
0076ce98: mov      r4, r0
0076ce9c: bl       #0x759c04
0076cea0: ldr      r2, [pc, #0x3c]
0076cea4: add      r5, pc, r5
0076cea8: mov      r3, #0
0076ceac: ldr      r2, [r5, r2]
0076ceb0: str      r3, [r4, #0x28]
0076ceb4: str      r3, [r4, #0xc]
0076ceb8: add      r2, r2, #8
0076cebc: str      r2, [r4]
0076cec0: str      r3, [r4, #0x10]
0076cec4: str      r3, [r4, #0x14]
0076cec8: str      r3, [r4, #0x18]
0076cecc: str      r3, [r4, #0x1c]
0076ced0: strb     r3, [r4, #0x20]
0076ced4: str      r3, [r4, #0x24]
0076ced8: mov      r0, r4
0076cedc: pop      {r4, r5, r6, pc}
0076cee0: eoreq    r7, r2, ip, ror #23
0076cee4: andeq    r4, r0, r0, ror #24

# _ZN7gameswf14player_contextD2Ev
0076cee8: push     {r4, r5, r6, lr}
0076ceec: ldr      r3, [pc, #0x98]
0076cef0: ldr      r2, [pc, #0x98]
0076cef4: ldr      r5, [r0, #0xc]
0076cef8: add      r3, pc, r3
0076cefc: ldr      r2, [r3, r2]
0076cf00: cmp      r5, #0
0076cf04: mov      r4, r0
0076cf08: add      r2, r2, #8
0076cf0c: str      r2, [r0]
0076cf10: beq      #0x76cf28
0076cf14: mov      r0, r5
0076cf18: bl       #0x7d1b40
0076cf1c: mov      r0, r5
0076cf20: mov      r1, #0
0076cf24: bl       #0x752b38
0076cf28: ldr      r0, [r4, #0x10]
0076cf2c: cmp      r0, #0
0076cf30: beq      #0x76cf38
0076cf34: bl       #0x76c9ac
0076cf38: ldr      r3, [r4, #0x18]
0076cf3c: add      r0, r4, #0x14
0076cf40: cmp      r3, #0
0076cf44: ble      #0x76cf68
0076cf48: mov      r3, #0
0076cf4c: mov      r1, r3
0076cf50: str      r3, [r4, #0x18]
0076cf54: bl       #0x76ca08
0076cf58: mov      r0, r4
0076cf5c: bl       #0x75dca4
0076cf60: mov      r0, r4
0076cf64: pop      {r4, r5, r6, pc}
0076cf68: bge      #0x76cf48
0076cf6c: lsl      r2, r3, #2
0076cf70: mov      ip, #0
0076cf74: ldr      r1, [r4, #0x14]
0076cf78: adds     r3, r3, #1
0076cf7c: str      ip, [r1, r2]
0076cf80: add      r2, r2, #4
0076cf84: bne      #0x76cf74
0076cf88: b        #0x76cf48
0076cf8c: mlaeq    r2, r8, fp, r7
0076cf90: andeq    r4, r0, r0, ror #24

# _ZN7gameswf14player_contextD1Ev
0076d4cc: push     {r4, r5, r6, lr}
0076d4d0: ldr      r3, [pc, #0x98]
0076d4d4: ldr      r2, [pc, #0x98]
0076d4d8: ldr      r5, [r0, #0xc]
0076d4dc: add      r3, pc, r3
0076d4e0: ldr      r2, [r3, r2]
0076d4e4: cmp      r5, #0
0076d4e8: mov      r4, r0
0076d4ec: add      r2, r2, #8
0076d4f0: str      r2, [r0]
0076d4f4: beq      #0x76d50c
0076d4f8: mov      r0, r5
0076d4fc: bl       #0x7d1b40
0076d500: mov      r0, r5
0076d504: mov      r1, #0
0076d508: bl       #0x752b38
0076d50c: ldr      r0, [r4, #0x10]
0076d510: cmp      r0, #0
0076d514: beq      #0x76d51c
0076d518: bl       #0x76c9ac
0076d51c: ldr      r3, [r4, #0x18]
0076d520: add      r0, r4, #0x14
0076d524: cmp      r3, #0
0076d528: ble      #0x76d54c
0076d52c: mov      r3, #0
0076d530: mov      r1, r3
0076d534: str      r3, [r4, #0x18]
0076d538: bl       #0x76ca08
0076d53c: mov      r0, r4
0076d540: bl       #0x75dca4
0076d544: mov      r0, r4
0076d548: pop      {r4, r5, r6, pc}
0076d54c: bge      #0x76d52c
0076d550: lsl      r2, r3, #2
0076d554: mov      ip, #0
0076d558: ldr      r1, [r4, #0x14]
0076d55c: adds     r3, r3, #1
0076d560: str      ip, [r1, r2]
0076d564: add      r2, r2, #4
0076d568: bne      #0x76d558
0076d56c: b        #0x76d52c

# _ZN7gameswf14player_contextD0Ev
0076d578: push     {r4, lr}
0076d57c: mov      r4, r0
0076d580: bl       #0x76d4cc
0076d584: mov      r0, r4
0076d588: bl       #0x30e2b0
0076d58c: mov      r0, r4
0076d590: pop      {r4, pc}

# _ZN7gameswf6playerC1EPNS_14player_contextE
0076f180: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0076f184: ldr      r6, [pc, #0x208]
0076f188: mov      r4, r0
0076f18c: mov      r7, r1
0076f190: bl       #0x759c04
0076f194: ldr      r3, [pc, #0x1fc]
0076f198: ldr      r1, [r4, #0x60]
0076f19c: ldr      r2, [r4, #0x78]
0076f1a0: add      r6, pc, r6
0076f1a4: ldr      r3, [r6, r3]
0076f1a8: mvn      r0, #0
0076f1ac: bfi      r2, r0, #0, #0x18
0076f1b0: bfi      r1, r0, #0, #0x18
0076f1b4: mov      r5, #0
0076f1b8: lsr      r0, r2, #0x18
0076f1bc: lsr      ip, r1, #0x18
0076f1c0: add      lr, r3, #8
0076f1c4: bfi      ip, r5, #0, #1
0076f1c8: mov      r3, #1
0076f1cc: bfi      r0, r5, #0, #1
0076f1d0: str      r2, [r4, #0x78]
0076f1d4: str      lr, [r4]
0076f1d8: strb     r3, [r4, #0x50]
0076f1dc: strb     r3, [r4, #0x68]
0076f1e0: str      r1, [r4, #0x60]
0076f1e4: strb     r0, [r4, #0x7b]
0076f1e8: strb     ip, [r4, #0x63]
0076f1ec: mov      r1, r5
0076f1f0: str      r5, [r4, #0xc]
0076f1f4: str      r5, [r4, #0x10]
0076f1f8: str      r5, [r4, #0x14]
0076f1fc: strb     r5, [r4, #0x18]
0076f200: str      r5, [r4, #0x1c]
0076f204: str      r5, [r4, #0x20]
0076f208: str      r5, [r4, #0x24]
0076f20c: strb     r5, [r4, #0x28]
0076f210: str      r5, [r4, #0x2c]
0076f214: str      r5, [r4, #0x34]
0076f218: str      r5, [r4, #0x38]
0076f21c: str      r5, [r4, #0x48]
0076f220: str      r5, [r4, #0x4c]
0076f224: strb     r5, [r4, #0x51]
0076f228: str      r5, [r4, #0x64]
0076f22c: strb     r5, [r4, #0x69]
0076f230: strb     r5, [r4, #0x7c]
0076f234: strb     r5, [r4, #0x7d]
0076f238: strb     r5, [r4, #0x88]
0076f23c: strb     r5, [r4, #0x89]
0076f240: str      r5, [r4, #0x9c]
0076f244: str      r5, [r4, #0xa0]
0076f248: str      r5, [r4, #0xa4]
0076f24c: mov      r0, #0x38
0076f250: strb     r5, [r4, #0xa8]
0076f254: str      r3, [r4, #0x30]
0076f258: str      r7, [r4, #0xac]
0076f25c: str      r5, [r4, #0xb0]
0076f260: str      r5, [r4, #0xb4]
0076f264: str      r5, [r4, #0xb8]
0076f268: strb     r5, [r4, #0xbc]
0076f26c: str      r5, [r4, #0xc0]
0076f270: str      r5, [r4, #0xc4]
0076f274: str      r5, [r4, #0xc8]
0076f278: strb     r5, [r4, #0xcc]
0076f27c: str      r5, [r4, #0xd0]
0076f280: str      r5, [r4, #0xd4]
0076f284: str      r5, [r4, #0xd8]
0076f288: strb     r5, [r4, #0xdc]
0076f28c: bl       #0x752ba8
0076f290: mov      r7, r0
0076f294: mov      r1, r4
0076f298: bl       #0x76b820
0076f29c: mov      r1, r7
0076f2a0: add      r0, r4, #0x34
0076f2a4: bl       #0x768cc8
0076f2a8: mov      r1, r5
0076f2ac: mov      r0, #0x38
0076f2b0: bl       #0x752ba8
0076f2b4: mov      r1, r4
0076f2b8: mov      r7, r0
0076f2bc: bl       #0x76f018
0076f2c0: mov      r1, r7
0076f2c4: add      r0, r4, #0x38
0076f2c8: bl       #0x768cc8
0076f2cc: mov      r1, r5
0076f2d0: mov      r0, #0x38
0076f2d4: bl       #0x752ba8
0076f2d8: mov      r1, r4
0076f2dc: mov      r5, r0
0076f2e0: bl       #0x76b820
0076f2e4: mov      r1, r5
0076f2e8: add      r0, r4, #0x7c
0076f2ec: bl       #0x797250
0076f2f0: ldr      r3, [pc, #0xa4]
0076f2f4: add      r0, r4, #0x88
0076f2f8: ldr      r1, [r6, r3]
0076f2fc: bl       #0x7972a0
0076f300: mov      r0, r4
0076f304: bl       #0x76e104
0076f308: ldr      r5, [r4, #0xac]
0076f30c: ldr      r3, [r5, #0x18]
0076f310: ldr      r2, [r5, #0x1c]
0076f314: add      r6, r3, #1
0076f318: cmp      r6, r2
0076f31c: bgt      #0x76f380
0076f320: ldr      r2, [r5, #0x14]
0076f324: str      r4, [r2, r3, lsl #2]
0076f328: str      r6, [r5, #0x18]
0076f32c: bl       #0x7b7af8
0076f330: and      r8, r0, #0xff
0076f334: mov      sb, #0
0076f338: orrs     r3, r8, sb
0076f33c: beq      #0x76f36c
0076f340: mov      r6, #0
0076f344: mov      r7, #0
0076f348: mov      sl, #1
0076f34c: mov      fp, #0
0076f350: adds     r6, r6, sl
0076f354: adc      r7, r7, fp
0076f358: bl       #0x7b7898
0076f35c: cmp      r6, r8
0076f360: bne      #0x76f350
0076f364: cmp      r7, sb
0076f368: bne      #0x76f350
0076f36c: mov      r3, #0
0076f370: strb     r3, [r4, #0x98]
0076f374: str      r3, [r4, #0x94]
0076f378: mov      r0, r4
0076f37c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0076f380: add      r0, r5, #0x14
0076f384: add      r1, r6, r6, asr #1
0076f388: bl       #0x76ca08
0076f38c: ldr      r3, [r5, #0x18]
0076f390: b        #0x76f320

# _ZN7gameswf6playerC2EPNS_14player_contextE
0076f3a0: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0076f3a4: ldr      r6, [pc, #0x208]
0076f3a8: mov      r4, r0
0076f3ac: mov      r7, r1
0076f3b0: bl       #0x759c04
0076f3b4: ldr      r3, [pc, #0x1fc]
0076f3b8: ldr      r1, [r4, #0x60]
0076f3bc: ldr      r2, [r4, #0x78]
0076f3c0: add      r6, pc, r6
0076f3c4: ldr      r3, [r6, r3]
0076f3c8: mvn      r0, #0
0076f3cc: bfi      r2, r0, #0, #0x18
0076f3d0: bfi      r1, r0, #0, #0x18
0076f3d4: mov      r5, #0
0076f3d8: lsr      r0, r2, #0x18
0076f3dc: lsr      ip, r1, #0x18
0076f3e0: add      lr, r3, #8
0076f3e4: bfi      ip, r5, #0, #1
0076f3e8: mov      r3, #1
0076f3ec: bfi      r0, r5, #0, #1
0076f3f0: str      r2, [r4, #0x78]
0076f3f4: str      lr, [r4]
0076f3f8: strb     r3, [r4, #0x50]
0076f3fc: strb     r3, [r4, #0x68]
0076f400: str      r1, [r4, #0x60]
0076f404: strb     r0, [r4, #0x7b]
0076f408: strb     ip, [r4, #0x63]
0076f40c: mov      r1, r5
0076f410: str      r5, [r4, #0xc]
0076f414: str      r5, [r4, #0x10]
0076f418: str      r5, [r4, #0x14]
0076f41c: strb     r5, [r4, #0x18]
0076f420: str      r5, [r4, #0x1c]
0076f424: str      r5, [r4, #0x20]
0076f428: str      r5, [r4, #0x24]
0076f42c: strb     r5, [r4, #0x28]
0076f430: str      r5, [r4, #0x2c]
0076f434: str      r5, [r4, #0x34]
0076f438: str      r5, [r4, #0x38]
0076f43c: str      r5, [r4, #0x48]
0076f440: str      r5, [r4, #0x4c]
0076f444: strb     r5, [r4, #0x51]
0076f448: str      r5, [r4, #0x64]
0076f44c: strb     r5, [r4, #0x69]
0076f450: strb     r5, [r4, #0x7c]
0076f454: strb     r5, [r4, #0x7d]
0076f458: strb     r5, [r4, #0x88]
0076f45c: strb     r5, [r4, #0x89]
0076f460: str      r5, [r4, #0x9c]
0076f464: str      r5, [r4, #0xa0]
0076f468: str      r5, [r4, #0xa4]
0076f46c: mov      r0, #0x38
0076f470: strb     r5, [r4, #0xa8]
0076f474: str      r3, [r4, #0x30]
0076f478: str      r7, [r4, #0xac]
0076f47c: str      r5, [r4, #0xb0]
0076f480: str      r5, [r4, #0xb4]
0076f484: str      r5, [r4, #0xb8]
0076f488: strb     r5, [r4, #0xbc]
0076f48c: str      r5, [r4, #0xc0]
0076f490: str      r5, [r4, #0xc4]
0076f494: str      r5, [r4, #0xc8]
0076f498: strb     r5, [r4, #0xcc]
0076f49c: str      r5, [r4, #0xd0]
0076f4a0: str      r5, [r4, #0xd4]
0076f4a4: str      r5, [r4, #0xd8]
0076f4a8: strb     r5, [r4, #0xdc]
0076f4ac: bl       #0x752ba8
0076f4b0: mov      r7, r0
0076f4b4: mov      r1, r4
0076f4b8: bl       #0x76b820
0076f4bc: mov      r1, r7
0076f4c0: add      r0, r4, #0x34
0076f4c4: bl       #0x768cc8
0076f4c8: mov      r1, r5
0076f4cc: mov      r0, #0x38
0076f4d0: bl       #0x752ba8
0076f4d4: mov      r1, r4
0076f4d8: mov      r7, r0
0076f4dc: bl       #0x76f018
0076f4e0: mov      r1, r7
0076f4e4: add      r0, r4, #0x38
0076f4e8: bl       #0x768cc8
0076f4ec: mov      r1, r5
0076f4f0: mov      r0, #0x38
0076f4f4: bl       #0x752ba8
0076f4f8: mov      r1, r4
0076f4fc: mov      r5, r0
0076f500: bl       #0x76b820
0076f504: mov      r1, r5
0076f508: add      r0, r4, #0x7c
0076f50c: bl       #0x797250
0076f510: ldr      r3, [pc, #0xa4]
0076f514: add      r0, r4, #0x88
0076f518: ldr      r1, [r6, r3]
0076f51c: bl       #0x7972a0
0076f520: mov      r0, r4
0076f524: bl       #0x76e104
0076f528: ldr      r5, [r4, #0xac]
0076f52c: ldr      r3, [r5, #0x18]
0076f530: ldr      r2, [r5, #0x1c]
0076f534: add      r6, r3, #1
0076f538: cmp      r6, r2
0076f53c: bgt      #0x76f5a0
0076f540: ldr      r2, [r5, #0x14]
0076f544: str      r4, [r2, r3, lsl #2]
0076f548: str      r6, [r5, #0x18]
0076f54c: bl       #0x7b7af8
0076f550: and      r8, r0, #0xff
0076f554: mov      sb, #0
0076f558: orrs     r3, r8, sb
0076f55c: beq      #0x76f58c
0076f560: mov      r6, #0
0076f564: mov      r7, #0
0076f568: mov      sl, #1
0076f56c: mov      fp, #0
0076f570: adds     r6, r6, sl
0076f574: adc      r7, r7, fp
0076f578: bl       #0x7b7898
0076f57c: cmp      r6, r8
0076f580: bne      #0x76f570
0076f584: cmp      r7, sb
0076f588: bne      #0x76f570
0076f58c: mov      r3, #0
0076f590: strb     r3, [r4, #0x98]
0076f594: str      r3, [r4, #0x94]
0076f598: mov      r0, r4
0076f59c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0076f5a0: add      r0, r5, #0x14
0076f5a4: add      r1, r6, r6, asr #1
0076f5a8: bl       #0x76ca08
0076f5ac: ldr      r3, [r5, #0x18]
0076f5b0: b        #0x76f540

# _ZN7gameswf12render_cache8is_validEPNS_9characterE
007738a4: push     {r4, r5, r6, lr}
007738a8: mov      r5, r1
007738ac: mov      r4, r0
007738b0: add      r0, r1, #0x2c
007738b4: bl       #0x755260
007738b8: ldr      r3, [r5, #0x30]
007738bc: mov      r0, r5
007738c0: ldr      r6, [r3, #0xac]
007738c4: bl       #0x753ec0
007738c8: mov      r0, r5
007738cc: bl       #0x753f74
007738d0: ldr      r3, [r6, #0xc]
007738d4: ldr      r3, [r3, #0x28]
007738d8: cmp      r3, #0
007738dc: beq      #0x77393c
007738e0: ldr      r2, [r3, #8]
007738e4: ldr      r1, [r4]
007738e8: ldr      r3, [r3, #0xc]
007738ec: cmp      r1, r2
007738f0: beq      #0x773930
007738f4: stm      r4, {r2, r3}
007738f8: ldr      r3, [r6, #0x10]
007738fc: mov      r0, #1
00773900: ldr      r3, [r3, #0xc]
00773904: cmp      r3, #0
00773908: beq      #0x773950
0077390c: ldr      r2, [r3, #8]
00773910: ldr      r1, [r4, #8]
00773914: ldr      r3, [r3, #0xc]
00773918: cmp      r1, r2
0077391c: beq      #0x773958
00773920: str      r3, [r4, #0xc]
00773924: str      r2, [r4, #8]
00773928: mov      r0, #0
0077392c: pop      {r4, r5, r6, pc}
00773930: ldr      r1, [r4, #4]
00773934: cmp      r1, r3
00773938: bne      #0x7738f4
0077393c: ldr      r3, [r6, #0x10]
00773940: mov      r0, #0
00773944: ldr      r3, [r3, #0xc]
00773948: cmp      r3, #0
0077394c: bne      #0x77390c
00773950: eor      r0, r0, #1
00773954: pop      {r4, r5, r6, pc}
00773958: ldr      r1, [r4, #0xc]
0077395c: cmp      r1, r3
00773960: beq      #0x773950
00773964: str      r3, [r4, #0xc]
00773968: str      r2, [r4, #8]
0077396c: mov      r0, #0
00773970: pop      {r4, r5, r6, pc}

# _ZN7gameswf4rootC1EPNS_6playerEPNS_14movie_def_implE
00775da0: push     {r4, r5, r6, r7, r8, sl, lr}
00775da4: ldr      r5, [pc, #0x1bc]
00775da8: sub      sp, sp, #0xc
00775dac: mov      r7, r2
00775db0: mov      r4, r0
00775db4: mov      r6, r1
00775db8: bl       #0x759c04
00775dbc: ldr      r3, [pc, #0x1a8]
00775dc0: add      r5, pc, r5
00775dc4: cmp      r7, #0
00775dc8: ldr      r3, [r5, r3]
00775dcc: str      r7, [r4, #0xc]
00775dd0: add      r3, r3, #8
00775dd4: str      r3, [r4]
00775dd8: beq      #0x775de4
00775ddc: mov      r0, r7
00775de0: bl       #0x759c64
00775de4: mov      r7, #0
00775de8: mov      r3, #0
00775dec: mov      r2, #1
00775df0: mov      r5, #0x3f800000
00775df4: mvn      r1, #0
00775df8: str      r2, [r4, #0x20]
00775dfc: str      r2, [r4, #0x1c]
00775e00: str      r3, [r4, #0x48]
00775e04: str      r3, [r4, #0x4c]
00775e08: str      r3, [r4, #0x60]
00775e0c: str      r3, [r4, #0x64]
00775e10: str      r3, [r4, #0x70]
00775e14: str      r3, [r4, #0x74]
00775e18: strb     r1, [r4, #0x3b]
00775e1c: add      r0, r4, #0xc8
00775e20: mov      r1, r6
00775e24: str      r7, [r4, #0x10]
00775e28: str      r7, [r4, #0x14]
00775e2c: str      r7, [r4, #0x18]
00775e30: str      r5, [r4, #0x34]
00775e34: strb     r7, [r4, #0x38]
00775e38: strb     r7, [r4, #0x39]
00775e3c: strb     r7, [r4, #0x3a]
00775e40: str      r7, [r4, #0x3c]
00775e44: str      r7, [r4, #0x40]
00775e48: str      r7, [r4, #0x44]
00775e4c: str      r7, [r4, #0x50]
00775e50: str      r7, [r4, #0x54]
00775e54: str      r7, [r4, #0x58]
00775e58: strb     r7, [r4, #0x5c]
00775e5c: strb     r7, [r4, #0x5d]
00775e60: strb     r7, [r4, #0x5e]
00775e64: str      r5, [r4, #0x68]
00775e68: str      r5, [r4, #0x6c]
00775e6c: str      r7, [r4, #0x78]
00775e70: str      r7, [r4, #0x7c]
00775e74: strb     r7, [r4, #0x80]
00775e78: strb     r7, [r4, #0x81]
00775e7c: strb     r7, [r4, #0x82]
00775e80: strb     r7, [r4, #0x84]
00775e84: strb     r7, [r4, #0x85]
00775e88: str      r3, [r4, #0x94]
00775e8c: strb     r7, [r4, #0x86]
00775e90: strb     r7, [r4, #0x87]
00775e94: str      r7, [r4, #0x88]
00775e98: str      r5, [r4, #0x8c]
00775e9c: str      r5, [r4, #0x90]
00775ea0: str      r7, [r4, #0x98]
00775ea4: str      r7, [r4, #0x9c]
00775ea8: str      r7, [r4, #0xa0]
00775eac: strb     r7, [r4, #0xa4]
00775eb0: str      r7, [r4, #0xa8]
00775eb4: str      r7, [r4, #0xac]
00775eb8: str      r7, [r4, #0xb0]
00775ebc: strb     r7, [r4, #0xb4]
00775ec0: str      r7, [r4, #0xb8]
00775ec4: str      r7, [r4, #0xbc]
00775ec8: str      r7, [r4, #0xc0]
00775ecc: strb     r7, [r4, #0xc4]
00775ed0: str      r7, [r4, #0xc8]
00775ed4: str      r7, [r4, #0xcc]
00775ed8: bl       #0x75e9ac
00775edc: ldr      r3, [r4, #0xc]
00775ee0: mov      r0, r3
00775ee4: ldr      r3, [r3]
00775ee8: mov      lr, pc
00775eec: ldr      pc, [r3, #0x30]
00775ef0: ldr      r3, [r4, #0xc]
00775ef4: mov      r8, r0
00775ef8: mov      r0, r3
00775efc: ldr      r3, [r3]
00775f00: mov      lr, pc
00775f04: ldr      pc, [r3, #0x34]
00775f08: mov      sl, r0
00775f0c: mov      r0, r8
00775f10: bl       #0x30e4cc
00775f14: mov      r8, r0
00775f18: mov      r0, sl
00775f1c: bl       #0x30e4cc
00775f20: mov      r3, r8
00775f24: mov      r2, r7
00775f28: mov      r1, r7
00775f2c: str      r0, [sp]
00775f30: mov      r0, r4
00775f34: bl       #0x775d38
00775f38: mov      r0, r4
00775f3c: bl       #0x7741a0
00775f40: mov      r1, r0
00775f44: mov      r0, r5
00775f48: bl       #0x30ec94
00775f4c: mov      r1, r4
00775f50: str      r0, [r4, #0x90]
00775f54: mov      r0, r6
00775f58: bl       #0x76d71c
00775f5c: mov      r0, r4
00775f60: add      sp, sp, #0xc
00775f64: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZN7gameswf4rootC2EPNS_6playerEPNS_14movie_def_implE
00775f70: push     {r4, r5, r6, r7, r8, sl, lr}
00775f74: ldr      r5, [pc, #0x1bc]
00775f78: sub      sp, sp, #0xc
00775f7c: mov      r7, r2
00775f80: mov      r4, r0
00775f84: mov      r6, r1
00775f88: bl       #0x759c04
00775f8c: ldr      r3, [pc, #0x1a8]
00775f90: add      r5, pc, r5
00775f94: cmp      r7, #0
00775f98: ldr      r3, [r5, r3]
00775f9c: str      r7, [r4, #0xc]
00775fa0: add      r3, r3, #8
00775fa4: str      r3, [r4]
00775fa8: beq      #0x775fb4
00775fac: mov      r0, r7
00775fb0: bl       #0x759c64
00775fb4: mov      r7, #0
00775fb8: mov      r3, #0
00775fbc: mov      r2, #1
00775fc0: mov      r5, #0x3f800000
00775fc4: mvn      r1, #0
00775fc8: str      r2, [r4, #0x20]
00775fcc: str      r2, [r4, #0x1c]
00775fd0: str      r3, [r4, #0x48]
00775fd4: str      r3, [r4, #0x4c]
00775fd8: str      r3, [r4, #0x60]
00775fdc: str      r3, [r4, #0x64]
00775fe0: str      r3, [r4, #0x70]
00775fe4: str      r3, [r4, #0x74]
00775fe8: strb     r1, [r4, #0x3b]
00775fec: add      r0, r4, #0xc8
00775ff0: mov      r1, r6
00775ff4: str      r7, [r4, #0x10]
00775ff8: str      r7, [r4, #0x14]
00775ffc: str      r7, [r4, #0x18]
00776000: str      r5, [r4, #0x34]
00776004: strb     r7, [r4, #0x38]
00776008: strb     r7, [r4, #0x39]
0077600c: strb     r7, [r4, #0x3a]
00776010: str      r7, [r4, #0x3c]
00776014: str      r7, [r4, #0x40]
00776018: str      r7, [r4, #0x44]
0077601c: str      r7, [r4, #0x50]
00776020: str      r7, [r4, #0x54]
00776024: str      r7, [r4, #0x58]
00776028: strb     r7, [r4, #0x5c]
0077602c: strb     r7, [r4, #0x5d]
00776030: strb     r7, [r4, #0x5e]
00776034: str      r5, [r4, #0x68]
00776038: str      r5, [r4, #0x6c]
0077603c: str      r7, [r4, #0x78]
00776040: str      r7, [r4, #0x7c]
00776044: strb     r7, [r4, #0x80]
00776048: strb     r7, [r4, #0x81]
0077604c: strb     r7, [r4, #0x82]
00776050: strb     r7, [r4, #0x84]
00776054: strb     r7, [r4, #0x85]
00776058: str      r3, [r4, #0x94]
0077605c: strb     r7, [r4, #0x86]
00776060: strb     r7, [r4, #0x87]
00776064: str      r7, [r4, #0x88]
00776068: str      r5, [r4, #0x8c]
0077606c: str      r5, [r4, #0x90]
00776070: str      r7, [r4, #0x98]
00776074: str      r7, [r4, #0x9c]
00776078: str      r7, [r4, #0xa0]
0077607c: strb     r7, [r4, #0xa4]
00776080: str      r7, [r4, #0xa8]
00776084: str      r7, [r4, #0xac]
00776088: str      r7, [r4, #0xb0]
0077608c: strb     r7, [r4, #0xb4]
00776090: str      r7, [r4, #0xb8]
00776094: str      r7, [r4, #0xbc]
00776098: str      r7, [r4, #0xc0]
0077609c: strb     r7, [r4, #0xc4]
007760a0: str      r7, [r4, #0xc8]
007760a4: str      r7, [r4, #0xcc]
007760a8: bl       #0x75e9ac
007760ac: ldr      r3, [r4, #0xc]
007760b0: mov      r0, r3
007760b4: ldr      r3, [r3]
007760b8: mov      lr, pc
007760bc: ldr      pc, [r3, #0x30]
007760c0: ldr      r3, [r4, #0xc]
007760c4: mov      r8, r0
007760c8: mov      r0, r3
007760cc: ldr      r3, [r3]
007760d0: mov      lr, pc
007760d4: ldr      pc, [r3, #0x34]
007760d8: mov      sl, r0
007760dc: mov      r0, r8
007760e0: bl       #0x30e4cc
007760e4: mov      r8, r0
007760e8: mov      r0, sl
007760ec: bl       #0x30e4cc
007760f0: mov      r3, r8
007760f4: mov      r2, r7
007760f8: mov      r1, r7
007760fc: str      r0, [sp]
00776100: mov      r0, r4
00776104: bl       #0x775d38
00776108: mov      r0, r4
0077610c: bl       #0x7741a0
00776110: mov      r1, r0
00776114: mov      r0, r5
00776118: bl       #0x30ec94
0077611c: mov      r1, r4
00776120: str      r0, [r4, #0x90]
00776124: mov      r0, r6
00776128: bl       #0x76d71c
0077612c: mov      r0, r4
00776130: add      sp, sp, #0xc
00776134: pop      {r4, r5, r6, r7, r8, sl, pc}
00776138: eoreq    lr, r1, r0, lsl #22
0077613c: andeq    r3, r0, r0, asr #13

# _ZN7gameswf5arrayINS_12render_cache5entryEE7reserveEi
0078a6f8: push     {r4, lr}
0078a6fc: ldrb     r3, [r0, #0xc]
0078a700: mov      r4, r0
0078a704: cmp      r3, #0
0078a708: bne      #0x78a754
0078a70c: cmp      r1, #0
0078a710: ldr      r2, [r0, #8]
0078a714: str      r1, [r0, #8]
0078a718: bne      #0x78a758
0078a71c: ldr      r0, [r0]
0078a720: cmp      r0, #0
0078a724: beq      #0x78a734
0078a728: mov      r1, #0x18
0078a72c: mul      r1, r1, r2
0078a730: bl       #0x752b38
0078a734: mov      r3, #0
0078a738: str      r3, [r4]
0078a73c: pop      {r4, pc}
0078a740: mov      r0, #0x18
0078a744: mul      r0, r0, r1
0078a748: mov      r1, ip
0078a74c: bl       #0x752b9c
0078a750: str      r0, [r4]
0078a754: pop      {r4, pc}
0078a758: ldr      ip, [r0]
0078a75c: cmp      ip, #0
0078a760: beq      #0x78a740
0078a764: mov      lr, #0x18
0078a768: mul      r2, lr, r2
0078a76c: mov      r0, ip
0078a770: mul      r1, lr, r1
0078a774: bl       #0x752bac
0078a778: str      r0, [r4]
0078a77c: pop      {r4, pc}

# _ZN7gameswf19preload_glyph_codesEPNS_14player_contextEPtiPNS_4fontEiPKNS_6filterE
0078b240: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078b244: sub      sp, sp, #0x5c
0078b248: ldr      r5, [sp, #0x80]
0078b24c: mov      r4, #0
0078b250: cmp      r2, #0
0078b254: mov      ip, #0x44000000
0078b258: mov      r6, r1
0078b25c: mvn      r1, #0
0078b260: mov      r8, r0
0078b264: str      r2, [sp, #0x18]
0078b268: str      ip, [sp, #0x20]
0078b26c: strb     r4, [sp, #0x42]
0078b270: mov      r7, r3
0078b274: str      r4, [sp, #0x24]
0078b278: str      r4, [sp, #0x38]
0078b27c: strh     r1, [sp, #0x3e]
0078b280: strh     r5, [sp, #0x3c]
0078b284: movle    r0, r4
0078b288: ble      #0x78b390
0078b28c: ldr      r2, [sp, #0x18]
0078b290: add      r3, sp, #0x44
0078b294: add      ip, sp, #0x54
0078b298: lsl      sl, r2, #1
0078b29c: add      sb, sp, #0x20
0078b2a0: str      r3, [sp, #0x14]
0078b2a4: str      ip, [sp, #0x1c]
0078b2a8: b        #0x78b2d8
0078b2ac: ldr      r3, [r8, #0x10]
0078b2b0: ldr      ip, [sp, #0x14]
0078b2b4: ldrh     r1, [sp, #0x40]
0078b2b8: ldr      r0, [r3, #0xc]
0078b2bc: ldr      r2, [sp, #0x38]
0078b2c0: ldrh     r3, [sp, #0x3c]
0078b2c4: str      ip, [sp]
0078b2c8: bl       #0x7c55d8
0078b2cc: add      r4, r4, #2
0078b2d0: cmp      r4, sl
0078b2d4: beq      #0x78b37c
0078b2d8: ldrh     ip, [r6, r4]
0078b2dc: mov      r0, r7
0078b2e0: mov      r1, sb
0078b2e4: mov      r2, ip
0078b2e8: mov      r3, r5
0078b2ec: strh     ip, [sp, #0x40]
0078b2f0: bl       #0x7d01bc
0078b2f4: cmp      r0, #0
0078b2f8: beq      #0x78b2cc
0078b2fc: ldr      fp, [r8, #0xc]
0078b300: ldr      r3, [fp, #0x28]
0078b304: cmp      r3, #0
0078b308: beq      #0x78b398
0078b30c: ldr      r3, [r3, #0x34]
0078b310: ldr      r2, [sp, #0x24]
0078b314: cmp      r2, r3
0078b318: bne      #0x78b2ac
0078b31c: ldr      r1, [sp, #0x84]
0078b320: mov      r2, #0
0078b324: strb     r2, [sp, #0x54]
0078b328: cmp      r1, #0
0078b32c: strb     r2, [sp, #0x55]
0078b330: strb     r2, [sp, #0x56]
0078b334: beq      #0x78b34c
0078b338: ldr      r3, [r1]
0078b33c: cmp      r3, #2
0078b340: beq      #0x78b3ac
0078b344: cmp      r3, #1
0078b348: bls      #0x78b3ec
0078b34c: ldr      ip, [sp, #0x1c]
0078b350: ldr      r0, [fp, #0x28]
0078b354: ldrh     r1, [sp, #0x40]
0078b358: str      ip, [sp]
0078b35c: ldr      ip, [sp, #0x14]
0078b360: ldrh     r3, [sp, #0x3c]
0078b364: ldr      r2, [sp, #0x38]
0078b368: add      r4, r4, #2
0078b36c: str      ip, [sp, #4]
0078b370: bl       #0x7d266c
0078b374: cmp      r4, sl
0078b378: bne      #0x78b2d8
0078b37c: ldr      r0, [sp, #0x24]
0078b380: cmp      r0, #0
0078b384: beq      #0x78b38c
0078b388: bl       #0x75a240
0078b38c: ldr      r0, [sp, #0x18]
0078b390: add      sp, sp, #0x5c
0078b394: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078b398: ldr      r2, [r8, #0x10]
0078b39c: ldr      r2, [r2, #0xc]
0078b3a0: cmp      r2, #0
0078b3a4: bne      #0x78b30c
0078b3a8: b        #0x78b2cc
0078b3ac: ldr      r3, [r1, #0x20]
0078b3b0: ldr      r2, [r1, #0x24]
0078b3b4: mov      r0, r3
0078b3b8: mov      r1, r2
0078b3bc: str      r2, [sp, #0x10]
0078b3c0: str      r3, [sp, #0xc]
0078b3c4: bl       #0x30e2f8
0078b3c8: ldr      r2, [sp, #0x10]
0078b3cc: ldr      r3, [sp, #0xc]
0078b3d0: cmp      r0, #0
0078b3d4: moveq    r0, r2
0078b3d8: movne    r0, r3
0078b3dc: bl       #0x8be2a0
0078b3e0: uxtb     r0, r0
0078b3e4: strb     r0, [sp, #0x54]
0078b3e8: b        #0x78b34c
0078b3ec: ldr      r3, [sp, #0x84]
0078b3f0: ldr      r0, [r3, #0x20]
0078b3f4: bl       #0x8be2a0
0078b3f8: ldr      ip, [sp, #0x84]
0078b3fc: strb     r0, [sp, #0x55]
0078b400: ldr      r0, [ip, #0x24]
0078b404: bl       #0x8be2a0
0078b408: strb     r0, [sp, #0x56]
0078b40c: b        #0x78b34c

# _ZN7gameswf12render_cacheD1Ev
0078ba28: push     {r4, r5, r6, lr}
0078ba2c: ldr      r3, [r0, #0x44]
0078ba30: mov      r4, r0
0078ba34: add      r0, r0, #0x40
0078ba38: cmp      r3, #0
0078ba3c: ble      #0x78bae4
0078ba40: mov      r5, #0
0078ba44: str      r5, [r4, #0x44]
0078ba48: mov      r1, r5
0078ba4c: bl       #0x779e7c
0078ba50: ldrb     r3, [r4, #0x3c]
0078ba54: str      r5, [r4, #0x34]
0078ba58: cmp      r3, r5
0078ba5c: bne      #0x78ba84
0078ba60: ldr      r0, [r4, #0x30]
0078ba64: ldr      r1, [r4, #0x38]
0078ba68: str      r3, [r4, #0x38]
0078ba6c: cmp      r0, r5
0078ba70: beq      #0x78ba7c
0078ba74: lsl      r1, r1, #3
0078ba78: bl       #0x752b38
0078ba7c: mov      r3, #0
0078ba80: str      r3, [r4, #0x30]
0078ba84: ldrb     r3, [r4, #0x2c]
0078ba88: mov      r2, #0
0078ba8c: str      r2, [r4, #0x24]
0078ba90: cmp      r3, r2
0078ba94: bne      #0x78bac0
0078ba98: ldr      r0, [r4, #0x20]
0078ba9c: ldr      r2, [r4, #0x28]
0078baa0: str      r3, [r4, #0x28]
0078baa4: cmp      r0, #0
0078baa8: beq      #0x78bab8
0078baac: mov      r1, #0xc
0078bab0: mul      r1, r1, r2
0078bab4: bl       #0x752b38
0078bab8: mov      r3, #0
0078babc: str      r3, [r4, #0x20]
0078bac0: ldr      ip, [r4, #0x14]
0078bac4: add      r0, r4, #0x10
0078bac8: cmp      ip, #0
0078bacc: ble      #0x78bb08
0078bad0: mov      r1, #0
0078bad4: str      r1, [r4, #0x14]
0078bad8: bl       #0x78a6f8
0078badc: mov      r0, r4
0078bae0: pop      {r4, r5, r6, pc}
0078bae4: bge      #0x78ba40
0078bae8: lsl      r2, r3, #1
0078baec: ldr      r1, [r4, #0x40]
0078baf0: mov      ip, #0
0078baf4: adds     r3, r3, #1
0078baf8: strh     ip, [r1, r2]
0078bafc: add      r2, r2, #2
0078bb00: bne      #0x78baec
0078bb04: b        #0x78ba40
0078bb08: bge      #0x78bad0
0078bb0c: mov      r1, #0x18
0078bb10: mul      r1, r1, ip
0078bb14: mov      r3, #0
0078bb18: ldr      lr, [r0]
0078bb1c: adds     ip, ip, #1
0078bb20: add      r2, lr, r1
0078bb24: str      r3, [lr, r1]
0078bb28: str      r3, [r2, #0x14]
0078bb2c: str      r3, [r2, #4]
0078bb30: str      r3, [r2, #8]
0078bb34: str      r3, [r2, #0xc]
0078bb38: str      r3, [r2, #0x10]
0078bb3c: add      r1, r1, #0x18
0078bb40: bne      #0x78bb18
0078bb44: mov      r1, #0
0078bb48: str      r1, [r4, #0x14]
0078bb4c: bl       #0x78a6f8
0078bb50: mov      r0, r4
0078bb54: pop      {r4, r5, r6, pc}

# _ZN7gameswf13texture_cache10unlock_allEPNS_14player_contextE
007935ec: push     {r4, r5, r6, lr}
007935f0: ldr      r3, [r0, #0xc]
007935f4: mov      r5, r0
007935f8: ldr      r4, [r3, #0x28]
007935fc: cmp      r4, #0
00793600: beq      #0x79362c
00793604: ldr      r3, [r4, #0x3c]
00793608: cmp      r3, #0
0079360c: beq      #0x79362c
00793610: ldr      r3, [r4, #0x34]
00793614: mov      r0, r3
00793618: ldr      r3, [r3]
0079361c: mov      lr, pc
00793620: ldr      pc, [r3, #0x1c]
00793624: mov      r3, #0
00793628: str      r3, [r4, #0x3c]
0079362c: ldr      r3, [r5, #0x10]
00793630: ldr      r4, [r3, #0xc]
00793634: cmp      r4, #0
00793638: beq      #0x793664
0079363c: ldr      r3, [r4, #0x3c]
00793640: cmp      r3, #0
00793644: beq      #0x793664
00793648: ldr      r3, [r4, #0x34]
0079364c: mov      r0, r3
00793650: ldr      r3, [r3]
00793654: mov      lr, pc
00793658: ldr      pc, [r3, #0x1c]
0079365c: mov      r3, #0
00793660: str      r3, [r4, #0x3c]
00793664: pop      {r4, r5, r6, pc}

# _ZN7gameswf8destructINS_14player_contextEEEvPKT_
007a7fb8: push     {r4, lr}
007a7fbc: subs     r4, r0, #0
007a7fc0: beq      #0x7a7fe0
007a7fc4: ldr      r3, [r4]
007a7fc8: mov      lr, pc
007a7fcc: ldr      pc, [r3]
007a7fd0: mov      r0, r4
007a7fd4: mov      r1, #0
007a7fd8: pop      {r4, lr}
007a7fdc: b        #0x752b38
007a7fe0: pop      {r4, pc}

# _ZN8RenderFX14DestroyContextEPN7gameswf14player_contextE
007a7fe4: cmp      r0, #0
007a7fe8: bxeq     lr
007a7fec: b        #0x7a7fb8

# _ZN8RenderFX14UnloadTexturesEPN7gameswf14player_contextE
007a8cdc: ldr      r3, [pc, #0x88]
007a8ce0: push     {r4, r5, r6, r7, r8, lr}
007a8ce4: subs     r7, r0, #0
007a8ce8: add      r3, pc, r3
007a8cec: beq      #0x7a8d5c
007a8cf0: ldr      r3, [r7, #0x18]
007a8cf4: cmp      r3, #0
007a8cf8: ble      #0x7a8d58
007a8cfc: mov      r6, #0
007a8d00: ldr      r3, [r7, #0x14]
007a8d04: ldr      r0, [r3, r6, lsl #2]
007a8d08: bl       #0x76d5b4
007a8d0c: ldr      r5, [r0, #0xc]
007a8d10: ldr      r3, [r5, #0xa0]
007a8d14: cmp      r3, #0
007a8d18: ble      #0x7a8d48
007a8d1c: mov      r4, #0
007a8d20: ldr      r3, [r5, #0x9c]
007a8d24: ldr      r3, [r3, r4, lsl #2]
007a8d28: add      r4, r4, #1
007a8d2c: mov      r0, r3
007a8d30: ldr      r3, [r3]
007a8d34: mov      lr, pc
007a8d38: ldr      pc, [r3, #0xc]
007a8d3c: ldr      r3, [r5, #0xa0]
007a8d40: cmp      r4, r3
007a8d44: blt      #0x7a8d20
007a8d48: ldr      r3, [r7, #0x18]
007a8d4c: add      r6, r6, #1
007a8d50: cmp      r6, r3
007a8d54: blt      #0x7a8d00
007a8d58: pop      {r4, r5, r6, r7, r8, pc}
007a8d5c: ldr      r2, [pc, #0xc]
007a8d60: ldr      r3, [r3, r2]
007a8d64: ldr      r7, [r3]
007a8d68: b        #0x7a8cf0
007a8d6c: andseq   fp, lr, r8, lsr #27
007a8d70: andeq    r4, r0, r0, lsr sb

# _ZN8RenderFX23ClearGlyphTextureCachesEPN7gameswf14player_contextE
007a953c: ldr      r3, [pc, #0x5c]
007a9540: push     {r4, lr}
007a9544: subs     r4, r0, #0
007a9548: add      r3, pc, r3
007a954c: beq      #0x7a9590
007a9550: ldr      r3, [r4, #0xc]
007a9554: cmp      r3, #0
007a9558: beq      #0x7a956c
007a955c: ldr      r0, [r3, #0x28]
007a9560: cmp      r0, #0
007a9564: beq      #0x7a956c
007a9568: bl       #0x793ed8
007a956c: ldr      r3, [r4, #0x10]
007a9570: cmp      r3, #0
007a9574: beq      #0x7a958c
007a9578: ldr      r0, [r3, #0xc]
007a957c: cmp      r0, #0
007a9580: beq      #0x7a958c
007a9584: pop      {r4, lr}
007a9588: b        #0x793ed8
007a958c: pop      {r4, pc}
007a9590: ldr      r2, [pc, #0xc]
007a9594: ldr      r3, [r3, r2]
007a9598: ldr      r4, [r3]
007a959c: b        #0x7a9550
007a95a0: andseq   fp, lr, r8, asr #10
007a95a4: andeq    r4, r0, r0, lsr sb

# _ZN8RenderFX19ForceTexturesToVRAMEbPN7gameswf14player_contextE
007aa0ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007aa0f0: sub      sp, sp, #0x5c
007aa0f4: add      sb, sp, #0x1c
007aa0f8: mov      ip, #0
007aa0fc: add      r3, sb, #8
007aa100: str      ip, [r3], #4
007aa104: ldr      sl, [pc, #0x1c8]
007aa108: str      ip, [r3], #4
007aa10c: mov      r2, #0
007aa110: mov      r4, #0x3f800000
007aa114: str      ip, [r3], #4
007aa118: subs     r7, r1, #0
007aa11c: mvn      r1, #0
007aa120: str      ip, [r3]
007aa124: add      sl, pc, sl
007aa128: str      r2, [sp, #0x3c]
007aa12c: strb     r1, [sp, #0x57]
007aa130: str      r4, [sp, #0x2c]
007aa134: mov      r6, r0
007aa138: str      r2, [sp, #0x44]
007aa13c: str      r2, [sp, #0x4c]
007aa140: str      r2, [sp, #0x48]
007aa144: str      r2, [sp, #0x50]
007aa148: str      r2, [sp, #0x34]
007aa14c: str      r4, [sp, #0x38]
007aa150: str      r4, [sp, #0x40]
007aa154: strb     r1, [sp, #0x54]
007aa158: strb     r1, [sp, #0x55]
007aa15c: strb     r1, [sp, #0x56]
007aa160: str      ip, [sp, #0x20]
007aa164: str      r4, [sp, #0x1c]
007aa168: beq      #0x7aa2c4
007aa16c: ldr      r3, [r7, #0xc]
007aa170: ldr      r3, [r3, #0x28]
007aa174: cmp      r3, #0
007aa178: beq      #0x7aa190
007aa17c: ldr      r3, [r3, #0x34]
007aa180: mov      r0, r3
007aa184: ldr      r3, [r3]
007aa188: mov      lr, pc
007aa18c: ldr      pc, [r3, #8]
007aa190: ldr      r3, [r7, #0x10]
007aa194: ldr      r3, [r3, #0xc]
007aa198: cmp      r3, #0
007aa19c: beq      #0x7aa1b4
007aa1a0: ldr      r3, [r3, #0x34]
007aa1a4: mov      r0, r3
007aa1a8: ldr      r3, [r3]
007aa1ac: mov      lr, pc
007aa1b0: ldr      pc, [r3, #8]
007aa1b4: ldr      r3, [r7, #0x18]
007aa1b8: cmp      r3, #0
007aa1bc: ble      #0x7aa29c
007aa1c0: add      r3, sp, #0x44
007aa1c4: ldr      fp, [pc, #0x10c]
007aa1c8: str      r3, [sp, #0xc]
007aa1cc: add      r3, sp, #0x34
007aa1d0: mov      r8, #0
007aa1d4: str      r3, [sp, #0x10]
007aa1d8: ldr      r3, [r7, #0x14]
007aa1dc: cmp      r6, #0
007aa1e0: ldr      r3, [r3, r8, lsl #2]
007aa1e4: str      r3, [sp, #0x14]
007aa1e8: bne      #0x7aa2b4
007aa1ec: ldr      r0, [sp, #0x14]
007aa1f0: bl       #0x76d5b4
007aa1f4: ldr      r5, [r0, #0xc]
007aa1f8: ldr      r3, [r5, #0xa0]
007aa1fc: cmp      r3, #0
007aa200: ble      #0x7aa284
007aa204: mov      r4, #0
007aa208: b        #0x7aa21c
007aa20c: ldr      r3, [r5, #0xa0]
007aa210: add      r4, r4, #1
007aa214: cmp      r4, r3
007aa218: bge      #0x7aa284
007aa21c: ldr      r3, [r5, #0x9c]
007aa220: ldr      r3, [r3, r4, lsl #2]
007aa224: mov      r0, r3
007aa228: ldr      r3, [r3]
007aa22c: mov      lr, pc
007aa230: ldr      pc, [r3, #8]
007aa234: cmp      r6, #0
007aa238: beq      #0x7aa20c
007aa23c: ldr      r3, [sl, fp]
007aa240: ldr      r2, [r5, #0x9c]
007aa244: mov      r1, sb
007aa248: ldr      r3, [r3]
007aa24c: ldr      r2, [r2, r4, lsl #2]
007aa250: add      r4, r4, #1
007aa254: ldr      ip, [r3]
007aa258: mov      r0, r3
007aa25c: ldr      r3, [sp, #0x54]
007aa260: str      r3, [sp, #4]
007aa264: ldr      r3, [sp, #0x10]
007aa268: str      r3, [sp]
007aa26c: ldr      r3, [sp, #0xc]
007aa270: mov      lr, pc
007aa274: ldr      pc, [ip, #0x80]
007aa278: ldr      r3, [r5, #0xa0]
007aa27c: cmp      r4, r3
007aa280: blt      #0x7aa21c
007aa284: cmp      r6, #0
007aa288: bne      #0x7aa2a4
007aa28c: ldr      r3, [r7, #0x18]
007aa290: add      r8, r8, #1
007aa294: cmp      r8, r3
007aa298: blt      #0x7aa1d8
007aa29c: add      sp, sp, #0x5c
007aa2a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007aa2a4: ldr      r0, [sp, #0x14]
007aa2a8: bl       #0x76d5b4
007aa2ac: bl       #0x774298
007aa2b0: b        #0x7aa28c
007aa2b4: mov      r0, r3
007aa2b8: bl       #0x76d5b4
007aa2bc: bl       #0x7751bc
007aa2c0: b        #0x7aa1ec
007aa2c4: ldr      r3, [pc, #0x10]
007aa2c8: ldr      r3, [sl, r3]
007aa2cc: ldr      r7, [r3]
007aa2d0: b        #0x7aa16c
007aa2d4: andseq   sl, lr, ip, ror #18
007aa2d8: strheq   r3, [r0], -r4
007aa2dc: andeq    r4, r0, r0, lsr sb

# _ZN8RenderFX10ClearFontsEPN7gameswf14player_contextE
007aac54: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007aac58: ldr      r7, [pc, #0x18c]
007aac5c: ldr      sb, [pc, #0x18c]
007aac60: sub      sp, sp, #0x18
007aac64: add      r7, pc, r7
007aac68: ldr      r3, [r7, sb]
007aac6c: subs     r6, r0, #0
007aac70: ldr      r3, [r3]
007aac74: str      r3, [sp, #0x14]
007aac78: beq      #0x7aadd8
007aac7c: ldr      r3, [sp, #0x10]
007aac80: mvn      r2, #0
007aac84: mov      r4, #0
007aac88: bfi      r3, r2, #0, #0x18
007aac8c: lsr      r2, r3, #0x18
007aac90: bfi      r2, r4, #0, #1
007aac94: mov      r1, #1
007aac98: str      r3, [sp, #0x10]
007aac9c: strb     r1, [sp]
007aaca0: strb     r2, [sp, #0x13]
007aaca4: strb     r4, [sp, #1]
007aaca8: ldr      r3, [r6, #0x18]
007aacac: cmp      r3, r4
007aacb0: ble      #0x7aad58
007aacb4: mov      r5, sp
007aacb8: ldr      r1, [r6, #0x14]
007aacbc: mov      r2, #0
007aacc0: mov      r3, r2
007aacc4: ldr      r1, [r1, r4, lsl #2]
007aacc8: ldr      r0, [r1, #0x94]
007aaccc: ldr      r1, [r0, #0x3c]
007aacd0: ldr      r1, [r1, #0x10]
007aacd4: bl       #0x7a8c08
007aacd8: ldr      r3, [r0, #4]
007aacdc: mov      sl, r0
007aace0: cmp      r3, #0
007aace4: ble      #0x7aad48
007aace8: mov      r8, #0
007aacec: b        #0x7aad00
007aacf0: ldr      r3, [sl, #4]
007aacf4: add      r8, r8, #1
007aacf8: cmp      r8, r3
007aacfc: bge      #0x7aad48
007aad00: ldr      r3, [sl]
007aad04: mov      r1, #0x20
007aad08: ldr      r3, [r3, r8, lsl #2]
007aad0c: mov      r0, r3
007aad10: ldr      r3, [r3]
007aad14: mov      lr, pc
007aad18: ldr      pc, [r3, #8]
007aad1c: cmp      r0, #0
007aad20: beq      #0x7aacf0
007aad24: ldr      r3, [sl]
007aad28: mov      r1, r5
007aad2c: mov      r2, #0
007aad30: ldr      r0, [r3, r8, lsl #2]
007aad34: bl       #0x790ab0
007aad38: ldr      r3, [sl, #4]
007aad3c: add      r8, r8, #1
007aad40: cmp      r8, r3
007aad44: blt      #0x7aad00
007aad48: ldr      r3, [r6, #0x18]
007aad4c: add      r4, r4, #1
007aad50: cmp      r4, r3
007aad54: blt      #0x7aacb8
007aad58: ldr      r4, [r6, #0xc]
007aad5c: cmp      r4, #0
007aad60: beq      #0x7aad7c
007aad64: add      r0, r4, #0x24
007aad68: bl       #0x7aa790
007aad6c: ldr      r0, [r4, #0x28]
007aad70: cmp      r0, #0
007aad74: beq      #0x7aad7c
007aad78: bl       #0x793ed8
007aad7c: ldr      r4, [r6, #0x10]
007aad80: cmp      r4, #0
007aad84: beq      #0x7aada0
007aad88: add      r0, r4, #4
007aad8c: bl       #0x7aa840
007aad90: ldr      r0, [r4, #0xc]
007aad94: cmp      r0, #0
007aad98: beq      #0x7aada0
007aad9c: bl       #0x793ed8
007aada0: ldrsb    r3, [sp]
007aada4: cmn      r3, #1
007aada8: beq      #0x7aadc8
007aadac: ldr      r3, [r7, sb]
007aadb0: ldr      r2, [sp, #0x14]
007aadb4: ldr      r3, [r3]
007aadb8: cmp      r2, r3
007aadbc: bne      #0x7aade8
007aadc0: add      sp, sp, #0x18
007aadc4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007aadc8: ldr      r0, [sp, #0xc]
007aadcc: ldr      r1, [sp, #8]
007aadd0: bl       #0x752b38
007aadd4: b        #0x7aadac
007aadd8: ldr      r3, [pc, #0x14]
007aaddc: ldr      r3, [r7, r3]
007aade0: ldr      r6, [r3]
007aade4: b        #0x7aac7c
007aade8: bl       #0x30e310
007aadec: andseq   sb, lr, ip, lsr #28
007aadf0: andeq    r4, r0, ip, lsr #1
007aadf4: andeq    r4, r0, r0, lsr sb

# _ZN8RenderFX4LoadEPKcPN7gameswf14player_contextE
007ab784: push     {r4, r5, r6, r7, r8, sl, lr}
007ab788: ldr      r6, [pc, #0x188]
007ab78c: ldr      r7, [pc, #0x188]
007ab790: sub      sp, sp, #0x34
007ab794: add      r6, pc, r6
007ab798: ldr      r3, [r6, r7]
007ab79c: subs     r8, r2, #0
007ab7a0: mov      r5, r0
007ab7a4: ldr      r3, [r3]
007ab7a8: mov      r4, r1
007ab7ac: str      r3, [sp, #0x2c]
007ab7b0: beq      #0x7ab8f4
007ab7b4: add      r0, r5, #0x44
007ab7b8: mov      r1, r4
007ab7bc: bl       #0x76c818
007ab7c0: mov      r1, #0
007ab7c4: mov      r0, #0xe0
007ab7c8: bl       #0x752ba8
007ab7cc: mov      r1, r8
007ab7d0: mov      sl, r0
007ab7d4: bl       #0x76f180
007ab7d8: mov      r1, sl
007ab7dc: add      r0, r5, #0x38
007ab7e0: bl       #0x7a87bc
007ab7e4: ldr      r3, [r5, #0x38]
007ab7e8: mov      r0, r4
007ab7ec: str      r5, [r3, #0x94]
007ab7f0: mov      r3, #1
007ab7f4: strb     r3, [sp, #0x18]
007ab7f8: mov      r3, #0
007ab7fc: strb     r3, [sp, #0x19]
007ab800: bl       #0x30de54
007ab804: adds     r3, r4, r0
007ab808: bhs      #0x7ab82c
007ab80c: ldrsb    r2, [r4, r0]
007ab810: cmp      r2, #0x2f
007ab814: beq      #0x7ab82c
007ab818: cmp      r2, #0x5c
007ab81c: beq      #0x7ab82c
007ab820: sub      r3, r3, #1
007ab824: cmp      r4, r3
007ab828: bls      #0x7ab8d4
007ab82c: rsb      r2, r4, #1
007ab830: add      r2, r3, r2
007ab834: cmp      r2, #0
007ab838: ble      #0x7ab870
007ab83c: add      r8, sp, #4
007ab840: mov      r1, r4
007ab844: mov      r0, r8
007ab848: bl       #0x751eb4
007ab84c: ldrsb    r3, [sp, #4]
007ab850: ldr      r0, [r5, #0x38]
007ab854: cmn      r3, #1
007ab858: addne    r1, r8, #1
007ab85c: ldreq    r1, [sp, #0x10]
007ab860: bl       #0x76c868
007ab864: ldrsb    r3, [sp, #4]
007ab868: cmn      r3, #1
007ab86c: beq      #0x7ab904
007ab870: ldrsb    r3, [sp, #0x18]
007ab874: cmn      r3, #1
007ab878: beq      #0x7ab8e4
007ab87c: mov      r2, r4
007ab880: mov      r0, sp
007ab884: ldr      r1, [r5, #0x38]
007ab888: bl       #0x7736f4
007ab88c: add      r0, r5, #0x3c
007ab890: ldr      r1, [sp]
007ab894: bl       #0x75a258
007ab898: ldr      r0, [sp]
007ab89c: cmp      r0, #0
007ab8a0: beq      #0x7ab8a8
007ab8a4: bl       #0x75a240
007ab8a8: ldr      r3, [r5, #0x3c]
007ab8ac: mov      r0, r5
007ab8b0: ldr      r1, [r3, #0x10]
007ab8b4: bl       #0x7a7ee8
007ab8b8: ldr      r3, [r6, r7]
007ab8bc: ldr      r2, [sp, #0x2c]
007ab8c0: ldr      r3, [r3]
007ab8c4: cmp      r2, r3
007ab8c8: bne      #0x7ab914
007ab8cc: add      sp, sp, #0x34
007ab8d0: pop      {r4, r5, r6, r7, r8, sl, pc}
007ab8d4: ldrsb    r2, [r3]
007ab8d8: cmp      r2, #0x2f
007ab8dc: bne      #0x7ab818
007ab8e0: b        #0x7ab82c
007ab8e4: ldr      r0, [sp, #0x24]
007ab8e8: ldr      r1, [sp, #0x20]
007ab8ec: bl       #0x752b38
007ab8f0: b        #0x7ab87c
007ab8f4: ldr      r3, [pc, #0x24]
007ab8f8: ldr      r3, [r6, r3]
007ab8fc: ldr      r8, [r3]
007ab900: b        #0x7ab7b4
007ab904: ldr      r0, [sp, #0x10]
007ab908: ldr      r1, [sp, #0xc]
007ab90c: bl       #0x752b38
007ab910: b        #0x7ab870
007ab914: bl       #0x30e310
007ab918: ldrsheq  sb, [lr], -ip
007ab91c: andeq    r4, r0, ip, lsr #1
007ab920: andeq    r4, r0, r0, lsr sb

# _ZN7gameswf26bitmap_glyph_texture_cacheC1Eii
007c4648: push     {r4, lr}
007c464c: mov      ip, #0
007c4650: sub      sp, sp, #8
007c4654: mov      r4, r0
007c4658: mov      r3, #4
007c465c: str      ip, [sp]
007c4660: bl       #0x7941c0
007c4664: mov      r0, r4
007c4668: add      sp, sp, #8
007c466c: pop      {r4, pc}

# _ZN7gameswf26bitmap_glyph_texture_cacheC2Eii
007c4670: push     {r4, lr}
007c4674: mov      ip, #0
007c4678: sub      sp, sp, #8
007c467c: mov      r4, r0
007c4680: mov      r3, #4
007c4684: str      ip, [sp]
007c4688: bl       #0x7941c0
007c468c: mov      r0, r4
007c4690: add      sp, sp, #8
007c4694: pop      {r4, pc}

# _ZN7gameswf26bitmap_glyph_texture_cache16add_glyph_regionEtPvi
007c53c4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007c53c8: sub      sp, sp, #0x64
007c53cc: mov      r4, r2
007c53d0: mov      r7, r1
007c53d4: mov      r6, r3
007c53d8: mov      r5, r0
007c53dc: bl       #0x7c44d8
007c53e0: add      r3, sp, #0x14
007c53e4: str      r3, [sp]
007c53e8: mov      r8, r0
007c53ec: ldr      ip, [r4]
007c53f0: mov      r0, r4
007c53f4: add      r1, sp, #0x48
007c53f8: mov      r2, r7
007c53fc: mov      r3, r6
007c5400: mov      lr, pc
007c5404: ldr      pc, [ip, #8]
007c5408: cmp      r0, #0
007c540c: bne      #0x7c541c
007c5410: mov      r0, #0
007c5414: add      sp, sp, #0x64
007c5418: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007c541c: ldr      r2, [sp, #0x4c]
007c5420: ldr      r3, [sp, #0x50]
007c5424: add      r1, sp, #0x58
007c5428: add      r2, r2, #1
007c542c: add      r3, r3, #1
007c5430: add      r0, sp, #0x5c
007c5434: str      r2, [sp, #0x5c]
007c5438: str      r3, [sp, #0x58]
007c543c: bl       #0x793560
007c5440: mov      r0, r5
007c5444: ldr      r1, [sp, #0x5c]
007c5448: ldr      r2, [sp, #0x58]
007c544c: bl       #0x794668
007c5450: subs     sb, r0, #0
007c5454: beq      #0x7c5410
007c5458: mov      r2, #1
007c545c: ldrd     sl, fp, [r5]
007c5460: uxtb     r1, r6
007c5464: adds     r2, r2, sl
007c5468: mov      r3, #0
007c546c: adc      r3, r3, fp
007c5470: mov      r0, r5
007c5474: ldrd     sl, fp, [r5]
007c5478: strd     sl, fp, [sb]
007c547c: mov      fp, r7
007c5480: mov      sl, #0
007c5484: strd     r2, r3, [r0], #0x30
007c5488: lsl      r7, r1, #0x10
007c548c: orr      r2, sl, r4
007c5490: mov      r3, fp
007c5494: strd     r2, r3, [sp, #8]
007c5498: orr      r3, fp, r7
007c549c: orr      r2, r2, sl
007c54a0: strd     r2, r3, [sp, #0x38]
007c54a4: add      r1, sp, #0x38
007c54a8: mov      r3, #0
007c54ac: mov      r2, #0
007c54b0: strd     r2, r3, [sp, #0x40]
007c54b4: bl       #0x7c536c
007c54b8: add      r2, sp, #0x28
007c54bc: mov      r1, sb
007c54c0: str      sb, [r0]
007c54c4: mov      r0, r5
007c54c8: bl       #0x756c30
007c54cc: ldr      r3, [r5, #0x34]
007c54d0: ldr      r6, [r5, #0x38]
007c54d4: ldr      r7, [sp, #0x30]
007c54d8: mov      r0, r3
007c54dc: ldr      r3, [r3]
007c54e0: mov      lr, pc
007c54e4: ldr      pc, [r3, #0x24]
007c54e8: mov      sb, r0
007c54ec: mov      r0, r6
007c54f0: bl       #0x30e964
007c54f4: mov      r4, r0
007c54f8: mov      r0, sb
007c54fc: bl       #0x30e964
007c5500: mov      r1, r0
007c5504: mov      r0, r7
007c5508: bl       #0x30ed6c
007c550c: mov      r1, r4
007c5510: bl       #0x30ed6c
007c5514: ldr      r1, [sp, #0x28]
007c5518: mov      r7, r0
007c551c: mov      r0, r4
007c5520: bl       #0x30ed6c
007c5524: mov      r1, r0
007c5528: mov      r0, r7
007c552c: bl       #0x30eba4
007c5530: bl       #0x30e4cc
007c5534: ldr      r3, [r5, #0x34]
007c5538: add      r8, r8, r0
007c553c: mov      r0, r3
007c5540: ldr      r3, [r3]
007c5544: mov      lr, pc
007c5548: ldr      pc, [r3, #0x24]
007c554c: ldr      r3, [sp, #0x58]
007c5550: mul      r5, r6, r0
007c5554: cmp      r3, sl
007c5558: ble      #0x7c5588
007c555c: mov      r4, r8
007c5560: ldr      r2, [sp, #0x5c]
007c5564: mov      r0, r4
007c5568: mov      r1, #0
007c556c: mul      r2, r2, r6
007c5570: bl       #0x30e460
007c5574: ldr      r3, [sp, #0x58]
007c5578: add      sl, sl, #1
007c557c: add      r4, r4, r5
007c5580: cmp      r3, sl
007c5584: bgt      #0x7c5560
007c5588: ldr      r3, [sp, #0x50]
007c558c: cmp      r3, #0
007c5590: ble      #0x7c55d0
007c5594: ldr      r1, [sp, #0x48]
007c5598: mov      r4, #0
007c559c: b        #0x7c55a4
007c55a0: ldr      r1, [sp, #0x48]
007c55a4: ldr      r3, [sp, #0x54]
007c55a8: ldr      r2, [sp, #0x4c]
007c55ac: mov      r0, r8
007c55b0: mla      r1, r4, r1, r3
007c55b4: mul      r2, r2, r6
007c55b8: bl       #0x30e868
007c55bc: ldr      r3, [sp, #0x50]
007c55c0: add      r4, r4, #1
007c55c4: add      r8, r8, r5
007c55c8: cmp      r3, r4
007c55cc: bgt      #0x7c55a0
007c55d0: mov      r0, #1
007c55d4: b        #0x7c5414

# _ZN7gameswf26bitmap_glyph_texture_cache16get_glyph_regionEtPviRNS_4rectE
007c55d8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007c55dc: mov      r8, r3
007c55e0: uxtb     r3, r3
007c55e4: sub      sp, sp, #0x24
007c55e8: mov      sl, #0
007c55ec: lsl      r3, r3, #0x10
007c55f0: str      sl, [sp, #8]
007c55f4: mov      r4, r0
007c55f8: str      r3, [sp, #0xc]
007c55fc: orr      r0, sl, r2
007c5600: mov      r7, r1
007c5604: mov      fp, r1
007c5608: mov      r6, r2
007c560c: add      sl, r4, #0x30
007c5610: ldrd     r2, r3, [sp, #8]
007c5614: add      sb, sp, #0x10
007c5618: orr      r0, r0, r2
007c561c: orr      r1, r1, r3
007c5620: strd     r0, r1, [sp, #0x10]
007c5624: mov      r2, #0
007c5628: mov      r3, #0
007c562c: mov      r0, sl
007c5630: mov      r1, sb
007c5634: strd     r2, r3, [sp, #0x18]
007c5638: bl       #0x756f1c
007c563c: ldr      r5, [pc, #0xa0]
007c5640: cmp      r0, #0
007c5644: add      r5, pc, r5
007c5648: blt      #0x7c5674
007c564c: ldr      r3, [r4, #0x30]
007c5650: add      r0, r3, r0, lsl #5
007c5654: ldr      r1, [r0, #0x20]
007c5658: cmp      r1, #0
007c565c: beq      #0x7c566c
007c5660: mov      r0, r4
007c5664: ldr      r2, [sp, #0x48]
007c5668: bl       #0x756c30
007c566c: add      sp, sp, #0x24
007c5670: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007c5674: mov      r0, r4
007c5678: mov      r1, r7
007c567c: mov      r2, r6
007c5680: mov      r3, r8
007c5684: bl       #0x7c53c4
007c5688: cmp      r0, #0
007c568c: beq      #0x7c56a8
007c5690: mov      r0, sl
007c5694: mov      r1, sb
007c5698: bl       #0x756f1c
007c569c: cmp      r0, #0
007c56a0: bge      #0x7c564c
007c56a4: b        #0x7c566c
007c56a8: ldr      r3, [pc, #0x38]
007c56ac: ldr      r3, [r5, r3]
007c56b0: ldr      r3, [r3]
007c56b4: mov      r0, r3
007c56b8: ldr      r3, [r3]
007c56bc: mov      lr, pc
007c56c0: ldr      pc, [r3, #0x94]
007c56c4: mov      r0, r4
007c56c8: bl       #0x793ed8
007c56cc: mov      r0, r4
007c56d0: mov      r1, r7
007c56d4: mov      r2, r6
007c56d8: mov      r3, r8
007c56dc: bl       #0x7c53c4
007c56e0: b        #0x7c5690
007c56e4: andseq   pc, ip, ip, asr #8
007c56e8: strheq   r3, [r0], -r4

# _ZN7gameswf19glyph_texture_cacheD1Ev
007d1a28: push     {r4, lr}
007d1a2c: ldr      r3, [r0, #0x44]
007d1a30: mov      r4, r0
007d1a34: add      r0, r0, #0x40
007d1a38: cmp      r3, #0
007d1a3c: ble      #0x7d1a60
007d1a40: mov      r3, #0
007d1a44: mov      r1, r3
007d1a48: str      r3, [r4, #0x44]
007d1a4c: bl       #0x75722c
007d1a50: mov      r0, r4
007d1a54: bl       #0x758b8c
007d1a58: mov      r0, r4
007d1a5c: pop      {r4, pc}
007d1a60: bge      #0x7d1a40
007d1a64: mov      r1, #0
007d1a68: ldr      r2, [r0]
007d1a6c: strb     r1, [r2, r3]
007d1a70: adds     r3, r3, #1
007d1a74: bne      #0x7d1a68
007d1a78: b        #0x7d1a40

# _ZN7gameswf19glyph_texture_cache16add_glyph_regionEtPviRNS0_11filter_infoEb
007d1da4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d1da8: sub      sp, sp, #0x13c
007d1dac: ldr      r6, [sp, #0x160]
007d1db0: str      r0, [sp, #0x10]
007d1db4: ldr      ip, [r0, #0x50]
007d1db8: str      r1, [sp, #0x18]
007d1dbc: ldrb     r1, [r6, #1]
007d1dc0: ldr      r8, [ip, #4]
007d1dc4: ldrb     fp, [r6]
007d1dc8: str      r1, [sp, #0x28]
007d1dcc: mov      r5, r2
007d1dd0: ldrb     r2, [r6, #2]
007d1dd4: mov      sb, r3
007d1dd8: ldrb     sl, [sp, #0x164]
007d1ddc: str      r2, [sp, #8]
007d1de0: bl       #0x7c44d8
007d1de4: str      r0, [sp, #0x24]
007d1de8: mov      r0, sb
007d1dec: bl       #0x30e964
007d1df0: mov      r1, r8
007d1df4: bl       #0x30ed6c
007d1df8: bl       #0x30e4cc
007d1dfc: mov      r1, #0
007d1e00: mov      r2, r0
007d1e04: ldr      r0, [r5, #0x24]
007d1e08: bl       #0x70a7fc
007d1e0c: ldr      r0, [r5, #0x24]
007d1e10: ldr      r1, [sp, #0x18]
007d1e14: mov      r2, #4
007d1e18: bl       #0x709660
007d1e1c: ldr      r3, [pc, #0x840]
007d1e20: subs     r7, r0, #0
007d1e24: add      r3, pc, r3
007d1e28: str      r3, [sp, #0x14]
007d1e2c: bne      #0x7d2138
007d1e30: ldr      r3, [r5, #0x24]
007d1e34: ldr      r4, [r3, #0x54]
007d1e38: ldrsb    r3, [r4, #0x5e]
007d1e3c: cmp      r3, #1
007d1e40: movne    r3, #0
007d1e44: moveq    r3, #1
007d1e48: cmp      r3, #0
007d1e4c: str      r3, [sp, #0x54]
007d1e50: addeq    r4, r4, #0x4c
007d1e54: bne      #0x7d2404
007d1e58: mov      r0, fp
007d1e5c: bl       #0x30e964
007d1e60: mov      r1, r8
007d1e64: bl       #0x30ed6c
007d1e68: bl       #0x8be2a0
007d1e6c: uxtb     fp, r0
007d1e70: ldr      r0, [sp, #0x28]
007d1e74: bl       #0x30e964
007d1e78: mov      r1, r8
007d1e7c: bl       #0x30ed6c
007d1e80: bl       #0x8be2a0
007d1e84: uxtb     r7, r0
007d1e88: ldr      r0, [sp, #8]
007d1e8c: bl       #0x30e964
007d1e90: mov      r1, r8
007d1e94: bl       #0x30ed6c
007d1e98: bl       #0x8be2a0
007d1e9c: ldr      r2, [r4, #8]
007d1ea0: cmp      fp, #0
007d1ea4: uxtb     r8, r0
007d1ea8: add      r2, r2, #1
007d1eac: str      r2, [sp, #0x134]
007d1eb0: ldr      r3, [r4]
007d1eb4: add      r3, r3, #1
007d1eb8: str      r3, [sp, #0x130]
007d1ebc: bne      #0x7d2144
007d1ec0: orrs     lr, r8, r7
007d1ec4: addne    r2, r2, r7, lsl #1
007d1ec8: addne    r3, r3, r8, lsl #1
007d1ecc: strne    r2, [sp, #0x134]
007d1ed0: strne    r3, [sp, #0x130]
007d1ed4: add      r0, sp, #0x134
007d1ed8: add      r1, sp, #0x130
007d1edc: bl       #0x793560
007d1ee0: cmp      sl, #0
007d1ee4: beq      #0x7d218c
007d1ee8: ldr      r0, [sp, #0x10]
007d1eec: ldr      r1, [sp, #0x134]
007d1ef0: ldr      r2, [sp, #0x130]
007d1ef4: bl       #0x794488
007d1ef8: mov      sl, r0
007d1efc: cmp      sl, #0
007d1f00: beq      #0x7d2138
007d1f04: ldr      r3, [sp, #0x10]
007d1f08: ldr      ip, [sp, #0x10]
007d1f0c: mov      r0, #1
007d1f10: ldrd     r2, r3, [r3]
007d1f14: mov      r1, #0
007d1f18: adds     r0, r0, r2
007d1f1c: adc      r1, r1, r3
007d1f20: strd     r2, r3, [sl]
007d1f24: strd     r0, r1, [ip], #0x30
007d1f28: ldrb     lr, [r6, #2]
007d1f2c: ldrb     r0, [r6, #1]
007d1f30: ldr      r3, [sp, #0x18]
007d1f34: ldrb     r1, [r6]
007d1f38: lsl      lr, lr, #8
007d1f3c: str      r3, [sp, #0xc]
007d1f40: orr      r3, lr, r0, lsl #16
007d1f44: mov      lr, #0
007d1f48: str      lr, [sp, #8]
007d1f4c: uxtb     sb, sb
007d1f50: orr      lr, r3, r1
007d1f54: ldrd     r0, r1, [sp, #8]
007d1f58: orr      r0, r0, r5
007d1f5c: strd     r0, r1, [sp, #0x28]
007d1f60: ldrd     r2, r3, [sp, #0x28]
007d1f64: mov      r1, #0
007d1f68: lsl      sb, sb, #0x10
007d1f6c: str      sb, [sp, #0x1c]
007d1f70: str      r1, [sp, #0x18]
007d1f74: ldrd     r0, r1, [sp, #0x18]
007d1f78: orr      r2, r2, r0
007d1f7c: orr      r3, r3, r1
007d1f80: add      r1, sp, #0x120
007d1f84: strd     r2, r3, [r1]
007d1f88: add      r0, sp, #0x130
007d1f8c: mov      r2, lr
007d1f90: asr      r3, r2, #0x1f
007d1f94: strd     r2, r3, [r0, #-8]
007d1f98: mov      r0, ip
007d1f9c: bl       #0x7c536c
007d1fa0: str      sl, [r0]
007d1fa4: mov      r1, sl
007d1fa8: add      r2, sp, #0x110
007d1fac: ldr      r0, [sp, #0x10]
007d1fb0: bl       #0x756c30
007d1fb4: ldr      r1, [sp, #0x10]
007d1fb8: ldr      sl, [sp, #0x118]
007d1fbc: ldr      r2, [r1, #0x38]
007d1fc0: ldr      r3, [r1, #0x34]
007d1fc4: str      r2, [sp, #0x40]
007d1fc8: mov      r0, r3
007d1fcc: ldr      r3, [r3]
007d1fd0: mov      lr, pc
007d1fd4: ldr      pc, [r3, #0x24]
007d1fd8: mov      r6, r0
007d1fdc: ldr      r0, [sp, #0x40]
007d1fe0: bl       #0x30e964
007d1fe4: mov      r5, r0
007d1fe8: mov      r0, r6
007d1fec: bl       #0x30e964
007d1ff0: mov      r1, r0
007d1ff4: mov      r0, sl
007d1ff8: bl       #0x30ed6c
007d1ffc: mov      r1, r5
007d2000: bl       #0x30ed6c
007d2004: ldr      r1, [sp, #0x110]
007d2008: mov      r6, r0
007d200c: mov      r0, r5
007d2010: bl       #0x30ed6c
007d2014: mov      r1, r0
007d2018: mov      r0, r6
007d201c: bl       #0x30eba4
007d2020: bl       #0x30e4cc
007d2024: ldr      ip, [sp, #0x10]
007d2028: ldr      lr, [sp, #0x24]
007d202c: ldr      r3, [ip, #0x34]
007d2030: add      r0, lr, r0
007d2034: str      r0, [sp, #0x50]
007d2038: mov      r0, r3
007d203c: ldr      r3, [r3]
007d2040: mov      lr, pc
007d2044: ldr      pc, [r3, #0x24]
007d2048: ldr      r1, [sp, #0x40]
007d204c: ldr      r3, [sp, #0x130]
007d2050: mul      r0, r1, r0
007d2054: cmp      r3, #0
007d2058: str      r0, [sp, #0x3c]
007d205c: ble      #0x7d2098
007d2060: ldr      r6, [sp, #8]
007d2064: ldr      r5, [sp, #0x50]
007d2068: mov      sb, r0
007d206c: mov      sl, r1
007d2070: ldr      r2, [sp, #0x134]
007d2074: mov      r0, r5
007d2078: mov      r1, #0
007d207c: mul      r2, r2, sl
007d2080: bl       #0x30e460
007d2084: ldr      r3, [sp, #0x130]
007d2088: add      r6, r6, #1
007d208c: add      r5, r5, sb
007d2090: cmp      r3, r6
007d2094: bgt      #0x7d2070
007d2098: cmp      fp, #0
007d209c: ldr      r6, [r4, #0xc]
007d20a0: ldr      r5, [r4, #8]
007d20a4: ldr      sb, [r4, #4]
007d20a8: ldr      sl, [r4]
007d20ac: bne      #0x7d21a4
007d20b0: orrs     r1, r8, r7
007d20b4: bne      #0x7d2468
007d20b8: cmp      sl, #0
007d20bc: ble      #0x7d2124
007d20c0: ldr      r7, [sp, #0x50]
007d20c4: ldr      fp, [sp, #0x40]
007d20c8: mov      r8, #0
007d20cc: mvn      r4, #0
007d20d0: cmp      fp, #1
007d20d4: mov      r3, r7
007d20d8: beq      #0x7d2178
007d20dc: cmp      sb, #0
007d20e0: movgt    r2, #0
007d20e4: ble      #0x7d210c
007d20e8: strb     r4, [r3]
007d20ec: strb     r4, [r3, #1]
007d20f0: strb     r4, [r3, #2]
007d20f4: ldrb     r1, [r6, r2]
007d20f8: add      r2, r2, #1
007d20fc: cmp      r2, sb
007d2100: strb     r1, [r3, #3]
007d2104: add      r3, r3, #4
007d2108: bne      #0x7d20e8
007d210c: ldr      r0, [sp, #0x3c]
007d2110: add      r8, r8, #1
007d2114: cmp      r8, sl
007d2118: add      r7, r7, r0
007d211c: add      r6, r6, r5
007d2120: bne      #0x7d20d0
007d2124: ldr      r1, [sp, #0x54]
007d2128: cmp      r1, #0
007d212c: bne      #0x7d215c
007d2130: mov      r0, #1
007d2134: b        #0x7d213c
007d2138: mov      r0, #0
007d213c: add      sp, sp, #0x13c
007d2140: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d2144: lsl      r1, fp, #1
007d2148: add      r3, r1, r3
007d214c: add      r2, r1, r2
007d2150: str      r2, [sp, #0x134]
007d2154: str      r3, [sp, #0x130]
007d2158: b        #0x7d1ed4
007d215c: ldr      r2, [sp, #0x10]
007d2160: add      r1, sp, #0xf8
007d2164: ldr      r3, [r2, #0x50]
007d2168: ldr      r0, [r3]
007d216c: bl       #0x70c924
007d2170: mov      r0, #1
007d2174: b        #0x7d213c
007d2178: mov      r0, r7
007d217c: mov      r1, r6
007d2180: mov      r2, sb
007d2184: bl       #0x30e868
007d2188: b        #0x7d210c
007d218c: ldr      r0, [sp, #0x10]
007d2190: ldr      r1, [sp, #0x134]
007d2194: ldr      r2, [sp, #0x130]
007d2198: bl       #0x794668
007d219c: mov      sl, r0
007d21a0: b        #0x7d1efc
007d21a4: ldr      r5, [sp, #0x134]
007d21a8: ldr      r2, [sp, #0x10]
007d21ac: mul      r5, r5, r3
007d21b0: add      r7, r2, #0x40
007d21b4: cmp      r5, #0
007d21b8: ldr      r6, [r2, #0x44]
007d21bc: beq      #0x7d21cc
007d21c0: ldr      r3, [r2, #0x48]
007d21c4: cmp      r5, r3
007d21c8: bgt      #0x7d262c
007d21cc: cmp      r5, r6
007d21d0: ble      #0x7d21ec
007d21d4: mov      r3, #0
007d21d8: ldr      r2, [r7]
007d21dc: strb     r3, [r2, r6]
007d21e0: add      r6, r6, #1
007d21e4: cmp      r6, r5
007d21e8: bne      #0x7d21d8
007d21ec: ldr      r3, [sp, #0x10]
007d21f0: mov      r2, r5
007d21f4: mov      r1, #0
007d21f8: str      r5, [r3, #0x44]
007d21fc: ldr      r0, [r3, #0x40]
007d2200: bl       #0x30e460
007d2204: mov      r0, fp
007d2208: bl       #0x30e964
007d220c: mov      r1, r0
007d2210: bl       #0x30eba4
007d2214: ldr      r2, [pc, #0x44c]
007d2218: ldr      ip, [sp, #0x14]
007d221c: lsl      r3, fp, #1
007d2220: rsb      fp, fp, r3
007d2224: ldr      r2, [ip, r2]
007d2228: ldr      lr, [sp, #0x10]
007d222c: add      fp, fp, #1
007d2230: add      r3, r3, #1
007d2234: ldr      sb, [sp, #0x134]
007d2238: str      r0, [sp, #0x18]
007d223c: str      r2, [sp, #0x28]
007d2240: str      fp, [sp, #0x48]
007d2244: str      r3, [sp, #0x4c]
007d2248: str      fp, [sp, #0x44]
007d224c: str      r3, [sp, #0x24]
007d2250: ldr      fp, [r4]
007d2254: ldr      r6, [lr, #0x40]
007d2258: str      r2, [sp, #0x38]
007d225c: add      r3, sp, #0x44
007d2260: ldm      r3, {r3, ip, lr}
007d2264: cmp      r3, #0
007d2268: rsblt    r3, r3, #0
007d226c: str      r3, [sp, #0x34]
007d2270: mov      r5, sb
007d2274: str      ip, [sp, #0x14]
007d2278: str      lr, [sp, #8]
007d227c: ldr      r0, [sp, #0x14]
007d2280: cmp      r0, #0
007d2284: rsblt    r0, r0, #0
007d2288: bl       #0x30e964
007d228c: mov      r1, r0
007d2290: ldr      r0, [sp, #0x18]
007d2294: bl       #0x30e3ac
007d2298: mov      r7, r0
007d229c: ldr      r0, [sp, #0x34]
007d22a0: bl       #0x30e964
007d22a4: mov      r1, r0
007d22a8: mov      r0, r7
007d22ac: bl       #0x30e3ac
007d22b0: ldr      r1, [sp, #0x18]
007d22b4: bl       #0x30ec94
007d22b8: mov      r1, #0x43000000
007d22bc: add      r1, r1, #0x7f0000
007d22c0: bl       #0x30ed6c
007d22c4: mov      r1, #0
007d22c8: mov      r8, r0
007d22cc: bl       #0x30e70c
007d22d0: cmp      r0, #0
007d22d4: ldr      r7, [r4, #0xc]
007d22d8: movne    r0, #0
007d22dc: bne      #0x7d22fc
007d22e0: mov      r1, #0x43000000
007d22e4: mov      r0, r8
007d22e8: add      r1, r1, #0x7f0000
007d22ec: bl       #0x30e70c
007d22f0: cmp      r0, #0
007d22f4: moveq    r0, #0xff
007d22f8: bne      #0x7d2648
007d22fc: ldr      r1, [sp, #0x28]
007d2300: ldrb     r0, [r1, r0]
007d2304: bl       #0x30e964
007d2308: mov      r1, #0x43000000
007d230c: add      r1, r1, #0x7f0000
007d2310: bl       #0x30ec94
007d2314: mov      r1, #0x43000000
007d2318: add      r1, r1, #0x7f0000
007d231c: bl       #0x30ed6c
007d2320: mov      r1, #0
007d2324: mov      r8, r0
007d2328: bl       #0x30e70c
007d232c: cmp      r0, #0
007d2330: movne    r0, #0
007d2334: bne      #0x7d2354
007d2338: mov      r1, #0x43000000
007d233c: mov      r0, r8
007d2340: add      r1, r1, #0x7f0000
007d2344: bl       #0x30e70c
007d2348: cmp      r0, #0
007d234c: moveq    r0, #0xff
007d2350: bne      #0x7d263c
007d2354: ldr      r2, [sp, #0x38]
007d2358: ldrb     r0, [r2, r0]
007d235c: bl       #0x30e964
007d2360: mov      r1, #0x43000000
007d2364: add      r1, r1, #0x7f0000
007d2368: bl       #0x30ec94
007d236c: cmp      fp, #0
007d2370: mov      r8, r0
007d2374: ble      #0x7d25e4
007d2378: ldr      ip, [sp, #0x24]
007d237c: ldr      r0, [sp, #8]
007d2380: mov      sl, #0
007d2384: mla      r3, ip, sb, r0
007d2388: add      r6, r6, r3
007d238c: ldr      r3, [r4, #4]
007d2390: cmp      r3, #0
007d2394: movgt    r5, #0
007d2398: ble      #0x7d23e4
007d239c: ldrb     r0, [r7, r5]
007d23a0: bl       #0x30e964
007d23a4: mov      r1, r8
007d23a8: bl       #0x30ed6c
007d23ac: bl       #0x30e4cc
007d23b0: ldrb     r3, [r6, r5]
007d23b4: cmp      r0, #0xff
007d23b8: movge    r0, #0xff
007d23bc: cmp      r0, r3
007d23c0: movge    r3, r0
007d23c4: movlt    r3, r3
007d23c8: strb     r3, [r6, r5]
007d23cc: ldr      r3, [r4, #4]
007d23d0: add      r5, r5, #1
007d23d4: cmp      r3, r5
007d23d8: bgt      #0x7d239c
007d23dc: ldr      fp, [r4]
007d23e0: ldr      sb, [sp, #0x134]
007d23e4: add      sl, sl, #1
007d23e8: cmp      fp, sl
007d23ec: ldr      r2, [r4, #8]
007d23f0: mov      r5, sb
007d23f4: ble      #0x7d25dc
007d23f8: add      r7, r7, r2
007d23fc: add      r6, r6, sb
007d2400: b        #0x7d2390
007d2404: add      r4, sp, #0xf8
007d2408: mov      r0, r4
007d240c: bl       #0x70c8fc
007d2410: ldr      r3, [r5, #0x24]
007d2414: ldr      ip, [sp, #0x10]
007d2418: mov      r2, r4
007d241c: ldr      r1, [r3, #0x54]
007d2420: ldr      r0, [ip, #0x50]
007d2424: mov      r3, #1
007d2428: add      r1, r1, #0x4c
007d242c: ldr      r0, [r0]
007d2430: bl       #0x70c980
007d2434: ldr      r2, [sp, #0xfc]
007d2438: ldr      r3, [sp, #0xf8]
007d243c: mul      r3, r2, r3
007d2440: cmp      r3, #0
007d2444: ble      #0x7d1e58
007d2448: ldr      r2, [sp, #0x104]
007d244c: ldrb     r1, [r2, r7]
007d2450: rsb      r1, r1, #0
007d2454: strb     r1, [r2, r7]
007d2458: add      r7, r7, #1
007d245c: cmp      r7, r3
007d2460: bne      #0x7d2448
007d2464: b        #0x7d1e58
007d2468: ldr      sb, [sp, #0x134]
007d246c: add      r3, r3, r8, lsl #1
007d2470: ldr      r2, [sp, #0x10]
007d2474: add      sb, sb, r7, lsl #1
007d2478: mul      sb, sb, r3
007d247c: add      sl, r2, #0x40
007d2480: lsls     r5, sb, #1
007d2484: ldr      r6, [r2, #0x44]
007d2488: beq      #0x7d2498
007d248c: ldr      r3, [r2, #0x48]
007d2490: cmp      r5, r3
007d2494: bgt      #0x7d2654
007d2498: mov      r3, #0
007d249c: b        #0x7d24ac
007d24a0: ldr      r2, [sl]
007d24a4: strb     r3, [r2, r6]
007d24a8: add      r6, r6, #1
007d24ac: cmp      r5, r6
007d24b0: bgt      #0x7d24a0
007d24b4: ldr      r3, [sp, #0x10]
007d24b8: mov      r2, r5
007d24bc: mov      r1, #0
007d24c0: str      r5, [r3, #0x44]
007d24c4: ldr      r0, [r3, #0x40]
007d24c8: mov      r5, #1
007d24cc: bl       #0x30e460
007d24d0: str      r5, [sp, #0xf4]
007d24d4: ldr      r3, [r4, #0xc]
007d24d8: mov      r6, #0
007d24dc: str      r6, [sp, #0xe4]
007d24e0: str      r6, [sp, #0xe0]
007d24e4: str      r3, [sp, #0xdc]
007d24e8: ldr      r3, [r4, #8]
007d24ec: ldr      ip, [sp, #0x10]
007d24f0: ldr      lr, [sp, #0x134]
007d24f4: str      r3, [sp, #0xf0]
007d24f8: ldr      r2, [r4, #4]
007d24fc: ldr      r3, [ip, #0x40]
007d2500: add      r1, sp, #0xc0
007d2504: str      r2, [sp, #0xe8]
007d2508: ldr      r2, [sp, #0x130]
007d250c: ldr      r4, [r4]
007d2510: sub      ip, lr, #1
007d2514: sub      r2, r2, #1
007d2518: add      r0, sp, #0xdc
007d251c: str      r3, [sp, #0xc0]
007d2520: str      ip, [sp, #0xcc]
007d2524: str      r2, [sp, #0xd0]
007d2528: str      lr, [sp, #0xd4]
007d252c: str      r5, [sp, #0xd8]
007d2530: str      r4, [sp, #0xec]
007d2534: str      r7, [sp, #0xc4]
007d2538: str      r8, [sp, #0xc8]
007d253c: bl       #0x75807c
007d2540: mov      r0, r7
007d2544: str      r5, [sp, #0x94]
007d2548: bl       #0x30e2e0
007d254c: str      r0, [sp, #0xb4]
007d2550: mov      r0, r8
007d2554: bl       #0x30e2e0
007d2558: ldr      lr, [sp, #0x10]
007d255c: ldr      r3, [sp, #0x134]
007d2560: ldr      r2, [sp, #0x130]
007d2564: ldr      ip, [lr, #0x40]
007d2568: sub      r1, r3, #1
007d256c: sub      r2, r2, #1
007d2570: add      sb, ip, sb
007d2574: str      r0, [sp, #0xb8]
007d2578: add      lr, sp, #0x94
007d257c: add      r0, sp, #0x58
007d2580: str      r5, [sp, #0x90]
007d2584: str      sb, [sp, #0x78]
007d2588: str      r6, [sp, #0x80]
007d258c: str      r5, [sp, #0xbc]
007d2590: str      r5, [sp, #0x74]
007d2594: str      r6, [sp, #0x60]
007d2598: str      r6, [sp, #0x64]
007d259c: str      r6, [sp, #0x7c]
007d25a0: str      lr, [sp, #0x58]
007d25a4: str      r3, [sp, #0x8c]
007d25a8: str      r1, [sp, #0x84]
007d25ac: str      r2, [sp, #0x88]
007d25b0: str      ip, [sp, #0x5c]
007d25b4: str      r3, [sp, #0x70]
007d25b8: str      r1, [sp, #0x68]
007d25bc: str      r2, [sp, #0x6c]
007d25c0: bl       #0x758140
007d25c4: ldr      r5, [sp, #0x134]
007d25c8: ldr      sl, [sp, #0x130]
007d25cc: ldr      r6, [sp, #0x78]
007d25d0: sub      sb, r5, #1
007d25d4: sub      sl, sl, #1
007d25d8: b        #0x7d20b8
007d25dc: ldr      r1, [sp, #0x10]
007d25e0: ldr      r6, [r1, #0x40]
007d25e4: ldr      r2, [sp, #8]
007d25e8: ldr      r3, [sp, #0x14]
007d25ec: subs     r2, r2, #1
007d25f0: sub      r3, r3, #1
007d25f4: str      r2, [sp, #8]
007d25f8: str      r3, [sp, #0x14]
007d25fc: bpl      #0x7d227c
007d2600: ldr      ip, [sp, #0x24]
007d2604: ldr      r0, [sp, #0x44]
007d2608: subs     ip, ip, #1
007d260c: sub      r0, r0, #1
007d2610: str      ip, [sp, #0x24]
007d2614: str      r0, [sp, #0x44]
007d2618: bpl      #0x7d225c
007d261c: ldr      sl, [sp, #0x130]
007d2620: sub      sb, sb, #1
007d2624: sub      sl, sl, #1
007d2628: b        #0x7d20b8
007d262c: mov      r0, r7
007d2630: add      r1, r5, r5, asr #1
007d2634: bl       #0x75722c
007d2638: b        #0x7d21cc
007d263c: mov      r0, r8
007d2640: bl       #0x30e4cc
007d2644: b        #0x7d2354
007d2648: mov      r0, r8
007d264c: bl       #0x30e4cc
007d2650: b        #0x7d22fc
007d2654: mov      r0, sl
007d2658: add      r1, r5, r5, asr #1
007d265c: bl       #0x75722c
007d2660: b        #0x7d2498
007d2664: andseq   r2, ip, ip, ror #24
007d2668: muleq    r0, r0, lr

# _ZN7gameswf19glyph_texture_cache16get_glyph_regionEtPviRNS0_11filter_infoERNS_4rectE
007d266c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d2670: sub      sp, sp, #0x34
007d2674: str      r3, [sp, #0x1c]
007d2678: ldr      r6, [sp, #0x58]
007d267c: ldr      r4, [sp, #0x1c]
007d2680: mov      sl, r1
007d2684: ldrb     r1, [r6, #2]
007d2688: ldrb     lr, [r6, #1]
007d268c: uxtb     ip, r4
007d2690: ldrb     r3, [r6]
007d2694: lsl      ip, ip, #0x10
007d2698: mov      r4, #0
007d269c: lsl      r1, r1, #8
007d26a0: orr      r1, r1, lr, lsl #16
007d26a4: str      ip, [sp, #0x14]
007d26a8: mov      r7, r0
007d26ac: str      r4, [sp, #0x10]
007d26b0: orr      r0, r4, r2
007d26b4: orr      ip, r1, r3
007d26b8: str      r2, [sp, #0x18]
007d26bc: ldrd     r2, r3, [sp, #0x10]
007d26c0: orr      r0, r0, r2
007d26c4: orr      r1, sl, r3
007d26c8: strd     r0, r1, [sp, #0x20]
007d26cc: add      sb, r7, #0x30
007d26d0: mov      r0, ip
007d26d4: asr      r1, r0, #0x1f
007d26d8: add      fp, sp, #0x20
007d26dc: strd     r0, r1, [sp, #0x28]
007d26e0: mov      r0, sb
007d26e4: mov      r1, fp
007d26e8: bl       #0x756f1c
007d26ec: ldr      r8, [pc, #0xd8]
007d26f0: cmp      r0, #0
007d26f4: mov      r5, sl
007d26f8: add      r8, pc, r8
007d26fc: blt      #0x7d2728
007d2700: ldr      r3, [r7, #0x30]
007d2704: add      r0, r3, r0, lsl #5
007d2708: ldr      r1, [r0, #0x20]
007d270c: cmp      r1, #0
007d2710: beq      #0x7d2720
007d2714: mov      r0, r7
007d2718: ldr      r2, [sp, #0x5c]
007d271c: bl       #0x756c30
007d2720: add      sp, sp, #0x34
007d2724: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d2728: mov      r0, r7
007d272c: mov      r1, sl
007d2730: ldr      r2, [sp, #0x18]
007d2734: ldr      r3, [sp, #0x1c]
007d2738: str      r4, [sp, #4]
007d273c: str      r6, [sp]
007d2740: bl       #0x7d1da4
007d2744: cmp      r0, #0
007d2748: beq      #0x7d2764
007d274c: mov      r0, sb
007d2750: mov      r1, fp
007d2754: bl       #0x756f1c
007d2758: cmp      r0, #0
007d275c: bge      #0x7d2700
007d2760: b        #0x7d2720
007d2764: ldr      r3, [pc, #0x64]
007d2768: ldr      r3, [r8, r3]
007d276c: ldr      r3, [r3]
007d2770: mov      r0, r3
007d2774: ldr      r3, [r3]
007d2778: mov      lr, pc
007d277c: ldr      pc, [r3, #0x94]
007d2780: mov      ip, #1
007d2784: mov      r0, r7
007d2788: mov      r1, sl
007d278c: ldr      r2, [sp, #0x18]
007d2790: ldr      r3, [sp, #0x1c]
007d2794: stm      sp, {r6, ip}
007d2798: bl       #0x7d1da4
007d279c: subs     r4, r0, #0
007d27a0: bne      #0x7d274c
007d27a4: mov      r0, r7
007d27a8: bl       #0x793ed8
007d27ac: mov      r1, sl
007d27b0: ldr      r2, [sp, #0x18]
007d27b4: ldr      r3, [sp, #0x1c]
007d27b8: mov      r0, r7
007d27bc: str      r6, [sp]
007d27c0: str      r4, [sp, #4]
007d27c4: bl       #0x7d1da4
007d27c8: b        #0x7d274c
007d27cc: mulseq   ip, r8, r3
007d27d0: strheq   r3, [r0], -r4

# _ZN21render_handler_glitch11set_contextEPN7gameswf14player_contextE
007d3e78: str      r1, [r0, #0x228]
007d3e7c: str      r1, [r0, #8]
007d3e80: bx       lr

# _ZN21render_handler_glitch16set_render_cacheEPN7gameswf12render_cacheE
007d4f8c: cmp      r1, #0
007d4f90: push     {r4, r5}
007d4f94: str      r1, [r0, #0xc]
007d4f98: beq      #0x7d4fbc
007d4f9c: ldr      ip, [r1, #0x14]
007d4fa0: add      r5, r1, #0x10
007d4fa4: cmp      ip, #0
007d4fa8: ble      #0x7d4fc4
007d4fac: mov      r3, #0
007d4fb0: str      r3, [r1, #0x34]
007d4fb4: str      r3, [r1, #0x14]
007d4fb8: str      r3, [r1, #0x24]
007d4fbc: pop      {r4, r5}
007d4fc0: bx       lr
007d4fc4: bge      #0x7d4fac
007d4fc8: mov      r0, #0x18
007d4fcc: mul      r0, r0, ip
007d4fd0: mov      r3, #0
007d4fd4: ldr      r4, [r5]
007d4fd8: adds     ip, ip, #1
007d4fdc: add      r2, r4, r0
007d4fe0: str      r3, [r4, r0]
007d4fe4: str      r3, [r2, #0x14]
007d4fe8: str      r3, [r2, #4]
007d4fec: str      r3, [r2, #8]
007d4ff0: str      r3, [r2, #0xc]
007d4ff4: str      r3, [r2, #0x10]
007d4ff8: add      r0, r0, #0x18
007d4ffc: bne      #0x7d4fd4
007d5000: b        #0x7d4fac

# _ZN21render_handler_glitch4drawEPN7gameswf12render_cacheE
007d93ac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d93b0: ldr      r3, [r1, #0x14]
007d93b4: mov      r5, r0
007d93b8: ldr      r0, [r0, #0x344]
007d93bc: sub      sp, sp, #0x14
007d93c0: cmp      r3, #0
007d93c4: mov      r6, r1
007d93c8: str      r0, [sp, #8]
007d93cc: ble      #0x7d94d8
007d93d0: mov      sl, #0
007d93d4: add      r2, r5, #0x378
007d93d8: add      fp, r5, #0x1f0
007d93dc: str      r2, [sp, #0xc]
007d93e0: mov      sb, sl
007d93e4: mov      r8, #0xc
007d93e8: mov      r7, r1
007d93ec: ldr      r4, [r7, #0x10]
007d93f0: mov      r0, fp
007d93f4: ldr      r1, [r4, sl]
007d93f8: add      r4, r4, sl
007d93fc: add      r1, r1, #0x10
007d9400: bl       #0x7d6a48
007d9404: ldr      r3, [sp, #8]
007d9408: cmp      r3, #0
007d940c: ldrle    r1, [r4, #0xc]
007d9410: ble      #0x7d944c
007d9414: ldr      r1, [r4, #0xc]
007d9418: cmp      r1, #0
007d941c: ble      #0x7d944c
007d9420: mov      r3, #0
007d9424: ldr      r2, [r4, #8]
007d9428: ldr      r0, [r7, #0x20]
007d942c: ldr      r1, [r5, #0x348]
007d9430: add      r2, r3, r2
007d9434: mla      r2, r8, r2, r0
007d9438: add      r3, r3, #1
007d943c: str      r1, [r2, #8]
007d9440: ldr      r1, [r4, #0xc]
007d9444: cmp      r1, r3
007d9448: bgt      #0x7d9424
007d944c: mov      r0, r5
007d9450: bl       #0x7d4340
007d9454: ldr      r3, [r5, #0x374]
007d9458: ldr      r2, [r4, #0xc]
007d945c: mov      ip, #0x18
007d9460: mla      r1, ip, r2, r3
007d9464: cmp      r3, r1
007d9468: beq      #0x7d9484
007d946c: ldr      r2, [r4, #4]
007d9470: strb     r2, [r3, #8]
007d9474: add      r3, r3, #0x18
007d9478: cmp      r1, r3
007d947c: bne      #0x7d946c
007d9480: ldr      r2, [r4, #0xc]
007d9484: ldr      r1, [r5, #0x378]
007d9488: ldr      r3, [r7, #0x40]
007d948c: ldr      r6, [r4, #0x10]
007d9490: str      r2, [r1, #8]
007d9494: ldr      ip, [r4, #0x14]
007d9498: add      r6, r3, r6, lsl #1
007d949c: ldr      r2, [r4, #0xc]
007d94a0: ldr      r1, [r5, #0x374]
007d94a4: mov      r0, r5
007d94a8: str      ip, [sp]
007d94ac: mov      r3, r6
007d94b0: mov      ip, #6
007d94b4: str      ip, [sp, #4]
007d94b8: bl       #0x7d860c
007d94bc: cmp      r0, #0
007d94c0: beq      #0x7d94e0
007d94c4: ldr      r3, [r7, #0x14]
007d94c8: add      sb, sb, #1
007d94cc: add      sl, sl, #0x18
007d94d0: cmp      sb, r3
007d94d4: blt      #0x7d93ec
007d94d8: add      sp, sp, #0x14
007d94dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d94e0: mov      r2, r6
007d94e4: ldr      r3, [r4, #0x14]
007d94e8: mov      r0, fp
007d94ec: ldr      r1, [sp, #0xc]
007d94f0: bl       #0x7d7094
007d94f4: b        #0x7d94c4

