# 0x3935dc _ZNK10GameObject17GetTargetPositionEv
003935dc: ldr r3, [r0, #0x180]
003935e0: cmp r3, #0
003935e4: beq #0x3935f8
003935e8: ldrb r3, [r0, #0x80]
003935ec: cmp r3, #0
003935f0: addne r0, r0, #0x184
003935f4: bxne lr
003935f8: add r0, r0, #0x160
003935fc: bx lr

# 0x470c24 _ZN12VisualObject11SetPositionERK7Point3DIfE
00470c24: push {r4, lr}
00470c28: mov r4, r0
00470c2c: ldr r0, [r0, #8]
00470c30: sub sp, sp, #0x10
00470c34: cmp r0, #0
00470c38: beq #0x470c7c
00470c3c: ldr r3, [r0]
00470c40: ldr lr, [r1]
00470c44: ldr ip, [r1, #4]
00470c48: ldr r2, [r1, #8]
00470c4c: ldr r3, [r3, #0xa4]
00470c50: add r1, sp, #4
00470c54: str lr, [sp, #4]
00470c58: str ip, [sp, #8]
00470c5c: str r2, [sp, #0xc]
00470c60: blx r3
00470c64: ldr r3, [r4, #8]
00470c68: mov r1, #0
00470c6c: mov r0, r3
00470c70: ldr r3, [r3]
00470c74: mov lr, pc
00470c78: ldr pc, [r3, #0xb8]
00470c7c: add sp, sp, #0x10
00470c80: pop {r4, pc}

# 0x432bbc _ZNK6glitch4core8CMatrix4IfE18getRotationDegreesEv
00432bbc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00432bc0: mov r4, r0
00432bc4: sub sp, sp, #0xc
00432bc8: ldr r0, [r1, #8]
00432bcc: mov r5, r1
00432bd0: bl #0x30dd88
00432bd4: add r0, r0, #0x80000000
00432bd8: bl #0x30e8a4
00432bdc: mov r8, r0
00432be0: mov sb, r1
00432be4: bl #0x30e148
00432be8: movw r2, #0xc1f8
00432bec: movw r3, #0xa5dc
00432bf0: mov r7, r0
00432bf4: mov r6, r1
00432bf8: movt r2, #0x1a63
00432bfc: movt r3, #0x404c
00432c00: mov r0, r8
00432c04: mov r1, sb
00432c08: bl #0x30eab4
00432c0c: movw r2, #0x8c3a
00432c10: movw r3, #0x798e
00432c14: mov sl, r0
00432c18: mov fp, r1
00432c1c: mov r0, r7
00432c20: bic r1, r6, #0x80000000
00432c24: movt r2, #0xe230
00432c28: movt r3, #0x3e45
00432c2c: bl #0x30de60
00432c30: cmp r0, #0
00432c34: beq #0x432e00
00432c38: mov r1, #0x3fc00000
00432c3c: mov r2, r7
00432c40: mov r3, r6
00432c44: mov r0, #0
00432c48: add r1, r1, #0x300000
00432c4c: bl #0x30e340
00432c50: mov r6, r0
00432c54: mov r7, r1
00432c58: ldr r0, [r5, #0x18]
00432c5c: bl #0x30e8a4
00432c60: mov r2, r6
00432c64: mov r3, r7
00432c68: bl #0x30eab4
00432c6c: mov r8, r0
00432c70: ldr r0, [r5, #0x28]
00432c74: mov sb, r1
00432c78: bl #0x30e8a4
00432c7c: mov r2, r6
00432c80: mov r3, r7
00432c84: bl #0x30eab4
00432c88: mov r2, r0
00432c8c: mov r3, r1
00432c90: mov r0, r8
00432c94: mov r1, sb
00432c98: bl #0x30df20
00432c9c: movw r2, #0xc1f8
00432ca0: movw r3, #0xa5dc
00432ca4: movt r2, #0x1a63
00432ca8: movt r3, #0x404c
00432cac: bl #0x30eab4
00432cb0: mov r8, r0
00432cb4: ldr r0, [r5, #4]
00432cb8: mov sb, r1
00432cbc: bl #0x30e8a4
00432cc0: mov r2, r6
00432cc4: mov r3, r7
00432cc8: bl #0x30eab4
00432ccc: strd r0, r1, [sp]
00432cd0: ldr r0, [r5]
00432cd4: bl #0x30e8a4
00432cd8: mov r2, r6
00432cdc: mov r3, r7
00432ce0: bl #0x30eab4
00432ce4: mov r2, r0
00432ce8: mov r3, r1
00432cec: ldrd r0, r1, [sp]
00432cf0: bl #0x30df20
00432cf4: movw r2, #0xc1f8
00432cf8: movw r3, #0xa5dc
00432cfc: movt r2, #0x1a63
00432d00: movt r3, #0x404c
00432d04: bl #0x30eab4
00432d08: mov r2, #0
00432d0c: mov r6, r0
00432d10: mov r7, r1
00432d14: mov r0, r8
00432d18: mov r1, sb
00432d1c: mov r3, #0
00432d20: bl #0x30e760
00432d24: cmp r0, #0
00432d28: beq #0x432d4c
00432d2c: movw r3, #0x8000
00432d30: mov r0, r8
00432d34: mov r1, sb
00432d38: mov r2, #0
00432d3c: movt r3, #0x4076
00432d40: bl #0x30eb44
00432d44: mov r8, r0
00432d48: mov sb, r1
00432d4c: mov r0, sl
00432d50: mov r1, fp
00432d54: mov r2, #0
00432d58: mov r3, #0
00432d5c: bl #0x30e760
00432d60: cmp r0, #0
00432d64: beq #0x432d88
00432d68: movw r3, #0x8000
00432d6c: mov r0, sl
00432d70: mov r1, fp
00432d74: mov r2, #0
00432d78: movt r3, #0x4076
00432d7c: bl #0x30eb44
00432d80: mov sl, r0
00432d84: mov fp, r1
00432d88: mov r0, r6
00432d8c: mov r1, r7
00432d90: mov r2, #0
00432d94: mov r3, #0
00432d98: bl #0x30e760
00432d9c: cmp r0, #0
00432da0: beq #0x432dc4
00432da4: movw r3, #0x8000
00432da8: mov r0, r6
00432dac: mov r1, r7
00432db0: mov r2, #0
00432db4: movt r3, #0x4076
00432db8: bl #0x30eb44
00432dbc: mov r6, r0
00432dc0: mov r7, r1
00432dc4: mov r1, sb
00432dc8: mov r0, r8
00432dcc: bl #0x30e6a0
00432dd0: mov r1, fp
00432dd4: str r0, [r4]
00432dd8: mov r0, sl
00432ddc: bl #0x30e6a0
00432de0: mov r1, r7
00432de4: str r0, [r4, #4]
00432de8: mov r0, r6
00432dec: bl #0x30e6a0
00432df0: str r0, [r4, #8]
00432df4: mov r0, r4
00432df8: add sp, sp, #0xc
00432dfc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00432e00: ldr r0, [r5, #0x10]
00432e04: mov r8, #0
00432e08: mov sb, #0
00432e0c: add r0, r0, #0x80000000
00432e10: bl #0x30e8a4
00432e14: mov r6, r0
00432e18: ldr r0, [r5, #0x14]
00432e1c: mov r7, r1
00432e20: bl #0x30e8a4
00432e24: mov r2, r0
00432e28: mov r3, r1
00432e2c: mov r0, r6
00432e30: mov r1, r7
00432e34: bl #0x30df20
00432e38: movw r2, #0xc1f8
00432e3c: movw r3, #0xa5dc
00432e40: movt r2, #0x1a63
00432e44: movt r3, #0x404c
00432e48: bl #0x30eab4
00432e4c: mov r6, r0
00432e50: mov r7, r1
00432e54: b #0x432d4c

# 0x4727ac _ZN12VisualObject10SetScalingERK7Point3DIfE
004727ac: push {r4, r5, r6, r7, r8, lr}
004727b0: ldr r3, [r0, #8]
004727b4: sub sp, sp, #0x10
004727b8: mov r5, r0
004727bc: cmp r3, #0
004727c0: mov r4, r1
004727c4: beq #0x47282c
004727c8: mov r0, r3
004727cc: ldr r3, [r3]
004727d0: mov lr, pc
004727d4: ldr pc, [r3, #0x90]
004727d8: ldr r7, [r4]
004727dc: ldr r1, [r0]
004727e0: mov r6, r0
004727e4: mov r0, r7
004727e8: bl #0x30df8c
004727ec: cmp r0, #0
004727f0: ldr r8, [r4, #8]
004727f4: ldr r4, [r4, #4]
004727f8: bne #0x472834
004727fc: ldr r0, [r5, #8]
00472800: add r1, sp, #4
00472804: ldr r3, [r0]
00472808: ldr r3, [r3, #0x94]
0047280c: str r7, [sp, #4]
00472810: str r4, [sp, #8]
00472814: str r8, [sp, #0xc]
00472818: blx r3
0047281c: mov r0, r5
00472820: bl #0x47211c
00472824: mov r0, r5
00472828: bl #0x470a54
0047282c: add sp, sp, #0x10
00472830: pop {r4, r5, r6, r7, r8, pc}
00472834: mov r0, r4
00472838: ldr r1, [r6, #4]
0047283c: bl #0x30df8c
00472840: cmp r0, #0
00472844: beq #0x4727fc
00472848: ldr r1, [r6, #8]
0047284c: mov r0, r8
00472850: bl #0x30df8c
00472854: cmp r0, #0
00472858: bne #0x47282c
0047285c: b #0x4727fc

# 0x472874 _ZN12VisualObject11SetRotationERK7Point3DIfE
00472874: push {r4, r5, r6, lr}
00472878: ldr r3, [r0, #8]
0047287c: sub sp, sp, #0x10
00472880: mov r4, r0
00472884: cmp r3, #0
00472888: beq #0x472900
0047288c: ldr r2, [r1]
00472890: ldr r3, [r1, #8]
00472894: mov r0, sp
00472898: add r2, r2, #0x80000000
0047289c: ldr r1, [r1, #4]
004728a0: add r3, r3, #0x80000000
004728a4: bl #0x35c9d8
004728a8: ldr r3, [r4, #8]
004728ac: mov r5, sp
004728b0: mov r0, r3
004728b4: ldr r3, [r3]
004728b8: mov lr, pc
004728bc: ldr pc, [r3, #0x98]
004728c0: ldr r1, [sp]
004728c4: mov r6, r0
004728c8: ldr r0, [r0]
004728cc: bl #0x30df8c
004728d0: cmp r0, #0
004728d4: bne #0x472908
004728d8: ldr r3, [r4, #8]
004728dc: mov r1, sp
004728e0: mov r0, r3
004728e4: ldr r3, [r3]
004728e8: mov lr, pc
004728ec: ldr pc, [r3, #0x9c]
004728f0: mov r0, r4
004728f4: bl #0x47211c
004728f8: mov r0, r4
004728fc: bl #0x470a54
00472900: add sp, sp, #0x10
00472904: pop {r4, r5, r6, pc}
00472908: ldr r0, [r6, #4]
0047290c: ldr r1, [sp, #4]
00472910: bl #0x30df8c
00472914: cmp r0, #0
00472918: beq #0x4728d8
0047291c: ldr r0, [r6, #8]
00472920: ldr r1, [sp, #8]
00472924: bl #0x30df8c
00472928: cmp r0, #0
0047292c: beq #0x4728d8
00472930: ldr r0, [r6, #0xc]
00472934: ldr r1, [sp, #0xc]
00472938: bl #0x30df8c
0047293c: cmp r0, #0
00472940: bne #0x472900
00472944: b #0x4728d8
