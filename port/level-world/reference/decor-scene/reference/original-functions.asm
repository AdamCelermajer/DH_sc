
# _ZN14ColladaFactory14createMeshNodeERKN6glitch7collada16CColladaDatabaseERKN5boost13intrusive_ptrINS1_5IMeshEEEPv
003508a4: push     {r4, r5, r6, lr}
003508a8: mov      r1, #0
003508ac: mov      r0, #0x144
003508b0: mov      r5, r2
003508b4: bl       #0x5341ac
003508b8: mov      r1, r5
003508bc: mov      r4, r0
003508c0: bl       #0x35b240
003508c4: mov      r0, r4
003508c8: pop      {r4, r5, r6, pc}

# _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeERKSsb
00352b74: push     {r4, r5, r6, r7, r8, lr}
00352b78: subs     r4, r1, #0
00352b7c: mov      r8, r0
00352b80: mov      r5, r2
00352b84: mov      r7, r3
00352b88: beq      #0x352c04
00352b8c: cmp      r3, #0
00352b90: bne      #0x352c0c
00352b94: ldr      r3, [r4]
00352b98: mov      r0, r4
00352b9c: mov      lr, pc
00352ba0: ldr      pc, [r3, #0x24]
00352ba4: ldr      r1, [r5, #0x14]
00352ba8: bl       #0x30e31c
00352bac: cmp      r0, #0
00352bb0: beq      #0x352c04
00352bb4: mov      r0, r4
00352bb8: bl       #0x5971c8
00352bbc: mov      r6, r0
00352bc0: ldr      r4, [r6, #4]!
00352bc4: cmp      r4, r6
00352bc8: moveq    r4, #0
00352bcc: beq      #0x352c04
00352bd0: cmp      r4, #0
00352bd4: moveq    r1, r4
00352bd8: subne    r1, r4, #4
00352bdc: mov      r0, r8
00352be0: mov      r2, r5
00352be4: mov      r3, r7
00352be8: bl       #0x352b74
00352bec: ldr      r4, [r4]
00352bf0: cmp      r6, r4
00352bf4: beq      #0x352c00
00352bf8: cmp      r0, #0
00352bfc: beq      #0x352bd0
00352c00: mov      r4, r0
00352c04: mov      r0, r4
00352c08: pop      {r4, r5, r6, r7, r8, pc}
00352c0c: ldr      r3, [r4]
00352c10: mov      r0, r4
00352c14: mov      lr, pc
00352c18: ldr      pc, [r3, #0x24]
00352c1c: ldr      r1, [r5, #0x14]
00352c20: ldr      r2, [r5, #0x10]
00352c24: rsb      r2, r1, r2
00352c28: bl       #0x30ec7c
00352c2c: cmp      r0, #0
00352c30: bne      #0x352bb4
00352c34: mov      r0, r4
00352c38: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada10CSceneNode18computeBoundingBoxEv
0065cda8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065cdac: movw     r3, #0x6164
0065cdb0: sub      sp, sp, #0x24
0065cdb4: movt     r3, #0x4d65
0065cdb8: mov      r7, r0
0065cdbc: str      r3, [sp, #4]
0065cdc0: ldr      r4, [r7, #0xf4]!
0065cdc4: movw     sl, #0x6164
0065cdc8: movw     r8, #0x6164
0065cdcc: movw     r6, #0x6164
0065cdd0: add      r3, sp, #8
0065cdd4: cmp      r7, r4
0065cdd8: mov      r5, r0
0065cddc: movt     sl, #0x7365
0065cde0: movt     r8, #0x6d65
0065cde4: movt     r6, #0x6e65
0065cde8: add      fp, r0, #0x130
0065cdec: mov      sb, #0
0065cdf0: str      r3, [sp]
0065cdf4: beq      #0x65cec0
0065cdf8: cmp      r4, #0
0065cdfc: moveq    r3, r4
0065ce00: subne    r3, r4, #4
0065ce04: mov      r0, r3
0065ce08: ldr      r3, [r3]
0065ce0c: mov      lr, pc
0065ce10: ldr      pc, [r3, #0xbc]
0065ce14: cmp      r0, r8
0065ce18: cmpne    r0, sl
0065ce1c: beq      #0x65ce30
0065ce20: ldr      r3, [sp, #4]
0065ce24: cmp      r0, r3
0065ce28: cmpne    r0, r6
0065ce2c: bne      #0x65ceb4
0065ce30: cmp      r0, r6
0065ce34: beq      #0x65cf4c
0065ce38: cmp      sb, #0
0065ce3c: bne      #0x65cec8
0065ce40: cmp      r4, #0
0065ce44: moveq    r3, r4
0065ce48: subne    r3, r4, #4
0065ce4c: mov      r0, r3
0065ce50: ldr      r3, [r3]
0065ce54: mov      lr, pc
0065ce58: ldr      pc, [r3, #0x30]
0065ce5c: ldr      r3, [r0]
0065ce60: cmp      r4, #0
0065ce64: mov      sb, #1
0065ce68: str      r3, [r5, #0x130]
0065ce6c: ldr      r3, [r0, #4]
0065ce70: str      r3, [r5, #0x134]
0065ce74: ldr      r3, [r0, #8]
0065ce78: str      r3, [r5, #0x138]
0065ce7c: ldr      r3, [r0, #0xc]
0065ce80: str      r3, [r5, #0x13c]
0065ce84: ldr      r3, [r0, #0x10]
0065ce88: str      r3, [r5, #0x140]
0065ce8c: ldr      r3, [r0, #0x14]
0065ce90: str      r3, [r5, #0x144]
0065ce94: moveq    r3, r4
0065ce98: subne    r3, r4, #4
0065ce9c: mov      r0, r3
0065cea0: ldr      r3, [r3]
0065cea4: mov      lr, pc
0065cea8: ldr      pc, [r3, #0x40]
0065ceac: mov      r1, fp
0065ceb0: bl       #0x597548
0065ceb4: ldr      r4, [r4]
0065ceb8: cmp      r7, r4
0065cebc: bne      #0x65cdf8
0065cec0: add      sp, sp, #0x24
0065cec4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065cec8: cmp      r4, #0
0065cecc: moveq    r3, r4
0065ced0: subne    r3, r4, #4
0065ced4: mov      r0, r3
0065ced8: ldr      r3, [r3]
0065cedc: mov      lr, pc
0065cee0: ldr      pc, [r3, #0x30]
0065cee4: ldr      r3, [r0]
0065cee8: cmp      r4, #0
0065ceec: str      r3, [sp, #8]
0065cef0: ldr      r3, [r0, #4]
0065cef4: str      r3, [sp, #0xc]
0065cef8: ldr      r3, [r0, #8]
0065cefc: str      r3, [sp, #0x10]
0065cf00: ldr      r3, [r0, #0xc]
0065cf04: str      r3, [sp, #0x14]
0065cf08: ldr      r3, [r0, #0x10]
0065cf0c: str      r3, [sp, #0x18]
0065cf10: ldr      r3, [r0, #0x14]
0065cf14: str      r3, [sp, #0x1c]
0065cf18: moveq    r3, r4
0065cf1c: subne    r3, r4, #4
0065cf20: mov      r0, r3
0065cf24: ldr      r3, [r3]
0065cf28: mov      lr, pc
0065cf2c: ldr      pc, [r3, #0x40]
0065cf30: ldr      r1, [sp]
0065cf34: bl       #0x597548
0065cf38: mov      r0, fp
0065cf3c: ldr      r1, [sp]
0065cf40: bl       #0x35c150
0065cf44: ldr      r4, [r4]
0065cf48: b        #0x65ceb8
0065cf4c: cmp      r4, #0
0065cf50: moveq    r3, r4
0065cf54: subne    r3, r4, #4
0065cf58: mov      r0, r3
0065cf5c: ldr      r3, [r3]
0065cf60: mov      lr, pc
0065cf64: ldr      pc, [r3, #0xf4]
0065cf68: b        #0x65ce38

# _ZN12VisualObject11CalcMeshBoxEv
0047211c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr      r2, [pc, #0x5d4]
00472124: ldr      r3, [r0, #0xc]
00472128: sub      sp, sp, #0x64
0047212c: add      r2, pc, r2
00472130: cmp      r3, #0
00472134: str      r2, [sp, #8]
00472138: mov      r4, r0
0047213c: beq      #0x472408
00472140: mov      r0, r3
00472144: ldr      r3, [r3]
00472148: mov      lr, pc
0047214c: ldr      pc, [r3, #0x30]
00472150: ldr      r1, [r0]
00472154: mov      r3, r0
00472158: ldr      r2, [r4, #0xc]
0047215c: str      r1, [r4, #0x10]
00472160: ldr      r1, [r0, #4]
00472164: mov      r0, r2
00472168: str      r1, [r4, #0x14]
0047216c: ldr      r3, [r3, #8]
00472170: str      r3, [r4, #0x18]
00472174: ldr      r3, [r2]
00472178: mov      lr, pc
0047217c: ldr      pc, [r3, #0x30]
00472180: ldr      r2, [r0, #0xc]
00472184: mov      r3, r0
00472188: ldr      r0, [r4, #0xc]
0047218c: str      r2, [r4, #0x1c]
00472190: ldr      r2, [r3, #0x10]
00472194: str      r2, [r4, #0x20]
00472198: ldr      r3, [r3, #0x14]
0047219c: str      r3, [r4, #0x24]
004721a0: bl       #0x597290
004721a4: ldr      r3, [r0]
004721a8: mov      lr, pc
004721ac: ldr      pc, [r3, #0x90]
004721b0: mov      r3, r0
004721b4: ldr      r1, [r0]
004721b8: ldr      r0, [r4, #0x10]
004721bc: ldr      r6, [r3, #4]
004721c0: ldr      r5, [r3, #8]
004721c4: bl       #0x30ed6c
004721c8: mov      r1, r6
004721cc: str      r0, [r4, #0x10]
004721d0: ldr      r0, [r4, #0x14]
004721d4: bl       #0x30ed6c
004721d8: mov      r1, r5
004721dc: str      r0, [r4, #0x14]
004721e0: ldr      r0, [r4, #0x18]
004721e4: bl       #0x30ed6c
004721e8: str      r0, [r4, #0x18]
004721ec: ldr      r0, [r4, #0xc]
004721f0: bl       #0x597290
004721f4: ldr      r3, [r0]
004721f8: mov      lr, pc
004721fc: ldr      pc, [r3, #0x90]
00472200: mov      r3, r0
00472204: ldr      r1, [r0]
00472208: ldr      r0, [r4, #0x1c]
0047220c: ldr      r6, [r3, #4]
00472210: ldr      r5, [r3, #8]
00472214: bl       #0x30ed6c
00472218: mov      r1, r6
0047221c: str      r0, [r4, #0x1c]
00472220: ldr      r0, [r4, #0x20]
00472224: bl       #0x30ed6c
00472228: mov      r1, r5
0047222c: str      r0, [r4, #0x20]
00472230: ldr      r0, [r4, #0x24]
00472234: bl       #0x30ed6c
00472238: str      r0, [r4, #0x24]
0047223c: ldr      r3, [r4, #8]
00472240: add      r5, sp, #0x10
00472244: mov      r6, #0
00472248: mov      r0, r3
0047224c: ldr      r3, [r3]
00472250: mov      lr, pc
00472254: ldr      pc, [r3, #0x40]
00472258: mov      r2, #0x41
0047225c: mov      r1, r0
00472260: mov      r0, r5
00472264: strb     r6, [sp, #0x50]
00472268: bl       #0x30e868
0047226c: mov      r3, #0
00472270: mov      r1, r5
00472274: add      r0, r4, #0x10
00472278: str      r3, [sp, #0x48]
0047227c: str      r3, [sp, #0x40]
00472280: str      r3, [sp, #0x44]
00472284: strb     r6, [sp, #0x50]
00472288: bl       #0x312da8
0047228c: mov      r1, r5
00472290: add      r0, r4, #0x1c
00472294: bl       #0x312da8
00472298: ldr      r7, [r4, #0x1c]
0047229c: ldr      r5, [r4, #0x10]
004722a0: mov      r0, r7
004722a4: mov      r1, r5
004722a8: bl       #0x30e70c
004722ac: mov      r1, r5
004722b0: cmp      r0, r6
004722b4: mov      r0, r7
004722b8: movne    fp, r7
004722bc: moveq    fp, r5
004722c0: bl       #0x30e2f8
004722c4: cmp      r0, #0
004722c8: ldr      r6, [r4, #0x20]
004722cc: moveq    r7, r5
004722d0: ldr      r5, [r4, #0x14]
004722d4: mov      r0, r6
004722d8: str      r7, [r4, #0x1c]
004722dc: mov      r1, r5
004722e0: str      fp, [r4, #0x10]
004722e4: bl       #0x30e70c
004722e8: mov      r1, r5
004722ec: cmp      r0, #0
004722f0: mov      r0, r6
004722f4: movne    sb, r6
004722f8: moveq    sb, r5
004722fc: bl       #0x30e2f8
00472300: cmp      r0, #0
00472304: ldr      r8, [r4, #0x18]
00472308: moveq    r6, r5
0047230c: ldr      r5, [r4, #0x24]
00472310: mov      r1, r8
00472314: str      r6, [r4, #0x20]
00472318: mov      r0, r5
0047231c: str      sb, [r4, #0x14]
00472320: bl       #0x30e70c
00472324: mov      r1, r8
00472328: cmp      r0, #0
0047232c: mov      r0, r5
00472330: movne    sl, r5
00472334: moveq    sl, r8
00472338: bl       #0x30e2f8
0047233c: cmp      r0, #0
00472340: moveq    r5, r8
00472344: mov      r1, fp
00472348: str      r5, [r4, #0x24]
0047234c: mov      r0, r7
00472350: str      sl, [r4, #0x18]
00472354: bl       #0x30e3ac
00472358: mov      r1, #0x3f000000
0047235c: bl       #0x30ed6c
00472360: mov      r1, sb
00472364: mov      r7, r0
00472368: mov      r0, r6
0047236c: bl       #0x30e3ac
00472370: mov      r1, #0x3f000000
00472374: bl       #0x30ed6c
00472378: mov      r1, sl
0047237c: mov      r8, r0
00472380: mov      r0, r5
00472384: bl       #0x30e3ac
00472388: mov      r1, #0x3f000000
0047238c: bl       #0x30ed6c
00472390: ldr      ip, [sp, #8]
00472394: ldr      r3, [pc, #0x364]
00472398: mov      r6, r0
0047239c: mov      r1, r7
004723a0: ldr      r5, [ip, r3]
004723a4: ldr      r0, [r5]
004723a8: bl       #0x30e3ac
004723ac: str      r0, [r4, #0x10]
004723b0: ldr      r1, [r5]
004723b4: mov      r0, r7
004723b8: bl       #0x30eba4
004723bc: str      r0, [r4, #0x1c]
004723c0: ldr      r0, [r5, #4]
004723c4: mov      r1, r8
004723c8: bl       #0x30e3ac
004723cc: str      r0, [r4, #0x14]
004723d0: ldr      r1, [r5, #4]
004723d4: mov      r0, r8
004723d8: bl       #0x30eba4
004723dc: str      r0, [r4, #0x20]
004723e0: ldr      r0, [r5, #8]
004723e4: mov      r1, r6
004723e8: bl       #0x30e3ac
004723ec: str      r0, [r4, #0x18]
004723f0: ldr      r1, [r5, #8]
004723f4: mov      r0, r6
004723f8: bl       #0x30eba4
004723fc: str      r0, [r4, #0x24]
00472400: add      sp, sp, #0x64
00472404: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr      ip, [sp, #8]
0047240c: ldr      r0, [pc, #0x2f0]
00472410: mvn      r1, #0x80000000
00472414: sub      r1, r1, #0x800000
00472418: ldr      r5, [ip, r0]
0047241c: mvn      r2, #0x800000
00472420: str      r1, [r4, #0x18]
00472424: str      r1, [r4, #0x10]
00472428: str      r1, [r4, #0x14]
0047242c: str      r2, [r4, #0x24]
00472430: str      r2, [r4, #0x1c]
00472434: str      r2, [r4, #0x20]
00472438: ldr      r2, [r5, #0x10]
0047243c: str      r3, [sp, #0x5c]
00472440: str      r3, [sp, #0x54]
00472444: str      r3, [sp, #0x58]
00472448: ldr      r3, [r2, #0x1c]
0047244c: add      r6, sp, #0x54
00472450: movw     r1, #0x6164
00472454: mov      r0, r3
00472458: ldr      ip, [r3]
0047245c: movt     r1, #0x7365
00472460: ldr      r3, [r4, #8]
00472464: mov      r2, r6
00472468: mov      lr, pc
0047246c: ldr      pc, [ip, #0x20]
00472470: ldr      r0, [sp, #0x54]
00472474: ldr      r3, [sp, #0x58]
00472478: rsb      r3, r0, r3
0047247c: asrs     r3, r3, #2
00472480: str      r3, [sp, #0xc]
00472484: beq      #0x472690
00472488: ldr      r2, [sp, #0xc]
0047248c: cmp      r2, #0
00472490: beq      #0x472680
00472494: mov      r5, #0
00472498: b        #0x4724a0
0047249c: ldr      r0, [sp, #0x54]
004724a0: ldr      r3, [r0, r5, lsl #2]
004724a4: mov      r0, r3
004724a8: ldr      r3, [r3]
004724ac: mov      lr, pc
004724b0: ldr      pc, [r3, #0x30]
004724b4: ldr      r3, [sp, #0x54]
004724b8: ldr      sl, [r0, #8]
004724bc: ldr      fp, [r0]
004724c0: ldr      r3, [r3, r5, lsl #2]
004724c4: ldr      sb, [r0, #4]
004724c8: mov      r0, r3
004724cc: ldr      r3, [r3]
004724d0: mov      lr, pc
004724d4: ldr      pc, [r3, #0x30]
004724d8: ldr      r2, [sp, #0x54]
004724dc: mov      r3, r0
004724e0: ldr      r6, [r0, #0x14]
004724e4: ldr      r8, [r0, #0xc]
004724e8: ldr      r0, [r2, r5, lsl #2]
004724ec: ldr      r7, [r3, #0x10]
004724f0: bl       #0x597290
004724f4: cmp      r0, #0
004724f8: beq      #0x4725bc
004724fc: ldr      r3, [sp, #0x54]
00472500: ldr      r0, [r3, r5, lsl #2]
00472504: bl       #0x597290
00472508: ldr      r3, [r0]
0047250c: mov      lr, pc
00472510: ldr      pc, [r3, #0x90]
00472514: mov      r2, r0
00472518: ldr      r3, [r2, #4]
0047251c: ldr      r2, [r2, #8]
00472520: ldr      r1, [r0]
00472524: mov      r0, fp
00472528: stm      sp, {r2, r3}
0047252c: bl       #0x30ed6c
00472530: ldr      r3, [sp, #4]
00472534: mov      fp, r0
00472538: mov      r0, sb
0047253c: mov      r1, r3
00472540: bl       #0x30ed6c
00472544: ldr      r2, [sp]
00472548: mov      sb, r0
0047254c: mov      r0, sl
00472550: mov      r1, r2
00472554: bl       #0x30ed6c
00472558: ldr      r3, [sp, #0x54]
0047255c: mov      sl, r0
00472560: ldr      r0, [r3, r5, lsl #2]
00472564: bl       #0x597290
00472568: ldr      r3, [r0]
0047256c: mov      lr, pc
00472570: ldr      pc, [r3, #0x90]
00472574: mov      r2, r0
00472578: ldr      r3, [r2, #4]
0047257c: ldr      r2, [r2, #8]
00472580: ldr      r1, [r0]
00472584: mov      r0, r8
00472588: stm      sp, {r2, r3}
0047258c: bl       #0x30ed6c
00472590: ldr      r3, [sp, #4]
00472594: mov      r8, r0
00472598: mov      r0, r7
0047259c: mov      r1, r3
004725a0: bl       #0x30ed6c
004725a4: ldr      r2, [sp]
004725a8: mov      r7, r0
004725ac: mov      r0, r6
004725b0: mov      r1, r2
004725b4: bl       #0x30ed6c
004725b8: mov      r6, r0
004725bc: ldr      r3, [r4, #0x10]
004725c0: mov      r1, fp
004725c4: add      r5, r5, #1
004725c8: mov      r0, r3
004725cc: str      r3, [sp, #4]
004725d0: bl       #0x30e2f8
004725d4: cmp      r0, #0
004725d8: ldr      r3, [sp, #4]
004725dc: movne    r3, fp
004725e0: ldr      fp, [r4, #0x14]
004725e4: str      r3, [r4, #0x10]
004725e8: mov      r1, sb
004725ec: mov      r0, fp
004725f0: bl       #0x30e2f8
004725f4: cmp      r0, #0
004725f8: movne    fp, sb
004725fc: ldr      sb, [r4, #0x18]
00472600: mov      r1, sl
00472604: str      fp, [r4, #0x14]
00472608: mov      r0, sb
0047260c: bl       #0x30e2f8
00472610: cmp      r0, #0
00472614: movne    sb, sl
00472618: ldr      sl, [r4, #0x1c]
0047261c: mov      r1, r8
00472620: str      sb, [r4, #0x18]
00472624: mov      r0, sl
00472628: bl       #0x30e70c
0047262c: cmp      r0, #0
00472630: movne    sl, r8
00472634: ldr      r8, [r4, #0x20]
00472638: mov      r1, r7
0047263c: str      sl, [r4, #0x1c]
00472640: mov      r0, r8
00472644: bl       #0x30e70c
00472648: cmp      r0, #0
0047264c: movne    r8, r7
00472650: ldr      r7, [r4, #0x24]
00472654: str      r8, [r4, #0x20]
00472658: mov      r1, r6
0047265c: mov      r0, r7
00472660: bl       #0x30e70c
00472664: ldr      r3, [sp, #0xc]
00472668: cmp      r0, #0
0047266c: movne    r7, r6
00472670: cmp      r5, r3
00472674: str      r7, [r4, #0x24]
00472678: bne      #0x47249c
0047267c: ldr      r0, [sp, #0x54]
00472680: cmp      r0, #0
00472684: beq      #0x47223c
00472688: bl       #0x310450
0047268c: b        #0x47223c
00472690: ldr      r3, [r5, #0x10]
00472694: movw     r1, #0x6164
00472698: movt     r1, #0x6d65
0047269c: ldr      ip, [r3, #0x1c]
004726a0: mov      r2, r6
004726a4: ldr      r3, [r4, #8]
004726a8: mov      r0, ip
004726ac: ldr      ip, [ip]
004726b0: mov      lr, pc
004726b4: ldr      pc, [ip, #0x20]
004726b8: ldr      r0, [sp, #0x54]
004726bc: ldr      r3, [sp, #0x58]
004726c0: rsb      r3, r0, r3
004726c4: asrs     r3, r3, #2
004726c8: str      r3, [sp, #0xc]
004726cc: bne      #0x472488
004726d0: mov      r3, #0
004726d4: cmp      r0, #0
004726d8: str      r3, [r4, #0x24]
004726dc: str      r3, [r4, #0x10]
004726e0: str      r3, [r4, #0x14]
004726e4: str      r3, [r4, #0x18]
004726e8: str      r3, [r4, #0x1c]
004726ec: str      r3, [r4, #0x20]
004726f0: beq      #0x472400
004726f4: bl       #0x310450
004726f8: b        #0x472400
004726fc: subseq   r2, r2, r4, ror #18
00472700: andeq    r3, r0, ip, lsr #30
00472704: strdeq   r3, r4, [r0], -r4

# _ZNK6glitch5scene10ISceneNode25getRelativeTransformationEv
00598908: push     {r4, r5, r6, lr}
0059890c: ldr      r3, [r0, #0x11c]
00598910: sub      sp, sp, #0x48
00598914: mov      r4, r0
00598918: tst      r3, #0xe
0059891c: addeq    r5, r0, #0x68
00598920: beq      #0x598958
00598924: ands     r2, r3, #6
00598928: bne      #0x598964
0059892c: ldr      ip, [r0, #0xac]
00598930: ldr      r1, [r4, #0xb4]
00598934: ldr      r0, [r0, #0xb0]
00598938: add      r5, r4, #0x68
0059893c: strb     r2, [r4, #0xa8]
00598940: str      ip, [r4, #0x98]
00598944: str      r0, [r4, #0x9c]
00598948: str      r1, [r4, #0xa0]
0059894c: bic      r3, r3, #0xe
00598950: orr      r3, r3, #0x10
00598954: str      r3, [r4, #0x11c]
00598958: mov      r0, r5
0059895c: add      sp, sp, #0x48
00598960: pop      {r4, r5, r6, pc}
00598964: add      r6, sp, #4
00598968: mov      r3, #0
0059896c: add      r5, r0, #0x68
00598970: mov      r1, r6
00598974: add      r0, r0, #0xb8
00598978: strb     r3, [sp, #0x44]
0059897c: bl       #0x5602d0
00598980: mov      r1, r6
00598984: mov      r2, #0x41
00598988: mov      r0, r5
0059898c: bl       #0x30e868
00598990: ldr      r0, [r4, #0xc8]
00598994: mov      r1, #0x3f800000
00598998: bl       #0x30df8c
0059899c: cmp      r0, #0
005989a0: beq      #0x5989b8
005989a4: ldr      r0, [r4, #0xcc]
005989a8: mov      r1, #0x3f800000
005989ac: bl       #0x30df8c
005989b0: cmp      r0, #0
005989b4: bne      #0x5989ec
005989b8: mov      r0, r5
005989bc: add      r1, r4, #0xc8
005989c0: bl       #0x597788
005989c4: ldr      r3, [r4, #0xb4]
005989c8: ldr      r1, [r4, #0xac]
005989cc: ldr      r2, [r4, #0xb0]
005989d0: mov      r0, #0
005989d4: str      r3, [r4, #0xa0]
005989d8: strb     r0, [r4, #0xa8]
005989dc: str      r1, [r4, #0x98]
005989e0: str      r2, [r4, #0x9c]
005989e4: ldr      r3, [r4, #0x11c]
005989e8: b        #0x59894c
005989ec: ldr      r0, [r4, #0xd0]
005989f0: mov      r1, #0x3f800000
005989f4: bl       #0x30df8c
005989f8: cmp      r0, #0
005989fc: bne      #0x5989c4
00598a00: b        #0x5989b8

# _ZN6glitch4core10quaternion3setEfff
0035c9d8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035c9dc: mov      r4, r0
0035c9e0: sub      sp, sp, #0x34
0035c9e4: mov      r0, r1
0035c9e8: mov      r5, r2
0035c9ec: mov      sl, r3
0035c9f0: bl       #0x30e8a4
0035c9f4: mov      r3, #0x3fc00000
0035c9f8: mov      r2, #0
0035c9fc: add      r3, r3, #0x200000
0035ca00: bl       #0x30eab4
0035ca04: mov      r6, r0
0035ca08: mov      r7, r1
0035ca0c: bl       #0x30dfec
0035ca10: mov      r8, r0
0035ca14: mov      sb, r1
0035ca18: mov      r0, r6
0035ca1c: mov      r1, r7
0035ca20: bl       #0x30e148
0035ca24: mov      r6, r0
0035ca28: mov      r0, r5
0035ca2c: mov      r7, r1
0035ca30: bl       #0x30e8a4
0035ca34: mov      r3, #0x3fc00000
0035ca38: mov      r2, #0
0035ca3c: add      r3, r3, #0x200000
0035ca40: bl       #0x30eab4
0035ca44: str      r0, [sp, #4]
0035ca48: str      r1, [sp]
0035ca4c: bl       #0x30dfec
0035ca50: ldr      r2, [sp, #4]
0035ca54: ldr      r3, [sp]
0035ca58: strd     r0, r1, [sp, #0x18]
0035ca5c: mov      r0, r2
0035ca60: mov      r1, r3
0035ca64: bl       #0x30e148
0035ca68: strd     r0, r1, [sp, #8]
0035ca6c: mov      r0, sl
0035ca70: bl       #0x30e8a4
0035ca74: mov      r3, #0x3fc00000
0035ca78: mov      r2, #0
0035ca7c: add      r3, r3, #0x200000
0035ca80: bl       #0x30eab4
0035ca84: mov      sl, r0
0035ca88: mov      fp, r1
0035ca8c: bl       #0x30dfec
0035ca90: strd     r0, r1, [sp, #0x10]
0035ca94: mov      r0, sl
0035ca98: mov      r1, fp
0035ca9c: bl       #0x30e148
0035caa0: mov      sl, r0
0035caa4: mov      fp, r1
0035caa8: mov      r2, sl
0035caac: ldrd     r0, r1, [sp, #8]
0035cab0: mov      r3, fp
0035cab4: bl       #0x30eab4
0035cab8: mov      r2, sl
0035cabc: strd     r0, r1, [sp, #0x20]
0035cac0: ldrd     r0, r1, [sp, #0x18]
0035cac4: mov      r3, fp
0035cac8: bl       #0x30eab4
0035cacc: ldrd     r2, r3, [sp, #0x10]
0035cad0: strd     r0, r1, [sp, #0x28]
0035cad4: ldrd     r0, r1, [sp, #8]
0035cad8: bl       #0x30eab4
0035cadc: ldrd     r2, r3, [sp, #0x10]
0035cae0: mov      sl, r0
0035cae4: mov      fp, r1
0035cae8: ldrd     r0, r1, [sp, #0x18]
0035caec: bl       #0x30eab4
0035caf0: ldrd     r2, r3, [sp, #0x20]
0035caf4: strd     r0, r1, [sp, #0x10]
0035caf8: mov      r0, r8
0035cafc: mov      r1, sb
0035cb00: bl       #0x30eab4
0035cb04: ldrd     r2, r3, [sp, #0x10]
0035cb08: strd     r0, r1, [sp, #8]
0035cb0c: mov      r0, r6
0035cb10: mov      r1, r7
0035cb14: bl       #0x30eab4
0035cb18: mov      r2, r0
0035cb1c: mov      r3, r1
0035cb20: ldrd     r0, r1, [sp, #8]
0035cb24: bl       #0x30e52c
0035cb28: bl       #0x30e6a0
0035cb2c: str      r0, [r4]
0035cb30: ldrd     r2, r3, [sp, #0x28]
0035cb34: mov      r0, r6
0035cb38: mov      r1, r7
0035cb3c: bl       #0x30eab4
0035cb40: mov      r2, sl
0035cb44: strd     r0, r1, [sp, #8]
0035cb48: mov      r3, fp
0035cb4c: mov      r0, r8
0035cb50: mov      r1, sb
0035cb54: bl       #0x30eab4
0035cb58: mov      r2, r0
0035cb5c: mov      r3, r1
0035cb60: ldrd     r0, r1, [sp, #8]
0035cb64: bl       #0x30eb44
0035cb68: bl       #0x30e6a0
0035cb6c: mov      r2, sl
0035cb70: str      r0, [r4, #4]
0035cb74: mov      r3, fp
0035cb78: mov      r0, r6
0035cb7c: mov      r1, r7
0035cb80: bl       #0x30eab4
0035cb84: ldrd     r2, r3, [sp, #0x28]
0035cb88: mov      sl, r0
0035cb8c: mov      fp, r1
0035cb90: mov      r0, r8
0035cb94: mov      r1, sb
0035cb98: bl       #0x30eab4
0035cb9c: mov      r2, r0
0035cba0: mov      r3, r1
0035cba4: mov      r0, sl
0035cba8: mov      r1, fp
0035cbac: bl       #0x30e52c
0035cbb0: bl       #0x30e6a0
0035cbb4: str      r0, [r4, #8]
0035cbb8: ldrd     r2, r3, [sp, #0x20]
0035cbbc: mov      r0, r6
0035cbc0: mov      r1, r7
0035cbc4: bl       #0x30eab4
0035cbc8: ldrd     r2, r3, [sp, #0x10]
0035cbcc: mov      r6, r0
0035cbd0: mov      r7, r1
0035cbd4: mov      r0, r8
0035cbd8: mov      r1, sb
0035cbdc: bl       #0x30eab4
0035cbe0: mov      r2, r0
0035cbe4: mov      r3, r1
0035cbe8: mov      r0, r6
0035cbec: mov      r1, r7
0035cbf0: bl       #0x30eb44
0035cbf4: bl       #0x30e6a0
0035cbf8: str      r0, [r4, #0xc]
0035cbfc: mov      r0, r4
0035cc00: add      sp, sp, #0x34
0035cc04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035cc08: b        #0x35c8f0

# _ZNK6glitch4core8CMatrix4IfE14transformBoxExERNS0_8aabbox3dIfEE
00597548: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059754c: sub      sp, sp, #0x54
00597550: str      r1, [sp, #8]
00597554: str      r0, [sp, #0x14]
00597558: ldr      r8, [r0, #0x30]
0059755c: ldr      r2, [r0, #0x34]
00597560: ldr      r3, [r0, #0x38]
00597564: ldr      r0, [r1]
00597568: ldr      r4, [sp, #8]
0059756c: mov      sb, r8
00597570: str      r0, [sp, #0x1c]
00597574: ldr      ip, [r1, #8]
00597578: ldr      lr, [r1, #4]
0059757c: ldr      r1, [r1, #0xc]
00597580: ldr      r5, [sp, #0x1c]
00597584: str      r1, [sp, #0x18]
00597588: ldr      r0, [r4, #0x10]
0059758c: ldr      r1, [r4, #0x14]
00597590: str      ip, [sp, #0x4c]
00597594: ldr      ip, [sp, #0x18]
00597598: str      r0, [sp, #0x3c]
0059759c: str      r1, [sp, #0x40]
005975a0: str      r2, [sp, #0x30]
005975a4: str      r3, [sp, #0x34]
005975a8: str      r2, [sp, #0x24]
005975ac: str      r3, [sp, #0x28]
005975b0: add      r0, sp, #0x44
005975b4: add      r1, sp, #0x38
005975b8: add      r2, sp, #0x2c
005975bc: add      r3, sp, #0x20
005975c0: str      r5, [sp, #0x44]
005975c4: str      lr, [sp, #0x48]
005975c8: str      ip, [sp, #0x38]
005975cc: str      r8, [sp, #0x20]
005975d0: str      r8, [sp, #0x2c]
005975d4: str      r0, [sp, #0xc]
005975d8: str      r1, [sp, #0x10]
005975dc: mov      r5, #0
005975e0: stm      sp, {r2, r3}
005975e4: ldr      r0, [sp, #0x14]
005975e8: ldr      sl, [sp, #0x18]
005975ec: ldr      r1, [sp, #0x1c]
005975f0: mov      r4, #0
005975f4: add      fp, r0, r5
005975f8: ldr      r7, [fp, r4, lsl #2]
005975fc: mov      r0, r7
00597600: bl       #0x30ed6c
00597604: mov      r1, sl
00597608: mov      r6, r0
0059760c: mov      r0, r7
00597610: bl       #0x30ed6c
00597614: mov      r7, r0
00597618: mov      r1, r7
0059761c: mov      r0, r6
00597620: bl       #0x30e70c
00597624: cmp      r0, #0
00597628: mov      r1, r8
0059762c: mov      r0, r6
00597630: beq      #0x597680
00597634: bl       #0x30eba4
00597638: ldr      ip, [sp]
0059763c: mov      r1, sb
00597640: add      r4, r4, #4
00597644: str      r0, [ip, r5]
00597648: mov      r0, r7
0059764c: bl       #0x30eba4
00597650: ldr      r1, [sp, #4]
00597654: cmp      r4, #0xc
00597658: str      r0, [r1, r5]
0059765c: beq      #0x5976b4
00597660: ldr      ip, [sp, #0xc]
00597664: ldr      r0, [sp, #0x10]
00597668: ldm      sp, {r2, r3}
0059766c: ldr      r1, [ip, r4]
00597670: ldr      sl, [r0, r4]
00597674: ldr      r8, [r2, r5]
00597678: ldr      sb, [r3, r5]
0059767c: b        #0x5975f8
00597680: mov      r1, r8
00597684: mov      r0, r7
00597688: bl       #0x30eba4
0059768c: ldr      r2, [sp]
00597690: mov      r1, sb
00597694: add      r4, r4, #4
00597698: str      r0, [r2, r5]
0059769c: mov      r0, r6
005976a0: bl       #0x30eba4
005976a4: ldr      r3, [sp, #4]
005976a8: cmp      r4, #0xc
005976ac: str      r0, [r3, r5]
005976b0: bne      #0x597660
005976b4: add      r5, r5, #4
005976b8: cmp      r5, #0xc
005976bc: beq      #0x5976d0
005976c0: ldm      sp, {r4, ip}
005976c4: ldr      r8, [r4, r5]
005976c8: ldr      sb, [ip, r5]
005976cc: b        #0x5975e4
005976d0: ldr      ip, [sp, #0x30]
005976d4: ldr      r1, [sp, #0x34]
005976d8: ldr      r2, [sp, #0x20]
005976dc: ldr      r3, [sp, #0x24]
005976e0: ldr      r0, [sp, #0x28]
005976e4: ldr      r4, [sp, #0x2c]
005976e8: ldr      r5, [sp, #8]
005976ec: str      r4, [r5]
005976f0: str      ip, [r5, #4]
005976f4: str      r0, [r5, #0x14]
005976f8: str      r1, [r5, #8]
005976fc: str      r2, [r5, #0xc]
00597700: str      r3, [r5, #0x10]
00597704: add      sp, sp, #0x54
00597708: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN12VisualObject10SetScalingERK7Point3DIfE
004727ac: push     {r4, r5, r6, r7, r8, lr}
004727b0: ldr      r3, [r0, #8]
004727b4: sub      sp, sp, #0x10
004727b8: mov      r5, r0
004727bc: cmp      r3, #0
004727c0: mov      r4, r1
004727c4: beq      #0x47282c
004727c8: mov      r0, r3
004727cc: ldr      r3, [r3]
004727d0: mov      lr, pc
004727d4: ldr      pc, [r3, #0x90]
004727d8: ldr      r7, [r4]
004727dc: ldr      r1, [r0]
004727e0: mov      r6, r0
004727e4: mov      r0, r7
004727e8: bl       #0x30df8c
004727ec: cmp      r0, #0
004727f0: ldr      r8, [r4, #8]
004727f4: ldr      r4, [r4, #4]
004727f8: bne      #0x472834
004727fc: ldr      r0, [r5, #8]
00472800: add      r1, sp, #4
00472804: ldr      r3, [r0]
00472808: ldr      r3, [r3, #0x94]
0047280c: str      r7, [sp, #4]
00472810: str      r4, [sp, #8]
00472814: str      r8, [sp, #0xc]
00472818: blx      r3
0047281c: mov      r0, r5
00472820: bl       #0x47211c
00472824: mov      r0, r5
00472828: bl       #0x470a54
0047282c: add      sp, sp, #0x10
00472830: pop      {r4, r5, r6, r7, r8, pc}
00472834: mov      r0, r4
00472838: ldr      r1, [r6, #4]
0047283c: bl       #0x30df8c
00472840: cmp      r0, #0
00472844: beq      #0x4727fc
00472848: ldr      r1, [r6, #8]
0047284c: mov      r0, r8
00472850: bl       #0x30df8c
00472854: cmp      r0, #0
00472858: bne      #0x47282c
0047285c: b        #0x4727fc

# _ZN12VisualObject11SetRotationERK7Point3DIfE
00472874: push     {r4, r5, r6, lr}
00472878: ldr      r3, [r0, #8]
0047287c: sub      sp, sp, #0x10
00472880: mov      r4, r0
00472884: cmp      r3, #0
00472888: beq      #0x472900
0047288c: ldr      r2, [r1]
00472890: ldr      r3, [r1, #8]
00472894: mov      r0, sp
00472898: add      r2, r2, #0x80000000
0047289c: ldr      r1, [r1, #4]
004728a0: add      r3, r3, #0x80000000
004728a4: bl       #0x35c9d8
004728a8: ldr      r3, [r4, #8]
004728ac: mov      r5, sp
004728b0: mov      r0, r3
004728b4: ldr      r3, [r3]
004728b8: mov      lr, pc
004728bc: ldr      pc, [r3, #0x98]
004728c0: ldr      r1, [sp]
004728c4: mov      r6, r0
004728c8: ldr      r0, [r0]
004728cc: bl       #0x30df8c
004728d0: cmp      r0, #0
004728d4: bne      #0x472908
004728d8: ldr      r3, [r4, #8]
004728dc: mov      r1, sp
004728e0: mov      r0, r3
004728e4: ldr      r3, [r3]
004728e8: mov      lr, pc
004728ec: ldr      pc, [r3, #0x9c]
004728f0: mov      r0, r4
004728f4: bl       #0x47211c
004728f8: mov      r0, r4
004728fc: bl       #0x470a54
00472900: add      sp, sp, #0x10
00472904: pop      {r4, r5, r6, pc}
00472908: ldr      r0, [r6, #4]
0047290c: ldr      r1, [sp, #4]
00472910: bl       #0x30df8c
00472914: cmp      r0, #0
00472918: beq      #0x4728d8
0047291c: ldr      r0, [r6, #8]
00472920: ldr      r1, [sp, #8]
00472924: bl       #0x30df8c
00472928: cmp      r0, #0
0047292c: beq      #0x4728d8
00472930: ldr      r0, [r6, #0xc]
00472934: ldr      r1, [sp, #0xc]
00472938: bl       #0x30df8c
0047293c: cmp      r0, #0
00472940: bne      #0x472900
00472944: b        #0x4728d8

# _ZNK6glitch4core10quaternion20getMatrix_transposedERNS0_8CMatrix4IfEE
005602d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005602d4: ldr      r6, [r0]
005602d8: sub      sp, sp, #0x1c
005602dc: mov      r4, r1
005602e0: mov      r8, r0
005602e4: mov      r1, r6
005602e8: mov      r0, r6
005602ec: bl       #0x30eba4
005602f0: mov      sl, r0
005602f4: mov      r1, sl
005602f8: mov      r0, r6
005602fc: bl       #0x30ed6c
00560300: str      r0, [sp, #4]
00560304: ldr      r6, [r8, #4]
00560308: mov      r5, #0
0056030c: mov      r1, r6
00560310: mov      r0, r6
00560314: bl       #0x30eba4
00560318: ldr      sb, [r8, #8]
0056031c: mov      r7, r0
00560320: mov      r1, sb
00560324: mov      r0, sb
00560328: bl       #0x30eba4
0056032c: mov      fp, r0
00560330: mov      r1, fp
00560334: mov      r0, sb
00560338: bl       #0x30ed6c
0056033c: mov      r1, r6
00560340: str      r0, [sp, #8]
00560344: mov      r0, sl
00560348: bl       #0x30ed6c
0056034c: mov      r1, sb
00560350: str      r0, [sp, #0xc]
00560354: mov      r0, sl
00560358: bl       #0x30ed6c
0056035c: str      r0, [sp, #0x10]
00560360: ldr      r8, [r8, #0xc]
00560364: mov      r0, sl
00560368: mov      r1, r8
0056036c: bl       #0x30ed6c
00560370: mov      r1, sb
00560374: mov      sl, r0
00560378: mov      r0, r7
0056037c: bl       #0x30ed6c
00560380: mov      r1, r8
00560384: str      r0, [sp, #0x14]
00560388: mov      r0, r7
0056038c: bl       #0x30ed6c
00560390: mov      r1, r8
00560394: mov      sb, r0
00560398: mov      r0, fp
0056039c: bl       #0x30ed6c
005603a0: mov      r3, #0
005603a4: strb     r3, [r4, #0x40]
005603a8: mov      r8, r0
005603ac: mov      r1, r7
005603b0: mov      r0, r6
005603b4: bl       #0x30ed6c
005603b8: mov      r1, r0
005603bc: mov      r0, #0x3f800000
005603c0: bl       #0x30e3ac
005603c4: ldr      r1, [sp, #8]
005603c8: mov      r6, r0
005603cc: bl       #0x30e3ac
005603d0: str      r0, [r4]
005603d4: ldr      r0, [sp, #0xc]
005603d8: mov      r1, r8
005603dc: bl       #0x30eba4
005603e0: str      r0, [r4, #0x10]
005603e4: ldr      r0, [sp, #0x10]
005603e8: mov      r1, sb
005603ec: bl       #0x30e3ac
005603f0: str      r5, [r4, #0x30]
005603f4: str      r0, [r4, #0x20]
005603f8: ldr      r0, [sp, #0xc]
005603fc: mov      r1, r8
00560400: bl       #0x30e3ac
00560404: str      r0, [r4, #4]
00560408: ldr      r1, [sp, #4]
0056040c: mov      r0, #0x3f800000
00560410: bl       #0x30e3ac
00560414: ldr      r1, [sp, #8]
00560418: bl       #0x30e3ac
0056041c: str      r0, [r4, #0x14]
00560420: ldr      r0, [sp, #0x14]
00560424: mov      r1, sl
00560428: bl       #0x30eba4
0056042c: str      r5, [r4, #0x34]
00560430: str      r0, [r4, #0x24]
00560434: mov      r1, sb
00560438: ldr      r0, [sp, #0x10]
0056043c: bl       #0x30eba4
00560440: mov      r1, sl
00560444: str      r0, [r4, #8]
00560448: ldr      r0, [sp, #0x14]
0056044c: bl       #0x30e3ac
00560450: str      r0, [r4, #0x18]
00560454: ldr      r1, [sp, #4]
00560458: mov      r0, r6
0056045c: bl       #0x30e3ac
00560460: mov      r3, #0x3f800000
00560464: str      r0, [r4, #0x28]
00560468: str      r5, [r4, #0x2c]
0056046c: str      r3, [r4, #0x3c]
00560470: str      r5, [r4, #0x38]
00560474: str      r5, [r4, #0xc]
00560478: str      r5, [r4, #0x1c]
0056047c: add      sp, sp, #0x1c
00560480: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CRootSceneNode10onPostLoadEv
0065b3dc: push     {r4, lr}
0065b3e0: mov      r4, r0
0065b3e4: bl       #0x65b380
0065b3e8: mov      r0, r4
0065b3ec: bl       #0x65b350
0065b3f0: mov      r0, r4
0065b3f4: bl       #0x65ae70
0065b3f8: mov      r0, r4
0065b3fc: mov      r1, #1
0065b400: ldr      r3, [r4]
0065b404: mov      lr, pc
0065b408: ldr      pc, [r3, #0xb8]
0065b40c: ldr      r3, [r4]
0065b410: mov      r0, r4
0065b414: mov      lr, pc
0065b418: ldr      pc, [r3, #0xf4]
0065b41c: ldr      r3, [r4, #0x11c]
0065b420: mov      r2, #1
0065b424: strb     r2, [r4, #0x1a8]
0065b428: orr      r3, r3, #0x100
0065b42c: str      r3, [r4, #0x11c]
0065b430: pop      {r4, pc}

# _ZN6glitch7collada15CColladaFactory10createNodeERKNS0_16CColladaDatabaseEPNS0_5SNodeE
00631800: push     {r4, r5, r6, lr}
00631804: mov      r0, #0x160
00631808: mov      r5, r1
0063180c: mov      r1, #0
00631810: mov      r6, r2
00631814: bl       #0x5341ac
00631818: mov      r1, r5
0063181c: mov      r4, r0
00631820: mov      r2, r6
00631824: bl       #0x65d150
00631828: mov      r0, r4
0063182c: pop      {r4, r5, r6, pc}

# _ZN6glitch4core10quaternion9normalizeEv
0035c8f0: push     {r4, r5, r6, r7, r8, lr}
0035c8f4: mov      r4, r0
0035c8f8: ldr      r0, [r0]
0035c8fc: ldr      r7, [r4, #4]
0035c900: ldr      r6, [r4, #8]
0035c904: mov      r1, r0
0035c908: bl       #0x30ed6c
0035c90c: mov      r1, r7
0035c910: mov      r5, r0
0035c914: mov      r0, r7
0035c918: bl       #0x30ed6c
0035c91c: mov      r1, r0
0035c920: mov      r0, r5
0035c924: bl       #0x30eba4
0035c928: mov      r1, r6
0035c92c: mov      r5, r0
0035c930: mov      r0, r6
0035c934: bl       #0x30ed6c
0035c938: mov      r1, r0
0035c93c: mov      r0, r5
0035c940: bl       #0x30eba4
0035c944: ldr      r6, [r4, #0xc]
0035c948: mov      r5, r0
0035c94c: mov      r1, r6
0035c950: mov      r0, r6
0035c954: bl       #0x30ed6c
0035c958: mov      r1, r0
0035c95c: mov      r0, r5
0035c960: bl       #0x30eba4
0035c964: mov      r1, #0x3f800000
0035c968: mov      r5, r0
0035c96c: bl       #0x30df8c
0035c970: cmp      r0, #0
0035c974: bne      #0x35c9d0
0035c978: mov      r0, r5
0035c97c: bl       #0x30e124
0035c980: mov      r1, r0
0035c984: mov      r0, #0x3f800000
0035c988: bl       #0x30ec94
0035c98c: mov      r5, r0
0035c990: mov      r1, r0
0035c994: ldr      r0, [r4]
0035c998: bl       #0x30ed6c
0035c99c: mov      r1, r5
0035c9a0: str      r0, [r4]
0035c9a4: ldr      r0, [r4, #4]
0035c9a8: bl       #0x30ed6c
0035c9ac: mov      r1, r5
0035c9b0: str      r0, [r4, #4]
0035c9b4: ldr      r0, [r4, #8]
0035c9b8: bl       #0x30ed6c
0035c9bc: mov      r1, r5
0035c9c0: str      r0, [r4, #8]
0035c9c4: ldr      r0, [r4, #0xc]
0035c9c8: bl       #0x30ed6c
0035c9cc: str      r0, [r4, #0xc]
0035c9d0: mov      r0, r4
0035c9d4: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeEPNS0_14CRootSceneNodeE
0061b2f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061b2f8: subs     r4, r2, #0
0061b2fc: sub      sp, sp, #0x44
0061b300: mov      r8, r0
0061b304: mov      fp, r1
0061b308: mov      sl, r3
0061b30c: moveq    r6, r4
0061b310: beq      #0x61b55c
0061b314: ldr      r3, [r4, #0x4c]
0061b318: cmp      r3, #0
0061b31c: beq      #0x61b874
0061b320: ldr      r3, [r0, #4]
0061b324: mov      r1, r0
0061b328: mov      r0, r3
0061b32c: ldr      r3, [r3]
0061b330: mov      lr, pc
0061b334: ldr      pc, [r3, #0x40]
0061b338: mov      r6, r0
0061b33c: ldr      r1, [r4, #0x40]
0061b340: cmp      r1, #0
0061b344: ble      #0x61b434
0061b348: add      r3, sp, #0x38
0061b34c: add      ip, sp, #0x3c
0061b350: mov      r5, #0
0061b354: str      r3, [sp, #8]
0061b358: str      ip, [sp, #0xc]
0061b35c: mov      r7, r6
0061b360: ldr      r2, [r4, #0x44]
0061b364: lsl      r6, r5, #3
0061b368: ldr      r3, [r2, r5, lsl #3]
0061b36c: add      r2, r2, r6
0061b370: sub      r3, r3, #1
0061b374: cmp      r3, #0xc
0061b378: addls    pc, pc, r3, lsl #2
0061b37c: b        #0x61b424
0061b380: b        #0x61b84c
0061b384: b        #0x61b720
0061b388: b        #0x61b68c
0061b38c: b        #0x61b664
0061b390: b        #0x61b424
0061b394: b        #0x61b424
0061b398: b        #0x61b424
0061b39c: b        #0x61b424
0061b3a0: b        #0x61b804
0061b3a4: b        #0x61b3b4
0061b3a8: b        #0x61b828
0061b3ac: b        #0x61b614
0061b3b0: b        #0x61b568
0061b3b4: ldr      r1, [r2, #4]
0061b3b8: mov      r0, r8
0061b3bc: mov      r2, fp
0061b3c0: mov      r3, sl
0061b3c4: bl       #0x61a608
0061b3c8: subs     sb, r0, #0
0061b3cc: beq      #0x61b600
0061b3d0: ldr      r2, [r4, #0x44]
0061b3d4: ldr      r3, [sb]
0061b3d8: add      r6, r2, r6
0061b3dc: ldr      r2, [r6, #4]
0061b3e0: ldr      r1, [r2, #0x14]
0061b3e4: mov      lr, pc
0061b3e8: ldr      pc, [r3, #0xd4]
0061b3ec: mov      r0, sb
0061b3f0: ldr      r3, [sb]
0061b3f4: mov      lr, pc
0061b3f8: ldr      pc, [r3, #0x104]
0061b3fc: mov      r0, r7
0061b400: ldr      r3, [r7]
0061b404: mov      r1, sb
0061b408: mov      lr, pc
0061b40c: ldr      pc, [r3, #0x5c]
0061b410: ldr      r3, [sb]
0061b414: ldr      r0, [r3, #-0xc]
0061b418: add      r0, sb, r0
0061b41c: bl       #0x31d584
0061b420: ldr      r1, [r4, #0x40]
0061b424: add      r5, r5, #1
0061b428: cmp      r5, r1
0061b42c: blt      #0x61b360
0061b430: mov      r6, r7
0061b434: mov      r0, r6
0061b438: ldr      r1, [r4, #4]
0061b43c: ldr      r3, [r6]
0061b440: mov      lr, pc
0061b444: ldr      pc, [r3, #0x28]
0061b448: ldr      r3, [r6]
0061b44c: ldr      r2, [r4, #0xc]
0061b450: mov      r0, r6
0061b454: ldr      r3, [r3, #0xa4]
0061b458: str      r2, [sp, #0x2c]
0061b45c: ldr      r2, [r4, #0x10]
0061b460: add      r1, sp, #0x2c
0061b464: str      r2, [sp, #0x30]
0061b468: ldr      r2, [r4, #0x14]
0061b46c: str      r2, [sp, #0x34]
0061b470: blx      r3
0061b474: ldr      r3, [r6]
0061b478: ldr      r2, [r4, #0x18]
0061b47c: mov      r0, r6
0061b480: ldr      r3, [r3, #0x9c]
0061b484: str      r2, [sp, #0x10]
0061b488: ldr      r2, [r4, #0x1c]
0061b48c: add      r1, sp, #0x10
0061b490: str      r2, [sp, #0x14]
0061b494: ldr      r2, [r4, #0x20]
0061b498: str      r2, [sp, #0x18]
0061b49c: ldr      r2, [r4, #0x24]
0061b4a0: str      r2, [sp, #0x1c]
0061b4a4: blx      r3
0061b4a8: ldr      r3, [r6]
0061b4ac: ldr      r2, [r4, #0x28]
0061b4b0: mov      r0, r6
0061b4b4: ldr      r3, [r3, #0x94]
0061b4b8: str      r2, [sp, #0x20]
0061b4bc: ldr      r2, [r4, #0x2c]
0061b4c0: add      r1, sp, #0x20
0061b4c4: str      r2, [sp, #0x24]
0061b4c8: ldr      r2, [r4, #0x30]
0061b4cc: str      r2, [sp, #0x28]
0061b4d0: blx      r3
0061b4d4: ldr      r1, [r4, #0x34]
0061b4d8: ldr      r3, [r6]
0061b4dc: mov      r0, r6
0061b4e0: subs     r1, r1, #0
0061b4e4: movne    r1, #1
0061b4e8: mov      lr, pc
0061b4ec: ldr      pc, [r3, #0x48]
0061b4f0: ldr      r3, [r4, #0x38]
0061b4f4: cmp      r3, #0
0061b4f8: ble      #0x61b55c
0061b4fc: mov      r5, #0
0061b500: mov      r7, r5
0061b504: mov      sb, r8
0061b508: ldr      r2, [r4, #0x3c]
0061b50c: mov      r3, sl
0061b510: mov      r1, fp
0061b514: add      r2, r2, r5
0061b518: mov      r0, sb
0061b51c: bl       #0x61b2f4
0061b520: ldr      r3, [r6]
0061b524: mov      r8, r0
0061b528: mov      r1, r0
0061b52c: mov      r0, r6
0061b530: mov      lr, pc
0061b534: ldr      pc, [r3, #0x5c]
0061b538: ldr      r3, [r8]
0061b53c: add      r7, r7, #1
0061b540: add      r5, r5, #0x50
0061b544: ldr      r0, [r3, #-0xc]
0061b548: add      r0, r8, r0
0061b54c: bl       #0x31d584
0061b550: ldr      r3, [r4, #0x38]
0061b554: cmp      r7, r3
0061b558: blt      #0x61b508
0061b55c: mov      r0, r6
0061b560: add      sp, sp, #0x44
0061b564: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061b568: ldr      r2, [r2, #4]
0061b56c: ldr      r0, [sp, #8]
0061b570: mov      r1, r8
0061b574: mov      r3, sl
0061b578: bl       #0x60e6f0
0061b57c: ldr      r3, [r8, #4]
0061b580: mov      r1, r8
0061b584: ldr      r2, [sp, #8]
0061b588: mov      r0, r3
0061b58c: ldr      ip, [r3]
0061b590: ldr      r3, [r4, #0x48]
0061b594: mov      lr, pc
0061b598: ldr      pc, [ip, #0x50]
0061b59c: subs     sb, r0, #0
0061b5a0: beq      #0x61b5f0
0061b5a4: ldr      r2, [r4, #0x44]
0061b5a8: ldr      r3, [sb]
0061b5ac: add      r6, r2, r6
0061b5b0: ldr      r2, [r6, #4]
0061b5b4: ldr      r1, [r2, #0x14]
0061b5b8: mov      lr, pc
0061b5bc: ldr      pc, [r3, #0xd4]
0061b5c0: mov      r0, sb
0061b5c4: mov      r1, #2
0061b5c8: bl       #0x59719c
0061b5cc: mov      r0, r7
0061b5d0: ldr      r3, [r7]
0061b5d4: mov      r1, sb
0061b5d8: mov      lr, pc
0061b5dc: ldr      pc, [r3, #0x5c]
0061b5e0: ldr      r3, [sb]
0061b5e4: ldr      r0, [r3, #-0xc]
0061b5e8: add      r0, sb, r0
0061b5ec: bl       #0x31d584
0061b5f0: ldr      r0, [sp, #0x38]
0061b5f4: cmp      r0, #0
0061b5f8: beq      #0x61b600
0061b5fc: bl       #0x31d584
0061b600: ldr      r1, [r4, #0x40]
0061b604: add      r5, r5, #1
0061b608: cmp      r5, r1
0061b60c: blt      #0x61b360
0061b610: b        #0x61b430
0061b614: ldr      r1, [r2, #4]
0061b618: mov      r0, r8
0061b61c: mov      r2, sl
0061b620: bl       #0x61a994
0061b624: subs     r6, r0, #0
0061b628: beq      #0x61b600
0061b62c: mov      r1, r6
0061b630: mov      r0, r7
0061b634: ldr      r3, [r7]
0061b638: mov      lr, pc
0061b63c: ldr      pc, [r3, #0x5c]
0061b640: ldr      r3, [r6]
0061b644: add      r5, r5, #1
0061b648: ldr      r0, [r3, #-0xc]
0061b64c: add      r0, r6, r0
0061b650: bl       #0x31d584
0061b654: ldr      r1, [r4, #0x40]
0061b658: cmp      r5, r1
0061b65c: blt      #0x61b360
0061b660: b        #0x61b430
0061b664: ldr      r3, [r2, #4]
0061b668: mov      r0, r8
0061b66c: mov      r2, sl
0061b670: ldr      r1, [r3, #4]
0061b674: add      r1, r1, #1
0061b678: bl       #0x61b24c
0061b67c: subs     r6, r0, #0
0061b680: bne      #0x61b62c
0061b684: ldr      r1, [r4, #0x40]
0061b688: b        #0x61b604
0061b68c: ldr      r3, [r2, #4]
0061b690: ldr      r0, [sp, #0xc]
0061b694: mov      r1, r8
0061b698: mov      r2, fp
0061b69c: str      sl, [sp]
0061b6a0: bl       #0x61aeb8
0061b6a4: ldr      r0, [sp, #0x3c]
0061b6a8: cmp      r0, #0
0061b6ac: str      r0, [sp, #0x38]
0061b6b0: ldrne    r3, [r0, #4]
0061b6b4: addne    r3, r3, #1
0061b6b8: strne    r3, [r0, #4]
0061b6bc: ldrne    r0, [sp, #0x3c]
0061b6c0: cmp      r0, #0
0061b6c4: beq      #0x61b6cc
0061b6c8: bl       #0x31d584
0061b6cc: ldr      r3, [sp, #0x38]
0061b6d0: cmp      r3, #0
0061b6d4: beq      #0x61b600
0061b6d8: ldr      r3, [r8, #4]
0061b6dc: mov      r1, r8
0061b6e0: ldr      r2, [sp, #8]
0061b6e4: mov      r0, r3
0061b6e8: ldr      ip, [r3]
0061b6ec: ldr      r3, [r4, #0x48]
0061b6f0: mov      lr, pc
0061b6f4: ldr      pc, [ip, #0x48]
0061b6f8: subs     sb, r0, #0
0061b6fc: beq      #0x61b7f0
0061b700: ldr      r2, [r4, #0x44]
0061b704: ldr      r3, [sb]
0061b708: add      r6, r2, r6
0061b70c: ldr      r2, [r6, #4]
0061b710: ldr      r1, [r2, #0x14]
0061b714: mov      lr, pc
0061b718: ldr      pc, [r3, #0xd4]
0061b71c: b        #0x61b7cc
0061b720: ldr      r3, [r2, #4]
0061b724: mov      ip, #1
0061b728: ldr      r0, [sp, #8]
0061b72c: mov      r1, r8
0061b730: mov      r2, fp
0061b734: stm      sp, {sl, ip}
0061b738: bl       #0x61ace8
0061b73c: ldr      r3, [sp, #0x38]
0061b740: mov      r0, r3
0061b744: ldr      r3, [r3]
0061b748: mov      lr, pc
0061b74c: ldr      pc, [r3, #0x30]
0061b750: cmp      r0, #2
0061b754: beq      #0x61b894
0061b758: ldr      r3, [sp, #0x38]
0061b75c: mov      r0, r3
0061b760: ldr      r3, [r3]
0061b764: mov      lr, pc
0061b768: ldr      pc, [r3, #0x30]
0061b76c: cmp      r0, #3
0061b770: beq      #0x61b894
0061b774: ldr      r3, [r8, #4]
0061b778: mov      r1, r8
0061b77c: ldr      r2, [sp, #8]
0061b780: mov      r0, r3
0061b784: ldr      ip, [r3]
0061b788: ldr      r3, [r4, #0x48]
0061b78c: mov      lr, pc
0061b790: ldr      pc, [ip, #0x48]
0061b794: mov      sb, r0
0061b798: cmp      sb, #0
0061b79c: beq      #0x61b7f0
0061b7a0: ldr      r2, [r4, #0x44]
0061b7a4: mov      r0, sb
0061b7a8: ldr      r3, [sb]
0061b7ac: add      r6, r2, r6
0061b7b0: ldr      r2, [r6, #4]
0061b7b4: ldr      r1, [r2, #0x14]
0061b7b8: mov      lr, pc
0061b7bc: ldr      pc, [r3, #0xd4]
0061b7c0: mov      r0, sb
0061b7c4: mov      r1, #2
0061b7c8: bl       #0x59719c
0061b7cc: mov      r0, r7
0061b7d0: ldr      r3, [r7]
0061b7d4: mov      r1, sb
0061b7d8: mov      lr, pc
0061b7dc: ldr      pc, [r3, #0x5c]
0061b7e0: ldr      r3, [sb]
0061b7e4: ldr      r0, [r3, #-0xc]
0061b7e8: add      r0, sb, r0
0061b7ec: bl       #0x31d584
0061b7f0: ldr      r0, [sp, #0x38]
0061b7f4: cmp      r0, #0
0061b7f8: bne      #0x61b41c
0061b7fc: ldr      r1, [r4, #0x40]
0061b800: b        #0x61b604
0061b804: ldr      r1, [r2, #4]
0061b808: mov      r0, r8
0061b80c: mov      r2, fp
0061b810: mov      r3, sl
0061b814: bl       #0x61a888
0061b818: subs     sb, r0, #0
0061b81c: bne      #0x61b3d0
0061b820: ldr      r1, [r4, #0x40]
0061b824: b        #0x61b604
0061b828: ldr      r1, [r2, #4]
0061b82c: mov      r0, r8
0061b830: mov      r2, fp
0061b834: mov      r3, sl
0061b838: bl       #0x61a77c
0061b83c: subs     r6, r0, #0
0061b840: bne      #0x61b62c
0061b844: ldr      r1, [r4, #0x40]
0061b848: b        #0x61b604
0061b84c: ldr      r3, [r2, #4]
0061b850: mov      r0, r8
0061b854: mov      r2, sl
0061b858: ldr      r1, [r3, #4]
0061b85c: add      r1, r1, #1
0061b860: bl       #0x61b2d0
0061b864: subs     r6, r0, #0
0061b868: bne      #0x61b62c
0061b86c: ldr      r1, [r4, #0x40]
0061b870: b        #0x61b604
0061b874: ldr      r3, [r0, #4]
0061b878: mov      r1, r0
0061b87c: mov      r0, r3
0061b880: ldr      r3, [r3]
0061b884: mov      lr, pc
0061b888: ldr      pc, [r3, #0x3c]
0061b88c: mov      r6, r0
0061b890: b        #0x61b33c
0061b894: ldr      r3, [r8, #4]
0061b898: mov      r1, r8
0061b89c: ldr      r2, [sp, #8]
0061b8a0: mov      r0, r3
0061b8a4: ldr      ip, [r3]
0061b8a8: ldr      r3, [r4, #0x48]
0061b8ac: mov      lr, pc
0061b8b0: ldr      pc, [ip, #0x4c]
0061b8b4: mov      sb, r0
0061b8b8: b        #0x61b798

# _ZN6glitch4core8CMatrix4IfE9postScaleERKNS0_8vector3dIfEE
00597788: push     {r4, r5, r6, lr}
0059778c: ldrb     r3, [r0, #0x40]
00597790: mov      r4, r0
00597794: mov      r5, r1
00597798: cmp      r3, #0
0059779c: bne      #0x59783c
005977a0: strb     r3, [r0, #0x40]
005977a4: ldr      r1, [r1]
005977a8: ldr      r0, [r0]
005977ac: bl       #0x30ed6c
005977b0: str      r0, [r4]
005977b4: ldr      r1, [r5]
005977b8: ldr      r0, [r4, #4]
005977bc: bl       #0x30ed6c
005977c0: str      r0, [r4, #4]
005977c4: ldr      r1, [r5]
005977c8: ldr      r0, [r4, #8]
005977cc: bl       #0x30ed6c
005977d0: str      r0, [r4, #8]
005977d4: ldr      r1, [r5, #4]
005977d8: ldr      r0, [r4, #0x10]
005977dc: bl       #0x30ed6c
005977e0: str      r0, [r4, #0x10]
005977e4: ldr      r1, [r5, #4]
005977e8: ldr      r0, [r4, #0x14]
005977ec: bl       #0x30ed6c
005977f0: str      r0, [r4, #0x14]
005977f4: ldr      r1, [r5, #4]
005977f8: ldr      r0, [r4, #0x18]
005977fc: bl       #0x30ed6c
00597800: str      r0, [r4, #0x18]
00597804: ldr      r1, [r5, #8]
00597808: ldr      r0, [r4, #0x20]
0059780c: bl       #0x30ed6c
00597810: str      r0, [r4, #0x20]
00597814: ldr      r1, [r5, #8]
00597818: ldr      r0, [r4, #0x24]
0059781c: bl       #0x30ed6c
00597820: str      r0, [r4, #0x24]
00597824: ldr      r1, [r5, #8]
00597828: ldr      r0, [r4, #0x28]
0059782c: bl       #0x30ed6c
00597830: str      r0, [r4, #0x28]
00597834: mov      r0, r4
00597838: pop      {r4, r5, r6, pc}
0059783c: mov      r3, #0
00597840: strb     r3, [r0, #0x40]
00597844: ldr      r3, [r1]
00597848: str      r3, [r0]
0059784c: ldr      r3, [r1, #4]
00597850: str      r3, [r0, #0x14]
00597854: ldr      r3, [r1, #8]
00597858: str      r3, [r0, #0x28]
0059785c: mov      r0, r4
00597860: pop      {r4, r5, r6, pc}

# _ZN10GameObject8InitPostEv
0038be5c: push     {r4, r5, r6, r7, r8, lr}
0038be60: mov      r4, r0
0038be64: bl       #0x33ec0c
0038be68: mov      r0, r4
0038be6c: bl       #0x38bd64
0038be70: ldr      r3, [r4, #0x274]
0038be74: ldr      r5, [pc, #0x204]
0038be78: cmp      r0, r3
0038be7c: add      r5, pc, r5
0038be80: bge      #0x38c02c
0038be84: ldr      r0, [r4, #0x120]
0038be88: mov      r3, #0
0038be8c: movw     r1, #0xb717
0038be90: str      r3, [r4, #0x2dc]
0038be94: movt     r1, #0x38d1
0038be98: bic      r0, r0, #0x80000000
0038be9c: bl       #0x30e70c
0038bea0: cmp      r0, #0
0038bea4: ldr      r0, [r4, #0x124]
0038bea8: movne    r3, #0x3f800000
0038beac: movw     r1, #0xb717
0038beb0: strne    r3, [r4, #0x120]
0038beb4: movt     r1, #0x38d1
0038beb8: bic      r0, r0, #0x80000000
0038bebc: bl       #0x30e70c
0038bec0: cmp      r0, #0
0038bec4: ldr      r0, [r4, #0x128]
0038bec8: movne    r3, #0x3f800000
0038becc: movw     r1, #0xb717
0038bed0: strne    r3, [r4, #0x124]
0038bed4: movt     r1, #0x38d1
0038bed8: bic      r0, r0, #0x80000000
0038bedc: bl       #0x30e70c
0038bee0: cmp      r0, #0
0038bee4: movne    r3, #0x3f800000
0038bee8: movw     r1, #0xfa35
0038beec: strne    r3, [r4, #0x128]
0038bef0: ldr      r0, [r4, #0x16c]
0038bef4: movt     r1, #0x3c8e
0038bef8: bl       #0x30ed6c
0038befc: movw     r1, #0xfa35
0038bf00: str      r0, [r4, #0x16c]
0038bf04: movt     r1, #0x3c8e
0038bf08: ldr      r0, [r4, #0x170]
0038bf0c: bl       #0x30ed6c
0038bf10: movw     r1, #0xfa35
0038bf14: str      r0, [r4, #0x170]
0038bf18: movt     r1, #0x3c8e
0038bf1c: ldr      r0, [r4, #0x174]
0038bf20: bl       #0x30ed6c
0038bf24: mov      r2, #1
0038bf28: str      r0, [r4, #0x178]
0038bf2c: str      r0, [r4, #0x174]
0038bf30: add      r1, r4, #0x160
0038bf34: mov      r0, r4
0038bf38: bl       #0x393db4
0038bf3c: ldr      r0, [r4, #0x144]
0038bf40: ldr      r1, [r4, #0x120]
0038bf44: bl       #0x30ed6c
0038bf48: ldr      r1, [r4, #0x124]
0038bf4c: str      r0, [r4, #0x144]
0038bf50: ldr      r0, [r4, #0x148]
0038bf54: bl       #0x30ed6c
0038bf58: ldr      r1, [r4, #0x128]
0038bf5c: str      r0, [r4, #0x148]
0038bf60: ldr      r0, [r4, #0x14c]
0038bf64: bl       #0x30ed6c
0038bf68: ldr      r1, [r4, #0x120]
0038bf6c: str      r0, [r4, #0x14c]
0038bf70: ldr      r0, [r4, #0x150]
0038bf74: bl       #0x30ed6c
0038bf78: ldr      r1, [r4, #0x124]
0038bf7c: str      r0, [r4, #0x150]
0038bf80: ldr      r0, [r4, #0x154]
0038bf84: bl       #0x30ed6c
0038bf88: ldr      r1, [r4, #0x128]
0038bf8c: str      r0, [r4, #0x154]
0038bf90: ldr      r0, [r4, #0x158]
0038bf94: bl       #0x30ed6c
0038bf98: str      r0, [r4, #0x158]
0038bf9c: mov      r0, r4
0038bfa0: bl       #0x38aac8
0038bfa4: ldr      r0, [r4, #0x2d8]
0038bfa8: cmp      r0, #0
0038bfac: beq      #0x38c040
0038bfb0: bl       #0x38ba74
0038bfb4: ldrb     r3, [r4, #0x15c]
0038bfb8: ldr      r7, [r4, #0x36c]
0038bfbc: cmp      r3, #0
0038bfc0: movne    r3, #0
0038bfc4: strbne   r3, [r4, #0x28]
0038bfc8: ldr      r3, [r4, #0x368]
0038bfcc: cmp      r3, r7
0038bfd0: beq      #0x38c02c
0038bfd4: ldr      r3, [pc, #0xa8]
0038bfd8: ldr      r3, [r5, r3]
0038bfdc: ldr      r8, [r3]
0038bfe0: cmp      r8, #0
0038bfe4: beq      #0x38c030
0038bfe8: ldr      r3, [pc, #0x98]
0038bfec: mov      r6, #0
0038bff0: ldr      r3, [r5, r3]
0038bff4: ldr      r5, [r3]
0038bff8: b        #0x38c008
0038bffc: add      r6, r6, #1
0038c000: cmp      r6, r8
0038c004: beq      #0x38c030
0038c008: ldr      r1, [r5, r6, lsl #2]
0038c00c: mov      r0, r7
0038c010: bl       #0x30e31c
0038c014: cmp      r0, #0
0038c018: bne      #0x38bffc
0038c01c: uxth     r6, r6
0038c020: mov      r3, #0x370
0038c024: strh     r6, [r4, r3]
0038c028: pop      {r4, r5, r6, r7, r8, pc}
0038c02c: pop      {r4, r5, r6, r7, r8, pc}
0038c030: movw     r6, #0xffff
0038c034: mov      r3, #0x370
0038c038: strh     r6, [r4, r3]
0038c03c: pop      {r4, r5, r6, r7, r8, pc}
0038c040: bl       #0x38174c
0038c044: cmp      r0, #0
0038c048: bne      #0x38c068
0038c04c: ldrb     r3, [r4, #0x60]
0038c050: cmp      r3, #0
0038c054: beq      #0x38c068
0038c058: ldrb     r3, [r4, #0x10c]
0038c05c: cmp      r3, #0
0038c060: strne    r0, [r4, #0x2d8]
0038c064: bne      #0x38bfb4
0038c068: mov      r0, r4
0038c06c: bl       #0x394eb0
0038c070: ldr      r0, [r4, #0x2d8]
0038c074: cmp      r0, #0
0038c078: beq      #0x38bfb4
0038c07c: b        #0x38bfb0
0038c080: rsbeq    r8, r0, r4, lsl ip
0038c084: andeq    r3, r0, r8, lsr sp
0038c088: andeq    r3, r0, r8, lsr #19
