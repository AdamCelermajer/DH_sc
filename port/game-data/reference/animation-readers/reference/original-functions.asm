
# _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
004db89c: str      lr, [sp, #-4]!
004db8a0: mov      r3, #0
004db8a4: sub      sp, sp, #0xc
004db8a8: ldr      ip, [r0]
004db8ac: mov      r2, #1
004db8b0: mov      lr, pc
004db8b4: ldr      pc, [ip, #0x18]
004db8b8: ldr      r3, [pc, #0x74]
004db8bc: cmp      r0, #1
004db8c0: add      r3, pc, r3
004db8c4: beq      #0x4db8f4
004db8c8: ldr      r2, [pc, #0x68]
004db8cc: ldr      r2, [r3, r2]
004db8d0: ldr      r2, [r2]
004db8d4: cmp      r2, #2
004db8d8: moveq    r3, #0
004db8dc: streq    r3, [r3]
004db8e0: beq      #0x4db8ec
004db8e4: cmp      r2, #1
004db8e8: beq      #0x4db900
004db8ec: add      sp, sp, #0xc
004db8f0: ldm      sp!, {pc}
004db8f4: cmp      r1, #0
004db8f8: beq      #0x4db8ec
004db8fc: b        #0x4db8c8
004db900: ldr      r0, [pc, #0x34]
004db904: ldr      r1, [pc, #0x34]
004db908: ldr      r2, [pc, #0x34]
004db90c: ldr      r0, [r3, r0]
004db910: ldr      r3, [pc, #0x30]
004db914: mov      ip, #0x50
004db918: add      r1, pc, r1
004db91c: add      r2, pc, r2
004db920: add      r3, pc, r3
004db924: add      r0, r0, #0xa8
004db928: str      ip, [sp]
004db92c: bl       #0x30e004
004db930: b        #0x4db8ec
004db934: ldrdeq   sb, sl, [fp], #-0x10
004db938: andeq    r3, r0, r0, asr #19
004db93c: andeq    r1, r0, r0, asr #19
004db940: eorseq   r2, lr, r0, asr #21
004db944: eorseq   r2, lr, r4, ror #23
004db948: eorseq   r4, lr, r0, lsr #8

# _ZN6Arrays9AnimTable4readEP11IStreamBase
004b38f0: push     {r4, r5, r6, r7, r8, sl, lr}
004b38f4: sub      sp, sp, #0xc
004b38f8: mov      sl, r0
004b38fc: bl       #0x313a90
004b3900: ldr      r6, [pc, #0x120]
004b3904: mov      r3, #1
004b3908: cmp      r3, #0
004b390c: str      r0, [sp, #4]
004b3910: str      r3, [sp]
004b3914: add      r6, pc, r6
004b3918: bne      #0x4b3960
004b391c: add      r3, sp, #4
004b3920: add      r2, r3, #2
004b3924: add      r3, r3, #1
004b3928: ldrb     r0, [r2, #1]
004b392c: ldrb     r1, [r3, #-1]
004b3930: cmp      r2, r3
004b3934: eor      r1, r0, r1
004b3938: strb     r1, [r3, #-1]
004b393c: ldrb     r0, [r2, #1]
004b3940: eor      r1, r1, r0
004b3944: strb     r1, [r2, #1]
004b3948: ldrb     r0, [r3, #-1]
004b394c: sub      r2, r2, #1
004b3950: eor      r1, r1, r0
004b3954: strb     r1, [r3, #-1]
004b3958: add      r3, r3, #1
004b395c: bhi      #0x4b3928
004b3960: bl       #0x4a9e38
004b3964: ldr      r7, [pc, #0xc0]
004b3968: ldr      r4, [sp, #4]
004b396c: mov      r5, #0x14
004b3970: ldr      r3, [r6, r7]
004b3974: mul      r0, r5, r4
004b3978: str      r4, [r3]
004b397c: add      r0, r0, #8
004b3980: mov      r1, #1
004b3984: bl       #0x31056c
004b3988: cmp      r4, #0
004b398c: str      r5, [r0]
004b3990: str      r4, [r0, #4]
004b3994: add      r3, r0, #8
004b3998: beq      #0x4b39c4
004b399c: ldr      r1, [pc, #0x8c]
004b39a0: mov      r2, #0
004b39a4: mov      ip, r2
004b39a8: ldr      r1, [r6, r1]
004b39ac: add      r1, r1, #8
004b39b0: add      r2, r2, #1
004b39b4: cmp      r2, r4
004b39b8: str      r1, [r0, #8]
004b39bc: str      ip, [r0, #0x14]!
004b39c0: bne      #0x4b39b0
004b39c4: ldr      r2, [r6, r7]
004b39c8: ldr      r8, [pc, #0x64]
004b39cc: ldr      r1, [r2]
004b39d0: ldr      r2, [r6, r8]
004b39d4: cmp      r1, #0
004b39d8: str      r3, [r2]
004b39dc: beq      #0x4b3a20
004b39e0: mov      r4, #0
004b39e4: mov      r5, r4
004b39e8: b        #0x4b39f4
004b39ec: ldr      r3, [r6, r8]
004b39f0: ldr      r3, [r3]
004b39f4: add      r0, r3, r4
004b39f8: mov      r1, sl
004b39fc: ldr      r3, [r3, r4]
004b3a00: mov      lr, pc
004b3a04: ldr      pc, [r3, #0xc]
004b3a08: ldr      r3, [r6, r7]
004b3a0c: add      r5, r5, #1
004b3a10: add      r4, r4, #0x14
004b3a14: ldr      r3, [r3]
004b3a18: cmp      r3, r5
004b3a1c: bhi      #0x4b39ec
004b3a20: add      sp, sp, #0xc
004b3a24: pop      {r4, r5, r6, r7, r8, sl, pc}
004b3a28: subeq    r1, lr, ip, ror r1
004b3a2c: andeq    r2, r0, r8, asr #20
004b3a30: ldrdeq   r0, r1, [r0], -ip
004b3a34: andeq    r3, r0, ip, ror ip

# _ZN7Structs7AnimTpl4readEP11IStreamBase
004ef2f0: push     {r4, r5, r6, r7, lr}
004ef2f4: mov      r5, r0
004ef2f8: sub      sp, sp, #0xc
004ef2fc: mov      r0, r1
004ef300: mov      r7, r1
004ef304: ldr      r6, [pc, #0x20c]
004ef308: add      r1, r5, #4
004ef30c: bl       #0x459090
004ef310: mov      r3, #1
004ef314: cmp      r3, #0
004ef318: str      r3, [sp, #4]
004ef31c: add      r6, pc, r6
004ef320: bne      #0x4ef364
004ef324: add      r3, r5, #5
004ef328: add      r2, r5, #6
004ef32c: ldrb     r0, [r2, #1]
004ef330: ldrb     r1, [r3, #-1]
004ef334: cmp      r2, r3
004ef338: eor      r1, r0, r1
004ef33c: strb     r1, [r3, #-1]
004ef340: ldrb     r0, [r2, #1]
004ef344: eor      r1, r1, r0
004ef348: strb     r1, [r2, #1]
004ef34c: ldrb     r0, [r3, #-1]
004ef350: sub      r2, r2, #1
004ef354: eor      r1, r1, r0
004ef358: strb     r1, [r3, #-1]
004ef35c: add      r3, r3, #1
004ef360: bhi      #0x4ef32c
004ef364: mov      r0, r7
004ef368: add      r1, r5, #8
004ef36c: bl       #0x3df1a0
004ef370: mov      r3, #1
004ef374: cmp      r3, #0
004ef378: str      r3, [sp, #4]
004ef37c: bne      #0x4ef3c0
004ef380: add      r3, r5, #9
004ef384: add      r2, r5, #0xa
004ef388: ldrb     r0, [r2, #1]
004ef38c: ldrb     r1, [r3, #-1]
004ef390: cmp      r2, r3
004ef394: eor      r1, r0, r1
004ef398: strb     r1, [r3, #-1]
004ef39c: ldrb     r0, [r2, #1]
004ef3a0: eor      r1, r1, r0
004ef3a4: strb     r1, [r2, #1]
004ef3a8: ldrb     r0, [r3, #-1]
004ef3ac: sub      r2, r2, #1
004ef3b0: eor      r1, r1, r0
004ef3b4: strb     r1, [r3, #-1]
004ef3b8: add      r3, r3, #1
004ef3bc: bhi      #0x4ef388
004ef3c0: ldr      r3, [r5, #0xc]
004ef3c4: cmp      r3, #0
004ef3c8: beq      #0x4ef410
004ef3cc: ldr      r2, [r3, #-4]
004ef3d0: mov      r0, #0x38
004ef3d4: mla      r0, r0, r2, r3
004ef3d8: cmp      r3, r0
004ef3dc: bne      #0x4ef3e8
004ef3e0: b        #0x4ef408
004ef3e4: mov      r0, r4
004ef3e8: sub      r4, r0, #0x38
004ef3ec: ldr      r3, [r0, #-0x38]
004ef3f0: mov      r0, r4
004ef3f4: mov      lr, pc
004ef3f8: ldr      pc, [r3]
004ef3fc: ldr      r0, [r5, #0xc]
004ef400: cmp      r0, r4
004ef404: bne      #0x4ef3e4
004ef408: sub      r0, r0, #8
004ef40c: bl       #0x310440
004ef410: ldr      r4, [r5, #8]
004ef414: mov      r0, #7
004ef418: mov      r1, #1
004ef41c: mul      r0, r0, r4
004ef420: add      r0, r0, r1
004ef424: lsl      r0, r0, #3
004ef428: bl       #0x31056c
004ef42c: mov      r3, #0x38
004ef430: cmp      r4, #0
004ef434: stm      r0, {r3, r4}
004ef438: add      r3, r0, #8
004ef43c: beq      #0x4ef46c
004ef440: ldr      r1, [pc, #0xd4]
004ef444: mov      r2, #0
004ef448: mov      ip, r2
004ef44c: ldr      r1, [r6, r1]
004ef450: add      r1, r1, #8
004ef454: add      r2, r2, #1
004ef458: cmp      r2, r4
004ef45c: str      r1, [r0, #8]
004ef460: str      ip, [r0, #0x2c]
004ef464: add      r0, r0, #0x38
004ef468: bne      #0x4ef454
004ef46c: ldr      r2, [r5, #8]
004ef470: str      r3, [r5, #0xc]
004ef474: cmp      r2, #0
004ef478: beq      #0x4ef4b4
004ef47c: mov      r4, #0
004ef480: mov      r6, r4
004ef484: b        #0x4ef48c
004ef488: ldr      r3, [r5, #0xc]
004ef48c: add      r0, r3, r4
004ef490: mov      r1, r7
004ef494: ldr      r3, [r3, r4]
004ef498: mov      lr, pc
004ef49c: ldr      pc, [r3, #0xc]
004ef4a0: ldr      r3, [r5, #8]
004ef4a4: add      r6, r6, #1
004ef4a8: add      r4, r4, #0x38
004ef4ac: cmp      r3, r6
004ef4b0: bhi      #0x4ef488
004ef4b4: mov      r0, r7
004ef4b8: add      r1, r5, #0x10
004ef4bc: bl       #0x459090
004ef4c0: mov      r3, #1
004ef4c4: cmp      r3, #0
004ef4c8: str      r3, [sp, #4]
004ef4cc: bne      #0x4ef510
004ef4d0: add      r3, r5, #0x12
004ef4d4: add      r5, r5, #0x11
004ef4d8: ldrb     r1, [r3, #1]
004ef4dc: ldrb     r2, [r5, #-1]
004ef4e0: cmp      r3, r5
004ef4e4: eor      r2, r1, r2
004ef4e8: strb     r2, [r5, #-1]
004ef4ec: ldrb     r1, [r3, #1]
004ef4f0: eor      r2, r2, r1
004ef4f4: strb     r2, [r3, #1]
004ef4f8: ldrb     r1, [r5, #-1]
004ef4fc: sub      r3, r3, #1
004ef500: eor      r2, r2, r1
004ef504: strb     r2, [r5, #-1]
004ef508: add      r5, r5, #1
004ef50c: bhi      #0x4ef4d8
004ef510: add      sp, sp, #0xc
004ef514: pop      {r4, r5, r6, r7, pc}
004ef518: subeq    r5, sl, r4, ror r7
004ef51c: muleq    r0, r4, sb

# _ZN7Structs10CamAnimSet4readEP11IStreamBase
004ef05c: push     {r4, r5, r6, r7, r8, lr}
004ef060: mov      r4, r0
004ef064: sub      sp, sp, #8
004ef068: mov      r0, r1
004ef06c: mov      r8, r1
004ef070: add      r1, r4, #4
004ef074: bl       #0x3df1a0
004ef078: mov      r3, #1
004ef07c: cmp      r3, #0
004ef080: str      r3, [sp, #4]
004ef084: bne      #0x4ef0c8
004ef088: add      r3, r4, #5
004ef08c: add      r2, r4, #6
004ef090: ldrb     r0, [r2, #1]
004ef094: ldrb     r1, [r3, #-1]
004ef098: cmp      r2, r3
004ef09c: eor      r1, r0, r1
004ef0a0: strb     r1, [r3, #-1]
004ef0a4: ldrb     r0, [r2, #1]
004ef0a8: eor      r1, r1, r0
004ef0ac: strb     r1, [r2, #1]
004ef0b0: ldrb     r0, [r3, #-1]
004ef0b4: sub      r2, r2, #1
004ef0b8: eor      r1, r1, r0
004ef0bc: strb     r1, [r3, #-1]
004ef0c0: add      r3, r3, #1
004ef0c4: bhi      #0x4ef090
004ef0c8: ldr      r0, [r4, #8]
004ef0cc: cmp      r0, #0
004ef0d0: beq      #0x4ef0d8
004ef0d4: bl       #0x310440
004ef0d8: ldr      r0, [r4, #4]
004ef0dc: mov      r1, #1
004ef0e0: lsl      r0, r0, #2
004ef0e4: bl       #0x31056c
004ef0e8: ldr      r3, [r4, #4]
004ef0ec: str      r0, [r4, #8]
004ef0f0: cmp      r3, #0
004ef0f4: beq      #0x4ef178
004ef0f8: mov      r5, #0
004ef0fc: mov      r7, #1
004ef100: lsl      r6, r5, #2
004ef104: add      r1, r0, r6
004ef108: mov      r0, r8
004ef10c: bl       #0x459090
004ef110: str      r7, [sp, #4]
004ef114: cmp      r7, #0
004ef118: ldr      r3, [r4, #8]
004ef11c: bne      #0x4ef164
004ef120: add      r6, r3, r6
004ef124: add      r3, r6, #2
004ef128: add      r6, r6, #1
004ef12c: ldrb     r1, [r3, #1]
004ef130: ldrb     r2, [r6, #-1]
004ef134: cmp      r3, r6
004ef138: eor      r2, r1, r2
004ef13c: strb     r2, [r6, #-1]
004ef140: ldrb     r1, [r3, #1]
004ef144: eor      r2, r2, r1
004ef148: strb     r2, [r3, #1]
004ef14c: ldrb     r1, [r6, #-1]
004ef150: sub      r3, r3, #1
004ef154: eor      r2, r2, r1
004ef158: strb     r2, [r6, #-1]
004ef15c: add      r6, r6, #1
004ef160: bhi      #0x4ef12c
004ef164: ldr      r3, [r4, #4]
004ef168: add      r5, r5, #1
004ef16c: cmp      r3, r5
004ef170: ldrhi    r0, [r4, #8]
004ef174: bhi      #0x4ef100
004ef178: mov      r0, r8
004ef17c: add      r1, r4, #0xc
004ef180: bl       #0x459090
004ef184: mov      r3, #1
004ef188: cmp      r3, #0
004ef18c: str      r3, [sp, #4]
004ef190: bne      #0x4ef1d4
004ef194: add      r3, r4, #0xd
004ef198: add      r2, r4, #0xe
004ef19c: ldrb     r0, [r2, #1]
004ef1a0: ldrb     r1, [r3, #-1]
004ef1a4: cmp      r2, r3
004ef1a8: eor      r1, r0, r1
004ef1ac: strb     r1, [r3, #-1]
004ef1b0: ldrb     r0, [r2, #1]
004ef1b4: eor      r1, r1, r0
004ef1b8: strb     r1, [r2, #1]
004ef1bc: ldrb     r0, [r3, #-1]
004ef1c0: sub      r2, r2, #1
004ef1c4: eor      r1, r1, r0
004ef1c8: strb     r1, [r3, #-1]
004ef1cc: add      r3, r3, #1
004ef1d0: bhi      #0x4ef19c
004ef1d4: mov      r0, r8
004ef1d8: add      r1, r4, #0x10
004ef1dc: bl       #0x459090
004ef1e0: mov      r3, #1
004ef1e4: cmp      r3, #0
004ef1e8: str      r3, [sp, #4]
004ef1ec: bne      #0x4ef230
004ef1f0: add      r3, r4, #0x11
004ef1f4: add      r2, r4, #0x12
004ef1f8: ldrb     r0, [r2, #1]
004ef1fc: ldrb     r1, [r3, #-1]
004ef200: cmp      r2, r3
004ef204: eor      r1, r0, r1
004ef208: strb     r1, [r3, #-1]
004ef20c: ldrb     r0, [r2, #1]
004ef210: eor      r1, r1, r0
004ef214: strb     r1, [r2, #1]
004ef218: ldrb     r0, [r3, #-1]
004ef21c: sub      r2, r2, #1
004ef220: eor      r1, r1, r0
004ef224: strb     r1, [r3, #-1]
004ef228: add      r3, r3, #1
004ef22c: bhi      #0x4ef1f8
004ef230: mov      r0, r8
004ef234: add      r1, r4, #0x14
004ef238: bl       #0x459090
004ef23c: mov      r3, #1
004ef240: cmp      r3, #0
004ef244: str      r3, [sp, #4]
004ef248: bne      #0x4ef28c
004ef24c: add      r3, r4, #0x15
004ef250: add      r2, r4, #0x16
004ef254: ldrb     r0, [r2, #1]
004ef258: ldrb     r1, [r3, #-1]
004ef25c: cmp      r2, r3
004ef260: eor      r1, r0, r1
004ef264: strb     r1, [r3, #-1]
004ef268: ldrb     r0, [r2, #1]
004ef26c: eor      r1, r1, r0
004ef270: strb     r1, [r2, #1]
004ef274: ldrb     r0, [r3, #-1]
004ef278: sub      r2, r2, #1
004ef27c: eor      r1, r1, r0
004ef280: strb     r1, [r3, #-1]
004ef284: add      r3, r3, #1
004ef288: bhi      #0x4ef254
004ef28c: mov      r0, r8
004ef290: add      r1, r4, #0x18
004ef294: bl       #0x459090
004ef298: mov      r3, #1
004ef29c: cmp      r3, #0
004ef2a0: str      r3, [sp, #4]
004ef2a4: bne      #0x4ef2e8
004ef2a8: add      r3, r4, #0x1a
004ef2ac: add      r4, r4, #0x19
004ef2b0: ldrb     r1, [r3, #1]
004ef2b4: ldrb     r2, [r4, #-1]
004ef2b8: cmp      r3, r4
004ef2bc: eor      r2, r1, r2
004ef2c0: strb     r2, [r4, #-1]
004ef2c4: ldrb     r1, [r3, #1]
004ef2c8: eor      r2, r2, r1
004ef2cc: strb     r2, [r3, #1]
004ef2d0: ldrb     r1, [r4, #-1]
004ef2d4: sub      r3, r3, #1
004ef2d8: eor      r2, r2, r1
004ef2dc: strb     r2, [r4, #-1]
004ef2e0: add      r4, r4, #1
004ef2e4: bhi      #0x4ef2b0
004ef2e8: add      sp, sp, #8
004ef2ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6Arrays15CamAnimSetTable4readEP11IStreamBase
004b37a4: push     {r4, r5, r6, r7, r8, sl, lr}
004b37a8: sub      sp, sp, #0xc
004b37ac: mov      sl, r0
004b37b0: bl       #0x313a90
004b37b4: ldr      r6, [pc, #0x124]
004b37b8: mov      r3, #1
004b37bc: cmp      r3, #0
004b37c0: str      r0, [sp, #4]
004b37c4: str      r3, [sp]
004b37c8: add      r6, pc, r6
004b37cc: bne      #0x4b3814
004b37d0: add      r3, sp, #4
004b37d4: add      r2, r3, #2
004b37d8: add      r3, r3, #1
004b37dc: ldrb     r0, [r2, #1]
004b37e0: ldrb     r1, [r3, #-1]
004b37e4: cmp      r2, r3
004b37e8: eor      r1, r0, r1
004b37ec: strb     r1, [r3, #-1]
004b37f0: ldrb     r0, [r2, #1]
004b37f4: eor      r1, r1, r0
004b37f8: strb     r1, [r2, #1]
004b37fc: ldrb     r0, [r3, #-1]
004b3800: sub      r2, r2, #1
004b3804: eor      r1, r1, r0
004b3808: strb     r1, [r3, #-1]
004b380c: add      r3, r3, #1
004b3810: bhi      #0x4b37dc
004b3814: bl       #0x4a9cb8
004b3818: ldr      r7, [pc, #0xc4]
004b381c: ldr      r4, [sp, #4]
004b3820: mov      r5, #0x1c
004b3824: ldr      r3, [r6, r7]
004b3828: mul      r0, r5, r4
004b382c: str      r4, [r3]
004b3830: add      r0, r0, #8
004b3834: mov      r1, #1
004b3838: bl       #0x31056c
004b383c: cmp      r4, #0
004b3840: str      r5, [r0]
004b3844: str      r4, [r0, #4]
004b3848: add      r3, r0, #8
004b384c: beq      #0x4b387c
004b3850: ldr      r1, [pc, #0x90]
004b3854: mov      r2, #0
004b3858: mov      ip, r2
004b385c: ldr      r1, [r6, r1]
004b3860: add      r1, r1, #8
004b3864: add      r2, r2, #1
004b3868: cmp      r2, r4
004b386c: str      r1, [r0, #8]
004b3870: str      ip, [r0, #0x10]
004b3874: add      r0, r0, #0x1c
004b3878: bne      #0x4b3864
004b387c: ldr      r2, [r6, r7]
004b3880: ldr      r8, [pc, #0x64]
004b3884: ldr      r1, [r2]
004b3888: ldr      r2, [r6, r8]
004b388c: cmp      r1, #0
004b3890: str      r3, [r2]
004b3894: beq      #0x4b38d8
004b3898: mov      r4, #0
004b389c: mov      r5, r4
004b38a0: b        #0x4b38ac
004b38a4: ldr      r3, [r6, r8]
004b38a8: ldr      r3, [r3]
004b38ac: add      r0, r3, r4
004b38b0: mov      r1, sl
004b38b4: ldr      r3, [r3, r4]
004b38b8: mov      lr, pc
004b38bc: ldr      pc, [r3, #0xc]
004b38c0: ldr      r3, [r6, r7]
004b38c4: add      r5, r5, #1
004b38c8: add      r4, r4, #0x1c
004b38cc: ldr      r3, [r3]
004b38d0: cmp      r3, r5
004b38d4: bhi      #0x4b38a4
004b38d8: add      sp, sp, #0xc
004b38dc: pop      {r4, r5, r6, r7, r8, sl, pc}
004b38e0: subeq    r1, lr, r8, asr #5
004b38e4: andeq    r3, r0, r4, ror #17
004b38e8: strheq   r3, [r0], -ip
004b38ec: ldrdeq   r3, r4, [r0], -r4

# _ZN6Arrays13CharAnimTable4readEP11IStreamBase
004b45d0: push     {r4, r5, r6, r7, r8, sl, lr}
004b45d4: sub      sp, sp, #0xc
004b45d8: mov      sl, r0
004b45dc: bl       #0x313a90
004b45e0: ldr      r6, [pc, #0x12c]
004b45e4: mov      r3, #1
004b45e8: cmp      r3, #0
004b45ec: str      r0, [sp, #4]
004b45f0: str      r3, [sp]
004b45f4: add      r6, pc, r6
004b45f8: bne      #0x4b4640
004b45fc: add      r3, sp, #4
004b4600: add      r2, r3, #2
004b4604: add      r3, r3, #1
004b4608: ldrb     r0, [r2, #1]
004b460c: ldrb     r1, [r3, #-1]
004b4610: cmp      r2, r3
004b4614: eor      r1, r0, r1
004b4618: strb     r1, [r3, #-1]
004b461c: ldrb     r0, [r2, #1]
004b4620: eor      r1, r1, r0
004b4624: strb     r1, [r2, #1]
004b4628: ldrb     r0, [r3, #-1]
004b462c: sub      r2, r2, #1
004b4630: eor      r1, r1, r0
004b4634: strb     r1, [r3, #-1]
004b4638: add      r3, r3, #1
004b463c: bhi      #0x4b4608
004b4640: bl       #0x4a9b38
004b4644: ldr      r4, [sp, #4]
004b4648: ldr      r7, [pc, #0xc8]
004b464c: mov      r0, #0x14
004b4650: mul      r0, r0, r4
004b4654: ldr      r3, [r6, r7]
004b4658: add      r0, r0, #1
004b465c: lsl      r0, r0, #3
004b4660: str      r4, [r3]
004b4664: mov      r1, #1
004b4668: bl       #0x31056c
004b466c: mov      r3, #0xa0
004b4670: cmp      r4, #0
004b4674: stm      r0, {r3, r4}
004b4678: add      r3, r0, #8
004b467c: beq      #0x4b46b0
004b4680: ldr      r1, [pc, #0x94]
004b4684: mov      r2, #0
004b4688: ldr      ip, [r6, r1]
004b468c: mov      r1, r2
004b4690: add      ip, ip, #8
004b4694: add      r2, r2, #1
004b4698: cmp      r2, r4
004b469c: str      ip, [r0, #8]
004b46a0: str      r1, [r0, #0x4c]
004b46a4: str      r1, [r0, #0x90]
004b46a8: add      r0, r0, #0xa0
004b46ac: bne      #0x4b4694
004b46b0: ldr      r2, [r6, r7]
004b46b4: ldr      r8, [pc, #0x64]
004b46b8: ldr      r1, [r2]
004b46bc: ldr      r2, [r6, r8]
004b46c0: cmp      r1, #0
004b46c4: str      r3, [r2]
004b46c8: beq      #0x4b470c
004b46cc: mov      r4, #0
004b46d0: mov      r5, r4
004b46d4: b        #0x4b46e0
004b46d8: ldr      r3, [r6, r8]
004b46dc: ldr      r3, [r3]
004b46e0: add      r0, r3, r4
004b46e4: mov      r1, sl
004b46e8: ldr      r3, [r3, r4]
004b46ec: mov      lr, pc
004b46f0: ldr      pc, [r3, #0xc]
004b46f4: ldr      r3, [r6, r7]
004b46f8: add      r5, r5, #1
004b46fc: add      r4, r4, #0xa0
004b4700: ldr      r3, [r3]
004b4704: cmp      r3, r5
004b4708: bhi      #0x4b46d8
004b470c: add      sp, sp, #0xc
004b4710: pop      {r4, r5, r6, r7, r8, sl, pc}
004b4714: umaaleq  r0, lr, ip, r4
004b4718: andeq    r2, r0, r0, asr #17
004b471c: andeq    r4, r0, r4, ror #10
004b4720: andeq    r4, r0, r4, asr #16

# _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
003df1a0: str      lr, [sp, #-4]!
003df1a4: mov      r3, #0
003df1a8: sub      sp, sp, #0xc
003df1ac: ldr      ip, [r0]
003df1b0: mov      r2, #4
003df1b4: mov      lr, pc
003df1b8: ldr      pc, [ip, #0x18]
003df1bc: ldr      r3, [pc, #0x74]
003df1c0: cmp      r0, #4
003df1c4: add      r3, pc, r3
003df1c8: beq      #0x3df1f8
003df1cc: ldr      r2, [pc, #0x68]
003df1d0: ldr      r2, [r3, r2]
003df1d4: ldr      r2, [r2]
003df1d8: cmp      r2, #2
003df1dc: moveq    r3, #0
003df1e0: streq    r3, [r3]
003df1e4: beq      #0x3df1f0
003df1e8: cmp      r2, #1
003df1ec: beq      #0x3df204
003df1f0: add      sp, sp, #0xc
003df1f4: ldm      sp!, {pc}
003df1f8: cmp      r1, #0
003df1fc: beq      #0x3df1f0
003df200: b        #0x3df1cc
003df204: ldr      r0, [pc, #0x34]
003df208: ldr      r1, [pc, #0x34]
003df20c: ldr      r2, [pc, #0x34]
003df210: ldr      r0, [r3, r0]
003df214: ldr      r3, [pc, #0x30]
003df218: mov      ip, #0x50
003df21c: add      r1, pc, r1
003df220: add      r2, pc, r2
003df224: add      r3, pc, r3
003df228: add      r0, r0, #0xa8
003df22c: str      ip, [sp]
003df230: bl       #0x30e004
003df234: b        #0x3df1f0
003df238: subseq   r5, fp, ip, asr #17
003df23c: andeq    r3, r0, r0, asr #19
003df240: andeq    r1, r0, r0, asr #19
003df244: strheq   pc, [sp], #-0x1c
003df248: subeq    pc, sp, r0, ror #5
003df24c: subeq    r0, lr, ip, lsl fp

# _ZN12StreamReader6readAsIfEEvP11IStreamBasePT_
004db94c: str      lr, [sp, #-4]!
004db950: mov      r3, #0
004db954: sub      sp, sp, #0xc
004db958: ldr      ip, [r0]
004db95c: mov      r2, #4
004db960: mov      lr, pc
004db964: ldr      pc, [ip, #0x18]
004db968: ldr      r3, [pc, #0x74]
004db96c: cmp      r0, #4
004db970: add      r3, pc, r3
004db974: beq      #0x4db9a4
004db978: ldr      r2, [pc, #0x68]
004db97c: ldr      r2, [r3, r2]
004db980: ldr      r2, [r2]
004db984: cmp      r2, #2
004db988: moveq    r3, #0
004db98c: streq    r3, [r3]
004db990: beq      #0x4db99c
004db994: cmp      r2, #1
004db998: beq      #0x4db9b0
004db99c: add      sp, sp, #0xc
004db9a0: ldm      sp!, {pc}
004db9a4: cmp      r1, #0
004db9a8: beq      #0x4db99c
004db9ac: b        #0x4db978
004db9b0: ldr      r0, [pc, #0x34]
004db9b4: ldr      r1, [pc, #0x34]
004db9b8: ldr      r2, [pc, #0x34]
004db9bc: ldr      r0, [r3, r0]
004db9c0: ldr      r3, [pc, #0x30]
004db9c4: mov      ip, #0x50
004db9c8: add      r1, pc, r1
004db9cc: add      r2, pc, r2
004db9d0: add      r3, pc, r3
004db9d4: add      r0, r0, #0xa8
004db9d8: str      ip, [sp]
004db9dc: bl       #0x30e004
004db9e0: b        #0x4db99c
004db9e4: subeq    sb, fp, r0, lsr #2
004db9e8: andeq    r3, r0, r0, asr #19
004db9ec: andeq    r1, r0, r0, asr #19
004db9f0: eorseq   r2, lr, r0, lsl sl
004db9f4: eorseq   r2, lr, r4, lsr fp
004db9f8: eorseq   r4, lr, r0, ror r3

# _ZN7Structs8CharAnim4readEP11IStreamBase
004ef64c: push     {r4, r5, r6, r7, r8, lr}
004ef650: mov      r4, r0
004ef654: sub      sp, sp, #8
004ef658: mov      r0, r1
004ef65c: mov      r5, r1
004ef660: add      r1, r4, #4
004ef664: bl       #0x459090
004ef668: mov      r3, #1
004ef66c: cmp      r3, #0
004ef670: str      r3, [sp, #4]
004ef674: bne      #0x4ef6b8
004ef678: add      r3, r4, #5
004ef67c: add      r2, r4, #6
004ef680: ldrb     r0, [r2, #1]
004ef684: ldrb     r1, [r3, #-1]
004ef688: cmp      r3, r2
004ef68c: eor      r1, r0, r1
004ef690: strb     r1, [r3, #-1]
004ef694: ldrb     r0, [r2, #1]
004ef698: eor      r1, r1, r0
004ef69c: strb     r1, [r2, #1]
004ef6a0: ldrb     r0, [r3, #-1]
004ef6a4: sub      r2, r2, #1
004ef6a8: eor      r1, r1, r0
004ef6ac: strb     r1, [r3, #-1]
004ef6b0: add      r3, r3, #1
004ef6b4: blo      #0x4ef680
004ef6b8: mov      r0, r5
004ef6bc: add      r1, r4, #8
004ef6c0: bl       #0x459090
004ef6c4: mov      r3, #1
004ef6c8: cmp      r3, #0
004ef6cc: str      r3, [sp, #4]
004ef6d0: bne      #0x4ef714
004ef6d4: add      r3, r4, #9
004ef6d8: add      r2, r4, #0xa
004ef6dc: ldrb     r0, [r2, #1]
004ef6e0: ldrb     r1, [r3, #-1]
004ef6e4: cmp      r3, r2
004ef6e8: eor      r1, r0, r1
004ef6ec: strb     r1, [r3, #-1]
004ef6f0: ldrb     r0, [r2, #1]
004ef6f4: eor      r1, r1, r0
004ef6f8: strb     r1, [r2, #1]
004ef6fc: ldrb     r0, [r3, #-1]
004ef700: sub      r2, r2, #1
004ef704: eor      r1, r1, r0
004ef708: strb     r1, [r3, #-1]
004ef70c: add      r3, r3, #1
004ef710: blo      #0x4ef6dc
004ef714: mov      r0, r5
004ef718: add      r1, r4, #0xc
004ef71c: bl       #0x459090
004ef720: mov      r3, #1
004ef724: cmp      r3, #0
004ef728: str      r3, [sp, #4]
004ef72c: bne      #0x4ef770
004ef730: add      r3, r4, #0xd
004ef734: add      r2, r4, #0xe
004ef738: ldrb     r0, [r2, #1]
004ef73c: ldrb     r1, [r3, #-1]
004ef740: cmp      r3, r2
004ef744: eor      r1, r0, r1
004ef748: strb     r1, [r3, #-1]
004ef74c: ldrb     r0, [r2, #1]
004ef750: eor      r1, r1, r0
004ef754: strb     r1, [r2, #1]
004ef758: ldrb     r0, [r3, #-1]
004ef75c: sub      r2, r2, #1
004ef760: eor      r1, r1, r0
004ef764: strb     r1, [r3, #-1]
004ef768: add      r3, r3, #1
004ef76c: blo      #0x4ef738
004ef770: mov      r0, r5
004ef774: add      r1, r4, #0x10
004ef778: bl       #0x459090
004ef77c: mov      r3, #1
004ef780: cmp      r3, #0
004ef784: str      r3, [sp, #4]
004ef788: bne      #0x4ef7cc
004ef78c: add      r3, r4, #0x11
004ef790: add      r2, r4, #0x12
004ef794: ldrb     r0, [r2, #1]
004ef798: ldrb     r1, [r3, #-1]
004ef79c: cmp      r3, r2
004ef7a0: eor      r1, r0, r1
004ef7a4: strb     r1, [r3, #-1]
004ef7a8: ldrb     r0, [r2, #1]
004ef7ac: eor      r1, r1, r0
004ef7b0: strb     r1, [r2, #1]
004ef7b4: ldrb     r0, [r3, #-1]
004ef7b8: sub      r2, r2, #1
004ef7bc: eor      r1, r1, r0
004ef7c0: strb     r1, [r3, #-1]
004ef7c4: add      r3, r3, #1
004ef7c8: blo      #0x4ef794
004ef7cc: mov      r0, r5
004ef7d0: add      r1, r4, #0x14
004ef7d4: bl       #0x459090
004ef7d8: mov      r3, #1
004ef7dc: cmp      r3, #0
004ef7e0: str      r3, [sp, #4]
004ef7e4: bne      #0x4ef828
004ef7e8: add      r3, r4, #0x15
004ef7ec: add      r2, r4, #0x16
004ef7f0: ldrb     r0, [r2, #1]
004ef7f4: ldrb     r1, [r3, #-1]
004ef7f8: cmp      r3, r2
004ef7fc: eor      r1, r0, r1
004ef800: strb     r1, [r3, #-1]
004ef804: ldrb     r0, [r2, #1]
004ef808: eor      r1, r1, r0
004ef80c: strb     r1, [r2, #1]
004ef810: ldrb     r0, [r3, #-1]
004ef814: sub      r2, r2, #1
004ef818: eor      r1, r1, r0
004ef81c: strb     r1, [r3, #-1]
004ef820: add      r3, r3, #1
004ef824: blo      #0x4ef7f0
004ef828: mov      r0, r5
004ef82c: add      r1, r4, #0x18
004ef830: bl       #0x459090
004ef834: mov      r3, #1
004ef838: cmp      r3, #0
004ef83c: str      r3, [sp, #4]
004ef840: bne      #0x4ef884
004ef844: add      r3, r4, #0x19
004ef848: add      r2, r4, #0x1a
004ef84c: ldrb     r0, [r2, #1]
004ef850: ldrb     r1, [r3, #-1]
004ef854: cmp      r3, r2
004ef858: eor      r1, r0, r1
004ef85c: strb     r1, [r3, #-1]
004ef860: ldrb     r0, [r2, #1]
004ef864: eor      r1, r1, r0
004ef868: strb     r1, [r2, #1]
004ef86c: ldrb     r0, [r3, #-1]
004ef870: sub      r2, r2, #1
004ef874: eor      r1, r1, r0
004ef878: strb     r1, [r3, #-1]
004ef87c: add      r3, r3, #1
004ef880: blo      #0x4ef84c
004ef884: mov      r0, r5
004ef888: add      r1, r4, #0x1c
004ef88c: bl       #0x459090
004ef890: mov      r3, #1
004ef894: cmp      r3, #0
004ef898: str      r3, [sp, #4]
004ef89c: bne      #0x4ef8e0
004ef8a0: add      r3, r4, #0x1d
004ef8a4: add      r2, r4, #0x1e
004ef8a8: ldrb     r0, [r2, #1]
004ef8ac: ldrb     r1, [r3, #-1]
004ef8b0: cmp      r3, r2
004ef8b4: eor      r1, r0, r1
004ef8b8: strb     r1, [r3, #-1]
004ef8bc: ldrb     r0, [r2, #1]
004ef8c0: eor      r1, r1, r0
004ef8c4: strb     r1, [r2, #1]
004ef8c8: ldrb     r0, [r3, #-1]
004ef8cc: sub      r2, r2, #1
004ef8d0: eor      r1, r1, r0
004ef8d4: strb     r1, [r3, #-1]
004ef8d8: add      r3, r3, #1
004ef8dc: blo      #0x4ef8a8
004ef8e0: mov      r0, r5
004ef8e4: add      r1, r4, #0x20
004ef8e8: bl       #0x459090
004ef8ec: mov      r3, #1
004ef8f0: cmp      r3, #0
004ef8f4: str      r3, [sp, #4]
004ef8f8: bne      #0x4ef93c
004ef8fc: add      r3, r4, #0x21
004ef900: add      r2, r4, #0x22
004ef904: ldrb     r0, [r2, #1]
004ef908: ldrb     r1, [r3, #-1]
004ef90c: cmp      r3, r2
004ef910: eor      r1, r0, r1
004ef914: strb     r1, [r3, #-1]
004ef918: ldrb     r0, [r2, #1]
004ef91c: eor      r1, r1, r0
004ef920: strb     r1, [r2, #1]
004ef924: ldrb     r0, [r3, #-1]
004ef928: sub      r2, r2, #1
004ef92c: eor      r1, r1, r0
004ef930: strb     r1, [r3, #-1]
004ef934: add      r3, r3, #1
004ef938: blo      #0x4ef904
004ef93c: mov      r0, r5
004ef940: add      r1, r4, #0x24
004ef944: bl       #0x459090
004ef948: mov      r3, #1
004ef94c: cmp      r3, #0
004ef950: str      r3, [sp, #4]
004ef954: bne      #0x4ef998
004ef958: add      r3, r4, #0x25
004ef95c: add      r2, r4, #0x26
004ef960: ldrb     r0, [r2, #1]
004ef964: ldrb     r1, [r3, #-1]
004ef968: cmp      r3, r2
004ef96c: eor      r1, r0, r1
004ef970: strb     r1, [r3, #-1]
004ef974: ldrb     r0, [r2, #1]
004ef978: eor      r1, r1, r0
004ef97c: strb     r1, [r2, #1]
004ef980: ldrb     r0, [r3, #-1]
004ef984: sub      r2, r2, #1
004ef988: eor      r1, r1, r0
004ef98c: strb     r1, [r3, #-1]
004ef990: add      r3, r3, #1
004ef994: blo      #0x4ef960
004ef998: mov      r0, r5
004ef99c: add      r1, r4, #0x28
004ef9a0: bl       #0x459090
004ef9a4: mov      r3, #1
004ef9a8: cmp      r3, #0
004ef9ac: str      r3, [sp, #4]
004ef9b0: bne      #0x4ef9f4
004ef9b4: add      r3, r4, #0x29
004ef9b8: add      r2, r4, #0x2a
004ef9bc: ldrb     r0, [r2, #1]
004ef9c0: ldrb     r1, [r3, #-1]
004ef9c4: cmp      r3, r2
004ef9c8: eor      r1, r0, r1
004ef9cc: strb     r1, [r3, #-1]
004ef9d0: ldrb     r0, [r2, #1]
004ef9d4: eor      r1, r1, r0
004ef9d8: strb     r1, [r2, #1]
004ef9dc: ldrb     r0, [r3, #-1]
004ef9e0: sub      r2, r2, #1
004ef9e4: eor      r1, r1, r0
004ef9e8: strb     r1, [r3, #-1]
004ef9ec: add      r3, r3, #1
004ef9f0: blo      #0x4ef9bc
004ef9f4: mov      r0, r5
004ef9f8: add      r1, r4, #0x2c
004ef9fc: bl       #0x459090
004efa00: mov      r3, #1
004efa04: cmp      r3, #0
004efa08: str      r3, [sp, #4]
004efa0c: bne      #0x4efa50
004efa10: add      r3, r4, #0x2d
004efa14: add      r2, r4, #0x2e
004efa18: ldrb     r0, [r2, #1]
004efa1c: ldrb     r1, [r3, #-1]
004efa20: cmp      r3, r2
004efa24: eor      r1, r0, r1
004efa28: strb     r1, [r3, #-1]
004efa2c: ldrb     r0, [r2, #1]
004efa30: eor      r1, r1, r0
004efa34: strb     r1, [r2, #1]
004efa38: ldrb     r0, [r3, #-1]
004efa3c: sub      r2, r2, #1
004efa40: eor      r1, r1, r0
004efa44: strb     r1, [r3, #-1]
004efa48: add      r3, r3, #1
004efa4c: blo      #0x4efa18
004efa50: mov      r0, r5
004efa54: add      r1, r4, #0x30
004efa58: bl       #0x459090
004efa5c: mov      r3, #1
004efa60: cmp      r3, #0
004efa64: str      r3, [sp, #4]
004efa68: bne      #0x4efaac
004efa6c: add      r3, r4, #0x31
004efa70: add      r2, r4, #0x32
004efa74: ldrb     r0, [r2, #1]
004efa78: ldrb     r1, [r3, #-1]
004efa7c: cmp      r3, r2
004efa80: eor      r1, r0, r1
004efa84: strb     r1, [r3, #-1]
004efa88: ldrb     r0, [r2, #1]
004efa8c: eor      r1, r1, r0
004efa90: strb     r1, [r2, #1]
004efa94: ldrb     r0, [r3, #-1]
004efa98: sub      r2, r2, #1
004efa9c: eor      r1, r1, r0
004efaa0: strb     r1, [r3, #-1]
004efaa4: add      r3, r3, #1
004efaa8: blo      #0x4efa74
004efaac: mov      r0, r5
004efab0: add      r1, r4, #0x34
004efab4: bl       #0x459090
004efab8: mov      r3, #1
004efabc: cmp      r3, #0
004efac0: str      r3, [sp, #4]
004efac4: bne      #0x4efb08
004efac8: add      r3, r4, #0x35
004efacc: add      r2, r4, #0x36
004efad0: ldrb     r0, [r2, #1]
004efad4: ldrb     r1, [r3, #-1]
004efad8: cmp      r3, r2
004efadc: eor      r1, r0, r1
004efae0: strb     r1, [r3, #-1]
004efae4: ldrb     r0, [r2, #1]
004efae8: eor      r1, r1, r0
004efaec: strb     r1, [r2, #1]
004efaf0: ldrb     r0, [r3, #-1]
004efaf4: sub      r2, r2, #1
004efaf8: eor      r1, r1, r0
004efafc: strb     r1, [r3, #-1]
004efb00: add      r3, r3, #1
004efb04: blo      #0x4efad0
004efb08: mov      r0, r5
004efb0c: add      r1, r4, #0x38
004efb10: bl       #0x459090
004efb14: mov      r3, #1
004efb18: cmp      r3, #0
004efb1c: str      r3, [sp, #4]
004efb20: bne      #0x4efb64
004efb24: add      r3, r4, #0x39
004efb28: add      r2, r4, #0x3a
004efb2c: ldrb     r0, [r2, #1]
004efb30: ldrb     r1, [r3, #-1]
004efb34: cmp      r3, r2
004efb38: eor      r1, r0, r1
004efb3c: strb     r1, [r3, #-1]
004efb40: ldrb     r0, [r2, #1]
004efb44: eor      r1, r1, r0
004efb48: strb     r1, [r2, #1]
004efb4c: ldrb     r0, [r3, #-1]
004efb50: sub      r2, r2, #1
004efb54: eor      r1, r1, r0
004efb58: strb     r1, [r3, #-1]
004efb5c: add      r3, r3, #1
004efb60: blo      #0x4efb2c
004efb64: mov      r0, r5
004efb68: add      r1, r4, #0x3c
004efb6c: bl       #0x459090
004efb70: mov      r3, #1
004efb74: cmp      r3, #0
004efb78: str      r3, [sp, #4]
004efb7c: bne      #0x4efbc0
004efb80: add      r3, r4, #0x3d
004efb84: add      r2, r4, #0x3e
004efb88: ldrb     r0, [r2, #1]
004efb8c: ldrb     r1, [r3, #-1]
004efb90: cmp      r3, r2
004efb94: eor      r1, r0, r1
004efb98: strb     r1, [r3, #-1]
004efb9c: ldrb     r0, [r2, #1]
004efba0: eor      r1, r1, r0
004efba4: strb     r1, [r2, #1]
004efba8: ldrb     r0, [r3, #-1]
004efbac: sub      r2, r2, #1
004efbb0: eor      r1, r1, r0
004efbb4: strb     r1, [r3, #-1]
004efbb8: add      r3, r3, #1
004efbbc: blo      #0x4efb88
004efbc0: mov      r0, r5
004efbc4: add      r1, r4, #0x40
004efbc8: bl       #0x3df1a0
004efbcc: mov      r3, #1
004efbd0: cmp      r3, #0
004efbd4: str      r3, [sp, #4]
004efbd8: bne      #0x4efc1c
004efbdc: add      r3, r4, #0x41
004efbe0: add      r2, r4, #0x42
004efbe4: ldrb     r0, [r2, #1]
004efbe8: ldrb     r1, [r3, #-1]
004efbec: cmp      r3, r2
004efbf0: eor      r1, r0, r1
004efbf4: strb     r1, [r3, #-1]
004efbf8: ldrb     r0, [r2, #1]
004efbfc: eor      r1, r1, r0
004efc00: strb     r1, [r2, #1]
004efc04: ldrb     r0, [r3, #-1]
004efc08: sub      r2, r2, #1
004efc0c: eor      r1, r1, r0
004efc10: strb     r1, [r3, #-1]
004efc14: add      r3, r3, #1
004efc18: blo      #0x4efbe4
004efc1c: ldr      r0, [r4, #0x44]
004efc20: cmp      r0, #0
004efc24: beq      #0x4efc2c
004efc28: bl       #0x310440
004efc2c: ldr      r0, [r4, #0x40]
004efc30: mov      r1, #1
004efc34: lsl      r0, r0, #2
004efc38: bl       #0x31056c
004efc3c: ldr      r3, [r4, #0x40]
004efc40: str      r0, [r4, #0x44]
004efc44: cmp      r3, #0
004efc48: beq      #0x4efccc
004efc4c: mov      r6, #0
004efc50: mov      r8, #1
004efc54: lsl      r7, r6, #2
004efc58: add      r1, r0, r7
004efc5c: mov      r0, r5
004efc60: bl       #0x459090
004efc64: str      r8, [sp, #4]
004efc68: cmp      r8, #0
004efc6c: ldr      r3, [r4, #0x44]
004efc70: bne      #0x4efcb8
004efc74: add      r7, r3, r7
004efc78: add      r3, r7, #2
004efc7c: add      r7, r7, #1
004efc80: ldrb     r1, [r3, #1]
004efc84: ldrb     r2, [r7, #-1]
004efc88: cmp      r7, r3
004efc8c: eor      r2, r1, r2
004efc90: strb     r2, [r7, #-1]
004efc94: ldrb     r1, [r3, #1]
004efc98: eor      r2, r2, r1
004efc9c: strb     r2, [r3, #1]
004efca0: ldrb     r1, [r7, #-1]
004efca4: sub      r3, r3, #1
004efca8: eor      r2, r2, r1
004efcac: strb     r2, [r7, #-1]
004efcb0: add      r7, r7, #1
004efcb4: blo      #0x4efc80
004efcb8: ldr      r3, [r4, #0x40]
004efcbc: add      r6, r6, #1
004efcc0: cmp      r3, r6
004efcc4: ldrhi    r0, [r4, #0x44]
004efcc8: bhi      #0x4efc54
004efccc: mov      r0, r5
004efcd0: add      r1, r4, #0x48
004efcd4: bl       #0x459090
004efcd8: mov      r3, #1
004efcdc: cmp      r3, #0
004efce0: str      r3, [sp, #4]
004efce4: bne      #0x4efd28
004efce8: add      r3, r4, #0x49
004efcec: add      r2, r4, #0x4a
004efcf0: ldrb     r0, [r2, #1]
004efcf4: ldrb     r1, [r3, #-1]
004efcf8: cmp      r3, r2
004efcfc: eor      r1, r0, r1
004efd00: strb     r1, [r3, #-1]
004efd04: ldrb     r0, [r2, #1]
004efd08: eor      r1, r1, r0
004efd0c: strb     r1, [r2, #1]
004efd10: ldrb     r0, [r3, #-1]
004efd14: sub      r2, r2, #1
004efd18: eor      r1, r1, r0
004efd1c: strb     r1, [r3, #-1]
004efd20: add      r3, r3, #1
004efd24: blo      #0x4efcf0
004efd28: mov      r0, r5
004efd2c: add      r1, r4, #0x4c
004efd30: bl       #0x459090
004efd34: mov      r3, #1
004efd38: cmp      r3, #0
004efd3c: str      r3, [sp, #4]
004efd40: bne      #0x4efd84
004efd44: add      r3, r4, #0x4d
004efd48: add      r2, r4, #0x4e
004efd4c: ldrb     r0, [r2, #1]
004efd50: ldrb     r1, [r3, #-1]
004efd54: cmp      r3, r2
004efd58: eor      r1, r0, r1
004efd5c: strb     r1, [r3, #-1]
004efd60: ldrb     r0, [r2, #1]
004efd64: eor      r1, r1, r0
004efd68: strb     r1, [r2, #1]
004efd6c: ldrb     r0, [r3, #-1]
004efd70: sub      r2, r2, #1
004efd74: eor      r1, r1, r0
004efd78: strb     r1, [r3, #-1]
004efd7c: add      r3, r3, #1
004efd80: blo      #0x4efd4c
004efd84: mov      r0, r5
004efd88: add      r1, r4, #0x50
004efd8c: bl       #0x459090
004efd90: mov      r3, #1
004efd94: cmp      r3, #0
004efd98: str      r3, [sp, #4]
004efd9c: bne      #0x4efde0
004efda0: add      r3, r4, #0x51
004efda4: add      r2, r4, #0x52
004efda8: ldrb     r0, [r2, #1]
004efdac: ldrb     r1, [r3, #-1]
004efdb0: cmp      r3, r2
004efdb4: eor      r1, r0, r1
004efdb8: strb     r1, [r3, #-1]
004efdbc: ldrb     r0, [r2, #1]
004efdc0: eor      r1, r1, r0
004efdc4: strb     r1, [r2, #1]
004efdc8: ldrb     r0, [r3, #-1]
004efdcc: sub      r2, r2, #1
004efdd0: eor      r1, r1, r0
004efdd4: strb     r1, [r3, #-1]
004efdd8: add      r3, r3, #1
004efddc: blo      #0x4efda8
004efde0: mov      r0, r5
004efde4: add      r1, r4, #0x54
004efde8: bl       #0x459090
004efdec: mov      r3, #1
004efdf0: cmp      r3, #0
004efdf4: str      r3, [sp, #4]
004efdf8: bne      #0x4efe3c
004efdfc: add      r3, r4, #0x55
004efe00: add      r2, r4, #0x56
004efe04: ldrb     r0, [r2, #1]
004efe08: ldrb     r1, [r3, #-1]
004efe0c: cmp      r3, r2
004efe10: eor      r1, r0, r1
004efe14: strb     r1, [r3, #-1]
004efe18: ldrb     r0, [r2, #1]
004efe1c: eor      r1, r1, r0
004efe20: strb     r1, [r2, #1]
004efe24: ldrb     r0, [r3, #-1]
004efe28: sub      r2, r2, #1
004efe2c: eor      r1, r1, r0
004efe30: strb     r1, [r3, #-1]
004efe34: add      r3, r3, #1
004efe38: blo      #0x4efe04
004efe3c: mov      r0, r5
004efe40: add      r1, r4, #0x58
004efe44: bl       #0x459090
004efe48: mov      r3, #1
004efe4c: cmp      r3, #0
004efe50: str      r3, [sp, #4]
004efe54: bne      #0x4efe98
004efe58: add      r3, r4, #0x59
004efe5c: add      r2, r4, #0x5a
004efe60: ldrb     r0, [r2, #1]
004efe64: ldrb     r1, [r3, #-1]
004efe68: cmp      r3, r2
004efe6c: eor      r1, r0, r1
004efe70: strb     r1, [r3, #-1]
004efe74: ldrb     r0, [r2, #1]
004efe78: eor      r1, r1, r0
004efe7c: strb     r1, [r2, #1]
004efe80: ldrb     r0, [r3, #-1]
004efe84: sub      r2, r2, #1
004efe88: eor      r1, r1, r0
004efe8c: strb     r1, [r3, #-1]
004efe90: add      r3, r3, #1
004efe94: blo      #0x4efe60
004efe98: mov      r0, r5
004efe9c: add      r1, r4, #0x5c
004efea0: bl       #0x459090
004efea4: mov      r3, #1
004efea8: cmp      r3, #0
004efeac: str      r3, [sp, #4]
004efeb0: bne      #0x4efef4
004efeb4: add      r3, r4, #0x5d
004efeb8: add      r2, r4, #0x5e
004efebc: ldrb     r0, [r2, #1]
004efec0: ldrb     r1, [r3, #-1]
004efec4: cmp      r3, r2
004efec8: eor      r1, r0, r1
004efecc: strb     r1, [r3, #-1]
004efed0: ldrb     r0, [r2, #1]
004efed4: eor      r1, r1, r0
004efed8: strb     r1, [r2, #1]
004efedc: ldrb     r0, [r3, #-1]
004efee0: sub      r2, r2, #1
004efee4: eor      r1, r1, r0
004efee8: strb     r1, [r3, #-1]
004efeec: add      r3, r3, #1
004efef0: blo      #0x4efebc
004efef4: mov      r0, r5
004efef8: add      r1, r4, #0x60
004efefc: bl       #0x459090
004eff00: mov      r3, #1
004eff04: cmp      r3, #0
004eff08: str      r3, [sp, #4]
004eff0c: bne      #0x4eff50
004eff10: add      r3, r4, #0x61
004eff14: add      r2, r4, #0x62
004eff18: ldrb     r0, [r2, #1]
004eff1c: ldrb     r1, [r3, #-1]
004eff20: cmp      r3, r2
004eff24: eor      r1, r0, r1
004eff28: strb     r1, [r3, #-1]
004eff2c: ldrb     r0, [r2, #1]
004eff30: eor      r1, r1, r0
004eff34: strb     r1, [r2, #1]
004eff38: ldrb     r0, [r3, #-1]
004eff3c: sub      r2, r2, #1
004eff40: eor      r1, r1, r0
004eff44: strb     r1, [r3, #-1]
004eff48: add      r3, r3, #1
004eff4c: blo      #0x4eff18
004eff50: mov      r0, r5
004eff54: add      r1, r4, #0x64
004eff58: bl       #0x459090
004eff5c: mov      r3, #1
004eff60: cmp      r3, #0
004eff64: str      r3, [sp, #4]
004eff68: bne      #0x4effac
004eff6c: add      r3, r4, #0x65
004eff70: add      r2, r4, #0x66
004eff74: ldrb     r0, [r2, #1]
004eff78: ldrb     r1, [r3, #-1]
004eff7c: cmp      r3, r2
004eff80: eor      r1, r0, r1
004eff84: strb     r1, [r3, #-1]
004eff88: ldrb     r0, [r2, #1]
004eff8c: eor      r1, r1, r0
004eff90: strb     r1, [r2, #1]
004eff94: ldrb     r0, [r3, #-1]
004eff98: sub      r2, r2, #1
004eff9c: eor      r1, r1, r0
004effa0: strb     r1, [r3, #-1]
004effa4: add      r3, r3, #1
004effa8: blo      #0x4eff74
004effac: mov      r0, r5
004effb0: add      r1, r4, #0x68
004effb4: bl       #0x459090
004effb8: mov      r3, #1
004effbc: cmp      r3, #0
004effc0: str      r3, [sp, #4]
004effc4: bne      #0x4f0008
004effc8: add      r3, r4, #0x69
004effcc: add      r2, r4, #0x6a
004effd0: ldrb     r0, [r2, #1]
004effd4: ldrb     r1, [r3, #-1]
004effd8: cmp      r3, r2
004effdc: eor      r1, r0, r1
004effe0: strb     r1, [r3, #-1]
004effe4: ldrb     r0, [r2, #1]
004effe8: eor      r1, r1, r0
004effec: strb     r1, [r2, #1]
004efff0: ldrb     r0, [r3, #-1]
004efff4: sub      r2, r2, #1
004efff8: eor      r1, r1, r0
004efffc: strb     r1, [r3, #-1]
004f0000: add      r3, r3, #1
004f0004: blo      #0x4effd0
004f0008: mov      r0, r5
004f000c: add      r1, r4, #0x6c
004f0010: bl       #0x459090
004f0014: mov      r3, #1
004f0018: cmp      r3, #0
004f001c: str      r3, [sp, #4]
004f0020: bne      #0x4f0064
004f0024: add      r3, r4, #0x6d
004f0028: add      r2, r4, #0x6e
004f002c: ldrb     r0, [r2, #1]
004f0030: ldrb     r1, [r3, #-1]
004f0034: cmp      r3, r2
004f0038: eor      r1, r0, r1
004f003c: strb     r1, [r3, #-1]
004f0040: ldrb     r0, [r2, #1]
004f0044: eor      r1, r1, r0
004f0048: strb     r1, [r2, #1]
004f004c: ldrb     r0, [r3, #-1]
004f0050: sub      r2, r2, #1
004f0054: eor      r1, r1, r0
004f0058: strb     r1, [r3, #-1]
004f005c: add      r3, r3, #1
004f0060: blo      #0x4f002c
004f0064: mov      r0, r5
004f0068: add      r1, r4, #0x70
004f006c: bl       #0x459090
004f0070: mov      r3, #1
004f0074: cmp      r3, #0
004f0078: str      r3, [sp, #4]
004f007c: bne      #0x4f00c0
004f0080: add      r3, r4, #0x71
004f0084: add      r2, r4, #0x72
004f0088: ldrb     r0, [r2, #1]
004f008c: ldrb     r1, [r3, #-1]
004f0090: cmp      r3, r2
004f0094: eor      r1, r0, r1
004f0098: strb     r1, [r3, #-1]
004f009c: ldrb     r0, [r2, #1]
004f00a0: eor      r1, r1, r0
004f00a4: strb     r1, [r2, #1]
004f00a8: ldrb     r0, [r3, #-1]
004f00ac: sub      r2, r2, #1
004f00b0: eor      r1, r1, r0
004f00b4: strb     r1, [r3, #-1]
004f00b8: add      r3, r3, #1
004f00bc: blo      #0x4f0088
004f00c0: mov      r0, r5
004f00c4: add      r1, r4, #0x74
004f00c8: bl       #0x459090
004f00cc: mov      r3, #1
004f00d0: cmp      r3, #0
004f00d4: str      r3, [sp, #4]
004f00d8: bne      #0x4f011c
004f00dc: add      r3, r4, #0x75
004f00e0: add      r2, r4, #0x76
004f00e4: ldrb     r0, [r2, #1]
004f00e8: ldrb     r1, [r3, #-1]
004f00ec: cmp      r3, r2
004f00f0: eor      r1, r0, r1
004f00f4: strb     r1, [r3, #-1]
004f00f8: ldrb     r0, [r2, #1]
004f00fc: eor      r1, r1, r0
004f0100: strb     r1, [r2, #1]
004f0104: ldrb     r0, [r3, #-1]
004f0108: sub      r2, r2, #1
004f010c: eor      r1, r1, r0
004f0110: strb     r1, [r3, #-1]
004f0114: add      r3, r3, #1
004f0118: blo      #0x4f00e4
004f011c: mov      r0, r5
004f0120: add      r1, r4, #0x78
004f0124: bl       #0x459090
004f0128: mov      r3, #1
004f012c: cmp      r3, #0
004f0130: str      r3, [sp, #4]
004f0134: bne      #0x4f0178
004f0138: add      r3, r4, #0x79
004f013c: add      r2, r4, #0x7a
004f0140: ldrb     r0, [r2, #1]
004f0144: ldrb     r1, [r3, #-1]
004f0148: cmp      r3, r2
004f014c: eor      r1, r0, r1
004f0150: strb     r1, [r3, #-1]
004f0154: ldrb     r0, [r2, #1]
004f0158: eor      r1, r1, r0
004f015c: strb     r1, [r2, #1]
004f0160: ldrb     r0, [r3, #-1]
004f0164: sub      r2, r2, #1
004f0168: eor      r1, r1, r0
004f016c: strb     r1, [r3, #-1]
004f0170: add      r3, r3, #1
004f0174: blo      #0x4f0140
004f0178: mov      r0, r5
004f017c: add      r1, r4, #0x7c
004f0180: bl       #0x459090
004f0184: mov      r3, #1
004f0188: cmp      r3, #0
004f018c: str      r3, [sp, #4]
004f0190: bne      #0x4f01d4
004f0194: add      r3, r4, #0x7d
004f0198: add      r2, r4, #0x7e
004f019c: ldrb     r0, [r2, #1]
004f01a0: ldrb     r1, [r3, #-1]
004f01a4: cmp      r3, r2
004f01a8: eor      r1, r0, r1
004f01ac: strb     r1, [r3, #-1]
004f01b0: ldrb     r0, [r2, #1]
004f01b4: eor      r1, r1, r0
004f01b8: strb     r1, [r2, #1]
004f01bc: ldrb     r0, [r3, #-1]
004f01c0: sub      r2, r2, #1
004f01c4: eor      r1, r1, r0
004f01c8: strb     r1, [r3, #-1]
004f01cc: add      r3, r3, #1
004f01d0: blo      #0x4f019c
004f01d4: mov      r0, r5
004f01d8: add      r1, r4, #0x80
004f01dc: bl       #0x459090
004f01e0: mov      r3, #1
004f01e4: cmp      r3, #0
004f01e8: str      r3, [sp, #4]
004f01ec: bne      #0x4f0230
004f01f0: add      r3, r4, #0x81
004f01f4: add      r2, r4, #0x82
004f01f8: ldrb     r0, [r2, #1]
004f01fc: ldrb     r1, [r3, #-1]
004f0200: cmp      r3, r2
004f0204: eor      r1, r0, r1
004f0208: strb     r1, [r3, #-1]
004f020c: ldrb     r0, [r2, #1]
004f0210: eor      r1, r1, r0
004f0214: strb     r1, [r2, #1]
004f0218: ldrb     r0, [r3, #-1]
004f021c: sub      r2, r2, #1
004f0220: eor      r1, r1, r0
004f0224: strb     r1, [r3, #-1]
004f0228: add      r3, r3, #1
004f022c: blo      #0x4f01f8
004f0230: mov      r0, r5
004f0234: add      r1, r4, #0x84
004f0238: bl       #0x3df1a0
004f023c: mov      r3, #1
004f0240: cmp      r3, #0
004f0244: str      r3, [sp, #4]
004f0248: bne      #0x4f028c
004f024c: add      r3, r4, #0x85
004f0250: add      r2, r4, #0x86
004f0254: ldrb     r0, [r2, #1]
004f0258: ldrb     r1, [r3, #-1]
004f025c: cmp      r2, r3
004f0260: eor      r1, r0, r1
004f0264: strb     r1, [r3, #-1]
004f0268: ldrb     r0, [r2, #1]
004f026c: eor      r1, r1, r0
004f0270: strb     r1, [r2, #1]
004f0274: ldrb     r0, [r3, #-1]
004f0278: sub      r2, r2, #1
004f027c: eor      r1, r1, r0
004f0280: strb     r1, [r3, #-1]
004f0284: add      r3, r3, #1
004f0288: bhi      #0x4f0254
004f028c: ldr      r0, [r4, #0x88]
004f0290: cmp      r0, #0
004f0294: beq      #0x4f029c
004f0298: bl       #0x310440
004f029c: ldr      r0, [r4, #0x84]
004f02a0: mov      r1, #1
004f02a4: lsl      r0, r0, #2
004f02a8: bl       #0x31056c
004f02ac: ldr      r3, [r4, #0x84]
004f02b0: str      r0, [r4, #0x88]
004f02b4: cmp      r3, #0
004f02b8: beq      #0x4f033c
004f02bc: mov      r6, #0
004f02c0: mov      r8, #1
004f02c4: lsl      r7, r6, #2
004f02c8: add      r1, r0, r7
004f02cc: mov      r0, r5
004f02d0: bl       #0x459090
004f02d4: str      r8, [sp, #4]
004f02d8: cmp      r8, #0
004f02dc: ldr      r3, [r4, #0x88]
004f02e0: bne      #0x4f0328
004f02e4: add      r7, r3, r7
004f02e8: add      r3, r7, #2
004f02ec: add      r7, r7, #1
004f02f0: ldrb     r1, [r3, #1]
004f02f4: ldrb     r2, [r7, #-1]
004f02f8: cmp      r7, r3
004f02fc: eor      r2, r1, r2
004f0300: strb     r2, [r7, #-1]
004f0304: ldrb     r1, [r3, #1]
004f0308: eor      r2, r2, r1
004f030c: strb     r2, [r3, #1]
004f0310: ldrb     r1, [r7, #-1]
004f0314: sub      r3, r3, #1
004f0318: eor      r2, r2, r1
004f031c: strb     r2, [r7, #-1]
004f0320: add      r7, r7, #1
004f0324: blo      #0x4f02f0
004f0328: ldr      r3, [r4, #0x84]
004f032c: add      r6, r6, #1
004f0330: cmp      r3, r6
004f0334: ldrhi    r0, [r4, #0x88]
004f0338: bhi      #0x4f02c4
004f033c: mov      r0, r5
004f0340: add      r1, r4, #0x8c
004f0344: bl       #0x459090
004f0348: mov      r3, #1
004f034c: cmp      r3, #0
004f0350: str      r3, [sp, #4]
004f0354: bne      #0x4f0398
004f0358: add      r3, r4, #0x8d
004f035c: add      r2, r4, #0x8e
004f0360: ldrb     r0, [r2, #1]
004f0364: ldrb     r1, [r3, #-1]
004f0368: cmp      r2, r3
004f036c: eor      r1, r0, r1
004f0370: strb     r1, [r3, #-1]
004f0374: ldrb     r0, [r2, #1]
004f0378: eor      r1, r1, r0
004f037c: strb     r1, [r2, #1]
004f0380: ldrb     r0, [r3, #-1]
004f0384: sub      r2, r2, #1
004f0388: eor      r1, r1, r0
004f038c: strb     r1, [r3, #-1]
004f0390: add      r3, r3, #1
004f0394: bhi      #0x4f0360
004f0398: mov      r0, r5
004f039c: add      r1, r4, #0x90
004f03a0: bl       #0x459090
004f03a4: mov      r3, #1
004f03a8: cmp      r3, #0
004f03ac: str      r3, [sp, #4]
004f03b0: bne      #0x4f03f4
004f03b4: add      r3, r4, #0x91
004f03b8: add      r2, r4, #0x92
004f03bc: ldrb     r0, [r2, #1]
004f03c0: ldrb     r1, [r3, #-1]
004f03c4: cmp      r2, r3
004f03c8: eor      r1, r0, r1
004f03cc: strb     r1, [r3, #-1]
004f03d0: ldrb     r0, [r2, #1]
004f03d4: eor      r1, r1, r0
004f03d8: strb     r1, [r2, #1]
004f03dc: ldrb     r0, [r3, #-1]
004f03e0: sub      r2, r2, #1
004f03e4: eor      r1, r1, r0
004f03e8: strb     r1, [r3, #-1]
004f03ec: add      r3, r3, #1
004f03f0: bhi      #0x4f03bc
004f03f4: mov      r0, r5
004f03f8: add      r1, r4, #0x94
004f03fc: bl       #0x459090
004f0400: mov      r3, #1
004f0404: cmp      r3, #0
004f0408: str      r3, [sp, #4]
004f040c: bne      #0x4f0450
004f0410: add      r3, r4, #0x95
004f0414: add      r2, r4, #0x96
004f0418: ldrb     r0, [r2, #1]
004f041c: ldrb     r1, [r3, #-1]
004f0420: cmp      r3, r2
004f0424: eor      r1, r0, r1
004f0428: strb     r1, [r3, #-1]
004f042c: ldrb     r0, [r2, #1]
004f0430: eor      r1, r1, r0
004f0434: strb     r1, [r2, #1]
004f0438: ldrb     r0, [r3, #-1]
004f043c: sub      r2, r2, #1
004f0440: eor      r1, r1, r0
004f0444: strb     r1, [r3, #-1]
004f0448: add      r3, r3, #1
004f044c: blo      #0x4f0418
004f0450: mov      r0, r5
004f0454: add      r1, r4, #0x98
004f0458: bl       #0x459090
004f045c: mov      r3, #1
004f0460: cmp      r3, #0
004f0464: str      r3, [sp, #4]
004f0468: bne      #0x4f04ac
004f046c: add      r3, r4, #0x99
004f0470: add      r2, r4, #0x9a
004f0474: ldrb     r0, [r2, #1]
004f0478: ldrb     r1, [r3, #-1]
004f047c: cmp      r3, r2
004f0480: eor      r1, r0, r1
004f0484: strb     r1, [r3, #-1]
004f0488: ldrb     r0, [r2, #1]
004f048c: eor      r1, r1, r0
004f0490: strb     r1, [r2, #1]
004f0494: ldrb     r0, [r3, #-1]
004f0498: sub      r2, r2, #1
004f049c: eor      r1, r1, r0
004f04a0: strb     r1, [r3, #-1]
004f04a4: add      r3, r3, #1
004f04a8: blo      #0x4f0474
004f04ac: mov      r0, r5
004f04b0: add      r1, r4, #0x9c
004f04b4: bl       #0x459090
004f04b8: mov      r3, #1
004f04bc: cmp      r3, #0
004f04c0: str      r3, [sp, #4]
004f04c4: bne      #0x4f0508
004f04c8: add      r3, r4, #0x9e
004f04cc: add      r4, r4, #0x9d
004f04d0: ldrb     r1, [r3, #1]
004f04d4: ldrb     r2, [r4, #-1]
004f04d8: cmp      r3, r4
004f04dc: eor      r2, r1, r2
004f04e0: strb     r2, [r4, #-1]
004f04e4: ldrb     r1, [r3, #1]
004f04e8: eor      r2, r2, r1
004f04ec: strb     r2, [r3, #1]
004f04f0: ldrb     r1, [r4, #-1]
004f04f4: sub      r3, r3, #1
004f04f8: eor      r2, r2, r1
004f04fc: strb     r2, [r4, #-1]
004f0500: add      r4, r4, #1
004f0504: bhi      #0x4f04d0
004f0508: add      sp, sp, #8
004f050c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7Structs8AnimStep4readEP11IStreamBase
004ec9f0: push     {r4, r5, r6, r7, r8, lr}
004ec9f4: mov      r4, r0
004ec9f8: sub      sp, sp, #8
004ec9fc: mov      r0, r1
004eca00: mov      r7, r1
004eca04: add      r1, r4, #4
004eca08: bl       #0x4db89c
004eca0c: mov      r0, r7
004eca10: add      r1, r4, #8
004eca14: bl       #0x459090
004eca18: mov      r3, #1
004eca1c: cmp      r3, #0
004eca20: str      r3, [sp, #4]
004eca24: bne      #0x4eca68
004eca28: add      r3, r4, #9
004eca2c: add      r2, r4, #0xa
004eca30: ldrb     r0, [r2, #1]
004eca34: ldrb     r1, [r3, #-1]
004eca38: cmp      r3, r2
004eca3c: eor      r1, r0, r1
004eca40: strb     r1, [r3, #-1]
004eca44: ldrb     r0, [r2, #1]
004eca48: eor      r1, r1, r0
004eca4c: strb     r1, [r2, #1]
004eca50: ldrb     r0, [r3, #-1]
004eca54: sub      r2, r2, #1
004eca58: eor      r1, r1, r0
004eca5c: strb     r1, [r3, #-1]
004eca60: add      r3, r3, #1
004eca64: blo      #0x4eca30
004eca68: mov      r0, r7
004eca6c: add      r1, r4, #0xc
004eca70: bl       #0x459090
004eca74: mov      r3, #1
004eca78: cmp      r3, #0
004eca7c: str      r3, [sp, #4]
004eca80: bne      #0x4ecac4
004eca84: add      r3, r4, #0xd
004eca88: add      r2, r4, #0xe
004eca8c: ldrb     r0, [r2, #1]
004eca90: ldrb     r1, [r3, #-1]
004eca94: cmp      r3, r2
004eca98: eor      r1, r0, r1
004eca9c: strb     r1, [r3, #-1]
004ecaa0: ldrb     r0, [r2, #1]
004ecaa4: eor      r1, r1, r0
004ecaa8: strb     r1, [r2, #1]
004ecaac: ldrb     r0, [r3, #-1]
004ecab0: sub      r2, r2, #1
004ecab4: eor      r1, r1, r0
004ecab8: strb     r1, [r3, #-1]
004ecabc: add      r3, r3, #1
004ecac0: blo      #0x4eca8c
004ecac4: mov      r0, r7
004ecac8: add      r1, r4, #0x10
004ecacc: bl       #0x459090
004ecad0: mov      r3, #1
004ecad4: cmp      r3, #0
004ecad8: str      r3, [sp, #4]
004ecadc: bne      #0x4ecb20
004ecae0: add      r3, r4, #0x11
004ecae4: add      r2, r4, #0x12
004ecae8: ldrb     r0, [r2, #1]
004ecaec: ldrb     r1, [r3, #-1]
004ecaf0: cmp      r3, r2
004ecaf4: eor      r1, r0, r1
004ecaf8: strb     r1, [r3, #-1]
004ecafc: ldrb     r0, [r2, #1]
004ecb00: eor      r1, r1, r0
004ecb04: strb     r1, [r2, #1]
004ecb08: ldrb     r0, [r3, #-1]
004ecb0c: sub      r2, r2, #1
004ecb10: eor      r1, r1, r0
004ecb14: strb     r1, [r3, #-1]
004ecb18: add      r3, r3, #1
004ecb1c: blo      #0x4ecae8
004ecb20: add      r1, r4, #0x14
004ecb24: mov      r0, r7
004ecb28: bl       #0x4db89c
004ecb2c: mov      r0, r7
004ecb30: add      r1, r4, #0x18
004ecb34: bl       #0x459090
004ecb38: mov      r3, #1
004ecb3c: cmp      r3, #0
004ecb40: str      r3, [sp, #4]
004ecb44: bne      #0x4ecb88
004ecb48: add      r3, r4, #0x19
004ecb4c: add      r2, r4, #0x1a
004ecb50: ldrb     r0, [r2, #1]
004ecb54: ldrb     r1, [r3, #-1]
004ecb58: cmp      r3, r2
004ecb5c: eor      r1, r0, r1
004ecb60: strb     r1, [r3, #-1]
004ecb64: ldrb     r0, [r2, #1]
004ecb68: eor      r1, r1, r0
004ecb6c: strb     r1, [r2, #1]
004ecb70: ldrb     r0, [r3, #-1]
004ecb74: sub      r2, r2, #1
004ecb78: eor      r1, r1, r0
004ecb7c: strb     r1, [r3, #-1]
004ecb80: add      r3, r3, #1
004ecb84: blo      #0x4ecb50
004ecb88: add      r1, r4, #0x1c
004ecb8c: mov      r0, r7
004ecb90: bl       #0x4db89c
004ecb94: mov      r0, r7
004ecb98: add      r1, r4, #0x20
004ecb9c: bl       #0x3df1a0
004ecba0: mov      r3, #1
004ecba4: cmp      r3, #0
004ecba8: str      r3, [sp, #4]
004ecbac: bne      #0x4ecbf0
004ecbb0: add      r3, r4, #0x21
004ecbb4: add      r2, r4, #0x22
004ecbb8: ldrb     r0, [r2, #1]
004ecbbc: ldrb     r1, [r3, #-1]
004ecbc0: cmp      r3, r2
004ecbc4: eor      r1, r0, r1
004ecbc8: strb     r1, [r3, #-1]
004ecbcc: ldrb     r0, [r2, #1]
004ecbd0: eor      r1, r1, r0
004ecbd4: strb     r1, [r2, #1]
004ecbd8: ldrb     r0, [r3, #-1]
004ecbdc: sub      r2, r2, #1
004ecbe0: eor      r1, r1, r0
004ecbe4: strb     r1, [r3, #-1]
004ecbe8: add      r3, r3, #1
004ecbec: blo      #0x4ecbb8
004ecbf0: ldr      r0, [r4, #0x24]
004ecbf4: cmp      r0, #0
004ecbf8: beq      #0x4ecc00
004ecbfc: bl       #0x310440
004ecc00: ldr      r0, [r4, #0x20]
004ecc04: mov      r1, #1
004ecc08: lsl      r0, r0, #2
004ecc0c: bl       #0x31056c
004ecc10: ldr      r3, [r4, #0x20]
004ecc14: str      r0, [r4, #0x24]
004ecc18: cmp      r3, #0
004ecc1c: beq      #0x4ecca0
004ecc20: mov      r5, #0
004ecc24: mov      r8, #1
004ecc28: lsl      r6, r5, #2
004ecc2c: add      r1, r0, r6
004ecc30: mov      r0, r7
004ecc34: bl       #0x459090
004ecc38: str      r8, [sp, #4]
004ecc3c: cmp      r8, #0
004ecc40: ldr      r3, [r4, #0x24]
004ecc44: bne      #0x4ecc8c
004ecc48: add      r6, r3, r6
004ecc4c: add      r3, r6, #2
004ecc50: add      r6, r6, #1
004ecc54: ldrb     r1, [r3, #1]
004ecc58: ldrb     r2, [r6, #-1]
004ecc5c: cmp      r6, r3
004ecc60: eor      r2, r1, r2
004ecc64: strb     r2, [r6, #-1]
004ecc68: ldrb     r1, [r3, #1]
004ecc6c: eor      r2, r2, r1
004ecc70: strb     r2, [r3, #1]
004ecc74: ldrb     r1, [r6, #-1]
004ecc78: sub      r3, r3, #1
004ecc7c: eor      r2, r2, r1
004ecc80: strb     r2, [r6, #-1]
004ecc84: add      r6, r6, #1
004ecc88: blo      #0x4ecc54
004ecc8c: ldr      r3, [r4, #0x20]
004ecc90: add      r5, r5, #1
004ecc94: cmp      r3, r5
004ecc98: ldrhi    r0, [r4, #0x24]
004ecc9c: bhi      #0x4ecc28
004ecca0: mov      r0, r7
004ecca4: add      r1, r4, #0x28
004ecca8: bl       #0x459090
004eccac: mov      r3, #1
004eccb0: cmp      r3, #0
004eccb4: str      r3, [sp, #4]
004eccb8: bne      #0x4eccfc
004eccbc: add      r3, r4, #0x29
004eccc0: add      r2, r4, #0x2a
004eccc4: ldrb     r0, [r2, #1]
004eccc8: ldrb     r1, [r3, #-1]
004ecccc: cmp      r3, r2
004eccd0: eor      r1, r0, r1
004eccd4: strb     r1, [r3, #-1]
004eccd8: ldrb     r0, [r2, #1]
004eccdc: eor      r1, r1, r0
004ecce0: strb     r1, [r2, #1]
004ecce4: ldrb     r0, [r3, #-1]
004ecce8: sub      r2, r2, #1
004eccec: eor      r1, r1, r0
004eccf0: strb     r1, [r3, #-1]
004eccf4: add      r3, r3, #1
004eccf8: blo      #0x4eccc4
004eccfc: mov      r0, r7
004ecd00: add      r1, r4, #0x2c
004ecd04: bl       #0x459090
004ecd08: mov      r3, #1
004ecd0c: cmp      r3, #0
004ecd10: str      r3, [sp, #4]
004ecd14: bne      #0x4ecd58
004ecd18: add      r3, r4, #0x2d
004ecd1c: add      r2, r4, #0x2e
004ecd20: ldrb     r0, [r2, #1]
004ecd24: ldrb     r1, [r3, #-1]
004ecd28: cmp      r3, r2
004ecd2c: eor      r1, r0, r1
004ecd30: strb     r1, [r3, #-1]
004ecd34: ldrb     r0, [r2, #1]
004ecd38: eor      r1, r1, r0
004ecd3c: strb     r1, [r2, #1]
004ecd40: ldrb     r0, [r3, #-1]
004ecd44: sub      r2, r2, #1
004ecd48: eor      r1, r1, r0
004ecd4c: strb     r1, [r3, #-1]
004ecd50: add      r3, r3, #1
004ecd54: blo      #0x4ecd20
004ecd58: mov      r0, r7
004ecd5c: add      r1, r4, #0x30
004ecd60: bl       #0x4db94c
004ecd64: mov      r3, #1
004ecd68: cmp      r3, #0
004ecd6c: str      r3, [sp, #4]
004ecd70: bne      #0x4ecdb4
004ecd74: add      r3, r4, #0x31
004ecd78: add      r2, r4, #0x32
004ecd7c: ldrb     r0, [r2, #1]
004ecd80: ldrb     r1, [r3, #-1]
004ecd84: cmp      r3, r2
004ecd88: eor      r1, r0, r1
004ecd8c: strb     r1, [r3, #-1]
004ecd90: ldrb     r0, [r2, #1]
004ecd94: eor      r1, r1, r0
004ecd98: strb     r1, [r2, #1]
004ecd9c: ldrb     r0, [r3, #-1]
004ecda0: sub      r2, r2, #1
004ecda4: eor      r1, r1, r0
004ecda8: strb     r1, [r3, #-1]
004ecdac: add      r3, r3, #1
004ecdb0: blo      #0x4ecd7c
004ecdb4: mov      r0, r7
004ecdb8: add      r1, r4, #0x34
004ecdbc: bl       #0x4db89c
004ecdc0: add      sp, sp, #8
004ecdc4: pop      {r4, r5, r6, r7, r8, pc}
