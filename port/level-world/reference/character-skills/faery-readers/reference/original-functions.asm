
# _ZN6Arrays10FaeryTable4readEP11IStreamBase
004bbfe0: push     {r4, r5, r6, r7, r8, sl, lr}
004bbfe4: sub      sp, sp, #0xc
004bbfe8: mov      sl, r0
004bbfec: bl       #0x313a90
004bbff0: ldr      r6, [pc, #0x124]
004bbff4: mov      r3, #1
004bbff8: cmp      r3, #0
004bbffc: str      r0, [sp, #4]
004bc000: str      r3, [sp]
004bc004: add      r6, pc, r6
004bc008: bne      #0x4bc050
004bc00c: add      r3, sp, #4
004bc010: add      r2, r3, #2
004bc014: add      r3, r3, #1
004bc018: ldrb     r0, [r2, #1]
004bc01c: ldrb     r1, [r3, #-1]
004bc020: cmp      r2, r3
004bc024: eor      r1, r0, r1
004bc028: strb     r1, [r3, #-1]
004bc02c: ldrb     r0, [r2, #1]
004bc030: eor      r1, r1, r0
004bc034: strb     r1, [r2, #1]
004bc038: ldrb     r0, [r3, #-1]
004bc03c: sub      r2, r2, #1
004bc040: eor      r1, r1, r0
004bc044: strb     r1, [r3, #-1]
004bc048: add      r3, r3, #1
004bc04c: bhi      #0x4bc018
004bc050: bl       #0x4a8350
004bc054: ldr      r7, [pc, #0xc4]
004bc058: ldr      r4, [sp, #4]
004bc05c: mov      r5, #0x24
004bc060: ldr      r3, [r6, r7]
004bc064: mul      r0, r5, r4
004bc068: str      r4, [r3]
004bc06c: add      r0, r0, #8
004bc070: mov      r1, #1
004bc074: bl       #0x31056c
004bc078: cmp      r4, #0
004bc07c: str      r5, [r0]
004bc080: str      r4, [r0, #4]
004bc084: add      r3, r0, #8
004bc088: beq      #0x4bc0b8
004bc08c: ldr      r1, [pc, #0x90]
004bc090: mov      r2, #0
004bc094: mov      ip, r2
004bc098: ldr      r1, [r6, r1]
004bc09c: add      r1, r1, #8
004bc0a0: add      r2, r2, #1
004bc0a4: cmp      r2, r4
004bc0a8: str      r1, [r0, #8]
004bc0ac: str      ip, [r0, #0x20]
004bc0b0: add      r0, r0, #0x24
004bc0b4: bne      #0x4bc0a0
004bc0b8: ldr      r2, [r6, r7]
004bc0bc: ldr      r8, [pc, #0x64]
004bc0c0: ldr      r1, [r2]
004bc0c4: ldr      r2, [r6, r8]
004bc0c8: cmp      r1, #0
004bc0cc: str      r3, [r2]
004bc0d0: beq      #0x4bc114
004bc0d4: mov      r4, #0
004bc0d8: mov      r5, r4
004bc0dc: b        #0x4bc0e8
004bc0e0: ldr      r3, [r6, r8]
004bc0e4: ldr      r3, [r3]
004bc0e8: add      r0, r3, r4
004bc0ec: mov      r1, sl
004bc0f0: ldr      r3, [r3, r4]
004bc0f4: mov      lr, pc
004bc0f8: ldr      pc, [r3, #0xc]
004bc0fc: ldr      r3, [r6, r7]
004bc100: add      r5, r5, #1
004bc104: add      r4, r4, #0x24
004bc108: ldr      r3, [r3]
004bc10c: cmp      r3, r5
004bc110: bhi      #0x4bc0e0
004bc114: add      sp, sp, #0xc
004bc118: pop      {r4, r5, r6, r7, r8, sl, pc}
004bc11c: subeq    r8, sp, ip, lsl #21
004bc120: ldrdeq   r3, r4, [r0], -r4
004bc124: andeq    r2, r0, r8, lsr #16
004bc128: strdeq   r0, r1, [r0], -r8

# _ZN6Arrays14FaeryListTable4readEP11IStreamBase
004bc12c: push     {r4, r5, r6, r7, r8, sl, lr}
004bc130: sub      sp, sp, #0xc
004bc134: mov      sl, r0
004bc138: bl       #0x313a90
004bc13c: ldr      r6, [pc, #0x124]
004bc140: mov      r3, #1
004bc144: cmp      r3, #0
004bc148: str      r0, [sp, #4]
004bc14c: str      r3, [sp]
004bc150: add      r6, pc, r6
004bc154: bne      #0x4bc19c
004bc158: add      r3, sp, #4
004bc15c: add      r2, r3, #2
004bc160: add      r3, r3, #1
004bc164: ldrb     r0, [r2, #1]
004bc168: ldrb     r1, [r3, #-1]
004bc16c: cmp      r2, r3
004bc170: eor      r1, r0, r1
004bc174: strb     r1, [r3, #-1]
004bc178: ldrb     r0, [r2, #1]
004bc17c: eor      r1, r1, r0
004bc180: strb     r1, [r2, #1]
004bc184: ldrb     r0, [r3, #-1]
004bc188: sub      r2, r2, #1
004bc18c: eor      r1, r1, r0
004bc190: strb     r1, [r3, #-1]
004bc194: add      r3, r3, #1
004bc198: bhi      #0x4bc164
004bc19c: bl       #0x4a84d0
004bc1a0: ldr      r7, [pc, #0xc4]
004bc1a4: ldr      r4, [sp, #4]
004bc1a8: mov      r5, #0xc
004bc1ac: ldr      r3, [r6, r7]
004bc1b0: mul      r0, r5, r4
004bc1b4: str      r4, [r3]
004bc1b8: add      r0, r0, #8
004bc1bc: mov      r1, #1
004bc1c0: bl       #0x31056c
004bc1c4: cmp      r4, #0
004bc1c8: str      r5, [r0]
004bc1cc: str      r4, [r0, #4]
004bc1d0: add      r3, r0, #8
004bc1d4: beq      #0x4bc204
004bc1d8: ldr      r1, [pc, #0x90]
004bc1dc: mov      r2, #0
004bc1e0: mov      ip, r2
004bc1e4: ldr      r1, [r6, r1]
004bc1e8: add      r1, r1, #8
004bc1ec: add      r2, r2, #1
004bc1f0: cmp      r2, r4
004bc1f4: str      r1, [r0, #8]
004bc1f8: str      ip, [r0, #0x10]
004bc1fc: add      r0, r0, #0xc
004bc200: bne      #0x4bc1ec
004bc204: ldr      r2, [r6, r7]
004bc208: ldr      r8, [pc, #0x64]
004bc20c: ldr      r1, [r2]
004bc210: ldr      r2, [r6, r8]
004bc214: cmp      r1, #0
004bc218: str      r3, [r2]
004bc21c: beq      #0x4bc260
004bc220: mov      r4, #0
004bc224: mov      r5, r4
004bc228: b        #0x4bc234
004bc22c: ldr      r3, [r6, r8]
004bc230: ldr      r3, [r3]
004bc234: add      r0, r3, r4
004bc238: mov      r1, sl
004bc23c: ldr      r3, [r3, r4]
004bc240: mov      lr, pc
004bc244: ldr      pc, [r3, #0xc]
004bc248: ldr      r3, [r6, r7]
004bc24c: add      r5, r5, #1
004bc250: add      r4, r4, #0xc
004bc254: ldr      r3, [r3]
004bc258: cmp      r3, r5
004bc25c: bhi      #0x4bc22c
004bc260: add      sp, sp, #0xc
004bc264: pop      {r4, r5, r6, r7, r8, sl, pc}
004bc268: subeq    r8, sp, r0, asr #18
004bc26c: andeq    r4, r0, r4, lsl r5
004bc270: andeq    r0, r0, r4, asr #20
004bc274: andeq    r3, r0, r4, asr lr

# _ZN7Structs5Faery4readEP11IStreamBase
0050637c: push     {r4, r5, r6, lr}
00506380: mov      r4, r0
00506384: sub      sp, sp, #8
00506388: mov      r0, r1
0050638c: mov      r5, r1
00506390: add      r1, r4, #4
00506394: bl       #0x459090
00506398: mov      r3, #1
0050639c: cmp      r3, #0
005063a0: str      r3, [sp, #4]
005063a4: bne      #0x5063e8
005063a8: add      r3, r4, #5
005063ac: add      r2, r4, #6
005063b0: ldrb     r0, [r2, #1]
005063b4: ldrb     r1, [r3, #-1]
005063b8: cmp      r3, r2
005063bc: eor      r1, r0, r1
005063c0: strb     r1, [r3, #-1]
005063c4: ldrb     r0, [r2, #1]
005063c8: eor      r1, r1, r0
005063cc: strb     r1, [r2, #1]
005063d0: ldrb     r0, [r3, #-1]
005063d4: sub      r2, r2, #1
005063d8: eor      r1, r1, r0
005063dc: strb     r1, [r3, #-1]
005063e0: add      r3, r3, #1
005063e4: blo      #0x5063b0
005063e8: mov      r0, r5
005063ec: add      r1, r4, #8
005063f0: bl       #0x459090
005063f4: mov      r3, #1
005063f8: cmp      r3, #0
005063fc: str      r3, [sp, #4]
00506400: bne      #0x506444
00506404: add      r3, r4, #9
00506408: add      r2, r4, #0xa
0050640c: ldrb     r0, [r2, #1]
00506410: ldrb     r1, [r3, #-1]
00506414: cmp      r3, r2
00506418: eor      r1, r0, r1
0050641c: strb     r1, [r3, #-1]
00506420: ldrb     r0, [r2, #1]
00506424: eor      r1, r1, r0
00506428: strb     r1, [r2, #1]
0050642c: ldrb     r0, [r3, #-1]
00506430: sub      r2, r2, #1
00506434: eor      r1, r1, r0
00506438: strb     r1, [r3, #-1]
0050643c: add      r3, r3, #1
00506440: blo      #0x50640c
00506444: mov      r0, r5
00506448: add      r1, r4, #0xc
0050644c: bl       #0x459090
00506450: mov      r3, #1
00506454: cmp      r3, #0
00506458: str      r3, [sp, #4]
0050645c: bne      #0x5064a0
00506460: add      r3, r4, #0xd
00506464: add      r2, r4, #0xe
00506468: ldrb     r0, [r2, #1]
0050646c: ldrb     r1, [r3, #-1]
00506470: cmp      r3, r2
00506474: eor      r1, r0, r1
00506478: strb     r1, [r3, #-1]
0050647c: ldrb     r0, [r2, #1]
00506480: eor      r1, r1, r0
00506484: strb     r1, [r2, #1]
00506488: ldrb     r0, [r3, #-1]
0050648c: sub      r2, r2, #1
00506490: eor      r1, r1, r0
00506494: strb     r1, [r3, #-1]
00506498: add      r3, r3, #1
0050649c: blo      #0x506468
005064a0: mov      r0, r5
005064a4: add      r1, r4, #0x10
005064a8: bl       #0x459090
005064ac: mov      r3, #1
005064b0: cmp      r3, #0
005064b4: str      r3, [sp, #4]
005064b8: bne      #0x5064fc
005064bc: add      r3, r4, #0x11
005064c0: add      r2, r4, #0x12
005064c4: ldrb     r0, [r2, #1]
005064c8: ldrb     r1, [r3, #-1]
005064cc: cmp      r3, r2
005064d0: eor      r1, r0, r1
005064d4: strb     r1, [r3, #-1]
005064d8: ldrb     r0, [r2, #1]
005064dc: eor      r1, r1, r0
005064e0: strb     r1, [r2, #1]
005064e4: ldrb     r0, [r3, #-1]
005064e8: sub      r2, r2, #1
005064ec: eor      r1, r1, r0
005064f0: strb     r1, [r3, #-1]
005064f4: add      r3, r3, #1
005064f8: blo      #0x5064c4
005064fc: mov      r0, r5
00506500: add      r1, r4, #0x14
00506504: bl       #0x3df1a0
00506508: mov      r3, #1
0050650c: cmp      r3, #0
00506510: str      r3, [sp, #4]
00506514: bne      #0x506558
00506518: add      r3, r4, #0x15
0050651c: add      r2, r4, #0x16
00506520: ldrb     r0, [r2, #1]
00506524: ldrb     r1, [r3, #-1]
00506528: cmp      r3, r2
0050652c: eor      r1, r0, r1
00506530: strb     r1, [r3, #-1]
00506534: ldrb     r0, [r2, #1]
00506538: eor      r1, r1, r0
0050653c: strb     r1, [r2, #1]
00506540: ldrb     r0, [r3, #-1]
00506544: sub      r2, r2, #1
00506548: eor      r1, r1, r0
0050654c: strb     r1, [r3, #-1]
00506550: add      r3, r3, #1
00506554: blo      #0x506520
00506558: ldr      r0, [r4, #0x18]
0050655c: cmp      r0, #0
00506560: beq      #0x506568
00506564: bl       #0x310440
00506568: ldr      r0, [r4, #0x14]
0050656c: mov      r1, #1
00506570: mov      r6, #0
00506574: add      r0, r0, r1
00506578: bl       #0x31056c
0050657c: ldr      r2, [r4, #0x14]
00506580: mov      r1, r0
00506584: str      r0, [r4, #0x18]
00506588: mov      r3, r6
0050658c: mov      r0, r5
00506590: bl       #0x317454
00506594: ldr      r3, [r4, #0x14]
00506598: ldr      r2, [r4, #0x18]
0050659c: mov      r0, r5
005065a0: add      r1, r4, #0x1c
005065a4: strb     r6, [r2, r3]
005065a8: bl       #0x459090
005065ac: mov      r3, #1
005065b0: cmp      r3, r6
005065b4: str      r3, [sp, #4]
005065b8: bne      #0x5065fc
005065bc: add      r3, r4, #0x1d
005065c0: add      r2, r4, #0x1e
005065c4: ldrb     r0, [r2, #1]
005065c8: ldrb     r1, [r3, #-1]
005065cc: cmp      r3, r2
005065d0: eor      r1, r0, r1
005065d4: strb     r1, [r3, #-1]
005065d8: ldrb     r0, [r2, #1]
005065dc: eor      r1, r1, r0
005065e0: strb     r1, [r2, #1]
005065e4: ldrb     r0, [r3, #-1]
005065e8: sub      r2, r2, #1
005065ec: eor      r1, r1, r0
005065f0: strb     r1, [r3, #-1]
005065f4: add      r3, r3, #1
005065f8: blo      #0x5065c4
005065fc: mov      r0, r5
00506600: add      r1, r4, #0x20
00506604: bl       #0x459090
00506608: mov      r3, #1
0050660c: cmp      r3, #0
00506610: str      r3, [sp, #4]
00506614: bne      #0x506658
00506618: add      r3, r4, #0x22
0050661c: add      r4, r4, #0x21
00506620: ldrb     r1, [r3, #1]
00506624: ldrb     r2, [r4, #-1]
00506628: cmp      r3, r4
0050662c: eor      r2, r1, r2
00506630: strb     r2, [r4, #-1]
00506634: ldrb     r1, [r3, #1]
00506638: eor      r2, r2, r1
0050663c: strb     r2, [r3, #1]
00506640: ldrb     r1, [r4, #-1]
00506644: sub      r3, r3, #1
00506648: eor      r2, r2, r1
0050664c: strb     r2, [r4, #-1]
00506650: add      r4, r4, #1
00506654: bhi      #0x506620
00506658: add      sp, sp, #8
0050665c: pop      {r4, r5, r6, pc}

# _ZN7Structs9FaeryList4readEP11IStreamBase
004eab9c: push     {r4, r5, r6, r7, r8, lr}
004eaba0: mov      r5, r0
004eaba4: sub      sp, sp, #8
004eaba8: mov      r0, r1
004eabac: mov      r8, r1
004eabb0: add      r1, r5, #4
004eabb4: bl       #0x3df1a0
004eabb8: mov      r3, #1
004eabbc: cmp      r3, #0
004eabc0: str      r3, [sp, #4]
004eabc4: bne      #0x4eac08
004eabc8: add      r3, r5, #5
004eabcc: add      r2, r5, #6
004eabd0: ldrb     r0, [r2, #1]
004eabd4: ldrb     r1, [r3, #-1]
004eabd8: cmp      r3, r2
004eabdc: eor      r1, r0, r1
004eabe0: strb     r1, [r3, #-1]
004eabe4: ldrb     r0, [r2, #1]
004eabe8: eor      r1, r1, r0
004eabec: strb     r1, [r2, #1]
004eabf0: ldrb     r0, [r3, #-1]
004eabf4: sub      r2, r2, #1
004eabf8: eor      r1, r1, r0
004eabfc: strb     r1, [r3, #-1]
004eac00: add      r3, r3, #1
004eac04: blo      #0x4eabd0
004eac08: ldr      r0, [r5, #8]
004eac0c: cmp      r0, #0
004eac10: beq      #0x4eac18
004eac14: bl       #0x310440
004eac18: ldr      r0, [r5, #4]
004eac1c: mov      r1, #1
004eac20: lsl      r0, r0, #2
004eac24: bl       #0x31056c
004eac28: ldr      r3, [r5, #4]
004eac2c: str      r0, [r5, #8]
004eac30: cmp      r3, #0
004eac34: beq      #0x4eacb8
004eac38: mov      r4, #0
004eac3c: mov      r7, #1
004eac40: lsl      r6, r4, #2
004eac44: add      r1, r0, r6
004eac48: mov      r0, r8
004eac4c: bl       #0x459090
004eac50: str      r7, [sp, #4]
004eac54: cmp      r7, #0
004eac58: ldr      r3, [r5, #8]
004eac5c: bne      #0x4eaca4
004eac60: add      r6, r3, r6
004eac64: add      r3, r6, #2
004eac68: add      r6, r6, #1
004eac6c: ldrb     r1, [r3, #1]
004eac70: ldrb     r2, [r6, #-1]
004eac74: cmp      r3, r6
004eac78: eor      r2, r1, r2
004eac7c: strb     r2, [r6, #-1]
004eac80: ldrb     r1, [r3, #1]
004eac84: eor      r2, r2, r1
004eac88: strb     r2, [r3, #1]
004eac8c: ldrb     r1, [r6, #-1]
004eac90: sub      r3, r3, #1
004eac94: eor      r2, r2, r1
004eac98: strb     r2, [r6, #-1]
004eac9c: add      r6, r6, #1
004eaca0: bhi      #0x4eac6c
004eaca4: ldr      r3, [r5, #4]
004eaca8: add      r4, r4, #1
004eacac: cmp      r3, r4
004eacb0: ldrhi    r0, [r5, #8]
004eacb4: bhi      #0x4eac40
004eacb8: add      sp, sp, #8
004eacbc: pop      {r4, r5, r6, r7, r8, pc}
