
# _ZN7gameswf4root7advanceEfb
00775304: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00775308: mov      r5, r1
0077530c: sub      sp, sp, #0xc
00775310: add      r6, r0, #0xb8
00775314: mov      r4, r0
00775318: mov      r7, r2
0077531c: bl       #0x773d38
00775320: mov      r1, r5
00775324: mov      r0, r6
00775328: bl       #0x760d58
0077532c: ldr      r1, [r4, #0x8c]
00775330: mov      r0, r5
00775334: bl       #0x30eba4
00775338: mov      r1, r5
0077533c: mov      r8, r0
00775340: str      r0, [r4, #0x8c]
00775344: ldr      r0, [r4, #0x94]
00775348: bl       #0x30e3ac
0077534c: ldr      r1, [r4, #0x90]
00775350: str      r0, [r4, #0x94]
00775354: mov      r0, r8
00775358: bl       #0x30e4b4
0077535c: cmp      r0, #0
00775360: bne      #0x775370
00775364: bl       #0x773d38
00775368: add      sp, sp, #0xc
0077536c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00775370: bl       #0x7b7898
00775374: ldrb     r3, [r4, #0x84]
00775378: cmp      r3, #0
0077537c: beq      #0x77548c
00775380: ldr      r1, [r4, #0x8c]
00775384: mov      sb, #1
00775388: mov      sl, #0xa
0077538c: mov      fp, sp
00775390: ldr      r0, [r4, #0x90]
00775394: bl       #0x30e9ac
00775398: cmp      r0, #0
0077539c: beq      #0x77545c
007753a0: ldrb     r3, [r4, #0x84]
007753a4: cmp      r3, #0
007753a8: bne      #0x7753f0
007753ac: ldr      r8, [r4, #0x10]
007753b0: cmp      r8, #0
007753b4: beq      #0x775484
007753b8: ldr      r3, [r8]
007753bc: mov      r0, r8
007753c0: mov      r1, #2
007753c4: mov      lr, pc
007753c8: ldr      pc, [r3, #8]
007753cc: cmp      r0, #0
007753d0: movne    r0, r8
007753d4: beq      #0x775484
007753d8: bl       #0x78069c
007753dc: ldr      r3, [r4, #0x10]
007753e0: mov      r0, r3
007753e4: ldr      r3, [r3]
007753e8: mov      lr, pc
007753ec: ldr      pc, [r3, #0x148]
007753f0: ldr      r0, [r4, #0x10]
007753f4: cmp      r7, #0
007753f8: moveq    r1, r5
007753fc: ldr      r3, [r0]
00775400: ldrne    r1, [r4, #0x90]
00775404: ldr      r3, [r3, #0x5c]
00775408: blx      r3
0077540c: ldrb     r2, [r4, #0x84]
00775410: cmp      r2, #0
00775414: bne      #0x775440
00775418: ldr      r0, [r4, #0x10]
0077541c: strb     sb, [r4, #0x84]
00775420: mov      r1, sp
00775424: ldr      r3, [r0]
00775428: ldr      r3, [r3, #0x2c]
0077542c: str      r2, [sp, #4]
00775430: strb     sl, [sp]
00775434: strb     r2, [sp, #1]
00775438: strh     r2, [sp, #2]
0077543c: blx      r3
00775440: ldr      r0, [r4, #0x8c]
00775444: ldr      r1, [r4, #0x90]
00775448: bl       #0x30e3ac
0077544c: cmp      r7, #0
00775450: mov      r1, r0
00775454: str      r0, [r4, #0x8c]
00775458: bne      #0x775390
0077545c: ldr      r0, [r4, #0x94]
00775460: mov      r1, #0
00775464: bl       #0x30e9ac
00775468: cmp      r0, #0
0077546c: bne      #0x7754cc
00775470: ldr      r0, [r4, #0x8c]
00775474: ldr      r1, [r4, #0x90]
00775478: bl       #0x30e7f0
0077547c: str      r0, [r4, #0x8c]
00775480: b        #0x775364
00775484: mov      r0, #0
00775488: b        #0x7753d8
0077548c: ldr      r1, [r4, #0xcc]
00775490: cmp      r1, #0
00775494: beq      #0x7754bc
00775498: ldr      r3, [r4, #0xc8]
0077549c: ldrb     r8, [r3, #4]
007754a0: cmp      r8, #0
007754a4: bne      #0x7754bc
007754a8: mov      r1, r8
007754ac: add      r0, r4, #0xc8
007754b0: bl       #0x41fe84
007754b4: str      r8, [r4, #0xcc]
007754b8: mov      r1, r8
007754bc: add      r1, r1, #0x68
007754c0: mov      r0, r4
007754c4: bl       #0x774660
007754c8: b        #0x775380
007754cc: ldr      r0, [r4, #0xcc]
007754d0: cmp      r0, #0
007754d4: beq      #0x7754e8
007754d8: ldr      r3, [r4, #0xc8]
007754dc: ldrb     r2, [r3, #4]
007754e0: cmp      r2, #0
007754e4: beq      #0x775548
007754e8: bl       #0x76c808
007754ec: mov      r0, r6
007754f0: bl       #0x760940
007754f4: ldr      r3, [r4, #0x10]
007754f8: mov      r0, r3
007754fc: ldr      r3, [r3]
00775500: mov      lr, pc
00775504: ldr      pc, [r3, #0x44]
00775508: ldr      r0, [r4, #0xcc]
0077550c: cmp      r0, #0
00775510: beq      #0x775538
00775514: ldr      r3, [r4, #0xc8]
00775518: ldrb     r5, [r3, #4]
0077551c: cmp      r5, #0
00775520: bne      #0x775538
00775524: add      r0, r4, #0xc8
00775528: mov      r1, r5
0077552c: bl       #0x41fe84
00775530: str      r5, [r4, #0xcc]
00775534: mov      r0, r5
00775538: bl       #0x76d2f8
0077553c: mov      r3, #0x40000000
00775540: str      r3, [r4, #0x94]
00775544: b        #0x775470
00775548: ldr      r1, [r3]
0077554c: sub      r1, r1, #1
00775550: cmp      r1, #0
00775554: str      r1, [r3]
00775558: bne      #0x775564
0077555c: mov      r0, r3
00775560: bl       #0x752b38
00775564: mov      r0, #0
00775568: str      r0, [r4, #0xc8]
0077556c: str      r0, [r4, #0xcc]
00775570: b        #0x7754e8

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

# _ZN8RenderFX9SendEventERNS_5EventE
007abf34: push     {r4, r5, r6, lr}
007abf38: ldr      r3, [r0, #0xfc]
007abf3c: mov      r5, r1
007abf40: sub      sp, sp, #8
007abf44: mov      r6, r0
007abf48: mov      r0, r3
007abf4c: ldr      r3, [r3]
007abf50: mov      lr, pc
007abf54: ldr      pc, [r3]
007abf58: ldrb     r3, [r5, #0x24]
007abf5c: ldr      r4, [pc, #0x280]
007abf60: cmp      r3, #0
007abf64: add      r4, pc, r4
007abf68: bne      #0x7ac07c
007abf6c: ldr      r3, [r5, #8]
007abf70: cmp      r3, #0xb
007abf74: addls    pc, pc, r3, lsl #2
007abf78: b        #0x7ac07c
007abf7c: b        #0x7ac0a8
007abf80: b        #0x7ac0cc
007abf84: b        #0x7ac0f0
007abf88: b        #0x7ac07c
007abf8c: b        #0x7ac114
007abf90: b        #0x7ac07c
007abf94: b        #0x7abfac
007abf98: b        #0x7ac138
007abf9c: b        #0x7ac1a4
007abfa0: b        #0x7ac15c
007abfa4: b        #0x7ac180
007abfa8: b        #0x7ac084
007abfac: ldr      r2, [pc, #0x234]
007abfb0: mov      ip, #0
007abfb4: ldr      r1, [r5]
007abfb8: add      r2, pc, r2
007abfbc: mov      r3, ip
007abfc0: mov      r0, r6
007abfc4: str      ip, [sp]
007abfc8: bl       #0x7abe0c
007abfcc: ldr      r6, [r5, #4]
007abfd0: ldr      r1, [pc, #0x214]
007abfd4: mov      r0, r6
007abfd8: add      r1, pc, r1
007abfdc: bl       #0x30e31c
007abfe0: cmp      r0, #0
007abfe4: bne      #0x7abffc
007abfe8: ldr      r3, [pc, #0x200]
007abfec: mov      r2, #0x13
007abff0: ldr      r3, [r4, r3]
007abff4: str      r2, [r3]
007abff8: ldr      r6, [r5, #4]
007abffc: ldr      r1, [pc, #0x1f0]
007ac000: mov      r0, r6
007ac004: add      r1, pc, r1
007ac008: bl       #0x30e31c
007ac00c: cmp      r0, #0
007ac010: bne      #0x7ac1c8
007ac014: ldr      r3, [pc, #0x1d4]
007ac018: mov      r2, #1
007ac01c: ldr      r3, [r4, r3]
007ac020: str      r2, [r3]
007ac024: ldr      r6, [r5, #4]
007ac028: ldr      r1, [pc, #0x1c8]
007ac02c: mov      r0, r6
007ac030: add      r1, pc, r1
007ac034: bl       #0x30e31c
007ac038: cmp      r0, #0
007ac03c: bne      #0x7ac054
007ac040: ldr      r3, [pc, #0x1a8]
007ac044: mov      r2, #0x14
007ac048: ldr      r3, [r4, r3]
007ac04c: str      r2, [r3]
007ac050: ldr      r6, [r5, #4]
007ac054: ldr      r1, [pc, #0x1a0]
007ac058: mov      r0, r6
007ac05c: add      r1, pc, r1
007ac060: bl       #0x30e31c
007ac064: cmp      r0, #0
007ac068: bne      #0x7ac07c
007ac06c: ldr      r3, [pc, #0x17c]
007ac070: mov      r2, #0xc
007ac074: ldr      r3, [r4, r3]
007ac078: str      r2, [r3]
007ac07c: add      sp, sp, #8
007ac080: pop      {r4, r5, r6, pc}
007ac084: ldr      r2, [pc, #0x174]
007ac088: mov      ip, #0
007ac08c: ldr      r1, [r5]
007ac090: mov      r0, r6
007ac094: add      r2, pc, r2
007ac098: mov      r3, ip
007ac09c: str      ip, [sp]
007ac0a0: bl       #0x7abe0c
007ac0a4: b        #0x7ac07c
007ac0a8: ldr      r2, [pc, #0x154]
007ac0ac: mov      ip, #0
007ac0b0: ldr      r1, [r5]
007ac0b4: mov      r0, r6
007ac0b8: add      r2, pc, r2
007ac0bc: mov      r3, ip
007ac0c0: str      ip, [sp]
007ac0c4: bl       #0x7abe0c
007ac0c8: b        #0x7ac07c
007ac0cc: ldr      r2, [pc, #0x134]
007ac0d0: mov      ip, #0
007ac0d4: ldr      r1, [r5]
007ac0d8: mov      r0, r6
007ac0dc: add      r2, pc, r2
007ac0e0: mov      r3, ip
007ac0e4: str      ip, [sp]
007ac0e8: bl       #0x7abe0c
007ac0ec: b        #0x7ac07c
007ac0f0: ldr      r2, [pc, #0x114]
007ac0f4: mov      ip, #0
007ac0f8: ldr      r1, [r5]
007ac0fc: mov      r0, r6
007ac100: add      r2, pc, r2
007ac104: mov      r3, ip
007ac108: str      ip, [sp]
007ac10c: bl       #0x7abe0c
007ac110: b        #0x7ac07c
007ac114: ldr      r2, [pc, #0xf4]
007ac118: mov      ip, #0
007ac11c: ldr      r1, [r5]
007ac120: mov      r0, r6
007ac124: add      r2, pc, r2
007ac128: mov      r3, ip
007ac12c: str      ip, [sp]
007ac130: bl       #0x7abe0c
007ac134: b        #0x7ac07c
007ac138: ldr      r2, [pc, #0xd4]
007ac13c: mov      ip, #0
007ac140: ldr      r1, [r5]
007ac144: mov      r0, r6
007ac148: add      r2, pc, r2
007ac14c: mov      r3, ip
007ac150: str      ip, [sp]
007ac154: bl       #0x7abe0c
007ac158: b        #0x7ac07c
007ac15c: ldr      r2, [pc, #0xb4]
007ac160: mov      ip, #0
007ac164: ldr      r1, [r5]
007ac168: mov      r0, r6
007ac16c: add      r2, pc, r2
007ac170: mov      r3, ip
007ac174: str      ip, [sp]
007ac178: bl       #0x7abe0c
007ac17c: b        #0x7ac07c
007ac180: ldr      r2, [pc, #0x94]
007ac184: mov      ip, #0
007ac188: ldr      r1, [r5]
007ac18c: mov      r0, r6
007ac190: add      r2, pc, r2
007ac194: mov      r3, ip
007ac198: str      ip, [sp]
007ac19c: bl       #0x7abe0c
007ac1a0: b        #0x7ac07c
007ac1a4: ldr      r2, [pc, #0x74]
007ac1a8: mov      ip, #0
007ac1ac: ldr      r1, [r5]
007ac1b0: mov      r0, r6
007ac1b4: add      r2, pc, r2
007ac1b8: mov      r3, ip
007ac1bc: str      ip, [sp]
007ac1c0: bl       #0x7abe0c
007ac1c4: b        #0x7ac07c
007ac1c8: ldr      r1, [pc, #0x54]
007ac1cc: mov      r0, r6
007ac1d0: add      r1, pc, r1
007ac1d4: bl       #0x30e31c
007ac1d8: cmp      r0, #0
007ac1dc: bne      #0x7ac028
007ac1e0: b        #0x7ac014
007ac1e4: andseq   r8, lr, ip, lsr #22
007ac1e8: ldrsbeq  sp, [r5], -r8
007ac1ec: andseq   lr, r5, r0, asr #18
007ac1f0: andeq    r3, r0, r0, asr r8
007ac1f4: andseq   lr, r5, r4, lsr #18
007ac1f8: andseq   lr, r5, r8, lsr sb
007ac1fc: andseq   lr, r5, ip, lsl sb
007ac200: andseq   sp, r5, ip, lsl ip
007ac204: andseq   lr, r5, r0, lsr r8
007ac208: andseq   lr, r5, ip, lsl r8
007ac20c: andseq   lr, r5, r8, lsl #16
007ac210: mulseq   r5, ip, fp
007ac214: andseq   sp, r5, r0, lsl #23
007ac218: andseq   lr, r5, ip, lsr #16
007ac21c: andseq   sp, r5, r0, lsl fp
007ac220: ldrsbeq  lr, [r5], -r4
007ac224: andseq   lr, r5, r8, ror r7

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
