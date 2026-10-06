0036c7b0 push {r4, r5, r6, r7, r8, sl, lr}
0036c7b4 ldr r5, [pc, #0x294]
0036c7b8 ldr r6, [pc, #0x294]
0036c7bc mov r4, r0
0036c7c0 add r5, pc, r5
0036c7c4 ldr r3, [r5, r6]
0036c7c8 mov r7, #0
0036c7cc add r2, r0, #0x38
0036c7d0 ldr r3, [r3]
0036c7d4 mvn r1, #0
0036c7d8 mov r8, #1
0036c7dc str r1, [r0, #0x2c]
0036c7e0 sub sp, sp, #0x214
0036c7e4 mov r0, r2
0036c7e8 str r1, [r4, #0x24]
0036c7ec str r1, [r4, #0x28]
0036c7f0 str r2, [r4, #0x48]
0036c7f4 str r2, [r4, #0x4c]
0036c7f8 str r7, [r4]
0036c7fc str r7, [r4, #0xc]
0036c800 str r7, [r4, #0x10]
0036c804 str r7, [r4, #0x14]
0036c808 strb r8, [r4, #0x18]
0036c80c str r7, [r4, #0x1c]
0036c810 strb r7, [r4, #0x20]
0036c814 strb r8, [r4, #0x30]
0036c818 strb r8, [r4, #0x31]
0036c81c strb r7, [r4, #0x32]
0036c820 strb r7, [r4, #0x33]
0036c824 mov r1, #0x10
0036c828 str r3, [sp, #0x20c]
0036c82c bl #0x31167c
0036c830 ldr r1, [r4, #0x48]
0036c834 mov r2, r4
0036c838 mov r3, #0x3e8
0036c83c strb r7, [r1]
0036c840 ldr r0, [pc, #0x210]
0036c844 mov r1, #0x3f800000
0036c848 str r3, [r4, #0x58]
0036c84c str r3, [r4, #0x54]
0036c850 str r1, [r4, #0x5c]
0036c854 mov r3, r4
0036c858 strb r7, [r4, #0x60]
0036c85c str r7, [r4, #0x64]
0036c860 str r7, [r4, #0x68]
0036c864 str r7, [r4, #0x6c]
0036c868 str r7, [r4, #0x70]
0036c86c str r7, [r4, #0x74]
0036c870 str r7, [r4, #0x78]
0036c874 str r7, [r4, #0x7c]
0036c878 str r7, [r4, #0x80]
0036c87c str r7, [r4, #0x84]
0036c880 str r7, [r4, #0x88]
0036c884 str r7, [r4, #0x8c]
0036c888 str r7, [r4, #0x90]
0036c88c str r7, [r4, #0x98]
0036c890 strb r7, [r2, #0x94]!
0036c894 str r2, [r4, #0xa0]
0036c898 str r2, [r4, #0x9c]
0036c89c str r7, [r4, #0xa4]
0036c8a0 str r7, [r4, #0xb0]
0036c8a4 strb r7, [r3, #0xac]!
0036c8a8 str r3, [r4, #0xb8]
0036c8ac str r3, [r4, #0xb4]
0036c8b0 str r7, [r4, #0xbc]
0036c8b4 add r0, pc, r0
0036c8b8 bl #0x324114
0036c8bc ldr r3, [pc, #0x198]
0036c8c0 ldr r3, [r5, r3]
0036c8c4 ldrb r3, [r3, #0xa8]
0036c8c8 cmp r3, r7
0036c8cc bne #0x36ca14
0036c8d0 ldr r3, [pc, #0x188]
0036c8d4 add r7, sp, #0xc
0036c8d8 mov r0, r7
0036c8dc ldr r3, [r5, r3]
0036c8e0 ldr r1, [r3]
0036c8e4 bl #0x30e520
0036c8e8 mov r0, r7
0036c8ec bl #0x30de54
0036c8f0 ldr r1, [pc, #0x16c]
0036c8f4 mov r2, #0xd
0036c8f8 add r0, r7, r0
0036c8fc add r1, pc, r1
0036c900 bl #0x30e868
0036c904 mov r0, r7
0036c908 bl #0x30de54
0036c90c ldr r1, [pc, #0x154]
0036c910 mov r2, #0xb
0036c914 add r0, r7, r0
0036c918 add r1, pc, r1
0036c91c bl #0x30e868
0036c920 mov r1, r7
0036c924 add r0, r4, #0x64
0036c928 bl #0x88d344
0036c92c ldr r0, [pc, #0x138]
0036c930 add r0, pc, r0
0036c934 bl #0x324114
0036c938 ldr r2, [r4, #0x68]
0036c93c ldr r3, [r4, #0x64]
0036c940 mov r1, #4
0036c944 rsb r3, r3, r2
0036c948 asr r3, r3, #2
0036c94c lsl r2, r3, r1
0036c950 rsb r2, r3, r2
0036c954 add r2, r2, r2, lsl #8
0036c958 add r2, r2, r2, lsl #16
0036c95c add r3, r3, r2, lsl #4
0036c960 str r3, [r4, #0x1c]
0036c964 lsl r0, r3, #2
0036c968 bl #0x31056c
0036c96c ldr r2, [r4, #0x1c]
0036c970 mov r1, #0
0036c974 str r0, [r4, #4]
0036c978 lsl r2, r2, #2
0036c97c bl #0x30e460
0036c980 ldr r0, [pc, #0xe8]
0036c984 add r0, pc, r0
0036c988 bl #0x324114
0036c98c ldr r0, [r4, #0x1c]
0036c990 mov r1, #4
0036c994 lsl r0, r0, #2
0036c998 bl #0x31056c
0036c99c ldr r2, [r4, #0x1c]
0036c9a0 mov r1, #0
0036c9a4 str r0, [r4, #8]
0036c9a8 lsl r2, r2, #2
0036c9ac bl #0x30e460
0036c9b0 ldr r0, [pc, #0xbc]
0036c9b4 add r0, pc, r0
0036c9b8 bl #0x324114
0036c9bc ldr r0, [pc, #0xb4]
0036c9c0 add r0, pc, r0
0036c9c4 bl #0x324114
0036c9c8 bl #0x862b30
0036c9cc str r0, [r4]
0036c9d0 ldr r3, [r0]
0036c9d4 mov lr, pc
0036c9d8 ldr pc, [r3, #8]
0036c9dc ldr r0, [pc, #0x98]
0036c9e0 add r0, pc, r0
0036c9e4 bl #0x324114
0036c9e8 ldr r0, [pc, #0x90]
0036c9ec add r0, pc, r0
0036c9f0 bl #0x324114
0036c9f4 ldr r3, [r5, r6]
0036c9f8 ldr r2, [sp, #0x20c]
0036c9fc mov r0, r4
0036ca00 ldr r3, [r3]
0036ca04 cmp r2, r3
0036ca08 bne #0x36ca4c
0036ca0c add sp, sp, #0x214
0036ca10 pop {r4, r5, r6, r7, r8, sl, pc}
0036ca14 bl #0x8945a4
0036ca18 ldr r3, [r0]
0036ca1c mov sl, r0
0036ca20 ldr r0, [pc, #0x5c]
0036ca24 ldr r7, [r3, #0x10]
0036ca28 add r0, pc, r0
0036ca2c bl #0x56e064
0036ca30 mov r2, r8
0036ca34 mov r1, r0
0036ca38 str r8, [sp]
0036ca3c mov r0, sl
0036ca40 mov r3, r8
0036ca44 blx r7
0036ca48 b #0x36c8d0
0036ca4c bl #0x30e310
0036ca50 .byte 0xd0, 0x82, 0x62, 0x00
0036ca54 andeq r4, r0, ip, lsr #1
0036ca58 subseq r4, r5, ip, lsl sl
0036ca5c strdeq r3, r4, [r0], -r4
0036ca60 andeq r0, r0, r0, lsl #12
0036ca64 subseq r4, r5, r4, ror r6
0036ca68 subseq r4, r5, r0, lsr sl
0036ca6c subseq r4, r5, r8, lsr #20
0036ca70 subseq r4, r5, r4, asr #20
0036ca74 subseq r4, r5, r4, lsl #21
0036ca78 subseq r4, r5, r8, ror #21
0036ca7c subseq r4, r5, r8, lsr fp
0036ca80 .byte 0x94, 0x4b, 0x55, 0x00
0036ca84 subseq r4, r5, r8, lsl #18
