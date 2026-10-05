
# _ZN8MenuBase4ShowEv
00425450: push     {r4, r5, r6, r7, r8, sl, lr}
00425454: ldr      r4, [pc, #0x288]
00425458: ldr      r6, [pc, #0x288]
0042545c: sub      sp, sp, #0x5c
00425460: add      r4, pc, r4
00425464: ldr      r2, [r4, r6]
00425468: ldr      r3, [r0]
0042546c: mov      r5, r0
00425470: ldr      r2, [r2]
00425474: str      r2, [sp, #0x54]
00425478: mov      lr, pc
0042547c: ldr      pc, [r3, #0x3c]
00425480: cmp      r0, #0
00425484: bne      #0x4254a4
00425488: ldr      r3, [r4, r6]
0042548c: ldr      r2, [sp, #0x54]
00425490: ldr      r3, [r3]
00425494: cmp      r2, r3
00425498: bne      #0x4256e0
0042549c: add      sp, sp, #0x5c
004254a0: pop      {r4, r5, r6, r7, r8, sl, pc}
004254a4: ldr      r3, [pc, #0x240]
004254a8: add      r7, sp, #0x3c
004254ac: ldr      r8, [r4, r3]
004254b0: mov      r0, r8
004254b4: bl       #0x337888
004254b8: ldr      r1, [pc, #0x230]
004254bc: add      r2, sp, #8
004254c0: mov      r0, r7
004254c4: add      r1, pc, r1
004254c8: bl       #0x3140ec
004254cc: mov      r1, r7
004254d0: mov      r0, r8
004254d4: bl       #0x337a88
004254d8: mov      r0, r7
004254dc: bl       #0x318254
004254e0: ldr      r3, [pc, #0x20c]
004254e4: mov      r2, #1
004254e8: ldr      r3, [r4, r3]
004254ec: strb     r2, [r3]
004254f0: ldrb     r3, [r5, #0x75]
004254f4: cmp      r3, #0
004254f8: beq      #0x42562c
004254fc: mov      r1, #1
00425500: mov      r0, r5
00425504: bl       #0x4223bc
00425508: ldr      r1, [r5, #0x4c]
0042550c: ldr      r8, [r5, #4]
00425510: cmp      r1, #0
00425514: beq      #0x425528
00425518: ldr      r0, [r5, #0x48]
0042551c: ldrb     r3, [r0, #4]
00425520: cmp      r3, #0
00425524: beq      #0x4256b8
00425528: ldr      r2, [pc, #0x1c8]
0042552c: mov      r7, #0
00425530: mov      r3, r7
00425534: add      r2, pc, r2
00425538: mov      r0, r8
0042553c: str      r7, [sp]
00425540: bl       #0x7abe0c
00425544: ldr      r1, [pc, #0x1b0]
00425548: add      r8, r5, #8
0042554c: str      r7, [r5, #0x78]
00425550: add      r1, pc, r1
00425554: mov      r0, r8
00425558: bl       #0x30e31c
0042555c: ldr      r7, [pc, #0x19c]
00425560: ldr      r1, [pc, #0x19c]
00425564: rsbs     r2, r0, #1
00425568: movlo    r2, #0
0042556c: ldr      r3, [r4, r7]
00425570: add      r1, pc, r1
00425574: mov      r0, r8
00425578: strb     r2, [r3, #0xec]
0042557c: bl       #0x30e31c
00425580: cmp      r0, #0
00425584: beq      #0x4255a0
00425588: ldr      r1, [pc, #0x178]
0042558c: mov      r0, r8
00425590: add      r1, pc, r1
00425594: bl       #0x30e31c
00425598: cmp      r0, #0
0042559c: bne      #0x4255ac
004255a0: ldr      r3, [pc, #0x164]
004255a4: ldr      r0, [r4, r3]
004255a8: bl       #0x384ef0
004255ac: ldr      r1, [pc, #0x15c]
004255b0: mov      r0, r8
004255b4: add      r1, pc, r1
004255b8: bl       #0x30e31c
004255bc: cmp      r0, #0
004255c0: bne      #0x4255f8
004255c4: ldr      r3, [pc, #0x148]
004255c8: mov      r2, #1
004255cc: ldr      r3, [r4, r3]
004255d0: strb     r2, [r3]
004255d4: ldr      r1, [pc, #0x13c]
004255d8: mov      r0, r8
004255dc: add      r1, pc, r1
004255e0: bl       #0x30e31c
004255e4: cmp      r0, #0
004255e8: beq      #0x425640
004255ec: mov      r0, r5
004255f0: bl       #0x42331c
004255f4: b        #0x425488
004255f8: ldr      r1, [pc, #0x11c]
004255fc: mov      r0, r8
00425600: add      r1, pc, r1
00425604: bl       #0x30e31c
00425608: cmp      r0, #0
0042560c: beq      #0x4255c4
00425610: ldr      r1, [pc, #0x108]
00425614: mov      r0, r8
00425618: add      r1, pc, r1
0042561c: bl       #0x30e31c
00425620: cmp      r0, #0
00425624: bne      #0x4255d4
00425628: b        #0x4255c4
0042562c: ldr      r3, [r5]
00425630: mov      r0, r5
00425634: mov      lr, pc
00425638: ldr      pc, [r3, #0x40]
0042563c: b        #0x4254fc
00425640: add      r8, sp, #0xc
00425644: mov      r0, r8
00425648: bl       #0x41aeec
0042564c: mov      r0, r5
00425650: ldr      sl, [r5, #4]
00425654: bl       #0x42204c
00425658: ldr      r1, [pc, #0xc4]
0042565c: mov      r3, r0
00425660: mov      r2, sl
00425664: add      r1, pc, r1
00425668: mov      r0, r8
0042566c: bl       #0x427ca0
00425670: mov      r0, r8
00425674: bl       #0x427d50
00425678: mov      sl, r0
0042567c: ldr      r0, [r4, r7]
00425680: bl       #0x320f5c
00425684: strb     r0, [sl, #0x9b]
00425688: ldr      r0, [sp, #0x34]
0042568c: cmp      r0, #0
00425690: beq      #0x4256ac
00425694: ldr      r1, [r0]
00425698: sub      r1, r1, #1
0042569c: cmp      r1, #0
004256a0: str      r1, [r0]
004256a4: bne      #0x4256ac
004256a8: bl       #0x752b38
004256ac: add      r0, r8, #8
004256b0: bl       #0x318254
004256b4: b        #0x4255ec
004256b8: ldr      r1, [r0]
004256bc: sub      r1, r1, #1
004256c0: cmp      r1, #0
004256c4: str      r1, [r0]
004256c8: bne      #0x4256d0
004256cc: bl       #0x752b38
004256d0: mov      r1, #0
004256d4: str      r1, [r5, #0x48]
004256d8: str      r1, [r5, #0x4c]
004256dc: b        #0x425528
004256e0: bl       #0x30e310
004256e4: subseq   pc, r6, r0, lsr r6
004256e8: andeq    r4, r0, ip, lsr #1
004256ec: andeq    r0, r0, r4, lsl #17
004256f0: subeq    r3, sl, ip, ror fp
004256f4: andeq    r3, r0, r4, ror #14
004256f8: strdeq   r3, r4, [sl], #-0xc4
004256fc: umaaleq  r3, sl, r8, ip
00425700: strdeq   r3, r4, [r0], -r4
00425704: ldrdeq   ip, sp, [sb], #-0x90
00425708: subeq    ip, sb, r0, lsr #19
0042570c: andeq    r1, r0, r8, ror #7
00425710: subeq    sb, sb, r4, ror #14
00425714: andeq    r2, r0, r0, lsr #31
00425718: subeq    r3, sl, r4, asr ip
0042571c: ldrdeq   r3, r4, [sl], #-0xb8
00425720: strheq   r3, [sl], #-0xb0
00425724: ldrdeq   r3, r4, [sl], #-0xbc

# _ZN8MenuBase10SetVisibleEb
004223bc: push     {r4, r5, r6, lr}
004223c0: ldr      r3, [r0]
004223c4: mov      r4, r0
004223c8: mov      r5, r1
004223cc: mov      lr, pc
004223d0: ldr      pc, [r3, #0x3c]
004223d4: cmp      r0, #0
004223d8: beq      #0x422400
004223dc: ldr      r3, [r4, #0x4c]
004223e0: cmp      r3, #0
004223e4: beq      #0x4223f8
004223e8: ldr      r0, [r4, #0x48]
004223ec: ldrb     r2, [r0, #4]
004223f0: cmp      r2, #0
004223f4: beq      #0x422404
004223f8: strb     r5, [r3, #0x9b]
004223fc: strb     r5, [r4, #0x74]
00422400: pop      {r4, r5, r6, pc}
00422404: ldr      r1, [r0]
00422408: sub      r1, r1, #1
0042240c: cmp      r1, #0
00422410: str      r1, [r0]
00422414: bne      #0x42241c
00422418: bl       #0x752b38
0042241c: mov      r3, #0
00422420: str      r3, [r4, #0x48]
00422424: str      r3, [r4, #0x4c]
00422428: b        #0x4223f8

# _ZN8MenuBase4HideEv
00424af4: push     {r4, r5, r6, r7, r8, sl, lr}
00424af8: ldr      r4, [pc, #0x228]
00424afc: ldr      r5, [pc, #0x228]
00424b00: sub      sp, sp, #0x2c
00424b04: add      r4, pc, r4
00424b08: ldr      r2, [r4, r5]
00424b0c: ldr      r3, [r0]
00424b10: mov      r6, r0
00424b14: ldr      r2, [r2]
00424b18: str      r2, [sp, #0x24]
00424b1c: mov      lr, pc
00424b20: ldr      pc, [r3, #0x3c]
00424b24: cmp      r0, #0
00424b28: bne      #0x424b48
00424b2c: ldr      r3, [r4, r5]
00424b30: ldr      r2, [sp, #0x24]
00424b34: ldr      r3, [r3]
00424b38: cmp      r2, r3
00424b3c: bne      #0x424d24
00424b40: add      sp, sp, #0x2c
00424b44: pop      {r4, r5, r6, r7, r8, sl, pc}
00424b48: ldr      r3, [pc, #0x1e0]
00424b4c: add      r7, sp, #0xc
00424b50: ldr      r8, [r4, r3]
00424b54: mov      r0, r8
00424b58: bl       #0x337888
00424b5c: ldr      r1, [pc, #0x1d0]
00424b60: add      r2, sp, #8
00424b64: mov      r0, r7
00424b68: add      r1, pc, r1
00424b6c: bl       #0x3140ec
00424b70: mov      r1, r7
00424b74: mov      r0, r8
00424b78: bl       #0x337a88
00424b7c: mov      r0, r7
00424b80: bl       #0x318254
00424b84: ldr      r0, [r6, #0x5c]
00424b88: cmp      r0, #0
00424b8c: beq      #0x424b94
00424b90: bl       #0x412aa8
00424b94: ldr      r1, [pc, #0x19c]
00424b98: add      r7, r6, #8
00424b9c: mov      r0, r7
00424ba0: add      r1, pc, r1
00424ba4: bl       #0x30e31c
00424ba8: cmp      r0, #0
00424bac: beq      #0x424c78
00424bb0: ldr      r1, [pc, #0x184]
00424bb4: mov      r0, r7
00424bb8: add      r1, pc, r1
00424bbc: bl       #0x30e31c
00424bc0: cmp      r0, #0
00424bc4: beq      #0x424c78
00424bc8: ldr      r1, [pc, #0x170]
00424bcc: mov      r0, r7
00424bd0: add      r1, pc, r1
00424bd4: bl       #0x30e31c
00424bd8: subs     sl, r0, #0
00424bdc: beq      #0x424ccc
00424be0: ldr      r1, [pc, #0x15c]
00424be4: mov      r0, r7
00424be8: add      r1, pc, r1
00424bec: bl       #0x30e31c
00424bf0: cmp      r0, #0
00424bf4: bne      #0x424c98
00424bf8: ldr      r3, [pc, #0x148]
00424bfc: mov      r2, #0
00424c00: ldr      r3, [r4, r3]
00424c04: strb     r2, [r3]
00424c08: ldr      r1, [pc, #0x13c]
00424c0c: mov      r0, r7
00424c10: add      r1, pc, r1
00424c14: bl       #0x30e31c
00424c18: cmp      r0, #0
00424c1c: beq      #0x424c88
00424c20: mov      r1, #0
00424c24: mov      r0, r6
00424c28: bl       #0x4223bc
00424c2c: bl       #0x42ca8c
00424c30: mov      r1, r6
00424c34: bl       #0x42e110
00424c38: ldr      r1, [r6, #0x4c]
00424c3c: ldr      r7, [r6, #4]
00424c40: cmp      r1, #0
00424c44: beq      #0x424c58
00424c48: ldr      r0, [r6, #0x48]
00424c4c: ldrb     r3, [r0, #4]
00424c50: cmp      r3, #0
00424c54: beq      #0x424cfc
00424c58: ldr      r2, [pc, #0xf0]
00424c5c: mov      ip, #0
00424c60: mov      r0, r7
00424c64: add      r2, pc, r2
00424c68: mov      r3, ip
00424c6c: str      ip, [sp]
00424c70: bl       #0x7abe0c
00424c74: b        #0x424b2c
00424c78: bl       #0x42ca8c
00424c7c: mov      r3, #0
00424c80: str      r3, [r0, #0x60]
00424c84: b        #0x424bc8
00424c88: ldr      r3, [pc, #0xc4]
00424c8c: ldr      r3, [r4, r3]
00424c90: strb     r0, [r3, #0xec]
00424c94: b        #0x424c20
00424c98: ldr      r1, [pc, #0xb8]
00424c9c: mov      r0, r7
00424ca0: add      r1, pc, r1
00424ca4: bl       #0x30e31c
00424ca8: cmp      r0, #0
00424cac: beq      #0x424bf8
00424cb0: ldr      r1, [pc, #0xa4]
00424cb4: mov      r0, r7
00424cb8: add      r1, pc, r1
00424cbc: bl       #0x30e31c
00424cc0: cmp      r0, #0
00424cc4: bne      #0x424c08
00424cc8: b        #0x424bf8
00424ccc: ldr      r3, [pc, #0x80]
00424cd0: ldr      r8, [r4, r3]
00424cd4: ldr      r0, [r8, #0x4c]
00424cd8: bl       #0x46d514
00424cdc: cmp      r0, #7
00424ce0: bls      #0x424be0
00424ce4: mov      r1, sl
00424ce8: ldr      r0, [r8, #0x4c]
00424cec: bl       #0x46d104
00424cf0: ldr      r0, [r8, #0x4c]
00424cf4: bl       #0x46cb34
00424cf8: b        #0x424be0
00424cfc: ldr      r1, [r0]
00424d00: sub      r1, r1, #1
00424d04: cmp      r1, #0
00424d08: str      r1, [r0]
00424d0c: bne      #0x424d14
00424d10: bl       #0x752b38
00424d14: mov      r1, #0
00424d18: str      r1, [r6, #0x4c]
00424d1c: str      r1, [r6, #0x48]
00424d20: b        #0x424c58
00424d24: bl       #0x30e310
00424d28: subseq   pc, r6, ip, lsl #31
00424d2c: andeq    r4, r0, ip, lsr #1
00424d30: andeq    r0, r0, r4, lsl #17
00424d34: ldrdeq   r4, r5, [sl], #-0x48
00424d38: subeq    sl, sb, r0, ror #2
00424d3c: subeq    r4, sl, r0, lsl r6
00424d40: subeq    sp, sb, r0, ror r3
00424d44: subeq    sl, sb, r0, lsr r1
00424d48: andeq    r2, r0, r0, lsr #31
00424d4c: ldrdeq   r4, r5, [sl], #-0x58
00424d50: subeq    r4, sl, r4, lsr #11
00424d54: strdeq   r3, r4, [r0], -r4
00424d58: subeq    r4, sl, r8, lsr r5
00424d5c: subeq    r4, sl, r0, lsl r5

# _ZNK8MenuBase21GetCurrentMenuContextEv
0042204c: push     {r4, lr}
00422050: mov      r4, r0
00422054: ldr      r0, [r0, #0x4c]
00422058: cmp      r0, #0
0042205c: beq      #0x422070
00422060: ldr      r3, [r4, #0x48]
00422064: ldrb     r2, [r3, #4]
00422068: cmp      r2, #0
0042206c: beq      #0x422074
00422070: pop      {r4, pc}
00422074: ldr      r1, [r3]
00422078: sub      r1, r1, #1
0042207c: cmp      r1, #0
00422080: str      r1, [r3]
00422084: bne      #0x422090
00422088: mov      r0, r3
0042208c: bl       #0x752b38
00422090: mov      r0, #0
00422094: str      r0, [r4, #0x4c]
00422098: str      r0, [r4, #0x48]
0042209c: pop      {r4, pc}

# _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi
007abe0c: push     {r4, r5, r6, r7, r8, sl, lr}
007abe10: ldr      r4, [pc, #0x114]
007abe14: ldr      r6, [pc, #0x114]
007abe18: subs     r5, r1, #0
007abe1c: add      r4, pc, r4
007abe20: ldr      r1, [r4, r6]
007abe24: mov      r7, r3
007abe28: sub      sp, sp, #0x24
007abe2c: ldr      r3, [r1]
007abe30: mov      r8, r2
007abe34: str      r3, [sp, #0x1c]
007abe38: beq      #0x7abed8
007abe3c: ldr      r3, [r5]
007abe40: mov      r0, r5
007abe44: mov      r1, #2
007abe48: mov      lr, pc
007abe4c: ldr      pc, [r3, #8]
007abe50: cmp      r0, #0
007abe54: movne    sl, r5
007abe58: beq      #0x7abec4
007abe5c: mov      r0, r5
007abe60: bl       #0x759c64
007abe64: ldr      r3, [sl]
007abe68: mov      r0, sl
007abe6c: mov      lr, pc
007abe70: ldr      pc, [r3, #0x58]
007abe74: ldr      ip, [sp, #0x40]
007abe78: mov      r1, r0
007abe7c: mov      r3, r8
007abe80: add      r0, sp, #8
007abe84: mov      r2, r5
007abe88: stm      sp, {r7, ip}
007abe8c: bl       #0x7bbbfc
007abe90: ldrsb    r3, [sp, #8]
007abe94: cmn      r3, #1
007abe98: beq      #0x7abee0
007abe9c: mov      r0, r5
007abea0: bl       #0x75a240
007abea4: mov      r0, #1
007abea8: ldr      r3, [r4, r6]
007abeac: ldr      r2, [sp, #0x1c]
007abeb0: ldr      r3, [r3]
007abeb4: cmp      r2, r3
007abeb8: bne      #0x7abf28
007abebc: add      sp, sp, #0x24
007abec0: pop      {r4, r5, r6, r7, r8, sl, pc}
007abec4: add      sl, r5, #0x3c
007abec8: mov      r0, sl
007abecc: bl       #0x438224
007abed0: cmp      r0, #0
007abed4: bne      #0x7abef0
007abed8: mov      r0, #0
007abedc: b        #0x7abea8
007abee0: ldr      r0, [sp, #0x14]
007abee4: ldr      r1, [sp, #0x10]
007abee8: bl       #0x752b38
007abeec: b        #0x7abe9c
007abef0: mov      r0, sl
007abef4: bl       #0x438224
007abef8: mov      r1, #2
007abefc: ldr      r3, [r0]
007abf00: mov      lr, pc
007abf04: ldr      pc, [r3, #8]
007abf08: cmp      r0, #0
007abf0c: beq      #0x7abed8
007abf10: mov      r0, sl
007abf14: bl       #0x438224
007abf18: subs     sl, r0, #0
007abf1c: bne      #0x7abe5c
007abf20: mov      r0, #0
007abf24: b        #0x7abea8
007abf28: bl       #0x30e310
007abf2c: andseq   r8, lr, r4, ror ip
007abf30: andeq    r4, r0, ip, lsr #1

# _ZN8MenuBase19ProcessLocalizationEv
00422d10: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00422d14: ldr      r1, [pc, #0x350]
00422d18: ldr      r2, [pc, #0x350]
00422d1c: sub      sp, sp, #0x274
00422d20: add      r1, pc, r1
00422d24: ldr      r3, [r1, r2]
00422d28: str      r1, [sp, #0x18]
00422d2c: str      r2, [sp, #0x2c]
00422d30: str      r0, [sp, #0x14]
00422d34: ldr      r2, [r0, #0x4c]
00422d38: ldr      r3, [r3]
00422d3c: cmp      r2, #0
00422d40: str      r3, [sp, #0x26c]
00422d44: beq      #0x423034
00422d48: ldr      r0, [r0, #0x48]
00422d4c: ldrb     r3, [r0, #4]
00422d50: cmp      r3, #0
00422d54: beq      #0x42300c
00422d58: ldr      r3, [sp, #0x18]
00422d5c: ldr      r1, [pc, #0x310]
00422d60: ldr      r2, [sp, #0x14]
00422d64: add      r4, sp, #0x254
00422d68: ldr      r5, [r3, r1]
00422d6c: str      r1, [sp, #0x1c]
00422d70: add      fp, r2, #8
00422d74: mov      r0, r5
00422d78: bl       #0x337888
00422d7c: ldr      r1, [pc, #0x2f4]
00422d80: add      r2, sp, #0x38
00422d84: mov      r0, r4
00422d88: add      r1, pc, r1
00422d8c: bl       #0x3140ec
00422d90: mov      r1, r4
00422d94: mov      r0, r5
00422d98: bl       #0x337a88
00422d9c: mov      r0, r4
00422da0: bl       #0x318254
00422da4: ldr      r3, [pc, #0x2d0]
00422da8: ldr      ip, [sp, #0x18]
00422dac: mov      r6, #0
00422db0: ldr      r3, [ip, r3]
00422db4: ldr      r4, [r3, #0x34]
00422db8: ldr      r3, [pc, #0x2c0]
00422dbc: add      r3, pc, r3
00422dc0: str      r3, [sp, #0x10]
00422dc4: ldr      r3, [pc, #0x2b8]
00422dc8: add      r3, pc, r3
00422dcc: str      r3, [sp, #0xc]
00422dd0: ldr      r3, [pc, #0x2b0]
00422dd4: add      r3, pc, r3
00422dd8: str      r3, [sp, #0x20]
00422ddc: ldr      r3, [pc, #0x2a8]
00422de0: add      r3, pc, r3
00422de4: str      r3, [sp, #0x24]
00422de8: b        #0x422e00
00422dec: cmp      r6, #0x13
00422df0: beq      #0x422e0c
00422df4: add      r6, r6, #1
00422df8: cmp      r6, #0x25
00422dfc: beq      #0x423058
00422e00: cmp      r6, #0x1c
00422e04: cmpne    r6, #0x14
00422e08: bne      #0x422dec
00422e0c: mov      r0, r4
00422e10: bl       #0x5075b0
00422e14: mov      r1, r6
00422e18: mov      r3, r0
00422e1c: mov      r2, #0
00422e20: mov      r0, r4
00422e24: bl       #0x5088c4
00422e28: ldr      r2, [sp, #0x14]
00422e2c: ldr      r1, [r2, #0x4c]
00422e30: ldr      r5, [r2, #4]
00422e34: cmp      r1, #0
00422e38: beq      #0x422e4c
00422e3c: ldr      r0, [r2, #0x48]
00422e40: ldrb     r3, [r0, #4]
00422e44: cmp      r3, #0
00422e48: beq      #0x422fe0
00422e4c: mov      r0, r5
00422e50: bl       #0x7a7ee8
00422e54: ldr      ip, [sp, #0x14]
00422e58: ldr      r1, [pc, #0x230]
00422e5c: mov      r5, #0
00422e60: ldr      r0, [ip, #4]
00422e64: str      r1, [sp, #0x28]
00422e68: bl       #0x7add94
00422e6c: mov      sb, r0
00422e70: mov      r0, r4
00422e74: bl       #0x5075b0
00422e78: mov      r1, r6
00422e7c: mov      r2, r0
00422e80: mov      r0, r4
00422e84: bl       #0x507550
00422e88: cmp      r5, r0
00422e8c: bge      #0x422df4
00422e90: mov      r0, r4
00422e94: bl       #0x5075b0
00422e98: mov      r1, r6
00422e9c: mov      r3, r0
00422ea0: mov      r2, r5
00422ea4: mov      r0, r4
00422ea8: bl       #0x5088c4
00422eac: ldr      ip, [sp, #0xc]
00422eb0: add      r7, sp, #0x3c
00422eb4: mov      r3, r0
00422eb8: mov      r2, fp
00422ebc: ldr      r1, [sp, #0x10]
00422ec0: mov      r8, r0
00422ec4: mov      r0, r7
00422ec8: str      ip, [sp]
00422ecc: bl       #0x30eae4
00422ed0: mov      r0, sb
00422ed4: mov      r1, r7
00422ed8: bl       #0x7aadf8
00422edc: subs     sl, r0, #0
00422ee0: beq      #0x422f78
00422ee4: ldr      r3, [sl]
00422ee8: mov      r0, sl
00422eec: mov      r1, #0x20
00422ef0: mov      lr, pc
00422ef4: ldr      pc, [r3, #8]
00422ef8: cmp      r0, #0
00422efc: beq      #0x422f70
00422f00: ldr      r2, [sp, #0x18]
00422f04: ldr      r1, [sp, #0x1c]
00422f08: add      r7, sp, #0x23c
00422f0c: ldr      r8, [r2, r1]
00422f10: mov      r0, r8
00422f14: bl       #0x337888
00422f18: add      r2, sp, #0x34
00422f1c: mov      r0, r7
00422f20: ldr      r1, [sp, #0x20]
00422f24: bl       #0x3140ec
00422f28: mov      r1, r7
00422f2c: mov      r0, r8
00422f30: bl       #0x337a88
00422f34: mov      r0, r7
00422f38: bl       #0x318254
00422f3c: ldr      r3, [sp, #0x14]
00422f40: mov      r1, r6
00422f44: mov      r2, r5
00422f48: mov      r0, r4
00422f4c: ldr      r7, [r3, #4]
00422f50: bl       #0x508c6c
00422f54: mov      ip, #1
00422f58: mov      r3, r0
00422f5c: mov      r1, sl
00422f60: mov      r0, r7
00422f64: ldr      r2, [sp, #0x24]
00422f68: str      ip, [sp]
00422f6c: bl       #0x7a947c
00422f70: add      r5, r5, #1
00422f74: b        #0x422e70
00422f78: ldr      ip, [sp, #0x28]
00422f7c: mov      r2, fp
00422f80: mov      r3, r8
00422f84: add      r1, pc, ip
00422f88: ldr      ip, [sp, #0xc]
00422f8c: mov      r0, r7
00422f90: str      ip, [sp]
00422f94: bl       #0x30eae4
00422f98: mov      r0, sb
00422f9c: mov      r1, r7
00422fa0: bl       #0x7aadf8
00422fa4: subs     sl, r0, #0
00422fa8: bne      #0x422ee4
00422fac: ldr      r1, [pc, #0xe0]
00422fb0: mov      r3, r8
00422fb4: mov      r2, fp
00422fb8: add      r1, pc, r1
00422fbc: mov      r0, r7
00422fc0: bl       #0x30eae4
00422fc4: mov      r0, sb
00422fc8: mov      r1, r7
00422fcc: bl       #0x7aadf8
00422fd0: subs     sl, r0, #0
00422fd4: bne      #0x422ee4
00422fd8: add      r5, r5, #1
00422fdc: b        #0x422e70
00422fe0: ldr      r1, [r0]
00422fe4: sub      r1, r1, #1
00422fe8: cmp      r1, #0
00422fec: str      r1, [r0]
00422ff0: bne      #0x422ff8
00422ff4: bl       #0x752b38
00422ff8: ldr      r3, [sp, #0x14]
00422ffc: mov      r1, #0
00423000: str      r1, [r3, #0x48]
00423004: str      r1, [r3, #0x4c]
00423008: b        #0x422e4c
0042300c: ldr      r1, [r0]
00423010: sub      r1, r1, #1
00423014: cmp      r1, #0
00423018: str      r1, [r0]
0042301c: bne      #0x423024
00423020: bl       #0x752b38
00423024: ldr      ip, [sp, #0x14]
00423028: mov      r3, #0
0042302c: str      r3, [ip, #0x4c]
00423030: str      r3, [ip, #0x48]
00423034: ldr      r2, [sp, #0x18]
00423038: ldr      r1, [sp, #0x2c]
0042303c: ldr      r3, [r2, r1]
00423040: ldr      r2, [sp, #0x26c]
00423044: ldr      r3, [r3]
00423048: cmp      r2, r3
0042304c: bne      #0x423068
00423050: add      sp, sp, #0x274
00423054: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00423058: ldr      ip, [sp, #0x14]
0042305c: mov      r3, #1
00423060: strb     r3, [ip, #0x75]
00423064: b        #0x423034
00423068: bl       #0x30e310
0042306c: subseq   r1, r7, r0, ror sp
00423070: andeq    r4, r0, ip, lsr #1
00423074: andeq    r0, r0, r4, lsl #17
00423078: strheq   r6, [sl], #-0x28
0042307c: strdeq   r3, r4, [r0], -r4
00423080: umaaleq  r6, sl, ip, r2
00423084: subeq    r5, sl, r8, asr #30
00423088: subeq    r6, sl, ip, ror #4
0042308c: subeq    ip, ip, r0, lsl r0
00423090: subeq    r6, sl, r4, ror #1
00423094: subeq    r6, sl, r0, asr #1

# _ZN6MenuFX15SetFocusDefaultEv
007ac444: push     {r4, lr}
007ac448: ldr      r2, [pc, #0x44]
007ac44c: mov      r3, #3
007ac450: ldr      r1, [r0, #0x40]
007ac454: add      r2, pc, r2
007ac458: mov      r4, r0
007ac45c: bl       #0x7a8c08
007ac460: ldr      r3, [r0, #4]
007ac464: cmp      r3, #0
007ac468: ble      #0x7ac484
007ac46c: ldr      r3, [r0]
007ac470: mov      r2, #0
007ac474: mov      r0, r4
007ac478: ldr      r1, [r3]
007ac47c: pop      {r4, lr}
007ac480: b        #0x7ac228
007ac484: mov      r0, r4
007ac488: mov      r1, #0
007ac48c: pop      {r4, lr}
007ac490: b        #0x7ac410
007ac494: andseq   ip, r1, ip, lsr #24

# _ZN8RenderFX8SetFocusEPN7gameswf9characterEi
007ac228: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ac22c: ldr      r4, [pc, #0x1cc]
007ac230: ldr      sl, [pc, #0x1cc]
007ac234: mov      r8, r2
007ac238: add      r4, pc, r4
007ac23c: ldr      r3, [r4, sl]
007ac240: sub      sp, sp, #0x134
007ac244: mov      r5, r0
007ac248: ldr      r2, [r3]
007ac24c: mov      r3, #0x28
007ac250: mla      r3, r3, r8, r0
007ac254: str      r2, [sp, #0x12c]
007ac258: ldr      r7, [r3, #0x68]
007ac25c: mov      r6, r1
007ac260: cmp      r1, r7
007ac264: beq      #0x7ac3b8
007ac268: ldr      sb, [r0, #0xf8]
007ac26c: ands     sb, sb, #0x40
007ac270: bne      #0x7ac314
007ac274: cmp      r7, #0
007ac278: beq      #0x7ac314
007ac27c: ldr      r3, [r7]
007ac280: mov      r0, r7
007ac284: mov      r1, #2
007ac288: mov      lr, pc
007ac28c: ldr      pc, [r3, #8]
007ac290: cmp      r0, #0
007ac294: beq      #0x7ac314
007ac298: ldrb     r3, [r7, #0xea]
007ac29c: cmp      r3, #0
007ac2a0: beq      #0x7ac314
007ac2a4: ldr      r2, [pc, #0x15c]
007ac2a8: mov      r1, r7
007ac2ac: mov      r3, sb
007ac2b0: add      r2, pc, r2
007ac2b4: mov      r0, r5
007ac2b8: bl       #0x7aba04
007ac2bc: mov      r3, #0
007ac2c0: mov      r2, #1
007ac2c4: str      sb, [sp, #0x1c]
007ac2c8: str      r3, [sp, #0x18]
007ac2cc: str      r2, [sp, #0xc]
007ac2d0: str      r3, [sp, #0x14]
007ac2d4: str      r3, [sp, #0x10]
007ac2d8: str      r7, [sp, #4]
007ac2dc: ldr      r2, [r7, #0x44]
007ac2e0: mov      r0, r5
007ac2e4: add      r1, sp, #4
007ac2e8: ldrsb    r3, [r2]
007ac2ec: cmn      r3, #1
007ac2f0: ldreq    r2, [r2, #0xc]
007ac2f4: mov      r3, #0
007ac2f8: addne    r2, r2, #1
007ac2fc: str      r2, [sp, #8]
007ac300: strb     r3, [sp, #0x28]
007ac304: strb     r3, [sp, #0x29]
007ac308: str      r3, [sp, #0x20]
007ac30c: str      r8, [sp, #0x24]
007ac310: bl       #0x7abf34
007ac314: mov      fp, #0x28
007ac318: mul      fp, fp, r8
007ac31c: mov      r1, r6
007ac320: add      fp, fp, #0x68
007ac324: add      fp, r5, fp
007ac328: mov      r0, fp
007ac32c: bl       #0x75518c
007ac330: ldr      r3, [r5, #0xf8]
007ac334: ands     r3, r3, #0x40
007ac338: bne      #0x7ac3b8
007ac33c: cmp      r6, #0
007ac340: beq      #0x7ac3b8
007ac344: ldr      r1, [r6, #0x44]
007ac348: mov      r2, #0
007ac34c: str      r3, [sp, #0xc]
007ac350: str      r2, [sp, #0x18]
007ac354: str      r2, [sp, #0x14]
007ac358: str      r2, [sp, #0x10]
007ac35c: str      r3, [sp, #0x1c]
007ac360: str      r6, [sp, #4]
007ac364: ldrsb    r3, [r1]
007ac368: mov      sb, #0
007ac36c: add      r7, sp, #4
007ac370: cmn      r3, #1
007ac374: ldreq    r1, [r1, #0xc]
007ac378: ldr      r3, [r5, #0xfc]
007ac37c: addne    r1, r1, #1
007ac380: str      r1, [sp, #8]
007ac384: str      r8, [sp, #0x24]
007ac388: strb     sb, [sp, #0x29]
007ac38c: str      sb, [sp, #0x20]
007ac390: strb     sb, [sp, #0x28]
007ac394: mov      r0, r3
007ac398: mov      r1, r7
007ac39c: ldr      r3, [r3]
007ac3a0: mov      lr, pc
007ac3a4: ldr      pc, [r3, #8]
007ac3a8: subs     r1, r0, #0
007ac3ac: bne      #0x7ac3d4
007ac3b0: mov      r0, fp
007ac3b4: bl       #0x75518c
007ac3b8: ldr      r3, [r4, sl]
007ac3bc: ldr      r2, [sp, #0x12c]
007ac3c0: ldr      r3, [r3]
007ac3c4: cmp      r2, r3
007ac3c8: bne      #0x7ac3fc
007ac3cc: add      sp, sp, #0x134
007ac3d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ac3d4: ldr      r2, [pc, #0x30]
007ac3d8: mov      r1, r6
007ac3dc: mov      r3, sb
007ac3e0: add      r2, pc, r2
007ac3e4: mov      r0, r5
007ac3e8: bl       #0x7aba04
007ac3ec: mov      r0, r5
007ac3f0: mov      r1, r7
007ac3f4: bl       #0x7abf34
007ac3f8: b        #0x7ac3b8
007ac3fc: bl       #0x30e310
007ac400: andseq   r8, lr, r8, asr r8
007ac404: andeq    r4, r0, ip, lsr #1
007ac408: ldrsheq  pc, [r1], -r0
007ac40c: andseq   pc, r1, r8, lsr r7

# _ZN8MenuBase17RegisterDeadZonesEv
0042331c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00423320: ldr      r8, [pc, #0x380]
00423324: ldr      r2, [pc, #0x380]
00423328: sub      sp, sp, #0x94
0042332c: add      r8, pc, r8
00423330: ldr      r3, [r8, r2]
00423334: str      r2, [sp, #0xc]
00423338: ldrb     r2, [r0, #0xb4]
0042333c: ldr      r3, [r3]
00423340: mov      r4, r0
00423344: cmp      r2, #0
00423348: str      r3, [sp, #0x8c]
0042334c: beq      #0x423370
00423350: ldr      r2, [sp, #0xc]
00423354: ldr      r3, [r8, r2]
00423358: ldr      r2, [sp, #0x8c]
0042335c: ldr      r3, [r3]
00423360: cmp      r2, r3
00423364: bne      #0x4236a4
00423368: add      sp, sp, #0x94
0042336c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00423370: ldr      r3, [r0, #0x4c]
00423374: cmp      r3, #0
00423378: beq      #0x423350
0042337c: ldr      r0, [r0, #0x48]
00423380: ldrb     r3, [r0, #4]
00423384: cmp      r3, #0
00423388: beq      #0x42366c
0042338c: ldr      r3, [pc, #0x31c]
00423390: add      r5, sp, #0x74
00423394: str      r3, [sp, #0x14]
00423398: mov      r3, #1
0042339c: strb     r3, [r4, #0xb4]
004233a0: ldr      r2, [sp, #0x14]
004233a4: ldr      r6, [r8, r2]
004233a8: mov      r0, r6
004233ac: bl       #0x337888
004233b0: ldr      r1, [pc, #0x2fc]
004233b4: add      r2, sp, #0x58
004233b8: mov      r0, r5
004233bc: add      r1, pc, r1
004233c0: bl       #0x3140ec
004233c4: mov      r1, r5
004233c8: mov      r0, r6
004233cc: bl       #0x337a88
004233d0: mov      r0, r5
004233d4: bl       #0x318254
004233d8: ldr      r1, [r4, #0x4c]
004233dc: ldr      r5, [r4, #4]
004233e0: cmp      r1, #0
004233e4: beq      #0x4233f8
004233e8: ldr      r0, [r4, #0x48]
004233ec: ldrb     r3, [r0, #4]
004233f0: cmp      r3, #0
004233f4: beq      #0x423638
004233f8: ldr      r2, [pc, #0x2b8]
004233fc: mov      r3, #0
00423400: mov      r0, r5
00423404: add      r2, pc, r2
00423408: bl       #0x7a8c08
0042340c: ldr      r3, [r0, #4]
00423410: mov      r7, r0
00423414: cmp      r3, #0
00423418: ble      #0x423350
0042341c: add      r3, r4, #0xc0
00423420: str      r3, [sp, #0x2c]
00423424: ldr      r3, [pc, #0x290]
00423428: add      r2, sp, #0x40
0042342c: str      r2, [sp, #0x1c]
00423430: add      r3, pc, r3
00423434: str      r3, [sp, #0x20]
00423438: add      r2, sp, #0x50
0042343c: add      r3, sp, #0x54
00423440: mov      r5, #0
00423444: add      r6, sp, #0x5c
00423448: str      r3, [sp, #0x18]
0042344c: str      r2, [sp, #0x30]
00423450: str      r8, [sp, #0x10]
00423454: ldr      r3, [r7]
00423458: ldr      r0, [sp, #0x1c]
0042345c: ldr      r1, [r3, r5, lsl #2]
00423460: bl       #0x416a7c
00423464: ldr      r2, [sp, #0x10]
00423468: ldr      r3, [sp, #0x14]
0042346c: ldr      r8, [sp, #0x4c]
00423470: ldr      sb, [sp, #0x48]
00423474: ldr      sl, [r2, r3]
00423478: ldr      r3, [sp, #0x40]
0042347c: ldr      fp, [sp, #0x44]
00423480: mov      r0, sl
00423484: str      r3, [sp, #8]
00423488: bl       #0x337888
0042348c: ldr      r2, [sp, #0x18]
00423490: ldr      r1, [sp, #0x20]
00423494: mov      r0, r6
00423498: bl       #0x3140ec
0042349c: mov      r0, sl
004234a0: mov      r1, r6
004234a4: bl       #0x337a88
004234a8: mov      r0, r6
004234ac: bl       #0x318254
004234b0: ldr      sl, [r4, #0xbc]
004234b4: ldr      r3, [r4, #0xc0]
004234b8: cmp      sl, r3
004234bc: beq      #0x4234f8
004234c0: ldr      r2, [sp, #8]
004234c4: str      r8, [sl, #0xc]
004234c8: str      sb, [sl, #8]
004234cc: str      r2, [sl]
004234d0: str      fp, [sl, #4]
004234d4: ldr      r3, [r4, #0xbc]
004234d8: add      r3, r3, #0x10
004234dc: str      r3, [r4, #0xbc]
004234e0: ldr      r3, [r7, #4]
004234e4: add      r5, r5, #1
004234e8: cmp      r5, r3
004234ec: blt      #0x423454
004234f0: ldr      r8, [sp, #0x10]
004234f4: b        #0x423350
004234f8: ldr      r3, [r4, #0xb8]
004234fc: rsb      r3, r3, sl
00423500: asr      r2, r3, #4
00423504: cmp      r2, #1
00423508: addhs    r3, r2, r2
0042350c: addlo    r3, r2, #1
00423510: cmn      r3, #0xf0000001
00423514: bls      #0x423660
00423518: mvn      r3, #0xf0000000
0042351c: mov      r1, r3
00423520: ldr      r0, [sp, #0x2c]
00423524: ldr      r2, [sp, #0x30]
00423528: str      r3, [sp, #0x50]
0042352c: bl       #0x42242c
00423530: str      r0, [sp, #0x24]
00423534: ldr      r3, [r4, #0xb8]
00423538: rsb      sl, r3, sl
0042353c: asr      sl, sl, #4
00423540: cmp      sl, #0
00423544: str      sl, [sp, #0x28]
00423548: movle    sl, r0
0042354c: ble      #0x4235c0
00423550: str      sb, [sp, #0x38]
00423554: ldr      sl, [sp, #0x28]
00423558: mov      sb, r7
0042355c: mov      r7, r6
00423560: ldr      r6, [sp, #0x24]
00423564: str      r8, [sp, #0x34]
00423568: str      fp, [sp, #0x3c]
0042356c: mov      ip, #0
00423570: mov      fp, r5
00423574: mov      r8, r4
00423578: mov      r5, r3
0042357c: add      r4, r6, ip
00423580: add      r3, r5, ip
00423584: subs     sl, sl, #1
00423588: ldm      r3, {r0, r1, r2, r3}
0042358c: add      ip, ip, #0x10
00423590: stm      r4, {r0, r1, r2, r3}
00423594: bne      #0x42357c
00423598: ldr      r3, [sp, #0x24]
0042359c: ldr      r2, [sp, #0x28]
004235a0: mov      r5, fp
004235a4: mov      r4, r8
004235a8: mov      r6, r7
004235ac: ldr      fp, [sp, #0x3c]
004235b0: mov      r7, sb
004235b4: ldr      r8, [sp, #0x34]
004235b8: ldr      sb, [sp, #0x38]
004235bc: add      sl, r3, r2, lsl #4
004235c0: str      r8, [sl, #0xc]
004235c4: str      sb, [sl, #8]
004235c8: str      fp, [sl, #4]
004235cc: ldr      r3, [sp, #8]
004235d0: mov      ip, sl
004235d4: str      r3, [ip], #0x10
004235d8: ldr      r3, [r4, #0xbc]
004235dc: ldr      r0, [r4, #0xb8]
004235e0: cmp      r3, r0
004235e4: subne    r2, r3, #0x10
004235e8: rsbne    r2, r0, r2
004235ec: mvnne    r2, r2, lsr #4
004235f0: addne    r3, r3, r2, lsl #4
004235f4: cmp      r3, #0
004235f8: ldr      r2, [r4, #0xc0]
004235fc: beq      #0x42361c
00423600: rsb      r3, r3, r2
00423604: bic      r1, r3, #0xf
00423608: cmp      r1, #0x80
0042360c: bhi      #0x423694
00423610: str      ip, [sp, #4]
00423614: bl       #0x708f00
00423618: ldr      ip, [sp, #4]
0042361c: ldr      r3, [sp, #0x50]
00423620: ldr      r2, [sp, #0x24]
00423624: str      ip, [r4, #0xbc]
00423628: add      r3, r2, r3, lsl #4
0042362c: str      r2, [r4, #0xb8]
00423630: str      r3, [r4, #0xc0]
00423634: b        #0x4234e0
00423638: ldr      r1, [r0]
0042363c: sub      r1, r1, #1
00423640: cmp      r1, #0
00423644: str      r1, [r0]
00423648: bne      #0x423650
0042364c: bl       #0x752b38
00423650: mov      r1, #0
00423654: str      r1, [r4, #0x48]
00423658: str      r1, [r4, #0x4c]
0042365c: b        #0x4233f8
00423660: cmp      r2, r3
00423664: bls      #0x42351c
00423668: b        #0x423518
0042366c: ldr      r1, [r0]
00423670: sub      r1, r1, #1
00423674: cmp      r1, #0
00423678: str      r1, [r0]
0042367c: bne      #0x423684
00423680: bl       #0x752b38
00423684: mov      r3, #0
00423688: str      r3, [r4, #0x4c]
0042368c: str      r3, [r4, #0x48]
00423690: b        #0x423350
00423694: str      ip, [sp, #4]
00423698: bl       #0x310440
0042369c: ldr      ip, [sp, #4]
004236a0: b        #0x42361c
004236a4: bl       #0x30e310
004236a8: subseq   r1, r7, r4, ror #14
004236ac: andeq    r4, r0, ip, lsr #1
004236b0: andeq    r0, r0, r4, lsl #17
004236b4: subeq    r5, sl, r4, lsl #25
004236b8: subeq    r5, sl, ip, ror #25
004236bc: subeq    r5, sl, r0, lsl ip

# _ZN8MenuBaseC1EPKc
004269dc: push     {r4, r5, r6, r7, r8, sl, lr}
004269e0: ldr      r4, [pc, #0x608]
004269e4: ldr      r3, [pc, #0x608]
004269e8: sub      sp, sp, #0x24
004269ec: add      r4, pc, r4
004269f0: ldr      r7, [r4, r3]
004269f4: mov      r5, r0
004269f8: mov      sl, #0
004269fc: ldr      r3, [r7]
00426a00: add      r6, sp, #4
00426a04: str      r3, [sp, #0x1c]
00426a08: bl       #0x421f80
00426a0c: ldr      r2, [pc, #0x5e4]
00426a10: add      r3, r5, #0x80
00426a14: add      r1, r5, #0x6c
00426a18: ldr      r2, [r4, r2]
00426a1c: mov      r0, r3
00426a20: str      r1, [r5, #0x70]
00426a24: add      r2, r2, #8
00426a28: str      r2, [r5]
00426a2c: str      r1, [r5, #0x6c]
00426a30: str      r3, [r5, #0x90]
00426a34: str      r3, [r5, #0x94]
00426a38: mov      r1, #0x10
00426a3c: str      sl, [r5, #0x5c]
00426a40: str      sl, [r5, #0x60]
00426a44: str      sl, [r5, #0x64]
00426a48: str      sl, [r5, #0x68]
00426a4c: strb     sl, [r5, #0x74]
00426a50: strb     sl, [r5, #0x75]
00426a54: str      sl, [r5, #0x78]
00426a58: strb     sl, [r5, #0x7c]
00426a5c: strb     sl, [r5, #0x7d]
00426a60: bl       #0x31167c
00426a64: ldr      r2, [r5, #0x90]
00426a68: add      r3, r5, #0x9c
00426a6c: mov      r0, r3
00426a70: strb     sl, [r2]
00426a74: mov      r1, #0x10
00426a78: str      r3, [r5, #0xac]
00426a7c: str      r3, [r5, #0xb0]
00426a80: bl       #0x31167c
00426a84: ldr      r2, [pc, #0x570]
00426a88: ldr      r3, [r5, #0xac]
00426a8c: ldr      r8, [r4, r2]
00426a90: strb     sl, [r3]
00426a94: str      sl, [r5, #0xc0]
00426a98: mov      r0, r8
00426a9c: strb     sl, [r5, #0xb4]
00426aa0: str      sl, [r5, #0xb8]
00426aa4: str      sl, [r5, #0xbc]
00426aa8: bl       #0x337888
00426aac: ldr      r1, [pc, #0x54c]
00426ab0: mov      r2, sp
00426ab4: mov      r0, r6
00426ab8: add      r1, pc, r1
00426abc: bl       #0x3140ec
00426ac0: mov      r1, r6
00426ac4: mov      r0, r8
00426ac8: bl       #0x337a88
00426acc: mov      r0, r6
00426ad0: bl       #0x318254
00426ad4: ldr      r3, [pc, #0x528]
00426ad8: ldr      r2, [pc, #0x528]
00426adc: ldr      r0, [pc, #0x528]
00426ae0: ldr      r3, [r4, r3]
00426ae4: ldr      r1, [r4, r2]
00426ae8: mov      r2, #1
00426aec: strb     r2, [r3]
00426af0: add      r0, pc, r0
00426af4: bl       #0x426980
00426af8: ldr      r3, [pc, #0x510]
00426afc: ldr      r0, [pc, #0x510]
00426b00: ldr      r1, [r4, r3]
00426b04: add      r0, pc, r0
00426b08: bl       #0x426980
00426b0c: ldr      r3, [pc, #0x504]
00426b10: ldr      r0, [pc, #0x504]
00426b14: ldr      r1, [r4, r3]
00426b18: add      r0, pc, r0
00426b1c: bl       #0x426980
00426b20: ldr      r3, [pc, #0x4f8]
00426b24: ldr      r0, [pc, #0x4f8]
00426b28: ldr      r1, [r4, r3]
00426b2c: add      r0, pc, r0
00426b30: bl       #0x426980
00426b34: ldr      r3, [pc, #0x4ec]
00426b38: ldr      r0, [pc, #0x4ec]
00426b3c: ldr      r1, [r4, r3]
00426b40: add      r0, pc, r0
00426b44: bl       #0x426980
00426b48: ldr      r3, [pc, #0x4e0]
00426b4c: ldr      r0, [pc, #0x4e0]
00426b50: ldr      r1, [r4, r3]
00426b54: add      r0, pc, r0
00426b58: bl       #0x426980
00426b5c: ldr      r3, [pc, #0x4d4]
00426b60: ldr      r0, [pc, #0x4d4]
00426b64: ldr      r1, [r4, r3]
00426b68: add      r0, pc, r0
00426b6c: bl       #0x426980
00426b70: ldr      r3, [pc, #0x4c8]
00426b74: ldr      r0, [pc, #0x4c8]
00426b78: ldr      r1, [r4, r3]
00426b7c: add      r0, pc, r0
00426b80: bl       #0x426980
00426b84: ldr      r3, [pc, #0x4bc]
00426b88: ldr      r0, [pc, #0x4bc]
00426b8c: ldr      r1, [r4, r3]
00426b90: add      r0, pc, r0
00426b94: bl       #0x426980
00426b98: ldr      r3, [pc, #0x4b0]
00426b9c: ldr      r0, [pc, #0x4b0]
00426ba0: ldr      r1, [r4, r3]
00426ba4: add      r0, pc, r0
00426ba8: bl       #0x426980
00426bac: ldr      r3, [pc, #0x4a4]
00426bb0: ldr      r0, [pc, #0x4a4]
00426bb4: ldr      r1, [r4, r3]
00426bb8: add      r0, pc, r0
00426bbc: bl       #0x426980
00426bc0: ldr      r3, [pc, #0x498]
00426bc4: ldr      r0, [pc, #0x498]
00426bc8: ldr      r1, [r4, r3]
00426bcc: add      r0, pc, r0
00426bd0: bl       #0x426980
00426bd4: ldr      r3, [pc, #0x48c]
00426bd8: ldr      r0, [pc, #0x48c]
00426bdc: ldr      r1, [r4, r3]
00426be0: add      r0, pc, r0
00426be4: bl       #0x426980
00426be8: ldr      r3, [pc, #0x480]
00426bec: ldr      r0, [pc, #0x480]
00426bf0: ldr      r1, [r4, r3]
00426bf4: add      r0, pc, r0
00426bf8: bl       #0x426980
00426bfc: ldr      r3, [pc, #0x474]
00426c00: ldr      r0, [pc, #0x474]
00426c04: ldr      r1, [r4, r3]
00426c08: add      r0, pc, r0
00426c0c: bl       #0x426980
00426c10: ldr      r3, [pc, #0x468]
00426c14: ldr      r0, [pc, #0x468]
00426c18: ldr      r1, [r4, r3]
00426c1c: add      r0, pc, r0
00426c20: bl       #0x426980
00426c24: ldr      r3, [pc, #0x45c]
00426c28: ldr      r0, [pc, #0x45c]
00426c2c: ldr      r1, [r4, r3]
00426c30: add      r0, pc, r0
00426c34: bl       #0x426980
00426c38: ldr      r3, [pc, #0x450]
00426c3c: ldr      r0, [pc, #0x450]
00426c40: ldr      r1, [r4, r3]
00426c44: add      r0, pc, r0
00426c48: bl       #0x426980
00426c4c: ldr      r3, [pc, #0x444]
00426c50: ldr      r0, [pc, #0x444]
00426c54: ldr      r1, [r4, r3]
00426c58: add      r0, pc, r0
00426c5c: bl       #0x426980
00426c60: ldr      r3, [pc, #0x438]
00426c64: ldr      r0, [pc, #0x438]
00426c68: ldr      r1, [r4, r3]
00426c6c: add      r0, pc, r0
00426c70: bl       #0x426980
00426c74: ldr      r3, [pc, #0x42c]
00426c78: ldr      r0, [pc, #0x42c]
00426c7c: ldr      r1, [r4, r3]
00426c80: add      r0, pc, r0
00426c84: bl       #0x426980
00426c88: ldr      r3, [pc, #0x420]
00426c8c: ldr      r0, [pc, #0x420]
00426c90: ldr      r1, [r4, r3]
00426c94: add      r0, pc, r0
00426c98: bl       #0x426980
00426c9c: ldr      r3, [pc, #0x414]
00426ca0: ldr      r0, [pc, #0x414]
00426ca4: ldr      r1, [r4, r3]
00426ca8: add      r0, pc, r0
00426cac: bl       #0x426980
00426cb0: ldr      r3, [pc, #0x408]
00426cb4: ldr      r0, [pc, #0x408]
00426cb8: ldr      r1, [r4, r3]
00426cbc: add      r0, pc, r0
00426cc0: bl       #0x426980
00426cc4: ldr      r3, [pc, #0x3fc]
00426cc8: ldr      r0, [pc, #0x3fc]
00426ccc: ldr      r1, [r4, r3]
00426cd0: add      r0, pc, r0
00426cd4: bl       #0x426980
00426cd8: ldr      r3, [pc, #0x3f0]
00426cdc: ldr      r0, [pc, #0x3f0]
00426ce0: ldr      r1, [r4, r3]
00426ce4: add      r0, pc, r0
00426ce8: bl       #0x426980
00426cec: ldr      r3, [pc, #0x3e4]
00426cf0: ldr      r0, [pc, #0x3e4]
00426cf4: ldr      r1, [r4, r3]
00426cf8: add      r0, pc, r0
00426cfc: bl       #0x426980
00426d00: ldr      r3, [pc, #0x3d8]
00426d04: ldr      r0, [pc, #0x3d8]
00426d08: ldr      r1, [r4, r3]
00426d0c: add      r0, pc, r0
00426d10: bl       #0x426980
00426d14: ldr      r3, [pc, #0x3cc]
00426d18: ldr      r0, [pc, #0x3cc]
00426d1c: ldr      r1, [r4, r3]
00426d20: add      r0, pc, r0
00426d24: bl       #0x426980
00426d28: ldr      r3, [pc, #0x3c0]
00426d2c: ldr      r0, [pc, #0x3c0]
00426d30: ldr      r1, [r4, r3]
00426d34: add      r0, pc, r0
00426d38: bl       #0x426980
00426d3c: ldr      r3, [pc, #0x3b4]
00426d40: ldr      r0, [pc, #0x3b4]
00426d44: ldr      r1, [r4, r3]
00426d48: add      r0, pc, r0
00426d4c: bl       #0x426980
00426d50: ldr      r3, [pc, #0x3a8]
00426d54: ldr      r0, [pc, #0x3a8]
00426d58: ldr      r1, [r4, r3]
00426d5c: add      r0, pc, r0
00426d60: bl       #0x426980
00426d64: ldr      r3, [pc, #0x39c]
00426d68: ldr      r0, [pc, #0x39c]
00426d6c: ldr      r1, [r4, r3]
00426d70: add      r0, pc, r0
00426d74: bl       #0x426980
00426d78: ldr      r3, [pc, #0x390]
00426d7c: ldr      r0, [pc, #0x390]
00426d80: ldr      r1, [r4, r3]
00426d84: add      r0, pc, r0
00426d88: bl       #0x426980
00426d8c: ldr      r3, [pc, #0x384]
00426d90: ldr      r0, [pc, #0x384]
00426d94: ldr      r1, [r4, r3]
00426d98: add      r0, pc, r0
00426d9c: bl       #0x426980
00426da0: ldr      r3, [pc, #0x378]
00426da4: ldr      r0, [pc, #0x378]
00426da8: ldr      r1, [r4, r3]
00426dac: add      r0, pc, r0
00426db0: bl       #0x426980
00426db4: ldr      r3, [pc, #0x36c]
00426db8: ldr      r0, [pc, #0x36c]
00426dbc: ldr      r1, [r4, r3]
00426dc0: add      r0, pc, r0
00426dc4: bl       #0x426980
00426dc8: ldr      r3, [pc, #0x360]
00426dcc: ldr      r0, [pc, #0x360]
00426dd0: ldr      r1, [r4, r3]
00426dd4: add      r0, pc, r0
00426dd8: bl       #0x426980
00426ddc: ldr      r3, [pc, #0x354]
00426de0: ldr      r0, [pc, #0x354]
00426de4: ldr      r1, [r4, r3]
00426de8: add      r0, pc, r0
00426dec: bl       #0x426980
00426df0: ldr      r3, [pc, #0x348]
00426df4: ldr      r0, [pc, #0x348]
00426df8: ldr      r1, [r4, r3]
00426dfc: add      r0, pc, r0
00426e00: bl       #0x426980
00426e04: ldr      r3, [pc, #0x33c]
00426e08: ldr      r0, [pc, #0x33c]
00426e0c: ldr      r1, [r4, r3]
00426e10: add      r0, pc, r0
00426e14: bl       #0x426980
00426e18: ldr      r3, [pc, #0x330]
00426e1c: ldr      r0, [pc, #0x330]
00426e20: ldr      r1, [r4, r3]
00426e24: add      r0, pc, r0
00426e28: bl       #0x426980
00426e2c: ldr      r3, [pc, #0x324]
00426e30: ldr      r0, [pc, #0x324]
00426e34: ldr      r1, [r4, r3]
00426e38: add      r0, pc, r0
00426e3c: bl       #0x426980
00426e40: ldr      r3, [pc, #0x318]
00426e44: ldr      r0, [pc, #0x318]
00426e48: ldr      r1, [r4, r3]
00426e4c: add      r0, pc, r0
00426e50: bl       #0x426980
00426e54: ldr      r3, [pc, #0x30c]
00426e58: ldr      r0, [pc, #0x30c]
00426e5c: ldr      r1, [r4, r3]
00426e60: add      r0, pc, r0
00426e64: bl       #0x426980
00426e68: ldr      r3, [pc, #0x300]
00426e6c: ldr      r0, [pc, #0x300]
00426e70: ldr      r1, [r4, r3]
00426e74: add      r0, pc, r0
00426e78: bl       #0x426980
00426e7c: ldr      r3, [pc, #0x2f4]
00426e80: ldr      r0, [pc, #0x2f4]
00426e84: ldr      r1, [r4, r3]
00426e88: add      r0, pc, r0
00426e8c: bl       #0x426980
00426e90: ldr      r3, [pc, #0x2e8]
00426e94: ldr      r0, [pc, #0x2e8]
00426e98: ldr      r1, [r4, r3]
00426e9c: add      r0, pc, r0
00426ea0: bl       #0x426980
00426ea4: ldr      r3, [pc, #0x2dc]
00426ea8: ldr      r0, [pc, #0x2dc]
00426eac: ldr      r1, [r4, r3]
00426eb0: add      r0, pc, r0
00426eb4: bl       #0x426980
00426eb8: ldr      r3, [pc, #0x2d0]
00426ebc: ldr      r0, [pc, #0x2d0]
00426ec0: ldr      r1, [r4, r3]
00426ec4: add      r0, pc, r0
00426ec8: bl       #0x426980
00426ecc: ldr      r3, [pc, #0x2c4]
00426ed0: ldr      r0, [pc, #0x2c4]
00426ed4: ldr      r1, [r4, r3]
00426ed8: add      r0, pc, r0
00426edc: bl       #0x426980
00426ee0: ldr      r3, [pc, #0x2b8]
00426ee4: ldr      r0, [pc, #0x2b8]
00426ee8: ldr      r1, [r4, r3]
00426eec: add      r0, pc, r0
00426ef0: bl       #0x426980
00426ef4: ldr      r3, [pc, #0x2ac]
00426ef8: ldr      r0, [pc, #0x2ac]
00426efc: ldr      r1, [r4, r3]
00426f00: add      r0, pc, r0
00426f04: bl       #0x426980
00426f08: ldr      r3, [pc, #0x2a0]
00426f0c: ldr      r0, [pc, #0x2a0]
00426f10: ldr      r1, [r4, r3]
00426f14: add      r0, pc, r0
00426f18: bl       #0x426980
00426f1c: ldr      r3, [pc, #0x294]
00426f20: ldr      r0, [pc, #0x294]
00426f24: ldr      r1, [r4, r3]
00426f28: add      r0, pc, r0
00426f2c: bl       #0x426980
00426f30: ldr      r3, [pc, #0x288]
00426f34: ldr      r0, [pc, #0x288]
00426f38: ldr      r1, [r4, r3]
00426f3c: add      r0, pc, r0
00426f40: bl       #0x426980
00426f44: ldr      r3, [pc, #0x27c]
00426f48: ldr      r0, [pc, #0x27c]
00426f4c: ldr      r1, [r4, r3]
00426f50: add      r0, pc, r0
00426f54: bl       #0x426980
00426f58: ldr      r3, [pc, #0x270]
00426f5c: ldr      r0, [pc, #0x270]
00426f60: ldr      r1, [r4, r3]
00426f64: add      r0, pc, r0
00426f68: bl       #0x426980
00426f6c: ldr      r3, [pc, #0x264]
00426f70: ldr      r0, [pc, #0x264]
00426f74: ldr      r1, [r4, r3]
00426f78: add      r0, pc, r0
00426f7c: bl       #0x426980
00426f80: ldr      r3, [pc, #0x258]
00426f84: ldr      r0, [pc, #0x258]
00426f88: ldr      r1, [r4, r3]
00426f8c: add      r0, pc, r0
00426f90: bl       #0x426980
00426f94: ldr      r3, [pc, #0x24c]
00426f98: ldr      r0, [pc, #0x24c]
00426f9c: ldr      r1, [r4, r3]
00426fa0: add      r0, pc, r0
00426fa4: bl       #0x426980
00426fa8: ldr      r3, [pc, #0x240]
00426fac: ldr      r0, [pc, #0x240]
00426fb0: ldr      r1, [r4, r3]
00426fb4: add      r0, pc, r0
00426fb8: bl       #0x426980
00426fbc: ldr      r3, [pc, #0x234]
00426fc0: ldr      r0, [pc, #0x234]
00426fc4: ldr      r1, [r4, r3]
00426fc8: add      r0, pc, r0
00426fcc: bl       #0x426980
00426fd0: ldr      r2, [sp, #0x1c]
00426fd4: ldr      r3, [r7]
00426fd8: mov      r0, r5
00426fdc: cmp      r2, r3
00426fe0: bne      #0x426fec
00426fe4: add      sp, sp, #0x24
00426fe8: pop      {r4, r5, r6, r7, r8, sl, pc}
00426fec: bl       #0x30e310
00426ff0: subseq   lr, r6, r4, lsr #1
00426ff4: andeq    r4, r0, ip, lsr #1
00426ff8: andeq    r4, r0, ip, lsl sl
00426ffc: andeq    r0, r0, r4, lsl #17
00427000: subeq    r2, sl, r8, lsl #11
00427004: andeq    r3, r0, r4, ror #14
00427008: andeq    r3, r0, ip, ror #29
0042700c: subeq    ip, sb, r8, lsr #25
00427010: andeq    r3, r0, r4, asr #10
00427014: subeq    ip, sb, ip, lsl #25
00427018: muleq    r0, r4, lr
0042701c: subeq    ip, sb, r8, ror #24
00427020: andeq    r1, r0, r0, lsr #3
00427024: subeq    r2, sl, r4, lsr #14
00427028: andeq    r4, r0, r0, lsr #32
0042702c: subeq    r2, sl, r0, lsr #14
00427030: andeq    r2, r0, r4, asr #5
00427034: subeq    r2, sl, ip, lsl r7
00427038: andeq    r3, r0, r8, asr #26
0042703c: subeq    r2, sl, r8, lsl r7
00427040: muleq    r0, r4, r3
00427044: subeq    r2, sl, r4, lsl r7
00427048: muleq    r0, r0, r7
0042704c: subeq    fp, sb, r0, rrx
00427050: ldrdeq   r2, r3, [r0], -r8
00427054: strdeq   r2, r3, [sl], #-0x6c
00427058: andeq    r2, r0, r8, ror #17
0042705c: subeq    r2, sl, r8, lsl #8
00427060: andeq    r2, r0, r0, lsr lr
00427064: subeq    r2, sl, r4, ror #13
00427068: andeq    r3, r0, r0, lsr sl
0042706c: subeq    r2, sl, r0, ror #13
00427070: ldrdeq   r0, r1, [r0], -ip
00427074: ldrdeq   r2, r3, [sl], #-0x6c
00427078: andeq    r0, r0, ip, asr ip
0042707c: ldrdeq   r2, r3, [sl], #-0x68
00427080: andeq    r0, r0, ip, lsr #17
00427084: ldrdeq   r2, r3, [sl], #-0x64
00427088: muleq    r0, r4, r7
0042708c: ldrdeq   r2, r3, [sl], #-0x60
00427090: andeq    r3, r0, ip, lsl #8
00427094: subeq    r2, sl, ip, asr #13
00427098: strdeq   r4, r5, [r0], -ip
0042709c: subeq    r2, sl, r8, asr #13
004270a0: andeq    r1, r0, r4, ror #18
004270a4: subeq    r2, sl, r4, asr #13
004270a8: andeq    r3, r0, r0, lsl #26
004270ac: subeq    r2, sl, r0, asr #13
004270b0: andeq    r2, r0, r0, lsl #29
004270b4: strheq   r2, [sl], #-0x6c
004270b8: andeq    r4, r0, r8, asr #9
004270bc: strdeq   r2, r3, [sl], #-0x10
004270c0: andeq    r0, r0, r0, lsr ip
004270c4: subeq    r2, sl, r4, lsr #13
004270c8: strheq   r2, [r0], -r4
004270cc: subeq    r2, sl, r0, lsr #13
004270d0: andeq    r1, r0, ip, ror #25
004270d4: umaaleq  r2, sl, ip, r6
004270d8: andeq    r2, r0, r0, lsl #16
004270dc: umaaleq  r2, sl, r8, r6
004270e0: andeq    r0, r0, r0, lsl #31
004270e4: umaaleq  r2, sl, r4, r6
004270e8: andeq    r2, r0, r4, lsl #11
004270ec: umaaleq  r2, sl, r0, r6
004270f0: strdeq   r4, r5, [r0], -ip
004270f4: subeq    r2, sl, ip, lsl #13
004270f8: andeq    r3, r0, r0, lsr #11
004270fc: subeq    r2, sl, r8, lsl #13
00427100: strdeq   r3, r4, [r0], -ip
00427104: subeq    r2, sl, r4, lsl #13
00427108: ldrdeq   r3, r4, [r0], -r4
0042710c: subeq    r2, sl, r8, ror r6
00427110: andeq    r2, r0, r8, lsl r0
00427114: subeq    r2, sl, r4, ror r6
00427118: andeq    r3, r0, ip, ror lr
0042711c: subeq    r2, sl, r0, ror r6
00427120: muleq    r0, r8, r0
00427124: subeq    r2, sl, ip, ror #12
00427128: andeq    r4, r0, r4, asr #18
0042712c: subeq    r2, sl, r0, ror r6
00427130: andeq    r3, r0, r0, lsr #21
00427134: subeq    r2, sl, ip, ror #12
00427138: strdeq   r0, r1, [r0], -r0
0042713c: subeq    r2, sl, r8, ror #12
00427140: andeq    r3, r0, r0, lsl #10
00427144: subeq    r2, sl, r4, ror #12
00427148: andeq    r2, r0, r4, lsr r0
0042714c: subeq    r2, sl, r8, ror #12
00427150: andeq    r1, r0, r4, ror #8
00427154: subeq    r2, sl, ip, ror #12
00427158: andeq    r3, r0, r4, lsr r6
0042715c: subeq    r2, sl, r0, ror r6
00427160: strheq   r3, [r0], -r4
00427164: subeq    r2, sl, ip, ror #12
00427168: andeq    r1, r0, r8, asr r3
0042716c: subeq    r2, sl, r8, ror #12
00427170: ldrdeq   r2, r3, [r0], -r0
00427174: subeq    r2, sl, r4, ror #12
00427178: strheq   r3, [r0], -ip
0042717c: subeq    r2, sl, r0, ror #12
00427180: andeq    r4, r0, r8, lsl #11
00427184: subeq    r2, sl, r4, ror #12
00427188: andeq    r3, r0, r4, asr r1
0042718c: subeq    r2, sl, r0, ror #12
00427190: andeq    r3, r0, r8, asr #4
00427194: subeq    r2, sl, ip, asr r6
00427198: andeq    r1, r0, ip, lsr #22
0042719c: subeq    r2, sl, r8, asr r6
004271a0: andeq    r2, r0, ip, lsl lr
004271a4: subeq    r2, sl, r4, asr r6
004271a8: andeq    r1, r0, r0, lsl #10
004271ac: subeq    r2, sl, r0, asr r6
004271b0: andeq    r4, r0, r8, lsr #3
004271b4: subeq    r2, sl, ip, asr #12
004271b8: andeq    r3, r0, r8, lsr #9
004271bc: subeq    r2, sl, r8, asr #12
004271c0: andeq    r4, r0, ip, lsr sb
004271c4: subeq    sp, sb, r4, lsr #6
004271c8: andeq    r1, r0, r4, asr #27
004271cc: subeq    r2, sl, r0, lsr r6
004271d0: andeq    r1, r0, r8, asr #29
004271d4: subeq    r2, sl, ip, lsr #12
004271d8: andeq    r2, r0, ip, lsl r3
004271dc: subeq    r2, sl, r8, lsr #12
004271e0: andeq    r2, r0, r0, lsr r7
004271e4: subeq    r2, sl, r4, lsr #12
004271e8: strheq   r1, [r0], -r4
004271ec: subeq    r2, sl, r0, lsr #12
004271f0: andeq    r1, r0, r4, asr #23
004271f4: subeq    r2, sl, ip, lsl r6
004271f8: ldrdeq   r1, r2, [r0], -r0
004271fc: subeq    r2, sl, r8, lsl r6

# _ZN8RenderFX8PlayAnimEPN7gameswf9characterEPKci
007aba04: mov      r3, #1
007aba08: b        #0x7ab924

# _ZN8RenderFX10ResetFocusEi
007ac410: push     {r4, r5, r6, lr}
007ac414: mov      r2, r1
007ac418: mov      r4, r1
007ac41c: mov      r1, #0
007ac420: mov      r5, r0
007ac424: bl       #0x7ac228
007ac428: mov      r0, #0x28
007ac42c: mul      r4, r0, r4
007ac430: mov      r1, #0
007ac434: add      r0, r4, #0x78
007ac438: add      r0, r5, r0
007ac43c: pop      {r4, r5, r6, lr}
007ac440: b        #0x75518c
