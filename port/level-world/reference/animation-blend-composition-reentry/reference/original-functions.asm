
# _ZN14AnimApplicator10ResetDeltaEjRKN6glitch4core8vector3dIfEE
003644cc: ldr      r3, [r2]
003644d0: mov      ip, #0
003644d4: str      r3, [r0, #0x18]
003644d8: ldr      r3, [r2, #4]
003644dc: str      r3, [r0, #0x1c]
003644e0: ldr      r3, [r2, #8]
003644e4: str      r1, [r0, #0x14]
003644e8: str      ip, [r0, #0x2c]
003644ec: str      r3, [r0, #0x20]
003644f0: str      ip, [r0, #0x24]
003644f4: str      ip, [r0, #0x28]
003644f8: bx       lr

# _ZN6glitch7collada19CTimelineController6jumpToEi
00666f10: push     {r4, lr}
00666f14: mov      r4, r0
00666f18: str      r1, [r0, #4]
00666f1c: mov      r0, r1
00666f20: bl       #0x30e964
00666f24: mov      r1, #0x44000000
00666f28: add      r1, r1, #0x7a0000
00666f2c: bl       #0x30ec94
00666f30: mov      r3, #0
00666f34: strb     r3, [r4, #0x3d]
00666f38: str      r0, [r4, #0x2c]
00666f3c: strb     r3, [r4, #0x3c]
00666f40: pop      {r4, pc}

# _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00369160: push     {r4, lr}
00369164: subs     r4, r0, #0
00369168: bne      #0x369174
0036916c: mov      r0, #0
00369170: pop      {r4, pc}
00369174: ldr      r3, [r4]
00369178: mov      lr, pc
0036917c: ldr      pc, [r3, #0x24]
00369180: sub      r0, r0, #0xb
00369184: cmp      r0, #4
00369188: addls    pc, pc, r0, lsl #2
0036918c: b        #0x36916c
00369190: b        #0x3691a4
00369194: b        #0x3691ac
00369198: b        #0x3691a4
0036919c: b        #0x3691b4
003691a0: b        #0x3691bc
003691a4: add      r0, r4, #0x58
003691a8: pop      {r4, pc}
003691ac: add      r0, r4, #0x88
003691b0: pop      {r4, pc}
003691b4: add      r0, r4, #0xd8
003691b8: pop      {r4, pc}
003691bc: add      r0, r4, #0x84
003691c0: pop      {r4, pc}

# _ZN6glitch7collada19CTimelineController7setClipEi
00666f7c: push     {r4, r5, r6, r7, r8, lr}
00666f80: mov      r3, #0
00666f84: str      r1, [r0, #0x38]
00666f88: strb     r3, [r0, #0x3d]
00666f8c: strb     r3, [r0, #0x3c]
00666f90: ldr      r3, [r0]
00666f94: mov      r4, r0
00666f98: mov      lr, pc
00666f9c: ldr      pc, [r3, #0x2c]
00666fa0: ldr      r3, [r4]
00666fa4: str      r0, [r4, #0x10]
00666fa8: mov      r0, r4
00666fac: mov      lr, pc
00666fb0: ldr      pc, [r3, #0x30]
00666fb4: mov      r7, r0
00666fb8: str      r0, [r4, #0x14]
00666fbc: ldr      r0, [r4, #0x10]
00666fc0: bl       #0x30e964
00666fc4: mov      r1, #0x44000000
00666fc8: add      r1, r1, #0x7a0000
00666fcc: bl       #0x30ec94
00666fd0: ldr      r6, [r4, #0x10]
00666fd4: mov      r5, r0
00666fd8: str      r0, [r4, #0x20]
00666fdc: rsb      r0, r6, r7
00666fe0: bl       #0x30e964
00666fe4: mov      r1, #0x44000000
00666fe8: add      r1, r1, #0x7a0000
00666fec: bl       #0x30ec94
00666ff0: str      r6, [r4, #4]
00666ff4: str      r0, [r4, #0x24]
00666ff8: str      r5, [r4, #0x2c]
00666ffc: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12CharAnimator6UpdateEv
003caf3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003caf40: ldr      r5, [pc, #0x37c]
003caf44: ldr      r6, [pc, #0x37c]
003caf48: mov      r4, r0
003caf4c: add      r5, pc, r5
003caf50: ldr      r3, [r5, r6]
003caf54: ldr      r0, [pc, #0x370]
003caf58: sub      sp, sp, #0x78
003caf5c: ldr      r3, [r3]
003caf60: add      r0, pc, r0
003caf64: str      r3, [sp, #0x74]
003caf68: bl       #0x3136b4
003caf6c: ldr      r3, [r4, #4]
003caf70: ldr      r3, [r3, #0x520]
003caf74: tst      r3, #0x200
003caf78: bne      #0x3cafb8
003caf7c: ldrb     r3, [r4, #0x5c]
003caf80: cmp      r3, #0
003caf84: bne      #0x3cb128
003caf88: ldr      r0, [pc, #0x340]
003caf8c: mov      r3, #0
003caf90: strb     r3, [r4, #0x5c]
003caf94: add      r0, pc, r0
003caf98: bl       #0x3136b8
003caf9c: ldr      r3, [r5, r6]
003cafa0: ldr      r2, [sp, #0x74]
003cafa4: ldr      r3, [r3]
003cafa8: cmp      r2, r3
003cafac: bne      #0x3cb2c0
003cafb0: add      sp, sp, #0x78
003cafb4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003cafb8: ldrb     r3, [r4, #0x5c]
003cafbc: cmp      r3, #0
003cafc0: beq      #0x3cb11c
003cafc4: ldrb     r3, [r4, #0x4a]
003cafc8: mov      r2, #1
003cafcc: strb     r2, [r4, #0x5c]
003cafd0: cmp      r3, #0
003cafd4: movne    r3, #0
003cafd8: strbne   r3, [r4, #0x4a]
003cafdc: strbne   r2, [r4, #0x49]
003cafe0: beq      #0x3cb134
003cafe4: ldr      sl, [r4, #0x2c]
003cafe8: ldr      r3, [pc, #0x2e4]
003cafec: mov      r8, #0xc
003caff0: mla      r8, r8, sl, r4
003caff4: ldr      r3, [r5, r3]
003caff8: ldr      r2, [r8, #8]
003caffc: mov      r7, #0x14
003cb000: ldr      r3, [r3]
003cb004: ldr      r0, [r4, #4]
003cb008: mov      r1, #0x27
003cb00c: mla      r7, r7, r2, r3
003cb010: mov      r2, #0
003cb014: bl       #0x3a4d5c
003cb018: ldr      r3, [r7, #0x10]
003cb01c: cmp      r3, #1
003cb020: bne      #0x3cb144
003cb024: ldr      r3, [r8, #0x10]
003cb028: ldr      r2, [r7, #8]
003cb02c: add      r3, r3, #1
003cb030: cmp      r3, r2
003cb034: beq      #0x3cb144
003cb038: mov      r8, #0xc
003cb03c: mla      r8, r8, sl, r4
003cb040: str      r3, [r8, #0x10]
003cb044: ldr      r2, [r7, #8]
003cb048: cmp      r2, r3
003cb04c: bhi      #0x3cb1e4
003cb050: mov      r2, #0xc
003cb054: mla      r2, r2, sl, r4
003cb058: ldr      r3, [r2, #0xc]
003cb05c: cmp      r3, #0
003cb060: beq      #0x3cb174
003cb064: subgt    r3, r3, #1
003cb068: strgt    r3, [r2, #0xc]
003cb06c: ldr      r3, [pc, #0x264]
003cb070: add      r7, sp, #0x44
003cb074: ldr      r8, [r5, r3]
003cb078: mov      r0, r8
003cb07c: bl       #0x337888
003cb080: ldr      r1, [pc, #0x254]
003cb084: add      r2, sp, #0xc
003cb088: mov      r0, r7
003cb08c: add      r1, pc, r1
003cb090: bl       #0x3140ec
003cb094: mov      r1, r7
003cb098: mov      r0, r8
003cb09c: bl       #0x337a88
003cb0a0: mov      r0, r7
003cb0a4: bl       #0x3139ac
003cb0a8: mov      r2, #0
003cb0ac: ldr      r0, [r4, #4]
003cb0b0: mov      r1, #0x23
003cb0b4: bl       #0x3a4d5c
003cb0b8: ldr      r2, [r4, #0x50]
003cb0bc: mov      r3, #0xc
003cb0c0: mla      r3, r3, sl, r4
003cb0c4: cmn      r2, #1
003cb0c8: ldr      r7, [r3, #0xc]
003cb0cc: beq      #0x3cb2ac
003cb0d0: mov      r3, #0xc
003cb0d4: mla      sl, r3, sl, r4
003cb0d8: cmp      r7, #0
003cb0dc: mvnlt    r3, #0
003cb0e0: strlt    r3, [sl, #0xc]
003cb0e4: strge    r7, [sl, #0xc]
003cb0e8: mov      r3, #0
003cb0ec: strb     r3, [r4, #0x49]
003cb0f0: ldr      r1, [r4, #0x50]
003cb0f4: cmn      r1, #1
003cb0f8: beq      #0x3cb10c
003cb0fc: mov      r0, r4
003cb100: bl       #0x3cacb0
003cb104: mvn      r3, #0
003cb108: str      r3, [r4, #0x50]
003cb10c: ldr      r0, [pc, #0x1cc]
003cb110: add      r0, pc, r0
003cb114: bl       #0x3136b8
003cb118: b        #0x3caf9c
003cb11c: mov      r0, r4
003cb120: bl       #0x3c9168
003cb124: b        #0x3cafc4
003cb128: mov      r0, r4
003cb12c: bl       #0x3c91c8
003cb130: b        #0x3caf88
003cb134: ldrb     r3, [r4, #0x49]
003cb138: cmp      r3, #0
003cb13c: beq      #0x3cb0f0
003cb140: b        #0x3cafe4
003cb144: ldr      r0, [r4, #4]
003cb148: mov      r1, #0x25
003cb14c: mov      r2, #0
003cb150: bl       #0x3a4d5c
003cb154: ldr      r3, [r7, #0x10]
003cb158: cmp      r3, #1
003cb15c: bne      #0x3cb050
003cb160: add      r3, r3, #0xb
003cb164: mla      r3, r3, sl, r4
003cb168: ldr      r3, [r3, #0x10]
003cb16c: add      r3, r3, #1
003cb170: b        #0x3cb038
003cb174: ldr      r3, [r4, #0x2c]
003cb178: cmp      r3, #0
003cb17c: bne      #0x3cb258
003cb180: ldrb     r7, [r4, #0x48]
003cb184: cmp      r7, #0
003cb188: bne      #0x3cb0e8
003cb18c: ldr      r3, [pc, #0x144]
003cb190: add      r8, sp, #0x14
003cb194: ldr      sl, [r5, r3]
003cb198: mov      r0, sl
003cb19c: bl       #0x337888
003cb1a0: ldr      r1, [pc, #0x13c]
003cb1a4: add      r2, sp, #4
003cb1a8: mov      r0, r8
003cb1ac: add      r1, pc, r1
003cb1b0: bl       #0x3140ec
003cb1b4: mov      r1, r8
003cb1b8: mov      r0, sl
003cb1bc: bl       #0x337a88
003cb1c0: mov      r0, r8
003cb1c4: bl       #0x3139ac
003cb1c8: mov      r3, #1
003cb1cc: strb     r3, [r4, #0x48]
003cb1d0: mov      r2, r7
003cb1d4: ldr      r0, [r4, #4]
003cb1d8: mov      r1, #0x22
003cb1dc: bl       #0x3a4d5c
003cb1e0: b        #0x3cb0e8
003cb1e4: ldr      r3, [pc, #0xec]
003cb1e8: add      sl, sp, #0x5c
003cb1ec: ldr      sb, [r5, r3]
003cb1f0: mov      r0, sb
003cb1f4: bl       #0x337888
003cb1f8: ldr      r1, [pc, #0xe8]
003cb1fc: add      r2, sp, #0x10
003cb200: mov      r0, sl
003cb204: add      r1, pc, r1
003cb208: bl       #0x3140ec
003cb20c: mov      r1, sl
003cb210: mov      r0, sb
003cb214: bl       #0x337a88
003cb218: mov      r0, sl
003cb21c: bl       #0x3139ac
003cb220: mov      r1, #0x23
003cb224: ldr      r0, [r4, #4]
003cb228: mov      r2, #0
003cb22c: bl       #0x3a4d5c
003cb230: ldr      r1, [r8, #0x10]
003cb234: ldr      r3, [r7, #8]
003cb238: cmp      r1, r3
003cb23c: bhs      #0x3cb24c
003cb240: mov      r0, r4
003cb244: bl       #0x3ca79c
003cb248: b        #0x3cb0e8
003cb24c: mov      r0, r4
003cb250: bl       #0x3caf3c
003cb254: b        #0x3cb0e8
003cb258: ldr      r3, [pc, #0x78]
003cb25c: add      r7, sp, #0x2c
003cb260: ldr      r8, [r5, r3]
003cb264: mov      r0, r8
003cb268: bl       #0x337888
003cb26c: ldr      r1, [pc, #0x78]
003cb270: add      r2, sp, #8
003cb274: mov      r0, r7
003cb278: add      r1, pc, r1
003cb27c: bl       #0x3140ec
003cb280: mov      r1, r7
003cb284: mov      r0, r8
003cb288: bl       #0x337a88
003cb28c: mov      r0, r7
003cb290: bl       #0x3139ac
003cb294: ldr      r3, [r4, #0x2c]
003cb298: mov      r0, r4
003cb29c: sub      r3, r3, #1
003cb2a0: str      r3, [r4, #0x2c]
003cb2a4: bl       #0x3caf3c
003cb2a8: b        #0x3cb0e8
003cb2ac: ldr      r1, [r3, #8]
003cb2b0: mov      r0, r4
003cb2b4: ldr      r2, [r4, #0x2c]
003cb2b8: bl       #0x3cab38
003cb2bc: b        #0x3cb0d0
003cb2c0: bl       #0x30e310
003cb2c4: subseq   sb, ip, r4, asr #22
003cb2c8: andeq    r4, r0, ip, lsr #1
003cb2cc: subeq    sl, pc, r0, asr #4
003cb2d0: subeq    sl, pc, ip, lsl #4
003cb2d4: andeq    r3, r0, ip, ror ip
003cb2d8: andeq    r0, r0, r4, lsl #17
003cb2dc: subeq    sb, pc, r4, asr #31
003cb2e0: umaaleq  sl, pc, r0, r0
003cb2e4: subeq    sb, pc, r4, lsr #29
003cb2e8: subeq    sb, pc, ip, asr #28
003cb2ec: ldrdeq   sb, sl, [pc], #-0xd8

# _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
0065f8c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065f8cc: mov      r4, r0
0065f8d0: mov      r5, r1
0065f8d4: bl       #0x65f0fc
0065f8d8: ldr      r3, [r4, #0x24]
0065f8dc: str      r0, [r4, #0x14]
0065f8e0: mov      r1, r5
0065f8e4: ldr      r2, [r3, #0x3c]
0065f8e8: mov      r0, r3
0065f8ec: str      r5, [r4, #0x50]
0065f8f0: mul      r3, r2, r5
0065f8f4: str      r3, [r4, #0x4c]
0065f8f8: bl       #0x65f0b4
0065f8fc: bl       #0x60e334
0065f900: ldr      r3, [r4]
0065f904: mov      r6, r0
0065f908: mov      r0, r4
0065f90c: mov      lr, pc
0065f910: ldr      pc, [r3, #0x44]
0065f914: cmp      r0, #0
0065f918: beq      #0x65f9fc
0065f91c: ldr      r8, [r6]
0065f920: cmp      r8, #0
0065f924: beq      #0x65f968
0065f928: ldr      r3, [r4]
0065f92c: mov      r0, r4
0065f930: mov      lr, pc
0065f934: ldr      pc, [r3, #0x44]
0065f938: str      r6, [r0, #0x34]
0065f93c: ldr      r2, [r6]
0065f940: cmp      r2, #0
0065f944: moveq    r1, #1
0065f948: streq    r1, [r0, #0x14]
0065f94c: streq    r2, [r0, #0x10]
0065f950: beq      #0x65f9d4
0065f954: ldr      r3, [r0]
0065f958: mov      r1, #0
0065f95c: mov      lr, pc
0065f960: ldr      pc, [r3, #0x10]
0065f964: b        #0x65f9d4
0065f968: ldr      r3, [r4]
0065f96c: mov      r0, r4
0065f970: mov      lr, pc
0065f974: ldr      pc, [r3, #0x44]
0065f978: mov      r7, #1
0065f97c: str      r8, [r0, #0x10]
0065f980: str      r8, [r0, #0x34]
0065f984: str      r7, [r0, #0x14]
0065f988: ldr      r3, [r4]
0065f98c: mov      r0, r4
0065f990: mov      lr, pc
0065f994: ldr      pc, [r3, #0x44]
0065f998: ldr      r3, [r0]
0065f99c: mov      r8, r0
0065f9a0: mov      r1, r5
0065f9a4: mov      r0, r4
0065f9a8: ldr      r6, [r3, #0x50]
0065f9ac: bl       #0x65f104
0065f9b0: mov      r1, r5
0065f9b4: mov      sl, r0
0065f9b8: mov      r0, r4
0065f9bc: bl       #0x65f10c
0065f9c0: mov      r1, sl
0065f9c4: mov      r2, r0
0065f9c8: mov      r3, r7
0065f9cc: mov      r0, r8
0065f9d0: blx      r6
0065f9d4: mov      r1, r5
0065f9d8: ldr      r0, [r4, #0x24]
0065f9dc: bl       #0x65f0b4
0065f9e0: ldr      r3, [r0]
0065f9e4: mov      r0, r4
0065f9e8: ldr      r3, [r3, #0x24]
0065f9ec: ldr      r3, [r3, #0x20]
0065f9f0: ldr      r1, [r3, #0x2c]
0065f9f4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0065f9f8: b        #0x60fab8
0065f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN15AnimatorBlender17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366628: push     {r4, r5, r6, lr}
0036662c: ldr      r2, [r0, #0x70]
00366630: ldr      r3, [r0, #0x28]
00366634: mov      r4, r0
00366638: mov      r6, r1
0036663c: ldr      r3, [r3, r2, lsl #2]
00366640: mov      r0, r3
00366644: ldr      r3, [r3]
00366648: mov      lr, pc
0036664c: ldr      pc, [r3, #0x44]
00366650: cmp      r0, r6
00366654: mov      r5, r0
00366658: beq      #0x366660
0036665c: pop      {r4, r5, r6, pc}
00366660: cmp      r0, #0
00366664: beq      #0x3666bc
00366668: mov      r1, #0x44000000
0036666c: add      r1, r1, #0x7a0000
00366670: ldr      r0, [r0, #0x1c]
00366674: bl       #0x30ed6c
00366678: bl       #0x30e4cc
0036667c: mov      r1, #0x44000000
00366680: mov      r6, r0
00366684: add      r1, r1, #0x7a0000
00366688: ldr      r0, [r5, #0x2c]
0036668c: bl       #0x30ed6c
00366690: bl       #0x30e4cc
00366694: ldr      r3, [r5, #4]
00366698: rsb      r0, r3, r0
0036669c: cmp      r0, r6
003666a0: movge    r3, #0
003666a4: movlt    r3, #1
003666a8: cmp      r0, #0
003666ac: movlt    r3, #0
003666b0: cmp      r3, #0
003666b4: rsbne    r3, r0, r6
003666b8: str      r3, [r4, #0x98]
003666bc: mov      r3, #1
003666c0: strb     r3, [r4, #0xb8]
003666c4: pop      {r4, r5, r6, pc}

# _ZN13RootSceneNode7NewAnimEb
0035d624: push     {r4, lr}
0035d628: mov      r4, r0
0035d62c: bl       #0x35d4cc
0035d630: ldrb     r3, [r4, #0x1ec]
0035d634: cmp      r3, #0
0035d638: beq      #0x35d654
0035d63c: ldr      r3, [r4, #0x1f0]
0035d640: cmp      r3, #0
0035d644: beq      #0x35d654
0035d648: ldr      r1, [r4, #0x1fc]
0035d64c: cmp      r1, #0
0035d650: bne      #0x35d658
0035d654: pop      {r4, pc}
0035d658: mov      r0, r4
0035d65c: add      r1, r1, #1
0035d660: bl       #0x35ce6c
0035d664: mov      r0, r4
0035d668: ldr      r3, [r4]
0035d66c: ldr      r1, [r4, #0x1fc]
0035d670: mov      lr, pc
0035d674: ldr      pc, [r3, #0x14]
0035d678: pop      {r4, pc}

# _ZN15AnimatorBlender17BlenderApplicator10ResetDeltaEj
00366a68: push     {r4, r5, r6, r7, r8, sl, lr}
00366a6c: ldr      r3, [r0, #8]
00366a70: ldr      sl, [pc, #0xf0]
00366a74: sub      sp, sp, #0x1c
00366a78: cmp      r3, #0
00366a7c: mov      r5, r0
00366a80: mov      r7, r1
00366a84: add      sl, pc, sl
00366a88: beq      #0x366b0c
00366a8c: ldr      r3, [r0, #0x3c]
00366a90: ldr      r2, [r3, #0x28]
00366a94: ldr      r3, [r3, #0x70]
00366a98: ldr      r4, [r2, r3, lsl #2]
00366a9c: ldr      r3, [r4]
00366aa0: mov      r0, r4
00366aa4: mov      lr, pc
00366aa8: ldr      pc, [r3, #0x44]
00366aac: mov      r8, r0
00366ab0: mov      r0, r4
00366ab4: bl       #0x369160
00366ab8: subs     r6, r0, #0
00366abc: beq      #0x366b14
00366ac0: ldr      r1, [r5, #0xc]
00366ac4: mov      r3, #0
00366ac8: str      r3, [sp, #0x14]
00366acc: cmn      r1, #1
00366ad0: str      r3, [sp, #0xc]
00366ad4: str      r3, [sp, #0x10]
00366ad8: beq      #0x366af4
00366adc: mov      r0, r4
00366ae0: ldr      r2, [r8, #0x10]
00366ae4: ldr      ip, [r4]
00366ae8: add      r3, sp, #0xc
00366aec: mov      lr, pc
00366af0: ldr      pc, [ip, #0x7c]
00366af4: cmp      r6, #0
00366af8: beq      #0x366b0c
00366afc: mov      r0, r6
00366b00: mov      r1, r7
00366b04: add      r2, sp, #0xc
00366b08: bl       #0x3644cc
00366b0c: add      sp, sp, #0x1c
00366b10: pop      {r4, r5, r6, r7, r8, sl, pc}
00366b14: ldr      r3, [pc, #0x50]
00366b18: ldr      r3, [sl, r3]
00366b1c: ldr      r3, [r3]
00366b20: cmp      r3, #2
00366b24: streq    r6, [r6]
00366b28: beq      #0x366ac0
00366b2c: cmp      r3, #1
00366b30: bne      #0x366ac0
00366b34: ldr      r0, [pc, #0x34]
00366b38: ldr      r1, [pc, #0x34]
00366b3c: ldr      r2, [pc, #0x34]
00366b40: ldr      r0, [sl, r0]
00366b44: ldr      r3, [pc, #0x30]
00366b48: movw     ip, #0x167
00366b4c: add      r1, pc, r1
00366b50: add      r2, pc, r2
00366b54: add      r3, pc, r3
00366b58: add      r0, r0, #0xa8
00366b5c: str      ip, [sp]
00366b60: bl       #0x30e004
00366b64: b        #0x366ac0
00366b68: rsbeq    lr, r2, ip
00366b6c: andeq    r3, r0, r0, asr #19
00366b70: andeq    r1, r0, r0, asr #19
00366b74: subseq   r7, r5, ip, lsl #17
00366b78: ldrsbeq  r6, [r7], #-0x78
00366b7c: subseq   sl, r5, r4, lsr #5

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38

# _ZN15AnimatorBlender9BlendPostEv
00366740: bx       lr

# _ZN15AnimatorBlender8SetScaleEf
003666d8: push     {r4, r5, r6, r7, r8, lr}
003666dc: ldr      r3, [r0, #0x28]
003666e0: ldr      r6, [r0, #0x2c]
003666e4: mov      r5, r0
003666e8: mov      r7, r1
003666ec: rsb      r6, r3, r6
003666f0: asrs     r6, r6, #2
003666f4: beq      #0x36673c
003666f8: mov      r4, #0
003666fc: b        #0x366704
00366700: ldr      r3, [r5, #0x28]
00366704: ldr      r3, [r3, r4, lsl #2]
00366708: add      r4, r4, #1
0036670c: mov      r0, r3
00366710: ldr      r3, [r3]
00366714: mov      lr, pc
00366718: ldr      pc, [r3, #0x44]
0036671c: subs     r3, r0, #0
00366720: mov      r1, r7
00366724: beq      #0x366734
00366728: ldr      r3, [r3]
0036672c: mov      lr, pc
00366730: ldr      pc, [r3, #0x48]
00366734: cmp      r4, r6
00366738: bne      #0x366700
0036673c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12CharAnimator8_SetAnimEij
003cab38: push     {r4, r5, r6, r7, r8, sl, lr}
003cab3c: ldr      r4, [pc, #0x150]
003cab40: ldr      r5, [pc, #0x150]
003cab44: sub      sp, sp, #0x44
003cab48: add      r4, pc, r4
003cab4c: ldr      r3, [r4, r5]
003cab50: cmp      r1, #0
003cab54: mov      r6, r0
003cab58: ldr      r3, [r3]
003cab5c: str      r3, [sp, #0x3c]
003cab60: blt      #0x3cab80
003cab64: ldr      r3, [pc, #0x130]
003cab68: ldr      r3, [r4, r3]
003cab6c: ldr      r3, [r3]
003cab70: cmp      r1, r3
003cab74: bge      #0x3cab80
003cab78: cmp      r2, #2
003cab7c: bls      #0x3cab9c
003cab80: ldr      r3, [r4, r5]
003cab84: ldr      r2, [sp, #0x3c]
003cab88: ldr      r3, [r3]
003cab8c: cmp      r2, r3
003cab90: bne      #0x3cac90
003cab94: add      sp, sp, #0x44
003cab98: pop      {r4, r5, r6, r7, r8, sl, pc}
003cab9c: ldr      r3, [pc, #0xfc]
003caba0: str      r1, [r0, #0x4c]
003caba4: mov      r8, #0x14
003caba8: ldr      r0, [r4, r3]
003cabac: mov      r3, #0xc
003cabb0: mla      r3, r3, r2, r6
003cabb4: ldr      r0, [r0]
003cabb8: str      r2, [r6, #0x2c]
003cabbc: ldr      r2, [pc, #0xe0]
003cabc0: mla      r8, r8, r1, r0
003cabc4: str      r1, [r3, #8]
003cabc8: ldr      sl, [r4, r2]
003cabcc: ldr      r2, [r8, #4]
003cabd0: add      r7, sp, #0x24
003cabd4: mov      r0, sl
003cabd8: str      r2, [r3, #0xc]
003cabdc: bl       #0x337888
003cabe0: ldr      r1, [pc, #0xc0]
003cabe4: add      r2, sp, #8
003cabe8: mov      r0, r7
003cabec: add      r1, pc, r1
003cabf0: bl       #0x3140ec
003cabf4: mov      r1, r7
003cabf8: mov      r0, sl
003cabfc: bl       #0x337a88
003cac00: mov      r0, r7
003cac04: bl       #0x3139ac
003cac08: ldr      r0, [r6, #4]
003cac0c: mov      r1, #0x24
003cac10: mov      r2, #0
003cac14: bl       #0x3a4d5c
003cac18: ldr      r3, [r8, #0x10]
003cac1c: cmp      r3, #2
003cac20: beq      #0x3cac34
003cac24: mov      r0, r6
003cac28: mov      r1, #0
003cac2c: bl       #0x3ca79c
003cac30: b        #0x3cab80
003cac34: mov      r0, sl
003cac38: bl       #0x337888
003cac3c: ldr      r1, [pc, #0x68]
003cac40: add      r7, sp, #0xc
003cac44: add      r2, sp, #4
003cac48: add      r1, pc, r1
003cac4c: mov      r0, r7
003cac50: bl       #0x3140ec
003cac54: mov      r0, sl
003cac58: mov      r1, r7
003cac5c: bl       #0x337a88
003cac60: mov      sl, r0
003cac64: eor      sl, sl, #1
003cac68: mov      r0, r7
003cac6c: bl       #0x3139ac
003cac70: tst      sl, #0xff
003cac74: beq      #0x3cac24
003cac78: ldr      r0, [r8, #8]
003cac7c: bl       #0x3ca708
003cac80: mov      r1, r0
003cac84: mov      r0, r6
003cac88: bl       #0x3ca79c
003cac8c: b        #0x3cab80
003cac90: bl       #0x30e310
003cac94: subseq   sb, ip, r8, asr #30
003cac98: andeq    r4, r0, ip, lsr #1
003cac9c: andeq    r2, r0, r8, asr #20
003caca0: andeq    r3, r0, ip, ror ip
003caca4: andeq    r0, r0, r4, lsl #17
003caca8: subeq    sl, pc, r4, ror #8
003cacac: umaaleq  r8, pc, r8, pc

# _ZN14AnimApplicator13CheckCallbackEPN6glitch5scene19ITimelineControllerE
0036440c: push     {r4, lr}
00364410: ldrb     r3, [r0, #0x30]
00364414: mov      r4, r0
00364418: cmp      r3, #0
0036441c: beq      #0x364440
00364420: ldr      r3, [r0, #0x34]
00364424: cmp      r3, #0
00364428: beq      #0x364440
0036442c: mov      r0, r1
00364430: ldr      r1, [r4, #0x38]
00364434: blx      r3
00364438: mov      r3, #0
0036443c: strb     r3, [r4, #0x30]
00364440: pop      {r4, pc}

# _ZN6glitch7collada19CTimelineController8setScaleEf
00666c20: str      r1, [r0, #0x30]
00666c24: bx       lr

# _ZN15AnimatorBlender5BlendEi
0036679c: push     {r4, r5, r6, lr}
003667a0: ldr      r5, [r0, #0x2c]
003667a4: ldr      r2, [r0, #0x28]
003667a8: ldr      r3, [pc, #0xc0]
003667ac: sub      sp, sp, #8
003667b0: rsb      r5, r2, r5
003667b4: asr      r5, r5, #2
003667b8: cmp      r5, #2
003667bc: mov      r4, r0
003667c0: mov      r6, r1
003667c4: add      r3, pc, r3
003667c8: beq      #0x3667f0
003667cc: ldr      r2, [pc, #0xa0]
003667d0: ldr      r2, [r3, r2]
003667d4: ldr      r2, [r2]
003667d8: cmp      r2, #2
003667dc: moveq    r3, #0
003667e0: streq    r3, [r3]
003667e4: beq      #0x3667f0
003667e8: cmp      r2, #1
003667ec: beq      #0x36683c
003667f0: ldr      r0, [r4, #0x70]
003667f4: mov      r1, r5
003667f8: str      r0, [r4, #0x74]
003667fc: add      r0, r0, #1
00366800: bl       #0x30eb2c
00366804: ldr      r0, [r4, #0x78]
00366808: str      r1, [r4, #0x70]
0036680c: cmp      r0, #0
00366810: str      r0, [r4, #0x7c]
00366814: ble      #0x36682c
00366818: bl       #0x30e964
0036681c: mov      r1, r0
00366820: mov      r0, #0x3f800000
00366824: bl       #0x30ec94
00366828: str      r0, [r4, #0x80]
0036682c: bic      r6, r6, r6, asr #31
00366830: str      r6, [r4, #0x78]
00366834: add      sp, sp, #8
00366838: pop      {r4, r5, r6, pc}
0036683c: ldr      r0, [pc, #0x34]
00366840: ldr      r1, [pc, #0x34]
00366844: ldr      r2, [pc, #0x34]
00366848: ldr      r0, [r3, r0]
0036684c: ldr      r3, [pc, #0x30]
00366850: mov      ip, #0x7a
00366854: add      r1, pc, r1
00366858: add      r2, pc, r2
0036685c: add      r3, pc, r3
00366860: add      r0, r0, #0xa8
00366864: str      ip, [sp]
00366868: bl       #0x30e004
0036686c: b        #0x3667f0
00366870: rsbeq    lr, r2, ip, asr #5
00366874: andeq    r3, r0, r0, asr #19
00366878: andeq    r1, r0, r0, asr #19
0036687c: subseq   r7, r5, r4, lsl #23
00366880: subseq   sl, r5, r8, lsl #11

# _ZN6glitch7collada19CTimelineController7setLoopEb
00666c28: strb     r1, [r0, #0x18]
00666c2c: bx       lr

# _ZN12CharAnimator12_SetAnimStepEj
003ca79c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0: ldr      r3, [r0, #0x2c]
003ca7a4: mov      r2, #0xc
003ca7a8: ldr      r5, [pc, #0x374]
003ca7ac: mla      r3, r2, r3, r0
003ca7b0: ldr      r2, [pc, #0x370]
003ca7b4: add      r5, pc, r5
003ca7b8: mov      r4, r0
003ca7bc: ldr      r2, [r5, r2]
003ca7c0: ldr      r0, [r3, #8]
003ca7c4: mov      ip, #0x14
003ca7c8: ldr      r2, [r2]
003ca7cc: sub      sp, sp, #0x24
003ca7d0: mla      r2, ip, r0, r2
003ca7d4: ldr      r0, [r2, #8]
003ca7d8: cmp      r0, r1
003ca7dc: bhi      #0x3ca7e8
003ca7e0: add      sp, sp, #0x24
003ca7e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8: ldr      r2, [r2, #0xc]
003ca7ec: mov      r6, #0x38
003ca7f0: str      r1, [r3, #0x10]
003ca7f4: mla      r6, r6, r1, r2
003ca7f8: ldr      r0, [r4, #4]
003ca7fc: mov      r1, #0x26
003ca800: mov      r2, #0
003ca804: bl       #0x3a4d5c
003ca808: ldr      r3, [r6, #0x28]
003ca80c: cmp      r3, #1
003ca810: beq      #0x3caad8
003ca814: ldr      r3, [r6, #0x10]
003ca818: cmn      r3, #1
003ca81c: beq      #0x3ca9d8
003ca820: ldr      r3, [pc, #0x304]
003ca824: ldr      r0, [r5, r3]
003ca828: bl       #0x31f594
003ca82c: cmp      r0, #0
003ca830: beq      #0x3ca870
003ca834: ldr      r7, [r0, #0x128]
003ca838: cmp      r7, #0
003ca83c: beq      #0x3ca870
003ca840: mov      r0, r7
003ca844: ldr      r1, [r4, #4]
003ca848: bl       #0x40f980
003ca84c: cmp      r0, #0
003ca850: beq      #0x3ca870
003ca854: ldr      r1, [r6, #0x10]
003ca858: cmn      r1, #1
003ca85c: beq      #0x3ca9f4
003ca860: mov      r0, r7
003ca864: mov      r2, #0
003ca868: mov      r3, #1
003ca86c: bl       #0x40f904
003ca870: ldrb     r3, [r6, #0x34]
003ca874: strb     r3, [r4, #0x30]
003ca878: ldrb     r3, [r6, #0x34]
003ca87c: cmp      r3, #0
003ca880: moveq    r7, #1
003ca884: bne      #0x3caa10
003ca888: ldr      r3, [pc, #0x2a0]
003ca88c: ldr      r0, [r4, #4]
003ca890: ldr      sb, [r6, #0x2c]
003ca894: ldr      r3, [r5, r3]
003ca898: ldr      fp, [r3]
003ca89c: bl       #0x3935dc
003ca8a0: ldr      lr, [r0]
003ca8a4: ldr      r8, [r0, #4]
003ca8a8: ldr      sl, [r0, #8]
003ca8ac: mov      ip, #0xbf000000
003ca8b0: add      ip, ip, #0x800000
003ca8b4: str      lr, [sp, #0x14]
003ca8b8: mov      r0, fp
003ca8bc: mov      lr, #1
003ca8c0: mov      r1, sb
003ca8c4: add      r2, sp, #0x14
003ca8c8: mov      r3, #0
003ca8cc: str      r8, [sp, #0x18]
003ca8d0: str      sl, [sp, #0x1c]
003ca8d4: str      lr, [sp]
003ca8d8: str      ip, [sp, #8]
003ca8dc: str      ip, [sp, #4]
003ca8e0: bl       #0x36b5d8
003ca8e4: cmp      r7, #0
003ca8e8: beq      #0x3ca91c
003ca8ec: ldr      r8, [r6, #0x18]
003ca8f0: cmn      r8, #1
003ca8f4: beq      #0x3ca91c
003ca8f8: ldrb     r7, [r6, #4]
003ca8fc: cmp      r7, #0
003ca900: beq      #0x3caa94
003ca904: ldr      r3, [pc, #0x228]
003ca908: mov      r1, r8
003ca90c: ldr      r2, [r4, #4]
003ca910: ldr      r0, [r5, r3]
003ca914: mov      r3, #0
003ca918: bl       #0x495f04
003ca91c: ldr      r2, [r6, #0x30]
003ca920: ldr      r3, [r4, #4]
003ca924: str      r2, [r4, #0x34]
003ca928: ldr      r5, [r3, #0x2d8]
003ca92c: cmp      r5, #0
003ca930: beq      #0x3ca9e8
003ca934: ldr      r3, [r6, #8]
003ca938: cmn      r3, #1
003ca93c: beq      #0x3ca9e8
003ca940: mov      r3, #0
003ca944: strb     r3, [r4, #0x48]
003ca948: mov      r0, r4
003ca94c: bl       #0x3c9b7c
003ca950: ldrb     r3, [r4, #0x54]
003ca954: cmp      r3, #0
003ca958: beq      #0x3caa80
003ca95c: ldrb     r1, [r4, #0x49]
003ca960: ldr      r3, [r5, #0x38]
003ca964: ldrb     r2, [r6, #0x1c]
003ca968: cmp      r1, #0
003ca96c: beq      #0x3caac4
003ca970: mov      r1, #0
003ca974: str      r1, [r3, #0x14]
003ca978: mov      r1, #0
003ca97c: str      r1, [r3, #0xc]
003ca980: strb     r2, [r3, #0x10]
003ca984: ldr      r2, [r5, #0x38]
003ca988: ldr      r1, [r6, #8]
003ca98c: mov      r6, #0
003ca990: ldr      r3, [r4, #0x44]
003ca994: ldr      ip, [r2]
003ca998: mov      r0, r2
003ca99c: str      r6, [sp]
003ca9a0: mov      r2, r6
003ca9a4: mov      lr, pc
003ca9a8: ldr      pc, [ip, #0x1c]
003ca9ac: ldr      r1, [r4, #0x34]
003ca9b0: ldr      r0, [r4, #0x40]
003ca9b4: bl       #0x30ed6c
003ca9b8: ldr      r5, [r5, #0x38]
003ca9bc: mov      r1, r0
003ca9c0: mov      r2, r6
003ca9c4: mov      r0, r5
003ca9c8: ldr      r3, [r5]
003ca9cc: mov      lr, pc
003ca9d0: ldr      pc, [r3, #0x28]
003ca9d4: b        #0x3ca7e0
003ca9d8: ldr      r3, [r6, #0x20]
003ca9dc: cmp      r3, #0
003ca9e0: beq      #0x3ca870
003ca9e4: b        #0x3ca820
003ca9e8: mov      r0, r4
003ca9ec: bl       #0x3c9924
003ca9f0: b        #0x3ca7e0
003ca9f4: ldr      r0, [r6, #0x20]
003ca9f8: cmp      r0, #0
003ca9fc: beq      #0x3ca870
003caa00: ldr      r8, [r6, #0x24]
003caa04: bl       #0x3ca708
003caa08: ldr      r1, [r8, r0, lsl #2]
003caa0c: b        #0x3ca860
003caa10: ldr      r0, [r4, #4]
003caa14: mov      r1, #1
003caa18: add      r0, r0, #0x37c
003caa1c: bl       #0x3ffe3c
003caa20: mov      r7, r0
003caa24: ldr      r0, [r4, #4]
003caa28: mov      r1, #2
003caa2c: add      r0, r0, #0x37c
003caa30: bl       #0x3ffe3c
003caa34: mov      r1, r7
003caa38: mov      sl, r0
003caa3c: mov      r0, r4
003caa40: bl       #0x3c94b8
003caa44: mov      r1, r7
003caa48: eor      r8, r0, #1
003caa4c: ldrb     r2, [r6, #4]
003caa50: mov      r0, r4
003caa54: bl       #0x3c955c
003caa58: uxtb     r8, r8
003caa5c: eor      r0, r0, #1
003caa60: cmp      r8, #0
003caa64: uxtb     r7, r0
003caa68: bne      #0x3caaf0
003caa6c: cmp      r7, #0
003caa70: bne      #0x3cab08
003caa74: cmp      r8, #0
003caa78: beq      #0x3ca8e4
003caa7c: b        #0x3ca888
003caa80: ldr      r2, [r5, #0x38]
003caa84: ldrb     r1, [r6, #0x1c]
003caa88: str      r3, [r2, #0xc]
003caa8c: strb     r1, [r2, #0x10]
003caa90: b        #0x3ca984
003caa94: ldr      r0, [r4, #4]
003caa98: bl       #0x3935dc
003caa9c: ldr      r3, [r4, #4]
003caaa0: mov      r2, r0
003caaa4: ldr      r0, [pc, #0x88]
003caaa8: mov      r1, r8
003caaac: add      r3, r3, #0x16c
003caab0: ldr      r0, [r5, r0]
003caab4: str      r7, [sp, #4]
003caab8: str      r7, [sp]
003caabc: bl       #0x495888
003caac0: b        #0x3ca91c
003caac4: ldrb     r1, [r4, #0x4a]
003caac8: cmp      r1, #0
003caacc: ldreq    r1, [r6, #0xc]
003caad0: beq      #0x3ca974
003caad4: b        #0x3ca970
003caad8: ldr      r2, [r4, #0x2c]
003caadc: mov      r0, r4
003caae0: ldr      r1, [r6, #8]
003caae4: add      r2, r2, #1
003caae8: bl       #0x3cab38
003caaec: b        #0x3ca7e0
003caaf0: mov      r0, r4
003caaf4: mov      r1, sl
003caaf8: bl       #0x3c94b8
003caafc: eor      r0, r0, #1
003cab00: uxtb     r8, r0
003cab04: b        #0x3caa6c
003cab08: mov      r1, sl
003cab0c: mov      r0, r4
003cab10: ldrb     r2, [r6, #4]
003cab14: bl       #0x3c955c
003cab18: eor      r0, r0, #1
003cab1c: uxtb     r8, r0
003cab20: b        #0x3caa74
003cab24: ldrsbeq  sl, [ip], #-0x2c
003cab28: andeq    r3, r0, ip, ror ip
003cab2c: strdeq   r3, r4, [r0], -r4
003cab30: andeq    r0, r0, r4, lsr #27
003cab34: andeq    r1, r0, r8, lsl #22

# _ZN11AnimatorSet19SetCurrentAnimationEi
003674ac: push     {r4, r5, r6, lr}
003674b0: mov      r5, r0
003674b4: ldr      r0, [r0, #0x94]
003674b8: bl       #0x3660f4
003674bc: mov      r4, r0
003674c0: ldr      r0, [r0, #0x20]
003674c4: cmn      r0, #1
003674c8: beq      #0x36750c
003674cc: ldr      r2, [r4, #0x24]
003674d0: ldr      r3, [r4, #0x2c]
003674d4: mov      r0, r5
003674d8: add      r2, r2, #1
003674dc: add      r3, r3, #1
003674e0: str      r2, [r4, #0x24]
003674e4: str      r3, [r4, #0x2c]
003674e8: ldr      r3, [r5, #0x98]
003674ec: str      r4, [r5, #0x98]
003674f0: cmp      r3, #0
003674f4: ldrne    r2, [r3, #0x24]
003674f8: subne    r2, r2, #1
003674fc: strne    r2, [r3, #0x24]
00367500: ldr      r1, [r4, #0x20]
00367504: bl       #0x65f8c8
00367508: ldr      r0, [r4, #0x20]
0036750c: pop      {r4, r5, r6, pc}

# _ZN24BlendedAnimSetController8PlayClipEjbij
0047680c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810: mov      r4, r1
00476814: ldr      r1, [sp, #0x28]
00476818: mov      sl, r2
0047681c: mov      r7, r0
00476820: bl       #0x4748b8
00476824: subs     r6, r0, #0
00476828: beq      #0x476900
0047682c: ldr      r1, [r7, #0x14]
00476830: bl       #0x36679c
00476834: cmn      r4, #1
00476838: beq      #0x476900
0047683c: ldr      r2, [r6, #0x70]
00476840: ldr      r3, [r6, #0x28]
00476844: ldr      r5, [r3, r2, lsl #2]
00476848: cmp      r5, #0
0047684c: beq      #0x476908
00476850: ldr      r3, [r5]
00476854: mov      r0, r5
00476858: mov      lr, pc
0047685c: ldr      pc, [r3, #0x44]
00476860: mov      r8, r0
00476864: mov      r0, r5
00476868: bl       #0x65f114
0047686c: mov      sb, r0
00476870: mov      r0, r6
00476874: bl       #0x369160
00476878: mov      r1, r4
0047687c: mov      fp, r0
00476880: mov      r0, r5
00476884: bl       #0x3674ac
00476888: cmn      r0, #1
0047688c: mov      r4, r0
00476890: beq      #0x476900
00476894: ldr      r3, [r8, #0x34]
00476898: cmp      r3, #0
0047689c: beq      #0x4768b4
004768a0: mov      r0, r5
004768a4: ldr      r3, [r5]
004768a8: ldr      r1, [r7, #0xc]
004768ac: mov      lr, pc
004768b0: ldr      pc, [r3, #0x30]
004768b4: cmp      sb, r4
004768b8: beq      #0x476920
004768bc: mov      r1, sl
004768c0: mov      r0, r8
004768c4: ldr      r3, [r8]
004768c8: mov      lr, pc
004768cc: ldr      pc, [r3, #0x40]
004768d0: ldr      r3, [r8]
004768d4: mov      r0, r8
004768d8: mov      r1, #0x3f800000
004768dc: mov      lr, pc
004768e0: ldr      pc, [r3, #0x48]
004768e4: ldrb     r1, [r7, #0x10]
004768e8: ldr      r0, [r7, #4]
004768ec: bl       #0x35d624
004768f0: mov      r0, r6
004768f4: bl       #0x366740
004768f8: mov      r0, #1
004768fc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900: mov      r0, #0
00476904: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908: mov      r0, r5
0047690c: bl       #0x65f114
00476910: mov      r0, r6
00476914: bl       #0x369160
00476918: mov      r0, r5
0047691c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920: ldr      r3, [r8]
00476924: mov      r0, r8
00476928: mov      lr, pc
0047692c: ldr      pc, [r3, #0x44]
00476930: cmp      r0, #0
00476934: bne      #0x4768bc
00476938: cmp      fp, #0
0047693c: ldr      r3, [r8]
00476940: ldr      r1, [r8, #0x10]
00476944: ldrne    fp, [fp, #0x10]
00476948: ldr      r3, [r3, #0xc]
0047694c: mov      r0, r8
00476950: add      r1, fp, r1
00476954: blx      r3
00476958: b        #0x4768bc
