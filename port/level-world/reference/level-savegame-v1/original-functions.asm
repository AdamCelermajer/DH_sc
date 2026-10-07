
# _ZN8Savegame3Job6finishEv 00313720 size 64
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

# _ZN8Savegame3JobD1Ev 00313c90 size 76
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

# _ZN8Savegame3Job4copyERKS0_ 00313cdc size 128
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

# _ZN8SavegameD1Ev 00313dfc size 140
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

# _ZN8SavegameD0Ev 00313e88 size 28
00313e88: push {r4, lr}
00313e8c: mov r4, r0
00313e90: bl #0x313dfc
00313e94: mov r0, r4
00313e98: bl #0x310440
00313e9c: mov r0, r4
00313ea0: pop {r4, pc}

# _ZN8SavegameD2Ev 00313ea4 size 140
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

# _ZN8Savegame18DeleteAllSlotFilesEi 00313fb0 size 256
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

# _ZN8Savegame3JobC1ERKS0_ 003145e0 size 72
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

# _ZN8Savegame10UpdateJobsEv 00314734 size 1480
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

# _ZN8Savegame9FlushJobsEPKc 00314cfc size 164
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

# _ZN8Savegame6AddJobERNS_3JobE 00315110 size 220
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

# _ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_ 00315848 size 188
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

# _ZN8Savegame15initSectionInfoEPKcPFvP11IStreamBasePvES6_S4_ 00315904 size 116
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

# _ZN8Savegame10_cacheFileEP12StreamBuffer 00315ad0 size 1032
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

# _ZN8SavegameC1EPKcb 00315ed8 size 112
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

# _ZN8SavegameC2EPKcb 00315f48 size 112
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

# _ZN8Savegame7saveAllEv 00315fb8 size 1032
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

# _ZN8Savegame5resetEv 003163c0 size 68
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

# _ZN13LevelSavegameD1Ev 00461498 size 92
00461498: push {r4, lr}
0046149c: ldr r3, [pc, #0x48]
004614a0: ldr r2, [pc, #0x48]
004614a4: ldr r1, [r0, #4]
004614a8: add r3, pc, r3
004614ac: ldr r2, [r3, r2]
004614b0: cmp r1, #0
004614b4: mov r4, r0
004614b8: add r2, r2, #8
004614bc: str r2, [r0]
004614c0: beq #0x4614dc
004614c4: ldr r3, [r1]
004614c8: mov r0, r1
004614cc: mov lr, pc
004614d0: ldr pc, [r3, #4]
004614d4: mov r3, #0
004614d8: str r3, [r4, #4]
004614dc: add r0, r4, #0x10
004614e0: bl #0x3139ac
004614e4: mov r0, r4
004614e8: pop {r4, pc}
004614ec: subseq r3, r3, r8, ror #11
004614f0: andeq r2, r0, r0, asr r8

# _ZN13LevelSavegameD0Ev 004614f4 size 28
004614f4: push {r4, lr}
004614f8: mov r4, r0
004614fc: bl #0x461498
00461500: mov r0, r4
00461504: bl #0x310440
00461508: mov r0, r4
0046150c: pop {r4, pc}

# _ZN13LevelSavegameD2Ev 00461510 size 92
00461510: push {r4, lr}
00461514: ldr r3, [pc, #0x48]
00461518: ldr r2, [pc, #0x48]
0046151c: ldr r1, [r0, #4]
00461520: add r3, pc, r3
00461524: ldr r2, [r3, r2]
00461528: cmp r1, #0
0046152c: mov r4, r0
00461530: add r2, r2, #8
00461534: str r2, [r0]
00461538: beq #0x461554
0046153c: ldr r3, [r1]
00461540: mov r0, r1
00461544: mov lr, pc
00461548: ldr pc, [r3, #4]
0046154c: mov r3, #0
00461550: str r3, [r4, #4]
00461554: add r0, r4, #0x10
00461558: bl #0x3139ac
0046155c: mov r0, r4
00461560: pop {r4, pc}
00461564: subseq r3, r3, r0, ror r5
00461568: andeq r2, r0, r0, asr r8

# _ZN13LevelSavegame4LoadEv 0046156c size 128
0046156c: push {r4, r5, lr}
00461570: ldr r4, [pc, #0x58]
00461574: ldr r3, [pc, #0x58]
00461578: ldr r1, [pc, #0x58]
0046157c: add r4, pc, r4
00461580: ldr r2, [r4, r3]
00461584: ldr r3, [pc, #0x50]
00461588: mov r5, r0
0046158c: sub sp, sp, #0xc
00461590: ldr r3, [r4, r3]
00461594: ldr r0, [r0, #4]
00461598: add r1, pc, r1
0046159c: str r5, [sp]
004615a0: bl #0x315848
004615a4: ldr r3, [pc, #0x34]
004615a8: ldr r1, [pc, #0x34]
004615ac: ldr r0, [r5, #4]
004615b0: ldr r2, [r4, r3]
004615b4: ldr r3, [pc, #0x2c]
004615b8: add r1, pc, r1
004615bc: str r5, [sp]
004615c0: ldr r3, [r4, r3]
004615c4: bl #0x315848
004615c8: add sp, sp, #0xc
004615cc: pop {r4, r5, pc}
004615d0: subseq r3, r3, r4, lsl r5
004615d4: andeq r1, r0, r4, lsr r6
004615d8: umaaleq fp, r6, r8, fp
004615dc: ldrdeq r2, r3, [r0], -r8
004615e0: andeq r1, r0, r0, asr #30
004615e4: subeq fp, r6, r0, lsl #23
004615e8: andeq r4, r0, r0, ror #15

# _ZN13LevelSavegame4SaveEv 004615ec size 124
004615ec: push {r4, r5, r6, lr}
004615f0: ldr r3, [r0, #4]
004615f4: ldr r4, [pc, #0x64]
004615f8: mov r5, r0
004615fc: cmp r3, #0
00461600: add r4, pc, r4
00461604: beq #0x461614
00461608: ldrb r3, [r0, #0x39]
0046160c: cmp r3, #0
00461610: beq #0x461618
00461614: pop {r4, r5, r6, pc}
00461618: bl #0x7fd794
0046161c: ldrb r3, [r0, #5]
00461620: cmp r3, #0
00461624: bne #0x461634
00461628: ldr r0, [r5, #4]
0046162c: pop {r4, r5, r6, lr}
00461630: b #0x315fb8
00461634: ldr r3, [pc, #0x28]
00461638: ldr r4, [r4, r3]
0046163c: ldr r0, [r4, #0x40]
00461640: bl #0x36f074
00461644: cmp r0, #0
00461648: beq #0x461614
0046164c: ldr r3, [r4, #0x40]
00461650: ldrb r3, [r3, #0x719]
00461654: cmp r3, #0
00461658: bne #0x461614
0046165c: b #0x461628

# _ZN13LevelSavegame15__SaveLevelInfoEP11IStreamBasePv 004616b8 size 8
004616b8: add r1, r1, #0x28
004616bc: b #0x38b808

# _ZN13LevelSavegame15__LoadLevelInfoEP11IStreamBasePv 00461820 size 8
00461820: add r1, r1, #0x2c
00461824: b #0x38b758

# _GLOBAL__I_.._.._sources_Game_SaveGames_LevelSavegame.cpp 004618d8 size 652
004618d8: push {r4, r5, r6, lr}
004618dc: ldr r5, [pc, #0x1f4]
004618e0: mov r3, #0x3f000000
004618e4: ldr r4, [pc, #0x1f0]
004618e8: add r5, pc, r5
004618ec: str r3, [r5, #8]
004618f0: str r3, [r5]
004618f4: str r3, [r5, #4]
004618f8: bl #0x809a38
004618fc: ldr r3, [pc, #0x1dc]
00461900: strb r0, [r5, #0xc]
00461904: ldr r0, [pc, #0x1d8]
00461908: add r4, pc, r4
0046190c: ldr r1, [r4, r3]
00461910: add r0, pc, r0
00461914: bl #0x80a2e8
00461918: ldr r3, [pc, #0x1c8]
0046191c: strb r0, [r5, #0xd]
00461920: ldr r0, [pc, #0x1c4]
00461924: ldr r1, [r4, r3]
00461928: add r0, pc, r0
0046192c: bl #0x80a2e8
00461930: ldr r3, [pc, #0x1b8]
00461934: strb r0, [r5, #0xe]
00461938: ldr r0, [pc, #0x1b4]
0046193c: ldr r1, [r4, r3]
00461940: add r0, pc, r0
00461944: bl #0x80a2e8
00461948: ldr r3, [pc, #0x1a8]
0046194c: strb r0, [r5, #0xf]
00461950: ldr r0, [pc, #0x1a4]
00461954: ldr r1, [r4, r3]
00461958: add r0, pc, r0
0046195c: bl #0x80a2e8
00461960: ldr r3, [pc, #0x198]
00461964: strb r0, [r5, #0x10]
00461968: ldr r0, [pc, #0x194]
0046196c: ldr r1, [r4, r3]
00461970: add r0, pc, r0
00461974: bl #0x80a2e8
00461978: ldr r3, [pc, #0x188]
0046197c: strb r0, [r5, #0x11]
00461980: ldr r0, [pc, #0x184]
00461984: ldr r1, [r4, r3]
00461988: add r0, pc, r0
0046198c: bl #0x80a2e8
00461990: ldr r3, [pc, #0x178]
00461994: strb r0, [r5, #0x12]
00461998: ldr r0, [pc, #0x174]
0046199c: ldr r1, [r4, r3]
004619a0: add r0, pc, r0
004619a4: bl #0x80a2e8
004619a8: ldr r3, [pc, #0x168]
004619ac: strb r0, [r5, #0x13]
004619b0: ldr r0, [pc, #0x164]
004619b4: ldr r1, [r4, r3]
004619b8: add r0, pc, r0
004619bc: bl #0x80a2e8
004619c0: ldr r3, [pc, #0x158]
004619c4: strb r0, [r5, #0x14]
004619c8: ldr r0, [pc, #0x154]
004619cc: ldr r1, [r4, r3]
004619d0: add r0, pc, r0
004619d4: bl #0x80a2e8
004619d8: ldr r3, [pc, #0x148]
004619dc: strb r0, [r5, #0x15]
004619e0: ldr r0, [pc, #0x144]
004619e4: ldr r1, [r4, r3]
004619e8: add r0, pc, r0
004619ec: bl #0x80a2e8
004619f0: ldr r3, [pc, #0x138]
004619f4: strb r0, [r5, #0x16]
004619f8: ldr r0, [pc, #0x134]
004619fc: ldr r1, [r4, r3]
00461a00: add r0, pc, r0
00461a04: bl #0x80a2e8
00461a08: ldr r3, [pc, #0x128]
00461a0c: strb r0, [r5, #0x17]
00461a10: ldr r0, [pc, #0x124]
00461a14: ldr r1, [r4, r3]
00461a18: add r0, pc, r0
00461a1c: bl #0x80a2e8
00461a20: ldr r3, [pc, #0x118]
00461a24: strb r0, [r5, #0x18]
00461a28: ldr r0, [pc, #0x114]
00461a2c: ldr r1, [r4, r3]
00461a30: add r0, pc, r0
00461a34: bl #0x80a2e8
00461a38: strb r0, [r5, #0x19]
00461a3c: bl #0x8099a0
00461a40: ldr r3, [pc, #0x100]
00461a44: strb r0, [r5, #0x1a]
00461a48: ldr r3, [r4, r3]
00461a4c: ldr r2, [r3]
00461a50: tst r2, #1
00461a54: beq #0x461aa4
00461a58: ldr r3, [pc, #0xec]
00461a5c: ldr r3, [r4, r3]
00461a60: ldr r2, [r3]
00461a64: tst r2, #1
00461a68: beq #0x461a70
00461a6c: pop {r4, r5, r6, pc}
00461a70: mov r2, #1
00461a74: str r2, [r3]
00461a78: ldr r3, [pc, #0xd0]
00461a7c: ldr r5, [r4, r3]
00461a80: mov r0, r5
00461a84: bl #0x32d79c
00461a88: ldr r3, [pc, #0xc4]
00461a8c: mov r0, r5
00461a90: ldr r1, [r4, r3]
00461a94: ldr r3, [pc, #0xbc]
00461a98: ldr r2, [r4, r3]
00461a9c: pop {r4, r5, r6, lr}
00461aa0: b #0x30e304
00461aa4: mov r2, #1
00461aa8: str r2, [r3]
00461aac: ldr r3, [pc, #0xa8]
00461ab0: ldr r5, [r4, r3]
00461ab4: mov r0, r5
00461ab8: bl #0x3790a8
00461abc: ldr r3, [pc, #0x9c]
00461ac0: mov r0, r5
00461ac4: ldr r1, [r4, r3]
00461ac8: ldr r3, [pc, #0x88]
00461acc: ldr r2, [r4, r3]
00461ad0: bl #0x30e304
00461ad4: b #0x461a58
00461ad8: subseq r4, r4, r0, lsr #14
00461adc: subseq r3, r3, r8, lsl #3
00461ae0: andeq r2, r0, r4, lsr #30
00461ae4: subeq sp, r5, r0, lsl r6
00461ae8: andeq r1, r0, r8, asr #16
00461aec: subeq sp, r5, r0, ror #11
00461af0: strheq r3, [r0], -r0
00461af4: strheq sp, [r5], #-0x50
00461af8: andeq r0, r0, r4, lsl #13
00461afc: subeq sp, r5, r8, lsl #11
00461b00: andeq r0, r0, ip, asr lr
00461b04: subeq sp, r5, r0, ror #10
00461b08: andeq r2, r0, r8, ror #12
00461b0c: subeq sp, r5, r8, lsr r5
00461b10: andeq r0, r0, r0, lsr #13
00461b14: subeq sp, r5, r8, lsl #10
00461b18: andeq r0, r0, r4, asr sl
00461b1c: subeq sp, r5, r0, ror #9
00461b20: andeq r3, r0, r4, asr #22
00461b24: strheq sp, [r5], #-0x48
00461b28: andeq r1, r0, r0, lsr #29
00461b2c: umaaleq sp, r5, r0, r4
00461b30: andeq r1, r0, r8, lsl r6
00461b34: subeq sp, r5, r8, ror #8
00461b38: andeq r2, r0, r4, lsr r8
00461b3c: subeq sp, r5, r0, asr #8
00461b40: strdeq r3, r4, [r0], -r0
00461b44: subeq sp, r5, r0, lsl #10
00461b48: strdeq r0, r1, [r0], -r4
00461b4c: andeq r0, r0, ip, lsr #31
00461b50: strdeq r3, r4, [r0], -r4
00461b54: andeq r0, r0, r0, asr #17
00461b58: muleq r0, r0, r8
00461b5c: andeq r2, r0, r4, lsl r7
00461b60: muleq r0, ip, r5

# _ZN13LevelSavegame13__LoadObjectsEP11IStreamBasePv 00461e90 size 612
00461e90: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00461e94: ldr r8, [pc, #0x248]
00461e98: ldr r2, [pc, #0x248]
00461e9c: ldr r3, [pc, #0x248]
00461ea0: sub sp, sp, #0x84
00461ea4: add r8, pc, r8
00461ea8: str r2, [sp, #0x24]
00461eac: str r3, [sp, #0x18]
00461eb0: ldr r2, [r8, r2]
00461eb4: ldr r3, [r8, r3]
00461eb8: add r6, sp, #0x64
00461ebc: ldr r2, [r2]
00461ec0: ldr r3, [r3, #0x38]
00461ec4: mov sl, r1
00461ec8: mov r4, r0
00461ecc: mov r1, #0x10
00461ed0: mov r0, r6
00461ed4: str r2, [sp, #0x7c]
00461ed8: str r3, [sp, #0x1c]
00461edc: str r6, [sp, #0x74]
00461ee0: str r6, [sp, #0x78]
00461ee4: bl #0x31167c
00461ee8: ldr r3, [sp, #0x74]
00461eec: add r7, sp, #0x4c
00461ef0: mov r5, #0
00461ef4: strb r5, [r3]
00461ef8: mov r0, r7
00461efc: mov r1, #0x10
00461f00: str r7, [sp, #0x5c]
00461f04: str r7, [sp, #0x60]
00461f08: bl #0x31167c
00461f0c: ldr r3, [sp, #0x5c]
00461f10: strb r5, [r3]
00461f14: ldrb r5, [sl, #0x38]
00461f18: cmp r5, #0
00461f1c: bne #0x4620a4
00461f20: mov r0, r4
00461f24: add r1, sp, #0x48
00461f28: bl #0x313b48
00461f2c: ldr r3, [sp, #0x48]
00461f30: cmp r3, #0
00461f34: beq #0x4620a4
00461f38: ldr r3, [pc, #0x1b0]
00461f3c: add fp, sp, #0x44
00461f40: add ip, sp, #0x38
00461f44: add r3, pc, r3
00461f48: add r0, sp, #0x2c
00461f4c: str r8, [sp, #0x20]
00461f50: str r3, [sp, #0xc]
00461f54: str ip, [sp, #8]
00461f58: str r0, [sp, #0x10]
00461f5c: mov sl, r5
00461f60: mov sb, r6
00461f64: str fp, [sp, #0x14]
00461f68: mov r8, r7
00461f6c: b #0x462008
00461f70: ldr r2, [sp, #0x18]
00461f74: ldr ip, [sp, #0x20]
00461f78: ldr r3, [ip, r2]
00461f7c: mov r2, #1
00461f80: ldr r0, [r3, #0x40]
00461f84: bl #0x36e478
00461f88: ldr r3, [r0, #0x660]
00461f8c: cmp r3, #0
00461f90: beq #0x461fdc
00461f94: mov r0, r3
00461f98: mov r1, r4
00461f9c: ldr r3, [r3]
00461fa0: mov lr, pc
00461fa4: ldr pc, [r3, #0x14]
00461fa8: ldr r3, [r4]
00461fac: mov r0, r4
00461fb0: mov lr, pc
00461fb4: ldr pc, [r3, #0x24]
00461fb8: ldrd r2, r3, [sp, #0x38]
00461fbc: adds r2, r2, r6
00461fc0: adc r3, r3, r7
00461fc4: cmp r0, r2
00461fc8: beq #0x4620d4
00461fcc: ldr r3, [r4]
00461fd0: mov r0, r4
00461fd4: mov lr, pc
00461fd8: ldr pc, [r3, #0x24]
00461fdc: ldrd r2, r3, [sp, #0x38]
00461fe0: adds r2, r2, r6
00461fe4: adc r3, r3, r7
00461fe8: ldr r1, [r4]
00461fec: mov r0, r4
00461ff0: mov lr, pc
00461ff4: ldr pc, [r1, #0x20]
00461ff8: ldr r3, [sp, #0x48]
00461ffc: add r5, r5, #1
00462000: cmp r3, r5
00462004: bls #0x462098
00462008: mov r0, r4
0046200c: mov r1, sb
00462010: bl #0x461da8
00462014: mov r0, r4
00462018: mov r1, r8
0046201c: bl #0x461da8
00462020: mov r0, r4
00462024: ldr r1, [sp, #0x14]
00462028: bl #0x38b758
0046202c: mov r0, r4
00462030: ldr r1, [sp, #8]
00462034: bl #0x461828
00462038: ldr r3, [r4]
0046203c: mov r0, r4
00462040: mov lr, pc
00462044: ldr pc, [r3, #0x24]
00462048: ldr fp, [sp, #0x60]
0046204c: mov r6, r0
00462050: mov r7, r1
00462054: ldr r0, [sp, #0xc]
00462058: mov r1, fp
0046205c: bl #0x30e31c
00462060: subs r1, r0, #0
00462064: beq #0x461f70
00462068: ldr r3, [sp, #0x44]
0046206c: mov r2, fp
00462070: ldr r0, [sp, #0x10]
00462074: ldr r1, [sp, #0x1c]
00462078: str sl, [sp]
0046207c: str sl, [sp, #4]
00462080: bl #0x34aca0
00462084: ldr r0, [sp, #0x10]
00462088: mov r1, sl
0046208c: bl #0x33fdc0
00462090: mov r3, r0
00462094: b #0x461f8c
00462098: mov r7, r8
0046209c: ldr r8, [sp, #0x20]
004620a0: mov r6, sb
004620a4: mov r0, r7
004620a8: bl #0x3139ac
004620ac: mov r0, r6
004620b0: bl #0x3139ac
004620b4: ldr r0, [sp, #0x24]
004620b8: ldr r2, [sp, #0x7c]
004620bc: ldr r3, [r8, r0]
004620c0: ldr r3, [r3]
004620c4: cmp r2, r3
004620c8: bne #0x4620e0
004620cc: add sp, sp, #0x84
004620d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004620d4: cmp r1, r3
004620d8: bne #0x461fcc
004620dc: b #0x461ff8
004620e0: bl #0x30e310
004620e4: subseq r2, r3, ip, ror #23
004620e8: andeq r4, r0, ip, lsr #1
004620ec: strdeq r3, r4, [r0], -r4
004620f0: subeq r5, r6, ip, asr #25

# _ZN13LevelSavegame18DeleteAllLevelSaveEj 004620f4 size 468
004620f4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004620f8: ldr r5, [pc, #0x1a8]
004620fc: ldr r2, [pc, #0x1a8]
00462100: ldr sb, [pc, #0x1a8]
00462104: add r5, pc, r5
00462108: sub sp, sp, #0x420
0046210c: sub sp, sp, #4
00462110: ldr r1, [r5, r2]
00462114: str r2, [sp, #8]
00462118: ldr r2, [r5, sb]
0046211c: ldr r1, [r1]
00462120: mov r3, #0
00462124: ldr r2, [r2, #0x10]
00462128: str r1, [sp, #0x41c]
0046212c: str r3, [sp, #0x18]
00462130: str r3, [sp, #0x10]
00462134: str r3, [sp, #0x14]
00462138: ldr r3, [r2, #0x34]
0046213c: ldr r1, [pc, #0x170]
00462140: add r2, sp, #0x10
00462144: str r2, [sp, #0xc]
00462148: mov r6, r0
0046214c: add r1, pc, r1
00462150: mov r0, r3
00462154: ldr r3, [r3]
00462158: mov lr, pc
0046215c: ldr pc, [r3, #0x7c]
00462160: ldr r1, [pc, #0x150]
00462164: ldr r2, [pc, #0x150]
00462168: add r4, sp, #0x20
0046216c: sub r4, r4, #4
00462170: mov r3, r6
00462174: add r1, pc, r1
00462178: add r2, pc, r2
0046217c: mov r0, r4
00462180: bl #0x30eae4
00462184: ldr r6, [sp, #0x10]
00462188: ldr r3, [pc, #0x130]
0046218c: ldr r8, [sp, #0x14]
00462190: add r6, r6, #0x18
00462194: add r3, pc, r3
00462198: ldr sl, [pc, #0x124]
0046219c: str r3, [sp, #4]
004621a0: sub r3, r6, #0x18
004621a4: cmp r8, r3
004621a8: add sl, pc, sl
004621ac: beq #0x462220
004621b0: ldr r7, [r6, #-4]
004621b4: mov r1, r4
004621b8: mov r0, r7
004621bc: bl #0x30ebd4
004621c0: cmp r0, #0
004621c4: beq #0x462210
004621c8: ldr fp, [r6, #-8]
004621cc: rsb fp, r7, fp
004621d0: mov r8, fp
004621d4: sub fp, fp, #0xf
004621d8: cmp fp, r8
004621dc: bhi #0x462250
004621e0: rsb r8, fp, r8
004621e4: cmp r8, #0xf
004621e8: movhs r8, #0xf
004621ec: cmp r8, #0xf
004621f0: movlt r2, r8
004621f4: movge r2, #0xf
004621f8: add r0, r7, fp
004621fc: mov r1, sl
00462200: bl #0x30e5e0
00462204: cmp r0, #0
00462208: beq #0x462268
0046220c: ldr r8, [sp, #0x14]
00462210: add r6, r6, #0x18
00462214: sub r3, r6, #0x18
00462218: cmp r8, r3
0046221c: bne #0x4621b0
00462220: ldr r0, [sp, #0xc]
00462224: bl #0x313f30
00462228: ldr r2, [sp, #8]
0046222c: mov r0, #1
00462230: ldr r3, [r5, r2]
00462234: ldr r2, [sp, #0x41c]
00462238: ldr r3, [r3]
0046223c: cmp r2, r3
00462240: bne #0x4622a4
00462244: add sp, sp, #0x24
00462248: add sp, sp, #0x400
0046224c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00462250: ldr r0, [sp, #4]
00462254: bl #0x708eb0
00462258: ldr r7, [r6, #-4]
0046225c: ldr r8, [r6, #-8]
00462260: rsb r8, r7, r8
00462264: b #0x4621e0
00462268: cmp r8, #0xe
0046226c: ble #0x46220c
00462270: cmp r8, #0xf
00462274: bne #0x46220c
00462278: ldr r3, [r5, sb]
0046227c: mov r1, r7
00462280: add r6, r6, #0x18
00462284: ldr r3, [r3, #0x10]
00462288: ldr r3, [r3, #0x34]
0046228c: mov r0, r3
00462290: ldr r3, [r3]
00462294: mov lr, pc
00462298: ldr pc, [r3, #0x9c]
0046229c: ldr r8, [sp, #0x14]
004622a0: b #0x462214
004622a4: bl #0x30e310
004622a8: subseq r2, r3, ip, lsl #19
004622ac: andeq r4, r0, ip, lsr #1
004622b0: strdeq r3, r4, [r0], -r4
004622b4: strdeq sl, fp, [r6], #-0xfc
004622b8: ldrdeq sl, fp, [r6], #-0xfc
004622bc: subeq ip, r5, r0, ror #7
004622c0: subeq ip, r5, r4, asr #5
004622c4: strheq sl, [r6], #-0xf0

# _ZN13LevelSavegame21GetCheckpointFilenameEjibRSs 004622c8 size 204
004622c8: push {r4, r5, r6, r7, r8, lr}
004622cc: ldr r4, [pc, #0xa4]
004622d0: ldr r6, [pc, #0xa4]
004622d4: cmp r2, #0
004622d8: add r4, pc, r4
004622dc: ldr r2, [r4, r6]
004622e0: sub sp, sp, #0x58
004622e4: mov r7, r1
004622e8: ldr r2, [r2]
004622ec: mov r8, r3
004622f0: str r2, [sp, #0x54]
004622f4: bne #0x462368
004622f8: ldr lr, [pc, #0x80]
004622fc: add lr, pc, lr
00462300: ldr ip, [pc, #0x7c]
00462304: ldr r1, [pc, #0x7c]
00462308: ldr r2, [pc, #0x7c]
0046230c: add r5, sp, #0x14
00462310: mov r3, r0
00462314: add ip, pc, ip
00462318: add r1, pc, r1
0046231c: add r2, pc, r2
00462320: mov r0, r5
00462324: str lr, [sp, #4]
00462328: str ip, [sp, #8]
0046232c: str r7, [sp]
00462330: bl #0x30eae4
00462334: mov r0, r5
00462338: bl #0x30de54
0046233c: mov r1, r5
00462340: add r2, r5, r0
00462344: mov r0, r8
00462348: bl #0x3109e0
0046234c: ldr r3, [r4, r6]
00462350: ldr r2, [sp, #0x54]
00462354: ldr r3, [r3]
00462358: cmp r2, r3
0046235c: bne #0x462374
00462360: add sp, sp, #0x58
00462364: pop {r4, r5, r6, r7, r8, pc}
00462368: ldr lr, [pc, #0x20]
0046236c: add lr, pc, lr
00462370: b #0x462300
00462374: bl #0x30e310
00462378: ldrheq r2, [r3], #-0x78
0046237c: andeq r4, r0, ip, lsr #1
00462380: subeq sl, r6, ip, ror #28
00462384: subeq sl, r6, r4, ror lr
00462388: subeq sl, r6, r0, ror #28
0046238c: subeq ip, r5, ip, lsr r2
00462390: subeq sl, r6, r4, lsl #28

# _ZNK13LevelSavegame16CheckpointExistsEi 00462394 size 336
00462394: push {r4, r5, r6, r7, r8, sb, sl, lr}
00462398: ldr r4, [pc, #0x138]
0046239c: ldr r6, [pc, #0x138]
004623a0: sub sp, sp, #0x38
004623a4: add r4, pc, r4
004623a8: ldr r3, [r4, r6]
004623ac: add r5, sp, #0x1c
004623b0: mov r7, r0
004623b4: ldr r3, [r3]
004623b8: mov r0, r5
004623bc: mov r8, r1
004623c0: mov r1, #0x10
004623c4: str r3, [sp, #0x34]
004623c8: str r5, [sp, #0x2c]
004623cc: str r5, [sp, #0x30]
004623d0: bl #0x31167c
004623d4: ldr r3, [sp, #0x2c]
004623d8: mov r2, #0
004623dc: strb r2, [r3]
004623e0: ldr sl, [r7, #0xc]
004623e4: bl #0x7fd794
004623e8: ldrb r3, [r0, #5]
004623ec: cmp r3, #0
004623f0: bne #0x462460
004623f4: ldr r7, [pc, #0xe4]
004623f8: mov r2, #0
004623fc: mov r0, r8
00462400: mov r1, sl
00462404: mov r3, r5
00462408: bl #0x4622c8
0046240c: ldr r7, [r4, r7]
00462410: ldr r1, [sp, #0x30]
00462414: ldr r3, [r7, #0x10]
00462418: ldr r3, [r3, #0x34]
0046241c: mov r0, r3
00462420: ldr r3, [r3]
00462424: mov lr, pc
00462428: ldr pc, [r3, #0xb0]
0046242c: cmp r0, #0
00462430: movne r8, #1
00462434: beq #0x462480
00462438: mov r0, r5
0046243c: bl #0x3139ac
00462440: ldr r3, [r4, r6]
00462444: ldr r2, [sp, #0x34]
00462448: mov r0, r8
0046244c: ldr r3, [r3]
00462450: cmp r2, r3
00462454: bne #0x4624d4
00462458: add sp, sp, #0x38
0046245c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00462460: ldr r7, [pc, #0x78]
00462464: ldr sb, [r4, r7]
00462468: ldr r0, [sb, #0x40]
0046246c: bl #0x36f074
00462470: cmp r0, #0
00462474: bne #0x4624bc
00462478: mov r2, #1
0046247c: b #0x4623fc
00462480: ldr r3, [r7, #0x10]
00462484: add r7, sp, #4
00462488: mov r1, r5
0046248c: ldr r8, [r3, #0x34]
00462490: mov r0, r7
00462494: ldr r3, [r8]
00462498: ldr sl, [r3, #0xb0]
0046249c: bl #0x461b64
004624a0: mov r0, r8
004624a4: ldr r1, [sp, #0x18]
004624a8: blx sl
004624ac: mov r8, r0
004624b0: mov r0, r7
004624b4: bl #0x3139ac
004624b8: b #0x462438
004624bc: ldr r3, [sb, #0x40]
004624c0: ldrb r3, [r3, #0x719]
004624c4: cmp r3, #0
004624c8: beq #0x4623f8
004624cc: mov r2, #1
004624d0: b #0x4623fc
004624d4: bl #0x30e310
004624d8: subseq r2, r3, ip, ror #13
004624dc: andeq r4, r0, ip, lsr #1
004624e0: strdeq r3, r4, [r0], -r4

# _ZN13LevelSavegame11GetFilenameEjiiiRSs 004624e4 size 184
004624e4: push {r4, r5, r6, r7, r8, lr}
004624e8: ldr ip, [pc, #0x98]
004624ec: ldr lr, [pc, #0x98]
004624f0: sub sp, sp, #0x410
004624f4: add ip, pc, ip
004624f8: ldr r5, [ip, lr]
004624fc: sub sp, sp, #8
00462500: bic r6, r2, r2, asr #31
00462504: bic r7, r1, r1, asr #31
00462508: ldr lr, [pc, #0x80]
0046250c: ldr r1, [pc, #0x80]
00462510: ldr r2, [pc, #0x80]
00462514: ldr r8, [r5]
00462518: add r4, sp, #0x18
0046251c: sub r4, r4, #4
00462520: add lr, pc, lr
00462524: add r1, pc, r1
00462528: add r2, pc, r2
0046252c: str r3, [sp]
00462530: mov r3, r0
00462534: mov r0, r4
00462538: str lr, [sp, #0xc]
0046253c: str r6, [sp, #8]
00462540: str r8, [sp, #0x414]
00462544: ldr r6, [sp, #0x430]
00462548: str r7, [sp, #4]
0046254c: bl #0x30eae4
00462550: mov r0, r4
00462554: bl #0x30de54
00462558: mov r1, r4
0046255c: add r2, r4, r0
00462560: mov r0, r6
00462564: bl #0x3109e0
00462568: ldr r2, [sp, #0x414]
0046256c: ldr r3, [r5]
00462570: cmp r2, r3
00462574: bne #0x462584
00462578: add sp, sp, #0x18
0046257c: add sp, sp, #0x400
00462580: pop {r4, r5, r6, r7, r8, pc}
00462584: bl #0x30e310

# _ZN13LevelSavegame14LoadCheckPointEiii 0046259c size 344
0046259c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004625a0: ldr r6, [pc, #0x140]
004625a4: ldr r8, [pc, #0x140]
004625a8: sub sp, sp, #0x34
004625ac: add r6, pc, r6
004625b0: ldr ip, [r6, r8]
004625b4: add r4, sp, #0x14
004625b8: mov r5, r0
004625bc: ldr ip, [ip]
004625c0: mov r0, r4
004625c4: mov r7, r1
004625c8: mov r1, #0x10
004625cc: str ip, [sp, #0x2c]
004625d0: mov sl, r2
004625d4: mov sb, r3
004625d8: str r4, [sp, #0x24]
004625dc: str r4, [sp, #0x28]
004625e0: bl #0x31167c
004625e4: ldr r3, [sp, #0x24]
004625e8: mov r2, #0
004625ec: strb r2, [r3]
004625f0: ldr fp, [r5, #0xc]
004625f4: bl #0x7fd794
004625f8: ldrb r3, [r0, #5]
004625fc: cmp r3, #0
00462600: bne #0x4626a8
00462604: mov r2, #0
00462608: mov r1, fp
0046260c: mov r3, r4
00462610: mov r0, r7
00462614: bl #0x4622c8
00462618: ldr fp, [sp, #0x28]
0046261c: mov r0, fp
00462620: bl #0x30de54
00462624: ldr r3, [r5, #4]
00462628: add r2, fp, r0
0046262c: mov r1, fp
00462630: add r0, r3, #4
00462634: bl #0x3109e0
00462638: mov r1, #0
0046263c: ldr r0, [r5, #4]
00462640: bl #0x315ad0
00462644: mov r0, r5
00462648: bl #0x46156c
0046264c: ldr r3, [r5, #0xc]
00462650: mov r1, sl
00462654: mov r2, sb
00462658: mov r0, r7
0046265c: str r4, [sp]
00462660: bl #0x4624e4
00462664: ldr r7, [sp, #0x28]
00462668: mov r0, r7
0046266c: bl #0x30de54
00462670: ldr r3, [r5, #4]
00462674: add r2, r7, r0
00462678: mov r1, r7
0046267c: add r0, r3, #4
00462680: bl #0x3109e0
00462684: mov r0, r4
00462688: bl #0x3139ac
0046268c: ldr r3, [r6, r8]
00462690: ldr r2, [sp, #0x2c]
00462694: ldr r3, [r3]
00462698: cmp r2, r3
0046269c: bne #0x4626e4
004626a0: add sp, sp, #0x34
004626a4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004626a8: ldr r3, [pc, #0x40]
004626ac: ldr r3, [r6, r3]
004626b0: ldr r0, [r3, #0x40]
004626b4: str r3, [sp, #0xc]
004626b8: bl #0x36f074
004626bc: cmp r0, #0
004626c0: ldr r3, [sp, #0xc]
004626c4: moveq r2, #1
004626c8: beq #0x462608
004626cc: ldr r3, [r3, #0x40]
004626d0: ldrb r3, [r3, #0x719]
004626d4: cmp r3, #0
004626d8: beq #0x462604
004626dc: mov r2, #1
004626e0: b #0x462608
004626e4: bl #0x30e310
004626e8: subseq r2, r3, r4, ror #9
004626ec: andeq r4, r0, ip, lsr #1
004626f0: strdeq r3, r4, [r0], -r4

# _ZN13LevelSavegame6DeleteEjiiibb 004626f4 size 304
004626f4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004626f8: ldr r4, [pc, #0x114]
004626fc: ldr r6, [pc, #0x114]
00462700: sub sp, sp, #0x34
00462704: add r4, pc, r4
00462708: ldr ip, [r4, r6]
0046270c: add r5, sp, #0x14
00462710: str r1, [sp, #0xc]
00462714: ldr ip, [ip]
00462718: ldrb r7, [sp, #0x58]
0046271c: mov r8, r0
00462720: mov r1, #0x10
00462724: mov r0, r5
00462728: mov sb, r2
0046272c: mov sl, r3
00462730: str ip, [sp, #0x2c]
00462734: str r5, [sp, #0x24]
00462738: str r5, [sp, #0x28]
0046273c: ldrb fp, [sp, #0x5c]
00462740: bl #0x31167c
00462744: ldr r3, [sp, #0x24]
00462748: mov r2, #0
0046274c: cmp r7, #0
00462750: strb r2, [r3]
00462754: beq #0x4627f4
00462758: mov r0, r8
0046275c: mov r1, sl
00462760: mov r2, fp
00462764: mov r3, r5
00462768: bl #0x4622c8
0046276c: ldr r3, [pc, #0xa8]
00462770: ldr r1, [sp, #0x28]
00462774: ldr r7, [r4, r3]
00462778: ldr r3, [r7, #0x10]
0046277c: ldr r3, [r3, #0x34]
00462780: mov r0, r3
00462784: ldr r3, [r3]
00462788: mov lr, pc
0046278c: ldr pc, [r3, #0x9c]
00462790: ldr r1, [pc, #0x88]
00462794: mov r8, r0
00462798: mov r0, r5
0046279c: add r1, pc, r1
004627a0: add r2, r1, #4
004627a4: bl #0x310804
004627a8: ldr r3, [r7, #0x10]
004627ac: ldr r1, [sp, #0x28]
004627b0: ldr r3, [r3, #0x34]
004627b4: mov r0, r3
004627b8: ldr r3, [r3]
004627bc: mov lr, pc
004627c0: ldr pc, [r3, #0x9c]
004627c4: orr r8, r0, r8
004627c8: mov r0, r5
004627cc: bl #0x3139ac
004627d0: ldr r3, [r4, r6]
004627d4: ldr r2, [sp, #0x2c]
004627d8: uxtb r8, r8
004627dc: ldr r3, [r3]
004627e0: mov r0, r8
004627e4: cmp r2, r3
004627e8: bne #0x462810
004627ec: add sp, sp, #0x34
004627f0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004627f4: mov r0, r8
004627f8: ldr r1, [sp, #0xc]
004627fc: mov r2, sb
00462800: mov r3, sl
00462804: str r5, [sp]
00462808: bl #0x4624e4
0046280c: b #0x46276c
00462810: bl #0x30e310
00462814: subseq r2, r3, ip, lsl #7
00462818: andeq r4, r0, ip, lsr #1
0046281c: strdeq r3, r4, [r0], -r4
00462820: subeq fp, r5, r4, asr #27

# _ZN13LevelSavegame6ExistsEjiii 00462824 size 272
00462824: push {r4, r5, r6, r7, r8, sb, sl, lr}
00462828: ldr r4, [pc, #0xf8]
0046282c: ldr r6, [pc, #0xf8]
00462830: sub sp, sp, #0x40
00462834: add r4, pc, r4
00462838: ldr ip, [r4, r6]
0046283c: add r5, sp, #0x24
00462840: mov r7, r0
00462844: ldr ip, [ip]
00462848: mov sb, r1
0046284c: mov r0, r5
00462850: mov r1, #0x10
00462854: mov r8, r3
00462858: str ip, [sp, #0x3c]
0046285c: mov sl, r2
00462860: str r5, [sp, #0x34]
00462864: str r5, [sp, #0x38]
00462868: bl #0x31167c
0046286c: ldr r3, [sp, #0x34]
00462870: mov r2, #0
00462874: mov r0, r7
00462878: strb r2, [r3]
0046287c: mov r1, sb
00462880: mov r3, r8
00462884: mov r2, sl
00462888: str r5, [sp]
0046288c: bl #0x4624e4
00462890: ldr r3, [pc, #0x98]
00462894: ldr r1, [sp, #0x38]
00462898: ldr r7, [r4, r3]
0046289c: ldr r3, [r7, #0x10]
004628a0: ldr r3, [r3, #0x34]
004628a4: mov r0, r3
004628a8: ldr r3, [r3]
004628ac: mov lr, pc
004628b0: ldr pc, [r3, #0xb0]
004628b4: cmp r0, #0
004628b8: movne r8, #1
004628bc: beq #0x4628e8
004628c0: mov r0, r5
004628c4: bl #0x3139ac
004628c8: ldr r3, [r4, r6]
004628cc: ldr r2, [sp, #0x3c]
004628d0: mov r0, r8
004628d4: ldr r3, [r3]
004628d8: cmp r2, r3
004628dc: bne #0x462924
004628e0: add sp, sp, #0x40
004628e4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
004628e8: ldr r3, [r7, #0x10]
004628ec: add r7, sp, #0xc
004628f0: mov r1, r5
004628f4: ldr r8, [r3, #0x34]
004628f8: mov r0, r7
004628fc: ldr r3, [r8]
00462900: ldr sl, [r3, #0xb0]
00462904: bl #0x461b64
00462908: mov r0, r8
0046290c: ldr r1, [sp, #0x20]
00462910: blx sl
00462914: mov r8, r0
00462918: mov r0, r7
0046291c: bl #0x3139ac
00462920: b #0x4628c0
00462924: bl #0x30e310
00462928: subseq r2, r3, ip, asr r2
0046292c: andeq r4, r0, ip, lsr #1
00462930: strdeq r3, r4, [r0], -r4

# _ZN13LevelSavegameC1EP5Leveljiiib 00462934 size 436
00462934: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00462938: ldr r5, [pc, #0x184]
0046293c: ldr ip, [pc, #0x184]
00462940: ldr r8, [pc, #0x184]
00462944: add r5, pc, r5
00462948: ldr ip, [r5, ip]
0046294c: ldr lr, [r5, r8]
00462950: mov r4, r0
00462954: add r0, ip, #8
00462958: sub sp, sp, #0x2c
0046295c: ldr lr, [lr]
00462960: str r0, [r4]
00462964: str r1, [r4, #8]
00462968: ldr r1, [sp, #0x54]
0046296c: mov r7, #0
00462970: add ip, r4, #0x10
00462974: str r1, [r4, #0xc]
00462978: str ip, [r4, #0x20]
0046297c: str ip, [r4, #0x24]
00462980: mov r0, ip
00462984: str r7, [r4, #4]
00462988: mov r1, #0x10
0046298c: str lr, [sp, #0x24]
00462990: mov sl, r2
00462994: mov fp, r3
00462998: ldrb sb, [sp, #0x58]
0046299c: bl #0x31167c
004629a0: ldr r2, [r4, #0x20]
004629a4: mvn r3, #0
004629a8: add r6, sp, #0xc
004629ac: strb r7, [r2]
004629b0: mov r2, #1
004629b4: str r3, [r4, #0x34]
004629b8: strb r2, [r4, #0x38]
004629bc: ldr r2, [sp, #0x50]
004629c0: mov r0, r6
004629c4: str r3, [r4, #0x2c]
004629c8: str r3, [r4, #0x30]
004629cc: str r2, [r4, #0x28]
004629d0: strb r7, [r4, #0x39]
004629d4: mov r1, #0x10
004629d8: str r6, [sp, #0x1c]
004629dc: str r6, [sp, #0x20]
004629e0: bl #0x31167c
004629e4: ldr r3, [sp, #0x1c]
004629e8: cmp sb, r7
004629ec: strb r7, [r3]
004629f0: beq #0x462aa4
004629f4: mov r0, sl
004629f8: mov r2, r7
004629fc: ldr r1, [r4, #0xc]
00462a00: mov r3, r6
00462a04: bl #0x4622c8
00462a08: mov r1, #0
00462a0c: mov r0, #0x3c
00462a10: ldr sl, [sp, #0x20]
00462a14: bl #0x310570
00462a18: mov r1, sl
00462a1c: mov r2, #0
00462a20: mov r7, r0
00462a24: bl #0x315ed8
00462a28: ldr r3, [pc, #0xa0]
00462a2c: ldr r1, [pc, #0xa0]
00462a30: mov r0, r7
00462a34: ldr r2, [r5, r3]
00462a38: ldr r3, [pc, #0x98]
00462a3c: add r1, pc, r1
00462a40: str r7, [r4, #4]
00462a44: ldr r3, [r5, r3]
00462a48: str r4, [sp]
00462a4c: bl #0x315904
00462a50: ldr r3, [pc, #0x84]
00462a54: ldr r1, [pc, #0x84]
00462a58: ldr r0, [r4, #4]
00462a5c: ldr r2, [r5, r3]
00462a60: ldr r3, [pc, #0x7c]
00462a64: add r1, pc, r1
00462a68: str r4, [sp]
00462a6c: ldr r3, [r5, r3]
00462a70: bl #0x315904
00462a74: mov r3, #0
00462a78: strb r3, [r4, #0x38]
00462a7c: mov r0, r6
00462a80: bl #0x3139ac
00462a84: ldr r3, [r5, r8]
00462a88: ldr r2, [sp, #0x24]
00462a8c: mov r0, r4
00462a90: ldr r3, [r3]
00462a94: cmp r2, r3
00462a98: bne #0x462ac0
00462a9c: add sp, sp, #0x2c
00462aa0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00462aa4: ldr r3, [r4, #0xc]
00462aa8: mov r0, sl
00462aac: mov r1, fp
00462ab0: ldr r2, [sp, #0x50]
00462ab4: str r6, [sp]
00462ab8: bl #0x4624e4
00462abc: b #0x462a08
00462ac0: bl #0x30e310
00462ac4: subseq r2, r3, ip, asr #2
00462ac8: andeq r2, r0, r0, asr r8
00462acc: andeq r4, r0, ip, lsr #1
00462ad0: andeq r1, r0, r4, lsr r6
00462ad4: strdeq sl, fp, [r6], #-0x64
00462ad8: ldrdeq r2, r3, [r0], -r8
00462adc: andeq r1, r0, r0, asr #30
00462ae0: ldrdeq sl, fp, [r6], #-0x64
00462ae4: andeq r4, r0, r0, ror #15

# _ZN13LevelSavegame9LoadLevelEjiii 00462ae8 size 232
00462ae8: push {r4, r5, r6, r7, r8, sb, sl, lr}
00462aec: ldr r4, [pc, #0xd0]
00462af0: ldr r6, [pc, #0xd0]
00462af4: sub sp, sp, #0x1f0
00462af8: add r4, pc, r4
00462afc: ldr ip, [r4, r6]
00462b00: mov r8, r0
00462b04: mov r7, r2
00462b08: ldr ip, [ip]
00462b0c: mov sl, r3
00462b10: str ip, [sp, #0x1ec]
00462b14: bl #0x462824
00462b18: cmp r0, #0
00462b1c: beq #0x462ba4
00462b20: mov r5, #0
00462b24: add sb, sp, #0x1b0
00462b28: mov r2, r7
00462b2c: mov r1, r5
00462b30: mov r3, r5
00462b34: add r7, sp, #0x18
00462b38: mov r0, sb
00462b3c: str r5, [sp]
00462b40: str r5, [sp, #4]
00462b44: str r5, [sp, #8]
00462b48: bl #0x462934
00462b4c: mov r1, r8
00462b50: mov r2, #1
00462b54: mov r3, r5
00462b58: mov r0, r7
00462b5c: bl #0x4655ac
00462b60: ldr r3, [pc, #0x64]
00462b64: mov ip, #1
00462b68: ldr r1, [sp, #0x1d4]
00462b6c: ldr r0, [r4, r3]
00462b70: ldr r2, [sp, #0x1e0]
00462b74: ldr r3, [sp, #0x1e4]
00462b78: str ip, [sp]
00462b7c: str sl, [sp, #8]
00462b80: str r5, [sp, #0x14]
00462b84: str r5, [sp, #4]
00462b88: str r5, [sp, #0xc]
00462b8c: str r5, [sp, #0x10]
00462b90: bl #0x32bdc8
00462b94: mov r0, r7
00462b98: bl #0x46378c
00462b9c: mov r0, sb
00462ba0: bl #0x461498
00462ba4: ldr r3, [r4, r6]
00462ba8: ldr r2, [sp, #0x1ec]
00462bac: ldr r3, [r3]
00462bb0: cmp r2, r3
00462bb4: bne #0x462bc0
00462bb8: add sp, sp, #0x1f0
00462bbc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00462bc0: bl #0x30e310

# _ZN13LevelSavegameC2EP5Leveljiiib 00462bd0 size 436
00462bd0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00462bd4: ldr r5, [pc, #0x184]
00462bd8: ldr ip, [pc, #0x184]
00462bdc: ldr r8, [pc, #0x184]
00462be0: add r5, pc, r5
00462be4: ldr ip, [r5, ip]
00462be8: ldr lr, [r5, r8]
00462bec: mov r4, r0
00462bf0: add r0, ip, #8
00462bf4: sub sp, sp, #0x2c
00462bf8: ldr lr, [lr]
00462bfc: str r0, [r4]
00462c00: str r1, [r4, #8]
00462c04: ldr r1, [sp, #0x54]
00462c08: mov r7, #0
00462c0c: add ip, r4, #0x10
00462c10: str r1, [r4, #0xc]
00462c14: str ip, [r4, #0x20]
00462c18: str ip, [r4, #0x24]
00462c1c: mov r0, ip
00462c20: str r7, [r4, #4]
00462c24: mov r1, #0x10
00462c28: str lr, [sp, #0x24]
00462c2c: mov sl, r2
00462c30: mov fp, r3
00462c34: ldrb sb, [sp, #0x58]
00462c38: bl #0x31167c
00462c3c: ldr r2, [r4, #0x20]
00462c40: mvn r3, #0
00462c44: add r6, sp, #0xc
00462c48: strb r7, [r2]
00462c4c: mov r2, #1
00462c50: str r3, [r4, #0x34]
00462c54: strb r2, [r4, #0x38]
00462c58: ldr r2, [sp, #0x50]
00462c5c: mov r0, r6
00462c60: str r3, [r4, #0x2c]
00462c64: str r3, [r4, #0x30]
00462c68: str r2, [r4, #0x28]
00462c6c: strb r7, [r4, #0x39]
00462c70: mov r1, #0x10
00462c74: str r6, [sp, #0x1c]
00462c78: str r6, [sp, #0x20]
00462c7c: bl #0x31167c
00462c80: ldr r3, [sp, #0x1c]
00462c84: cmp sb, r7
00462c88: strb r7, [r3]
00462c8c: beq #0x462d40
00462c90: mov r0, sl
00462c94: mov r2, r7
00462c98: ldr r1, [r4, #0xc]
00462c9c: mov r3, r6
00462ca0: bl #0x4622c8
00462ca4: mov r1, #0
00462ca8: mov r0, #0x3c
00462cac: ldr sl, [sp, #0x20]
00462cb0: bl #0x310570
00462cb4: mov r1, sl
00462cb8: mov r2, #0
00462cbc: mov r7, r0
00462cc0: bl #0x315ed8
00462cc4: ldr r3, [pc, #0xa0]
00462cc8: ldr r1, [pc, #0xa0]
00462ccc: mov r0, r7
00462cd0: ldr r2, [r5, r3]
00462cd4: ldr r3, [pc, #0x98]
00462cd8: add r1, pc, r1
00462cdc: str r7, [r4, #4]
00462ce0: ldr r3, [r5, r3]
00462ce4: str r4, [sp]
00462ce8: bl #0x315904
00462cec: ldr r3, [pc, #0x84]
00462cf0: ldr r1, [pc, #0x84]
00462cf4: ldr r0, [r4, #4]
00462cf8: ldr r2, [r5, r3]
00462cfc: ldr r3, [pc, #0x7c]
00462d00: add r1, pc, r1
00462d04: str r4, [sp]
00462d08: ldr r3, [r5, r3]
00462d0c: bl #0x315904
00462d10: mov r3, #0
00462d14: strb r3, [r4, #0x38]
00462d18: mov r0, r6
00462d1c: bl #0x3139ac
00462d20: ldr r3, [r5, r8]
00462d24: ldr r2, [sp, #0x24]
00462d28: mov r0, r4
00462d2c: ldr r3, [r3]
00462d30: cmp r2, r3
00462d34: bne #0x462d5c
00462d38: add sp, sp, #0x2c
00462d3c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00462d40: ldr r3, [r4, #0xc]
00462d44: mov r0, sl
00462d48: mov r1, fp
00462d4c: ldr r2, [sp, #0x50]
00462d50: str r6, [sp]
00462d54: bl #0x4624e4
00462d58: b #0x462ca4
00462d5c: bl #0x30e310
00462d60: ldrheq r1, [r3], #-0xe0
00462d64: andeq r2, r0, r0, asr r8
00462d68: andeq r4, r0, ip, lsr #1
00462d6c: andeq r1, r0, r4, lsr r6
00462d70: subeq sl, r6, r8, asr r4
00462d74: ldrdeq r2, r3, [r0], -r8
00462d78: andeq r1, r0, r0, asr #30
00462d7c: subeq sl, r6, r8, lsr r4
00462d80: andeq r4, r0, r0, ror #15

# _ZN13LevelSavegame13__SaveObjectsEP11IStreamBasePv 00462d84 size 940
00462d84: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00462d88: ldr r2, [pc, #0x390]
00462d8c: ldr r8, [pc, #0x390]
00462d90: ldr r1, [pc, #0x390]
00462d94: sub sp, sp, #0x74
00462d98: str r2, [sp, #0x14]
00462d9c: add r8, pc, r8
00462da0: str r1, [sp, #8]
00462da4: ldr r2, [r8, r1]
00462da8: ldr r1, [sp, #0x14]
00462dac: add r7, sp, #0x54
00462db0: ldr r2, [r2]
00462db4: ldr r3, [r8, r1]
00462db8: mov r5, r0
00462dbc: str r2, [sp, #0x6c]
00462dc0: ldr r3, [r3, #0x38]
00462dc4: mov sl, #0
00462dc8: mov r0, r7
00462dcc: mov r1, #0x10
00462dd0: ldr r4, [r3, #0x14]
00462dd4: add r6, r3, #0xc
00462dd8: str sl, [sp, #0x38]
00462ddc: str r7, [sp, #0x64]
00462de0: str r7, [sp, #0x68]
00462de4: bl #0x31167c
00462de8: ldr r3, [sp, #0x64]
00462dec: add r2, sp, #0x38
00462df0: str r2, [sp, #0xc]
00462df4: strb sl, [r3]
00462df8: ldr r3, [r5]
00462dfc: mov r0, r5
00462e00: mov lr, pc
00462e04: ldr pc, [r3, #0x30]
00462e08: strd r0, r1, [sp, #0x18]
00462e0c: mov r0, r5
00462e10: ldr r1, [sp, #0xc]
00462e14: bl #0x461770
00462e18: add r3, sp, #0x34
00462e1c: str r3, [sp, #0x20]
00462e20: ldr r3, [pc, #0x304]
00462e24: add r0, sp, #0x28
00462e28: str r0, [sp, #0x10]
00462e2c: add r3, pc, r3
00462e30: str r3, [sp, #0x24]
00462e34: add sb, sp, #0x3c
00462e38: mov sl, r8
00462e3c: cmp r6, r4
00462e40: beq #0x462fc8
00462e44: ldr r8, [r4, #0x2c]
00462e48: cmp r8, #0
00462e4c: beq #0x462f9c
00462e50: ldrb r3, [r8, #0x28]
00462e54: cmp r3, #0
00462e58: beq #0x462f9c
00462e5c: bl #0x7fd794
00462e60: ldrb r3, [r0, #5]
00462e64: cmp r3, #0
00462e68: bne #0x463048
00462e6c: ldr r3, [sp, #0x38]
00462e70: add r3, r3, #1
00462e74: str r3, [sp, #0x38]
00462e78: ldr fp, [r8, #0x5c]
00462e7c: mov r0, fp
00462e80: bl #0x30de54
00462e84: mov r1, fp
00462e88: add r2, fp, r0
00462e8c: mov r0, r7
00462e90: bl #0x3109e0
00462e94: mov r0, r5
00462e98: mov r1, r7
00462e9c: bl #0x461668
00462ea0: ldr r3, [r8]
00462ea4: mov r0, r8
00462ea8: mov lr, pc
00462eac: ldr pc, [r3, #0x24]
00462eb0: cmp r0, #0
00462eb4: bne #0x4630a4
00462eb8: mov r0, r5
00462ebc: add r1, r4, #0x14
00462ec0: bl #0x461668
00462ec4: ldr r3, [r8, #0x64]
00462ec8: mov r0, r5
00462ecc: ldr r1, [sp, #0x20]
00462ed0: str r3, [sp, #0x34]
00462ed4: bl #0x38b808
00462ed8: mov r0, #0
00462edc: mov r1, #0
00462ee0: strd r0, r1, [sp, #0x28]
00462ee4: mov r0, r5
00462ee8: ldr r3, [r5]
00462eec: mov lr, pc
00462ef0: ldr pc, [r3, #0x30]
00462ef4: strd r0, r1, [sp]
00462ef8: mov r0, r5
00462efc: ldr r1, [sp, #0x10]
00462f00: bl #0x4616c0
00462f04: mov r0, r8
00462f08: mov r1, r5
00462f0c: ldr r3, [r8]
00462f10: mov lr, pc
00462f14: ldr pc, [r3, #0x10]
00462f18: ldr r3, [r5]
00462f1c: mov r0, r5
00462f20: mov lr, pc
00462f24: ldr pc, [r3, #0x30]
00462f28: mvn r2, #7
00462f2c: adds r2, r2, r0
00462f30: mvn r3, #0
00462f34: adc r3, r3, r1
00462f38: ldrd r0, r1, [sp]
00462f3c: subs r2, r2, r0
00462f40: sbc r3, r3, r1
00462f44: strd r2, r3, [sp, #0x28]
00462f48: mov r2, r0
00462f4c: mov r3, r1
00462f50: mov r0, r5
00462f54: ldr r1, [r5]
00462f58: mov lr, pc
00462f5c: ldr pc, [r1, #0x2c]
00462f60: mov r0, r5
00462f64: ldr r1, [sp, #0x10]
00462f68: bl #0x4616c0
00462f6c: ldrd r2, r3, [sp, #0x28]
00462f70: mov r0, #8
00462f74: adds r2, r2, r0
00462f78: mov r1, #0
00462f7c: adc r3, r3, r1
00462f80: ldrd r0, r1, [sp]
00462f84: adds r2, r2, r0
00462f88: adc r3, r3, r1
00462f8c: mov r0, r5
00462f90: ldr r1, [r5]
00462f94: mov lr, pc
00462f98: ldr pc, [r1, #0x2c]
00462f9c: ldr r3, [r4, #0xc]
00462fa0: cmp r3, #0
00462fa4: bne #0x462fb0
00462fa8: b #0x463014
00462fac: mov r3, r2
00462fb0: ldr r2, [r3, #8]
00462fb4: cmp r2, #0
00462fb8: bne #0x462fac
00462fbc: mov r4, r3
00462fc0: cmp r6, r4
00462fc4: bne #0x462e44
00462fc8: ldrd r2, r3, [sp, #0x18]
00462fcc: ldr r1, [r5]
00462fd0: mov r0, r5
00462fd4: mov lr, pc
00462fd8: ldr pc, [r1, #0x2c]
00462fdc: ldr r1, [sp, #0xc]
00462fe0: mov r0, r5
00462fe4: bl #0x461770
00462fe8: mov r0, r7
00462fec: bl #0x3139ac
00462ff0: ldr r1, [sp, #8]
00462ff4: ldr r2, [sp, #0x6c]
00462ff8: mov r8, sl
00462ffc: ldr r3, [sl, r1]
00463000: ldr r3, [r3]
00463004: cmp r2, r3
00463008: bne #0x46311c
0046300c: add sp, sp, #0x74
00463010: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00463014: ldr r2, [r4, #4]
00463018: ldr r1, [r2, #0xc]
0046301c: cmp r1, r4
00463020: bne #0x46303c
00463024: mov r4, r2
00463028: ldr r2, [r2, #4]
0046302c: ldr r3, [r2, #0xc]
00463030: cmp r4, r3
00463034: beq #0x463024
00463038: ldr r3, [r4, #0xc]
0046303c: cmp r3, r2
00463040: movne r4, r2
00463044: b #0x462e3c
00463048: ldr r3, [r8]
0046304c: mov r0, r8
00463050: mov lr, pc
00463054: ldr pc, [r3, #0x24]
00463058: cmp r0, #0
0046305c: beq #0x462e6c
00463060: ldr r3, [r8]
00463064: mov r0, r8
00463068: mov lr, pc
0046306c: ldr pc, [r3, #0x28]
00463070: cmp r0, #0
00463074: beq #0x462e6c
00463078: ldr r1, [sp, #0x14]
0046307c: ldr r3, [sl, r1]
00463080: mov r1, r8
00463084: ldr r0, [r3, #0x40]
00463088: bl #0x36effc
0046308c: cmp r0, #0
00463090: beq #0x462f9c
00463094: ldrb r3, [r8, #0x81]
00463098: cmp r3, #0
0046309c: bne #0x462f9c
004630a0: b #0x462e6c
004630a4: ldr r3, [r8]
004630a8: mov r0, r8
004630ac: mov lr, pc
004630b0: ldr pc, [r3, #0x28]
004630b4: cmp r0, #0
004630b8: beq #0x462eb8
004630bc: ldr r0, [sp, #0x24]
004630c0: ldr r1, [r4, #0x28]
004630c4: bl #0x30e31c
004630c8: cmp r0, #0
004630cc: beq #0x462eb8
004630d0: mov r0, sb
004630d4: mov r1, #0x12
004630d8: str sb, [sp, #0x4c]
004630dc: str sb, [sp, #0x50]
004630e0: bl #0x31167c
004630e4: ldr r1, [sp, #0x24]
004630e8: mov r2, #0x11
004630ec: ldr r0, [sp, #0x50]
004630f0: bl #0x30e868
004630f4: mov r2, #0
004630f8: add r3, r0, #0x11
004630fc: str r3, [sp, #0x4c]
00463100: mov r1, sb
00463104: strb r2, [r0, #0x11]
00463108: mov r0, r5
0046310c: bl #0x461668
00463110: mov r0, sb
00463114: bl #0x3139ac
00463118: b #0x462ec4
0046311c: bl #0x30e310
00463120: strdeq r3, r4, [r0], -r4
00463124: ldrsheq r1, [r3], #-0xc4
00463128: andeq r4, r0, ip, lsr #1
0046312c: subeq r4, r6, r4, ror #27

# _ZN13LevelSavegame14SaveCheckPointEiii 00463130 size 344
00463130: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00463134: ldr r4, [pc, #0x140]
00463138: ldr r7, [pc, #0x140]
0046313c: mov r5, r0
00463140: add r4, pc, r4
00463144: ldr ip, [r4, r7]
00463148: ldr r0, [r0, #4]
0046314c: mov r8, r1
00463150: ldr r1, [ip]
00463154: sub sp, sp, #0x34
00463158: cmp r0, #0
0046315c: mov sl, r2
00463160: str r1, [sp, #0x2c]
00463164: mov sb, r3
00463168: beq #0x463220
0046316c: add r6, sp, #0x14
00463170: mov r0, r6
00463174: mov r1, #0x10
00463178: str r6, [sp, #0x24]
0046317c: str r6, [sp, #0x28]
00463180: bl #0x31167c
00463184: ldr r3, [sp, #0x24]
00463188: mov r2, #0
0046318c: strb r2, [r3]
00463190: ldr fp, [r5, #0xc]
00463194: bl #0x7fd794
00463198: ldrb r3, [r0, #5]
0046319c: cmp r3, #0
004631a0: bne #0x46323c
004631a4: mov r2, #0
004631a8: mov r1, fp
004631ac: mov r3, r6
004631b0: mov r0, r8
004631b4: bl #0x4622c8
004631b8: ldr fp, [sp, #0x28]
004631bc: mov r0, fp
004631c0: bl #0x30de54
004631c4: ldr r3, [r5, #4]
004631c8: add r2, fp, r0
004631cc: mov r1, fp
004631d0: add r0, r3, #4
004631d4: bl #0x3109e0
004631d8: ldr r0, [r5, #4]
004631dc: bl #0x315fb8
004631e0: ldr r3, [r5, #0xc]
004631e4: mov r1, sl
004631e8: mov r2, sb
004631ec: mov r0, r8
004631f0: str r6, [sp]
004631f4: bl #0x4624e4
004631f8: ldr r8, [sp, #0x28]
004631fc: mov r0, r8
00463200: bl #0x30de54
00463204: ldr r3, [r5, #4]
00463208: add r2, r8, r0
0046320c: mov r1, r8
00463210: add r0, r3, #4
00463214: bl #0x3109e0
00463218: mov r0, r6
0046321c: bl #0x3139ac
00463220: ldr r3, [r4, r7]
00463224: ldr r2, [sp, #0x2c]
00463228: ldr r3, [r3]
0046322c: cmp r2, r3
00463230: bne #0x463278
00463234: add sp, sp, #0x34
00463238: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046323c: ldr r3, [pc, #0x40]
00463240: ldr r3, [r4, r3]
00463244: ldr r0, [r3, #0x40]
00463248: str r3, [sp, #0xc]
0046324c: bl #0x36f074
00463250: cmp r0, #0
00463254: ldr r3, [sp, #0xc]
00463258: moveq r2, #1
0046325c: beq #0x4631a8
00463260: ldr r3, [r3, #0x40]
00463264: ldrb r3, [r3, #0x719]
00463268: cmp r3, #0
0046326c: beq #0x4631a4
00463270: mov r2, #1
00463274: b #0x4631a8
00463278: bl #0x30e310
0046327c: subseq r1, r3, r0, asr sb
00463280: andeq r4, r0, ip, lsr #1
00463284: strdeq r3, r4, [r0], -r4

# _ZN13LevelSavegame18ValidateCheckpointEiii 00463288 size 604
00463288: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046328c: ldr r4, [pc, #0x238]
00463290: ldr r7, [pc, #0x238]
00463294: sub sp, sp, #0x64
00463298: add r4, pc, r4
0046329c: ldr ip, [r4, r7]
004632a0: add r5, sp, #0x44
004632a4: mov r6, r0
004632a8: ldr ip, [ip]
004632ac: mov r0, r5
004632b0: mov r8, r1
004632b4: mov r1, #0x10
004632b8: str ip, [sp, #0x5c]
004632bc: str r2, [sp, #0xc]
004632c0: mov sb, r3
004632c4: str r5, [sp, #0x54]
004632c8: str r5, [sp, #0x58]
004632cc: bl #0x31167c
004632d0: ldr r3, [sp, #0x54]
004632d4: mov r2, #0
004632d8: strb r2, [r3]
004632dc: ldr fp, [r6, #0xc]
004632e0: bl #0x7fd794
004632e4: ldrb r3, [r0, #5]
004632e8: cmp r3, #0
004632ec: bne #0x46348c
004632f0: ldr sl, [pc, #0x1dc]
004632f4: mov r2, #0
004632f8: mov r1, fp
004632fc: mov r0, r8
00463300: mov r3, r5
00463304: bl #0x4622c8
00463308: ldr sl, [r4, sl]
0046330c: ldr r1, [sp, #0x58]
00463310: ldr r3, [sl, #0x10]
00463314: ldr r3, [r3, #0x34]
00463318: mov r0, r3
0046331c: ldr r3, [r3]
00463320: mov lr, pc
00463324: ldr pc, [r3, #0xb0]
00463328: cmp r0, #0
0046332c: beq #0x4633fc
00463330: ldr sl, [sp, #0x58]
00463334: mov r0, sl
00463338: bl #0x30de54
0046333c: ldr r3, [r6, #4]
00463340: add r2, sl, r0
00463344: mov r1, sl
00463348: add r0, r3, #4
0046334c: bl #0x3109e0
00463350: ldr r0, [r6, #4]
00463354: mov r1, #0
00463358: bl #0x315ad0
0046335c: ldr r3, [pc, #0x174]
00463360: ldr r1, [pc, #0x174]
00463364: ldr r0, [r6, #4]
00463368: ldr r2, [r4, r3]
0046336c: ldr r3, [pc, #0x16c]
00463370: add r1, pc, r1
00463374: str r6, [sp]
00463378: ldr r3, [r4, r3]
0046337c: bl #0x315848
00463380: ldr ip, [r6, #0x2c]
00463384: ldr r3, [r6, #0xc]
00463388: ldr r1, [sp, #0xc]
0046338c: mov r2, sb
00463390: mov r0, r8
00463394: str r5, [sp]
00463398: cmp ip, sb
0046339c: movne r8, #0
004633a0: moveq r8, #1
004633a4: bl #0x4624e4
004633a8: ldr sl, [sp, #0x58]
004633ac: mov r0, sl
004633b0: bl #0x30de54
004633b4: ldr r3, [r6, #4]
004633b8: add r2, sl, r0
004633bc: mov r1, sl
004633c0: add r0, r3, #4
004633c4: bl #0x3109e0
004633c8: ldr r0, [r6, #4]
004633cc: mov r1, #0
004633d0: bl #0x315ad0
004633d4: mov r0, r5
004633d8: bl #0x3139ac
004633dc: ldr r3, [r4, r7]
004633e0: ldr r2, [sp, #0x5c]
004633e4: mov r0, r8
004633e8: ldr r3, [r3]
004633ec: cmp r2, r3
004633f0: bne #0x4634c8
004633f4: add sp, sp, #0x64
004633f8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004633fc: ldr r2, [sl, #0x10]
00463400: add r3, sp, #0x2c
00463404: mov r0, r3
00463408: ldr sl, [r2, #0x34]
0046340c: mov r1, r5
00463410: ldr r2, [sl]
00463414: ldr fp, [r2, #0xb0]
00463418: str r3, [sp, #8]
0046341c: bl #0x461b64
00463420: mov r0, sl
00463424: ldr r1, [sp, #0x40]
00463428: blx fp
0046342c: ldr r3, [sp, #8]
00463430: mov sl, r0
00463434: mov r0, r3
00463438: bl #0x3139ac
0046343c: cmp sl, #0
00463440: moveq r8, sl
00463444: beq #0x4633d4
00463448: ldr r3, [r6, #4]
0046344c: add sl, sp, #0x14
00463450: mov r1, r5
00463454: mov r0, sl
00463458: str r3, [sp, #8]
0046345c: bl #0x461b64
00463460: ldr fp, [sp, #0x28]
00463464: mov r0, fp
00463468: bl #0x30de54
0046346c: ldr r3, [sp, #8]
00463470: add r2, fp, r0
00463474: mov r1, fp
00463478: add r0, r3, #4
0046347c: bl #0x3109e0
00463480: mov r0, sl
00463484: bl #0x3139ac
00463488: b #0x463350
0046348c: ldr sl, [pc, #0x40]
00463490: ldr r3, [r4, sl]
00463494: ldr r0, [r3, #0x40]
00463498: str r3, [sp, #8]
0046349c: bl #0x36f074
004634a0: cmp r0, #0
004634a4: ldr r3, [sp, #8]
004634a8: moveq r2, #1
004634ac: beq #0x4632f8
004634b0: ldr r3, [r3, #0x40]
004634b4: ldrb r3, [r3, #0x719]
004634b8: cmp r3, #0
004634bc: beq #0x4632f4
004634c0: mov r2, #1
004634c4: b #0x4632f8
004634c8: bl #0x30e310
004634cc: ldrsheq r1, [r3], #-0x78
004634d0: andeq r4, r0, ip, lsr #1
004634d4: strdeq r3, r4, [r0], -r4
004634d8: andeq r1, r0, r4, lsr r6
004634dc: subeq sb, r6, r0, asr #27
004634e0: ldrdeq r2, r3, [r0], -r8
