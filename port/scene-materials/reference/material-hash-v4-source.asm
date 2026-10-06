_ZNK6glitch5video9CMaterial12getTechniqueEv 0x5c5d34 72
005c5d34 ldr r2, [r0, #4]
005c5d38 ldrb ip, [r0, #0x14]
005c5d3c ldrb r0, [r0, #8]
005c5d40 ldr r3, [r2, #4]
005c5d44 ldrh r1, [r2, #0xc]
005c5d48 ldr r2, [r3, #0x108]
005c5d4c ldr r3, [r3, #0x104]
005c5d50 cmp r2, #0
005c5d54 bxeq lr
005c5d58 ldr r3, [r3, #0x18]
005c5d5c add r1, r3, r1, lsl #3
005c5d60 ldr r3, [r1, #4]
005c5d64 ldr r3, [r3, #0x1c]
005c5d68 cmn r3, #1
005c5d6c ldrne r2, [r2, ip, lsl #2]
005c5d70 addne r0, r0, r3
005c5d74 ldrbne r0, [r2, r0]
005c5d78 bx lr
_ZNK6glitch5video9CMaterial14updateHashCodeEh 0x5c5fd8 188
005c5fd8 push {r4, r5, r6, lr}
005c5fdc mov r4, r0
005c5fe0 mov r5, r1
005c5fe4 bl #0x5c5d34
005c5fe8 ldr r3, [r4, #4]
005c5fec mov r2, #0xc
005c5ff0 ldr r3, [r3, #0x18]
005c5ff4 mla r2, r2, r0, r3
005c5ff8 ldrb r2, [r2, #4]
005c5ffc cmp r2, #1
005c6000 bls #0x5c6024
005c6004 ldr r3, [r4, #0x18]
005c6008 mvn r2, #0
005c600c str r2, [r3, r5, lsl #2]
005c6010 ldr r3, [r4, #0x10]
005c6014 mov r2, #1
005c6018 bic r5, r3, r2, lsl r5
005c601c str r5, [r4, #0x10]
005c6020 pop {r4, r5, r6, pc}
005c6024 ldr r2, [r4, #0xc]
005c6028 lsr r2, r2, r5
005c602c tst r2, #1
005c6030 bne #0x5c6068
005c6034 mov r2, #0xc
005c6038 mla r3, r2, r5, r3
005c603c ldrb r2, [r3, #4]
005c6040 cmp r2, #1
005c6044 bls #0x5c6080
005c6048 mov r1, r5
005c604c mov r0, r4
005c6050 bl #0x5c5fa4
005c6054 ldr r3, [r4, #0x10]
005c6058 mov r2, #1
005c605c bic r5, r3, r2, lsl r5
005c6060 str r5, [r4, #0x10]
005c6064 pop {r4, r5, r6, pc}
005c6068 mov r0, r4
005c606c mov r1, r5
005c6070 bl #0x5c5d88
005c6074 ldr r3, [r4, #4]
005c6078 ldr r3, [r3, #0x18]
005c607c b #0x5c6034
005c6080 ldr r3, [r3, #8]
005c6084 ldrb r3, [r3, #0x30]
005c6088 cmp r3, #0
005c608c beq #0x5c6010
005c6090 b #0x5c6048
_ZNK6glitch5video9CMaterial25updateRenderStateHashCodeEh 0x5c5fa4 52
005c5fa4 ldr r3, [r0, #4]
005c5fa8 ldr r2, [r3, #0x18]
005c5fac ldr r3, [r0, #0x18]
005c5fb0 mov r0, #0xc
005c5fb4 mla r2, r0, r1, r2
005c5fb8 ldr r0, [r3, r1, lsl #2]
005c5fbc ldr ip, [r2, #8]
005c5fc0 bic r0, r0, #0xf00
005c5fc4 ldrb r2, [ip]
005c5fc8 and r2, r2, #0xf
005c5fcc orr r2, r0, r2, lsl #8
005c5fd0 str r2, [r3, r1, lsl #2]
005c5fd4 bx lr
_ZNK6glitch5video9CMaterial24updateParametersHashCodeEh 0x5c5d88 540
005c5d88 push {r4, r5, r6, r7, r8, sb, sl, fp}
005c5d8c sub sp, sp, #0x10
005c5d90 str r0, [sp]
005c5d94 ldr r3, [r0, #4]
005c5d98 str r1, [sp, #4]
005c5d9c add fp, r0, #0x20
005c5da0 ldr r2, [r3, #0x18]
005c5da4 ldr r0, [sp, #4]
005c5da8 mov r1, #0xc
005c5dac ldr sb, [pc, #0x1e4]
005c5db0 mla r2, r1, r0, r2
005c5db4 add sb, pc, sb
005c5db8 ldr r1, [r2, #8]
005c5dbc ldr r2, [r1, #0x20]
005c5dc0 ldr r6, [r1, #0x24]
005c5dc4 ldrh r8, [r2, #0x36]
005c5dc8 ldrh r0, [r2, #0x2e]
005c5dcc ldrh r1, [r2, #0x2c]
005c5dd0 ldrh r2, [r2, #0x34]
005c5dd4 add r8, r8, r0
005c5dd8 uxth r8, r8
005c5ddc rsb r8, r1, r8
005c5de0 rsb r8, r2, r8
005c5de4 uxth r8, r8
005c5de8 add r8, r6, r8, lsl #1
005c5dec cmp r8, r6
005c5df0 moveq r1, #0
005c5df4 moveq r5, r1
005c5df8 beq #0x5c5ea0
005c5dfc ldr r1, [pc, #0x198]
005c5e00 ldr r2, [pc, #0x198]
005c5e04 mov ip, #0xd
005c5e08 str r1, [sp, #8]
005c5e0c mov r1, #0
005c5e10 str r2, [sp, #0xc]
005c5e14 mov r5, r1
005c5e18 ldrh r2, [r6]
005c5e1c tst r2, #0x8000
005c5e20 bne #0x5c5e94
005c5e24 ldrh r0, [r3, #0xe]
005c5e28 cmp r0, r2
005c5e2c ldrhi r0, [r3, #0x20]
005c5e30 movls r2, #0
005c5e34 addhi r2, r0, r2, lsl #4
005c5e38 ldrh r0, [r2, #4]
005c5e3c ldr sl, [r2, #8]
005c5e40 cmp r0, #2
005c5e44 beq #0x5c5edc
005c5e48 cmp r0, #0xb
005c5e4c beq #0x5c5e94
005c5e50 cmp r0, #0xf
005c5e54 beq #0x5c5e94
005c5e58 ldrb r0, [r2, #6]
005c5e5c cmp r0, #0xb
005c5e60 beq #0x5c5f10
005c5e64 ldr r7, [sp, #8]
005c5e68 ldr r2, [r2, #0xc]
005c5e6c ldr r4, [sb, r7]
005c5e70 add r2, fp, r2
005c5e74 ldrb r0, [r4, r0]
005c5e78 mla r0, sl, r0, r2
005c5e7c cmp r2, r0
005c5e80 beq #0x5c5e94
005c5e84 ldrb r4, [r2], #1
005c5e88 cmp r2, r0
005c5e8c mla r1, ip, r1, r4
005c5e90 bne #0x5c5e84
005c5e94 add r6, r6, #2
005c5e98 cmp r8, r6
005c5e9c bne #0x5c5e18
005c5ea0 ldm sp, {r0, r7}
005c5ea4 lsl r5, r5, #0x14
005c5ea8 ldr r2, [r0, #0x18]
005c5eac and r1, r1, #0xff
005c5eb0 lsr r5, r5, #0x14
005c5eb4 ldr r3, [r2, r7, lsl #2]
005c5eb8 bic r3, r3, #0xff0000
005c5ebc bic r3, r3, #0xf000
005c5ec0 bic r3, r3, #0xff
005c5ec4 orr r3, r1, r3
005c5ec8 orr r5, r3, r5, lsl #12
005c5ecc str r5, [r2, r7, lsl #2]
005c5ed0 add sp, sp, #0x10
005c5ed4 pop {r4, r5, r6, r7, r8, sb, sl, fp}
005c5ed8 bx lr
005c5edc ldr r2, [r2, #0xc]
005c5ee0 add r2, fp, r2
005c5ee4 add sl, r2, sl, lsl #2
005c5ee8 cmp r2, sl
005c5eec beq #0x5c5e94
005c5ef0 ldrb r0, [r2], #1
005c5ef4 cmp r2, sl
005c5ef8 mla r5, ip, r5, r0
005c5efc bne #0x5c5ef0
005c5f00 add r6, r6, #2
005c5f04 cmp r8, r6
005c5f08 bne #0x5c5e18
005c5f0c b #0x5c5ea0
005c5f10 ldr r7, [r2, #0xc]
005c5f14 add r7, fp, r7
005c5f18 add sl, r7, sl, lsl #2
005c5f1c cmp r7, sl
005c5f20 beq #0x5c5e94
005c5f24 mov r4, r3
005c5f28 ldr r0, [r7]
005c5f2c cmp r0, #0
005c5f30 beq #0x5c5f6c
005c5f34 mov r3, #0
005c5f38 ldrb r2, [r0, r3]
005c5f3c add r3, r3, #1
005c5f40 cmp r3, #0x44
005c5f44 mla r1, ip, r1, r2
005c5f48 bne #0x5c5f38
005c5f4c add r7, r7, #4
005c5f50 cmp sl, r7
005c5f54 bne #0x5c5f28
005c5f58 add r6, r6, #2
005c5f5c cmp r8, r6
005c5f60 mov r3, r4
005c5f64 bne #0x5c5e18
005c5f68 b #0x5c5ea0
005c5f6c ldr r2, [sp, #0xc]
005c5f70 ldr r3, [sb, r2]
005c5f74 ldrb r2, [r0, r3]
005c5f78 add r0, r0, #1
005c5f7c cmp r0, #0x44
005c5f80 mla r1, ip, r1, r2
005c5f84 bne #0x5c5f74
005c5f88 add r7, r7, #4
005c5f8c cmp sl, r7
005c5f90 bne #0x5c5f28
005c5f94 b #0x5c5f58
005c5f98 ldrsbteq lr, [ip], -ip
005c5f9c andeq r1, r0, r0, asr #11
005c5fa0 andeq r2, r0, r0, lsr r8
_ZN6glitch5video9CMaterialC1ERKN5boost13intrusive_ptrINS0_17CMaterialRendererEEEPKcRKNS1_24SStateWithoutRenderStateEPKhPKNS0_6detail8material12SRenderStateE 0x5cbc70 288
005cbc70 push {r4, r5, r6, lr}
005cbc74 mov r4, r0
005cbc78 mov r0, #0
005cbc7c str r0, [r4]
005cbc80 mov r5, r1
005cbc84 ldr r1, [r1]
005cbc88 cmp r1, r0
005cbc8c str r1, [r4, #4]
005cbc90 ldrne r0, [r1]
005cbc94 addne r0, r0, #1
005cbc98 strne r0, [r1]
005cbc9c ldrb r1, [r3]
005cbca0 mov r0, r2
005cbca4 strb r1, [r4, #8]
005cbca8 ldr r2, [r3, #4]
005cbcac mov r1, #1
005cbcb0 str r2, [r4, #0xc]
005cbcb4 ldr r2, [r3, #8]
005cbcb8 str r2, [r4, #0x10]
005cbcbc ldrb r3, [r3, #0xc]
005cbcc0 mov r2, #0
005cbcc4 str r2, [r4, #0x18]
005cbcc8 strb r3, [r4, #0x14]
005cbccc bl #0x6a5074
005cbcd0 cmp r0, #0
005cbcd4 str r0, [r4, #0x1c]
005cbcd8 ldrne r3, [r0]
005cbcdc addne r3, r3, #1
005cbce0 strne r3, [r0]
005cbce4 ldr r3, [r5]
005cbce8 mov r0, r3
005cbcec ldr r5, [r3, #0x14]
005cbcf0 bl #0x5c5d7c
005cbcf4 add r5, r5, #0x20
005cbcf8 add r5, r5, r0
005cbcfc add r5, r4, r5
005cbd00 str r5, [r4, #0x18]
005cbd04 ldr r1, [sp, #0x10]
005cbd08 ldr r2, [sp, #0x14]
005cbd0c mov r0, r4
005cbd10 mov r3, #0
005cbd14 bl #0x5cbbe0
005cbd18 ldr r1, [r4, #4]
005cbd1c ldrb r6, [r1, #0x10]
005cbd20 cmp r6, #0
005cbd24 beq #0x5cbd88
005cbd28 sub r6, r6, #1
005cbd2c uxtb r3, r6
005cbd30 mov r6, #0xc
005cbd34 mla r6, r3, r6, r6
005cbd38 mov r3, #0
005cbd3c mov r2, r3
005cbd40 b #0x5cbd48
005cbd44 ldr r1, [r4, #4]
005cbd48 ldr r0, [r1, #0x18]
005cbd4c ldr r1, [r4, #0x18]
005cbd50 add r0, r0, r2
005cbd54 ldr r0, [r0, #8]
005cbd58 ldr ip, [r1, r3]
005cbd5c add r2, r2, #0xc
005cbd60 ldr r5, [r0, #0x20]
005cbd64 bic ip, ip, #0xff000000
005cbd68 cmp r2, r6
005cbd6c ldrh r0, [r5, #0x40]
005cbd70 and r5, r0, #0xff
005cbd74 eor r0, r5, r0, lsr #8
005cbd78 orr r0, ip, r0, lsl #24
005cbd7c str r0, [r1, r3]
005cbd80 add r3, r3, #4
005cbd84 bne #0x5cbd44
005cbd88 mov r0, r4
005cbd8c pop {r4, r5, r6, pc}
_ZN6glitch5video7IShaderC1EtPKcPNS0_12IVideoDriverE 0x5e4a4c 148
005e4a4c ldr ip, [pc, #0x84]
005e4a50 push {r4, r5, r6, lr}
005e4a54 ldr lr, [pc, #0x80]
005e4a58 add ip, pc, ip
005e4a5c mov r5, #0
005e4a60 ldr lr, [ip, lr]
005e4a64 sub sp, sp, #8
005e4a68 mov r4, r0
005e4a6c add lr, lr, #8
005e4a70 str r3, [r0, #8]
005e4a74 str lr, [r0]
005e4a78 mov r6, r1
005e4a7c str r5, [r0, #4]
005e4a80 mov r1, r2
005e4a84 add r0, r0, #0xc
005e4a88 add r2, sp, #4
005e4a8c bl #0x32603c
005e4a90 add r3, r4, #0x30
005e4a94 str r5, [r4, #0x24]
005e4a98 str r5, [r4, #0x28]
005e4a9c strh r5, [r4, #0x2c]
005e4aa0 strh r5, [r4, #0x2e]
005e4aa4 str r5, [r4, #0x30]
005e4aa8 strh r5, [r3, #6]
005e4aac strh r5, [r3, #4]
005e4ab0 mvn r3, #0
005e4ab4 strb r3, [r4, #0x3d]
005e4ab8 mov r3, #1
005e4abc str r5, [r4, #0x38]
005e4ac0 strb r5, [r4, #0x3c]
005e4ac4 strb r3, [r4, #0x3e]
005e4ac8 strh r6, [r4, #0x40]
005e4acc mov r0, r4
005e4ad0 add sp, sp, #8
005e4ad4 pop {r4, r5, r6, pc}
005e4ad8 eorseq r0, fp, r8, lsr r0
005e4adc andeq r4, r0, ip, ror r7
_ZN6glitch5video11CGLSLShader11linkProgramEv 0x6de9f8 1444
006de9f8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006de9fc mov r4, r0
006dea00 sub sp, sp, #0x4c
006dea04 ldr r0, [r0, #0x4c]
006dea08 bl #0x30ed78
006dea0c mov r6, #0
006dea10 add r2, sp, #0x48
006dea14 str r6, [r2, #-4]!
006dea18 ldr r0, [r4, #0x4c]
006dea1c movw r1, #0x8b82
006dea20 bl #0x30e28c
006dea24 ldr r5, [sp, #0x44]
006dea28 cmp r5, r6
006dea2c bne #0x6dea9c
006dea30 add r2, sp, #0x48
006dea34 str r5, [r2, #-0x1c]!
006dea38 movw r1, #0x8b84
006dea3c ldr r0, [r4, #0x4c]
006dea40 bl #0x30e28c
006dea44 ldr r0, [sp, #0x2c]
006dea48 bl #0x5345f4
006dea4c mov r6, r0
006dea50 ldr r1, [sp, #0x2c]
006dea54 ldr r0, [r4, #0x4c]
006dea58 add r2, sp, #0x30
006dea5c mov r3, r6
006dea60 bl #0x30e214
006dea64 ldr r1, [pc, #0x524]
006dea68 mov r0, #3
006dea6c ldr r2, [r4, #0x20]
006dea70 add r1, pc, r1
006dea74 mov r3, r6
006dea78 bl #0x60b034
006dea7c cmp r6, #0
006dea80 strb r5, [r4, #0x3e]
006dea84 beq #0x6dee44
006dea88 mov r0, r6
006dea8c bl #0x534688
006dea90 mov r0, r5
006dea94 add sp, sp, #0x4c
006dea98 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006dea9c add r7, sp, #0x48
006deaa0 str r6, [r7, #-0x18]!
006deaa4 ldr r0, [r4, #0x4c]
006deaa8 movw r1, #0x8b84
006deaac mov r2, r7
006deab0 bl #0x30e28c
006deab4 ldr r0, [sp, #0x30]
006deab8 cmp r0, #1
006deabc ble #0x6deaec
006deac0 bl #0x5345f4
006deac4 mov r5, r0
006deac8 ldr r1, [sp, #0x30]
006deacc ldr r0, [r4, #0x4c]
006dead0 add r2, sp, #0x2c
006dead4 mov r3, r5
006dead8 bl #0x30e214
006deadc cmp r5, #0
006deae0 beq #0x6deaec
006deae4 mov r0, r5
006deae8 bl #0x534688
006deaec mov r5, #0
006deaf0 add r2, sp, #0x48
006deaf4 str r5, [r2, #-8]!
006deaf8 ldr r0, [r4, #0x4c]
006deafc movw r1, #0x8b89
006deb00 bl #0x30e28c
006deb04 add r2, sp, #0x48
006deb08 str r5, [r2, #-0xc]!
006deb0c movw r1, #0x8b86
006deb10 ldr r0, [r4, #0x4c]
006deb14 bl #0x30e28c
006deb18 mov r0, r4
006deb1c bl #0x6de924
006deb20 add r2, sp, #0x48
006deb24 str r5, [r2, #-0x10]!
006deb28 ldr r0, [r4, #0x4c]
006deb2c movw r1, #0x8b8a
006deb30 bl #0x30e28c
006deb34 ldr r3, [sp, #0x3c]
006deb38 cmp r3, r5
006deb3c ble #0x6deb4c
006deb40 ldr r6, [sp, #0x38]
006deb44 cmp r6, r5
006deb48 beq #0x6dee30
006deb4c add r2, sp, #0x48
006deb50 mov r5, #0
006deb54 str r5, [r2, #-0x14]!
006deb58 ldr r0, [r4, #0x4c]
006deb5c movw r1, #0x8b87
006deb60 bl #0x30e28c
006deb64 ldr r6, [sp, #0x34]
006deb68 cmp r6, r5
006deb6c beq #0x6deeec
006deb70 ldr r3, [sp, #0x40]
006deb74 ldr r0, [sp, #0x3c]
006deb78 mov r1, r5
006deb7c lsl r3, r3, #3
006deb80 add r0, r3, r0, lsl #4
006deb84 str r3, [sp, #0x1c]
006deb88 bl #0x5341a8
006deb8c str r0, [sp, #0x18]
006deb90 ldr r3, [sp, #0x40]
006deb94 ldr r0, [sp, #0x38]
006deb98 ldr r1, [sp, #0x18]
006deb9c strb r3, [r4, #0x3c]
006deba0 add r0, r0, #1
006deba4 str r1, [r4, #0x24]
006deba8 bl #0x5345f4
006debac ldr r3, [sp, #0x40]
006debb0 mov r6, r0
006debb4 cmp r3, #0
006debb8 ble #0x6dec7c
006debbc add sl, sp, #0x2c
006debc0 mov r8, #1
006debc4 b #0x6debe4
006debc8 ldr r3, [r4, #0x38]
006debcc add r5, r5, #1
006debd0 orr sb, r3, r8, lsl sb
006debd4 ldr r3, [sp, #0x40]
006debd8 str sb, [r4, #0x38]
006debdc cmp r3, r5
006debe0 ble #0x6dec7c
006debe4 ldr r0, [r4, #0x4c]
006debe8 mov r1, r5
006debec ldr r2, [sp, #0x38]
006debf0 mov r3, #0
006debf4 str sl, [sp]
006debf8 str r7, [sp, #4]
006debfc str r6, [sp, #8]
006dec00 bl #0x30e01c
006dec04 mov r0, r6
006dec08 bl #0x6dbb50
006dec0c cmp r0, #0x1d
006dec10 mov sb, r0
006dec14 bgt #0x6debc8
006dec18 mov r1, r6
006dec1c ldr r0, [r4, #0x4c]
006dec20 bl #0x30e73c
006dec24 mov r1, #1
006dec28 uxth r3, r0
006dec2c mov r0, r6
006dec30 ldr fp, [r4, #0x24]
006dec34 str r3, [sp, #0x14]
006dec38 bl #0x6a5074
006dec3c str r0, [fp, r5, lsl #3]
006dec40 cmp r0, #0
006dec44 ldrne r2, [r0]
006dec48 ldr r3, [sp, #0x14]
006dec4c add fp, fp, r5, lsl #3
006dec50 addne r2, r2, #1
006dec54 strne r2, [r0]
006dec58 strh sb, [fp, #4]
006dec5c strh r3, [fp, #6]
006dec60 ldr r3, [r4, #0x38]
006dec64 add r5, r5, #1
006dec68 orr sb, r3, r8, lsl sb
006dec6c ldr r3, [sp, #0x40]
006dec70 str sb, [r4, #0x38]
006dec74 cmp r3, r5
006dec78 bgt #0x6debe4
006dec7c cmp r6, #0
006dec80 beq #0x6dec8c
006dec84 mov r0, r6
006dec88 bl #0x534688
006dec8c ldr r3, [sp, #0x3c]
006dec90 cmp r3, #0
006dec94 beq #0x6dee28
006dec98 ldr r2, [sp, #0x18]
006dec9c ldr ip, [sp, #0x1c]
006deca0 ldr r0, [sp, #0x34]
006deca4 add r2, r2, ip
006deca8 str r2, [sp, #0x24]
006decac add r0, r0, #1
006decb0 strh r3, [r4, #0x2e]
006decb4 str r2, [r4, #0x28]
006decb8 bl #0x5345f4
006decbc ldr r1, [sp, #0x3c]
006decc0 mvn r3, #0
006decc4 mov r6, r0
006decc8 cmp r1, #0
006deccc strb r3, [r4, #0x3d]
006decd0 ble #0x6dee08
006decd4 ldr r5, [sp, #0x24]
006decd8 add sl, sp, #0x2c
006decdc mov r8, #0
006dece0 str sl, [sp, #0x1c]
006dece4 str r7, [sp, #0x20]
006dece8 ldr ip, [sp, #0x20]
006decec ldr r0, [r4, #0x4c]
006decf0 mov r1, r8
006decf4 str ip, [sp]
006decf8 ldr ip, [sp, #0x1c]
006decfc mov r3, #0
006ded00 ldr r2, [sp, #0x34]
006ded04 str ip, [sp, #4]
006ded08 str r6, [sp, #8]
006ded0c bl #0x30e6d0
006ded10 ldr r3, [sp, #0x2c]
006ded14 movw r1, #0x8b57
006ded18 cmp r3, r1
006ded1c beq #0x6def38
006ded20 bhi #0x6dee7c
006ded24 movw r2, #0x8b52
006ded28 cmp r3, r2
006ded2c moveq sb, #8
006ded30 beq #0x6ded5c
006ded34 bhi #0x6dee4c
006ded38 movw r2, #0x1406
006ded3c cmp r3, r2
006ded40 moveq sb, #5
006ded44 beq #0x6ded5c
006ded48 bhi #0x6def6c
006ded4c movw r2, #0x1404
006ded50 cmp r3, r2
006ded54 beq #0x6dee74
006ded58 mov sb, #0xff
006ded5c mov r0, r6
006ded60 bl #0x5e2284
006ded64 cmp r0, #0xff
006ded68 subne r1, r0, #0x13
006ded6c mov r7, r0
006ded70 strne r1, [sp, #0x18]
006ded74 uxthne sl, r0
006ded78 beq #0x6deec4
006ded7c mov r1, r6
006ded80 ldr r0, [r4, #0x4c]
006ded84 bl #0x30e13c
006ded88 mov r1, r7
006ded8c mov r3, r0
006ded90 mov r0, r6
006ded94 str r3, [sp, #0x14]
006ded98 bl #0x5e7f80
006ded9c mov r1, #1
006deda0 mov r7, r0
006deda4 mov r0, r6
006deda8 ldr fp, [sp, #0x30]
006dedac bl #0x6a5074
006dedb0 cmp r0, #0
006dedb4 str r0, [r5]
006dedb8 ldrne r2, [r0]
006dedbc ldr r3, [sp, #0x14]
006dedc0 addne r2, r2, #1
006dedc4 strne r2, [r0]
006dedc8 ldr ip, [sp, #0x18]
006dedcc strh sl, [r5, #4]
006dedd0 strb sb, [r5, #6]
006dedd4 cmp ip, #8
006dedd8 str fp, [r5, #8]
006deddc str r3, [r5, #0xc]
006dede0 strb r7, [r5, #7]
006dede4 bhi #0x6dedf4
006dede8 ldrb r3, [r4, #0x3d]
006dedec cmp r3, r7
006dedf0 strbhi r7, [r4, #0x3d]
006dedf4 ldr r1, [sp, #0x3c]
006dedf8 add r8, r8, #1
006dedfc add r5, r5, #0x10
006dee00 cmp r1, r8
006dee04 bgt #0x6dece8
006dee08 mov r5, #1
006dee0c strb r5, [r4, #0x50]
006dee10 ldr r0, [sp, #0x24]
006dee14 uxth r1, r1
006dee18 bl #0x5e7b50
006dee1c cmp r6, #0
006dee20 strh r0, [r4, #0x2c]
006dee24 bne #0x6dea88
006dee28 mov r0, #1
006dee2c b #0x6dea94
006dee30 ldr r1, [pc, #0x15c]
006dee34 ldr r0, [r4, #0x20]
006dee38 mov r2, #3
006dee3c add r1, pc, r1
006dee40 bl #0x60ace8
006dee44 mov r0, r6
006dee48 b #0x6dea94
006dee4c movw r2, #0x8b54
006dee50 cmp r3, r2
006dee54 beq #0x6def40
006dee58 blo #0x6def38
006dee5c movw r2, #0x8b55
006dee60 cmp r3, r2
006dee64 beq #0x6def08
006dee68 movw r2, #0x8b56
006dee6c cmp r3, r2
006dee70 bne #0x6ded58
006dee74 mov sb, #1
006dee78 b #0x6ded5c
006dee7c movw r2, #0x8b5c
006dee80 cmp r3, r2
006dee84 moveq sb, #0xb
006dee88 beq #0x6ded5c
006dee8c bhi #0x6def10
006dee90 movw r2, #0x8b59
006dee94 cmp r3, r2
006dee98 beq #0x6def08
006dee9c blo #0x6def40
006deea0 movw r2, #0x8b5a
006deea4 cmp r3, r2
006deea8 moveq sb, #9
006deeac beq #0x6ded5c
006deeb0 movw r2, #0x8b5b
006deeb4 cmp r3, r2
006deeb8 bne #0x6ded58
006deebc mov sb, #0xa
006deec0 b #0x6ded5c
006deec4 sub r3, sb, #0xc
006deec8 cmp r3, #3
006deecc movhi sl, #0
006deed0 mvnhi r2, #0x12
006deed4 movls sl, #2
006deed8 mvnls r3, #0x10
006deedc strhi r2, [sp, #0x18]
006deee0 strls r3, [sp, #0x18]
006deee4 mov r7, sl
006deee8 b #0x6ded7c
006deeec ldr r1, [pc, #0xa4]
006deef0 ldr r0, [r4, #0x20]
006deef4 mov r2, #3
006deef8 add r1, pc, r1
006deefc bl #0x60ace8
006def00 mov r0, r6
006def04 b #0x6dea94
006def08 mov sb, #4
006def0c b #0x6ded5c
006def10 movw ip, #0x8b5f
006def14 cmp r3, ip
006def18 moveq sb, #0xd
006def1c beq #0x6ded5c
006def20 bhi #0x6def48
006def24 movw r2, #0x8b5e
006def28 cmp r3, r2
006def2c bne #0x6ded58
006def30 mov sb, #0xc
006def34 b #0x6ded5c
006def38 mov sb, #2
006def3c b #0x6ded5c
006def40 mov sb, #3
006def44 b #0x6ded5c
006def48 movw r2, #0x8b60
006def4c cmp r3, r2
006def50 moveq sb, #0xe
006def54 beq #0x6ded5c
006def58 movw r2, #0x8b63
006def5c cmp r3, r2
006def60 bne #0x6ded58
006def64 mov sb, #0xf
006def68 b #0x6ded5c
006def6c movw r2, #0x8b50
006def70 cmp r3, r2
006def74 moveq sb, #6
006def78 beq #0x6ded5c
006def7c movw r2, #0x8b51
006def80 cmp r3, r2
006def84 bne #0x6ded58
006def88 mov sb, #7
006def8c b #0x6ded5c
006def90 eoreq r0, r1, r8, asr r3
006def94 .byte 0xbc, 0xff, 0x20, 0x00
006def98 eoreq pc, r0, r8, lsr #30
_ZN6glitch5video9CMaterial4initEPKhPKNS0_6detail8material12SRenderStateEb 0x5cbbe0 92
005cbbe0 push {r4, lr}
005cbbe4 ldr r2, [r0, #4]
005cbbe8 sub sp, sp, #8
005cbbec mov r4, r0
005cbbf0 ldrh ip, [r2, #0xe]
005cbbf4 cmp ip, #0
005cbbf8 beq #0x5cbc20
005cbbfc cmp r3, #0
005cbc00 bne #0x5cbc28
005cbc04 ldr r2, [r2, #0x14]
005cbc08 add r0, r4, #0x20
005cbc0c bl #0x30e868
005cbc10 mov r0, r4
005cbc14 add sp, sp, #8
005cbc18 pop {r4, lr}
005cbc1c b #0x5cb8dc
005cbc20 add sp, sp, #8
005cbc24 pop {r4, pc}
005cbc28 str r1, [sp, #4]
005cbc2c bl #0x5cbba4
005cbc30 ldr r2, [r4, #4]
005cbc34 ldr r1, [sp, #4]
005cbc38 b #0x5cbc04
_ZN6glitch5video24guessShaderParameterTypeEPKc 0x5e2284 9492
005e2284 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005e2288 ldr r3, [pc, #0x284]
005e228c ldr r4, [pc, #0x284]
005e2290 sub sp, sp, #0x730
005e2294 add r3, pc, r3
005e2298 ldr r5, [r3, #0x110]
005e229c sub sp, sp, #4
005e22a0 add r4, pc, r4
005e22a4 ands r5, r5, #1
005e22a8 str r0, [sp, #0xc]
005e22ac beq #0x5e2428
005e22b0 ldr r0, [sp, #0xc]
005e22b4 bl #0x30de54
005e22b8 mov r6, r0
005e22bc bl #0x534254
005e22c0 mov r7, r0
005e22c4 mov r0, #1
005e22c8 bl #0x534268
005e22cc add r0, r6, #1
005e22d0 bl #0x5345f4
005e22d4 ldr r3, [sp, #0xc]
005e22d8 mov r5, r0
005e22dc add r6, r3, r6
005e22e0 cmp r3, r6
005e22e4 moveq r0, r0
005e22e8 beq #0x5e2350
005e22ec ldr lr, [pc, #0x228]
005e22f0 ldr r8, [pc, #0x228]
005e22f4 ldr r3, [sp, #0xc]
005e22f8 mov r0, r5
005e22fc mov r1, #0
005e2300 ldrsb r2, [r3]
005e2304 cmp r2, #0x5b
005e2308 addeq r1, r1, #1
005e230c beq #0x5e2344
005e2310 cmp r2, #0x5d
005e2314 subeq r1, r1, #1
005e2318 beq #0x5e2344
005e231c cmp r1, #0
005e2320 bne #0x5e2344
005e2324 cmn r2, #1
005e2328 beq #0x5e2420
005e232c ldr ip, [r4, lr]
005e2330 ldr ip, [ip]
005e2334 uxtab ip, ip, r2
005e2338 ldrb ip, [ip, #1]
005e233c tst ip, #4
005e2340 beq #0x5e240c
005e2344 add r3, r3, #1
005e2348 cmp r3, r6
005e234c bne #0x5e2300
005e2350 ldr r6, [pc, #0x1cc]
005e2354 mov r3, #0
005e2358 strb r3, [r0]
005e235c add r6, pc, r6
005e2360 ldr r4, [r6, #0x118]
005e2364 cmp r4, r3
005e2368 addeq r4, r6, #0x114
005e236c beq #0x5e23cc
005e2370 add r6, r6, #0x114
005e2374 b #0x5e237c
005e2378 mov r4, r3
005e237c ldr r0, [r4, #0x10]
005e2380 mov r1, r5
005e2384 bl #0x30e31c
005e2388 cmp r0, #0
005e238c ldrlt r3, [r4, #0xc]
005e2390 ldrge r3, [r4, #8]
005e2394 movlt r4, r6
005e2398 mov r6, r4
005e239c cmp r3, #0
005e23a0 bne #0x5e2378
005e23a4 ldr r6, [pc, #0x17c]
005e23a8 add r6, pc, r6
005e23ac add r6, r6, #0x114
005e23b0 cmp r4, r6
005e23b4 beq #0x5e23cc
005e23b8 ldr r1, [r4, #0x10]
005e23bc mov r0, r5
005e23c0 bl #0x30e31c
005e23c4 cmp r0, #0
005e23c8 movlt r4, r6
005e23cc ldr r3, [pc, #0x158]
005e23d0 add r3, pc, r3
005e23d4 add r3, r3, #0x114
005e23d8 cmp r4, r3
005e23dc moveq r4, #0xff
005e23e0 ldrne r4, [r4, #0x14]
005e23e4 cmp r5, #0
005e23e8 beq #0x5e23f4
005e23ec mov r0, r5
005e23f0 bl #0x534688
005e23f4 mov r0, r7
005e23f8 bl #0x534268
005e23fc mov r0, r4
005e2400 add sp, sp, #0x334
005e2404 add sp, sp, #0x400
005e2408 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005e240c cmp r2, #0xff
005e2410 ldrls ip, [r4, r8]
005e2414 ldrls ip, [ip]
005e2418 addls r2, ip, r2, lsl #1
005e241c ldrshls r2, [r2, #2]
005e2420 strb r2, [r0], #1
005e2424 b #0x5e2344
005e2428 add r0, r3, #0x110
005e242c bl #0x30e76c
005e2430 cmp r0, #0
005e2434 beq #0x5e22b0
005e2438 add r6, sp, #0x30
005e243c sub r6, r6, #4
005e2440 mov r1, r5
005e2444 mov r0, r6
005e2448 str r5, [sp, #0x2c]
005e244c str r5, [sp, #0x30]
005e2450 str r5, [sp, #0x34]
005e2454 str r5, [sp, #0x38]
005e2458 str r5, [sp, #0x3c]
005e245c str r5, [sp, #0x40]
005e2460 str r5, [sp, #0x44]
005e2464 str r5, [sp, #0x48]
005e2468 str r5, [sp, #0x4c]
005e246c str r5, [sp, #0x50]
005e2470 bl #0x5e1e00
005e2474 ldr r1, [sp, #0x44]
005e2478 ldr r3, [pc, #0xb0]
005e247c ldr r2, [sp, #0x3c]
005e2480 sub r1, r1, #8
005e2484 add r3, pc, r3
005e2488 cmp r2, r1
005e248c mov r1, #0x32
005e2490 str r1, [sp, #0x88]
005e2494 str r3, [sp, #0x84]
005e2498 beq #0x5e45d0
005e249c str r3, [r2]
005e24a0 ldr r3, [sp, #0x88]
005e24a4 add fp, sp, #0x90
005e24a8 str r3, [r2, #4]
005e24ac ldr r3, [sp, #0x3c]
005e24b0 add r3, r3, #8
005e24b4 str r3, [sp, #0x3c]
005e24b8 add r3, sp, #0x60
005e24bc sub r3, r3, #0xc
005e24c0 mov r0, r3
005e24c4 mov r1, r6
005e24c8 str r3, [sp, #0x1c]
005e24cc bl #0x5e1eac
005e24d0 ldr r3, [sp, #0x2c]
005e24d4 ldr r2, [sp, #0x34]
005e24d8 ldr r1, [sp, #0x3c]
005e24dc ldr r0, [sp, #0x38]
005e24e0 cmp r1, r3
005e24e4 beq #0x5e2754
005e24e8 add r3, r3, #8
005e24ec cmp r3, r2
005e24f0 beq #0x5e2508
005e24f4 cmp r1, r3
005e24f8 add r3, r3, #8
005e24fc beq #0x5e2754
005e2500 cmp r2, r3
005e2504 bne #0x5e24f4
005e2508 ldr r3, [r0, #4]!
005e250c add r2, r3, #0x80
005e2510 b #0x5e24e0
005e2514 ldrdeq r4, r5, [r1], #-0x7c
005e2518 ldrshteq r2, [fp], -r0
005e251c ldrdeq r1, r2, [r0], -ip
005e2520 andeq r3, r0, r0, ror #13
005e2524 subeq r4, r1, r4, lsl r7
005e2528 subeq r4, r1, r8, asr #13
005e252c subeq r4, r1, r0, lsr #13
005e2530 .byte 0xbc, 0xf5, 0x2f, 0x00
005e2534 eoreq pc, pc, r4, ror #5
005e2538 .byte 0xdc, 0xf2, 0x2f, 0x00
005e253c eoreq pc, pc, r8, asr #5
005e2540 eoreq pc, pc, r4, asr #5
005e2544 eoreq pc, pc, r8, lsr #5
005e2548 eoreq pc, pc, r4, lsr #5
005e254c mlaeq pc, r4, r2, pc
005e2550 eoreq pc, pc, r8, ror r2
005e2554 eoreq pc, pc, r4, ror r2
005e2558 eoreq pc, pc, r0, ror #4
005e255c eoreq pc, pc, r8, asr #4
005e2560 eoreq pc, pc, r0, lsr r2
005e2564 eoreq pc, pc, r8, lsr #4
005e2568 eoreq pc, pc, r0, lsr #4
005e256c eoreq pc, pc, r0, lsr #4
005e2570 eoreq pc, pc, r8, lsl r2
005e2574 eoreq pc, pc, ip, lsl #4
005e2578 eoreq pc, pc, ip, lsl #4
005e257c eoreq pc, pc, ip, lsl #4
005e2580 eoreq pc, pc, r4, lsl #4
005e2584 eoreq pc, pc, ip, lsl #4
005e2588 eoreq pc, pc, r4, lsl #4
005e258c eoreq pc, pc, r8, lsl #4
005e2590 eoreq pc, pc, r0, lsl #4
005e2594 .byte 0xf0, 0xf1, 0x2f, 0x00
005e2598 eoreq pc, pc, r8, ror #3
005e259c .byte 0xd8, 0xf1, 0x2f, 0x00
005e25a0 .byte 0xd0, 0xf1, 0x2f, 0x00
005e25a4 eoreq pc, pc, ip, asr #3
005e25a8 eoreq pc, pc, r4, asr #3
005e25ac .byte 0xbc, 0xf1, 0x2f, 0x00
005e25b0 .byte 0xb8, 0xf1, 0x2f, 0x00
005e25b4 eoreq pc, pc, ip, lsr #3
005e25b8 mlaeq pc, r4, r1, pc
005e25bc eoreq pc, pc, ip, lsl #3
005e25c0 eoreq pc, pc, ip, lsl #3
005e25c4 eoreq pc, pc, r4, lsl #3
005e25c8 eoreq pc, pc, r4, lsl #3
005e25cc eoreq pc, pc, ip, ror r1
005e25d0 eoreq pc, pc, ip, ror r1
005e25d4 eoreq pc, pc, ip, ror r1
005e25d8 eoreq pc, pc, r4, ror r1
005e25dc eoreq pc, pc, r4, ror r1
005e25e0 eoreq pc, pc, r0, ror r1
005e25e4 eoreq lr, pc, r0, ror #29
005e25e8 eoreq pc, pc, r8, asr #2
005e25ec eoreq pc, pc, ip, asr #2
005e25f0 eoreq pc, pc, ip, lsr r1
005e25f4 eoreq pc, pc, r4, lsr r1
005e25f8 eoreq pc, pc, r4, lsr #2
005e25fc eoreq pc, pc, ip, lsl r1
005e2600 eoreq pc, pc, r0, lsl r1
005e2604 eoreq pc, pc, r8, lsl #2
005e2608 eoreq pc, pc, r8, lsr r3
005e260c eoreq pc, pc, r0, lsr r3
005e2610 eoreq pc, pc, r0, asr #1
005e2614 strhteq pc, [pc], -r4
005e2618 .byte 0xfc, 0xf3, 0x2f, 0x00
005e261c eoreq pc, pc, r4, lsl #1
005e2620 eoreq pc, pc, r8, ror r0
005e2624 eoreq pc, pc, r0, ror r0
005e2628 eoreq pc, pc, ip, rrx
005e262c eoreq pc, pc, r4, asr r0
005e2630 eoreq pc, pc, r4, lsr r0
005e2634 eoreq pc, pc, r8, lsr #32
005e2638 eoreq pc, pc, r0, lsr #32
005e263c eoreq pc, pc, ip, lsl r0
005e2640 eoreq pc, pc, ip
005e2644 .byte 0xfc, 0xef, 0x2f, 0x00
005e2648 eoreq lr, pc, ip, ror #31
005e264c .byte 0xd8, 0xef, 0x2f, 0x00
005e2650 .byte 0xd0, 0xef, 0x2f, 0x00
005e2654 eoreq lr, pc, r8, asr pc
005e2658 mlaeq pc, r4, pc, lr
005e265c eoreq lr, pc, r4, lsl #31
005e2660 eoreq lr, pc, r4, ror pc
005e2664 eoreq lr, pc, r4, ror #30
005e2668 eoreq lr, pc, r4, asr pc
005e266c eoreq lr, pc, ip, lsr pc
005e2670 eoreq lr, pc, ip, lsr #30
005e2674 eoreq lr, pc, r4, lsr #30
005e2678 eoreq lr, pc, ip, lsl #30
005e267c .byte 0xfc, 0xee, 0x2f, 0x00
005e2680 .byte 0xf0, 0xee, 0x2f, 0x00
005e2684 eoreq lr, pc, ip, ror #29
005e2688 eoreq lr, pc, r4, ror #29
005e268c .byte 0xdc, 0xee, 0x2f, 0x00
005e2690 eoreq lr, pc, r8, asr #29
005e2694 eoreq pc, pc, r8, asr #2
005e2698 eoreq pc, pc, r8, lsr r1
005e269c eoreq lr, pc, ip, ror lr
005e26a0 eoreq lr, pc, r4, ror lr
005e26a4 eoreq lr, pc, r4, ror lr
005e26a8 eoreq lr, pc, ip, ror #28
005e26ac eoreq lr, pc, r0, ror lr
005e26b0 eoreq lr, pc, ip, ror #28
005e26b4 eoreq lr, pc, r8, ror #28
005e26b8 eoreq lr, pc, ip, ror #28
005e26bc eoreq lr, pc, r8, ror #28
005e26c0 eoreq lr, pc, r4, ror #28
005e26c4 eoreq lr, pc, r8, ror #28
005e26c8 eoreq lr, pc, r8, asr lr
005e26cc eoreq lr, pc, r4, asr lr
005e26d0 eoreq lr, pc, r8, asr #28
005e26d4 eoreq lr, pc, ip, lsr lr
005e26d8 eoreq lr, pc, r8, lsr lr
005e26dc eoreq lr, pc, r8, lsr #28
005e26e0 eoreq lr, pc, ip, lsl lr
005e26e4 eoreq lr, pc, r0, lsl lr
005e26e8 eoreq lr, pc, r4, lsl lr
005e26ec eoreq lr, pc, r8, lsl #28
005e26f0 eoreq lr, pc, ip, lsl #28
005e26f4 .byte 0xfc, 0xed, 0x2f, 0x00
005e26f8 .byte 0xf0, 0xed, 0x2f, 0x00
005e26fc eoreq lr, pc, r4, ror #27
005e2700 .byte 0xd0, 0xed, 0x2f, 0x00
005e2704 eoreq lr, pc, r0, asr #27
005e2708 .byte 0xf8, 0xe6, 0x2f, 0x00
005e270c eoreq lr, pc, ip, ror #13
005e2710 eoreq lr, pc, r0, lsl #27
005e2714 eoreq lr, pc, r0, ror sp
005e2718 eoreq lr, pc, r8, ror #26
005e271c eoreq lr, pc, r0, ror #26
005e2720 eoreq lr, pc, r8, asr sp
005e2724 eoreq lr, pc, r4, asr sp
005e2728 eoreq lr, pc, ip, asr #26
005e272c eoreq lr, pc, ip, lsr sp
005e2730 eoreq lr, pc, r8, lsr sp
005e2734 eoreq lr, pc, r0, lsr sp
005e2738 eoreq lr, pc, r0, lsr #26
005e273c eoreq lr, pc, r8, lsl sp
005e2740 eoreq lr, pc, r8, lsl #26
005e2744 eoreq lr, pc, r0, lsl #26
005e2748 eoreq lr, pc, r0, lsl #26
005e274c .byte 0xf8, 0xec, 0x2f, 0x00
005e2750 .byte 0xf0, 0xec, 0x2f, 0x00
005e2754 mov r0, r6
005e2758 bl #0x5e1d1c
005e275c ldr r1, [pc, #-0x230]
005e2760 mov r5, #0x32
005e2764 add r2, sp, #0x730
005e2768 ldr r0, [sp, #0x1c]
005e276c add r1, pc, r1
005e2770 str r5, [r2, #-4]!
005e2774 bl #0x5e222c
005e2778 ldr r3, [pc, #-0x248]
005e277c add r1, sp, #0x640
005e2780 add r1, r1, #4
005e2784 add r3, pc, r3
005e2788 mov r6, r0
005e278c str r3, [sp, #0x644]
005e2790 str r5, [sp, #0x648]
005e2794 bl #0x5e2134
005e2798 ldr r1, [pc, #-0x264]
005e279c add r2, sp, #0x730
005e27a0 str r5, [r2, #-8]!
005e27a4 mov r0, r6
005e27a8 add r1, pc, r1
005e27ac bl #0x5e222c
005e27b0 ldr r3, [pc, #-0x278]
005e27b4 add r1, sp, #0x630
005e27b8 add r1, r1, #0xc
005e27bc add r3, pc, r3
005e27c0 mov r6, r0
005e27c4 str r3, [sp, #0x63c]
005e27c8 str r5, [sp, #0x640]
005e27cc bl #0x5e2134
005e27d0 ldr r1, [pc, #-0x294]
005e27d4 add r2, sp, #0x730
005e27d8 str r5, [r2, #-0xc]!
005e27dc mov r0, r6
005e27e0 add r1, pc, r1
005e27e4 bl #0x5e217c
005e27e8 ldr r3, [pc, #-0x2a8]
005e27ec add r1, sp, #0x630
005e27f0 add r1, r1, #4
005e27f4 add r3, pc, r3
005e27f8 mov r6, r0
005e27fc str r5, [sp, #0x638]
005e2800 str r3, [sp, #0x634]
005e2804 bl #0x5e2134
005e2808 ldr r3, [pc, #-0x2c4]
005e280c add r1, sp, #0x620
005e2810 mov r0, r6
005e2814 add r3, pc, r3
005e2818 add r1, r1, #0xc
005e281c str r3, [sp, #0x62c]
005e2820 str r5, [sp, #0x630]
005e2824 bl #0x5e2134
005e2828 ldr r1, [pc, #-0x2e0]
005e282c add r2, sp, #0x730
005e2830 str r5, [r2, #-0x10]!
005e2834 mov r0, r6
005e2838 add r1, pc, r1
005e283c bl #0x5e217c
005e2840 ldr r3, [pc, #-0x2f4]
005e2844 add r1, sp, #0x620
005e2848 add r1, r1, #4
005e284c add r3, pc, r3
005e2850 mov r6, r0
005e2854 str r5, [sp, #0x628]
005e2858 str r3, [sp, #0x624]
005e285c bl #0x5e2134
005e2860 ldr r3, [pc, #-0x310]
005e2864 add r1, sp, #0x610
005e2868 mov r5, #0x27
005e286c mov r0, r6
005e2870 add r3, pc, r3
005e2874 add r1, r1, #0xc
005e2878 str r3, [sp, #0x61c]
005e287c str r5, [sp, #0x620]
005e2880 bl #0x5e2134
005e2884 ldr r3, [pc, #-0x330]
005e2888 add r1, sp, #0x610
005e288c mov r0, r6
005e2890 add r3, pc, r3
005e2894 add r1, r1, #4
005e2898 str r3, [sp, #0x614]
005e289c str r5, [sp, #0x618]
005e28a0 bl #0x5e2134
005e28a4 ldr r3, [pc, #-0x34c]
005e28a8 add r1, sp, #0x600
005e28ac mov r0, r6
005e28b0 add r3, pc, r3
005e28b4 add r1, r1, #0xc
005e28b8 str r3, [sp, #0x60c]
005e28bc str r5, [sp, #0x610]
005e28c0 bl #0x5e2134
005e28c4 ldr r3, [pc, #-0x368]
005e28c8 add r1, sp, #0x600
005e28cc mov r0, r6
005e28d0 add r3, pc, r3
005e28d4 add r1, r1, #4
005e28d8 str r3, [sp, #0x604]
005e28dc str r5, [sp, #0x608]
005e28e0 bl #0x5e2134
005e28e4 ldr r3, [pc, #-0x384]
005e28e8 add r1, sp, #0x5f0
005e28ec mov r0, r6
005e28f0 add r3, pc, r3
005e28f4 add r1, r1, #0xc
005e28f8 str r3, [sp, #0x5fc]
005e28fc str r5, [sp, #0x600]
005e2900 bl #0x5e2134
005e2904 ldr r3, [pc, #-0x3a0]
005e2908 add r1, sp, #0x5f0
005e290c mov r0, r6
005e2910 add r3, pc, r3
005e2914 add r1, r1, #4
005e2918 str r3, [sp, #0x5f4]
005e291c str r5, [sp, #0x5f8]
005e2920 bl #0x5e2134
005e2924 ldr r3, [pc, #-0x3bc]
005e2928 add r1, sp, #0x5e0
005e292c mov r0, r6
005e2930 add r3, pc, r3
005e2934 add r1, r1, #0xc
005e2938 str r3, [sp, #0x5ec]
005e293c str r5, [sp, #0x5f0]
005e2940 bl #0x5e2134
005e2944 ldr r3, [pc, #-0x3d8]
005e2948 add r1, sp, #0x5e0
005e294c mov r5, #0x31
005e2950 mov r0, r6
005e2954 add r3, pc, r3
005e2958 add r1, r1, #4
005e295c str r3, [sp, #0x5e4]
005e2960 str r5, [sp, #0x5e8]
005e2964 bl #0x5e2134
005e2968 ldr r3, [pc, #-0x3f8]
005e296c add r1, sp, #0x5d0
005e2970 mov r0, r6
005e2974 add r3, pc, r3
005e2978 add r1, r1, #0xc
005e297c str r3, [sp, #0x5dc]
005e2980 str r5, [sp, #0x5e0]
005e2984 bl #0x5e2134
005e2988 ldr r3, [pc, #-0x414]
005e298c add r1, sp, #0x5d0
005e2990 mov r0, r6
005e2994 add r3, pc, r3
005e2998 add r1, r1, #4
005e299c str r3, [sp, #0x5d4]
005e29a0 str r5, [sp, #0x5d8]
005e29a4 bl #0x5e2134
005e29a8 ldr r3, [pc, #-0x430]
005e29ac add r1, sp, #0x5c0
005e29b0 mov r0, r6
005e29b4 add r3, pc, r3
005e29b8 add r1, r1, #0xc
005e29bc str r3, [sp, #0x5cc]
005e29c0 str r5, [sp, #0x5d0]
005e29c4 bl #0x5e2134
005e29c8 ldr r3, [pc, #-0x44c]
005e29cc add r1, sp, #0x5c0
005e29d0 mov r0, r6
005e29d4 add r3, pc, r3
005e29d8 add r1, r1, #4
005e29dc str r3, [sp, #0x5c4]
005e29e0 str r5, [sp, #0x5c8]
005e29e4 bl #0x5e2134
005e29e8 ldr r3, [pc, #-0x468]
005e29ec add r1, sp, #0x5b0
005e29f0 mov r0, r6
005e29f4 add r3, pc, r3
005e29f8 add r1, r1, #0xc
005e29fc str r3, [sp, #0x5bc]
005e2a00 str r5, [sp, #0x5c0]
005e2a04 bl #0x5e2134
005e2a08 ldr r1, [pc, #-0x484]
005e2a0c mov r5, #0x2b
005e2a10 add r2, sp, #0x730
005e2a14 str r5, [r2, #-0x14]!
005e2a18 add r1, pc, r1
005e2a1c mov r0, r6
005e2a20 bl #0x5e222c
005e2a24 ldr r3, [pc, #-0x49c]
005e2a28 add r1, sp, #0x5b0
005e2a2c add r1, r1, #4
005e2a30 add r3, pc, r3
005e2a34 mov r6, r0
005e2a38 str r3, [sp, #0x5b4]
005e2a3c str r5, [sp, #0x5b8]
005e2a40 bl #0x5e2134
005e2a44 ldr r1, [pc, #-0x4b8]
005e2a48 add r2, sp, #0x730
005e2a4c str r5, [r2, #-0x18]!
005e2a50 add r1, pc, r1
005e2a54 mov r0, r6
005e2a58 bl #0x5e222c
005e2a5c ldr r3, [pc, #-0x4cc]
005e2a60 add r1, sp, #0x5a0
005e2a64 add r1, r1, #0xc
005e2a68 add r3, pc, r3
005e2a6c str r3, [sp, #0x5ac]
005e2a70 mov r6, r0
005e2a74 str r5, [sp, #0x5b0]
005e2a78 bl #0x5e2134
005e2a7c ldr r1, [pc, #-0x4e8]
005e2a80 add r2, sp, #0x730
005e2a84 str r5, [r2, #-0x1c]!
005e2a88 add r1, pc, r1
005e2a8c mov r0, r6
005e2a90 bl #0x5e217c
005e2a94 ldr r1, [pc, #-0x4fc]
005e2a98 add r2, sp, #0x730
005e2a9c str r5, [r2, #-0x20]!
005e2aa0 add r1, pc, r1
005e2aa4 bl #0x5e21d4
005e2aa8 ldr r3, [pc, #-0x50c]
005e2aac add r1, sp, #0x5a0
005e2ab0 add r1, r1, #4
005e2ab4 add r3, pc, r3
005e2ab8 str r3, [sp, #0x5a4]
005e2abc mov r6, r0
005e2ac0 str r5, [sp, #0x5a8]
005e2ac4 bl #0x5e2134
005e2ac8 ldr r1, [pc, #-0x528]
005e2acc add r2, sp, #0x730
005e2ad0 str r5, [r2, #-0x24]!
005e2ad4 add r1, pc, r1
005e2ad8 mov r0, r6
005e2adc bl #0x5e217c
005e2ae0 ldr r1, [pc, #-0x53c]
005e2ae4 add r2, sp, #0x730
005e2ae8 str r5, [r2, #-0x28]!
005e2aec add r1, pc, r1
005e2af0 bl #0x5e21d4
005e2af4 ldr r3, [pc, #-0x54c]
005e2af8 add r1, sp, #0x590
005e2afc add r1, r1, #0xc
005e2b00 add r3, pc, r3
005e2b04 mov r6, r0
005e2b08 str r5, [sp, #0x5a0]
005e2b0c str r3, [sp, #0x59c]
005e2b10 bl #0x5e2134
005e2b14 ldr r3, [pc, #-0x568]
005e2b18 add r1, sp, #0x590
005e2b1c mov r5, #0x35
005e2b20 mov r0, r6
005e2b24 add r3, pc, r3
005e2b28 add r1, r1, #4
005e2b2c str r3, [sp, #0x594]
005e2b30 str r5, [sp, #0x598]
005e2b34 bl #0x5e2134
005e2b38 ldr r3, [pc, #-0x588]
005e2b3c add r1, sp, #0x580
005e2b40 mov r0, r6
005e2b44 add r3, pc, r3
005e2b48 add r1, r1, #0xc
005e2b4c str r3, [sp, #0x58c]
005e2b50 str r5, [sp, #0x590]
005e2b54 bl #0x5e2134
005e2b58 ldr r3, [pc, #-0x5a4]
005e2b5c add r1, sp, #0x580
005e2b60 mov r0, r6
005e2b64 add r3, pc, r3
005e2b68 add r1, r1, #4
005e2b6c str r3, [sp, #0x584]
005e2b70 str r5, [sp, #0x588]
005e2b74 bl #0x5e2134
005e2b78 ldr r3, [pc, #-0x5c0]
005e2b7c add r1, sp, #0x570
005e2b80 mov r0, r6
005e2b84 add r3, pc, r3
005e2b88 add r1, r1, #0xc
005e2b8c str r3, [sp, #0x57c]
005e2b90 str r5, [sp, #0x580]
005e2b94 bl #0x5e2134
005e2b98 ldr r3, [pc, #-0x5dc]
005e2b9c add r1, sp, #0x570
005e2ba0 mov r0, r6
005e2ba4 add r3, pc, r3
005e2ba8 add r1, r1, #4
005e2bac str r3, [sp, #0x574]
005e2bb0 str r5, [sp, #0x578]
005e2bb4 bl #0x5e2134
005e2bb8 ldr r3, [pc, #-0x5f8]
005e2bbc add r1, sp, #0x560
005e2bc0 mov r0, r6
005e2bc4 add r3, pc, r3
005e2bc8 add r1, r1, #0xc
005e2bcc str r3, [sp, #0x56c]
005e2bd0 str r5, [sp, #0x570]
005e2bd4 bl #0x5e2134
005e2bd8 ldr r3, [pc, #-0x614]
005e2bdc add r1, sp, #0x560
005e2be0 mov r0, r6
005e2be4 add r3, pc, r3
005e2be8 add r1, r1, #4
005e2bec str r3, [sp, #0x564]
005e2bf0 str r5, [sp, #0x568]
005e2bf4 bl #0x5e2134
005e2bf8 ldr r3, [pc, #-0x630]
005e2bfc add r1, sp, #0x550
005e2c00 mov r0, r6
005e2c04 add r3, pc, r3
005e2c08 add r1, r1, #0xc
005e2c0c str r3, [sp, #0x55c]
005e2c10 str r5, [sp, #0x560]
005e2c14 bl #0x5e2134
005e2c18 ldr r3, [pc, #-0x64c]
005e2c1c add r1, sp, #0x550
005e2c20 mov r0, r6
005e2c24 add r3, pc, r3
005e2c28 add r1, r1, #4
005e2c2c str r3, [sp, #0x554]
005e2c30 str r5, [sp, #0x558]
005e2c34 bl #0x5e2134
005e2c38 ldr r3, [pc, #-0x668]
005e2c3c add r1, sp, #0x540
005e2c40 mov r0, r6
005e2c44 add r3, pc, r3
005e2c48 add r1, r1, #0xc
005e2c4c str r3, [sp, #0x54c]
005e2c50 str r5, [sp, #0x550]
005e2c54 bl #0x5e2134
005e2c58 ldr r3, [pc, #-0x684]
005e2c5c add r1, sp, #0x540
005e2c60 mov r0, r6
005e2c64 add r3, pc, r3
005e2c68 add r1, r1, #4
005e2c6c str r3, [sp, #0x544]
005e2c70 str r5, [sp, #0x548]
005e2c74 bl #0x5e2134
005e2c78 ldr r3, [pc, #-0x6a0]
005e2c7c add r1, sp, #0x530
005e2c80 mov r5, #0x2f
005e2c84 mov r0, r6
005e2c88 add r3, pc, r3
005e2c8c add r1, r1, #0xc
005e2c90 str r3, [sp, #0x53c]
005e2c94 str r5, [sp, #0x540]
005e2c98 bl #0x5e2134
005e2c9c ldr r3, [pc, #-0x6c0]
005e2ca0 add r1, sp, #0x530
005e2ca4 mov r0, r6
005e2ca8 add r3, pc, r3
005e2cac add r1, r1, #4
005e2cb0 str r3, [sp, #0x534]
005e2cb4 str r5, [sp, #0x538]
005e2cb8 bl #0x5e2134
005e2cbc ldr r3, [pc, #-0x6dc]
005e2cc0 add r1, sp, #0x520
005e2cc4 mov r0, r6
005e2cc8 add r3, pc, r3
005e2ccc add r1, r1, #0xc
005e2cd0 str r3, [sp, #0x52c]
005e2cd4 str r5, [sp, #0x530]
005e2cd8 bl #0x5e2134
005e2cdc ldr r3, [pc, #-0x6f8]
005e2ce0 add r1, sp, #0x520
005e2ce4 mov r5, #0x36
005e2ce8 mov r0, r6
005e2cec add r3, pc, r3
005e2cf0 add r1, r1, #4
005e2cf4 str r3, [sp, #0x524]
005e2cf8 str r5, [sp, #0x528]
005e2cfc bl #0x5e2134
005e2d00 ldr r3, [pc, #-0x718]
005e2d04 add r1, sp, #0x510
005e2d08 mov r0, r6
005e2d0c add r3, pc, r3
005e2d10 add r1, r1, #0xc
005e2d14 str r3, [sp, #0x51c]
005e2d18 str r5, [sp, #0x520]
005e2d1c bl #0x5e2134
005e2d20 ldr r3, [pc, #-0x734]
005e2d24 add r1, sp, #0x510
005e2d28 mov r0, r6
005e2d2c add r3, pc, r3
005e2d30 add r1, r1, #4
005e2d34 str r3, [sp, #0x514]
005e2d38 str r5, [sp, #0x518]
005e2d3c bl #0x5e2134
005e2d40 ldr r1, [pc, #-0x750]
005e2d44 add r2, sp, #0x730
005e2d48 str r5, [r2, #-0x2c]!
005e2d4c add r1, pc, r1
005e2d50 mov r0, r6
005e2d54 bl #0x5e222c
005e2d58 ldr r3, [pc, #-0x764]
005e2d5c add r1, sp, #0x500
005e2d60 add r1, r1, #0xc
005e2d64 add r3, pc, r3
005e2d68 mov r6, r0
005e2d6c str r5, [sp, #0x510]
005e2d70 str r3, [sp, #0x50c]
005e2d74 bl #0x5e2134
005e2d78 ldr r3, [pc, #-0x780]
005e2d7c add r1, sp, #0x500
005e2d80 mov r5, #0x2a
005e2d84 mov r0, r6
005e2d88 add r3, pc, r3
005e2d8c add r1, r1, #4
005e2d90 str r3, [sp, #0x504]
005e2d94 str r5, [sp, #0x508]
005e2d98 bl #0x5e2134
005e2d9c ldr r3, [pc, #-0x7a0]
005e2da0 add r1, sp, #0x4f0
005e2da4 mov r0, r6
005e2da8 add r3, pc, r3
005e2dac add r1, r1, #0xc
005e2db0 str r3, [sp, #0x4fc]
005e2db4 str r5, [sp, #0x500]
005e2db8 bl #0x5e2134
005e2dbc ldr r3, [pc, #-0x7bc]
005e2dc0 add r1, sp, #0x4f0
005e2dc4 mov r0, r6
005e2dc8 add r3, pc, r3
005e2dcc add r1, r1, #4
005e2dd0 str r3, [sp, #0x4f4]
005e2dd4 str r5, [sp, #0x4f8]
005e2dd8 bl #0x5e2134
005e2ddc ldr r3, [pc, #-0x7d8]
005e2de0 add r1, sp, #0x4e0
005e2de4 mov r0, r6
005e2de8 add r3, pc, r3
005e2dec add r1, r1, #0xc
005e2df0 str r3, [sp, #0x4ec]
005e2df4 str r5, [sp, #0x4f0]
005e2df8 bl #0x5e2134
005e2dfc ldr r3, [pc, #-0x7f4]
005e2e00 add r1, sp, #0x4e0
005e2e04 mov r0, r6
005e2e08 add r3, pc, r3
005e2e0c add r1, r1, #4
005e2e10 str r3, [sp, #0x4e4]
005e2e14 str r5, [sp, #0x4e8]
005e2e18 bl #0x5e2134
005e2e1c ldr r3, [pc, #-0x810]
005e2e20 add r1, sp, #0x4d0
005e2e24 mov r5, #0x2e
005e2e28 mov r0, r6
005e2e2c add r3, pc, r3
005e2e30 add r1, r1, #0xc
005e2e34 str r3, [sp, #0x4dc]
005e2e38 str r5, [sp, #0x4e0]
005e2e3c bl #0x5e2134
005e2e40 ldr r3, [pc, #-0x830]
005e2e44 add r1, sp, #0x4d0
005e2e48 mov r0, r6
005e2e4c add r3, pc, r3
005e2e50 add r1, r1, #4
005e2e54 str r3, [sp, #0x4d4]
005e2e58 str r5, [sp, #0x4d8]
005e2e5c bl #0x5e2134
005e2e60 ldr r3, [pc, #-0x84c]
005e2e64 add r1, sp, #0x4c0
005e2e68 mov r0, r6
005e2e6c add r3, pc, r3
005e2e70 add r1, r1, #0xc
005e2e74 str r3, [sp, #0x4cc]
005e2e78 str r5, [sp, #0x4d0]
005e2e7c bl #0x5e2134
005e2e80 ldr r1, [pc, #-0x868]
005e2e84 mov r5, #0x2d
005e2e88 add r2, sp, #0x730
005e2e8c str r5, [r2, #-0x30]!
005e2e90 add r1, pc, r1
005e2e94 mov r0, r6
005e2e98 bl #0x5e217c
005e2e9c ldr r1, [pc, #-0x880]
005e2ea0 add r2, sp, #0x730
005e2ea4 str r5, [r2, #-0x34]!
005e2ea8 add r1, pc, r1
005e2eac bl #0x5e217c
005e2eb0 ldr r7, [pc, #-0x890]
005e2eb4 add r1, sp, #0x4c0
005e2eb8 add r1, r1, #4
005e2ebc add r7, pc, r7
005e2ec0 mov r6, r0
005e2ec4 str r7, [sp, #0x4c4]
005e2ec8 str r5, [sp, #0x4c8]
005e2ecc bl #0x5e2134
005e2ed0 ldr r3, [pc, #-0x8ac]
005e2ed4 add r1, sp, #0x4b0
005e2ed8 mov r0, r6
005e2edc add r3, pc, r3
005e2ee0 add r1, r1, #0xc
005e2ee4 str r3, [sp, #0x4bc]
005e2ee8 str r5, [sp, #0x4c0]
005e2eec bl #0x5e2134
005e2ef0 add r1, sp, #0x4b0
005e2ef4 mov r0, r6
005e2ef8 add r1, r1, #4
005e2efc str r7, [sp, #0x4b4]
005e2f00 str r5, [sp, #0x4b8]
005e2f04 bl #0x5e2134
005e2f08 ldr r3, [pc, #-0x8e0]
005e2f0c add r1, sp, #0x4a0
005e2f10 mov r0, r6
005e2f14 add r3, pc, r3
005e2f18 add r1, r1, #0xc
005e2f1c str r3, [sp, #0x4ac]
005e2f20 str r5, [sp, #0x4b0]
005e2f24 bl #0x5e2134
005e2f28 ldr r1, [pc, #-0x8fc]
005e2f2c mov r5, #0x26
005e2f30 add r2, sp, #0x730
005e2f34 str r5, [r2, #-0x38]!
005e2f38 add r1, pc, r1
005e2f3c mov r0, r6
005e2f40 bl #0x5e21d4
005e2f44 ldr r1, [pc, #-0x914]
005e2f48 add r2, sp, #0x730
005e2f4c str r5, [r2, #-0x3c]!
005e2f50 add r1, pc, r1
005e2f54 bl #0x5e21d4
005e2f58 ldr r3, [pc, #-0x924]
005e2f5c add r1, sp, #0x4a0
005e2f60 add r1, r1, #4
005e2f64 add r3, pc, r3
005e2f68 mov r6, r0
005e2f6c str r5, [sp, #0x4a8]
005e2f70 str r3, [sp, #0x4a4]
005e2f74 bl #0x5e2134
005e2f78 ldr r3, [pc, #-0x940]
005e2f7c add r1, sp, #0x490
005e2f80 mov r0, r6
005e2f84 add r3, pc, r3
005e2f88 add r1, r1, #0xc
005e2f8c str r3, [sp, #0x49c]
005e2f90 str r5, [sp, #0x4a0]
005e2f94 bl #0x5e2134
005e2f98 ldr r3, [pc, #-0x95c]
005e2f9c add r1, sp, #0x490
005e2fa0 mov r0, r6
005e2fa4 add r3, pc, r3
005e2fa8 add r1, r1, #4
005e2fac str r3, [sp, #0x494]
005e2fb0 str r5, [sp, #0x498]
005e2fb4 bl #0x5e2134
005e2fb8 ldr r3, [pc, #-0x978]
005e2fbc add r1, sp, #0x480
005e2fc0 mov r0, r6
005e2fc4 add r3, pc, r3
005e2fc8 add r1, r1, #0xc
005e2fcc str r3, [sp, #0x48c]
005e2fd0 str r5, [sp, #0x490]
005e2fd4 bl #0x5e2134
005e2fd8 ldr r1, [pc, #-0x994]
005e2fdc add r2, sp, #0x730
005e2fe0 mov r5, #0x24
005e2fe4 str r5, [r2, #-0x40]!
005e2fe8 add r1, pc, r1
005e2fec mov r0, r6
005e2ff0 bl #0x5e217c
005e2ff4 ldr r3, [pc, #-0x9ac]
005e2ff8 add r1, sp, #0x480
005e2ffc add r1, r1, #4
005e3000 add r3, pc, r3
005e3004 mov r6, r0
005e3008 str r5, [sp, #0x488]
005e300c str r3, [sp, #0x484]
005e3010 bl #0x5e2134
005e3014 ldr r3, [pc, #-0x9c8]
005e3018 add r1, sp, #0x470
005e301c mov r0, r6
005e3020 add r3, pc, r3
005e3024 add r1, r1, #0xc
005e3028 str r3, [sp, #0x47c]
005e302c str r5, [sp, #0x480]
005e3030 bl #0x5e2134
005e3034 ldr r3, [pc, #-0x9e4]
005e3038 add r1, sp, #0x470
005e303c mov r5, #0x28
005e3040 mov r0, r6
005e3044 add r3, pc, r3
005e3048 add r1, r1, #4
005e304c str r3, [sp, #0x474]
005e3050 str r5, [sp, #0x478]
005e3054 bl #0x5e2134
005e3058 ldr r3, [pc, #-0xa04]
005e305c add r1, sp, #0x460
005e3060 mov r0, r6
005e3064 add r3, pc, r3
005e3068 add r1, r1, #0xc
005e306c str r3, [sp, #0x46c]
005e3070 str r5, [sp, #0x470]
005e3074 bl #0x5e2134
005e3078 ldr r3, [pc, #-0xa20]
005e307c add r1, sp, #0x460
005e3080 mov r0, r6
005e3084 add r3, pc, r3
005e3088 add r1, r1, #4
005e308c str r3, [sp, #0x464]
005e3090 str r5, [sp, #0x468]
005e3094 bl #0x5e2134
005e3098 ldr r3, [pc, #-0xa3c]
005e309c add r1, sp, #0x450
005e30a0 mov r0, r6
005e30a4 add r3, pc, r3
005e30a8 add r1, r1, #0xc
005e30ac str r3, [sp, #0x45c]
005e30b0 str r5, [sp, #0x460]
005e30b4 bl #0x5e2134
005e30b8 ldr r3, [pc, #-0xa58]
005e30bc add r1, sp, #0x450
005e30c0 mov r0, r6
005e30c4 add r3, pc, r3
005e30c8 add r1, r1, #4
005e30cc str r3, [sp, #0x454]
005e30d0 str r5, [sp, #0x458]
005e30d4 bl #0x5e2134
005e30d8 ldr r3, [pc, #-0xa74]
005e30dc add r1, sp, #0x440
005e30e0 mov r0, r6
005e30e4 add r3, pc, r3
005e30e8 add r1, r1, #0xc
005e30ec str r3, [sp, #0x44c]
005e30f0 str r5, [sp, #0x450]
005e30f4 bl #0x5e2134
005e30f8 ldr r1, [pc, #-0xa90]
005e30fc add r2, sp, #0x730
005e3100 str r5, [r2, #-0x44]!
005e3104 add r1, pc, r1
005e3108 mov r0, r6
005e310c bl #0x5e21d4
005e3110 ldr r3, [pc, #-0xaa4]
005e3114 add r1, sp, #0x440
005e3118 add r1, r1, #4
005e311c add r3, pc, r3
005e3120 mov r6, r0
005e3124 str r5, [sp, #0x448]
005e3128 str r3, [sp, #0x444]
005e312c bl #0x5e2134
005e3130 ldr r3, [pc, #-0xac0]
005e3134 add r1, sp, #0x430
005e3138 mov r0, r6
005e313c add r3, pc, r3
005e3140 add r1, r1, #0xc
005e3144 str r3, [sp, #0x43c]
005e3148 str r5, [sp, #0x440]
005e314c bl #0x5e2134
005e3150 ldr r1, [pc, #-0xadc]
005e3154 add r2, sp, #0x730
005e3158 str r5, [r2, #-0x48]!
005e315c add r1, pc, r1
005e3160 mov r0, r6
005e3164 bl #0x5e21d4
005e3168 ldr r1, [pc, #-0xaf0]
005e316c mov r5, #0x25
005e3170 add r2, sp, #0x730
005e3174 str r5, [r2, #-0x4c]!
005e3178 add r1, pc, r1
005e317c bl #0x5e222c
005e3180 ldr r3, [pc, #-0xb04]
005e3184 add r1, sp, #0x430
005e3188 add r1, r1, #4
005e318c add r3, pc, r3
005e3190 mov r6, r0
005e3194 str r3, [sp, #0x434]
005e3198 str r5, [sp, #0x438]
005e319c bl #0x5e2134
005e31a0 ldr r1, [pc, #-0xb20]
005e31a4 add r2, sp, #0x730
005e31a8 str r5, [r2, #-0x50]!
005e31ac add r1, pc, r1
005e31b0 mov r0, r6
005e31b4 bl #0x5e217c
005e31b8 ldr r3, [pc, #-0xb34]
005e31bc add r1, sp, #0x420
005e31c0 mov r5, #0x33
005e31c4 add r3, pc, r3
005e31c8 add r1, r1, #0xc
005e31cc mov r6, r0
005e31d0 str r3, [sp, #0x42c]
005e31d4 str r5, [sp, #0x430]
005e31d8 bl #0x5e2134
005e31dc ldr r1, [pc, #-0xb54]
005e31e0 add r2, sp, #0x730
005e31e4 str r5, [r2, #-0x54]!
005e31e8 add r1, pc, r1
005e31ec mov r0, r6
005e31f0 bl #0x5e21d4
005e31f4 ldr r3, [pc, #-0xb68]
005e31f8 add r1, sp, #0x420
005e31fc add r1, r1, #4
005e3200 add r3, pc, r3
005e3204 mov r6, r0
005e3208 str r5, [sp, #0x428]
005e320c str r3, [sp, #0x424]
005e3210 bl #0x5e2134
005e3214 ldr r3, [pc, #-0xb84]
005e3218 add r1, sp, #0x410
005e321c mov r0, r6
005e3220 add r3, pc, r3
005e3224 add r1, r1, #0xc
005e3228 str r3, [sp, #0x41c]
005e322c str r5, [sp, #0x420]
005e3230 bl #0x5e2134
005e3234 ldr r3, [pc, #-0xba0]
005e3238 add r1, sp, #0x410
005e323c mov r5, #0x2c
005e3240 mov r0, r6
005e3244 add r3, pc, r3
005e3248 add r1, r1, #4
005e324c str r3, [sp, #0x414]
005e3250 str r5, [sp, #0x418]
005e3254 bl #0x5e2134
005e3258 ldr r3, [pc, #-0xbc0]
005e325c add r1, sp, #0x400
005e3260 mov r0, r6
005e3264 add r3, pc, r3
005e3268 add r1, r1, #0xc
005e326c str r3, [sp, #0x40c]
005e3270 str r5, [sp, #0x410]
005e3274 bl #0x5e2134
005e3278 ldr r3, [pc, #-0xbdc]
005e327c add r1, sp, #0x400
005e3280 mov r0, r6
005e3284 add r3, pc, r3
005e3288 add r1, r1, #4
005e328c str r3, [sp, #0x404]
005e3290 str r5, [sp, #0x408]
005e3294 bl #0x5e2134
005e3298 ldr r3, [pc, #-0xbf8]
005e329c mov r0, r6
005e32a0 add r1, sp, #0x3fc
005e32a4 add r3, pc, r3
005e32a8 str r3, [sp, #0x3fc]
005e32ac str r5, [sp, #0x400]
005e32b0 bl #0x5e2134
005e32b4 ldr r3, [pc, #-0xc10]
005e32b8 mov r0, r6
005e32bc add r1, sp, #0x3f4
005e32c0 add r3, pc, r3
005e32c4 str r3, [sp, #0x3f4]
005e32c8 str r5, [sp, #0x3f8]
005e32cc bl #0x5e2134
005e32d0 ldr r3, [pc, #-0xc28]
005e32d4 mov r0, r6
005e32d8 add r1, sp, #0x3ec
005e32dc add r3, pc, r3
005e32e0 str r3, [sp, #0x3ec]
005e32e4 str r5, [sp, #0x3f0]
005e32e8 bl #0x5e2134
005e32ec ldr r3, [pc, #-0xc40]
005e32f0 mov r0, r6
005e32f4 add r1, sp, #0x3e4
005e32f8 add r3, pc, r3
005e32fc str r3, [sp, #0x3e4]
005e3300 str r5, [sp, #0x3e8]
005e3304 bl #0x5e2134
005e3308 ldr r3, [pc, #-0xc58]
005e330c mov r0, r6
005e3310 add r1, sp, #0x3dc
005e3314 add r3, pc, r3
005e3318 str r3, [sp, #0x3dc]
005e331c str r5, [sp, #0x3e0]
005e3320 bl #0x5e2134
005e3324 ldr r3, [pc, #-0xc70]
005e3328 mov r0, r6
005e332c add r1, sp, #0x3d4
005e3330 add r3, pc, r3
005e3334 str r3, [sp, #0x3d4]
005e3338 str r5, [sp, #0x3d8]
005e333c bl #0x5e2134
005e3340 ldr r3, [pc, #-0xc88]
005e3344 mov r0, r6
005e3348 add r1, sp, #0x3cc
005e334c add r3, pc, r3
005e3350 str r3, [sp, #0x3cc]
005e3354 str r5, [sp, #0x3d0]
005e3358 bl #0x5e2134
005e335c ldr r3, [pc, #-0xca0]
005e3360 mov r5, #0x37
005e3364 mov r0, r6
005e3368 add r3, pc, r3
005e336c add r1, sp, #0x3c4
005e3370 str r3, [sp, #0x3c4]
005e3374 str r5, [sp, #0x3c8]
005e3378 bl #0x5e2134
005e337c ldr r3, [pc, #-0xcbc]
005e3380 mov r0, r6
005e3384 add r1, sp, #0x3bc
005e3388 add r3, pc, r3
005e338c str r3, [sp, #0x3bc]
005e3390 str r5, [sp, #0x3c0]
005e3394 bl #0x5e2134
005e3398 ldr r3, [pc, #-0xcd4]
005e339c mov r0, r6
005e33a0 add r1, sp, #0x3b4
005e33a4 add r3, pc, r3
005e33a8 str r3, [sp, #0x3b4]
005e33ac str r5, [sp, #0x3b8]
005e33b0 bl #0x5e2134
005e33b4 ldr r3, [pc, #-0xcec]
005e33b8 mov r0, r6
005e33bc add r1, sp, #0x3ac
005e33c0 add r3, pc, r3
005e33c4 str r3, [sp, #0x3ac]
005e33c8 str r5, [sp, #0x3b0]
005e33cc bl #0x5e2134
005e33d0 ldr r3, [pc, #-0xd04]
005e33d4 mov r0, r6
005e33d8 add r1, sp, #0x3a4
005e33dc add r3, pc, r3
005e33e0 str r3, [sp, #0x3a4]
005e33e4 str r5, [sp, #0x3a8]
005e33e8 bl #0x5e2134
005e33ec ldr r3, [pc, #-0xd1c]
005e33f0 mov r5, #0x30
005e33f4 mov r0, r6
005e33f8 add r3, pc, r3
005e33fc add r1, sp, #0x39c
005e3400 str r3, [sp, #0x39c]
005e3404 str r5, [sp, #0x3a0]
005e3408 bl #0x5e2134
005e340c ldr r3, [pc, #-0xd38]
005e3410 mov r0, r6
005e3414 add r1, sp, #0x394
005e3418 add r3, pc, r3
005e341c str r3, [sp, #0x394]
005e3420 str r5, [sp, #0x398]
005e3424 bl #0x5e2134
005e3428 ldr r3, [pc, #-0xd50]
005e342c mov r0, r6
005e3430 add r1, sp, #0x38c
005e3434 add r3, pc, r3
005e3438 str r3, [sp, #0x38c]
005e343c str r5, [sp, #0x390]
005e3440 bl #0x5e2134
005e3444 ldr r3, [pc, #-0xd68]
005e3448 mov r0, r6
005e344c add r1, sp, #0x384
005e3450 add r3, pc, r3
005e3454 str r3, [sp, #0x384]
005e3458 str r5, [sp, #0x388]
005e345c bl #0x5e2134
005e3460 ldr r3, [pc, #-0xd80]
005e3464 mov r0, r6
005e3468 add r1, sp, #0x37c
005e346c add r3, pc, r3
005e3470 str r3, [sp, #0x37c]
005e3474 str r5, [sp, #0x380]
005e3478 bl #0x5e2134
005e347c ldr r3, [pc, #-0xd98]
005e3480 mov r0, r6
005e3484 add r1, sp, #0x374
005e3488 add r3, pc, r3
005e348c str r3, [sp, #0x374]
005e3490 str r5, [sp, #0x378]
005e3494 bl #0x5e2134
005e3498 ldr r3, [pc, #-0xdb0]
005e349c mov r5, #0x23
005e34a0 mov r0, r6
005e34a4 add r3, pc, r3
005e34a8 add r1, sp, #0x36c
005e34ac str r3, [sp, #0x36c]
005e34b0 str r5, [sp, #0x370]
005e34b4 bl #0x5e2134
005e34b8 ldr r3, [pc, #-0xdcc]
005e34bc mov r0, r6
005e34c0 add r1, sp, #0x364
005e34c4 add r3, pc, r3
005e34c8 str r3, [sp, #0x364]
005e34cc str r5, [sp, #0x368]
005e34d0 bl #0x5e2134
005e34d4 ldr r3, [pc, #-0xde4]
005e34d8 mov r0, r6
005e34dc add r1, sp, #0x35c
005e34e0 add r3, pc, r3
005e34e4 str r3, [sp, #0x35c]
005e34e8 str r5, [sp, #0x360]
005e34ec bl #0x5e2134
005e34f0 ldr r3, [pc, #-0xdfc]
005e34f4 mov r0, r6
005e34f8 add r1, sp, #0x354
005e34fc add r3, pc, r3
005e3500 str r3, [sp, #0x354]
005e3504 str r5, [sp, #0x358]
005e3508 bl #0x5e2134
005e350c ldr r3, [pc, #-0xe14]
005e3510 mov r5, #0x29
005e3514 mov r0, r6
005e3518 add r3, pc, r3
005e351c add r1, sp, #0x34c
005e3520 str r3, [sp, #0x34c]
005e3524 str r5, [sp, #0x350]
005e3528 bl #0x5e2134
005e352c ldr r1, [pc, #-0xe30]
005e3530 add r2, sp, #0x730
005e3534 str r5, [r2, #-0x58]!
005e3538 add r1, pc, r1
005e353c mov r0, r6
005e3540 bl #0x5e217c
005e3544 ldr r3, [pc, #-0xe44]
005e3548 add r1, sp, #0x344
005e354c mov r6, r0
005e3550 add r3, pc, r3
005e3554 str r5, [sp, #0x348]
005e3558 str r3, [sp, #0x344]
005e355c bl #0x5e2134
005e3560 ldr r3, [pc, #-0xe5c]
005e3564 mov r0, r6
005e3568 add r1, sp, #0x33c
005e356c add r3, pc, r3
005e3570 str r3, [sp, #0x33c]
005e3574 str r5, [sp, #0x340]
005e3578 bl #0x5e2134
005e357c ldr r3, [pc, #-0xe74]
005e3580 mov r0, r6
005e3584 add r1, sp, #0x334
005e3588 add r3, pc, r3
005e358c str r3, [sp, #0x334]
005e3590 str r5, [sp, #0x338]
005e3594 bl #0x5e2134
005e3598 ldr r1, [pc, #-0xe8c]
005e359c mov r5, #0x34
005e35a0 add r2, sp, #0x730
005e35a4 str r5, [r2, #-0x5c]!
005e35a8 add r1, pc, r1
005e35ac mov r0, r6
005e35b0 bl #0x5e222c
005e35b4 ldr r3, [pc, #-0xea4]
005e35b8 add r6, sp, #0x730
005e35bc str r5, [r6, #-0x400]!
005e35c0 add r3, pc, r3
005e35c4 sub r1, r6, #4
005e35c8 mov r7, r0
005e35cc str r3, [sp, #0x32c]
005e35d0 bl #0x5e2134
005e35d4 ldr r1, [pc, #-0xec0]
005e35d8 add r2, sp, #0x730
005e35dc str r5, [r2, #-0x60]!
005e35e0 add r1, pc, r1
005e35e4 mov r0, r7
005e35e8 bl #0x5e222c
005e35ec ldr r3, [pc, #-0xed4]
005e35f0 sub r1, r6, #0xc
005e35f4 mov r7, r0
005e35f8 add r3, pc, r3
005e35fc str r3, [sp, #0x324]
005e3600 str r5, [sp, #0x328]
005e3604 bl #0x5e2134
005e3608 ldr r1, [pc, #-0xeec]
005e360c add r2, sp, #0x730
005e3610 str r5, [r2, #-0x64]!
005e3614 add r1, pc, r1
005e3618 mov r0, r7
005e361c bl #0x5e217c
005e3620 ldr r3, [pc, #-0xf00]
005e3624 add r7, sp, #0x730
005e3628 str r5, [r7, #-0x410]!
005e362c add r3, pc, r3
005e3630 sub r1, r7, #4
005e3634 mov r6, r0
005e3638 str r3, [sp, #0x31c]
005e363c bl #0x5e2134
005e3640 ldr r3, [pc, #-0xf1c]
005e3644 sub r1, r7, #0xc
005e3648 mov r0, r6
005e364c add r3, pc, r3
005e3650 str r3, [sp, #0x314]
005e3654 str r5, [sp, #0x318]
005e3658 bl #0x5e2134
005e365c ldr r1, [pc, #-0xf34]
005e3660 add r2, sp, #0x730
005e3664 str r5, [r2, #-0x68]!
005e3668 add r1, pc, r1
005e366c mov r0, r6
005e3670 bl #0x5e217c
005e3674 ldr r3, [pc, #-0xf48]
005e3678 add r7, sp, #0x730
005e367c str r5, [r7, #-0x420]!
005e3680 add r3, pc, r3
005e3684 sub r1, r7, #4
005e3688 mov r6, r0
005e368c str r3, [sp, #0x30c]
005e3690 bl #0x5e2134
005e3694 ldr r3, [pc, #-0xf64]
005e3698 sub r1, r7, #0xc
005e369c mov r0, r6
005e36a0 add r3, pc, r3
005e36a4 str r3, [sp, #0x304]
005e36a8 str r5, [sp, #0x308]
005e36ac bl #0x5e2134
005e36b0 ldr r3, [pc, #-0xf7c]
005e36b4 mov r7, #0x38
005e36b8 add r5, sp, #0x730
005e36bc str r7, [r5, #-0x430]!
005e36c0 add r3, pc, r3
005e36c4 mov r0, r6
005e36c8 sub r1, r5, #4
005e36cc str r3, [sp, #0x2fc]
005e36d0 bl #0x5e2134
005e36d4 ldr r1, [pc, #-0xf9c]
005e36d8 add r2, sp, #0x730
005e36dc str r7, [r2, #-0x6c]!
005e36e0 add r1, pc, r1
005e36e4 mov r0, r6
005e36e8 bl #0x5e21d4
005e36ec ldr r3, [pc, #-0xfb0]
005e36f0 sub r1, r5, #0xc
005e36f4 mov r6, r0
005e36f8 add r3, pc, r3
005e36fc str r3, [sp, #0x2f4]
005e3700 str r7, [sp, #0x2f8]
005e3704 bl #0x5e2134
005e3708 ldr r1, [pc, #-0xfc8]
005e370c mov r5, #0x21
005e3710 add r2, sp, #0x730
005e3714 str r5, [r2, #-0x70]!
005e3718 add r1, pc, r1
005e371c mov r0, r6
005e3720 bl #0x5e21d4
005e3724 ldr r1, [pc, #-0xfe0]
005e3728 add r2, sp, #0x730
005e372c str r5, [r2, #-0x74]!
005e3730 add r1, pc, r1
005e3734 bl #0x5e21d4
005e3738 ldr r3, [pc, #-0xff0]
005e373c add r6, sp, #0x730
005e3740 mov r2, #0x22
005e3744 str r2, [r6, #-0x440]!
005e3748 add r3, pc, r3
005e374c sub r1, r6, #4
005e3750 mov r5, r0
005e3754 str r3, [sp, #0x2ec]
005e3758 bl #0x5e2134
005e375c ldr r3, [pc, #0xe80]
005e3760 sub r1, r6, #0xc
005e3764 mov r0, r5
005e3768 add r3, pc, r3
005e376c mov r6, #0x3c
005e3770 str r3, [sp, #0x2e4]
005e3774 str r6, [sp, #0x2e8]
005e3778 bl #0x5e2134
005e377c ldr r1, [pc, #0xe64]
005e3780 add r2, sp, #0x730
005e3784 str r6, [r2, #-0x78]!
005e3788 add r1, pc, r1
005e378c mov r0, r5
005e3790 bl #0x5e21d4
005e3794 ldr r3, [pc, #0xe50]
005e3798 mov r5, #0x3a
005e379c add r6, sp, #0x730
005e37a0 str r5, [r6, #-0x450]!
005e37a4 add r3, pc, r3
005e37a8 sub r1, r6, #4
005e37ac mov r7, r0
005e37b0 str r3, [sp, #0x2dc]
005e37b4 bl #0x5e2134
005e37b8 ldr r3, [pc, #0xe30]
005e37bc sub r1, r6, #0xc
005e37c0 mov r0, r7
005e37c4 add r3, pc, r3
005e37c8 str r3, [sp, #0x2d4]
005e37cc str r5, [sp, #0x2d8]
005e37d0 bl #0x5e2134
005e37d4 ldr r3, [pc, #0xe18]
005e37d8 add r6, sp, #0x730
005e37dc str r5, [r6, #-0x460]!
005e37e0 add r3, pc, r3
005e37e4 mov r0, r7
005e37e8 sub r1, r6, #4
005e37ec str r3, [sp, #0x2cc]
005e37f0 bl #0x5e2134
005e37f4 ldr r1, [pc, #0xdfc]
005e37f8 add r2, sp, #0x730
005e37fc str r5, [r2, #-0x7c]!
005e3800 add r1, pc, r1
005e3804 mov r0, r7
005e3808 bl #0x5e21d4
005e380c ldr r3, [pc, #0xde8]
005e3810 sub r1, r6, #0xc
005e3814 mov r7, r0
005e3818 add r3, pc, r3
005e381c str r5, [sp, #0x2c8]
005e3820 str r3, [sp, #0x2c4]
005e3824 bl #0x5e2134
005e3828 ldr r3, [pc, #0xdd0]
005e382c add r6, sp, #0x730
005e3830 str r5, [r6, #-0x470]!
005e3834 add r3, pc, r3
005e3838 mov r0, r7
005e383c sub r1, r6, #4
005e3840 str r3, [sp, #0x2bc]
005e3844 bl #0x5e2134
005e3848 ldr r3, [pc, #0xdb4]
005e384c sub r1, r6, #0xc
005e3850 mov r0, r7
005e3854 add r3, pc, r3
005e3858 str r3, [sp, #0x2b4]
005e385c str r5, [sp, #0x2b8]
005e3860 bl #0x5e2134
005e3864 ldr r1, [pc, #0xd9c]
005e3868 add r2, sp, #0x730
005e386c str r5, [r2, #-0x80]!
005e3870 add r1, pc, r1
005e3874 mov r0, r7
005e3878 bl #0x5e217c
005e387c ldr r1, [pc, #0xd88]
005e3880 add r2, sp, #0x730
005e3884 mov r3, #0x39
005e3888 str r3, [r2, #-0x84]!
005e388c add r1, pc, r1
005e3890 bl #0x5e222c
005e3894 ldr r3, [pc, #0xd74]
005e3898 add r6, sp, #0x730
005e389c mov r7, #0x3e
005e38a0 str r7, [r6, #-0x480]!
005e38a4 add r3, pc, r3
005e38a8 sub r1, r6, #4
005e38ac mov r5, r0
005e38b0 str r3, [sp, #0x2ac]
005e38b4 bl #0x5e2134
005e38b8 ldr r1, [pc, #0xd54]
005e38bc add r2, sp, #0x730
005e38c0 str r7, [r2, #-0x88]!
005e38c4 add r1, pc, r1
005e38c8 mov r0, r5
005e38cc bl #0x5e217c
005e38d0 ldr r3, [pc, #0xd40]
005e38d4 sub r1, r6, #0xc
005e38d8 mov r5, r0
005e38dc add r3, pc, r3
005e38e0 str r3, [sp, #0x2a4]
005e38e4 mov r3, #0
005e38e8 str r3, [sp, #0x2a8]
005e38ec bl #0x5e2134
005e38f0 ldr r3, [pc, #0xd24]
005e38f4 add r6, sp, #0x730
005e38f8 mov r7, #0x3d
005e38fc str r7, [r6, #-0x490]!
005e3900 add r3, pc, r3
005e3904 mov r0, r5
005e3908 sub r1, r6, #4
005e390c str r3, [sp, #0x29c]
005e3910 bl #0x5e2134
005e3914 ldr r3, [pc, #0xd04]
005e3918 sub r1, r6, #0xc
005e391c mov r0, r5
005e3920 add r3, pc, r3
005e3924 str r3, [sp, #0x294]
005e3928 str r7, [sp, #0x298]
005e392c bl #0x5e2134
005e3930 ldr r3, [pc, #0xcec]
005e3934 mov r2, #0x3b
005e3938 add r6, sp, #0x730
005e393c str r2, [r6, #-0x4a0]!
005e3940 add r3, pc, r3
005e3944 mov r0, r5
005e3948 sub r1, r6, #4
005e394c str r3, [sp, #0x28c]
005e3950 bl #0x5e2134
005e3954 ldr r3, [pc, #0xccc]
005e3958 mov r8, #0x18
005e395c sub r1, r6, #0xc
005e3960 add r3, pc, r3
005e3964 mov r0, r5
005e3968 str r3, [sp, #0x284]
005e396c str r8, [sp, #0x288]
005e3970 bl #0x5e2134
005e3974 ldr r3, [pc, #0xcb0]
005e3978 add r6, sp, #0x730
005e397c str r8, [r6, #-0x4b0]!
005e3980 add r3, pc, r3
005e3984 mov r0, r5
005e3988 sub r1, r6, #4
005e398c str r3, [sp, #0x27c]
005e3990 bl #0x5e2134
005e3994 ldr r3, [pc, #0xc94]
005e3998 sub r1, r6, #0xc
005e399c mov r0, r5
005e39a0 add r3, pc, r3
005e39a4 str r3, [sp, #0x274]
005e39a8 str r8, [sp, #0x278]
005e39ac bl #0x5e2134
005e39b0 ldr r3, [pc, #0xc7c]
005e39b4 add r6, sp, #0x730
005e39b8 ldr r7, [pc, #0xc78]
005e39bc str r8, [r6, #-0x4c0]!
005e39c0 add r3, pc, r3
005e39c4 mov r0, r5
005e39c8 sub r1, r6, #4
005e39cc str r3, [sp, #0x26c]
005e39d0 add r7, pc, r7
005e39d4 bl #0x5e2134
005e39d8 sub r1, r6, #0xc
005e39dc mov r0, r5
005e39e0 mov r6, #0x14
005e39e4 str r7, [sp, #0x264]
005e39e8 str r6, [sp, #0x268]
005e39ec bl #0x5e2134
005e39f0 ldr r3, [pc, #0xc44]
005e39f4 add r8, sp, #0x730
005e39f8 str r6, [r8, #-0x4d0]!
005e39fc add r3, pc, r3
005e3a00 mov r0, r5
005e3a04 sub r1, r8, #4
005e3a08 str r3, [sp, #0x25c]
005e3a0c bl #0x5e2134
005e3a10 ldr r3, [pc, #0xc28]
005e3a14 sub r1, r8, #0xc
005e3a18 mov r0, r5
005e3a1c add r3, pc, r3
005e3a20 str r3, [sp, #0x254]
005e3a24 str r6, [sp, #0x258]
005e3a28 bl #0x5e2134
005e3a2c ldr r3, [pc, #0xc10]
005e3a30 add r8, sp, #0x730
005e3a34 str r6, [r8, #-0x4e0]!
005e3a38 add r3, pc, r3
005e3a3c mov r0, r5
005e3a40 sub r1, r8, #4
005e3a44 str r3, [sp, #0x24c]
005e3a48 bl #0x5e2134
005e3a4c sub r1, r8, #0xc
005e3a50 mov r0, r5
005e3a54 str r7, [sp, #0x244]
005e3a58 str r6, [sp, #0x248]
005e3a5c bl #0x5e2134
005e3a60 ldr r3, [pc, #0xbe0]
005e3a64 mov r6, #0x1a
005e3a68 add r7, sp, #0x730
005e3a6c str r6, [r7, #-0x4f0]!
005e3a70 add r3, pc, r3
005e3a74 mov r0, r5
005e3a78 sub r1, r7, #4
005e3a7c str r3, [sp, #0x23c]
005e3a80 bl #0x5e2134
005e3a84 ldr r3, [pc, #0xbc0]
005e3a88 sub r1, r7, #0xc
005e3a8c mov r0, r5
005e3a90 add r3, pc, r3
005e3a94 str r3, [sp, #0x234]
005e3a98 str r6, [sp, #0x238]
005e3a9c bl #0x5e2134
005e3aa0 ldr r3, [pc, #0xba8]
005e3aa4 add r7, sp, #0x730
005e3aa8 str r6, [r7, #-0x500]!
005e3aac add r3, pc, r3
005e3ab0 mov r0, r5
005e3ab4 sub r1, r7, #4
005e3ab8 str r3, [sp, #0x22c]
005e3abc bl #0x5e2134
005e3ac0 ldr r3, [pc, #0xb8c]
005e3ac4 sub r1, r7, #0xc
005e3ac8 mov r0, r5
005e3acc add r3, pc, r3
005e3ad0 str r3, [sp, #0x224]
005e3ad4 str r6, [sp, #0x228]
005e3ad8 bl #0x5e2134
005e3adc ldr r3, [pc, #0xb74]
005e3ae0 mov r6, #0x16
005e3ae4 add r7, sp, #0x730
005e3ae8 str r6, [r7, #-0x510]!
005e3aec add r3, pc, r3
005e3af0 mov r0, r5
005e3af4 sub r1, r7, #4
005e3af8 str r3, [sp, #0x21c]
005e3afc bl #0x5e2134
005e3b00 ldr r3, [pc, #0xb54]
005e3b04 sub r1, r7, #0xc
005e3b08 mov r0, r5
005e3b0c add r3, pc, r3
005e3b10 str r3, [sp, #0x214]
005e3b14 str r6, [sp, #0x218]
005e3b18 bl #0x5e2134
005e3b1c ldr r3, [pc, #0xb3c]
005e3b20 add r7, sp, #0x730
005e3b24 str r6, [r7, #-0x520]!
005e3b28 add r3, pc, r3
005e3b2c mov r0, r5
005e3b30 sub r1, r7, #4
005e3b34 str r3, [sp, #0x20c]
005e3b38 bl #0x5e2134
005e3b3c ldr r3, [pc, #0xb20]
005e3b40 sub r1, r7, #0xc
005e3b44 mov r0, r5
005e3b48 add r3, pc, r3
005e3b4c str r3, [sp, #0x204]
005e3b50 str r6, [sp, #0x208]
005e3b54 bl #0x5e2134
005e3b58 ldr r1, [pc, #0xb08]
005e3b5c add r2, sp, #0x730
005e3b60 mov r6, #0x17
005e3b64 str r6, [r2, #-0x8c]!
005e3b68 add r1, pc, r1
005e3b6c mov r0, r5
005e3b70 bl #0x5e217c
005e3b74 ldr r3, [pc, #0xaf0]
005e3b78 add r7, sp, #0x730
005e3b7c str r6, [r7, #-0x530]!
005e3b80 add r3, pc, r3
005e3b84 sub r1, r7, #4
005e3b88 mov r5, r0
005e3b8c str r3, [sp, #0x1fc]
005e3b90 bl #0x5e2134
005e3b94 ldr r3, [pc, #0xad4]
005e3b98 sub r1, r7, #0xc
005e3b9c mov r0, r5
005e3ba0 add r3, pc, r3
005e3ba4 str r3, [sp, #0x1f4]
005e3ba8 str r6, [sp, #0x1f8]
005e3bac bl #0x5e2134
005e3bb0 ldr r3, [pc, #0xabc]
005e3bb4 add r7, sp, #0x730
005e3bb8 str r6, [r7, #-0x540]!
005e3bbc add r3, pc, r3
005e3bc0 mov r0, r5
005e3bc4 sub r1, r7, #4
005e3bc8 str r3, [sp, #0x1ec]
005e3bcc bl #0x5e2134
005e3bd0 ldr r3, [pc, #0xaa0]
005e3bd4 sub r1, r7, #0xc
005e3bd8 mov r0, r5
005e3bdc add r3, pc, r3
005e3be0 str r3, [sp, #0x1e4]
005e3be4 str r6, [sp, #0x1e8]
005e3be8 bl #0x5e2134
005e3bec ldr r3, [pc, #0xa88]
005e3bf0 mov r7, #0x15
005e3bf4 add r6, sp, #0x730
005e3bf8 str r7, [r6, #-0x550]!
005e3bfc add r3, pc, r3
005e3c00 mov r0, r5
005e3c04 sub r1, r6, #4
005e3c08 str r3, [sp, #0x1dc]
005e3c0c bl #0x5e2134
005e3c10 ldr r3, [pc, #0xa68]
005e3c14 sub r1, r6, #0xc
005e3c18 mov r0, r5
005e3c1c add r3, pc, r3
005e3c20 str r3, [sp, #0x1d4]
005e3c24 str r7, [sp, #0x1d8]
005e3c28 bl #0x5e2134
005e3c2c ldr r3, [pc, #0xa50]
005e3c30 add r6, sp, #0x730
005e3c34 ldr r8, [pc, #0xa4c]
005e3c38 str r7, [r6, #-0x560]!
005e3c3c add r3, pc, r3
005e3c40 mov r0, r5
005e3c44 sub r1, r6, #4
005e3c48 str r3, [sp, #0x1cc]
005e3c4c add r8, pc, r8
005e3c50 bl #0x5e2134
005e3c54 sub r1, r6, #0xc
005e3c58 mov r0, r5
005e3c5c mov r6, #0x13
005e3c60 str r8, [sp, #0x1c4]
005e3c64 str r6, [sp, #0x1c8]
005e3c68 bl #0x5e2134
005e3c6c ldr r3, [pc, #0xa18]
005e3c70 add r7, sp, #0x730
005e3c74 str r6, [r7, #-0x570]!
005e3c78 add r3, pc, r3
005e3c7c mov r0, r5
005e3c80 sub r1, r7, #4
005e3c84 str r3, [sp, #0x1bc]
005e3c88 bl #0x5e2134
005e3c8c ldr r3, [pc, #0x9fc]
005e3c90 sub r1, r7, #0xc
005e3c94 mov r0, r5
005e3c98 add r3, pc, r3
005e3c9c str r3, [sp, #0x1b4]
005e3ca0 str r6, [sp, #0x1b8]
005e3ca4 bl #0x5e2134
005e3ca8 ldr r1, [pc, #0x9e4]
005e3cac add r2, sp, #0x730
005e3cb0 str r6, [r2, #-0x90]!
005e3cb4 add r1, pc, r1
005e3cb8 mov r0, r5
005e3cbc bl #0x5e222c
005e3cc0 add r7, sp, #0x730
005e3cc4 str r6, [r7, #-0x580]!
005e3cc8 sub r1, r7, #4
005e3ccc mov r5, r0
005e3cd0 str r8, [sp, #0x1ac]
005e3cd4 bl #0x5e2134
005e3cd8 ldr r3, [pc, #0x9b8]
005e3cdc sub r1, r7, #0xc
005e3ce0 mov r0, r5
005e3ce4 mov r7, #0x19
005e3ce8 add r3, pc, r3
005e3cec str r3, [sp, #0x1a4]
005e3cf0 str r7, [sp, #0x1a8]
005e3cf4 bl #0x5e2134
005e3cf8 ldr r3, [pc, #0x99c]
005e3cfc add r6, sp, #0x730
005e3d00 str r7, [r6, #-0x590]!
005e3d04 add r3, pc, r3
005e3d08 mov r0, r5
005e3d0c sub r1, r6, #4
005e3d10 str r3, [sp, #0x19c]
005e3d14 bl #0x5e2134
005e3d18 ldr r3, [pc, #0x980]
005e3d1c sub r1, r6, #0xc
005e3d20 mov r0, r5
005e3d24 add r3, pc, r3
005e3d28 str r3, [sp, #0x194]
005e3d2c str r7, [sp, #0x198]
005e3d30 bl #0x5e2134
005e3d34 ldr r3, [pc, #0x968]
005e3d38 add r6, sp, #0x730
005e3d3c str r7, [r6, #-0x5a0]!
005e3d40 add r3, pc, r3
005e3d44 mov r0, r5
005e3d48 sub r1, r6, #4
005e3d4c str r3, [sp, #0x18c]
005e3d50 bl #0x5e2134
005e3d54 ldr r3, [pc, #0x94c]
005e3d58 mov r7, #0x1f
005e3d5c sub r1, r6, #0xc
005e3d60 add r3, pc, r3
005e3d64 mov r0, r5
005e3d68 str r3, [sp, #0x184]
005e3d6c str r7, [sp, #0x188]
005e3d70 bl #0x5e2134
005e3d74 ldr r3, [pc, #0x930]
005e3d78 add r6, sp, #0x730
005e3d7c str r7, [r6, #-0x5b0]!
005e3d80 add r3, pc, r3
005e3d84 mov r0, r5
005e3d88 sub r1, r6, #4
005e3d8c str r3, [sp, #0x17c]
005e3d90 bl #0x5e2134
005e3d94 ldr r3, [pc, #0x914]
005e3d98 sub r1, r6, #0xc
005e3d9c mov r0, r5
005e3da0 add r3, pc, r3
005e3da4 str r3, [sp, #0x174]
005e3da8 str r7, [sp, #0x178]
005e3dac bl #0x5e2134
005e3db0 ldr r3, [pc, #0x8fc]
005e3db4 mov r6, #0x1e
005e3db8 add r7, sp, #0x730
005e3dbc str r6, [r7, #-0x5c0]!
005e3dc0 add r3, pc, r3
005e3dc4 mov r0, r5
005e3dc8 sub r1, r7, #4
005e3dcc str r3, [sp, #0x16c]
005e3dd0 bl #0x5e2134
005e3dd4 ldr r1, [pc, #0x8dc]
005e3dd8 add r2, sp, #0x730
005e3ddc str r6, [r2, #-0x94]!
005e3de0 add r1, pc, r1
005e3de4 mov r0, r5
005e3de8 bl #0x5e21d4
005e3dec ldr r1, [pc, #0x8c8]
005e3df0 add r2, sp, #0x730
005e3df4 str r6, [r2, #-0x98]!
005e3df8 add r1, pc, r1
005e3dfc bl #0x5e222c
005e3e00 ldr r1, [pc, #0x8b8]
005e3e04 add r2, sp, #0x730
005e3e08 str r6, [r2, #-0x9c]!
005e3e0c add r1, pc, r1
005e3e10 bl #0x5e21d4
005e3e14 ldr r1, [pc, #0x8a8]
005e3e18 add r2, sp, #0x730
005e3e1c str r6, [r2, #-0xa0]!
005e3e20 add r1, pc, r1
005e3e24 bl #0x5e222c
005e3e28 ldr r1, [pc, #0x898]
005e3e2c add r2, sp, #0x730
005e3e30 mov r6, #0x1d
005e3e34 str r6, [r2, #-0xa4]!
005e3e38 add r1, pc, r1
005e3e3c bl #0x5e217c
005e3e40 ldr r3, [pc, #0x884]
005e3e44 sub r1, r7, #0xc
005e3e48 mov r5, r0
005e3e4c add r3, pc, r3
005e3e50 str r6, [sp, #0x168]
005e3e54 str r3, [sp, #0x164]
005e3e58 bl #0x5e2134
005e3e5c ldr r3, [pc, #0x86c]
005e3e60 add r7, sp, #0x730
005e3e64 str r6, [r7, #-0x5d0]!
005e3e68 add r3, pc, r3
005e3e6c mov r0, r5
005e3e70 sub r1, r7, #4
005e3e74 str r3, [sp, #0x15c]
005e3e78 bl #0x5e2134
005e3e7c ldr r3, [pc, #0x850]
005e3e80 sub r1, r7, #0xc
005e3e84 mov r0, r5
005e3e88 mov r7, #3
005e3e8c add r3, pc, r3
005e3e90 str r3, [sp, #0x154]
005e3e94 str r7, [sp, #0x158]
005e3e98 bl #0x5e2134
005e3e9c ldr r3, [pc, #0x834]
005e3ea0 add r6, sp, #0x730
005e3ea4 str r7, [r6, #-0x5e0]!
005e3ea8 add r3, pc, r3
005e3eac mov r0, r5
005e3eb0 sub r1, r6, #4
005e3eb4 str r3, [sp, #0x14c]
005e3eb8 bl #0x5e2134
005e3ebc ldr r1, [pc, #0x818]
005e3ec0 add r2, sp, #0x730
005e3ec4 str r7, [r2, #-0xa8]!
005e3ec8 add r1, pc, r1
005e3ecc mov r0, r5
005e3ed0 bl #0x5e222c
005e3ed4 ldr r3, [pc, #0x804]
005e3ed8 sub r1, r6, #0xc
005e3edc mov r5, r0
005e3ee0 add r3, pc, r3
005e3ee4 str r3, [sp, #0x144]
005e3ee8 mov r3, #4
005e3eec str r3, [sp, #0x148]
005e3ef0 bl #0x5e2134
005e3ef4 ldr r3, [pc, #0x7e8]
005e3ef8 add r6, sp, #0x730
005e3efc mov r7, #2
005e3f00 str r7, [r6, #-0x5f0]!
005e3f04 add r3, pc, r3
005e3f08 mov r0, r5
005e3f0c sub r1, r6, #4
005e3f10 str r3, [sp, #0x13c]
005e3f14 bl #0x5e2134
005e3f18 ldr r3, [pc, #0x7c8]
005e3f1c sub r1, r6, #0xc
005e3f20 mov r0, r5
005e3f24 add r3, pc, r3
005e3f28 str r3, [sp, #0x134]
005e3f2c str r7, [sp, #0x138]
005e3f30 bl #0x5e2134
005e3f34 ldr r3, [pc, #0x7b0]
005e3f38 add r6, sp, #0x730
005e3f3c mov r7, #8
005e3f40 str r7, [r6, #-0x600]!
005e3f44 add r3, pc, r3
005e3f48 mov r0, r5
005e3f4c sub r1, r6, #4
005e3f50 str r3, [sp, #0x12c]
005e3f54 bl #0x5e2134
005e3f58 ldr r1, [pc, #0x790]
005e3f5c add r2, sp, #0x730
005e3f60 str r7, [r2, #-0xac]!
005e3f64 add r1, pc, r1
005e3f68 mov r0, r5
005e3f6c bl #0x5e222c
005e3f70 ldr r3, [pc, #0x77c]
005e3f74 sub r1, r6, #0xc
005e3f78 mov r6, #7
005e3f7c add r3, pc, r3
005e3f80 mov r5, r0
005e3f84 str r3, [sp, #0x124]
005e3f88 str r6, [sp, #0x128]
005e3f8c bl #0x5e2134
005e3f90 ldr r1, [pc, #0x760]
005e3f94 add r2, sp, #0x730
005e3f98 str r6, [r2, #-0xb0]!
005e3f9c add r1, pc, r1
005e3fa0 mov r0, r5
005e3fa4 bl #0x5e222c
005e3fa8 ldr r3, [pc, #0x74c]
005e3fac add r5, sp, #0x730
005e3fb0 mov r7, #6
005e3fb4 str r7, [r5, #-0x610]!
005e3fb8 add r3, pc, r3
005e3fbc sub r1, r5, #4
005e3fc0 mov r6, r0
005e3fc4 str r3, [sp, #0x11c]
005e3fc8 bl #0x5e2134
005e3fcc ldr r1, [pc, #0x72c]
005e3fd0 add r2, sp, #0x730
005e3fd4 str r7, [r2, #-0xb4]!
005e3fd8 add r1, pc, r1
005e3fdc mov r0, r6
005e3fe0 bl #0x5e21d4
005e3fe4 ldr r3, [pc, #0x718]
005e3fe8 sub r1, r5, #0xc
005e3fec mov r5, #5
005e3ff0 add r3, pc, r3
005e3ff4 mov r6, r0
005e3ff8 str r3, [sp, #0x114]
005e3ffc str r5, [sp, #0x118]
005e4000 bl #0x5e2134
005e4004 ldr r1, [pc, #0x6fc]
005e4008 add r2, sp, #0x730
005e400c str r5, [r2, #-0xb8]!
005e4010 add r1, pc, r1
005e4014 mov r0, r6
005e4018 bl #0x5e21d4
005e401c ldr r3, [pc, #0x6e8]
005e4020 mov r7, #9
005e4024 add r6, sp, #0x730
005e4028 str r7, [r6, #-0x620]!
005e402c add r3, pc, r3
005e4030 sub r1, r6, #4
005e4034 mov r5, r0
005e4038 str r3, [sp, #0x10c]
005e403c bl #0x5e2134
005e4040 ldr r3, [pc, #0x6c8]
005e4044 sub r1, r6, #0xc
005e4048 mov r0, r5
005e404c add r3, pc, r3
005e4050 str r3, [sp, #0x104]
005e4054 str r7, [sp, #0x108]
005e4058 bl #0x5e2134
005e405c ldr r1, [pc, #0x6b0]
005e4060 add r2, sp, #0x730
005e4064 str r7, [r2, #-0xbc]!
005e4068 add r1, pc, r1
005e406c mov r0, r5
005e4070 bl #0x5e222c
005e4074 ldr r1, [pc, #0x69c]
005e4078 add r2, sp, #0x730
005e407c mov r6, #0xf
005e4080 str r6, [r2, #-0xc0]!
005e4084 add r1, pc, r1
005e4088 bl #0x5e217c
005e408c ldr r3, [pc, #0x688]
005e4090 add r7, sp, #0x730
005e4094 str r6, [r7, #-0x630]!
005e4098 add r3, pc, r3
005e409c sub r1, r7, #4
005e40a0 mov r5, r0
005e40a4 str r3, [sp, #0xfc]
005e40a8 bl #0x5e2134
005e40ac ldr r3, [pc, #0x66c]
005e40b0 sub r1, r7, #0xc
005e40b4 mov r0, r5
005e40b8 mov r7, #0xd
005e40bc add r3, pc, r3
005e40c0 str r3, [sp, #0xf4]
005e40c4 str r7, [sp, #0xf8]
005e40c8 bl #0x5e2134
005e40cc ldr r3, [pc, #0x650]
005e40d0 add r6, sp, #0x730
005e40d4 str r7, [r6, #-0x640]!
005e40d8 add r3, pc, r3
005e40dc mov r0, r5
005e40e0 sub r1, r6, #4
005e40e4 str r3, [sp, #0xec]
005e40e8 bl #0x5e2134
005e40ec ldr r3, [pc, #0x634]
005e40f0 sub r1, r6, #0xc
005e40f4 mov r7, #0xe
005e40f8 add r3, pc, r3
005e40fc mov r0, r5
005e4100 str r3, [sp, #0xe4]
005e4104 str r7, [sp, #0xe8]
005e4108 bl #0x5e2134
005e410c ldr r3, [pc, #0x618]
005e4110 add r6, sp, #0x730
005e4114 str r7, [r6, #-0x650]!
005e4118 add r3, pc, r3
005e411c mov r0, r5
005e4120 sub r1, r6, #4
005e4124 str r3, [sp, #0xdc]
005e4128 bl #0x5e2134
005e412c ldr r1, [pc, #0x5fc]
005e4130 mov r7, #0xb
005e4134 add r2, sp, #0x730
005e4138 str r7, [r2, #-0xc4]!
005e413c add r1, pc, r1
005e4140 mov r0, r5
005e4144 bl #0x5e222c
005e4148 ldr r1, [pc, #0x5e4]
005e414c add r2, sp, #0x730
005e4150 str r7, [r2, #-0xc8]!
005e4154 add r1, pc, r1
005e4158 bl #0x5e21d4
005e415c ldr r1, [pc, #0x5d4]
005e4160 add r2, sp, #0x730
005e4164 mov r5, #0xc
005e4168 str r5, [r2, #-0xcc]!
005e416c add r1, pc, r1
005e4170 bl #0x5e21d4
005e4174 ldr r3, [pc, #0x5c0]
005e4178 sub r1, r6, #0xc
005e417c mov r7, r0
005e4180 add r3, pc, r3
005e4184 str r5, [sp, #0xd8]
005e4188 str r3, [sp, #0xd4]
005e418c bl #0x5e2134
005e4190 ldr r3, [pc, #0x5a8]
005e4194 add r5, sp, #0x730
005e4198 mov r6, #0x20
005e419c str r6, [r5, #-0x660]!
005e41a0 add r3, pc, r3
005e41a4 mov r0, r7
005e41a8 sub r1, r5, #4
005e41ac str r3, [sp, #0xcc]
005e41b0 bl #0x5e2134
005e41b4 ldr r1, [pc, #0x588]
005e41b8 add r2, sp, #0x730
005e41bc str r6, [r2, #-0xd0]!
005e41c0 add r1, pc, r1
005e41c4 mov r0, r7
005e41c8 bl #0x5e217c
005e41cc ldr r3, [pc, #0x574]
005e41d0 sub r1, r5, #0xc
005e41d4 mov r5, #0x1c
005e41d8 add r3, pc, r3
005e41dc mov r7, r0
005e41e0 str r3, [sp, #0xc4]
005e41e4 str r5, [sp, #0xc8]
005e41e8 bl #0x5e2134
005e41ec ldr r3, [pc, #0x558]
005e41f0 add r6, sp, #0x730
005e41f4 str r5, [r6, #-0x670]!
005e41f8 add r3, pc, r3
005e41fc mov r0, r7
005e4200 sub r1, r6, #4
005e4204 str r3, [sp, #0xbc]
005e4208 bl #0x5e2134
005e420c ldr r1, [pc, #0x53c]
005e4210 add r2, sp, #0x730
005e4214 str r5, [r2, #-0xd4]!
005e4218 add r1, pc, r1
005e421c mov r0, r7
005e4220 bl #0x5e21d4
005e4224 ldr r1, [pc, #0x528]
005e4228 add r2, sp, #0x730
005e422c str r5, [r2, #-0xd8]!
005e4230 add r1, pc, r1
005e4234 bl #0x5e222c
005e4238 ldr r3, [pc, #0x518]
005e423c sub r1, r6, #0xc
005e4240 mov r7, r0
005e4244 add r3, pc, r3
005e4248 str r5, [sp, #0xb8]
005e424c str r3, [sp, #0xb4]
005e4250 bl #0x5e2134
005e4254 ldr r3, [pc, #0x500]
005e4258 add r6, sp, #0x730
005e425c str r5, [r6, #-0x680]!
005e4260 add r3, pc, r3
005e4264 mov r0, r7
005e4268 sub r1, r6, #4
005e426c str r3, [sp, #0xac]
005e4270 bl #0x5e2134
005e4274 ldr r1, [pc, #0x4e4]
005e4278 add r2, sp, #0x730
005e427c str r5, [r2, #-0xdc]!
005e4280 add r1, pc, r1
005e4284 mov r0, r7
005e4288 bl #0x5e21d4
005e428c ldr r1, [pc, #0x4d0]
005e4290 add r2, sp, #0x730
005e4294 str r5, [r2, #-0xe0]!
005e4298 add r1, pc, r1
005e429c bl #0x5e222c
005e42a0 ldr r3, [pc, #0x4c0]
005e42a4 sub r1, r6, #0xc
005e42a8 mov r6, #0xa
005e42ac add r3, pc, r3
005e42b0 mov r7, r0
005e42b4 str r3, [sp, #0xa4]
005e42b8 str r6, [sp, #0xa8]
005e42bc bl #0x5e2134
005e42c0 ldr r3, [pc, #0x4a4]
005e42c4 add r5, sp, #0x730
005e42c8 str r6, [r5, #-0x690]!
005e42cc add r3, pc, r3
005e42d0 mov r0, r7
005e42d4 sub r1, r5, #4
005e42d8 str r3, [sp, #0x9c]
005e42dc bl #0x5e2134
005e42e0 ldr r1, [pc, #0x488]
005e42e4 add r2, sp, #0x730
005e42e8 mov r3, #0x10
005e42ec str r3, [r2, #-0xe4]!
005e42f0 add r1, pc, r1
005e42f4 mov r0, r7
005e42f8 bl #0x5e222c
005e42fc ldr r3, [pc, #0x470]
005e4300 sub r1, r5, #0xc
005e4304 mov sl, r0
005e4308 add r3, pc, r3
005e430c str r3, [sp, #0x94]
005e4310 mov r3, #0x11
005e4314 str r3, [sp, #0x98]
005e4318 bl #0x5e2134
005e431c ldr r3, [pc, #0x454]
005e4320 ldr sb, [pc, #0x454]
005e4324 ldr r7, [pc, #0x454]
005e4328 add r3, pc, r3
005e432c add sb, pc, sb
005e4330 mov r6, #0
005e4334 sub fp, fp, #4
005e4338 add r2, r3, #7
005e433c str r3, [sp, #0x10]
005e4340 str fp, [sp, #0x18]
005e4344 mov r8, r6
005e4348 str r2, [sp, #0x14]
005e434c str sb, [sp, #8]
005e4350 uxth r3, r8
005e4354 cmp r3, #0xff
005e4358 bne #0x5e457c
005e435c ldr r2, [sp, #0x14]
005e4360 ldr fp, [sp, #0x10]
005e4364 mov r0, #8
005e4368 mov r3, #7
005e436c str r2, [sp, #4]
005e4370 str r3, [sp]
005e4374 bl #0x5345f4
005e4378 ldr r2, [sp, #4]
005e437c mov r5, r0
005e4380 ldr r3, [sp]
005e4384 cmp r2, fp
005e4388 beq #0x5e43bc
005e438c rsb r2, fp, r2
005e4390 mov r1, #0
005e4394 ldrsb r0, [fp, r1]
005e4398 cmp r0, #0xff
005e439c ldrls ip, [r4, r7]
005e43a0 ldrls ip, [ip]
005e43a4 addls r0, ip, r0, lsl #1
005e43a8 ldrshls r0, [r0, #2]
005e43ac strb r0, [r5, r1]
005e43b0 add r1, r1, #1
005e43b4 cmp r1, r2
005e43b8 bne #0x5e4394
005e43bc mov r2, #0
005e43c0 strb r2, [r5, r3]
005e43c4 mov r0, r5
005e43c8 mov r1, #1
005e43cc bl #0x6a5074
005e43d0 subs fp, r0, #0
005e43d4 ldrne r3, [fp]
005e43d8 addne r3, r3, #2
005e43dc strne r3, [fp]
005e43e0 ldr r0, [sb, r6]
005e43e4 str fp, [sb, r6]
005e43e8 cmp r0, #0
005e43ec beq #0x5e4404
005e43f0 ldr r3, [r0]
005e43f4 sub r3, r3, #1
005e43f8 cmp r3, #0
005e43fc str r3, [r0]
005e4400 beq #0x5e45a0
005e4404 cmp fp, #0
005e4408 beq #0x5e4420
005e440c ldr r3, [fp]
005e4410 sub r3, r3, #1
005e4414 cmp r3, #0
005e4418 str r3, [fp]
005e441c beq #0x5e45a8
005e4420 ldr r2, [sp, #8]
005e4424 ldr r3, [r2, r6]
005e4428 str r8, [sp, #0x90]
005e442c cmp r3, #0
005e4430 addne r3, r3, #4
005e4434 cmp r5, #0
005e4438 str r3, [sp, #0x8c]
005e443c beq #0x5e4448
005e4440 mov r0, r5
005e4444 bl #0x534688
005e4448 ldr r2, [sl, #0x18]
005e444c ldr r3, [sl, #0x10]
005e4450 sub r2, r2, #8
005e4454 cmp r3, r2
005e4458 beq #0x5e45b4
005e445c ldr r2, [sp, #0x8c]
005e4460 str r2, [r3]
005e4464 ldr r2, [sp, #0x90]
005e4468 str r2, [r3, #4]
005e446c ldr r3, [sl, #0x10]
005e4470 add r3, r3, #8
005e4474 str r3, [sl, #0x10]
005e4478 add r6, r6, #4
005e447c cmp r6, #0xfc
005e4480 add r8, r8, #1
005e4484 bne #0x5e4350
005e4488 ldr r3, [pc, #0x2f4]
005e448c ldr r8, [pc, #0x2f4]
005e4490 mov r1, #0
005e4494 add r3, pc, r3
005e4498 mov r2, r3
005e449c ldr sb, [sl, #0x10]
005e44a0 ldr fp, [sl, #0xc]
005e44a4 ldr r6, [sl, #8]
005e44a8 ldr r5, [sl]
005e44ac add r8, pc, r8
005e44b0 strb r1, [r2, #0x114]!
005e44b4 add r7, sp, #0x80
005e44b8 str r1, [r3, #0x124]
005e44bc str r2, [r3, #0x120]
005e44c0 str r1, [r3, #0x118]
005e44c4 str r2, [r3, #0x11c]
005e44c8 add r8, r8, #0x114
005e44cc sub r7, r7, #4
005e44d0 add sl, sp, #0x20
005e44d4 b #0x5e44fc
005e44d8 ldr r3, [r5]
005e44dc str r3, [sp, #0x7c]
005e44e0 ldr r3, [r5, #4]
005e44e4 add r5, r5, #8
005e44e8 str r3, [sp, #0x80]
005e44ec bl #0x5e1b64
005e44f0 cmp r6, r5
005e44f4 ldreq r5, [fp, #4]!
005e44f8 addeq r6, r5, #0x80
005e44fc cmp r5, sb
005e4500 mov r0, sl
005e4504 mov r1, r8
005e4508 mov r2, r7
005e450c bne #0x5e44d8
005e4510 ldr r5, [pc, #0x274]
005e4514 add r5, pc, r5
005e4518 add r0, r5, #0x110
005e451c bl #0x30ea3c
005e4520 ldr r3, [pc, #0x268]
005e4524 add r0, r5, #0x114
005e4528 ldr r1, [r4, r3]
005e452c ldr r3, [pc, #0x260]
005e4530 ldr r2, [r4, r3]
005e4534 bl #0x30e304
005e4538 ldr r3, [sp, #0x54]
005e453c ldr r2, [sp, #0x5c]
005e4540 ldr r1, [sp, #0x64]
005e4544 ldr r0, [sp, #0x60]
005e4548 cmp r1, r3
005e454c beq #0x5e45c4
005e4550 add r3, r3, #8
005e4554 cmp r3, r2
005e4558 beq #0x5e4570
005e455c cmp r1, r3
005e4560 add r3, r3, #8
005e4564 beq #0x5e45c4
005e4568 cmp r2, r3
005e456c bne #0x5e455c
005e4570 ldr r3, [r0, #4]!
005e4574 add r2, r3, #0x80
005e4578 b #0x5e4548
005e457c mov r0, #0
005e4580 bl #0x5e80a4
005e4584 ldr fp, [r0, r6]
005e4588 mov r0, fp
005e458c bl #0x30de54
005e4590 mov r3, r0
005e4594 add r2, fp, r3
005e4598 add r0, r0, #1
005e459c b #0x5e436c
005e45a0 bl #0x6a4d9c
005e45a4 b #0x5e4404
005e45a8 mov r0, fp
005e45ac bl #0x6a4d9c
005e45b0 b #0x5e4420
005e45b4 mov r0, sl
005e45b8 ldr r1, [sp, #0x18]
005e45bc bl #0x5e1fa8
005e45c0 b #0x5e4478
005e45c4 ldr r0, [sp, #0x1c]
005e45c8 bl #0x5e1d1c
005e45cc b #0x5e22b0
005e45d0 add fp, sp, #0x90
005e45d4 mov r0, r6
005e45d8 sub r1, fp, #0xc
005e45dc bl #0x5e1fa8
005e45e0 b #0x5e24b8
005e45e4 eoreq lr, pc, r0, ror #25
005e45e8 .byte 0xd8, 0xec, 0x2f, 0x00
005e45ec eoreq lr, pc, ip, asr #25
005e45f0 .byte 0xb4, 0xec, 0x2f, 0x00
005e45f4 eoreq lr, pc, r0, lsr #25
005e45f8 mlaeq pc, r0, ip, lr
005e45fc eoreq lr, pc, r8, lsl #25
005e4600 eoreq lr, pc, ip, ror ip
005e4604 eoreq lr, pc, ip, ror #24
005e4608 eoreq lr, pc, r0, ror #24
005e460c eoreq lr, pc, r4, asr ip
005e4610 eoreq lr, pc, ip, asr #24
005e4614 eoreq lr, pc, r4, lsr ip
005e4618 eoreq lr, pc, ip, lsr #24
005e461c eoreq lr, pc, r8, lsl ip
005e4620 eoreq lr, pc, r0, lsl ip
005e4624 eoreq lr, pc, r8, lsl #24
005e4628 .byte 0xf8, 0xeb, 0x2f, 0x00
005e462c .byte 0xf0, 0xeb, 0x2f, 0x00
005e4630 eoreq lr, pc, r8, ror #23
005e4634 eoreq lr, pc, r0, ror #23
005e4638 eoreq lr, pc, r8, ror #23
005e463c eoreq lr, pc, ip, asr #23
005e4640 .byte 0xbc, 0xeb, 0x2f, 0x00
005e4644 .byte 0xb0, 0xeb, 0x2f, 0x00
005e4648 eoreq lr, pc, r8, lsl #23
005e464c eoreq lr, pc, r0, lsl #23
005e4650 eoreq lr, pc, ip, ror fp
005e4654 eoreq lr, pc, ip, ror #22
005e4658 eoreq lr, pc, r4, ror #22
005e465c eoreq lr, pc, ip, asr fp
005e4660 eoreq lr, pc, r8, asr fp
005e4664 eoreq lr, pc, r0, asr fp
005e4668 eoreq lr, pc, r8, asr #22
005e466c eoreq lr, pc, r0, asr #22
005e4670 eoreq lr, pc, r8, lsr fp
005e4674 eoreq lr, pc, r4, lsr fp
005e4678 eoreq lr, pc, ip, lsr #22
005e467c eoreq lr, pc, r4, lsr #22
005e4680 eoreq lr, pc, ip, lsl fp
005e4684 eoreq lr, pc, r4, lsl fp
005e4688 eoreq lr, pc, ip, lsl fp
005e468c eoreq lr, pc, r0, lsl #22
005e4690 .byte 0xf0, 0xea, 0x2f, 0x00
005e4694 eoreq lr, pc, r4, ror #21
005e4698 eoreq lr, pc, r0, asr #21
005e469c .byte 0xbc, 0xea, 0x2f, 0x00
005e46a0 .byte 0xb4, 0xea, 0x2f, 0x00
005e46a4 .byte 0xb0, 0xea, 0x2f, 0x00
005e46a8 eoreq lr, pc, r8, lsr #21
005e46ac eoreq lr, sp, r0, asr #11
005e46b0 eoreq lr, pc, r8, ror sl
005e46b4 eoreq lr, pc, r8, ror #20
005e46b8 eoreq lr, pc, r8, asr sl
005e46bc eoreq lr, pc, r0, asr sl
005e46c0 eoreq lr, pc, ip, asr #20
005e46c4 eoreq lr, pc, r8, asr #20
005e46c8 eoreq lr, pc, r0, asr #20
005e46cc eoreq lr, pc, ip, lsr sl
005e46d0 eoreq lr, pc, r0, lsr sl
005e46d4 eoreq lr, pc, ip, lsl sl
005e46d8 eoreq lr, pc, r0, lsl sl
005e46dc eoreq lr, pc, r0, lsl #20
005e46e0 .byte 0xf8, 0xe9, 0x2f, 0x00
005e46e4 eorseq r8, r0, r4, lsl lr
005e46e8 eoreq lr, pc, r4, asr #19
005e46ec eoreq lr, pc, ip, lsr #19
005e46f0 eoreq lr, pc, r4, lsr #19
005e46f4 mlaeq pc, ip, sb, lr
005e46f8 mlaeq pc, r4, sb, lr
005e46fc eoreq lr, pc, r8, lsl #19
005e4700 eoreq lr, pc, r0, lsl #19
005e4704 eoreq lr, pc, r8, ror sb
005e4708 eoreq lr, pc, r0, ror sb
005e470c eoreq lr, pc, r4, ror #18
005e4710 eoreq lr, pc, ip, asr sb
005e4714 eoreq lr, pc, r0, asr sb
005e4718 eoreq lr, pc, r4, asr #18
005e471c eoreq lr, pc, r0, asr #18
005e4720 eoreq lr, pc, ip, lsr #18
005e4724 eoreq lr, pc, r8, lsr #18
005e4728 eoreq lr, pc, r0, lsr #18
005e472c eoreq lr, pc, r0, lsl sb
005e4730 .byte 0xfc, 0xe8, 0x2f, 0x00
005e4734 .byte 0xf4, 0xe8, 0x2f, 0x00
005e4738 eoreq lr, pc, ip, ror #17
005e473c eoreq lr, pc, r8, ror #17
005e4740 .byte 0xd8, 0xe8, 0x2f, 0x00
005e4744 eoreq lr, pc, r8, asr #17
005e4748 eoreq lr, pc, r0, asr #17
005e474c .byte 0xb8, 0xe8, 0x2f, 0x00
005e4750 .byte 0xb0, 0xe8, 0x2f, 0x00
005e4754 eoreq lr, pc, r8, lsr #17
005e4758 eoreq lr, pc, r4, lsr #17
005e475c eoreq lr, pc, r0, lsr #17
005e4760 mlaeq pc, r8, r8, lr
005e4764 mlaeq pc, r0, r8, lr
005e4768 eoreq lr, pc, ip, lsl #17
005e476c eoreq lr, pc, ip, ror r8
005e4770 eoreq lr, pc, r8, ror #16
005e4774 eoreq lr, pc, r0, ror #16
005e4778 eoreq r2, lr, r8, lsr r1
005e477c subeq r2, r1, r4, asr #14
005e4780 andeq r3, r0, r0, ror #13
005e4784 ldrdeq r2, r3, [r1], #-0x5c
005e4788 subeq r2, r1, r4, asr #11
005e478c subeq r2, r1, ip, asr r5
005e4790 andeq r3, r0, r0, lsl sl
005e4794 muleq r0, r0, r8
_ZN6glitch5video18guessSubIdFromNameEPKcNS0_23E_SHADER_PARAMETER_TYPEE 0x5e7f80 292
005e7f80 sub r3, r1, #0x13
005e7f84 cmp r3, #8
005e7f88 push {r4, lr}
005e7f8c mov r4, r0
005e7f90 bls #0x5e7fc4
005e7f94 cmp r1, #0x20
005e7f98 beq #0x5e802c
005e7f9c cmp r1, #0xe
005e7fa0 beq #0x5e805c
005e7fa4 sub r3, r1, #0x1d
005e7fa8 cmp r3, #2
005e7fac bls #0x5e7fdc
005e7fb0 cmp r1, #0x21
005e7fb4 cmpne r1, #2
005e7fb8 beq #0x5e7ff8
005e7fbc mov r0, #0xff
005e7fc0 pop {r4, pc}
005e7fc4 ldr r1, [pc, #0xb8]
005e7fc8 add r1, pc, r1
005e7fcc bl #0x5e7e00
005e7fd0 cmp r0, #0xff
005e7fd4 beq #0x5e7ff0
005e7fd8 pop {r4, pc}
005e7fdc ldr r1, [pc, #0xa4]
005e7fe0 add r1, pc, r1
005e7fe4 bl #0x5e7e00
005e7fe8 cmp r0, #0xff
005e7fec bne #0x5e7fd8
005e7ff0 mov r0, #0
005e7ff4 pop {r4, pc}
005e7ff8 ldr r1, [pc, #0x8c]
005e7ffc add r1, pc, r1
005e8000 bl #0x5e7e00
005e8004 cmp r0, #0xff
005e8008 bne #0x5e7fd8
005e800c ldr r1, [pc, #0x7c]
005e8010 mov r0, r4
005e8014 add r1, pc, r1
005e8018 bl #0x5e7e00
005e801c cmp r0, #0xff
005e8020 bne #0x5e7fd8
005e8024 mov r0, #0
005e8028 b #0x5e7ff4
005e802c ldr r1, [pc, #0x60]
005e8030 add r1, pc, r1
005e8034 bl #0x5e7e00
005e8038 cmp r0, #0xff
005e803c bne #0x5e7fd8
005e8040 ldr r1, [pc, #0x50]
005e8044 mov r0, r4
005e8048 add r1, pc, r1
005e804c bl #0x5e7e00
005e8050 cmp r0, #0xff
005e8054 bne #0x5e7fd8
005e8058 b #0x5e7ff0
005e805c ldr r1, [pc, #0x38]
005e8060 add r1, pc, r1
005e8064 bl #0x5e7e00
005e8068 cmp r0, #0xff
005e806c bne #0x5e7fd8
005e8070 ldr r1, [pc, #0x28]
005e8074 mov r0, r4
005e8078 add r1, pc, r1
005e807c pop {r4, lr}
005e8080 b #0x5e7e00
005e8084 eoreq sl, pc, r8, ror #22
005e8088 eorseq r4, r0, r8, lsl #30
005e808c eoreq sl, pc, ip, ror #17
005e8090 eorseq r4, r0, r4, lsl #26
005e8094 eoreq sl, pc, r8, asr #20
005e8098 eoreq sl, pc, r0, asr #20
005e809c eoreq sl, pc, r8, asr #19
005e80a0 eoreq sl, pc, r0, lsr #19
_ZN6glitch5video14sortParametersEPNS0_19SShaderParameterDefEt 0x5e7b50 688
005e7b50 cmp r1, #0
005e7b54 push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
005e7b58 mov fp, r0
005e7b5c moveq sb, r1
005e7b60 bne #0x5e7b6c
005e7b64 mov r0, sb
005e7b68 pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
005e7b6c lsl sb, r1, #4
005e7b70 mov r0, sb
005e7b74 bl #0x5345f4
005e7b78 mov sl, r0
005e7b7c lsr r0, sb, #4
005e7b80 cmp r0, #0
005e7b84 ble #0x5e7be0
005e7b88 add r2, fp, #0x10
005e7b8c add r3, sl, #0x10
005e7b90 ldr r1, [r2, #-0x10]
005e7b94 str r1, [r3, #-0x10]
005e7b98 cmp r1, #0
005e7b9c ldrne ip, [r1]
005e7ba0 addne ip, ip, #1
005e7ba4 strne ip, [r1]
005e7ba8 ldrh r1, [r2, #-0xc]
005e7bac subs r0, r0, #1
005e7bb0 strh r1, [r3, #-0xc]
005e7bb4 ldrb r1, [r2, #-0xa]
005e7bb8 strb r1, [r3, #-0xa]
005e7bbc ldrb r1, [r2, #-9]
005e7bc0 strb r1, [r3, #-9]
005e7bc4 ldr r1, [r2, #-8]
005e7bc8 str r1, [r3, #-8]
005e7bcc ldr r1, [r2, #-4]
005e7bd0 add r2, r2, #0x10
005e7bd4 str r1, [r3, #-4]
005e7bd8 add r3, r3, #0x10
005e7bdc bne #0x5e7b90
005e7be0 add sb, sl, sb
005e7be4 cmp sb, sl
005e7be8 moveq sb, #0
005e7bec beq #0x5e7ddc
005e7bf0 mov r4, sl
005e7bf4 mov r7, fp
005e7bf8 mov r6, sl
005e7bfc b #0x5e7c80
005e7c00 ldr r3, [r4]
005e7c04 add r5, r7, #0x10
005e7c08 cmp r3, #0
005e7c0c ldrne r2, [r3]
005e7c10 addne r2, r2, #1
005e7c14 strne r2, [r3]
005e7c18 ldr r0, [r7]
005e7c1c str r3, [r7]
005e7c20 cmp r0, #0
005e7c24 beq #0x5e7c40
005e7c28 ldr r3, [r0]
005e7c2c sub r3, r3, #1
005e7c30 cmp r3, #0
005e7c34 str r3, [r0]
005e7c38 bne #0x5e7c40
005e7c3c bl #0x6a4d9c
005e7c40 ldrh r3, [r4, #4]
005e7c44 mov r8, r6
005e7c48 strh r3, [r7, #4]
005e7c4c ldrb r3, [r4, #6]
005e7c50 strb r3, [r7, #6]
005e7c54 ldrb r3, [r4, #7]
005e7c58 strb r3, [r7, #7]
005e7c5c ldr r3, [r4, #8]
005e7c60 str r3, [r7, #8]
005e7c64 ldr r3, [r4, #0xc]
005e7c68 add r4, r4, #0x10
005e7c6c cmp sb, r4
005e7c70 str r3, [r7, #0xc]
005e7c74 beq #0x5e7d08
005e7c78 mov r7, r5
005e7c7c mov r6, r8
005e7c80 ldrh r3, [r4, #4]
005e7c84 sub r3, r3, #0x22
005e7c88 cmp r3, #0x1c
005e7c8c bls #0x5e7c00
005e7c90 ldr r3, [r4]
005e7c94 add r8, r6, #0x10
005e7c98 cmp r3, #0
005e7c9c ldrne r2, [r3]
005e7ca0 addne r2, r2, #1
005e7ca4 strne r2, [r3]
005e7ca8 ldr r0, [r6]
005e7cac str r3, [r6]
005e7cb0 cmp r0, #0
005e7cb4 beq #0x5e7cd0
005e7cb8 ldr r3, [r0]
005e7cbc sub r3, r3, #1
005e7cc0 cmp r3, #0
005e7cc4 str r3, [r0]
005e7cc8 bne #0x5e7cd0
005e7ccc bl #0x6a4d9c
005e7cd0 ldrh r1, [r4, #4]
005e7cd4 mov r5, r7
005e7cd8 strh r1, [r6, #4]
005e7cdc ldrb r3, [r4, #6]
005e7ce0 strb r3, [r6, #6]
005e7ce4 ldrb r3, [r4, #7]
005e7ce8 strb r3, [r6, #7]
005e7cec ldr r3, [r4, #8]
005e7cf0 str r3, [r6, #8]
005e7cf4 ldr r3, [r4, #0xc]
005e7cf8 add r4, r4, #0x10
005e7cfc cmp sb, r4
005e7d00 str r3, [r6, #0xc]
005e7d04 bne #0x5e7c78
005e7d08 rsb r7, sl, r8
005e7d0c asr r7, r7, #4
005e7d10 rsb fp, fp, r5
005e7d14 cmp r7, #0
005e7d18 ubfx sb, fp, #4, #0x10
005e7d1c ble #0x5e7df4
005e7d20 add r5, r5, #0x10
005e7d24 add r6, sl, #0x10
005e7d28 ldr r3, [r6, #-0x10]
005e7d2c cmp r3, #0
005e7d30 ldrne r2, [r3]
005e7d34 addne r2, r2, #1
005e7d38 strne r2, [r3]
005e7d3c ldr r0, [r5, #-0x10]
005e7d40 str r3, [r5, #-0x10]
005e7d44 cmp r0, #0
005e7d48 beq #0x5e7d64
005e7d4c ldr r3, [r0]
005e7d50 sub r3, r3, #1
005e7d54 cmp r3, #0
005e7d58 str r3, [r0]
005e7d5c bne #0x5e7d64
005e7d60 bl #0x6a4d9c
005e7d64 ldrh r3, [r6, #-0xc]
005e7d68 subs r7, r7, #1
005e7d6c strh r3, [r5, #-0xc]
005e7d70 ldrb r3, [r6, #-0xa]
005e7d74 strb r3, [r5, #-0xa]
005e7d78 ldrb r3, [r6, #-9]
005e7d7c strb r3, [r5, #-9]
005e7d80 ldr r3, [r6, #-8]
005e7d84 str r3, [r5, #-8]
005e7d88 ldr r3, [r6, #-4]
005e7d8c add r6, r6, #0x10
005e7d90 str r3, [r5, #-4]
005e7d94 add r5, r5, #0x10
005e7d98 bne #0x5e7d28
005e7d9c mov r5, sl
005e7da0 b #0x5e7dac
005e7da4 cmp r4, r5
005e7da8 beq #0x5e7ddc
005e7dac ldr r0, [r5]
005e7db0 add r5, r5, #0x10
005e7db4 cmp r0, #0
005e7db8 beq #0x5e7da4
005e7dbc ldr r3, [r0]
005e7dc0 sub r3, r3, #1
005e7dc4 cmp r3, #0
005e7dc8 str r3, [r0]
005e7dcc bne #0x5e7da4
005e7dd0 bl #0x6a4d9c
005e7dd4 cmp r4, r5
005e7dd8 bne #0x5e7dac
005e7ddc cmp sl, #0
005e7de0 beq #0x5e7b64
005e7de4 mov r0, sl
005e7de8 bl #0x534688
005e7dec mov r0, sb
005e7df0 pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
005e7df4 cmp sl, r4
005e7df8 bne #0x5e7d9c
005e7dfc b #0x5e7ddc
