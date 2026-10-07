00865ee0 push {r4, r5, r6, r7, r8, lr}
00865ee4 add r5, r0, #0x3f4
00865ee8 mov r4, r0
00865eec mov r0, r5
00865ef0 mov r6, r1
00865ef4 mov r7, r2
00865ef8 bl #0x89347c
00865efc cmp r6, #2
00865f00 moveq r3, #1
00865f04 mov r0, r5
00865f08 strbeq r3, [r4, #0x436]
00865f0c streq r7, [r4, #0x430]
00865f10 pop {r4, r5, r6, r7, r8, lr}
00865f14 b #0x893478
