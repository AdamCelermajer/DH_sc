0036c2e4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036c2e8 ldr sb, [pc, #0x3fc]
0036c2ec ldr fp, [pc, #0x3fc]
0036c2f0 mov r5, r0
0036c2f4 add sb, pc, sb
0036c2f8 ldr r3, [sb, fp]
0036c2fc ldr r0, [pc, #0x3f0]
0036c300 sub sp, sp, #0x7c
0036c304 ldr r3, [r3]
0036c308 add r0, pc, r0
0036c30c add r4, sp, #0x5c
0036c310 str r3, [sp, #0x74]
0036c314 bl #0x324114
0036c318 ldr r1, [pc, #0x3d8]
0036c31c mov r3, #0
0036c320 add r6, r5, #0x38
0036c324 strb r3, [r5, #0x33]
0036c328 add r1, pc, r1
0036c32c mov r0, r4
0036c330 add r2, sp, #0x40
0036c334 bl #0x3140ec
0036c338 cmp r6, r4
0036c33c beq #0x36c350
0036c340 mov r0, r6
0036c344 ldr r1, [sp, #0x70]
0036c348 ldr r2, [sp, #0x6c]
0036c34c bl #0x3109e0
0036c350 mov r0, r4
0036c354 bl #0x3139ac
0036c358 ldr r0, [pc, #0x39c]
0036c35c add r0, pc, r0
0036c360 bl #0x381744
0036c364 ldr r3, [pc, #0x394]
0036c368 ldr r1, [pc, #0x394]
0036c36c cmp r0, #0
0036c370 ldr r4, [sb, r3]
0036c374 strbeq r0, [r5, #0x33]
0036c378 add r1, pc, r1
0036c37c mov r0, r4
0036c380 bl #0x320e44
0036c384 bl #0x30e964
0036c388 mov r1, #1
0036c38c mov r2, r0
0036c390 mov r0, r5
0036c394 bl #0x369da0
0036c398 ldr r1, [pc, #0x368]
0036c39c mov r0, r4
0036c3a0 add r1, pc, r1
0036c3a4 bl #0x320e44
0036c3a8 bl #0x30e964
0036c3ac mov r1, #2
0036c3b0 mov r2, r0
0036c3b4 mov r0, r5
0036c3b8 bl #0x369da0
0036c3bc ldr r1, [pc, #0x348]
0036c3c0 ldr r0, [r5]
0036c3c4 add r1, pc, r1
0036c3c8 bl #0x861a08
0036c3cc mov r1, #2
0036c3d0 mov r2, #4
0036c3d4 ldr r0, [r5]
0036c3d8 bl #0x861b38
