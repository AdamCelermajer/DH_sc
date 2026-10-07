_ZN14PlayerSavegame17m_difficultyLevelE 009a6060
_ZN9Character20SG_GetGameDifficultyEv 003bb8e4
003bb8e4 movw r3, #0x14e8
003bb8e8 ldr r2, [r0, r3]
003bb8ec ldr r3, [pc, #0x1c]
003bb8f0 cmp r2, #0
003bb8f4 add r3, pc, r3
003bb8f8 mvneq r0, #0
003bb8fc bxeq lr
003bb900 ldr r2, [pc, #0xc]
003bb904 ldr r3, [r3, r2]
003bb908 ldr r0, [r3]
003bb90c bx lr
_ZN9Character28SG_GetGameDifficultyUnlockedEv 003bb918
003bb918 movw r3, #0x14e8
003bb91c ldr r3, [r0, r3]
003bb920 cmp r3, #0
003bb924 mvneq r0, #0
003bb928 ldrne r0, [r3, #0x3c]
003bb92c bx lr
_Z26NativeSetCurrentDifficultyRKN7gameswf7fn_callE 0043cd68
0043cd68 push {r4, lr}
0043cd6c ldr r2, [r0, #0xc]
0043cd70 ldr r3, [pc, #0x2c]
0043cd74 ldr r0, [r0, #0x14]
0043cd78 ldr r1, [r2]
0043cd7c ldr r2, [pc, #0x24]
0043cd80 add r3, pc, r3
0043cd84 mov ip, #0xc
0043cd88 ldr r2, [r3, r2]
0043cd8c mla r0, ip, r0, r1
0043cd90 ldr r4, [r2, #0x4c]
0043cd94 bl #0x797a54
0043cd98 bl #0x30ea24
0043cd9c str r0, [r4, #0xc]
0043cda0 pop {r4, pc}
0043cda4 subseq r7, r5, r0, lsl sp
0043cda8 strdeq r3, r4, [r0], -r4
