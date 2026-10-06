_Z16NativeEndLoadingRKN7gameswf7fn_callE 0x43ab00
0043ab00: ldr r3, [pc, #0x30]
0043ab04: ldr r2, [pc, #0x30]
0043ab08: push {r4, lr}
0043ab0c: add r3, pc, r3
0043ab10: ldr r0, [r3, r2]
0043ab14: bl #0x31f594
0043ab18: cmp r0, #0
0043ab1c: beq #0x43ab34
0043ab20: ldr r3, [r0, #0x130]
0043ab24: cmp r3, #0x24
0043ab28: ldreq r3, [r0, #0x130]
0043ab2c: addeq r3, r3, #1
0043ab30: streq r3, [r0, #0x130]
0043ab34: pop {r4, pc}
0043ab38: subseq sb, r5, r4, lsl #31
0043ab3c: strdeq r3, r4, [r0], -r4
_Z24NativeGetLoadingProgressRKN7gameswf7fn_callE 0x43fa58
0043fa58: push {r4, r5, r6, lr}
0043fa5c: ldr r3, [pc, #0x6c]
0043fa60: ldr r2, [pc, #0x6c]
0043fa64: sub sp, sp, #0x60
0043fa68: add r3, pc, r3
0043fa6c: mov r4, r0
0043fa70: ldr r0, [r3, r2]
0043fa74: bl #0x31f594
0043fa78: cmp r0, #0
0043fa7c: ldrne r0, [r0, #0x30]
0043fa80: beq #0x43faa4
0043fa84: bl #0x30ed30
0043fa88: ldr r4, [r4]
0043fa8c: mov r2, r0
0043fa90: mov r3, r1
0043fa94: mov r0, r4
0043fa98: bl #0x797488
0043fa9c: add sp, sp, #0x60
0043faa0: pop {r4, r5, r6, pc}
0043faa4: add r5, sp, #4
0043faa8: mov r0, r5
0043faac: bl #0x4a0040
0043fab0: bl #0x320e98
0043fab4: ldr r6, [r0, #0x34]
0043fab8: mov r0, r5
0043fabc: bl #0x43f9c4
0043fac0: cmp r6, #3
0043fac4: movne r0, #0x64
0043fac8: moveq r0, #0
0043facc: b #0x43fa84
0043fad0: subseq r5, r5, r8, lsr #32
0043fad4: strdeq r3, r4, [r0], -r4
