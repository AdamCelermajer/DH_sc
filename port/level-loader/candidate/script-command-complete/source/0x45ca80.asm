0045ca80 f0452de9 push {r4, r5, r6, r7, r8, sl, lr}
0045ca84 fc409fe5 ldr r4, [pc, #0xfc]
0045ca88 fc309fe5 ldr r3, [pc, #0xfc]
0045ca8c 24d04de2 sub sp, sp, #0x24
0045ca90 04408fe0 add r4, pc, r4
0045ca94 036094e7 ldr r6, [r4, r3]
0045ca98 f0309fe5 ldr r3, [pc, #0xf0]
0045ca9c 04508de2 add r5, sp, #4
0045caa0 037094e7 ldr r7, [r4, r3]
0045caa4 003096e5 ldr r3, [r6]
0045caa8 0700a0e1 mov r0, r7
0045caac 1c308de5 str r3, [sp, #0x1c]
0045cab0 746bfbeb bl #0x337888
0045cab4 d8109fe5 ldr r1, [pc, #0xd8]
0045cab8 0d20a0e1 mov r2, sp
0045cabc 0500a0e1 mov r0, r5
0045cac0 01108fe0 add r1, pc, r1
0045cac4 88ddfaeb bl #0x3140ec
0045cac8 0510a0e1 mov r1, r5
0045cacc 0700a0e1 mov r0, r7
0045cad0 ec6bfbeb bl #0x337a88
0045cad4 0500a0e1 mov r0, r5
0045cad8 ddedfaeb bl #0x318254
0045cadc b4309fe5 ldr r3, [pc, #0xb4]
0045cae0 0010a0e3 mov r1, #0
0045cae4 0120a0e3 mov r2, #1
0045cae8 037094e7 ldr r7, [r4, r3]
0045caec 400097e5 ldr r0, [r7, #0x40]
0045caf0 6046fceb bl #0x36e478
0045caf4 605690e5 ldr r5, [r0, #0x660]
0045caf8 0500a0e1 mov r0, r5
0045cafc 787bfdeb bl #0x3bb8e4
0045cb00 00a0a0e1 mov sl, r0
0045cb04 0500a0e1 mov r0, r5
0045cb08 757bfdeb bl #0x3bb8e4
0045cb0c 2d10a0e3 mov r1, #0x2d
0045cb10 018080e2 add r8, r0, #1
0045cb14 0a20a0e1 mov r2, sl
0045cb18 0500a0e1 mov r0, r5
0045cb1c 4c7bfdeb bl #0x3bb854
0045cb20 020058e3 cmp r8, #2
0045cb24 0280a0a3 movge r8, #2
0045cb28 0a20a0e1 mov r2, sl
0045cb2c 0500a0e1 mov r0, r5
0045cb30 0510a0e3 mov r1, #5
0045cb34 587bfdeb bl #0x3bb89c
0045cb38 0810a0e1 mov r1, r8
0045cb3c 0500a0e1 mov r0, r5
0045cb40 7a7bfdeb bl #0x3bb930
0045cb44 0500a0e1 mov r0, r5
0045cb48 567efdeb bl #0x3bc4a8
0045cb4c 0500a0e1 mov r0, r5
0045cb50 0810a0e1 mov r1, r8
0045cb54 7d7bfdeb bl #0x3bb950
0045cb58 3c309fe5 ldr r3, [pc, #0x3c]
0045cb5c 0020a0e3 mov r2, #0
0045cb60 180097e5 ldr r0, [r7, #0x18]
0045cb64 031094e7 ldr r1, [r4, r3]
0045cb68 0676fbeb bl #0x33a388
0045cb6c 1c209de5 ldr r2, [sp, #0x1c]
0045cb70 003096e5 ldr r3, [r6]
0045cb74 030052e1 cmp r2, r3
0045cb78 0100001a bne #0x45cb84
0045cb7c 24d08de2 add sp, sp, #0x24
0045cb80 f085bde8 pop {r4, r5, r6, r7, r8, sl, pc}
0045cb84 e1c5faeb bl #0x30e310
0045cb88 00805300 subseq r8, r3, r0
0045cb8c ac400000 andeq r4, r0, ip, lsr #1
0045cb90 84080000 andeq r0, r0, r4, lsl #17
0045cb94 c0054700 subeq r0, r7, r0, asr #11
0045cb98 f4370000 strdeq r3, r4, [r0], -r4
0045cb9c 303c0000 andeq r3, r0, r0, lsr ip
