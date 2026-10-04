
# _ZN7gameswf9as_object10set_memberERKNS_10tu_stringiERKNS_8as_valueE
0076c1e0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0076c1e4: add      sl, r0, #0xc
0076c1e8: sub      sp, sp, #0xec
0076c1ec: mov      r4, r0
0076c1f0: mov      r0, sl
0076c1f4: mov      fp, r2
0076c1f8: mov      r5, r1
0076c1fc: bl       #0x768eec
0076c200: subs     r6, r0, #0
0076c204: movlt    r6, #0
0076c208: strlt    r6, [sp, #0x10]
0076c20c: blt      #0x76c234
0076c210: cmp      sl, #0
0076c214: beq      #0x76c230
0076c218: ldr      r3, [r4, #0xc]
0076c21c: cmp      r3, #0
0076c220: beq      #0x76c230
0076c224: ldr      r2, [r3, #4]
0076c228: cmp      r6, r2
0076c22c: ble      #0x76c434
0076c230: str      sl, [sp, #0x10]
0076c234: mov      sb, #0
0076c238: ldr      r3, [r4, #0x1c]
0076c23c: cmp      r3, #0
0076c240: beq      #0x76c3e8
0076c244: cmp      sb, #0
0076c248: bne      #0x76c4ac
0076c24c: add      r3, sp, #0xcc
0076c250: strb     sb, [sp, #0xcc]
0076c254: strb     sb, [sp, #0xcd]
0076c258: str      r3, [sp, #0xc]
0076c25c: add      r8, sp, #0xc0
0076c260: mov      r7, #0
0076c264: add      r2, sp, #0xa4
0076c268: mov      r0, r8
0076c26c: mov      r1, fp
0076c270: str      r2, [sp, #8]
0076c274: strb     r7, [sp, #0xc0]
0076c278: strb     r7, [sp, #0xc1]
0076c27c: bl       #0x79773c
0076c280: ldr      r0, [r4, #0x1c]
0076c284: mov      r1, r5
0076c288: ldr      r2, [sp, #8]
0076c28c: strb     r7, [sp, #0xa9]
0076c290: str      r7, [sp, #0xa4]
0076c294: strb     r7, [sp, #0xa8]
0076c298: bl       #0x76bed4
0076c29c: ldr      r3, [sp, #0xa4]
0076c2a0: cmp      r3, r7
0076c2a4: beq      #0x76c3cc
0076c2a8: ldr      r1, [r4, #0x30]
0076c2ac: cmp      r1, r7
0076c2b0: beq      #0x76c2c4
0076c2b4: ldr      r0, [r4, #0x2c]
0076c2b8: ldrb     r3, [r0, #4]
0076c2bc: cmp      r3, r7
0076c2c0: beq      #0x76c4f0
0076c2c4: add      r7, sp, #0x1c
0076c2c8: mov      r0, r7
0076c2cc: bl       #0x75eb4c
0076c2d0: ldr      r3, [sp, #8]
0076c2d4: mov      r0, r7
0076c2d8: add      r1, r3, #4
0076c2dc: bl       #0x769098
0076c2e0: mov      r0, r7
0076c2e4: mov      r1, r8
0076c2e8: bl       #0x769098
0076c2ec: mov      r0, r7
0076c2f0: ldr      r1, [sp, #0xc]
0076c2f4: bl       #0x769098
0076c2f8: ldrsb    r3, [r5]
0076c2fc: add      r1, sp, #0xe8
0076c300: mov      r0, r7
0076c304: cmn      r3, #1
0076c308: ldreq    r3, [r5, #0xc]
0076c30c: addne    r3, r5, #1
0076c310: str      r3, [r1, #-4]!
0076c314: bl       #0x7691f0
0076c318: mov      r0, r8
0076c31c: bl       #0x797124
0076c320: ldr      r3, [sp, #0xa4]
0076c324: mov      r2, #0
0076c328: strb     r2, [sp, #0xc1]
0076c32c: ldr      r1, [r3]
0076c330: mov      r0, r4
0076c334: ldr      r1, [r1, #0x64]
0076c338: strb     r2, [sp, #0xb4]
0076c33c: mov      r2, #5
0076c340: str      r1, [sp, #0x14]
0076c344: strb     r2, [sp, #0xb5]
0076c348: str      r3, [sp, #4]
0076c34c: str      r4, [sp, #0xb8]
0076c350: bl       #0x759c64
0076c354: ldrsb    r2, [r5]
0076c358: ldrsb    r0, [sp, #0xb5]
0076c35c: ldr      r1, [sp, #0x20]
0076c360: cmn      r2, #1
0076c364: ldreq    ip, [r5, #0xc]
0076c368: sub      r1, r1, #1
0076c36c: addne    ip, r5, #1
0076c370: cmp      r0, #5
0076c374: ldr      r3, [sp, #4]
0076c378: str      r1, [sp, #0x9c]
0076c37c: ldreq    r1, [sp, #0xb8]
0076c380: add      r2, sp, #0xb4
0076c384: movne    r1, #0
0076c388: mov      r0, #4
0076c38c: str      r0, [sp, #0x98]
0076c390: str      ip, [sp, #0xa0]
0076c394: mov      r0, r3
0076c398: str      r2, [sp, #0x90]
0076c39c: str      r1, [sp, #0x8c]
0076c3a0: str      r2, [sp, #4]
0076c3a4: add      r1, sp, #0x88
0076c3a8: ldr      r3, [sp, #0x14]
0076c3ac: str      r8, [sp, #0x88]
0076c3b0: str      r7, [sp, #0x94]
0076c3b4: blx      r3
0076c3b8: ldr      r2, [sp, #4]
0076c3bc: mov      r0, r2
0076c3c0: bl       #0x797124
0076c3c4: mov      r0, r7
0076c3c8: bl       #0x75e03c
0076c3cc: ldr      r2, [sp, #8]
0076c3d0: add      r0, r2, #4
0076c3d4: bl       #0x797124
0076c3d8: mov      r0, r8
0076c3dc: bl       #0x797124
0076c3e0: ldr      r0, [sp, #0xc]
0076c3e4: bl       #0x797124
0076c3e8: mov      r0, r4
0076c3ec: ldr      r3, [r4]
0076c3f0: mov      r1, r5
0076c3f4: mov      r2, fp
0076c3f8: mov      lr, pc
0076c3fc: ldr      pc, [r3, #0x60]
0076c400: cmp      sb, #0
0076c404: beq      #0x76c498
0076c408: ldr      r2, [sp, #0x10]
0076c40c: add      r6, r6, r6, lsl #2
0076c410: add      r0, r6, #1
0076c414: ldr      r3, [r2]
0076c418: add      r0, r3, r0, lsl #3
0076c41c: ldrb     r3, [r0, #0x1c]
0076c420: tst      r3, #4
0076c424: beq      #0x76c4e0
0076c428: mov      r0, #1
0076c42c: add      sp, sp, #0xec
0076c430: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0076c434: add      r2, r6, r6, lsl #2
0076c438: add      r3, r3, r2, lsl #3
0076c43c: ldrsb    r3, [r3, #0x25]
0076c440: cmp      r3, #6
0076c444: strne    sl, [sp, #0x10]
0076c448: movne    sb, #1
0076c44c: bne      #0x76c238
0076c450: mov      r3, #0
0076c454: strb     r3, [sp, #0xd9]
0076c458: strb     r3, [sp, #0xd8]
0076c45c: add      r6, sp, #0xd8
0076c460: mov      r0, r4
0076c464: mov      r1, r5
0076c468: ldr      r3, [r4]
0076c46c: mov      r2, r6
0076c470: mov      lr, pc
0076c474: ldr      pc, [r3, #0x20]
0076c478: cmp      r0, #0
0076c47c: beq      #0x76c48c
0076c480: mov      r1, fp
0076c484: mov      r0, r6
0076c488: bl       #0x797950
0076c48c: mov      r0, r6
0076c490: bl       #0x797124
0076c494: b        #0x76c428
0076c498: mov      r0, sl
0076c49c: mov      r1, r5
0076c4a0: mov      r2, fp
0076c4a4: bl       #0x76a4bc
0076c4a8: b        #0x76c428
0076c4ac: ldr      r2, [sp, #0x10]
0076c4b0: add      r1, r6, r6, lsl #2
0076c4b4: ldr      r3, [r2]
0076c4b8: add      r2, sp, #0xcc
0076c4bc: mov      r0, r2
0076c4c0: add      r1, r3, r1, lsl #3
0076c4c4: add      r1, r1, #0x24
0076c4c8: mov      r3, #0
0076c4cc: str      r2, [sp, #0xc]
0076c4d0: strb     r3, [sp, #0xcd]
0076c4d4: strb     r3, [sp, #0xcc]
0076c4d8: bl       #0x79773c
0076c4dc: b        #0x76c25c
0076c4e0: add      r0, r0, #0x1c
0076c4e4: mov      r1, fp
0076c4e8: bl       #0x79773c
0076c4ec: b        #0x76c428
0076c4f0: ldr      r1, [r0]
0076c4f4: sub      r1, r1, #1
0076c4f8: cmp      r1, r7
0076c4fc: str      r1, [r0]
0076c500: bne      #0x76c508
0076c504: bl       #0x752b38
0076c508: mov      r1, #0
0076c50c: str      r1, [r4, #0x2c]
0076c510: str      r1, [r4, #0x30]
0076c514: b        #0x76c2c4
