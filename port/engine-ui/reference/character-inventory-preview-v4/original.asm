# 0x42baec _ZN12MenuMainMenu19DestroyAvatarCameraEv
0042baec: ldr r3, [pc, #0x48]
0042baf0: ldr r2, [pc, #0x48]
0042baf4: push {r4, lr}
0042baf8: add r3, pc, r3
0042bafc: ldr r4, [r3, r2]
0042bb00: ldr r3, [r4]
0042bb04: cmp r3, #0
0042bb08: beq #0x42bb38
0042bb0c: mov r0, r3
0042bb10: ldr r3, [r3]
0042bb14: mov lr, pc
0042bb18: ldr pc, [r3, #0x68]
0042bb1c: ldr r3, [r4]
0042bb20: ldr r2, [r3]
0042bb24: ldr r0, [r2, #-0xc]
0042bb28: add r0, r3, r0
0042bb2c: bl #0x31d584
0042bb30: mov r3, #0
0042bb34: str r3, [r4]
0042bb38: pop {r4, pc}

# 0x42cf30 _ZN17MenuFlash2DCamera12SetLimitClipEPN7gameswf9characterE
0042cf30: push {r4, r5, lr}
0042cf34: cmp r1, #0
0042cf38: mov r4, r0
0042cf3c: sub sp, sp, #0x14
0042cf40: str r1, [r4, #8]
0042cf44: beq #0x42cf60
0042cf48: mov r0, sp
0042cf4c: bl #0x416a7c
0042cf50: mov r5, sp
0042cf54: add ip, r4, #0xc
0042cf58: ldm r5, {r0, r1, r2, r3}
0042cf5c: stm ip, {r0, r1, r2, r3}
0042cf60: add sp, sp, #0x14
0042cf64: pop {r4, r5, pc}

# 0x42cd84 _ZN17MenuFlash2DCamera6UpdateEv
0042cd84: push {r4, r5, r6, r7, r8, lr}
0042cd88: ldr r2, [r0, #0x24]
0042cd8c: ldr r1, [r0, #0x2c]
0042cd90: ldr r3, [pc, #0x190]
0042cd94: sub sp, sp, #8
0042cd98: cmp r2, r1
0042cd9c: mov r4, r0
0042cda0: add r3, pc, r3
0042cda4: bge #0x42cefc
0042cda8: movw r0, #0x6667
0042cdac: rsb r1, r2, r1
0042cdb0: movt r0, #0x6666
0042cdb4: smull ip, r0, r0, r1
0042cdb8: asr r1, r1, #0x1f
0042cdbc: rsb r1, r1, r0, asr #2
0042cdc0: add r2, r2, #1
0042cdc4: add r2, r2, r1
0042cdc8: str r2, [r4, #0x24]
0042cdcc: ldr r2, [r4, #0x28]
0042cdd0: ldr r1, [r4, #0x30]
0042cdd4: cmp r2, r1
0042cdd8: bge #0x42ced0
0042cddc: movw r0, #0x6667
0042cde0: rsb r1, r2, r1
0042cde4: movt r0, #0x6666
0042cde8: smull ip, r0, r0, r1
0042cdec: asr r1, r1, #0x1f
0042cdf0: rsb r1, r1, r0, asr #2
0042cdf4: add r2, r2, #1
0042cdf8: add r2, r2, r1
0042cdfc: str r2, [r4, #0x28]
0042ce00: ldr r1, [pc, #0x124]
0042ce04: ldr r2, [r4, #8]
0042ce08: ldr r3, [r3, r1]
0042ce0c: cmp r2, #0
0042ce10: ldr r3, [r3, #0x10]
0042ce14: ldr r3, [r3, #0x10]
0042ce18: ldr r3, [r3, #0xcc]
0042ce1c: ldr r3, [r3, #-4]
0042ce20: ldr r5, [r3, #0x10]
0042ce24: ldr r6, [r3, #0xc]
0042ce28: beq #0x42ce94
0042ce2c: ldr r0, [r4, #0xc]
0042ce30: bl #0x30e4cc
0042ce34: ldr r8, [r4, #0x24]
0042ce38: add r3, r0, r8
0042ce3c: cmp r3, #0
0042ce40: rsbgt r8, r0, #0
0042ce44: strgt r8, [r4, #0x24]
0042ce48: ldr r0, [r4, #0x14]
0042ce4c: bl #0x30e4cc
0042ce50: ldr r7, [r4, #0x28]
0042ce54: add r3, r0, r7
0042ce58: cmp r3, #0
0042ce5c: rsbgt r7, r0, #0
0042ce60: strgt r7, [r4, #0x28]
0042ce64: ldr r0, [r4, #0x10]
0042ce68: bl #0x30e4cc
0042ce6c: add r8, r0, r8
0042ce70: cmp r6, r8
0042ce74: rsbgt r0, r0, r6
0042ce78: strgt r0, [r4, #0x24]
0042ce7c: ldr r0, [r4, #0x18]
0042ce80: bl #0x30e4cc
0042ce84: add r7, r0, r7
0042ce88: cmp r5, r7
0042ce8c: rsbgt r0, r0, r5
0042ce90: strgt r0, [r4, #0x28]
0042ce94: mov r1, #0
0042ce98: ldr r0, [r4, #4]
0042ce9c: mov r2, r1
0042cea0: mov r3, r6
0042cea4: str r5, [sp]
0042cea8: bl #0x7a9bac
0042ceac: ldr r2, [r4, #0x28]
0042ceb0: ldr r0, [r4, #4]
0042ceb4: ldr r1, [r4, #0x24]
0042ceb8: mov ip, #0
0042cebc: mov r3, r6
0042cec0: stm sp, {r5, ip}
0042cec4: bl #0x7a9b30
0042cec8: add sp, sp, #8
0042cecc: pop {r4, r5, r6, r7, r8, pc}
0042ced0: ble #0x42ce00
0042ced4: movw r0, #0x6667
0042ced8: rsb r1, r1, r2
0042cedc: movt r0, #0x6666
0042cee0: smull ip, r0, r0, r1
0042cee4: asr r1, r1, #0x1f
0042cee8: rsb r1, r1, r0, asr #2
0042ceec: mvn r1, r1
0042cef0: add r2, r1, r2
0042cef4: str r2, [r4, #0x28]
0042cef8: b #0x42ce00
0042cefc: ble #0x42cdcc
0042cf00: movw r0, #0x6667
0042cf04: rsb r1, r1, r2
0042cf08: movt r0, #0x6666
0042cf0c: smull ip, r0, r0, r1
0042cf10: asr r1, r1, #0x1f
0042cf14: rsb r1, r1, r0, asr #2
0042cf18: mvn r1, r1
0042cf1c: add r2, r1, r2
0042cf20: str r2, [r4, #0x24]
0042cf24: b #0x42cdcc
0042cf28: ldrsheq r7, [r6], #-0xc0
0042cf2c: strdeq r3, r4, [r0], -r4

# 0x42bf3c _ZN12MenuMainMenu18CreateAvatarCameraEv
0042bf3c: push {r4, r5, r6, r7, r8, lr}
0042bf40: ldr r4, [pc, #0x17c]
0042bf44: ldr r3, [pc, #0x17c]
0042bf48: sub sp, sp, #0x30
0042bf4c: add r4, pc, r4
0042bf50: ldr r5, [r4, r3]
0042bf54: ldr r6, [r5]
0042bf58: cmp r6, #0
0042bf5c: beq #0x42bf68
0042bf60: add sp, sp, #0x30
0042bf64: pop {r4, r5, r6, r7, r8, pc}
0042bf68: mov r3, #0xc4000000
0042bf6c: add r3, r3, #0x610000
0042bf70: str r3, [sp, #0x28]
0042bf74: mov r3, #0x43000000
0042bf78: add r3, r3, #0x160000
0042bf7c: str r3, [sp, #0x2c]
0042bf80: mov r3, #0x43000000
0042bf84: mov r7, #0
0042bf88: mov r1, r6
0042bf8c: add r3, r3, #0x610000
0042bf90: mov r0, #0x38c
0042bf94: str r3, [sp, #0x20]
0042bf98: str r7, [sp, #0x24]
0042bf9c: str r7, [sp, #0x18]
0042bfa0: str r7, [sp, #0x1c]
0042bfa4: bl #0x5341ac
0042bfa8: add r2, sp, #0x24
0042bfac: add r3, sp, #0x18
0042bfb0: mvn r1, #0
0042bfb4: mov r8, r0
0042bfb8: str r6, [sp]
0042bfbc: bl #0x583734
0042bfc0: ldr r3, [pc, #0x104]
0042bfc4: str r8, [r5]
0042bfc8: mov r1, r8
0042bfcc: ldr r4, [r4, r3]
0042bfd0: ldr r3, [r4, #0x10]
0042bfd4: ldr r3, [r3, #0x1c]
0042bfd8: ldr r3, [r3, #4]
0042bfdc: mov r0, r3
0042bfe0: ldr r3, [r3]
0042bfe4: mov lr, pc
0042bfe8: ldr pc, [r3, #0x5c]
0042bfec: ldr r3, [r5]
0042bff0: ldr r2, [r3]
0042bff4: ldr r0, [r2, #-0xc]
0042bff8: add r0, r3, r0
0042bffc: bl #0x31d584
0042c000: ldr r3, [r4, #0x10]
0042c004: ldr r1, [r5]
0042c008: ldr r0, [r3, #0x1c]
0042c00c: bl #0x5890c0
0042c010: ldr r0, [r5]
0042c014: mov r2, #0x3f800000
0042c018: add r1, sp, #0xc
0042c01c: ldr r3, [r0]
0042c020: ldr r3, [r3, #0x114]
0042c024: str r2, [sp, #0x14]
0042c028: str r7, [sp, #0x10]
0042c02c: str r7, [sp, #0xc]
0042c030: blx r3
0042c034: ldr r3, [r5]
0042c038: movw r1, #0x78e9
0042c03c: movt r1, #0x3fd5
0042c040: mov r0, r3
0042c044: ldr r3, [r3]
0042c048: mov lr, pc
0042c04c: ldr pc, [r3, #0x138]
0042c050: ldr r3, [r5]
0042c054: movw r1, #0x79c8
0042c058: movt r1, #0x3f35
0042c05c: mov r0, r3
0042c060: ldr r3, [r3]
0042c064: mov lr, pc
0042c068: ldr pc, [r3, #0x13c]
0042c06c: ldr r3, [r5]
0042c070: mov r1, #0x41000000
0042c074: add r1, r1, #0x200000
0042c078: ldr r2, [r3]
0042c07c: ldr r2, [r2, #-0xc]
0042c080: add r3, r3, r2
0042c084: ldr r2, [r3, #4]
0042c088: add r2, r2, #1
0042c08c: str r2, [r3, #4]
0042c090: ldr r3, [r5]
0042c094: mov r0, r3
0042c098: ldr r3, [r3]
0042c09c: mov lr, pc
0042c0a0: ldr pc, [r3, #0x130]
0042c0a4: ldr r3, [r5]
0042c0a8: mov r1, #0x44000000
0042c0ac: add r1, r1, #0xfa0000
0042c0b0: mov r0, r3
0042c0b4: ldr r3, [r3]
0042c0b8: mov lr, pc
0042c0bc: ldr pc, [r3, #0x134]
0042c0c0: b #0x42bf60
0042c0c4: subseq r8, r6, r4, asr #22
0042c0c8: strheq r4, [r0], -r0
0042c0cc: strdeq r3, r4, [r0], -r4

# 0x453488 _ZN16MenuCharMenu_Map16DestroyMapCameraEv
00453488: push {r4, r5, r6, lr}
0045348c: ldr r3, [r0, #0x1e8]
00453490: ldr r4, [pc, #0x7c]
00453494: mov r5, r0
00453498: cmp r3, #0
0045349c: add r4, pc, r4
004534a0: beq #0x4534f0
004534a4: ldr r6, [pc, #0x6c]
004534a8: ldr r3, [r4, r6]
004534ac: ldr r0, [r3, #0x50]
004534b0: bl #0x3830bc
004534b4: ldr r3, [r5, #0x1e8]
004534b8: cmp r3, #0
004534bc: beq #0x4534d8
004534c0: mov r0, r3
004534c4: ldr r3, [r3]
004534c8: mov lr, pc
004534cc: ldr pc, [r3, #4]
004534d0: mov r3, #0
004534d4: str r3, [r5, #0x1e8]
004534d8: mov r3, #0
004534dc: str r3, [r5, #0x1e8]
004534e0: bl #0x42ca8c
004534e4: ldr r3, [r0, #0x60]
004534e8: cmp r3, #0
004534ec: beq #0x4534f4
004534f0: pop {r4, r5, r6, pc}
004534f4: ldr r3, [r4, r6]
004534f8: ldr r3, [r3, #0x10]
004534fc: ldr r4, [r3, #0x1c]
00453500: bl #0x42ca8c
00453504: ldr r1, [r0, #0x60]
00453508: mov r0, r4
0045350c: pop {r4, r5, r6, lr}
00453510: b #0x5890c0
00453514: ldrsheq r1, [r4], #-0x54
00453518: strdeq r3, r4, [r0], -r4

# 0x434df8 _ZN11MenuMinimap16DisableMapCameraEv
00434df8: push {r4, lr}
00434dfc: ldr r3, [r0]
00434e00: mov lr, pc
00434e04: ldr pc, [r3, #0x94]
00434e08: ldr r3, [pc, #0x20]
00434e0c: subs r1, r0, #0
00434e10: add r3, pc, r3
00434e14: beq #0x434e2c
00434e18: ldr r2, [pc, #0x14]
00434e1c: ldr r3, [r3, r2]
00434e20: ldr r0, [r3, #0x50]
00434e24: pop {r4, lr}
00434e28: b #0x381fb0
00434e2c: pop {r4, pc}
00434e30: subseq pc, r5, r0, lsl #25
00434e34: strdeq r3, r4, [r0], -r4

# 0x3301d8 _ZN7Console15_setMenuCamerasEv
003301d8: bx lr

# 0x4528d0 _ZN20MenuCharMenu_InvMain18CreateAvatarCameraEv
004528d0: push {r4, r5, r6, r7, r8, sb, sl, lr}
004528d4: ldr r4, [pc, #0x234]
004528d8: ldr r5, [pc, #0x234]
004528dc: sub sp, sp, #0x30
004528e0: add r4, pc, r4
004528e4: ldr r3, [r4, r5]
004528e8: ldr r3, [r3]
004528ec: cmp r3, #0
004528f0: beq #0x4528fc
004528f4: add sp, sp, #0x30
004528f8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
004528fc: bl #0x42ca8c
00452900: ldr r3, [r0, #0x60]
00452904: cmp r3, #0
00452908: beq #0x452af0
0045290c: ldr r6, [pc, #0x204]
00452910: mov r2, #0x43000000
00452914: mov ip, #0xc4000000
00452918: mov r3, #0
0045291c: add ip, ip, #0x480000
00452920: add r2, r2, #0x480000
00452924: mov r1, #0
00452928: mov r0, #0x38c
0045292c: mov r8, r1
00452930: str ip, [sp, #0x28]
00452934: str r3, [sp, #0x1c]
00452938: str r2, [sp, #0x20]
0045293c: str r3, [sp, #0x24]
00452940: str r2, [sp, #0x2c]
00452944: str r3, [sp, #0x18]
00452948: bl #0x5341ac
0045294c: add r2, sp, #0x24
00452950: add r3, sp, #0x18
00452954: mvn r1, #0
00452958: mov r7, r0
0045295c: str r8, [sp]
00452960: bl #0x583734
00452964: ldr sb, [r4, r6]
00452968: ldr sl, [r4, r5]
0045296c: mov r1, r7
00452970: ldr r3, [sb, #0x10]
00452974: str r7, [sl]
00452978: ldr r3, [r3, #0x1c]
0045297c: ldr r3, [r3, #4]
00452980: mov r0, r3
00452984: ldr r3, [r3]
00452988: mov lr, pc
0045298c: ldr pc, [r3, #0x5c]
00452990: ldr r0, [sb, #0x40]
00452994: mov r1, r8
00452998: mov r2, #1
0045299c: bl #0x36e478
004529a0: ldr r3, [r0, #0x660]
004529a4: cmp r3, r8
004529a8: beq #0x4529d0
004529ac: ldr r3, [r3, #0x2d8]
004529b0: cmp r3, r8
004529b4: beq #0x4529d0
004529b8: ldr r3, [r3, #8]
004529bc: ldr r1, [sl]
004529c0: mov r0, r3
004529c4: ldr r3, [r3]
004529c8: mov lr, pc
004529cc: ldr pc, [r3, #0x5c]
004529d0: ldr r5, [r4, r5]
004529d4: ldr r3, [r5]
004529d8: ldr r2, [r3]
004529dc: ldr r0, [r2, #-0xc]
004529e0: add r0, r3, r0
004529e4: bl #0x31d584
004529e8: ldr r4, [r4, r6]
004529ec: ldr r1, [r5]
004529f0: ldr r3, [r4, #0x10]
004529f4: ldr r0, [r3, #0x1c]
004529f8: bl #0x5890c0
004529fc: ldr r0, [r5]
00452a00: mov ip, #0x3f800000
00452a04: mov r2, #0
00452a08: ldr r3, [r0]
00452a0c: add r1, sp, #0xc
00452a10: ldr r3, [r3, #0x114]
00452a14: str ip, [sp, #0x14]
00452a18: str r2, [sp, #0x10]
00452a1c: str r2, [sp, #0xc]
00452a20: blx r3
00452a24: ldr r3, [r5]
00452a28: movw r1, #0x78e9
00452a2c: movt r1, #0x3fd5
00452a30: mov r0, r3
00452a34: ldr r3, [r3]
00452a38: mov lr, pc
00452a3c: ldr pc, [r3, #0x138]
00452a40: ldr r3, [r5]
00452a44: movw r1, #0xfb1a
00452a48: movt r1, #0x3f0e
00452a4c: mov r0, r3
00452a50: ldr r3, [r3]
00452a54: mov lr, pc
00452a58: ldr pc, [r3, #0x13c]
00452a5c: ldr r3, [r5]
00452a60: mov r1, #0x41000000
00452a64: add r1, r1, #0x200000
00452a68: ldr r2, [r3]
00452a6c: ldr r2, [r2, #-0xc]
00452a70: add r3, r3, r2
00452a74: ldr r2, [r3, #4]
00452a78: add r2, r2, #1
00452a7c: str r2, [r3, #4]
00452a80: ldr r3, [r5]
00452a84: mov r0, r3
00452a88: ldr r3, [r3]
00452a8c: mov lr, pc
00452a90: ldr pc, [r3, #0x130]
00452a94: ldr r3, [r5]
00452a98: mov r1, #0x44000000
00452a9c: add r1, r1, #0x7a0000
00452aa0: mov r0, r3
00452aa4: ldr r3, [r3]
00452aa8: mov lr, pc
00452aac: ldr pc, [r3, #0x134]
00452ab0: ldr r0, [r4, #0x40]
00452ab4: mov r1, #0
00452ab8: mov r2, #1
00452abc: bl #0x36e478
00452ac0: ldr r4, [r0, #0x660]
00452ac4: cmp r4, #0
00452ac8: beq #0x4528f4
00452acc: add r4, r4, #0x4f0
00452ad0: add r4, r4, #0xc
00452ad4: mov r0, r4
00452ad8: bl #0x3c03f0
00452adc: subs r1, r0, #0
00452ae0: bne #0x4528f4
00452ae4: mov r0, r4
00452ae8: bl #0x3c1a00
00452aec: b #0x4528f4
00452af0: bl #0x42ca8c
00452af4: ldr r6, [pc, #0x1c]
00452af8: ldr r3, [r4, r6]
00452afc: ldr r3, [r3, #0x10]
00452b00: ldr r3, [r3, #0x1c]
00452b04: ldr r3, [r3, #0xe4]
00452b08: str r3, [r0, #0x60]
00452b0c: b #0x452910
00452b10: ldrheq r2, [r4], #-0x10
00452b14: andeq r1, r0, ip, lsl #15
00452b18: strdeq r3, r4, [r0], -r4

# 0x42cd50 _ZN17MenuFlash2DCameraD0Ev
0042cd50: ldr r3, [pc, #0x24]
0042cd54: ldr r2, [pc, #0x24]
0042cd58: push {r4, lr}
0042cd5c: add r3, pc, r3
0042cd60: ldr r2, [r3, r2]
0042cd64: mov r4, r0
0042cd68: add r2, r2, #8
0042cd6c: str r2, [r0]
0042cd70: bl #0x310440
0042cd74: mov r0, r4
0042cd78: pop {r4, pc}
0042cd7c: subseq r7, r6, r4, lsr sp
0042cd80: andeq r0, r0, r4, ror #28

# 0x432b38 _ZN12MenuMerchant19DestroyAvatarCameraEv
00432b38: push {r4, r5, r6, lr}
00432b3c: ldr r4, [pc, #0x68]
00432b40: ldr r3, [pc, #0x68]
00432b44: add r4, pc, r4
00432b48: ldr r5, [r4, r3]
00432b4c: ldr r3, [r5]
00432b50: cmp r3, #0
00432b54: beq #0x432ba8
00432b58: mov r0, r3
00432b5c: ldr r3, [r3]
00432b60: mov lr, pc
00432b64: ldr pc, [r3, #0x68]
00432b68: ldr r3, [r5]
00432b6c: ldr r2, [r3]
00432b70: ldr r0, [r2, #-0xc]
00432b74: add r0, r3, r0
00432b78: bl #0x31d584
00432b7c: ldr r3, [pc, #0x30]
00432b80: ldr r2, [pc, #0x30]
00432b84: mov r1, #0
00432b88: ldr r3, [r4, r3]
00432b8c: ldr r2, [r4, r2]
00432b90: str r1, [r5]
00432b94: ldr r3, [r3, #0x10]
00432b98: ldr r1, [r2]
00432b9c: ldr r0, [r3, #0x1c]
00432ba0: pop {r4, r5, r6, lr}
00432ba4: b #0x5890c0
00432ba8: pop {r4, r5, r6, pc}
00432bac: subseq r1, r6, ip, asr #30
00432bb0: andeq r4, r0, r4, asr r8
00432bb4: strdeq r3, r4, [r0], -r4
00432bb8: strheq r3, [r0], -ip

# 0x42ccd0 _ZN17MenuFlash2DCameraC1EP6MenuFX
0042ccd0: ldr r2, [pc, #0x68]
0042ccd4: ldr ip, [pc, #0x68]
0042ccd8: ldr r3, [pc, #0x68]
0042ccdc: add r2, pc, r2
0042cce0: push {r4, r5}
0042cce4: ldr ip, [r2, ip]
0042cce8: ldr r4, [r2, r3]
0042ccec: str r1, [r0, #4]
0042ccf0: add r5, ip, #8
0042ccf4: mov ip, #0
0042ccf8: str r5, [r0]
0042ccfc: str ip, [r0, #8]
0042cd00: ldr r1, [r4]
0042cd04: ldr r4, [pc, #0x40]
0042cd08: add r1, r1, r1, lsr #31
0042cd0c: ldr r2, [r2, r4]
0042cd10: asr r1, r1, #1
0042cd14: str r1, [r0, #0x1c]
0042cd18: ldr r2, [r2]
0042cd1c: str ip, [r0, #0x30]
0042cd20: str ip, [r0, #0x24]
0042cd24: add r2, r2, r2, lsr #31
0042cd28: str ip, [r0, #0x28]
0042cd2c: asr r2, r2, #1
0042cd30: str r2, [r0, #0x20]
0042cd34: str ip, [r0, #0x2c]
0042cd38: pop {r4, r5}
0042cd3c: bx lr
0042cd40: ldrheq r7, [r6], #-0xd4
0042cd44: andeq r0, r0, r4, ror #28
0042cd48: andeq r2, r0, r4, asr #11
0042cd4c: strdeq r2, r3, [r0], -r8

# 0x42cbb0 _ZN11MenuManager14GetRootCamera2Ev
0042cbb0: ldr r3, [r0, #0xf4]
0042cbb4: ldr r0, [r3, #0x14c]
0042cbb8: bx lr

# 0x42cb98 _ZN11MenuManager13GetRootCameraEv
0042cb98: ldr r3, [r0, #0xf4]
0042cb9c: ldr r0, [r3, #0x14c]
0042cba0: bx lr

# 0x42ca88 _ZN17MenuFlash2DCameraD1Ev
0042ca88: bx lr

# 0x452810 _ZN20MenuCharMenu_InvMain19DestroyAvatarCameraEv
00452810: push {r4, r5, r6, lr}
00452814: ldr r4, [pc, #0x78]
00452818: ldr r3, [pc, #0x78]
0045281c: add r4, pc, r4
00452820: ldr r5, [r4, r3]
00452824: ldr r3, [r5]
00452828: cmp r3, #0
0045282c: beq #0x452890
00452830: mov r0, r3
00452834: ldr r3, [r3]
00452838: mov lr, pc
0045283c: ldr pc, [r3, #0x68]
00452840: ldr r3, [r5]
00452844: ldr r2, [r3]
00452848: ldr r0, [r2, #-0xc]
0045284c: add r0, r3, r0
00452850: bl #0x31d584
00452854: mov r3, #0
00452858: str r3, [r5]
0045285c: bl #0x42ca8c
00452860: ldr r3, [r0, #0x60]
00452864: cmp r3, #0
00452868: beq #0x452890
0045286c: ldr r3, [pc, #0x28]
00452870: ldr r3, [r4, r3]
00452874: ldr r3, [r3, #0x10]
00452878: ldr r4, [r3, #0x1c]
0045287c: bl #0x42ca8c
00452880: ldr r1, [r0, #0x60]
00452884: mov r0, r4
00452888: pop {r4, r5, r6, lr}
0045288c: b #0x5890c0
00452890: pop {r4, r5, r6, pc}
00452894: subseq r2, r4, r4, ror r2
00452898: andeq r1, r0, ip, lsl #15
0045289c: strdeq r3, r4, [r0], -r4

# 0x42cba4 _ZN11MenuManager16GetHUDRootCameraEv
0042cba4: ldr r3, [r0, #0xf4]
0042cba8: ldr r0, [r3, #0x150]
0042cbac: bx lr

# 0x432ee8 _ZN12MenuMerchant19RenderCharacterPaneERN7gameswf12render_stateEPv
00432ee8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00432eec: mov r0, r1
00432ef0: sub sp, sp, #0x5c
00432ef4: mov r4, r1
00432ef8: ldr r5, [r1, #4]
00432efc: bl #0x42204c
00432f00: ldr r1, [pc, #0x398]
00432f04: mov r2, r0
00432f08: mov r0, r5
00432f0c: add r1, pc, r1
00432f10: bl #0x7a8a84
00432f14: ldr r5, [pc, #0x388]
00432f18: mov r1, r0
00432f1c: add r0, sp, #0x30
00432f20: bl #0x416a7c
00432f24: ldr r3, [pc, #0x37c]
00432f28: add r5, pc, r5
00432f2c: ldr r0, [r4, #4]
00432f30: ldr r6, [r5, r3]
00432f34: ldr r3, [r6, #0x10]
00432f38: ldr r8, [r3, #0x10]
00432f3c: ldr r3, [r8, #0xcc]
00432f40: ldr r3, [r3, #-4]
00432f44: ldr r2, [r3, #0x14]
00432f48: str r2, [sp, #0x20]
00432f4c: ldr r2, [r3, #0x18]
00432f50: str r2, [sp, #0x24]
00432f54: ldr r2, [r3, #0x1c]
00432f58: str r2, [sp, #0x28]
00432f5c: ldr r3, [r3, #0x20]
00432f60: str r3, [sp, #0x2c]
00432f64: bl #0x7a7cac
00432f68: bl #0x416538
00432f6c: mov r7, r0
00432f70: ldr r0, [r4, #4]
00432f74: bl #0x7a7cac
00432f78: bl #0x416578
00432f7c: mov r1, r7
00432f80: mov r4, r0
00432f84: ldr r0, [sp, #0x30]
00432f88: bl #0x30ec94
00432f8c: bl #0x30e4cc
00432f90: mov r1, r7
00432f94: mov sb, r0
00432f98: ldr r0, [sp, #0x34]
00432f9c: bl #0x30ec94
00432fa0: bl #0x30e4cc
00432fa4: mov r1, r4
00432fa8: mov r7, r0
00432fac: ldr r0, [sp, #0x38]
00432fb0: bl #0x30ec94
00432fb4: bl #0x30e4cc
00432fb8: mov r1, r4
00432fbc: mov sl, r0
00432fc0: ldr r0, [sp, #0x3c]
00432fc4: bl #0x30ec94
00432fc8: bl #0x30e4cc
00432fcc: str r7, [sp, #0x18]
00432fd0: str r0, [sp, #0x1c]
00432fd4: str sb, [sp, #0x10]
00432fd8: str sl, [sp, #0x14]
00432fdc: ldr r3, [r8, #0xcc]
00432fe0: add r1, sp, #0x10
00432fe4: ldr r3, [r3, #-4]
00432fe8: mov r0, r3
00432fec: ldr r3, [r3]
00432ff0: mov lr, pc
00432ff4: ldr pc, [r3, #0xc]
00432ff8: ldr r3, [sp, #0x10]
00432ffc: ldr r0, [sp, #0x18]
00433000: rsb r0, r3, r0
00433004: ldr r3, [pc, #0x2a0]
00433008: ldr r4, [r5, r3]
0043300c: bl #0x30e964
00433010: ldr r3, [sp, #0x14]
00433014: mov r7, r0
00433018: ldr r0, [sp, #0x1c]
0043301c: ldr r5, [r4]
00433020: rsb r0, r3, r0
00433024: bl #0x30e964
00433028: mov r1, r0
0043302c: mov r0, r7
00433030: bl #0x30ec94
00433034: ldr r3, [r5]
00433038: mov r1, r0
0043303c: mov r0, r5
00433040: mov lr, pc
00433044: ldr pc, [r3, #0x138]
00433048: ldr r3, [r6, #0x10]
0043304c: ldr r1, [r4]
00433050: ldr r0, [r3, #0x1c]
00433054: bl #0x5890c0
00433058: mov r0, r6
0043305c: bl #0x31f594
00433060: mov r1, #0
00433064: mov fp, r0
00433068: mov r2, #1
0043306c: ldr r0, [r6, #0x40]
00433070: bl #0x36e478
00433074: ldr r5, [r0, #0x660]
00433078: cmp r5, #0
0043307c: beq #0x433280
00433080: add r0, r5, #0x490
00433084: add r0, r0, #0xc
00433088: bl #0x3caf3c
0043308c: ldr r3, [r6, #0x10]
00433090: mov r0, r6
00433094: mov r7, #0
00433098: ldr r4, [r3, #0x1c]
0043309c: mov sl, sp
004330a0: ldr r3, [r4]
004330a4: ldr sb, [r3, #0x60]
004330a8: bl #0x31f66c
004330ac: bl #0x30e2e0
004330b0: mov r2, #0
004330b4: mov r1, r0
004330b8: mov r0, r4
004330bc: blx sb
004330c0: ldr r3, [r5, #0x2d8]
004330c4: ldr r0, [r4, #0x254]
004330c8: ldr r4, [r3, #8]
004330cc: bl #0x30e4cc
004330d0: mov r1, r0
004330d4: mov r0, r4
004330d8: bl #0x35c268
004330dc: ldr r3, [r4]
004330e0: mov r0, r4
004330e4: mov lr, pc
004330e8: ldr pc, [r3, #0xa0]
004330ec: ldr r3, [r4]
004330f0: add r1, sp, #0x4c
004330f4: mov r0, r4
004330f8: ldr r3, [r3, #0xa4]
004330fc: str r7, [sp, #0x4c]
00433100: str r7, [sp, #0x50]
00433104: str r7, [sp, #0x54]
00433108: blx r3
0043310c: ldr r3, [r4]
00433110: mov r0, r4
00433114: mov lr, pc
00433118: ldr pc, [r3, #0x98]
0043311c: str r7, [sp, #0x48]
00433120: str r7, [sp, #0x40]
00433124: str r7, [sp, #0x44]
00433128: ldr r3, [r4]
0043312c: mov r0, r4
00433130: mov lr, pc
00433134: ldr pc, [r3, #0x98]
00433138: add r1, sp, #0x40
0043313c: bl #0x432e58
00433140: movw r1, #0xfa35
00433144: ldr r0, [sp, #0x40]
00433148: movt r1, #0x3c8e
0043314c: bl #0x30ed6c
00433150: movw r1, #0xfa35
00433154: mov r7, r0
00433158: movt r1, #0x3c8e
0043315c: ldr r0, [sp, #0x44]
00433160: str r7, [sp, #0x40]
00433164: bl #0x30ed6c
00433168: movw r1, #0xfa35
0043316c: mov sb, r0
00433170: movt r1, #0x3c8e
00433174: ldr r0, [sp, #0x48]
00433178: str sb, [sp, #0x44]
0043317c: bl #0x30ed6c
00433180: str r0, [sp, #0x48]
00433184: ldr ip, [r4]
00433188: mov r2, sb
0043318c: mov r1, r7
00433190: mov r3, #0xbf000000
00433194: mov r0, sp
00433198: ldr r7, [ip, #0x9c]
0043319c: bl #0x35c9d8
004331a0: mov r0, r4
004331a4: mov r1, sp
004331a8: blx r7
004331ac: ldr r3, [r4]
004331b0: mov r0, r4
004331b4: mov r1, #1
004331b8: mov lr, pc
004331bc: ldr pc, [r3, #0xb8]
004331c0: ldr r3, [r6, #0x10]
004331c4: ldr r3, [r3, #0x1c]
004331c8: ldr r2, [r3, #0xe4]
004331cc: cmp r2, #0
004331d0: beq #0x4331e8
004331d4: mov r0, r3
004331d8: mov r1, r4
004331dc: ldr r3, [r3]
004331e0: mov lr, pc
004331e4: ldr pc, [r3, #0x3c]
004331e8: ldr r3, [r8, #0xcc]
004331ec: add r1, sp, #0x20
004331f0: ldr r3, [r3, #-4]
004331f4: mov r0, r3
004331f8: ldr r3, [r3]
004331fc: mov lr, pc
00433200: ldr pc, [r3, #0xc]
00433204: mov r0, r5
00433208: add r1, r5, #0x160
0043320c: mov r2, #1
00433210: bl #0x393db4
00433214: ldr r3, [r5, #0x2e0]
00433218: cmp r3, #0
0043321c: beq #0x433244
00433220: mov r0, r3
00433224: ldr r3, [r3]
00433228: mov lr, pc
0043322c: ldr pc, [r3, #8]
00433230: ldr r3, [r5, #0x2e0]
00433234: mov r0, r3
00433238: ldr r3, [r3]
0043323c: mov lr, pc
00433240: ldr pc, [r3, #0xc]
00433244: ldr r4, [fp, #0x128]
00433248: mov r1, r5
0043324c: mov r2, #0
00433250: mov r0, r4
00433254: bl #0x4119c4
00433258: mov r0, r4
0043325c: bl #0x40f45c
00433260: mov r0, r4
00433264: ldr r3, [r4]
00433268: mov lr, pc
0043326c: ldr pc, [r3, #0x10]
00433270: ldr r0, [r5, #0x2d8]
00433274: bl #0x472948
00433278: add sp, sp, #0x5c
0043327c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00433280: ldr r3, [r8, #0xcc]
00433284: add r1, sp, #0x20
00433288: ldr r3, [r3, #-4]
0043328c: mov r0, r3
00433290: ldr r3, [r3]
00433294: mov lr, pc
00433298: ldr pc, [r3, #0xc]
0043329c: b #0x433278
004332a0: subeq r8, sb, ip, lsr #12
004332a4: subseq r1, r6, r8, ror #22
004332a8: strdeq r3, r4, [r0], -r4
004332ac: andeq r4, r0, r4, asr r8

# 0x45351c _ZN16MenuCharMenu_Map15CreateMapCameraEv
0045351c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00453520: ldr r3, [r0, #0x1e8]
00453524: ldr r4, [pc, #0x260]
00453528: sub sp, sp, #0x1c
0045352c: cmp r3, #0
00453530: mov r5, r0
00453534: add r4, pc, r4
00453538: beq #0x453544
0045353c: add sp, sp, #0x1c
00453540: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00453544: bl #0x42ca8c
00453548: ldr r3, [r0, #0x60]
0045354c: cmp r3, #0
00453550: beq #0x45376c
00453554: ldr sb, [pc, #0x234]
00453558: mov r1, #0
0045355c: mov r0, #0xa8
00453560: bl #0x310570
00453564: mov r6, r0
00453568: bl #0x40ff70
0045356c: cmp r6, #0
00453570: str r6, [r5, #0x1e8]
00453574: beq #0x453714
00453578: ldr r3, [pc, #0x214]
0045357c: ldr r3, [r4, r3]
00453580: ldr r8, [r3]
00453584: cmp r8, #0
00453588: beq #0x4535d0
0045358c: ldr r3, [pc, #0x204]
00453590: ldr fp, [pc, #0x204]
00453594: mov r7, #0
00453598: ldr r3, [r4, r3]
0045359c: add fp, pc, fp
004535a0: ldr sl, [r3]
004535a4: b #0x4535b4
004535a8: add r7, r7, #1
004535ac: cmp r7, r8
004535b0: beq #0x4535d0
004535b4: ldr r1, [sl, r7, lsl #2]
004535b8: mov r0, fp
004535bc: bl #0x30e31c
004535c0: cmp r0, #0
004535c4: bne #0x4535a8
004535c8: mov r2, r7
004535cc: b #0x4535d4
004535d0: mvn r2, #0
004535d4: ldr r3, [pc, #0x1c4]
004535d8: ldr r1, [pc, #0x1c4]
004535dc: mov r0, r6
004535e0: add r3, pc, r3
004535e4: add r1, pc, r1
004535e8: bl #0x41068c
004535ec: ldr r3, [r5, #0x1e8]
004535f0: mov r8, #1
004535f4: mov sl, #0x3f800000
004535f8: strb r8, [r3, #0x85]
004535fc: ldr r3, [r5, #0x1e8]
00453600: mov r7, #0
00453604: mov r1, #0
00453608: str sl, [r3, #0x8c]
0045360c: ldr r3, [r5, #0x1e8]
00453610: mov r6, r1
00453614: str r7, [r3, #0x88]
00453618: ldr r0, [r5, #0x1e8]
0045361c: bl #0x41161c
00453620: movw r1, #0xf877
00453624: movw ip, #0x5000
00453628: ldr r0, [r5, #0x1e8]
0045362c: movt ip, #0x47c3
00453630: mov r3, r7
00453634: mov r2, #0x3fc00000
00453638: movt r1, #0x3edb
0045363c: str ip, [sp]
00453640: str r6, [sp, #4]
00453644: bl #0x40e9a8
00453648: ldr r3, [r5, #0x1e8]
0045364c: mov r2, #0xbf000000
00453650: add r2, r2, #0x800000
00453654: ldr r0, [r3, #8]
00453658: add r1, sp, #0xc
0045365c: ldr r3, [r0]
00453660: ldr r3, [r3, #0x114]
00453664: str r2, [sp, #0xc]
00453668: str r7, [sp, #0x14]
0045366c: str sl, [sp, #0x10]
00453670: blx r3
00453674: ldr r0, [r5, #0x1e8]
00453678: bl #0x40f45c
0045367c: ldr r3, [pc, #0x124]
00453680: ldr r0, [r5, #0x1e8]
00453684: mov lr, #0x1c
00453688: ldr r3, [r4, r3]
0045368c: ldr r1, [r0, #0x80]
00453690: mov r2, r6
00453694: ldr ip, [r3]
00453698: mov r3, r6
0045369c: mla r1, lr, r1, ip
004536a0: ldr r1, [r1, #0x10]
004536a4: bl #0x40f904
004536a8: ldr r4, [r4, sb]
004536ac: mov r1, r6
004536b0: mov r2, r8
004536b4: ldr r0, [r4, #0x40]
004536b8: ldr r7, [r5, #0x1e8]
004536bc: bl #0x36e478
004536c0: mov r2, r6
004536c4: ldr r1, [r0, #0x660]
004536c8: mov r0, r7
004536cc: bl #0x4119c4
004536d0: ldr r3, [r4, #0x10]
004536d4: ldr r2, [r5, #0x1e8]
004536d8: ldr r3, [r3, #0x1c]
004536dc: ldr r1, [r2, #4]
004536e0: ldr r3, [r3, #0x28c]
004536e4: mov r0, r3
004536e8: ldr r3, [r3]
004536ec: mov lr, pc
004536f0: ldr pc, [r3, #0x5c]
004536f4: ldr r0, [r4, #0x50]
004536f8: ldr r1, [r5, #0x1e8]
004536fc: bl #0x381fb0
00453700: ldr r3, [r4, #0x50]
00453704: mov r0, r5
00453708: strb r8, [r3, #0x24]
0045370c: bl #0x45310c
00453710: b #0x45353c
00453714: ldr r3, [pc, #0x90]
00453718: ldr r3, [r4, r3]
0045371c: ldr r3, [r3]
00453720: cmp r3, #2
00453724: streq r6, [r6]
00453728: beq #0x453578
0045372c: cmp r3, #1
00453730: bne #0x453578
00453734: ldr r0, [pc, #0x74]
00453738: ldr r1, [pc, #0x74]
0045373c: ldr r2, [pc, #0x74]
00453740: ldr r0, [r4, r0]
00453744: ldr r3, [pc, #0x70]
00453748: mov ip, #0x118
0045374c: add r1, pc, r1
00453750: add r0, r0, #0xa8
00453754: add r2, pc, r2
00453758: add r3, pc, r3
0045375c: str ip, [sp]
00453760: bl #0x30e004
00453764: ldr r6, [r5, #0x1e8]
00453768: b #0x453578
0045376c: bl #0x42ca8c
00453770: ldr sb, [pc, #0x18]
00453774: ldr r3, [r4, sb]
00453778: ldr r3, [r3, #0x10]
0045377c: ldr r3, [r3, #0x1c]
00453780: ldr r3, [r3, #0xe4]
00453784: str r3, [r0, #0x60]
00453788: b #0x453558
0045378c: subseq r1, r4, ip, asr r5
00453790: strdeq r3, r4, [r0], -r4
00453794: andeq r3, r0, r4, ror #17
00453798: andeq r3, r0, ip, asr sl
0045379c: ldrdeq r3, r4, [r7], #-0x24
004537a0: subeq sb, r7, r0, lsr #16
004537a4: strdeq sb, sl, [r7], #-0x74
004537a8: ldrdeq r3, r4, [r0], -r4
004537ac: andeq r3, r0, r0, asr #19
004537b0: andeq r1, r0, r0, asr #19
004537b4: subeq sl, r6, ip, lsl #25
004537b8: subeq r5, r7, ip, lsl #2
004537bc: subeq sb, r7, r8, lsr #12

# 0x42cc50 _ZN17MenuFlash2DCameraC2EP6MenuFX
0042cc50: ldr r2, [pc, #0x68]
0042cc54: ldr ip, [pc, #0x68]
0042cc58: ldr r3, [pc, #0x68]
0042cc5c: add r2, pc, r2
0042cc60: push {r4, r5}
0042cc64: ldr ip, [r2, ip]
0042cc68: ldr r4, [r2, r3]
0042cc6c: str r1, [r0, #4]
0042cc70: add r5, ip, #8
0042cc74: mov ip, #0
0042cc78: str r5, [r0]
0042cc7c: str ip, [r0, #8]
0042cc80: ldr r1, [r4]
0042cc84: ldr r4, [pc, #0x40]
0042cc88: add r1, r1, r1, lsr #31
0042cc8c: ldr r2, [r2, r4]
0042cc90: asr r1, r1, #1
0042cc94: str r1, [r0, #0x1c]
0042cc98: ldr r2, [r2]
0042cc9c: str ip, [r0, #0x30]
0042cca0: str ip, [r0, #0x24]
0042cca4: add r2, r2, r2, lsr #31
0042cca8: str ip, [r0, #0x28]
0042ccac: asr r2, r2, #1
0042ccb0: str r2, [r0, #0x20]
0042ccb4: str ip, [r0, #0x2c]
0042ccb8: pop {r4, r5}
0042ccbc: bx lr
0042ccc0: subseq r7, r6, r4, lsr lr
0042ccc4: andeq r0, r0, r4, ror #28
0042ccc8: andeq r2, r0, r4, asr #11
0042cccc: strdeq r2, r3, [r0], -r8

# 0x434e38 _ZN11MenuMinimap15CreateMapCameraEv
00434e38: push {r4, r5, r6, r7, r8, sb, sl, lr}
00434e3c: ldr r1, [r0, #0xe0]
00434e40: ldr r4, [pc, #0x194]
00434e44: sub sp, sp, #8
00434e48: cmp r1, #0
00434e4c: mov r5, r0
00434e50: add r4, pc, r4
00434e54: beq #0x434e60
00434e58: add sp, sp, #8
00434e5c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00434e60: mov r0, #0xa8
00434e64: bl #0x310570
00434e68: mov r6, r0
00434e6c: bl #0x40ff70
00434e70: cmp r6, #0
00434e74: str r6, [r5, #0xe0]
00434e78: beq #0x434f84
00434e7c: ldr r3, [pc, #0x15c]
00434e80: ldr r3, [r4, r3]
00434e84: ldr r8, [r3]
00434e88: cmp r8, #0
00434e8c: beq #0x434ed4
00434e90: ldr r3, [pc, #0x14c]
00434e94: ldr sb, [pc, #0x14c]
00434e98: mov r7, #0
00434e9c: ldr r3, [r4, r3]
00434ea0: add sb, pc, sb
00434ea4: ldr sl, [r3]
00434ea8: b #0x434eb8
00434eac: add r7, r7, #1
00434eb0: cmp r7, r8
00434eb4: beq #0x434ed4
00434eb8: ldr r1, [sl, r7, lsl #2]
00434ebc: mov r0, sb
00434ec0: bl #0x30e31c
00434ec4: cmp r0, #0
00434ec8: bne #0x434eac
00434ecc: mov r2, r7
00434ed0: b #0x434ed8
00434ed4: mvn r2, #0
00434ed8: ldr r3, [pc, #0x10c]
00434edc: ldr r1, [pc, #0x10c]
00434ee0: mov r0, r6
00434ee4: add r3, pc, r3
00434ee8: add r1, pc, r1
00434eec: bl #0x41068c
00434ef0: ldr r3, [r5, #0xe0]
00434ef4: mov r2, #1
00434ef8: mov r1, #0
00434efc: strb r2, [r3, #0x85]
00434f00: ldr r0, [r5, #0xe0]
00434f04: bl #0x41161c
00434f08: mov r6, #0
00434f0c: movw r1, #0xf877
00434f10: movw r2, #0x8e39
00434f14: movw ip, #0x5000
00434f18: ldr r0, [r5, #0xe0]
00434f1c: mov r7, #0
00434f20: mov r3, r6
00434f24: movt ip, #0x47c3
00434f28: movt r1, #0x3edb
00434f2c: movt r2, #0x3fe3
00434f30: str ip, [sp]
00434f34: str r7, [sp, #4]
00434f38: bl #0x40e9a8
00434f3c: ldr r3, [r5, #0xe0]
00434f40: mov r2, #0x3f800000
00434f44: str r2, [r3, #0x8c]
00434f48: ldr r1, [r5, #0xe0]
00434f4c: ldr r3, [pc, #0xa0]
00434f50: mov r2, r7
00434f54: str r6, [r1, #0x88]
00434f58: ldr r3, [r4, r3]
00434f5c: ldr r0, [r5, #0xe0]
00434f60: mov r4, #0x1c
00434f64: ldr ip, [r3]
00434f68: ldr r1, [r0, #0x80]
00434f6c: mov r3, r7
00434f70: mla r1, r4, r1, ip
00434f74: ldr r1, [r1, #0x10]
00434f78: add sp, sp, #8
00434f7c: pop {r4, r5, r6, r7, r8, sb, sl, lr}
00434f80: b #0x40f904
00434f84: ldr r3, [pc, #0x6c]
00434f88: ldr r3, [r4, r3]
00434f8c: ldr r3, [r3]
00434f90: cmp r3, #2
00434f94: streq r6, [r6]
00434f98: beq #0x434e7c
00434f9c: cmp r3, #1
00434fa0: bne #0x434e7c
00434fa4: ldr r0, [pc, #0x50]
00434fa8: ldr r1, [pc, #0x50]
00434fac: ldr r2, [pc, #0x50]
00434fb0: ldr r0, [r4, r0]
00434fb4: ldr r3, [pc, #0x4c]
00434fb8: mov ip, #0xb3
00434fbc: add r1, pc, r1
00434fc0: add r0, r0, #0xa8
00434fc4: add r2, pc, r2
00434fc8: add r3, pc, r3
00434fcc: str ip, [sp]
00434fd0: bl #0x30e004
00434fd4: ldr r6, [r5, #0xe0]
00434fd8: b #0x434e7c
00434fdc: subseq pc, r5, r0, asr #24
00434fe0: andeq r3, r0, r4, ror #17
00434fe4: andeq r3, r0, ip, asr sl
00434fe8: subeq r3, sb, r8, lsl sl
00434fec: subeq r1, sb, r4, lsl #15
00434ff0: subeq r1, sb, r0, ror #14
00434ff4: ldrdeq r3, r4, [r0], -r4
00434ff8: andeq r3, r0, r0, asr #19
00434ffc: andeq r1, r0, r0, asr #19
00435000: subeq sb, r8, ip, lsl r4
00435004: umaaleq r3, sb, ip, r8
00435008: subeq r6, sb, r8, ror #13

# 0x42bb44 _ZN12MenuMainMenu19RenderCharacterPaneERN7gameswf12render_stateEPv
0042bb44: bx lr

# 0x45310c _ZN16MenuCharMenu_Map15UpdateMapCameraEv
0045310c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00453110: ldr r5, [r0, #0x1e8]
00453114: sub sp, sp, #0xc
00453118: mov r4, r0
0045311c: cmp r5, #0
00453120: beq #0x4531e4
00453124: ldr sb, [r0, #0xec]
00453128: ldr r1, [r5, #0x98]
0045312c: mov r0, sb
00453130: bl #0x30eba4
00453134: ldr sl, [r4, #0xf0]
00453138: ldr r1, [r5, #0x9c]
0045313c: mov r7, r0
00453140: mov r0, sl
00453144: bl #0x30eba4
00453148: ldr r8, [r4, #0xf4]
0045314c: ldr r1, [r5, #0xa0]
00453150: mov r6, r0
00453154: mov r0, r8
00453158: bl #0x30eba4
0045315c: str r0, [sp, #4]
00453160: ldr fp, [r4, #0xe0]
00453164: mov r1, r7
00453168: mov r0, fp
0045316c: bl #0x30e70c
00453170: cmp r0, #0
00453174: beq #0x453208
00453178: mov r7, fp
0045317c: ldr fp, [r4, #0xe4]
00453180: mov r1, r6
00453184: mov r0, fp
00453188: bl #0x30e70c
0045318c: cmp r0, #0
00453190: beq #0x4531ec
00453194: mov r6, fp
00453198: mov r0, r7
0045319c: mov r1, sb
004531a0: bl #0x30e3ac
004531a4: mov r1, sl
004531a8: mov r7, r0
004531ac: mov r0, r6
004531b0: bl #0x30e3ac
004531b4: mov r1, r8
004531b8: mov r6, r0
004531bc: ldr r0, [sp, #4]
004531c0: bl #0x30e3ac
004531c4: str r7, [r5, #0x98]
004531c8: str r0, [r5, #0xa0]
004531cc: str r6, [r5, #0x9c]
004531d0: ldr r3, [r4, #0x1e8]
004531d4: mov r0, r3
004531d8: ldr r3, [r3]
004531dc: mov lr, pc
004531e0: ldr pc, [r3, #0x10]
004531e4: add sp, sp, #0xc
004531e8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004531ec: ldr fp, [r4, #0xd8]
004531f0: mov r1, r6
004531f4: mov r0, fp
004531f8: bl #0x30e2f8
004531fc: cmp r0, #0
00453200: beq #0x453198
00453204: b #0x453194
00453208: ldr fp, [r4, #0xd4]
0045320c: mov r1, r7
00453210: mov r0, fp
00453214: bl #0x30e2f8
00453218: cmp r0, #0
0045321c: beq #0x45317c
00453220: b #0x453178

# 0x433318 _ZN12MenuMerchant18CreateAvatarCameraEv
00433318: push {r4, r5, r6, r7, r8, sb, sl, lr}
0043331c: ldr r4, [pc, #0x214]
00433320: ldr r7, [pc, #0x214]
00433324: sub sp, sp, #0x30
00433328: add r4, pc, r4
0043332c: ldr r6, [r4, r7]
00433330: ldr r5, [r6]
00433334: cmp r5, #0
00433338: beq #0x433344
0043333c: add sp, sp, #0x30
00433340: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00433344: ldr sb, [pc, #0x1f4]
00433348: ldr r2, [pc, #0x1f4]
0043334c: mov r3, #0
00433350: ldr r8, [r4, sb]
00433354: ldr ip, [r4, r2]
00433358: mov r2, #0x43000000
0043335c: ldr r0, [r8, #0x10]
00433360: add r2, r2, #0x480000
00433364: mov r1, r5
00433368: ldr lr, [r0, #0x1c]
0043336c: mov r0, #0x38c
00433370: ldr sl, [lr, #0xe4]
00433374: mov lr, #0xc4000000
00433378: add lr, lr, #0x480000
0043337c: str sl, [ip]
00433380: str lr, [sp, #0x28]
00433384: str r3, [sp, #0x1c]
00433388: str r2, [sp, #0x20]
0043338c: str r3, [sp, #0x24]
00433390: str r2, [sp, #0x2c]
00433394: str r3, [sp, #0x18]
00433398: bl #0x5341ac
0043339c: add r2, sp, #0x24
004333a0: add r3, sp, #0x18
004333a4: mvn r1, #0
004333a8: mov sl, r0
004333ac: str r5, [sp]
004333b0: bl #0x583734
004333b4: ldr r3, [r8, #0x10]
004333b8: str sl, [r6]
004333bc: mov r1, sl
004333c0: ldr r3, [r3, #0x1c]
004333c4: ldr r3, [r3, #4]
004333c8: mov r0, r3
004333cc: ldr r3, [r3]
004333d0: mov lr, pc
004333d4: ldr pc, [r3, #0x5c]
004333d8: ldr r0, [r8, #0x40]
004333dc: mov r1, r5
004333e0: mov r2, #1
004333e4: bl #0x36e478
004333e8: ldr r3, [r0, #0x660]
004333ec: cmp r3, #0
004333f0: beq #0x433418
004333f4: ldr r3, [r3, #0x2d8]
004333f8: cmp r3, #0
004333fc: beq #0x433418
00433400: ldr r3, [r3, #8]
00433404: ldr r1, [r6]
00433408: mov r0, r3
0043340c: ldr r3, [r3]
00433410: mov lr, pc
00433414: ldr pc, [r3, #0x5c]
00433418: ldr r5, [r4, r7]
0043341c: ldr r3, [r5]
00433420: ldr r2, [r3]
00433424: ldr r0, [r2, #-0xc]
00433428: add r0, r3, r0
0043342c: bl #0x31d584
00433430: ldr r4, [r4, sb]
00433434: ldr r1, [r5]
00433438: ldr r3, [r4, #0x10]
0043343c: ldr r0, [r3, #0x1c]
00433440: bl #0x5890c0
00433444: ldr r0, [r5]
00433448: mov ip, #0x3f800000
0043344c: mov r2, #0
00433450: ldr r3, [r0]
00433454: add r1, sp, #0xc
00433458: ldr r3, [r3, #0x114]
0043345c: str ip, [sp, #0x14]
00433460: str r2, [sp, #0x10]
00433464: str r2, [sp, #0xc]
00433468: blx r3
0043346c: ldr r3, [r5]
00433470: movw r1, #0x78e9
00433474: movt r1, #0x3fd5
00433478: mov r0, r3
0043347c: ldr r3, [r3]
00433480: mov lr, pc
00433484: ldr pc, [r3, #0x138]
00433488: ldr r3, [r5]
0043348c: movw r1, #0xfb1a
00433490: movt r1, #0x3f0e
00433494: mov r0, r3
00433498: ldr r3, [r3]
0043349c: mov lr, pc
004334a0: ldr pc, [r3, #0x13c]
004334a4: ldr r3, [r5]
004334a8: mov r1, #0x41000000
004334ac: add r1, r1, #0x200000
004334b0: ldr r2, [r3]
004334b4: ldr r2, [r2, #-0xc]
004334b8: add r3, r3, r2
004334bc: ldr r2, [r3, #4]
004334c0: add r2, r2, #1
004334c4: str r2, [r3, #4]
004334c8: ldr r3, [r5]
004334cc: mov r0, r3
004334d0: ldr r3, [r3]
004334d4: mov lr, pc
004334d8: ldr pc, [r3, #0x130]
004334dc: ldr r3, [r5]
004334e0: mov r1, #0x44000000
004334e4: add r1, r1, #0x7a0000
004334e8: mov r0, r3
004334ec: ldr r3, [r3]
004334f0: mov lr, pc
004334f4: ldr pc, [r3, #0x134]
004334f8: ldr r0, [r4, #0x40]
004334fc: mov r1, #0
00433500: mov r2, #1
00433504: bl #0x36e478
00433508: ldr r4, [r0, #0x660]
0043350c: cmp r4, #0
00433510: beq #0x43333c
00433514: add r4, r4, #0x4f0
00433518: add r4, r4, #0xc
0043351c: mov r0, r4
00433520: bl #0x3c03f0
00433524: subs r1, r0, #0
00433528: bne #0x43333c
0043352c: mov r0, r4
00433530: bl #0x3c1a00
00433534: b #0x43333c
00433538: subseq r1, r6, r8, ror #14
0043353c: andeq r4, r0, r4, asr r8
00433540: strdeq r3, r4, [r0], -r4
00433544: strheq r3, [r0], -ip

# 0x452468 _ZN20MenuCharMenu_InvMain19RenderCharacterPaneERN7gameswf12render_stateEPv
00452468: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045246c: mov r0, r1
00452470: sub sp, sp, #0x5c
00452474: mov r4, r1
00452478: ldr r5, [r1, #4]
0045247c: bl #0x42204c
00452480: ldr r1, [pc, #0x378]
00452484: mov r2, r0
00452488: mov r0, r5
0045248c: add r1, pc, r1
00452490: bl #0x7a8a84
00452494: ldr r5, [pc, #0x368]
00452498: mov r1, r0
0045249c: add r0, sp, #0x30
004524a0: bl #0x416a7c
004524a4: ldr r3, [pc, #0x35c]
004524a8: add r5, pc, r5
004524ac: ldr r0, [r4, #4]
004524b0: ldr r6, [r5, r3]
004524b4: ldr r3, [r6, #0x10]
004524b8: ldr r8, [r3, #0x10]
004524bc: ldr r3, [r8, #0xcc]
004524c0: ldr r3, [r3, #-4]
004524c4: ldr r2, [r3, #0x14]
004524c8: str r2, [sp, #0x20]
004524cc: ldr r2, [r3, #0x18]
004524d0: str r2, [sp, #0x24]
004524d4: ldr r2, [r3, #0x1c]
004524d8: str r2, [sp, #0x28]
004524dc: ldr r3, [r3, #0x20]
004524e0: str r3, [sp, #0x2c]
004524e4: bl #0x7a7cac
004524e8: bl #0x416538
004524ec: mov r7, r0
004524f0: ldr r0, [r4, #4]
004524f4: bl #0x7a7cac
004524f8: bl #0x416578
004524fc: mov r1, r7
00452500: mov r4, r0
00452504: ldr r0, [sp, #0x30]
00452508: bl #0x30ec94
0045250c: bl #0x30e4cc
00452510: mov r1, r7
00452514: mov sb, r0
00452518: ldr r0, [sp, #0x34]
0045251c: bl #0x30ec94
00452520: bl #0x30e4cc
00452524: mov r1, r4
00452528: mov r7, r0
0045252c: ldr r0, [sp, #0x38]
00452530: bl #0x30ec94
00452534: bl #0x30e4cc
00452538: mov r1, r4
0045253c: mov sl, r0
00452540: ldr r0, [sp, #0x3c]
00452544: bl #0x30ec94
00452548: bl #0x30e4cc
0045254c: str r7, [sp, #0x18]
00452550: str r0, [sp, #0x1c]
00452554: str sb, [sp, #0x10]
00452558: str sl, [sp, #0x14]
0045255c: ldr r3, [r8, #0xcc]
00452560: add r1, sp, #0x10
00452564: ldr r3, [r3, #-4]
00452568: mov r0, r3
0045256c: ldr r3, [r3]
00452570: mov lr, pc
00452574: ldr pc, [r3, #0xc]
00452578: ldr r3, [sp, #0x10]
0045257c: ldr r0, [sp, #0x18]
00452580: rsb r0, r3, r0
00452584: ldr r3, [pc, #0x280]
00452588: ldr r4, [r5, r3]
0045258c: bl #0x30e964
00452590: ldr r3, [sp, #0x14]
00452594: mov r7, r0
00452598: ldr r0, [sp, #0x1c]
0045259c: ldr r5, [r4]
004525a0: rsb r0, r3, r0
004525a4: bl #0x30e964
004525a8: mov r1, r0
004525ac: mov r0, r7
004525b0: bl #0x30ec94
004525b4: ldr r3, [r5]
004525b8: mov r1, r0
004525bc: mov r0, r5
004525c0: mov lr, pc
004525c4: ldr pc, [r3, #0x138]
004525c8: ldr r3, [r6, #0x10]
004525cc: ldr r1, [r4]
004525d0: ldr r0, [r3, #0x1c]
004525d4: bl #0x5890c0
004525d8: mov r0, r6
004525dc: bl #0x31f594
004525e0: mov r1, #0
004525e4: mov fp, r0
004525e8: mov r2, #1
004525ec: ldr r0, [r6, #0x40]
004525f0: bl #0x36e478
004525f4: ldr r5, [r0, #0x660]
004525f8: cmp r5, #0
004525fc: beq #0x4527f8
00452600: add r0, r5, #0x490
00452604: add r0, r0, #0xc
00452608: bl #0x3caf3c
0045260c: ldr r3, [r6, #0x10]
00452610: mov r0, r6
00452614: mov r7, #0
00452618: ldr r4, [r3, #0x1c]
0045261c: mov sl, sp
00452620: ldr r3, [r4]
00452624: ldr sb, [r3, #0x60]
00452628: bl #0x31f66c
0045262c: bl #0x30e2e0
00452630: mov r2, #0
00452634: mov r1, r0
00452638: mov r0, r4
0045263c: blx sb
00452640: ldr r3, [r5, #0x2d8]
00452644: ldr r0, [r4, #0x254]
00452648: ldr r4, [r3, #8]
0045264c: bl #0x30e4cc
00452650: mov r1, r0
00452654: mov r0, r4
00452658: bl #0x35c268
0045265c: ldr r3, [r4]
00452660: mov r0, r4
00452664: mov lr, pc
00452668: ldr pc, [r3, #0xa0]
0045266c: ldr r3, [r4]
00452670: add r1, sp, #0x4c
00452674: mov r0, r4
00452678: ldr r3, [r3, #0xa4]
0045267c: str r7, [sp, #0x4c]
00452680: str r7, [sp, #0x50]
00452684: str r7, [sp, #0x54]
00452688: blx r3
0045268c: ldr r3, [r4]
00452690: mov r0, r4
00452694: mov lr, pc
00452698: ldr pc, [r3, #0x98]
0045269c: str r7, [sp, #0x48]
004526a0: str r7, [sp, #0x40]
004526a4: str r7, [sp, #0x44]
004526a8: ldr r3, [r4]
004526ac: mov r0, r4
004526b0: mov lr, pc
004526b4: ldr pc, [r3, #0x98]
004526b8: add r1, sp, #0x40
004526bc: bl #0x432e58
004526c0: movw r1, #0xfa35
004526c4: ldr r0, [sp, #0x40]
004526c8: movt r1, #0x3c8e
004526cc: bl #0x30ed6c
004526d0: movw r1, #0xfa35
004526d4: mov r7, r0
004526d8: movt r1, #0x3c8e
004526dc: ldr r0, [sp, #0x44]
004526e0: str r7, [sp, #0x40]
004526e4: bl #0x30ed6c
004526e8: movw r1, #0xfa35
004526ec: mov sb, r0
004526f0: movt r1, #0x3c8e
004526f4: ldr r0, [sp, #0x48]
004526f8: str sb, [sp, #0x44]
004526fc: bl #0x30ed6c
00452700: str r0, [sp, #0x48]
00452704: ldr ip, [r4]
00452708: mov r2, sb
0045270c: mov r1, r7
00452710: mov r3, #0xbf000000
00452714: mov r0, sp
00452718: ldr r7, [ip, #0x9c]
0045271c: bl #0x35c9d8
00452720: mov r0, r4
00452724: mov r1, sp
00452728: blx r7
0045272c: ldr r3, [r4]
00452730: mov r0, r4
00452734: mov r1, #1
00452738: mov lr, pc
0045273c: ldr pc, [r3, #0xb8]
00452740: ldr r3, [r6, #0x10]
00452744: ldr r3, [r3, #0x1c]
00452748: ldr r2, [r3, #0xe4]
0045274c: cmp r2, #0
00452750: beq #0x452768
00452754: mov r0, r3
00452758: mov r1, r4
0045275c: ldr r3, [r3]
00452760: mov lr, pc
00452764: ldr pc, [r3, #0x3c]
00452768: ldr r3, [r8, #0xcc]
0045276c: add r1, sp, #0x20
00452770: ldr r3, [r3, #-4]
00452774: mov r0, r3
00452778: ldr r3, [r3]
0045277c: mov lr, pc
00452780: ldr pc, [r3, #0xc]
00452784: mov r0, r5
00452788: add r1, r5, #0x160
0045278c: mov r2, #1
00452790: bl #0x393db4
00452794: ldr r3, [r5, #0x2e0]
00452798: cmp r3, #0
0045279c: beq #0x4527c4
004527a0: mov r0, r3
004527a4: ldr r3, [r3]
004527a8: mov lr, pc
004527ac: ldr pc, [r3, #8]
004527b0: ldr r3, [r5, #0x2e0]
004527b4: mov r0, r3
004527b8: ldr r3, [r3]
004527bc: mov lr, pc
004527c0: ldr pc, [r3, #0xc]
004527c4: ldr r4, [fp, #0x128]
004527c8: mov r1, r5
004527cc: mov r2, #0
004527d0: mov r0, r4
004527d4: bl #0x4119c4
004527d8: mov r0, r4
004527dc: bl #0x40f45c
004527e0: mov r0, r4
004527e4: ldr r3, [r4]
004527e8: mov lr, pc
004527ec: ldr pc, [r3, #0x10]
004527f0: ldr r0, [r5, #0x2d8]
004527f4: bl #0x472948
004527f8: add sp, sp, #0x5c
004527fc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00452800: subeq sb, r7, ip, lsr #1
00452804: subseq r2, r4, r8, ror #11
00452808: strdeq r3, r4, [r0], -r4
0045280c: andeq r1, r0, ip, lsl #15
