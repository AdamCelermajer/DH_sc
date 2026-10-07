# _ZN10PlayerInfo8IsActiveEv 36d48c 96
0036d48c push {r4, lr}
0036d490 mov r4, r0
0036d494 bl #0x7fd794
0036d498 ldrb r3, [r0, #5]
0036d49c cmp r3, #0
0036d4a0 beq #0x36d4e4
0036d4a4 ldr r3, [r4, #0x178]
0036d4a8 cmp r3, #0
0036d4ac blt #0x36d4dc
0036d4b0 ldr r3, [r4, #0x1a0]
0036d4b4 cmp r3, #0
0036d4b8 blt #0x36d4dc
0036d4bc ldr r3, [r4, #0x1c8]
0036d4c0 cmp r3, #0
0036d4c4 blt #0x36d4dc
0036d4c8 ldr r0, [r4, #0x1f0]
0036d4cc cmp r0, #3
0036d4d0 movne r0, #0
0036d4d4 moveq r0, #1
0036d4d8 pop {r4, pc}
0036d4dc mov r0, #0
0036d4e0 pop {r4, pc}
0036d4e4 mov r0, #1
0036d4e8 pop {r4, pc}
# _ZN13PlayerManager14GetLocalPlayerEib 36e478 32
0036e478 push {r4, lr}
0036e47c mov r4, r0
0036e480 bl #0x36e2cc
0036e484 mov r2, #0
0036e488 mov r1, r0
0036e48c mov r0, r4
0036e490 pop {r4, lr}
0036e494 b #0x36dfb0
# _ZN13PlayerManager34_AttachControllerToPlayerCharacterEi 36f0dc 392
0036f0dc push {r4, r5, r6, r7, r8, lr}
0036f0e0 mov r2, #0
0036f0e4 bl #0x36dfb0
0036f0e8 ldrb r1, [r0, #0x66c]
0036f0ec mov r6, r0
0036f0f0 ldr r5, [r0, #0x660]
0036f0f4 cmp r1, #0
0036f0f8 beq #0x36f1d8
0036f0fc mov r1, #0
0036f100 mov r0, #0x24
0036f104 bl #0x310570
0036f108 cmp r5, #0
0036f10c addne r7, r5, #0x374
0036f110 moveq r7, #0x374
0036f114 movne r1, r7
0036f118 moveq r1, r5
0036f11c mov r4, r0
0036f120 bl #0x408dd8
0036f124 mov r0, r7
0036f128 mov r1, r4
0036f12c bl #0x404e10
0036f130 ldr r3, [r5, #0x378]
0036f134 str r5, [r3, #0xc]
0036f138 bl #0x7fd794
0036f13c ldrb r3, [r0, #5]
0036f140 mov r1, #0
0036f144 mov r0, #0x20
0036f148 cmp r3, #0
0036f14c ldrne r3, [r5, #0x378]
0036f150 movne r2, #1
0036f154 strbne r2, [r3, #0xa]
0036f158 ldr r7, [r6, #0x668]
0036f15c bl #0x310570
0036f160 cmp r4, #0
0036f164 mov r6, r0
0036f168 beq #0x36f210
0036f16c add r5, r4, #0x10
0036f170 mov r2, r7
0036f174 mov r1, r5
0036f178 bl #0x406978
0036f17c mov r1, r6
0036f180 mov r0, r4
0036f184 bl #0x408fc0
0036f188 mov r1, #0
0036f18c mov r0, #0x1c
0036f190 bl #0x310570
0036f194 mov r1, r5
0036f198 mov r6, r0
0036f19c bl #0x4065ac
0036f1a0 mov r1, r6
0036f1a4 mov r0, r4
0036f1a8 bl #0x408fc0
0036f1ac mov r0, #0x10
0036f1b0 mov r1, #0
0036f1b4 bl #0x310570
0036f1b8 mov r6, r0
0036f1bc mov r1, r5
0036f1c0 mov r0, r6
0036f1c4 bl #0x408930
0036f1c8 mov r0, r4
0036f1cc mov r1, r6
0036f1d0 pop {r4, r5, r6, r7, r8, lr}
0036f1d4 b #0x408fc0
0036f1d8 mov r0, #0x10
0036f1dc bl #0x310570
0036f1e0 add r4, r5, #0x374
0036f1e4 cmp r5, #0
0036f1e8 movne r1, r4
0036f1ec moveq r1, #0
0036f1f0 mov r6, r0
0036f1f4 bl #0x4091f4
0036f1f8 mov r0, r4
0036f1fc mov r1, r6
0036f200 bl #0x404e10
0036f204 ldr r3, [r5, #0x378]
0036f208 str r5, [r3, #0xc]
0036f20c pop {r4, r5, r6, r7, r8, pc}
0036f210 mov r2, r7
0036f214 mov r1, r4
0036f218 bl #0x406978
0036f21c mov r1, r6
0036f220 mov r0, r4
0036f224 bl #0x408fc0
0036f228 mov r1, r4
0036f22c mov r0, #0x1c
0036f230 bl #0x310570
0036f234 mov r1, r4
0036f238 mov r5, r0
0036f23c bl #0x4065ac
0036f240 mov r1, r5
0036f244 mov r0, r4
0036f248 bl #0x408fc0
0036f24c mov r0, #0x10
0036f250 mov r1, r4
0036f254 bl #0x310570
0036f258 mov r5, r4
0036f25c mov r6, r0
0036f260 b #0x36f1bc
# _ZN13PlayerManager29_AttachLightToPlayerCharacterEi 371050 144
00371050 ldr r3, [pc, #0x80]
00371054 push {r4, r5, lr}
00371058 ldr r2, [pc, #0x7c]
0037105c add r3, pc, r3
00371060 sub sp, sp, #0xc
00371064 ldr r2, [r3, r2]
00371068 mov r1, sp
0037106c str sp, [sp]
00371070 ldr r0, [r2, #0x38]
00371074 str sp, [sp, #4]
00371078 mov r5, sp
0037107c bl #0x34336c
00371080 ldr r4, [sp]
00371084 b #0x3710a0
00371088 ldr r3, [r4, #8]
0037108c mov r0, r3
00371090 ldr r3, [r3]
00371094 mov lr, pc
00371098 ldr pc, [r3, #0x58]
0037109c ldr r4, [r4]
003710a0 cmp r4, r5
003710a4 bne #0x371088
003710a8 ldr r0, [sp]
003710ac cmp r0, r5
003710b0 bne #0x3710bc
003710b4 b #0x3710d0
003710b8 mov r0, r4
003710bc ldr r4, [r0]
003710c0 mov r1, #0xc
003710c4 bl #0x708f00
003710c8 cmp r4, r5
003710cc bne #0x3710b8
003710d0 add sp, sp, #0xc
003710d4 pop {r4, r5, pc}
003710d8 rsbeq r3, r2, r4, lsr sl
003710dc strdeq r3, r4, [r0], -r4
# _ZN10GameObject21SetCameraAnchorObjectEP10AnchorBase 394378 64
00394378 push {r4, r5, r6, lr}
0039437c ldr r3, [r0, #0x2e0]
00394380 mov r4, r0
00394384 mov r5, r1
00394388 cmp r3, r1
0039438c beq #0x3943b4
00394390 cmp r3, #0
00394394 beq #0x3943b0
00394398 mov r0, r3
0039439c ldr r3, [r3]
003943a0 mov lr, pc
003943a4 ldr pc, [r3, #4]
003943a8 mov r3, #0
003943ac str r3, [r4, #0x2e0]
003943b0 str r5, [r4, #0x2e0]
003943b4 pop {r4, r5, r6, pc}
# _ZN9Character7InitAllEv 3b35f0 40
003b35f0 push {r4, lr}
003b35f4 mov r4, r0
003b35f8 ldr r3, [r0]
003b35fc mov lr, pc
003b3600 ldr pc, [r3, #0x1c]
003b3604 mov r0, r4
003b3608 ldr r3, [r4]
003b360c mov lr, pc
003b3610 ldr pc, [r3, #0x58]
003b3614 pop {r4, pc}
# _ZN9Character24InitializePlayerSavegameEv 3b36b0 52
003b36b0 push {r4, r5, r6, lr}
003b36b4 mov r1, #0
003b36b8 mov r4, r0
003b36bc mov r0, #0x198
003b36c0 bl #0x310570
003b36c4 mov r5, r0
003b36c8 bl #0x465ae0
003b36cc movw r3, #0x14e8
003b36d0 mov r0, r4
003b36d4 mov r1, r4
003b36d8 str r5, [r4, r3]
003b36dc pop {r4, r5, r6, lr}
003b36e0 b #0x3bb754
# _ZN9Character9_InitHpMpEv 3b3a70 32
003b3a70 push {r4, lr}
003b3a74 mvn r1, #0
003b3a78 mov r4, r0
003b3a7c bl #0x3bdca4
003b3a80 mov r0, r4
003b3a84 mvn r1, #0
003b3a88 pop {r4, lr}
003b3a8c b #0x3bdbb8
# _ZN9Character11_InitSoundsEv 3b3b00 264
003b3b00 push {r4, r5, r6, r7, r8, lr}
003b3b04 bl #0x3a32d0
003b3b08 ldr r6, [pc, #0xf0]
003b3b0c ldr r7, [pc, #0xf0]
003b3b10 mov r4, r0
003b3b14 add r6, pc, r6
003b3b18 ldr r3, [r6, r7]
003b3b1c ldr r0, [r3]
003b3b20 cmp r0, #0
003b3b24 beq #0x3b3bfc
003b3b28 ldr r3, [r4, #0xc]
003b3b2c cmp r3, #0
003b3b30 beq #0x3b3b60
003b3b34 mov r5, #0
003b3b38 b #0x3b3b44
003b3b3c ldr r3, [r6, r7]
003b3b40 ldr r0, [r3]
003b3b44 ldr r3, [r4, #0x10]
003b3b48 ldr r1, [r3, r5, lsl #2]
003b3b4c bl #0x3699fc
003b3b50 ldr r3, [r4, #0xc]
003b3b54 add r5, r5, #1
003b3b58 cmp r3, r5
003b3b5c bhi #0x3b3b3c
003b3b60 ldr r3, [r4, #4]
003b3b64 cmp r3, #0
003b3b68 beq #0x3b3b94
003b3b6c ldr r8, [r6, r7]
003b3b70 mov r5, #0
003b3b74 ldr r3, [r4, #8]
003b3b78 ldr r0, [r8]
003b3b7c ldr r1, [r3, r5, lsl #2]
003b3b80 bl #0x3699fc
003b3b84 ldr r3, [r4, #4]
003b3b88 add r5, r5, #1
003b3b8c cmp r3, r5
003b3b90 bhi #0x3b3b74
003b3b94 ldr r3, [r4, #0x14]
003b3b98 cmp r3, #0
003b3b9c beq #0x3b3bc8
003b3ba0 ldr r8, [r6, r7]
003b3ba4 mov r5, #0
003b3ba8 ldr r3, [r4, #0x18]
003b3bac ldr r0, [r8]
003b3bb0 ldr r1, [r3, r5, lsl #2]
003b3bb4 bl #0x3699fc
003b3bb8 ldr r3, [r4, #0x14]
003b3bbc add r5, r5, #1
003b3bc0 cmp r3, r5
003b3bc4 bhi #0x3b3ba8
003b3bc8 ldr r3, [r4, #0x1c]
003b3bcc cmp r3, #0
003b3bd0 beq #0x3b3bfc
003b3bd4 ldr r6, [r6, r7]
003b3bd8 mov r5, #0
003b3bdc ldr r3, [r4, #0x20]
003b3be0 ldr r0, [r6]
003b3be4 ldr r1, [r3, r5, lsl #2]
003b3be8 bl #0x3699fc
003b3bec ldr r3, [r4, #0x1c]
003b3bf0 add r5, r5, #1
003b3bf4 cmp r3, r5
003b3bf8 bhi #0x3b3bdc
003b3bfc pop {r4, r5, r6, r7, r8, pc}
003b3c00 subseq r0, lr, ip, ror pc
003b3c04 andeq r0, r0, r4, lsr #27
# _ZN9Character24RegisterCharacterFXTableEv 3b4738 252
003b4738 push {r4, r5, r6, r7, r8, sb, sl, lr}
003b473c ldr r4, [pc, #0xdc]
003b4740 ldr r6, [pc, #0xdc]
003b4744 sub sp, sp, #0x20
003b4748 add r4, pc, r4
003b474c ldr r3, [r4, r6]
003b4750 mov r7, r0
003b4754 add r5, sp, #4
003b4758 ldr r3, [r3]
003b475c str r3, [sp, #0x1c]
003b4760 bl #0x3a3300
003b4764 mov sl, r0
003b4768 mov r0, r7
003b476c bl #0x3a33d0
003b4770 mov sb, r0
003b4774 mov r0, r7
003b4778 bl #0x3a3368
003b477c ldr r3, [pc, #0xa4]
003b4780 mov r8, r0
003b4784 ldr r7, [r4, r3]
003b4788 mov r0, r7
003b478c bl #0x337888
003b4790 ldr r1, [pc, #0x94]
003b4794 mov r2, sp
003b4798 mov r0, r5
003b479c add r1, pc, r1
003b47a0 bl #0x3140ec
003b47a4 mov r1, r5
003b47a8 mov r0, r7
003b47ac bl #0x337a88
003b47b0 mov r0, r5
003b47b4 bl #0x318254
003b47b8 cmp sl, #0
003b47bc blt #0x3b47d0
003b47c0 ldr r3, [pc, #0x68]
003b47c4 mov r1, sl
003b47c8 ldr r0, [r4, r3]
003b47cc bl #0x4967e8
003b47d0 cmp sb, #0
003b47d4 blt #0x3b47e8
003b47d8 ldr r3, [pc, #0x50]
003b47dc mov r1, sb
003b47e0 ldr r0, [r4, r3]
003b47e4 bl #0x4967e8
003b47e8 cmp r8, #0
003b47ec blt #0x3b4800
003b47f0 ldr r3, [pc, #0x38]
003b47f4 mov r1, r8
003b47f8 ldr r0, [r4, r3]
003b47fc bl #0x4967e8
003b4800 ldr r3, [r4, r6]
003b4804 ldr r2, [sp, #0x1c]
003b4808 ldr r3, [r3]
003b480c cmp r2, r3
003b4810 bne #0x3b481c
003b4814 add sp, sp, #0x20
003b4818 pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b481c bl #0x30e310
003b4820 subseq r0, lr, r8, asr #6
003b4824 andeq r4, r0, ip, lsr #1
003b4828 andeq r0, r0, r4, lsl #17
003b482c subseq pc, r0, r4, ror r6
003b4830 andeq r1, r0, r8, lsl #22
# _ZN9Character7InitCamEv 3b4bc4 412
003b4bc4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4bc8 ldr r4, [pc, #0x16c]
003b4bcc ldr r5, [pc, #0x16c]
003b4bd0 ldr r2, [pc, #0x16c]
003b4bd4 add r4, pc, r4
003b4bd8 ldr r3, [r4, r5]
003b4bdc ldr r7, [r4, r2]
003b4be0 sub sp, sp, #0x2c
003b4be4 ldr r3, [r3]
003b4be8 mov r6, r0
003b4bec mov r0, r7
003b4bf0 str r3, [sp, #0x24]
003b4bf4 bl #0x337888
003b4bf8 ldr r1, [pc, #0x148]
003b4bfc add r8, sp, #0xc
003b4c00 add r2, sp, #8
003b4c04 add r1, pc, r1
003b4c08 mov r0, r8
003b4c0c bl #0x3140ec
003b4c10 mov r0, r7
003b4c14 mov r1, r8
003b4c18 bl #0x337a88
003b4c1c mov r7, r0
003b4c20 mov r0, r8
003b4c24 bl #0x318254
003b4c28 cmp r7, #0
003b4c2c beq #0x3b4c74
003b4c30 mov r1, #0
003b4c34 mov r0, #0x1c
003b4c38 bl #0x310570
003b4c3c mov r1, r6
003b4c40 mov r7, r0
003b4c44 mov r2, #0
003b4c48 bl #0x4770d0
003b4c4c mov r0, r6
003b4c50 mov r1, r7
003b4c54 bl #0x394378
003b4c58 ldr r3, [r4, r5]
003b4c5c ldr r2, [sp, #0x24]
003b4c60 ldr r3, [r3]
003b4c64 cmp r2, r3
003b4c68 bne #0x3b4d38
003b4c6c add sp, sp, #0x2c
003b4c70 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4c74 ldr r3, [pc, #0xd0]
003b4c78 ldr r8, [pc, #0xd0]
003b4c7c ldr r2, [pc, #0xd0]
003b4c80 ldr sl, [r4, r3]
003b4c84 add r8, pc, r8
003b4c88 mov r1, r8
003b4c8c add r2, pc, r2
003b4c90 ldr r0, [sl, #0x2c]
003b4c94 bl #0x4c4bdc
003b4c98 ldr r2, [pc, #0xb8]
003b4c9c mov sb, r0
003b4ca0 mov r1, r8
003b4ca4 ldr r0, [sl, #0x2c]
003b4ca8 add r2, pc, r2
003b4cac bl #0x4c4bdc
003b4cb0 ldr r2, [pc, #0xa4]
003b4cb4 mov fp, r0
003b4cb8 mov r1, r8
003b4cbc add r2, pc, r2
003b4cc0 ldr r0, [sl, #0x2c]
003b4cc4 bl #0x4c4bdc
003b4cc8 mov r1, r7
003b4ccc mov r8, r0
003b4cd0 mov r0, #0x64
003b4cd4 bl #0x310570
003b4cd8 mov r7, r0
003b4cdc mov r0, sb
003b4ce0 bl #0x30e964
003b4ce4 mov sb, r0
003b4ce8 mov r0, fp
003b4cec bl #0x30e964
003b4cf0 mov sl, r0
003b4cf4 mov r0, r8
003b4cf8 bl #0x30e964
003b4cfc movw r1, #0xd70a
003b4d00 movt r1, #0x3c23
003b4d04 bl #0x30ed6c
003b4d08 mov r1, r6
003b4d0c str r0, [sp]
003b4d10 mov ip, #2
003b4d14 mov r2, sb
003b4d18 mov r3, sl
003b4d1c mov r0, r7
003b4d20 str ip, [sp, #4]
003b4d24 bl #0x477a80
003b4d28 mov r0, r6
003b4d2c mov r1, r7
003b4d30 bl #0x394378
003b4d34 b #0x3b4c58
003b4d38 bl #0x30e310
003b4d3c ldrheq pc, [sp], #-0xec
003b4d40 andeq r4, r0, ip, lsr #1
003b4d44 andeq r0, r0, r4, lsl #17
003b4d48 subseq pc, r0, r4, asr #4
003b4d4c strdeq r3, r4, [r0], -r4
003b4d50 subseq ip, r0, ip, asr #21
003b4d54 subseq pc, r0, ip, asr #3
003b4d58 ldrsbeq pc, [r0], #-0x10
003b4d5c ldrsbeq pc, [r0], #-0x1c
# _ZN9Character10SG_SetSlotEj 3bb740 20
003bb740 movw r3, #0x14e8
003bb744 ldr r3, [r0, r3]
003bb748 cmp r3, #0
003bb74c strne r1, [r3, #4]
003bb750 bx lr
# _ZN9Character17SG_SetPlayerClassEi 3bb814 20
003bb814 movw r3, #0x14e8
003bb818 ldr r3, [r0, r3]
003bb81c cmp r3, #0
003bb820 strne r1, [r3, #0x34]
003bb824 bx lr
# _ZN9Character17SG_SetSkillInSlotEij 3bbe54 20
003bbe54 movw r3, #0x14e8
003bbe58 ldr r0, [r0, r3]
003bbe5c cmp r0, #0
003bbe60 bxeq lr
003bbe64 b #0x4680a8
# _ZN9Character16SG_SetSkillLevelEji 3bbebc 20
003bbebc movw r3, #0x14e8
003bbec0 ldr r0, [r0, r3]
003bbec4 cmp r0, #0
003bbec8 bxeq lr
003bbecc b #0x4667c4
# _ZN9Character17SG_SaveCheckpointEv 3bc494 20
003bc494 movw r3, #0x14e8
003bc498 ldr r0, [r0, r3]
003bc49c cmp r0, #0
003bc4a0 bxeq lr
003bc4a4 b #0x466184
# _ZN9Character7SG_SaveEv 3bc4a8 20
003bc4a8 movw r3, #0x14e8
003bc4ac ldr r0, [r0, r3]
003bc4b0 cmp r0, #0
003bc4b4 bxeq lr
003bc4b8 b #0x464b2c
# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE 3def34 80
003def34 ldr r3, [pc, #0x40]
003def38 ldr r2, [pc, #0x40]
003def3c push {r4, r5, r6, r7, r8, lr}
003def40 add r3, pc, r3
003def44 mov r8, r0
003def48 ldr r7, [r3, r2]
003def4c mov r6, r1
003def50 mov r4, #0
003def54 mov r1, r4
003def58 mov r0, r8
003def5c ldr r5, [r7, r4, lsl #2]
003def60 bl #0x3def10
003def64 add r4, r4, #1
003def68 add r5, r5, #4
003def6c cmp r4, #0xe0
003def70 str r0, [r6, r5]
003def74 bne #0x3def54
003def78 pop {r4, r5, r6, r7, r8, pc}
003def7c subseq r5, fp, r0, asr fp
003def80 andeq r2, r0, r8, lsr #5
# _ZN14CharProperties19LoadGearsPropertiesEv 3df480 200
003df480 push {r4, r5, r6, r7, r8, lr}
003df484 mov r5, r0
003df488 ldr r0, [r5, #4]
003df48c mov r7, #0
003df490 add r0, r0, #0x37c
003df494 bl #0x3ffd20
003df498 cmp r7, r0
003df49c bhs #0x3df544
003df4a0 ldr r0, [r5, #4]
003df4a4 mov r1, r7
003df4a8 add r0, r0, #0x37c
003df4ac bl #0x3ffe3c
003df4b0 subs r6, r0, #0
003df4b4 beq #0x3df52c
003df4b8 bl #0x3f9e00
003df4bc mov r8, r0
003df4c0 ldr r0, [r5, #4]
003df4c4 mov r1, r7
003df4c8 mov r4, #0
003df4cc add r0, r0, #0x37c
003df4d0 bl #0x40022c
003df4d4 mov r1, r8
003df4d8 mov r2, r0
003df4dc mov r0, r5
003df4e0 bl #0x3e3154
003df4e4 b #0x3df514
003df4e8 bl #0x3fa038
003df4ec mov r8, r0
003df4f0 ldr r0, [r5, #4]
003df4f4 mov r1, r7
003df4f8 add r4, r4, #1
003df4fc add r0, r0, #0x37c
003df500 bl #0x40022c
003df504 mov r1, r8
003df508 mov r2, r0
003df50c mov r0, r5
003df510 bl #0x3e32b4
003df514 mov r0, r6
003df518 bl #0x3f9e80
003df51c cmp r4, r0
003df520 mov r1, r4
003df524 mov r0, r6
003df528 blo #0x3df4e8
003df52c ldr r0, [r5, #4]
003df530 add r7, r7, #1
003df534 add r0, r0, #0x37c
003df538 bl #0x3ffd20
003df53c cmp r7, r0
003df540 blo #0x3df4a0
003df544 pop {r4, r5, r6, r7, r8, pc}
# _ZN5Level13SG_SavePlayerEP9Characterb 3efa54 152
003efa54 push {r4, r5, r6, r7, r8, lr}
003efa58 subs r4, r1, #0
003efa5c mov r5, r0
003efa60 mov r7, r2
003efa64 beq #0x3efad8
003efa68 ldr r3, [r4]
003efa6c mov r0, r4
003efa70 mov lr, pc
003efa74 ldr pc, [r3, #0x28]
003efa78 cmp r0, #0
003efa7c beq #0x3efad8
003efa80 mov r0, r4
003efa84 bl #0x3bb784
003efa88 cmp r7, #0
003efa8c mov r6, r0
003efa90 bne #0x3efadc
003efa94 mov r0, r4
003efa98 bl #0x3bd120
003efa9c mov r1, r0
003efaa0 mov r0, r4
003efaa4 bl #0x3bb840
003efaa8 mov r0, r4
003efaac bl #0x3bbc58
003efab0 ldr r1, [r5, #0x110]
003efab4 mov r0, r4
003efab8 mvn r2, #0
003efabc bl #0x3bb89c
003efac0 mov r0, r4
003efac4 bl #0x3bc4a8
003efac8 mov r0, r4
003efacc mov r1, r6
003efad0 pop {r4, r5, r6, r7, r8, lr}
003efad4 b #0x3bb770
003efad8 pop {r4, r5, r6, r7, r8, pc}
003efadc mov r0, r4
003efae0 mov r1, #0
003efae4 bl #0x3bb770
003efae8 b #0x3efa94
# _ZN5Level9QuickSaveEb 3f059c 244
003f059c ldr r3, [pc, #0xe4]
003f05a0 ldr r2, [pc, #0xe4]
003f05a4 push {r4, r5, r6, r7, r8, lr}
003f05a8 add r3, pc, r3
003f05ac ldr r5, [r3, r2]
003f05b0 mov r4, r0
003f05b4 mov r6, r1
003f05b8 mov r2, #1
003f05bc ldr r0, [r5, #0x40]
003f05c0 mov r1, #0
003f05c4 bl #0x36e478
003f05c8 ldr r3, [r4, #0xec]
003f05cc ldr r7, [r0, #0x660]
003f05d0 cmp r3, #0
003f05d4 beq #0x3f05e4
003f05d8 ldr r3, [r4, #0x130]
003f05dc cmp r3, #0x26
003f05e0 beq #0x3f05e8
003f05e4 pop {r4, r5, r6, r7, r8, pc}
003f05e8 cmp r7, #0
003f05ec beq #0x3f05e4
003f05f0 ldr r3, [r7]
003f05f4 mov r0, r7
003f05f8 mov lr, pc
003f05fc ldr pc, [r3, #0x34]
003f0600 cmp r0, #0
003f0604 bne #0x3f05e4
003f0608 bl #0x7fd794
003f060c ldrb r3, [r0, #5]
003f0610 cmp r3, #0
003f0614 bne #0x3f0664
003f0618 ldr r0, [r7, #0x168]
003f061c ldr r1, [r7, #0x160]
003f0620 ldr r2, [r7, #0x164]
003f0624 movw r3, #0x1470
003f0628 str r0, [r7, r3]
003f062c movw r3, #0x1468
003f0630 str r1, [r7, r3]
003f0634 movw r3, #0x146c
003f0638 str r2, [r7, r3]
003f063c ldr r0, [r4, #0xec]
003f0640 cmp r6, #0
003f0644 movne r3, #0
003f0648 ldrb r5, [r0, #0x39]
003f064c strbne r3, [r0, #0x39]
003f0650 ldrne r0, [r4, #0xec]
003f0654 bl #0x4615ec
003f0658 ldr r3, [r4, #0xec]
003f065c strb r5, [r3, #0x39]
003f0660 b #0x3f05e4
003f0664 ldr r0, [r5, #0x40]
003f0668 bl #0x36f074
003f066c cmp r0, #0
003f0670 beq #0x3f05e4
003f0674 ldr r3, [r5, #0x40]
003f0678 ldrb r3, [r3, #0x719]
003f067c cmp r3, #0
003f0680 bne #0x3f05e4
003f0684 b #0x3f0618
003f0688 subseq r4, sl, r8, ror #9
003f068c strdeq r3, r4, [r0], -r4
# _ZN10AnchorBaseD2Ev 47706c 4
0047706c bx lr
# _ZN10AnchorBaseD1Ev 477070 4
00477070 bx lr
# _ZN10AnchorBase5ResetEv 477074 32
00477074 ldr r3, [r0, #8]
00477078 ldr r2, [r3, #0x160]
0047707c str r2, [r0, #0xc]
00477080 ldr r2, [r3, #0x164]
00477084 str r2, [r0, #0x10]
00477088 ldr r3, [r3, #0x168]
0047708c str r3, [r0, #0x14]
00477090 bx lr
# _ZN10AnchorBase6UpdateEv 477094 32
00477094 ldr r3, [r0, #8]
00477098 ldr r2, [r3, #0x160]
0047709c str r2, [r0, #0xc]
004770a0 ldr r2, [r3, #0x164]
004770a4 str r2, [r0, #0x10]
004770a8 ldr r3, [r3, #0x168]
004770ac str r3, [r0, #0x14]
004770b0 bx lr
# _ZN10AnchorBaseD0Ev 4770b4 28
004770b4 push {r4, lr}
004770b8 mov r4, r0
004770bc bl #0x477070
004770c0 mov r0, r4
004770c4 bl #0x310440
004770c8 mov r0, r4
004770cc pop {r4, pc}
# _ZN10AnchorBaseC1EP10GameObjectNS_10AnchorTypeE 4770d0 208
004770d0 ldr r3, [pc, #0xac]
004770d4 push {r4, lr}
004770d8 ldr lr, [pc, #0xa8]
004770dc add r3, pc, r3
004770e0 mov ip, #0
004770e4 ldr lr, [r3, lr]
004770e8 str r2, [r0, #4]
004770ec cmp r1, #0
004770f0 add lr, lr, #8
004770f4 mov r2, #1
004770f8 sub sp, sp, #8
004770fc mov r4, r0
00477100 str lr, [r0]
00477104 str ip, [r0, #0x14]
00477108 strb r2, [r0, #0x18]
0047710c str r1, [r0, #8]
00477110 str ip, [r0, #0xc]
00477114 str ip, [r0, #0x10]
00477118 beq #0x477130
0047711c mov r0, r4
00477120 bl #0x477074
00477124 mov r0, r4
00477128 add sp, sp, #8
0047712c pop {r4, pc}
00477130 ldr r2, [pc, #0x54]
00477134 ldr r2, [r3, r2]
00477138 ldr r2, [r2]
0047713c cmp r2, #2
00477140 streq r1, [r1]
00477144 beq #0x47711c
00477148 cmp r2, #1
0047714c bne #0x47711c
00477150 ldr r0, [pc, #0x38]
00477154 ldr r1, [pc, #0x38]
00477158 ldr r2, [pc, #0x38]
0047715c ldr r0, [r3, r0]
00477160 ldr r3, [pc, #0x34]
00477164 mov ip, #0x18
00477168 add r1, pc, r1
0047716c add r2, pc, r2
00477170 add r3, pc, r3
00477174 add r0, r0, #0xa8
00477178 str ip, [sp]
0047717c bl #0x30e004
00477180 b #0x47711c
00477184 ldrheq sp, [r1], #-0x94
00477188 strheq r1, [r0], -ip
0047718c andeq r3, r0, r0, asr #19
00477190 andeq r1, r0, r0, asr #19
00477194 subeq r7, r4, r0, ror r2
00477198 subeq fp, r4, ip, lsr #4
0047719c subeq r6, r5, r8, ror r7
# _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE 4771a0 208
004771a0 ldr r3, [pc, #0xac]
004771a4 push {r4, lr}
004771a8 ldr lr, [pc, #0xa8]
004771ac add r3, pc, r3
004771b0 mov ip, #0
004771b4 ldr lr, [r3, lr]
004771b8 str r2, [r0, #4]
004771bc cmp r1, #0
004771c0 add lr, lr, #8
004771c4 mov r2, #1
004771c8 sub sp, sp, #8
004771cc mov r4, r0
004771d0 str lr, [r0]
004771d4 str ip, [r0, #0x14]
004771d8 strb r2, [r0, #0x18]
004771dc str r1, [r0, #8]
004771e0 str ip, [r0, #0xc]
004771e4 str ip, [r0, #0x10]
004771e8 beq #0x477200
004771ec mov r0, r4
004771f0 bl #0x477074
004771f4 mov r0, r4
004771f8 add sp, sp, #8
004771fc pop {r4, pc}
00477200 ldr r2, [pc, #0x54]
00477204 ldr r2, [r3, r2]
00477208 ldr r2, [r2]
0047720c cmp r2, #2
00477210 streq r1, [r1]
00477214 beq #0x4771ec
00477218 cmp r2, #1
0047721c bne #0x4771ec
00477220 ldr r0, [pc, #0x38]
00477224 ldr r1, [pc, #0x38]
00477228 ldr r2, [pc, #0x38]
0047722c ldr r0, [r3, r0]
00477230 ldr r3, [pc, #0x34]
00477234 mov ip, #0x18
00477238 add r1, pc, r1
0047723c add r2, pc, r2
00477240 add r3, pc, r3
00477244 add r0, r0, #0xa8
00477248 str ip, [sp]
0047724c bl #0x30e004
00477250 b #0x4771ec
00477254 subseq sp, r1, r4, ror #17
00477258 strheq r1, [r0], -ip
0047725c andeq r3, r0, r0, asr #19
00477260 andeq r1, r0, r0, asr #19
00477264 subeq r7, r4, r0, lsr #3
00477268 subeq fp, r4, ip, asr r1
0047726c subeq r6, r5, r8, lsr #13
# _ZN13AnchorForward6UpdateEv 47734c 1680
0047734c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00477350 ldrb r3, [r0, #0x18]
00477354 ldr sb, [pc, #0x678]
00477358 sub sp, sp, #0x2c
0047735c cmp r3, #0
00477360 mov r4, r0
00477364 add sb, pc, sb
00477368 beq #0x4776b8
0047736c ldrb r1, [r0, #0x38]
00477370 cmp r1, #0
00477374 bne #0x4775e8
00477378 ldr r5, [r0, #8]
0047737c mov r3, #5
00477380 str r3, [r0, #0x3c]
00477384 ldrb r3, [r5, #0x1b5]
00477388 cmp r3, #0
0047738c beq #0x4776dc
00477390 ldr r0, [r0, #0x28]
00477394 cmp r0, #0
00477398 beq #0x4776dc
0047739c add r0, r0, #0x4f0
004773a0 add r0, r0, #0xc
004773a4 bl #0x3c029c
004773a8 cmp r0, #0
004773ac beq #0x4776c0
004773b0 ldr r3, [r4, #8]
004773b4 add r7, r4, #0x58
004773b8 add r5, sp, #0x1c
004773bc ldr r2, [r3, #0x1b8]
004773c0 mov r1, r5
004773c4 mov r0, r7
004773c8 str r2, [sp, #0x1c]
004773cc ldr r2, [r3, #0x1bc]
004773d0 ldr ip, [r4, #0x1c]
004773d4 str r2, [sp, #0x20]
004773d8 str ip, [sp, #0xc]
004773dc ldr r3, [r3, #0x1c0]
004773e0 str r3, [sp, #0x24]
004773e4 bl #0x313058
004773e8 ldr r3, [r4, #8]
004773ec ldr r2, [r4, #0x2c]
004773f0 mov fp, r0
004773f4 ldr sl, [r3, #0x168]
004773f8 ldr r0, [r4, #0x28]
004773fc str r2, [sp, #0x10]
00477400 ldr ip, [r3, #0x160]
00477404 add r0, r0, #0x4f0
00477408 mov r1, #0
0047740c str ip, [sp, #0x14]
00477410 add r0, r0, #0xc
00477414 ldr r8, [r4, #0x30]
00477418 ldr r6, [r3, #0x164]
0047741c ldr sb, [r4, #0x34]
00477420 bl #0x3c029c
00477424 cmp r0, #0
00477428 bne #0x477790
0047742c ldr r0, [r4, #0x1c]
00477430 mov r1, #0x3f000000
00477434 bl #0x30ed6c
00477438 mov r6, r0
0047743c mov r0, r5
00477440 bl #0x34d0b0
00477444 ldr r5, [r4, #8]
00477448 mov r8, r0
0047744c ldr r1, [r0]
00477450 mov r0, r6
00477454 bl #0x30ed6c
00477458 ldr r1, [r5, #0x160]
0047745c bl #0x30eba4
00477460 ldr r1, [r8, #4]
00477464 mov sb, r0
00477468 mov r0, r6
0047746c bl #0x30ed6c
00477470 ldr r1, [r5, #0x164]
00477474 bl #0x30eba4
00477478 ldr r1, [r8, #8]
0047747c mov sl, r0
00477480 mov r0, r6
00477484 bl #0x30ed6c
00477488 ldr r1, [r5, #0x168]
0047748c bl #0x30eba4
00477490 mov r1, #0x3f800000
00477494 mov r8, r0
00477498 mov r0, fp
0047749c bl #0x30e2f8
004774a0 cmp r0, #0
004774a4 ldr fp, [r4, #0x20]
004774a8 beq #0x477828
004774ac mov r0, fp
004774b0 mov r1, #0x3e800000
004774b4 bl #0x30ed6c
004774b8 mov r1, r0
004774bc ldr r0, [r4, #0x54]
004774c0 bl #0x30e3ac
004774c4 mov r1, #0
004774c8 mov fp, r0
004774cc bl #0x30e2f8
004774d0 cmp r0, #0
004774d4 moveq fp, #0
004774d8 str fp, [r4, #0x54]
004774dc ldr r1, [r4, #0xc]
004774e0 mov r0, sb
004774e4 bl #0x30e3ac
004774e8 ldr r1, [r4, #0x10]
004774ec mov fp, r0
004774f0 mov r0, sl
004774f4 bl #0x30e3ac
004774f8 ldr r1, [r4, #0x14]
004774fc mov r3, r0
00477500 mov r0, r8
00477504 str r3, [sp, #4]
00477508 bl #0x30e3ac
0047750c mov r1, fp
00477510 mov r2, r0
00477514 mov r0, fp
00477518 str r2, [sp, #8]
0047751c bl #0x30ed6c
00477520 ldr r3, [sp, #4]
00477524 mov fp, r0
00477528 mov r1, r3
0047752c mov r0, r3
00477530 bl #0x30ed6c
00477534 mov r1, r0
00477538 mov r0, fp
0047753c bl #0x30eba4
00477540 ldr r2, [sp, #8]
00477544 mov fp, r0
00477548 mov r1, r2
0047754c mov r0, r2
00477550 bl #0x30ed6c
00477554 mov r1, r0
00477558 mov r0, fp
0047755c bl #0x30eba4
00477560 mov r1, #0
00477564 bl #0x30e2f8
00477568 cmp r0, #0
0047756c beq #0x477818
00477570 ldr fp, [r4, #0x54]
00477574 mov r0, r6
00477578 mov r1, fp
0047757c bl #0x30e3ac
00477580 movw r1, #0xcccd
00477584 movt r1, #0x3d4c
00477588 bl #0x30e2f8
0047758c cmp r0, #0
00477590 beq #0x477818
00477594 ldr r1, [sp, #0x20]
00477598 mov r0, fp
0047759c bl #0x30ed6c
004775a0 ldr r1, [r5, #0x164]
004775a4 bl #0x30eba4
004775a8 ldr r1, [sp, #0x24]
004775ac mov r8, r0
004775b0 mov r0, fp
004775b4 bl #0x30ed6c
004775b8 ldr r1, [r5, #0x168]
004775bc bl #0x30eba4
004775c0 ldr r1, [sp, #0x1c]
004775c4 mov r6, r0
004775c8 mov r0, fp
004775cc bl #0x30ed6c
004775d0 ldr r1, [r5, #0x160]
004775d4 bl #0x30eba4
004775d8 str r8, [r4, #0x10]
004775dc str r0, [r4, #0xc]
004775e0 str r6, [r4, #0x14]
004775e4 b #0x477760
004775e8 ldr r0, [r0, #0x28]
004775ec mov r1, #0
004775f0 ldr r5, [r4, #8]
004775f4 add r0, r0, #0x4f0
004775f8 add r0, r0, #0xc
004775fc bl #0x3c029c
00477600 cmp r0, #0
00477604 ldrne r6, [r4, #0x1c]
00477608 bne #0x47761c
0047760c ldr r0, [r4, #0x1c]
00477610 mov r1, #0x3f000000
00477614 bl #0x30ed6c
00477618 mov r6, r0
0047761c ldr r3, [r4, #8]
00477620 ldr r7, [r5, #0x1b8]
00477624 ldr sl, [r5, #0x1bc]
00477628 ldrb r3, [r3, #0x1b5]
0047762c ldr r8, [r5, #0x1c0]
00477630 cmp r3, #0
00477634 beq #0x477674
00477638 ldr r0, [r4, #0x28]
0047763c cmp r0, #0
00477640 beq #0x477674
00477644 add r0, r0, #0x4f0
00477648 add r0, r0, #0xc
0047764c mov r1, #0
00477650 bl #0x3c029c
00477654 cmp r0, #0
00477658 bne #0x477890
0047765c ldr r0, [r4, #0x28]
00477660 add r0, r0, #0x4f0
00477664 add r0, r0, #0xc
00477668 bl #0x3c02d0
0047766c cmp r0, #0
00477670 bne #0x477890
00477674 ldrb r3, [r4, #0x4c]
00477678 cmp r3, #0
0047767c moveq r3, #1
00477680 streq r3, [r4, #0x3c]
00477684 bne #0x4779a4
00477688 ldr r3, [r5, #0x160]
0047768c str r3, [r4, #0x40]
00477690 ldr r3, [r5, #0x164]
00477694 str r3, [r4, #0x44]
00477698 ldr r3, [r5, #0x168]
0047769c str r3, [r4, #0x48]
004776a0 ldr r3, [r5, #0x160]
004776a4 str r3, [r4, #0xc]
004776a8 ldr r3, [r5, #0x164]
004776ac str r3, [r4, #0x10]
004776b0 ldr r3, [r5, #0x168]
004776b4 str r3, [r4, #0x14]
004776b8 add sp, sp, #0x2c
004776bc pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004776c0 ldr r0, [r4, #0x28]
004776c4 add r0, r0, #0x4f0
004776c8 add r0, r0, #0xc
004776cc bl #0x3c02d0
004776d0 cmp r0, #0
004776d4 ldreq r5, [r4, #8]
004776d8 bne #0x4773b0
004776dc movw r1, #0xcccd
004776e0 movt r1, #0x3ecc
004776e4 ldr r0, [r4, #0x1c]
004776e8 bl #0x30ed6c
004776ec ldr r7, [r4, #0x54]
004776f0 mov r6, r0
004776f4 mov r1, r6
004776f8 mov r0, r7
004776fc bl #0x30e2f8
00477700 cmp r0, #0
00477704 moveq r6, r7
00477708 bne #0x477788
0047770c ldr r1, [r4, #0x5c]
00477710 mov r0, r6
00477714 bl #0x30ed6c
00477718 ldr r1, [r5, #0x164]
0047771c bl #0x30eba4
00477720 ldr r1, [r4, #0x60]
00477724 mov r8, r0
00477728 mov r0, r6
0047772c bl #0x30ed6c
00477730 ldr r1, [r5, #0x168]
00477734 bl #0x30eba4
00477738 ldr r1, [r4, #0x58]
0047773c mov r7, r0
00477740 mov r0, r6
00477744 bl #0x30ed6c
00477748 ldr r1, [r5, #0x160]
0047774c bl #0x30eba4
00477750 str r7, [r4, #0x14]
00477754 str r8, [r4, #0x10]
00477758 str r0, [r4, #0xc]
0047775c add r7, r4, #0x58
00477760 ldr r3, [r5, #0x160]
00477764 mov r0, r5
00477768 mov r1, r7
0047776c str r3, [r4, #0x2c]
00477770 ldr r3, [r5, #0x164]
00477774 str r3, [r4, #0x30]
00477778 ldr r3, [r5, #0x168]
0047777c str r3, [r4, #0x34]
00477780 bl #0x393ae4
00477784 b #0x4776b8
00477788 str r6, [r4, #0x54]
0047778c b #0x47770c
00477790 mov r1, sl
00477794 mov r0, sb
00477798 bl #0x30e3ac
0047779c mov r1, r6
004777a0 mov sl, r0
004777a4 mov r0, r8
004777a8 bl #0x30e3ac
004777ac ldr r1, [sp, #0x14]
004777b0 mov r6, r0
004777b4 ldr r0, [sp, #0x10]
004777b8 bl #0x30e3ac
004777bc mov r1, r0
004777c0 bl #0x30ed6c
004777c4 mov r1, r6
004777c8 mov r8, r0
004777cc mov r0, r6
004777d0 bl #0x30ed6c
004777d4 mov r1, r0
004777d8 mov r0, r8
004777dc bl #0x30eba4
004777e0 mov r1, sl
004777e4 mov r6, r0
004777e8 mov r0, sl
004777ec bl #0x30ed6c
004777f0 mov r1, r0
004777f4 mov r0, r6
004777f8 bl #0x30eba4
004777fc movw r1, #0xcccd
00477800 movt r1, #0x3d4c
00477804 bl #0x30e2f8
00477808 cmp r0, #0
0047780c ldrne r6, [r4, #0x1c]
00477810 bne #0x47743c
00477814 b #0x47742c
00477818 str sb, [r4, #0xc]
0047781c str sl, [r4, #0x10]
00477820 str r8, [r4, #0x14]
00477824 b #0x477760
00477828 mov r1, #0x3f000000
0047782c ldr r0, [sp, #0xc]
00477830 bl #0x30ed6c
00477834 ldr r2, [r4, #0x54]
00477838 str r2, [sp, #0xc]
0047783c bl #0x30e4cc
00477840 bl #0x30e964
00477844 mov r3, r0
00477848 mov r1, r3
0047784c ldr r0, [sp, #0xc]
00477850 str r3, [sp, #4]
00477854 bl #0x30e70c
00477858 ldr r3, [sp, #4]
0047785c cmp r0, #0
00477860 strne r3, [r4, #0x54]
00477864 bne #0x4774dc
00477868 mov r0, fp
0047786c ldr r1, [sp, #0xc]
00477870 bl #0x30eba4
00477874 mov fp, r0
00477878 mov r1, fp
0047787c mov r0, r6
00477880 bl #0x30e70c
00477884 cmp r0, #0
00477888 movne fp, r6
0047788c b #0x4774d8
00477890 ldrb r3, [r4, #0x4c]
00477894 cmp r3, #0
00477898 movne r3, #2
0047789c strne r3, [r4, #0x3c]
004778a0 bne #0x477940
004778a4 ldr r1, [r4, #0x40]
004778a8 ldr r0, [r5, #0x160]
004778ac bl #0x30e3ac
004778b0 ldr r1, [r4, #0x44]
004778b4 mov sb, r0
004778b8 ldr r0, [r5, #0x164]
004778bc bl #0x30e3ac
004778c0 ldr r1, [r4, #0x48]
004778c4 mov fp, r0
004778c8 ldr r0, [r5, #0x168]
004778cc bl #0x30e3ac
004778d0 mov r1, sb
004778d4 mov r3, r0
004778d8 mov r0, sb
004778dc str r3, [sp, #4]
004778e0 bl #0x30ed6c
004778e4 mov r1, fp
004778e8 mov sb, r0
004778ec mov r0, fp
004778f0 bl #0x30ed6c
004778f4 mov r1, r0
004778f8 mov r0, sb
004778fc bl #0x30eba4
00477900 ldr r3, [sp, #4]
00477904 mov sb, r0
00477908 mov r1, r3
0047790c mov r0, r3
00477910 bl #0x30ed6c
00477914 mov r1, r0
00477918 mov r0, sb
0047791c bl #0x30eba4
00477920 movw r1, #0x2400
00477924 movt r1, #0x4774
00477928 bl #0x30e4b4
0047792c cmp r0, #0
00477930 movne r3, #3
00477934 strne r3, [r4, #0x3c]
00477938 streq r0, [r4, #0x3c]
0047793c beq #0x4776a0
00477940 mov r1, sl
00477944 mov r0, r6
00477948 bl #0x30ed6c
0047794c ldr r1, [r5, #0x164]
00477950 bl #0x30eba4
00477954 mov r1, r8
00477958 mov sl, r0
0047795c mov r0, r6
00477960 bl #0x30ed6c
00477964 ldr r1, [r5, #0x168]
00477968 bl #0x30eba4
0047796c mov r1, r7
00477970 mov r8, r0
00477974 mov r0, r6
00477978 bl #0x30ed6c
0047797c ldr r1, [r5, #0x160]
00477980 bl #0x30eba4
00477984 movw r3, #0x15e
00477988 str r3, [r4, #0x50]
0047798c mov r3, #1
00477990 str r0, [r4, #0xc]
00477994 str sl, [r4, #0x10]
00477998 str r8, [r4, #0x14]
0047799c strb r3, [r4, #0x4c]
004779a0 b #0x4776b8
004779a4 ldr r3, [pc, #0x2c]
004779a8 ldr r6, [r4, #0x50]
004779ac ldr r0, [sb, r3]
004779b0 bl #0x31f66c
004779b4 rsb r0, r0, r6
004779b8 cmp r0, #0
004779bc movle r3, #0
004779c0 strble r3, [r4, #0x4c]
004779c4 movle r3, #4
004779c8 str r0, [r4, #0x50]
004779cc strle r3, [r4, #0x3c]
004779d0 b #0x477688
004779d4 subseq sp, r1, ip, lsr #14
004779d8 strdeq r3, r4, [r0], -r4
# _ZN13AnchorForward5ResetEv 4779dc 32
004779dc push {r4, lr}
004779e0 mov r4, r0
004779e4 bl #0x477074
004779e8 mov r3, #0
004779ec str r3, [r4, #0x34]
004779f0 str r3, [r4, #0x2c]
004779f4 str r3, [r4, #0x30]
004779f8 pop {r4, pc}
# _ZN13AnchorForwardD1Ev 4779fc 52
004779fc ldr r3, [pc, #0x24]
00477a00 ldr r2, [pc, #0x24]
00477a04 push {r4, lr}
00477a08 add r3, pc, r3
00477a0c ldr r2, [r3, r2]
00477a10 mov r4, r0
00477a14 add r2, r2, #8
00477a18 str r2, [r0]
00477a1c bl #0x47706c
00477a20 mov r0, r4
00477a24 pop {r4, pc}
00477a28 subseq sp, r1, r8, lsl #1
00477a2c muleq r0, r8, r3
# _ZN13AnchorForwardD0Ev 477a30 28
00477a30 push {r4, lr}
00477a34 mov r4, r0
00477a38 bl #0x4779fc
00477a3c mov r0, r4
00477a40 bl #0x310440
00477a44 mov r0, r4
00477a48 pop {r4, pc}
# _ZN13AnchorForwardD2Ev 477a4c 52
00477a4c ldr r3, [pc, #0x24]
00477a50 ldr r2, [pc, #0x24]
00477a54 push {r4, lr}
00477a58 add r3, pc, r3
00477a5c ldr r2, [r3, r2]
00477a60 mov r4, r0
00477a64 add r2, r2, #8
00477a68 str r2, [r0]
00477a6c bl #0x47706c
00477a70 mov r0, r4
00477a74 pop {r4, pc}
00477a78 subseq sp, r1, r8, lsr r0
00477a7c muleq r0, r8, r3
# _ZN13AnchorForwardC1EP10GameObjectfffN10AnchorBase10AnchorTypeE 477a80 596
00477a80 push {r4, r5, r6, r7, r8, sl, lr}
00477a84 sub sp, sp, #0x1c
00477a88 mov r6, r2
00477a8c ldr r5, [pc, #0x20c]
00477a90 ldr r2, [sp, #0x3c]
00477a94 mov r4, r0
00477a98 mov r8, r3
00477a9c mov r7, r1
00477aa0 bl #0x4771a0
00477aa4 ldr r3, [pc, #0x1f8]
00477aa8 add r5, pc, r5
00477aac mov r1, #0
00477ab0 ldr r3, [r5, r3]
00477ab4 mov sl, #0
00477ab8 mov r0, r6
00477abc add r3, r3, #8
00477ac0 str r3, [r4]
00477ac4 ldr r3, [sp, #0x38]
00477ac8 str r6, [r4, #0x1c]
00477acc str r8, [r4, #0x20]
00477ad0 str r3, [r4, #0x24]
00477ad4 mov r3, #5
00477ad8 str r3, [r4, #0x3c]
00477adc str sl, [r4, #0x28]
00477ae0 str r1, [r4, #0x2c]
00477ae4 str r1, [r4, #0x30]
00477ae8 str r1, [r4, #0x34]
00477aec strb sl, [r4, #0x38]
00477af0 str r1, [r4, #0x40]
00477af4 str r1, [r4, #0x44]
00477af8 str r1, [r4, #0x48]
00477afc strb sl, [r4, #0x4c]
00477b00 str sl, [r4, #0x50]
00477b04 str r1, [r4, #0x54]
00477b08 str r1, [r4, #0x58]
00477b0c str r1, [r4, #0x5c]
00477b10 str r1, [r4, #0x60]
00477b14 bl #0x30e4b4
00477b18 cmp r0, sl
00477b1c bne #0x477b40
00477b20 ldr r3, [pc, #0x180]
00477b24 ldr r3, [r5, r3]
00477b28 ldr r3, [r3]
00477b2c cmp r3, #2
00477b30 streq sl, [sl]
00477b34 beq #0x477b40
00477b38 cmp r3, #1
00477b3c beq #0x477c34
00477b40 mov r0, r8
00477b44 mov r1, #0
00477b48 bl #0x30e4b4
00477b4c cmp r0, #0
00477b50 bne #0x477b78
00477b54 ldr r3, [pc, #0x14c]
00477b58 ldr r3, [r5, r3]
00477b5c ldr r3, [r3]
00477b60 cmp r3, #2
00477b64 moveq r3, #0
00477b68 streq r3, [r3]
00477b6c beq #0x477b78
00477b70 cmp r3, #1
00477b74 beq #0x477c6c
00477b78 ldr r6, [r4, #0x24]
00477b7c mov r1, #0
00477b80 mov r0, r6
00477b84 bl #0x30e4b4
00477b88 cmp r0, #0
00477b8c beq #0x477bdc
00477b90 mov r0, r6
00477b94 mov r1, #0x3f800000
00477b98 bl #0x30e9ac
00477b9c cmp r0, #0
00477ba0 beq #0x477bdc
00477ba4 cmp r7, #0
00477ba8 beq #0x477bc8
00477bac add r5, sp, #0xc
00477bb0 mov r1, r7
00477bb4 mov r0, r5
00477bb8 bl #0x33dd2c
00477bbc mov r0, r5
00477bc0 bl #0x33ff54
00477bc4 str r0, [r4, #0x28]
00477bc8 mov r0, r4
00477bcc bl #0x4779dc
00477bd0 mov r0, r4
00477bd4 add sp, sp, #0x1c
00477bd8 pop {r4, r5, r6, r7, r8, sl, pc}
00477bdc ldr r3, [pc, #0xc4]
00477be0 ldr r3, [r5, r3]
00477be4 ldr r3, [r3]
00477be8 cmp r3, #2
00477bec moveq r3, #0
00477bf0 streq r3, [r3]
00477bf4 beq #0x477ba4
00477bf8 cmp r3, #1
00477bfc bne #0x477ba4
00477c00 ldr r0, [pc, #0xa4]
00477c04 ldr r1, [pc, #0xa4]
00477c08 ldr r2, [pc, #0xa4]
00477c0c ldr r0, [r5, r0]
00477c10 ldr r3, [pc, #0xa0]
00477c14 mov ip, #0x32
00477c18 add r1, pc, r1
00477c1c add r2, pc, r2
00477c20 add r3, pc, r3
00477c24 add r0, r0, #0xa8
00477c28 str ip, [sp]
00477c2c bl #0x30e004
00477c30 b #0x477ba4
00477c34 ldr r0, [pc, #0x70]
00477c38 ldr r1, [pc, #0x7c]
00477c3c ldr r2, [pc, #0x7c]
00477c40 ldr r0, [r5, r0]
00477c44 ldr r3, [pc, #0x78]
00477c48 mov ip, #0x30
00477c4c add r1, pc, r1
00477c50 add r0, r0, #0xa8
00477c54 add r2, pc, r2
00477c58 add r3, pc, r3
00477c5c str ip, [sp]
00477c60 bl #0x30e004
00477c64 ldr r8, [r4, #0x20]
00477c68 b #0x477b40
00477c6c ldr r0, [pc, #0x38]
00477c70 ldr r1, [pc, #0x50]
00477c74 ldr r2, [pc, #0x50]
00477c78 ldr r0, [r5, r0]
00477c7c ldr r3, [pc, #0x4c]
00477c80 mov ip, #0x31
00477c84 add r1, pc, r1
00477c88 add r2, pc, r2
00477c8c add r3, pc, r3
00477c90 add r0, r0, #0xa8
00477c94 str ip, [sp]
00477c98 bl #0x30e004
00477c9c b #0x477b78
00477ca0 subseq ip, r1, r8, ror #31
00477ca4 muleq r0, r8, r3
00477ca8 andeq r3, r0, r0, asr #19
00477cac andeq r1, r0, r0, asr #19
00477cb0 subeq r6, r4, r0, asr #15
00477cb4 subeq r5, r5, r4, lsr #27
00477cb8 subeq r5, r5, r0, lsr sp
00477cbc subeq r6, r4, ip, lsl #15
00477cc0 subeq r5, r5, r4, ror #25
00477cc4 strdeq r5, r6, [r5], #-0xc8
00477cc8 subeq r6, r4, r4, asr r7
00477ccc subeq r5, r5, r0, lsr #26
00477cd0 subeq r5, r5, r4, asr #25
# _ZN13AnchorForwardC2EP10GameObjectfffN10AnchorBase10AnchorTypeE 477cd4 596
00477cd4 push {r4, r5, r6, r7, r8, sl, lr}
00477cd8 sub sp, sp, #0x1c
00477cdc mov r6, r2
00477ce0 ldr r5, [pc, #0x20c]
00477ce4 ldr r2, [sp, #0x3c]
00477ce8 mov r4, r0
00477cec mov r8, r3
00477cf0 mov r7, r1
00477cf4 bl #0x4771a0
00477cf8 ldr r3, [pc, #0x1f8]
00477cfc add r5, pc, r5
00477d00 mov r1, #0
00477d04 ldr r3, [r5, r3]
00477d08 mov sl, #0
00477d0c mov r0, r6
00477d10 add r3, r3, #8
00477d14 str r3, [r4]
00477d18 ldr r3, [sp, #0x38]
00477d1c str r6, [r4, #0x1c]
00477d20 str r8, [r4, #0x20]
00477d24 str r3, [r4, #0x24]
00477d28 mov r3, #5
00477d2c str r3, [r4, #0x3c]
00477d30 str sl, [r4, #0x28]
00477d34 str r1, [r4, #0x2c]
00477d38 str r1, [r4, #0x30]
00477d3c str r1, [r4, #0x34]
00477d40 strb sl, [r4, #0x38]
00477d44 str r1, [r4, #0x40]
00477d48 str r1, [r4, #0x44]
00477d4c str r1, [r4, #0x48]
00477d50 strb sl, [r4, #0x4c]
00477d54 str sl, [r4, #0x50]
00477d58 str r1, [r4, #0x54]
00477d5c str r1, [r4, #0x58]
00477d60 str r1, [r4, #0x5c]
00477d64 str r1, [r4, #0x60]
00477d68 bl #0x30e4b4
00477d6c cmp r0, sl
00477d70 bne #0x477d94
00477d74 ldr r3, [pc, #0x180]
00477d78 ldr r3, [r5, r3]
00477d7c ldr r3, [r3]
00477d80 cmp r3, #2
00477d84 streq sl, [sl]
00477d88 beq #0x477d94
00477d8c cmp r3, #1
00477d90 beq #0x477e88
00477d94 mov r0, r8
00477d98 mov r1, #0
00477d9c bl #0x30e4b4
00477da0 cmp r0, #0
00477da4 bne #0x477dcc
00477da8 ldr r3, [pc, #0x14c]
00477dac ldr r3, [r5, r3]
00477db0 ldr r3, [r3]
00477db4 cmp r3, #2
00477db8 moveq r3, #0
00477dbc streq r3, [r3]
00477dc0 beq #0x477dcc
00477dc4 cmp r3, #1
00477dc8 beq #0x477ec0
00477dcc ldr r6, [r4, #0x24]
00477dd0 mov r1, #0
00477dd4 mov r0, r6
00477dd8 bl #0x30e4b4
00477ddc cmp r0, #0
00477de0 beq #0x477e30
00477de4 mov r0, r6
00477de8 mov r1, #0x3f800000
00477dec bl #0x30e9ac
00477df0 cmp r0, #0
00477df4 beq #0x477e30
00477df8 cmp r7, #0
00477dfc beq #0x477e1c
00477e00 add r5, sp, #0xc
00477e04 mov r1, r7
00477e08 mov r0, r5
00477e0c bl #0x33dd2c
00477e10 mov r0, r5
00477e14 bl #0x33ff54
00477e18 str r0, [r4, #0x28]
00477e1c mov r0, r4
00477e20 bl #0x4779dc
00477e24 mov r0, r4
00477e28 add sp, sp, #0x1c
00477e2c pop {r4, r5, r6, r7, r8, sl, pc}
00477e30 ldr r3, [pc, #0xc4]
00477e34 ldr r3, [r5, r3]
00477e38 ldr r3, [r3]
00477e3c cmp r3, #2
00477e40 moveq r3, #0
00477e44 streq r3, [r3]
00477e48 beq #0x477df8
00477e4c cmp r3, #1
00477e50 bne #0x477df8
00477e54 ldr r0, [pc, #0xa4]
00477e58 ldr r1, [pc, #0xa4]
00477e5c ldr r2, [pc, #0xa4]
00477e60 ldr r0, [r5, r0]
00477e64 ldr r3, [pc, #0xa0]
00477e68 mov ip, #0x32
00477e6c add r1, pc, r1
00477e70 add r2, pc, r2
00477e74 add r3, pc, r3
00477e78 add r0, r0, #0xa8
00477e7c str ip, [sp]
00477e80 bl #0x30e004
00477e84 b #0x477df8
00477e88 ldr r0, [pc, #0x70]
00477e8c ldr r1, [pc, #0x7c]
00477e90 ldr r2, [pc, #0x7c]
00477e94 ldr r0, [r5, r0]
00477e98 ldr r3, [pc, #0x78]
00477e9c mov ip, #0x30
00477ea0 add r1, pc, r1
00477ea4 add r0, r0, #0xa8
00477ea8 add r2, pc, r2
00477eac add r3, pc, r3
00477eb0 str ip, [sp]
00477eb4 bl #0x30e004
00477eb8 ldr r8, [r4, #0x20]
00477ebc b #0x477d94
00477ec0 ldr r0, [pc, #0x38]
00477ec4 ldr r1, [pc, #0x50]
00477ec8 ldr r2, [pc, #0x50]
00477ecc ldr r0, [r5, r0]
00477ed0 ldr r3, [pc, #0x4c]
00477ed4 mov ip, #0x31
00477ed8 add r1, pc, r1
00477edc add r2, pc, r2
00477ee0 add r3, pc, r3
00477ee4 add r0, r0, #0xa8
00477ee8 str ip, [sp]
00477eec bl #0x30e004
00477ef0 b #0x477dcc
# _ZN11AnchorGroupC1EP10GameObjectN10AnchorBase10AnchorTypeE 478578 60
00478578 push {r4, r5, r6, lr}
0047857c ldr r4, [pc, #0x28]
00478580 mov r5, r0
00478584 bl #0x4771a0
00478588 ldr r3, [pc, #0x20]
0047858c add r4, pc, r4
00478590 mov r0, r5
00478594 ldr r3, [r4, r3]
00478598 add r3, r3, #8
0047859c str r3, [r5]
004785a0 bl #0x478520
004785a4 mov r0, r5
004785a8 pop {r4, r5, r6, pc}
004785ac subseq ip, r1, r4, lsl #10
004785b0 andeq r4, r0, r0, lsl #5
# _ZN11AnchorGroupC2EP10GameObjectN10AnchorBase10AnchorTypeE 4785b4 60
004785b4 push {r4, r5, r6, lr}
004785b8 ldr r4, [pc, #0x28]
004785bc mov r5, r0
004785c0 bl #0x4771a0
004785c4 ldr r3, [pc, #0x20]
004785c8 add r4, pc, r4
004785cc mov r0, r5
004785d0 ldr r3, [r4, r3]
004785d4 add r3, r3, #8
004785d8 str r3, [r5]
004785dc bl #0x478520
004785e0 mov r0, r5
004785e4 pop {r4, r5, r6, pc}
004785e8 subseq ip, r1, r8, asr #9
004785ec andeq r4, r0, r0, lsl #5
