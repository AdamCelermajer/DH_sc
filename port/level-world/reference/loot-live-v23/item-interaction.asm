# 0x3ec048 _ZN10ItemObject17OnCollisionBeginsEP9Character
003ec048: push {r4, r5, r6, r7, r8, lr}
003ec04c: ldr r4, [pc, #0x94]
003ec050: subs r6, r1, #0
003ec054: mov r5, r0
003ec058: add r4, pc, r4
003ec05c: beq #0x3ec06c
003ec060: bl #0x3ebffc
003ec064: subs r7, r0, #0
003ec068: beq #0x3ec070
003ec06c: pop {r4, r5, r6, r7, r8, pc}
003ec070: ldr r3, [r5]
003ec074: mov r0, r5
003ec078: mov r1, r6
003ec07c: mov lr, pc
003ec080: ldr pc, [r3, #0x90]
003ec084: cmn r0, #1
003ec088: beq #0x3ec0c8
003ec08c: movw r3, #0x14a4
003ec090: ldr r3, [r6, r3]
003ec094: cmp r5, r3
003ec098: bne #0x3ec06c
003ec09c: ldr r3, [pc, #0x48]
003ec0a0: mov r1, r6
003ec0a4: ldr r3, [r4, r3]
003ec0a8: ldr r0, [r3, #0x40]
003ec0ac: bl #0x36effc
003ec0b0: cmp r0, #0
003ec0b4: beq #0x3ec06c
003ec0b8: mov r0, r5
003ec0bc: str r6, [r5, #0x3c4]
003ec0c0: pop {r4, r5, r6, r7, r8, lr}
003ec0c4: b #0x3ebd5c
003ec0c8: add r0, r6, #0x4f0
003ec0cc: add r0, r0, #0xc
003ec0d0: mov r1, r7
003ec0d4: bl #0x3c029c
003ec0d8: cmp r0, #0
003ec0dc: beq #0x3ec06c
003ec0e0: str r6, [r5, #0x2e4]
003ec0e4: pop {r4, r5, r6, r7, r8, pc}
003ec0e8: subseq r8, sl, r8, lsr sl
003ec0ec: strdeq r3, r4, [r0], -r4

# 0x3ebeb4 _ZNK10ItemObject18GetInteractionTypeEP10GameObject
003ebeb4: push {r4, lr}
003ebeb8: ldr r3, [r0]
003ebebc: mov r4, r0
003ebec0: mov lr, pc
003ebec4: ldr pc, [r3, #0x88]
003ebec8: cmp r0, #0
003ebecc: beq #0x3ebedc
003ebed0: add r0, r4, #0x374
003ebed4: mov r1, #0
003ebed8: bl #0x3fdbf8
003ebedc: mvn r0, #0
003ebee0: pop {r4, pc}

# 0x36f074 _ZN13PlayerManager20IsLocalPlayerHostingEv
0036f074: push {r4, lr}
0036f078: mov r4, r0
0036f07c: bl #0x7fd794
0036f080: ldrb r3, [r0, #5]
0036f084: cmp r3, #0
0036f088: bne #0x36f0a4
0036f08c: bl #0x7fd794
0036f090: ldrb r3, [r0, #5]
0036f094: cmp r3, #0
0036f098: bne #0x36f0c4
0036f09c: mov r0, #1
0036f0a0: pop {r4, pc}
0036f0a4: bl #0x320e98
0036f0a8: ldr r3, [r0, #0x34]
0036f0ac: sub r3, r3, #3
0036f0b0: cmp r3, #1
0036f0b4: bhi #0x36f08c
0036f0b8: bl #0x800f8c
0036f0bc: pop {r4, lr}
0036f0c0: b #0x81f524
0036f0c4: mov r1, #0
0036f0c8: mov r0, r4
0036f0cc: mov r2, r1
0036f0d0: bl #0x36e478
0036f0d4: pop {r4, lr}
0036f0d8: b #0x80f23c

# 0x36effc _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc: subs r3, r1, #0
0036f000: push {r4, lr}
0036f004: beq #0x36f020
0036f008: mov r2, #0
0036f00c: bl #0x36eea8
0036f010: ldr r3, [r0]
0036f014: mov lr, pc
0036f018: ldr pc, [r3, #0x50]
0036f01c: pop {r4, pc}
0036f020: mov r0, r3
0036f024: pop {r4, pc}

# 0x3ec014 _ZNK10ItemObject13IsInteractiveEP10GameObject
003ec014: push {r4, lr}
003ec018: ldrb r2, [r0, #0x81]
003ec01c: cmp r2, #0
003ec020: bne #0x3ec030
003ec024: ldrb r3, [r0, #0x80]
003ec028: cmp r3, #0
003ec02c: bne #0x3ec038
003ec030: mov r0, #0
003ec034: pop {r4, pc}
003ec038: bl #0x3ebffc
003ec03c: eor r0, r0, #1
003ec040: uxtb r0, r0
003ec044: pop {r4, pc}

# 0x3ebd2c _ZN10ItemObject15OnCollisionEndsEP9Character
003ebd2c: cmp r1, #0
003ebd30: push {r4, lr}
003ebd34: mov r4, r0
003ebd38: beq #0x3ebd58
003ebd3c: movw r3, #0x14a4
003ebd40: ldr r3, [r1, r3]
003ebd44: cmp r0, r3
003ebd48: beq #0x3ebd58
003ebd4c: bl #0x3ebcf8
003ebd50: mov r3, #0
003ebd54: str r3, [r4, #0x3c4]
003ebd58: pop {r4, pc}
