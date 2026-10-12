
# _ZN16PropertyInstance8ToStringEv 0x510fe0 size=92
00510fe0: push {r4, lr}
00510fe4: ldr r2, [r1, #4]
00510fe8: sub sp, sp, #8
00510fec: mov r4, r0
00510ff0: cmp r2, #0
00510ff4: beq #0x511020
00510ff8: ldr r3, [r1]
00510ffc: cmp r3, #0
00511000: beq #0x511020
00511004: mov r1, r3
00511008: ldr r3, [r3]
0051100c: mov lr, pc
00511010: ldr pc, [r3]
00511014: mov r0, r4
00511018: add sp, sp, #8
0051101c: pop {r4, pc}
00511020: ldr r1, [pc, #0x10]
00511024: mov r0, r4
00511028: add r2, sp, #4
0051102c: add r1, pc, r1
00511030: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00511034: b #0x511014
00511038: ldrsbteq sl, [fp], -ip

# _ZN18SimpleTypePropertyISsE8ToStringEPv 0x33e218 size=44
0033e218: push {r4, lr}
0033e21c: ldr r3, [r1, #4]
0033e220: mov r4, r0
0033e224: str r0, [r4, #0x10]
0033e228: str r0, [r4, #0x14]
0033e22c: add r3, r2, r3
0033e230: ldr r2, [r3, #0x10]
0033e234: ldr r1, [r3, #0x14]
0033e238: bl #0x3116e8 ; _ZNSs19_M_range_initializeEPKcS0_
0033e23c: mov r0, r4
0033e240: pop {r4, pc}

# _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE8ToStringEPv 0x3f3858 size=72
003f3858: push {r4, r5, r6, r7, lr}
003f385c: mov r4, r0
003f3860: sub sp, sp, #0xc
003f3864: mov r0, #0x100
003f3868: mov r7, r2
003f386c: mov r6, r1
003f3870: bl #0x310454 ; _Z11CustomAllocj
003f3874: ldr r1, [r6, #4]
003f3878: mov r5, r0
003f387c: add r1, r7, r1
003f3880: bl #0x30ee60 ; _Z8ObjToStrPcRSt6vectorI7Point3DIfESaIS2_EE
003f3884: mov r0, r4
003f3888: mov r1, r5
003f388c: add r2, sp, #4
003f3890: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f3894: mov r0, r4
003f3898: add sp, sp, #0xc
003f389c: pop {r4, r5, r6, r7, pc}

# _ZN18SimpleTypePropertyI7Point3DIfEE8ToStringEPv 0x389980 size=72
00389980: push {r4, r5, r6, r7, lr}
00389984: mov r4, r0
00389988: sub sp, sp, #0xc
0038998c: mov r0, #0x100
00389990: mov r7, r2
00389994: mov r6, r1
00389998: bl #0x310454 ; _Z11CustomAllocj
0038999c: ldr r1, [r6, #4]
003899a0: mov r5, r0
003899a4: add r1, r7, r1
003899a8: bl #0x30efb0 ; _Z8ObjToStrPcR7Point3DIfE
003899ac: mov r0, r4
003899b0: mov r1, r5
003899b4: add r2, sp, #4
003899b8: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003899bc: mov r0, r4
003899c0: add sp, sp, #0xc
003899c4: pop {r4, r5, r6, r7, pc}

# _Z8ObjToStrPcRf 0x30eec8 size=52
0030eec8: push {r4, r5, r6, lr}
0030eecc: mov r5, r0
0030eed0: ldr r0, [r1]
0030eed4: bl #0x30e8a4 ; 
0030eed8: ldr r4, [pc, #0x18]
0030eedc: mov r2, r0
0030eee0: mov r3, r1
0030eee4: add r4, pc, r4
0030eee8: mov r0, r5
0030eeec: mov r1, r4
0030eef0: pop {r4, r5, r6, lr}
0030eef4: b #0x30eae4
0030eef8: subseq pc, sl, ip, ror r4

# _Z8ObjToStrPcR7Point3DIfE 0x30efb0 size=96
0030efb0: push {r4, r5, r6, r7, lr}
0030efb4: mov r5, r0
0030efb8: sub sp, sp, #0x14
0030efbc: ldr r0, [r1]
0030efc0: mov r4, r1
0030efc4: bl #0x30e8a4 ; 
0030efc8: mov r6, r0
0030efcc: ldr r0, [r4, #4]
0030efd0: mov r7, r1
0030efd4: bl #0x30e8a4 ; 
0030efd8: strd r0, r1, [sp]
0030efdc: ldr r0, [r4, #8]
0030efe0: bl #0x30e8a4 ; 
0030efe4: ldr r3, [pc, #0x20]
0030efe8: strd r0, r1, [sp, #8]
0030efec: add r3, pc, r3
0030eff0: mov r1, r3
0030eff4: mov r0, r5
0030eff8: mov r2, r6
0030effc: mov r3, r7
0030f000: bl #0x30eae4 ; 
0030f004: add sp, sp, #0x14
0030f008: pop {r4, r5, r6, r7, pc}
0030f00c: subseq pc, sl, r4, lsl #7

# _Z8ObjToStrPcRi 0x30eea0 size=20
0030eea0: ldr r2, [r1]
0030eea4: ldr r1, [pc, #4]
0030eea8: add r1, pc, r1
0030eeac: b #0x30eae4
0030eeb0: subseq r3, fp, r8

# _Z8ObjToStrPcRb 0x30eeb4 size=20
0030eeb4: ldrb r2, [r1]
0030eeb8: ldr r1, [pc, #4]
0030eebc: add r1, pc, r1
0030eec0: b #0x30eae4
0030eec4: ldrsheq r2, [fp], #-0xf4

# _Z8ObjToStrPcRSt6vectorI7Point3DIfESaIS2_EE 0x30ee60 size=4
0030ee60: bx lr
