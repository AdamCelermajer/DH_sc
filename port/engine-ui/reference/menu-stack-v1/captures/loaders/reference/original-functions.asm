
# _ZN16MultiMenuManager11LoadSWFFileEPKci
00437d68: ldr      r3, [pc, #0xac]
00437d6c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00437d70: cmp      r2, #2
00437d74: mov      r4, r2
00437d78: ldr      r2, [pc, #0xa0]
00437d7c: add      r3, pc, r3
00437d80: add      r5, r4, #0x4c
00437d84: ldr      r3, [r3, r2]
00437d88: moveq    r2, #1
00437d8c: movne    r2, #0
00437d90: strb     r2, [r3]
00437d94: add      r5, r0, r5, lsl #2
00437d98: ldr      sl, [r5, #4]
00437d9c: mov      r6, r0
00437da0: mov      r7, r1
00437da4: cmp      sl, #0
00437da8: beq      #0x437db4
00437dac: mov      r0, sl
00437db0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437db4: mov      r1, #8
00437db8: mov      r0, #0x124
00437dbc: bl       #0x310570
00437dc0: mov      r8, r0
00437dc4: bl       #0x7a86e4
00437dc8: str      r8, [r5, #4]
00437dcc: mov      r2, sl
00437dd0: ldr      r3, [r8]
00437dd4: mov      r0, r8
00437dd8: mov      r1, r7
00437ddc: mov      lr, pc
00437de0: ldr      pc, [r3, #8]
00437de4: ldr      r0, [r5, #4]
00437de8: mov      r1, #1
00437dec: bl       #0x7a7cb4
00437df0: mov      r1, #8
00437df4: mov      r0, #0x34
00437df8: bl       #0x310570
00437dfc: add      r4, r6, r4, lsl #2
00437e00: mov      r7, r0
00437e04: ldr      r1, [r5, #4]
00437e08: bl       #0x42ccd0
00437e0c: str      r7, [r4, #0x144]
00437e10: ldr      sl, [r5, #4]
00437e14: mov      r0, sl
00437e18: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437e1c: subseq   ip, r5, r4, lsl sp
00437e20: strheq   r0, [r0], -ip

# _ZN11MenuManager11LoadSWFFileEPKci
0042d290: push     {r4, r5, r6, r7, r8, lr}
0042d294: mov      r6, r0
0042d298: mov      r8, r1
0042d29c: mov      r0, #0x124
0042d2a0: mov      r1, #8
0042d2a4: mov      r7, r2
0042d2a8: bl       #0x310570
0042d2ac: add      r4, r7, #0x22
0042d2b0: add      r4, r6, r4, lsl #2
0042d2b4: mov      r5, r0
0042d2b8: bl       #0x7a86e4
0042d2bc: str      r5, [r4, #4]
0042d2c0: ldr      r3, [r5]
0042d2c4: mov      r2, #0
0042d2c8: mov      r0, r5
0042d2cc: mov      r1, r8
0042d2d0: mov      lr, pc
0042d2d4: ldr      pc, [r3, #8]
0042d2d8: ldr      r0, [r4, #4]
0042d2dc: mov      r1, #1
0042d2e0: bl       #0x7a7cb4
0042d2e4: mov      r1, #8
0042d2e8: mov      r0, #0x34
0042d2ec: bl       #0x310570
0042d2f0: add      r6, r6, r7, lsl #2
0042d2f4: mov      r5, r0
0042d2f8: ldr      r1, [r4, #4]
0042d2fc: bl       #0x42ccd0
0042d300: str      r5, [r6, #0x9c]
0042d304: ldr      r0, [r4, #4]
0042d308: pop      {r4, r5, r6, r7, r8, pc}

# _ZN11MenuManager8LoadMenuEi
00431ea4: ldr      r3, [pc, #0x608]
00431ea8: ldr      r2, [pc, #0x608]
00431eac: push     {r4, r5, r6, lr}
00431eb0: add      r3, pc, r3
00431eb4: ldr      r2, [r3, r2]
00431eb8: mov      r5, r1
00431ebc: movw     r1, #0x356
00431ec0: ldr      r2, [r2]
00431ec4: mov      r4, r0
00431ec8: cmp      r2, r1
00431ecc: beq      #0x432404
00431ed0: cmp      r2, #0x3c0
00431ed4: beq      #0x4323e8
00431ed8: cmp      r2, #0x320
00431edc: beq      #0x432170
00431ee0: ldr      r3, [pc, #0x5d4]
00431ee4: ldr      r0, [r0, #0xf4]
00431ee8: mov      r2, r5
00431eec: add      r3, pc, r3
00431ef0: ldr      r1, [r3, r5, lsl #2]
00431ef4: bl       #0x437d68
00431ef8: cmp      r5, #3
00431efc: ldr      r3, [r4, #0xf4]
00431f00: bhi      #0x4321bc
00431f04: add      r6, r5, #0x4c
00431f08: add      r3, r3, r6, lsl #2
00431f0c: ldr      r0, [r3, #4]
00431f10: mov      r1, #0x84
00431f14: bl       #0x7a7c98
00431f18: cmp      r5, #3
00431f1c: bne      #0x431f78
00431f20: ldr      r3, [r4, #0xf4]
00431f24: mov      r1, #4
00431f28: ldr      r0, [r3, #0x140]
00431f2c: bl       #0x7a7c98
00431f30: bl       #0x41edd0
00431f34: ldr      r3, [r4, #0xf4]
00431f38: ldr      r3, [r3, #0x140]
00431f3c: str      r3, [r0, #0x57c]
00431f40: bl       #0x41d880
00431f44: bl       #0x41b11c
00431f48: ldr      r3, [r4, #0xf4]
00431f4c: ldr      r3, [r3, #0x140]
00431f50: str      r3, [r0, #0x658]
00431f54: bl       #0x419b4c
00431f58: bl       #0x413e90
00431f5c: ldr      r3, [r4, #0xf4]
00431f60: ldr      r1, [r3, #0x140]
00431f64: bl       #0x4151d8
00431f68: mov      r0, r4
00431f6c: bl       #0x42cb8c
00431f70: add      r1, r4, #4
00431f74: bl       #0x7a7c6c
00431f78: ldr      r3, [r4, #0xf4]
00431f7c: add      r6, r3, r6, lsl #2
00431f80: ldr      r3, [r6, #4]
00431f84: mov      r0, r3
00431f88: mov      r1, #1
00431f8c: ldr      r3, [r3]
00431f90: mov      r2, #0
00431f94: mov      lr, pc
00431f98: ldr      pc, [r3, #0x10]
00431f9c: bl       #0x42a660
00431fa0: ldr      r3, [r0, #0x4c]
00431fa4: mov      r5, r0
00431fa8: cmp      r3, #0
00431fac: beq      #0x4323bc
00431fb0: ldr      r0, [r0, #0x48]
00431fb4: ldrb     r3, [r0, #4]
00431fb8: cmp      r3, #0
00431fbc: beq      #0x43239c
00431fc0: bl       #0x42a4fc
00431fc4: ldr      r3, [r0, #0x4c]
00431fc8: mov      r5, r0
00431fcc: cmp      r3, #0
00431fd0: beq      #0x432390
00431fd4: ldr      r0, [r0, #0x48]
00431fd8: ldrb     r3, [r0, #4]
00431fdc: cmp      r3, #0
00431fe0: beq      #0x432370
00431fe4: bl       #0x42c35c
00431fe8: ldr      r3, [r0, #0x4c]
00431fec: mov      r5, r0
00431ff0: cmp      r3, #0
00431ff4: beq      #0x432354
00431ff8: ldr      r0, [r0, #0x48]
00431ffc: ldrb     r3, [r0, #4]
00432000: cmp      r3, #0
00432004: beq      #0x432334
00432008: bl       #0x433720
0043200c: ldr      r3, [r0, #0x4c]
00432010: mov      r5, r0
00432014: cmp      r3, #0
00432018: beq      #0x432318
0043201c: ldr      r0, [r0, #0x48]
00432020: ldrb     r3, [r0, #4]
00432024: cmp      r3, #0
00432028: beq      #0x4322f8
0043202c: bl       #0x429b10
00432030: ldr      r3, [r0, #0x4c]
00432034: mov      r5, r0
00432038: cmp      r3, #0
0043203c: beq      #0x432294
00432040: ldr      r0, [r0, #0x48]
00432044: ldrb     r3, [r0, #4]
00432048: cmp      r3, #0
0043204c: beq      #0x432274
00432050: bl       #0x42b038
00432054: ldr      r3, [r0, #0x4c]
00432058: mov      r5, r0
0043205c: cmp      r3, #0
00432060: beq      #0x432258
00432064: ldr      r0, [r0, #0x48]
00432068: ldrb     r3, [r0, #4]
0043206c: cmp      r3, #0
00432070: beq      #0x432238
00432074: bl       #0x435880
00432078: ldr      r3, [r0, #0x4c]
0043207c: mov      r5, r0
00432080: cmp      r3, #0
00432084: beq      #0x43221c
00432088: ldr      r0, [r0, #0x48]
0043208c: ldrb     r3, [r0, #4]
00432090: cmp      r3, #0
00432094: beq      #0x4321fc
00432098: bl       #0x452eb0
0043209c: ldr      r3, [r0, #0x4c]
004320a0: mov      r5, r0
004320a4: cmp      r3, #0
004320a8: beq      #0x4321f0
004320ac: ldr      r0, [r0, #0x48]
004320b0: ldrb     r3, [r0, #4]
004320b4: cmp      r3, #0
004320b8: beq      #0x4321d0
004320bc: bl       #0x453da4
004320c0: ldr      r3, [r0, #0x4c]
004320c4: mov      r5, r0
004320c8: cmp      r3, #0
004320cc: beq      #0x4322ec
004320d0: ldr      r0, [r0, #0x48]
004320d4: ldrb     r3, [r0, #4]
004320d8: cmp      r3, #0
004320dc: beq      #0x4322cc
004320e0: bl       #0x4364f8
004320e4: ldr      r3, [r0, #0x4c]
004320e8: mov      r5, r0
004320ec: cmp      r3, #0
004320f0: beq      #0x4322c0
004320f4: ldr      r0, [r0, #0x48]
004320f8: ldrb     r3, [r0, #4]
004320fc: cmp      r3, #0
00432100: beq      #0x4322a0
00432104: ldr      r3, [r4, #0x64]
00432108: ldr      r5, [r4, #0x68]
0043210c: mov      r0, r4
00432110: rsb      r5, r3, r5
00432114: bl       #0x42efb8
00432118: ldr      r3, [r4, #0x64]
0043211c: ldr      r2, [r4, #0x68]
00432120: asr      r5, r5, #2
00432124: rsb      r2, r3, r2
00432128: cmp      r5, r2, asr #2
0043212c: bhs      #0x4321b8
00432130: ldr      r0, [r3, r5, lsl #2]
00432134: mov      r1, #0
00432138: bl       #0x423db8
0043213c: ldr      r3, [r4, #0x64]
00432140: ldr      r3, [r3, r5, lsl #2]
00432144: add      r5, r5, #1
00432148: mov      r0, r3
0043214c: ldr      r3, [r3]
00432150: mov      lr, pc
00432154: ldr      pc, [r3, #0x10]
00432158: ldr      r3, [r4, #0x64]
0043215c: ldr      r2, [r4, #0x68]
00432160: rsb      r2, r3, r2
00432164: cmp      r5, r2, asr #2
00432168: blo      #0x432130
0043216c: pop      {r4, r5, r6, pc}
00432170: ldr      r2, [pc, #0x348]
00432174: ldr      r2, [r3, r2]
00432178: ldrb     r2, [r2]
0043217c: cmp      r2, #0
00432180: bne      #0x432474
00432184: ldr      r2, [pc, #0x338]
00432188: ldr      r3, [r3, r2]
0043218c: ldrb     r3, [r3]
00432190: cmp      r3, #0
00432194: beq      #0x432494
00432198: ldr      r3, [pc, #0x328]
0043219c: mov      r2, r5
004321a0: ldr      r0, [r0, #0xf4]
004321a4: add      r3, pc, r3
004321a8: add      r3, r3, r5, lsl #2
004321ac: ldr      r1, [r3, #0x20]
004321b0: bl       #0x437d68
004321b4: b        #0x431ef8
004321b8: pop      {r4, r5, r6, pc}
004321bc: mov      r0, #0
004321c0: mov      r1, #0x84
004321c4: bl       #0x7a7c98
004321c8: mov      r3, #0
004321cc: b        #0x431f84
004321d0: ldr      r1, [r0]
004321d4: sub      r1, r1, #1
004321d8: cmp      r1, #0
004321dc: str      r1, [r0]
004321e0: beq      #0x43243c
004321e4: mov      r3, #0
004321e8: str      r3, [r5, #0x4c]
004321ec: str      r3, [r5, #0x48]
004321f0: bl       #0x452eb0
004321f4: bl       #0x452e00
004321f8: b        #0x4320bc
004321fc: ldr      r1, [r0]
00432200: sub      r1, r1, #1
00432204: cmp      r1, #0
00432208: str      r1, [r0]
0043220c: beq      #0x43246c
00432210: mov      r3, #0
00432214: str      r3, [r5, #0x4c]
00432218: str      r3, [r5, #0x48]
0043221c: bl       #0x42ca8c
00432220: mov      r5, r0
00432224: bl       #0x435880
00432228: mov      r1, r0
0043222c: mov      r0, r5
00432230: bl       #0x42ee94
00432234: b        #0x432098
00432238: ldr      r1, [r0]
0043223c: sub      r1, r1, #1
00432240: cmp      r1, #0
00432244: str      r1, [r0]
00432248: beq      #0x432464
0043224c: mov      r3, #0
00432250: str      r3, [r5, #0x4c]
00432254: str      r3, [r5, #0x48]
00432258: bl       #0x42ca8c
0043225c: mov      r5, r0
00432260: bl       #0x42b038
00432264: mov      r1, r0
00432268: mov      r0, r5
0043226c: bl       #0x42ee94
00432270: b        #0x432074
00432274: ldr      r1, [r0]
00432278: sub      r1, r1, #1
0043227c: cmp      r1, #0
00432280: str      r1, [r0]
00432284: beq      #0x43245c
00432288: mov      r3, #0
0043228c: str      r3, [r5, #0x4c]
00432290: str      r3, [r5, #0x48]
00432294: bl       #0x429b10
00432298: bl       #0x428cc0
0043229c: b        #0x432050
004322a0: ldr      r1, [r0]
004322a4: sub      r1, r1, #1
004322a8: cmp      r1, #0
004322ac: str      r1, [r0]
004322b0: beq      #0x432454
004322b4: mov      r3, #0
004322b8: str      r3, [r5, #0x4c]
004322bc: str      r3, [r5, #0x48]
004322c0: bl       #0x4364f8
004322c4: bl       #0x4360ec
004322c8: b        #0x432104
004322cc: ldr      r1, [r0]
004322d0: sub      r1, r1, #1
004322d4: cmp      r1, #0
004322d8: str      r1, [r0]
004322dc: beq      #0x432444
004322e0: mov      r3, #0
004322e4: str      r3, [r5, #0x4c]
004322e8: str      r3, [r5, #0x48]
004322ec: bl       #0x453da4
004322f0: bl       #0x453c3c
004322f4: b        #0x4320e0
004322f8: ldr      r1, [r0]
004322fc: sub      r1, r1, #1
00432300: cmp      r1, #0
00432304: str      r1, [r0]
00432308: beq      #0x43244c
0043230c: mov      r3, #0
00432310: str      r3, [r5, #0x4c]
00432314: str      r3, [r5, #0x48]
00432318: bl       #0x42ca8c
0043231c: mov      r5, r0
00432320: bl       #0x433720
00432324: mov      r1, r0
00432328: mov      r0, r5
0043232c: bl       #0x42ee94
00432330: b        #0x43202c
00432334: ldr      r1, [r0]
00432338: sub      r1, r1, #1
0043233c: cmp      r1, #0
00432340: str      r1, [r0]
00432344: beq      #0x432434
00432348: mov      r3, #0
0043234c: str      r3, [r5, #0x4c]
00432350: str      r3, [r5, #0x48]
00432354: bl       #0x42ca8c
00432358: mov      r5, r0
0043235c: bl       #0x42c35c
00432360: mov      r1, r0
00432364: mov      r0, r5
00432368: bl       #0x42ee94
0043236c: b        #0x432008
00432370: ldr      r1, [r0]
00432374: sub      r1, r1, #1
00432378: cmp      r1, #0
0043237c: str      r1, [r0]
00432380: beq      #0x43242c
00432384: mov      r3, #0
00432388: str      r3, [r5, #0x4c]
0043238c: str      r3, [r5, #0x48]
00432390: bl       #0x42a4fc
00432394: bl       #0x429f4c
00432398: b        #0x431fe4
0043239c: ldr      r1, [r0]
004323a0: sub      r1, r1, #1
004323a4: cmp      r1, #0
004323a8: str      r1, [r0]
004323ac: beq      #0x432424
004323b0: mov      r3, #0
004323b4: str      r3, [r5, #0x4c]
004323b8: str      r3, [r5, #0x48]
004323bc: bl       #0x42a660
004323c0: bl       #0x42a040
004323c4: bl       #0x42a660
004323c8: bl       #0x42db28
004323cc: cmp      r0, #0
004323d0: beq      #0x431fc0
004323d4: bl       #0x42a660
004323d8: bl       #0x42db28
004323dc: mov      r3, #0
004323e0: strb     r3, [r0, #0x9b]
004323e4: b        #0x431fc0
004323e8: ldr      r3, [pc, #0xdc]
004323ec: mov      r2, r5
004323f0: ldr      r0, [r0, #0xf4]
004323f4: add      r3, pc, r3
004323f8: ldr      r1, [r3, r5, lsl #2]
004323fc: bl       #0x437d68
00432400: b        #0x431ef8
00432404: ldr      r3, [pc, #0xc4]
00432408: mov      r2, r5
0043240c: ldr      r0, [r0, #0xf4]
00432410: add      r3, pc, r3
00432414: add      r3, r3, r5, lsl #2
00432418: ldr      r1, [r3, #0x40]
0043241c: bl       #0x437d68
00432420: b        #0x431ef8
00432424: bl       #0x752b38
00432428: b        #0x4323b0
0043242c: bl       #0x752b38
00432430: b        #0x432384
00432434: bl       #0x752b38
00432438: b        #0x432348
0043243c: bl       #0x752b38
00432440: b        #0x4321e4
00432444: bl       #0x752b38
00432448: b        #0x4322e0
0043244c: bl       #0x752b38
00432450: b        #0x43230c
00432454: bl       #0x752b38
00432458: b        #0x4322b4
0043245c: bl       #0x752b38
00432460: b        #0x432288
00432464: bl       #0x752b38
00432468: b        #0x43224c
0043246c: bl       #0x752b38
00432470: b        #0x432210
00432474: ldr      r3, [pc, #0x58]
00432478: mov      r2, r5
0043247c: ldr      r0, [r0, #0xf4]
00432480: add      r3, pc, r3
00432484: add      r3, r3, r5, lsl #2
00432488: ldr      r1, [r3, #0x10]
0043248c: bl       #0x437d68
00432490: b        #0x431ef8
00432494: ldr      r3, [pc, #0x3c]
00432498: mov      r2, r5
0043249c: ldr      r0, [r0, #0xf4]
004324a0: add      r3, pc, r3
004324a4: add      r3, r3, r5, lsl #2
004324a8: ldr      r1, [r3, #0x30]
004324ac: bl       #0x437d68
004324b0: b        #0x431ef8
004324b4: subseq   r2, r6, r0, ror #23
004324b8: andeq    r2, r0, r4, asr #11
004324bc: subseq   r4, r2, r0, ror fp
004324c0: andeq    r2, r0, r8, ror #14
004324c4: andeq    r1, r0, r0, ror #13
004324c8: ldrheq   r4, [r2], #-0x88
004324cc: subseq   r4, r2, r8, ror #12
004324d0: subseq   r4, r2, ip, asr #12
004324d4: ldrsbeq  r4, [r2], #-0x5c
004324d8: ldrheq   r4, [r2], #-0x5c
