0045c438 88309fe5 ldr r3, [pc, #0x88]
0045c43c 88209fe5 ldr r2, [pc, #0x88]
0045c440 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045c444 03308fe0 add r3, pc, r3
0045c448 026093e7 ldr r6, [r3, r2]
0045c44c 7c209fe5 ldr r2, [pc, #0x7c]
0045c450 20d04de2 sub sp, sp, #0x20
0045c454 0040a0e1 mov r4, r0
0045c458 027093e7 ldr r7, [r3, r2]
0045c45c 002096e5 ldr r2, [r6]
0045c460 0c8094e5 ldr r8, [r4, #0xc]
0045c464 0700a0e1 mov r0, r7
0045c468 1c208de5 str r2, [sp, #0x1c]
0045c46c 056dfbeb bl #0x337888
0045c470 5c109fe5 ldr r1, [pc, #0x5c]
0045c474 04508de2 add r5, sp, #4
0045c478 0d20a0e1 mov r2, sp
0045c47c 01108fe0 add r1, pc, r1
0045c480 0500a0e1 mov r0, r5
0045c484 18dffaeb bl #0x3140ec
0045c488 0510a0e1 mov r1, r5
0045c48c 0700a0e1 mov r0, r7
0045c490 7c6dfbeb bl #0x337a88
0045c494 0500a0e1 mov r0, r5
0045c498 6deffaeb bl #0x318254
0045c49c 0030a0e3 mov r3, #0
0045c4a0 103084e5 str r3, [r4, #0x10]
0045c4a4 083098e5 ldr r3, [r8, #8]
0045c4a8 1c209de5 ldr r2, [sp, #0x1c]
0045c4ac 143084e5 str r3, [r4, #0x14]
0045c4b0 003096e5 ldr r3, [r6]
0045c4b4 030052e1 cmp r2, r3
0045c4b8 0100001a bne #0x45c4c4
0045c4bc 20d08de2 add sp, sp, #0x20
0045c4c0 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045c4c4 91c7faeb bl #0x30e310
0045c4c8 4c865300 subseq r8, r3, ip, asr #12
0045c4cc ac400000 andeq r4, r0, ip, lsr #1
0045c4d0 84080000 andeq r0, r0, r4, lsl #17
0045c4d4 040c4700 subeq r0, r7, r4, lsl #24
