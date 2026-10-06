
# 0x461668
00461668: push {r4, r5, lr}
0046166c: ldr r2, [r1, #0x10]
00461670: ldr r3, [r1, #0x14]
00461674: sub sp, sp, #0xc
00461678: mov r5, r1
0046167c: rsb r3, r3, r2
00461680: add r1, sp, #8
00461684: add r3, r3, #1
00461688: str r3, [r1, #-4]!
0046168c: mov r4, r0
00461690: bl #0x38b808
00461694: ldr r2, [sp, #4]
00461698: mov r0, r4
0046169c: ldr r1, [r5, #0x14]
004616a0: asr r3, r2, #0x1f
004616a4: ldr ip, [r4]
004616a8: mov lr, pc
004616ac: ldr pc, [ip, #0x1c]
004616b0: add sp, sp, #0xc
004616b4: pop {r4, r5, pc}

# 0x461770
00461770: str lr, [sp, #-4]!
00461774: mov r3, #0

# 0x4616c0
004616c0: str lr, [sp, #-4]!
004616c4: mov r3, #0
004616c8: sub sp, sp, #0xc
004616cc: ldr ip, [r0]
004616d0: mov r2, #8
004616d4: mov lr, pc
004616d8: ldr pc, [ip, #0x1c]
004616dc: ldr r3, [pc, #0x74]
004616e0: cmp r0, #8
004616e4: add r3, pc, r3
004616e8: beq #0x461718

# 0x461828
00461828: str lr, [sp, #-4]!
0046182c: mov r3, #0
00461830: sub sp, sp, #0xc
00461834: ldr ip, [r0]
00461838: mov r2, #8
0046183c: mov lr, pc
00461840: ldr pc, [ip, #0x18]
00461844: ldr r3, [pc, #0x74]
00461848: cmp r0, #8

# 0x461da8
00461da8: push {r4, r5, lr}
00461dac: sub sp, sp, #0x14
00461db0: mov r4, r1
00461db4: add r1, sp, #0xc
00461db8: mov r5, r0
00461dbc: bl #0x38b758
00461dc0: ldr r1, [sp, #0xc]
00461dc4: ldr r3, [pc, #0xac]
00461dc8: cmp r1, #0
00461dcc: add r3, pc, r3
00461dd0: ble #0x461e04
00461dd4: sub r1, r1, #1
00461dd8: mov r0, r4
00461ddc: bl #0x461ca8
00461de0: ldr r2, [sp, #0xc]
00461de4: mov r0, r5
00461de8: ldr r1, [r4, #0x14]
00461dec: asr r3, r2, #0x1f
00461df0: ldr ip, [r5]
00461df4: mov lr, pc
00461df8: ldr pc, [ip, #0x18]
00461dfc: add sp, sp, #0x14
00461e00: pop {r4, r5, pc}
00461e04: ldr r2, [pc, #0x70]
00461e08: ldr r2, [r3, r2]
00461e0c: ldr r2, [r2]
00461e10: cmp r2, #2
00461e14: moveq r3, #0
00461e18: streq r3, [r3]
00461e1c: beq #0x461e28
00461e20: cmp r2, #1
00461e24: beq #0x461e38
00461e28: mov r0, r4
00461e2c: mov r1, #0
00461e30: bl #0x461ca8
00461e34: b #0x461dfc
00461e38: ldr r0, [pc, #0x40]
00461e3c: ldr r1, [pc, #0x40]
00461e40: ldr r2, [pc, #0x40]
00461e44: ldr r0, [r3, r0]
00461e48: ldr r3, [pc, #0x3c]
00461e4c: add r1, pc, r1
00461e50: mov ip, #0x57
00461e54: add r0, r0, #0xa8
00461e58: add r2, pc, r2

# 0x3139e0
003139e0: str lr, [sp, #-4]!
003139e4: mov r3, #0
003139e8: sub sp, sp, #0xc
003139ec: ldr ip, [r0]
003139f0: mov r2, #4
003139f4: mov lr, pc
003139f8: ldr pc, [ip, #0x1c]
003139fc: ldr r3, [pc, #0x74]
00313a00: cmp r0, #4
00313a04: add r3, pc, r3
00313a08: beq #0x313a38
00313a0c: ldr r2, [pc, #0x68]

# 0x313b48
00313b48: str lr, [sp, #-4]!
00313b4c: mov r3, #0
00313b50: sub sp, sp, #0xc
00313b54: ldr ip, [r0]
00313b58: mov r2, #4
00313b5c: mov lr, pc
00313b60: ldr pc, [ip, #0x18]
00313b64: ldr r3, [pc, #0x74]
00313b68: cmp r0, #4
00313b6c: add r3, pc, r3
00313b70: beq #0x313ba0
00313b74: ldr r2, [pc, #0x68]
