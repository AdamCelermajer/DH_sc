# 0x3694d4 _ZN15VoxSoundManager14BackToMainMenuEv
003694d4: str lr, [sp, #-4]!
003694d8: sub sp, sp, #0xc
003694dc: bl #0x42ca8c
003694e0: bl #0x42cb8c
003694e4: ldr r1, [pc, #0x20]
003694e8: ldr r2, [pc, #0x20]
003694ec: mov ip, #0
003694f0: add r1, pc, r1
003694f4: add r2, pc, r2
003694f8: mov r3, ip
003694fc: str ip, [sp]
00369500: bl #0x7ad7e8
00369504: add sp, sp, #0xc
00369508: ldm sp!, {pc}
0036950c: subseq r7, r5, r0, lsr sl
00369510: subseq r7, r5, r4, asr #20

# 0x369484 _ZN15VoxSoundManager15SetIPodPlaylistEi
00369484: mov r3, #1
00369488: push {r4, lr}
0036948c: strb r3, [r0, #0x33]
00369490: mov r0, r1
00369494: bl #0x533850
00369498: pop {r4, lr}
0036949c: b #0x5339c8

# 0x36b80c _ZN15VoxSoundManager4PlayEibiib
0036b80c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b810: ldr r4, [pc, #0x268]
0036b814: ldr r5, [pc, #0x268]
0036b818: ldr lr, [pc, #0x268]
0036b81c: add r4, pc, r4
0036b820: ldr ip, [r4, r5]
0036b824: ldr r6, [r4, lr]
0036b828: sub sp, sp, #0x8c
0036b82c: ldr ip, [ip]
0036b830: mov r7, r0
0036b834: mov r0, r6
0036b838: str r3, [sp, #0x14]
0036b83c: str ip, [sp, #0x84]
0036b840: mov sl, r1
0036b844: mov fp, r2
0036b848: ldrb sb, [sp, #0xb4]
0036b84c: bl #0x337888
0036b850: ldr r1, [pc, #0x234]
0036b854: add r8, sp, #0x6c
0036b858: add r2, sp, #0x68
0036b85c: add r1, pc, r1
0036b860: mov r0, r8
0036b864: bl #0x3140ec
0036b868: mov r0, r6
0036b86c: mov r1, r8
0036b870: bl #0x337a88
0036b874: mov r6, r0
0036b878: mov r0, r8
0036b87c: bl #0x3139ac
0036b880: cmp r6, #0
0036b884: bne #0x36b9e0
0036b888: cmp sl, #0
0036b88c: blt #0x36b9e0
0036b890: cmp sb, #0
0036b894: beq #0x36ba20
0036b898: ldr r3, [pc, #0x1f0]
0036b89c: ldr r3, [r4, r3]
0036b8a0: ldrb r3, [r3]
0036b8a4: cmp r3, #0
0036b8a8: bne #0x36ba00
0036b8ac: ldr r3, [pc, #0x1e0]
0036b8b0: mov r2, #0xc
0036b8b4: add r1, sp, #0x60
0036b8b8: ldr r3, [r4, r3]
0036b8bc: add ip, sp, #0x5c
0036b8c0: add r8, r7, #0x64
0036b8c4: ldr r3, [r3]
0036b8c8: mov r0, r8
0036b8cc: mla sl, r2, sl, r3
0036b8d0: add r3, sp, #0x58
0036b8d4: ldr r6, [sl, #4]
0036b8d8: add r2, sp, #0x50
0036b8dc: stm sp, {r1, ip}
0036b8e0: mov r1, r6
0036b8e4: add ip, sp, #0x54
0036b8e8: str ip, [sp, #8]
0036b8ec: bl #0x8896f4
0036b8f0: ldr r3, [r7, #8]
0036b8f4: ldr r1, [r3, r6, lsl #2]
0036b8f8: cmp r1, #0
0036b8fc: beq #0x36ba5c
0036b900: ldr r0, [r7]
0036b904: bl #0x8624f8
0036b908: cmp r0, #0
0036b90c: beq #0x36b9e0
0036b910: ldr r0, [sp, #0x14]
0036b914: bl #0x30e964
0036b918: mov r1, #0x44000000
0036b91c: add r1, r1, #0x7a0000
0036b920: bl #0x30ec94
0036b924: ldr r3, [r7, #8]
0036b928: ldr r2, [sp, #0x5c]
0036b92c: mov sl, r0
0036b930: ldr r1, [r3, r6, lsl #2]
0036b934: ldr r0, [r7]
0036b938: bl #0x862618
0036b93c: add ip, sp, #0x67
0036b940: str ip, [sp]
0036b944: add ip, sp, #0x44
0036b948: mov r1, r6
0036b94c: mov r0, r8
0036b950: add r2, sp, #0x4c
0036b954: add r3, sp, #0x48
0036b958: str ip, [sp, #4]
0036b95c: add ip, sp, #0x40
0036b960: str ip, [sp, #8]
0036b964: bl #0x889894
0036b968: ldr r3, [r7, #8]
0036b96c: ldr r1, [r7]
0036b970: mov r8, #0
0036b974: ldr r2, [r3, r6, lsl #2]
0036b978: add r6, sp, #0x18
0036b97c: ldr r3, [sp, #0x4c]
0036b980: mov r0, r6
0036b984: str r8, [sp]
0036b988: bl #0x862438
0036b98c: ldr r0, [r7]
0036b990: mov r1, r6
0036b994: mov r2, r8
0036b998: mov r3, #1
0036b99c: bl #0x861d60
0036b9a0: ldr r3, [sp, #0x40]
0036b9a4: mov r2, r8
0036b9a8: ldr r0, [r7]
0036b9ac: mov r1, r6
0036b9b0: bl #0x861950
0036b9b4: ldr r3, [sp, #0xb0]
0036b9b8: sub r3, r3, #1
0036b9bc: cmp r3, #0x1d
0036b9c0: bls #0x36ba48
0036b9c4: ldr r0, [r7]
0036b9c8: mov r3, sl
0036b9cc: mov r1, r6
0036b9d0: ldrb r2, [sp, #0x67]
0036b9d4: bl #0x8621c0
0036b9d8: mov r0, r6
0036b9dc: bl #0x8683ac
0036b9e0: ldr r3, [r4, r5]
0036b9e4: ldr r2, [sp, #0x84]
0036b9e8: mov r0, #0
0036b9ec: ldr r3, [r3]
0036b9f0: cmp r2, r3
0036b9f4: bne #0x36ba7c
0036b9f8: add sp, sp, #0x8c
0036b9fc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036ba00: ldr r3, [pc, #0x90]
0036ba04: mov r0, sl
0036ba08: mov r2, fp
0036ba0c: ldr r1, [r4, r3]
0036ba10: mov r3, #2
0036ba14: ldr r1, [r1]
0036ba18: bl #0x531348
0036ba1c: b #0x36b9e0
0036ba20: bl #0x7fd794
0036ba24: ldrb r3, [r0, #5]
0036ba28: cmp r3, #0
0036ba2c: beq #0x36b898
0036ba30: ldr r3, [pc, #0x64]
0036ba34: ldr r3, [r4, r3]
0036ba38: ldrb r3, [r3]
0036ba3c: cmp r3, #0
0036ba40: bne #0x36b9e0
0036ba44: b #0x36b898
0036ba48: ldr r0, [r7]
0036ba4c: mov r1, r6
0036ba50: ldr r2, [sp, #0x48]
0036ba54: bl #0x862058
0036ba58: b #0x36b9c4
0036ba5c: mov r1, r6
0036ba60: mov r0, r7
0036ba64: bl #0x3699fc
0036ba68: ldr r3, [r7, #8]
0036ba6c: ldr r1, [r3, r6, lsl #2]
0036ba70: cmp r1, #0
0036ba74: beq #0x36b9e0
0036ba78: b #0x36b900
0036ba7c: bl #0x30e310
0036ba80: rsbeq sb, r2, r4, ror r2
0036ba84: andeq r4, r0, ip, lsr #1
0036ba88: andeq r0, r0, r4, lsl #17
0036ba8c: subseq r5, r5, ip, lsl #17
0036ba90: andeq r3, r0, r0, lsr fp
0036ba94: andeq r3, r0, ip, lsr lr
0036ba98: andeq r0, r0, r0, lsl #13
0036ba9c: andeq r2, r0, r0, lsr #31

# 0x36b120 _ZN15VoxSoundManager21ShowPlayingMusicTitleEPc
0036b120: push {r4, r5, r6, r7, r8, sb, sl, lr}
0036b124: ldr r4, [pc, #0x17c]
0036b128: ldr r5, [pc, #0x17c]
0036b12c: mov r7, r0
0036b130: add r4, pc, r4
0036b134: ldr r3, [r4, r5]
0036b138: ldr r0, [pc, #0x170]
0036b13c: sub sp, sp, #0x48
0036b140: ldr r3, [r3]
0036b144: add r0, pc, r0
0036b148: mov sl, r1
0036b14c: str r3, [sp, #0x44]
0036b150: bl #0x324114
0036b154: bl #0x42ca8c
0036b158: bl #0x42cb8c
0036b15c: subs r8, r0, #0
0036b160: beq #0x36b248
0036b164: ldr r3, [pc, #0x148]
0036b168: ldr r0, [r4, r3]
0036b16c: bl #0x31f504
0036b170: cmp r0, #0
0036b174: beq #0x36b248
0036b178: ldr r1, [pc, #0x138]
0036b17c: add r6, sp, #0x30
0036b180: mov r2, #2
0036b184: add r1, pc, r1
0036b188: mov r0, r6
0036b18c: bl #0x30eae4
0036b190: mov r1, r6
0036b194: mov r0, r8
0036b198: bl #0x7a9160
0036b19c: mov r1, r0
0036b1a0: mov sb, r0
0036b1a4: ldr r0, [pc, #0x110]
0036b1a8: add r6, sp, #0x18
0036b1ac: add r0, pc, r0
0036b1b0: bl #0x324114
0036b1b4: mov r1, sl
0036b1b8: add r2, sp, #0x14
0036b1bc: mov r0, r6
0036b1c0: bl #0x3140ec
0036b1c4: ldr r3, [sp, #0x28]
0036b1c8: ldr r1, [sp, #0x2c]
0036b1cc: rsb r2, r1, r3
0036b1d0: cmp r2, #0xf
0036b1d4: bhi #0x36b264
0036b1d8: add r0, r7, #0x38
0036b1dc: cmp r0, r6
0036b1e0: beq #0x36b1f0
0036b1e4: ldr r2, [sp, #0x28]
0036b1e8: bl #0x3109e0
0036b1ec: ldr r1, [sp, #0x2c]
0036b1f0: ldr r0, [pc, #0xc8]
0036b1f4: add r7, sp, #8
0036b1f8: add r0, pc, r0
0036b1fc: bl #0x324114
0036b200: mov r3, #0
0036b204: mov r0, r7
0036b208: ldr r1, [sp, #0x2c]
0036b20c: strb r3, [sp, #9]
0036b210: strb r3, [sp, #8]
0036b214: bl #0x797350
0036b218: ldr r2, [pc, #0xa4]
0036b21c: mov ip, #1
0036b220: mov r1, sb
0036b224: add r2, pc, r2
0036b228: mov r3, r7
0036b22c: mov r0, r8
0036b230: str ip, [sp]
0036b234: bl #0x7abe0c
0036b238: mov r0, r7
0036b23c: bl #0x797124
0036b240: mov r0, r6
0036b244: bl #0x3139ac
0036b248: ldr r3, [r4, r5]
0036b24c: ldr r2, [sp, #0x44]
0036b250: ldr r3, [r3]
0036b254: cmp r2, r3
0036b258: bne #0x36b2a4
0036b25c: add sp, sp, #0x48
0036b260: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0036b264: add r2, r1, #0xc
0036b268: cmp r3, r2
0036b26c: beq #0x36b288
0036b270: ldrb r0, [r3]
0036b274: rsb r3, r3, r2
0036b278: strb r0, [r1, #0xc]
0036b27c: ldr r2, [sp, #0x28]
0036b280: add r3, r2, r3
0036b284: str r3, [sp, #0x28]
0036b288: ldr r1, [pc, #0x38]
0036b28c: mov r0, r6
0036b290: add r1, pc, r1
0036b294: add r2, r1, #3
0036b298: bl #0x310804
0036b29c: ldr r1, [sp, #0x2c]
0036b2a0: b #0x36b1d8
0036b2a4: bl #0x30e310
0036b2a8: rsbeq sb, r2, r0, ror #18
0036b2ac: andeq r4, r0, ip, lsr #1

# 0x3698f0 _ZN15VoxSoundManager12StopAllMusicEi
003698f0: ldr r3, [pc, #0x8c]
003698f4: ldr r2, [pc, #0x8c]
003698f8: push {r4, r5, lr}
003698fc: add r3, pc, r3
00369900: ldr r2, [r3, r2]
00369904: sub sp, sp, #0xc
00369908: mov r4, r0
0036990c: ldrb r3, [r2]
00369910: mov r5, r1
00369914: cmp r3, #0
00369918: bne #0x369968
0036991c: ldr r1, [pc, #0x68]
00369920: add r2, sp, #8
00369924: str r3, [r2, #-4]!
00369928: add r1, pc, r1
0036992c: add r0, r0, #0x64
00369930: bl #0x88cc14
00369934: mov r0, r5
00369938: bl #0x30e964
0036993c: ldr r5, [r4]
00369940: mov r2, r0
00369944: ldr r1, [sp, #4]
00369948: mov r0, r5
0036994c: bl #0x8620d0
00369950: ldr r3, [r4, #0x24]
00369954: mvn r2, #0
00369958: str r2, [r4, #0x24]
0036995c: str r3, [r4, #0x28]
00369960: add sp, sp, #0xc
00369964: pop {r4, r5, pc}
00369968: mvn r0, #0
0036996c: bl #0x531940
00369970: ldr r3, [r4, #0x24]
00369974: mvn r2, #0
00369978: str r2, [r4, #0x24]
0036997c: str r3, [r4, #0x28]
00369980: b #0x369960
00369984: mlseq r2, r4, r1, fp
00369988: andeq r3, r0, r0, lsr fp
0036998c: subseq r7, r5, r0, asr #12

# 0x36a664 _ZN15VoxSoundManager5PauseEi
0036a664: push {r4, r5, r6, r7, r8, sb, lr}
0036a668: ldr r3, [pc, #0x104]
0036a66c: cmp r1, #0
0036a670: sub sp, sp, #0x194
0036a674: mov r6, r0
0036a678: add r3, pc, r3
0036a67c: blt #0x36a760
0036a680: ldr r2, [pc, #0xf0]
0036a684: ldr r2, [r3, r2]
0036a688: ldrb r4, [r2]
0036a68c: cmp r4, #0
0036a690: bne #0x36a768
0036a694: ldr r2, [r0, #8]
0036a698: ldr r0, [pc, #0xdc]
0036a69c: mov ip, #0xc
0036a6a0: ldr r0, [r3, r0]
0036a6a4: ldr r0, [r0]
0036a6a8: mla r1, ip, r1, r0
0036a6ac: ldr r1, [r1, #4]
0036a6b0: ldr r0, [r2, r1, lsl #2]
0036a6b4: cmp r0, #0
0036a6b8: beq #0x36a760
0036a6bc: ldr r0, [pc, #0xbc]
0036a6c0: mov r5, sp
0036a6c4: add ip, sp, #0x1b8
0036a6c8: ldr r0, [r3, r0]
0036a6cc: mvn r8, #0
0036a6d0: add r3, sp, #0x28
0036a6d4: add r0, r0, #8
0036a6d8: mvn sb, #0
0036a6dc: strd r8, sb, [r3, #-0x20]
0036a6e0: str r4, [r3, #-0x18]
0036a6e4: str r4, [r3, #-0x14]
0036a6e8: str r4, [r3, #-0x10]
0036a6ec: str r4, [r3, #-0xc]
0036a6f0: str r4, [r3, #-8]
0036a6f4: str r0, [r3, #-0x28]
0036a6f8: add r3, r3, #0x28
0036a6fc: cmp r3, ip
0036a700: bne #0x36a6dc
0036a704: ldr r1, [r2, r1, lsl #2]
0036a708: ldr r0, [r6]
0036a70c: mov r2, sp
0036a710: mov r3, #0xa
0036a714: bl #0x862548
0036a718: subs r7, r0, #0
0036a71c: ble #0x36a744
0036a720: mov r8, #0x28
0036a724: movw r2, #0xcccd
0036a728: mla r1, r8, r4, r5
0036a72c: ldr r0, [r6]
0036a730: add r4, r4, #1
0036a734: movt r2, #0x3d4c
0036a738: bl #0x862170
0036a73c: cmp r4, r7
0036a740: bne #0x36a724
0036a744: add r4, sp, #0x190
0036a748: ldr r3, [r4, #-0x28]!
0036a74c: mov r0, r4
0036a750: mov lr, pc
0036a754: ldr pc, [r3]
0036a758: cmp r4, r5
0036a75c: bne #0x36a748
0036a760: add sp, sp, #0x194
0036a764: pop {r4, r5, r6, r7, r8, sb, pc}
0036a768: mov r0, r1
0036a76c: bl #0x531420
0036a770: b #0x36a760
0036a774: rsbeq sl, r2, r8, lsl r4
0036a778: andeq r3, r0, r0, lsr fp
0036a77c: andeq r3, r0, ip, lsr lr
0036a780: andeq r2, r0, r8, lsr #28

# 0x36c7b0 _ZN15VoxSoundManagerC1Ev
0036c7b0: push {r4, r5, r6, r7, r8, sl, lr}
0036c7b4: ldr r5, [pc, #0x294]
0036c7b8: ldr r6, [pc, #0x294]
0036c7bc: mov r4, r0
0036c7c0: add r5, pc, r5
0036c7c4: ldr r3, [r5, r6]
0036c7c8: mov r7, #0
0036c7cc: add r2, r0, #0x38
0036c7d0: ldr r3, [r3]
0036c7d4: mvn r1, #0
0036c7d8: mov r8, #1
0036c7dc: str r1, [r0, #0x2c]
0036c7e0: sub sp, sp, #0x214
0036c7e4: mov r0, r2
0036c7e8: str r1, [r4, #0x24]
0036c7ec: str r1, [r4, #0x28]
0036c7f0: str r2, [r4, #0x48]
0036c7f4: str r2, [r4, #0x4c]
0036c7f8: str r7, [r4]
0036c7fc: str r7, [r4, #0xc]
0036c800: str r7, [r4, #0x10]
0036c804: str r7, [r4, #0x14]
0036c808: strb r8, [r4, #0x18]
0036c80c: str r7, [r4, #0x1c]
0036c810: strb r7, [r4, #0x20]
0036c814: strb r8, [r4, #0x30]
0036c818: strb r8, [r4, #0x31]
0036c81c: strb r7, [r4, #0x32]
0036c820: strb r7, [r4, #0x33]
0036c824: mov r1, #0x10
0036c828: str r3, [sp, #0x20c]
0036c82c: bl #0x31167c
0036c830: ldr r1, [r4, #0x48]
0036c834: mov r2, r4
0036c838: mov r3, #0x3e8
0036c83c: strb r7, [r1]
0036c840: ldr r0, [pc, #0x210]
0036c844: mov r1, #0x3f800000
0036c848: str r3, [r4, #0x58]
0036c84c: str r3, [r4, #0x54]
0036c850: str r1, [r4, #0x5c]
0036c854: mov r3, r4
0036c858: strb r7, [r4, #0x60]
0036c85c: str r7, [r4, #0x64]
0036c860: str r7, [r4, #0x68]
0036c864: str r7, [r4, #0x6c]
0036c868: str r7, [r4, #0x70]
0036c86c: str r7, [r4, #0x74]
0036c870: str r7, [r4, #0x78]
0036c874: str r7, [r4, #0x7c]
0036c878: str r7, [r4, #0x80]
0036c87c: str r7, [r4, #0x84]
0036c880: str r7, [r4, #0x88]
0036c884: str r7, [r4, #0x8c]
0036c888: str r7, [r4, #0x90]
0036c88c: str r7, [r4, #0x98]
0036c890: strb r7, [r2, #0x94]!
0036c894: str r2, [r4, #0xa0]
0036c898: str r2, [r4, #0x9c]
0036c89c: str r7, [r4, #0xa4]
0036c8a0: str r7, [r4, #0xb0]
0036c8a4: strb r7, [r3, #0xac]!
0036c8a8: str r3, [r4, #0xb8]
0036c8ac: str r3, [r4, #0xb4]
0036c8b0: str r7, [r4, #0xbc]
0036c8b4: add r0, pc, r0
0036c8b8: bl #0x324114
0036c8bc: ldr r3, [pc, #0x198]
0036c8c0: ldr r3, [r5, r3]
0036c8c4: ldrb r3, [r3, #0xa8]
0036c8c8: cmp r3, r7
0036c8cc: bne #0x36ca14
0036c8d0: ldr r3, [pc, #0x188]
0036c8d4: add r7, sp, #0xc
0036c8d8: mov r0, r7
0036c8dc: ldr r3, [r5, r3]
0036c8e0: ldr r1, [r3]
0036c8e4: bl #0x30e520
0036c8e8: mov r0, r7
0036c8ec: bl #0x30de54
0036c8f0: ldr r1, [pc, #0x16c]
0036c8f4: mov r2, #0xd
0036c8f8: add r0, r7, r0
0036c8fc: add r1, pc, r1
0036c900: bl #0x30e868
0036c904: mov r0, r7
0036c908: bl #0x30de54
0036c90c: ldr r1, [pc, #0x154]
0036c910: mov r2, #0xb
0036c914: add r0, r7, r0
0036c918: add r1, pc, r1
0036c91c: bl #0x30e868
0036c920: mov r1, r7
0036c924: add r0, r4, #0x64
0036c928: bl #0x88d344
0036c92c: ldr r0, [pc, #0x138]
0036c930: add r0, pc, r0
0036c934: bl #0x324114
0036c938: ldr r2, [r4, #0x68]
0036c93c: ldr r3, [r4, #0x64]
0036c940: mov r1, #4
0036c944: rsb r3, r3, r2
0036c948: asr r3, r3, #2
0036c94c: lsl r2, r3, r1
0036c950: rsb r2, r3, r2
0036c954: add r2, r2, r2, lsl #8
0036c958: add r2, r2, r2, lsl #16
0036c95c: add r3, r3, r2, lsl #4
0036c960: str r3, [r4, #0x1c]
0036c964: lsl r0, r3, #2
0036c968: bl #0x31056c
0036c96c: ldr r2, [r4, #0x1c]
0036c970: mov r1, #0
0036c974: str r0, [r4, #4]
0036c978: lsl r2, r2, #2
0036c97c: bl #0x30e460
0036c980: ldr r0, [pc, #0xe8]
0036c984: add r0, pc, r0
0036c988: bl #0x324114
0036c98c: ldr r0, [r4, #0x1c]
0036c990: mov r1, #4
0036c994: lsl r0, r0, #2
0036c998: bl #0x31056c
0036c99c: ldr r2, [r4, #0x1c]
0036c9a0: mov r1, #0
0036c9a4: str r0, [r4, #8]
0036c9a8: lsl r2, r2, #2
0036c9ac: bl #0x30e460
0036c9b0: ldr r0, [pc, #0xbc]
0036c9b4: add r0, pc, r0
0036c9b8: bl #0x324114
0036c9bc: ldr r0, [pc, #0xb4]
0036c9c0: add r0, pc, r0
0036c9c4: bl #0x324114
0036c9c8: bl #0x862b30
0036c9cc: str r0, [r4]
0036c9d0: ldr r3, [r0]
0036c9d4: mov lr, pc
0036c9d8: ldr pc, [r3, #8]
0036c9dc: ldr r0, [pc, #0x98]
0036c9e0: add r0, pc, r0
0036c9e4: bl #0x324114
0036c9e8: ldr r0, [pc, #0x90]
0036c9ec: add r0, pc, r0
0036c9f0: bl #0x324114
0036c9f4: ldr r3, [r5, r6]
0036c9f8: ldr r2, [sp, #0x20c]
0036c9fc: mov r0, r4
0036ca00: ldr r3, [r3]
0036ca04: cmp r2, r3
0036ca08: bne #0x36ca4c
0036ca0c: add sp, sp, #0x214
0036ca10: pop {r4, r5, r6, r7, r8, sl, pc}
0036ca14: bl #0x8945a4
0036ca18: ldr r3, [r0]
0036ca1c: mov sl, r0
0036ca20: ldr r0, [pc, #0x5c]
0036ca24: ldr r7, [r3, #0x10]
0036ca28: add r0, pc, r0
0036ca2c: bl #0x56e064
0036ca30: mov r2, r8
0036ca34: mov r1, r0
0036ca38: str r8, [sp]
0036ca3c: mov r0, sl
0036ca40: mov r3, r8
0036ca44: blx r7
0036ca48: b #0x36c8d0
0036ca4c: bl #0x30e310

# 0x36bd78 _ZN15VoxSoundManager9PlayMusicEibbi
0036bd78: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036bd7c: ldr r4, [pc, #0x268]
0036bd80: ldr r5, [pc, #0x268]
0036bd84: mov sb, r2
0036bd88: add r4, pc, r4
0036bd8c: ldr ip, [r4, r5]
0036bd90: sub sp, sp, #0x54
0036bd94: mov r6, r1
0036bd98: ldr r2, [ip]
0036bd9c: mov r1, #2
0036bda0: mov fp, r3
0036bda4: str r2, [sp, #0x4c]
0036bda8: mov r7, r0
0036bdac: bl #0x369b38
0036bdb0: mov r1, #0x3f000000
0036bdb4: bl #0x30e70c
0036bdb8: cmp r0, #0
0036bdbc: beq #0x36bddc
0036bdc0: ldr r3, [r4, r5]
0036bdc4: ldr r2, [sp, #0x4c]
0036bdc8: ldr r3, [r3]
0036bdcc: cmp r2, r3
0036bdd0: bne #0x36bfe8
0036bdd4: add sp, sp, #0x54
0036bdd8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036bddc: ldr r3, [pc, #0x210]
0036bde0: add r8, sp, #0x34
0036bde4: ldr sl, [r4, r3]
0036bde8: mov r0, sl
0036bdec: bl #0x337888
0036bdf0: ldr r1, [pc, #0x200]
0036bdf4: add r2, sp, #0x30
0036bdf8: mov r0, r8
0036bdfc: add r1, pc, r1
0036be00: bl #0x3140ec
0036be04: mov r0, sl
0036be08: mov r1, r8
0036be0c: bl #0x337a88
0036be10: mov sl, r0
0036be14: mov r0, r8
0036be18: bl #0x3139ac
0036be1c: cmp sl, #0
0036be20: bne #0x36bdc0
0036be24: ldr r3, [pc, #0x1d0]
0036be28: ldr r3, [r4, r3]
0036be2c: ldrb r3, [r3, #0xb4]
0036be30: cmp r3, #0
0036be34: beq #0x36bdc0
0036be38: cmn r6, #1
0036be3c: beq #0x36bed4
0036be40: ldr ip, [r7, #0x24]
0036be44: cmp ip, r6
0036be48: beq #0x36bef0
0036be4c: ldr r8, [pc, #0x1ac]
0036be50: cmp ip, r6
0036be54: str ip, [r7, #0x28]
0036be58: beq #0x36be68
0036be5c: mov r0, r7
0036be60: ldr r1, [sp, #0x78]
0036be64: bl #0x36a1a0
0036be68: ldr r3, [r4, r8]
0036be6c: str r6, [r7, #0x24]
0036be70: strb sb, [r7, #0x30]
0036be74: ldrb r3, [r3]
0036be78: cmp r3, #0
0036be7c: beq #0x36beac
0036be80: ldr r2, [pc, #0x17c]
0036be84: ldr r3, [pc, #0x17c]
0036be88: mov r0, r6
0036be8c: ldr ip, [r4, r2]
0036be90: ldr r3, [r4, r3]
0036be94: mov r2, sb
0036be98: str r6, [ip]
0036be9c: ldr r1, [r3]
0036bea0: mov r3, #1
0036bea4: bl #0x531348
0036bea8: b #0x36bdc0
0036beac: mov ip, #2
0036beb0: str ip, [sp]
0036beb4: mov r0, r7
0036beb8: mov ip, #1
0036bebc: mov r1, r6
0036bec0: mov r2, sb
0036bec4: ldr r3, [sp, #0x78]
0036bec8: str ip, [sp, #4]
0036becc: bl #0x36b80c
0036bed0: b #0x36bdc0
0036bed4: cmp fp, #0
0036bed8: beq #0x36bdc0
0036bedc: str r6, [r7, #0x24]
0036bee0: mov r0, r7
0036bee4: ldr r1, [sp, #0x78]
0036bee8: bl #0x36a1a0
0036beec: b #0x36bdc0
0036bef0: ldr r8, [pc, #0x108]
0036bef4: ldr r3, [r4, r8]
0036bef8: ldrb r2, [r3]
0036befc: cmp r2, #0
0036bf00: bne #0x36bfbc
0036bf04: ldr r1, [pc, #0x100]
0036bf08: ldr r3, [pc, #0x100]
0036bf0c: mov ip, #0xc
0036bf10: ldr r1, [r4, r1]
0036bf14: ldr r3, [r4, r3]
0036bf18: mvn sl, #0
0036bf1c: ldr r0, [r1]
0036bf20: mvn fp, #0
0036bf24: ldr r1, [r7, #8]
0036bf28: mla r0, ip, r6, r0
0036bf2c: add r3, r3, #8
0036bf30: ldr r0, [r0, #4]
0036bf34: strd sl, fp, [sp, #0x10]
0036bf38: str r2, [sp, #0x28]
0036bf3c: str r2, [sp, #0x18]
0036bf40: str r2, [sp, #0x1c]
0036bf44: str r2, [sp, #0x20]
0036bf48: str r2, [sp, #0x24]
0036bf4c: str r3, [sp, #8]
0036bf50: ldr r1, [r1, r0, lsl #2]
0036bf54: cmp r1, #0
0036bf58: beq #0x36bfa4
0036bf5c: add sl, sp, #8
0036bf60: mov r3, #1
0036bf64: ldr r0, [r7]
0036bf68: mov r2, sl
0036bf6c: bl #0x862548
0036bf70: cmp r0, #0
0036bf74: ldrle r3, [sp, #8]
0036bf78: ble #0x36bfa8
0036bf7c: movw r2, #0xcccd
0036bf80: ldr r0, [r7]
0036bf84: mov r1, sl
0036bf88: movt r2, #0x3d4c
0036bf8c: bl #0x862148
0036bf90: mov r0, sl
0036bf94: ldr r3, [sp, #8]
0036bf98: mov lr, pc
0036bf9c: ldr pc, [r3]
0036bfa0: b #0x36bdc0
0036bfa4: add sl, sp, #8
0036bfa8: mov r0, sl
0036bfac: mov lr, pc
0036bfb0: ldr pc, [r3]
0036bfb4: ldr ip, [r7, #0x24]
0036bfb8: b #0x36be50
0036bfbc: ldr r2, [pc, #0x40]
0036bfc0: ldr r3, [pc, #0x40]
0036bfc4: mov r0, ip
0036bfc8: ldr lr, [r4, r2]
0036bfcc: ldr r3, [r4, r3]
0036bfd0: mov r2, sb
0036bfd4: str ip, [lr]
0036bfd8: ldr r1, [r3]
0036bfdc: mov r3, #1
0036bfe0: bl #0x531348
0036bfe4: b #0x36bdc0
0036bfe8: bl #0x30e310
0036bfec: rsbeq r8, r2, r8, lsl #26
0036bff0: andeq r4, r0, ip, lsr #1
0036bff4: andeq r0, r0, r4, lsl #17
0036bff8: subseq r5, r5, ip, ror #5
0036bffc: strdeq r3, r4, [r0], -r4
0036c000: andeq r3, r0, r0, lsr fp
0036c004: muleq r0, r8, r8
0036c008: andeq r0, r0, r8, lsl #24
0036c00c: andeq r3, r0, ip, lsr lr
0036c010: andeq r2, r0, r8, lsr #28

# 0x369804 _ZN15VoxSoundManager14IsMusicPlayingEv
00369804: push {r4, r5, r6, r7, lr}
00369808: ldr r1, [r0, #0x24]
0036980c: ldr r2, [pc, #0xd0]
00369810: sub sp, sp, #0x2c
00369814: cmn r1, #1
00369818: mov r4, r0
0036981c: add r2, pc, r2
00369820: beq #0x3698d4
00369824: ldr r0, [pc, #0xbc]
00369828: ldr r3, [pc, #0xbc]
0036982c: mov ip, #0xc
00369830: ldr r0, [r2, r0]
00369834: ldr r3, [r2, r3]
00369838: mvn r6, #0
0036983c: ldr r2, [r0]
00369840: mvn r7, #0
00369844: ldr r0, [r4, #8]
00369848: mla r1, ip, r1, r2
0036984c: add r3, r3, #8
00369850: ldr r1, [r1, #4]
00369854: strd r6, r7, [sp, #8]
00369858: mov r2, #0
0036985c: str r2, [sp, #0x20]
00369860: str r2, [sp, #0x10]
00369864: str r2, [sp, #0x14]
00369868: str r2, [sp, #0x18]
0036986c: str r2, [sp, #0x1c]
00369870: str r3, [sp]
00369874: ldr r1, [r0, r1, lsl #2]
00369878: cmp r1, r2
0036987c: beq #0x3698c4
00369880: mov r3, #1
00369884: ldr r0, [r4]
00369888: mov r2, sp
0036988c: bl #0x862548
00369890: cmp r0, #0
00369894: mov r5, sp
00369898: ldrle r3, [sp]
0036989c: ble #0x3698c8
003698a0: ldr r0, [r4]
003698a4: mov r1, sp
003698a8: bl #0x861f38
003698ac: ldr r3, [sp]
003698b0: mov r4, r0
003698b4: mov r0, sp
003698b8: mov lr, pc
003698bc: ldr pc, [r3]
003698c0: b #0x3698d8
003698c4: mov r5, sp
003698c8: mov r0, sp
003698cc: mov lr, pc
003698d0: ldr pc, [r3]
003698d4: mov r4, #0
003698d8: mov r0, r4
003698dc: add sp, sp, #0x2c
003698e0: pop {r4, r5, r6, r7, pc}
003698e4: rsbeq fp, r2, r4, ror r2
003698e8: andeq r3, r0, ip, lsr lr
003698ec: andeq r2, r0, r8, lsr #28

# 0x36a218 _ZN15VoxSoundManager6Stop3DEiiP7Point3DIfEf
0036a218: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036a21c: ldr r7, [pc, #0x23c]
0036a220: cmp r1, #0
0036a224: sub sp, sp, #0x1c4
0036a228: add r7, pc, r7
0036a22c: mov r8, r0
0036a230: mov r6, r2
0036a234: mov r5, r3
0036a238: blt #0x36a434
0036a23c: ldr r3, [pc, #0x220]
0036a240: ldr r3, [r7, r3]
0036a244: ldrb r4, [r3]
0036a248: cmp r4, #0
0036a24c: bne #0x36a454
0036a250: ldr r2, [pc, #0x210]
0036a254: ldr r3, [r0, #8]
0036a258: mov r0, #0xc
0036a25c: ldr r2, [r7, r2]
0036a260: ldr r2, [r2]
0036a264: mla r1, r0, r1, r2
0036a268: ldr sl, [r1, #4]
0036a26c: ldr r1, [r3, sl, lsl #2]
0036a270: cmp r1, #0
0036a274: beq #0x36a434
0036a278: ldr r0, [r8]
0036a27c: bl #0x8624f8
0036a280: cmp r0, #0
0036a284: beq #0x36a434
0036a288: mov r0, r6
0036a28c: bl #0x30e964
0036a290: mov r1, #0x44000000
0036a294: add r1, r1, #0x7a0000
0036a298: bl #0x30ec94
0036a29c: ldr r3, [pc, #0x1c8]
0036a2a0: add r6, sp, #0x20
0036a2a4: str r0, [sp, #0x1c]
0036a2a8: ldr r2, [r7, r3]
0036a2ac: add ip, r6, #0x1b8
0036a2b0: add r3, r6, #0x28
0036a2b4: add r2, r2, #8
0036a2b8: mvn r0, #0
0036a2bc: mvn r1, #0
0036a2c0: strd r0, r1, [r3, #-0x20]
0036a2c4: str r4, [r3, #-0x18]
0036a2c8: str r4, [r3, #-0x14]
0036a2cc: str r4, [r3, #-0x10]
0036a2d0: str r4, [r3, #-0xc]
0036a2d4: str r4, [r3, #-8]
0036a2d8: str r2, [r3, #-0x28]
0036a2dc: add r3, r3, #0x28
0036a2e0: cmp r3, ip
0036a2e4: bne #0x36a2c0
0036a2e8: ldr r3, [r8, #8]
0036a2ec: ldr r0, [r8]
0036a2f0: mov r2, r6
0036a2f4: ldr r1, [r3, sl, lsl #2]
0036a2f8: mov r3, #0xa
0036a2fc: bl #0x862548
0036a300: cmp r0, #0
0036a304: str r0, [sp, #0xc]
0036a308: ble #0x36a418
0036a30c: add r3, sp, #0x1bc
0036a310: str r3, [sp, #0x10]
0036a314: add ip, sp, #0x1b8
0036a318: add r3, sp, #0x1b4
0036a31c: str ip, [sp, #0x14]
0036a320: str r3, [sp, #0x18]
0036a324: b #0x36a338
0036a328: ldr ip, [sp, #0xc]
0036a32c: add r4, r4, #1
0036a330: cmp r4, ip
0036a334: beq #0x36a418
0036a338: mov ip, #0x28
0036a33c: mla r7, ip, r4, r6
0036a340: cmp r5, #0
0036a344: ldr r2, [sp, #0x10]
0036a348: ldr r3, [sp, #0x14]
0036a34c: mov r1, r7
0036a350: beq #0x36a43c
0036a354: ldr ip, [sp, #0x18]
0036a358: ldr r0, [r8]
0036a35c: str ip, [sp]
0036a360: bl #0x861d28
0036a364: ldr r1, [r5]
0036a368: ldr r0, [sp, #0x1bc]
0036a36c: bl #0x30e3ac
0036a370: ldr r1, [r5, #4]
0036a374: mov sb, r0
0036a378: ldr r0, [sp, #0x1b8]
0036a37c: bl #0x30e3ac
0036a380: ldr r1, [r5, #8]
0036a384: mov fp, r0
0036a388: ldr r0, [sp, #0x1b4]
0036a38c: bl #0x30e3ac
0036a390: mov r1, sb
0036a394: mov sl, r0
0036a398: mov r0, sb
0036a39c: bl #0x30ed6c
0036a3a0: mov r1, fp
0036a3a4: mov sb, r0
0036a3a8: mov r0, fp
0036a3ac: bl #0x30ed6c
0036a3b0: mov r1, r0
0036a3b4: mov r0, sb
0036a3b8: bl #0x30eba4
0036a3bc: mov r1, sl
0036a3c0: mov sb, r0
0036a3c4: mov r0, sl
0036a3c8: bl #0x30ed6c
0036a3cc: mov r1, r0
0036a3d0: mov r0, sb
0036a3d4: bl #0x30eba4
0036a3d8: bl #0x30e8a4
0036a3dc: bl #0x30e1c0
0036a3e0: bl #0x30e6a0
0036a3e4: mov r1, r0
0036a3e8: ldr r0, [sp, #0x1e8]
0036a3ec: bl #0x30e70c
0036a3f0: cmp r0, #0
0036a3f4: beq #0x36a328
0036a3f8: mov r1, r7
0036a3fc: ldr r0, [r8]
0036a400: ldr r2, [sp, #0x1c]
0036a404: bl #0x862198
0036a408: ldr ip, [sp, #0xc]
0036a40c: add r4, r4, #1
0036a410: cmp r4, ip
0036a414: bne #0x36a338
0036a418: add r4, r6, #0x190
0036a41c: ldr r3, [r4, #-0x28]!
0036a420: mov r0, r4
0036a424: mov lr, pc
0036a428: ldr pc, [r3]
0036a42c: cmp r4, r6
0036a430: bne #0x36a41c
0036a434: add sp, sp, #0x1c4
0036a438: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036a43c: mov r3, #0x28
0036a440: mla r1, r3, r4, r6
0036a444: ldr r0, [r8]
0036a448: ldr r2, [sp, #0x1c]
0036a44c: bl #0x862198
0036a450: b #0x36a328
0036a454: mov r0, r1
0036a458: bl #0x531570
0036a45c: b #0x36a434
0036a460: rsbeq sl, r2, r8, ror #16
0036a464: andeq r3, r0, r0, lsr fp
0036a468: andeq r3, r0, ip, lsr lr
0036a46c: andeq r2, r0, r8, lsr #28

# 0x369298 _ZN15VoxSoundManager15UnloadAllSoundsEv
00369298: ldr r3, [pc, #0x48]
0036929c: ldr r2, [pc, #0x48]
003692a0: push {r4, r5, r6, lr}
003692a4: add r3, pc, r3
003692a8: ldr r2, [r3, r2]
003692ac: mov r5, r0
003692b0: ldrb r4, [r2]
003692b4: cmp r4, #0
003692b8: bne #0x3692e4
003692bc: ldr r3, [r0, #0x1c]
003692c0: cmp r3, #0
003692c4: ble #0x3692e4
003692c8: mov r1, r4
003692cc: mov r0, r5
003692d0: bl #0x36921c
003692d4: ldr r3, [r5, #0x1c]
003692d8: add r4, r4, #1
003692dc: cmp r3, r4
003692e0: bgt #0x3692c8
003692e4: pop {r4, r5, r6, pc}
003692e8: rsbeq fp, r2, ip, ror #15
003692ec: andeq r3, r0, r0, lsr fp

# 0x36baa0 _ZN15VoxSoundManager14CrossfadeMusicEii
0036baa0: push {r4, r5, r6, r7, r8, sb, sl, lr}
0036baa4: ldr r5, [pc, #0x178]
0036baa8: ldr r3, [pc, #0x178]
0036baac: sub sp, sp, #0x30
0036bab0: add r5, pc, r5
0036bab4: ldr r3, [r5, r3]
0036bab8: mov r4, r0
0036babc: ldrb r3, [r3]
0036bac0: cmp r3, #0
0036bac4: bne #0x36bbf8
0036bac8: cmn r2, #1
0036bacc: beq #0x36bbf8
0036bad0: ldrb r3, [r0, #0x31]
0036bad4: cmp r3, #0
0036bad8: ldr r3, [pc, #0x14c]
0036badc: moveq sl, r1
0036bae0: movne r8, r1
0036bae4: ldr r3, [r5, r3]
0036bae8: movne sl, r2
0036baec: mov r1, #0xc
0036baf0: ldr r3, [r3]
0036baf4: moveq r8, r2
0036baf8: mla r2, r1, sl, r3
0036bafc: mla r3, r1, r8, r3
0036bb00: ldr r7, [r2, #4]
0036bb04: ldr sb, [r3, #4]
0036bb08: str r7, [r3, #4]
0036bb0c: ldr r2, [r2, #8]
0036bb10: str r2, [r3, #8]
0036bb14: ldr r3, [r0, #8]
0036bb18: ldr r2, [r3, sb, lsl #2]
0036bb1c: cmp r2, #0
0036bb20: beq #0x36bc00
0036bb24: ldr r2, [r3, r7, lsl #2]
0036bb28: cmp r2, #0
0036bb2c: beq #0x36bc10
0036bb30: ldr r2, [pc, #0xf8]
0036bb34: mvn r0, #0
0036bb38: mvn r1, #0
0036bb3c: ldr r2, [r5, r2]
0036bb40: strd r0, r1, [sp, #0x10]
0036bb44: mov r6, #0
0036bb48: add r5, sp, #0x30
0036bb4c: add r2, r2, #8
0036bb50: str r2, [r5, #-0x28]!
0036bb54: str r6, [sp, #0x18]
0036bb58: str r6, [sp, #0x1c]
0036bb5c: str r6, [sp, #0x20]
0036bb60: str r6, [sp, #0x24]
0036bb64: str r6, [sp, #0x28]
0036bb68: ldr r1, [r3, sb, lsl #2]
0036bb6c: mov r2, r5
0036bb70: mov r3, #1
0036bb74: ldr r0, [r4]
0036bb78: bl #0x862548
0036bb7c: mov r1, r5
0036bb80: ldr r0, [r4]
0036bb84: bl #0x862000
0036bb88: mov r1, r8
0036bb8c: mov sb, r0
0036bb90: mov r2, #0x7d0
0036bb94: mov r0, r4
0036bb98: bl #0x369fec
0036bb9c: mov ip, #2
0036bba0: mov r1, sl
0036bba4: mov r2, #1
0036bba8: mov r3, #0x7d0
0036bbac: mov r0, r4
0036bbb0: str ip, [sp]
0036bbb4: str r6, [sp, #4]
0036bbb8: bl #0x36b80c
0036bbbc: ldr r1, [r4, #8]
0036bbc0: mov r3, #1
0036bbc4: mov r2, r5
0036bbc8: ldr r1, [r1, r7, lsl #2]
0036bbcc: ldr r0, [r4]
0036bbd0: bl #0x862548
0036bbd4: ldr r0, [r4]
0036bbd8: mov r2, sb
0036bbdc: mov r1, r5
0036bbe0: bl #0x861fd8
0036bbe4: ldrb r3, [r4, #0x31]
0036bbe8: mov r0, r5
0036bbec: eor r3, r3, #1
0036bbf0: strb r3, [r4, #0x31]
0036bbf4: bl #0x8683ac
0036bbf8: add sp, sp, #0x30
0036bbfc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0036bc00: mov r1, sb
0036bc04: bl #0x3699fc
0036bc08: ldr r3, [r4, #8]
0036bc0c: b #0x36bb24
0036bc10: mov r0, r4
0036bc14: mov r1, r7
0036bc18: bl #0x3699fc
0036bc1c: ldr r3, [r4, #8]
0036bc20: b #0x36bb30
0036bc24: rsbeq r8, r2, r0, ror #31
0036bc28: andeq r3, r0, r0, lsr fp
0036bc2c: andeq r3, r0, ip, lsr lr
0036bc30: andeq r2, r0, r8, lsr #28

# 0x36b048 _ZN15VoxSoundManager14DeleteInstanceEv
0036b048: ldr r3, [pc, #0x30]
0036b04c: ldr r2, [pc, #0x30]
0036b050: push {r4, lr}
0036b054: add r3, pc, r3
0036b058: ldr r2, [r3, r2]
0036b05c: ldr r4, [r2]
0036b060: cmp r4, #0
0036b064: beq #0x36b07c
0036b068: mov r0, r4
0036b06c: bl #0x36afb0
0036b070: mov r0, r4
0036b074: pop {r4, lr}
0036b078: b #0x310440
0036b07c: pop {r4, pc}
0036b080: rsbeq sb, r2, ip, lsr sl
0036b084: andeq r0, r0, r4, lsr #27

# 0x36b420 _ZN15VoxSoundManager9PlayEventEiRKN6glitch4core8vector3dIfEEff
0036b420: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b424: ldr r4, [pc, #0x190]
0036b428: ldr r6, [pc, #0x190]
0036b42c: ldr lr, [pc, #0x190]
0036b430: add r4, pc, r4
0036b434: ldr ip, [r4, r6]
0036b438: ldr r5, [r4, lr]
0036b43c: sub sp, sp, #0x7c
0036b440: ldr ip, [ip]
0036b444: mov r7, r0
0036b448: mov r0, r5
0036b44c: mov fp, r3
0036b450: str ip, [sp, #0x74]
0036b454: mov sl, r1
0036b458: mov sb, r2
0036b45c: bl #0x337888
0036b460: ldr r1, [pc, #0x160]
0036b464: add r8, sp, #0x5c
0036b468: add r2, sp, #0x40
0036b46c: add r1, pc, r1
0036b470: mov r0, r8
0036b474: bl #0x3140ec
0036b478: mov r1, r8
0036b47c: mov r0, r5
0036b480: bl #0x337a88
0036b484: mov r2, r0
0036b488: mov r0, r8
0036b48c: str r2, [sp, #0x18]
0036b490: bl #0x3139ac
0036b494: ldr r2, [sp, #0x18]
0036b498: cmp r2, #0
0036b49c: bne #0x36b57c
0036b4a0: cmp sl, #0
0036b4a4: blt #0x36b57c
0036b4a8: ldr r3, [pc, #0x11c]
0036b4ac: ldr r3, [r4, r3]
0036b4b0: ldrb r3, [r3]
0036b4b4: cmp r3, #0
0036b4b8: bne #0x36b59c
0036b4bc: add r3, r7, #0x64
0036b4c0: mov r0, r3
0036b4c4: mov r1, sl
0036b4c8: add r2, sp, #0x38
0036b4cc: str r3, [sp, #0x1c]
0036b4d0: bl #0x88bb6c
0036b4d4: ldr r3, [sp, #0x38]
0036b4d8: cmp r3, #0
0036b4dc: blt #0x36b57c
0036b4e0: mov r0, r5
0036b4e4: bl #0x337888
0036b4e8: ldr r1, [pc, #0xe0]
0036b4ec: add r8, sp, #0x44
0036b4f0: add r2, sp, #0x3c
0036b4f4: add r1, pc, r1
0036b4f8: mov r0, r8
0036b4fc: bl #0x3140ec
0036b500: mov r1, r8
0036b504: mov r0, r5
0036b508: bl #0x337a88
0036b50c: mov r0, r8
0036b510: bl #0x3139ac
0036b514: add ip, sp, #0x34
0036b518: str ip, [sp]
0036b51c: add ip, sp, #0x30
0036b520: add r2, sp, #0x24
0036b524: ldr r1, [sp, #0x38]
0036b528: add r3, sp, #0x2c
0036b52c: str ip, [sp, #4]
0036b530: ldr r0, [sp, #0x1c]
0036b534: add ip, sp, #0x28
0036b538: str ip, [sp, #8]
0036b53c: bl #0x8896f4
0036b540: ldr ip, [sp, #0x34]
0036b544: mov r0, r7
0036b548: ldr r1, [sp, #0x38]
0036b54c: str ip, [sp]
0036b550: ldr ip, [sp, #0x30]
0036b554: ldr r2, [sp, #0x24]
0036b558: ldr r3, [sp, #0x2c]
0036b55c: str ip, [sp, #4]
0036b560: ldr ip, [sp, #0x28]
0036b564: str sb, [sp, #0xc]
0036b568: str fp, [sp, #0x10]
0036b56c: str ip, [sp, #8]
0036b570: ldr ip, [sp, #0xa0]
0036b574: str ip, [sp, #0x14]
0036b578: bl #0x36a7c0
0036b57c: ldr r3, [r4, r6]
0036b580: ldr r2, [sp, #0x74]
0036b584: mov r0, #0
0036b588: ldr r3, [r3]
0036b58c: cmp r2, r3
0036b590: bne #0x36b5b8
0036b594: add sp, sp, #0x7c
0036b598: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b59c: ldr r3, [pc, #0x30]
0036b5a0: mov r0, sl
0036b5a4: ldr r1, [r4, r3]
0036b5a8: mov r3, #2
0036b5ac: ldr r1, [r1]
0036b5b0: bl #0x531348
0036b5b4: b #0x36b57c
0036b5b8: bl #0x30e310
0036b5bc: rsbeq sb, r2, r0, ror #12
0036b5c0: andeq r4, r0, ip, lsr #1
0036b5c4: andeq r0, r0, r4, lsl #17
0036b5c8: subseq r5, r5, ip, ror ip
0036b5cc: andeq r3, r0, r0, lsr fp
0036b5d0: subseq r5, r5, ip, lsl #24
0036b5d4: andeq r0, r0, r0, lsl #13

# 0x369c2c _ZN15VoxSoundManager16SetInitialVolumeEfff
00369c2c: ldr ip, [pc, #0x150]
00369c30: push {r4, r5, r6, r7, r8, sl, lr}
00369c34: mov r4, r0
00369c38: ldr r0, [pc, #0x148]
00369c3c: add ip, pc, ip
00369c40: mov sl, r3
00369c44: ldr r0, [ip, r0]
00369c48: sub sp, sp, #0xc
00369c4c: mov r6, r2
00369c50: ldrb r3, [r0]
00369c54: mov r8, r1
00369c58: cmp r3, #0
00369c5c: bne #0x369d4c
00369c60: ldrb r3, [r4, #0x20]
00369c64: cmp r3, #0
00369c68: beq #0x369c74
00369c6c: add sp, sp, #0xc
00369c70: pop {r4, r5, r6, r7, r8, sl, pc}
00369c74: ldr r1, [pc, #0x110]
00369c78: add r5, sp, #8
00369c7c: str r3, [r5, #-4]!
00369c80: add r7, r4, #0x64
00369c84: mov r2, r5
00369c88: add r1, pc, r1
00369c8c: mov r0, r7
00369c90: bl #0x88cc14
00369c94: mov r1, #0x42000000
00369c98: mov r0, r8
00369c9c: add r1, r1, #0xc80000
00369ca0: bl #0x30ec94
00369ca4: ldr r8, [r4]
00369ca8: movw r3, #0xcccd
00369cac: movt r3, #0x3d4c
00369cb0: mov r2, r0
00369cb4: ldr r1, [sp, #4]
00369cb8: mov r0, r8
00369cbc: bl #0x8617fc
00369cc0: ldr r1, [pc, #0xc8]
00369cc4: mov r2, r5
00369cc8: mov r0, r7
00369ccc: add r1, pc, r1
00369cd0: bl #0x88cc14
00369cd4: mov r1, #0x42000000
00369cd8: mov r0, r6
00369cdc: add r1, r1, #0xc80000
00369ce0: bl #0x30ec94
00369ce4: ldr r6, [r4]
00369ce8: movw r3, #0xcccd
00369cec: movt r3, #0x3d4c
00369cf0: mov r2, r0
00369cf4: ldr r1, [sp, #4]
00369cf8: mov r0, r6
00369cfc: bl #0x8617fc
00369d00: ldr r1, [pc, #0x8c]
00369d04: mov r2, r5
00369d08: mov r0, r7
00369d0c: add r1, pc, r1
00369d10: bl #0x88cc14
00369d14: mov r1, #0x42000000
00369d18: add r1, r1, #0xc80000
00369d1c: mov r0, sl
00369d20: bl #0x30ec94
00369d24: ldr r5, [r4]
00369d28: movw r3, #0xcccd
00369d2c: mov r2, r0
00369d30: movt r3, #0x3d4c
00369d34: mov r0, r5
00369d38: ldr r1, [sp, #4]
00369d3c: bl #0x8617fc
00369d40: mov r3, #1
00369d44: strb r3, [r4, #0x20]
00369d48: b #0x369c6c
00369d4c: ldr r3, [pc, #0x44]
00369d50: mov r2, #1
00369d54: ldr r5, [ip, r3]
00369d58: ldr r3, [pc, #0x3c]
00369d5c: str r6, [r5]
00369d60: ldr r3, [ip, r3]
00369d64: str r1, [r3]
00369d68: ldr r0, [r4, #0x24]
00369d6c: bl #0x53177c
00369d70: ldr r0, [r4, #0x24]
00369d74: ldr r1, [r5]
00369d78: mov r2, #2
00369d7c: bl #0x53177c
00369d80: b #0x369c6c
00369d84: rsbeq sl, r2, r4, asr lr
00369d88: andeq r3, r0, r0, lsr fp
00369d8c: subseq r7, r5, r0, ror #5
00369d90: ldrheq r7, [r5], #-0x24
00369d94: subseq r5, r5, ip, ror #21
00369d98: andeq r0, r0, r0, lsl #13
00369d9c: andeq r0, r0, r8, lsl #24

# 0x36bc34 _ZN15VoxSoundManager8PlayMenuEibii
0036bc34: str lr, [sp, #-4]!
0036bc38: mov ip, #1
0036bc3c: sub sp, sp, #0xc
0036bc40: str ip, [sp, #4]
0036bc44: ldr ip, [sp, #0x10]
0036bc48: str ip, [sp]
0036bc4c: bl #0x36b80c
0036bc50: add sp, sp, #0xc
0036bc54: ldm sp!, {pc}

# 0x36a1a0 _ZN15VoxSoundManager9StopMusicEi
0036a1a0: push {r4, lr}
0036a1a4: ldr ip, [r0, #0x24]
0036a1a8: ldr r3, [pc, #0x5c]
0036a1ac: mov r4, r0
0036a1b0: cmn ip, #1
0036a1b4: mov r2, r1
0036a1b8: add r3, pc, r3
0036a1bc: beq #0x36a1ec
0036a1c0: ldr r1, [pc, #0x48]
0036a1c4: ldr r1, [r3, r1]
0036a1c8: ldrb r1, [r1]
0036a1cc: cmp r1, #0
0036a1d0: bne #0x36a1f0
0036a1d4: mov r1, ip
0036a1d8: bl #0x369fec
0036a1dc: ldr r3, [r4, #0x24]
0036a1e0: mvn r2, #0
0036a1e4: str r2, [r4, #0x24]
0036a1e8: str r3, [r4, #0x28]
0036a1ec: pop {r4, pc}
0036a1f0: ldr r2, [pc, #0x1c]
0036a1f4: ldr r3, [r3, r2]
0036a1f8: mvn r2, #0
0036a1fc: str r2, [r3]
0036a200: ldr r0, [r0, #0x24]
0036a204: bl #0x531570
0036a208: b #0x36a1dc

# 0x36934c _ZN15VoxSoundManager15UpdateIPodAsyncEv
0036934c: bx lr

# 0x36cda8 _ZN15VoxSoundManager11IPodControlEPKc
0036cda8: push {r4, r5, r6, r7, r8, sb, sl, lr}
0036cdac: ldr r4, [pc, #0x3f4]
0036cdb0: ldr r5, [pc, #0x3f4]
0036cdb4: mov r8, r1
0036cdb8: add r4, pc, r4
0036cdbc: ldr r3, [r4, r5]
0036cdc0: ldr r1, [pc, #0x3e8]
0036cdc4: sub sp, sp, #0x68
0036cdc8: ldr r3, [r3]
0036cdcc: mov r6, r0
0036cdd0: add r1, pc, r1
0036cdd4: mov r0, r8
0036cdd8: str r3, [sp, #0x64]
0036cddc: bl #0x30e31c
0036cde0: cmp r0, #0
0036cde4: bne #0x36ce34
0036cde8: ldrb r3, [r6, #0x33]
0036cdec: cmp r3, #0
0036cdf0: beq #0x36cfa4
0036cdf4: ldr r3, [r6, #0x50]
0036cdf8: cmp r3, #0
0036cdfc: beq #0x36cfcc
0036ce00: cmp r3, #1
0036ce04: beq #0x36d140
0036ce08: bl #0x533978
0036ce0c: cmp r0, #1
0036ce10: mov r7, r0
0036ce14: beq #0x36d094
0036ce18: ldr r3, [r4, r5]
0036ce1c: ldr r2, [sp, #0x64]
0036ce20: ldr r3, [r3]
0036ce24: cmp r2, r3
0036ce28: bne #0x36d1a4
0036ce2c: add sp, sp, #0x68
0036ce30: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0036ce34: ldr r1, [pc, #0x378]
0036ce38: mov r0, r8
0036ce3c: add r1, pc, r1
0036ce40: bl #0x30e31c
0036ce44: subs r7, r0, #0
0036ce48: beq #0x36cfd4
0036ce4c: ldr r1, [pc, #0x364]
0036ce50: mov r0, r8
0036ce54: add r1, pc, r1
0036ce58: bl #0x30e31c
0036ce5c: cmp r0, #0
0036ce60: beq #0x36d08c
0036ce64: ldr r1, [pc, #0x350]
0036ce68: mov r0, r8
0036ce6c: add r1, pc, r1
0036ce70: bl #0x30e31c
0036ce74: cmp r0, #0
0036ce78: beq #0x36d134
0036ce7c: ldr r1, [pc, #0x33c]
0036ce80: mov r0, r8
0036ce84: add r1, pc, r1
0036ce88: bl #0x30e31c
0036ce8c: cmp r0, #0
0036ce90: beq #0x36d080
0036ce94: ldr r1, [pc, #0x328]
0036ce98: mov r0, r8
0036ce9c: add r1, pc, r1
0036cea0: bl #0x30e31c
0036cea4: subs r8, r0, #0
0036cea8: bne #0x36ce18
0036ceac: bl #0x42ca8c
0036ceb0: bl #0x42cb8c
0036ceb4: ldr r3, [pc, #0x30c]
0036ceb8: ldr r1, [pc, #0x30c]
0036cebc: mov r7, r0
0036cec0: ldr r0, [r4, r3]
0036cec4: add r1, pc, r1
0036cec8: bl #0x320e44
0036cecc: ldr r1, [pc, #0x2fc]
0036ced0: add sl, sp, #0x50
0036ced4: mov r2, r0
0036ced8: add r1, pc, r1
0036cedc: mov r0, sl
0036cee0: bl #0x30eae4
0036cee4: mov r1, sl
0036cee8: mov r0, r7
0036ceec: bl #0x7a9160
0036cef0: mov sb, r0
0036cef4: bl #0x533978
0036cef8: cmp r0, #1
0036cefc: mov ip, r0
0036cf00: beq #0x36d150
0036cf04: mov r3, #0x3fc00000
0036cf08: mov r2, #0
0036cf0c: add r3, r3, #0x300000
0036cf10: mov ip, #2
0036cf14: strd r2, r3, [sp, #0x48]
0036cf18: strb ip, [sp, #0x25]
0036cf1c: mov ip, #0
0036cf20: str ip, [sp, #0x28]
0036cf24: ldr r2, [pc, #0x2a8]
0036cf28: ldr ip, [sp, #0x4c]
0036cf2c: add sl, sp, #0x24
0036cf30: add r2, pc, r2
0036cf34: str ip, [sl, #8]
0036cf38: mov r0, r7
0036cf3c: mov ip, #1
0036cf40: mov r1, sb
0036cf44: mov r3, sl
0036cf48: strb r8, [sp, #0x24]
0036cf4c: str ip, [sp]
0036cf50: bl #0x7abe0c
0036cf54: mov r0, sl
0036cf58: bl #0x797124
0036cf5c: ldr r1, [r6, #0x4c]
0036cf60: add r6, sp, #0xc
0036cf64: mov r3, #0
0036cf68: mov r0, r6
0036cf6c: strb r3, [sp, #0xd]
0036cf70: strb r3, [sp, #0xc]
0036cf74: bl #0x797350
0036cf78: ldr r2, [pc, #0x258]
0036cf7c: mov ip, #1
0036cf80: mov r0, r7
0036cf84: mov r1, sb
0036cf88: add r2, pc, r2
0036cf8c: mov r3, r6
0036cf90: str ip, [sp]
0036cf94: bl #0x7abe0c
0036cf98: mov r0, r6
0036cf9c: bl #0x797124
0036cfa0: b #0x36ce18
0036cfa4: bl #0x533a10
0036cfa8: cmp r0, #0
0036cfac: mvnle r0, #0
0036cfb0: movgt r0, #0
0036cfb4: bl #0x533850
0036cfb8: mov r3, #1
0036cfbc: strb r3, [r6, #0x33]
0036cfc0: ldr r3, [r6, #0x50]
0036cfc4: cmp r3, #0
0036cfc8: bne #0x36ce00
0036cfcc: bl #0x5339c8
0036cfd0: b #0x36ce08
0036cfd4: mov r8, #1
0036cfd8: bl #0x533980
0036cfdc: str r8, [r6, #0x50]
0036cfe0: str r7, [r6, #0x34]
0036cfe4: bl #0x42ca8c
0036cfe8: bl #0x42cb8c
0036cfec: ldr r3, [pc, #0x1d4]
0036cff0: ldr r1, [pc, #0x1e4]
0036cff4: mov sl, r0
0036cff8: ldr r0, [r4, r3]
0036cffc: add r1, pc, r1
0036d000: bl #0x320e44
0036d004: ldr r1, [pc, #0x1d4]
0036d008: add r6, sp, #0x50
0036d00c: mov r2, r0
0036d010: add r1, pc, r1
0036d014: mov r0, r6
0036d018: bl #0x30eae4
0036d01c: mov r1, r6
0036d020: mov r0, sl
0036d024: bl #0x7a9160
0036d028: mov r3, #0x3fc00000
0036d02c: mov r2, #0
0036d030: add r3, r3, #0x300000
0036d034: mov ip, #2
0036d038: strd r2, r3, [sp, #0x48]
0036d03c: strb ip, [sp, #0x31]
0036d040: mov ip, #0
0036d044: str ip, [sp, #0x34]
0036d048: ldr r2, [pc, #0x194]
0036d04c: ldr ip, [sp, #0x4c]
0036d050: add r6, sp, #0x30
0036d054: mov r1, r0
0036d058: str ip, [r6, #8]
0036d05c: mov r0, sl
0036d060: add r2, pc, r2
0036d064: mov r3, r6
0036d068: strb r7, [sp, #0x30]
0036d06c: str r8, [sp]
0036d070: bl #0x7abe0c
0036d074: mov r0, r6
0036d078: bl #0x797124
0036d07c: b #0x36ce18
0036d080: mvn r0, #0
0036d084: bl #0x53389c
0036d088: b #0x36ce18
0036d08c: bl #0x533930
0036d090: b #0x36ce18
0036d094: bl #0x42ca8c
0036d098: bl #0x42cb8c
0036d09c: ldr r3, [pc, #0x124]
0036d0a0: ldr r1, [pc, #0x140]
0036d0a4: mov sl, r0
0036d0a8: ldr r0, [r4, r3]
0036d0ac: add r1, pc, r1
0036d0b0: bl #0x320e44
0036d0b4: ldr r1, [pc, #0x130]
0036d0b8: add r8, sp, #0x50
0036d0bc: mov r2, r0
0036d0c0: add r1, pc, r1
0036d0c4: mov r0, r8
0036d0c8: bl #0x30eae4
0036d0cc: mov r1, r8
0036d0d0: mov r0, sl
0036d0d4: bl #0x7a9160
0036d0d8: mov ip, #0
0036d0dc: strb ip, [sp, #0x3c]
0036d0e0: mov r2, #0
0036d0e4: mov r3, #0
0036d0e8: mov ip, #2
0036d0ec: strd r2, r3, [sp, #0x48]
0036d0f0: strb ip, [sp, #0x3d]
0036d0f4: mov ip, #0
0036d0f8: str ip, [sp, #0x40]
0036d0fc: ldr r2, [pc, #0xec]
0036d100: ldr ip, [sp, #0x4c]
0036d104: add r8, sp, #0x3c
0036d108: mov r1, r0
0036d10c: str ip, [r8, #8]
0036d110: mov r0, sl
0036d114: add r2, pc, r2
0036d118: mov r3, r8
0036d11c: str r7, [sp]
0036d120: bl #0x7abe0c
0036d124: str r7, [r6, #0x34]
0036d128: mov r0, r8
0036d12c: bl #0x797124
0036d130: b #0x36ce18
0036d134: mov r0, #1
0036d138: bl #0x53389c
0036d13c: b #0x36ce18
0036d140: mov r3, #0
0036d144: str r3, [r6, #0x50]
0036d148: bl #0x5338e8
0036d14c: b #0x36ce08
0036d150: mov r2, #0
0036d154: mov r3, #0
0036d158: mov lr, #2
0036d15c: strd r2, r3, [sp, #0x48]
0036d160: strb lr, [sp, #0x19]
0036d164: mov lr, #0
0036d168: str lr, [sp, #0x1c]
0036d16c: ldr r2, [pc, #0x80]
0036d170: ldr lr, [sp, #0x4c]
0036d174: add sl, sp, #0x18
0036d178: add r2, pc, r2
0036d17c: str lr, [sl, #8]
0036d180: mov r0, r7
0036d184: mov r1, sb
0036d188: mov r3, sl
0036d18c: strb r8, [sp, #0x18]
0036d190: str ip, [sp]
0036d194: bl #0x7abe0c
0036d198: mov r0, sl
0036d19c: bl #0x797124
0036d1a0: b #0x36cf5c
0036d1a4: bl #0x30e310

# 0x36c734 _ZN15VoxSoundManager19GetIPodPlaylistNameEi
0036c734: push {r4, r5, r6, lr}
0036c738: mov r4, r0
0036c73c: str r0, [r4, #0x10]
0036c740: str r0, [r4, #0x14]
0036c744: mov r1, #0x10
0036c748: mov r5, r2
0036c74c: bl #0x31167c
0036c750: ldr r3, [r4, #0x10]
0036c754: mov r2, #0
0036c758: strb r2, [r3]
0036c75c: bl #0x533a10
0036c760: cmp r0, r5
0036c764: bgt #0x36c784
0036c768: ldr r1, [pc, #0x3c]
0036c76c: mov r0, r4
0036c770: add r1, pc, r1
0036c774: mov r2, r1
0036c778: bl #0x3109e0
0036c77c: mov r0, r4
0036c780: pop {r4, r5, r6, pc}
0036c784: mov r0, r5
0036c788: bl #0x533a58
0036c78c: mov r5, r0
0036c790: bl #0x30de54
0036c794: mov r1, r5
0036c798: add r2, r5, r0
0036c79c: mov r0, r4
0036c7a0: bl #0x3109e0
0036c7a4: mov r0, r4
0036c7a8: pop {r4, r5, r6, pc}

# 0x369b38 _ZN15VoxSoundManager14GetSoundVolumeEi
00369b38: ldr r3, [pc, #0xd8]
00369b3c: ldr r2, [pc, #0xd8]
00369b40: push {r4, lr}
00369b44: add r3, pc, r3
00369b48: ldr r2, [r3, r2]
00369b4c: mov r4, r0
00369b50: sub sp, sp, #8
00369b54: ldrb r3, [r2]
00369b58: cmp r3, #0
00369b5c: movne r0, #0x3f800000
00369b60: bne #0x369bb0
00369b64: cmp r1, #2
00369b68: str r3, [sp, #4]
00369b6c: beq #0x369bb8
00369b70: cmp r1, #3
00369b74: beq #0x369be8
00369b78: cmp r1, #1
00369b7c: movne r1, r3
00369b80: bne #0x369b9c
00369b84: ldr r1, [pc, #0x94]
00369b88: add r0, r4, #0x64
00369b8c: add r2, sp, #4
00369b90: add r1, pc, r1
00369b94: bl #0x88cc14
00369b98: ldr r1, [sp, #4]
00369b9c: ldr r0, [r4]
00369ba0: bl #0x861e80
00369ba4: mov r1, #0x42000000
00369ba8: add r1, r1, #0xc80000
00369bac: bl #0x30ed6c
00369bb0: add sp, sp, #8
00369bb4: pop {r4, pc}
00369bb8: ldr r1, [pc, #0x64]
00369bbc: add r2, sp, #4
00369bc0: add r0, r4, #0x64
00369bc4: add r1, pc, r1
00369bc8: bl #0x88cc14
00369bcc: ldr r1, [sp, #4]
00369bd0: ldr r0, [r4]
00369bd4: bl #0x861e80
00369bd8: mov r1, #0x42000000
00369bdc: add r1, r1, #0xc80000
00369be0: bl #0x30ed6c
00369be4: b #0x369bb0
00369be8: ldr r1, [pc, #0x38]
00369bec: add r2, sp, #4
00369bf0: add r0, r4, #0x64
00369bf4: add r1, pc, r1
00369bf8: bl #0x88cc14
00369bfc: ldr r1, [sp, #4]
00369c00: ldr r0, [r4]
00369c04: bl #0x861e80
00369c08: mov r1, #0x42000000
00369c0c: add r1, r1, #0xc80000
00369c10: bl #0x30ed6c
00369c14: b #0x369bb0
00369c18: rsbeq sl, r2, ip, asr #30
00369c1c: andeq r3, r0, r0, lsr fp
00369c20: ldrsheq r7, [r5], #-0x30
00369c24: subseq r7, r5, r4, lsr #7
00369c28: subseq r5, r5, r4, lsl #24

# 0x369da0 _ZN15VoxSoundManager14SetSoundVolumeEif
00369da0: ldr r3, [pc, #0x124]
00369da4: push {r4, r5, r6, lr}
00369da8: mov r4, r0
00369dac: ldr r0, [pc, #0x11c]
00369db0: add r3, pc, r3
00369db4: sub sp, sp, #8
00369db8: ldr r0, [r3, r0]
00369dbc: mov r5, r2
00369dc0: ldrb r6, [r0]
00369dc4: cmp r6, #0
00369dc8: beq #0x369dec
00369dcc: cmp r1, #2
00369dd0: beq #0x369e38
00369dd4: cmp r1, #3
00369dd8: beq #0x369e74
00369ddc: cmp r1, #1
00369de0: beq #0x369e74
00369de4: add sp, sp, #8
00369de8: pop {r4, r5, r6, pc}
00369dec: cmp r1, #2
00369df0: str r6, [sp, #4]
00369df4: beq #0x369e58
00369df8: cmp r1, #3
00369dfc: beq #0x369eb0
00369e00: cmp r1, #1
00369e04: beq #0x369e94
00369e08: mov r1, #0x42000000
00369e0c: mov r0, r5
00369e10: add r1, r1, #0xc80000
00369e14: bl #0x30ec94
00369e18: ldr r4, [r4]
00369e1c: movw r3, #0xcccd
00369e20: mov r2, r0
00369e24: mov r1, r6
00369e28: mov r0, r4
00369e2c: movt r3, #0x3d4c
00369e30: bl #0x8617fc
00369e34: b #0x369de4
00369e38: ldr r0, [pc, #0x94]
00369e3c: mov r1, r2
00369e40: mov r2, #1
00369e44: ldr r3, [r3, r0]
00369e48: str r5, [r3]
00369e4c: ldr r0, [r4, #0x24]
00369e50: bl #0x53177c
00369e54: b #0x369de4
00369e58: ldr r1, [pc, #0x78]
00369e5c: add r0, r4, #0x64
00369e60: add r2, sp, #4
00369e64: add r1, pc, r1
00369e68: bl #0x88cc14
00369e6c: ldr r6, [sp, #4]
00369e70: b #0x369e08
00369e74: ldr r0, [pc, #0x60]
00369e78: mov r1, r5
00369e7c: mov r2, #2
00369e80: ldr r3, [r3, r0]
00369e84: str r5, [r3]
00369e88: ldr r0, [r4, #0x24]
00369e8c: bl #0x53177c
00369e90: b #0x369de4
00369e94: ldr r1, [pc, #0x44]
00369e98: add r0, r4, #0x64
00369e9c: add r2, sp, #4
00369ea0: add r1, pc, r1
00369ea4: bl #0x88cc14
00369ea8: ldr r6, [sp, #4]
00369eac: b #0x369e08
00369eb0: ldr r1, [pc, #0x2c]
00369eb4: add r0, r4, #0x64
00369eb8: add r2, sp, #4
00369ebc: add r1, pc, r1
00369ec0: bl #0x88cc14
00369ec4: ldr r6, [sp, #4]
00369ec8: b #0x369e08
00369ecc: rsbeq sl, r2, r0, ror #25
00369ed0: andeq r3, r0, r0, lsr fp
00369ed4: andeq r0, r0, r8, lsl #24
00369ed8: subseq r7, r5, r4, lsl #2
00369edc: andeq r0, r0, r0, lsl #13
00369ee0: subseq r7, r5, r0, ror #1
00369ee4: subseq r5, r5, ip, lsr sb

# 0x369990 _ZN15VoxSoundManager13StopAllSoundsEi
00369990: ldr r3, [pc, #0x5c]
00369994: ldr r2, [pc, #0x5c]
00369998: push {r4, lr}
0036999c: add r3, pc, r3
003699a0: ldr r2, [r3, r2]
003699a4: ldrb r3, [r2]
003699a8: cmp r3, #0
003699ac: bne #0x3699e4
003699b0: ldr r4, [r0]
003699b4: cmp r4, #0
003699b8: beq #0x3699f0
003699bc: mov r0, r1
003699c0: bl #0x30e964
003699c4: mov r1, #0x44000000
003699c8: add r1, r1, #0x7a0000
003699cc: bl #0x30ec94
003699d0: mvn r1, #0
003699d4: mov r2, r0
003699d8: mov r0, r4
003699dc: pop {r4, lr}
003699e0: b #0x8620d0
003699e4: mvn r0, #0
003699e8: pop {r4, lr}
003699ec: b #0x531940
003699f0: pop {r4, pc}

# 0x369514 _ZN15VoxSoundManager13SetMusicStateEPKc
00369514: push {r4, r5, r6, r8, sb, lr}
00369518: ldr r2, [r0, #0x24]
0036951c: ldr r3, [pc, #0xc0]
00369520: sub sp, sp, #0x28
00369524: cmp r2, #0
00369528: mov r4, r0
0036952c: mov r6, r1
00369530: add r3, pc, r3
00369534: blt #0x3695dc
00369538: ldr r1, [pc, #0xa8]
0036953c: ldr r1, [r3, r1]
00369540: ldrb r1, [r1]
00369544: cmp r1, #0
00369548: bne #0x3695dc
0036954c: ldr ip, [pc, #0x98]
00369550: mov r5, #0xc
00369554: ldr r0, [r0, #8]
00369558: ldr ip, [r3, ip]
0036955c: ldr ip, [ip]
00369560: mla r2, r5, r2, ip
00369564: ldr r2, [r2, #4]
00369568: ldr ip, [r0, r2, lsl #2]
0036956c: cmp ip, #0
00369570: beq #0x3695dc
00369574: ldr ip, [pc, #0x74]
00369578: mvn r8, #0
0036957c: mvn sb, #0
00369580: ldr ip, [r3, ip]
00369584: strd r8, sb, [sp, #8]
00369588: add ip, ip, #8
0036958c: add r5, sp, #0x28
00369590: str r1, [sp, #0x20]
00369594: str ip, [r5, #-0x28]!
00369598: str r1, [sp, #0x10]
0036959c: str r1, [sp, #0x14]
003695a0: str r1, [sp, #0x18]
003695a4: str r1, [sp, #0x1c]
003695a8: ldr r1, [r0, r2, lsl #2]
003695ac: mov r3, #1
003695b0: ldr r0, [r4]
003695b4: mov r2, sp
003695b8: bl #0x862548
003695bc: cmp r0, #0
003695c0: ble #0x3695d4
003695c4: ldr r0, [r4]
003695c8: mov r2, r6
003695cc: mov r1, sp
003695d0: bl #0x861928
003695d4: mov r0, sp
003695d8: bl #0x8683ac
003695dc: add sp, sp, #0x28
003695e0: pop {r4, r5, r6, r8, sb, pc}
003695e4: rsbeq fp, r2, r0, ror #10
003695e8: andeq r3, r0, r0, lsr fp
003695ec: andeq r3, r0, ip, lsr lr
003695f0: andeq r2, r0, r8, lsr #28

# 0x3695f4 _ZN15VoxSoundManager15SetLevelRoutingEi
003695f4: push {r4, r5, r6, r7, r8, lr}
003695f8: ldr r4, [pc, #0xc4]
003695fc: ldr r5, [pc, #0xc4]
00369600: sub sp, sp, #0x208
00369604: add r4, pc, r4
00369608: ldr r2, [r4, r5]
0036960c: cmn r1, #1
00369610: mov r3, r1
00369614: ldr r2, [r2]
00369618: mov r7, r0
0036961c: str r2, [sp, #0x204]
00369620: beq #0x369638
00369624: ldr r2, [pc, #0xa0]
00369628: ldr r2, [r4, r2]
0036962c: ldrb r2, [r2]
00369630: cmp r2, #0
00369634: beq #0x369658
00369638: mov r0, #0
0036963c: ldr r3, [r4, r5]
00369640: ldr r2, [sp, #0x204]
00369644: ldr r3, [r3]
00369648: cmp r2, r3
0036964c: bne #0x3696c0
00369650: add sp, sp, #0x208
00369654: pop {r4, r5, r6, r7, r8, pc}
00369658: ldr r2, [pc, #0x70]
0036965c: ldr r1, [pc, #0x70]
00369660: add r6, sp, #4
00369664: ldr r2, [r4, r2]
00369668: ldr r1, [r4, r1]
0036966c: mov r0, r6
00369670: ldr r2, [r2]
00369674: ldr r1, [r1]
00369678: mov r8, #0x48
0036967c: mla r8, r8, r3, r2
00369680: bl #0x30e520
00369684: mov r0, r6
00369688: bl #0x30de54
0036968c: ldr r1, [pc, #0x44]
00369690: mov r2, #0xe
00369694: add r0, r6, r0
00369698: add r1, pc, r1
0036969c: bl #0x30e868
003696a0: ldr r1, [r8, #0xc]
003696a4: mov r0, r6
003696a8: bl #0x30ed90
003696ac: ldr r0, [r7]
003696b0: mov r1, r6
003696b4: bl #0x8619e0
003696b8: mov r0, #1
003696bc: b #0x36963c
003696c0: bl #0x30e310
003696c4: rsbeq fp, r2, ip, lsl #9
003696c8: andeq r4, r0, ip, lsr #1
003696cc: andeq r3, r0, r0, lsr fp
003696d0: andeq r0, r0, r4, ror r8
003696d4: andeq r0, r0, r0, lsl #12
003696d8: subseq r7, r5, r0, asr #17

# 0x36a470 _ZN15VoxSoundManager6ResumeEi
0036a470: push {r4, r5, r6, r7, r8, sb, lr}
0036a474: ldr r3, [pc, #0x104]
0036a478: cmp r1, #0
0036a47c: sub sp, sp, #0x194
0036a480: mov r6, r0
0036a484: add r3, pc, r3
0036a488: blt #0x36a56c
0036a48c: ldr r2, [pc, #0xf0]
0036a490: ldr r2, [r3, r2]
0036a494: ldrb r4, [r2]
0036a498: cmp r4, #0
0036a49c: bne #0x36a574
0036a4a0: ldr r2, [r0, #8]
0036a4a4: ldr r0, [pc, #0xdc]
0036a4a8: mov ip, #0xc
0036a4ac: ldr r0, [r3, r0]
0036a4b0: ldr r0, [r0]
0036a4b4: mla r1, ip, r1, r0
0036a4b8: ldr r1, [r1, #4]
0036a4bc: ldr r0, [r2, r1, lsl #2]
0036a4c0: cmp r0, #0
0036a4c4: beq #0x36a56c
0036a4c8: ldr r0, [pc, #0xbc]
0036a4cc: mov r5, sp
0036a4d0: add ip, sp, #0x1b8
0036a4d4: ldr r0, [r3, r0]
0036a4d8: mvn r8, #0
0036a4dc: add r3, sp, #0x28
0036a4e0: add r0, r0, #8
0036a4e4: mvn sb, #0
0036a4e8: strd r8, sb, [r3, #-0x20]
0036a4ec: str r4, [r3, #-0x18]
0036a4f0: str r4, [r3, #-0x14]
0036a4f4: str r4, [r3, #-0x10]
0036a4f8: str r4, [r3, #-0xc]
0036a4fc: str r4, [r3, #-8]
0036a500: str r0, [r3, #-0x28]
0036a504: add r3, r3, #0x28
0036a508: cmp r3, ip
0036a50c: bne #0x36a4e8
0036a510: ldr r1, [r2, r1, lsl #2]
0036a514: ldr r0, [r6]
0036a518: mov r2, sp
0036a51c: mov r3, #0xa
0036a520: bl #0x862548
0036a524: subs r7, r0, #0
0036a528: ble #0x36a550
0036a52c: mov r8, #0x28
0036a530: movw r2, #0xcccd
0036a534: mla r1, r8, r4, r5
0036a538: ldr r0, [r6]
0036a53c: add r4, r4, #1
0036a540: movt r2, #0x3d4c
0036a544: bl #0x862148
0036a548: cmp r4, r7
0036a54c: bne #0x36a530
0036a550: add r4, sp, #0x190
0036a554: ldr r3, [r4, #-0x28]!
0036a558: mov r0, r4
0036a55c: mov lr, pc
0036a560: ldr pc, [r3]
0036a564: cmp r4, r5
0036a568: bne #0x36a554
0036a56c: add sp, sp, #0x194
0036a570: pop {r4, r5, r6, r7, r8, sb, pc}
0036a574: mov r0, r1
0036a578: bl #0x5314c8
0036a57c: b #0x36a56c
0036a580: rsbeq sl, r2, ip, lsl #12
0036a584: andeq r3, r0, r0, lsr fp
0036a588: andeq r3, r0, ip, lsr lr
0036a58c: andeq r2, r0, r8, lsr #28

# 0x36921c _ZN15VoxSoundManager11UnloadSoundEi
0036921c: ldr r3, [pc, #0x6c]
00369220: ldr r2, [pc, #0x6c]
00369224: push {r4, r5, lr}
00369228: add r3, pc, r3
0036922c: ldr r2, [r3, r2]
00369230: sub sp, sp, #0xc
00369234: mov r4, r0
00369238: ldrb r5, [r2]
0036923c: cmp r5, #0
00369240: bne #0x369288
00369244: cmp r1, #0
00369248: blt #0x369288
0036924c: ldr r3, [r0, #0x1c]
00369250: cmp r1, r3
00369254: bge #0x369288
00369258: ldr r3, [r0, #8]
0036925c: ldr r3, [r3, r1, lsl #2]
00369260: cmp r3, #0
00369264: beq #0x369288
00369268: mov r0, r3
0036926c: ldr r3, [r3]
00369270: str r1, [sp, #4]
00369274: mov lr, pc
00369278: ldr pc, [r3, #4]
0036927c: ldr r3, [r4, #8]
00369280: ldr r1, [sp, #4]
00369284: str r5, [r3, r1, lsl #2]
00369288: add sp, sp, #0xc
0036928c: pop {r4, r5, pc}
00369290: rsbeq fp, r2, r8, ror #16
00369294: andeq r3, r0, r0, lsr fp

# 0x369f50 _ZN15VoxSoundManager14SetListenerPosERKN6glitch4core8vector3dIfEES5_S5_iif
00369f50: ldr ip, [pc, #0x8c]
00369f54: push {r4, r5, r6, lr}
00369f58: mov r4, r0
00369f5c: ldr r0, [pc, #0x84]
00369f60: add ip, pc, ip
00369f64: mov r6, r2
00369f68: ldr r0, [ip, r0]
00369f6c: sub sp, sp, #0x10
00369f70: mov ip, r1
00369f74: ldrb r2, [r0]
00369f78: mov r5, r3
00369f7c: cmp r2, #0
00369f80: bne #0x369fdc
00369f84: ldr r3, [r1, #8]
00369f88: ldr r2, [ip, #4]
00369f8c: ldr r0, [r4]
00369f90: ldr r1, [r1]
00369f94: bl #0x861c18
00369f98: ldr r3, [r6, #8]
00369f9c: ldr r1, [r6]
00369fa0: ldr r2, [r6, #4]
00369fa4: ldr ip, [r5, #8]
00369fa8: ldr r6, [r5]
00369fac: ldr lr, [r5, #4]
00369fb0: ldr r0, [r4]
00369fb4: str r6, [sp]
00369fb8: str lr, [sp, #4]
00369fbc: str ip, [sp, #8]
00369fc0: bl #0x861bb0
00369fc4: ldr r3, [sp, #0x28]
00369fc8: str r3, [r4, #0x5c]
00369fcc: ldr r3, [sp, #0x20]
00369fd0: str r3, [r4, #0x54]
00369fd4: ldr r3, [sp, #0x24]
00369fd8: str r3, [r4, #0x58]
00369fdc: add sp, sp, #0x10
00369fe0: pop {r4, r5, r6, pc}
00369fe4: rsbeq sl, r2, r0, lsr fp
00369fe8: andeq r3, r0, r0, lsr fp

# 0x36ca88 _ZN15VoxSoundManager14CreateInstanceEv
0036ca88: ldr r3, [pc, #0x38]
0036ca8c: ldr r2, [pc, #0x38]
0036ca90: push {r4, r5, r6, lr}
0036ca94: add r3, pc, r3
0036ca98: ldr r4, [r3, r2]
0036ca9c: ldr r3, [r4]
0036caa0: cmp r3, #0
0036caa4: beq #0x36caac
0036caa8: pop {r4, r5, r6, pc}
0036caac: mov r1, #4
0036cab0: mov r0, #0xc4
0036cab4: bl #0x310570
0036cab8: mov r5, r0
0036cabc: bl #0x36c7b0
0036cac0: str r5, [r4]
0036cac4: pop {r4, r5, r6, pc}

# 0x36a590 _ZN15VoxSoundManager15ResumeAllSoundsEv
0036a590: push {r4, lr}
0036a594: ldr r4, [pc, #0x78]
0036a598: ldr r3, [pc, #0x78]
0036a59c: add r4, pc, r4
0036a5a0: ldr r3, [r4, r3]
0036a5a4: ldrb r3, [r3]
0036a5a8: cmp r3, #0
0036a5ac: bne #0x36a5d0
0036a5b0: ldr r0, [r0]
0036a5b4: cmp r0, #0
0036a5b8: beq #0x36a5ec
0036a5bc: movw r2, #0xcccd
0036a5c0: mvn r1, #0
0036a5c4: movt r2, #0x3e4c
0036a5c8: pop {r4, lr}
0036a5cc: b #0x862080
0036a5d0: movw r0, #0x270f
0036a5d4: bl #0x5314c8
0036a5d8: ldr r3, [pc, #0x3c]
0036a5dc: ldr r3, [r4, r3]
0036a5e0: ldrb r2, [r3]
0036a5e4: cmp r2, #0
0036a5e8: bne #0x36a5f0
0036a5ec: pop {r4, pc}
0036a5f0: mov r2, #0
0036a5f4: strb r2, [r3]
0036a5f8: ldr r3, [pc, #0x20]
0036a5fc: ldr r3, [r4, r3]
0036a600: ldr r0, [r3]
0036a604: cmn r0, #1
0036a608: beq #0x36a5ec
0036a60c: pop {r4, lr}
0036a610: b #0x5314c8

# 0x369ee8 _ZN15VoxSoundManager15GetMasterVolumeEv
00369ee8: ldr r3, [pc, #0x24]
00369eec: ldr r2, [pc, #0x24]
00369ef0: add r3, pc, r3
00369ef4: ldr r2, [r3, r2]
00369ef8: ldrb r3, [r2]
00369efc: cmp r3, #0
00369f00: beq #0x369f0c
00369f04: mov r0, #0x3f800000
00369f08: bx lr
00369f0c: ldr r0, [r0]
00369f10: b #0x861eb0
00369f14: rsbeq sl, r2, r0, lsr #23
00369f18: andeq r3, r0, r0, lsr fp

# 0x36c164 _ZN15VoxSoundManager11ResumeMusicEi
0036c164: push {r4, r5, r6, r7, r8, sl, lr}
0036c168: ldr r4, [pc, #0xa4]
0036c16c: ldr r5, [pc, #0xa4]
0036c170: ldr r2, [pc, #0xa4]
0036c174: add r4, pc, r4
0036c178: ldr r3, [r4, r5]
0036c17c: ldr r8, [r4, r2]
0036c180: sub sp, sp, #0x2c
0036c184: ldr r3, [r3]
0036c188: mov sl, r0
0036c18c: mov r0, r8
0036c190: str r3, [sp, #0x24]
0036c194: mov r6, r1
0036c198: bl #0x337888
0036c19c: ldr r1, [pc, #0x7c]
0036c1a0: add r7, sp, #0xc
0036c1a4: add r2, sp, #8
0036c1a8: add r1, pc, r1
0036c1ac: mov r0, r7
0036c1b0: bl #0x3140ec
0036c1b4: mov r0, r8
0036c1b8: mov r1, r7
0036c1bc: bl #0x337a88
0036c1c0: mov r8, r0
0036c1c4: mov r0, r7
0036c1c8: bl #0x3139ac
0036c1cc: cmp r8, #0
0036c1d0: bne #0x36c1f4
0036c1d4: ldr r1, [sl, #0x24]
0036c1d8: cmn r1, #1
0036c1dc: beq #0x36c1f4
0036c1e0: ldrb r2, [sl, #0x30]
0036c1e4: mov r0, sl
0036c1e8: mov r3, #1
0036c1ec: str r6, [sp]
0036c1f0: bl #0x36bd78
0036c1f4: ldr r3, [r4, r5]
0036c1f8: ldr r2, [sp, #0x24]
0036c1fc: ldr r3, [r3]
0036c200: cmp r2, r3
0036c204: bne #0x36c210
0036c208: add sp, sp, #0x2c
0036c20c: pop {r4, r5, r6, r7, r8, sl, pc}
0036c210: bl #0x30e310
0036c214: rsbeq r8, r2, ip, lsl sb
0036c218: andeq r4, r0, ip, lsr #1
0036c21c: andeq r0, r0, r4, lsl #17
0036c220: subseq r4, r5, r0, asr #30

# 0x3b3b00 _ZN9Character11_InitSoundsEv
003b3b00: push {r4, r5, r6, r7, r8, lr}
003b3b04: bl #0x3a32d0
003b3b08: ldr r6, [pc, #0xf0]
003b3b0c: ldr r7, [pc, #0xf0]
003b3b10: mov r4, r0
003b3b14: add r6, pc, r6
003b3b18: ldr r3, [r6, r7]
003b3b1c: ldr r0, [r3]
003b3b20: cmp r0, #0
003b3b24: beq #0x3b3bfc
003b3b28: ldr r3, [r4, #0xc]
003b3b2c: cmp r3, #0
003b3b30: beq #0x3b3b60
003b3b34: mov r5, #0
003b3b38: b #0x3b3b44
003b3b3c: ldr r3, [r6, r7]
003b3b40: ldr r0, [r3]
003b3b44: ldr r3, [r4, #0x10]
003b3b48: ldr r1, [r3, r5, lsl #2]
003b3b4c: bl #0x3699fc
003b3b50: ldr r3, [r4, #0xc]
003b3b54: add r5, r5, #1
003b3b58: cmp r3, r5
003b3b5c: bhi #0x3b3b3c
003b3b60: ldr r3, [r4, #4]
003b3b64: cmp r3, #0
003b3b68: beq #0x3b3b94
003b3b6c: ldr r8, [r6, r7]
003b3b70: mov r5, #0
003b3b74: ldr r3, [r4, #8]
003b3b78: ldr r0, [r8]
003b3b7c: ldr r1, [r3, r5, lsl #2]
003b3b80: bl #0x3699fc
003b3b84: ldr r3, [r4, #4]
003b3b88: add r5, r5, #1
003b3b8c: cmp r3, r5
003b3b90: bhi #0x3b3b74
003b3b94: ldr r3, [r4, #0x14]
003b3b98: cmp r3, #0
003b3b9c: beq #0x3b3bc8
003b3ba0: ldr r8, [r6, r7]
003b3ba4: mov r5, #0
003b3ba8: ldr r3, [r4, #0x18]
003b3bac: ldr r0, [r8]
003b3bb0: ldr r1, [r3, r5, lsl #2]
003b3bb4: bl #0x3699fc
003b3bb8: ldr r3, [r4, #0x14]
003b3bbc: add r5, r5, #1
003b3bc0: cmp r3, r5
003b3bc4: bhi #0x3b3ba8
003b3bc8: ldr r3, [r4, #0x1c]
003b3bcc: cmp r3, #0
003b3bd0: beq #0x3b3bfc
003b3bd4: ldr r6, [r6, r7]
003b3bd8: mov r5, #0
003b3bdc: ldr r3, [r4, #0x20]
003b3be0: ldr r0, [r6]
003b3be4: ldr r1, [r3, r5, lsl #2]
003b3be8: bl #0x3699fc
003b3bec: ldr r3, [r4, #0x1c]
003b3bf0: add r5, r5, #1
003b3bf4: cmp r3, r5
003b3bf8: bhi #0x3b3bdc
003b3bfc: pop {r4, r5, r6, r7, r8, pc}
003b3c00: subseq r0, lr, ip, ror pc
003b3c04: andeq r0, r0, r4, lsr #27

# 0x3699fc _ZN15VoxSoundManager9LoadSoundEi
003699fc: push {r4, r5, r6, r7, r8, sl, lr}
00369a00: ldr r4, [pc, #0x11c]
00369a04: ldr r3, [pc, #0x11c]
00369a08: ldr r5, [pc, #0x11c]
00369a0c: add r4, pc, r4
00369a10: ldr r2, [r4, r3]
00369a14: ldr r3, [r4, r5]
00369a18: sub sp, sp, #0x22c
00369a1c: ldrb r2, [r2]
00369a20: ldr r3, [r3]
00369a24: mov r7, r0
00369a28: cmp r2, #0
00369a2c: mov r6, r1
00369a30: str r3, [sp, #0x224]
00369a34: bne #0x369a4c
00369a38: cmp r1, #0
00369a3c: blt #0x369a4c
00369a40: ldr r3, [r0, #0x1c]
00369a44: cmp r1, r3
00369a48: ble #0x369a68
00369a4c: ldr r3, [r4, r5]
00369a50: ldr r2, [sp, #0x224]
00369a54: ldr r3, [r3]
00369a58: cmp r2, r3
00369a5c: bne #0x369b20
00369a60: add sp, sp, #0x22c
00369a64: pop {r4, r5, r6, r7, r8, sl, pc}
00369a68: add ip, sp, #0x20
00369a6c: str ip, [sp]
00369a70: add ip, sp, #0x1c
00369a74: add r3, sp, #0x18
00369a78: str ip, [sp, #4]
00369a7c: add r0, r0, #0x64
00369a80: add ip, sp, #0x14
00369a84: add r2, sp, #0x10
00369a88: str ip, [sp, #8]
00369a8c: bl #0x8896f4
00369a90: ldr r3, [r7, #0x1c]
00369a94: cmp r6, r3
00369a98: bgt #0x369a4c
00369a9c: ldr r3, [r7, #8]
00369aa0: ldr r3, [r3, r6, lsl #2]
00369aa4: cmp r3, #0
00369aa8: bne #0x369a4c
00369aac: ldr r3, [pc, #0x7c]
00369ab0: add sl, sp, #0x24
00369ab4: mov r0, sl
00369ab8: ldr r3, [r4, r3]
00369abc: ldr r1, [r3]
00369ac0: bl #0x30e520
00369ac4: mov r0, sl
00369ac8: bl #0x30de54
00369acc: ldr r1, [pc, #0x60]
00369ad0: mov r2, #0xd
00369ad4: add r0, sl, r0
00369ad8: add r1, pc, r1
00369adc: bl #0x30e868
00369ae0: ldr r1, [sp, #0x10]
00369ae4: mov r0, sl
00369ae8: bl #0x30ed90
00369aec: mov r1, #4
00369af0: mov r0, #0x28
00369af4: bl #0x310570
00369af8: ldr ip, [sp, #0x20]
00369afc: ldr r3, [sp, #0x14]
00369b00: mov r1, sl
00369b04: ldr r2, [sp, #0x18]
00369b08: mov r8, r0
00369b0c: str ip, [sp]
00369b10: bl #0x86f5fc
00369b14: ldr r3, [r7, #8]
00369b18: str r8, [r3, r6, lsl #2]
00369b1c: b #0x369a4c
00369b20: bl #0x30e310
00369b24: rsbeq fp, r2, r4, lsl #1
00369b28: andeq r3, r0, r0, lsr fp
00369b2c: andeq r4, r0, ip, lsr #1
00369b30: andeq r0, r0, r0, lsl #12

# 0x3692f0 _ZN15VoxSoundManager24ConvertVisual3DToSound3DERKN6glitch4core8vector3dIfEE
003692f0: push {r4, r5, r6, r7, r8, lr}
003692f4: mov r5, r1
003692f8: movw r1, #0xd70a
003692fc: mov r4, r0
00369300: movt r1, #0x3c23
00369304: ldr r0, [r5, #4]
00369308: bl #0x30ed6c
0036930c: movw r1, #0xd70a
00369310: mov r7, r0
00369314: movt r1, #0x3c23
00369318: ldr r0, [r5, #8]
0036931c: bl #0x30ed6c
00369320: movw r1, #0xd70a
00369324: mov r6, r0
00369328: movt r1, #0x3c23
0036932c: ldr r0, [r5]
00369330: bl #0x30ed6c
00369334: str r7, [r4, #4]
00369338: str r0, [r4]
0036933c: str r6, [r4, #8]
00369340: mov r0, r4
00369344: pop {r4, r5, r6, r7, r8, pc}

# 0x36a784 _ZN15VoxSoundManager10PauseMusicEv
0036a784: ldr r1, [r0, #0x24]
0036a788: ldr r3, [pc, #0x28]
0036a78c: cmn r1, #1
0036a790: add r3, pc, r3
0036a794: bxeq lr
0036a798: ldr r2, [pc, #0x1c]
0036a79c: ldr r3, [r3, r2]
0036a7a0: ldrb r3, [r3]
0036a7a4: cmp r3, #0
0036a7a8: bne #0x36a7b0
0036a7ac: b #0x36a664
0036a7b0: mov r0, r1
0036a7b4: b #0x531420
0036a7b8: rsbeq sl, r2, r0, lsl #6
0036a7bc: andeq r3, r0, r0, lsr fp

# 0x369348 _ZN15VoxSoundManager14SetIPodShuffleEv
00369348: bx lr

# 0x36bc58 _ZN15VoxSoundManager8PlayBeatEibb
0036bc58: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036bc5c: ldr r4, [pc, #0x100]
0036bc60: ldr r5, [pc, #0x100]
0036bc64: ldr lr, [pc, #0x100]
0036bc68: add r4, pc, r4
0036bc6c: ldr ip, [r4, r5]
0036bc70: ldr r6, [r4, lr]
0036bc74: sub sp, sp, #0x2c
0036bc78: ldr ip, [ip]
0036bc7c: mov r7, r0
0036bc80: mov r0, r6
0036bc84: mov fp, r3
0036bc88: str ip, [sp, #0x24]
0036bc8c: mov sl, r1
0036bc90: mov sb, r2
0036bc94: bl #0x337888
0036bc98: ldr r1, [pc, #0xd0]
0036bc9c: add r8, sp, #0xc
0036bca0: add r2, sp, #8
0036bca4: add r1, pc, r1
0036bca8: mov r0, r8
0036bcac: bl #0x3140ec
0036bcb0: mov r0, r6
0036bcb4: mov r1, r8
0036bcb8: bl #0x337a88
0036bcbc: mov r6, r0
0036bcc0: mov r0, r8
0036bcc4: bl #0x3139ac
0036bcc8: cmp r6, #0
0036bccc: bne #0x36bd30
0036bcd0: ldr r3, [pc, #0x9c]
0036bcd4: ldr r3, [r4, r3]
0036bcd8: ldrb r3, [r3, #0xb4]
0036bcdc: cmp r3, #0
0036bce0: beq #0x36bd30
0036bce4: cmn sl, #1
0036bce8: beq #0x36bd4c
0036bcec: ldr r3, [r7, #0x2c]
0036bcf0: cmp r3, sl
0036bcf4: beq #0x36bd30
0036bcf8: mov r0, r7
0036bcfc: bl #0x36a138
0036bd00: ldr r3, [r7, #0x2c]
0036bd04: cmp r3, sl
0036bd08: beq #0x36bd30
0036bd0c: str sl, [r7, #0x2c]
0036bd10: mov ip, #2
0036bd14: mov r0, r7
0036bd18: mov r1, sl
0036bd1c: mov r2, sb
0036bd20: mov r3, r6
0036bd24: str ip, [sp]
0036bd28: str r6, [sp, #4]
0036bd2c: bl #0x36b80c
0036bd30: ldr r3, [r4, r5]
0036bd34: ldr r2, [sp, #0x24]
0036bd38: ldr r3, [r3]
0036bd3c: cmp r2, r3
0036bd40: bne #0x36bd60
0036bd44: add sp, sp, #0x2c
0036bd48: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036bd4c: cmp fp, #0
0036bd50: beq #0x36bd30
0036bd54: mov r0, r7
0036bd58: bl #0x36a138
0036bd5c: b #0x36bd30
0036bd60: bl #0x30e310
0036bd64: rsbeq r8, r2, r8, lsr #28
0036bd68: andeq r4, r0, ip, lsr #1
0036bd6c: andeq r0, r0, r4, lsl #17
0036bd70: subseq r5, r5, r4, asr #8
0036bd74: strdeq r3, r4, [r0], -r4

# 0x36afb0 _ZN15VoxSoundManagerD1Ev
0036afb0: ldr r3, [pc, #0x88]
0036afb4: ldr r2, [pc, #0x88]
0036afb8: push {r4, lr}
0036afbc: add r3, pc, r3
0036afc0: ldr r2, [r3, r2]
0036afc4: mov r4, r0
0036afc8: ldrb r3, [r2]
0036afcc: cmp r3, #0
0036afd0: beq #0x36b014
0036afd4: add r0, r4, #0x64
0036afd8: bl #0x88c2cc
0036afdc: add r0, r4, #0x38
0036afe0: bl #0x3139ac
0036afe4: ldr r0, [r4, #0xc]
0036afe8: add r3, r4, #0xc
0036afec: cmp r0, #0
0036aff0: beq #0x36b00c
0036aff4: ldr r1, [r3, #8]
0036aff8: rsb r1, r0, r1
0036affc: bic r1, r1, #3
0036b000: cmp r1, #0x80
0036b004: bhi #0x36b034
0036b008: bl #0x708f00
0036b00c: mov r0, r4
0036b010: pop {r4, pc}
0036b014: ldr r0, [r0, #4]
0036b018: bl #0x310440
0036b01c: mov r0, r4
0036b020: bl #0x369298
0036b024: ldr r0, [r4, #8]
0036b028: bl #0x310440
0036b02c: bl #0x8628f8
0036b030: b #0x36afd4
0036b034: bl #0x310440
0036b038: mov r0, r4
0036b03c: pop {r4, pc}

# 0x36b2cc _ZN15VoxSoundManager16ReleaseLvlSoundsEv
0036b2cc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b2d0: ldr r5, [pc, #0x134]
0036b2d4: ldr r3, [pc, #0x134]
0036b2d8: ldr r8, [pc, #0x134]
0036b2dc: add r5, pc, r5
0036b2e0: ldr r3, [r5, r3]
0036b2e4: ldr r2, [r5, r8]
0036b2e8: sub sp, sp, #0x34
0036b2ec: ldrb r3, [r3]
0036b2f0: ldr r2, [r2]
0036b2f4: mov r4, r0
0036b2f8: cmp r3, #0
0036b2fc: str r2, [sp, #0x2c]
0036b300: bne #0x36b380
0036b304: ldr r1, [r0, #0x1c]
0036b308: str r3, [sp, #8]
0036b30c: cmp r1, #0
0036b310: ble #0x36b380
0036b314: ldr r2, [pc, #0xfc]
0036b318: ldr fp, [pc, #0xfc]
0036b31c: add r7, sp, #8
0036b320: add r2, pc, r2
0036b324: str r2, [sp]
0036b328: add r2, sp, #0x10
0036b32c: add r6, sp, #0xc
0036b330: add sl, sp, #0x14
0036b334: str r2, [sp, #4]
0036b338: ldr r2, [r4, #8]
0036b33c: ldr r3, [r2, r3, lsl #2]
0036b340: cmp r3, #0
0036b344: beq #0x36b36c
0036b348: mov r3, r6
0036b34c: ldr r0, [r4, #0xc]
0036b350: ldr r1, [r4, #0x10]
0036b354: mov r2, r7
0036b358: bl #0x369350
0036b35c: ldr r3, [r4, #0x10]
0036b360: cmp r3, r0
0036b364: beq #0x36b39c
0036b368: ldr r1, [r4, #0x1c]
0036b36c: ldr r3, [sp, #8]
0036b370: add r3, r3, #1
0036b374: cmp r1, r3
0036b378: str r3, [sp, #8]
0036b37c: bgt #0x36b338
0036b380: ldr r3, [r5, r8]
0036b384: ldr r2, [sp, #0x2c]
0036b388: ldr r3, [r3]
0036b38c: cmp r2, r3
0036b390: bne #0x36b408
0036b394: add sp, sp, #0x34
0036b398: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b39c: ldr sb, [r5, fp]
0036b3a0: mov r0, sb
0036b3a4: bl #0x337888
0036b3a8: ldm sp, {r1, r2}
0036b3ac: mov r0, sl
0036b3b0: bl #0x3140ec
0036b3b4: mov r1, sl
0036b3b8: mov r0, sb
0036b3bc: bl #0x337a88
0036b3c0: mov r0, sl
0036b3c4: bl #0x3139ac
0036b3c8: ldr r1, [sp, #8]
0036b3cc: ldr r2, [r4, #8]
0036b3d0: ldr r3, [r2, r1, lsl #2]
0036b3d4: add r2, r2, r1, lsl #2
0036b3d8: cmp r3, #0
0036b3dc: beq #0x36b3fc
0036b3e0: mov r0, r3
0036b3e4: ldr r3, [r3]
0036b3e8: mov lr, pc
0036b3ec: ldr pc, [r3, #4]
0036b3f0: ldr r3, [r4, #8]
0036b3f4: ldr r2, [sp, #8]
0036b3f8: add r2, r3, r2, lsl #2
0036b3fc: mov r3, #0
0036b400: str r3, [r2]
0036b404: b #0x36b368
0036b408: bl #0x30e310
0036b40c: strhteq sb, [r2], #-0x74
0036b410: andeq r3, r0, r0, lsr fp
0036b414: andeq r4, r0, ip, lsr #1
0036b418: ldrheq r5, [r5], #-0xd0
0036b41c: andeq r0, r0, r4, lsl #17

# 0x36b5d8 _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff
0036b5d8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b5dc: ldr r4, [pc, #0x200]
0036b5e0: ldr r5, [pc, #0x200]
0036b5e4: ldr r7, [pc, #0x200]
0036b5e8: add r4, pc, r4
0036b5ec: ldr ip, [r4, r5]
0036b5f0: ldr r6, [r4, r7]
0036b5f4: sub sp, sp, #0x74
0036b5f8: ldr ip, [ip]
0036b5fc: mov sb, r0
0036b600: mov r0, r6
0036b604: str r3, [sp, #0x1c]
0036b608: str ip, [sp, #0x6c]
0036b60c: mov sl, r1
0036b610: mov fp, r2
0036b614: bl #0x337888
0036b618: ldr r1, [pc, #0x1d0]
0036b61c: add r8, sp, #0x54
0036b620: add r2, sp, #0x38
0036b624: add r1, pc, r1
0036b628: mov r0, r8
0036b62c: bl #0x3140ec
0036b630: mov r0, r6
0036b634: mov r1, r8
0036b638: bl #0x337a88
0036b63c: mov r6, r0
0036b640: mov r0, r8
0036b644: bl #0x3139ac
0036b648: cmp r6, #0
0036b64c: beq #0x36b670
0036b650: ldr r3, [r4, r5]
0036b654: ldr r2, [sp, #0x6c]
0036b658: mov r0, #0
0036b65c: ldr r3, [r3]
0036b660: cmp r2, r3
0036b664: bne #0x36b7e0
0036b668: add sp, sp, #0x74
0036b66c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b670: ldr r3, [pc, #0x17c]
0036b674: ldr r0, [r4, r3]
0036b678: bl #0x31f594
0036b67c: cmp r0, #0
0036b680: beq #0x36b650
0036b684: ldr r3, [r0, #0x130]
0036b688: cmp r3, #0x26
0036b68c: bne #0x36b650
0036b690: bl #0x7fd794
0036b694: ldrb r3, [r0, #5]
0036b698: cmp r3, #0
0036b69c: beq #0x36b6b4
0036b6a0: ldr r3, [pc, #0x150]
0036b6a4: ldr r3, [r4, r3]
0036b6a8: ldrb r3, [r3]
0036b6ac: cmp r3, #0
0036b6b0: bne #0x36b650
0036b6b4: cmp sl, #0
0036b6b8: blt #0x36b650
0036b6bc: ldr r3, [pc, #0x138]
0036b6c0: ldr r3, [r4, r3]
0036b6c4: ldrb r3, [r3]
0036b6c8: cmp r3, #0
0036b6cc: bne #0x36b79c
0036b6d0: ldr r3, [pc, #0x128]
0036b6d4: mov r2, #0xc
0036b6d8: ldr r3, [r4, r3]
0036b6dc: ldr r3, [r3]
0036b6e0: mla sl, r2, sl, r3
0036b6e4: ldr r3, [sl, #8]
0036b6e8: ldr r8, [sl, #4]
0036b6ec: cmp r3, #1
0036b6f0: beq #0x36b7bc
0036b6f4: add ip, sp, #0x30
0036b6f8: str ip, [sp]
0036b6fc: add ip, sp, #0x2c
0036b700: add r3, sp, #0x28
0036b704: mov r1, r8
0036b708: add r2, sp, #0x20
0036b70c: str ip, [sp, #4]
0036b710: add r0, sb, #0x64
0036b714: add ip, sp, #0x24
0036b718: str ip, [sp, #8]
0036b71c: bl #0x8896f4
0036b720: ldr r7, [r4, r7]
0036b724: add r6, sp, #0x3c
0036b728: mov r0, r7
0036b72c: bl #0x337888
0036b730: ldr r1, [pc, #0xcc]
0036b734: add r2, sp, #0x34
0036b738: mov r0, r6
0036b73c: add r1, pc, r1
0036b740: bl #0x3140ec
0036b744: mov r1, r6
0036b748: mov r0, r7
0036b74c: bl #0x337a88
0036b750: mov r0, r6
0036b754: bl #0x3139ac
0036b758: ldr ip, [sp, #0x30]
0036b75c: mov r0, sb
0036b760: mov r1, r8
0036b764: str ip, [sp]
0036b768: ldr ip, [sp, #0x2c]
0036b76c: ldr r2, [sp, #0x20]
0036b770: ldr r3, [sp, #0x28]
0036b774: str ip, [sp, #4]
0036b778: ldr ip, [sp, #0x24]
0036b77c: str fp, [sp, #0xc]
0036b780: str ip, [sp, #8]
0036b784: ldr ip, [sp, #0x9c]
0036b788: str ip, [sp, #0x10]
0036b78c: ldr ip, [sp, #0xa0]
0036b790: str ip, [sp, #0x14]
0036b794: bl #0x36a7c0
0036b798: b #0x36b650
0036b79c: ldr r3, [pc, #0x64]
0036b7a0: mov r0, sl
0036b7a4: ldr r2, [sp, #0x1c]
0036b7a8: ldr r1, [r4, r3]
0036b7ac: mov r3, #2
0036b7b0: ldr r1, [r1]
0036b7b4: bl #0x531348
0036b7b8: b #0x36b650
0036b7bc: mov ip, #0xbf000000
0036b7c0: add ip, ip, #0x800000
0036b7c4: mov r0, sb
0036b7c8: mov r1, r8
0036b7cc: mov r2, fp
0036b7d0: mov r3, ip
0036b7d4: str ip, [sp]
0036b7d8: bl #0x36b420
0036b7dc: b #0x36b650
0036b7e0: bl #0x30e310
0036b7e4: rsbeq sb, r2, r8, lsr #9
0036b7e8: andeq r4, r0, ip, lsr #1
0036b7ec: andeq r0, r0, r4, lsl #17
0036b7f0: subseq r5, r5, r4, asr #21
0036b7f4: strdeq r3, r4, [r0], -r4
0036b7f8: andeq r2, r0, r0, lsr #31
0036b7fc: andeq r3, r0, r0, lsr fp
0036b800: andeq r3, r0, ip, lsr lr
0036b804: subseq r5, r5, r4, asr #19
0036b808: andeq r0, r0, r0, lsl #13

# 0x36a624 _ZN15VoxSoundManager14PauseAllSoundsEv
0036a624: ldr r3, [pc, #0x30]
0036a628: ldr r2, [pc, #0x30]
0036a62c: add r3, pc, r3
0036a630: ldr r2, [r3, r2]
0036a634: ldrb r3, [r2]
0036a638: cmp r3, #0
0036a63c: bxne lr
0036a640: ldr r0, [r0]
0036a644: cmp r0, #0
0036a648: bxeq lr
0036a64c: movw r2, #0xcccd
0036a650: mvn r1, #0
0036a654: movt r2, #0x3e4c
0036a658: b #0x8620a8
0036a65c: rsbeq sl, r2, r4, ror #8
0036a660: andeq r3, r0, r0, lsr fp

# 0x36a7c0 _ZN15VoxSoundManager18PlaySoundPackSoundEiPKcN3vox11FormatTypesEiiNS2_21VoxSourceLoadingFlagsERKN6glitch4core8vector3dIfEEff
0036a7c0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036a7c4: ldr r5, [pc, #0x6e4]
0036a7c8: ldr r3, [pc, #0x6e4]
0036a7cc: sub sp, sp, #0xcc
0036a7d0: add r5, pc, r5
0036a7d4: ldr r3, [r5, r3]
0036a7d8: mov r4, r0
0036a7dc: mov r6, r1
0036a7e0: ldrb r3, [r3]
0036a7e4: ldr r7, [sp, #0xfc]
0036a7e8: ldr sl, [sp, #0x100]
0036a7ec: cmp r3, #0
0036a7f0: ldr sb, [sp, #0x104]
0036a7f4: bne #0x36a9c0
0036a7f8: ldr r3, [r0, #8]
0036a7fc: ldr r3, [r3, r1, lsl #2]
0036a800: cmp r3, #0
0036a804: beq #0x36aaf0
0036a808: mov r1, r3
0036a80c: ldr r0, [r4]
0036a810: bl #0x8624f8
0036a814: cmp r0, #0
0036a818: beq #0x36a9c0
0036a81c: ldr r3, [r4, #8]
0036a820: ldr r2, [sp, #0xf4]
0036a824: ldr r0, [r4]
0036a828: ldr r1, [r3, r6, lsl #2]
0036a82c: bl #0x862618
0036a830: add ip, sp, #0xc7
0036a834: str ip, [sp]
0036a838: add r8, r4, #0x64
0036a83c: add ip, sp, #0xb8
0036a840: mov r1, r6
0036a844: add r2, sp, #0xc0
0036a848: add r3, sp, #0xbc
0036a84c: str ip, [sp, #4]
0036a850: mov r0, r8
0036a854: add ip, sp, #0xb4
0036a858: str ip, [sp, #8]
0036a85c: bl #0x889894
0036a860: ldr r3, [r4, #8]
0036a864: ldr r1, [r4]
0036a868: mov ip, #0
0036a86c: ldr r2, [r3, r6, lsl #2]
0036a870: add r6, sp, #0x38
0036a874: ldr r3, [sp, #0xc0]
0036a878: mov r0, r6
0036a87c: str ip, [sp]
0036a880: bl #0x862438
0036a884: ldr r2, [sp, #0xb8]
0036a888: cmp r2, #0
0036a88c: beq #0x36a9cc
0036a890: mov r0, sl
0036a894: mov r1, #0
0036a898: bl #0x30e4b4
0036a89c: cmp r0, #0
0036a8a0: beq #0x36a8b8
0036a8a4: mov r0, sb
0036a8a8: mov r1, #0
0036a8ac: bl #0x30e4b4
0036a8b0: cmp r0, #0
0036a8b4: bne #0x36ab08
0036a8b8: ldr ip, [r7, #8]
0036a8bc: ldr r2, [r7]
0036a8c0: ldr r3, [r7, #4]
0036a8c4: ldr r0, [r4]
0036a8c8: mov r1, r6
0036a8cc: str ip, [sp]
0036a8d0: bl #0x861e48
0036a8d4: ldr r0, [r4, #0x54]
0036a8d8: bl #0x30e964
0036a8dc: ldr r7, [r4]
0036a8e0: mov r3, r0
0036a8e4: mov r1, r6
0036a8e8: mov r0, r7
0036a8ec: mov r2, #2
0036a8f0: bl #0x861d88
0036a8f4: ldr r0, [r4, #0x58]
0036a8f8: bl #0x30e964
0036a8fc: ldr r7, [r4]
0036a900: mov r3, r0
0036a904: mov r1, r6
0036a908: mov r0, r7
0036a90c: mov r2, #1
0036a910: bl #0x861d88
0036a914: ldr r3, [r4, #0x5c]
0036a918: ldr r0, [r4]
0036a91c: mov r1, r6
0036a920: mov r2, #3
0036a924: bl #0x861d88
0036a928: ldr r3, [sp, #0xb8]
0036a92c: cmp r3, #2
0036a930: beq #0x36ab50
0036a934: ldr r7, [pc, #0x57c]
0036a938: ldr r3, [sp, #0xb4]
0036a93c: ldr r0, [r4]
0036a940: mov r1, r6
0036a944: add r7, pc, r7
0036a948: mov r2, #0
0036a94c: bl #0x861950
0036a950: ldr r3, [r7]
0036a954: tst r3, #1
0036a958: beq #0x36aac0
0036a95c: ldr r7, [pc, #0x558]
0036a960: add r7, pc, r7
0036a964: ldr r3, [r7, #8]
0036a968: tst r3, #1
0036a96c: beq #0x36aa8c
0036a970: ldr r3, [pc, #0x548]
0036a974: ldr r2, [sp, #0xbc]
0036a978: add r3, pc, r3
0036a97c: ldr r1, [r3, #4]
0036a980: cmp r2, r1
0036a984: beq #0x36a9e0
0036a988: ldr r3, [r3, #0xc]
0036a98c: cmp r2, r3
0036a990: beq #0x36a9e0
0036a994: ldr r0, [r4]
0036a998: mov r1, r6
0036a99c: bl #0x862058
0036a9a0: movw r3, #0xcccd
0036a9a4: ldr r0, [r4]
0036a9a8: mov r1, r6
0036a9ac: ldrb r2, [sp, #0xc7]
0036a9b0: movt r3, #0x3d4c
0036a9b4: bl #0x8621c0
0036a9b8: mov r0, r6
0036a9bc: bl #0x8683ac
0036a9c0: mov r0, #0
0036a9c4: add sp, sp, #0xcc
0036a9c8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036a9cc: ldr r0, [r4]
0036a9d0: mov r1, r6
0036a9d4: mov r3, #1
0036a9d8: bl #0x861d60
0036a9dc: b #0x36a914
0036a9e0: ldr r3, [pc, #0x4dc]
0036a9e4: ldr r1, [pc, #0x4dc]
0036a9e8: ldr r0, [r5, r3]
0036a9ec: add r1, pc, r1
0036a9f0: bl #0x320e44
0036a9f4: mov r7, r0
0036a9f8: bl #0x30eda8
0036a9fc: mov r5, r0
0036aa00: mov r0, r7
0036aa04: bl #0x30e964
0036aa08: bl #0x30e8a4
0036aa0c: movw r3, #0x215
0036aa10: movt r3, #0x214d
0036aa14: smull r2, r3, r3, r5
0036aa18: asr r2, r5, #0x1f
0036aa1c: rsb r3, r2, r3, asr #4
0036aa20: mov r8, r0
0036aa24: mov r0, #0x7b
0036aa28: mls r0, r0, r3, r5
0036aa2c: mov sb, r1
0036aa30: bl #0x30ed30
0036aa34: movw r2, #0xa9fc
0036aa38: movw r3, #0x624d
0036aa3c: movt r2, #0xd2f1
0036aa40: movt r3, #0x3f50
0036aa44: bl #0x30eab4
0036aa48: movw r2, #0x1eb8
0036aa4c: movw r3, #0xb851
0036aa50: movt r2, #0xeb85
0036aa54: movt r3, #0x3fee
0036aa58: bl #0x30eb44
0036aa5c: mov r2, r0
0036aa60: mov r3, r1
0036aa64: mov r0, r8
0036aa68: mov r1, sb
0036aa6c: bl #0x30eab4
0036aa70: bl #0x30e6a0
0036aa74: ldr r1, [sp, #0xbc]
0036aa78: mov r2, r0
0036aa7c: mov r0, r4
0036aa80: bl #0x369da0
0036aa84: ldr r2, [sp, #0xbc]
0036aa88: b #0x36a994
0036aa8c: add sl, r7, #8
0036aa90: mov r0, sl
0036aa94: bl #0x30e76c
0036aa98: cmp r0, #0
0036aa9c: beq #0x36a970
0036aaa0: ldr r1, [pc, #0x424]
0036aaa4: mov r0, r8
0036aaa8: add r1, pc, r1
0036aaac: bl #0x88b8b8
0036aab0: str r0, [r7, #0xc]
0036aab4: mov r0, sl
0036aab8: bl #0x30ea3c
0036aabc: b #0x36a970
0036aac0: mov r0, r7
0036aac4: bl #0x30e76c
0036aac8: cmp r0, #0
0036aacc: beq #0x36a95c
0036aad0: ldr r1, [pc, #0x3f8]
0036aad4: mov r0, r8
0036aad8: add r1, pc, r1
0036aadc: bl #0x88b8b8
0036aae0: str r0, [r7, #4]
0036aae4: mov r0, r7
0036aae8: bl #0x30ea3c
0036aaec: b #0x36a95c
0036aaf0: bl #0x3699fc
0036aaf4: ldr r3, [r4, #8]
0036aaf8: ldr r3, [r3, r6, lsl #2]
0036aafc: cmp r3, #0
0036ab00: beq #0x36a9c0
0036ab04: b #0x36a808
0036ab08: ldr ip, [r7, #8]
0036ab0c: ldr r0, [r4]
0036ab10: ldr r2, [r7]
0036ab14: ldr r3, [r7, #4]
0036ab18: mov r1, r6
0036ab1c: str ip, [sp]
0036ab20: bl #0x861e48
0036ab24: mov r3, sl
0036ab28: ldr r0, [r4]
0036ab2c: mov r1, r6
0036ab30: mov r2, #2
0036ab34: bl #0x861d88
0036ab38: mov r3, sb
0036ab3c: ldr r0, [r4]
0036ab40: mov r1, r6
0036ab44: mov r2, #1
0036ab48: bl #0x861d88
0036ab4c: b #0x36a914
0036ab50: ldr r0, [r4]
0036ab54: mov r1, r6
0036ab58: mov r2, #0
0036ab5c: mov r3, #1
0036ab60: bl #0x861d60
0036ab64: add ip, sp, #0xa4
0036ab68: ldr r0, [r4]
0036ab6c: str ip, [sp]
0036ab70: add ip, sp, #0xa0
0036ab74: add r1, sp, #0xb0
0036ab78: add r2, sp, #0xac
0036ab7c: add r3, sp, #0xa8
0036ab80: str ip, [sp, #4]
0036ab84: add ip, sp, #0x9c
0036ab88: str ip, [sp, #8]
0036ab8c: bl #0x861aa8
0036ab90: ldr r3, [sp, #0xac]
0036ab94: ldr r2, [sp, #0xa4]
0036ab98: add r0, sp, #0x78
0036ab9c: str r3, [sp, #0x7c]
0036aba0: ldr r3, [sp, #0xb0]
0036aba4: str r2, [sp, #0x1c]
0036aba8: ldr fp, [sp, #0x9c]
0036abac: str r3, [sp, #0x78]
0036abb0: ldr r3, [sp, #0xa8]
0036abb4: str r3, [sp, #0x80]
0036abb8: ldr r3, [sp, #0xa0]
0036abbc: str r3, [sp, #0x18]
0036abc0: bl #0x35e8e0
0036abc4: ldr lr, [sp, #0x18]
0036abc8: ldr sb, [r0, #8]
0036abcc: ldr sl, [r0, #4]
0036abd0: add r1, lr, #0x80000000
0036abd4: mov r3, r0
0036abd8: mov r0, sb
0036abdc: ldr r7, [r3]
0036abe0: bl #0x30ed6c
0036abe4: mov r1, sl
0036abe8: mov r3, r0
0036abec: mov r0, fp
0036abf0: str r3, [sp, #0x14]
0036abf4: bl #0x30ed6c
0036abf8: ldr r3, [sp, #0x14]
0036abfc: mov r1, r0
0036ac00: mov r0, r3
0036ac04: bl #0x30eba4
0036ac08: add r1, fp, #0x80000000
0036ac0c: str r0, [sp, #0x6c]
0036ac10: mov r0, r7
0036ac14: bl #0x30ed6c
0036ac18: mov r1, sb
0036ac1c: mov fp, r0
0036ac20: ldr r0, [sp, #0x1c]
0036ac24: bl #0x30ed6c
0036ac28: mov r1, r0
0036ac2c: mov r0, fp
0036ac30: bl #0x30eba4
0036ac34: ldr r2, [sp, #0x1c]
0036ac38: str r0, [sp, #0x70]
0036ac3c: mov r0, sl
0036ac40: add r1, r2, #0x80000000
0036ac44: bl #0x30ed6c
0036ac48: mov r1, r7
0036ac4c: mov fp, r0
0036ac50: ldr r0, [sp, #0x18]
0036ac54: bl #0x30ed6c
0036ac58: mov r1, r0
0036ac5c: mov r0, fp
0036ac60: bl #0x30eba4
0036ac64: str r0, [sp, #0x74]
0036ac68: add r0, sp, #0x6c
0036ac6c: bl #0x35e8e0
0036ac70: ldr lr, [r0, #8]
0036ac74: mov r3, r0
0036ac78: add r1, sl, #0x80000000
0036ac7c: str lr, [sp, #0x20]
0036ac80: ldr r2, [r0, #4]
0036ac84: mov r0, lr
0036ac88: str r2, [sp, #0x1c]
0036ac8c: ldr r3, [r3]
0036ac90: str r3, [sp, #0x18]
0036ac94: bl #0x30ed6c
0036ac98: ldr r1, [sp, #0x1c]
0036ac9c: mov fp, r0
0036aca0: mov r0, sb
0036aca4: bl #0x30ed6c
0036aca8: mov r1, r0
0036acac: mov r0, fp
0036acb0: bl #0x30eba4
0036acb4: add r1, sb, #0x80000000
0036acb8: str r0, [sp, #0x60]
0036acbc: ldr r0, [sp, #0x18]
0036acc0: bl #0x30ed6c
0036acc4: ldr r1, [sp, #0x20]
0036acc8: mov fp, r0
0036accc: mov r0, r7
0036acd0: bl #0x30ed6c
0036acd4: mov r1, r0
0036acd8: mov r0, fp
0036acdc: bl #0x30eba4
0036ace0: add r1, r7, #0x80000000
0036ace4: str r0, [sp, #0x64]
0036ace8: ldr r0, [sp, #0x1c]
0036acec: bl #0x30ed6c
0036acf0: ldr r1, [sp, #0x18]
0036acf4: mov fp, r0
0036acf8: mov r0, sl
0036acfc: bl #0x30ed6c
0036ad00: mov r1, r0
0036ad04: mov r0, fp
0036ad08: bl #0x30eba4
0036ad0c: str r0, [sp, #0x68]
0036ad10: add r0, sp, #0x60
0036ad14: bl #0x35e8e0
0036ad18: mov ip, r0
0036ad1c: ldr lr, [ip, #8]
0036ad20: ldr r0, [r4]
0036ad24: mov r1, r6
0036ad28: str lr, [sp, #0x34]
0036ad2c: ldr lr, [ip]
0036ad30: add r2, sp, #0x98
0036ad34: add r3, sp, #0x94
0036ad38: str lr, [sp, #0x2c]
0036ad3c: ldr ip, [ip, #4]
0036ad40: str ip, [sp, #0x30]
0036ad44: add ip, sp, #0x90
0036ad48: str ip, [sp]
0036ad4c: bl #0x861d28
0036ad50: add r2, sp, #0x88
0036ad54: add r3, sp, #0x84
0036ad58: ldr r0, [r4]
0036ad5c: add r1, sp, #0x8c
0036ad60: bl #0x861b10
0036ad64: ldr r1, [sp, #0x8c]
0036ad68: ldr r0, [sp, #0x98]
0036ad6c: bl #0x30e3ac
0036ad70: ldr r1, [sp, #0x88]
0036ad74: mov fp, r0
0036ad78: ldr r0, [sp, #0x94]
0036ad7c: bl #0x30e3ac
0036ad80: ldr r1, [sp, #0x84]
0036ad84: str r0, [sp, #0x24]
0036ad88: ldr r0, [sp, #0x90]
0036ad8c: bl #0x30e3ac
0036ad90: mov r1, fp
0036ad94: str r0, [sp, #0x28]
0036ad98: ldr r0, [sp, #0x18]
0036ad9c: bl #0x30ed6c
0036ada0: ldr r1, [sp, #0x24]
0036ada4: mov r3, r0
0036ada8: ldr r0, [sp, #0x1c]
0036adac: str r3, [sp, #0x14]
0036adb0: bl #0x30ed6c
0036adb4: ldr r3, [sp, #0x14]
0036adb8: mov r1, r0
0036adbc: mov r0, r3
0036adc0: bl #0x30eba4
0036adc4: ldr r1, [sp, #0x28]
0036adc8: mov r3, r0
0036adcc: ldr r0, [sp, #0x20]
0036add0: str r3, [sp, #0x14]
0036add4: bl #0x30ed6c
0036add8: ldr r3, [sp, #0x14]
0036addc: mov r1, r0
0036ade0: mov r0, r3
0036ade4: bl #0x30eba4
0036ade8: mov r1, fp
0036adec: mov r2, r0
0036adf0: ldr r0, [sp, #0x2c]
0036adf4: str r2, [sp, #0x10]
0036adf8: bl #0x30ed6c
0036adfc: ldr r1, [sp, #0x24]
0036ae00: mov r3, r0
0036ae04: ldr r0, [sp, #0x30]
0036ae08: str r3, [sp, #0x14]
0036ae0c: bl #0x30ed6c
0036ae10: ldr r3, [sp, #0x14]
0036ae14: mov r1, r0
0036ae18: mov r0, r3
0036ae1c: bl #0x30eba4
0036ae20: ldr r1, [sp, #0x28]
0036ae24: mov r3, r0
0036ae28: ldr r0, [sp, #0x34]
0036ae2c: str r3, [sp, #0x14]
0036ae30: bl #0x30ed6c
0036ae34: ldr r3, [sp, #0x14]
0036ae38: mov r1, r0
0036ae3c: mov r0, r3
0036ae40: bl #0x30eba4
0036ae44: mov r1, fp
0036ae48: mov r3, r0
0036ae4c: mov r0, r7
0036ae50: str r3, [sp, #0x14]
0036ae54: bl #0x30ed6c
0036ae58: ldr r1, [sp, #0x24]
0036ae5c: mov r7, r0
0036ae60: mov r0, sl
0036ae64: bl #0x30ed6c
0036ae68: mov r1, r0
0036ae6c: mov r0, r7
0036ae70: bl #0x30eba4
0036ae74: ldr r1, [sp, #0x28]
0036ae78: mov r7, r0
0036ae7c: mov r0, sb
0036ae80: bl #0x30ed6c
0036ae84: mov r1, r0
0036ae88: mov r0, r7
0036ae8c: bl #0x30eba4
0036ae90: ldr r1, [r4]
0036ae94: ldr r2, [sp, #0x10]
0036ae98: str r0, [sp]
0036ae9c: ldr r3, [sp, #0x14]
0036aea0: mov r0, r1
0036aea4: mov r1, r6
0036aea8: bl #0x861e48
0036aeac: b #0x36a934
0036aeb0: rsbeq sl, r2, r0, asr #5
0036aeb4: andeq r3, r0, r0, lsr fp
0036aeb8: rsbeq r7, r3, r8, lsl sl

# 0x3696dc _ZN15VoxSoundManager14IsSoundPlayingEi
003696dc: push {r4, r5, r6, r7, r8, sb, lr}
003696e0: ldr r3, [pc, #0x110]
003696e4: cmp r1, #0
003696e8: sub sp, sp, #0x194
003696ec: mov r6, r0
003696f0: add r3, pc, r3
003696f4: blt #0x3697d4
003696f8: ldr r2, [pc, #0xfc]
003696fc: ldr r2, [r3, r2]
00369700: ldrb r4, [r2]
00369704: cmp r4, #0
00369708: bne #0x3697e4
0036970c: ldr r2, [r0, #8]
00369710: ldr r0, [r2, r1, lsl #2]
00369714: cmp r0, #0
00369718: beq #0x3697d4
0036971c: ldr r0, [pc, #0xdc]
00369720: mov r5, sp
00369724: add ip, sp, #0x1b8
00369728: ldr r0, [r3, r0]
0036972c: mvn r8, #0
00369730: add r3, sp, #0x28
00369734: add r0, r0, #8
00369738: mvn sb, #0
0036973c: strd r8, sb, [r3, #-0x20]
00369740: str r4, [r3, #-0x18]
00369744: str r4, [r3, #-0x14]
00369748: str r4, [r3, #-0x10]
0036974c: str r4, [r3, #-0xc]
00369750: str r4, [r3, #-8]
00369754: str r0, [r3, #-0x28]
00369758: add r3, r3, #0x28
0036975c: cmp r3, ip
00369760: bne #0x36973c
00369764: ldr r1, [r2, r1, lsl #2]
00369768: ldr r0, [r6]
0036976c: mov r2, sp
00369770: mov r3, #0xa
00369774: bl #0x862548
00369778: subs r8, r0, #0
0036977c: ble #0x3697dc
00369780: mov r7, #0x28
00369784: b #0x369790
00369788: cmp r4, r8
0036978c: beq #0x3697dc
00369790: mla r1, r7, r4, r5
00369794: ldr r0, [r6]
00369798: bl #0x861f38
0036979c: cmp r0, #0
003697a0: add r4, r4, #1
003697a4: beq #0x369788
003697a8: mov r6, #1
003697ac: add r4, sp, #0x190
003697b0: ldr r3, [r4, #-0x28]!
003697b4: mov r0, r4
003697b8: mov lr, pc
003697bc: ldr pc, [r3]
003697c0: cmp r4, r5
003697c4: bne #0x3697b0
003697c8: mov r0, r6
003697cc: add sp, sp, #0x194
003697d0: pop {r4, r5, r6, r7, r8, sb, pc}
003697d4: mov r6, #0
003697d8: b #0x3697c8
003697dc: mov r6, #0
003697e0: b #0x3697ac
003697e4: mov r0, r1
003697e8: bl #0x531a64
003697ec: subs r6, r0, #0
003697f0: movne r6, #1
003697f4: b #0x3697c8
003697f8: rsbeq fp, r2, r0, lsr #7
003697fc: andeq r3, r0, r0, lsr fp
00369800: andeq r2, r0, r8, lsr #28

# 0x369f1c _ZN15VoxSoundManager15SetMasterVolumeEf
00369f1c: ldr r3, [pc, #0x24]
00369f20: ldr r2, [pc, #0x24]
00369f24: add r3, pc, r3
00369f28: ldr r2, [r3, r2]
00369f2c: ldrb r3, [r2]
00369f30: cmp r3, #0
00369f34: bxne lr
00369f38: ldr r0, [r0]
00369f3c: movw r2, #0xcccd
00369f40: movt r2, #0x3d4c
00369f44: b #0x8617c0
00369f48: rsbeq sl, r2, ip, ror #22
00369f4c: andeq r3, r0, r0, lsr fp

# 0x36c2e4 _ZN15VoxSoundManager10InitializeEv
0036c2e4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036c2e8: ldr sb, [pc, #0x3fc]
0036c2ec: ldr fp, [pc, #0x3fc]
0036c2f0: mov r5, r0
0036c2f4: add sb, pc, sb
0036c2f8: ldr r3, [sb, fp]
0036c2fc: ldr r0, [pc, #0x3f0]
0036c300: sub sp, sp, #0x7c
0036c304: ldr r3, [r3]
0036c308: add r0, pc, r0
0036c30c: add r4, sp, #0x5c
0036c310: str r3, [sp, #0x74]
0036c314: bl #0x324114
0036c318: ldr r1, [pc, #0x3d8]
0036c31c: mov r3, #0
0036c320: add r6, r5, #0x38
0036c324: strb r3, [r5, #0x33]
0036c328: add r1, pc, r1
0036c32c: mov r0, r4
0036c330: add r2, sp, #0x40
0036c334: bl #0x3140ec
0036c338: cmp r6, r4
0036c33c: beq #0x36c350
0036c340: mov r0, r6
0036c344: ldr r1, [sp, #0x70]
0036c348: ldr r2, [sp, #0x6c]
0036c34c: bl #0x3109e0
0036c350: mov r0, r4
0036c354: bl #0x3139ac
0036c358: ldr r0, [pc, #0x39c]
0036c35c: add r0, pc, r0
0036c360: bl #0x381744
0036c364: ldr r3, [pc, #0x394]
0036c368: ldr r1, [pc, #0x394]
0036c36c: cmp r0, #0
0036c370: ldr r4, [sb, r3]
0036c374: strbeq r0, [r5, #0x33]
0036c378: add r1, pc, r1
0036c37c: mov r0, r4
0036c380: bl #0x320e44
0036c384: bl #0x30e964
0036c388: mov r1, #1
0036c38c: mov r2, r0
0036c390: mov r0, r5
0036c394: bl #0x369da0
0036c398: ldr r1, [pc, #0x368]
0036c39c: mov r0, r4
0036c3a0: add r1, pc, r1
0036c3a4: bl #0x320e44
0036c3a8: bl #0x30e964
0036c3ac: mov r1, #2
0036c3b0: mov r2, r0
0036c3b4: mov r0, r5
0036c3b8: bl #0x369da0
0036c3bc: ldr r1, [pc, #0x348]
0036c3c0: ldr r0, [r5]
0036c3c4: add r1, pc, r1
0036c3c8: bl #0x861a08
0036c3cc: mov r1, #2
0036c3d0: mov r2, #4
0036c3d4: ldr r0, [r5]
0036c3d8: bl #0x861b38
0036c3dc: ldr r3, [r5, #0x7c]
0036c3e0: ldr r2, [r5, #0x80]
0036c3e4: rsb r2, r3, r2
0036c3e8: asr r3, r2, #3
0036c3ec: add r1, r3, r3, lsl #1
0036c3f0: add r1, r1, r1, lsl #4
0036c3f4: add r1, r1, r1, lsl #8
0036c3f8: add r1, r1, r1, lsl #16
0036c3fc: add r1, r3, r1, lsl #2
0036c400: cmp r1, #8
0036c404: bgt #0x36c5bc
0036c408: cmp r2, #0x4f
0036c40c: ble #0x36c488
0036c410: add r7, r5, #0x64
0036c414: mov r4, #1
0036c418: add r6, sp, #0x38
0036c41c: add r8, sp, #0x34
0036c420: add sl, sp, #0x30
0036c424: mov r1, r4
0036c428: mov r2, r6
0036c42c: mov r3, r8
0036c430: mov r0, r7
0036c434: str sl, [sp]
0036c438: bl #0x889498
0036c43c: ldr ip, [sp, #0x30]
0036c440: ldr r0, [r5]
0036c444: ldr r2, [sp, #0x38]
0036c448: ldr r3, [sp, #0x34]
0036c44c: mov r1, r4
0036c450: str ip, [sp]
0036c454: bl #0x862858
0036c458: ldr r2, [r5, #0x80]
0036c45c: ldr r3, [r5, #0x7c]
0036c460: add r4, r4, #1
0036c464: rsb r3, r3, r2
0036c468: asr r3, r3, #3
0036c46c: add r2, r3, r3, lsl #1
0036c470: add r2, r2, r2, lsl #4
0036c474: add r2, r2, r2, lsl #8
0036c478: add r2, r2, r2, lsl #16
0036c47c: add r3, r3, r2, lsl #2
0036c480: cmp r4, r3
0036c484: blt #0x36c424
0036c488: ldr r3, [pc, #0x280]
0036c48c: ldr r3, [sb, r3]
0036c490: ldrb r3, [r3]
0036c494: cmp r3, #0
0036c498: beq #0x36c4c4
0036c49c: ldr r0, [pc, #0x270]
0036c4a0: add r0, pc, r0
0036c4a4: bl #0x324114
0036c4a8: ldr r3, [sb, fp]
0036c4ac: ldr r2, [sp, #0x74]
0036c4b0: ldr r3, [r3]
0036c4b4: cmp r2, r3
0036c4b8: bne #0x36c6e8
0036c4bc: add sp, sp, #0x7c
0036c4c0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036c4c4: bl #0x38174c
0036c4c8: cmp r0, #0
0036c4cc: beq #0x36c5dc
0036c4d0: ldr r4, [pc, #0x240]
0036c4d4: add r8, r5, #0x64
0036c4d8: mov r0, r8
0036c4dc: add r4, pc, r4
0036c4e0: mov r1, r4
0036c4e4: bl #0x88a490
0036c4e8: subs sl, r0, #0
0036c4ec: ble #0x36c49c
0036c4f0: ldr r2, [pc, #0x224]
0036c4f4: ldr r3, [pc, #0x224]
0036c4f8: str fp, [sp, #0x1c]
0036c4fc: str r2, [sp, #0xc]
0036c500: add r2, r5, #0x14
0036c504: str r2, [sp, #0x20]
0036c508: add r2, sp, #0x2c
0036c50c: str r2, [sp, #0x10]
0036c510: add r2, sp, #0x3c
0036c514: add r3, pc, r3
0036c518: str r2, [sp, #0x14]
0036c51c: add r2, sp, #0x28
0036c520: str r4, [sp, #0x18]
0036c524: mov r7, #0
0036c528: add r6, sp, #0x44
0036c52c: str r2, [sp, #0x24]
0036c530: mov fp, r3
0036c534: ldr r1, [sp, #0x18]
0036c538: ldr r2, [sp, #0x10]
0036c53c: mov r0, r8
0036c540: bl #0x88bdac
0036c544: ldr r3, [sp, #0xc]
0036c548: ldr r4, [sb, r3]
0036c54c: mov r0, r4
0036c550: bl #0x337888
0036c554: ldr r2, [sp, #0x14]
0036c558: mov r1, fp
0036c55c: mov r0, r6
0036c560: bl #0x3140ec
0036c564: mov r0, r4
0036c568: mov r1, r6
0036c56c: bl #0x337a88
0036c570: mov r0, r6
0036c574: bl #0x3139ac
0036c578: ldr r4, [r5, #0x10]
0036c57c: ldr r3, [r5, #0x14]
0036c580: cmp r4, r3
0036c584: beq #0x36c61c
0036c588: ldr r3, [sp, #0x2c]
0036c58c: str r3, [r4]
0036c590: ldr r3, [r5, #0x10]
0036c594: add r3, r3, #4
0036c598: str r3, [r5, #0x10]
0036c59c: add r7, r7, #1
0036c5a0: mov r0, r5
0036c5a4: ldr r1, [sp, #0x2c]
0036c5a8: bl #0x3699fc
0036c5ac: cmp r7, sl
0036c5b0: bne #0x36c534
0036c5b4: ldr fp, [sp, #0x1c]
0036c5b8: b #0x36c49c
0036c5bc: ldr r0, [pc, #0x160]
0036c5c0: mov r2, #8
0036c5c4: add r0, pc, r0
0036c5c8: bl #0x30de84
0036c5cc: ldr r2, [r5, #0x80]
0036c5d0: ldr r3, [r5, #0x7c]
0036c5d4: rsb r2, r3, r2
0036c5d8: b #0x36c408
0036c5dc: ldr r3, [pc, #0x144]
0036c5e0: ldr r3, [sb, r3]
0036c5e4: ldrb r3, [r3]
0036c5e8: cmp r3, #0
0036c5ec: bne #0x36c4d0
0036c5f0: ldr r3, [pc, #0x134]
0036c5f4: ldr r3, [sb, r3]
0036c5f8: ldrb r3, [r3]
0036c5fc: cmp r3, #0
0036c600: bne #0x36c4d0
0036c604: ldr r3, [pc, #0x124]
0036c608: ldr r3, [sb, r3]
0036c60c: ldrb r3, [r3]
0036c610: cmp r3, #0
0036c614: beq #0x36c49c
0036c618: b #0x36c4d0
0036c61c: ldr r2, [r5, #0xc]
0036c620: rsb r2, r2, r4
0036c624: asr r2, r2, #2
0036c628: cmp r2, #1
0036c62c: addhs r3, r2, r2
0036c630: addlo r3, r2, #1
0036c634: cmn r3, #0xc0000001
0036c638: bhi #0x36c6b8
0036c63c: cmp r2, r3
0036c640: bhi #0x36c6b8
0036c644: mov r1, r3
0036c648: ldr r0, [sp, #0x20]
0036c64c: ldr r2, [sp, #0x24]
0036c650: str r3, [sp, #0x28]
0036c654: bl #0x35fd5c
0036c658: ldr r1, [r5, #0xc]
0036c65c: mov ip, r0
0036c660: subs r4, r4, r1
0036c664: moveq r4, r0
0036c668: bne #0x36c6d0
0036c66c: ldr r3, [sp, #0x2c]
0036c670: str r3, [r4], #4
0036c674: ldr r0, [r5, #0xc]
0036c678: ldr r3, [r5, #0x14]
0036c67c: cmp r0, #0
0036c680: beq #0x36c6a0
0036c684: rsb r3, r0, r3
0036c688: bic r1, r3, #3
0036c68c: cmp r1, #0x80
0036c690: bhi #0x36c6c0
0036c694: str ip, [sp, #8]
0036c698: bl #0x708f00
0036c69c: ldr ip, [sp, #8]
0036c6a0: ldr r3, [sp, #0x28]
0036c6a4: str ip, [r5, #0xc]
0036c6a8: str r4, [r5, #0x10]
0036c6ac: add r3, ip, r3, lsl #2
0036c6b0: str r3, [r5, #0x14]
0036c6b4: b #0x36c59c
0036c6b8: mvn r3, #0xc0000000
0036c6bc: b #0x36c644
0036c6c0: str ip, [sp, #8]
0036c6c4: bl #0x310440
0036c6c8: ldr ip, [sp, #8]
0036c6cc: b #0x36c6a0
0036c6d0: mov r2, r4
0036c6d4: str r0, [sp, #8]
0036c6d8: bl #0x30df38
0036c6dc: ldr ip, [sp, #8]
0036c6e0: add r4, r0, r4
0036c6e4: b #0x36c66c
0036c6e8: bl #0x30e310
0036c6ec: mlseq r2, ip, r7, r8
0036c6f0: andeq r4, r0, ip, lsr #1
0036c6f4: subseq r4, r5, r0, ror #28
0036c6f8: subseq pc, r5, r0, ror #9
0036c6fc: subseq r4, r5, ip, asr #28
0036c700: strdeq r3, r4, [r0], -r4
0036c704: subseq r4, r5, r8, asr ip
0036c708: subseq r4, r5, r0, lsl lr
0036c70c: ldrsheq r4, [r5], #-0xdc
0036c710: andeq r3, r0, r0, lsr fp
0036c714: subseq r4, r5, r0, ror #27

# 0x36cad0 _ZN15VoxSoundManagerC2Ev
0036cad0: push {r4, r5, r6, r7, r8, sl, lr}
0036cad4: ldr r5, [pc, #0x294]
0036cad8: ldr r6, [pc, #0x294]
0036cadc: mov r4, r0
0036cae0: add r5, pc, r5
0036cae4: ldr r3, [r5, r6]
0036cae8: mov r7, #0
0036caec: add r2, r0, #0x38
0036caf0: ldr r3, [r3]
0036caf4: mvn r1, #0
0036caf8: mov r8, #1
0036cafc: str r1, [r0, #0x2c]
0036cb00: sub sp, sp, #0x214
0036cb04: mov r0, r2
0036cb08: str r1, [r4, #0x24]
0036cb0c: str r1, [r4, #0x28]
0036cb10: str r2, [r4, #0x48]
0036cb14: str r2, [r4, #0x4c]
0036cb18: str r7, [r4]
0036cb1c: str r7, [r4, #0xc]
0036cb20: str r7, [r4, #0x10]
0036cb24: str r7, [r4, #0x14]
0036cb28: strb r8, [r4, #0x18]
0036cb2c: str r7, [r4, #0x1c]
0036cb30: strb r7, [r4, #0x20]
0036cb34: strb r8, [r4, #0x30]
0036cb38: strb r8, [r4, #0x31]
0036cb3c: strb r7, [r4, #0x32]
0036cb40: strb r7, [r4, #0x33]
0036cb44: mov r1, #0x10
0036cb48: str r3, [sp, #0x20c]
0036cb4c: bl #0x31167c
0036cb50: ldr r1, [r4, #0x48]
0036cb54: mov r2, r4
0036cb58: mov r3, #0x3e8
0036cb5c: strb r7, [r1]
0036cb60: ldr r0, [pc, #0x210]
0036cb64: mov r1, #0x3f800000
0036cb68: str r3, [r4, #0x58]
0036cb6c: str r3, [r4, #0x54]
0036cb70: str r1, [r4, #0x5c]
0036cb74: mov r3, r4
0036cb78: strb r7, [r4, #0x60]
0036cb7c: str r7, [r4, #0x64]
0036cb80: str r7, [r4, #0x68]
0036cb84: str r7, [r4, #0x6c]
0036cb88: str r7, [r4, #0x70]
0036cb8c: str r7, [r4, #0x74]
0036cb90: str r7, [r4, #0x78]
0036cb94: str r7, [r4, #0x7c]
0036cb98: str r7, [r4, #0x80]
0036cb9c: str r7, [r4, #0x84]
0036cba0: str r7, [r4, #0x88]
0036cba4: str r7, [r4, #0x8c]
0036cba8: str r7, [r4, #0x90]
0036cbac: str r7, [r4, #0x98]
0036cbb0: strb r7, [r2, #0x94]!
0036cbb4: str r2, [r4, #0xa0]
0036cbb8: str r2, [r4, #0x9c]
0036cbbc: str r7, [r4, #0xa4]
0036cbc0: str r7, [r4, #0xb0]
0036cbc4: strb r7, [r3, #0xac]!
0036cbc8: str r3, [r4, #0xb8]
0036cbcc: str r3, [r4, #0xb4]
0036cbd0: str r7, [r4, #0xbc]
0036cbd4: add r0, pc, r0
0036cbd8: bl #0x324114
0036cbdc: ldr r3, [pc, #0x198]
0036cbe0: ldr r3, [r5, r3]
0036cbe4: ldrb r3, [r3, #0xa8]
0036cbe8: cmp r3, r7
0036cbec: bne #0x36cd34
0036cbf0: ldr r3, [pc, #0x188]
0036cbf4: add r7, sp, #0xc
0036cbf8: mov r0, r7
0036cbfc: ldr r3, [r5, r3]
0036cc00: ldr r1, [r3]
0036cc04: bl #0x30e520
0036cc08: mov r0, r7
0036cc0c: bl #0x30de54
0036cc10: ldr r1, [pc, #0x16c]
0036cc14: mov r2, #0xd
0036cc18: add r0, r7, r0
0036cc1c: add r1, pc, r1
0036cc20: bl #0x30e868
0036cc24: mov r0, r7
0036cc28: bl #0x30de54
0036cc2c: ldr r1, [pc, #0x154]
0036cc30: mov r2, #0xb
0036cc34: add r0, r7, r0
0036cc38: add r1, pc, r1
0036cc3c: bl #0x30e868
0036cc40: mov r1, r7
0036cc44: add r0, r4, #0x64
0036cc48: bl #0x88d344
0036cc4c: ldr r0, [pc, #0x138]
0036cc50: add r0, pc, r0
0036cc54: bl #0x324114
0036cc58: ldr r2, [r4, #0x68]
0036cc5c: ldr r3, [r4, #0x64]
0036cc60: mov r1, #4
0036cc64: rsb r3, r3, r2
0036cc68: asr r3, r3, #2
0036cc6c: lsl r2, r3, r1
0036cc70: rsb r2, r3, r2
0036cc74: add r2, r2, r2, lsl #8
0036cc78: add r2, r2, r2, lsl #16
0036cc7c: add r3, r3, r2, lsl #4
0036cc80: str r3, [r4, #0x1c]
0036cc84: lsl r0, r3, #2
0036cc88: bl #0x31056c
0036cc8c: ldr r2, [r4, #0x1c]
0036cc90: mov r1, #0
0036cc94: str r0, [r4, #4]
0036cc98: lsl r2, r2, #2
0036cc9c: bl #0x30e460
0036cca0: ldr r0, [pc, #0xe8]
0036cca4: add r0, pc, r0
0036cca8: bl #0x324114
0036ccac: ldr r0, [r4, #0x1c]
0036ccb0: mov r1, #4
0036ccb4: lsl r0, r0, #2
0036ccb8: bl #0x31056c
0036ccbc: ldr r2, [r4, #0x1c]
0036ccc0: mov r1, #0
0036ccc4: str r0, [r4, #8]
0036ccc8: lsl r2, r2, #2
0036cccc: bl #0x30e460
0036ccd0: ldr r0, [pc, #0xbc]
0036ccd4: add r0, pc, r0
0036ccd8: bl #0x324114
0036ccdc: ldr r0, [pc, #0xb4]
0036cce0: add r0, pc, r0
0036cce4: bl #0x324114
0036cce8: bl #0x862b30
0036ccec: str r0, [r4]
0036ccf0: ldr r3, [r0]
0036ccf4: mov lr, pc
0036ccf8: ldr pc, [r3, #8]
0036ccfc: ldr r0, [pc, #0x98]
0036cd00: add r0, pc, r0
0036cd04: bl #0x324114
0036cd08: ldr r0, [pc, #0x90]
0036cd0c: add r0, pc, r0
0036cd10: bl #0x324114
0036cd14: ldr r3, [r5, r6]
0036cd18: ldr r2, [sp, #0x20c]
0036cd1c: mov r0, r4
0036cd20: ldr r3, [r3]
0036cd24: cmp r2, r3
0036cd28: bne #0x36cd6c
0036cd2c: add sp, sp, #0x214
0036cd30: pop {r4, r5, r6, r7, r8, sl, pc}
0036cd34: bl #0x8945a4
0036cd38: ldr r3, [r0]
0036cd3c: mov sl, r0
0036cd40: ldr r0, [pc, #0x5c]
0036cd44: ldr r7, [r3, #0x10]
0036cd48: add r0, pc, r0
0036cd4c: bl #0x56e064
0036cd50: mov r2, r8
0036cd54: mov r1, r0
0036cd58: str r8, [sp]
0036cd5c: mov r0, sl
0036cd60: mov r3, r8
0036cd64: blx r7
0036cd68: b #0x36cbf0
0036cd6c: bl #0x30e310
0036cd70: strhteq r7, [r2], #-0xf0
0036cd74: andeq r4, r0, ip, lsr #1
0036cd78: ldrsheq r4, [r5], #-0x6c
0036cd7c: strdeq r3, r4, [r0], -r4
0036cd80: andeq r0, r0, r0, lsl #12
0036cd84: subseq r4, r5, r4, asr r3
0036cd88: subseq r4, r5, r0, lsl r7
0036cd8c: subseq r4, r5, r8, lsl #14
0036cd90: subseq r4, r5, r4, lsr #14
0036cd94: subseq r4, r5, r4, ror #14
0036cd98: subseq r4, r5, r8, asr #15
0036cd9c: subseq r4, r5, r8, lsl r8
0036cda0: subseq r4, r5, r4, ror r8
0036cda4: subseq r4, r5, r8, ror #11

# 0x36b088 _ZN15VoxSoundManagerD2Ev
0036b088: ldr r3, [pc, #0x88]
0036b08c: ldr r2, [pc, #0x88]
0036b090: push {r4, lr}
0036b094: add r3, pc, r3
0036b098: ldr r2, [r3, r2]
0036b09c: mov r4, r0
0036b0a0: ldrb r3, [r2]
0036b0a4: cmp r3, #0
0036b0a8: beq #0x36b0ec
0036b0ac: add r0, r4, #0x64
0036b0b0: bl #0x88c2cc
0036b0b4: add r0, r4, #0x38
0036b0b8: bl #0x3139ac
0036b0bc: ldr r0, [r4, #0xc]
0036b0c0: add r3, r4, #0xc
0036b0c4: cmp r0, #0
0036b0c8: beq #0x36b0e4
0036b0cc: ldr r1, [r3, #8]
0036b0d0: rsb r1, r0, r1
0036b0d4: bic r1, r1, #3
0036b0d8: cmp r1, #0x80
0036b0dc: bhi #0x36b10c
0036b0e0: bl #0x708f00
0036b0e4: mov r0, r4
0036b0e8: pop {r4, pc}
0036b0ec: ldr r0, [r0, #4]
0036b0f0: bl #0x310440
0036b0f4: mov r0, r4
0036b0f8: bl #0x369298
0036b0fc: ldr r0, [r4, #8]
0036b100: bl #0x310440
0036b104: bl #0x8628f8
0036b108: b #0x36b0ac
0036b10c: bl #0x310440
0036b110: mov r0, r4
0036b114: pop {r4, pc}

# 0x36c224 _ZN15VoxSoundManager12RestartMusicEi
0036c224: push {r4, r5, r6, r7, r8, sl, lr}
0036c228: ldr r4, [pc, #0xa4]
0036c22c: ldr r5, [pc, #0xa4]
0036c230: ldr r2, [pc, #0xa4]
0036c234: add r4, pc, r4
0036c238: ldr r3, [r4, r5]
0036c23c: ldr r8, [r4, r2]
0036c240: sub sp, sp, #0x2c
0036c244: ldr r3, [r3]
0036c248: mov sl, r0
0036c24c: mov r0, r8
0036c250: str r3, [sp, #0x24]
0036c254: mov r6, r1
0036c258: bl #0x337888
0036c25c: ldr r1, [pc, #0x7c]
0036c260: add r7, sp, #0xc
0036c264: add r2, sp, #8
0036c268: add r1, pc, r1
0036c26c: mov r0, r7
0036c270: bl #0x3140ec
0036c274: mov r0, r8
0036c278: mov r1, r7
0036c27c: bl #0x337a88
0036c280: mov r8, r0
0036c284: mov r0, r7
0036c288: bl #0x3139ac
0036c28c: cmp r8, #0
0036c290: bne #0x36c2b4
0036c294: ldr r1, [sl, #0x28]
0036c298: cmn r1, #1
0036c29c: beq #0x36c2b4
0036c2a0: ldrb r2, [sl, #0x30]
0036c2a4: mov r0, sl
0036c2a8: mov r3, #1
0036c2ac: str r6, [sp]
0036c2b0: bl #0x36bd78
0036c2b4: ldr r3, [r4, r5]
0036c2b8: ldr r2, [sp, #0x24]
0036c2bc: ldr r3, [r3]
0036c2c0: cmp r2, r3
0036c2c4: bne #0x36c2d0
0036c2c8: add sp, sp, #0x2c
0036c2cc: pop {r4, r5, r6, r7, r8, sl, pc}
0036c2d0: bl #0x30e310
0036c2d4: rsbeq r8, r2, ip, asr r8
0036c2d8: andeq r4, r0, ip, lsr #1
0036c2dc: andeq r0, r0, r4, lsl #17
0036c2e0: subseq r4, r5, r0, lsl #29

# 0x369480 _ZN15VoxSoundManager20GetIPodPlaylistCountEv
00369480: b #0x533a10

# 0x36a138 _ZN15VoxSoundManager8StopBeatEv
0036a138: push {r4, r5, r6, lr}
0036a13c: ldr r5, [r0, #0x2c]
0036a140: ldr r3, [pc, #0x48]
0036a144: mov r4, r0
0036a148: cmn r5, #1
0036a14c: add r3, pc, r3
0036a150: beq #0x36a18c
0036a154: ldr r2, [pc, #0x38]
0036a158: ldr r1, [pc, #0x38]
0036a15c: ldr r3, [r3, r2]
0036a160: ldr r2, [pc, #0x34]
0036a164: add r1, pc, r1
0036a168: ldr r0, [r3, #0x2c]
0036a16c: add r2, pc, r2
0036a170: bl #0x4c4bdc
0036a174: mov r1, r5
0036a178: mov r2, r0
0036a17c: mov r0, r4
0036a180: bl #0x369fec
0036a184: mvn r3, #0
0036a188: str r3, [r4, #0x2c]
0036a18c: pop {r4, r5, r6, pc}
0036a190: rsbeq sl, r2, r4, asr #18
0036a194: strdeq r3, r4, [r0], -r4
0036a198: subseq r6, r5, r4, lsr #28
0036a19c: subseq r6, r5, ip, lsr #28

# 0x36c014 _ZN15VoxSoundManager18SetInSafeZoneMusicEb
0036c014: push {r4, r5, r6, r7, lr}
0036c018: ldr r6, [pc, #0x124]
0036c01c: ldr r3, [pc, #0x124]
0036c020: sub sp, sp, #0xc
0036c024: add r6, pc, r6
0036c028: ldr r3, [r6, r3]
0036c02c: mov r4, r0
0036c030: mov r7, r1
0036c034: ldrb r5, [r3]
0036c038: cmp r5, #0
0036c03c: bne #0x36c0c0
0036c040: cmp r1, #0
0036c044: beq #0x36c08c
0036c048: ldr r3, [pc, #0xfc]
0036c04c: ldr r0, [r6, r3]
0036c050: bl #0x31f594
0036c054: ldr r1, [r0, #0x120]
0036c058: cmp r1, #0
0036c05c: blt #0x36c0f0
0036c060: ldrb r3, [r4, #0x31]
0036c064: mov r2, #1
0036c068: strb r2, [r4, #0x32]
0036c06c: cmp r3, #0
0036c070: beq #0x36c0b8
0036c074: mov ip, #0x7d0
0036c078: mov r0, r4
0036c07c: mov r3, r5
0036c080: str ip, [sp]
0036c084: bl #0x36bd78
0036c088: b #0x36c0b8
0036c08c: ldr r3, [pc, #0xb8]
0036c090: strb r1, [r4, #0x32]
0036c094: ldr r0, [r6, r3]
0036c098: bl #0x31f594
0036c09c: mov ip, #0x7d0
0036c0a0: ldr r1, [r0, #0x11c]
0036c0a4: mov r3, r7
0036c0a8: mov r0, r4
0036c0ac: mov r2, #1
0036c0b0: str ip, [sp]
0036c0b4: bl #0x36bd78
0036c0b8: add sp, sp, #0xc
0036c0bc: pop {r4, r5, r6, r7, pc}
0036c0c0: ldr r3, [pc, #0x84]
0036c0c4: strb r1, [r4, #0x32]
0036c0c8: ldr r0, [r6, r3]
0036c0cc: bl #0x31f594
0036c0d0: mov ip, #0x7d0
0036c0d4: ldr r1, [r0, #0x11c]
0036c0d8: mov r2, #1
0036c0dc: mov r0, r4
0036c0e0: mov r3, #0
0036c0e4: str ip, [sp]
0036c0e8: bl #0x36bd78
0036c0ec: b #0x36c0b8
0036c0f0: ldr r3, [pc, #0x58]
0036c0f4: ldr r3, [r6, r3]
0036c0f8: ldr r3, [r3]
0036c0fc: cmp r3, #2
0036c100: streq r5, [r5]
0036c104: beq #0x36c0b8
0036c108: cmp r3, #1
0036c10c: bne #0x36c0b8
0036c110: ldr r0, [pc, #0x3c]
0036c114: ldr r1, [pc, #0x3c]
0036c118: ldr r2, [pc, #0x3c]
0036c11c: ldr r0, [r6, r0]
0036c120: ldr r3, [pc, #0x38]
0036c124: mov ip, #0x5a0
0036c128: add r1, pc, r1
0036c12c: add r2, pc, r2
0036c130: add r3, pc, r3
0036c134: add r0, r0, #0xa8
0036c138: str ip, [sp]
0036c13c: bl #0x30e004
0036c140: b #0x36c0b8
0036c144: rsbeq r8, r2, ip, ror #20
0036c148: andeq r3, r0, r0, lsr fp
0036c14c: strdeq r3, r4, [r0], -r4
0036c150: andeq r3, r0, r0, asr #19
0036c154: andeq r1, r0, r0, asr #19
0036c158: ldrheq r2, [r5], #-0x20
0036c15c: subseq r2, r5, ip, lsr r4
0036c160: subseq r4, r5, r0, ror #31

# 0x369fec _ZN15VoxSoundManager4StopEii
00369fec: push {r4, r5, r6, r7, r8, sl, lr}
00369ff0: ldr r8, [pc, #0x130]
00369ff4: cmp r1, #0
00369ff8: sub sp, sp, #0x194
00369ffc: mov r6, r0
0036a000: add r8, pc, r8
0036a004: mov r5, r2
0036a008: blt #0x36a114
0036a00c: ldr r3, [pc, #0x118]
0036a010: ldr r3, [r8, r3]
0036a014: ldrb r4, [r3]
0036a018: cmp r4, #0
0036a01c: bne #0x36a11c
0036a020: ldr r2, [pc, #0x108]
0036a024: ldr r3, [r0, #8]
0036a028: mov r0, #0xc
0036a02c: ldr r2, [r8, r2]
0036a030: ldr r2, [r2]
0036a034: mla r1, r0, r1, r2
0036a038: ldr sl, [r1, #4]
0036a03c: ldr r1, [r3, sl, lsl #2]
0036a040: cmp r1, #0
0036a044: beq #0x36a114
0036a048: ldr r0, [r6]
0036a04c: bl #0x8624f8
0036a050: cmp r0, #0
0036a054: beq #0x36a114
0036a058: mov r0, r5
0036a05c: bl #0x30e964
0036a060: mov r1, #0x44000000
0036a064: add r1, r1, #0x7a0000
0036a068: bl #0x30ec94
0036a06c: ldr r3, [pc, #0xc0]
0036a070: mov r7, r0
0036a074: mov r5, sp
0036a078: ldr r2, [r8, r3]
0036a07c: add ip, sp, #0x1b8
0036a080: add r3, sp, #0x28
0036a084: add r2, r2, #8
0036a088: mvn r0, #0
0036a08c: mvn r1, #0
0036a090: strd r0, r1, [r3, #-0x20]
0036a094: str r4, [r3, #-0x18]
0036a098: str r4, [r3, #-0x14]
0036a09c: str r4, [r3, #-0x10]
0036a0a0: str r4, [r3, #-0xc]
0036a0a4: str r4, [r3, #-8]
0036a0a8: str r2, [r3, #-0x28]
0036a0ac: add r3, r3, #0x28
0036a0b0: cmp r3, ip
0036a0b4: bne #0x36a090
0036a0b8: ldr r3, [r6, #8]
0036a0bc: ldr r0, [r6]
0036a0c0: mov r2, sp
0036a0c4: ldr r1, [r3, sl, lsl #2]
0036a0c8: mov r3, #0xa
0036a0cc: bl #0x862548
0036a0d0: subs r8, r0, #0
0036a0d4: ble #0x36a0f8
0036a0d8: mov sl, #0x28
0036a0dc: mla r1, sl, r4, r5
0036a0e0: ldr r0, [r6]
0036a0e4: add r4, r4, #1
0036a0e8: mov r2, r7
0036a0ec: bl #0x862198
0036a0f0: cmp r4, r8
0036a0f4: bne #0x36a0dc
0036a0f8: add r4, sp, #0x190
0036a0fc: ldr r3, [r4, #-0x28]!
0036a100: mov r0, r4
0036a104: mov lr, pc
0036a108: ldr pc, [r3]
0036a10c: cmp r4, r5
0036a110: bne #0x36a0fc
0036a114: add sp, sp, #0x194
0036a118: pop {r4, r5, r6, r7, r8, sl, pc}
0036a11c: mov r0, r1
0036a120: bl #0x531570
0036a124: b #0x36a114
0036a128: mlseq r2, r0, sl, sl
0036a12c: andeq r3, r0, r0, lsr fp
0036a130: andeq r3, r0, ip, lsr lr
0036a134: andeq r2, r0, r8, lsr #28

# 0x36aed4 _GLOBAL__I_.._.._sources_Core_VOXSoundManager_VoxSoundManager.cpp
0036aed4: push {r4, r5, r6, lr}
0036aed8: ldr r4, [pc, #0xac]
0036aedc: ldr r2, [pc, #0xac]
0036aee0: ldr r3, [pc, #0xac]
0036aee4: add r4, pc, r4
0036aee8: ldr r1, [r4, r2]
0036aeec: add r3, pc, r3
0036aef0: mov r2, #0x3f000000
0036aef4: ldr r0, [r1]
0036aef8: str r2, [r3, #0x18]
0036aefc: str r2, [r3, #0x10]
0036af00: tst r0, #1
0036af04: str r2, [r3, #0x14]
0036af08: beq #0x36af58
0036af0c: ldr r3, [pc, #0x84]
0036af10: ldr r3, [r4, r3]
0036af14: ldr r2, [r3]
0036af18: tst r2, #1
0036af1c: beq #0x36af24
0036af20: pop {r4, r5, r6, pc}
0036af24: mov r2, #1
0036af28: str r2, [r3]
0036af2c: ldr r3, [pc, #0x68]
0036af30: ldr r5, [r4, r3]
0036af34: mov r0, r5
0036af38: bl #0x32d79c
0036af3c: ldr r3, [pc, #0x5c]
0036af40: mov r0, r5
0036af44: ldr r1, [r4, r3]
0036af48: ldr r3, [pc, #0x54]
0036af4c: ldr r2, [r4, r3]
0036af50: pop {r4, r5, r6, lr}
0036af54: b #0x30e304
0036af58: mov r3, #1
0036af5c: str r3, [r1]
0036af60: ldr r3, [pc, #0x40]
0036af64: ldr r5, [r4, r3]
0036af68: mov r0, r5
0036af6c: bl #0x3790a8
0036af70: ldr r3, [pc, #0x34]
0036af74: mov r0, r5
0036af78: ldr r1, [r4, r3]
0036af7c: ldr r3, [pc, #0x20]
0036af80: ldr r2, [r4, r3]
0036af84: bl #0x30e304
0036af88: b #0x36af0c
0036af8c: rsbeq sb, r2, ip, lsr #23
0036af90: strdeq r0, r1, [r0], -r4
0036af94: rsbeq r7, r3, r0, ror r4
0036af98: andeq r0, r0, ip, lsr #31
0036af9c: strdeq r3, r4, [r0], -r4
0036afa0: andeq r0, r0, r0, asr #17
0036afa4: muleq r0, r0, r8
0036afa8: andeq r2, r0, r4, lsl r7
0036afac: muleq r0, ip, r5
