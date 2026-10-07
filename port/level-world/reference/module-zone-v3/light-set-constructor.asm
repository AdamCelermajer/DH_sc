# _ZN15LightSetManagerC2Ev
0040daa8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0040daac: ldr r5, [pc, #0x2b4]
0040dab0: add r3, r0, #4
0040dab4: sub sp, sp, #0xc
0040dab8: mov r7, #0
0040dabc: mov r4, r0
0040dac0: str r3, [sp, #4]
0040dac4: mov r6, r3
0040dac8: mov r8, r7
0040dacc: add r5, pc, r5
0040dad0: str r6, [r6, #0x10]
0040dad4: str r6, [r6, #0x14]
0040dad8: mov r0, r6
0040dadc: mov r1, #0x10
0040dae0: bl #0x31167c
0040dae4: ldr r3, [r6, #0x10]
0040dae8: add r7, r7, #0x18
0040daec: cmp r7, #0x60
0040daf0: strb r8, [r3]
0040daf4: add r6, r6, #0x18
0040daf8: bne #0x40dad0
0040dafc: add r3, r4, #0x78
0040db00: add r1, r4, #0xc8
0040db04: mov r2, #0
0040db08: str r2, [r3]
0040db0c: str r2, [r3, #4]
0040db10: str r2, [r3, #8]
0040db14: str r2, [r3, #0xc]
0040db18: str r2, [r3, #0x10]
0040db1c: add r3, r3, #0x14
0040db20: cmp r3, r1
0040db24: bne #0x40db08
0040db28: ldr r0, [pc, #0x23c]
0040db2c: mov r1, r2
0040db30: add r3, r4, #0xd4
0040db34: ldr r0, [r5, r0]
0040db38: str r2, [r4, #0xc8]
0040db3c: str r2, [r4, #0xcc]
0040db40: str r2, [r4, #0xd0]
0040db44: str r2, [r4, #0xd4]
0040db48: add r0, r0, #8
0040db4c: str r2, [r3, #4]
0040db50: mov r7, r1
0040db54: add r2, r4, #0xdc
0040db58: mov r3, r2
0040db5c: str r0, [r3, r1]!
0040db60: add r1, r1, #8
0040db64: cmp r1, #0x40
0040db68: str r7, [r3, #4]
0040db6c: bne #0x40db58
0040db70: ldr r3, [pc, #0x1f8]
0040db74: add r6, r4, #0x11c
0040db78: add sl, r4, #0x16c
0040db7c: ldr r8, [r5, r3]
0040db80: add r8, r8, #8
0040db84: str r8, [r6]
0040db88: str r7, [r6, #4]
0040db8c: add r0, r6, #8
0040db90: mov r1, r7
0040db94: add r6, r6, #0x14
0040db98: bl #0x33f524
0040db9c: cmp r6, sl
0040dba0: bne #0x40db84
0040dba4: mov r3, #0
0040dba8: str r3, [r4, #0x184]
0040dbac: str r3, [r4, #0x178]
0040dbb0: str r3, [r4, #0x17c]
0040dbb4: str r3, [r4, #0x180]
0040dbb8: ldr r3, [pc, #0x1b4]
0040dbbc: movw r2, #0x2400
0040dbc0: movt r2, #0xc974
0040dbc4: mov r1, #1
0040dbc8: str r2, [r4, #0x174]
0040dbcc: strb r1, [r4, #0x19d]
0040dbd0: str r2, [r4, #0x16c]
0040dbd4: str r2, [r4, #0x170]
0040dbd8: str r7, [r4, #0x188]
0040dbdc: str r7, [r4, #0x18c]
0040dbe0: str r7, [r4, #0x190]
0040dbe4: str r7, [r4, #0x194]
0040dbe8: str r7, [r4, #0x198]
0040dbec: strb r1, [r4, #0x19c]
0040dbf0: strb r7, [r4, #0x19e]
0040dbf4: str r7, [r4, #0x1a0]
0040dbf8: mov sb, r4
0040dbfc: str r4, [sp]
0040dc00: mov fp, r7
0040dc04: mov sl, r7
0040dc08: mov r4, r3
0040dc0c: mov r3, #0x14
0040dc10: mul r7, r3, fp
0040dc14: ldr r8, [sp]
0040dc18: add r7, r7, #0x78
0040dc1c: mov r6, #0
0040dc20: add r7, r8, r7
0040dc24: ldr r0, [r7]
0040dc28: str sl, [r7]
0040dc2c: cmp r0, #0
0040dc30: beq #0x40dc6c
0040dc34: ldr r3, [r0]
0040dc38: sub r3, r3, #1
0040dc3c: cmp r3, #0
0040dc40: str r3, [r0]
0040dc44: bne #0x40dc6c
0040dc48: ldrb r3, [r0, #0x54]
0040dc4c: cmp r3, #0
0040dc50: ldreq r3, [r5, r4]
0040dc54: ldreq r2, [r0, #0x50]
0040dc58: ldreq r1, [r3]
0040dc5c: streq r1, [r2]
0040dc60: streq r2, [r3]
0040dc64: str sl, [r0, #0x50]
0040dc68: bl #0x310440
0040dc6c: add r3, sb, r6
0040dc70: strb sl, [r3, #0x64]
0040dc74: ldr r0, [r8, #0xc8]
0040dc78: str sl, [r8, #0xc8]
0040dc7c: cmp r0, #0
0040dc80: beq #0x40dcbc
0040dc84: ldr r3, [r0]
0040dc88: sub r3, r3, #1
0040dc8c: cmp r3, #0
0040dc90: str r3, [r0]
0040dc94: bne #0x40dcbc
0040dc98: ldrb r3, [r0, #0x54]
0040dc9c: cmp r3, #0
0040dca0: ldreq r3, [r5, r4]
0040dca4: ldreq r2, [r0, #0x50]
0040dca8: ldreq r1, [r3]
0040dcac: streq r1, [r2]
0040dcb0: streq r2, [r3]
0040dcb4: str sl, [r0, #0x50]
0040dcb8: bl #0x310440
0040dcbc: add r6, r6, #1
0040dcc0: cmp r6, #5
0040dcc4: add r8, r8, #4
0040dcc8: add r7, r7, #4
0040dccc: bne #0x40dc24
0040dcd0: add fp, fp, #1
0040dcd4: cmp fp, #4
0040dcd8: add sb, sb, #5
0040dcdc: bne #0x40dc0c
0040dce0: ldr r3, [pc, #0x90]
0040dce4: ldr r4, [sp]
0040dce8: ldr r5, [r5, r3]
0040dcec: ldr r6, [r5]
0040dcf0: mov r0, r6
0040dcf4: bl #0x30de54
0040dcf8: mov r1, r6
0040dcfc: add r2, r6, r0
0040dd00: ldr r0, [sp, #4]
0040dd04: bl #0x3109e0
0040dd08: ldr r6, [r5, #4]
0040dd0c: mov r0, r6
0040dd10: bl #0x30de54
0040dd14: mov r1, r6
0040dd18: add r2, r6, r0
0040dd1c: add r0, r4, #0x1c
0040dd20: bl #0x3109e0
0040dd24: ldr r6, [r5, #8]
0040dd28: mov r0, r6
0040dd2c: bl #0x30de54
0040dd30: mov r1, r6
0040dd34: add r2, r6, r0
0040dd38: add r0, r4, #0x34
0040dd3c: bl #0x3109e0
0040dd40: ldr r5, [r5, #0xc]
0040dd44: mov r0, r5
0040dd48: bl #0x30de54
0040dd4c: mov r1, r5
0040dd50: add r2, r5, r0
0040dd54: add r0, r4, #0x4c
0040dd58: bl #0x3109e0
0040dd5c: mov r0, r4
0040dd60: add sp, sp, #0xc
0040dd64: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0040dd68: subseq r6, r8, r4, asr #31
0040dd6c: strheq r4, [r0], -r8
0040dd70: andeq r3, r0, r0, lsr #13
0040dd74: andeq r3, r0, r0, asr #25
0040dd78: andeq r0, r0, r4, lsr #23
# _ZN15LightSetManagerC1Ev
0040d7d4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0040d7d8: ldr r5, [pc, #0x2b4]
0040d7dc: add r3, r0, #4
0040d7e0: sub sp, sp, #0xc
0040d7e4: mov r7, #0
0040d7e8: mov r4, r0
0040d7ec: str r3, [sp, #4]
0040d7f0: mov r6, r3
0040d7f4: mov r8, r7
0040d7f8: add r5, pc, r5
0040d7fc: str r6, [r6, #0x10]
0040d800: str r6, [r6, #0x14]
0040d804: mov r0, r6
0040d808: mov r1, #0x10
0040d80c: bl #0x31167c
0040d810: ldr r3, [r6, #0x10]
0040d814: add r7, r7, #0x18
0040d818: cmp r7, #0x60
0040d81c: strb r8, [r3]
0040d820: add r6, r6, #0x18
0040d824: bne #0x40d7fc
0040d828: add r3, r4, #0x78
0040d82c: add r1, r4, #0xc8
0040d830: mov r2, #0
0040d834: str r2, [r3]
0040d838: str r2, [r3, #4]
0040d83c: str r2, [r3, #8]
0040d840: str r2, [r3, #0xc]
0040d844: str r2, [r3, #0x10]
0040d848: add r3, r3, #0x14
0040d84c: cmp r3, r1
0040d850: bne #0x40d834
0040d854: ldr r0, [pc, #0x23c]
0040d858: mov r1, r2
0040d85c: add r3, r4, #0xd4
0040d860: ldr r0, [r5, r0]
0040d864: str r2, [r4, #0xc8]
0040d868: str r2, [r4, #0xcc]
0040d86c: str r2, [r4, #0xd0]
0040d870: str r2, [r4, #0xd4]
0040d874: add r0, r0, #8
0040d878: str r2, [r3, #4]
0040d87c: mov r7, r1
0040d880: add r2, r4, #0xdc
0040d884: mov r3, r2
0040d888: str r0, [r3, r1]!
0040d88c: add r1, r1, #8
0040d890: cmp r1, #0x40
0040d894: str r7, [r3, #4]
0040d898: bne #0x40d884
0040d89c: ldr r3, [pc, #0x1f8]
0040d8a0: add r6, r4, #0x11c
0040d8a4: add sl, r4, #0x16c
0040d8a8: ldr r8, [r5, r3]
0040d8ac: add r8, r8, #8
0040d8b0: str r8, [r6]
0040d8b4: str r7, [r6, #4]
0040d8b8: add r0, r6, #8
0040d8bc: mov r1, r7
0040d8c0: add r6, r6, #0x14
0040d8c4: bl #0x33f524
0040d8c8: cmp r6, sl
0040d8cc: bne #0x40d8b0
0040d8d0: mov r3, #0
0040d8d4: str r3, [r4, #0x184]
0040d8d8: str r3, [r4, #0x178]
0040d8dc: str r3, [r4, #0x17c]
0040d8e0: str r3, [r4, #0x180]
0040d8e4: ldr r3, [pc, #0x1b4]
0040d8e8: movw r2, #0x2400
0040d8ec: movt r2, #0xc974
0040d8f0: mov r1, #1
0040d8f4: str r2, [r4, #0x174]
0040d8f8: strb r1, [r4, #0x19d]
0040d8fc: str r2, [r4, #0x16c]
0040d900: str r2, [r4, #0x170]
0040d904: str r7, [r4, #0x188]
0040d908: str r7, [r4, #0x18c]
0040d90c: str r7, [r4, #0x190]
0040d910: str r7, [r4, #0x194]
0040d914: str r7, [r4, #0x198]
0040d918: strb r1, [r4, #0x19c]
0040d91c: strb r7, [r4, #0x19e]
0040d920: str r7, [r4, #0x1a0]
0040d924: mov sb, r4
0040d928: str r4, [sp]
0040d92c: mov fp, r7
0040d930: mov sl, r7
0040d934: mov r4, r3
0040d938: mov r3, #0x14
0040d93c: mul r7, r3, fp
0040d940: ldr r8, [sp]
0040d944: add r7, r7, #0x78
0040d948: mov r6, #0
0040d94c: add r7, r8, r7
0040d950: ldr r0, [r7]
0040d954: str sl, [r7]
0040d958: cmp r0, #0
0040d95c: beq #0x40d998
0040d960: ldr r3, [r0]
0040d964: sub r3, r3, #1
0040d968: cmp r3, #0
0040d96c: str r3, [r0]
0040d970: bne #0x40d998
0040d974: ldrb r3, [r0, #0x54]
0040d978: cmp r3, #0
0040d97c: ldreq r3, [r5, r4]
0040d980: ldreq r2, [r0, #0x50]
0040d984: ldreq r1, [r3]
0040d988: streq r1, [r2]
0040d98c: streq r2, [r3]
0040d990: str sl, [r0, #0x50]
0040d994: bl #0x310440
0040d998: add r3, sb, r6
0040d99c: strb sl, [r3, #0x64]
0040d9a0: ldr r0, [r8, #0xc8]
0040d9a4: str sl, [r8, #0xc8]
0040d9a8: cmp r0, #0
0040d9ac: beq #0x40d9e8
0040d9b0: ldr r3, [r0]
0040d9b4: sub r3, r3, #1
0040d9b8: cmp r3, #0
0040d9bc: str r3, [r0]
0040d9c0: bne #0x40d9e8
0040d9c4: ldrb r3, [r0, #0x54]
0040d9c8: cmp r3, #0
0040d9cc: ldreq r3, [r5, r4]
0040d9d0: ldreq r2, [r0, #0x50]
0040d9d4: ldreq r1, [r3]
0040d9d8: streq r1, [r2]
0040d9dc: streq r2, [r3]
0040d9e0: str sl, [r0, #0x50]
0040d9e4: bl #0x310440
0040d9e8: add r6, r6, #1
0040d9ec: cmp r6, #5
0040d9f0: add r8, r8, #4
0040d9f4: add r7, r7, #4
0040d9f8: bne #0x40d950
0040d9fc: add fp, fp, #1
0040da00: cmp fp, #4
0040da04: add sb, sb, #5
0040da08: bne #0x40d938
0040da0c: ldr r3, [pc, #0x90]
0040da10: ldr r4, [sp]
0040da14: ldr r5, [r5, r3]
0040da18: ldr r6, [r5]
0040da1c: mov r0, r6
0040da20: bl #0x30de54
0040da24: mov r1, r6
0040da28: add r2, r6, r0
0040da2c: ldr r0, [sp, #4]
0040da30: bl #0x3109e0
0040da34: ldr r6, [r5, #4]
0040da38: mov r0, r6
0040da3c: bl #0x30de54
0040da40: mov r1, r6
0040da44: add r2, r6, r0
0040da48: add r0, r4, #0x1c
0040da4c: bl #0x3109e0
0040da50: ldr r6, [r5, #8]
0040da54: mov r0, r6
0040da58: bl #0x30de54
0040da5c: mov r1, r6
0040da60: add r2, r6, r0
0040da64: add r0, r4, #0x34
0040da68: bl #0x3109e0
0040da6c: ldr r5, [r5, #0xc]
0040da70: mov r0, r5
0040da74: bl #0x30de54
0040da78: mov r1, r5
0040da7c: add r2, r5, r0
0040da80: add r0, r4, #0x4c
0040da84: bl #0x3109e0
0040da88: mov r0, r4
0040da8c: add sp, sp, #0xc
0040da90: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
