0045a2ec 70402de9 push {r4, r5, r6, lr}
0045a2f0 84c09fe5 ldr ip, [pc, #0x84]
0045a2f4 84309fe5 ldr r3, [pc, #0x84]
0045a2f8 0c5090e5 ldr r5, [r0, #0xc]
0045a2fc 0cc08fe0 add ip, pc, ip
0045a300 03309ce7 ldr r3, [ip, r3]
0045a304 18d04de2 sub sp, sp, #0x18
0045a308 0c608de2 add r6, sp, #0xc
0045a30c 381093e5 ldr r1, [r3, #0x38]
0045a310 182095e5 ldr r2, [r5, #0x18]
0045a314 0040a0e3 mov r4, #0
0045a318 0030e0e3 mvn r3, #0
0045a31c 0600a0e1 mov r0, r6
0045a320 00408de5 str r4, [sp]
0045a324 04408de5 str r4, [sp, #4]
0045a328 5cc2fbeb bl #0x34aca0
0045a32c 0600a0e1 mov r0, r6
0045a330 0410a0e1 mov r1, r4
0045a334 a196fbeb bl #0x33fdc0
0045a338 040050e1 cmp r0, r4
0045a33c 0100001a bne #0x45a348
0045a340 18d08de2 add sp, sp, #0x18
0045a344 7080bde8 pop {r4, r5, r6, pc}
0045a348 0600a0e1 mov r0, r6
0045a34c 0097fbeb bl #0x33ff54
0045a350 004050e2 subs r4, r0, #0
0045a354 f9ffff0a beq #0x45a340
0045a358 494e84e2 add r4, r4, #0x490
0045a35c 0c4084e2 add r4, r4, #0xc
0045a360 081095e5 ldr r1, [r5, #8]
0045a364 0400a0e1 mov r0, r4
0045a368 e9bdfdeb bl #0x3c9b14
0045a36c 0400a0e1 mov r0, r4
0045a370 0c1095e5 ldr r1, [r5, #0xc]
0045a374 e6bdfdeb bl #0x3c9b14
0045a378 f0ffffea b #0x45a340
