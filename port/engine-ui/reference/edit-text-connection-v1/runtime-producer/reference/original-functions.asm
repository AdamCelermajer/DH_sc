# _ZN8RenderFX23SetTextBufferingEnabledEb
007a7cb4: ldr      r3, [r0, #0x3c]
007a7cb8: strb     r1, [r3, #0x85]
007a7cbc: bx       lr

# _ZN7gameswf4root19flush_buffered_textEv
007748b8: push     {r4, r5, r6, lr}
007748bc: ldr      r3, [r0, #0x9c]
007748c0: mov      r5, r0
007748c4: cmp      r3, #0
007748c8: ble      #0x774914
007748cc: mov      r3, #1
007748d0: strb     r3, [r0, #0x86]
007748d4: mov      r4, #0
007748d8: ldr      r3, [r5, #0x98]
007748dc: ldr      r3, [r3, r4, lsl #2]
007748e0: add      r4, r4, #1
007748e4: mov      r0, r3
007748e8: ldr      r3, [r3]
007748ec: mov      lr, pc
007748f0: ldr      pc, [r3, #0x120]
007748f4: ldr      r3, [r5, #0x9c]
007748f8: cmp      r4, r3
007748fc: blt      #0x7748d8
00774900: mov      r3, #0
00774904: add      r0, r5, #0x98
00774908: strb     r3, [r5, #0x86]
0077490c: pop      {r4, r5, r6, lr}
00774910: b        #0x774848
00774914: pop      {r4, r5, r6, pc}

# _ZN7gameswf4root18set_display_boundsEiiiiNS_10scale_modeE
007755f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007755f8: ldr      r5, [pc, #0x718]
007755fc: ldr      r6, [pc, #0x718]
00775600: ldr      lr, [pc, #0x718]
00775604: add      r5, pc, r5
00775608: ldr      ip, [r5, r6]
0077560c: ldr      r8, [r5, lr]
00775610: mov      r4, r0
00775614: ldr      r0, [ip]
00775618: ldr      ip, [r8]
0077561c: sub      sp, sp, #0xd4
00775620: str      r0, [sp, #0xcc]
00775624: mov      r7, r1
00775628: mov      r0, ip
0077562c: ldr      r1, [ip]
00775630: mov      sl, r2
00775634: mov      sb, r3
00775638: ldr      fp, [sp, #0xf8]
0077563c: mov      lr, pc
00775640: ldr      pc, [r1, #0xac]
00775644: cmp      r0, #0
00775648: bne      #0x775ba4
0077564c: mov      r0, sb
00775650: bl       #0x30e964
00775654: ldr      r8, [r4, #0xc]
00775658: mov      r3, r0
0077565c: ldr      r1, [r8, #0xb4]
00775660: ldr      r0, [r8, #0xb8]
00775664: str      r3, [sp]
00775668: bl       #0x30e3ac
0077566c: mov      r1, #0x41000000
00775670: add      r1, r1, #0xa00000
00775674: bl       #0x30ec94
00775678: ldr      r3, [sp]
0077567c: mov      r1, r0
00775680: mov      r0, r3
00775684: bl       #0x30ec94
00775688: mov      r3, r0
0077568c: mov      r0, fp
00775690: str      r3, [sp]
00775694: bl       #0x30e964
00775698: ldr      r1, [r8, #0xbc]
0077569c: mov      r2, r0
007756a0: ldr      r0, [r8, #0xc0]
007756a4: str      r2, [sp, #4]
007756a8: bl       #0x30e3ac
007756ac: mov      r1, #0x41000000
007756b0: add      r1, r1, #0xa00000
007756b4: bl       #0x30ec94
007756b8: ldr      r2, [sp, #4]
007756bc: mov      r1, r0
007756c0: mov      r0, r2
007756c4: bl       #0x30ec94
007756c8: ldr      r3, [sp]
007756cc: mov      r2, #1
007756d0: str      r2, [sp, #0xc]
007756d4: mov      r1, r3
007756d8: bl       #0x30ec94
007756dc: ldr      r3, [sp, #0xfc]
007756e0: str      r0, [sp, #8]
007756e4: cmp      r3, #1
007756e8: beq      #0x775b6c
007756ec: cmp      r3, #2
007756f0: beq      #0x775b04
007756f4: ldr      r3, [r4, #0x24]
007756f8: cmp      r3, r7
007756fc: beq      #0x775b44
00775700: ldr      r2, [sp, #0xc]
00775704: str      r7, [r4, #0x24]
00775708: str      sl, [r4, #0x28]
0077570c: cmp      r2, #0
00775710: str      sb, [r4, #0x2c]
00775714: str      fp, [r4, #0x30]
00775718: bne      #0x775ab8
0077571c: mov      r0, sb
00775720: bl       #0x30e964
00775724: ldr      r1, [r8, #0xbc]
00775728: mov      r7, r0
0077572c: ldr      r0, [r8, #0xc0]
00775730: bl       #0x30e3ac
00775734: mov      r1, #0x41000000
00775738: add      r1, r1, #0xa00000
0077573c: bl       #0x30ec94
00775740: mov      r1, r0
00775744: mov      r0, r7
00775748: bl       #0x30ec94
0077574c: mov      sl, r0
00775750: mov      r0, fp
00775754: bl       #0x30e964
00775758: ldr      r1, [r8, #0xb4]
0077575c: mov      r7, r0
00775760: ldr      r0, [r8, #0xb8]
00775764: bl       #0x30e3ac
00775768: mov      r1, #0x41000000
0077576c: add      r1, r1, #0xa00000
00775770: bl       #0x30ec94
00775774: mov      r1, r0
00775778: mov      r0, r7
0077577c: bl       #0x30ec94
00775780: mov      r7, r0
00775784: mov      r1, r7
00775788: mov      r0, sl
0077578c: bl       #0x30e70c
00775790: ldr      r3, [r4, #0xcc]
00775794: cmp      r0, #0
00775798: moveq    r7, sl
0077579c: cmp      r3, #0
007757a0: str      r7, [r4, #0x34]
007757a4: beq      #0x775a9c
007757a8: ldr      r0, [r4, #0xc8]
007757ac: ldrb     r3, [r0, #4]
007757b0: cmp      r3, #0
007757b4: beq      #0x775c74
007757b8: mov      r3, #0
007757bc: ldr      r0, [r4, #0x18]
007757c0: str      r3, [sp, #0x64]
007757c4: str      r3, [sp, #0x60]
007757c8: bl       #0x30e964
007757cc: mov      r7, r0
007757d0: ldr      r0, [r4, #0x20]
007757d4: bl       #0x30e964
007757d8: mov      r1, r0
007757dc: mov      r0, r7
007757e0: bl       #0x30eba4
007757e4: mov      r7, r0
007757e8: ldr      r0, [r4, #0x14]
007757ec: bl       #0x30e964
007757f0: mov      r8, r0
007757f4: ldr      r0, [r4, #0x1c]
007757f8: bl       #0x30e964
007757fc: mov      r1, r0
00775800: mov      r0, r8
00775804: bl       #0x30eba4
00775808: add      r1, sp, #0x60
0077580c: str      r0, [sp, #0x58]
00775810: mov      r0, r4
00775814: str      r7, [sp, #0x5c]
00775818: bl       #0x773dc0
0077581c: mov      r0, r4
00775820: add      r1, sp, #0x58
00775824: bl       #0x773dc0
00775828: ldr      r8, [r4, #0xcc]
0077582c: cmp      r8, #0
00775830: beq      #0x775844
00775834: ldr      r0, [r4, #0xc8]
00775838: ldrb     r3, [r0, #4]
0077583c: cmp      r3, #0
00775840: beq      #0x775cec
00775844: mov      r1, #0
00775848: mov      r0, #0x38
0077584c: bl       #0x752ba8
00775850: mov      r1, r8
00775854: mov      r7, r0
00775858: bl       #0x76b820
0077585c: ldr      r1, [pc, #0x4c0]
00775860: ldr      r3, [r7]
00775864: add      sb, sp, #0xb8
00775868: add      r1, pc, r1
0077586c: mov      r0, sb
00775870: ldr      sl, [r3, #0x1c]
00775874: bl       #0x413a7c
00775878: mov      r3, #0
0077587c: ldr      r0, [sp, #0x60]
00775880: strb     r3, [sp, #0x44]
00775884: mov      r3, #2
00775888: strb     r3, [sp, #0x45]
0077588c: bl       #0x30e8a4
00775890: strd     r0, r1, [sp, #0x50]
00775894: ldr      r3, [sp, #0x50]
00775898: add      r8, sp, #0x44
0077589c: mov      r1, sb
007758a0: str      r3, [sp, #0x48]
007758a4: ldr      r3, [sp, #0x54]
007758a8: mov      r2, r8
007758ac: mov      r0, r7
007758b0: str      r3, [r8, #8]
007758b4: blx      sl
007758b8: mov      r0, r8
007758bc: bl       #0x797124
007758c0: ldrsb    r3, [sp, #0xb8]
007758c4: cmn      r3, #1
007758c8: beq      #0x775cac
007758cc: ldr      r1, [pc, #0x454]
007758d0: ldr      r3, [r7]
007758d4: add      sb, sp, #0xa4
007758d8: add      r1, pc, r1
007758dc: mov      r0, sb
007758e0: ldr      sl, [r3, #0x1c]
007758e4: bl       #0x413a7c
007758e8: mov      r3, #0
007758ec: ldr      r0, [sp, #0x64]
007758f0: strb     r3, [sp, #0x38]
007758f4: mov      r3, #2
007758f8: strb     r3, [sp, #0x39]
007758fc: bl       #0x30e8a4
00775900: strd     r0, r1, [sp, #0x50]
00775904: ldr      r3, [sp, #0x50]
00775908: add      r8, sp, #0x38
0077590c: mov      r1, sb
00775910: str      r3, [sp, #0x3c]
00775914: ldr      r3, [sp, #0x54]
00775918: mov      r2, r8
0077591c: mov      r0, r7
00775920: str      r3, [r8, #8]
00775924: blx      sl
00775928: mov      r0, r8
0077592c: bl       #0x797124
00775930: ldrsb    r3, [sp, #0xa4]
00775934: cmn      r3, #1
00775938: beq      #0x775c9c
0077593c: ldr      r1, [pc, #0x3e8]
00775940: ldr      r3, [r7]
00775944: add      sb, sp, #0x90
00775948: add      r1, pc, r1
0077594c: mov      r0, sb
00775950: ldr      sl, [r3, #0x1c]
00775954: bl       #0x413a7c
00775958: mov      r3, #0
0077595c: ldr      r0, [sp, #0x58]
00775960: strb     r3, [sp, #0x2c]
00775964: mov      r3, #2
00775968: strb     r3, [sp, #0x2d]
0077596c: bl       #0x30e8a4
00775970: strd     r0, r1, [sp, #0x50]
00775974: ldr      r3, [sp, #0x50]
00775978: add      r8, sp, #0x2c
0077597c: mov      r1, sb
00775980: str      r3, [sp, #0x30]
00775984: ldr      r3, [sp, #0x54]
00775988: mov      r2, r8
0077598c: mov      r0, r7
00775990: str      r3, [r8, #8]
00775994: blx      sl
00775998: mov      r0, r8
0077599c: bl       #0x797124
007759a0: ldrsb    r3, [sp, #0x90]
007759a4: cmn      r3, #1
007759a8: beq      #0x775cdc
007759ac: ldr      r1, [pc, #0x37c]
007759b0: ldr      r3, [r7]
007759b4: add      sb, sp, #0x7c
007759b8: add      r1, pc, r1
007759bc: mov      r0, sb
007759c0: ldr      sl, [r3, #0x1c]
007759c4: bl       #0x413a7c
007759c8: mov      r3, #0
007759cc: ldr      r0, [sp, #0x5c]
007759d0: strb     r3, [sp, #0x20]
007759d4: mov      r3, #2
007759d8: strb     r3, [sp, #0x21]
007759dc: bl       #0x30e8a4
007759e0: strd     r0, r1, [sp, #0x50]
007759e4: ldr      r3, [sp, #0x50]
007759e8: add      r8, sp, #0x20
007759ec: mov      r1, sb
007759f0: str      r3, [sp, #0x24]
007759f4: ldr      r3, [sp, #0x54]
007759f8: mov      r2, r8
007759fc: mov      r0, r7
00775a00: str      r3, [r8, #8]
00775a04: blx      sl
00775a08: mov      r0, r8
00775a0c: bl       #0x797124
00775a10: ldrsb    r3, [sp, #0x7c]
00775a14: cmn      r3, #1
00775a18: beq      #0x775ccc
00775a1c: mov      r3, #0
00775a20: strb     r3, [sp, #0x14]
00775a24: mov      r0, r7
00775a28: mov      r3, #5
00775a2c: strb     r3, [sp, #0x15]
00775a30: str      r7, [sp, #0x18]
00775a34: bl       #0x759c64
00775a38: ldr      r3, [r4, #0xcc]
00775a3c: cmp      r3, #0
00775a40: beq      #0x775a54
00775a44: ldr      r0, [r4, #0xc8]
00775a48: ldrb     r2, [r0, #4]
00775a4c: cmp      r2, #0
00775a50: beq      #0x775c4c
00775a54: ldr      sl, [r3, #0x34]
00775a58: ldr      r1, [pc, #0x2d4]
00775a5c: add      r8, sp, #0x68
00775a60: ldr      r3, [sl]
00775a64: add      r1, pc, r1
00775a68: mov      r0, r8
00775a6c: add      r4, sp, #0x14
00775a70: ldr      r7, [r3, #0x1c]
00775a74: bl       #0x413a7c
00775a78: mov      r0, sl
00775a7c: mov      r1, r8
00775a80: mov      r2, r4
00775a84: blx      r7
00775a88: ldrsb    r3, [sp, #0x68]
00775a8c: cmn      r3, #1
00775a90: beq      #0x775cbc
00775a94: mov      r0, r4
00775a98: bl       #0x797124
00775a9c: ldr      r3, [r5, r6]
00775aa0: ldr      r2, [sp, #0xcc]
00775aa4: ldr      r3, [r3]
00775aa8: cmp      r2, r3
00775aac: bne      #0x775d14
00775ab0: add      sp, sp, #0xd4
00775ab4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00775ab8: mov      r0, sb
00775abc: bl       #0x30e964
00775ac0: ldr      r1, [r8, #0xb4]
00775ac4: mov      r7, r0
00775ac8: ldr      r0, [r8, #0xb8]
00775acc: bl       #0x30e3ac
00775ad0: mov      r1, #0x41000000
00775ad4: add      r1, r1, #0xa00000
00775ad8: bl       #0x30ec94
00775adc: mov      r1, r0
00775ae0: mov      r0, r7
00775ae4: bl       #0x30ec94
00775ae8: mov      sl, r0
00775aec: mov      r0, fp
00775af0: bl       #0x30e964
00775af4: ldr      r1, [r8, #0xbc]
00775af8: mov      r7, r0
00775afc: ldr      r0, [r8, #0xc0]
00775b00: b        #0x775764
00775b04: mov      r1, #0x3f800000
00775b08: bl       #0x30e4b4
00775b0c: cmp      r0, #0
00775b10: beq      #0x775b7c
00775b14: mov      r0, fp
00775b18: bl       #0x30e964
00775b1c: ldr      r1, [sp, #8]
00775b20: bl       #0x30ec94
00775b24: bl       #0x30e4cc
00775b28: rsb      r0, fp, r0
00775b2c: add      r3, r0, r0, lsr #31
00775b30: add      fp, fp, r0
00775b34: sub      sl, sl, r3, asr #1
00775b38: ldr      r3, [r4, #0x24]
00775b3c: cmp      r3, r7
00775b40: bne      #0x775700
00775b44: ldr      r3, [r4, #0x28]
00775b48: cmp      r3, sl
00775b4c: bne      #0x775700
00775b50: ldr      r3, [r4, #0x2c]
00775b54: cmp      r3, sb
00775b58: bne      #0x775700
00775b5c: ldr      r3, [r4, #0x30]
00775b60: cmp      r3, fp
00775b64: bne      #0x775700
00775b68: b        #0x775a9c
00775b6c: mov      r1, #0x3f800000
00775b70: bl       #0x30e4b4
00775b74: cmp      r0, #0
00775b78: beq      #0x775b14
00775b7c: mov      r0, sb
00775b80: bl       #0x30e964
00775b84: ldr      r1, [sp, #8]
00775b88: bl       #0x30ed6c
00775b8c: bl       #0x30e4cc
00775b90: rsb      r0, sb, r0
00775b94: add      r3, r0, r0, lsr #31
00775b98: add      sb, sb, r0
00775b9c: sub      r7, r7, r3, asr #1
00775ba0: b        #0x7756f4
00775ba4: ldr      r3, [r8]
00775ba8: mov      r0, r3
00775bac: ldr      r3, [r3]
00775bb0: mov      lr, pc
00775bb4: ldr      pc, [r3, #0xac]
00775bb8: cmp      r0, #2
00775bbc: beq      #0x77564c
00775bc0: mov      r0, sb
00775bc4: bl       #0x30e964
00775bc8: ldr      r8, [r4, #0xc]
00775bcc: mov      r3, r0
00775bd0: ldr      r1, [r8, #0xbc]
00775bd4: ldr      r0, [r8, #0xc0]
00775bd8: str      r3, [sp]
00775bdc: bl       #0x30e3ac
00775be0: mov      r1, #0x41000000
00775be4: add      r1, r1, #0xa00000
00775be8: bl       #0x30ec94
00775bec: ldr      r3, [sp]
00775bf0: mov      r1, r0
00775bf4: mov      r0, r3
00775bf8: bl       #0x30ec94
00775bfc: mov      r3, r0
00775c00: mov      r0, fp
00775c04: str      r3, [sp]
00775c08: bl       #0x30e964
00775c0c: ldr      r1, [r8, #0xb4]
00775c10: mov      r2, r0
00775c14: ldr      r0, [r8, #0xb8]
00775c18: str      r2, [sp, #4]
00775c1c: bl       #0x30e3ac
00775c20: mov      r1, #0x41000000
00775c24: add      r1, r1, #0xa00000
00775c28: bl       #0x30ec94
00775c2c: ldr      r2, [sp, #4]
00775c30: mov      r1, r0
00775c34: mov      r0, r2
00775c38: bl       #0x30ec94
00775c3c: mov      r2, #0
00775c40: str      r2, [sp, #0xc]
00775c44: ldr      r3, [sp]
00775c48: b        #0x7756d4
00775c4c: ldr      r1, [r0]
00775c50: sub      r1, r1, #1
00775c54: cmp      r1, #0
00775c58: str      r1, [r0]
00775c5c: bne      #0x775c64
00775c60: bl       #0x752b38
00775c64: mov      r3, #0
00775c68: str      r3, [r4, #0xcc]
00775c6c: str      r3, [r4, #0xc8]
00775c70: b        #0x775a54
00775c74: ldr      r1, [r0]
00775c78: sub      r1, r1, #1
00775c7c: cmp      r1, #0
00775c80: str      r1, [r0]
00775c84: bne      #0x775c8c
00775c88: bl       #0x752b38
00775c8c: mov      r3, #0
00775c90: str      r3, [r4, #0xcc]
00775c94: str      r3, [r4, #0xc8]
00775c98: b        #0x775a9c
00775c9c: ldr      r0, [sp, #0xb0]
00775ca0: ldr      r1, [sp, #0xac]
00775ca4: bl       #0x752b38
00775ca8: b        #0x77593c
00775cac: ldr      r0, [sp, #0xc4]
00775cb0: ldr      r1, [sp, #0xc0]
00775cb4: bl       #0x752b38
00775cb8: b        #0x7758cc
00775cbc: ldr      r0, [sp, #0x74]
00775cc0: ldr      r1, [sp, #0x70]
00775cc4: bl       #0x752b38
00775cc8: b        #0x775a94
00775ccc: ldr      r0, [sp, #0x88]
00775cd0: ldr      r1, [sp, #0x84]
00775cd4: bl       #0x752b38
00775cd8: b        #0x775a1c
00775cdc: ldr      r0, [sp, #0x9c]
00775ce0: ldr      r1, [sp, #0x98]
00775ce4: bl       #0x752b38
00775ce8: b        #0x7759ac
00775cec: ldr      r1, [r0]
00775cf0: sub      r1, r1, #1
00775cf4: cmp      r1, #0
00775cf8: str      r1, [r0]
00775cfc: bne      #0x775d04
00775d00: bl       #0x752b38
00775d04: mov      r8, #0
00775d08: str      r8, [r4, #0xc8]
00775d0c: str      r8, [r4, #0xcc]
00775d10: b        #0x775844
00775d14: bl       #0x30e310
00775d18: eoreq    pc, r1, ip, lsl #9
00775d1c: andeq    r4, r0, ip, lsr #1
00775d20: strheq   r3, [r0], -r4
00775d24: andseq   r4, sb, r0, lsl #1
00775d28: andseq   r4, sb, r8, lsl r0
00775d2c: ldrheq   r3, [sb], -r0
00775d30: andseq   r3, sb, r8, asr #30
00775d34: andseq   sp, r6, ip, lsr #16

# _ZNK7gameswf4root15get_pixel_scaleEv
007741bc: ldr      r0, [r0, #0x34]
007741c0: bx       lr

# _ZN8RenderFX10SetContextEPN7gameswf9characterE
007a7ee8: str      r1, [r0, #0x40]
007a7eec: bx       lr

# _ZN12GameSWFUtils14GetPixelScaleXEPN7gameswf4rootE
004164c0: push     {r4, r5, r6, lr}
004164c4: ldr      r4, [r0, #0xc]
004164c8: ldr      r0, [r0, #0x2c]
004164cc: bl       #0x30e964
004164d0: ldr      r1, [r4, #0xb4]
004164d4: mov      r5, r0
004164d8: ldr      r0, [r4, #0xb8]
004164dc: bl       #0x30e3ac
004164e0: mov      r1, #0x41000000
004164e4: add      r1, r1, #0xa00000
004164e8: bl       #0x30ec94
004164ec: mov      r1, r0
004164f0: mov      r0, r5
004164f4: bl       #0x30ec94
004164f8: pop      {r4, r5, r6, pc}

# _ZN12GameSWFUtils14GetPixelScaleYEPN7gameswf4rootE
004164fc: push     {r4, r5, r6, lr}
00416500: ldr      r4, [r0, #0xc]
00416504: ldr      r0, [r0, #0x30]
00416508: bl       #0x30e964
0041650c: ldr      r1, [r4, #0xbc]
00416510: mov      r5, r0
00416514: ldr      r0, [r4, #0xc0]
00416518: bl       #0x30e3ac
0041651c: mov      r1, #0x41000000
00416520: add      r1, r1, #0xa00000
00416524: bl       #0x30ec94
00416528: mov      r1, r0
0041652c: mov      r0, r5
00416530: bl       #0x30ec94
00416534: pop      {r4, r5, r6, pc}

# _ZN12GameSWFUtils17GetInvPixelScaleXEPN7gameswf4rootE
00416538: push     {r4, r5, r6, lr}
0041653c: ldr      r3, [r0, #0xc]
00416540: mov      r4, r0
00416544: ldr      r1, [r3, #0xb4]
00416548: ldr      r0, [r3, #0xb8]
0041654c: bl       #0x30e3ac
00416550: mov      r1, #0x41000000
00416554: add      r1, r1, #0xa00000
00416558: bl       #0x30ec94
0041655c: mov      r5, r0
00416560: ldr      r0, [r4, #0x2c]
00416564: bl       #0x30e964
00416568: mov      r1, r0
0041656c: mov      r0, r5
00416570: bl       #0x30ec94
00416574: pop      {r4, r5, r6, pc}

# _ZN12GameSWFUtils17GetInvPixelScaleYEPN7gameswf4rootE
00416578: push     {r4, r5, r6, lr}
0041657c: ldr      r3, [r0, #0xc]
00416580: mov      r4, r0
00416584: ldr      r1, [r3, #0xbc]
00416588: ldr      r0, [r3, #0xc0]
0041658c: bl       #0x30e3ac
00416590: mov      r1, #0x41000000
00416594: add      r1, r1, #0xa00000
00416598: bl       #0x30ec94
0041659c: mov      r5, r0
004165a0: ldr      r0, [r4, #0x30]
004165a4: bl       #0x30e964
004165a8: mov      r1, r0
004165ac: mov      r0, r5
004165b0: bl       #0x30ec94
004165b4: pop      {r4, r5, r6, pc}

# _ZN8RenderFX7SetTextEPN7gameswf9characterEPKcb
007a92e0: push     {r4, r5, r6, r7, r8, sl, lr}
007a92e4: ldr      r4, [pc, #0x9c]
007a92e8: ldr      r5, [pc, #0x9c]
007a92ec: subs     r6, r1, #0
007a92f0: add      r4, pc, r4
007a92f4: ldr      r1, [r4, r5]
007a92f8: mov      r8, r3
007a92fc: sub      sp, sp, #0x1c
007a9300: ldr      r3, [r1]
007a9304: mov      r7, r2
007a9308: str      r3, [sp, #0x14]
007a930c: beq      #0x7a932c
007a9310: ldr      ip, [r6]
007a9314: mov      r0, r6
007a9318: mov      r1, #0x20
007a931c: mov      lr, pc
007a9320: ldr      pc, [ip, #8]
007a9324: cmp      r0, #0
007a9328: bne      #0x7a9348
007a932c: ldr      r3, [r4, r5]
007a9330: ldr      r2, [sp, #0x14]
007a9334: ldr      r3, [r3]
007a9338: cmp      r2, r3
007a933c: bne      #0x7a9384
007a9340: add      sp, sp, #0x1c
007a9344: pop      {r4, r5, r6, r7, r8, sl, pc}
007a9348: mov      r1, r7
007a934c: mov      r0, sp
007a9350: bl       #0x413a7c
007a9354: mov      r0, r6
007a9358: mov      r1, sp
007a935c: mov      r2, r8
007a9360: bl       #0x790ab0
007a9364: ldrsb    r3, [sp]
007a9368: mov      sl, sp
007a936c: cmn      r3, #1
007a9370: bne      #0x7a932c
007a9374: ldr      r0, [sp, #0xc]
007a9378: ldr      r1, [sp, #8]
007a937c: bl       #0x752b38
007a9380: b        #0x7a932c
007a9384: bl       #0x30e310
007a9388: andseq   fp, lr, r0, lsr #15
007a938c: andeq    r4, r0, ip, lsr #1

# _ZN8RenderFX7SetTextEPKcS1_b
007a9390: push     {r4, r5, r6, lr}
007a9394: mov      r5, r2
007a9398: mov      r4, r3
007a939c: mov      r6, r0
007a93a0: bl       #0x7a9160
007a93a4: mov      r2, r5
007a93a8: mov      r1, r0
007a93ac: mov      r3, r4
007a93b0: mov      r0, r6
007a93b4: pop      {r4, r5, r6, lr}
007a93b8: b        #0x7a92e0

# _ZN8RenderFX13PreloadGlyphsEPN7gameswf9characterE
007a95a8: push     {r4, r5, r6, lr}
007a95ac: cmp      r1, #0
007a95b0: ldreq    r3, [r0, #0x3c]
007a95b4: mov      r2, #0
007a95b8: ldreq    r1, [r3, #0x10]
007a95bc: mov      r3, r2
007a95c0: bl       #0x7a8c08
007a95c4: ldr      r3, [r0, #4]
007a95c8: mov      r5, r0
007a95cc: cmp      r3, #0
007a95d0: ble      #0x7a962c
007a95d4: mov      r4, #0
007a95d8: b        #0x7a95ec
007a95dc: ldr      r3, [r5, #4]
007a95e0: add      r4, r4, #1
007a95e4: cmp      r4, r3
007a95e8: bge      #0x7a962c
007a95ec: ldr      r3, [r5]
007a95f0: mov      r1, #0x20
007a95f4: ldr      r3, [r3, r4, lsl #2]
007a95f8: mov      r0, r3
007a95fc: ldr      r3, [r3]
007a9600: mov      lr, pc
007a9604: ldr      pc, [r3, #8]
007a9608: cmp      r0, #0
007a960c: beq      #0x7a95dc
007a9610: ldr      r3, [r5]
007a9614: ldr      r0, [r3, r4, lsl #2]
007a9618: bl       #0x78c2d0
007a961c: ldr      r3, [r5, #4]
007a9620: add      r4, r4, #1
007a9624: cmp      r4, r3
007a9628: blt      #0x7a95ec
007a962c: mov      r0, #1
007a9630: pop      {r4, r5, r6, pc}

# _ZN8RenderFX13PreloadGlyphsEPtiPKcibbPKN7gameswf6filterE
007aafac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007aafb0: ldr      r5, [pc, #0xf8]
007aafb4: ldr      r6, [pc, #0xf8]
007aafb8: mov      r7, r0
007aafbc: add      r5, pc, r5
007aafc0: ldr      r0, [r5, r6]
007aafc4: sub      sp, sp, #0x34
007aafc8: mov      sl, r3
007aafcc: ldr      ip, [r0]
007aafd0: ldr      r3, [sp, #0x64]
007aafd4: str      r1, [sp, #0xc]
007aafd8: mov      r0, #0x88
007aafdc: mov      r1, #0
007aafe0: ldr      r8, [r7, #0x38]
007aafe4: ldrb     fp, [sp, #0x5c]
007aafe8: str      ip, [sp, #0x2c]
007aafec: str      r2, [sp, #0x10]
007aaff0: str      r3, [sp, #0x14]
007aaff4: ldrb     sb, [sp, #0x60]
007aaff8: bl       #0x752ba8
007aaffc: mov      r4, r0
007ab000: mov      r1, r8
007ab004: bl       #0x7cf8d8
007ab008: cmp      r4, #0
007ab00c: beq      #0x7ab018
007ab010: mov      r0, r4
007ab014: bl       #0x759c64
007ab018: add      r8, sp, #0x18
007ab01c: mov      r1, sl
007ab020: strb     fp, [r4, #0x4d]
007ab024: strb     sb, [r4, #0x4c]
007ab028: mov      r0, r8
007ab02c: bl       #0x413a7c
007ab030: mov      r1, r8
007ab034: add      r0, r4, #0x30
007ab038: bl       #0x752f50
007ab03c: ldrsb    r3, [sp, #0x18]
007ab040: cmn      r3, #1
007ab044: beq      #0x7ab09c
007ab048: ldr      r3, [r7, #0x38]
007ab04c: ldr      ip, [sp, #0x58]
007ab050: ldr      r2, [sp, #0x10]
007ab054: ldr      r0, [r3, #0xac]
007ab058: str      ip, [sp]
007ab05c: ldr      ip, [sp, #0x14]
007ab060: mov      r3, r4
007ab064: ldr      r1, [sp, #0xc]
007ab068: str      ip, [sp, #4]
007ab06c: bl       #0x78b240
007ab070: mov      r7, r0
007ab074: mov      r0, r4
007ab078: bl       #0x75a240
007ab07c: ldr      r3, [r5, r6]
007ab080: ldr      r2, [sp, #0x2c]
007ab084: mov      r0, r7
007ab088: ldr      r3, [r3]
007ab08c: cmp      r2, r3
007ab090: bne      #0x7ab0ac
007ab094: add      sp, sp, #0x34
007ab098: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ab09c: ldr      r0, [sp, #0x24]
007ab0a0: ldr      r1, [sp, #0x20]
007ab0a4: bl       #0x752b38
007ab0a8: b        #0x7ab048
007ab0ac: bl       #0x30e310
007ab0b0: ldrsbeq  sb, [lr], -r4
007ab0b4: andeq    r4, r0, ip, lsr #1

# _ZN8RenderFX13PreloadGlyphsEPKcS1_ibbPKN7gameswf6filterE
007ab0b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ab0bc: sub      sp, sp, #0x2c
007ab0c0: ldrb     ip, [sp, #0x54]
007ab0c4: ldrb     r8, [sp, #0x50]
007ab0c8: str      r1, [sp, #0x14]
007ab0cc: str      ip, [sp, #0x10]
007ab0d0: mov      ip, #0
007ab0d4: strb     ip, [sp, #0x24]
007ab0d8: mov      r7, r0
007ab0dc: mov      r6, r2
007ab0e0: mov      r5, r3
007ab0e4: str      ip, [sp, #0x18]
007ab0e8: str      ip, [sp, #0x1c]
007ab0ec: str      ip, [sp, #0x20]
007ab0f0: add      fp, sp, #0x14
007ab0f4: add      r4, sp, #0x18
007ab0f8: b        #0x7ab10c
007ab0fc: ldr      r2, [sp, #0x18]
007ab100: lsl      r3, r3, #1
007ab104: strh     sb, [r2, r3]
007ab108: str      sl, [sp, #0x1c]
007ab10c: mov      r0, fp
007ab110: bl       #0x752494
007ab114: subs     sb, r0, #0
007ab118: beq      #0x7ab144
007ab11c: ldr      r3, [sp, #0x1c]
007ab120: ldr      r2, [sp, #0x20]
007ab124: add      sl, r3, #1
007ab128: cmp      sl, r2
007ab12c: ble      #0x7ab0fc
007ab130: mov      r0, r4
007ab134: add      r1, sl, sl, asr #1
007ab138: bl       #0x779e7c
007ab13c: ldr      r3, [sp, #0x1c]
007ab140: b        #0x7ab0fc
007ab144: ldr      r2, [sp, #0x1c]
007ab148: cmp      r2, #0
007ab14c: ble      #0x7ab1a4
007ab150: ldr      ip, [sp, #0x10]
007ab154: mov      r0, r7
007ab158: ldr      r1, [sp, #0x18]
007ab15c: str      ip, [sp, #8]
007ab160: ldr      ip, [sp, #0x58]
007ab164: mov      r3, r6
007ab168: stm      sp, {r5, r8}
007ab16c: str      ip, [sp, #0xc]
007ab170: bl       #0x7aafac
007ab174: ldr      r2, [sp, #0x1c]
007ab178: mov      sb, r0
007ab17c: cmp      r2, #0
007ab180: ble      #0x7ab1a4
007ab184: mov      r3, #0
007ab188: mov      r0, r4
007ab18c: mov      r1, r3
007ab190: str      r3, [sp, #0x1c]
007ab194: bl       #0x779e7c
007ab198: mov      r0, sb
007ab19c: add      sp, sp, #0x2c
007ab1a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ab1a4: cmp      r2, #0
007ab1a8: bge      #0x7ab184
007ab1ac: lsl      r3, r2, #1
007ab1b0: ldr      r1, [sp, #0x18]
007ab1b4: mov      r0, #0
007ab1b8: adds     r2, r2, #1
007ab1bc: strh     r0, [r1, r3]
007ab1c0: add      r3, r3, #2
007ab1c4: bne      #0x7ab1b0
007ab1c8: b        #0x7ab184

