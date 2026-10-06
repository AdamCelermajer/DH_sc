_ZN6glitch7collada19CTimelineControllerC1Ev
00666e40 ldr      r1, [pc, #0xa4]
00666e44 ldr      r3, [pc, #0xa4]
00666e48 ldr      r2, [pc, #0xa4]
00666e4c add      r1, pc, r1
00666e50 push     {r4, r5, r6, r7, r8}
00666e54 ldr      r4, [r1, r3]
00666e58 ldr      r2, [r1, r2]
00666e5c mov      r5, #1
00666e60 ldr      ip, [r4, #8]
00666e64 add      r2, r2, #8
00666e68 str      r2, [r0, #0x40]
00666e6c str      ip, [r0]
00666e70 str      r5, [r0, #0x44]
00666e74 ldr      r7, [ip, #-0xc]
00666e78 ldr      r6, [r4, #4]
00666e7c ldr      r8, [r4, #0xc]
00666e80 ldr      ip, [pc, #0x70]
00666e84 mov      r2, #0
00666e88 str      r8, [r0, r7]
00666e8c ldr      ip, [r1, ip]
00666e90 str      r6, [r0]
00666e94 str      r2, [r0, #4]
00666e98 ldr      r7, [r6, #-0xc]
00666e9c ldr      r8, [r4, #0x10]
00666ea0 add      r6, ip, #0x70
00666ea4 add      ip, ip, #0xc
00666ea8 str      r8, [r0, r7]
00666eac mov      r4, #0
00666eb0 str      ip, [r0]
00666eb4 mov      ip, #0x3f800000
00666eb8 strb     r2, [r0, #0x3d]
00666ebc str      r6, [r0, #0x40]
00666ec0 strb     r5, [r0, #0x18]
00666ec4 str      r4, [r0, #0x2c]
00666ec8 str      ip, [r0, #0x30]
00666ecc str      r2, [r0, #8]
00666ed0 str      r4, [r0, #0x20]
00666ed4 str      r4, [r0, #0x24]
00666ed8 str      r2, [r0, #0x34]
00666edc str      r2, [r0, #0x38]
00666ee0 strb     r2, [r0, #0x3c]
00666ee4 pop      {r4, r5, r6, r7, r8}
00666ee8 bx       lr
00666eec eorseq   sp, r2, r4, asr #24
00666ef0 andeq    r2, r0, r0, ror #28
00666ef4 andeq    r2, r0, r4, asr #22
00666ef8 andeq    r2, r0, r4, lsl pc
_ZN6glitch7collada18CSceneNodeAnimatorC1ERKNS0_16CColladaDatabaseERNS0_22SLibraryAnimationClipsE
0065dbd8 push     {r4, r5, r6, r7, r8, lr}
0065dbdc ldr      r5, [pc, #0x1a8]
0065dbe0 ldr      ip, [pc, #0x1a8]
0065dbe4 ldr      r3, [pc, #0x1a8]
0065dbe8 add      r5, pc, r5
0065dbec ldr      ip, [r5, ip]
0065dbf0 ldr      r3, [r5, r3]
0065dbf4 mov      lr, #1
0065dbf8 add      ip, ip, #8
0065dbfc mov      r6, r1
0065dc00 str      lr, [r0, #0x5c]
0065dc04 add      r1, r3, #4
0065dc08 str      ip, [r0, #0x58]
0065dc0c mov      r4, r0
0065dc10 mov      r8, r2
0065dc14 bl       #0x6698fc ; _ZN6glitch7collada18ISceneNodeAnimatorC2Ev
0065dc18 ldr      r3, [r6]
0065dc1c str      r3, [r4, #0x28]
0065dc20 ldr      r2, [r6, #4]
0065dc24 cmp      r3, #0
0065dc28 str      r2, [r4, #0x2c]
0065dc2c beq      #0x65dc40
0065dc30 ldr      r2, [r3, #4]
0065dc34 cmp      r2, #0
0065dc38 addne    r2, r2, #1
0065dc3c strne    r2, [r3, #4]
0065dc40 ldr      r2, [pc, #0x150]
0065dc44 ldr      r3, [pc, #0x150]
0065dc48 mov      r1, #0x3f800000
0065dc4c ldr      r2, [r5, r2]
0065dc50 ldr      r3, [r5, r3]
0065dc54 str      r1, [r4, #0x30]
0065dc58 add      r2, r2, #4
0065dc5c add      r1, r3, #0xa8
0065dc60 add      r0, r3, #0xc
0065dc64 add      r3, r3, #0xc4
0065dc68 str      r2, [r4, #0x24]
0065dc6c stm      r4, {r0, r1}
0065dc70 str      r3, [r4, #0x58]
0065dc74 ldr      r3, [r6]
0065dc78 mov      r7, #0
0065dc7c mov      r1, r7
0065dc80 ldr      r3, [r3, #0x24]
0065dc84 mov      r0, #0x48
0065dc88 ldr      r3, [r3, #0x20]
0065dc8c ldr      r3, [r3, #0x14]
0065dc90 str      r8, [r4, #0x40]
0065dc94 str      r7, [r4, #0x44]
0065dc98 subs     r3, r3, r7
0065dc9c movne    r3, #1
0065dca0 strb     r3, [r4, #0x34]
0065dca4 str      r7, [r4, #0x48]
0065dca8 str      r7, [r4, #0x4c]
0065dcac str      r7, [r4, #0x54]
0065dcb0 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
0065dcb4 mov      r5, r0
0065dcb8 bl       #0x666e40 ; _ZN6glitch7collada19CTimelineControllerC1Ev
0065dcbc ldr      r3, [r8]
0065dcc0 cmp      r3, r7
0065dcc4 beq      #0x65dd60
0065dcc8 ldr      r3, [r4, #0x40]
0065dccc cmp      r3, r7
0065dcd0 str      r3, [r5, #0x34]
0065dcd4 beq      #0x65dce4
0065dcd8 ldr      r3, [r3]
0065dcdc cmp      r3, r7
0065dce0 bne      #0x65dd48
0065dce4 mov      r3, #0
0065dce8 str      r3, [r5, #0x10]
0065dcec mov      r3, #1
0065dcf0 str      r3, [r5, #0x14]
0065dcf4 ldr      r3, [r6]
0065dcf8 mov      r0, r4
0065dcfc mov      r1, r5
0065dd00 ldr      r3, [r3, #0x24]
0065dd04 ldr      r3, [r3, #0x20]
0065dd08 ldr      r2, [r3, #0x1c]
0065dd0c str      r2, [r4, #0x38]
0065dd10 ldr      r3, [r6]
0065dd14 ldr      r3, [r3, #0x24]
0065dd18 ldr      r3, [r3, #0x20]
0065dd1c ldr      r3, [r3, #0x20]
0065dd20 rsb      r2, r2, r3
0065dd24 str      r2, [r4, #0x14]
0065dd28 str      r3, [r4, #0x3c]
0065dd2c bl       #0x599818 ; _ZN6glitch5scene18ISceneNodeAnimator15setTimelineCtrlEPNS0_19ITimelineControllerE
0065dd30 ldr      r3, [r5]
0065dd34 ldr      r0, [r3, #-0xc]
0065dd38 add      r0, r5, r0
0065dd3c bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0065dd40 mov      r0, r4
0065dd44 pop      {r4, r5, r6, r7, r8, pc}
0065dd48 mov      r1, r7
0065dd4c ldr      r3, [r5]
0065dd50 mov      r0, r5
0065dd54 mov      lr, pc
0065dd58 ldr      pc, [r3, #0x10]
0065dd5c b        #0x65dcf4
0065dd60 ldr      r3, [r6]
0065dd64 ldr      ip, [r5]
0065dd68 mov      r0, r5
0065dd6c ldr      r2, [r3, #0x24]
0065dd70 mov      r3, #1
0065dd74 ldr      r1, [r2, #0x20]
0065dd78 ldr      r2, [r1, #0x20]
0065dd7c ldr      r1, [r1, #0x1c]
0065dd80 mov      lr, pc
0065dd84 ldr      pc, [ip, #0x50]
0065dd88 b        #0x65dcf4
0065dd8c eorseq   r6, r3, r8, lsr #29
0065dd90 andeq    r2, r0, r4, asr #22
0065dd94 andeq    r3, r0, ip, lsl #18
0065dd98 strheq   r1, [r0], -r4
0065dd9c andeq    r4, r0, r8, lsl #23
_ZN8AnimatorC1ERKN6glitch7collada16CColladaDatabaseERNS1_22SLibraryAnimationClipsE
00366490 push     {r4, r5, r6, lr}
00366494 ldr      r5, [pc, #0x6c]
00366498 ldr      lr, [pc, #0x6c]
0036649c ldr      ip, [pc, #0x6c]
003664a0 add      r5, pc, r5
003664a4 ldr      lr, [r5, lr]
003664a8 ldr      ip, [r5, ip]
003664ac mov      r6, r1
003664b0 add      lr, lr, #8
003664b4 mov      r3, r2
003664b8 mov      r2, #1
003664bc str      lr, [r0, #0x94]
003664c0 add      r1, ip, #4
003664c4 str      r2, [r0, #0x98]
003664c8 mov      r2, r6
003664cc mov      r4, r0
003664d0 bl       #0x65dda0 ; _ZN6glitch7collada18CSceneNodeAnimatorC2ERKNS0_16CColladaDatabaseERNS0_22SLibraryAnimationClipsE
003664d4 ldr      r3, [pc, #0x38]
003664d8 add      r0, r4, #0x58
003664dc mov      r1, r4
003664e0 ldr      r3, [r5, r3]
003664e4 add      r2, r3, #0xa8
003664e8 add      ip, r3, #0xc
003664ec add      r3, r3, #0xc4
003664f0 str      ip, [r4]
003664f4 str      r3, [r4, #0x94]
003664f8 str      r2, [r4, #4]
003664fc bl       #0x364398 ; _ZN14AnimApplicatorC1EPN6glitch7collada18ISceneNodeAnimatorE
00366500 mov      r0, r4
00366504 pop      {r4, r5, r6, pc}
_ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE
0035d824 push     {r4, r5, r6, lr}
0035d828 ldr      r5, [pc, #0xd0]
0035d82c ldr      r3, [pc, #0xd0]
0035d830 ldr      r2, [pc, #0xd0]
0035d834 add      r5, pc, r5
0035d838 ldr      r3, [r5, r3]
0035d83c ldr      r2, [r5, r2]
0035d840 mov      r6, #1
0035d844 ldr      ip, [r3, #0x3c]
0035d848 add      r2, r2, #8
0035d84c str      r2, [r0, #0x20c]
0035d850 str      r6, [r0, #0x210]
0035d854 str      ip, [r0]
0035d858 ldr      lr, [r3, #0x40]
0035d85c ldr      ip, [ip, #-0xc]
0035d860 mov      r2, r1
0035d864 add      r1, r3, #4
0035d868 str      lr, [r0, ip]
0035d86c mov      r4, r0
0035d870 bl       #0x65b844 ; _ZN6glitch7collada14CRootSceneNodeC2ERKNS0_16CColladaDatabaseE
0035d874 ldr      r3, [pc, #0x90]
0035d878 add      r2, r4, #0x1bc
0035d87c mov      r0, r2
0035d880 ldr      r3, [r5, r3]
0035d884 str      r2, [r4, #0x1cc]
0035d888 str      r2, [r4, #0x1d0]
0035d88c add      r2, r3, #0x124
0035d890 add      r3, r3, #0x1c
0035d894 str      r3, [r4]
0035d898 str      r2, [r4, #0x20c]
0035d89c mov      r1, #0x10
0035d8a0 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0035d8a4 ldr      r2, [r4, #0x1cc]
0035d8a8 mov      r5, #0
0035d8ac add      r3, r4, #0x1d4
0035d8b0 strb     r5, [r2]
0035d8b4 mov      r0, r3
0035d8b8 str      r3, [r4, #0x1e4]
0035d8bc str      r3, [r4, #0x1e8]
0035d8c0 mov      r1, #0x10
0035d8c4 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0035d8c8 ldr      r3, [r4, #0x1e4]
0035d8cc mov      r0, r4
0035d8d0 strb     r5, [r3]
0035d8d4 strb     r6, [r4, #0x208]
0035d8d8 strb     r5, [r4, #0x20a]
0035d8dc strb     r5, [r4, #0x1ec]
0035d8e0 str      r5, [r4, #0x1f0]
0035d8e4 str      r5, [r4, #0x1f4]
0035d8e8 str      r5, [r4, #0x1f8]
0035d8ec str      r5, [r4, #0x1fc]
0035d8f0 strb     r6, [r4, #0x200]
0035d8f4 str      r5, [r4, #0x204]
0035d8f8 strb     r5, [r4, #0x209]
0035d8fc pop      {r4, r5, r6, pc}
0035d900 rsbeq    r7, r3, ip, asr r2
0035d904 strdeq   r1, r2, [r0], -r4
0035d908 andeq    r2, r0, r4, asr #22
0035d90c andeq    r3, r0, r8, lsl #18
