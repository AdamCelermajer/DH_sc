00460b1c f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
00460b20 f8409fe5 ldr r4, [pc, #0xf8]
00460b24 f8309fe5 ldr r3, [pc, #0xf8]
00460b28 f8509fe5 ldr r5, [pc, #0xf8]
00460b2c 04408fe0 add r4, pc, r4
00460b30 03a094e7 ldr sl, [r4, r3]
00460b34 70d04de2 sub sp, sp, #0x70
00460b38 54808de2 add r8, sp, #0x54
00460b3c 00309ae5 ldr r3, [sl]
00460b40 05508fe0 add r5, pc, r5
00460b44 3c708de2 add r7, sp, #0x3c
00460b48 6c308de5 str r3, [sp, #0x6c]
00460b4c 0c6090e5 ldr r6, [r0, #0xc]
00460b50 0510a0e1 mov r1, r5
00460b54 04208de2 add r2, sp, #4
00460b58 0800a0e1 mov r0, r8
00460b5c 62cdfaeb bl #0x3140ec
00460b60 0510a0e1 mov r1, r5
00460b64 0d20a0e1 mov r2, sp
00460b68 08508de2 add r5, sp, #8
00460b6c 0700a0e1 mov r0, r7
00460b70 5dcdfaeb bl #0x3140ec
00460b74 0820a0e1 mov r2, r8
00460b78 0730a0e1 mov r3, r7
00460b7c 0010e0e3 mvn r1, #0
00460b80 0500a0e1 mov r0, r5
00460b84 ee4bffeb bl #0x433b44
00460b88 0700a0e1 mov r0, r7
00460b8c b0ddfaeb bl #0x318254
00460b90 0800a0e1 mov r0, r8
00460b94 aeddfaeb bl #0x318254
00460b98 183096e5 ldr r3, [r6, #0x18]
00460b9c 047085e2 add r7, r5, #4
00460ba0 1c8085e2 add r8, r5, #0x1c
00460ba4 08308de5 str r3, [sp, #8]
00460ba8 0c9096e5 ldr sb, [r6, #0xc]
00460bac 0900a0e1 mov r0, sb
00460bb0 a7b4faeb bl #0x30de54
00460bb4 0910a0e1 mov r1, sb
00460bb8 002089e0 add r2, sb, r0
00460bbc 0700a0e1 mov r0, r7
00460bc0 86bffaeb bl #0x3109e0
00460bc4 146096e5 ldr r6, [r6, #0x14]
00460bc8 0600a0e1 mov r0, r6
00460bcc a0b4faeb bl #0x30de54
00460bd0 0610a0e1 mov r1, r6
00460bd4 002086e0 add r2, r6, r0
00460bd8 0800a0e1 mov r0, r8
00460bdc 7fbffaeb bl #0x3109e0
00460be0 44009fe5 ldr r0, [pc, #0x44]
00460be4 0510a0e1 mov r1, r5
00460be8 000094e7 ldr r0, [r4, r0]
00460bec 040080e2 add r0, r0, #4
00460bf0 ffeaffeb bl #0x45b7f4
00460bf4 0800a0e1 mov r0, r8
00460bf8 95ddfaeb bl #0x318254
00460bfc 0700a0e1 mov r0, r7
00460c00 93ddfaeb bl #0x318254
00460c04 6c209de5 ldr r2, [sp, #0x6c]
00460c08 00309ae5 ldr r3, [sl]
00460c0c 030052e1 cmp r2, r3
00460c10 0100001a bne #0x460c1c
00460c14 70d08de2 add sp, sp, #0x70
00460c18 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
00460c1c bbb5faeb bl #0x30e310
00460c20 643f5300 subseq r3, r3, r4, ror #30
00460c24 ac400000 andeq r4, r0, ip, lsr #1
00460c28 c8ac4600 subeq sl, r6, r8, asr #25
00460c2c 00490000 andeq r4, r0, r0, lsl #18
