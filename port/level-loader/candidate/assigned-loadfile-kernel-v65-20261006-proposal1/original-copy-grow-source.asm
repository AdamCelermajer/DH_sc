
# _ZN6Buffer7setSizeEj 0x317014 size=364
00317014: push {r4, r5, r6, lr}
00317018: mov r4, r0
0031701c: ldr r2, [r4, #4]
00317020: ldr r0, [r0, #8]
00317024: ldr r3, [pc, #0x13c]
00317028: sub sp, sp, #0x18
0031702c: rsb r2, r2, r0
00317030: lsrs r2, r2, #2
00317034: mov r5, r1
00317038: add r3, pc, r3
0031703c: beq #0x317064
00317040: ldr r2, [pc, #0x124]
00317044: ldr r2, [r3, r2]
00317048: ldr r2, [r2]
0031704c: cmp r2, #2
00317050: moveq r3, #0
00317054: streq r3, [r3]
00317058: beq #0x317064
0031705c: cmp r2, #1
00317060: beq #0x3170e0
00317064: add r6, r4, #4
00317068: mov r1, #2
0031706c: mov r0, r6
00317070: bl #0x316874
00317074: mov r1, #0
00317078: mov r0, r5
0031707c: bl #0x31056c
00317080: ldr r1, [r4, #8]
00317084: ldr r3, [r4, #0xc]
00317088: str r0, [sp, #0xc]
0031708c: cmp r1, r3
00317090: beq #0x317114
00317094: str r0, [r1]
00317098: ldr r1, [r4, #8]
0031709c: ldr r2, [r4, #0xc]
003170a0: mov r3, #0
003170a4: add r1, r1, #4
003170a8: cmp r1, r2
003170ac: str r1, [r4, #8]
003170b0: str r3, [sp, #8]
003170b4: beq #0x317148
003170b8: str r3, [r1]
003170bc: ldr r3, [r4, #8]
003170c0: add r3, r3, #4
003170c4: str r3, [r4, #8]
003170c8: mov r3, #1
003170cc: strb r3, [r4, #0x14]
003170d0: str r5, [r4]
003170d4: str r5, [r4, #0x10]
003170d8: add sp, sp, #0x18
003170dc: pop {r4, r5, r6, pc}
003170e0: ldr r0, [pc, #0x88]
003170e4: ldr r1, [pc, #0x88]
003170e8: ldr r2, [pc, #0x88]
003170ec: ldr r0, [r3, r0]
003170f0: ldr r3, [pc, #0x84]
003170f4: mov ip, #0x75
003170f8: add r1, pc, r1
003170fc: add r2, pc, r2
00317100: add r3, pc, r3
00317104: add r0, r0, #0xa8
00317108: str ip, [sp]
0031710c: bl #0x30e004
00317110: b #0x317064
00317114: mov ip, #1
00317118: add r2, sp, #0xc
0031711c: add r3, sp, #0x14
00317120: mov r0, r6
00317124: str ip, [sp, #4]
00317128: str ip, [sp]
0031712c: bl #0x316a6c
00317130: ldr r1, [r4, #8]
00317134: ldr r2, [r4, #0xc]
00317138: mov r3, #0
0031713c: str r3, [sp, #8]
00317140: cmp r1, r2
00317144: bne #0x3170b8
00317148: mov ip, #1
0031714c: mov r0, r6
00317150: add r2, sp, #8
00317154: add r3, sp, #0x10
00317158: str ip, [sp, #4]
0031715c: str ip, [sp]
00317160: bl #0x316a6c
00317164: b #0x3170c8
00317168: rsbeq sp, r7, r8, asr sl
0031716c: andeq r3, r0, r0, asr #19
00317170: andeq r1, r0, r0, asr #19
00317174: subseq r7, sl, r0, ror #5
00317178: subseq r7, sl, r4, lsl #11
0031717c: subseq r7, sl, r0, asr #10

# _ZN6Buffer5clearEv 0x31692c size=92
0031692c: push {r4, r5, r6, lr}
00316930: ldr r4, [r0, #4]
00316934: ldr r3, [r0, #8]
00316938: mov r5, r0
0031693c: cmp r4, r3
00316940: beq #0x316970
00316944: ldr r0, [r4]
00316948: add r4, r4, #4
0031694c: cmp r0, #0
00316950: beq #0x31695c
00316954: bl #0x310440
00316958: ldr r3, [r5, #8]
0031695c: cmp r4, r3
00316960: bne #0x316944
00316964: ldr r3, [r5, #4]
00316968: cmp r4, r3
0031696c: strne r3, [r5, #8]
00316970: mov r3, #0
00316974: mov r2, #0x800
00316978: strb r3, [r5, #0x14]
0031697c: str r2, [r5]
00316980: str r3, [r5, #0x10]
00316984: pop {r4, r5, r6, pc}

# _ZNSt6vectorIPhSaIS0_EE7reserveEj 0x316874 size=184
00316874: push {r4, r5, r6, lr}
00316878: mov r4, r0
0031687c: ldr r2, [r0]
00316880: ldr r0, [r0, #8]
00316884: sub sp, sp, #8
00316888: str r1, [sp, #4]
0031688c: rsb r0, r2, r0
00316890: cmp r1, r0, asr #2
00316894: bls #0x3168f4
00316898: cmn r1, #0xc0000001
0031689c: bhi #0x3168fc
003168a0: ldr r3, [r4, #4]
003168a4: cmp r2, #0
003168a8: rsb r5, r2, r3
003168ac: asr r5, r5, #2
003168b0: beq #0x316910
003168b4: add r1, sp, #4
003168b8: mov r0, r4
003168bc: bl #0x316838
003168c0: mov r6, r0
003168c4: mov r0, r4
003168c8: ldr r1, [r0], #8
003168cc: ldr r2, [r4, #8]
003168d0: rsb r2, r1, r2
003168d4: asr r2, r2, #2
003168d8: bl #0x31677c
003168dc: ldr r3, [sp, #4]
003168e0: add r5, r6, r5, lsl #2
003168e4: str r5, [r4, #4]
003168e8: add r3, r6, r3, lsl #2
003168ec: str r3, [r4, #8]
003168f0: str r6, [r4]
003168f4: add sp, sp, #8
003168f8: pop {r4, r5, r6, pc}
003168fc: ldr r0, [pc, #0x24]
00316900: add r0, pc, r0
00316904: bl #0x708e40
00316908: ldr r2, [r4]
0031690c: b #0x3168a0
00316910: add r2, sp, #8
00316914: ldr r1, [r2, #-4]!
00316918: add r0, r4, #8
0031691c: bl #0x3167cc
00316920: mov r6, r0
00316924: b #0x3168dc
00316928: subseq r7, sl, r8, ror #22

# _ZNSt6vectorIPhSaIS0_EE18_M_insert_overflowEPS0_RKS0_RKSt11__true_typejb 0x316a6c size=240
00316a6c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00316a70: sub sp, sp, #0xc
00316a74: ldr r6, [sp, #0x30]
00316a78: mov sb, r1
00316a7c: mov r5, r0
00316a80: mov r1, r6
00316a84: mov r4, r2
00316a88: ldrb fp, [sp, #0x34]
00316a8c: bl #0x31671c
00316a90: add sl, r5, #8
00316a94: add r2, sp, #8
00316a98: mov r1, r0
00316a9c: str r0, [r2, #-4]!
00316aa0: mov r0, sl
00316aa4: bl #0x3167cc
00316aa8: ldr r1, [r5]
00316aac: mov r8, r0
00316ab0: subs r7, sb, r1
00316ab4: moveq r0, r0
00316ab8: bne #0x316b34
00316abc: cmp r6, #0
00316ac0: mov r7, r0
00316ac4: beq #0x316ae8
00316ac8: mov r2, r6
00316acc: mov r3, #0
00316ad0: ldr r1, [r4]
00316ad4: subs r2, r2, #1
00316ad8: str r1, [r0, r3]
00316adc: add r3, r3, #4
00316ae0: bne #0x316ad0
00316ae4: add r7, r0, r6, lsl #2
00316ae8: cmp fp, #0
00316aec: bne #0x316afc
00316af0: ldr r4, [r5, #4]
00316af4: subs r4, r4, sb
00316af8: bne #0x316b44
00316afc: ldr r3, [r5]
00316b00: ldr r2, [r5, #8]
00316b04: mov r0, sl
00316b08: mov r1, r3
00316b0c: rsb r3, r3, r2
00316b10: asr r2, r3, #2
00316b14: bl #0x31677c
00316b18: ldr r3, [sp, #4]
00316b1c: str r8, [r5]
00316b20: str r7, [r5, #4]
00316b24: add r8, r8, r3, lsl #2
00316b28: str r8, [r5, #8]
00316b2c: add sp, sp, #0xc
00316b30: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00316b34: mov r2, r7
00316b38: bl #0x30df38
00316b3c: add r0, r0, r7
00316b40: b #0x316abc
00316b44: mov r0, r7
00316b48: mov r1, sb
00316b4c: mov r2, r4
00316b50: bl #0x30df38
00316b54: add r7, r0, r4
00316b58: b #0x316afc

# _ZNSt6vectorIPhSaIS0_EE20_M_allocate_and_copyIPS0_EES4_RjT_S6_ 0x316838 size=60
00316838: push {r4, r5, r6, lr}
0031683c: add r0, r0, #8
00316840: mov r4, r2
00316844: mov r2, r1
00316848: ldr r1, [r1]
0031684c: mov r5, r3
00316850: bl #0x3167cc
00316854: cmp r4, r5
00316858: mov r6, r0
0031685c: beq #0x31686c
00316860: mov r1, r4
00316864: rsb r2, r4, r5
00316868: bl #0x30e868
0031686c: mov r0, r6
00316870: pop {r4, r5, r6, pc}

# _ZNSaIPhE10deallocateEPS_j 0x31677c size=28
0031677c: subs r0, r1, #0
00316780: bxeq lr
00316784: lsl r1, r2, #2
00316788: cmp r1, #0x80
0031678c: bhi #0x316794
00316790: b #0x708f00
00316794: b #0x310440

# _ZNSaIPhE11_M_allocateEjRj 0x3167cc size=108
003167cc: push {r4, lr}
003167d0: cmn r1, #0xc0000001
003167d4: sub sp, sp, #8
003167d8: mov r4, r2
003167dc: bhi #0x316820
003167e0: cmp r1, #0
003167e4: moveq r0, r1
003167e8: beq #0x316810
003167ec: lsl r0, r1, #2
003167f0: cmp r0, #0x80
003167f4: str r0, [sp, #4]
003167f8: bhi #0x316818
003167fc: add r0, sp, #4
00316800: bl #0x708ec0
00316804: ldr r3, [sp, #4]
00316808: lsr r3, r3, #2
0031680c: str r3, [r4]
00316810: add sp, sp, #8
00316814: pop {r4, pc}
00316818: bl #0x310454
0031681c: b #0x316804
00316820: ldr r0, [pc, #0xc]
00316824: add r0, pc, r0
00316828: bl #0x30e0c4
0031682c: mov r0, #1
00316830: bl #0x30de48
00316834: subseq r7, sl, ip, asr #24

# _ZNSt6vectorIPhSaIS0_EE20_M_compute_next_sizeEj 0x31671c size=96
0031671c: push {r4, r5, r6, lr}
00316720: ldm r0, {r2, r4}
00316724: movw r3, #0xffff
00316728: movt r3, #0x3fff
0031672c: rsb r4, r2, r4
00316730: asr r4, r4, #2
00316734: rsb r3, r4, r3
00316738: cmp r3, r1
0031673c: mov r5, r1
00316740: blo #0x316768
00316744: cmp r4, r5
00316748: addhs r0, r4, r4
0031674c: addlo r0, r4, r5
00316750: cmn r0, #0xc0000001
00316754: bhi #0x316760
00316758: cmp r0, r4
0031675c: bhs #0x316764
00316760: mvn r0, #0xc0000000
00316764: pop {r4, r5, r6, pc}
00316768: ldr r0, [pc, #8]
0031676c: add r0, pc, r0
00316770: bl #0x708e40
00316774: b #0x316744
00316778: ldrsheq r7, [sl], #-0xcc
