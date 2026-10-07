
# _ZNK8MenuBase11IsValidMenuEv
0041b3f0: ldrb     r0, [r0, #0x7c]
0041b3f4: bx       lr

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

# _ZN8MenuBase9LostFocusEv
0041b3ec: bx       lr

# _ZN8MenuBase8GotFocusEv
0041b3e8: bx       lr
