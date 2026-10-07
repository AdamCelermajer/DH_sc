
SOURCE _ZN14PlayerSavegame20SG_GetFilenamePrefixEv
004634e4 ldr r0, [pc, #4]
004634e8 add r0, pc, r0
004634ec bx lr
004634f0 subeq fp, r5, r0, ror r0

SOURCE _ZN14PlayerSavegame14SG_GetFilenameEjRSsbb
00463c84 push {r4, r5, r6, r7, r8, sb, sl, lr}
00463c88 ldr r4, [pc, #0xc4]
00463c8c ldr r6, [pc, #0xc4]
00463c90 mov r5, r2
00463c94 add r4, pc, r4
00463c98 ldr r2, [r4, r6]
00463c9c sub sp, sp, #0x50
00463ca0 mov r8, r0
00463ca4 ldr r2, [r2]
00463ca8 mov r7, r1
00463cac mov sl, r3
00463cb0 str r2, [sp, #0x4c]
00463cb4 bl #0x4634e4
00463cb8 cmp r5, #0
00463cbc mov sb, r0
00463cc0 beq #0x463ce0
00463cc4 cmp sl, #0
00463cc8 bne #0x463d44
00463ccc ldr sl, [pc, #0x88]
00463cd0 add sl, pc, sl
00463cd4 bl #0x463504
00463cd8 mov ip, r0
00463cdc b #0x463cf0
00463ce0 bl #0x4634f4
00463ce4 ldr sl, [pc, #0x74]
00463ce8 mov ip, r0
00463cec add sl, pc, sl
00463cf0 ldr r1, [pc, #0x6c]
00463cf4 add r5, sp, #0xc
00463cf8 mov r3, r8
00463cfc mov r2, sb
00463d00 add r1, pc, r1
00463d04 mov r0, r5
00463d08 stm sp, {sl, ip}
00463d0c bl #0x30eae4
00463d10 mov r0, r5
00463d14 bl #0x30de54
00463d18 mov r1, r5
00463d1c add r2, r5, r0
00463d20 mov r0, r7
00463d24 bl #0x3109e0
00463d28 ldr r3, [r4, r6]
00463d2c ldr r2, [sp, #0x4c]
00463d30 ldr r3, [r3]
00463d34 cmp r2, r3
00463d38 bne #0x463d50
00463d3c add sp, sp, #0x50
00463d40 pop {r4, r5, r6, r7, r8, sb, sl, pc}
00463d44 ldr sl, [pc, #0x1c]
00463d48 add sl, pc, sl
00463d4c b #0x463cd4
00463d50 bl #0x30e310
00463d54 ldrsheq r0, [r3], #-0xdc
00463d58 andeq r4, r0, ip, lsr #1
00463d5c umaaleq sb, r6, r8, r4
00463d60 subeq r7, r6, ip, lsl fp
00463d64 subeq sb, r6, r8, asr #9
00463d68 subeq sb, r6, r8, lsr #8

SOURCE _ZN14PlayerSavegame33SG_GetCheckpointFilenameExtensionEv
00463504 ldr r0, [pc, #4]
00463508 add r0, pc, r0
0046350c bx lr
00463510 strheq sb, [r6], #-0xc0

SOURCE _ZN14PlayerSavegame23SG_GetFilenameExtensionEv
004634f4 ldr r0, [pc, #4]
004634f8 add r0, pc, r0
004634fc bx lr
00463500 subeq sp, r5, r8, lsl #5

SOURCE _ZNK14PlayerSavegame18SG_GetSavefileNameEv
004635a4 ldr r3, [r0, #8]
004635a8 ldr r0, [r3, #0x18]
004635ac bx lr
