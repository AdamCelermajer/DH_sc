
# _ZN8Savegame3Job6finishEv 00313720 size64
00313720: push {r4, lr}
00313724: ldrb r3, [r0, #0x1d]
00313728: mov r4, r0
0031372c: cmp r3, #0
00313730: beq #0x313750
00313734: ldr r3, [r0]
00313738: cmp r3, #0
0031373c: beq #0x313750
00313740: mov r0, r3
00313744: ldr r3, [r3]
00313748: mov lr, pc
0031374c: ldr pc, [r3, #4]
00313750: mov r3, #0
00313754: strb r3, [r4, #0x1d]
00313758: str r3, [r4]
0031375c: pop {r4, pc}

# _ZN8Savegame3JobD1Ev 00313c90 size76
00313c90: push {r4, lr}
00313c94: ldrb r3, [r0, #0x1d]
00313c98: mov r4, r0
00313c9c: cmp r3, #0
00313ca0: beq #0x313cc0
00313ca4: ldr r3, [r0]
00313ca8: cmp r3, #0
00313cac: beq #0x313cc0
00313cb0: mov r0, r3
00313cb4: ldr r3, [r3]
00313cb8: mov lr, pc
00313cbc: ldr pc, [r3, #4]
00313cc0: mov r3, #0
00313cc4: add r0, r4, #4
00313cc8: strb r3, [r4, #0x1d]
00313ccc: str r3, [r4]
00313cd0: bl #0x3139ac
00313cd4: mov r0, r4
00313cd8: pop {r4, pc}

# _ZN8Savegame3Job4copyERKS0_ 00313cdc size128
00313cdc: push {r4, r5, r6, lr}
00313ce0: ldrb r3, [r0, #0x1d]
00313ce4: mov r4, r0
00313ce8: mov r5, r1
00313cec: cmp r3, #0
00313cf0: beq #0x313d10
00313cf4: ldr r3, [r0]
00313cf8: cmp r3, #0
00313cfc: beq #0x313d10
00313d00: mov r0, r3
00313d04: ldr r3, [r3]
00313d08: mov lr, pc
00313d0c: ldr pc, [r3, #4]
00313d10: mov r3, #0
00313d14: strb r3, [r4, #0x1d]
00313d18: str r3, [r4]
00313d1c: mov r3, r5
00313d20: ldr r2, [r3], #4
00313d24: mov r0, r4
00313d28: str r2, [r0], #4
00313d2c: cmp r0, r3
00313d30: beq #0x313d40
00313d34: ldr r1, [r5, #0x18]
00313d38: ldr r2, [r5, #0x14]
00313d3c: bl #0x3109e0
00313d40: ldrb r3, [r5, #0x1d]
00313d44: strb r3, [r4, #0x1d]
00313d48: ldrb r3, [r5, #0x1c]
00313d4c: strb r3, [r4, #0x1c]
00313d50: mov r3, #0
00313d54: strb r3, [r5, #0x1d]
00313d58: pop {r4, r5, r6, pc}

# _ZN8SavegameD1Ev 00313dfc size140
00313dfc: push {r4, r5, r6, lr}
00313e00: ldr r3, [pc, #0x78]
00313e04: ldr r2, [pc, #0x78]
00313e08: ldr r1, [r0, #0x1c]
00313e0c: add r3, pc, r3
00313e10: ldr r2, [r3, r2]
00313e14: cmp r1, #0
00313e18: mov r4, r0
00313e1c: add r2, r2, #8
00313e20: str r2, [r0]
00313e24: beq #0x313e40
00313e28: ldr r3, [r1]
00313e2c: mov r0, r1
00313e30: mov lr, pc
00313e34: ldr pc, [r3, #4]
00313e38: mov r3, #0
00313e3c: str r3, [r4, #0x1c]
00313e40: ldr r3, [r4, #0x30]
00313e44: cmp r3, #0
00313e48: beq #0x313e70
00313e4c: add r5, r4, #0x20
00313e50: mov r0, r5
00313e54: ldr r1, [r4, #0x24]
00313e58: bl #0x313dbc
00313e5c: mov r3, #0
00313e60: str r5, [r4, #0x2c]
00313e64: str r3, [r4, #0x30]
00313e68: str r5, [r4, #0x28]
00313e6c: str r3, [r4, #0x24]
00313e70: add r0, r4, #4
00313e74: bl #0x3139ac
00313e78: mov r0, r4
00313e7c: pop {r4, r5, r6, pc}
00313e80: rsbeq r0, r8, r4, lsl #25
00313e84: strheq r0, [r0], -r8

# _ZN8SavegameD0Ev 00313e88 size28
00313e88: push {r4, lr}
00313e8c: mov r4, r0
00313e90: bl #0x313dfc
00313e94: mov r0, r4
00313e98: bl #0x310440
00313e9c: mov r0, r4
00313ea0: pop {r4, pc}

# _ZN8SavegameD2Ev 00313ea4 size140
00313ea4: push {r4, r5, r6, lr}
00313ea8: ldr r3, [pc, #0x78]
00313eac: ldr r2, [pc, #0x78]
00313eb0: ldr r1, [r0, #0x1c]
00313eb4: add r3, pc, r3
00313eb8: ldr r2, [r3, r2]
00313ebc: cmp r1, #0
00313ec0: mov r4, r0
00313ec4: add r2, r2, #8
00313ec8: str r2, [r0]
00313ecc: beq #0x313ee8
00313ed0: ldr r3, [r1]
00313ed4: mov r0, r1
00313ed8: mov lr, pc
00313edc: ldr pc, [r3, #4]
00313ee0: mov r3, #0
00313ee4: str r3, [r4, #0x1c]
00313ee8: ldr r3, [r4, #0x30]
00313eec: cmp r3, #0
00313ef0: beq #0x313f18
00313ef4: add r5, r4, #0x20
00313ef8: mov r0, r5
00313efc: ldr r1, [r4, #0x24]
00313f00: bl #0x313dbc
00313f04: mov r3, #0
00313f08: str r5, [r4, #0x2c]
00313f0c: str r3, [r4, #0x30]
00313f10: str r5, [r4, #0x28]
00313f14: str r3, [r4, #0x24]
00313f18: add r0, r4, #4
00313f1c: bl #0x3139ac
00313f20: mov r0, r4
00313f24: pop {r4, r5, r6, pc}

# _ZN8Savegame18DeleteAllSlotFilesEi 00313fb0 size256
00313fb0: push {r4, r5, r6, r7, r8, sb, sl, lr}
00313fb4: ldr r4, [pc, #0xe0]
00313fb8: ldr r8, [pc, #0xe0]
00313fbc: sub sp, sp, #0x410
00313fc0: add r4, pc, r4
00313fc4: ldr r3, [r4, r8]
00313fc8: ldr r1, [pc, #0xd4]
00313fcc: ldr r2, [pc, #0xd4]
00313fd0: ldr ip, [r3]
00313fd4: add r6, sp, #0x10
00313fd8: sub r6, r6, #4
00313fdc: mov r3, r0
00313fe0: add r1, pc, r1
00313fe4: add r2, pc, r2
00313fe8: mov r0, r6
00313fec: str ip, [sp, #0x40c]
00313ff0: bl #0x30eae4
00313ff4: ldr r2, [pc, #0xb0]
00313ff8: mov r3, #0
00313ffc: str r3, [sp, #8]
00314000: ldr r7, [r4, r2]
00314004: str r3, [sp]
00314008: str r3, [sp, #4]
0031400c: ldr r3, [r7, #0x10]
00314010: mov r5, sp
00314014: ldr sb, [r3, #0x34]
00314018: ldr r3, [sb]
0031401c: mov r0, sb
00314020: ldr sl, [r3, #0x80]
00314024: mov lr, pc
00314028: ldr pc, [r3, #0x74]
0031402c: mov r2, r6
00314030: mov r1, r0
00314034: mov r3, sp
00314038: mov r0, sb
0031403c: blx sl
00314040: ldm sp, {r6, sl}
00314044: cmp sl, r6
00314048: beq #0x314074
0031404c: ldr r3, [r7, #0x10]
00314050: ldr r1, [r6, #0x14]
00314054: add r6, r6, #0x18
00314058: ldr r3, [r3, #0x34]
0031405c: mov r0, r3
00314060: ldr r3, [r3]
00314064: mov lr, pc
00314068: ldr pc, [r3, #0x9c]
0031406c: cmp r6, sl
00314070: bne #0x31404c
00314074: mov r0, sp
00314078: bl #0x313f30
0031407c: ldr r3, [r4, r8]
00314080: ldr r2, [sp, #0x40c]
00314084: ldr r3, [r3]
00314088: cmp r2, r3
0031408c: bne #0x314098
00314090: add sp, sp, #0x410
00314094: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00314098: bl #0x30e310

# _ZN8Savegame3JobC1ERKS0_ 003145e0 size72
003145e0: push {r4, r5, r6, lr}
003145e4: add r3, r0, #4
003145e8: mov r4, r0
003145ec: mov r5, r1
003145f0: mov r0, r3
003145f4: str r3, [r4, #0x14]
003145f8: str r3, [r4, #0x18]
003145fc: mov r1, #0x10
00314600: bl #0x31167c
00314604: ldr r2, [r4, #0x14]
00314608: mov r3, #0
0031460c: mov r0, r4
00314610: strb r3, [r2]
00314614: mov r1, r5
00314618: strb r3, [r4, #0x1d]
0031461c: bl #0x313cdc
00314620: mov r0, r4
00314624: pop {r4, r5, r6, pc}

# _ZN8Savegame10UpdateJobsEv 00314734 size1480
00314734: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00314738: ldr r4, [pc, #0x560]
0031473c: ldr r6, [pc, #0x560]
00314740: ldr sl, [pc, #0x560]
00314744: add r4, pc, r4
00314748: ldr r3, [r4, r6]
0031474c: add sl, pc, sl
00314750: ldrb r2, [sl, #0xc]
00314754: ldr r3, [r3]
00314758: sub sp, sp, #0x4c
0031475c: cmp r2, #0
00314760: str r3, [sp, #0x44]
00314764: movne r0, #0
00314768: bne #0x314818
0031476c: ldr r8, [pc, #0x538]
00314770: mov sb, #1
00314774: strb sb, [sl, #0xc]
00314778: ldr r7, [r4, r8]
0031477c: ldr r5, [r7]
00314780: cmp r5, #0
00314784: beq #0x314980
00314788: ldr r3, [pc, #0x520]
0031478c: add r3, pc, r3
00314790: ldr r1, [r3, #0x10]
00314794: cmp r1, #0
00314798: beq #0x31486c
0031479c: ldr r7, [pc, #0x510]
003147a0: ldr r3, [r5, #0x1c]
003147a4: mov r0, r5
003147a8: add r7, pc, r7
003147ac: ldr sl, [r7, #0x18]
003147b0: ldr sb, [r3, r1, lsl #2]
003147b4: ldr r3, [r5]
003147b8: ldr r2, [sl]
003147bc: ldr r5, [r2, #0x1c]
003147c0: mov lr, pc
003147c4: ldr pc, [r3, #0x34]
003147c8: mov r3, r1
003147cc: mov r2, r0
003147d0: mov r1, sb
003147d4: mov r0, sl
003147d8: blx r5
003147dc: ldr r3, [r7, #0x10]
003147e0: cmp r3, #0
003147e4: beq #0x314834
003147e8: ldr r5, [pc, #0x4c8]
003147ec: add r3, r3, #1
003147f0: add r5, pc, r5
003147f4: ldr r2, [r5, #0x14]
003147f8: str r3, [r5, #0x10]
003147fc: cmp r3, r2
00314800: beq #0x314924
00314804: mov r0, #1
00314808: ldr r3, [pc, #0x4ac]
0031480c: mov r2, #0
00314810: add r3, pc, r3
00314814: strb r2, [r3, #0xc]
00314818: ldr r3, [r4, r6]
0031481c: ldr r2, [sp, #0x44]
00314820: ldr r3, [r3]
00314824: cmp r2, r3
00314828: bne #0x314c68
0031482c: add sp, sp, #0x4c
00314830: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00314834: ldr r5, [r4, r8]
00314838: mov r3, #0
0031483c: mov r2, #0
00314840: ldr r1, [r5]
00314844: mov r0, r1
00314848: ldr r1, [r1]
0031484c: mov lr, pc
00314850: ldr pc, [r1, #0x2c]
00314854: ldr r1, [pc, #0x464]
00314858: ldr r0, [r5]
0031485c: add r1, pc, r1
00314860: bl #0x3139e0
00314864: ldr r3, [r7, #0x10]
00314868: b #0x3147e8
0031486c: mov r0, r5
00314870: mov r2, #0
00314874: mov r3, #0
00314878: ldr r1, [r5]
0031487c: mov lr, pc
00314880: ldr pc, [r1, #0x20]
00314884: ldr r1, [r4, r8]
00314888: mov r2, #4
0031488c: mov r3, #0
00314890: ldr ip, [r1]
00314894: ldr r1, [pc, #0x428]
00314898: mov r0, ip
0031489c: add r1, pc, r1
003148a0: ldr ip, [ip]
003148a4: mov lr, pc
003148a8: ldr pc, [ip, #0x18]
003148ac: cmp r0, #4
003148b0: beq #0x314b8c
003148b4: ldr r3, [pc, #0x40c]
003148b8: ldr r3, [r4, r3]
003148bc: ldr r3, [r3]
003148c0: cmp r3, #2
003148c4: moveq r3, #0
003148c8: streq r3, [r3]
003148cc: beq #0x3148d8
003148d0: cmp r3, #1
003148d4: beq #0x314b98
003148d8: ldr r5, [r4, r8]
003148dc: add r7, sp, #0x48
003148e0: mvn r3, #0
003148e4: ldr r1, [r5]
003148e8: str r3, [r7, #-0x24]!
003148ec: mov r2, #0
003148f0: mov r3, #0
003148f4: mov r0, r1
003148f8: ldr r1, [r1]
003148fc: mov lr, pc
00314900: ldr pc, [r1, #0x2c]
00314904: mov r1, r7
00314908: ldr r0, [r5]
0031490c: bl #0x3139e0
00314910: ldr r3, [pc, #0x3b4]
00314914: ldr r5, [r5]
00314918: add r3, pc, r3
0031491c: ldr r1, [r3, #0x10]
00314920: b #0x31479c
00314924: ldr r1, [r5, #0x18]
00314928: mov r3, #0
0031492c: mov r2, #0
00314930: mov r0, r1
00314934: ldr r1, [r1]
00314938: mov lr, pc
0031493c: ldr pc, [r1, #0x20]
00314940: ldr r1, [pc, #0x388]
00314944: ldr r0, [r5, #0x18]!
00314948: add r1, pc, r1
0031494c: bl #0x3139e0
00314950: ldr r3, [pc, #0x37c]
00314954: mov r1, r5
00314958: ldr r3, [r4, r3]
0031495c: ldr r3, [r3, #0x10]
00314960: ldr r3, [r3, #0x34]
00314964: mov r0, r3
00314968: ldr r3, [r3]
0031496c: mov lr, pc
00314970: ldr pc, [r3, #0x78]
00314974: ldr r0, [r4, r8]
00314978: bl #0x313720
0031497c: b #0x314804
00314980: ldr r3, [pc, #0x350]
00314984: ldr fp, [r4, r3]
00314988: ldr r1, [fp]
0031498c: cmp r1, fp
00314990: moveq r0, r5
00314994: beq #0x314808
00314998: add r1, r1, #8
0031499c: mov r0, r7
003149a0: bl #0x313cdc
003149a4: ldr r3, [fp]
003149a8: add r0, sp, #0x10
003149ac: add r1, sp, #0x20
003149b0: str r3, [sp, #0x20]
003149b4: bl #0x3140b0
003149b8: ldrb r3, [r7, #0x1c]
003149bc: cmp r3, #0
003149c0: beq #0x314b04
003149c4: mvn r3, #0
003149c8: str r3, [sp, #0x28]
003149cc: ldr r3, [pc, #0x300]
003149d0: ldr r1, [r7, #0x18]
003149d4: mov r2, r5
003149d8: ldr sl, [r4, r3]
003149dc: ldr r3, [sl, #0x10]
003149e0: ldr r3, [r3, #0x34]
003149e4: mov r0, r3
003149e8: ldr r3, [r3]
003149ec: mov lr, pc
003149f0: ldr pc, [r3, #0x94]
003149f4: cmp r0, #0
003149f8: str r0, [sp, #0x24]
003149fc: beq #0x314974
00314a00: add r1, sp, #0x28
00314a04: bl #0x313b48
00314a08: ldr r3, [sl, #0x10]
00314a0c: add r1, sp, #0x24
00314a10: ldr r3, [r3, #0x34]
00314a14: mov r0, r3
00314a18: ldr r3, [r3]
00314a1c: mov lr, pc
00314a20: ldr pc, [r3, #0x78]
00314a24: ldr r3, [sp, #0x28]
00314a28: cmn r3, #1
00314a2c: beq #0x314974
00314a30: ldr r3, [sl, #0x10]
00314a34: ldr fp, [r7, #0x18]
00314a38: ldr r1, [r7, #0x14]
00314a3c: ldr sb, [r3, #0x34]
00314a40: add sl, sp, #0x2c
00314a44: rsb r1, fp, r1
00314a48: ldr r3, [sb]
00314a4c: add r1, r1, #5
00314a50: mov r0, sl
00314a54: ldr r3, [r3, #0xa0]
00314a58: str sl, [sp, #0x3c]
00314a5c: str sl, [sp, #0x40]
00314a60: str r3, [sp, #8]
00314a64: bl #0x31167c
00314a68: ldr r3, [sp, #0x3c]
00314a6c: mov r0, sl
00314a70: strb r5, [r3]
00314a74: ldr r2, [r7, #0x14]
00314a78: ldr r1, [r7, #0x18]
00314a7c: bl #0x310804
00314a80: ldr r3, [sp, #0x40]
00314a84: cmp r3, sl
00314a88: ldrne r2, [sp, #0x2c]
00314a8c: ldr r3, [sp, #0x3c]
00314a90: addeq r2, sl, #0x10
00314a94: rsb r2, r3, r2
00314a98: cmp r2, #4
00314a9c: bls #0x314bcc
00314aa0: mov r2, #0x2e
00314aa4: strb r2, [r3]
00314aa8: ldr r2, [sp, #0x3c]
00314aac: mov r1, #0x62
00314ab0: strb r1, [r2, #1]
00314ab4: add r3, r2, #1
00314ab8: mov r2, #0x6b
00314abc: strb r2, [r3, #2]
00314ac0: mov r2, #0x61
00314ac4: strb r2, [r3, #1]
00314ac8: ldr r3, [sp, #0x3c]
00314acc: mov r2, #0
00314ad0: strb r2, [r3, #4]
00314ad4: ldr r3, [sp, #0x3c]
00314ad8: ldr r7, [sp, #0x40]
00314adc: add r3, r3, #4
00314ae0: str r3, [sp, #0x3c]
00314ae4: mov r0, sb
00314ae8: mov r1, fp
00314aec: mov r2, r7
00314af0: ldr r3, [sp, #8]
00314af4: blx r3
00314af8: mov r0, sl
00314afc: bl #0x3139ac
00314b00: b #0x314974
00314b04: ldr r3, [r7]
00314b08: str r5, [sl, #0x10]
00314b0c: mov r2, sb
00314b10: ldr r0, [r3, #0x20]
00314b14: ldr r1, [r3, #0x1c]
00314b18: ldr r3, [pc, #0x1b4]
00314b1c: rsb r1, r1, r0
00314b20: ldr r3, [r4, r3]
00314b24: asr r1, r1, #2
00314b28: sub r1, r1, #1
00314b2c: ldr r3, [r3, #0x10]
00314b30: str r1, [sl, #0x14]
00314b34: ldr r1, [r7, #0x18]
00314b38: ldr r3, [r3, #0x34]
00314b3c: mov r0, r3
00314b40: ldr r3, [r3]
00314b44: mov lr, pc
00314b48: ldr pc, [r3, #0x94]
00314b4c: cmp r0, #0
00314b50: str r0, [sl, #0x18]
00314b54: ldrne r5, [r7]
00314b58: bne #0x314788
00314b5c: ldr r3, [pc, #0x164]
00314b60: ldr r3, [r4, r3]
00314b64: ldr r3, [r3]
00314b68: cmp r3, #2
00314b6c: streq r0, [r0]
00314b70: beq #0x314b7c
00314b74: cmp r3, #1
00314b78: beq #0x314c6c
00314b7c: ldr r0, [r4, r8]
00314b80: bl #0x313720
00314b84: mov r0, #1
00314b88: b #0x314808
00314b8c: cmp r1, #0
00314b90: beq #0x3148d8
00314b94: b #0x3148b4
00314b98: ldr r0, [pc, #0x13c]
00314b9c: ldr r1, [pc, #0x13c]
00314ba0: ldr r2, [pc, #0x13c]
00314ba4: ldr r0, [r4, r0]
00314ba8: ldr r3, [pc, #0x138]
00314bac: mov ip, #0x50
00314bb0: add r1, pc, r1
00314bb4: add r2, pc, r2
00314bb8: add r3, pc, r3
00314bbc: add r0, r0, #0xa8
00314bc0: str ip, [sp]
00314bc4: bl #0x30e004
00314bc8: b #0x3148d8
00314bcc: mov r0, sl
00314bd0: mov r1, #4
00314bd4: bl #0x3107a0
00314bd8: subs r3, r0, #0
00314bdc: streq r3, [sp, #0xc]
00314be0: moveq r7, r3
00314be4: bne #0x314c48
00314be8: ldr r1, [sp, #0x40]
00314bec: ldr r5, [sp, #0x3c]
00314bf0: cmp r1, r5
00314bf4: moveq r0, r7
00314bf8: beq #0x314c10
00314bfc: rsb r5, r1, r5
00314c00: mov r0, r7
00314c04: mov r2, r5
00314c08: bl #0x30e868
00314c0c: add r0, r0, r5
00314c10: ldr r1, [pc, #0xd4]
00314c14: mov r2, #4
00314c18: add r1, pc, r1
00314c1c: bl #0x30e868
00314c20: mov r3, #0
00314c24: strb r3, [r0, #4]
00314c28: add r5, r0, #4
00314c2c: mov r0, sl
00314c30: bl #0x3139ac
00314c34: ldr r3, [sp, #0xc]
00314c38: str r5, [sp, #0x3c]
00314c3c: str r7, [sp, #0x40]
00314c40: str r3, [sp, #0x2c]
00314c44: b #0x314ae4
00314c48: add r0, sp, #0x48
00314c4c: str r3, [r0, #-0x2c]!
00314c50: bl #0x313994
00314c54: ldr r3, [sp, #0x1c]
00314c58: mov r7, r0
00314c5c: add r3, r0, r3
00314c60: str r3, [sp, #0xc]
00314c64: b #0x314be8
00314c68: bl #0x30e310
00314c6c: ldr r0, [pc, #0x68]
00314c70: ldr r1, [pc, #0x78]
00314c74: ldr r2, [pc, #0x78]
00314c78: ldr r0, [r4, r0]
00314c7c: ldr r3, [pc, #0x74]
00314c80: mov ip, #0x76
00314c84: add r1, pc, r1
00314c88: add r2, pc, r2
00314c8c: add r3, pc, r3
00314c90: add r0, r0, #0xa8
00314c94: str ip, [sp]
00314c98: bl #0x30e004
00314c9c: b #0x314b7c
00314ca0: rsbeq r0, r8, ip, asr #6
00314ca4: andeq r4, r0, ip, lsr #1
00314ca8: rsbeq fp, r8, r8, asr r1
00314cac: andeq r3, r0, r4, asr pc
00314cb0: rsbeq fp, r8, r8, lsl r1

# _ZN8Savegame9FlushJobsEPKc 00314cfc size164
00314cfc: push {r4, r5, r6, lr}
00314d00: ldr r4, [pc, #0x8c]
00314d04: subs r5, r0, #0
00314d08: add r4, pc, r4
00314d0c: beq #0x314d74
00314d10: ldr r0, [pc, #0x80]
00314d14: mov r1, r5
00314d18: ldr r0, [r4, r0]
00314d1c: add r0, r0, #4
00314d20: bl #0x313c48
00314d24: cmp r0, #0
00314d28: bne #0x314d64
00314d2c: ldr r3, [pc, #0x68]
00314d30: ldr r3, [r4, r3]
00314d34: mov r6, r3
00314d38: ldr r4, [r3]
00314d3c: b #0x314d50
00314d40: bl #0x313c48
00314d44: cmp r0, #0
00314d48: bne #0x314d84
00314d4c: ldr r4, [r4]
00314d50: cmp r4, r6
00314d54: add r0, r4, #0xc
00314d58: mov r1, r5
00314d5c: bne #0x314d40
00314d60: pop {r4, r5, r6, pc}
00314d64: bl #0x314734
00314d68: cmp r0, #0
00314d6c: bne #0x314d64
00314d70: pop {r4, r5, r6, pc}
00314d74: bl #0x314734
00314d78: cmp r0, #0
00314d7c: bne #0x314d74
00314d80: pop {r4, r5, r6, pc}
00314d84: bl #0x314734
00314d88: cmp r0, #0
00314d8c: bne #0x314d84
00314d90: pop {r4, r5, r6, pc}
00314d94: rsbeq pc, r7, r8, lsl #27
00314d98: andeq r3, r0, r4, asr pc
00314d9c: andeq r2, r0, r4, lsl r4

# _ZN8Savegame6AddJobERNS_3JobE 00315110 size220
00315110: ldr r3, [pc, #0xcc]
00315114: ldr r2, [pc, #0xcc]
00315118: push {r4, r5, r6, r7, r8, sl, lr}
0031511c: add r3, pc, r3
00315120: ldr r2, [r3, r2]
00315124: sub sp, sp, #0x14
00315128: mov r5, r0
0031512c: ldr r4, [r2]
00315130: mov r7, r2
00315134: mov sl, sp
00315138: cmp r4, r7
0031513c: add r8, sp, #0xc
00315140: beq #0x315174
00315144: ldr r1, [r4, #0x20]
00315148: ldr r3, [r4, #0x1c]
0031514c: ldr r0, [r5, #0x18]
00315150: ldr r2, [r5, #0x14]
00315154: rsb r3, r1, r3
00315158: ldr r6, [r4]
0031515c: rsb r2, r0, r2
00315160: cmp r2, r3
00315164: beq #0x3151b0
00315168: mov r4, r6
0031516c: cmp r4, r7
00315170: bne #0x315144
00315174: mov r3, #0x28
00315178: add r0, sp, #0x10
0031517c: str r3, [r0, #-8]!
00315180: bl #0x708ec0
00315184: mov r1, r5
00315188: mov r6, r0
0031518c: add r0, r0, #8
00315190: bl #0x3145e0
00315194: ldr r3, [r4, #4]
00315198: str r4, [r6]
0031519c: str r3, [r6, #4]
003151a0: str r6, [r3]
003151a4: str r6, [r4, #4]
003151a8: add sp, sp, #0x14
003151ac: pop {r4, r5, r6, r7, r8, sl, pc}
003151b0: bl #0x30e5e0
003151b4: cmp r0, #0
003151b8: bne #0x315168
003151bc: ldrb r3, [r4, #0x24]
003151c0: ldrb r2, [r5, #0x1c]
003151c4: cmp r2, r3
003151c8: bne #0x315168
003151cc: mov r0, sp
003151d0: mov r1, r8
003151d4: str r4, [sp, #0xc]
003151d8: bl #0x3140b0
003151dc: mov r4, r6
003151e0: b #0x31516c
003151e4: rsbeq pc, r7, r4, ror sb
003151e8: andeq r2, r0, r4, lsl r4

# _ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_ 00315848 size188
00315848: push {r4, r5, r6, r7, r8, sl, lr}
0031584c: sub sp, sp, #0xc
00315850: add r4, sp, #8
00315854: str r1, [r4, #-4]!
00315858: add r5, r0, #0x20
0031585c: mov r6, r0
00315860: mov r1, r4
00315864: mov r0, r5
00315868: mov sl, r3
0031586c: mov r7, r2
00315870: ldr r8, [sp, #0x28]
00315874: bl #0x314120
00315878: cmp r0, r5
0031587c: mov r3, r0
00315880: beq #0x3158d8
00315884: ldr r2, [r0, #0x30]
00315888: str sl, [r0, #0x38]
0031588c: str r7, [r0, #0x34]
00315890: cmp r2, #0
00315894: str r8, [r0, #0x3c]
00315898: beq #0x3158d0
0031589c: ldr r1, [r6, #0x1c]
003158a0: cmp r1, #0
003158a4: beq #0x3158d0
003158a8: mov r0, r1
003158ac: ldrd r2, r3, [r3, #0x28]
003158b0: ldr r1, [r1]
003158b4: mov lr, pc
003158b8: ldr pc, [r1, #0x20]
003158bc: cmp r7, #0
003158c0: beq #0x3158d0
003158c4: ldr r0, [r6, #0x1c]
003158c8: mov r1, r8
003158cc: blx r7
003158d0: add sp, sp, #0xc
003158d4: pop {r4, r5, r6, r7, r8, sl, pc}
003158d8: mov r1, r4
003158dc: bl #0x3156f0
003158e0: mov r3, #0
003158e4: mov r2, #0
003158e8: strd r2, r3, [r0]
003158ec: mov r3, #0
003158f0: str r8, [r0, #0x14]
003158f4: str sl, [r0, #0x10]
003158f8: str r7, [r0, #0xc]
003158fc: str r3, [r0, #8]
00315900: b #0x3158d0

# _ZN8Savegame15initSectionInfoEPKcPFvP11IStreamBasePvES6_S4_ 00315904 size116
00315904: push {r4, r5, r6, r7, r8, lr}
00315908: sub sp, sp, #8
0031590c: add r4, sp, #8
00315910: str r1, [r4, #-4]!
00315914: add r5, r0, #0x20
00315918: mov r0, r5
0031591c: mov r1, r4
00315920: mov r6, r2
00315924: mov r7, r3
00315928: ldr r8, [sp, #0x20]
0031592c: bl #0x314120
00315930: cmp r0, r5
00315934: strne r8, [r0, #0x3c]
00315938: strne r6, [r0, #0x34]
0031593c: strne r7, [r0, #0x38]
00315940: beq #0x31594c
00315944: add sp, sp, #8
00315948: pop {r4, r5, r6, r7, r8, pc}
0031594c: mov r1, r4
00315950: bl #0x3156f0
00315954: mov r3, #0
00315958: mov r2, #0
0031595c: strd r2, r3, [r0]
00315960: mov r3, #0
00315964: str r8, [r0, #0x14]
00315968: str r7, [r0, #0x10]
0031596c: str r6, [r0, #0xc]
00315970: str r3, [r0, #8]
00315974: b #0x315944

# _ZN8Savegame10_cacheFileEP12StreamBuffer 00315ad0 size1032
00315ad0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315ad4: ldr r3, [r0, #0x1c]
00315ad8: ldr r6, [pc, #0x3ec]
00315adc: sub sp, sp, #0x1c
00315ae0: cmp r3, #0
00315ae4: mov r4, r0
00315ae8: mov r5, r1
00315aec: add r6, pc, r6
00315af0: beq #0x315b0c
00315af4: mov r0, r3
00315af8: ldr r3, [r3]
00315afc: mov lr, pc
00315b00: ldr pc, [r3, #4]
00315b04: mov r3, #0
00315b08: str r3, [r4, #0x1c]
00315b0c: cmp r5, #0
00315b10: beq #0x315e30
00315b14: mov r0, r5
00315b18: mov r2, #0
00315b1c: mov r3, #0
00315b20: ldr r1, [r5]
00315b24: mov lr, pc
00315b28: ldr pc, [r1, #0x20]
00315b2c: mov r2, #0
00315b30: mov r3, #0
00315b34: mov r0, r5
00315b38: ldr r1, [r5]
00315b3c: mov lr, pc
00315b40: ldr pc, [r1, #0x2c]
00315b44: mov r1, #0
00315b48: mov r0, #0x30
00315b4c: bl #0x310570
00315b50: mov r1, r5
00315b54: mov r7, r0
00315b58: bl #0x3172d8
00315b5c: str r7, [r4, #0x1c]
00315b60: ldrb r3, [r4, #0x38]
00315b64: cmp r3, #0
00315b68: bne #0x315df4
00315b6c: ldr r3, [r4, #0x1c]
00315b70: cmp r3, #0
00315b74: beq #0x315bbc
00315b78: mov r0, r3
00315b7c: ldr r3, [r3]
00315b80: mov lr, pc
00315b84: ldr pc, [r3, #8]
00315b88: cmp r1, #0
00315b8c: bne #0x315dfc
00315b90: cmp r0, #3
00315b94: bhi #0x315dfc
00315b98: ldr r3, [r4, #0x1c]
00315b9c: cmp r3, #0
00315ba0: beq #0x315bbc
00315ba4: mov r0, r3
00315ba8: ldr r3, [r3]
00315bac: mov lr, pc
00315bb0: ldr pc, [r3, #4]
00315bb4: mov r3, #0
00315bb8: str r3, [r4, #0x1c]
00315bbc: ldr r3, [r4, #0x18]
00315bc0: ldr r0, [r4, #0x14]
00315bc4: mov r1, #0
00315bc8: ldr r7, [pc, #0x300]
00315bcc: rsb r0, r3, r0
00315bd0: add r0, r0, #5
00315bd4: bl #0x31056c
00315bd8: ldr r1, [r4, #0x18]
00315bdc: mov r5, r0
00315be0: bl #0x30e520
00315be4: mov r0, r5
00315be8: bl #0x30de54
00315bec: ldr r1, [pc, #0x2e0]
00315bf0: mov r2, #5
00315bf4: add r0, r5, r0
00315bf8: add r1, pc, r1
00315bfc: bl #0x30e868
00315c00: ldr r3, [r6, r7]
00315c04: mov r1, r5
00315c08: mov r2, #0
00315c0c: ldr r3, [r3, #0x10]
00315c10: ldr r3, [r3, #0x34]
00315c14: mov r0, r3
00315c18: ldr r3, [r3]
00315c1c: mov lr, pc
00315c20: ldr pc, [r3, #0x94]
00315c24: cmp r5, #0
00315c28: str r0, [sp, #0x14]
00315c2c: beq #0x315c3c
00315c30: mov r0, r5
00315c34: bl #0x310440
00315c38: ldr r0, [sp, #0x14]
00315c3c: cmp r0, #0
00315c40: beq #0x315c94
00315c44: mov r1, #0
00315c48: mov r0, #0x30
00315c4c: bl #0x310570
00315c50: add r5, sp, #0x18
00315c54: ldr r1, [r5, #-4]!
00315c58: mov r8, r0
00315c5c: bl #0x3172d8
00315c60: ldr r3, [r6, r7]
00315c64: str r8, [r4, #0x1c]
00315c68: mov r1, r5
00315c6c: ldr r3, [r3, #0x10]
00315c70: ldr r3, [r3, #0x34]
00315c74: mov r0, r3
00315c78: ldr r3, [r3]
00315c7c: mov lr, pc
00315c80: ldr pc, [r3, #0x78]
00315c84: ldr r0, [r4, #0x1c]
00315c88: bl #0x313a90
00315c8c: cmn r0, #1
00315c90: beq #0x315ea4
00315c94: ldr r3, [r4, #0x1c]
00315c98: cmp r3, #0
00315c9c: beq #0x315df4
00315ca0: mov r0, r3
00315ca4: ldr r3, [r3]
00315ca8: mov lr, pc
00315cac: ldr pc, [r3, #8]
00315cb0: cmp r1, #0
00315cb4: bne #0x315cc0
00315cb8: cmp r0, #3
00315cbc: bls #0x315df4
00315cc0: ldr r1, [r4, #0x1c]
00315cc4: mov r2, #0
00315cc8: mov r3, #0
00315ccc: mov r0, r1
00315cd0: ldr r1, [r1]
00315cd4: mov lr, pc
00315cd8: ldr pc, [r1, #0x20]
00315cdc: ldr r0, [r4, #0x1c]
00315ce0: bl #0x313a90
00315ce4: cmp r0, #0
00315ce8: str r0, [sp, #4]
00315cec: beq #0x315df4
00315cf0: mov r5, #0
00315cf4: add r7, r4, #0x20
00315cf8: mov sb, r5
00315cfc: add r8, sp, #0xc
00315d00: ldr r3, [r4, #0x1c]
00315d04: mov r0, r3
00315d08: ldr r3, [r3]
00315d0c: mov lr, pc
00315d10: ldr pc, [r3, #0x24]
00315d14: ldr r3, [r4, #0x1c]
00315d18: mov sl, r0
00315d1c: mov r6, r1
00315d20: mov r0, r3
00315d24: ldr r3, [r3]
00315d28: mov lr, pc
00315d2c: ldr pc, [r3, #8]
00315d30: cmp r1, r6
00315d34: bhi #0x315d44
00315d38: bne #0x315df4
00315d3c: cmp r0, sl
00315d40: bls #0x315df4
00315d44: ldr r0, [r4, #0x1c]
00315d48: bl #0x313a90
00315d4c: mov r2, #4
00315d50: mov r3, #0
00315d54: mov r1, r8
00315d58: mov r6, r0
00315d5c: ldr r0, [r4, #0x1c]
00315d60: strb sb, [sp, #0xc]
00315d64: strb sb, [sp, #0xd]
00315d68: strb sb, [sp, #0xe]
00315d6c: strb sb, [sp, #0xf]
00315d70: strb sb, [sp, #0x10]
00315d74: bl #0x317454
00315d78: ldr r3, [r4, #0x1c]
00315d7c: mov r0, r3
00315d80: ldr r3, [r3]
00315d84: mov lr, pc
00315d88: ldr pc, [r3, #0x24]
00315d8c: mov sl, r0
00315d90: mov fp, r1
00315d94: mov r0, r7
00315d98: mov r1, r8
00315d9c: bl #0x314380
00315da0: cmp r7, r0
00315da4: mov r1, r8
00315da8: beq #0x315e10
00315dac: mov r0, r7
00315db0: bl #0x315978
00315db4: mov r1, r8
00315db8: strd sl, fp, [r0]
00315dbc: mov r0, r7
00315dc0: bl #0x315978
00315dc4: str r6, [r0, #8]
00315dc8: ldr r1, [r4, #0x1c]
00315dcc: adds r2, sl, r6
00315dd0: adc r3, fp, #0
00315dd4: mov r0, r1
00315dd8: ldr r1, [r1]
00315ddc: mov lr, pc
00315de0: ldr pc, [r1, #0x20]
00315de4: ldr r3, [sp, #4]
00315de8: add r5, r5, #1
00315dec: cmp r5, r3
00315df0: bne #0x315d00
00315df4: add sp, sp, #0x1c
00315df8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315dfc: ldr r0, [r4, #0x1c]
00315e00: bl #0x313a90
00315e04: cmn r0, #1
00315e08: bne #0x315c94
00315e0c: b #0x315b98
00315e10: mov r1, r8
00315e14: bl #0x315978
00315e18: strd sl, fp, [r0]
00315e1c: str sb, [r0, #0x14]
00315e20: str sb, [r0, #0x10]
00315e24: str sb, [r0, #0xc]
00315e28: str r6, [r0, #8]
00315e2c: b #0x315dc8
00315e30: ldr r3, [pc, #0x98]
00315e34: ldr r1, [r4, #0x18]
00315e38: mov r2, r5
00315e3c: ldr r7, [r6, r3]
00315e40: ldr r3, [r7, #0x10]
00315e44: ldr r3, [r3, #0x34]
00315e48: mov r0, r3
00315e4c: ldr r3, [r3]
00315e50: mov lr, pc
00315e54: ldr pc, [r3, #0x94]
00315e58: cmp r0, #0
00315e5c: str r0, [sp, #0x14]
00315e60: beq #0x315b60
00315e64: mov r1, r5
00315e68: mov r0, #0x30
00315e6c: bl #0x310570
00315e70: add r5, sp, #0x18
00315e74: mov r8, r0
00315e78: ldr r1, [r5, #-4]!
00315e7c: bl #0x3172d8
00315e80: str r8, [r4, #0x1c]
00315e84: ldr r3, [r7, #0x10]
00315e88: mov r1, r5
00315e8c: ldr r3, [r3, #0x34]
00315e90: mov r0, r3
00315e94: ldr r3, [r3]
00315e98: mov lr, pc
00315e9c: ldr pc, [r3, #0x78]
00315ea0: b #0x315b60
00315ea4: ldr r3, [r4, #0x1c]
00315ea8: cmp r3, #0
00315eac: beq #0x315df4
00315eb0: mov r0, r3
00315eb4: ldr r3, [r3]
00315eb8: mov lr, pc
00315ebc: ldr pc, [r3, #4]
00315ec0: mov r3, #0
00315ec4: str r3, [r4, #0x1c]
00315ec8: b #0x315df4
00315ecc: rsbeq lr, r7, r4, lsr #31
00315ed0: strdeq r3, r4, [r0], -r4
00315ed4: subseq r8, sl, r8, ror #18

# _ZN8SavegameC1EPKcb 00315ed8 size112
00315ed8: ldr r3, [pc, #0x60]
00315edc: ldr ip, [pc, #0x60]
00315ee0: push {r4, r5, lr}
00315ee4: add r3, pc, r3
00315ee8: ldr ip, [r3, ip]
00315eec: sub sp, sp, #0xc
00315ef0: mov r4, r0
00315ef4: add ip, ip, #8
00315ef8: mov r5, r2
00315efc: str ip, [r0], #4
00315f00: add r2, sp, #4
00315f04: bl #0x3140ec
00315f08: mov r1, #0
00315f0c: mov r3, r4
00315f10: str r1, [r4, #0x1c]
00315f14: str r1, [r4, #0x24]
00315f18: strb r1, [r3, #0x20]!
00315f1c: mov r0, r4
00315f20: str r3, [r4, #0x2c]
00315f24: strb r5, [r4, #0x38]
00315f28: str r3, [r4, #0x28]
00315f2c: str r1, [r4, #0x30]
00315f30: bl #0x315ad0
00315f34: mov r0, r4
00315f38: add sp, sp, #0xc
00315f3c: pop {r4, r5, pc}
00315f40: rsbeq lr, r7, ip, lsr #23
00315f44: strheq r0, [r0], -r8

# _ZN8SavegameC2EPKcb 00315f48 size112
00315f48: ldr r3, [pc, #0x60]
00315f4c: ldr ip, [pc, #0x60]
00315f50: push {r4, r5, lr}
00315f54: add r3, pc, r3
00315f58: ldr ip, [r3, ip]
00315f5c: sub sp, sp, #0xc
00315f60: mov r4, r0
00315f64: add ip, ip, #8
00315f68: mov r5, r2
00315f6c: str ip, [r0], #4
00315f70: add r2, sp, #4
00315f74: bl #0x3140ec
00315f78: mov r1, #0
00315f7c: mov r3, r4
00315f80: str r1, [r4, #0x1c]
00315f84: str r1, [r4, #0x24]
00315f88: strb r1, [r3, #0x20]!
00315f8c: mov r0, r4
00315f90: str r3, [r4, #0x2c]
00315f94: strb r5, [r4, #0x38]
00315f98: str r3, [r4, #0x28]
00315f9c: str r1, [r4, #0x30]
00315fa0: bl #0x315ad0
00315fa4: mov r0, r4
00315fa8: add sp, sp, #0xc
00315fac: pop {r4, r5, pc}
00315fb0: rsbeq lr, r7, ip, lsr fp
00315fb4: strheq r0, [r0], -r8

# _ZN8Savegame7saveAllEv 00315fb8 size1032
00315fb8: ldr r1, [pc, #0x3dc]
00315fbc: ldr r2, [pc, #0x3dc]
00315fc0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315fc4: add r1, pc, r1
00315fc8: ldr r3, [r1, r2]
00315fcc: mov sb, r0
00315fd0: ldr r0, [pc, #0x3cc]
00315fd4: ldr r3, [r3]
00315fd8: sub sp, sp, #0x8c
00315fdc: str r1, [sp, #0x14]
00315fe0: add r0, pc, r0
00315fe4: add r1, sp, #0x88
00315fe8: str r2, [sp, #0x20]
00315fec: str r1, [sp, #0x18]
00315ff0: str r3, [sp, #0x84]
00315ff4: bl #0x3136b4
00315ff8: ldr r2, [sp, #0x18]
00315ffc: mov r4, #0
00316000: add fp, sb, #0x20
00316004: str r4, [r2, #-0x24]!
00316008: add r3, r2, #4
0031600c: str r2, [sp, #0x18]
00316010: ldr r2, [sb, #0x14]
00316014: ldr r1, [sb, #0x18]
00316018: mov r0, r3
0031601c: str r3, [sp, #0x78]
00316020: str r3, [sp, #0x7c]
00316024: bl #0x3116e8
00316028: mov r3, #1
0031602c: ldr r0, [sp, #0x18]
00316030: strb r3, [sp, #0x80]
00316034: strb r4, [sp, #0x81]
00316038: bl #0x315110
0031603c: mov r0, #0x30
00316040: bl #0x310454
00316044: add r3, sp, #0x88
00316048: mov r4, r0
0031604c: str r3, [sp, #0x1c]
00316050: bl #0x316d3c
00316054: ldr r0, [sp, #0x1c]
00316058: mvn r3, #0
0031605c: add r7, sp, #0x3c
00316060: str r3, [r0, #-0x48]!
00316064: str r0, [sp, #0x1c]
00316068: ldr r1, [sp, #0x1c]
0031606c: mov r0, r4
00316070: bl #0x3139e0
00316074: ldr r3, [pc, #0x32c]
00316078: ldr r1, [pc, #0x32c]
0031607c: ldr r2, [pc, #0x32c]
00316080: add r3, pc, r3
00316084: str r3, [sp, #0x2c]
00316088: ldr r3, [pc, #0x324]
0031608c: add r3, pc, r3
00316090: str r3, [sp, #0x30]
00316094: ldr r3, [pc, #0x31c]
00316098: add r3, pc, r3
0031609c: str r3, [sp, #0x34]
003160a0: ldr r5, [sb, #0x28]
003160a4: str r1, [sp, #0x24]
003160a8: str r2, [sp, #0x28]
003160ac: cmp r5, fp
003160b0: beq #0x3161a0
003160b4: ldr r3, [r4]
003160b8: mov r0, r4
003160bc: mov lr, pc
003160c0: ldr pc, [r3, #0x30]
003160c4: mov r3, #0
003160c8: strd r0, r1, [sp, #8]
003160cc: mov r1, r7
003160d0: mov r0, r4
003160d4: str r3, [sp, #0x3c]
003160d8: bl #0x3139e0
003160dc: ldr r1, [r5, #0x24]
003160e0: mov r2, #4
003160e4: mov r3, #0
003160e8: mov r0, r4
003160ec: bl #0x317604
003160f0: ldr r3, [r4]
003160f4: mov r0, r4
003160f8: mov lr, pc
003160fc: ldr pc, [r3, #0x30]
00316100: ldr r3, [r5, #0x38]
00316104: mov r8, r0
00316108: cmp r3, #0
0031610c: beq #0x316284
00316110: mov r0, r4
00316114: ldr r1, [r5, #0x3c]
00316118: blx r3
0031611c: ldr r3, [r4]
00316120: mov r0, r4
00316124: mov lr, pc
00316128: ldr pc, [r3, #0x30]
0031612c: ldrd r2, r3, [sp, #8]
00316130: rsb r8, r8, r0
00316134: str r8, [sp, #0x3c]
00316138: mov r6, r0
0031613c: mov sl, r1
00316140: mov r0, r4
00316144: ldr r1, [r4]
00316148: mov lr, pc
0031614c: ldr pc, [r1, #0x2c]
00316150: mov r0, r4
00316154: mov r1, r7
00316158: bl #0x3139e0
0031615c: mov r3, sl
00316160: mov r2, r6
00316164: ldr r1, [r4]
00316168: mov r0, r4
0031616c: mov lr, pc
00316170: ldr pc, [r1, #0x2c]
00316174: ldr r3, [r5, #0xc]
00316178: cmp r3, #0
0031617c: bne #0x316188
00316180: b #0x3162ec
00316184: mov r3, r2
00316188: ldr r2, [r3, #8]
0031618c: cmp r2, #0
00316190: bne #0x316184
00316194: mov r5, r3
00316198: cmp r5, fp
0031619c: bne #0x3160b4
003161a0: ldr r3, [r4]
003161a4: mov r0, r4
003161a8: mov lr, pc
003161ac: ldr pc, [r3, #0x30]
003161b0: mov r2, #0
003161b4: mov r6, r0
003161b8: mov r7, r1
003161bc: mov r3, #0
003161c0: mov r0, r4
003161c4: ldr r1, [r4]
003161c8: mov lr, pc
003161cc: ldr pc, [r1, #0x2c]
003161d0: ldr r3, [sb, #0x30]
003161d4: ldr r1, [sp, #0x1c]
003161d8: mov r0, r4
003161dc: str r3, [sp, #0x40]
003161e0: bl #0x3139e0
003161e4: mov r2, r6
003161e8: mov r3, r7
003161ec: ldr r1, [r4]
003161f0: mov r0, r4
003161f4: mov lr, pc
003161f8: ldr pc, [r1, #0x2c]
003161fc: add r5, sp, #0x88
00316200: mov r0, sb
00316204: mov r1, r4
00316208: bl #0x315ad0
0031620c: str r4, [r5, #-0x44]!
00316210: add r3, r5, #4
00316214: ldr r2, [sb, #0x14]
00316218: ldr r1, [sb, #0x18]
0031621c: mov r0, r3
00316220: str r3, [sp, #0x58]
00316224: str r3, [sp, #0x5c]
00316228: bl #0x3116e8
0031622c: mov r3, #0
00316230: mov r0, r5
00316234: strb r3, [sp, #0x60]
00316238: mov r3, #1
0031623c: strb r3, [sp, #0x61]
00316240: bl #0x315110
00316244: mov r0, r5
00316248: bl #0x313c90
0031624c: ldr r0, [sp, #0x18]
00316250: bl #0x313c90
00316254: ldr r0, [pc, #0x160]
00316258: add r0, pc, r0
0031625c: bl #0x3136b8
00316260: ldr r1, [sp, #0x14]
00316264: ldr r0, [sp, #0x20]
00316268: ldr r2, [sp, #0x84]
0031626c: ldr r3, [r1, r0]
00316270: ldr r3, [r3]
00316274: cmp r2, r3
00316278: bne #0x316398
0031627c: add sp, sp, #0x8c
00316280: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00316284: ldr r6, [sb, #0x1c]
00316288: cmp r6, #0
0031628c: beq #0x316320
00316290: ldrb r3, [r6, #0x2c]
00316294: ldr r2, [r4]
00316298: cmp r3, #0
0031629c: ldr sl, [r2, #0x1c]
003162a0: bne #0x3162c8
003162a4: ldr r1, [sp, #0x14]
003162a8: ldr r0, [sp, #0x24]
003162ac: ldr r2, [r1, r0]
003162b0: ldr r2, [r2]
003162b4: cmp r2, #2
003162b8: streq r3, [r3]
003162bc: beq #0x3162c8
003162c0: cmp r2, #1
003162c4: beq #0x31636c
003162c8: ldr r3, [r6, #0x1c]
003162cc: ldr ip, [r5, #0x28]
003162d0: mov r0, r4
003162d4: ldr r1, [r3]
003162d8: ldr r2, [r5, #0x30]
003162dc: mov r3, #0
003162e0: add r1, r1, ip
003162e4: blx sl
003162e8: b #0x31611c
003162ec: ldr r2, [r5, #4]
003162f0: ldr r1, [r2, #0xc]
003162f4: cmp r5, r1
003162f8: bne #0x316314
003162fc: mov r5, r2
00316300: ldr r2, [r2, #4]
00316304: ldr r3, [r2, #0xc]
00316308: cmp r3, r5
0031630c: beq #0x3162fc
00316310: ldr r3, [r5, #0xc]
00316314: cmp r3, r2
00316318: movne r5, r2
0031631c: b #0x3160ac
00316320: mov r1, r6
00316324: ldr r0, [r5, #0x30]
00316328: bl #0x31056c
0031632c: mov r1, r6
00316330: mov sl, r0
00316334: ldr r2, [r5, #0x30]
00316338: bl #0x30e460
0031633c: mov r3, r6
00316340: ldr ip, [r4]
00316344: mov r0, r4
00316348: mov r1, sl
0031634c: ldr r2, [r5, #0x30]
00316350: mov lr, pc
00316354: ldr pc, [ip, #0x1c]
00316358: cmp sl, #0
0031635c: beq #0x31611c
00316360: mov r0, sl
00316364: bl #0x310440
00316368: b #0x31611c
0031636c: ldr r3, [sp, #0x14]
00316370: ldr r2, [sp, #0x28]
00316374: mov ip, #0x82
00316378: ldr r1, [sp, #0x2c]
0031637c: ldr r0, [r3, r2]
00316380: ldr r2, [sp, #0x30]
00316384: ldr r3, [sp, #0x34]
00316388: add r0, r0, #0xa8
0031638c: str ip, [sp]
00316390: bl #0x30e004
00316394: b #0x3162c8
00316398: bl #0x30e310
0031639c: rsbeq lr, r7, ip, asr #21
003163a0: andeq r4, r0, ip, lsr #1
003163a4: ldrsbeq r8, [sl], #-0x50
003163a8: subseq r8, sl, r8, asr r3
003163ac: andeq r3, r0, r0, asr #19
003163b0: andeq r1, r0, r0, asr #19
003163b4: subseq r8, sl, ip, lsr r5
003163b8: subseq r8, sl, r8, lsr r5
003163bc: subseq r8, sl, r8, asr r3

# _ZN8Savegame5resetEv 003163c0 size68
003163c0: push {r4, r5, r6, lr}
003163c4: ldr r3, [r0, #0x30]
003163c8: mov r4, r0
003163cc: cmp r3, #0
003163d0: beq #0x3163f8
003163d4: add r5, r0, #0x20
003163d8: mov r0, r5
003163dc: ldr r1, [r4, #0x24]
003163e0: bl #0x313dbc
003163e4: mov r3, #0
003163e8: str r5, [r4, #0x2c]
003163ec: str r3, [r4, #0x30]
003163f0: str r5, [r4, #0x28]
003163f4: str r3, [r4, #0x24]
003163f8: mov r0, r4
003163fc: pop {r4, r5, r6, lr}
00316400: b #0x315fb8

# _ZNK12StreamBuffer4sizeEv 00316430 size12
00316430: ldr r0, [r0, #0x28]
00316434: mov r1, #0
00316438: bx lr

# _ZNK12StreamBuffer4sizeEi 0031643c size28
0031643c: ldr r3, [r0, #0x18]
00316440: ldr r2, [r0, #0x28]
00316444: mls r0, r3, r1, r2
00316448: mov r1, #0
0031644c: cmp r3, r0
00316450: movlo r0, r3
00316454: bx lr

# _ZNK12StreamBuffer7canReadEv 00316458 size8
00316458: mov r0, #1
0031645c: bx lr

# _ZNK12StreamBuffer8canWriteEv 00316460 size8
00316460: mov r0, #0
00316464: bx lr

# _ZN12StreamBuffer4skipEy 00316468 size32
00316468: push {r4, r5, r6, lr}
0031646c: ldrd r4, r5, [r0, #8]
00316470: adds r2, r2, r4
00316474: adc r3, r3, r5
00316478: ldr r1, [r0]
0031647c: mov lr, pc
00316480: ldr pc, [r1, #0x20]
00316484: pop {r4, r5, r6, pc}

# _ZNK12StreamBuffer9tellWriteEv 00316488 size12
00316488: ldr r1, [r0, #0x14]
0031648c: ldr r0, [r0, #0x10]
00316490: bx lr

# _ZN12StreamBuffer4readEPvy 00316494 size56
00316494: push {r4, lr}
00316498: ldr ip, [r0]
0031649c: mov r4, r0
003164a0: mov lr, pc
003164a4: ldr pc, [ip, #0x14]
003164a8: mov r2, r0
003164ac: mov r3, r1
003164b0: ldrd r0, r1, [r4, #8]
003164b4: adds r0, r0, r2
003164b8: adc r1, r1, r3
003164bc: strd r0, r1, [r4, #8]
003164c0: mov r1, r3
003164c4: mov r0, r2
003164c8: pop {r4, pc}

# _ZNK12StreamBuffer4tellEv 003164cc size12
003164cc: ldr r1, [r0, #0xc]
003164d0: ldr r0, [r0, #8]
003164d4: bx lr

# _ZN12StreamBuffer4seekEy 003164d8 size56
003164d8: cmp r3, #0
003164dc: ldr r1, [r0, #0x28]
003164e0: bhi #0x3164fc
003164e4: beq #0x3164f4
003164e8: str r3, [r0, #0xc]
003164ec: str r2, [r0, #8]
003164f0: bx lr
003164f4: cmp r2, r1
003164f8: bls #0x3164e8
003164fc: mov r2, r1
00316500: mov r3, #0
00316504: str r3, [r0, #0xc]
00316508: str r2, [r0, #8]
0031650c: bx lr

# _ZN12StreamBuffer9seekWriteEy 00316510 size56
00316510: cmp r3, #0
00316514: ldr r1, [r0, #0x28]
00316518: bhi #0x316534
0031651c: beq #0x31652c
00316520: str r3, [r0, #0x14]
00316524: str r2, [r0, #0x10]
00316528: bx lr
0031652c: cmp r2, r1
00316530: bls #0x316520
00316534: mov r2, r1
00316538: mov r3, #0
0031653c: str r3, [r0, #0x14]
00316540: str r2, [r0, #0x10]
00316544: bx lr

# _GLOBAL__I_.._.._sources_Utils_StreamBuffer.cpp 00316548 size32
00316548: ldr r3, [pc, #0x14]
0031654c: mov r2, #0x3f000000
00316550: add r3, pc, r3
00316554: str r2, [r3, #8]
00316558: str r2, [r3]
0031655c: str r2, [r3, #4]
00316560: bx lr
00316564: mlseq r8, r8, r3, sb

# _ZNK12StreamBuffer4peekEPvy 003166bc size96
003166bc: push {r4, r5, r6, r7, r8, sb, sl, lr}
003166c0: ldr ip, [r0, #0x28]
003166c4: ldrd r6, r7, [r0, #8]
003166c8: mov r5, r3
003166cc: rsbs r8, r6, ip
003166d0: rsc sb, r7, #0
003166d4: cmp r3, sb
003166d8: mov r4, r2
003166dc: mov r3, r1
003166e0: bhi #0x316710
003166e4: beq #0x316708
003166e8: mov r1, r6
003166ec: mov r2, r3
003166f0: add r0, r0, #0x18
003166f4: mov r3, r4
003166f8: bl #0x316568
003166fc: mov r0, r4
00316700: mov r1, r5
00316704: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00316708: cmp r2, r8
0031670c: bls #0x3166e8
00316710: mov r5, sb
00316714: mov r4, r8
00316718: b #0x3166e8

# _ZN12StreamBufferD1Ev 003169c4 size52
003169c4: ldr r3, [pc, #0x24]
003169c8: ldr r2, [pc, #0x24]
003169cc: push {r4, lr}
003169d0: add r3, pc, r3
003169d4: ldr r2, [r3, r2]
003169d8: mov r4, r0
003169dc: add r2, r2, #8
003169e0: str r2, [r0], #0x18
003169e4: bl #0x316988
003169e8: mov r0, r4
003169ec: pop {r4, pc}
003169f0: rsbeq lr, r7, r0, asr #1
003169f4: andeq r0, r0, r4, ror fp

# _ZN12StreamBufferD0Ev 003169f8 size28
003169f8: push {r4, lr}
003169fc: mov r4, r0
00316a00: bl #0x3169c4
00316a04: mov r0, r4
00316a08: bl #0x310440
00316a0c: mov r0, r4
00316a10: pop {r4, pc}

# _ZN12StreamBufferD2Ev 00316a14 size52
00316a14: ldr r3, [pc, #0x24]
00316a18: ldr r2, [pc, #0x24]
00316a1c: push {r4, lr}
00316a20: add r3, pc, r3
00316a24: ldr r2, [r3, r2]
00316a28: mov r4, r0
00316a2c: add r2, r2, #8
00316a30: str r2, [r0], #0x18
00316a34: bl #0x316988
00316a38: mov r0, r4
00316a3c: pop {r4, pc}
00316a40: rsbeq lr, r7, r0, ror r0
00316a44: andeq r0, r0, r4, ror fp

# _ZN12StreamBuffer5clearEv 00316a48 size36
00316a48: push {r4, lr}
00316a4c: mov r4, r0
00316a50: add r0, r0, #0x18
00316a54: bl #0x31692c
00316a58: mov r2, #0
00316a5c: mov r3, #0
00316a60: strd r2, r3, [r4, #0x10]
00316a64: strd r2, r3, [r4, #8]
00316a68: pop {r4, pc}

# _ZN12StreamBufferC1Ev 00316d3c size92
00316d3c: ldr r1, [pc, #0x4c]
00316d40: ldr ip, [pc, #0x4c]
00316d44: mov r2, #0
00316d48: add r1, pc, r1
00316d4c: ldr ip, [r1, ip]
00316d50: push {r4, r5}
00316d54: mov r4, #0
00316d58: mov r5, #0
00316d5c: add ip, ip, #8
00316d60: mov r1, #0x800
00316d64: strb r2, [r0, #0x2c]
00316d68: str ip, [r0]
00316d6c: strd r4, r5, [r0, #0x10]
00316d70: str r1, [r0, #0x18]
00316d74: strd r4, r5, [r0, #8]
00316d78: str r2, [r0, #0x1c]
00316d7c: str r2, [r0, #0x20]
00316d80: str r2, [r0, #0x24]
00316d84: str r2, [r0, #0x28]
00316d88: pop {r4, r5}
00316d8c: bx lr
00316d90: rsbeq sp, r7, r8, asr #26
00316d94: andeq r0, r0, r4, ror fp

# _ZN12StreamBuffer5writeEPKvy 00316fd4 size64
00316fd4: push {r4, r5, r6, lr}
00316fd8: mov r6, r0
00316fdc: mov r4, r2
00316fe0: mov r5, r3
00316fe4: mov r2, r1
00316fe8: mov r3, r4
00316fec: ldr r1, [r6, #0x10]
00316ff0: add r0, r0, #0x18
00316ff4: bl #0x316f0c
00316ff8: ldrd r2, r3, [r6, #0x10]
00316ffc: adds r2, r2, r4
00317000: adc r3, r3, r5
00317004: strd r2, r3, [r6, #0x10]
00317008: mov r1, r5
0031700c: mov r0, r4
00317010: pop {r4, r5, r6, pc}

# _ZN12StreamBuffer6expandEy 00317180 size56
00317180: push {r4, r5, r6, lr}
00317184: add r5, r0, #0x18
00317188: mov r6, r2
0031718c: mov r4, r0
00317190: mov r0, r5
00317194: bl #0x31692c
00317198: mov r0, r5
0031719c: mov r1, r6
003171a0: bl #0x317014
003171a4: mov r2, #0
003171a8: mov r3, #0
003171ac: strd r2, r3, [r4, #0x10]
003171b0: strd r2, r3, [r4, #8]
003171b4: pop {r4, r5, r6, pc}

# _ZN12StreamBufferC2EP11IStreamBase 003171b8 size288
003171b8: push {r4, r5, r6, r7, lr}
003171bc: ldr r5, [pc, #0xf8]
003171c0: ldr r3, [pc, #0xf8]
003171c4: mov r6, #0
003171c8: add r5, pc, r5
003171cc: ldr r3, [r5, r3]
003171d0: mov r7, #0
003171d4: strd r6, r7, [r0, #0x10]
003171d8: strd r6, r7, [r0, #8]
003171dc: add r3, r3, #8
003171e0: mov r2, #0
003171e4: str r3, [r0]
003171e8: mov r3, #0x800
003171ec: strb r2, [r0, #0x2c]
003171f0: str r2, [r0, #0x1c]
003171f4: str r2, [r0, #0x20]
003171f8: str r2, [r0, #0x24]
003171fc: str r2, [r0, #0x28]
00317200: str r3, [r0, #0x18]
00317204: mov r4, r0
00317208: sub sp, sp, #0xc
0031720c: ldr r3, [r1]
00317210: mov r0, r1
00317214: mov r6, r1
00317218: mov lr, pc
0031721c: ldr pc, [r3, #8]
00317220: mov r2, r0
00317224: mov r3, r1
00317228: mov r0, r4
0031722c: bl #0x317180
00317230: ldrb r3, [r4, #0x2c]
00317234: ldr r2, [r6]
00317238: cmp r3, #0
0031723c: ldr r7, [r2, #0x18]
00317240: bne #0x317264
00317244: ldr r2, [pc, #0x78]
00317248: ldr r2, [r5, r2]
0031724c: ldr r2, [r2]
00317250: cmp r2, #2
00317254: streq r3, [r3]
00317258: beq #0x317264
0031725c: cmp r2, #1
00317260: beq #0x317288
00317264: ldr r3, [r4, #0x1c]
00317268: mov r0, r6
0031726c: ldr r2, [r4, #0x28]
00317270: ldr r1, [r3]
00317274: mov r3, #0
00317278: blx r7
0031727c: mov r0, r4
00317280: add sp, sp, #0xc
00317284: pop {r4, r5, r6, r7, pc}
00317288: ldr r0, [pc, #0x38]
0031728c: ldr r1, [pc, #0x38]
00317290: ldr r2, [pc, #0x38]
00317294: ldr r0, [r5, r0]
00317298: ldr r3, [pc, #0x34]
0031729c: mov ip, #0x82
003172a0: add r1, pc, r1
003172a4: add r2, pc, r2
003172a8: add r3, pc, r3
003172ac: add r0, r0, #0xa8
003172b0: str ip, [sp]
003172b4: bl #0x30e004
003172b8: b #0x317264
003172bc: rsbeq sp, r7, r8, asr #17
003172c0: andeq r0, r0, r4, ror fp
003172c4: andeq r3, r0, r0, asr #19
003172c8: andeq r1, r0, r0, asr #19
003172cc: subseq r7, sl, r8, lsr r1
003172d0: subseq r7, sl, r4, lsr #6

# _ZN12StreamBufferC1EP11IStreamBase 003172d8 size288
003172d8: push {r4, r5, r6, r7, lr}
003172dc: ldr r5, [pc, #0xf8]
003172e0: ldr r3, [pc, #0xf8]
003172e4: mov r6, #0
003172e8: add r5, pc, r5
003172ec: ldr r3, [r5, r3]
003172f0: mov r7, #0
003172f4: strd r6, r7, [r0, #0x10]
003172f8: strd r6, r7, [r0, #8]
003172fc: add r3, r3, #8
00317300: mov r2, #0
00317304: str r3, [r0]
00317308: mov r3, #0x800
0031730c: strb r2, [r0, #0x2c]
00317310: str r2, [r0, #0x1c]
00317314: str r2, [r0, #0x20]
00317318: str r2, [r0, #0x24]
0031731c: str r2, [r0, #0x28]
00317320: str r3, [r0, #0x18]
00317324: mov r4, r0
00317328: sub sp, sp, #0xc
0031732c: ldr r3, [r1]
00317330: mov r0, r1
00317334: mov r6, r1
00317338: mov lr, pc
0031733c: ldr pc, [r3, #8]
00317340: mov r2, r0
00317344: mov r3, r1
00317348: mov r0, r4
0031734c: bl #0x317180
00317350: ldrb r3, [r4, #0x2c]
00317354: ldr r2, [r6]
00317358: cmp r3, #0
0031735c: ldr r7, [r2, #0x18]
00317360: bne #0x317384
00317364: ldr r2, [pc, #0x78]
00317368: ldr r2, [r5, r2]
0031736c: ldr r2, [r2]
00317370: cmp r2, #2
00317374: streq r3, [r3]
00317378: beq #0x317384
0031737c: cmp r2, #1
00317380: beq #0x3173a8
00317384: ldr r3, [r4, #0x1c]
00317388: mov r0, r6
0031738c: ldr r2, [r4, #0x28]
00317390: ldr r1, [r3]
00317394: mov r3, #0
00317398: blx r7
0031739c: mov r0, r4
003173a0: add sp, sp, #0xc
003173a4: pop {r4, r5, r6, r7, pc}
003173a8: ldr r0, [pc, #0x38]
003173ac: ldr r1, [pc, #0x38]
003173b0: ldr r2, [pc, #0x38]
003173b4: ldr r0, [r5, r0]
003173b8: ldr r3, [pc, #0x34]
003173bc: mov ip, #0x82
003173c0: add r1, pc, r1
003173c4: add r2, pc, r2
003173c8: add r3, pc, r3
003173cc: add r0, r0, #0xa8
003173d0: str ip, [sp]
003173d4: bl #0x30e004
003173d8: b #0x317384
003173dc: rsbeq sp, r7, r8, lsr #15
003173e0: andeq r0, r0, r4, ror fp
003173e4: andeq r3, r0, r0, asr #19
003173e8: andeq r1, r0, r0, asr #19
003173ec: subseq r7, sl, r8, lsl r0
003173f0: subseq r7, sl, r4, lsl #4
003173f4: subseq r7, sl, r8, ror r2

# _ZN12StreamBufferC2Ev 003173f8 size92
003173f8: ldr r1, [pc, #0x4c]
003173fc: ldr ip, [pc, #0x4c]
00317400: mov r2, #0
00317404: add r1, pc, r1
00317408: ldr ip, [r1, ip]
0031740c: push {r4, r5}
00317410: mov r4, #0
00317414: mov r5, #0
00317418: add ip, ip, #8
0031741c: mov r1, #0x800
00317420: strb r2, [r0, #0x2c]
00317424: str ip, [r0]
00317428: strd r4, r5, [r0, #0x10]
0031742c: str r1, [r0, #0x18]
00317430: strd r4, r5, [r0, #8]
00317434: str r2, [r0, #0x1c]
00317438: str r2, [r0, #0x20]
0031743c: str r2, [r0, #0x24]
00317440: str r2, [r0, #0x28]
00317444: pop {r4, r5}
00317448: bx lr
0031744c: rsbeq sp, r7, ip, lsl #13
00317450: andeq r0, r0, r4, ror fp

# _ZN3sfc6script3lua8Instance8loadFileER12StreamBuffer 0031acf4 size244
0031acf4: push {r4, r5, r6, r7, r8, sl, lr}
0031acf8: ldr r4, [pc, #0xd8]
0031acfc: ldr r8, [pc, #0xd8]
0031ad00: sub sp, sp, #0x410
0031ad04: add r4, pc, r4
0031ad08: ldr r3, [r4, r8]
0031ad0c: sub sp, sp, #0xc
0031ad10: mov r6, r1
0031ad14: ldr r3, [r3]
0031ad18: mov sl, r2
0031ad1c: mov r5, r0
0031ad20: str r3, [sp, #0x414]
0031ad24: bl #0x31a804
0031ad28: ldr r2, [pc, #0xb0]
0031ad2c: ldr r7, [r6, #4]
0031ad30: mov ip, #0
0031ad34: ldr r3, [pc, #0xa8]
0031ad38: str ip, [sp, #4]
0031ad3c: add ip, sp, #0x18
0031ad40: ldr r1, [r4, r2]
0031ad44: sub ip, ip, #4
0031ad48: add r2, sp, #8
0031ad4c: add r3, pc, r3
0031ad50: sub r2, r2, #8
0031ad54: str ip, [sp, #0xc]
0031ad58: mov r0, r7
0031ad5c: mov ip, #0x400
0031ad60: str ip, [sp, #0x10]
0031ad64: str sl, [sp, #8]
0031ad68: str r6, [sp]
0031ad6c: bl #0x84bbbc
0031ad70: mov r1, r7
0031ad74: mov r2, r0
0031ad78: mov r0, r5
0031ad7c: bl #0x31a8ac
0031ad80: ldr r1, [r5, #4]
0031ad84: cmp r1, #0
0031ad88: bne #0x31adb0
0031ad8c: ldr r6, [r6, #4]
0031ad90: mov r2, r1
0031ad94: mov r3, r1
0031ad98: mov r0, r6
0031ad9c: bl #0x84bc50
0031ada0: mov r1, r6
0031ada4: mov r2, r0
0031ada8: mov r0, r5
0031adac: bl #0x31a8ac
0031adb0: ldr r3, [r4, r8]
0031adb4: ldr r2, [sp, #0x414]
0031adb8: mov r0, r5
0031adbc: ldr r3, [r3]
0031adc0: cmp r2, r3
0031adc4: bne #0x31add4
0031adc8: add sp, sp, #0x1c
0031adcc: add sp, sp, #0x400
0031add0: pop {r4, r5, r6, r7, r8, sl, pc}
0031add4: bl #0x30e310
0031add8: rsbeq sb, r7, ip, lsl #27
0031addc: andeq r4, r0, ip, lsr #1
0031ade0: andeq r0, r0, r4, asr sb
0031ade4: subseq r3, sl, ip, lsl fp

# _ZN14FileSystemBase14doesFileExistsEPKc 0034df94 size76
0034df94: push {r4, r5, lr}
0034df98: mov r2, #0
0034df9c: sub sp, sp, #0xc
0034dfa0: mov r3, r2
0034dfa4: ldr ip, [r0]
0034dfa8: mov r4, r0
0034dfac: mov lr, pc
0034dfb0: ldr pc, [ip, #0x88]
0034dfb4: add r1, sp, #8
0034dfb8: str r0, [r1, #-4]!
0034dfbc: mov r5, r0
0034dfc0: ldr r3, [r4]
0034dfc4: mov r0, r4
0034dfc8: mov lr, pc
0034dfcc: ldr pc, [r3, #0x78]
0034dfd0: subs r0, r5, #0
0034dfd4: movne r0, #1
0034dfd8: add sp, sp, #0xc
0034dfdc: pop {r4, r5, pc}

# _ZN14FileSystemBase18doesResourceExistsEPKc 0034dfe0 size68
0034dfe0: push {r4, r5, lr}
0034dfe4: sub sp, sp, #0xc
0034dfe8: ldr r3, [r0]
0034dfec: mov r4, r0
0034dff0: mov lr, pc
0034dff4: ldr pc, [r3, #0x90]
0034dff8: add r1, sp, #8
0034dffc: str r0, [r1, #-4]!
0034e000: mov r5, r0
0034e004: ldr r3, [r4]
0034e008: mov r0, r4
0034e00c: mov lr, pc
0034e010: ldr pc, [r3, #0x78]
0034e014: subs r0, r5, #0
0034e018: movne r0, #1
0034e01c: add sp, sp, #0xc
0034e020: pop {r4, r5, pc}

# _ZN14FileSystemBase20decodeObfuscatedDataEiPviS0_ 0034e0fc size84
0034e0fc: cmp r0, #3
0034e100: str r4, [sp, #-4]!
0034e104: bgt #0x34e148
0034e108: rsb r3, r0, #4
0034e10c: cmp r2, r3
0034e110: movhs r2, r3
0034e114: cmp r2, #0
0034e118: ble #0x34e148
0034e11c: mvn r0, r0
0034e120: uxtb r0, r0
0034e124: mov r3, #0
0034e128: ldrb r4, [r1, r3]
0034e12c: sub ip, r0, #1
0034e130: add r0, r0, r4
0034e134: strb r0, [r1, r3]
0034e138: add r3, r3, #1
0034e13c: cmp r2, r3
0034e140: uxtb r0, ip
0034e144: bne #0x34e128
0034e148: ldm sp!, {r4}
0034e14c: bx lr

# _ZN14FileSystemBase18doesSavefileExistsEPKc 0034e150 size92
0034e150: push {r4, r5, lr}
0034e154: mov r4, r0
0034e158: sub sp, sp, #0xc
0034e15c: mov r5, r1
0034e160: mov r0, r1
0034e164: bl #0x314cfc
0034e168: mov r1, r5
0034e16c: mov r2, #0
0034e170: ldr r3, [r4]
0034e174: mov r0, r4
0034e178: mov lr, pc
0034e17c: ldr pc, [r3, #0x94]
0034e180: add r1, sp, #8
0034e184: str r0, [r1, #-4]!
0034e188: mov r5, r0
0034e18c: ldr r3, [r4]
0034e190: mov r0, r4
0034e194: mov lr, pc
0034e198: ldr pc, [r3, #0x78]
0034e19c: subs r0, r5, #0
0034e1a0: movne r0, #1
0034e1a4: add sp, sp, #0xc
0034e1a8: pop {r4, r5, pc}

# _ZNK14FileSystemBase14formatFilePathERKSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS2_6memory13E_MEMORY_HINTE0EEEE 0034e1ac size40
0034e1ac: push {r4, lr}
0034e1b0: mov r4, r0
0034e1b4: str r0, [r4, #0x10]
0034e1b8: str r0, [r4, #0x14]
0034e1bc: mov r3, r2
0034e1c0: ldr r1, [r3, #0x14]
0034e1c4: ldr r2, [r2, #0x10]
0034e1c8: bl #0x325ff4
0034e1cc: mov r0, r4
0034e1d0: pop {r4, pc}

# _ZN14FileSystemBase24changeWorkingDirectoryToEPKc 0034e1d4 size72
0034e1d4: push {r4, r5, r6, lr}
0034e1d8: mov r0, r1
0034e1dc: mov r5, r1
0034e1e0: bl #0x30ed60
0034e1e4: ldr r3, [pc, #0x28]
0034e1e8: rsbs r4, r0, #1
0034e1ec: movlo r4, #0
0034e1f0: cmp r4, #0
0034e1f4: add r3, pc, r3
0034e1f8: beq #0x34e20c
0034e1fc: ldr r2, [pc, #0x14]
0034e200: mov r1, r5
0034e204: ldr r0, [r3, r2]
0034e208: bl #0x30e520
0034e20c: mov r0, r4
0034e210: pop {r4, r5, r6, pc}
0034e214: mlseq r4, ip, r8, r6
0034e218: andeq r0, r0, r8, ror #28

# _ZN14FileSystemBaseD1Ev 0034e480 size52
0034e480: ldr r3, [pc, #0x24]
0034e484: ldr r2, [pc, #0x24]
0034e488: push {r4, lr}
0034e48c: add r3, pc, r3
0034e490: ldr r2, [r3, r2]
0034e494: mov r4, r0
0034e498: add r2, r2, #8
0034e49c: str r2, [r0]
0034e4a0: bl #0x56ccc4
0034e4a4: mov r0, r4
0034e4a8: pop {r4, pc}
0034e4ac: rsbeq r6, r4, r4, lsl #12
0034e4b0: andeq r2, r0, ip, lsl #29

# _ZN14FileSystemBaseD0Ev 0034e4b4 size28
0034e4b4: push {r4, lr}
0034e4b8: mov r4, r0
0034e4bc: bl #0x34e480
0034e4c0: mov r0, r4
0034e4c4: bl #0x310440
0034e4c8: mov r0, r4
0034e4cc: pop {r4, pc}

# _ZN14FileSystemBaseD2Ev 0034e4d0 size52
0034e4d0: ldr r3, [pc, #0x24]
0034e4d4: ldr r2, [pc, #0x24]
0034e4d8: push {r4, lr}
0034e4dc: add r3, pc, r3
0034e4e0: ldr r2, [r3, r2]
0034e4e4: mov r4, r0
0034e4e8: add r2, r2, #8
0034e4ec: str r2, [r0]
0034e4f0: bl #0x56ccc4
0034e4f4: mov r0, r4
0034e4f8: pop {r4, pc}
0034e4fc: strhteq r6, [r4], #-0x54
0034e500: andeq r2, r0, ip, lsl #29

# _ZN14FileSystemBaseC1Ev 0034e504 size52
0034e504: push {r4, r5, r6, lr}
0034e508: ldr r4, [pc, #0x20]
0034e50c: mov r5, r0
0034e510: bl #0x56c058
0034e514: ldr r3, [pc, #0x18]
0034e518: add r4, pc, r4
0034e51c: mov r0, r5
0034e520: ldr r3, [r4, r3]
0034e524: add r3, r3, #8
0034e528: str r3, [r5]
0034e52c: pop {r4, r5, r6, pc}
0034e530: rsbeq r6, r4, r8, ror r5
0034e534: andeq r2, r0, ip, lsl #29

# _ZN14FileSystemBaseC2Ev 0034e538 size52
0034e538: push {r4, r5, r6, lr}
0034e53c: ldr r4, [pc, #0x20]
0034e540: mov r5, r0
0034e544: bl #0x56c058
0034e548: ldr r3, [pc, #0x18]
0034e54c: add r4, pc, r4
0034e550: mov r0, r5
0034e554: ldr r3, [r4, r3]
0034e558: add r3, r3, #8
0034e55c: str r3, [r5]
0034e560: pop {r4, r5, r6, pc}
0034e564: rsbeq r6, r4, r4, asr #10
0034e568: andeq r2, r0, ip, lsl #29

# _GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemBase.cpp 0034e56c size136
0034e56c: push {r4, r5, r6, lr}
0034e570: ldr r4, [pc, #0x64]
0034e574: ldr r2, [pc, #0x64]
0034e578: ldr r3, [pc, #0x64]
0034e57c: add r4, pc, r4
0034e580: ldr r1, [r4, r2]
0034e584: add r3, pc, r3
0034e588: mov r2, #0x3f000000
0034e58c: ldr r0, [r1]
0034e590: str r2, [r3, #8]
0034e594: str r2, [r3]
0034e598: tst r0, #1
0034e59c: str r2, [r3, #4]
0034e5a0: beq #0x34e5a8
0034e5a4: pop {r4, r5, r6, pc}
0034e5a8: mov r3, #1
0034e5ac: str r3, [r1]
0034e5b0: ldr r3, [pc, #0x30]
0034e5b4: ldr r5, [r4, r3]
0034e5b8: mov r0, r5
0034e5bc: bl #0x32d79c
0034e5c0: ldr r3, [pc, #0x24]
0034e5c4: mov r0, r5
0034e5c8: ldr r1, [r4, r3]
0034e5cc: ldr r3, [pc, #0x1c]
0034e5d0: ldr r2, [r4, r3]
0034e5d4: pop {r4, r5, r6, lr}
0034e5d8: b #0x30e304
0034e5dc: rsbeq r6, r4, r4, lsl r5
0034e5e0: andeq r0, r0, ip, lsr #31
0034e5e4: rsbeq r3, r5, r8, lsr #18
0034e5e8: strdeq r3, r4, [r0], -r4
0034e5ec: andeq r0, r0, r0, asr #17
0034e5f0: muleq r0, r0, r8

# _ZNK14FileSystemBase15getAbsolutePathERKSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS2_6memory13E_MEMORY_HINTE0EEEE 0034e5f4 size148
0034e5f4: push {r4, r5, r6, r7, r8, lr}
0034e5f8: ldr r4, [pc, #0x80]
0034e5fc: ldr r7, [pc, #0x80]
0034e600: sub sp, sp, #0x20
0034e604: add r4, pc, r4
0034e608: ldr r3, [r4, r7]
0034e60c: add r5, sp, #4
0034e610: mov r8, r1
0034e614: ldr ip, [r3]
0034e618: ldr r3, [r1]
0034e61c: mov r6, r0
0034e620: str ip, [sp, #0x1c]
0034e624: mov r0, r5
0034e628: ldr r2, [r2, #0x14]
0034e62c: mov lr, pc
0034e630: ldr pc, [r3, #0xb8]
0034e634: mov r0, r6
0034e638: mov r1, r8
0034e63c: mov r2, r5
0034e640: bl #0x56c348
0034e644: ldr r0, [sp, #0x18]
0034e648: cmp r0, r5
0034e64c: beq #0x34e65c
0034e650: cmp r0, #0
0034e654: beq #0x34e65c
0034e658: bl #0x310450
0034e65c: ldr r3, [r4, r7]
0034e660: ldr r2, [sp, #0x1c]
0034e664: mov r0, r6
0034e668: ldr r3, [r3]
0034e66c: cmp r2, r3
0034e670: bne #0x34e67c
0034e674: add sp, sp, #0x20
0034e678: pop {r4, r5, r6, r7, r8, pc}
0034e67c: bl #0x30e310
0034e680: rsbeq r6, r4, ip, lsl #9
0034e684: andeq r4, r0, ip, lsr #1

# _ZN14FileSystemBase17createAndOpenFileEPKc 0034e688 size740
0034e688: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034e68c: ldr r4, [pc, #0x2b0]
0034e690: ldr sb, [pc, #0x2b0]
0034e694: sub sp, sp, #0xa4
0034e698: add r4, pc, r4
0034e69c: ldr r3, [r4, sb]
0034e6a0: mov r5, r0
0034e6a4: add sl, sp, #0x84
0034e6a8: ldr r3, [r3]
0034e6ac: mov r2, r1
0034e6b0: add r6, sp, #0x6c
0034e6b4: str r3, [sp, #0x9c]
0034e6b8: ldr r3, [r5]
0034e6bc: mov r0, sl
0034e6c0: mov r1, r5
0034e6c4: mov lr, pc
0034e6c8: ldr pc, [r3, #0xb8]
0034e6cc: mov r0, r6
0034e6d0: mov r1, #0x10
0034e6d4: str r6, [sp, #0x7c]
0034e6d8: str r6, [sp, #0x80]
0034e6dc: bl #0x3209a8
0034e6e0: ldr r3, [sp, #0x7c]
0034e6e4: mov r2, #0
0034e6e8: add r7, sp, #0x54
0034e6ec: strb r2, [r3]
0034e6f0: ldr r3, [sp, #0x98]
0034e6f4: ldr r2, [sp, #0x94]
0034e6f8: mov r0, r7
0034e6fc: mov r1, r3
0034e700: rsb r3, r3, r2
0034e704: cmp r3, #8
0034e708: addls r2, r1, r3
0034e70c: addhi r2, r1, #8
0034e710: str r7, [sp, #0x64]
0034e714: str r7, [sp, #0x68]
0034e718: bl #0x325ff4
0034e71c: ldr r8, [sp, #0x98]
0034e720: ldr r1, [pc, #0x224]
0034e724: mov r0, r8
0034e728: add r1, pc, r1
0034e72c: bl #0x30ebd4
0034e730: cmp r0, #0
0034e734: beq #0x34e8a0
0034e738: mov r1, r8
0034e73c: mov r0, r6
0034e740: ldr r2, [sp, #0x94]
0034e744: bl #0x320b88
0034e748: add r8, sp, #0x24
0034e74c: mov r0, r8
0034e750: ldr r1, [sp, #0x80]
0034e754: ldr r2, [sp, #0x7c]
0034e758: str r8, [sp, #0x34]
0034e75c: str r8, [sp, #0x38]
0034e760: bl #0x325ff4
0034e764: ldr r3, [pc, #0x1e4]
0034e768: ldr r1, [sp, #0x38]
0034e76c: ldr r0, [r4, r3]
0034e770: bl #0x320678
0034e774: cmp r0, #0
0034e778: beq #0x34e808
0034e77c: ldr r3, [r5]
0034e780: mov r0, r5
0034e784: mov lr, pc
0034e788: ldr pc, [r3, #0x2c]
0034e78c: add fp, sp, #0xc
0034e790: str fp, [sp, #0x1c]
0034e794: str fp, [sp, #0x20]
0034e798: str r0, [sp]
0034e79c: bl #0x30de54
0034e7a0: ldr r1, [sp]
0034e7a4: add r2, r1, r0
0034e7a8: mov r0, fp
0034e7ac: bl #0x3116e8
0034e7b0: ldr r1, [pc, #0x19c]
0034e7b4: ldr r3, [r5]
0034e7b8: mov r0, r5
0034e7bc: add r1, pc, r1
0034e7c0: mov lr, pc
0034e7c4: ldr pc, [r3, #0x30]
0034e7c8: mov r0, r5
0034e7cc: ldr r1, [sp, #0x38]
0034e7d0: bl #0x56d4e0
0034e7d4: cmp r0, #0
0034e7d8: str r0, [sp, #4]
0034e7dc: beq #0x34e7ec
0034e7e0: mov r0, fp
0034e7e4: bl #0x3139ac
0034e7e8: b #0x34e820
0034e7ec: ldr r3, [r5]
0034e7f0: mov r0, r5
0034e7f4: ldr r1, [sp, #0x20]
0034e7f8: mov lr, pc
0034e7fc: ldr pc, [r3, #0x30]
0034e800: mov r0, fp
0034e804: bl #0x3139ac
0034e808: mov r0, r5
0034e80c: ldr r1, [sp, #0x38]
0034e810: bl #0x56d4e0
0034e814: cmp r0, #0
0034e818: str r0, [sp, #4]
0034e81c: beq #0x34e918
0034e820: ldr r0, [sp, #0x38]
0034e824: cmp r0, r8
0034e828: beq #0x34e838
0034e82c: cmp r0, #0
0034e830: beq #0x34e838
0034e834: bl #0x310450
0034e838: ldr r0, [sp, #0x68]
0034e83c: cmp r0, r7
0034e840: beq #0x34e850
0034e844: cmp r0, #0
0034e848: beq #0x34e850
0034e84c: bl #0x310450
0034e850: ldr r0, [sp, #0x80]
0034e854: cmp r0, r6
0034e858: beq #0x34e868
0034e85c: cmp r0, #0
0034e860: beq #0x34e868
0034e864: bl #0x310450
0034e868: ldr r0, [sp, #0x98]
0034e86c: cmp r0, sl
0034e870: beq #0x34e880
0034e874: cmp r0, #0
0034e878: beq #0x34e880
0034e87c: bl #0x310450
0034e880: ldr r3, [r4, sb]
0034e884: ldr r2, [sp, #0x9c]
0034e888: ldr r0, [sp, #4]
0034e88c: ldr r3, [r3]
0034e890: cmp r2, r3
0034e894: bne #0x34e940
0034e898: add sp, sp, #0xa4
0034e89c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034e8a0: ldr r1, [pc, #0xb0]
0034e8a4: mov r0, r8
0034e8a8: add r1, pc, r1
0034e8ac: bl #0x30ebd4
0034e8b0: cmp r0, #0
0034e8b4: bne #0x34e738
0034e8b8: ldr r1, [pc, #0x9c]
0034e8bc: ldr r0, [sp, #0x68]
0034e8c0: add r1, pc, r1
0034e8c4: bl #0x30e31c
0034e8c8: cmp r0, #0
0034e8cc: beq #0x34e738
0034e8d0: ldr r3, [pc, #0x88]
0034e8d4: add r8, sp, #0x3c
0034e8d8: mov r2, sl
0034e8dc: ldr r3, [r4, r3]
0034e8e0: mov r0, r8
0034e8e4: ldr r1, [r3]
0034e8e8: bl #0x34e394
0034e8ec: mov r0, r6
0034e8f0: ldr r1, [sp, #0x50]
0034e8f4: ldr r2, [sp, #0x4c]
0034e8f8: bl #0x320b88
0034e8fc: ldr r0, [sp, #0x50]
0034e900: cmp r0, r8
0034e904: beq #0x34e748
0034e908: cmp r0, #0
0034e90c: beq #0x34e748
0034e910: bl #0x310450
0034e914: b #0x34e748
0034e918: ldr r0, [pc, #0x44]
0034e91c: ldr r1, [sp, #0x38]
0034e920: add r0, pc, r0
0034e924: bl #0x324114
0034e928: ldr r0, [pc, #0x38]
0034e92c: ldr r1, [sp, #0x38]
0034e930: mov r2, #3
0034e934: add r0, pc, r0
0034e938: bl #0x60ace8
0034e93c: b #0x34e820
0034e940: bl #0x30e310

# _ZNK14FileSystemBase18ApplyFilenameHacksEPKc 0034e96c size496
0034e96c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034e970: ldr r5, [pc, #0x1d4]
0034e974: ldr r8, [pc, #0x1d4]
0034e978: mov r4, r0
0034e97c: add r5, pc, r5
0034e980: ldr r3, [r5, r8]
0034e984: sub sp, sp, #0x54
0034e988: mov sl, r1
0034e98c: ldr r3, [r3]
0034e990: mov r1, #0x10
0034e994: str r0, [r4, #0x10]
0034e998: str r0, [r4, #0x14]
0034e99c: mov r6, r2
0034e9a0: str r3, [sp, #0x4c]
0034e9a4: bl #0x3209a8
0034e9a8: ldr r3, [r4, #0x10]
0034e9ac: mov r2, #0
0034e9b0: mov r0, sl
0034e9b4: strb r2, [r3]
0034e9b8: ldr r3, [sl]
0034e9bc: mov lr, pc
0034e9c0: ldr pc, [r3, #0x2c]
0034e9c4: mov r7, r0
0034e9c8: bl #0x30de54
0034e9cc: mov r1, r7
0034e9d0: mov sb, r0
0034e9d4: mov r0, r6
0034e9d8: bl #0x30ebd4
0034e9dc: subs r7, r0, #0
0034e9e0: beq #0x34eb2c
0034e9e4: add fp, sb, #1
0034e9e8: add fp, r7, fp
0034e9ec: mov r0, fp
0034e9f0: bl #0x30de54
0034e9f4: mov r1, fp
0034e9f8: add r2, fp, r0
0034e9fc: mov r0, r4
0034ea00: bl #0x320b88
0034ea04: ldr r1, [pc, #0x148]
0034ea08: ldr r0, [r4, #0x14]
0034ea0c: add r1, pc, r1
0034ea10: bl #0x30ebd4
0034ea14: cmp r0, #0
0034ea18: beq #0x34ea70
0034ea1c: add fp, sp, #0x34
0034ea20: mov r3, #1
0034ea24: mov r1, sl
0034ea28: mov r0, fp
0034ea2c: mov r2, r4
0034ea30: bl #0x56c740
0034ea34: ldr r1, [pc, #0x11c]
0034ea38: mov r0, r4
0034ea3c: add r1, pc, r1
0034ea40: add r2, r1, #0x10
0034ea44: bl #0x320b88
0034ea48: mov r0, r4
0034ea4c: ldr r1, [sp, #0x48]
0034ea50: ldr r2, [sp, #0x44]
0034ea54: bl #0x320a4c
0034ea58: ldr r0, [sp, #0x48]
0034ea5c: cmp r0, fp
0034ea60: beq #0x34ea70
0034ea64: cmp r0, #0
0034ea68: beq #0x34ea70
0034ea6c: bl #0x310450
0034ea70: mov r0, r4
0034ea74: mov r1, #0
0034ea78: mvn r2, #0
0034ea7c: bl #0x34e090
0034ea80: cmp r7, #0
0034ea84: beq #0x34eb0c
0034ea88: rsb r3, r6, #1
0034ea8c: add sb, r3, sb
0034ea90: mov r1, r6
0034ea94: add r7, r7, sb
0034ea98: add r6, sp, #0x1c
0034ea9c: add r2, r1, r7
0034eaa0: mov r0, r6
0034eaa4: add r7, sp, #4
0034eaa8: str r6, [sp, #0x2c]
0034eaac: str r6, [sp, #0x30]
0034eab0: bl #0x325ff4
0034eab4: mov r0, r7
0034eab8: mov r1, r6
0034eabc: mov r2, r4
0034eac0: bl #0x34e324
0034eac4: cmp r4, r7
0034eac8: beq #0x34eadc
0034eacc: mov r0, r4
0034ead0: ldr r1, [sp, #0x18]
0034ead4: ldr r2, [sp, #0x14]
0034ead8: bl #0x320b88
0034eadc: ldr r0, [sp, #0x18]
0034eae0: cmp r0, r7
0034eae4: beq #0x34eaf4
0034eae8: cmp r0, #0
0034eaec: beq #0x34eaf4
0034eaf0: bl #0x310450
0034eaf4: ldr r0, [sp, #0x30]
0034eaf8: cmp r0, r6
0034eafc: beq #0x34eb0c
0034eb00: cmp r0, #0
0034eb04: beq #0x34eb0c
0034eb08: bl #0x310450
0034eb0c: ldr r3, [r5, r8]
0034eb10: ldr r2, [sp, #0x4c]
0034eb14: mov r0, r4
0034eb18: ldr r3, [r3]
0034eb1c: cmp r2, r3
0034eb20: bne #0x34eb48
0034eb24: add sp, sp, #0x54
0034eb28: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034eb2c: mov r0, r6
0034eb30: bl #0x30de54
0034eb34: mov r1, r6
0034eb38: add r2, r6, r0
0034eb3c: mov r0, r4
0034eb40: bl #0x320b88
0034eb44: b #0x34ea04
0034eb48: bl #0x30e310
0034eb4c: rsbeq r6, r4, r4, lsl r1
0034eb50: andeq r4, r0, ip, lsr #1
0034eb54: subseq sb, r7, ip, lsr #14
0034eb58: ldrsheq r1, [r7], #-0xcc

# _ZNK15FileSystemWin3211_FileHandle7canReadEv 0034eb94 size8
0034eb94: ldrb r0, [r0, #0xc]
0034eb98: bx lr

# _ZNK15FileSystemWin3211_FileHandle8canWriteEv 0034eb9c size8
0034eb9c: ldrb r0, [r0, #0xd]
0034eba0: bx lr

# _ZNK15FileSystemWin3211getRootPathEv 0034eba4 size8
0034eba4: add r0, r0, #0x2c
0034eba8: bx lr

# _ZNK15FileSystemWin3216getResourcesPathEv 0034ebac size8
0034ebac: add r0, r0, #0x234
0034ebb0: bx lr

# _ZNK15FileSystemWin3216getSavefilesPathEv 0034ebb4 size12
0034ebb4: add r0, r0, #0x430
0034ebb8: add r0, r0, #0xc
0034ebbc: bx lr

# _ZN15FileSystemWin3212copySavefileEPKc 0034ebc0 size4
0034ebc0: bx lr

# _ZNK15FileSystemWin3212_getFileInfoEPKcS1_Pv 0034ebc4 size8
0034ebc4: mov r0, #1
0034ebc8: bx lr

# _ZN15FileSystemWin329closeFileERP11IFileStream 0034ebcc size48
0034ebcc: ldr r3, [r1]
0034ebd0: push {r4, lr}
0034ebd4: cmp r3, #0
0034ebd8: mov r4, r1
0034ebdc: beq #0x34ebf8
0034ebe0: mov r0, r3
0034ebe4: ldr r3, [r3]
0034ebe8: mov lr, pc
0034ebec: ldr pc, [r3, #4]
0034ebf0: mov r3, #0
0034ebf4: str r3, [r4]
0034ebf8: pop {r4, pc}

# _ZNK15FileSystemWin3210getFoldersEPKcRSt6vectorISsSaISsEE 0034ebfc size4
0034ebfc: bx lr

# _ZNK15FileSystemWin3215isFileNewerThanEPKcS1_ 0034ec00 size4
0034ec00: bx lr

# _ZNK15FileSystemWin3211_FileHandle4tellEv 0034edf0 size32
0034edf0: push {r4, lr}
0034edf4: ldr r0, [r0, #4]
0034edf8: bl #0x30de3c
0034edfc: mov r2, r0
0034ee00: asr r3, r2, #0x1f
0034ee04: mov r1, r3
0034ee08: mov r0, r2
0034ee0c: pop {r4, pc}

# _ZN15FileSystemWin3211_FileHandle4seekEy 0034ee10 size16
0034ee10: ldr r0, [r0, #4]
0034ee14: mov r1, r2
0034ee18: mov r2, #0
0034ee1c: b #0x30e5ec

# _ZNK15FileSystemWin3211_FileHandle4sizeEv 0034ee20 size84
0034ee20: push {r4, r5, r6, lr}
0034ee24: mov r4, r0
0034ee28: ldr r0, [r0, #4]
0034ee2c: bl #0x30de3c
0034ee30: mov r1, #0
0034ee34: mov r6, r0
0034ee38: mov r2, #2
0034ee3c: ldr r0, [r4, #4]
0034ee40: bl #0x30e5ec
0034ee44: ldr r0, [r4, #4]
0034ee48: bl #0x30de3c
0034ee4c: mov r1, r6
0034ee50: mov r5, r0
0034ee54: mov r2, #0
0034ee58: ldr r0, [r4, #4]
0034ee5c: bl #0x30e5ec
0034ee60: mov r2, r5
0034ee64: asr r3, r2, #0x1f
0034ee68: mov r1, r3
0034ee6c: mov r0, r2
0034ee70: pop {r4, r5, r6, pc}

# _ZN15FileSystemWin3211_FileHandle5writeEPKvy 0034ee74 size84
0034ee74: push {r4, r5, r6, lr}
0034ee78: ldr r3, [r0]
0034ee7c: mov r6, r2
0034ee80: mov r4, r0
0034ee84: mov r5, r1
0034ee88: mov lr, pc
0034ee8c: ldr pc, [r3, #0x10]
0034ee90: cmp r0, #0
0034ee94: moveq r2, #0
0034ee98: moveq r3, #0
0034ee9c: beq #0x34eebc
0034eea0: mov r2, r6
0034eea4: ldr r3, [r4, #4]
0034eea8: mov r0, r5
0034eeac: mov r1, #1
0034eeb0: bl #0x30e598
0034eeb4: mov r2, r0
0034eeb8: mov r3, #0
0034eebc: mov r1, r3
0034eec0: mov r0, r2
0034eec4: pop {r4, r5, r6, pc}

# _ZN15FileSystemWin3211_FileHandle4readEPvy 0034eec8 size144
0034eec8: push {r4, r5, r6, r7, r8, sb, sl, lr}
0034eecc: ldr r3, [r0]
0034eed0: mov r4, r0
0034eed4: mov r5, r1
0034eed8: mov r8, r2
0034eedc: mov lr, pc
0034eee0: ldr pc, [r3, #0xc]
0034eee4: cmp r0, #0
0034eee8: moveq r6, #0
0034eeec: moveq r7, #0
0034eef0: bne #0x34ef00
0034eef4: mov r1, r7
0034eef8: mov r0, r6
0034eefc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0034ef00: ldr r0, [r4, #4]
0034ef04: bl #0x30de3c
0034ef08: ldr r3, [r4, #4]
0034ef0c: mov sl, r0
0034ef10: mov r1, #1
0034ef14: mov r0, r5
0034ef18: mov r2, r8
0034ef1c: bl #0x30e2ec
0034ef20: ldr r3, [r4, #8]
0034ef24: mov r6, r0
0034ef28: mov r7, #0
0034ef2c: ldrb r3, [r3, #0x24]
0034ef30: cmp r3, #0
0034ef34: beq #0x34eef4
0034ef38: cmp sl, #3
0034ef3c: bgt #0x34eef4
0034ef40: mov r0, sl
0034ef44: mov r1, r5
0034ef48: mov r2, r8
0034ef4c: ldr r3, [r4, #4]
0034ef50: bl #0x34e0fc
0034ef54: b #0x34eef4

# _ZNK15FileSystemWin3211_FileHandle4peekEPvy 0034ef58 size172
0034ef58: push {r4, r5, r6, r7, r8, lr}
0034ef5c: ldr r3, [r0]
0034ef60: mov r5, r2
0034ef64: mov r4, r0
0034ef68: mov r6, r1
0034ef6c: mov lr, pc
0034ef70: ldr pc, [r3, #0xc]
0034ef74: cmp r0, #0
0034ef78: moveq r2, #0
0034ef7c: moveq r3, #0
0034ef80: bne #0x34ef90
0034ef84: mov r1, r3
0034ef88: mov r0, r2
0034ef8c: pop {r4, r5, r6, r7, r8, pc}
0034ef90: ldr r0, [r4, #4]
0034ef94: bl #0x30de3c
0034ef98: ldr r3, [r4, #4]
0034ef9c: mov r8, r0
0034efa0: mov r1, #1
0034efa4: mov r0, r6
0034efa8: mov r2, r5
0034efac: bl #0x30e2ec
0034efb0: ldr r3, [r4, #8]
0034efb4: mov r7, r0
0034efb8: ldrb r3, [r3, #0x24]
0034efbc: cmp r3, #0
0034efc0: beq #0x34efe0
0034efc4: cmp r8, #3
0034efc8: bgt #0x34efe0
0034efcc: mov r0, r8
0034efd0: mov r1, r6
0034efd4: mov r2, r5
0034efd8: ldr r3, [r4, #4]
0034efdc: bl #0x34e0fc
0034efe0: rsb r1, r7, #0
0034efe4: ldr r0, [r4, #4]
0034efe8: mov r2, #1
0034efec: bl #0x30e5ec
0034eff0: mov r2, r7
0034eff4: asr r3, r2, #0x1f
0034eff8: mov r1, r3
0034effc: mov r0, r2
0034f000: pop {r4, r5, r6, r7, r8, pc}

# _ZN15FileSystemWin329_getPathsEv 0034f068 size164
0034f068: ldr r3, [pc, #0x8c]
0034f06c: ldr r2, [pc, #0x8c]
0034f070: push {r4, r5, r6, r7, lr}
0034f074: add r3, pc, r3
0034f078: ldr r6, [r3, r2]
0034f07c: ldr r2, [pc, #0x80]
0034f080: sub sp, sp, #0x10c
0034f084: mov r5, r0
0034f088: ldr r4, [r3, r2]
0034f08c: ldr r2, [r6]
0034f090: add r0, r0, #0x2c
0034f094: ldr r1, [r4]
0034f098: str r2, [sp, #0x104]
0034f09c: bl #0x30e520
0034f0a0: ldr r1, [r4]
0034f0a4: add r0, r5, #0x130
0034f0a8: bl #0x30e520
0034f0ac: ldr r1, [pc, #0x54]
0034f0b0: add r7, sp, #4
0034f0b4: ldr r2, [r4]
0034f0b8: add r1, pc, r1
0034f0bc: mov r0, r7
0034f0c0: bl #0x30eae4
0034f0c4: mov r1, r7
0034f0c8: add r0, r5, #0x338
0034f0cc: bl #0x30e520
0034f0d0: add r0, r5, #0x430
0034f0d4: add r0, r0, #0xc
0034f0d8: ldr r1, [r4]
0034f0dc: bl #0x30e520
0034f0e0: ldr r2, [sp, #0x104]
0034f0e4: ldr r3, [r6]
0034f0e8: cmp r2, r3
0034f0ec: bne #0x34f0f8
0034f0f0: add sp, sp, #0x10c
0034f0f4: pop {r4, r5, r6, r7, pc}
0034f0f8: bl #0x30e310
0034f0fc: rsbeq r5, r4, ip, lsl sl
0034f100: andeq r4, r0, ip, lsr #1
0034f104: andeq r0, r0, r0, lsl #12
0034f108: subseq r1, r7, r0, lsr #13

# _ZNK15FileSystemWin3214backupSavefileEPKcS1_ 0034f10c size360
0034f10c: push {r4, r5, r6, r7, r8, sb, sl, lr}
0034f110: ldr r4, [pc, #0x140]
0034f114: ldr r3, [pc, #0x140]
0034f118: ldr r8, [pc, #0x140]
0034f11c: add r4, pc, r4
0034f120: ldr r0, [r4, r3]
0034f124: ldr r3, [r4, r8]
0034f128: ldr r5, [pc, #0x134]
0034f12c: ldr r7, [r0]
0034f130: sub sp, sp, #0x248
0034f134: ldr ip, [r3]
0034f138: add r5, pc, r5
0034f13c: add sl, sp, #0x110
0034f140: mov r3, r1
0034f144: mov sb, r2
0034f148: mov r1, r5
0034f14c: mov r2, r7
0034f150: add r6, sp, #0xc
0034f154: mov r0, sl
0034f158: str ip, [sp, #0x244]
0034f15c: bl #0x30eae4
0034f160: mov r1, r5
0034f164: mov r2, r7
0034f168: mov r3, sb
0034f16c: mov r0, r6
0034f170: bl #0x30eae4
0034f174: mov r0, r6
0034f178: bl #0x30e4a8
0034f17c: cmp r0, #0
0034f180: beq #0x34f1f0
0034f184: bl #0x30ddd0
0034f188: ldr r3, [r0]
0034f18c: cmp r3, #2
0034f190: beq #0x34f1f0
0034f194: ldr r3, [pc, #0xcc]
0034f198: add r5, sp, #0x22c
0034f19c: ldr r6, [r4, r3]
0034f1a0: mov r0, r6
0034f1a4: bl #0x337888
0034f1a8: ldr r1, [pc, #0xbc]
0034f1ac: add r2, sp, #8
0034f1b0: mov r0, r5
0034f1b4: add r1, pc, r1
0034f1b8: bl #0x3140ec
0034f1bc: mov r1, r5
0034f1c0: mov r0, r6
0034f1c4: bl #0x337a88
0034f1c8: mov r0, r5
0034f1cc: bl #0x318254
0034f1d0: mov r0, #0
0034f1d4: ldr r3, [r4, r8]
0034f1d8: ldr r2, [sp, #0x244]
0034f1dc: ldr r3, [r3]
0034f1e0: cmp r2, r3
0034f1e4: bne #0x34f254
0034f1e8: add sp, sp, #0x248
0034f1ec: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0034f1f0: mov r0, sl
0034f1f4: mov r1, r6
0034f1f8: bl #0x30e664
0034f1fc: cmp r0, #0
0034f200: moveq r0, #1
0034f204: beq #0x34f1d4
0034f208: bl #0x30ddd0
0034f20c: ldr r3, [r0]
0034f210: ldr r3, [pc, #0x50]
0034f214: add r5, sp, #0x214
0034f218: ldr r6, [r4, r3]
0034f21c: mov r0, r6
0034f220: bl #0x337888
0034f224: ldr r1, [pc, #0x44]
0034f228: add r2, sp, #4
0034f22c: mov r0, r5
0034f230: add r1, pc, r1
0034f234: bl #0x3140ec
0034f238: mov r1, r5
0034f23c: mov r0, r6
0034f240: bl #0x337a88
0034f244: mov r0, r5
0034f248: bl #0x318254
0034f24c: mov r0, #0
0034f250: b #0x34f1d4
0034f254: bl #0x30e310
0034f258: rsbeq r5, r4, r4, ror sb
0034f25c: andeq r0, r0, r0, lsl #12
0034f260: andeq r4, r0, ip, lsr #1
0034f264: ldrsbeq r1, [r7], #-0x90
0034f268: andeq r0, r0, r4, lsl #17
0034f26c: subseq r1, r7, ip, lsr #11
0034f270: subseq r1, r7, r0, lsr r5

# _ZNK15FileSystemWin3214deleteSavefileEPKc 0034f274 size216
0034f274: push {r4, r5, r6, r7, lr}
0034f278: ldr r4, [pc, #0xb4]
0034f27c: ldr r5, [pc, #0xb4]
0034f280: ldr r2, [pc, #0xb4]
0034f284: add r4, pc, r4
0034f288: ldr r3, [r4, r5]
0034f28c: ldr r2, [r4, r2]
0034f290: sub sp, sp, #0x12c
0034f294: ldr ip, [r3]
0034f298: mov r3, r1
0034f29c: ldr r1, [pc, #0x9c]
0034f2a0: add r6, sp, #8
0034f2a4: ldr r2, [r2]
0034f2a8: add r1, pc, r1
0034f2ac: mov r0, r6
0034f2b0: str ip, [sp, #0x124]
0034f2b4: bl #0x30eae4
0034f2b8: mov r0, r6
0034f2bc: bl #0x30e4a8
0034f2c0: cmp r0, #0
0034f2c4: moveq r0, #1
0034f2c8: beq #0x34f314
0034f2cc: bl #0x30ddd0
0034f2d0: ldr r3, [r0]
0034f2d4: ldr r3, [pc, #0x68]
0034f2d8: add r6, sp, #0x10c
0034f2dc: ldr r7, [r4, r3]
0034f2e0: mov r0, r7
0034f2e4: bl #0x337888
0034f2e8: ldr r1, [pc, #0x58]
0034f2ec: add r2, sp, #4
0034f2f0: mov r0, r6
0034f2f4: add r1, pc, r1
0034f2f8: bl #0x3140ec
0034f2fc: mov r1, r6
0034f300: mov r0, r7
0034f304: bl #0x337a88
0034f308: mov r0, r6
0034f30c: bl #0x318254
0034f310: mov r0, #0
0034f314: ldr r3, [r4, r5]
0034f318: ldr r2, [sp, #0x124]
0034f31c: ldr r3, [r3]
0034f320: cmp r2, r3
0034f324: bne #0x34f330
0034f328: add sp, sp, #0x12c
0034f32c: pop {r4, r5, r6, r7, pc}
0034f330: bl #0x30e310
0034f334: rsbeq r5, r4, ip, lsl #16
0034f338: andeq r4, r0, ip, lsr #1
0034f33c: andeq r0, r0, r0, lsl #12
0034f340: subseq r1, r7, r0, ror #16
0034f344: andeq r0, r0, r4, lsl #17
0034f348: subseq r1, r7, ip, ror #8

# _ZN15FileSystemWin3212makeFullPathEPKcS1_Pci 0034f34c size16
0034f34c: mov r0, r2
0034f350: mov r1, #0
0034f354: mov r2, r3
0034f358: b #0x30e460

# _ZN15FileSystemWin3212makeFullPathEPKwS1_Pwi 0034f35c size108
0034f35c: push {r4, r5, r6, lr}
0034f360: mov r4, r2
0034f364: mov r5, r1
0034f368: lsl r2, r3, #2
0034f36c: mov r1, #0
0034f370: mov r6, r0
0034f374: mov r0, r4
0034f378: bl #0x30e460
0034f37c: ldr r1, [pc, #0x40]
0034f380: mov r0, r5
0034f384: mov r2, #1
0034f388: add r1, pc, r1
0034f38c: bl #0x30e358
0034f390: cmp r0, #0
0034f394: beq #0x34f3b4
0034f398: mov r1, r6
0034f39c: mov r0, r4
0034f3a0: bl #0x30defc
0034f3a4: mov r0, r4
0034f3a8: mov r1, r5
0034f3ac: pop {r4, r5, r6, lr}
0034f3b0: b #0x30defc
0034f3b4: mov r0, r4
0034f3b8: add r1, r5, #4
0034f3bc: pop {r4, r5, r6, lr}
0034f3c0: b #0x30defc
0034f3c4: subseq r1, r7, r8, asr #7

# _ZN15FileSystemWin32D1Ev 0034f670 size52
0034f670: ldr r3, [pc, #0x24]
0034f674: ldr r2, [pc, #0x24]
0034f678: push {r4, lr}
0034f67c: add r3, pc, r3
0034f680: ldr r2, [r3, r2]
0034f684: mov r4, r0
0034f688: add r2, r2, #8
0034f68c: str r2, [r0]
0034f690: bl #0x34e4d0
0034f694: mov r0, r4
0034f698: pop {r4, pc}
0034f69c: rsbeq r5, r4, r4, lsl r4
0034f6a0: andeq r1, r0, r4, asr r6

# _ZN15FileSystemWin32D0Ev 0034f6a4 size28
0034f6a4: push {r4, lr}
0034f6a8: mov r4, r0
0034f6ac: bl #0x34f670
0034f6b0: mov r0, r4
0034f6b4: bl #0x310440
0034f6b8: mov r0, r4
0034f6bc: pop {r4, pc}

# _ZN15FileSystemWin32D2Ev 0034f6c0 size52
0034f6c0: ldr r3, [pc, #0x24]
0034f6c4: ldr r2, [pc, #0x24]
0034f6c8: push {r4, lr}
0034f6cc: add r3, pc, r3
0034f6d0: ldr r2, [r3, r2]
0034f6d4: mov r4, r0
0034f6d8: add r2, r2, #8
0034f6dc: str r2, [r0]
0034f6e0: bl #0x34e4d0
0034f6e4: mov r0, r4
0034f6e8: pop {r4, pc}
0034f6ec: rsbeq r5, r4, r4, asr #7
0034f6f0: andeq r1, r0, r4, asr r6

# _ZN15FileSystemWin32C1Ev 0034f6f4 size148
0034f6f4: push {r4, r5, r6, lr}
0034f6f8: ldr r6, [pc, #0x80]
0034f6fc: mov r4, r0
0034f700: bl #0x34e538
0034f704: ldr r3, [pc, #0x78]
0034f708: add r6, pc, r6
0034f70c: mov r5, #0x104
0034f710: ldr r3, [r6, r3]
0034f714: mov r0, r4
0034f718: mov r2, r5
0034f71c: add r3, r3, #8
0034f720: str r3, [r0], #0x2c
0034f724: mov r1, #0
0034f728: bl #0x30e460
0034f72c: mov r2, r5
0034f730: mov r1, #0
0034f734: add r0, r4, #0x130
0034f738: bl #0x30e460
0034f73c: mov r2, r5
0034f740: mov r1, #0
0034f744: add r0, r4, #0x234
0034f748: bl #0x30e460
0034f74c: mov r2, r5
0034f750: mov r1, #0
0034f754: add r0, r4, #0x338
0034f758: bl #0x30e460
0034f75c: add r0, r4, #0x430
0034f760: mov r2, r5
0034f764: mov r1, #0
0034f768: add r0, r0, #0xc
0034f76c: bl #0x30e460
0034f770: mov r0, r4
0034f774: bl #0x34f068
0034f778: mov r0, r4
0034f77c: pop {r4, r5, r6, pc}
0034f780: rsbeq r5, r4, r8, lsl #7
0034f784: andeq r1, r0, r4, asr r6

# _ZN15FileSystemWin32C2Ev 0034f788 size148
0034f788: push {r4, r5, r6, lr}
0034f78c: ldr r6, [pc, #0x80]
0034f790: mov r4, r0
0034f794: bl #0x34e538
0034f798: ldr r3, [pc, #0x78]
0034f79c: add r6, pc, r6
0034f7a0: mov r5, #0x104
0034f7a4: ldr r3, [r6, r3]
0034f7a8: mov r0, r4
0034f7ac: mov r2, r5
0034f7b0: add r3, r3, #8
0034f7b4: str r3, [r0], #0x2c
0034f7b8: mov r1, #0
0034f7bc: bl #0x30e460
0034f7c0: mov r2, r5
0034f7c4: mov r1, #0
0034f7c8: add r0, r4, #0x130
0034f7cc: bl #0x30e460
0034f7d0: mov r2, r5
0034f7d4: mov r1, #0
0034f7d8: add r0, r4, #0x234
0034f7dc: bl #0x30e460
0034f7e0: mov r2, r5
0034f7e4: mov r1, #0
0034f7e8: add r0, r4, #0x338
0034f7ec: bl #0x30e460
0034f7f0: add r0, r4, #0x430
0034f7f4: mov r2, r5
0034f7f8: mov r1, #0
0034f7fc: add r0, r0, #0xc
0034f800: bl #0x30e460
0034f804: mov r0, r4
0034f808: bl #0x34f068
0034f80c: mov r0, r4
0034f810: pop {r4, r5, r6, pc}

# _GLOBAL__I_.._.._sources_Core_Irrlicht_FileSystemWin32.cpp 0034f81c size136
0034f81c: push {r4, r5, r6, lr}
0034f820: ldr r4, [pc, #0x64]
0034f824: ldr r2, [pc, #0x64]
0034f828: ldr r3, [pc, #0x64]
0034f82c: add r4, pc, r4
0034f830: ldr r1, [r4, r2]
0034f834: add r3, pc, r3
0034f838: mov r2, #0x3f000000
0034f83c: ldr r0, [r1]
0034f840: str r2, [r3, #8]
0034f844: str r2, [r3]
0034f848: tst r0, #1
0034f84c: str r2, [r3, #4]
0034f850: beq #0x34f858
0034f854: pop {r4, r5, r6, pc}
0034f858: mov r3, #1
0034f85c: str r3, [r1]
0034f860: ldr r3, [pc, #0x30]
0034f864: ldr r5, [r4, r3]
0034f868: mov r0, r5
0034f86c: bl #0x32d79c
0034f870: ldr r3, [pc, #0x24]
0034f874: mov r0, r5
0034f878: ldr r1, [r4, r3]
0034f87c: ldr r3, [pc, #0x1c]
0034f880: ldr r2, [r4, r3]
0034f884: pop {r4, r5, r6, lr}
0034f888: b #0x30e304
0034f88c: rsbeq r5, r4, r4, ror #4
0034f890: andeq r0, r0, ip, lsr #31
0034f894: rsbeq r2, r5, r4, lsl #13
0034f898: strdeq r3, r4, [r0], -r4
0034f89c: andeq r0, r0, r0, asr #17
0034f8a0: muleq r0, r0, r8

# _ZNK15FileSystemWin3214formatFilePathERKSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS2_6memory13E_MEMORY_HINTE0EEEE 0034f8a4 size728
0034f8a4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034f8a8: mov r4, r0
0034f8ac: str r0, [r4, #0x10]
0034f8b0: str r0, [r4, #0x14]
0034f8b4: mov r3, r2
0034f8b8: sub sp, sp, #0x2c
0034f8bc: ldr r1, [r3, #0x14]
0034f8c0: ldr r2, [r2, #0x10]
0034f8c4: bl #0x325ff4
0034f8c8: ldr r3, [pc, #0x28c]
0034f8cc: ldr r1, [pc, #0x28c]
0034f8d0: ldr r2, [pc, #0x28c]
0034f8d4: add r3, pc, r3
0034f8d8: add r1, pc, r1
0034f8dc: str r1, [sp, #0x10]
0034f8e0: add r3, r3, #1
0034f8e4: ldr fp, [pc, #0x27c]
0034f8e8: str r3, [sp, #0x1c]
0034f8ec: ldr r3, [sp, #0x10]
0034f8f0: add r2, pc, r2
0034f8f4: add r2, r2, #1
0034f8f8: add fp, pc, fp
0034f8fc: str r2, [sp, #0xc]
0034f900: add r3, r3, #1
0034f904: add r2, fp, #1
0034f908: ldr r1, [r4, #0x10]
0034f90c: ldr r0, [r4, #0x14]
0034f910: mov r7, #0
0034f914: str r2, [sp, #0x14]
0034f918: str r3, [sp, #0x18]
0034f91c: rsb r3, r0, r1
0034f920: cmp r7, r3
0034f924: blo #0x34f934
0034f928: mov r0, r4
0034f92c: add sp, sp, #0x2c
0034f930: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034f934: add r2, sp, #0x28
0034f938: mov r3, #0x2f
0034f93c: strb r3, [r2, #-4]!
0034f940: add r0, r0, r7
0034f944: add r3, sp, #0x20
0034f948: bl #0x34ec04
0034f94c: ldr r2, [r4, #0x10]
0034f950: cmp r0, r2
0034f954: beq #0x34f928
0034f958: ldr r5, [r4, #0x14]
0034f95c: rsb r3, r5, r0
0034f960: cmn r3, #1
0034f964: beq #0x34f928
0034f968: rsb r5, r5, r2
0034f96c: cmp r3, r5
0034f970: add r7, r3, #1
0034f974: bhi #0x34fae0
0034f978: movw r2, #0xfffe
0034f97c: movt r2, #0xffff
0034f980: rsb r8, r3, r5
0034f984: cmp r8, #1
0034f988: movhs r8, #1
0034f98c: rsb r2, r5, r2
0034f990: add r2, r2, r8
0034f994: cmp r2, #0
0034f998: beq #0x34fac8
0034f99c: ldr r2, [pc, #0x1c8]
0034f9a0: ldr r5, [r4, #0x14]
0034f9a4: add r8, r8, r3
0034f9a8: add r2, pc, r2
0034f9ac: cmp r5, r2
0034f9b0: add r8, r5, r8
0034f9b4: add r6, r5, r3
0034f9b8: movhi sb, #0
0034f9bc: bhi #0x34f9d0
0034f9c0: ldr sb, [r4, #0x10]
0034f9c4: cmp sb, r2
0034f9c8: movls sb, #0
0034f9cc: movhi sb, #1
0034f9d0: rsb sl, r6, r8
0034f9d4: cmp sl, #0
0034f9d8: ble #0x34fa24
0034f9dc: add r6, r6, #1
0034f9e0: mov r2, #0x5c
0034f9e4: cmp r8, r6
0034f9e8: strb r2, [r5, r3]
0034f9ec: beq #0x34fa7c
0034f9f0: ldr r3, [r4, #0x10]
0034f9f4: add r2, r3, #1
0034f9f8: subs r2, r2, r8
0034f9fc: beq #0x34fa10
0034fa00: mov r0, r6
0034fa04: mov r1, r8
0034fa08: bl #0x30df38
0034fa0c: ldr r3, [r4, #0x10]
0034fa10: rsb r6, r8, r6
0034fa14: add r1, r3, r6
0034fa18: str r1, [r4, #0x10]
0034fa1c: ldr r0, [r4, #0x14]
0034fa20: b #0x34f91c
0034fa24: cmp sb, #0
0034fa28: beq #0x34fa88
0034fa2c: ldr r2, [sp, #0x14]
0034fa30: cmp r6, r2
0034fa34: movlo r3, #0
0034fa38: movhs r3, #1
0034fa3c: cmp r8, fp
0034fa40: orrls r3, r3, #1
0034fa44: cmp r3, #0
0034fa48: bne #0x34fa88
0034fa4c: cmp r6, fp
0034fa50: bhi #0x34faf8
0034fa54: cmp sl, #0
0034fa58: add r5, sl, fp
0034fa5c: bne #0x34fb48
0034fa60: mov ip, #1
0034fa64: mov r1, r8
0034fa68: mov r2, r5
0034fa6c: mov r0, r4
0034fa70: ldr r3, [sp, #0x1c]
0034fa74: str ip, [sp]
0034fa78: bl #0x34f3c8
0034fa7c: ldr r1, [r4, #0x10]
0034fa80: ldr r0, [r4, #0x14]
0034fa84: b #0x34f91c
0034fa88: ldr r1, [pc, #0xe0]
0034fa8c: add r1, pc, r1
0034fa90: add sl, sl, r1
0034fa94: subs r2, sl, fp
0034fa98: beq #0x34faa4
0034fa9c: mov r0, r6
0034faa0: bl #0x30e868
0034faa4: mov r1, r8
0034faa8: mov r0, r4
0034faac: mov r2, sl
0034fab0: ldr r3, [sp, #0xc]
0034fab4: str sb, [sp]
0034fab8: bl #0x34f3c8
0034fabc: ldr r1, [r4, #0x10]
0034fac0: ldr r0, [r4, #0x14]
0034fac4: b #0x34f91c
0034fac8: ldr r0, [pc, #0xa4]
0034facc: str r3, [sp, #8]
0034fad0: add r0, pc, r0
0034fad4: bl #0x708e40
0034fad8: ldr r3, [sp, #8]
0034fadc: b #0x34f99c
0034fae0: ldr r0, [pc, #0x90]
0034fae4: str r3, [sp, #8]
0034fae8: add r0, pc, r0
0034faec: bl #0x708eb0
0034faf0: ldr r3, [sp, #8]
0034faf4: b #0x34f978
0034faf8: mov r1, r8
0034fafc: mov r0, r4
0034fb00: mov ip, #1
0034fb04: add r2, sl, fp
0034fb08: ldr r3, [sp, #0x14]
0034fb0c: str ip, [sp]
0034fb10: bl #0x34f3c8
0034fb14: cmp sl, #0
0034fb18: ldr r0, [r4, #0x14]
0034fb1c: ldreq r1, [r4, #0x10]
0034fb20: beq #0x34f91c
0034fb24: rsb r6, r5, r6
0034fb28: rsb r1, r5, fp
0034fb2c: add r1, r0, r1
0034fb30: mov r2, sl
0034fb34: add r0, r0, r6
0034fb38: bl #0x30df38
0034fb3c: ldr r1, [r4, #0x10]
0034fb40: ldr r0, [r4, #0x14]
0034fb44: b #0x34f91c
0034fb48: mov r0, r6
0034fb4c: mov r2, sl
0034fb50: mov r1, fp
0034fb54: bl #0x30e868
0034fb58: b #0x34fa60
0034fb5c: subseq r0, r7, r4, lsr #29
0034fb60: subseq r0, r7, r0, lsr #29
0034fb64: subseq r0, r7, r8, lsl #29
0034fb68: subseq r0, r7, r0, lsl #29
0034fb6c: ldrsbeq r0, [r7], #-0xd0
0034fb70: subseq r0, r7, ip, ror #25
0034fb74: subseq lr, r6, r8, lsl #19
0034fb78: subseq lr, r6, r0, ror sb

# _ZN15FileSystemWin3211_FileHandleD1Ev 0034fb7c size64
0034fb7c: push {r4, lr}
0034fb80: ldr r3, [pc, #0x2c]
0034fb84: ldr r2, [pc, #0x2c]
0034fb88: mov r4, r0
0034fb8c: add r3, pc, r3
0034fb90: ldr r0, [r0, #8]
0034fb94: ldr r2, [r3, r2]
0034fb98: cmp r0, #0
0034fb9c: add r2, r2, #8
0034fba0: str r2, [r4]
0034fba4: beq #0x34fbac
0034fba8: bl #0x34f038
0034fbac: mov r0, r4
0034fbb0: pop {r4, pc}
0034fbb4: rsbeq r4, r4, r4, lsl #30
0034fbb8: andeq r1, r0, r0, lsl #24

# _ZN15FileSystemWin3211_FileHandleD0Ev 0034fbbc size28
0034fbbc: push {r4, lr}
0034fbc0: mov r4, r0
0034fbc4: bl #0x34fb7c
0034fbc8: mov r0, r4
0034fbcc: bl #0x310440
0034fbd0: mov r0, r4
0034fbd4: pop {r4, pc}

# _ZN15FileSystemWin3211_FileHandleD2Ev 0034fbd8 size64
0034fbd8: push {r4, lr}
0034fbdc: ldr r3, [pc, #0x2c]
0034fbe0: ldr r2, [pc, #0x2c]
0034fbe4: mov r4, r0
0034fbe8: add r3, pc, r3
0034fbec: ldr r0, [r0, #8]
0034fbf0: ldr r2, [r3, r2]
0034fbf4: cmp r0, #0
0034fbf8: add r2, r2, #8
0034fbfc: str r2, [r4]
0034fc00: beq #0x34fc08
0034fc04: bl #0x34f038
0034fc08: mov r0, r4
0034fc0c: pop {r4, pc}
0034fc10: rsbeq r4, r4, r8, lsr #29
0034fc14: andeq r1, r0, r0, lsl #24

# _ZN15FileSystemWin3211_FileHandleC1EPKcS2_bb 0034fc18 size568
0034fc18: push {r4, r5, r6, r7, r8, sb, sl, lr}
0034fc1c: ldr r5, [pc, #0x204]
0034fc20: ldr r7, [pc, #0x204]
0034fc24: ldr lr, [pc, #0x204]
0034fc28: add r5, pc, r5
0034fc2c: ldr ip, [r5, r7]
0034fc30: ldr lr, [r5, lr]
0034fc34: sub sp, sp, #0x118
0034fc38: ldr ip, [ip]
0034fc3c: add lr, lr, #8
0034fc40: str lr, [r0]
0034fc44: mov r4, r0
0034fc48: add r6, sp, #0x10
0034fc4c: mov r0, #0
0034fc50: mov sl, r2
0034fc54: str r0, [r4, #8]
0034fc58: mov r0, r6
0034fc5c: str ip, [sp, #0x114]
0034fc60: mov r8, r3
0034fc64: ldrb sb, [sp, #0x138]
0034fc68: bl #0x30e520
0034fc6c: mov r1, sl
0034fc70: mov r0, r6
0034fc74: bl #0x30ed90
0034fc78: ldr r1, [pc, #0x1b4]
0034fc7c: mov r0, sl
0034fc80: add r1, pc, r1
0034fc84: bl #0x30ebd4
0034fc88: cmp r0, #0
0034fc8c: beq #0x34fcb0
0034fc90: ldr r2, [pc, #0x1a0]
0034fc94: ldr r1, [pc, #0x1a0]
0034fc98: mov r3, sl
0034fc9c: ldr r2, [r5, r2]
0034fca0: add r1, pc, r1
0034fca4: mov r0, r6
0034fca8: ldr r2, [r2]
0034fcac: bl #0x30eae4
0034fcb0: ldr r3, [pc, #0x188]
0034fcb4: ldr r3, [r5, r3]
0034fcb8: ldrb r3, [r3, #0xa8]
0034fcbc: cmp r3, #0
0034fcc0: bne #0x34fe10
0034fcc4: cmp r8, #0
0034fcc8: beq #0x34fdb0
0034fccc: cmp sb, #0
0034fcd0: beq #0x34fd64
0034fcd4: ldr r2, [pc, #0x168]
0034fcd8: add r0, sp, #0xc
0034fcdc: mov r1, r6
0034fce0: add r2, pc, r2
0034fce4: bl #0x56dc40
0034fce8: ldr r3, [sp, #0xc]
0034fcec: cmp r3, #0
0034fcf0: ldrne r2, [r3]
0034fcf4: addne r2, r2, #1
0034fcf8: strne r2, [r3]
0034fcfc: ldr r0, [r4, #8]
0034fd00: str r3, [r4, #8]
0034fd04: cmp r0, #0
0034fd08: beq #0x34fd10
0034fd0c: bl #0x34f038
0034fd10: ldr r0, [sp, #0xc]
0034fd14: cmp r0, #0
0034fd18: beq #0x34fd20
0034fd1c: bl #0x34f038
0034fd20: mov r3, #1
0034fd24: strb r3, [r4, #0xd]
0034fd28: strb r3, [r4, #0xc]
0034fd2c: ldr r3, [r4, #8]
0034fd30: mov r2, #0
0034fd34: str r2, [r4, #4]
0034fd38: cmp r3, r2
0034fd3c: ldrne r3, [r3, #4]
0034fd40: mov r0, r4
0034fd44: strne r3, [r4, #4]
0034fd48: ldr r3, [r5, r7]
0034fd4c: ldr r2, [sp, #0x114]
0034fd50: ldr r3, [r3]
0034fd54: cmp r2, r3
0034fd58: bne #0x34fe24
0034fd5c: add sp, sp, #0x118
0034fd60: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0034fd64: ldr r2, [pc, #0xdc]
0034fd68: add r0, sp, #8
0034fd6c: mov r1, r6
0034fd70: add r2, pc, r2
0034fd74: bl #0x56dc40
0034fd78: ldr r3, [sp, #8]
0034fd7c: cmp r3, #0
0034fd80: ldrne r2, [r3]
0034fd84: addne r2, r2, #1
0034fd88: strne r2, [r3]
0034fd8c: ldr r0, [r4, #8]
0034fd90: str r3, [r4, #8]
0034fd94: cmp r0, #0
0034fd98: beq #0x34fda0
0034fd9c: bl #0x34f038
0034fda0: ldr r0, [sp, #8]
0034fda4: cmp r0, #0
0034fda8: bne #0x34fd1c
0034fdac: b #0x34fd20
0034fdb0: ldr r2, [pc, #0x94]
0034fdb4: add r0, sp, #4
0034fdb8: mov r1, r6
0034fdbc: add r2, pc, r2
0034fdc0: bl #0x56dc40
0034fdc4: ldr r3, [sp, #4]
0034fdc8: cmp r3, #0
0034fdcc: ldrne r2, [r3]
0034fdd0: addne r2, r2, #1
0034fdd4: strne r2, [r3]
0034fdd8: ldr r0, [r4, #8]
0034fddc: str r3, [r4, #8]
0034fde0: cmp r0, #0
0034fde4: beq #0x34fdec
0034fde8: bl #0x34f038
0034fdec: ldr r0, [sp, #4]
0034fdf0: cmp r0, #0
0034fdf4: beq #0x34fdfc
0034fdf8: bl #0x34f038
0034fdfc: mov r3, #1
0034fe00: strb r3, [r4, #0xc]
0034fe04: mov r3, #0
0034fe08: strb r3, [r4, #0xd]
0034fe0c: b #0x34fd2c
0034fe10: mov r0, r6
0034fe14: mov r1, #0
0034fe18: mvn r2, #0
0034fe1c: bl #0x34e414
0034fe20: b #0x34fcc4
0034fe24: bl #0x30e310
0034fe28: rsbeq r4, r4, r8, ror #28
0034fe2c: andeq r4, r0, ip, lsr #1
0034fe30: andeq r1, r0, r0, lsl #24
0034fe34: subseq r0, r7, r0, lsl #22
0034fe38: andeq r0, r0, r0, lsl #12
0034fe3c: subseq r0, r7, r8, ror #28
0034fe40: strdeq r3, r4, [r0], -r4
0034fe44: ldrheq r0, [r7], #-0xa0
0034fe48: subseq r0, r7, r8, lsr #20
0034fe4c: subseq r0, r7, r4, ror #19

# _ZN15FileSystemWin3213_createHandleEPKcS1_bb 0034fe50 size148
0034fe50: push {r4, r5, r6, r7, r8, sl, lr}
0034fe54: mov r4, r1
0034fe58: ldrsb r1, [r1]
0034fe5c: sub sp, sp, #0xc
0034fe60: mov r8, r2
0034fe64: cmp r1, #0x2e
0034fe68: mov r7, r3
0034fe6c: ldrb r6, [sp, #0x28]
0034fe70: beq #0x34feb4
0034fe74: mov sl, #0
0034fe78: mov r1, #0
0034fe7c: mov r0, #0x10
0034fe80: bl #0x310570
0034fe84: add r1, r4, sl
0034fe88: mov r5, r0
0034fe8c: mov r2, r8
0034fe90: mov r3, r7
0034fe94: str r6, [sp]
0034fe98: bl #0x34fc18
0034fe9c: ldr r4, [r5, #4]
0034fea0: cmp r4, #0
0034fea4: movne r0, r5
0034fea8: beq #0x34fecc
0034feac: add sp, sp, #0xc
0034feb0: pop {r4, r5, r6, r7, r8, sl, pc}
0034feb4: ldrsb r3, [r4, #1]
0034feb8: cmp r3, #0x2f
0034febc: cmpne r3, #0x5c
0034fec0: moveq sl, #2
0034fec4: beq #0x34fe78
0034fec8: b #0x34fe74
0034fecc: mov r0, r5
0034fed0: ldr r3, [r5]
0034fed4: mov lr, pc
0034fed8: ldr pc, [r3, #4]
0034fedc: mov r0, r4
0034fee0: b #0x34feac

# _ZN15FileSystemWin3212openSavefileEPKcb 0034fee4 size68
0034fee4: push {r4, r5, r6, lr}
0034fee8: mov r6, r0
0034feec: sub sp, sp, #8
0034fef0: mov r0, r1
0034fef4: mov r4, r1
0034fef8: mov r5, r2
0034fefc: bl #0x314cfc
0034ff00: add r1, r6, #0x430
0034ff04: mov ip, #0
0034ff08: mov r0, r6
0034ff0c: add r1, r1, #0xc
0034ff10: mov r2, r4
0034ff14: mov r3, r5
0034ff18: str ip, [sp]
0034ff1c: bl #0x34fe50
0034ff20: add sp, sp, #8
0034ff24: pop {r4, r5, r6, pc}

# _ZN15FileSystemWin3212openResourceEPKc 0034ff28 size92
0034ff28: push {r4, r5, lr}
0034ff2c: mov ip, #0
0034ff30: mov r4, r1
0034ff34: sub sp, sp, #0xc
0034ff38: mov r3, ip
0034ff3c: add r1, r0, #0x234
0034ff40: mov r2, r4
0034ff44: str ip, [sp]
0034ff48: mov r5, r0
0034ff4c: bl #0x34fe50
0034ff50: subs ip, r0, #0
0034ff54: beq #0x34ff64
0034ff58: mov r0, ip
0034ff5c: add sp, sp, #0xc
0034ff60: pop {r4, r5, pc}
0034ff64: mov r3, ip
0034ff68: mov r0, r5
0034ff6c: mov r2, r4
0034ff70: add r1, r5, #0x338
0034ff74: str ip, [sp]
0034ff78: bl #0x34fe50
0034ff7c: mov ip, r0
0034ff80: b #0x34ff58

# _ZN15FileSystemWin3213openTraceFileEPKc 0034ff84 size44
0034ff84: str lr, [sp, #-4]!
0034ff88: mov ip, #1
0034ff8c: mov r2, r1
0034ff90: add r1, r0, #0x430
0034ff94: sub sp, sp, #0xc
0034ff98: add r1, r1, #0xc
0034ff9c: mov r3, ip
0034ffa0: str ip, [sp]
0034ffa4: bl #0x34fe50
0034ffa8: add sp, sp, #0xc
0034ffac: ldm sp!, {pc}

# _ZN15FileSystemWin328openFileEPKcbb 0034ffb0 size140
0034ffb0: push {r4, r5, r6, r7, lr}
0034ffb4: ldr ip, [pc, #0x74]
0034ffb8: mov r5, r0
0034ffbc: ldr r0, [pc, #0x70]
0034ffc0: add ip, pc, ip
0034ffc4: sub sp, sp, #0xc
0034ffc8: ldr r0, [ip, r0]
0034ffcc: mov r7, r2
0034ffd0: mov r6, r3
0034ffd4: mov r4, r1
0034ffd8: bl #0x320678
0034ffdc: cmp r0, #0
0034ffe0: beq #0x350014
0034ffe4: ldr r1, [pc, #0x4c]
0034ffe8: mov ip, #0
0034ffec: mov r3, ip
0034fff0: add r1, pc, r1
0034fff4: mov r0, r5
0034fff8: mov r2, r4
0034fffc: str ip, [sp]
00350000: bl #0x34fe50
00350004: cmp r0, #0
00350008: beq #0x350014
0035000c: add sp, sp, #0xc
00350010: pop {r4, r5, r6, r7, pc}
00350014: mov r0, r5
00350018: mov r2, r4
0035001c: mov r3, r7
00350020: add r1, r5, #0x2c
00350024: str r6, [sp]
00350028: bl #0x34fe50
0035002c: b #0x35000c

# _ZN15FileSystemWin3211_FileHandleC2EPKcS2_bb 0035003c size568
0035003c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00350040: ldr r5, [pc, #0x204]
00350044: ldr r7, [pc, #0x204]
00350048: ldr lr, [pc, #0x204]
0035004c: add r5, pc, r5
00350050: ldr ip, [r5, r7]
00350054: ldr lr, [r5, lr]
00350058: sub sp, sp, #0x118
0035005c: ldr ip, [ip]
00350060: add lr, lr, #8
00350064: str lr, [r0]
00350068: mov r4, r0
0035006c: add r6, sp, #0x10
00350070: mov r0, #0
00350074: mov sl, r2
00350078: str r0, [r4, #8]
0035007c: mov r0, r6
00350080: str ip, [sp, #0x114]
00350084: mov r8, r3
00350088: ldrb sb, [sp, #0x138]
0035008c: bl #0x30e520
00350090: mov r1, sl
00350094: mov r0, r6
00350098: bl #0x30ed90
0035009c: ldr r1, [pc, #0x1b4]
003500a0: mov r0, sl
003500a4: add r1, pc, r1
003500a8: bl #0x30ebd4
003500ac: cmp r0, #0
003500b0: beq #0x3500d4
003500b4: ldr r2, [pc, #0x1a0]
003500b8: ldr r1, [pc, #0x1a0]
003500bc: mov r3, sl
003500c0: ldr r2, [r5, r2]
003500c4: add r1, pc, r1
003500c8: mov r0, r6
003500cc: ldr r2, [r2]
003500d0: bl #0x30eae4
003500d4: ldr r3, [pc, #0x188]
003500d8: ldr r3, [r5, r3]
003500dc: ldrb r3, [r3, #0xa8]
003500e0: cmp r3, #0
003500e4: bne #0x350234
003500e8: cmp r8, #0
003500ec: beq #0x3501d4
003500f0: cmp sb, #0
003500f4: beq #0x350188
003500f8: ldr r2, [pc, #0x168]
003500fc: add r0, sp, #0xc
00350100: mov r1, r6
00350104: add r2, pc, r2
00350108: bl #0x56dc40
0035010c: ldr r3, [sp, #0xc]
00350110: cmp r3, #0
00350114: ldrne r2, [r3]
00350118: addne r2, r2, #1
0035011c: strne r2, [r3]
00350120: ldr r0, [r4, #8]
00350124: str r3, [r4, #8]
00350128: cmp r0, #0
0035012c: beq #0x350134
00350130: bl #0x34f038
00350134: ldr r0, [sp, #0xc]
00350138: cmp r0, #0
0035013c: beq #0x350144
00350140: bl #0x34f038
00350144: mov r3, #1
00350148: strb r3, [r4, #0xd]
0035014c: strb r3, [r4, #0xc]
00350150: ldr r3, [r4, #8]
00350154: mov r2, #0
00350158: str r2, [r4, #4]
0035015c: cmp r3, r2
00350160: ldrne r3, [r3, #4]
00350164: mov r0, r4
00350168: strne r3, [r4, #4]
0035016c: ldr r3, [r5, r7]
00350170: ldr r2, [sp, #0x114]
00350174: ldr r3, [r3]
00350178: cmp r2, r3
0035017c: bne #0x350248
00350180: add sp, sp, #0x118
00350184: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00350188: ldr r2, [pc, #0xdc]
0035018c: add r0, sp, #8
00350190: mov r1, r6
00350194: add r2, pc, r2
00350198: bl #0x56dc40
0035019c: ldr r3, [sp, #8]
003501a0: cmp r3, #0
003501a4: ldrne r2, [r3]
003501a8: addne r2, r2, #1
003501ac: strne r2, [r3]
003501b0: ldr r0, [r4, #8]
003501b4: str r3, [r4, #8]
003501b8: cmp r0, #0
003501bc: beq #0x3501c4
003501c0: bl #0x34f038
003501c4: ldr r0, [sp, #8]
003501c8: cmp r0, #0
003501cc: bne #0x350140
003501d0: b #0x350144
003501d4: ldr r2, [pc, #0x94]
003501d8: add r0, sp, #4
003501dc: mov r1, r6
003501e0: add r2, pc, r2
003501e4: bl #0x56dc40
003501e8: ldr r3, [sp, #4]
003501ec: cmp r3, #0
003501f0: ldrne r2, [r3]
003501f4: addne r2, r2, #1
003501f8: strne r2, [r3]
003501fc: ldr r0, [r4, #8]
00350200: str r3, [r4, #8]
00350204: cmp r0, #0
00350208: beq #0x350210
0035020c: bl #0x34f038
00350210: ldr r0, [sp, #4]
00350214: cmp r0, #0
00350218: beq #0x350220
0035021c: bl #0x34f038
00350220: mov r3, #1
00350224: strb r3, [r4, #0xc]
00350228: mov r3, #0
0035022c: strb r3, [r4, #0xd]
00350230: b #0x350150
00350234: mov r0, r6
00350238: mov r1, #0
0035023c: mvn r2, #0
00350240: bl #0x34e414
00350244: b #0x3500e8
00350248: bl #0x30e310
0035024c: rsbeq r4, r4, r4, asr #20
00350250: andeq r4, r0, ip, lsr #1
00350254: andeq r1, r0, r0, lsl #24
00350258: ldrsbeq r0, [r7], #-0x6c
0035025c: andeq r0, r0, r0, lsl #12
00350260: subseq r0, r7, r4, asr #20
00350264: strdeq r3, r4, [r0], -r4
00350268: subseq r0, r7, ip, lsl #13
0035026c: subseq r0, r7, r4, lsl #12
00350270: subseq r0, r7, r0, asr #11

# _ZNK15FileSystemWin3216getFilesMatchingEPKcS1_RSt6vectorISsSaISsEE 0035041c size388
0035041c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00350420: ldr sb, [pc, #0x170]
00350424: ldr r0, [pc, #0x170]
00350428: sub sp, sp, #0xf4
0035042c: add sb, pc, sb
00350430: str r0, [sp, #0xc]
00350434: ldr r0, [sb, r0]
00350438: add ip, sp, #0xd4
0035043c: str ip, [sp, #0x10]
00350440: ldr ip, [r0]
00350444: mov r4, r1
00350448: ldr r0, [sp, #0x10]
0035044c: mov r1, r2
00350450: add fp, sp, #0xbc
00350454: add r2, sp, #0x24
00350458: str ip, [sp, #0xec]
0035045c: mov sl, r3
00350460: bl #0x3140ec
00350464: mov r1, r4
00350468: add r2, sp, #0x20
0035046c: mov r0, fp
00350470: bl #0x3140ec
00350474: mov r0, r4
00350478: bl #0x30eaa8
0035047c: subs r7, r0, #0
00350480: beq #0x350564
00350484: add r0, sp, #0x18
00350488: add r6, sp, #0x28
0035048c: add r5, sp, #0xa4
00350490: add r8, sp, #0x1c
00350494: add r4, sp, #0x8c
00350498: str r0, [sp, #0x14]
0035049c: b #0x3504c4
003504a0: cmp r3, #0
003504a4: bne #0x3504b4
003504a8: mov r0, sl
003504ac: mov r1, r5
003504b0: bl #0x32bad8
003504b4: mov r0, r4
003504b8: bl #0x318254
003504bc: mov r0, r5
003504c0: bl #0x318254
003504c4: mov r0, r7
003504c8: bl #0x30eb80
003504cc: cmp r0, #0
003504d0: beq #0x35055c
003504d4: add r1, r0, #0x13
003504d8: mov r0, r6
003504dc: bl #0x30e520
003504e0: mov r1, r6
003504e4: mov r2, r8
003504e8: mov r0, r5
003504ec: bl #0x3140ec
003504f0: mov r0, r4
003504f4: ldr r1, [sp, #0xb8]
003504f8: ldr r2, [sp, #0xb4]
003504fc: str r4, [sp, #0x9c]
00350500: str r4, [sp, #0xa0]
00350504: bl #0x3116e8
00350508: ldr r1, [sp, #0x9c]
0035050c: ldr r0, [sp, #0xa0]
00350510: ldr r2, [sp, #0xe8]
00350514: ldr r3, [sp, #0xe4]
00350518: subs ip, r1, r0
0035051c: rsb r3, r2, r3
00350520: beq #0x3504a0
00350524: cmp r3, ip
00350528: bhi #0x3504b4
0035052c: ldr ip, [sp, #0x14]
00350530: add r3, r2, r3
00350534: str ip, [sp]
00350538: bl #0x34ed2c
0035053c: ldr r3, [sp, #0x9c]
00350540: cmp r0, r3
00350544: beq #0x3504b4
00350548: ldr r3, [sp, #0xa0]
0035054c: rsb r0, r3, r0
00350550: cmn r0, #1
00350554: bne #0x3504a8
00350558: b #0x3504b4
0035055c: mov r0, r7
00350560: bl #0x30e9b8
00350564: mov r0, fp
00350568: bl #0x318254
0035056c: ldr r0, [sp, #0x10]
00350570: bl #0x318254
00350574: ldr r0, [sp, #0xc]
00350578: ldr r2, [sp, #0xec]
0035057c: ldr r3, [sb, r0]
00350580: ldr r3, [r3]
00350584: cmp r2, r3
00350588: bne #0x350594
0035058c: add sp, sp, #0xf4
00350590: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00350594: bl #0x30e310
00350598: rsbeq r4, r4, r4, ror #12
0035059c: andeq r4, r0, ip, lsr #1

# _ZNK15FileSystemWin328getFilesEPKcRSt6vectorISsSaISsEE 003505a0 size260
003505a0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003505a4: ldr sb, [pc, #0xec]
003505a8: ldr r0, [pc, #0xec]
003505ac: sub sp, sp, #0xc4
003505b0: add sb, pc, sb
003505b4: ldr r3, [sb, r0]
003505b8: add fp, sp, #0xa4
003505bc: str r0, [sp, #4]
003505c0: ldr r3, [r3]
003505c4: mov sl, r2
003505c8: mov r0, fp
003505cc: add r2, sp, #0xc
003505d0: str r3, [sp, #0xbc]
003505d4: bl #0x3140ec
003505d8: ldr r3, [pc, #0xc0]
003505dc: ldr r3, [sb, r3]
003505e0: ldr r0, [r3]
003505e4: bl #0x30eaa8
003505e8: subs r7, r0, #0
003505ec: beq #0x35066c
003505f0: add r6, sp, #0x10
003505f4: add r5, sp, #0x8c
003505f8: add r8, sp, #8
003505fc: add r4, sp, #0x74
00350600: b #0x350654
00350604: add r1, r0, #0x13
00350608: mov r0, r6
0035060c: bl #0x30e520
00350610: mov r1, r6
00350614: mov r2, r8
00350618: mov r0, r5
0035061c: bl #0x3140ec
00350620: ldr r2, [sp, #0x9c]
00350624: mov r0, r4
00350628: ldr r1, [sp, #0xa0]
0035062c: str r4, [sp, #0x84]
00350630: str r4, [sp, #0x88]
00350634: bl #0x3116e8
00350638: mov r0, sl
0035063c: mov r1, r5
00350640: bl #0x32bad8
00350644: mov r0, r4
00350648: bl #0x318254
0035064c: mov r0, r5
00350650: bl #0x318254
00350654: mov r0, r7
00350658: bl #0x30eb80
0035065c: cmp r0, #0
00350660: bne #0x350604
00350664: mov r0, r7
00350668: bl #0x30e9b8
0035066c: mov r0, fp
00350670: bl #0x318254
00350674: ldr r2, [sp, #4]
00350678: ldr r3, [sb, r2]
0035067c: ldr r2, [sp, #0xbc]
00350680: ldr r3, [r3]
00350684: cmp r2, r3
00350688: bne #0x350694
0035068c: add sp, sp, #0xc4
00350690: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00350694: bl #0x30e310
00350698: rsbeq r4, r4, r0, ror #9
0035069c: andeq r4, r0, ip, lsr #1
003506a0: andeq r0, r0, r0, lsl #12

# _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE8_M_eraseEPNS_18_Rb_tree_node_baseE 00379fa8 size64
00379fa8: push {r4, r5, r6, lr}
00379fac: subs r4, r1, #0
00379fb0: mov r6, r0
00379fb4: beq #0x379fe4
00379fb8: ldr r1, [r4, #0xc]
00379fbc: mov r0, r6
00379fc0: bl #0x379fa8
00379fc4: ldr r5, [r4, #8]
00379fc8: add r0, r4, #0x10
00379fcc: bl #0x3139ac
00379fd0: mov r0, r4
00379fd4: mov r1, #0x2c
00379fd8: bl #0x708f00
00379fdc: subs r4, r5, #0
00379fe0: bne #0x379fb8
00379fe4: pop {r4, r5, r6, pc}

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_ 0037a300 size368
0037a300: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a304: ldr fp, [pc, #0x15c]
0037a308: ldr r2, [pc, #0x15c]
0037a30c: sub sp, sp, #0x54
0037a310: add fp, pc, fp
0037a314: ldr r3, [fp, r2]
0037a318: str r2, [sp, #0xc]
0037a31c: str r0, [sp, #8]
0037a320: ldr r4, [r0, #4]
0037a324: ldr r3, [r3]
0037a328: mov r8, r1
0037a32c: cmp r4, #0
0037a330: str r3, [sp, #0x4c]
0037a334: beq #0x37a43c
0037a338: mov sl, r0
0037a33c: add r7, sp, #0x34
0037a340: add sb, sp, #0x18
0037a344: ldr r1, [r8]
0037a348: mov r2, sb
0037a34c: mov r0, r7
0037a350: bl #0x3140ec
0037a354: ldr r3, [r4, #0x24]
0037a358: ldr r1, [sp, #0x48]
0037a35c: ldr r6, [r4, #0x20]
0037a360: ldr r5, [sp, #0x44]
0037a364: mov r0, r3
0037a368: rsb r6, r3, r6
0037a36c: rsb r5, r1, r5
0037a370: cmp r5, r6
0037a374: movlt r2, r5
0037a378: movge r2, r6
0037a37c: bl #0x30e5e0
0037a380: subs r3, r0, #0
0037a384: bne #0x37a39c
0037a388: cmp r6, r5
0037a38c: mvnlt r3, #0
0037a390: blt #0x37a39c
0037a394: movle r3, #0
0037a398: movgt r3, #1
0037a39c: mov r0, r7
0037a3a0: str r3, [sp, #4]
0037a3a4: bl #0x3139ac
0037a3a8: ldr r3, [sp, #4]
0037a3ac: cmp r3, #0
0037a3b0: movge sl, r4
0037a3b4: ldrlt r4, [r4, #0xc]
0037a3b8: ldrge r4, [r4, #8]
0037a3bc: cmp r4, #0
0037a3c0: bne #0x37a344
0037a3c4: ldr r3, [sp, #8]
0037a3c8: cmp sl, r3
0037a3cc: beq #0x37a440
0037a3d0: add r4, sp, #0x1c
0037a3d4: ldr r1, [r8]
0037a3d8: add r2, sp, #0x14
0037a3dc: mov r0, r4
0037a3e0: bl #0x3140ec
0037a3e4: ldr r3, [sp, #0x30]
0037a3e8: ldr r1, [sl, #0x24]
0037a3ec: ldr r5, [sl, #0x20]
0037a3f0: ldr r6, [sp, #0x2c]
0037a3f4: mov r0, r3
0037a3f8: rsb r5, r1, r5
0037a3fc: rsb r6, r3, r6
0037a400: cmp r5, r6
0037a404: movlt r2, r5
0037a408: movge r2, r6
0037a40c: bl #0x30e5e0
0037a410: subs r7, r0, #0
0037a414: bne #0x37a42c
0037a418: cmp r6, r5
0037a41c: mvnlt r7, #0
0037a420: blt #0x37a42c
0037a424: movle r7, #0
0037a428: movgt r7, #1
0037a42c: mov r0, r4
0037a430: bl #0x3139ac
0037a434: cmp r7, #0
0037a438: bge #0x37a440
0037a43c: ldr sl, [sp, #8]
0037a440: ldr r2, [sp, #0xc]
0037a444: mov r0, sl
0037a448: ldr r3, [fp, r2]
0037a44c: ldr r2, [sp, #0x4c]
0037a450: ldr r3, [r3]
0037a454: cmp r2, r3
0037a458: bne #0x37a464
0037a45c: add sp, sp, #0x54
0037a460: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037a464: bl #0x30e310
0037a468: rsbeq sl, r1, r0, lsl #15
0037a46c: andeq r4, r0, ip, lsr #1

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE14_M_lower_boundIPKcEEPNS_18_Rb_tree_node_baseERKT_ 0037a470 size240
0037a470: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a474: ldr fp, [pc, #0xdc]
0037a478: ldr r2, [pc, #0xdc]
0037a47c: sub sp, sp, #0x2c
0037a480: add fp, pc, fp
0037a484: ldr r3, [fp, r2]
0037a488: str r2, [sp, #4]
0037a48c: mov sb, r0
0037a490: ldr r3, [r3]
0037a494: mov r8, r1
0037a498: str r3, [sp, #0x24]
0037a49c: ldr r4, [r0, #4]
0037a4a0: cmp r4, #0
0037a4a4: beq #0x37a530
0037a4a8: add r7, sp, #0xc
0037a4ac: add sl, sp, #8
0037a4b0: ldr r1, [r8]
0037a4b4: mov r2, sl
0037a4b8: mov r0, r7
0037a4bc: bl #0x3140ec
0037a4c0: ldr r3, [r4, #0x24]
0037a4c4: ldr r1, [sp, #0x20]
0037a4c8: ldr r6, [r4, #0x20]
0037a4cc: ldr r5, [sp, #0x1c]
0037a4d0: mov r0, r3
0037a4d4: rsb r6, r3, r6
0037a4d8: rsb r5, r1, r5
0037a4dc: cmp r5, r6
0037a4e0: movlt r2, r5
0037a4e4: movge r2, r6
0037a4e8: bl #0x30e5e0
0037a4ec: subs r3, r0, #0
0037a4f0: bne #0x37a508
0037a4f4: cmp r6, r5
0037a4f8: mvnlt r3, #0
0037a4fc: blt #0x37a508
0037a500: movle r3, #0
0037a504: movgt r3, #1
0037a508: mov r0, r7
0037a50c: str r3, [sp]
0037a510: bl #0x3139ac
0037a514: ldr r3, [sp]
0037a518: cmp r3, #0
0037a51c: movge sb, r4
0037a520: ldrlt r4, [r4, #0xc]
0037a524: ldrge r4, [r4, #8]
0037a528: cmp r4, #0
0037a52c: bne #0x37a4b0
0037a530: ldr r2, [sp, #4]
0037a534: mov r0, sb
0037a538: ldr r3, [fp, r2]
0037a53c: ldr r2, [sp, #0x24]
0037a540: ldr r3, [r3]
0037a544: cmp r2, r3
0037a548: bne #0x37a554
0037a54c: add sp, sp, #0x2c
0037a550: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037a554: bl #0x30e310
0037a558: rsbeq sl, r1, r0, lsl r6
0037a55c: andeq r4, r0, ip, lsr #1

# _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE14_M_create_nodeERKS7_ 0037a560 size88
0037a560: push {r4, r5, lr}
0037a564: sub sp, sp, #0xc
0037a568: mov r3, #0x2c
0037a56c: add r0, sp, #8
0037a570: str r3, [r0, #-4]!
0037a574: mov r5, r1
0037a578: bl #0x708ec0
0037a57c: mov r4, r0
0037a580: add r0, r0, #0x10
0037a584: str r0, [r4, #0x20]
0037a588: str r0, [r4, #0x24]
0037a58c: ldr r2, [r5, #0x10]
0037a590: ldr r1, [r5, #0x14]
0037a594: bl #0x3116e8
0037a598: ldr r2, [r5, #0x18]
0037a59c: mov r3, #0
0037a5a0: str r3, [r4, #0xc]
0037a5a4: str r2, [r4, #0x28]
0037a5a8: str r3, [r4, #8]
0037a5ac: mov r0, r4
0037a5b0: add sp, sp, #0xc
0037a5b4: pop {r4, r5, pc}

# _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS7_SF_SF_ 0037a5b8 size240
0037a5b8: push {r4, r5, r6, r7, lr}
0037a5bc: cmp r1, r2
0037a5c0: sub sp, sp, #0xc
0037a5c4: mov r4, r1
0037a5c8: mov r5, r2
0037a5cc: mov r6, r0
0037a5d0: beq #0x37a664
0037a5d4: ldr r2, [sp, #0x24]
0037a5d8: cmp r2, #0
0037a5dc: beq #0x37a62c
0037a5e0: mov r1, r3
0037a5e4: mov r0, r4
0037a5e8: bl #0x37a560
0037a5ec: str r0, [r5, #0xc]
0037a5f0: ldr r3, [r4, #0xc]
0037a5f4: mov r7, r0
0037a5f8: cmp r5, r3
0037a5fc: beq #0x37a65c
0037a600: mov r0, r7
0037a604: str r5, [r7, #4]
0037a608: add r1, r4, #4
0037a60c: bl #0x313760
0037a610: ldr r3, [r4, #0x10]
0037a614: mov r0, r6
0037a618: add r3, r3, #1
0037a61c: str r3, [r4, #0x10]
0037a620: str r7, [r6]
0037a624: add sp, sp, #0xc
0037a628: pop {r4, r5, r6, r7, pc}
0037a62c: ldr r2, [sp, #0x20]
0037a630: cmp r2, #0
0037a634: beq #0x37a684
0037a638: mov r1, r3
0037a63c: mov r0, r4
0037a640: bl #0x37a560
0037a644: str r0, [r5, #8]
0037a648: ldr r3, [r4, #8]
0037a64c: mov r7, r0
0037a650: cmp r5, r3
0037a654: streq r0, [r4, #8]
0037a658: b #0x37a600
0037a65c: str r7, [r4, #0xc]
0037a660: b #0x37a600
0037a664: mov r1, r3
0037a668: mov r0, r4
0037a66c: bl #0x37a560
0037a670: mov r7, r0
0037a674: str r0, [r4, #8]
0037a678: str r0, [r4, #4]
0037a67c: str r0, [r4, #0xc]
0037a680: b #0x37a600
0037a684: add r0, r1, #0x14
0037a688: add r2, r5, #0x10
0037a68c: mov r1, r3
0037a690: str r3, [sp, #4]
0037a694: bl #0x313bf8
0037a698: cmp r0, #0
0037a69c: ldr r3, [sp, #4]
0037a6a0: beq #0x37a5e0
0037a6a4: b #0x37a638

# _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueERKS7_ 0037a6a8 size536
0037a6a8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a6ac: ldr r5, [r1, #4]
0037a6b0: sub sp, sp, #0x14
0037a6b4: mov sb, r1
0037a6b8: cmp r5, #0
0037a6bc: mov r4, r0
0037a6c0: mov r8, r2
0037a6c4: beq #0x37a7ac
0037a6c8: ldr r7, [r2, #0x14]
0037a6cc: ldr fp, [r2, #0x10]
0037a6d0: rsb sl, r7, fp
0037a6d4: b #0x37a6ec
0037a6d8: ldr r3, [r5, #8]
0037a6dc: mov r1, #1
0037a6e0: cmp r3, #0
0037a6e4: beq #0x37a744
0037a6e8: mov r5, r3
0037a6ec: ldr r3, [r5, #0x24]
0037a6f0: ldr r6, [r5, #0x20]
0037a6f4: mov r0, r7
0037a6f8: mov r1, r3
0037a6fc: rsb r6, r3, r6
0037a700: cmp r6, sl
0037a704: movlt r2, r6
0037a708: movge r2, sl
0037a70c: bl #0x30e5e0
0037a710: cmp r0, #0
0037a714: mov r2, r5
0037a718: bne #0x37a72c
0037a71c: cmp sl, r6
0037a720: blt #0x37a6d8
0037a724: movle r0, #0
0037a728: movgt r0, #1
0037a72c: cmp r0, #0
0037a730: blt #0x37a6d8
0037a734: ldr r3, [r5, #0xc]
0037a738: mov r1, #0
0037a73c: cmp r3, #0
0037a740: bne #0x37a6e8
0037a744: cmp r1, #0
0037a748: moveq sl, r5
0037a74c: bne #0x37a7b0
0037a750: ldr r0, [r2, #0x24]
0037a754: ldr r6, [r2, #0x20]
0037a758: rsb fp, r7, fp
0037a75c: mov r1, r7
0037a760: rsb r6, r0, r6
0037a764: cmp fp, r6
0037a768: movlt r2, fp
0037a76c: movge r2, r6
0037a770: bl #0x30e5e0
0037a774: cmp r0, #0
0037a778: bne #0x37a78c
0037a77c: cmp r6, fp
0037a780: blt #0x37a808
0037a784: movle r0, #0
0037a788: movgt r0, #1
0037a78c: cmp r0, #0
0037a790: movge r3, #0
0037a794: strge sl, [r4]
0037a798: strbge r3, [r4, #4]
0037a79c: blt #0x37a808
0037a7a0: mov r0, r4
0037a7a4: add sp, sp, #0x14
0037a7a8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037a7ac: mov r5, r1
0037a7b0: ldr r3, [sb, #8]
0037a7b4: cmp r5, r3
0037a7b8: beq #0x37a888
0037a7bc: ldrb r3, [r5]
0037a7c0: cmp r3, #0
0037a7c4: bne #0x37a7d8
0037a7c8: ldr r3, [r5, #4]
0037a7cc: ldr r3, [r3, #4]
0037a7d0: cmp r5, r3
0037a7d4: beq #0x37a874
0037a7d8: ldr r2, [r5, #8]
0037a7dc: cmp r2, #0
0037a7e0: bne #0x37a7ec
0037a7e4: b #0x37a83c
0037a7e8: mov r2, r3
0037a7ec: ldr r3, [r2, #0xc]
0037a7f0: cmp r3, #0
0037a7f4: bne #0x37a7e8
0037a7f8: mov sl, r2
0037a7fc: ldr r7, [r8, #0x14]
0037a800: ldr fp, [r8, #0x10]
0037a804: b #0x37a750
0037a808: mov ip, #0
0037a80c: mov r2, r5
0037a810: mov r3, r8
0037a814: mov r1, sb
0037a818: add r0, sp, #8
0037a81c: str ip, [sp, #4]
0037a820: str ip, [sp]
0037a824: bl #0x37a5b8
0037a828: ldr r3, [sp, #8]
0037a82c: mov r2, #1
0037a830: strb r2, [r4, #4]
0037a834: str r3, [r4]
0037a838: b #0x37a7a0
0037a83c: ldr r3, [r5, #4]
0037a840: ldr r2, [r3, #8]
0037a844: cmp r5, r2
0037a848: beq #0x37a854
0037a84c: b #0x37a8b8
0037a850: mov r3, r2
0037a854: ldr r2, [r3, #4]
0037a858: ldr r1, [r2, #8]
0037a85c: cmp r1, r3
0037a860: beq #0x37a850
0037a864: ldr r7, [r8, #0x14]
0037a868: ldr fp, [r8, #0x10]
0037a86c: mov sl, r2
0037a870: b #0x37a750
0037a874: ldr r2, [r5, #0xc]
0037a878: ldr r7, [r8, #0x14]
0037a87c: ldr fp, [r8, #0x10]
0037a880: mov sl, r2
0037a884: b #0x37a750
0037a888: mov r2, r5
0037a88c: mov r3, r8
0037a890: mov ip, #0
0037a894: mov r1, sb
0037a898: add r0, sp, #0xc
0037a89c: stm sp, {r5, ip}
0037a8a0: bl #0x37a5b8
0037a8a4: ldr r3, [sp, #0xc]
0037a8a8: mov r2, #1
0037a8ac: strb r2, [r4, #4]
0037a8b0: str r3, [r4]
0037a8b4: b #0x37a7a0
0037a8b8: mov r2, r3
0037a8bc: b #0x37a7f8

# _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueENS_17_Rb_tree_iteratorIS7_SB_EERKS7_ 0037abf8 size1284
0037abf8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037abfc: sub sp, sp, #0x44
0037ac00: str r2, [sp, #0x14]
0037ac04: ldr r5, [r2]
0037ac08: ldr r2, [r1, #8]
0037ac0c: mov r6, r1
0037ac10: mov r7, r0
0037ac14: cmp r5, r2
0037ac18: mov r8, r3
0037ac1c: beq #0x37ae14
0037ac20: cmp r5, r1
0037ac24: beq #0x37af6c
0037ac28: ldrb r3, [r5]
0037ac2c: cmp r3, #0
0037ac30: beq #0x37ad0c
0037ac34: ldr r4, [r5, #8]
0037ac38: cmp r4, #0
0037ac3c: bne #0x37ac48
0037ac40: b #0x37ad2c
0037ac44: mov r4, r3
0037ac48: ldr r3, [r4, #0xc]
0037ac4c: cmp r3, #0
0037ac50: bne #0x37ac44
0037ac54: ldr r3, [r5, #0x24]
0037ac58: ldr sb, [r8, #0x14]
0037ac5c: ldr fp, [r8, #0x10]
0037ac60: ldr r2, [r5, #0x20]
0037ac64: mov r1, r3
0037ac68: rsb fp, sb, fp
0037ac6c: rsb r2, r3, r2
0037ac70: str r2, [sp, #0x18]
0037ac74: mov r0, sb
0037ac78: cmp r2, fp
0037ac7c: movge r2, fp
0037ac80: str r3, [sp, #0xc]
0037ac84: str r2, [sp, #0x1c]
0037ac88: bl #0x30e5e0
0037ac8c: cmp r0, #0
0037ac90: ldr r3, [sp, #0xc]
0037ac94: bne #0x37acb0
0037ac98: ldr r2, [sp, #0x18]
0037ac9c: cmp fp, r2
0037aca0: mvnlt r0, #0
0037aca4: blt #0x37acb0
0037aca8: movle r0, #0
0037acac: movgt r0, #1
0037acb0: lsrs ip, r0, #0x1f
0037acb4: bne #0x37ad5c
0037acb8: ldr r4, [r5, #0xc]
0037acbc: cmp r4, #0
0037acc0: bne #0x37accc
0037acc4: b #0x37affc
0037acc8: mov r4, r2
0037accc: ldr r2, [r4, #8]
0037acd0: cmp r2, #0
0037acd4: bne #0x37acc8
0037acd8: cmp ip, #0
0037acdc: bne #0x37adf0
0037ace0: mov r0, r3
0037ace4: mov r1, sb
0037ace8: ldr r2, [sp, #0x1c]
0037acec: bl #0x30e5e0
0037acf0: cmp r0, #0
0037acf4: bne #0x37adcc
0037acf8: ldr r3, [sp, #0x18]
0037acfc: cmp fp, r3
0037ad00: bgt #0x37add0
0037ad04: str r5, [r7]
0037ad08: b #0x37ae08
0037ad0c: ldr r3, [r5, #4]
0037ad10: ldr r3, [r3, #4]
0037ad14: cmp r5, r3
0037ad18: ldreq r4, [r5, #0xc]
0037ad1c: beq #0x37ac54
0037ad20: ldr r4, [r5, #8]
0037ad24: cmp r4, #0
0037ad28: bne #0x37ac48
0037ad2c: ldr r4, [r5, #4]
0037ad30: ldr r3, [r4, #8]
0037ad34: cmp r5, r3
0037ad38: beq #0x37ad44
0037ad3c: b #0x37ac54
0037ad40: mov r4, r3
0037ad44: ldr r3, [r4, #4]
0037ad48: ldr r2, [r3, #8]
0037ad4c: cmp r2, r4
0037ad50: beq #0x37ad40
0037ad54: mov r4, r3
0037ad58: b #0x37ac54
0037ad5c: ldr r2, [r4, #0x24]
0037ad60: ldr sl, [r4, #0x20]
0037ad64: mov r1, sb
0037ad68: mov r0, r2
0037ad6c: rsb sl, r2, sl
0037ad70: cmp fp, sl
0037ad74: movlt r2, fp
0037ad78: movge r2, sl
0037ad7c: str r3, [sp, #0xc]
0037ad80: str ip, [sp, #0x10]
0037ad84: bl #0x30e5e0
0037ad88: cmp r0, #0
0037ad8c: ldr r3, [sp, #0xc]
0037ad90: ldr ip, [sp, #0x10]
0037ad94: bne #0x37af58
0037ad98: cmp fp, sl
0037ad9c: ble #0x37acb8
0037ada0: ldr ip, [r4, #0xc]
0037ada4: cmp ip, #0
0037ada8: beq #0x37af38
0037adac: mov ip, #0
0037adb0: mov r1, r6
0037adb4: mov r2, r5
0037adb8: mov r3, r8
0037adbc: mov r0, r7
0037adc0: stm sp, {r5, ip}
0037adc4: bl #0x37a5b8
0037adc8: b #0x37ae08
0037adcc: bge #0x37ad04
0037add0: cmp r6, r4
0037add4: beq #0x37b048
0037add8: add r0, r6, #0x14
0037addc: mov r1, r8
0037ade0: add r2, r4, #0x10
0037ade4: bl #0x313bf8
0037ade8: cmp r0, #0
0037adec: bne #0x37b040
0037adf0: mov r1, r6
0037adf4: mov r2, r8
0037adf8: add r0, sp, #0x20
0037adfc: bl #0x37a6a8
0037ae00: ldr r3, [sp, #0x20]
0037ae04: str r3, [r7]
0037ae08: mov r0, r7
0037ae0c: add sp, sp, #0x44
0037ae10: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037ae14: ldr r3, [r1, #0x10]
0037ae18: cmp r3, #0
0037ae1c: beq #0x37b0a0
0037ae20: ldr r3, [r8, #0x14]
0037ae24: ldr r1, [r5, #0x24]
0037ae28: ldr r4, [r8, #0x10]
0037ae2c: ldr sl, [r5, #0x20]
0037ae30: mov r0, r3
0037ae34: rsb r4, r3, r4
0037ae38: rsb sl, r1, sl
0037ae3c: cmp sl, r4
0037ae40: movlt r2, sl
0037ae44: movge r2, r4
0037ae48: bl #0x30e5e0
0037ae4c: cmp r0, #0
0037ae50: bne #0x37ae64
0037ae54: cmp r4, sl
0037ae58: blt #0x37adac
0037ae5c: movle r0, #0
0037ae60: movgt r0, #1
0037ae64: cmp r0, #0
0037ae68: blt #0x37adac
0037ae6c: add sl, r6, #0x14
0037ae70: add r1, r5, #0x10
0037ae74: mov r0, sl
0037ae78: mov r2, r8
0037ae7c: bl #0x313bf8
0037ae80: cmp r0, #0
0037ae84: beq #0x37b090
0037ae88: ldr r3, [sp, #0x14]
0037ae8c: ldr ip, [r3]
0037ae90: ldr r4, [ip, #0xc]
0037ae94: cmp r4, #0
0037ae98: bne #0x37af28
0037ae9c: ldr r3, [ip, #4]
0037aea0: ldr r2, [r3, #0xc]
0037aea4: cmp ip, r2
0037aea8: movne r4, ip
0037aeac: bne #0x37aec4
0037aeb0: mov r4, r3
0037aeb4: ldr r3, [r3, #4]
0037aeb8: ldr r2, [r3, #0xc]
0037aebc: cmp r2, r4
0037aec0: beq #0x37aeb0
0037aec4: ldr r2, [r4, #0xc]
0037aec8: cmp r3, r2
0037aecc: movne r4, r3
0037aed0: cmp r6, r4
0037aed4: beq #0x37b0d8
0037aed8: mov r0, sl
0037aedc: mov r1, r8
0037aee0: add r2, r4, #0x10
0037aee4: bl #0x313bf8
0037aee8: cmp r0, #0
0037aeec: beq #0x37b074
0037aef0: ldr r2, [sp, #0x14]
0037aef4: ldr ip, [r2]
0037aef8: ldr lr, [ip, #0xc]
0037aefc: cmp lr, #0
0037af00: beq #0x37b0b8
0037af04: mov ip, #0
0037af08: mov r1, r6
0037af0c: mov r2, r4
0037af10: mov r3, r8
0037af14: mov r0, r7
0037af18: stm sp, {r4, ip}
0037af1c: bl #0x37a5b8
0037af20: b #0x37ae08
0037af24: mov r4, r3
0037af28: ldr r3, [r4, #8]
0037af2c: cmp r3, #0
0037af30: bne #0x37af24
0037af34: b #0x37aed0
0037af38: mov r1, r6
0037af3c: mov r2, r4
0037af40: mov r3, r8
0037af44: mov r0, r7
0037af48: str ip, [sp]
0037af4c: str r4, [sp, #4]
0037af50: bl #0x37a5b8
0037af54: b #0x37ae08
0037af58: bge #0x37acb8
0037af5c: ldr ip, [r4, #0xc]
0037af60: cmp ip, #0
0037af64: bne #0x37adac
0037af68: b #0x37af38
0037af6c: ldr r4, [r5, #0xc]
0037af70: ldr r1, [r3, #0x14]
0037af74: ldr sb, [r3, #0x10]
0037af78: ldr sl, [r4, #0x20]
0037af7c: ldr r3, [r4, #0x24]
0037af80: rsb sb, r1, sb
0037af84: rsb sl, r3, sl
0037af88: cmp sb, sl
0037af8c: movlt r2, sb
0037af90: movge r2, sl
0037af94: mov r0, r3
0037af98: bl #0x30e5e0
0037af9c: cmp r0, #0
0037afa0: bne #0x37afb4
0037afa4: cmp sl, sb
0037afa8: blt #0x37afbc
0037afac: movle r0, #0
0037afb0: movgt r0, #1
0037afb4: cmp r0, #0
0037afb8: bge #0x37afe0
0037afbc: mov ip, #0
0037afc0: mov r1, r6
0037afc4: mov r2, r4
0037afc8: mov r3, r8
0037afcc: mov r0, r7
0037afd0: str ip, [sp]
0037afd4: str r5, [sp, #4]
0037afd8: bl #0x37a5b8
0037afdc: b #0x37ae08
0037afe0: mov r1, r6
0037afe4: mov r2, r8
0037afe8: add r0, sp, #0x28
0037afec: bl #0x37a6a8
0037aff0: ldr r3, [sp, #0x28]
0037aff4: str r3, [r7]
0037aff8: b #0x37ae08
0037affc: ldr r2, [r5, #4]
0037b000: ldr r1, [r2, #0xc]
0037b004: cmp r5, r1
0037b008: movne r4, r5
0037b00c: beq #0x37b024
0037b010: ldr r1, [r4, #0xc]
0037b014: cmp r2, r1
0037b018: movne r4, r2
0037b01c: b #0x37acd8
0037b020: mov r2, r1
0037b024: ldr r1, [r2, #4]
0037b028: ldr r0, [r1, #0xc]
0037b02c: cmp r0, r2
0037b030: beq #0x37b020
0037b034: mov r4, r2
0037b038: mov r2, r1
0037b03c: b #0x37b010
0037b040: ldr r2, [sp, #0x14]
0037b044: ldr r5, [r2]
0037b048: ldr ip, [r5, #0xc]
0037b04c: cmp ip, #0
0037b050: bne #0x37af04
0037b054: mov r1, r6
0037b058: mov r2, r5
0037b05c: mov r3, r8
0037b060: mov r0, r7
0037b064: str ip, [sp]
0037b068: str r5, [sp, #4]
0037b06c: bl #0x37a5b8
0037b070: b #0x37ae08
0037b074: mov r1, r6
0037b078: mov r2, r8
0037b07c: add r0, sp, #0x30
0037b080: bl #0x37a6a8
0037b084: ldr r3, [sp, #0x30]
0037b088: str r3, [r7]
0037b08c: b #0x37ae08
0037b090: ldr r2, [sp, #0x14]
0037b094: ldr r3, [r2]
0037b098: str r3, [r7]
0037b09c: b #0x37ae08
0037b0a0: mov r2, r8
0037b0a4: add r0, sp, #0x38
0037b0a8: bl #0x37a6a8
0037b0ac: ldr r3, [sp, #0x38]
0037b0b0: str r3, [r7]
0037b0b4: b #0x37ae08
0037b0b8: mov r1, r6
0037b0bc: mov r2, ip
0037b0c0: mov r3, r8
0037b0c4: mov r0, r7
0037b0c8: str lr, [sp]
0037b0cc: str ip, [sp, #4]
0037b0d0: bl #0x37a5b8
0037b0d4: b #0x37ae08
0037b0d8: mov lr, #0
0037b0dc: mov r1, r6
0037b0e0: mov r2, ip
0037b0e4: mov r3, r8
0037b0e8: mov r0, r7
0037b0ec: str lr, [sp]
0037b0f0: str ip, [sp, #4]
0037b0f4: bl #0x37a5b8
0037b0f8: b #0x37ae08

# _ZNSt3mapISsP12StreamBufferSt4lessISsESaISt4pairIKSsS1_EEEixIPKcEERS1_RKT_ 0037b0fc size320
0037b0fc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b100: ldr r4, [pc, #0x12c]
0037b104: ldr r7, [pc, #0x12c]
0037b108: sub sp, sp, #0x6c
0037b10c: add r4, pc, r4
0037b110: ldr r3, [r4, r7]
0037b114: mov r8, r0
0037b118: mov r6, r1
0037b11c: ldr r3, [r3]
0037b120: str r3, [sp, #0x64]
0037b124: bl #0x37a470
0037b128: cmp r0, r8
0037b12c: mov r5, r0
0037b130: beq #0x37b1c4
0037b134: add sl, sp, #0x4c
0037b138: ldr r1, [r6]
0037b13c: add r2, sp, #0x14
0037b140: mov r0, sl
0037b144: bl #0x3140ec
0037b148: ldr r3, [sp, #0x60]
0037b14c: ldr r1, [r5, #0x24]
0037b150: ldr fp, [r5, #0x20]
0037b154: ldr sb, [sp, #0x5c]
0037b158: mov r0, r3
0037b15c: rsb fp, r1, fp
0037b160: rsb sb, r3, sb
0037b164: cmp fp, sb
0037b168: movlt r2, fp
0037b16c: movge r2, sb
0037b170: bl #0x30e5e0
0037b174: cmp r0, #0
0037b178: mov r3, r5
0037b17c: bne #0x37b1b8
0037b180: cmp sb, fp
0037b184: blt #0x37b1bc
0037b188: mov r0, sl
0037b18c: str r3, [sp, #4]
0037b190: bl #0x3139ac
0037b194: ldr r3, [sp, #4]
0037b198: ldr r1, [r4, r7]
0037b19c: ldr r2, [sp, #0x64]
0037b1a0: add r0, r3, #0x28
0037b1a4: ldr r3, [r1]
0037b1a8: cmp r2, r3
0037b1ac: bne #0x37b230
0037b1b0: add sp, sp, #0x6c
0037b1b4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b1b8: bge #0x37b188
0037b1bc: mov r0, sl
0037b1c0: bl #0x3139ac
0037b1c4: add sl, sp, #0x34
0037b1c8: ldr r1, [r6]
0037b1cc: add r2, sp, #0x10
0037b1d0: add r6, sp, #0x18
0037b1d4: mov r0, sl
0037b1d8: bl #0x3140ec
0037b1dc: mov r0, r6
0037b1e0: ldr r1, [sp, #0x48]
0037b1e4: ldr r2, [sp, #0x44]
0037b1e8: str r6, [sp, #0x28]
0037b1ec: str r6, [sp, #0x2c]
0037b1f0: bl #0x3116e8
0037b1f4: mov r3, r6
0037b1f8: mov ip, #0
0037b1fc: mov r1, r8
0037b200: add r2, sp, #8
0037b204: add r0, sp, #0xc
0037b208: str ip, [sp, #0x30]
0037b20c: str r5, [sp, #8]
0037b210: bl #0x37abf8
0037b214: ldr r5, [sp, #0xc]
0037b218: mov r0, r6
0037b21c: bl #0x3139ac
0037b220: mov r0, sl
0037b224: bl #0x3139ac
0037b228: mov r3, r5
0037b22c: b #0x37b198
0037b230: bl #0x30e310
0037b234: rsbeq sb, r1, r4, lsl #19
0037b238: andeq r4, r0, ip, lsr #1

# _ZN5Level25AssignSteamToLoadDataFileER12StreamBuffer 003f02ac size100
003f02ac: push {r4, r5, r6, r7, r8, lr}
003f02b0: mov r6, r0
003f02b4: mov r7, r1
003f02b8: mov r0, #0x50
003f02bc: mov r1, #0
003f02c0: bl #0x310570
003f02c4: ldr r5, [pc, #0x3c]
003f02c8: ldr r3, [pc, #0x3c]
003f02cc: mov r4, r0
003f02d0: add r5, pc, r5
003f02d4: ldr r3, [r5, r3]
003f02d8: mov r1, r7
003f02dc: add r3, r3, #8
003f02e0: str r3, [r0], #8
003f02e4: bl #0x3172d8
003f02e8: mov r3, #0
003f02ec: strb r3, [r4, #0x48]
003f02f0: str r3, [r4, #0x38]
003f02f4: str r3, [r4, #0x3c]
003f02f8: str r3, [r4, #0x40]
003f02fc: str r3, [r4, #0x44]
003f0300: str r4, [r6, #0x140]
003f0304: pop {r4, r5, r6, r7, r8, pc}
003f0308: subseq r4, sl, r0, asr #15
003f030c: andeq r2, r0, r0, lsr #4

# _ZN5Level19GenerateRandomLevelER12StreamBufferj 003f07f8 size160
003f07f8: push {r4, r5, r6, r7, r8, lr}
003f07fc: ldr r5, [r0, #0x14c]
003f0800: mov r4, r0
003f0804: mov r6, r1
003f0808: cmp r5, #0
003f080c: mov r7, r2
003f0810: beq #0x3f082c
003f0814: mov r0, r5
003f0818: bl #0x488504
003f081c: mov r0, r5
003f0820: bl #0x310440
003f0824: mov r3, #0
003f0828: str r3, [r4, #0x14c]
003f082c: mov r1, #0
003f0830: mov r0, #0x190
003f0834: bl #0x310570
003f0838: mov r5, r0
003f083c: bl #0x487e84
003f0840: str r5, [r4, #0x14c]
003f0844: mov r0, r5
003f0848: ldr r1, [r4, #0x10c]
003f084c: bl #0x489508
003f0850: mov r1, r7
003f0854: ldr r0, [r4, #0x14c]
003f0858: bl #0x488134
003f085c: mov r1, r6
003f0860: mov r5, r0
003f0864: ldr r0, [r4, #0x14c]
003f0868: bl #0x488b84
003f086c: ldr r6, [r4, #0x14c]
003f0870: cmp r6, #0
003f0874: beq #0x3f0890
003f0878: mov r0, r6
003f087c: bl #0x488504
003f0880: mov r0, r6
003f0884: bl #0x310440
003f0888: mov r3, #0
003f088c: str r3, [r4, #0x14c]
003f0890: mov r0, r5
003f0894: pop {r4, r5, r6, r7, r8, pc}

# _ZN14PlayerSavegame18SG_UnpackQuestSyncER12StreamBuffer 00467160 size116
00467160: push {r4, r5, r6, r7, r8, lr}
00467164: add r5, r0, #0x118
00467168: mov r7, r1
0046716c: mov r6, r0
00467170: mov r0, r5
00467174: bl #0x46c1a8
00467178: ldr r4, [pc, #0x4c]
0046717c: mov r2, #0
00467180: mov r3, #0
00467184: mov r0, r7
00467188: ldr r1, [r7]
0046718c: mov lr, pc
00467190: ldr pc, [r1, #0x20]
00467194: ldr r3, [pc, #0x34]
00467198: add r4, pc, r4
0046719c: mov r2, r7
004671a0: ldr r1, [r4, r3]
004671a4: mov r0, r5
004671a8: mov r3, #1
004671ac: ldr r1, [r1]
004671b0: bl #0x46c48c
004671b4: mov r0, r5
004671b8: mov r1, #1
004671bc: bl #0x46b824
004671c0: mov r3, #1
004671c4: strb r3, [r6, #0x14]
004671c8: pop {r4, r5, r6, r7, r8, pc}
004671cc: ldrsheq sp, [r2], #-0x88
004671d0: muleq r0, ip, sl

# _ZN3rnd15RandomGenerator24SaveModularLevelToStreamER12StreamBuffer 00488b84 size716
00488b84: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00488b88: ldr r5, [pc, #0x28c]
00488b8c: ldr r2, [pc, #0x28c]
00488b90: sub sp, sp, #0x15c
00488b94: add r5, pc, r5
00488b98: ldr r3, [r5, r2]
00488b9c: add r4, sp, #0x90
00488ba0: mov r7, r0
00488ba4: ldr r3, [r3]
00488ba8: mov r0, r4
00488bac: str r2, [sp, #0xc]
00488bb0: str r3, [sp, #0x154]
00488bb4: mov sb, r1
00488bb8: bl #0x516ec4
00488bbc: mov r1, #0
00488bc0: mov r0, #0x88
00488bc4: bl #0x310570
00488bc8: ldr r1, [pc, #0x254]
00488bcc: ldr r2, [pc, #0x254]
00488bd0: ldr r3, [pc, #0x254]
00488bd4: mov r8, r0
00488bd8: add r2, pc, r2
00488bdc: add r3, pc, r3
00488be0: add r1, pc, r1
00488be4: bl #0x517690
00488be8: mov r1, r8
00488bec: mov r0, r4
00488bf0: bl #0x515964
00488bf4: mov r1, #0
00488bf8: mov r0, #0x40
00488bfc: bl #0x310570
00488c00: ldr sl, [pc, #0x228]
00488c04: mov r1, #2
00488c08: mov fp, r0
00488c0c: bl #0x515e18
00488c10: ldr sl, [r5, sl]
00488c14: ldr r1, [pc, #0x218]
00488c18: mov r0, fp
00488c1c: add sl, sl, #8
00488c20: add r1, pc, r1
00488c24: add r2, r1, #0x40
00488c28: str sl, [r0], #0x20
00488c2c: bl #0x3109e0
00488c30: mov r1, #0
00488c34: mov r0, #0x40
00488c38: bl #0x310570
00488c3c: mov r1, #2
00488c40: str r0, [sp, #8]
00488c44: bl #0x515e18
00488c48: ldr r1, [pc, #0x1e8]
00488c4c: ldr r0, [sp, #8]
00488c50: add r6, sp, #0x10
00488c54: add r1, pc, r1
00488c58: add r2, r1, #0x17
00488c5c: str sl, [r0], #0x20
00488c60: bl #0x3109e0
00488c64: ldr r1, [pc, #0x1d0]
00488c68: ldr r2, [r7, #0x188]
00488c6c: mov r0, r6
00488c70: add r1, pc, r1
00488c74: bl #0x30eae4
00488c78: mov r1, #0
00488c7c: mov r0, #0x40
00488c80: bl #0x310570
00488c84: mov r1, #2
00488c88: mov r8, r0
00488c8c: str r0, [sp, #4]
00488c90: bl #0x515e18
00488c94: str sl, [r8], #0x20
00488c98: mov r0, r6
00488c9c: bl #0x30de54
00488ca0: mov r1, r6
00488ca4: add r2, r6, r0
00488ca8: mov r0, r8
00488cac: bl #0x3109e0
00488cb0: mov r1, fp
00488cb4: mov r0, r4
00488cb8: bl #0x515964
00488cbc: ldr r1, [sp, #8]
00488cc0: mov r0, r4
00488cc4: bl #0x515964
00488cc8: ldr r3, [sp, #4]
00488ccc: mov r0, r4
00488cd0: mov r1, r3
00488cd4: bl #0x515964
00488cd8: mov r1, #0
00488cdc: mov r0, #0x8c
00488ce0: bl #0x310570
00488ce4: ldr r1, [pc, #0x154]
00488ce8: mov r6, r0
00488cec: add r1, pc, r1
00488cf0: bl #0x51745c
00488cf4: mov r1, r6
00488cf8: mov r0, r4
00488cfc: bl #0x515964
00488d00: ldr r0, [r7, #0x18c]
00488d04: mov r1, r6
00488d08: mov r2, #0
00488d0c: add r0, r0, #4
00488d10: bl #0x5134f0
00488d14: ldr r0, [r7, #0x114]
00488d18: cmp r0, #0
00488d1c: beq #0x488d2c
00488d20: mov r1, r6
00488d24: mov r2, #0
00488d28: bl #0x491d90
00488d2c: add r6, sp, #0x100
00488d30: ldr r7, [pc, #0x10c]
00488d34: mov r0, r6
00488d38: bl #0x485aa0
00488d3c: ldr r1, [pc, #0x104]
00488d40: ldr r7, [r5, r7]
00488d44: add r0, r6, #0x3c
00488d48: add r1, pc, r1
00488d4c: add r2, r1, #2
00488d50: add r7, r7, #8
00488d54: str r7, [sp, #0x100]
00488d58: bl #0x3109e0
00488d5c: mov r1, r6
00488d60: mov r0, r4
00488d64: bl #0x5145c8
00488d68: ldr r1, [sp, #0x11c]
00488d6c: ldr r2, [sp, #0x120]
00488d70: mov r0, sb
00488d74: mov r3, #0
00488d78: rsb r2, r2, r1
00488d7c: bl #0x317180
00488d80: ldr r1, [sp, #0x120]
00488d84: ldr r2, [sp, #0x11c]
00488d88: mov r3, #0
00488d8c: ldr ip, [sb]
00488d90: rsb r2, r1, r2
00488d94: mov r0, sb
00488d98: mov lr, pc
00488d9c: ldr pc, [ip, #0x1c]
00488da0: mov r0, r6
00488da4: str r7, [sp, #0x100]
00488da8: bl #0x488434
00488dac: ldr r3, [pc, #0x98]
00488db0: ldr r0, [sp, #0xec]
00488db4: add r2, r4, #0x48
00488db8: ldr r3, [r5, r3]
00488dbc: cmp r0, r2
00488dc0: add r3, r3, #8
00488dc4: str r3, [sp, #0x90]
00488dc8: beq #0x488de8
00488dcc: cmp r0, #0
00488dd0: beq #0x488de8
00488dd4: ldr r1, [sp, #0xd8]
00488dd8: rsb r1, r0, r1
00488ddc: cmp r1, #0x80
00488de0: bhi #0x488e10
00488de4: bl #0x708f00
00488de8: mov r0, r4
00488dec: bl #0x514ab4
00488df0: ldr r2, [sp, #0xc]
00488df4: ldr r3, [r5, r2]
00488df8: ldr r2, [sp, #0x154]
00488dfc: ldr r3, [r3]
00488e00: cmp r2, r3
00488e04: bne #0x488e18
00488e08: add sp, sp, #0x15c
00488e0c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00488e10: bl #0x310440
00488e14: b #0x488de8
00488e18: bl #0x30e310
00488e1c: ldrsheq fp, [r0], #-0xec
00488e20: andeq r4, r0, ip, lsr #1
00488e24: subeq ip, r4, r8
00488e28: subeq ip, r4, r8, lsl r0
00488e2c: subeq r2, r4, ip, lsr #24
00488e30: andeq r0, r0, r4, lsl #29
00488e34: ldrdeq fp, ip, [r4], #-0xf8
00488e38: subeq fp, r4, ip, ror #31
00488e3c: subeq fp, r4, r8, ror #31
00488e40: subeq sp, r3, r4, asr #21
00488e44: andeq r4, r0, ip, ror #5
00488e48: strheq r7, [r3], #-0x28
00488e4c: andeq r0, r0, r0, lsr sb

# _ZN6glitch7collada15CResFileManagerC2EPNS_7IDeviceE 00657894 size128
00657894: ldr r2, [pc, #0x68]
00657898: ldr r3, [pc, #0x68]
0065789c: ldr ip, [pc, #0x68]
006578a0: add r2, pc, r2
006578a4: push {r4, r5, r6, r7, r8}
006578a8: ldr r7, [r2, r3]
006578ac: ldr r3, [pc, #0x5c]
006578b0: ldr ip, [r2, ip]
006578b4: mov r4, #0
006578b8: ldr r6, [r2, r3]
006578bc: mov r5, r0
006578c0: add r8, ip, #8
006578c4: mov ip, #1
006578c8: stm r0, {r8, ip}
006578cc: str r4, [r0, #0xc]
006578d0: strb r4, [r5, #8]!
006578d4: str r5, [r0, #0x14]
006578d8: str r1, [r0, #0x20]
006578dc: str r7, [r0, #0x24]
006578e0: strb r4, [r0, #0x29]
006578e4: strb ip, [r0, #0x2b]
006578e8: str r5, [r0, #0x10]
006578ec: str r4, [r0, #0x18]
006578f0: strb ip, [r0, #0x28]
006578f4: strb ip, [r0, #0x2a]
006578f8: str r0, [r6]
006578fc: pop {r4, r5, r6, r7, r8}
00657900: bx lr
00657904: ldrshteq sp, [r3], -r0
00657908: andeq r1, r0, ip, ror #23
0065790c: andeq r2, r0, r8, asr r2
00657910: andeq r4, r0, r8, asr #8

# _ZN6glitch7collada15CResFileManagerC1EPNS_7IDeviceE 00657914 size128
00657914: ldr r2, [pc, #0x68]
00657918: ldr r3, [pc, #0x68]
0065791c: ldr ip, [pc, #0x68]
00657920: add r2, pc, r2
00657924: push {r4, r5, r6, r7, r8}
00657928: ldr r7, [r2, r3]
0065792c: ldr r3, [pc, #0x5c]
00657930: ldr ip, [r2, ip]
00657934: mov r4, #0
00657938: ldr r6, [r2, r3]
0065793c: mov r5, r0
00657940: add r8, ip, #8
00657944: mov ip, #1
00657948: stm r0, {r8, ip}
0065794c: str r4, [r0, #0xc]
00657950: strb r4, [r5, #8]!
00657954: str r5, [r0, #0x14]
00657958: str r1, [r0, #0x20]
0065795c: str r7, [r0, #0x24]
00657960: strb r4, [r0, #0x29]
00657964: strb ip, [r0, #0x2b]
00657968: str r5, [r0, #0x10]
0065796c: str r4, [r0, #0x18]
00657970: strb ip, [r0, #0x28]
00657974: strb ip, [r0, #0x2a]
00657978: str r0, [r6]
0065797c: pop {r4, r5, r6, r7, r8}
00657980: bx lr
00657984: eorseq sp, r3, r0, ror r1
00657988: andeq r1, r0, ip, ror #23
0065798c: andeq r2, r0, r8, asr r2
00657990: andeq r4, r0, r8, asr #8

# _ZN6glitch7collada15CResFileManager12checkVersionEPNS_2io9IReadFileE 00657994 size8
00657994: mov r0, #0
00657998: bx lr

# _ZN6glitch7collada15CResFileManager12checkVersionEPKc 0065799c size68
0065799c: push {r4, r5, r6, lr}
006579a0: ldr r3, [r0, #0x20]
006579a4: mov r5, r0
006579a8: ldr r3, [r3, #0x34]
006579ac: mov r0, r3
006579b0: ldr r3, [r3]
006579b4: mov lr, pc
006579b8: ldr pc, [r3, #0xc]
006579bc: mov r4, r0
006579c0: mov r1, r4
006579c4: mov r0, r5
006579c8: bl #0x657994
006579cc: mov r5, r0
006579d0: mov r0, r4
006579d4: bl #0x31d584
006579d8: mov r0, r5
006579dc: pop {r4, r5, r6, pc}

# _GLOBAL__I_.._source_glitch_collada_CColladaResFileManager.cpp 00657a24 size112
00657a24: ldr r3, [pc, #0x50]
00657a28: ldr r1, [pc, #0x50]
00657a2c: ldr r2, [pc, #0x50]
00657a30: str r4, [sp, #-4]!
00657a34: add r3, pc, r3
00657a38: mov r0, #0x3f000000
00657a3c: add r1, pc, r1
00657a40: ldr r4, [pc, #0x40]
00657a44: ldr ip, [r3, r2]
00657a48: str r0, [r1, #8]
00657a4c: str r0, [r1]
00657a50: str r0, [r1, #4]
00657a54: ldr r2, [pc, #0x30]
00657a58: ldr r1, [pc, #0x30]
00657a5c: ldr r4, [r3, r4]
00657a60: ldr r2, [r3, r2]
00657a64: ldr r1, [r3, r1]
00657a68: add r4, r4, #8
00657a6c: mov r0, ip
00657a70: str r4, [ip]
00657a74: ldm sp!, {r4}
00657a78: b #0x30e304
00657a7c: eorseq sp, r3, ip, asr r0
00657a80: eorseq pc, sb, r8, asr #11
00657a84: andeq r1, r0, ip, ror #23
00657a88: andeq r2, r0, r0, lsr #28
00657a8c: muleq r0, r0, r8
00657a90: andeq r2, r0, ip, ror sp

# _ZN6glitch7collada15CResFileManager11getReadFileEPNS_2io9IReadFileE 00657bd0 size188
00657bd0: push {r4, r5, r6, lr}
00657bd4: ldrb r3, [r0, #0x2a]
00657bd8: sub sp, sp, #0x20
00657bdc: mov r4, r0
00657be0: cmp r3, #0
00657be4: mov r5, r1
00657be8: bne #0x657c28
00657bec: ldrb r3, [r4, #0x2b]
00657bf0: cmp r3, #0
00657bf4: bne #0x657c14
00657bf8: ldr r3, [r5, #4]
00657bfc: mov r6, r5
00657c00: add r3, r3, #1
00657c04: str r3, [r5, #4]
00657c08: mov r0, r6
00657c0c: add sp, sp, #0x20
00657c10: pop {r4, r5, r6, pc}
00657c14: ldrb r3, [r4, #0x2a]
00657c18: cmp r3, #0
00657c1c: movne r3, #0
00657c20: strbne r3, [r4, #0x2a]
00657c24: b #0x657bf8
00657c28: mov r0, r1
00657c2c: bl #0x576d2c
00657c30: cmp r0, #0
00657c34: beq #0x657bec
00657c38: mov r1, #0
00657c3c: mov r2, r1
00657c40: ldr r3, [r5]
00657c44: mov r0, r5
00657c48: mov lr, pc
00657c4c: ldr pc, [r3, #0x18]
00657c50: mov r2, #1
00657c54: mov r1, r5
00657c58: mov r3, r2
00657c5c: mov r0, sp
00657c60: bl #0x577fa4
00657c64: ldr r1, [pc, #0x1c]
00657c68: mov r0, sp
00657c6c: mov r4, sp
00657c70: add r1, pc, r1
00657c74: bl #0x578624
00657c78: mov r6, r0
00657c7c: mov r0, sp
00657c80: bl #0x577488
00657c84: b #0x657c08

# _ZN6glitch7collada15CResFileManagerD1Ev 006582c4 size228
006582c4: push {r4, r5, r6, r7, r8, lr}
006582c8: ldr r7, [pc, #0xcc]
006582cc: ldr r3, [pc, #0xcc]
006582d0: ldr r6, [r0, #0x10]
006582d4: add r7, pc, r7
006582d8: ldr r3, [r7, r3]
006582dc: mov r5, r0
006582e0: add r4, r0, #8
006582e4: add r3, r3, #8
006582e8: str r3, [r0]
006582ec: cmp r4, r6
006582f0: beq #0x658328
006582f4: ldr r0, [r6, #0x28]
006582f8: bl #0x31d584
006582fc: ldr r2, [r6, #0xc]
00658300: cmp r2, #0
00658304: bne #0x658310
00658308: b #0x658368
0065830c: mov r2, r3
00658310: ldr r3, [r2, #8]
00658314: cmp r3, #0
00658318: bne #0x65830c
0065831c: mov r6, r2
00658320: cmp r4, r6
00658324: bne #0x6582f4
00658328: ldr r3, [pc, #0x74]
0065832c: mov r6, #0
00658330: ldr r3, [r7, r3]
00658334: str r6, [r3]
00658338: ldr r3, [r5, #0x18]
0065833c: cmp r3, r6
00658340: beq #0x658360
00658344: mov r0, r4
00658348: ldr r1, [r5, #0xc]
0065834c: bl #0x658270
00658350: str r4, [r5, #0x14]
00658354: str r6, [r5, #0x18]
00658358: str r4, [r5, #0x10]
0065835c: str r6, [r5, #0xc]
00658360: mov r0, r5
00658364: pop {r4, r5, r6, r7, r8, pc}
00658368: ldr r3, [r6, #4]
0065836c: ldr r1, [r3, #0xc]
00658370: cmp r6, r1
00658374: bne #0x658390
00658378: mov r6, r3
0065837c: ldr r3, [r3, #4]
00658380: ldr r2, [r3, #0xc]
00658384: cmp r6, r2
00658388: beq #0x658378
0065838c: ldr r2, [r6, #0xc]
00658390: cmp r2, r3
00658394: movne r6, r3
00658398: b #0x6582ec
0065839c: ldrhteq ip, [r3], -ip
006583a0: andeq r2, r0, r8, asr r2
006583a4: andeq r4, r0, r8, asr #8

# _ZN6glitch7collada15CResFileManagerD0Ev 006583a8 size28
006583a8: push {r4, lr}
006583ac: mov r4, r0
006583b0: bl #0x6582c4
006583b4: mov r0, r4
006583b8: bl #0x30e2b0
006583bc: mov r0, r4
006583c0: pop {r4, pc}

# _ZN6glitch7collada15CResFileManagerD2Ev 006583c4 size228
006583c4: push {r4, r5, r6, r7, r8, lr}
006583c8: ldr r7, [pc, #0xcc]
006583cc: ldr r3, [pc, #0xcc]
006583d0: ldr r6, [r0, #0x10]
006583d4: add r7, pc, r7
006583d8: ldr r3, [r7, r3]
006583dc: mov r5, r0
006583e0: add r4, r0, #8
006583e4: add r3, r3, #8
006583e8: str r3, [r0]
006583ec: cmp r4, r6
006583f0: beq #0x658428
006583f4: ldr r0, [r6, #0x28]
006583f8: bl #0x31d584
006583fc: ldr r2, [r6, #0xc]
00658400: cmp r2, #0
00658404: bne #0x658410
00658408: b #0x658468
0065840c: mov r2, r3
00658410: ldr r3, [r2, #8]
00658414: cmp r3, #0
00658418: bne #0x65840c
0065841c: mov r6, r2
00658420: cmp r4, r6
00658424: bne #0x6583f4
00658428: ldr r3, [pc, #0x74]
0065842c: mov r6, #0
00658430: ldr r3, [r7, r3]
00658434: str r6, [r3]
00658438: ldr r3, [r5, #0x18]
0065843c: cmp r3, r6
00658440: beq #0x658460
00658444: mov r0, r4
00658448: ldr r1, [r5, #0xc]
0065844c: bl #0x658270
00658450: str r4, [r5, #0x14]
00658454: str r6, [r5, #0x18]
00658458: str r4, [r5, #0x10]
0065845c: str r6, [r5, #0xc]
00658460: mov r0, r5
00658464: pop {r4, r5, r6, r7, r8, pc}
00658468: ldr r3, [r6, #4]
0065846c: ldr r1, [r3, #0xc]
00658470: cmp r6, r1
00658474: bne #0x658490
00658478: mov r6, r3
0065847c: ldr r3, [r3, #4]
00658480: ldr r2, [r3, #0xc]
00658484: cmp r6, r2
00658488: beq #0x658478
0065848c: ldr r2, [r6, #0xc]
00658490: cmp r2, r3
00658494: movne r6, r3
00658498: b #0x6583ec
0065849c: ldrhteq ip, [r3], -ip
006584a0: andeq r2, r0, r8, asr r2
006584a4: andeq r4, r0, r8, asr #8

# _ZN6glitch7collada15CResFileManager6unloadENSt4priv17_Rb_tree_iteratorISt4pairIKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEEPNS0_8CResFileEENS2_11_MapTraitsTISG_EEEEb 006584fc size104
006584fc: push {r4, r5, r6, lr}
00658500: ldr r3, [r1]
00658504: add r4, r0, #8
00658508: sub sp, sp, #8
0065850c: cmp r4, r3
00658510: mov r5, r1
00658514: moveq r6, #3
00658518: beq #0x658558
0065851c: ldr r0, [r3, #0x28]
00658520: ldr r3, [r0, #4]
00658524: cmp r3, #1
00658528: movls r6, #0
0065852c: bls #0x658540
00658530: cmp r2, #0
00658534: moveq r6, #2
00658538: beq #0x658558
0065853c: mov r6, #1
00658540: bl #0x31d584
00658544: ldr r3, [r5]
00658548: add r1, sp, #8
0065854c: mov r0, r4
00658550: str r3, [r1, #-4]!
00658554: bl #0x6584a8
00658558: mov r0, r6
0065855c: add sp, sp, #8
00658560: pop {r4, r5, r6, pc}

# _ZN6glitch7collada15CResFileManager23updateExternalResourcesEPNS0_8CResFileEPNS_2io9IReadFileE 00658564 size480
00658564: ldr r3, [pc, #0x1d0]
00658568: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065856c: ldr ip, [pc, #0x1cc]
00658570: add r3, pc, r3
00658574: mov fp, r1
00658578: ldr r1, [r3, ip]
0065857c: sub sp, sp, #0x64
00658580: str ip, [sp, #0x24]
00658584: str r3, [sp, #0x20]
00658588: ldr ip, [r1]
0065858c: ldr r3, [fp, #0x24]
00658590: add sb, sp, #0x44
00658594: str ip, [sp, #0x5c]
00658598: ldr r8, [r3, #0x20]
0065859c: str r2, [sp, #0x18]
006585a0: mov r7, r0
006585a4: ldr r2, [r8, #0x4c]
006585a8: mov r1, #0x10
006585ac: mov r0, sb
006585b0: str r2, [sp, #0x10]
006585b4: str sb, [sp, #0x54]
006585b8: str sb, [sp, #0x58]
006585bc: bl #0x3209a8
006585c0: ldr r3, [sp, #0x54]
006585c4: mov r2, #0
006585c8: add r4, sp, #0x2c
006585cc: strb r2, [r3]
006585d0: ldr r3, [r7, #0x20]
006585d4: add r2, fp, #0xc
006585d8: mov r0, r4
006585dc: ldr r3, [r3, #0x34]
006585e0: mov r1, r3
006585e4: ldr r3, [r3]
006585e8: mov lr, pc
006585ec: ldr pc, [r3, #0x38]
006585f0: mov r0, sb
006585f4: ldr r1, [sp, #0x40]
006585f8: ldr r2, [sp, #0x3c]
006585fc: bl #0x320b88
00658600: ldr r0, [sp, #0x40]
00658604: cmp r0, r4
00658608: beq #0x658618
0065860c: cmp r0, #0
00658610: beq #0x658618
00658614: bl #0x310450
00658618: ldr r3, [r7, #0x20]
0065861c: ldr ip, [sp, #0x10]
00658620: ldr r3, [r3, #0x10]
00658624: cmp ip, #0
00658628: ldr r3, [r3, #0xe0]
0065862c: str r3, [sp, #0x14]
00658630: ble #0x6586e8
00658634: mov r5, #0
00658638: add r0, sp, #0x28
0065863c: mov r6, r5
00658640: str r0, [sp, #0x1c]
00658644: ldr r4, [r8, #0x50]
00658648: bl #0x60adc0
0065864c: mov sl, r0
00658650: mov r0, #3
00658654: bl #0x60ad80
00658658: ldr r0, [r7, #0x24]
0065865c: add r4, r4, r5
00658660: mov r3, sb
00658664: ldr ip, [r0]
00658668: mov r1, r0
0065866c: ldr r0, [sp, #0x18]
00658670: mov r2, fp
00658674: str r4, [sp, #8]
00658678: str r0, [sp]
0065867c: ldr r0, [sp, #0x14]
00658680: str r0, [sp, #4]
00658684: ldr r0, [sp, #0x1c]
00658688: mov lr, pc
0065868c: ldr pc, [ip, #8]
00658690: mov r0, sl
00658694: bl #0x60ad80
00658698: ldr r3, [sp, #0x28]
0065869c: cmp r3, #0
006586a0: beq #0x658724
006586a4: ldr r2, [r3, #4]
006586a8: add r2, r2, #1
006586ac: str r2, [r3, #4]
006586b0: ldr r0, [r4, #0x10]
006586b4: str r3, [r4, #0x10]
006586b8: cmp r0, #0
006586bc: beq #0x6586c4
006586c0: bl #0x31d584
006586c4: ldr r0, [sp, #0x28]
006586c8: cmp r0, #0
006586cc: beq #0x6586d4
006586d0: bl #0x31d584
006586d4: ldr r1, [sp, #0x10]
006586d8: add r6, r6, #1
006586dc: add r5, r5, #0x14
006586e0: cmp r6, r1
006586e4: bne #0x658644
006586e8: ldr r0, [sp, #0x58]
006586ec: cmp r0, sb
006586f0: beq #0x658700
006586f4: cmp r0, #0
006586f8: beq #0x658700
006586fc: bl #0x310450
00658700: ldr r2, [sp, #0x24]
00658704: ldr ip, [sp, #0x20]
00658708: ldr r3, [ip, r2]
0065870c: ldr r2, [sp, #0x5c]
00658710: ldr r3, [r3]
00658714: cmp r2, r3
00658718: bne #0x658738
0065871c: add sp, sp, #0x64
00658720: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00658724: ldr r0, [r4, #0x10]
00658728: str r3, [r4, #0x10]
0065872c: cmp r0, #0
00658730: bne #0x6586c0
00658734: b #0x6586c4
00658738: bl #0x30e310
0065873c: eorseq ip, r3, r0, lsr #10
00658740: andeq r4, r0, ip, lsr #1

# _ZN6glitch7collada15CResFileManager15postLoadProcessEPNS0_8CResFileEPNS_2io9IReadFileE 00658c90 size2064
00658c90: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00658c94: ldr r3, [pc, #0x7dc]
00658c98: ldr r4, [pc, #0x7dc]
00658c9c: sub sp, sp, #0x84
00658ca0: add r3, pc, r3
00658ca4: str r3, [sp, #0x14]
00658ca8: ldr r5, [pc, #0x7d0]
00658cac: ldr r3, [r3, r4]
00658cb0: str r4, [sp, #0x28]
00658cb4: str r5, [sp, #0x30]
00658cb8: mov sl, r1
00658cbc: ldr lr, [r3]
00658cc0: ldr r4, [sp, #0x14]
00658cc4: ldr r1, [r1, #4]
00658cc8: ldr ip, [sl, #0x24]
00658ccc: ldr r3, [r4, r5]
00658cd0: str lr, [sp, #0x7c]
00658cd4: ldr r4, [ip, #0x20]
00658cd8: cmp r1, #0
00658cdc: addne r1, r1, #1
00658ce0: str r3, [sp, #0x44]
00658ce4: str r2, [sp, #0x18]
00658ce8: str sl, [sp, #0x40]
00658cec: strne r1, [sl, #4]
00658cf0: ldr r3, [r4, #0x10]
00658cf4: mov fp, r0
00658cf8: cmp r3, #0
00658cfc: bne #0x65938c
00658d00: add r5, sp, #0x40
00658d04: mov r0, r5
00658d08: bl #0x60e2a8
00658d0c: ldr r1, [pc, #0x770]
00658d10: add r1, pc, r1
00658d14: bl #0x30e31c
00658d18: cmp r0, #0
00658d1c: bne #0x6593dc
00658d20: ldr r3, [sl, #0x4c]
00658d24: cmp r3, #0
00658d28: streq r3, [r4, #4]
00658d2c: bne #0x6593a0
00658d30: ldr r8, [r4, #0x24]
00658d34: cmp r8, #0
00658d38: ble #0x658d60
00658d3c: mov r6, #0
00658d40: ldr r7, [r4, #0x28]
00658d44: add r7, r7, r6, lsl #5
00658d48: mov r0, r7
00658d4c: bl #0x611ae0
00658d50: add r6, r6, #1
00658d54: cmp r6, r8
00658d58: str r0, [r7, #0x14]
00658d5c: bne #0x658d40
00658d60: add lr, sp, #0x64
00658d64: str lr, [sp, #0x1c]
00658d68: mov r0, lr
00658d6c: mov r1, #0x10
00658d70: ldr sb, [r4, #0x4c]
00658d74: str lr, [sp, #0x74]
00658d78: str lr, [sp, #0x78]
00658d7c: bl #0x3209a8
00658d80: ldr r3, [sp, #0x74]
00658d84: mov r2, #0
00658d88: add r6, sp, #0x4c
00658d8c: strb r2, [r3]
00658d90: ldr r3, [fp, #0x20]
00658d94: add r2, sl, #0xc
00658d98: mov r0, r6
00658d9c: ldr r3, [r3, #0x34]
00658da0: mov r1, r3
00658da4: ldr r3, [r3]
00658da8: mov lr, pc
00658dac: ldr pc, [r3, #0x38]
00658db0: ldr r0, [sp, #0x1c]
00658db4: ldr r1, [sp, #0x60]
00658db8: ldr r2, [sp, #0x5c]
00658dbc: bl #0x320b88
00658dc0: ldr r0, [sp, #0x60]
00658dc4: cmp r0, r6
00658dc8: beq #0x658dd8
00658dcc: cmp r0, #0
00658dd0: beq #0x658dd8
00658dd4: bl #0x310450
00658dd8: ldr r3, [fp, #0x20]
00658ddc: ldr r0, [r3, #0x34]
00658de0: str r0, [sp, #0x24]
00658de4: ldr r3, [r3, #0x10]
00658de8: cmp r0, #0
00658dec: ldr r3, [r3, #0xe0]
00658df0: str r3, [sp, #0x20]
00658df4: ldrne r3, [r0, #4]
00658df8: addne r3, r3, #1
00658dfc: strne r3, [r0, #4]
00658e00: cmp sb, #0
00658e04: ble #0x658ee4
00658e08: mov r7, #0
00658e0c: add r1, sp, #0x38
00658e10: str r5, [sp, #0x34]
00658e14: mov r8, r7
00658e18: str r1, [sp, #0x2c]
00658e1c: mov r5, r4
00658e20: b #0x658e34
00658e24: add r8, r8, #1
00658e28: cmp r8, sb
00658e2c: add r7, r7, #0x14
00658e30: beq #0x658edc
00658e34: ldr r4, [r5, #0x50]
00658e38: add r4, r4, r7
00658e3c: ldr r3, [r4, #0xc]
00658e40: cmp r3, #0
00658e44: bne #0x658e24
00658e48: bl #0x60adc0
00658e4c: mov r6, r0
00658e50: mov r0, #3
00658e54: bl #0x60ad80
00658e58: ldr r0, [fp, #0x24]
00658e5c: ldr lr, [sp, #0x18]
00658e60: ldr r3, [sp, #0x1c]
00658e64: ldr ip, [r0]
00658e68: mov r1, r0
00658e6c: ldr r0, [sp, #0x20]
00658e70: mov r2, sl
00658e74: str lr, [sp]
00658e78: stmib sp, {r0, r4}
00658e7c: ldr r0, [sp, #0x2c]
00658e80: mov lr, pc
00658e84: ldr pc, [ip, #8]
00658e88: mov r0, r6
00658e8c: bl #0x60ad80
00658e90: ldr r3, [sp, #0x38]
00658e94: cmp r3, #0
00658e98: beq #0x658e24
00658e9c: ldr r2, [r3, #4]
00658ea0: add r2, r2, #1
00658ea4: str r2, [r3, #4]
00658ea8: ldr r0, [r4, #0x10]
00658eac: str r3, [r4, #0x10]
00658eb0: cmp r0, #0
00658eb4: beq #0x658ebc
00658eb8: bl #0x31d584
00658ebc: ldr r0, [sp, #0x38]
00658ec0: cmp r0, #0
00658ec4: beq #0x658e24
00658ec8: add r8, r8, #1
00658ecc: bl #0x31d584
00658ed0: cmp r8, sb
00658ed4: add r7, r7, #0x14
00658ed8: bne #0x658e34
00658edc: mov r4, r5
00658ee0: ldr r5, [sp, #0x34]
00658ee4: ldr r3, [r4, #0x5c]
00658ee8: cmp r3, #0
00658eec: ble #0x658fc8
00658ef0: ldr r2, [pc, #0x590]
00658ef4: mov r7, #0
00658ef8: add r1, sp, #0x38
00658efc: add r2, pc, r2
00658f00: mov r8, r7
00658f04: str r2, [sp, #0x2c]
00658f08: str r1, [sp, #0x18]
00658f0c: mov fp, r7
00658f10: mov sb, #0x14
00658f14: mov sl, r3
00658f18: str r5, [sp, #0x20]
00658f1c: ldr r5, [r4, #0x60]
00658f20: ldr r2, [r4, #0x54]
00658f24: add r5, r5, r7
00658f28: ldr r3, [r5, #0x18]
00658f2c: cmp r3, r2
00658f30: strgt fp, [r5, #0x18]
00658f34: bgt #0x658fb4
00658f38: ldr r1, [r5, #0x10]
00658f3c: cmp r1, #0
00658f40: ble #0x658f9c
00658f44: mov r3, #0
00658f48: mov r2, r3
00658f4c: ldr ip, [r5, #0x14]
00658f50: add ip, ip, r3
00658f54: ldr r0, [ip, #8]
00658f58: cmp r0, #0xa
00658f5c: bls #0x658f88
00658f60: cmp r0, #0xe
00658f64: bhi #0x658f88
00658f68: ldr r0, [ip, #0x14]
00658f6c: ldr r0, [r0]
00658f70: ldr ip, [r0]
00658f74: cmn ip, #1
00658f78: ldrne lr, [r4, #0x50]
00658f7c: streq fp, [r0]
00658f80: mlane ip, sb, ip, lr
00658f84: strne ip, [r0]
00658f88: add r2, r2, #1
00658f8c: cmp r2, r1
00658f90: add r3, r3, #0x18
00658f94: bne #0x658f4c
00658f98: ldr r3, [r5, #0x18]
00658f9c: cmn r3, #1
00658fa0: beq #0x659264
00658fa4: ldr r2, [r4, #0x58]
00658fa8: mov lr, #0x74
00658fac: mla r3, lr, r3, r2
00658fb0: str r3, [r5, #0x18]
00658fb4: add r8, r8, #1
00658fb8: cmp r8, sl
00658fbc: add r7, r7, #0x24
00658fc0: bne #0x658f1c
00658fc4: ldr r5, [sp, #0x20]
00658fc8: ldr r7, [r4, #0x54]
00658fcc: cmp r7, #0
00658fd0: ble #0x6591cc
00658fd4: mov lr, #0
00658fd8: mov r6, lr
00658fdc: mov ip, lr
00658fe0: mov r0, #0x14
00658fe4: ldr r3, [r4, #0x58]
00658fe8: add r3, r3, lr
00658fec: cmn r3, #8
00658ff0: beq #0x659060
00658ff4: ldr r8, [r3, #0x10]
00658ff8: cmp r8, #0
00658ffc: ble #0x659060
00659000: mov r2, #0
00659004: mov r1, r2
00659008: ldr sb, [r3, #0x14]
0065900c: add sb, sb, r2
00659010: ldr sl, [sb, #4]
00659014: cmp sl, #0xa
00659018: bls #0x659050
0065901c: cmp sl, #0xe
00659020: bhi #0x659050
00659024: ldr sl, [sb, #0x14]
00659028: ldr sb, [sl]
0065902c: ldr sl, [sb]
00659030: cmn sl, #1
00659034: streq ip, [sb]
00659038: beq #0x659050
0065903c: ldr fp, [r4, #0x4c]
00659040: cmp sl, fp
00659044: ldrlt fp, [r4, #0x50]
00659048: mlalt sl, r0, sl, fp
0065904c: strlt sl, [sb]
00659050: add r1, r1, #1
00659054: cmp r1, r8
00659058: add r2, r2, #0x18
0065905c: bne #0x659008
00659060: cmn r3, #0x20
00659064: beq #0x6590d4
00659068: ldr r8, [r3, #0x28]
0065906c: cmp r8, #0
00659070: ble #0x6590d4
00659074: mov r2, #0
00659078: mov r1, r2
0065907c: ldr sb, [r3, #0x2c]
00659080: add sb, sb, r2
00659084: ldr sl, [sb, #4]
00659088: cmp sl, #0xa
0065908c: bls #0x6590c4
00659090: cmp sl, #0xe
00659094: bhi #0x6590c4
00659098: ldr sl, [sb, #0x14]
0065909c: ldr sb, [sl]
006590a0: ldr sl, [sb]
006590a4: cmn sl, #1
006590a8: streq ip, [sb]
006590ac: beq #0x6590c4
006590b0: ldr fp, [r4, #0x4c]
006590b4: cmp sl, fp
006590b8: ldrlt fp, [r4, #0x50]
006590bc: mlalt sl, r0, sl, fp
006590c0: strlt sl, [sb]
006590c4: add r1, r1, #1
006590c8: cmp r1, r8
006590cc: add r2, r2, #0x18
006590d0: bne #0x65907c
006590d4: cmn r3, #0x3c
006590d8: beq #0x659148
006590dc: ldr r8, [r3, #0x44]
006590e0: cmp r8, #0
006590e4: ble #0x659148
006590e8: mov r2, #0
006590ec: mov r1, r2
006590f0: ldr sb, [r3, #0x48]
006590f4: add sb, sb, r2
006590f8: ldr sl, [sb, #4]
006590fc: cmp sl, #0xa
00659100: bls #0x659138
00659104: cmp sl, #0xe
00659108: bhi #0x659138
0065910c: ldr sl, [sb, #0x14]
00659110: ldr sb, [sl]
00659114: ldr sl, [sb]
00659118: cmn sl, #1
0065911c: streq ip, [sb]
00659120: beq #0x659138
00659124: ldr fp, [r4, #0x4c]
00659128: cmp sl, fp
0065912c: ldrlt fp, [r4, #0x50]
00659130: mlalt sl, r0, sl, fp
00659134: strlt sl, [sb]
00659138: add r1, r1, #1
0065913c: cmp r1, r8
00659140: add r2, r2, #0x18
00659144: bne #0x6590f0
00659148: cmn r3, #0x58
0065914c: beq #0x6591bc
00659150: ldr r8, [r3, #0x60]
00659154: cmp r8, #0
00659158: ble #0x6591bc
0065915c: mov r2, #0
00659160: mov r1, r2
00659164: ldr sb, [r3, #0x64]
00659168: add sb, sb, r2
0065916c: ldr sl, [sb, #4]
00659170: cmp sl, #0xa
00659174: bls #0x6591ac
00659178: cmp sl, #0xe
0065917c: bhi #0x6591ac
00659180: ldr sl, [sb, #0x14]
00659184: ldr sb, [sl]
00659188: ldr sl, [sb]
0065918c: cmn sl, #1
00659190: streq ip, [sb]
00659194: beq #0x6591ac
00659198: ldr fp, [r4, #0x4c]
0065919c: cmp sl, fp
006591a0: ldrlt fp, [r4, #0x50]
006591a4: mlalt sl, r0, sl, fp
006591a8: strlt sl, [sb]
006591ac: add r1, r1, #1
006591b0: cmp r1, r8
006591b4: add r2, r2, #0x18
006591b8: bne #0x659164
006591bc: add r6, r6, #1
006591c0: cmp r6, r7
006591c4: add lr, lr, #0x74
006591c8: bne #0x658fe4
006591cc: ldr r3, [sp, #0x40]
006591d0: ldr r2, [r3, #0x24]
006591d4: ldr r2, [r2, #0x20]
006591d8: ldr fp, [r2, #0x70]
006591dc: cmp fp, #0
006591e0: ble #0x6592b8
006591e4: mov r8, #0
006591e8: b #0x6591f8
006591ec: add r8, r8, #1
006591f0: cmp r8, fp
006591f4: beq #0x6592b4
006591f8: mov r0, r5
006591fc: mov r1, r8
00659200: bl #0x60e434
00659204: ldr r3, [r0]
00659208: cmp r3, #1
0065920c: bne #0x6591ec
00659210: ldr sb, [r0, #8]
00659214: ldr sl, [sb, #0x10]
00659218: cmp sl, #0
0065921c: ble #0x6591ec
00659220: mov r6, #0
00659224: b #0x659234
00659228: add r6, r6, #1
0065922c: cmp r6, sl
00659230: beq #0x6591ec
00659234: ldr r3, [sp, #0x40]
00659238: ldr r7, [sb, #0x14]
0065923c: ldr r3, [r3, #0x24]
00659240: ldr r1, [r7, r6, lsl #2]
00659244: ldr r3, [r3, #0x20]
00659248: ldr r3, [r3, #0x68]
0065924c: cmp r1, r3
00659250: bhi #0x659228
00659254: mov r0, r5
00659258: bl #0x60e41c
0065925c: str r0, [r7, r6, lsl #2]
00659260: b #0x659228
00659264: ldr r1, [r5, #8]
00659268: cmp r1, #0
0065926c: streq r1, [r5, #0x18]
00659270: beq #0x658fb4
00659274: ldr ip, [sp, #0x14]
00659278: ldr r3, [sp, #0x30]
0065927c: ldr r0, [sp, #0x18]
00659280: ldr r2, [ip, r3]
00659284: bl #0x60f25c
00659288: ldr r6, [sp, #0x38]
0065928c: cmp r6, #0
00659290: beq #0x65945c
00659294: ldr r1, [r5, #0xc]
00659298: ldr r0, [sp, #0x18]
0065929c: add r1, r1, #1
006592a0: bl #0x61b0ac
006592a4: str r0, [r5, #0x18]
006592a8: ldr r0, [sp, #0x18]
006592ac: bl #0x619474
006592b0: b #0x658fb4
006592b4: ldr r3, [sp, #0x40]
006592b8: ldr r3, [r3, #0x24]
006592bc: ldr r3, [r3, #0x20]
006592c0: ldr r7, [r3, #0x78]
006592c4: cmp r7, #0
006592c8: ble #0x659324
006592cc: mov r6, #0
006592d0: b #0x6592e0
006592d4: add r6, r6, #1
006592d8: cmp r6, r7
006592dc: beq #0x659324
006592e0: mov r0, r5
006592e4: mov r1, r6
006592e8: bl #0x60e468
006592ec: ldr r3, [r0, #0x50]
006592f0: cmp r3, #2
006592f4: bne #0x6592d4
006592f8: ldr r8, [r0, #0x54]
006592fc: mov r0, r5
00659300: add r6, r6, #1
00659304: ldr r1, [r8]
00659308: add r1, r1, #1
0065930c: bl #0x61c290
00659310: ldr r3, [r0, #0x44]
00659314: cmp r6, r7
00659318: ldr r3, [r3, #4]
0065931c: str r3, [r8, #4]
00659320: bne #0x6592e0
00659324: ldr r0, [sp, #0x24]
00659328: mov r3, #1
0065932c: str r3, [r4, #0x10]
00659330: cmp r0, #0
00659334: beq #0x659340
00659338: ldr r0, [sp, #0x24]
0065933c: bl #0x31d584
00659340: ldr r0, [sp, #0x78]
00659344: ldr r1, [sp, #0x1c]
00659348: cmp r0, r1
0065934c: beq #0x65935c
00659350: cmp r0, #0
00659354: beq #0x65935c
00659358: bl #0x310450
0065935c: mov r0, r5
00659360: bl #0x619474
00659364: ldr r2, [sp, #0x28]
00659368: ldr r4, [sp, #0x14]
0065936c: mov r0, #0
00659370: ldr r3, [r4, r2]
00659374: ldr r2, [sp, #0x7c]
00659378: ldr r3, [r3]
0065937c: cmp r2, r3
00659380: bne #0x659474
00659384: add sp, sp, #0x84
00659388: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065938c: mov r1, sl
00659390: ldr r2, [sp, #0x18]
00659394: bl #0x658564
00659398: add r5, sp, #0x40
0065939c: b #0x65935c
006593a0: ldr ip, [sp, #0x18]
006593a4: ldr r3, [ip]
006593a8: mov r0, ip
006593ac: mov lr, pc
006593b0: ldr pc, [r3, #0x2c]
006593b4: mov r1, #0
006593b8: mov r6, r0
006593bc: mov r0, #0x18
006593c0: bl #0x5341ac
006593c4: mov r1, r6
006593c8: mov r7, r0
006593cc: add r2, sp, #0x48
006593d0: bl #0x32603c
006593d4: str r7, [r4, #4]
006593d8: b #0x658d30
006593dc: ldr r0, [pc, #0xa8]
006593e0: mov r1, #2
006593e4: add r0, pc, r0
006593e8: bl #0x60aca0
006593ec: ldr r0, [pc, #0x9c]
006593f0: mov r1, #2
006593f4: add r0, pc, r0
006593f8: bl #0x60aca0
006593fc: mov r1, #2
00659400: ldr r0, [sl, #0x20]
00659404: bl #0x60aca0
00659408: mov r0, r5
0065940c: bl #0x60e2a8
00659410: mov r1, #2
00659414: bl #0x60aca0
00659418: ldr r0, [pc, #0x74]
0065941c: mov r1, #2
00659420: add r0, pc, r0
00659424: bl #0x60aca0
00659428: ldr r0, [pc, #0x68]
0065942c: mov r1, #2
00659430: add r0, pc, r0
00659434: bl #0x60aca0
00659438: ldr r0, [pc, #0x5c]
0065943c: mov r1, #2
00659440: add r0, pc, r0
00659444: bl #0x60aca0
00659448: ldr r3, [sl, #0x4c]
0065944c: cmp r3, #0
00659450: streq r3, [r4, #4]
00659454: beq #0x658d30
00659458: b #0x6593a0
0065945c: mov r0, #3
00659460: ldr r1, [sp, #0x2c]
00659464: ldr r2, [r5, #8]
00659468: bl #0x60b034
0065946c: str r6, [r5, #0x18]
00659470: b #0x6592a8
00659474: bl #0x30e310
00659478: ldrshteq fp, [r3], -r0
0065947c: andeq r4, r0, ip, lsr #1
00659480: andeq r4, r0, r0, lsl r7
00659484: eoreq ip, r8, r0, lsl #19
00659488: mlaeq r8, r4, r8, ip

# _ZN6glitch7collada15CResFileManager6unloadEPKcb 00659b60 size220
00659b60: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00659b64: ldr r4, [pc, #0xc8]
00659b68: ldr sb, [pc, #0xc8]
00659b6c: mov r7, r0
00659b70: add r4, pc, r4
00659b74: ldr r0, [r4, sb]
00659b78: ldr r3, [r7, #0x20]
00659b7c: sub sp, sp, #0x44
00659b80: ldr r0, [r0]
00659b84: add r5, sp, #0x24
00659b88: add r6, sp, #0xc
00659b8c: str r0, [sp, #0x3c]
00659b90: ldr r8, [r3, #0x34]
00659b94: mov fp, r2
00659b98: mov r0, r5
00659b9c: ldr r3, [r8]
00659ba0: add r2, sp, #8
00659ba4: ldr sl, [r3, #0x34]
00659ba8: bl #0x32603c
00659bac: mov r2, r5
00659bb0: mov r1, r8
00659bb4: mov r0, r6
00659bb8: blx sl
00659bbc: mov r1, r6
00659bc0: add r0, r7, #8
00659bc4: bl #0x659a8c
00659bc8: add r1, sp, #0x40
00659bcc: str r0, [r1, #-0x3c]!
00659bd0: mov r2, fp
00659bd4: mov r0, r7
00659bd8: bl #0x6584fc
00659bdc: mov r7, r0
00659be0: ldr r0, [sp, #0x20]
00659be4: cmp r0, r6
00659be8: beq #0x659bf8
00659bec: cmp r0, #0
00659bf0: beq #0x659bf8
00659bf4: bl #0x310450
00659bf8: ldr r0, [sp, #0x38]
00659bfc: cmp r0, r5
00659c00: beq #0x659c10
00659c04: cmp r0, #0
00659c08: beq #0x659c10
00659c0c: bl #0x310450
00659c10: ldr r3, [r4, sb]
00659c14: ldr r2, [sp, #0x3c]
00659c18: mov r0, r7
00659c1c: ldr r3, [r3]
00659c20: cmp r2, r3
00659c24: bne #0x659c30
00659c28: add sp, sp, #0x44
00659c2c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00659c30: bl #0x30e310
00659c34: eorseq sl, r3, r0, lsr #30
00659c38: andeq r4, r0, ip, lsr #1

# _ZN6glitch7collada15CResFileManager9unloadAllEv 00659c3c size192
00659c3c: push {r4, r5, r6, r7, r8, lr}
00659c40: ldr r3, [r0, #0x10]
00659c44: add r6, r0, #8
00659c48: mov r5, r0
00659c4c: cmp r6, r3
00659c50: mov r7, #0
00659c54: beq #0x659c9c
00659c58: ldr r4, [r3, #0xc]
00659c5c: cmp r4, #0
00659c60: bne #0x659c6c
00659c64: b #0x659ca4
00659c68: mov r4, r2
00659c6c: ldr r2, [r4, #8]
00659c70: cmp r2, #0
00659c74: bne #0x659c68
00659c78: ldr r1, [r3, #0x24]
00659c7c: mov r0, r5
00659c80: mov r2, #0
00659c84: bl #0x659b60
00659c88: cmp r0, #0
00659c8c: addeq r7, r7, #1
00659c90: mov r3, r4
00659c94: cmp r6, r3
00659c98: bne #0x659c58
00659c9c: mov r0, r7
00659ca0: pop {r4, r5, r6, r7, r8, pc}
00659ca4: ldr r2, [r3, #4]
00659ca8: ldr r1, [r2, #0xc]
00659cac: cmp r3, r1
00659cb0: movne r4, r3
00659cb4: movne r1, #0
00659cb8: bne #0x659cd4
00659cbc: mov r4, r2
00659cc0: ldr r2, [r2, #4]
00659cc4: ldr r1, [r2, #0xc]
00659cc8: cmp r1, r4
00659ccc: beq #0x659cbc
00659cd0: ldr r1, [r4, #0xc]
00659cd4: cmp r1, r2
00659cd8: movne r4, r2
00659cdc: ldr r1, [r3, #0x24]
00659ce0: mov r0, r5
00659ce4: mov r2, #0
00659ce8: bl #0x659b60
00659cec: cmp r0, #0
00659cf0: addeq r7, r7, #1
00659cf4: mov r3, r4
00659cf8: b #0x659c94

# _ZN6glitch7collada15CResFileManager6unloadEPKNS0_8SColladaEb 00659cfc size160
00659cfc: push {r4, r5, r6}
00659d00: ldr r3, [r0, #0x10]
00659d04: add r5, r0, #8
00659d08: cmp r5, r3
00659d0c: beq #0x659d50
00659d10: ldr ip, [r3, #0x28]
00659d14: ldr ip, [ip, #0x24]
00659d18: ldr ip, [ip, #0x20]
00659d1c: cmp r1, ip
00659d20: beq #0x659d90
00659d24: ldr ip, [r3, #0xc]
00659d28: cmp ip, #0
00659d2c: bne #0x659d38
00659d30: b #0x659d5c
00659d34: mov ip, r3
00659d38: ldr r3, [ip, #8]
00659d3c: cmp r3, #0
00659d40: bne #0x659d34
00659d44: mov r3, ip
00659d48: cmp r5, r3
00659d4c: bne #0x659d10
00659d50: mov r0, #3
00659d54: pop {r4, r5, r6}
00659d58: bx lr
00659d5c: ldr r4, [r3, #4]
00659d60: ldr r6, [r4, #0xc]
00659d64: cmp r3, r6
00659d68: bne #0x659d84
00659d6c: mov r3, r4
00659d70: ldr r4, [r4, #4]
00659d74: ldr ip, [r4, #0xc]
00659d78: cmp ip, r3
00659d7c: beq #0x659d6c
00659d80: ldr ip, [r3, #0xc]
00659d84: cmp ip, r4
00659d88: movne r3, r4
00659d8c: b #0x659d08
00659d90: ldr r1, [r3, #0x24]
00659d94: pop {r4, r5, r6}
00659d98: b #0x659b60

# _ZN6glitch7collada15CResFileManager3getEPNS_2io9IReadFileEbb 0065a748 size580
0065a748: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065a74c: ldr r4, [pc, #0x220]
0065a750: ldr fp, [pc, #0x220]
0065a754: ldr sb, [pc, #0x220]
0065a758: add r4, pc, r4
0065a75c: ldr lr, [r4, fp]
0065a760: ldr ip, [r4, sb]
0065a764: sub sp, sp, #0x54
0065a768: ldr lr, [lr]
0065a76c: ldr ip, [ip]
0065a770: mov r8, r0
0065a774: str lr, [sp, #0x4c]
0065a778: ldrb r0, [ip, #0x28]
0065a77c: mov r7, r1
0065a780: add sl, sp, #0x1c
0065a784: str r0, [sp, #4]
0065a788: mov r0, #0
0065a78c: strb r0, [ip, #0x28]
0065a790: ldr r0, [r8, #0x20]
0065a794: ldr r1, [r1]
0065a798: add r5, sp, #0x34
0065a79c: ldr r6, [r0, #0x34]
0065a7a0: str r3, [sp, #0xc]
0065a7a4: mov r0, r7
0065a7a8: ldr r3, [r6]
0065a7ac: str r2, [sp, #8]
0065a7b0: ldr r3, [r3, #0x34]
0065a7b4: str r3, [sp]
0065a7b8: mov lr, pc
0065a7bc: ldr pc, [r1, #0x28]
0065a7c0: add r2, sp, #0x18
0065a7c4: mov r1, r0
0065a7c8: mov r0, sl
0065a7cc: bl #0x32603c
0065a7d0: mov r0, r5
0065a7d4: mov r1, r6
0065a7d8: mov r2, sl
0065a7dc: ldr r3, [sp]
0065a7e0: blx r3
0065a7e4: ldr r0, [sp, #0x30]
0065a7e8: cmp r0, sl
0065a7ec: beq #0x65a7fc
0065a7f0: cmp r0, #0
0065a7f4: beq #0x65a7fc
0065a7f8: bl #0x310450
0065a7fc: add r6, r8, #8
0065a800: mov r0, r6
0065a804: mov r1, r5
0065a808: bl #0x659a8c
0065a80c: cmp r0, r6
0065a810: mov sl, r0
0065a814: beq #0x65a8d4
0065a818: mov r1, r5
0065a81c: mov r0, r6
0065a820: bl #0x65a440
0065a824: ldr r3, [sp, #0x48]
0065a828: add r1, sp, #0x50
0065a82c: mov r0, r6
0065a830: str r3, [r1, #-0x40]!
0065a834: bl #0x65a5cc
0065a838: ldr r6, [r0]
0065a83c: ldr r3, [pc, #0x13c]
0065a840: ldr r2, [r6, #0x24]
0065a844: ldr r1, [r4, r3]
0065a848: ldr r3, [pc, #0x134]
0065a84c: ldr ip, [r2, #0x14]
0065a850: ldr r3, [r4, r3]
0065a854: lsr ip, ip, #0x1f
0065a858: str r2, [r1, ip, lsl #2]
0065a85c: ldr r2, [r6, #0x24]
0065a860: ldr r1, [pc, #0x120]
0065a864: ldr r0, [r3]
0065a868: ldr ip, [r2, #0x10]
0065a86c: ldr r2, [r2, #0x14]
0065a870: ldr r1, [r4, r1]
0065a874: add r0, r0, ip, lsl #2
0065a878: lsr r2, r2, #0x1f
0065a87c: str r0, [r1, r2, lsl #2]
0065a880: ldr r2, [r6, #0x24]
0065a884: ldr r2, [r2, #8]
0065a888: str r2, [r3]
0065a88c: ldr r0, [sp, #0x48]
0065a890: cmp r0, r5
0065a894: beq #0x65a8a4
0065a898: cmp r0, #0
0065a89c: beq #0x65a8a4
0065a8a0: bl #0x310450
0065a8a4: ldr r2, [r4, sb]
0065a8a8: ldr r1, [sp, #4]
0065a8ac: ldr r3, [r4, fp]
0065a8b0: ldr r2, [r2]
0065a8b4: mov r0, r6
0065a8b8: strb r1, [r2, #0x28]
0065a8bc: ldr r2, [sp, #0x4c]
0065a8c0: ldr r3, [r3]
0065a8c4: cmp r2, r3
0065a8c8: bne #0x65a970
0065a8cc: add sp, sp, #0x54
0065a8d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065a8d4: ldr r1, [sp, #8]
0065a8d8: cmp r1, #0
0065a8dc: moveq r6, #0
0065a8e0: beq #0x65a88c
0065a8e4: ldr r3, [sp, #0x48]
0065a8e8: mov r1, #0
0065a8ec: mov r0, #0x50
0065a8f0: str r3, [sp]
0065a8f4: bl #0x5341ac
0065a8f8: ldr r3, [sp]
0065a8fc: mov r2, r7
0065a900: mov r6, r0
0065a904: mov r1, r3
0065a908: ldr r3, [sp, #0xc]
0065a90c: bl #0x658048
0065a910: ldr r3, [sp, #0x48]
0065a914: add r1, sp, #0x50
0065a918: mov r0, sl
0065a91c: str r3, [r1, #-0x3c]!
0065a920: bl #0x65a5cc
0065a924: str r6, [r0]
0065a928: ldr r3, [r6, #0x24]
0065a92c: ldr r3, [r3, #0x14]
0065a930: cmp r3, #0
0065a934: bne #0x65a88c
0065a938: mov r1, r7
0065a93c: mov r0, r8
0065a940: bl #0x657bd0
0065a944: mov r7, r0
0065a948: mov r1, r6
0065a94c: mov r0, r8
0065a950: mov r2, r7
0065a954: bl #0x658c90
0065a958: mov r8, r0
0065a95c: mov r0, r7
0065a960: bl #0x31d584
0065a964: cmp r8, #0
0065a968: movne r6, #0
0065a96c: b #0x65a88c
0065a970: bl #0x30e310
0065a974: eorseq sl, r3, r8, lsr r3
0065a978: andeq r4, r0, ip, lsr #1
0065a97c: andeq r4, r0, r8, asr #8
0065a980: strheq r2, [r0], -r4
0065a984: andeq r1, r0, r4, lsl #1
0065a988: andeq r3, r0, r4, lsr sb

# _ZN6glitch7collada15CResFileManager4loadEPNS_2io9IReadFileEbPFvPKcPKNS0_8SColladaEEb 0065a98c size28
0065a98c: cmp r2, #0
0065a990: ldrb r3, [sp]
0065a994: beq #0x65a9a0
0065a998: mov r0, #0
0065a99c: bx lr
0065a9a0: mov r2, #1
0065a9a4: b #0x65a748

# _ZN6glitch7collada15CResFileManager3getEPKcb 0065a9a8 size692
0065a9a8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065a9ac: ldr r4, [pc, #0x288]
0065a9b0: ldr fp, [pc, #0x288]
0065a9b4: ldr sb, [pc, #0x288]
0065a9b8: add r4, pc, r4
0065a9bc: ldr ip, [r4, fp]
0065a9c0: ldr r3, [r4, sb]
0065a9c4: sub sp, sp, #0x5c
0065a9c8: ldr ip, [ip]
0065a9cc: ldr r3, [r3]
0065a9d0: mov r6, r0
0065a9d4: str ip, [sp, #0x54]
0065a9d8: ldrb r0, [r3, #0x28]
0065a9dc: add r5, sp, #0x24
0065a9e0: add r7, sp, #0x3c
0065a9e4: str r0, [sp, #4]
0065a9e8: mov r0, #0
0065a9ec: strb r0, [r3, #0x28]
0065a9f0: ldr r3, [r6, #0x20]
0065a9f4: str r2, [sp, #8]
0065a9f8: mov r0, r5
0065a9fc: ldr r8, [r3, #0x34]
0065aa00: add r2, sp, #0x20
0065aa04: ldr r3, [r8]
0065aa08: str r1, [sp, #0xc]
0065aa0c: ldr sl, [r3, #0x34]
0065aa10: bl #0x32603c
0065aa14: mov r0, r7
0065aa18: mov r1, r8
0065aa1c: mov r2, r5
0065aa20: blx sl
0065aa24: ldr r0, [sp, #0x38]
0065aa28: cmp r0, r5
0065aa2c: beq #0x65aa3c
0065aa30: cmp r0, #0
0065aa34: beq #0x65aa3c
0065aa38: bl #0x310450
0065aa3c: ldr r3, [sp, #0x50]
0065aa40: add r1, sp, #0x58
0065aa44: add r5, r6, #8
0065aa48: str r3, [r1, #-0x3c]!
0065aa4c: mov r0, r5
0065aa50: bl #0x659d9c
0065aa54: cmp r0, r5
0065aa58: mov r8, r0
0065aa5c: beq #0x65ab10
0065aa60: ldr r3, [sp, #0x50]
0065aa64: add r1, sp, #0x58
0065aa68: mov r0, r5
0065aa6c: str r3, [r1, #-0x44]!
0065aa70: bl #0x65a5cc
0065aa74: ldr r5, [r0]
0065aa78: ldr r3, [pc, #0x1c8]
0065aa7c: ldr r2, [r5, #0x24]
0065aa80: ldr r1, [r4, r3]
0065aa84: ldr r3, [pc, #0x1c0]
0065aa88: ldr ip, [r2, #0x14]
0065aa8c: ldr r3, [r4, r3]
0065aa90: lsr ip, ip, #0x1f
0065aa94: str r2, [r1, ip, lsl #2]
0065aa98: ldr r2, [r5, #0x24]
0065aa9c: ldr r1, [pc, #0x1ac]
0065aaa0: ldr r0, [r3]
0065aaa4: ldr ip, [r2, #0x10]
0065aaa8: ldr r2, [r2, #0x14]
0065aaac: ldr r1, [r4, r1]
0065aab0: add r0, r0, ip, lsl #2
0065aab4: lsr r2, r2, #0x1f
0065aab8: str r0, [r1, r2, lsl #2]
0065aabc: ldr r2, [r5, #0x24]
0065aac0: ldr r2, [r2, #8]
0065aac4: str r2, [r3]
0065aac8: ldr r0, [sp, #0x50]
0065aacc: cmp r0, r7
0065aad0: beq #0x65aae0
0065aad4: cmp r0, #0
0065aad8: beq #0x65aae0
0065aadc: bl #0x310450
0065aae0: ldr r2, [r4, sb]
0065aae4: ldr r1, [sp, #4]
0065aae8: ldr r3, [r4, fp]
0065aaec: ldr r2, [r2]
0065aaf0: mov r0, r5
0065aaf4: strb r1, [r2, #0x28]
0065aaf8: ldr r2, [sp, #0x54]
0065aafc: ldr r3, [r3]
0065ab00: cmp r2, r3
0065ab04: bne #0x65ac10
0065ab08: add sp, sp, #0x5c
0065ab0c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065ab10: ldr r1, [sp, #8]
0065ab14: cmp r1, #0
0065ab18: moveq r5, r1
0065ab1c: beq #0x65aac8
0065ab20: ldr r3, [r6, #0x20]
0065ab24: ldr r1, [sp, #0xc]
0065ab28: ldr r3, [r3, #0x34]
0065ab2c: mov r0, r3
0065ab30: ldr r3, [r3]
0065ab34: mov lr, pc
0065ab38: ldr pc, [r3, #0xc]
0065ab3c: subs sl, r0, #0
0065ab40: beq #0x65ac14
0065ab44: ldr r3, [sp, #0x50]
0065ab48: mov r1, #0
0065ab4c: mov r0, #0x50
0065ab50: str r3, [sp]
0065ab54: bl #0x5341ac
0065ab58: ldr r3, [sp]
0065ab5c: mov r5, r0
0065ab60: mov r2, sl
0065ab64: mov r1, r3
0065ab68: mov r3, #0
0065ab6c: bl #0x658048
0065ab70: cmp r5, #0
0065ab74: beq #0x65aba0
0065ab78: ldr r3, [sp, #0x50]
0065ab7c: add r1, sp, #0x58
0065ab80: mov r0, r8
0065ab84: str r3, [r1, #-0x40]!
0065ab88: bl #0x65a5cc
0065ab8c: str r5, [r0]
0065ab90: ldr r3, [r5, #0x24]
0065ab94: ldr r8, [r3, #0x14]
0065ab98: cmp r8, #0
0065ab9c: beq #0x65abac
0065aba0: mov r0, sl
0065aba4: bl #0x31d584
0065aba8: b #0x65aac8
0065abac: mov r1, sl
0065abb0: mov r0, r6
0065abb4: bl #0x657bd0
0065abb8: mov ip, r0
0065abbc: mov r2, ip
0065abc0: mov r1, r5
0065abc4: mov r0, r6
0065abc8: str ip, [sp]
0065abcc: bl #0x658c90
0065abd0: ldr ip, [sp]
0065abd4: mov r3, r0
0065abd8: str r3, [sp]
0065abdc: mov r0, ip
0065abe0: bl #0x31d584
0065abe4: ldr r3, [sp]
0065abe8: cmp r3, #0
0065abec: beq #0x65aba0
0065abf0: mov r0, r6
0065abf4: ldr r1, [sp, #0x50]
0065abf8: mov r2, r8
0065abfc: bl #0x659b60
0065ac00: mov r0, sl
0065ac04: mov r5, r8
0065ac08: bl #0x31d584
0065ac0c: b #0x65aac8
0065ac10: bl #0x30e310
0065ac14: ldr r0, [pc, #0x38]
0065ac18: mov r5, sl
0065ac1c: add r0, pc, r0
0065ac20: bl #0x60b280
0065ac24: ldr r0, [sp, #0xc]
0065ac28: bl #0x60b280
0065ac2c: ldr r0, [pc, #0x24]
0065ac30: add r0, pc, r0
0065ac34: bl #0x60b280
0065ac38: b #0x65aac8
0065ac3c: ldrsbteq sl, [r3], -r8
0065ac40: andeq r4, r0, ip, lsr #1
0065ac44: andeq r4, r0, r8, asr #8
0065ac48: strheq r2, [r0], -r4
0065ac4c: andeq r1, r0, r4, lsl #1
0065ac50: andeq r3, r0, r4, lsr sb
0065ac54: eoreq sl, r8, ip, lsl #23
0065ac58: mlaeq r8, r8, fp, sl

# _ZN6glitch7collada15CResFileManager4loadEPKcbPFvS3_PKNS0_8SColladaEE 0065ac5c size24
0065ac5c: cmp r2, #0
0065ac60: beq #0x65ac6c
0065ac64: mov r0, #0
0065ac68: bx lr
0065ac6c: mov r2, #1
0065ac70: b #0x65a9a8

# _ZN6glitch7collada15CResFileManager3getEPNS0_8CResFileEPKcb 0065ac74 size336
0065ac74: push {r4, r5, r6, r7, r8, sb, sl, lr}
0065ac78: ldr r4, [pc, #0x138]
0065ac7c: ldr r7, [pc, #0x138]
0065ac80: mov r5, r0
0065ac84: add r4, pc, r4
0065ac88: ldr ip, [r4, r7]
0065ac8c: ldr r0, [r0, #0x20]
0065ac90: sub sp, sp, #0x20
0065ac94: ldr ip, [ip]
0065ac98: add r1, r1, #0xc
0065ac9c: add r6, sp, #4
0065aca0: str ip, [sp, #0x1c]
0065aca4: ldr ip, [r0, #0x34]
0065aca8: mov r8, r2
0065acac: mov r0, r6
0065acb0: mov r2, r1
0065acb4: mov r1, ip
0065acb8: ldr ip, [ip]
0065acbc: mov sb, r3
0065acc0: mov lr, pc
0065acc4: ldr pc, [ip, #0x38]
0065acc8: ldr r1, [sp, #0x18]
0065accc: ldr r3, [sp, #0x14]
0065acd0: cmp r1, r3
0065acd4: beq #0x65ad9c
0065acd8: ldrsb r3, [r3, #-1]
0065acdc: cmp r3, #0x5c
0065ace0: beq #0x65ad04
0065ace4: cmp r3, #0x2f
0065ace8: beq #0x65ad04
0065acec: ldr r1, [pc, #0xcc]
0065acf0: mov r0, r6
0065acf4: add r1, pc, r1
0065acf8: add r2, r1, #1
0065acfc: bl #0x320a4c
0065ad00: ldr r1, [sp, #0x18]
0065ad04: ldr r3, [r5, #0x20]
0065ad08: mov r2, #1
0065ad0c: ldr ip, [r3, #0x34]
0065ad10: mov r3, r2
0065ad14: mov r0, ip
0065ad18: ldr ip, [ip]
0065ad1c: mov lr, pc
0065ad20: ldr pc, [ip, #0x1c]
0065ad24: mov r1, r8
0065ad28: mov sl, r0
0065ad2c: mov r2, sb
0065ad30: mov r0, r5
0065ad34: bl #0x65a9a8
0065ad38: cmp sl, #0
0065ad3c: mov r8, r0
0065ad40: bne #0x65ad7c
0065ad44: ldr r0, [sp, #0x18]
0065ad48: cmp r0, r6
0065ad4c: beq #0x65ad5c
0065ad50: cmp r0, #0
0065ad54: beq #0x65ad5c
0065ad58: bl #0x310450
0065ad5c: ldr r3, [r4, r7]
0065ad60: ldr r2, [sp, #0x1c]
0065ad64: mov r0, r8
0065ad68: ldr r3, [r3]
0065ad6c: cmp r2, r3
0065ad70: bne #0x65adb4
0065ad74: add sp, sp, #0x20
0065ad78: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0065ad7c: ldr r3, [r5, #0x20]
0065ad80: ldr r1, [sp, #0x18]
0065ad84: ldr r3, [r3, #0x34]
0065ad88: mov r0, r3
0065ad8c: ldr r3, [r3]
0065ad90: mov lr, pc
0065ad94: ldr pc, [r3, #0x28]
0065ad98: b #0x65ad44
0065ad9c: mov r1, r8
0065ada0: mov r0, r5
0065ada4: mov r2, sb
0065ada8: bl #0x65a9a8
0065adac: mov r8, r0
0065adb0: b #0x65ad44
0065adb4: bl #0x30e310
0065adb8: eorseq sb, r3, ip, lsl #28
0065adbc: andeq r4, r0, ip, lsr #1
0065adc0: eoreq r5, r6, r4, ror #30

# _ZN6glitch5video14CVertexStreams15setStreamBufferEPNS0_13SVertexStreamERKN5boost13intrusive_ptrINS0_7IBufferEEEb.clone.1 00663ca8 size64
00663ca8: push {r4, lr}
00663cac: ldr r3, [r2]
00663cb0: mov r4, r0
00663cb4: cmp r3, #0
00663cb8: ldrne r2, [r3, #4]
00663cbc: addne r2, r2, #1
00663cc0: strne r2, [r3, #4]
00663cc4: ldr r0, [r1]
00663cc8: str r3, [r1]
00663ccc: cmp r0, #0
00663cd0: beq #0x663cd8
00663cd4: bl #0x31d584
00663cd8: mov r0, r4
00663cdc: mov r1, #1
00663ce0: pop {r4, lr}
00663ce4: b #0x5a0bfc

# _ZN6glitch5video14CVertexStreams15setStreamBufferEPNS0_13SVertexStreamERKN5boost13intrusive_ptrINS0_7IBufferEEEb.clone.0 006dc848 size64
006dc848: push {r4, lr}
006dc84c: ldr r3, [r2]
006dc850: mov r4, r0
006dc854: cmp r3, #0
006dc858: ldrne r2, [r3, #4]
006dc85c: addne r2, r2, #1
006dc860: strne r2, [r3, #4]
006dc864: ldr r0, [r1]
006dc868: str r3, [r1]
006dc86c: cmp r0, #0
006dc870: beq #0x6dc878
006dc874: bl #0x31d584
006dc878: mov r0, r4
006dc87c: mov r1, #1
006dc880: pop {r4, lr}
006dc884: b #0x5a0bfc
