
# luaU_undump
0085c320: push     {r4, r5, r6, r7, r8, sl, lr}
0085c324: ldr      r4, [pc, #0xe4]
0085c328: ldr      r5, [pc, #0xe4]
0085c32c: ldrsb    ip, [r3]
0085c330: add      r4, pc, r4
0085c334: ldr      lr, [r4, r5]
0085c338: mov      r7, r0
0085c33c: cmp      ip, #0x40
0085c340: cmpne    ip, #0x3d
0085c344: ldr      r0, [lr]
0085c348: sub      sp, sp, #0x34
0085c34c: addeq    r3, r3, #1
0085c350: str      r0, [sp, #0x2c]
0085c354: streq    r3, [sp, #0x10]
0085c358: beq      #0x85c368
0085c35c: cmp      ip, #0x1b
0085c360: strne    r3, [sp, #0x10]
0085c364: beq      #0x85c3fc
0085c368: add      sl, sp, #0x20
0085c36c: mov      r0, sl
0085c370: add      r8, sp, #0x14
0085c374: add      r6, sp, #4
0085c378: str      r1, [sp, #8]
0085c37c: str      r2, [sp, #0xc]
0085c380: str      r7, [sp, #4]
0085c384: bl       #0x85bc64
0085c388: mov      r2, #0xc
0085c38c: mov      r0, r6
0085c390: mov      r1, r8
0085c394: bl       #0x85bcfc
0085c398: mov      r0, sl
0085c39c: mov      r1, r8
0085c3a0: mov      r2, #0xc
0085c3a4: bl       #0x30e5e0
0085c3a8: cmp      r0, #0
0085c3ac: beq      #0x85c3c0
0085c3b0: ldr      r1, [pc, #0x60]
0085c3b4: mov      r0, r6
0085c3b8: add      r1, pc, r1
0085c3bc: bl       #0x85bcc8
0085c3c0: ldr      r1, [pc, #0x54]
0085c3c4: mov      r2, #2
0085c3c8: mov      r0, r7
0085c3cc: add      r1, pc, r1
0085c3d0: bl       #0x857f08
0085c3d4: mov      r1, r0
0085c3d8: mov      r0, r6
0085c3dc: bl       #0x85bddc
0085c3e0: ldr      r3, [r4, r5]
0085c3e4: ldr      r2, [sp, #0x2c]
0085c3e8: ldr      r3, [r3]
0085c3ec: cmp      r2, r3
0085c3f0: bne      #0x85c40c
0085c3f4: add      sp, sp, #0x34
0085c3f8: pop      {r4, r5, r6, r7, r8, sl, pc}
0085c3fc: ldr      r3, [pc, #0x1c]
0085c400: add      r3, pc, r3
0085c404: str      r3, [sp, #0x10]
0085c408: b        #0x85c368
0085c40c: bl       #0x30e310
0085c410: andseq   r8, r3, r0, ror #14
0085c414: andeq    r4, r0, ip, lsr #1
0085c418: strdeq   r4, r5, [fp], -r8
0085c41c: strdeq   r4, r5, [fp], -r4
0085c420: andeq    r4, fp, r0, lsr #15

# luaO_str2d
00854d74: push     {r4, r5, r6, r7, lr}
00854d78: sub      sp, sp, #0xc
00854d7c: add      r6, sp, #4
00854d80: mov      r4, r1
00854d84: mov      r1, r6
00854d88: mov      r5, r0
00854d8c: bl       #0x30e6ac
00854d90: bl       #0x30e6a0
00854d94: ldr      r3, [sp, #4]
00854d98: str      r0, [r4]
00854d9c: cmp      r3, r5
00854da0: moveq    r0, #0
00854da4: beq      #0x854df0
00854da8: ldrb     r7, [r3]
00854dac: cmp      r7, #0x78
00854db0: cmpne    r7, #0x58
00854db4: beq      #0x854df8
00854db8: cmp      r7, #0
00854dbc: moveq    r0, #1
00854dc0: movne    r4, r3
00854dc4: bne      #0x854dd4
00854dc8: b        #0x854df0
00854dcc: str      r4, [sp, #4]
00854dd0: ldrb     r7, [r4]
00854dd4: mov      r0, r7
00854dd8: bl       #0x30e478
00854ddc: cmp      r0, #0
00854de0: add      r4, r4, #1
00854de4: bne      #0x854dcc
00854de8: rsbs     r0, r7, #1
00854dec: movlo    r0, #0
00854df0: add      sp, sp, #0xc
00854df4: pop      {r4, r5, r6, r7, pc}
00854df8: mov      r1, r6
00854dfc: mov      r2, #0x10
00854e00: mov      r0, r5
00854e04: bl       #0x30eb38
00854e08: bl       #0x30e2e0
00854e0c: ldr      r3, [sp, #4]
00854e10: str      r0, [r4]
00854e14: ldrb     r7, [r3]
00854e18: b        #0x854db8

# luaU_header
0085bc64: push     {r4, r5, lr}
0085bc68: ldr      r1, [pc, #0x54]
0085bc6c: mov      r5, #4
0085bc70: sub      sp, sp, #0xc
0085bc74: mov      r3, #1
0085bc78: add      r1, pc, r1
0085bc7c: mov      r2, r5
0085bc80: mov      r4, r0
0085bc84: str      r3, [sp, #4]
0085bc88: bl       #0x30e868
0085bc8c: ldrb     r1, [sp, #4]
0085bc90: mov      r3, #0
0085bc94: add      r2, r4, #0xa
0085bc98: mov      r0, #0x51
0085bc9c: strb     r5, [r4, #0xa]
0085bca0: strb     r0, [r4, #4]
0085bca4: strb     r1, [r4, #6]
0085bca8: strb     r3, [r4, #5]
0085bcac: strb     r5, [r4, #7]
0085bcb0: strb     r5, [r4, #8]
0085bcb4: strb     r5, [r4, #9]
0085bcb8: strb     r3, [r2, #1]
0085bcbc: add      sp, sp, #0xc
0085bcc0: pop      {r4, r5, pc}
0085bcc4: strheq   r4, [fp], -r0

# luaV_execute
0085d09c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0085d0a0: ldr      r3, [pc, #0xfd4]
0085d0a4: sub      sp, sp, #0x54
0085d0a8: str      r1, [sp, #0x24]
0085d0ac: add      r3, pc, r3
0085d0b0: str      r3, [sp, #0x38]
0085d0b4: ldr      r3, [pc, #0xfc4]
0085d0b8: mov      r4, r0
0085d0bc: add      r3, pc, r3
0085d0c0: str      r3, [sp, #0x3c]
0085d0c4: ldr      r3, [pc, #0xfb8]
0085d0c8: add      r3, pc, r3
0085d0cc: str      r3, [sp, #0x30]
0085d0d0: ldr      r3, [pc, #0xfb0]
0085d0d4: add      r3, pc, r3
0085d0d8: str      r3, [sp, #0x34]
0085d0dc: ldr      r3, [r0, #0x14]
0085d0e0: ldr      r3, [r3, #4]
0085d0e4: ldr      r0, [pc, #0xfa0]
0085d0e8: ldr      sl, [r4, #0x18]
0085d0ec: ldr      r3, [r3]
0085d0f0: ldr      r6, [r4, #0xc]
0085d0f4: str      r0, [sp, #0x2c]
0085d0f8: str      r3, [sp, #0x10]
0085d0fc: ldr      r3, [r3, #0x10]
0085d100: ldr      r3, [r3, #8]
0085d104: str      r3, [sp, #0x14]
0085d108: ldrb     r2, [r4, #0x38]
0085d10c: mov      r5, sl
0085d110: ldr      r8, [r5], #4
0085d114: tst      r2, #0xc
0085d118: beq      #0x85d1d0
0085d11c: ldr      r3, [r4, #0x40]
0085d120: sub      r3, r3, #1
0085d124: cmp      r3, #0
0085d128: str      r3, [r4, #0x40]
0085d12c: andeq    r7, r2, #4
0085d130: beq      #0x85d13c
0085d134: ands     r7, r2, #4
0085d138: beq      #0x85d1d0
0085d13c: tst      r2, #8
0085d140: ldr      r6, [r4, #0x18]
0085d144: str      r5, [r4, #0x18]
0085d148: beq      #0x85d154
0085d14c: cmp      r3, #0
0085d150: beq      #0x85e060
0085d154: cmp      r7, #0
0085d158: beq      #0x85d1c0
0085d15c: ldr      r3, [r4, #0x14]
0085d160: ldr      r3, [r3, #4]
0085d164: ldr      r3, [r3]
0085d168: ldr      r3, [r3, #0x10]
0085d16c: ldr      r0, [r3, #0xc]
0085d170: ldr      r3, [r3, #0x14]
0085d174: rsb      r1, r0, r5
0085d178: asr      r1, r1, #2
0085d17c: cmp      r3, #0
0085d180: sub      r1, r1, #1
0085d184: moveq    r2, r3
0085d188: ldrne    r2, [r3, r1, lsl #2]
0085d18c: cmp      r1, #0
0085d190: cmpne    r5, r6
0085d194: bls      #0x85d1b4
0085d198: cmp      r3, #0
0085d19c: rsbne    r0, r0, r6
0085d1a0: asrne    r0, r0, #2
0085d1a4: subne    r0, r0, #1
0085d1a8: ldrne    r3, [r3, r0, lsl #2]
0085d1ac: cmp      r3, r2
0085d1b0: beq      #0x85d1c0
0085d1b4: mov      r0, r4
0085d1b8: mov      r1, #2
0085d1bc: bl       #0x851588
0085d1c0: ldrb     r3, [r4, #6]
0085d1c4: cmp      r3, #1
0085d1c8: beq      #0x85e090
0085d1cc: ldr      r6, [r4, #0xc]
0085d1d0: ubfx     sb, r8, #6, #8
0085d1d4: lsl      fp, sb, #3
0085d1d8: and      r3, r8, #0x3f
0085d1dc: add      r7, r6, fp
0085d1e0: cmp      r3, #0x25
0085d1e4: addls    pc, pc, r3, lsl #2
0085d1e8: b        #0x85d2d0
0085d1ec: b        #0x85dac8
0085d1f0: b        #0x85daa4
0085d1f4: b        #0x85da80
0085d1f8: b        #0x85da5c
0085d1fc: b        #0x85da30
0085d200: b        #0x85d9f0
0085d204: b        #0x85d9c0
0085d208: b        #0x85d980
0085d20c: b        #0x85d924
0085d210: b        #0x85d8d0
0085d214: b        #0x85d878
0085d218: b        #0x85d284
0085d21c: b        #0x85d810
0085d220: b        #0x85d7a8
0085d224: b        #0x85d740
0085d228: b        #0x85d6d8
0085d22c: b        #0x85df9c
0085d230: b        #0x85df34
0085d234: b        #0x85dcac
0085d238: b        #0x85dc78
0085d23c: b        #0x85e004
0085d240: b        #0x85dee0
0085d244: b        #0x85db58
0085d248: b        #0x85dae8
0085d24c: b        #0x85de84
0085d250: b        #0x85dbe0
0085d254: b        #0x85dba8
0085d258: b        #0x85db70
0085d25c: b        #0x85dddc
0085d260: b        #0x85dce8
0085d264: b        #0x85de20
0085d268: b        #0x85d660
0085d26c: b        #0x85d5c8
0085d270: b        #0x85d530
0085d274: b        #0x85d464
0085d278: b        #0x85d450
0085d27c: b        #0x85d36c
0085d280: b        #0x85d2d8
0085d284: lsr      r1, r8, #0x17
0085d288: ldr      r3, [r6, r1, lsl #3]
0085d28c: add      r1, r6, r1, lsl #3
0085d290: lsr      r8, r8, #0xe
0085d294: str      r3, [r7, #8]
0085d298: ldr      r3, [r1, #4]
0085d29c: tst      r8, #0x100
0085d2a0: lsleq    r2, r8, #0x17
0085d2a4: str      r3, [r7, #0xc]
0085d2a8: str      r5, [r4, #0x18]
0085d2ac: ldrne    r3, [sp, #0x14]
0085d2b0: lsreq    r2, r2, #0x17
0085d2b4: andne    r2, r8, #0xff
0085d2b8: addne    r2, r3, r2, lsl #3
0085d2bc: addeq    r2, r6, r2, lsl #3
0085d2c0: mov      r3, r7
0085d2c4: mov      r0, r4
0085d2c8: bl       #0x85c8dc
0085d2cc: ldr      r6, [r4, #0xc]
0085d2d0: mov      sl, r5
0085d2d4: b        #0x85d108
0085d2d8: ldr      r2, [sp, #0x10]
0085d2dc: ldr      sb, [r4, #0x14]
0085d2e0: lsr      r8, r8, #0x17
0085d2e4: ldr      r3, [r2, #0x10]
0085d2e8: ldm      sb, {r1, r2}
0085d2ec: ldrb     r3, [r3, #0x49]
0085d2f0: sub      r8, r8, #1
0085d2f4: rsb      r2, r2, r1
0085d2f8: mvn      r3, r3
0085d2fc: cmn      r8, #1
0085d300: add      sl, r3, r2, asr #3
0085d304: beq      #0x85e30c
0085d308: cmp      r8, #0
0085d30c: ble      #0x85d2d0
0085d310: mvn      r2, #7
0085d314: mul      r2, r2, sl
0085d318: mov      r1, #0
0085d31c: mov      r3, r1
0085d320: mov      fp, r5
0085d324: cmp      sl, r3
0085d328: ldrgt    ip, [sb]
0085d32c: movgt    r0, r7
0085d330: add      r3, r3, #1
0085d334: ldrgt    r5, [ip, r2]
0085d338: addgt    ip, ip, r2
0085d33c: addle    r0, r7, r1
0085d340: strgt    r5, [r0, r1]!
0085d344: ldrgt    ip, [ip, #4]
0085d348: movle    ip, #0
0085d34c: cmp      r3, r8
0085d350: str      ip, [r0, #4]
0085d354: add      r2, r2, #8
0085d358: add      r1, r1, #8
0085d35c: bne      #0x85d324
0085d360: mov      r5, fp
0085d364: mov      sl, r5
0085d368: b        #0x85d108
0085d36c: ldr      ip, [sp, #0x10]
0085d370: lsr      r8, r8, #0xe
0085d374: mov      r0, r4
0085d378: ldr      r3, [ip, #0x10]
0085d37c: ldr      r2, [ip, #0xc]
0085d380: ldr      r3, [r3, #0x10]
0085d384: ldr      r8, [r3, r8, lsl #2]
0085d388: ldrb     r1, [r8, #0x48]
0085d38c: str      r1, [sp, #0x18]
0085d390: bl       #0x852954
0085d394: ldr      r2, [sp, #0x18]
0085d398: str      r0, [sp, #0x28]
0085d39c: str      r8, [r0, #0x10]
0085d3a0: cmp      r2, #0
0085d3a4: beq      #0x85d41c
0085d3a8: ldr      sb, [sp, #0x28]
0085d3ac: add      fp, r2, #1
0085d3b0: mov      r8, #1
0085d3b4: b        #0x85d3e0
0085d3b8: ldr      ip, [sp, #0x10]
0085d3bc: lsr      r3, r3, #0x17
0085d3c0: add      r3, r3, #4
0085d3c4: add      r3, ip, r3, lsl #2
0085d3c8: ldr      r3, [r3, r2]
0085d3cc: add      r8, r8, #1
0085d3d0: cmp      r8, fp
0085d3d4: str      r3, [sb, #0x14]
0085d3d8: add      sb, sb, #4
0085d3dc: beq      #0x85d414
0085d3e0: ldr      r3, [sl, r8, lsl #2]
0085d3e4: mov      r0, r4
0085d3e8: lsr      r1, r3, #0x17
0085d3ec: and      r2, r3, #0x3f
0085d3f0: cmp      r2, #4
0085d3f4: add      r1, r6, r1, lsl #3
0085d3f8: beq      #0x85d3b8
0085d3fc: bl       #0x8527b8
0085d400: add      r8, r8, #1
0085d404: cmp      r8, fp
0085d408: str      r0, [sb, #0x14]
0085d40c: add      sb, sb, #4
0085d410: bne      #0x85d3e0
0085d414: ldr      r0, [sp, #0x18]
0085d418: add      r5, r5, r0, lsl #2
0085d41c: mov      r3, #6
0085d420: str      r3, [r7, #4]
0085d424: ldr      r1, [sp, #0x28]
0085d428: str      r1, [r7]
0085d42c: ldr      r3, [r4, #0x10]
0085d430: str      r5, [r4, #0x18]
0085d434: ldr      r2, [r3, #0x40]
0085d438: ldr      r3, [r3, #0x44]
0085d43c: cmp      r3, r2
0085d440: bhs      #0x85d8c0
0085d444: ldr      r6, [r4, #0xc]
0085d448: mov      sl, r5
0085d44c: b        #0x85d108
0085d450: mov      r1, r7
0085d454: mov      r0, r4
0085d458: bl       #0x852a18
0085d45c: mov      sl, r5
0085d460: b        #0x85d108
0085d464: lsrs     sb, r8, #0x17
0085d468: ubfx     r8, r8, #0xe, #9
0085d46c: bne      #0x85d48c
0085d470: ldr      r3, [r4, #0x14]
0085d474: ldr      sb, [r4, #8]
0085d478: ldr      r3, [r3, #8]
0085d47c: rsb      sb, r7, sb
0085d480: asr      sb, sb, #3
0085d484: str      r3, [r4, #8]
0085d488: sub      sb, sb, #1
0085d48c: ldr      r3, [r7, #4]
0085d490: cmp      r8, #0
0085d494: ldreq    r8, [r5], #4
0085d498: cmp      r3, #5
0085d49c: bne      #0x85d2d0
0085d4a0: ldr      sl, [r7]
0085d4a4: sub      r3, r8, #1
0085d4a8: mov      r8, #0x32
0085d4ac: mla      r8, r8, r3, sb
0085d4b0: ldr      r3, [sl, #0x1c]
0085d4b4: cmp      r8, r3
0085d4b8: bgt      #0x85e39c
0085d4bc: cmp      sb, #0
0085d4c0: ble      #0x85d2d0
0085d4c4: add      r7, r7, sb, lsl #3
0085d4c8: rsb      sb, sb, r8
0085d4cc: mov      r2, r8
0085d4d0: mov      r1, sl
0085d4d4: mov      r0, r4
0085d4d8: bl       #0x85ac80
0085d4dc: ldr      r3, [r7]
0085d4e0: sub      fp, r8, #1
0085d4e4: mov      r8, fp
0085d4e8: str      r3, [r0]
0085d4ec: ldr      r3, [r7, #4]
0085d4f0: str      r3, [r0, #4]
0085d4f4: ldr      r3, [r7, #4]
0085d4f8: cmp      r3, #3
0085d4fc: ble      #0x85d51c
0085d500: ldr      r3, [r7]
0085d504: ldrb     r3, [r3, #5]
0085d508: tst      r3, #3
0085d50c: beq      #0x85d51c
0085d510: ldrb     r3, [sl, #5]
0085d514: tst      r3, #4
0085d518: bne      #0x85e340
0085d51c: cmp      fp, sb
0085d520: sub      r7, r7, #8
0085d524: bne      #0x85d4cc
0085d528: mov      sl, r5
0085d52c: b        #0x85d108
0085d530: ldr      r0, [r7, #0x10]
0085d534: ldr      r1, [r7, #0x14]
0085d538: ldr      r2, [r7, #8]
0085d53c: ldr      r3, [r7, #0xc]
0085d540: str      r1, [r7, #0x2c]
0085d544: str      r2, [r7, #0x20]
0085d548: str      r3, [r7, #0x24]
0085d54c: str      r0, [r7, #0x28]
0085d550: ldr      r0, [r6, sb, lsl #3]
0085d554: ldr      r2, [r7, #4]
0085d558: add      r3, r7, #0x30
0085d55c: add      r1, r7, #0x18
0085d560: str      r0, [r7, #0x18]
0085d564: str      r2, [r1, #4]
0085d568: mov      r0, r4
0085d56c: str      r3, [r4, #8]
0085d570: ubfx     r2, r8, #0xe, #9
0085d574: str      r5, [r4, #0x18]
0085d578: bl       #0x8520e8
0085d57c: ldr      r3, [r4, #0x14]
0085d580: ldr      r6, [r4, #0xc]
0085d584: add      sb, sb, #3
0085d588: ldr      r2, [r3, #8]
0085d58c: add      r3, r6, sb, lsl #3
0085d590: str      r2, [r4, #8]
0085d594: ldr      r2, [r3, #4]
0085d598: cmp      r2, #0
0085d59c: beq      #0x85d5bc
0085d5a0: ldr      r1, [r6, sb, lsl #3]
0085d5a4: stmdb    r3, {r1, r2}
0085d5a8: ldr      r3, [r5]
0085d5ac: lsr      r3, r3, #0xe
0085d5b0: sub      r3, r3, #0x20000
0085d5b4: add      r3, r3, #1
0085d5b8: add      r5, r5, r3, lsl #2
0085d5bc: add      r5, r5, #4
0085d5c0: mov      sl, r5
0085d5c4: b        #0x85d108
0085d5c8: str      r5, [r4, #0x18]
0085d5cc: ldr      r3, [r7, #4]
0085d5d0: add      sl, r7, #0x10
0085d5d4: cmp      r3, #3
0085d5d8: beq      #0x85d5f0
0085d5dc: mov      r0, r7
0085d5e0: mov      r1, r7
0085d5e4: bl       #0x85ccd8
0085d5e8: cmp      r0, #0
0085d5ec: beq      #0x85e40c
0085d5f0: add      r0, r7, #8
0085d5f4: ldr      r3, [r0, #4]
0085d5f8: cmp      r3, #3
0085d5fc: beq      #0x85d610
0085d600: mov      r1, r0
0085d604: bl       #0x85ccd8
0085d608: cmp      r0, #0
0085d60c: beq      #0x85e420
0085d610: ldr      r3, [sl, #4]
0085d614: cmp      r3, #3
0085d618: beq      #0x85d630
0085d61c: mov      r0, sl
0085d620: mov      r1, sl
0085d624: bl       #0x85ccd8
0085d628: subs     sl, r0, #0
0085d62c: beq      #0x85e430
0085d630: ldr      r1, [sl]
0085d634: ldr      r0, [r7]
0085d638: bl       #0x30e3ac
0085d63c: lsr      r3, r8, #0xe
0085d640: sub      r3, r3, #0x20000
0085d644: add      r3, r3, #1
0085d648: mov      r2, #3
0085d64c: add      r5, r5, r3, lsl #2
0085d650: str      r2, [r7, #4]
0085d654: str      r0, [r7]
0085d658: mov      sl, r5
0085d65c: b        #0x85d108
0085d660: ldr      fp, [r7, #0x10]
0085d664: ldr      r1, [r6, sb, lsl #3]
0085d668: mov      r0, fp
0085d66c: bl       #0x30eba4
0085d670: mov      r1, #0
0085d674: mov      sl, r0
0085d678: mov      r0, fp
0085d67c: bl       #0x30e2f8
0085d680: cmp      r0, #0
0085d684: ldr      r1, [r7, #8]
0085d688: beq      #0x85e0e8
0085d68c: mov      r0, sl
0085d690: bl       #0x30e9ac
0085d694: cmp      r0, #0
0085d698: mov      r3, #0
0085d69c: bne      #0x85e100
0085d6a0: uxtb     r3, r3
0085d6a4: cmp      r3, #0
0085d6a8: beq      #0x85d2d0
0085d6ac: lsr      r2, r8, #0xe
0085d6b0: sub      r2, r2, #0x20000
0085d6b4: add      r2, r2, #1
0085d6b8: mov      r3, #3
0085d6bc: add      r5, r5, r2, lsl #2
0085d6c0: str      sl, [r7, #0x18]
0085d6c4: str      sl, [r7]
0085d6c8: str      r3, [r7, #0x1c]
0085d6cc: str      r3, [r7, #4]
0085d6d0: mov      sl, r5
0085d6d4: b        #0x85d108
0085d6d8: lsr      r2, r8, #0x17
0085d6dc: tst      r2, #0x100
0085d6e0: ldrne    r3, [sp, #0x14]
0085d6e4: bicne    r2, r2, #0x100
0085d6e8: lsr      r8, r8, #0xe
0085d6ec: addne    r2, r3, r2, lsl #3
0085d6f0: addeq    r2, r6, r2, lsl #3
0085d6f4: tst      r8, #0x100
0085d6f8: lsleq    r3, r8, #0x17
0085d6fc: ldrne    ip, [sp, #0x14]
0085d700: ldr      r1, [r2, #4]
0085d704: lsreq    r3, r3, #0x17
0085d708: andne    r3, r8, #0xff
0085d70c: addne    r3, ip, r3, lsl #3
0085d710: addeq    r3, r6, r3, lsl #3
0085d714: cmp      r1, #3
0085d718: beq      #0x85e21c
0085d71c: str      r5, [r4, #0x18]
0085d720: mov      ip, #8
0085d724: mov      r1, r7
0085d728: mov      r0, r4
0085d72c: str      ip, [sp]
0085d730: mov      sl, r5
0085d734: bl       #0x85cd38
0085d738: ldr      r6, [r4, #0xc]
0085d73c: b        #0x85d108
0085d740: lsr      r2, r8, #0x17
0085d744: tst      r2, #0x100
0085d748: ldrne    r0, [sp, #0x14]
0085d74c: bicne    r2, r2, #0x100
0085d750: lsr      r8, r8, #0xe
0085d754: addne    r2, r0, r2, lsl #3
0085d758: addeq    r2, r6, r2, lsl #3
0085d75c: tst      r8, #0x100
0085d760: ldrne    r1, [sp, #0x14]
0085d764: lsleq    r3, r8, #0x17
0085d768: andne    r3, r8, #0xff
0085d76c: addne    r3, r1, r3, lsl #3
0085d770: ldr      r1, [r2, #4]
0085d774: lsreq    r3, r3, #0x17
0085d778: addeq    r3, r6, r3, lsl #3
0085d77c: cmp      r1, #3
0085d780: beq      #0x85e26c
0085d784: str      r5, [r4, #0x18]
0085d788: mov      ip, #7
0085d78c: mov      r1, r7
0085d790: mov      r0, r4
0085d794: str      ip, [sp]
0085d798: mov      sl, r5
0085d79c: bl       #0x85cd38
0085d7a0: ldr      r6, [r4, #0xc]
0085d7a4: b        #0x85d108
0085d7a8: lsr      r2, r8, #0x17
0085d7ac: tst      r2, #0x100
0085d7b0: ldrne    r1, [sp, #0x14]
0085d7b4: bicne    r2, r2, #0x100
0085d7b8: lsr      r8, r8, #0xe
0085d7bc: addne    r2, r1, r2, lsl #3
0085d7c0: addeq    r2, r6, r2, lsl #3
0085d7c4: tst      r8, #0x100
0085d7c8: lsleq    r3, r8, #0x17
0085d7cc: ldrne    ip, [sp, #0x14]
0085d7d0: ldr      r1, [r2, #4]
0085d7d4: lsreq    r3, r3, #0x17
0085d7d8: andne    r3, r8, #0xff
0085d7dc: addne    r3, ip, r3, lsl #3
0085d7e0: addeq    r3, r6, r3, lsl #3
0085d7e4: cmp      r1, #3
0085d7e8: beq      #0x85e244
0085d7ec: str      r5, [r4, #0x18]
0085d7f0: mov      ip, #6
0085d7f4: mov      r1, r7
0085d7f8: mov      r0, r4
0085d7fc: str      ip, [sp]
0085d800: mov      sl, r5
0085d804: bl       #0x85cd38
0085d808: ldr      r6, [r4, #0xc]
0085d80c: b        #0x85d108
0085d810: lsr      r2, r8, #0x17
0085d814: tst      r2, #0x100
0085d818: ldrne    ip, [sp, #0x14]
0085d81c: bicne    r2, r2, #0x100
0085d820: lsr      r8, r8, #0xe
0085d824: addne    r2, ip, r2, lsl #3
0085d828: addeq    r2, r6, r2, lsl #3
0085d82c: tst      r8, #0x100
0085d830: lsleq    r3, r8, #0x17
0085d834: ldrne    r0, [sp, #0x14]
0085d838: ldr      r1, [r2, #4]
0085d83c: lsreq    r3, r3, #0x17
0085d840: andne    r3, r8, #0xff
0085d844: addne    r3, r0, r3, lsl #3
0085d848: addeq    r3, r6, r3, lsl #3
0085d84c: cmp      r1, #3
0085d850: beq      #0x85e124
0085d854: str      r5, [r4, #0x18]
0085d858: mov      ip, #5
0085d85c: mov      r1, r7
0085d860: mov      r0, r4
0085d864: str      ip, [sp]
0085d868: mov      sl, r5
0085d86c: bl       #0x85cd38
0085d870: ldr      r6, [r4, #0xc]
0085d874: b        #0x85d108
0085d878: lsr      r0, r8, #0x17
0085d87c: bl       #0x854838
0085d880: mov      sl, r0
0085d884: ubfx     r0, r8, #0xe, #9
0085d888: bl       #0x854838
0085d88c: mov      r1, sl
0085d890: mov      r2, r0
0085d894: mov      r0, r4
0085d898: bl       #0x85b0ec
0085d89c: mov      r3, #5
0085d8a0: str      r0, [r6, sb, lsl #3]
0085d8a4: str      r3, [r7, #4]
0085d8a8: ldr      r3, [r4, #0x10]
0085d8ac: str      r5, [r4, #0x18]
0085d8b0: ldr      r2, [r3, #0x40]
0085d8b4: ldr      r3, [r3, #0x44]
0085d8b8: cmp      r3, r2
0085d8bc: blo      #0x85d444
0085d8c0: mov      r0, r4
0085d8c4: bl       #0x853d58
0085d8c8: ldr      r6, [r4, #0xc]
0085d8cc: b        #0x85d448
0085d8d0: lsr      r2, r8, #0x17
0085d8d4: tst      r2, #0x100
0085d8d8: str      r5, [r4, #0x18]
0085d8dc: ldrne    ip, [sp, #0x14]
0085d8e0: bicne    r2, r2, #0x100
0085d8e4: lsr      r8, r8, #0xe
0085d8e8: addeq    r2, r6, r2, lsl #3
0085d8ec: addne    r2, ip, r2, lsl #3
0085d8f0: tst      r8, #0x100
0085d8f4: lsleq    r3, r8, #0x17
0085d8f8: ldrne    r0, [sp, #0x14]
0085d8fc: lsreq    r3, r3, #0x17
0085d900: andne    r3, r8, #0xff
0085d904: addne    r3, r0, r3, lsl #3
0085d908: addeq    r3, r6, r3, lsl #3
0085d90c: mov      r1, r7
0085d910: mov      r0, r4
0085d914: bl       #0x85c6ec
0085d918: mov      sl, r5
0085d91c: ldr      r6, [r4, #0xc]
0085d920: b        #0x85d108
0085d924: ldr      r3, [sp, #0x10]
0085d928: lsr      r8, r8, #0x17
0085d92c: ldr      r2, [r6, sb, lsl #3]
0085d930: add      r8, r3, r8, lsl #2
0085d934: ldr      r1, [r8, #0x14]
0085d938: ldr      r3, [r1, #8]
0085d93c: str      r2, [r3]
0085d940: ldr      r2, [r7, #4]
0085d944: str      r2, [r3, #4]
0085d948: ldr      r3, [r7, #4]
0085d94c: cmp      r3, #3
0085d950: ble      #0x85d2d0
0085d954: ldr      r2, [r7]
0085d958: ldrb     r3, [r2, #5]
0085d95c: tst      r3, #3
0085d960: beq      #0x85d2d0
0085d964: ldrb     r3, [r1, #5]
0085d968: tst      r3, #4
0085d96c: beq      #0x85d2d0
0085d970: mov      r0, r4
0085d974: bl       #0x852cb4
0085d978: mov      sl, r5
0085d97c: b        #0x85d108
0085d980: ldr      r0, [sp, #0x10]
0085d984: ldr      r1, [sp, #0x14]
0085d988: lsr      r2, r8, #0xe
0085d98c: ldr      ip, [r0, #0xc]
0085d990: add      r2, r1, r2, lsl #3
0085d994: str      r5, [r4, #0x18]
0085d998: mov      r3, r7
0085d99c: str      ip, [sp, #0x40]
0085d9a0: mov      r0, r4
0085d9a4: mov      ip, #5
0085d9a8: add      r1, sp, #0x40
0085d9ac: str      ip, [sp, #0x44]
0085d9b0: mov      sl, r5
0085d9b4: bl       #0x85c6ec
0085d9b8: ldr      r6, [r4, #0xc]
0085d9bc: b        #0x85d108
0085d9c0: lsr      r3, r8, #0xe
0085d9c4: tst      r3, #0x100
0085d9c8: lsleq    r3, r3, #0x17
0085d9cc: str      r5, [r4, #0x18]
0085d9d0: ldrne    ip, [sp, #0x14]
0085d9d4: lsr      r1, r8, #0x17
0085d9d8: lsreq    r3, r3, #0x17
0085d9dc: andne    r3, r3, #0xff
0085d9e0: add      r1, r6, r1, lsl #3
0085d9e4: addne    r2, ip, r3, lsl #3
0085d9e8: addeq    r2, r6, r3, lsl #3
0085d9ec: b        #0x85d2c0
0085d9f0: ldr      r3, [sp, #0x10]
0085d9f4: ldr      r0, [sp, #0x14]
0085d9f8: lsr      r2, r8, #0xe
0085d9fc: ldr      ip, [r3, #0xc]
0085da00: add      r2, r0, r2, lsl #3
0085da04: str      r5, [r4, #0x18]
0085da08: mov      r3, r7
0085da0c: str      ip, [sp, #0x48]
0085da10: mov      r0, r4
0085da14: mov      ip, #5
0085da18: add      r1, sp, #0x48
0085da1c: str      ip, [sp, #0x4c]
0085da20: mov      sl, r5
0085da24: bl       #0x85c8dc
0085da28: ldr      r6, [r4, #0xc]
0085da2c: b        #0x85d108
0085da30: ldr      r2, [sp, #0x10]
0085da34: lsr      r8, r8, #0x17
0085da38: mov      sl, r5
0085da3c: add      r8, r2, r8, lsl #2
0085da40: ldr      r3, [r8, #0x14]
0085da44: ldr      r3, [r3, #8]
0085da48: ldr      r2, [r3]
0085da4c: str      r2, [r6, sb, lsl #3]
0085da50: ldr      r3, [r3, #4]
0085da54: str      r3, [r7, #4]
0085da58: b        #0x85d108
0085da5c: lsr      r8, r8, #0x17
0085da60: mov      r3, #0
0085da64: add      r8, r6, r8, lsl #3
0085da68: str      r3, [r8, #4]
0085da6c: sub      r8, r8, #8
0085da70: cmp      r7, r8
0085da74: bls      #0x85da68
0085da78: mov      sl, r5
0085da7c: b        #0x85d108
0085da80: ubfx     r3, r8, #0xe, #9
0085da84: cmp      r3, #0
0085da88: lsr      r8, r8, #0x17
0085da8c: mov      r3, #1
0085da90: str      r8, [r6, sb, lsl #3]
0085da94: str      r3, [r7, #4]
0085da98: beq      #0x85d2d0
0085da9c: add      r5, r5, #4
0085daa0: b        #0x85d5c0
0085daa4: ldr      r1, [sp, #0x14]
0085daa8: lsr      r8, r8, #0xe
0085daac: mov      sl, r5
0085dab0: ldr      r3, [r1, r8, lsl #3]
0085dab4: add      r8, r1, r8, lsl #3
0085dab8: str      r3, [r6, sb, lsl #3]
0085dabc: ldr      r3, [r8, #4]
0085dac0: str      r3, [r7, #4]
0085dac4: b        #0x85d108
0085dac8: lsr      r8, r8, #0x17
0085dacc: ldr      r3, [r6, r8, lsl #3]
0085dad0: add      r8, r6, r8, lsl #3
0085dad4: mov      sl, r5
0085dad8: str      r3, [r6, sb, lsl #3]
0085dadc: ldr      r3, [r8, #4]
0085dae0: str      r3, [r7, #4]
0085dae4: b        #0x85d108
0085dae8: lsr      r1, r8, #0x17
0085daec: tst      r1, #0x100
0085daf0: ldrne    r0, [sp, #0x14]
0085daf4: bicne    r1, r1, #0x100
0085daf8: lsr      r8, r8, #0xe
0085dafc: addne    r1, r0, r1, lsl #3
0085db00: addeq    r1, r6, r1, lsl #3
0085db04: tst      r8, #0x100
0085db08: lsleq    r2, r8, #0x17
0085db0c: ldrne    r3, [sp, #0x14]
0085db10: lsreq    r2, r2, #0x17
0085db14: andne    r2, r8, #0xff
0085db18: addne    r2, r3, r2, lsl #3
0085db1c: addeq    r2, r6, r2, lsl #3
0085db20: str      r5, [r4, #0x18]
0085db24: ldr      r0, [r1, #4]
0085db28: ldr      r3, [r2, #4]
0085db2c: cmp      r0, r3
0085db30: beq      #0x85e36c
0085db34: mov      r3, #0
0085db38: cmp      r3, sb
0085db3c: bne      #0x85ded0
0085db40: ldr      r3, [r5]
0085db44: lsr      r3, r3, #0xe
0085db48: sub      r3, r3, #0x20000
0085db4c: add      r3, r3, #1
0085db50: add      r5, r5, r3, lsl #2
0085db54: b        #0x85ded0
0085db58: lsr      r3, r8, #0xe
0085db5c: sub      r3, r3, #0x20000
0085db60: add      r3, r3, #1
0085db64: add      r5, r5, r3, lsl #2
0085db68: mov      sl, r5
0085db6c: b        #0x85d108
0085db70: lsr      r3, r8, #0x17
0085db74: add      r3, r6, r3, lsl #3
0085db78: ldr      r2, [r3, #4]
0085db7c: cmp      r2, #0
0085db80: moveq    r2, #1
0085db84: bne      #0x85e0cc
0085db88: ubfx     r8, r8, #0xe, #9
0085db8c: cmp      r2, r8
0085db90: beq      #0x85d5bc
0085db94: ldr      r2, [r3]
0085db98: str      r2, [r7]
0085db9c: ldr      r3, [r3, #4]
0085dba0: str      r3, [r7, #4]
0085dba4: b        #0x85dbc4
0085dba8: ldr      r3, [r7, #4]
0085dbac: cmp      r3, #0
0085dbb0: moveq    r3, #1
0085dbb4: bne      #0x85e108
0085dbb8: ubfx     r8, r8, #0xe, #9
0085dbbc: cmp      r3, r8
0085dbc0: beq      #0x85d5bc
0085dbc4: ldr      r3, [r5]
0085dbc8: lsr      r3, r3, #0xe
0085dbcc: sub      r3, r3, #0x20000
0085dbd0: add      r3, r3, #1
0085dbd4: add      r5, r5, r3, lsl #2
0085dbd8: add      r5, r5, #4
0085dbdc: b        #0x85d5c0
0085dbe0: lsr      r7, r8, #0x17
0085dbe4: tst      r7, #0x100
0085dbe8: str      r5, [r4, #0x18]
0085dbec: ldrne    r1, [sp, #0x14]
0085dbf0: lsr      r8, r8, #0xe
0085dbf4: bicne    r7, r7, #0x100
0085dbf8: addeq    r7, r6, r7, lsl #3
0085dbfc: addne    r7, r1, r7, lsl #3
0085dc00: tst      r8, #0x100
0085dc04: lsleq    r8, r8, #0x17
0085dc08: ldrne    r2, [sp, #0x14]
0085dc0c: lsreq    r8, r8, #0x17
0085dc10: andne    r6, r8, #0xff
0085dc14: addne    r6, r2, r6, lsl #3
0085dc18: addeq    r6, r6, r8, lsl #3
0085dc1c: ldr      r3, [r7, #4]
0085dc20: ldr      r2, [r6, #4]
0085dc24: cmp      r3, r2
0085dc28: bne      #0x85e2ac
0085dc2c: cmp      r3, #3
0085dc30: beq      #0x85e384
0085dc34: cmp      r3, #4
0085dc38: beq      #0x85e3c8
0085dc3c: mov      r0, r4
0085dc40: mov      r1, r7
0085dc44: mov      r2, r6
0085dc48: mov      r3, #0xe
0085dc4c: bl       #0x85c5c4
0085dc50: cmn      r0, #1
0085dc54: beq      #0x85e3e4
0085dc58: cmp      sb, r0
0085dc5c: bne      #0x85ded0
0085dc60: ldr      r3, [r5]
0085dc64: lsr      r3, r3, #0xe
0085dc68: sub      r3, r3, #0x20000
0085dc6c: add      r3, r3, #1
0085dc70: add      r5, r5, r3, lsl #2
0085dc74: b        #0x85ded0
0085dc78: lsr      r8, r8, #0x17
0085dc7c: add      r8, r6, r8, lsl #3
0085dc80: ldr      r3, [r8, #4]
0085dc84: cmp      r3, #0
0085dc88: moveq    r2, #1
0085dc8c: beq      #0x85dc9c
0085dc90: cmp      r3, #1
0085dc94: movne    r2, #0
0085dc98: beq      #0x85e350
0085dc9c: mov      r3, #1
0085dca0: stm      r7, {r2, r3}
0085dca4: mov      sl, r5
0085dca8: b        #0x85d108
0085dcac: lsr      r8, r8, #0x17
0085dcb0: add      r2, r6, r8, lsl #3
0085dcb4: ldr      r3, [r2, #4]
0085dcb8: cmp      r3, #3
0085dcbc: beq      #0x85e294
0085dcc0: str      r5, [r4, #0x18]
0085dcc4: mov      ip, #0xb
0085dcc8: mov      r1, r7
0085dccc: mov      r0, r4
0085dcd0: mov      r3, r2
0085dcd4: str      ip, [sp]
0085dcd8: mov      sl, r5
0085dcdc: bl       #0x85cd38
0085dce0: ldr      r6, [r4, #0xc]
0085dce4: b        #0x85d108
0085dce8: lsrs     r8, r8, #0x17
0085dcec: str      r5, [r4, #0x18]
0085dcf0: addne    r8, r7, r8, lsl #3
0085dcf4: strne    r8, [r4, #8]
0085dcf8: mov      r1, r7
0085dcfc: mov      r0, r4
0085dd00: mvn      r2, #0
0085dd04: bl       #0x851ba4
0085dd08: cmp      r0, #0
0085dd0c: bne      #0x85e0c0
0085dd10: ldr      r8, [r4, #0x14]
0085dd14: ldr      r3, [r4, #0x58]
0085dd18: sub      r7, r8, #0x18
0085dd1c: ldr      r6, [r7, #4]
0085dd20: cmp      r3, #0
0085dd24: ldr      r5, [r8, #4]
0085dd28: moveq    r2, r6
0085dd2c: beq      #0x85dd40
0085dd30: mov      r0, r4
0085dd34: ldr      r1, [r8, #-0x18]
0085dd38: bl       #0x852a18
0085dd3c: ldr      r2, [r7, #4]
0085dd40: ldr      r3, [r8]
0085dd44: rsb      r3, r5, r3
0085dd48: bic      r3, r3, #7
0085dd4c: add      r3, r2, r3
0085dd50: str      r3, [r8, #-0x18]
0085dd54: ldr      r2, [r4, #8]
0085dd58: str      r3, [r4, #0xc]
0085dd5c: cmp      r5, r2
0085dd60: movhs    r3, #0
0085dd64: bhs      #0x85ddac
0085dd68: add      r1, r5, #8
0085dd6c: mov      r3, #8
0085dd70: mov      r2, #0
0085dd74: b        #0x85dd7c
0085dd78: mov      r3, r8
0085dd7c: ldr      ip, [r5]
0085dd80: add      r0, r6, r2
0085dd84: add      r8, r3, #8
0085dd88: str      ip, [r6, r2]
0085dd8c: ldr      r2, [r5, #4]
0085dd90: mov      r5, r1
0085dd94: add      r1, r1, #8
0085dd98: str      r2, [r0, #4]
0085dd9c: ldr      r0, [r4, #8]
0085dda0: mov      r2, r3
0085dda4: cmp      r0, r5
0085dda8: bhi      #0x85dd78
0085ddac: add      r3, r6, r3
0085ddb0: str      r3, [r4, #8]
0085ddb4: ldr      r2, [r7, #0x14]
0085ddb8: str      r3, [r7, #8]
0085ddbc: ldr      r3, [r4, #0x18]
0085ddc0: add      r2, r2, #1
0085ddc4: str      r2, [r7, #0x14]
0085ddc8: str      r3, [r7, #0xc]
0085ddcc: ldr      r3, [r4, #0x14]
0085ddd0: sub      r3, r3, #0x18
0085ddd4: str      r3, [r4, #0x14]
0085ddd8: b        #0x85d0e0
0085dddc: lsrs     r3, r8, #0x17
0085dde0: ubfx     r8, r8, #0xe, #9
0085dde4: addne    r3, r7, r3, lsl #3
0085dde8: sub      r8, r8, #1
0085ddec: strne    r3, [r4, #8]
0085ddf0: str      r5, [r4, #0x18]
0085ddf4: mov      r1, r7
0085ddf8: mov      r0, r4
0085ddfc: mov      r2, r8
0085de00: bl       #0x851ba4
0085de04: cmp      r0, #0
0085de08: bne      #0x85e09c
0085de0c: ldr      r3, [sp, #0x24]
0085de10: add      r3, r3, #1
0085de14: str      r3, [sp, #0x24]
0085de18: ldr      r3, [r4, #0x14]
0085de1c: b        #0x85d0e0
0085de20: lsrs     r8, r8, #0x17
0085de24: subne    r3, r8, #1
0085de28: addne    r3, r7, r3, lsl #3
0085de2c: strne    r3, [r4, #8]
0085de30: ldr      r3, [r4, #0x58]
0085de34: cmp      r3, #0
0085de38: beq      #0x85de48
0085de3c: mov      r1, r6
0085de40: mov      r0, r4
0085de44: bl       #0x852a18
0085de48: str      r5, [r4, #0x18]
0085de4c: mov      r1, r7
0085de50: mov      r0, r4
0085de54: bl       #0x8516a4
0085de58: ldr      ip, [sp, #0x24]
0085de5c: subs     ip, ip, #1
0085de60: str      ip, [sp, #0x24]
0085de64: beq      #0x85e094
0085de68: cmp      r0, #0
0085de6c: ldreq    r3, [r4, #0x14]
0085de70: beq      #0x85d0e0
0085de74: ldr      r3, [r4, #0x14]
0085de78: ldr      r2, [r3, #8]
0085de7c: str      r2, [r4, #8]
0085de80: b        #0x85d0e0
0085de84: lsr      r1, r8, #0x17
0085de88: tst      r1, #0x100
0085de8c: str      r5, [r4, #0x18]
0085de90: ldrne    ip, [sp, #0x14]
0085de94: bicne    r1, r1, #0x100
0085de98: lsr      r8, r8, #0xe
0085de9c: addne    r1, ip, r1, lsl #3
0085dea0: addeq    r1, r6, r1, lsl #3
0085dea4: tst      r8, #0x100
0085dea8: lsleq    r2, r8, #0x17
0085deac: ldrne    r0, [sp, #0x14]
0085deb0: lsreq    r2, r2, #0x17
0085deb4: andne    r2, r8, #0xff
0085deb8: addne    r2, r0, r2, lsl #3
0085debc: addeq    r2, r6, r2, lsl #3
0085dec0: mov      r0, r4
0085dec4: bl       #0x85c668
0085dec8: cmp      r0, sb
0085decc: beq      #0x85db40
0085ded0: add      r5, r5, #4
0085ded4: ldr      r6, [r4, #0xc]
0085ded8: mov      sl, r5
0085dedc: b        #0x85d108
0085dee0: ubfx     r2, r8, #0xe, #9
0085dee4: lsr      r8, r8, #0x17
0085dee8: rsb      r1, r8, r2
0085deec: str      r5, [r4, #0x18]
0085def0: add      r1, r1, #1
0085def4: mov      r0, r4
0085def8: bl       #0x85cac4
0085defc: ldr      r3, [r4, #0x10]
0085df00: ldr      r2, [r3, #0x40]
0085df04: ldr      r3, [r3, #0x44]
0085df08: cmp      r3, r2
0085df0c: bhs      #0x85e300
0085df10: ldr      r6, [r4, #0xc]
0085df14: mov      sl, r5
0085df18: ldr      r3, [r6, r8, lsl #3]
0085df1c: add      r8, r6, r8, lsl #3
0085df20: add      fp, r6, fp
0085df24: str      r3, [r6, sb, lsl #3]
0085df28: ldr      r3, [r8, #4]
0085df2c: str      r3, [fp, #4]
0085df30: b        #0x85d108
0085df34: lsr      r2, r8, #0x17
0085df38: tst      r2, #0x100
0085df3c: ldrne    r3, [sp, #0x14]
0085df40: bicne    r2, r2, #0x100
0085df44: lsr      r8, r8, #0xe
0085df48: addne    r2, r3, r2, lsl #3
0085df4c: addeq    r2, r6, r2, lsl #3
0085df50: tst      r8, #0x100
0085df54: lsleq    r3, r8, #0x17
0085df58: ldrne    ip, [sp, #0x14]
0085df5c: ldr      r1, [r2, #4]
0085df60: lsreq    r3, r3, #0x17
0085df64: andne    r3, r8, #0xff
0085df68: addne    r3, ip, r3, lsl #3
0085df6c: addeq    r3, r6, r3, lsl #3
0085df70: cmp      r1, #3
0085df74: beq      #0x85e1c8
0085df78: str      r5, [r4, #0x18]
0085df7c: mov      ip, #0xa
0085df80: mov      r1, r7
0085df84: mov      r0, r4
0085df88: str      ip, [sp]
0085df8c: mov      sl, r5
0085df90: bl       #0x85cd38
0085df94: ldr      r6, [r4, #0xc]
0085df98: b        #0x85d108
0085df9c: lsr      r2, r8, #0x17
0085dfa0: tst      r2, #0x100
0085dfa4: ldrne    r0, [sp, #0x14]
0085dfa8: bicne    r2, r2, #0x100
0085dfac: lsr      r8, r8, #0xe
0085dfb0: addne    r2, r0, r2, lsl #3
0085dfb4: addeq    r2, r6, r2, lsl #3
0085dfb8: tst      r8, #0x100
0085dfbc: ldrne    r1, [sp, #0x14]
0085dfc0: lsleq    r3, r8, #0x17
0085dfc4: andne    r3, r8, #0xff
0085dfc8: addne    r3, r1, r3, lsl #3
0085dfcc: ldr      r1, [r2, #4]
0085dfd0: lsreq    r3, r3, #0x17
0085dfd4: addeq    r3, r6, r3, lsl #3
0085dfd8: cmp      r1, #3
0085dfdc: beq      #0x85e14c
0085dfe0: str      r5, [r4, #0x18]
0085dfe4: mov      ip, #9
0085dfe8: mov      r1, r7
0085dfec: mov      r0, r4
0085dff0: str      ip, [sp]
0085dff4: mov      sl, r5
0085dff8: bl       #0x85cd38
0085dffc: ldr      r6, [r4, #0xc]
0085e000: b        #0x85d108
0085e004: lsr      r8, r8, #0x17
0085e008: add      sl, r6, r8, lsl #3
0085e00c: ldr      r3, [sl, #4]
0085e010: cmp      r3, #4
0085e014: beq      #0x85e2e0
0085e018: cmp      r3, #5
0085e01c: beq      #0x85e2c0
0085e020: str      r5, [r4, #0x18]
0085e024: mov      ip, #0xc
0085e028: mov      r3, r7
0085e02c: mov      r0, r4
0085e030: mov      r1, sl
0085e034: ldr      r2, [sp, #0x30]
0085e038: str      ip, [sp]
0085e03c: bl       #0x85c4d0
0085e040: cmp      r0, #0
0085e044: bne      #0x85d444
0085e048: mov      r1, sl
0085e04c: mov      r0, r4
0085e050: ldr      r2, [sp, #0x34]
0085e054: bl       #0x8509e4
0085e058: ldr      r6, [r4, #0xc]
0085e05c: b        #0x85d448
0085e060: ldr      r3, [r4, #0x3c]
0085e064: mov      r0, r4
0085e068: mov      r1, #3
0085e06c: str      r3, [r4, #0x40]
0085e070: mvn      r2, #0
0085e074: bl       #0x851588
0085e078: b        #0x85d154
0085e07c: strheq   r3, [fp], -ip
0085e080: andeq    r3, fp, ip, lsl #23
0085e084: strheq   r3, [fp], -r8
0085e088: andeq    r3, fp, ip, lsr fp
0085e08c: andeq    r2, fp, ip, lsl #16
0085e090: str      sl, [r4, #0x18]
0085e094: add      sp, sp, #0x54
0085e098: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0085e09c: cmp      r0, #1
0085e0a0: bne      #0x85e094
0085e0a4: cmn      r8, #1
0085e0a8: beq      #0x85d444
0085e0ac: ldr      r3, [r4, #0x14]
0085e0b0: ldr      r6, [r4, #0xc]
0085e0b4: ldr      r3, [r3, #8]
0085e0b8: str      r3, [r4, #8]
0085e0bc: b        #0x85d448
0085e0c0: cmp      r0, #1
0085e0c4: bne      #0x85e094
0085e0c8: b        #0x85d444
0085e0cc: cmp      r2, #1
0085e0d0: movne    r2, #0
0085e0d4: bne      #0x85db88
0085e0d8: ldr      r2, [r3]
0085e0dc: rsbs     r2, r2, #1
0085e0e0: movlo    r2, #0
0085e0e4: b        #0x85db88
0085e0e8: mov      r0, r1
0085e0ec: mov      r1, sl
0085e0f0: bl       #0x30e9ac
0085e0f4: cmp      r0, #0
0085e0f8: mov      r3, #0
0085e0fc: beq      #0x85d6a0
0085e100: mov      r3, #1
0085e104: b        #0x85d6a0
0085e108: cmp      r3, #1
0085e10c: movne    r3, #0
0085e110: bne      #0x85dbb8
0085e114: ldr      r3, [r7]
0085e118: rsbs     r3, r3, #1
0085e11c: movlo    r3, #0
0085e120: b        #0x85dbb8
0085e124: ldr      r8, [r3, #4]
0085e128: cmp      r8, #3
0085e12c: bne      #0x85d854
0085e130: ldr      r0, [r2]
0085e134: ldr      r1, [r3]
0085e138: bl       #0x30eba4
0085e13c: mov      sl, r5
0085e140: str      r8, [r7, #4]
0085e144: str      r0, [r7]
0085e148: b        #0x85d108
0085e14c: ldr      r8, [r3, #4]
0085e150: cmp      r8, #3
0085e154: bne      #0x85dfe0
0085e158: ldr      sl, [r2]
0085e15c: ldr      sb, [r3]
0085e160: mov      r0, sl
0085e164: mov      r1, sb
0085e168: bl       #0x30ec94
0085e16c: bl       #0x30e8a4
0085e170: bl       #0x30eac0
0085e174: strd     r0, r1, [sp, #0x18]
0085e178: mov      r0, sl
0085e17c: bl       #0x30e8a4
0085e180: mov      sl, r0
0085e184: mov      r0, sb
0085e188: mov      fp, r1
0085e18c: bl       #0x30e8a4
0085e190: mov      r2, r0
0085e194: mov      r3, r1
0085e198: ldrd     r0, r1, [sp, #0x18]
0085e19c: bl       #0x30eab4
0085e1a0: mov      r2, r0
0085e1a4: mov      r3, r1
0085e1a8: mov      r0, sl
0085e1ac: mov      r1, fp
0085e1b0: bl       #0x30e52c
0085e1b4: bl       #0x30e6a0
0085e1b8: mov      sl, r5
0085e1bc: str      r8, [r7, #4]
0085e1c0: str      r0, [r7]
0085e1c4: b        #0x85d108
0085e1c8: ldr      r8, [r3, #4]
0085e1cc: cmp      r8, #3
0085e1d0: bne      #0x85df78
0085e1d4: ldr      r0, [r2]
0085e1d8: str      r3, [sp, #0xc]
0085e1dc: bl       #0x30e8a4
0085e1e0: ldr      r3, [sp, #0xc]
0085e1e4: mov      sl, r0
0085e1e8: mov      fp, r1
0085e1ec: ldr      r0, [r3]
0085e1f0: bl       #0x30e8a4
0085e1f4: mov      r2, r0
0085e1f8: mov      r3, r1
0085e1fc: mov      r0, sl
0085e200: mov      r1, fp
0085e204: bl       #0x30e9c4
0085e208: bl       #0x30e6a0
0085e20c: mov      sl, r5
0085e210: str      r8, [r7, #4]
0085e214: str      r0, [r7]
0085e218: b        #0x85d108
0085e21c: ldr      r8, [r3, #4]
0085e220: cmp      r8, #3
0085e224: bne      #0x85d71c
0085e228: ldr      r0, [r2]
0085e22c: ldr      r1, [r3]
0085e230: bl       #0x30ec94
0085e234: mov      sl, r5
0085e238: str      r8, [r7, #4]
0085e23c: str      r0, [r7]
0085e240: b        #0x85d108
0085e244: ldr      r8, [r3, #4]
0085e248: cmp      r8, #3
0085e24c: bne      #0x85d7ec
0085e250: ldr      r0, [r2]
0085e254: ldr      r1, [r3]
0085e258: bl       #0x30e3ac
0085e25c: mov      sl, r5
0085e260: str      r8, [r7, #4]
0085e264: str      r0, [r7]
0085e268: b        #0x85d108
0085e26c: ldr      r8, [r3, #4]
0085e270: cmp      r8, #3
0085e274: bne      #0x85d784
0085e278: ldr      r0, [r2]
0085e27c: ldr      r1, [r3]
0085e280: bl       #0x30ed6c
0085e284: mov      sl, r5
0085e288: str      r8, [r7, #4]
0085e28c: str      r0, [r7]
0085e290: b        #0x85d108
0085e294: ldr      r2, [r6, r8, lsl #3]
0085e298: mov      sl, r5
0085e29c: str      r3, [r7, #4]
0085e2a0: add      r2, r2, #0x80000000
0085e2a4: str      r2, [r7]
0085e2a8: b        #0x85d108
0085e2ac: mov      r1, r7
0085e2b0: mov      r2, r6
0085e2b4: mov      r0, r4
0085e2b8: bl       #0x850788
0085e2bc: b        #0x85dc58
0085e2c0: ldr      r0, [sl]
0085e2c4: bl       #0x85a3b8
0085e2c8: bl       #0x30e964
0085e2cc: mov      r3, #3
0085e2d0: str      r3, [r7, #4]
0085e2d4: str      r0, [r7]
0085e2d8: mov      sl, r5
0085e2dc: b        #0x85d108
0085e2e0: ldr      r3, [r6, r8, lsl #3]
0085e2e4: mov      sl, r5
0085e2e8: ldr      r0, [r3, #0xc]
0085e2ec: bl       #0x30e2e0
0085e2f0: mov      r3, #3
0085e2f4: str      r3, [r7, #4]
0085e2f8: str      r0, [r7]
0085e2fc: b        #0x85d108
0085e300: mov      r0, r4
0085e304: bl       #0x853d58
0085e308: b        #0x85df10
0085e30c: ldr      r1, [r4, #0x1c]
0085e310: ldr      r2, [r4, #8]
0085e314: lsl      r3, sl, #3
0085e318: str      r5, [r4, #0x18]
0085e31c: rsb      r2, r2, r1
0085e320: cmp      r2, r3
0085e324: ble      #0x85e3b0
0085e328: ldr      r6, [r4, #0xc]
0085e32c: mov      r8, sl
0085e330: add      r7, r6, fp
0085e334: add      r3, r7, r3
0085e338: str      r3, [r4, #8]
0085e33c: b        #0x85d308
0085e340: mov      r0, r4
0085e344: mov      r1, sl
0085e348: bl       #0x852ce8
0085e34c: b        #0x85d51c
0085e350: ldr      r2, [r8]
0085e354: mov      r3, #1
0085e358: str      r3, [r7, #4]
0085e35c: rsbs     r2, r2, #1
0085e360: movlo    r2, #0
0085e364: str      r2, [r7]
0085e368: b        #0x85dca4
0085e36c: mov      r0, r4
0085e370: bl       #0x85cfb0
0085e374: cmp      r0, #0
0085e378: movne    r3, #1
0085e37c: bne      #0x85db38
0085e380: b        #0x85db34
0085e384: ldr      r0, [r7]
0085e388: ldr      r1, [r6]
0085e38c: bl       #0x30e9ac
0085e390: subs     r0, r0, #0
0085e394: movne    r0, #1
0085e398: b        #0x85dc58
0085e39c: mov      r0, r4
0085e3a0: mov      r1, sl
0085e3a4: mov      r2, r8
0085e3a8: bl       #0x85ae9c
0085e3ac: b        #0x85d4bc
0085e3b0: mov      r0, r4
0085e3b4: mov      r1, sl
0085e3b8: str      r3, [sp, #0xc]
0085e3bc: bl       #0x8513fc
0085e3c0: ldr      r3, [sp, #0xc]
0085e3c4: b        #0x85e328
0085e3c8: ldr      r0, [r7]
0085e3cc: ldr      r1, [r6]
0085e3d0: bl       #0x85c548
0085e3d4: cmp      r0, #0
0085e3d8: movgt    r0, #0
0085e3dc: movle    r0, #1
0085e3e0: b        #0x85dc58
0085e3e4: mov      r0, r4
0085e3e8: mov      r1, r6
0085e3ec: mov      r2, r7
0085e3f0: mov      r3, #0xd
0085e3f4: bl       #0x85c5c4
0085e3f8: cmn      r0, #1
0085e3fc: beq      #0x85e2ac
0085e400: rsbs     r0, r0, #1
0085e404: movlo    r0, #0
0085e408: b        #0x85dc58
0085e40c: ldr      r2, [sp, #0x2c]
0085e410: mov      r0, r4
0085e414: add      r1, pc, r2
0085e418: bl       #0x850650
0085e41c: b        #0x85d630
0085e420: mov      r0, r4
0085e424: ldr      r1, [sp, #0x3c]
0085e428: bl       #0x850650
0085e42c: b        #0x85d630
0085e430: mov      r0, r4
0085e434: ldr      r1, [sp, #0x38]
0085e438: bl       #0x850650
0085e43c: b        #0x85d630
