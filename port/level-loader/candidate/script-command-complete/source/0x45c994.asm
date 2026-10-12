0045c994 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045c998 cc409fe5 ldr r4, [pc, #0xcc]
0045c99c cc609fe5 ldr r6, [pc, #0xcc]
0045c9a0 cc209fe5 ldr r2, [pc, #0xcc]
0045c9a4 04408fe0 add r4, pc, r4
0045c9a8 063094e7 ldr r3, [r4, r6]
0045c9ac 028094e7 ldr r8, [r4, r2]
0045c9b0 20d04de2 sub sp, sp, #0x20
0045c9b4 003093e5 ldr r3, [r3]
0045c9b8 0800a0e1 mov r0, r8
0045c9bc 04508de2 add r5, sp, #4
0045c9c0 1c308de5 str r3, [sp, #0x1c]
0045c9c4 af6bfbeb bl #0x337888
0045c9c8 a8109fe5 ldr r1, [pc, #0xa8]
0045c9cc 0d20a0e1 mov r2, sp
0045c9d0 0500a0e1 mov r0, r5
0045c9d4 01108fe0 add r1, pc, r1
0045c9d8 9c709fe5 ldr r7, [pc, #0x9c]
0045c9dc c2ddfaeb bl #0x3140ec
0045c9e0 0510a0e1 mov r1, r5
0045c9e4 0800a0e1 mov r0, r8
0045c9e8 266cfbeb bl #0x337a88
0045c9ec 0500a0e1 mov r0, r5
0045c9f0 17eefaeb bl #0x318254
0045c9f4 070094e7 ldr r0, [r4, r7]
0045c9f8 e50afbeb bl #0x31f594
0045c9fc 005050e2 subs r5, r0, #0
0045ca00 0400000a beq #0x45ca18
0045ca04 0010a0e3 mov r1, #0
0045ca08 0a4afeeb bl #0x3ef238
0045ca0c 0500a0e1 mov r0, r5
0045ca10 0010a0e3 mov r1, #0
0045ca14 e04efeeb bl #0x3f059c
0045ca18 073094e7 ldr r3, [r4, r7]
0045ca1c 0010a0e3 mov r1, #0
0045ca20 0120a0e3 mov r2, #1
0045ca24 400093e5 ldr r0, [r3, #0x40]
0045ca28 9246fceb bl #0x36e478
0045ca2c 605690e5 ldr r5, [r0, #0x660]
0045ca30 000055e3 cmp r5, #0
0045ca34 0400000a beq #0x45ca4c
0045ca38 0500a0e1 mov r0, r5
0045ca3c 0010a0e3 mov r1, #0
0045ca40 4a7bfdeb bl #0x3bb770
0045ca44 0500a0e1 mov r0, r5
0045ca48 967efdeb bl #0x3bc4a8
0045ca4c 063094e7 ldr r3, [r4, r6]
0045ca50 1c209de5 ldr r2, [sp, #0x1c]
0045ca54 003093e5 ldr r3, [r3]
0045ca58 030052e1 cmp r2, r3
0045ca5c 0100001a bne #0x45ca68
0045ca60 20d08de2 add sp, sp, #0x20
0045ca64 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045ca68 28c6faeb bl #0x30e310
0045ca6c ec805300 subseq r8, r3, ip, ror #1
0045ca70 ac400000 andeq r4, r0, ip, lsr #1
0045ca74 84080000 andeq r0, r0, r4, lsl #17
0045ca78 ac064700 subeq r0, r7, ip, lsr #13
0045ca7c f4370000 strdeq r3, r4, [r0], -r4
