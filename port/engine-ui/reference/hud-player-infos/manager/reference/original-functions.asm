
# _ZN7gameswf9tu_stringC1EPKc.clone.1
0041ddec: mov      r3, #1
0041ddf0: push     {r4, lr}
0041ddf4: strb     r3, [r0]
0041ddf8: mov      r3, #0
0041ddfc: mov      r4, r0
0041de00: strb     r3, [r0, #1]
0041de04: mov      r1, #6
0041de08: bl       #0x751d14
0041de0c: ldrsb    r3, [r4]
0041de10: ldr      r1, [pc, #0x38]
0041de14: mov      r2, #7
0041de18: cmn      r3, #1
0041de1c: addne    r0, r4, #1
0041de20: ldreq    r0, [r4, #0xc]
0041de24: add      r1, pc, r1
0041de28: bl       #0x30e868
0041de2c: ldr      r3, [r4, #0x10]
0041de30: mvn      r2, #0
0041de34: mov      r0, r4
0041de38: bfi      r3, r2, #0, #0x18
0041de3c: lsr      r2, r3, #0x18
0041de40: bfc      r2, #0, #1
0041de44: str      r3, [r4, #0x10]
0041de48: strb     r2, [r4, #0x13]
0041de4c: pop      {r4, pc}
0041de50: subeq    fp, sl, r4, lsr r0

# _ZN14InfoHUDManager18applyOneTimeValuesEv
0041d728: push     {r4, r5, r6, r7, r8, lr}
0041d72c: mov      r4, r0
0041d730: add      r0, r0, #0x36c
0041d734: bl       #0x427d50
0041d738: ldr      r7, [pc, #0x134]
0041d73c: ldr      r3, [pc, #0x134]
0041d740: ldr      r1, [pc, #0x134]
0041d744: add      r7, pc, r7
0041d748: ldr      r5, [r7, r3]
0041d74c: mov      r6, r0
0041d750: add      r1, pc, r1
0041d754: mov      r0, r5
0041d758: bl       #0x320e44
0041d75c: subs     r0, r0, #0
0041d760: movne    r0, #1
0041d764: strb     r0, [r6, #0x9b]
0041d768: mov      r1, #0
0041d76c: ldr      r0, [r5, #0x40]
0041d770: mov      r2, r1
0041d774: bl       #0x36e478
0041d778: ldr      r0, [r0, #0x660]
0041d77c: cmp      r0, #0
0041d780: beq      #0x41d870
0041d784: bl       #0x3bb7fc
0041d788: sub      r0, r0, #0x120
0041d78c: sub      r0, r0, #2
0041d790: cmp      r0, #0x25
0041d794: addls    pc, pc, r0, lsl #2
0041d798: b        #0x41d868
0041d79c: b        #0x41d860
0041d7a0: b        #0x41d860
0041d7a4: b        #0x41d860
0041d7a8: b        #0x41d868
0041d7ac: b        #0x41d868
0041d7b0: b        #0x41d868
0041d7b4: b        #0x41d868
0041d7b8: b        #0x41d868
0041d7bc: b        #0x41d868
0041d7c0: b        #0x41d868
0041d7c4: b        #0x41d868
0041d7c8: b        #0x41d868
0041d7cc: b        #0x41d868
0041d7d0: b        #0x41d868
0041d7d4: b        #0x41d868
0041d7d8: b        #0x41d868
0041d7dc: b        #0x41d868
0041d7e0: b        #0x41d868
0041d7e4: b        #0x41d868
0041d7e8: b        #0x41d868
0041d7ec: b        #0x41d868
0041d7f0: b        #0x41d868
0041d7f4: b        #0x41d868
0041d7f8: b        #0x41d868
0041d7fc: b        #0x41d868
0041d800: b        #0x41d868
0041d804: b        #0x41d868
0041d808: b        #0x41d868
0041d80c: b        #0x41d868
0041d810: b        #0x41d868
0041d814: b        #0x41d868
0041d818: b        #0x41d868
0041d81c: b        #0x41d868
0041d820: b        #0x41d868
0041d824: b        #0x41d868
0041d828: b        #0x41d834
0041d82c: b        #0x41d834
0041d830: b        #0x41d834
0041d834: mov      r5, #1
0041d838: add      r0, r4, #0x450
0041d83c: add      r0, r0, #0xc
0041d840: ldr      r4, [r4, #0x57c]
0041d844: bl       #0x427d50
0041d848: mov      r2, r5
0041d84c: mov      r1, r0
0041d850: mov      r3, #0
0041d854: mov      r0, r4
0041d858: pop      {r4, r5, r6, r7, r8, lr}
0041d85c: b        #0x7a7d34
0041d860: mov      r5, #2
0041d864: b        #0x41d838
0041d868: mov      r5, #0
0041d86c: b        #0x41d838
0041d870: pop      {r4, r5, r6, r7, r8, pc}
0041d874: subseq   r7, r7, ip, asr #6
0041d878: strdeq   r3, r4, [r0], -r4
0041d87c: strdeq   r1, r2, [sl], #-0x40

# _ZN14InfoHUDManager15initCachedCharsEv
0041d880: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041d884: ldr      r6, [pc, #0x3f4]
0041d888: ldr      sb, [pc, #0x3f4]
0041d88c: ldr      r2, [r0, #0x57c]
0041d890: add      r6, pc, r6
0041d894: ldr      r3, [r6, sb]
0041d898: sub      sp, sp, #0x64
0041d89c: cmp      r2, #0
0041d8a0: ldr      r3, [r3]
0041d8a4: mov      r4, r0
0041d8a8: str      r3, [sp, #0x5c]
0041d8ac: beq      #0x41dbd4
0041d8b0: ldr      r3, [pc, #0x3d0]
0041d8b4: ldr      r1, [pc, #0x3d0]
0041d8b8: add      r5, sp, #0x48
0041d8bc: ldr      r0, [r6, r3]
0041d8c0: add      r1, pc, r1
0041d8c4: bl       #0x320e44
0041d8c8: ldr      r1, [pc, #0x3c0]
0041d8cc: mov      r2, r0
0041d8d0: mov      r7, r0
0041d8d4: add      r1, pc, r1
0041d8d8: mov      r0, r5
0041d8dc: bl       #0x30eae4
0041d8e0: mov      r1, r5
0041d8e4: ldr      r0, [r4, #0x57c]
0041d8e8: bl       #0x7a9160
0041d8ec: ldr      r1, [pc, #0x3a0]
0041d8f0: mov      r5, r0
0041d8f4: ldr      r2, [r4, #0x57c]
0041d8f8: add      r0, r4, #0xc
0041d8fc: add      r1, pc, r1
0041d900: mov      r3, r5
0041d904: bl       #0x427ca0
0041d908: ldr      r1, [pc, #0x388]
0041d90c: add      r0, r4, #0x3c
0041d910: ldr      r2, [r4, #0x57c]
0041d914: add      r1, pc, r1
0041d918: mov      r3, r5
0041d91c: bl       #0x427ca0
0041d920: ldr      r1, [pc, #0x374]
0041d924: add      r0, r4, #0x6c
0041d928: ldr      r2, [r4, #0x57c]
0041d92c: add      r1, pc, r1
0041d930: mov      r3, #0
0041d934: bl       #0x427ca0
0041d938: ldr      r1, [pc, #0x360]
0041d93c: add      r0, r4, #0x9c
0041d940: ldr      r2, [r4, #0x57c]
0041d944: add      r1, pc, r1
0041d948: mov      r3, r5
0041d94c: bl       #0x427ca0
0041d950: ldr      r1, [pc, #0x34c]
0041d954: add      r0, r4, #0xcc
0041d958: ldr      r2, [r4, #0x57c]
0041d95c: add      r1, pc, r1
0041d960: mov      r3, r5
0041d964: bl       #0x427ca0
0041d968: ldr      r1, [pc, #0x338]
0041d96c: add      r0, r4, #0xfc
0041d970: ldr      r2, [r4, #0x57c]
0041d974: add      r1, pc, r1
0041d978: mov      r3, r5
0041d97c: bl       #0x427ca0
0041d980: ldr      r1, [pc, #0x324]
0041d984: add      r0, r4, #0x12c
0041d988: ldr      r2, [r4, #0x57c]
0041d98c: add      r1, pc, r1
0041d990: mov      r3, r5
0041d994: bl       #0x427ca0
0041d998: ldr      r1, [pc, #0x310]
0041d99c: add      r0, r4, #0x15c
0041d9a0: ldr      r2, [r4, #0x57c]
0041d9a4: add      r1, pc, r1
0041d9a8: mov      r3, r5
0041d9ac: bl       #0x427ca0
0041d9b0: cmp      r7, #1
0041d9b4: ble      #0x41dbf0
0041d9b8: ldr      r1, [pc, #0x2f4]
0041d9bc: add      r0, r4, #0x18c
0041d9c0: ldr      r2, [r4, #0x57c]
0041d9c4: add      r1, pc, r1
0041d9c8: mov      r3, r5
0041d9cc: bl       #0x427ca0
0041d9d0: ldr      r1, [pc, #0x2e0]
0041d9d4: add      r0, r4, #0x21c
0041d9d8: ldr      r2, [r4, #0x57c]
0041d9dc: add      r1, pc, r1
0041d9e0: mov      r3, r5
0041d9e4: bl       #0x427ca0
0041d9e8: ldr      r1, [pc, #0x2cc]
0041d9ec: add      r0, r4, #0x2ac
0041d9f0: ldr      r2, [r4, #0x57c]
0041d9f4: add      r1, pc, r1
0041d9f8: mov      r3, r5
0041d9fc: bl       #0x427ca0
0041da00: ldr      r1, [pc, #0x2b8]
0041da04: add      r0, r4, #0x1bc
0041da08: ldr      r2, [r4, #0x57c]
0041da0c: add      r1, pc, r1
0041da10: mov      r3, r5
0041da14: bl       #0x427ca0
0041da18: ldr      r1, [pc, #0x2a4]
0041da1c: add      r0, r4, #0x24c
0041da20: ldr      r2, [r4, #0x57c]
0041da24: add      r1, pc, r1
0041da28: mov      r3, r5
0041da2c: bl       #0x427ca0
0041da30: ldr      r1, [pc, #0x290]
0041da34: add      r0, r4, #0x2dc
0041da38: ldr      r2, [r4, #0x57c]
0041da3c: add      r1, pc, r1
0041da40: mov      r3, r5
0041da44: bl       #0x427ca0
0041da48: ldr      r1, [pc, #0x27c]
0041da4c: add      r0, r4, #0x1ec
0041da50: ldr      r2, [r4, #0x57c]
0041da54: add      r1, pc, r1
0041da58: mov      r3, r5
0041da5c: bl       #0x427ca0
0041da60: ldr      r1, [pc, #0x268]
0041da64: add      r0, r4, #0x27c
0041da68: ldr      r2, [r4, #0x57c]
0041da6c: add      r1, pc, r1
0041da70: mov      r3, r5
0041da74: bl       #0x427ca0
0041da78: ldr      r1, [pc, #0x254]
0041da7c: add      r0, r4, #0x30c
0041da80: ldr      r2, [r4, #0x57c]
0041da84: add      r1, pc, r1
0041da88: mov      r3, r5
0041da8c: bl       #0x427ca0
0041da90: ldr      r1, [pc, #0x240]
0041da94: add      r0, r4, #0x33c
0041da98: ldr      r2, [r4, #0x57c]
0041da9c: add      r1, pc, r1
0041daa0: mov      r3, r5
0041daa4: bl       #0x427ca0
0041daa8: ldr      r1, [pc, #0x22c]
0041daac: add      r0, r4, #0x36c
0041dab0: ldr      r2, [r4, #0x57c]
0041dab4: add      r1, pc, r1
0041dab8: mov      r3, r5
0041dabc: bl       #0x427ca0
0041dac0: ldr      r1, [pc, #0x218]
0041dac4: add      r0, r4, #0x39c
0041dac8: ldr      r2, [r4, #0x57c]
0041dacc: add      r1, pc, r1
0041dad0: mov      r3, r5
0041dad4: bl       #0x427ca0
0041dad8: ldr      r1, [pc, #0x204]
0041dadc: add      r0, r4, #0x3cc
0041dae0: ldr      r2, [r4, #0x57c]
0041dae4: add      r1, pc, r1
0041dae8: mov      r3, r5
0041daec: bl       #0x427ca0
0041daf0: ldr      r1, [pc, #0x1f0]
0041daf4: add      r0, r4, #0x3fc
0041daf8: ldr      r2, [r4, #0x57c]
0041dafc: add      r1, pc, r1
0041db00: mov      r3, r5
0041db04: bl       #0x427ca0
0041db08: ldr      r1, [pc, #0x1dc]
0041db0c: add      r0, r4, #0x420
0041db10: add      r0, r0, #0xc
0041db14: add      r1, pc, r1
0041db18: ldr      r2, [r4, #0x57c]
0041db1c: mov      r3, r5
0041db20: bl       #0x427ca0
0041db24: ldr      r1, [pc, #0x1c4]
0041db28: add      r0, r4, #0x4e0
0041db2c: add      r0, r0, #0xc
0041db30: add      r1, pc, r1
0041db34: ldr      r2, [r4, #0x57c]
0041db38: mov      r3, r5
0041db3c: bl       #0x427ca0
0041db40: ldr      r1, [pc, #0x1ac]
0041db44: add      r0, r4, #0x510
0041db48: add      r0, r0, #0xc
0041db4c: add      r1, pc, r1
0041db50: ldr      r2, [r4, #0x57c]
0041db54: mov      r3, r5
0041db58: bl       #0x427ca0
0041db5c: ldr      r1, [pc, #0x194]
0041db60: add      r0, r4, #0x540
0041db64: add      r0, r0, #0xc
0041db68: add      r1, pc, r1
0041db6c: ldr      r2, [r4, #0x57c]
0041db70: mov      r3, r5
0041db74: bl       #0x427ca0
0041db78: ldr      r1, [pc, #0x17c]
0041db7c: add      r0, r4, #0x450
0041db80: add      r0, r0, #0xc
0041db84: add      r1, pc, r1
0041db88: mov      r3, r5
0041db8c: ldr      r2, [r4, #0x57c]
0041db90: bl       #0x427ca0
0041db94: ldr      r1, [pc, #0x164]
0041db98: add      r0, r4, #0x480
0041db9c: add      r0, r0, #0xc
0041dba0: add      r1, pc, r1
0041dba4: ldr      r2, [r4, #0x57c]
0041dba8: mov      r3, #0
0041dbac: bl       #0x427ca0
0041dbb0: ldr      r1, [pc, #0x14c]
0041dbb4: add      r0, r4, #0x4b0
0041dbb8: mov      r3, #0
0041dbbc: add      r0, r0, #0xc
0041dbc0: add      r1, pc, r1
0041dbc4: ldr      r2, [r4, #0x57c]
0041dbc8: bl       #0x427ca0
0041dbcc: mov      r3, #1
0041dbd0: strb     r3, [r4, #4]
0041dbd4: ldr      r3, [r6, sb]
0041dbd8: ldr      r2, [sp, #0x5c]
0041dbdc: ldr      r3, [r3]
0041dbe0: cmp      r2, r3
0041dbe4: bne      #0x41dc7c
0041dbe8: add      sp, sp, #0x64
0041dbec: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041dbf0: ldr      r3, [pc, #0x110]
0041dbf4: ldr      fp, [pc, #0x110]
0041dbf8: mov      r7, #0
0041dbfc: add      r3, pc, r3
0041dc00: add      fp, pc, fp
0041dc04: str      r3, [sp, #4]
0041dc08: add      r8, sp, #8
0041dc0c: mov      sl, r6
0041dc10: mov      r3, #0x30
0041dc14: mul      r6, r3, r7
0041dc18: add      r7, r7, #1
0041dc1c: mov      r1, fp
0041dc20: mov      r2, r7
0041dc24: mov      r0, r8
0041dc28: bl       #0x30eae4
0041dc2c: add      r0, r4, r6
0041dc30: mov      r3, r5
0041dc34: add      r0, r0, #0x21c
0041dc38: mov      r1, r8
0041dc3c: ldr      r2, [r4, #0x57c]
0041dc40: bl       #0x427ca0
0041dc44: ldr      r1, [sp, #4]
0041dc48: mov      r2, r7
0041dc4c: mov      r0, r8
0041dc50: bl       #0x30eae4
0041dc54: add      r0, r4, r6
0041dc58: add      r0, r0, #0x2ac
0041dc5c: mov      r1, r8
0041dc60: ldr      r2, [r4, #0x57c]
0041dc64: mov      r3, r5
0041dc68: bl       #0x427ca0
0041dc6c: cmp      r7, #3
0041dc70: bne      #0x41dc10
0041dc74: mov      r6, sl
0041dc78: b        #0x41da90
0041dc7c: bl       #0x30e310
0041dc80: subseq   r7, r7, r0, lsl #4
0041dc84: andeq    r4, r0, ip, lsr #1
0041dc88: strdeq   r3, r4, [r0], -r4
0041dc8c: subeq    r3, sl, r8, lsr sp
0041dc90: subeq    r3, sl, ip, ror r7
0041dc94: subeq    fp, sl, r4
0041dc98: subeq    fp, sl, r4, lsl r0
0041dc9c: subeq    fp, sl, ip, lsr #32
0041dca0: subeq    fp, sl, ip, lsr #32
0041dca4: subeq    fp, sl, ip, lsr r0
0041dca8: subeq    fp, sl, ip, asr #32
0041dcac: subeq    fp, sl, r4, rrx
0041dcb0: subeq    fp, sl, r4, lsl #1
0041dcb4: strdeq   fp, ip, [sl], #-0xc
0041dcb8: subeq    fp, sl, r4, lsl r1
0041dcbc: subeq    fp, sl, r4, lsr r1
0041dcc0: subeq    fp, sl, ip, asr #2
0041dcc4: subeq    fp, sl, r4, ror #2
0041dcc8: subeq    fp, sl, r4, lsl #3
0041dccc: subeq    fp, sl, r4, lsr #3
0041dcd0: strheq   fp, [sl], #-0x1c
0041dcd4: ldrdeq   fp, ip, [sl], #-0x1c
0041dcd8: strdeq   fp, ip, [sl], #-0x1c
0041dcdc: subeq    sl, sl, r4, lsl sl
0041dce0: strdeq   fp, ip, [sl], #-0x1c
0041dce4: subeq    fp, sl, r4, lsl #4
0041dce8: subeq    fp, sl, ip, lsl r2
0041dcec: subeq    fp, sl, r4, lsr r2
0041dcf0: subeq    fp, sl, r8, asr #4
0041dcf4: subeq    fp, sl, ip, asr #4
0041dcf8: subeq    fp, sl, r0, asr r2
0041dcfc: subeq    fp, sl, r4, asr r2
0041dd00: subeq    fp, sl, r8, ror #4
0041dd04: subeq    fp, sl, r0, ror #4
0041dd08: umaaleq  sl, sl, r4, lr
0041dd0c: subeq    sl, sl, r8, asr lr

# _ZN14InfoHUDManager10SlowUpdateEv
0041de54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041de58: ldr      r5, [pc, #0x1f4]
0041de5c: ldr      r1, [pc, #0x1f4]
0041de60: ldr      r4, [pc, #0x1f4]
0041de64: add      r5, pc, r5
0041de68: ldr      r3, [r5, r1]
0041de6c: ldr      r2, [r5, r4]
0041de70: sub      sp, sp, #0x3c
0041de74: ldr      r3, [r3]
0041de78: str      r1, [sp, #0xc]
0041de7c: mov      r1, #0
0041de80: mov      r6, r0
0041de84: ldr      r0, [r2, #0x40]
0041de88: mov      r2, r1
0041de8c: str      r3, [sp, #0x34]
0041de90: bl       #0x36e478
0041de94: ldr      r8, [r0, #0x660]
0041de98: cmp      r8, #0
0041de9c: beq      #0x41dfdc
0041dea0: mov      r7, #0
0041dea4: add      fp, r8, #0x3c8
0041dea8: add      sl, sp, #0x1c
0041deac: mov      sb, r7
0041deb0: mov      r0, r8
0041deb4: mov      r1, r7
0041deb8: bl       #0x3bbe68
0041debc: cmn      r0, #1
0041dec0: strb     sb, [sl, r7]
0041dec4: beq      #0x41ded8
0041dec8: mov      r1, r0
0041decc: mov      r0, fp
0041ded0: bl       #0x3d8358
0041ded4: strb     r0, [sl, r7]
0041ded8: add      r7, r7, #1
0041dedc: cmp      r7, #3
0041dee0: bne      #0x41deb0
0041dee4: add      r0, r6, #0x15c
0041dee8: bl       #0x427d50
0041deec: mov      r7, r0
0041def0: mov      r0, fp
0041def4: bl       #0x3d80b4
0041def8: ldr      r1, [pc, #0x160]
0041defc: eor      r0, r0, #1
0041df00: strb     r0, [r7, #0x9b]
0041df04: add      r1, pc, r1
0041df08: ldr      r0, [r5, r4]
0041df0c: bl       #0x320e44
0041df10: cmp      r0, #1
0041df14: ble      #0x41e00c
0041df18: mov      r4, #0
0041df1c: add      r8, sp, #0x20
0041df20: add      r7, sp, #0x10
0041df24: mov      sb, r4
0041df28: mov      fp, #0x30
0041df2c: mla      r0, fp, r4, r6
0041df30: strb     sb, [sp, #0x10]
0041df34: add      r0, r0, #0x18c
0041df38: strb     sb, [sp, #0x11]
0041df3c: bl       #0x427d50
0041df40: ldr      r2, [r0]
0041df44: mov      r3, r0
0041df48: mov      r0, r8
0041df4c: ldr      sl, [r2, #0x20]
0041df50: str      r3, [sp, #4]
0041df54: bl       #0x41ddec
0041df58: ldr      r3, [sp, #4]
0041df5c: mov      r1, r8
0041df60: mov      r2, r7
0041df64: mov      r0, r3
0041df68: blx      sl
0041df6c: ldrsb    r3, [sp, #0x20]
0041df70: cmn      r3, #1
0041df74: beq      #0x41dffc
0041df78: mov      r0, r7
0041df7c: bl       #0x797a54
0041df80: bl       #0x30ea24
0041df84: mla      sl, fp, r4, r6
0041df88: cmp      r0, #2
0041df8c: movhi    r0, #0
0041df90: add      sl, sl, #0x2ac
0041df94: str      r0, [sp, #8]
0041df98: mov      r0, sl
0041df9c: bl       #0x427d50
0041dfa0: cmp      r0, #0
0041dfa4: beq      #0x41dfc8
0041dfa8: mov      r0, sl
0041dfac: bl       #0x427d50
0041dfb0: ldr      r2, [sp, #8]
0041dfb4: add      r1, sp, #0x38
0041dfb8: add      r3, r1, r2
0041dfbc: ldrb     r3, [r3, #-0x1c]
0041dfc0: eor      r3, r3, #1
0041dfc4: strb     r3, [r0, #0x9b]
0041dfc8: add      r4, r4, #1
0041dfcc: mov      r0, r7
0041dfd0: bl       #0x797124
0041dfd4: cmp      r4, #3
0041dfd8: bne      #0x41df2c
0041dfdc: ldr      r2, [sp, #0xc]
0041dfe0: ldr      r3, [r5, r2]
0041dfe4: ldr      r2, [sp, #0x34]
0041dfe8: ldr      r3, [r3]
0041dfec: cmp      r2, r3
0041dff0: bne      #0x41e050
0041dff4: add      sp, sp, #0x3c
0041dff8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041dffc: ldr      r0, [sp, #0x2c]
0041e000: ldr      r1, [sp, #0x28]
0041e004: bl       #0x752b38
0041e008: b        #0x41df78
0041e00c: mov      r4, #0
0041e010: mov      r8, #0x30
0041e014: mla      r7, r8, r4, r6
0041e018: add      r7, r7, #0x2ac
0041e01c: mov      r0, r7
0041e020: bl       #0x427d50
0041e024: cmp      r0, #0
0041e028: beq      #0x41e040
0041e02c: mov      r0, r7
0041e030: bl       #0x427d50
0041e034: ldrb     r3, [sl, r4]
0041e038: eor      r3, r3, #1
0041e03c: strb     r3, [r0, #0x9b]
0041e040: add      r4, r4, #1
0041e044: cmp      r4, #3
0041e048: bne      #0x41e014
0041e04c: b        #0x41dfdc
0041e050: bl       #0x30e310
0041e054: subseq   r6, r7, ip, lsr #24
0041e058: andeq    r4, r0, ip, lsr #1
0041e05c: strdeq   r3, r4, [r0], -r4
0041e060: strdeq   r3, r4, [sl], #-0x64

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

# _ZN14InfoHUDManager6UpdateEv
0041ec00: push     {r4, r5, r6, r7, r8, lr}
0041ec04: ldr      r4, [pc, #0x9c]
0041ec08: ldr      r6, [pc, #0x9c]
0041ec0c: mov      r5, r0
0041ec10: add      r4, pc, r4
0041ec14: ldr      r0, [r4, r6]
0041ec18: bl       #0x31f594
0041ec1c: cmp      r0, #0
0041ec20: beq      #0x41ec30
0041ec24: ldrb     r3, [r0, #0x198]
0041ec28: cmp      r3, #0
0041ec2c: beq      #0x41ec70
0041ec30: ldr      r3, [r5, #0x57c]
0041ec34: cmp      r3, #0
0041ec38: beq      #0x41ec70
0041ec3c: ldrb     r7, [r5, #4]
0041ec40: cmp      r7, #0
0041ec44: beq      #0x41ec74
0041ec48: ldr      r7, [r5, #8]
0041ec4c: cmp      r7, #0
0041ec50: blt      #0x41ec94
0041ec54: ldr      r0, [r4, r6]
0041ec58: bl       #0x31f66c
0041ec5c: rsb      r0, r0, r7
0041ec60: str      r0, [r5, #8]
0041ec64: mov      r0, r5
0041ec68: pop      {r4, r5, r6, r7, r8, lr}
0041ec6c: b        #0x41e064
0041ec70: pop      {r4, r5, r6, r7, r8, pc}
0041ec74: mov      r0, r5
0041ec78: bl       #0x41d880
0041ec7c: mov      r0, r5
0041ec80: bl       #0x41d728
0041ec84: str      r7, [r5]
0041ec88: ldr      r7, [r5, #8]
0041ec8c: cmp      r7, #0
0041ec90: bge      #0x41ec54
0041ec94: mov      r3, #0x1f4
0041ec98: str      r3, [r5, #8]
0041ec9c: mov      r0, r5
0041eca0: bl       #0x41de54
0041eca4: b        #0x41ec64
0041eca8: subseq   r5, r7, r0, lsl #29
0041ecac: strdeq   r3, r4, [r0], -r4

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

# _ZN8RenderFX11SetPositionEPN7gameswf9characterEii
007aa3f0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007aa3f4: subs     r5, r1, #0
007aa3f8: sub      sp, sp, #0x18
007aa3fc: mov      r7, r3
007aa400: beq      #0x7aa5a0
007aa404: add      r3, sp, #0xc
007aa408: mov      r1, #0
007aa40c: ldr      r6, [r5, #0x4c]
007aa410: str      r1, [r3], #4
007aa414: str      r1, [r3], #4
007aa418: str      r1, [r3]
007aa41c: mov      ip, #0x3f800000
007aa420: mov      r0, r2
007aa424: str      ip, [sp, #0x10]
007aa428: str      ip, [sp]
007aa42c: str      r1, [sp, #4]
007aa430: bl       #0x30e964
007aa434: mov      r1, #0x41000000
007aa438: add      r1, r1, #0xa00000
007aa43c: bl       #0x30ed6c
007aa440: mov      r8, r0
007aa444: mov      r0, r7
007aa448: bl       #0x30e964
007aa44c: mov      r1, #0x41000000
007aa450: add      r1, r1, #0xa00000
007aa454: bl       #0x30ed6c
007aa458: mov      r1, #0
007aa45c: mov      sl, r0
007aa460: bl       #0x30ed6c
007aa464: mov      r1, r0
007aa468: mov      r0, r8
007aa46c: bl       #0x30eba4
007aa470: mov      r1, #0
007aa474: bl       #0x30eba4
007aa478: mvn      r1, #0x800000
007aa47c: mov      r7, r0
007aa480: bl       #0x30e4b4
007aa484: cmp      r0, #0
007aa488: mov      r4, sp
007aa48c: bne      #0x7aa5c4
007aa490: mov      r7, #0
007aa494: ldr      r1, [sp, #0xc]
007aa498: mov      r0, r8
007aa49c: str      r7, [sp, #8]
007aa4a0: bl       #0x30ed6c
007aa4a4: mov      r1, r0
007aa4a8: mov      r0, sl
007aa4ac: bl       #0x30eba4
007aa4b0: ldr      r1, [sp, #0x14]
007aa4b4: bl       #0x30eba4
007aa4b8: mvn      r1, #0x800000
007aa4bc: mov      r7, r0
007aa4c0: bl       #0x30e4b4
007aa4c4: cmp      r0, #0
007aa4c8: bne      #0x7aa5a8
007aa4cc: mov      r7, #0
007aa4d0: str      r7, [sp, #0x14]
007aa4d4: ldr      r0, [r6]
007aa4d8: ldr      r8, [r6, #4]
007aa4dc: mov      r1, r0
007aa4e0: bl       #0x30ed6c
007aa4e4: mov      r1, r8
007aa4e8: mov      r7, r0
007aa4ec: mov      r0, r8
007aa4f0: bl       #0x30ed6c
007aa4f4: mov      r1, r0
007aa4f8: mov      r0, r7
007aa4fc: bl       #0x30eba4
007aa500: bl       #0x30e124
007aa504: ldr      r8, [r6, #0x10]
007aa508: mov      sl, r0
007aa50c: ldr      r1, [r6]
007aa510: mov      r0, r8
007aa514: bl       #0x30ed6c
007aa518: ldr      r7, [r6, #0xc]
007aa51c: ldr      r1, [r6, #4]
007aa520: mov      sb, r0
007aa524: mov      r0, r7
007aa528: bl       #0x30ed6c
007aa52c: mov      r1, r0
007aa530: mov      r0, sb
007aa534: bl       #0x30e3ac
007aa538: mov      r1, #0
007aa53c: bl       #0x30e70c
007aa540: mov      r1, r8
007aa544: cmp      r0, #0
007aa548: mov      r0, r8
007aa54c: addne    sl, sl, #0x80000000
007aa550: bl       #0x30ed6c
007aa554: mov      r1, r7
007aa558: mov      r8, r0
007aa55c: mov      r0, r7
007aa560: bl       #0x30ed6c
007aa564: mov      r1, r0
007aa568: mov      r0, r8
007aa56c: bl       #0x30eba4
007aa570: bl       #0x30e124
007aa574: mov      r7, r0
007aa578: mov      r0, r6
007aa57c: bl       #0x7968b8
007aa580: mov      r1, sl
007aa584: mov      r3, r0
007aa588: mov      r2, r7
007aa58c: mov      r0, sp
007aa590: bl       #0x796920
007aa594: mov      r0, r5
007aa598: mov      r1, sp
007aa59c: bl       #0x4121f8
007aa5a0: add      sp, sp, #0x18
007aa5a4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007aa5a8: mvn      r1, #0x80000000
007aa5ac: mov      r0, r7
007aa5b0: sub      r1, r1, #0x800000
007aa5b4: bl       #0x30e9ac
007aa5b8: cmp      r0, #0
007aa5bc: bne      #0x7aa4d0
007aa5c0: b        #0x7aa4cc
007aa5c4: mvn      r1, #0x80000000
007aa5c8: mov      r0, r7
007aa5cc: sub      r1, r1, #0x800000
007aa5d0: bl       #0x30e9ac
007aa5d4: cmp      r0, #0
007aa5d8: bne      #0x7aa494
007aa5dc: b        #0x7aa490

# _ZN14InfoHUDManager10FastUpdateEv
0041e064: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041e068: ldr      r8, [pc, #0xb5c]
0041e06c: ldr      r1, [pc, #0xb5c]
0041e070: ldr      r2, [pc, #0xb5c]
0041e074: add      r8, pc, r8
0041e078: ldr      r3, [r8, r1]
0041e07c: sub      sp, sp, #0x18c
0041e080: str      r2, [sp, #0xc]
0041e084: ldr      r2, [r8, r2]
0041e088: ldr      r3, [r3]
0041e08c: str      r1, [sp, #0x18]
0041e090: mov      r1, #0
0041e094: mov      r4, r0
0041e098: ldr      r0, [r2, #0x40]
0041e09c: mov      r2, r1
0041e0a0: str      r3, [sp, #0x184]
0041e0a4: bl       #0x36e478
0041e0a8: ldr      r6, [r0, #0x660]
0041e0ac: cmp      r6, #0
0041e0b0: beq      #0x41e5c0
0041e0b4: add      r0, r6, #0x37c
0041e0b8: bl       #0x3fc690
0041e0bc: ldr      r1, [pc, #0xb14]
0041e0c0: add      r5, sp, #0xd8
0041e0c4: mov      r2, r0
0041e0c8: add      r1, pc, r1
0041e0cc: mov      r0, r5
0041e0d0: bl       #0x30eae4
0041e0d4: add      r0, r4, #0xfc
0041e0d8: ldr      r7, [r4, #0x57c]
0041e0dc: bl       #0x427d50
0041e0e0: mov      r2, r5
0041e0e4: mov      r1, r0
0041e0e8: mov      r3, #0
0041e0ec: mov      r0, r7
0041e0f0: bl       #0x7a92e0
0041e0f4: movw     r3, #0x1088
0041e0f8: ldr      r0, [r6, r3]
0041e0fc: mov      sl, #0x64
0041e100: movw     r3, #0x1090
0041e104: ldr      r1, [r6, r3]
0041e108: mul      r0, sl, r0
0041e10c: bl       #0x30e2a4
0041e110: movw     r3, #0x109c
0041e114: ldr      r3, [r6, r3]
0041e118: movw     r2, #0x10a4
0041e11c: ldr      r1, [r6, r2]
0041e120: sub      r5, r0, #1
0041e124: mul      r0, sl, r3
0041e128: bl       #0x30e2a4
0041e12c: movw     r3, #0x107c
0041e130: ldr      r3, [r6, r3]
0041e134: sub      r7, r0, #1
0041e138: bic      r5, r5, r5, asr #31
0041e13c: mul      r0, sl, r3
0041e140: mov      r3, #0x1080
0041e144: ldr      r1, [r6, r3]
0041e148: bl       #0x30e2a4
0041e14c: cmp      r0, #0x63
0041e150: movlt    sl, r0
0041e154: movge    sl, #0x63
0041e158: add      r0, r4, #0xc
0041e15c: ldr      sb, [r4, #0x57c]
0041e160: bl       #0x427d50
0041e164: cmp      r5, #0x63
0041e168: movge    r5, #0x63
0041e16c: mov      r1, r0
0041e170: mov      r2, r5
0041e174: mov      r0, sb
0041e178: mov      r3, #0
0041e17c: bl       #0x7a7d34
0041e180: add      r0, r4, #0x9c
0041e184: ldr      sb, [r4, #0x57c]
0041e188: bl       #0x427d50
0041e18c: cmp      r7, #0x63
0041e190: movge    r7, #0x63
0041e194: mov      r1, r0
0041e198: mov      r2, r7
0041e19c: mov      r0, sb
0041e1a0: mov      r3, #0
0041e1a4: bl       #0x7a7d34
0041e1a8: add      r0, r4, #0xcc
0041e1ac: ldr      r7, [r4, #0x57c]
0041e1b0: bl       #0x427d50
0041e1b4: mov      r2, sl
0041e1b8: mov      r1, r0
0041e1bc: mov      r3, #0
0041e1c0: mov      r0, r7
0041e1c4: bl       #0x7a7d34
0041e1c8: add      r0, r4, #0x3c
0041e1cc: ldr      r7, [r4, #0x57c]
0041e1d0: bl       #0x427d50
0041e1d4: mov      r2, r5
0041e1d8: mov      r1, r0
0041e1dc: mov      r3, #0
0041e1e0: mov      r0, r7
0041e1e4: bl       #0x7a7d34
0041e1e8: add      r0, r4, #0x6c
0041e1ec: ldr      r7, [r4, #0x57c]
0041e1f0: bl       #0x427d50
0041e1f4: mov      r2, r5
0041e1f8: mov      r1, r0
0041e1fc: mov      r3, #0
0041e200: mov      r0, r7
0041e204: bl       #0x7a7d34
0041e208: add      r0, r4, #0x33c
0041e20c: bl       #0x427d50
0041e210: mov      r1, #0x94
0041e214: mov      r5, r0
0041e218: mov      r2, #0
0041e21c: add      r0, r6, #0x560
0041e220: bl       #0x3df6e0
0041e224: mov      r7, #0
0041e228: subs     r0, r0, #0
0041e22c: movne    r0, #1
0041e230: strb     r0, [r5, #0x9b]
0041e234: mov      sl, #0
0041e238: mov      sb, r7
0041e23c: add      r5, sp, #0xa4
0041e240: mov      r0, r6
0041e244: mov      r1, sb
0041e248: bl       #0x3bbe68
0041e24c: cmn      r0, #1
0041e250: str      sl, [r5, r7]
0041e254: beq      #0x41e270
0041e258: ldr      r3, [r6, #0x47c]
0041e25c: ldr      r0, [r3, r0, lsl #2]
0041e260: cmp      r0, #0
0041e264: beq      #0x41e270
0041e268: bl       #0x3da3d0
0041e26c: str      r0, [r5, r7]
0041e270: add      r7, r7, #4
0041e274: cmp      r7, #0xc
0041e278: add      sb, sb, #1
0041e27c: bne      #0x41e240
0041e280: ldr      r3, [sp, #0xc]
0041e284: ldr      r1, [pc, #0x950]
0041e288: ldr      r0, [r8, r3]
0041e28c: add      r1, pc, r1
0041e290: bl       #0x320e44
0041e294: cmp      r0, #1
0041e298: ble      #0x41eaac
0041e29c: mov      r5, #0
0041e2a0: add      sb, sp, #0x170
0041e2a4: add      r7, sp, #0x98
0041e2a8: mov      fp, r5
0041e2ac: str      r6, [sp, #0x10]
0041e2b0: mov      r1, #0x30
0041e2b4: mla      r0, r1, r5, r4
0041e2b8: strb     fp, [sp, #0x98]
0041e2bc: add      r0, r0, #0x18c
0041e2c0: strb     fp, [sp, #0x99]
0041e2c4: bl       #0x427d50
0041e2c8: ldr      r3, [r0]
0041e2cc: mov      sl, r0
0041e2d0: mov      r0, sb
0041e2d4: ldr      r6, [r3, #0x20]
0041e2d8: bl       #0x41ddec
0041e2dc: mov      r0, sl
0041e2e0: mov      r1, sb
0041e2e4: mov      r2, r7
0041e2e8: blx      r6
0041e2ec: ldrb     r0, [sp, #0x170]
0041e2f0: sxtb     r3, r0
0041e2f4: cmn      r3, #1
0041e2f8: beq      #0x41e620
0041e2fc: mov      r0, r7
0041e300: bl       #0x797a54
0041e304: bl       #0x30ea24
0041e308: mov      r1, #0x30
0041e30c: mla      r2, r1, r5, r4
0041e310: cmp      r0, #2
0041e314: movls    r3, r0
0041e318: movhi    r3, #0
0041e31c: add      r0, r2, #0x21c
0041e320: ldr      r6, [r4, #0x57c]
0041e324: str      r3, [sp, #8]
0041e328: bl       #0x427d50
0041e32c: ldr      r3, [sp, #8]
0041e330: add      r2, sp, #0x188
0041e334: mov      r1, #0x42000000
0041e338: add      r3, r2, r3, lsl #2
0041e33c: add      r1, r1, #0xc80000
0041e340: mov      sl, r0
0041e344: ldr      r0, [r3, #-0xe4]
0041e348: bl       #0x30ed6c
0041e34c: bl       #0x30e4cc
0041e350: sub      r2, r0, #1
0041e354: mov      r1, sl
0041e358: bic      r2, r2, r2, asr #31
0041e35c: mov      r0, r6
0041e360: mov      r3, #0
0041e364: bl       #0x7a7d34
0041e368: add      r5, r5, #1
0041e36c: mov      r0, r7
0041e370: bl       #0x797124
0041e374: cmp      r5, #3
0041e378: bne      #0x41e2b0
0041e37c: ldr      r6, [sp, #0x10]
0041e380: ldr      r3, [r6, #0x47c]
0041e384: ldr      r3, [r3]
0041e388: cmp      r3, #0
0041e38c: beq      #0x41e3d4
0041e390: add      r0, r4, #0x12c
0041e394: ldr      r5, [r4, #0x57c]
0041e398: bl       #0x427d50
0041e39c: ldr      r3, [r6, #0x488]
0041e3a0: mov      r7, r0
0041e3a4: ldr      r0, [r3]
0041e3a8: bl       #0x3da3d0
0041e3ac: mov      r1, #0x42000000
0041e3b0: add      r1, r1, #0xc80000
0041e3b4: bl       #0x30ed6c
0041e3b8: bl       #0x30e4cc
0041e3bc: sub      r2, r0, #1
0041e3c0: mov      r1, r7
0041e3c4: mov      r0, r5
0041e3c8: bic      r2, r2, r2, asr #31
0041e3cc: mov      r3, #0
0041e3d0: bl       #0x7a7d34
0041e3d4: bl       #0x7fd794
0041e3d8: ldrb     r3, [r0, #5]
0041e3dc: cmp      r3, #0
0041e3e0: bne      #0x41e674
0041e3e4: ldr      r3, [r6]
0041e3e8: mov      r0, r6
0041e3ec: mov      lr, pc
0041e3f0: ldr      pc, [r3, #0x34]
0041e3f4: cmp      r0, #0
0041e3f8: beq      #0x41e5e0
0041e3fc: mov      r7, #0
0041e400: add      r5, r6, #0x3c8
0041e404: mov      r0, r5
0041e408: bl       #0x3d5450
0041e40c: cmp      r0, #0
0041e410: beq      #0x41e428
0041e414: mov      r0, r5
0041e418: bl       #0x3d5450
0041e41c: bl       #0x3a3064
0041e420: cmp      r0, #0
0041e424: bne      #0x41eb04
0041e428: cmp      r7, #0
0041e42c: beq      #0x41eb58
0041e430: movw     r3, #0x14a4
0041e434: ldr      r5, [r6, r3]
0041e438: cmp      r5, #0
0041e43c: beq      #0x41eb58
0041e440: ldr      r3, [r4]
0041e444: cmp      r5, r3
0041e448: beq      #0x41e56c
0041e44c: ldr      r1, [sp, #0xc]
0041e450: mov      r2, #0x1040
0041e454: str      r5, [r4]
0041e458: ldr      r3, [r8, r1]
0041e45c: ldr      r1, [r5, r2]
0041e460: ldr      r0, [r3, #0x34]
0041e464: bl       #0x508edc
0041e468: mov      sb, r0
0041e46c: mov      r0, r5
0041e470: bl       #0x3a3158
0041e474: cmp      r0, #0
0041e478: beq      #0x41eb2c
0041e47c: movw     r3, #0x3f3f
0041e480: strh     r3, [sp, #0x98]
0041e484: mov      r3, #0
0041e488: strb     r3, [sp, #0x9a]
0041e48c: add      r7, sp, #0x98
0041e490: add      r6, r4, #0x39c
0041e494: mov      r0, r6
0041e498: bl       #0x427d50
0041e49c: mov      r3, #1
0041e4a0: strb     r3, [r0, #0x9b]
0041e4a4: mov      r0, r6
0041e4a8: ldr      r6, [r4, #0x57c]
0041e4ac: bl       #0x427d50
0041e4b0: ldr      r2, [pc, #0x728]
0041e4b4: mov      r1, r0
0041e4b8: mov      r3, #0
0041e4bc: add      r2, pc, r2
0041e4c0: mov      r0, r6
0041e4c4: bl       #0x7ab924
0041e4c8: ldr      r3, [pc, #0x714]
0041e4cc: add      sl, sp, #0xf8
0041e4d0: ldr      r6, [r8, r3]
0041e4d4: mov      r0, r6
0041e4d8: bl       #0x337888
0041e4dc: ldr      r1, [pc, #0x704]
0041e4e0: mov      r0, sl
0041e4e4: str      sl, [sp, #0x108]
0041e4e8: add      r1, pc, r1
0041e4ec: add      r2, r1, #0x19
0041e4f0: str      sl, [sp, #0x10c]
0041e4f4: bl       #0x3116e8
0041e4f8: mov      r0, r6
0041e4fc: mov      r1, sl
0041e500: bl       #0x337a88
0041e504: mov      r6, r0
0041e508: mov      r0, sl
0041e50c: bl       #0x3139ac
0041e510: cmp      r6, #0
0041e514: beq      #0x41e630
0041e518: ldr      r1, [pc, #0x6cc]
0041e51c: ldr      r2, [r5, #0x108]
0041e520: mov      r0, r7
0041e524: add      r1, pc, r1
0041e528: bl       #0x30eae4
0041e52c: add      r0, r4, #0x3cc
0041e530: ldr      r6, [r4, #0x57c]
0041e534: bl       #0x427d50
0041e538: ldr      r2, [r5, #0x44]
0041e53c: mov      r1, r0
0041e540: mov      r3, #0
0041e544: mov      r0, r6
0041e548: bl       #0x7a92e0
0041e54c: add      r0, r4, #0x3fc
0041e550: ldr      r6, [r4, #0x57c]
0041e554: bl       #0x427d50
0041e558: mov      r2, r7
0041e55c: mov      r1, r0
0041e560: mov      r3, #0
0041e564: mov      r0, r6
0041e568: bl       #0x7a92e0
0041e56c: mov      r0, r5
0041e570: bl       #0x3bd2dc
0041e574: mov      r6, r0
0041e578: add      r0, r4, #0x420
0041e57c: add      r0, r0, #0xc
0041e580: ldr      r4, [r4, #0x57c]
0041e584: bl       #0x427d50
0041e588: mov      r1, #0x42000000
0041e58c: add      r1, r1, #0xc80000
0041e590: mov      r5, r0
0041e594: mov      r0, r6
0041e598: bl       #0x30ed6c
0041e59c: bl       #0x30e4cc
0041e5a0: cmp      r0, #0x64
0041e5a4: movlt    r2, r0
0041e5a8: movge    r2, #0x64
0041e5ac: mov      r1, r5
0041e5b0: mov      r0, r4
0041e5b4: sub      r2, r2, #1
0041e5b8: mov      r3, #0
0041e5bc: bl       #0x7a7d34
0041e5c0: ldr      r2, [sp, #0x18]
0041e5c4: ldr      r3, [r8, r2]
0041e5c8: ldr      r2, [sp, #0x184]
0041e5cc: ldr      r3, [r3]
0041e5d0: cmp      r2, r3
0041e5d4: bne      #0x41ebc8
0041e5d8: add      sp, sp, #0x18c
0041e5dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041e5e0: movw     r5, #0x14a4
0041e5e4: ldr      r3, [r6, r5]
0041e5e8: cmp      r3, #0
0041e5ec: beq      #0x41e3fc
0041e5f0: mov      r0, r3
0041e5f4: ldr      r3, [r3]
0041e5f8: mov      lr, pc
0041e5fc: ldr      pc, [r3, #0x24]
0041e600: cmp      r0, #0
0041e604: beq      #0x41e3fc
0041e608: ldr      r0, [r6, r5]
0041e60c: bl       #0x3a3064
0041e610: cmp      r0, #0
0041e614: movne    r7, #1
0041e618: bne      #0x41e400
0041e61c: b        #0x41e3fc
0041e620: ldr      r0, [sp, #0x17c]
0041e624: ldr      r1, [sp, #0x178]
0041e628: bl       #0x752b38
0041e62c: b        #0x41e2fc
0041e630: add      r0, r4, #0x3cc
0041e634: ldr      sl, [r4, #0x57c]
0041e638: bl       #0x427d50
0041e63c: mov      r2, sb
0041e640: mov      r1, r0
0041e644: mov      r3, r6
0041e648: mov      r0, sl
0041e64c: bl       #0x7a92e0
0041e650: add      r0, r4, #0x3fc
0041e654: ldr      sl, [r4, #0x57c]
0041e658: bl       #0x427d50
0041e65c: mov      r2, r7
0041e660: mov      r1, r0
0041e664: mov      r3, r6
0041e668: mov      r0, sl
0041e66c: bl       #0x7a92e0
0041e670: b        #0x41e56c
0041e674: ldr      r0, [sp, #0xc]
0041e678: mov      r1, #0
0041e67c: mov      r2, r1
0041e680: ldr      r3, [r8, r0]
0041e684: ldr      r0, [r3, #0x40]
0041e688: bl       #0x36e478
0041e68c: ldr      r3, [r0, #0x3a8]
0041e690: cmp      r3, #0
0041e694: blt      #0x41e6e4
0041e698: movw     r2, #0x4dd3
0041e69c: movt     r2, #0x1062
0041e6a0: smull    r1, r2, r2, r3
0041e6a4: ldr      r1, [pc, #0x544]
0041e6a8: asr      r3, r3, #0x1f
0041e6ac: add      r5, sp, #0xb8
0041e6b0: rsb      r2, r3, r2, asr #6
0041e6b4: add      r1, pc, r1
0041e6b8: mov      r0, r5
0041e6bc: bl       #0x30eae4
0041e6c0: add      r0, r4, #0x4b0
0041e6c4: add      r0, r0, #0xc
0041e6c8: ldr      r7, [r4, #0x57c]
0041e6cc: bl       #0x427d50
0041e6d0: mov      r2, r5
0041e6d4: mov      r1, r0
0041e6d8: mov      r3, #0
0041e6dc: mov      r0, r7
0041e6e0: bl       #0x7a92e0
0041e6e4: ldr      r2, [sp, #0xc]
0041e6e8: add      r5, sp, #0x44
0041e6ec: mov      sl, #0
0041e6f0: ldr      r3, [r8, r2]
0041e6f4: mov      fp, r4
0041e6f8: mov      r7, sl
0041e6fc: ldr      r0, [r3, #0x40]
0041e700: bl       #0x36d7a8
0041e704: movw     r3, #0x4dd3
0041e708: movt     r3, #0x1062
0041e70c: str      r3, [sp, #0x38]
0041e710: ldr      r3, [pc, #0x4dc]
0041e714: str      r0, [sp, #0x28]
0041e718: add      r1, r5, #0xc
0041e71c: add      r3, pc, r3
0041e720: str      r3, [sp, #0x34]
0041e724: add      r0, sp, #0x110
0041e728: add      r3, sp, #0x158
0041e72c: str      sl, [sp, #0x20]
0041e730: add      sb, sp, #0x128
0041e734: str      r0, [sp, #0x10]
0041e738: str      r1, [sp, #0x30]
0041e73c: str      r6, [sp, #0x3c]
0041e740: str      r8, [sp, #0x24]
0041e744: mov      r4, r3
0041e748: mov      r0, #0x30
0041e74c: mla      r0, r0, sl, fp
0041e750: add      r0, r0, #0x4e0
0041e754: add      r0, r0, #0xc
0041e758: bl       #0x427d50
0041e75c: mov      r1, #0x10
0041e760: str      r0, [sp, #0x2c]
0041e764: mov      r0, r4
0041e768: str      r4, [sp, #0x168]
0041e76c: str      r4, [sp, #0x16c]
0041e770: bl       #0x31167c
0041e774: ldr      r3, [sp, #0x28]
0041e778: ldr      r2, [sp, #0x20]
0041e77c: cmp      r2, r3
0041e780: ldr      r3, [sp, #0x168]
0041e784: strb     r7, [r3]
0041e788: blt      #0x41e910
0041e78c: mov      r6, #0
0041e790: mvn      r0, #0
0041e794: str      r6, [sp, #0x14]
0041e798: mov      r3, r6
0041e79c: str      r0, [sp, #0x1c]
0041e7a0: ldr      r0, [sp, #0x24]
0041e7a4: ldr      r1, [sp, #0xc]
0041e7a8: mov      r8, #1
0041e7ac: str      sb, [sp, #0x138]
0041e7b0: ldr      r2, [r0, r1]
0041e7b4: mov      r0, sb
0041e7b8: ldr      r1, [sp, #0x16c]
0041e7bc: ldr      ip, [r2, #0x34]
0041e7c0: ldr      r2, [sp, #0x168]
0041e7c4: strb     r3, [sp, #0x48]
0041e7c8: strb     r8, [sp, #0x45]
0041e7cc: str      ip, [sp, #8]
0041e7d0: strb     r7, [sp, #0x44]
0041e7d4: str      sb, [sp, #0x13c]
0041e7d8: bl       #0x3116e8
0041e7dc: ldr      ip, [sp, #8]
0041e7e0: mov      r2, sb
0041e7e4: mov      r3, r8
0041e7e8: mov      r1, ip
0041e7ec: ldr      r0, [sp, #0x10]
0041e7f0: bl       #0x507c0c
0041e7f4: ldr      r1, [sp, #0x124]
0041e7f8: ldr      r0, [sp, #0x30]
0041e7fc: strb     r7, [sp, #0x50]
0041e800: strb     r7, [sp, #0x51]
0041e804: bl       #0x797350
0041e808: ldr      r0, [sp, #0x10]
0041e80c: bl       #0x3139ac
0041e810: mov      r0, sb
0041e814: bl       #0x3139ac
0041e818: mov      r8, #2
0041e81c: ldr      r0, [sp, #0x14]
0041e820: strb     r7, [sp, #0x5c]
0041e824: strb     r8, [sp, #0x5d]
0041e828: bl       #0x30ed30
0041e82c: strd     r0, r1, [sp, #0xb0]
0041e830: ldr      r3, [sp, #0xb0]
0041e834: mov      r0, r6
0041e838: str      r3, [r5, #0x1c]
0041e83c: ldr      r3, [sp, #0xb4]
0041e840: str      r3, [r5, #0x20]
0041e844: strb     r7, [sp, #0x68]
0041e848: strb     r8, [sp, #0x69]
0041e84c: bl       #0x30ed30
0041e850: strd     r0, r1, [sp, #0xb0]
0041e854: ldr      r3, [sp, #0xb0]
0041e858: mov      r0, sl
0041e85c: add      sl, sl, #1
0041e860: str      r3, [r5, #0x28]
0041e864: ldr      r3, [sp, #0xb4]
0041e868: str      r3, [r5, #0x2c]
0041e86c: strb     r7, [sp, #0x74]
0041e870: strb     r8, [sp, #0x75]
0041e874: bl       #0x30ed30
0041e878: strd     r0, r1, [sp, #0xb0]
0041e87c: ldr      r3, [sp, #0xb0]
0041e880: ldr      r0, [sp, #0x1c]
0041e884: str      r3, [r5, #0x34]
0041e888: ldr      r3, [sp, #0xb4]
0041e88c: str      r3, [r5, #0x38]
0041e890: strb     r8, [sp, #0x81]
0041e894: strb     r7, [sp, #0x80]
0041e898: bl       #0x30ed30
0041e89c: strd     r0, r1, [sp, #0xb0]
0041e8a0: ldr      r3, [sp, #0xb0]
0041e8a4: ldr      r6, [fp, #0x57c]
0041e8a8: str      r3, [r5, #0x40]
0041e8ac: ldr      r3, [sp, #0xb4]
0041e8b0: mov      r0, r6
0041e8b4: str      r3, [r5, #0x44]
0041e8b8: bl       #0x7a7ca0
0041e8bc: mov      ip, #6
0041e8c0: mov      r1, r0
0041e8c4: ldr      r2, [sp, #0x34]
0041e8c8: mov      r0, r6
0041e8cc: mov      r3, r5
0041e8d0: str      ip, [sp]
0041e8d4: bl       #0x7abe0c
0041e8d8: add      r6, r5, #0x48
0041e8dc: sub      r6, r6, #0xc
0041e8e0: mov      r0, r6
0041e8e4: bl       #0x797124
0041e8e8: cmp      r6, r5
0041e8ec: bne      #0x41e8dc
0041e8f0: mov      r0, r4
0041e8f4: bl       #0x3139ac
0041e8f8: cmp      sl, #2
0041e8fc: ble      #0x41e748
0041e900: ldr      r6, [sp, #0x3c]
0041e904: mov      r4, fp
0041e908: ldr      r8, [sp, #0x24]
0041e90c: b        #0x41e3e4
0041e910: ldr      r1, [sp, #0x24]
0041e914: ldr      r0, [sp, #0xc]
0041e918: mov      r2, #0
0041e91c: ldr      r3, [r1, r0]
0041e920: ldr      r1, [sp, #0x20]
0041e924: ldr      r0, [r3, #0x40]
0041e928: bl       #0x36e744
0041e92c: ldr      r3, [r0]
0041e930: mov      r8, r0
0041e934: mov      lr, pc
0041e938: ldr      pc, [r3, #0x50]
0041e93c: cmp      r0, #0
0041e940: bne      #0x41eb14
0041e944: ldr      r0, [r8, #0x660]
0041e948: cmp      r0, #0
0041e94c: beq      #0x41e78c
0041e950: ldrb     r3, [r8, #0x4e5]
0041e954: cmp      r3, #0
0041e958: beq      #0x41e78c
0041e95c: ldr      r3, [r8, #0x330]
0041e960: str      r3, [sp, #0x14]
0041e964: bl       #0x3bd2dc
0041e968: mov      r1, #0x42000000
0041e96c: add      r1, r1, #0xc80000
0041e970: bl       #0x30ed6c
0041e974: bl       #0x30e4cc
0041e978: add      r3, sp, #0x140
0041e97c: str      r3, [sp, #0x150]
0041e980: str      r3, [sp, #0x154]
0041e984: ldr      r1, [r8, #0x2e4]
0041e988: ldr      r2, [r8, #0x2e0]
0041e98c: sub      r6, r0, #1
0041e990: mov      r0, r3
0041e994: str      r3, [sp, #8]
0041e998: bl       #0x3116e8
0041e99c: ldr      r1, [sp, #0x154]
0041e9a0: ldr      r2, [sp, #0x150]
0041e9a4: mov      r0, r4
0041e9a8: bl       #0x3109e0
0041e9ac: ldr      r3, [sp, #8]
0041e9b0: cmp      r6, #0x63
0041e9b4: movge    r6, #0x63
0041e9b8: bic      r6, r6, r6, asr #31
0041e9bc: mov      r0, r3
0041e9c0: bl       #0x3139ac
0041e9c4: ldr      r3, [r8, #0x3a8]
0041e9c8: cmp      r3, #0
0041e9cc: ldrge    r1, [sp, #0x38]
0041e9d0: mvnlt    r0, #0
0041e9d4: strlt    r0, [sp, #0x1c]
0041e9d8: smullge  r1, r2, r1, r3
0041e9dc: asrge    r3, r3, #0x1f
0041e9e0: rsbge    r3, r3, r2, asr #6
0041e9e4: ldr      r2, [sp, #0x20]
0041e9e8: strge    r3, [sp, #0x1c]
0041e9ec: ldr      r3, [r8, #0x660]
0041e9f0: add      r2, r2, #1
0041e9f4: str      r2, [sp, #0x20]
0041e9f8: ldr      r2, [r3, #0x160]
0041e9fc: ldr      r0, [r3, #0x168]
0041ea00: ldr      r3, [r3, #0x164]
0041ea04: mov      r1, #0x43000000
0041ea08: add      r1, r1, #0xaf0000
0041ea0c: str      r2, [sp, #0x8c]
0041ea10: str      r3, [sp, #0x90]
0041ea14: bl       #0x30eba4
0041ea18: mov      r3, #0
0041ea1c: add      r1, sp, #0x98
0041ea20: str      r0, [sp, #0x94]
0041ea24: add      r0, sp, #0x8c
0041ea28: str      r3, [sp, #0x98]
0041ea2c: str      r3, [sp, #0x9c]
0041ea30: bl       #0x50e830
0041ea34: ldr      r0, [fp, #0x57c]
0041ea38: bl       #0x7a7cac
0041ea3c: bl       #0x416538
0041ea40: mov      r8, r0
0041ea44: ldr      r0, [fp, #0x57c]
0041ea48: bl       #0x7a7cac
0041ea4c: bl       #0x416578
0041ea50: mov      r3, r0
0041ea54: ldr      r0, [sp, #0x98]
0041ea58: str      r3, [sp, #8]
0041ea5c: bl       #0x30e964
0041ea60: mov      r1, r0
0041ea64: mov      r0, r8
0041ea68: bl       #0x30ed6c
0041ea6c: bl       #0x30e4cc
0041ea70: mov      r8, r0
0041ea74: ldr      r0, [sp, #0x9c]
0041ea78: bl       #0x30e964
0041ea7c: ldr      r3, [sp, #8]
0041ea80: mov      r1, r0
0041ea84: mov      r0, r3
0041ea88: bl       #0x30ed6c
0041ea8c: bl       #0x30e4cc
0041ea90: ldr      r1, [sp, #0x2c]
0041ea94: mov      r3, r0
0041ea98: mov      r2, r8
0041ea9c: ldr      r0, [fp, #0x57c]
0041eaa0: bl       #0x7aa3f0
0041eaa4: mov      r3, #1
0041eaa8: b        #0x41e7a0
0041eaac: mov      r7, #0
0041eab0: mov      fp, #0x30
0041eab4: mla      r0, fp, r7, r4
0041eab8: ldr      sl, [r4, #0x57c]
0041eabc: add      r0, r0, #0x21c
0041eac0: bl       #0x427d50
0041eac4: mov      r1, #0x42000000
0041eac8: add      r1, r1, #0xc80000
0041eacc: mov      sb, r0
0041ead0: ldr      r0, [r5, r7, lsl #2]
0041ead4: bl       #0x30ed6c
0041ead8: bl       #0x30e4cc
0041eadc: sub      r2, r0, #1
0041eae0: mov      r1, sb
0041eae4: mov      r0, sl
0041eae8: bic      r2, r2, r2, asr #31
0041eaec: add      r7, r7, #1
0041eaf0: mov      r3, #0
0041eaf4: bl       #0x7a7d34
0041eaf8: cmp      r7, #3
0041eafc: bne      #0x41eab4
0041eb00: b        #0x41e380
0041eb04: mov      r0, r5
0041eb08: bl       #0x3d5450
0041eb0c: mov      r5, r0
0041eb10: b        #0x41e438
0041eb14: ldr      r2, [sp, #0x20]
0041eb18: mov      r0, r4
0041eb1c: add      r2, r2, #1
0041eb20: str      r2, [sp, #0x20]
0041eb24: bl       #0x3139ac
0041eb28: b        #0x41e8f8
0041eb2c: mov      r0, r5
0041eb30: bl       #0x3bd120
0041eb34: cmn      r0, #1
0041eb38: mov      r2, r0
0041eb3c: beq      #0x41e47c
0041eb40: ldr      r1, [pc, #0xb0]
0041eb44: add      r7, sp, #0x98
0041eb48: mov      r0, r7
0041eb4c: add      r1, pc, r1
0041eb50: bl       #0x30eae4
0041eb54: b        #0x41e490
0041eb58: ldr      r3, [r4]
0041eb5c: cmp      r3, #0
0041eb60: beq      #0x41e5c0
0041eb64: mov      r5, #0
0041eb68: mov      r6, r4
0041eb6c: str      r5, [r6], #0x39c
0041eb70: mov      r0, r6
0041eb74: bl       #0x427d50
0041eb78: strb     r5, [r0, #0x9b]
0041eb7c: add      r0, r4, #0x420
0041eb80: add      r0, r0, #0xc
0041eb84: ldr      r7, [r4, #0x57c]
0041eb88: bl       #0x427d50
0041eb8c: mov      r2, r5
0041eb90: mov      r1, r0
0041eb94: mov      r3, r5
0041eb98: mov      r0, r7
0041eb9c: bl       #0x7a7d34
0041eba0: mov      r0, r6
0041eba4: ldr      r4, [r4, #0x57c]
0041eba8: bl       #0x427d50
0041ebac: ldr      r2, [pc, #0x48]
0041ebb0: mov      r1, r0
0041ebb4: mov      r3, r5
0041ebb8: mov      r0, r4
0041ebbc: add      r2, pc, r2
0041ebc0: bl       #0x7ab924
0041ebc4: b        #0x41e5c0
0041ebc8: bl       #0x30e310
0041ebcc: subseq   r6, r7, ip, lsl sl
0041ebd0: andeq    r4, r0, ip, lsr #1
0041ebd4: strdeq   r3, r4, [r0], -r4
0041ebd8: subeq    r3, sl, r8, ror #27
0041ebdc: subeq    r3, sl, ip, ror #6
0041ebe0: subeq    sl, sl, ip, lsr r4
0041ebe4: andeq    r0, r0, r4, lsl #17
0041ebe8: umaaleq  sl, sl, r0, sb
0041ebec: subeq    r3, sl, ip, lsl #19
0041ebf0: strdeq   r3, r4, [sl], #-0x7c
0041ebf4: subeq    sl, sl, r4, asr #14
0041ebf8: subeq    r3, sl, r4, ror #6
0041ebfc: subeq    sb, sl, r4, lsr sp
