0045f400 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045f404 c8409fe5 ldr r4, [pc, #0xc8]
0045f408 c8509fe5 ldr r5, [pc, #0xc8]
0045f40c c8109fe5 ldr r1, [pc, #0xc8]
0045f410 04408fe0 add r4, pc, r4
0045f414 053094e7 ldr r3, [r4, r5]
0045f418 018094e7 ldr r8, [r4, r1]
0045f41c 38d04de2 sub sp, sp, #0x38
0045f420 003093e5 ldr r3, [r3]
0045f424 0290a0e1 mov sb, r2
0045f428 1c708de2 add r7, sp, #0x1c
0045f42c 34308de5 str r3, [sp, #0x34]
0045f430 0ca090e5 ldr sl, [r0, #0xc]
0045f434 0800a0e1 mov r0, r8
0045f438 1261fbeb bl #0x337888
0045f43c 9c109fe5 ldr r1, [pc, #0x9c]
0045f440 18208de2 add r2, sp, #0x18
0045f444 0700a0e1 mov r0, r7
0045f448 01108fe0 add r1, pc, r1
0045f44c 26d3faeb bl #0x3140ec
0045f450 0710a0e1 mov r1, r7
0045f454 0800a0e1 mov r0, r8
0045f458 8a61fbeb bl #0x337a88
0045f45c 0700a0e1 mov r0, r7
0045f460 7be3faeb bl #0x318254
0045f464 78109fe5 ldr r1, [pc, #0x78]
0045f468 0c608de2 add r6, sp, #0xc
0045f46c 0c209ae5 ldr r2, [sl, #0xc]
0045f470 011094e7 ldr r1, [r4, r1]
0045f474 0930a0e1 mov r3, sb
0045f478 0070a0e3 mov r7, #0
0045f47c 381091e5 ldr r1, [r1, #0x38]
0045f480 0600a0e1 mov r0, r6
0045f484 00708de5 str r7, [sp]
0045f488 04708de5 str r7, [sp, #4]
0045f48c 03aefbeb bl #0x34aca0
0045f490 0600a0e1 mov r0, r6
0045f494 ae82fbeb bl #0x33ff54
0045f498 000050e3 cmp r0, #0
0045f49c 0400000a beq #0x45f4b4
0045f4a0 4f0e80e2 add r0, r0, #0x4f0
0045f4a4 0710a0e1 mov r1, r7
0045f4a8 0c0080e2 add r0, r0, #0xc
0045f4ac 0720a0e1 mov r2, r7
0045f4b0 9f8cfdeb bl #0x3c2734
0045f4b4 053094e7 ldr r3, [r4, r5]
0045f4b8 34209de5 ldr r2, [sp, #0x34]
0045f4bc 003093e5 ldr r3, [r3]
0045f4c0 030052e1 cmp r2, r3
0045f4c4 0100001a bne #0x45f4d0
0045f4c8 38d08de2 add sp, sp, #0x38
0045f4cc f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045f4d0 8ebbfaeb bl #0x30e310
0045f4d4 80565300 subseq r5, r3, r0, lsl #13
0045f4d8 ac400000 andeq r4, r0, ip, lsr #1
0045f4dc 84080000 andeq r0, r0, r4, lsl #17
0045f4e0 38dc4600 subeq sp, r6, r8, lsr ip
0045f4e4 f4370000 strdeq r3, r4, [r0], -r4
