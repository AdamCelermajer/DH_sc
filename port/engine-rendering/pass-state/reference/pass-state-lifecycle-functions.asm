; Exact selected ARM function rows from the APK-matched recovered assembly listings.
; Each range is byte-compared with the original libDungeonHunter2.so member.

; FUNCTION 0x005b69c0, declared_size=780, range_size=780, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18restoreRenderStateEv
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::restoreRenderState()
; decoder-mode: arm
005b69c0  f0 40 2d e9                                      push {r4, r5, r6, r7, lr}
005b69c4  c4 31 d0 e5                                      ldrb r3, [r0, #0x1c4]
005b69c8  24 d0 4d e2                                      sub sp, sp, #0x24
005b69cc  00 40 a0 e1                                      mov r4, r0
005b69d0  00 00 53 e3                                      cmp r3, #0
005b69d4  93 00 00 1a                                      bne #0x5b6c28
005b69d8  e2 0b 00 e3                                      movw r0, #0xbe2
005b69dc  61 5d f5 eb                                      bl #0x30df68
005b69e0  9c 30 94 e5                                      ldr r3, [r4, #0x9c]
005b69e4  01 07 13 e3                                      tst r3, #0x40000
005b69e8  93 00 00 1a                                      bne #0x5b6c3c
005b69ec  00 22 94 e5                                      ldr r2, [r4, #0x200]
005b69f0  c0 32 9f e5                                      ldr r3, [pc, #0x2c0]
005b69f4  52 14 e7 e7                                      ubfx r1, r2, #8, #8
005b69f8  03 30 8f e0                                      add r3, pc, r3
005b69fc  72 20 ef e6                                      uxtb r2, r2
005b6a00  02 01 93 e7                                      ldr r0, [r3, r2, lsl #2]
005b6a04  01 11 93 e7                                      ldr r1, [r3, r1, lsl #2]
005b6a08  82 5e f5 eb                                      bl #0x30e418
005b6a0c  ee 21 d4 e5                                      ldrb r2, [r4, #0x1ee]
005b6a10  ef 31 d4 e5                                      ldrb r3, [r4, #0x1ef]
005b6a14  ed 11 d4 e5                                      ldrb r1, [r4, #0x1ed]
005b6a18  ec 01 d4 e5                                      ldrb r0, [r4, #0x1ec]
005b6a1c  2a 5d f5 eb                                      bl #0x30decc
005b6a20  08 02 d4 e5                                      ldrb r0, [r4, #0x208]
005b6a24  ce 5f f5 eb                                      bl #0x30e964
005b6a28  81 10 08 e3                                      movw r1, #0x8081
005b6a2c  80 1b 43 e3                                      movt r1, #0x3b80
005b6a30  cd 60 f5 eb                                      bl #0x30ed6c
005b6a34  00 70 a0 e1                                      mov r7, r0
005b6a38  09 02 d4 e5                                      ldrb r0, [r4, #0x209]
005b6a3c  c8 5f f5 eb                                      bl #0x30e964
005b6a40  81 10 08 e3                                      movw r1, #0x8081
005b6a44  80 1b 43 e3                                      movt r1, #0x3b80
005b6a48  c7 60 f5 eb                                      bl #0x30ed6c
005b6a4c  00 60 a0 e1                                      mov r6, r0
005b6a50  0a 02 d4 e5                                      ldrb r0, [r4, #0x20a]
005b6a54  c2 5f f5 eb                                      bl #0x30e964
005b6a58  81 10 08 e3                                      movw r1, #0x8081
005b6a5c  80 1b 43 e3                                      movt r1, #0x3b80
005b6a60  c1 60 f5 eb                                      bl #0x30ed6c
005b6a64  00 50 a0 e1                                      mov r5, r0
005b6a68  0b 02 d4 e5                                      ldrb r0, [r4, #0x20b]
005b6a6c  bc 5f f5 eb                                      bl #0x30e964
005b6a70  81 10 08 e3                                      movw r1, #0x8081
005b6a74  80 1b 43 e3                                      movt r1, #0x3b80
005b6a78  bb 60 f5 eb                                      bl #0x30ed6c
005b6a7c  06 10 a0 e1                                      mov r1, r6
005b6a80  00 30 a0 e1                                      mov r3, r0
005b6a84  05 20 a0 e1                                      mov r2, r5
005b6a88  07 00 a0 e1                                      mov r0, r7
005b6a8c  db 5f f5 eb                                      bl #0x30ea00
005b6a90  c5 31 d4 e5                                      ldrb r3, [r4, #0x1c5]
005b6a94  00 00 53 e3                                      cmp r3, #0
005b6a98  83 00 00 1a                                      bne #0x5b6cac
005b6a9c  44 0b 00 e3                                      movw r0, #0xb44
005b6aa0  30 5d f5 eb                                      bl #0x30df68
005b6aa4  10 32 9f e5                                      ldr r3, [pc, #0x210]
005b6aa8  d8 21 94 e5                                      ldr r2, [r4, #0x1d8]
005b6aac  03 30 8f e0                                      add r3, pc, r3
005b6ab0  50 30 83 e2                                      add r3, r3, #0x50
005b6ab4  02 01 93 e7                                      ldr r0, [r3, r2, lsl #2]
005b6ab8  7c 5f f5 eb                                      bl #0x30e8b0
005b6abc  a0 34 d4 e5                                      ldrb r3, [r4, #0x4a0]
005b6ac0  dc 21 94 e5                                      ldr r2, [r4, #0x1dc]
005b6ac4  00 00 53 e3                                      cmp r3, #0
005b6ac8  f0 31 9f e5                                      ldr r3, [pc, #0x1f0]
005b6acc  01 20 62 12                                      rsbne r2, r2, #1
005b6ad0  03 30 8f e0                                      add r3, pc, r3
005b6ad4  02 31 83 e0                                      add r3, r3, r2, lsl #2
005b6ad8  9c 00 93 e5                                      ldr r0, [r3, #0x9c]
005b6adc  e5 5c f5 eb                                      bl #0x30de78
005b6ae0  c6 31 d4 e5                                      ldrb r3, [r4, #0x1c6]
005b6ae4  00 00 53 e3                                      cmp r3, #0
005b6ae8  6c 00 00 1a                                      bne #0x5b6ca0
005b6aec  71 0b 00 e3                                      movw r0, #0xb71
005b6af0  1c 5d f5 eb                                      bl #0x30df68
005b6af4  c8 31 9f e5                                      ldr r3, [pc, #0x1c8]
005b6af8  e0 21 94 e5                                      ldr r2, [r4, #0x1e0]
005b6afc  03 30 8f e0                                      add r3, pc, r3
005b6b00  5c 30 83 e2                                      add r3, r3, #0x5c
005b6b04  02 01 93 e7                                      ldr r0, [r3, r2, lsl #2]
005b6b08  e2 5d f5 eb                                      bl #0x30e298
005b6b0c  c7 01 d4 e5                                      ldrb r0, [r4, #0x1c7]
005b6b10  c8 5d f5 eb                                      bl #0x30e238
005b6b14  0c 02 94 e5                                      ldr r0, [r4, #0x20c]
005b6b18  5f 5e f5 eb                                      bl #0x30e49c
005b6b1c  10 02 94 e5                                      ldr r0, [r4, #0x210]
005b6b20  14 12 94 e5                                      ldr r1, [r4, #0x214]
005b6b24  af 5f f5 eb                                      bl #0x30e9e8
005b6b28  c8 31 d4 e5                                      ldrb r3, [r4, #0x1c8]
005b6b2c  00 00 53 e3                                      cmp r3, #0
005b6b30  57 00 00 1a                                      bne #0x5b6c94
005b6b34  bd 0e a0 e3                                      mov r0, #0xbd0
005b6b38  0a 5d f5 eb                                      bl #0x30df68
005b6b3c  18 02 94 e5                                      ldr r0, [r4, #0x218]
005b6b40  65 60 f5 eb                                      bl #0x30ecdc
005b6b44  cc 31 d4 e5                                      ldrb r3, [r4, #0x1cc]
005b6b48  00 00 53 e3                                      cmp r3, #0
005b6b4c  4d 00 00 1a                                      bne #0x5b6c88
005b6b50  37 00 08 e3                                      movw r0, #0x8037
005b6b54  03 5d f5 eb                                      bl #0x30df68
005b6b58  20 02 94 e5                                      ldr r0, [r4, #0x220]
005b6b5c  24 12 94 e5                                      ldr r1, [r4, #0x224]
005b6b60  e2 5f f5 eb                                      bl #0x30eaf0
005b6b64  d0 31 d4 e5                                      ldrb r3, [r4, #0x1d0]
005b6b68  00 00 53 e3                                      cmp r3, #0
005b6b6c  42 00 00 1a                                      bne #0x5b6c7c
005b6b70  9e 00 08 e3                                      movw r0, #0x809e
005b6b74  fb 5c f5 eb                                      bl #0x30df68
005b6b78  d1 31 d4 e5                                      ldrb r3, [r4, #0x1d1]
005b6b7c  00 00 53 e3                                      cmp r3, #0
005b6b80  3a 00 00 1a                                      bne #0x5b6c70
005b6b84  a0 00 08 e3                                      movw r0, #0x80a0
005b6b88  f6 5c f5 eb                                      bl #0x30df68
005b6b8c  28 02 94 e5                                      ldr r0, [r4, #0x228]
005b6b90  d2 11 d4 e5                                      ldrb r1, [r4, #0x1d2]
005b6b94  90 5c f5 eb                                      bl #0x30dddc
005b6b98  d3 31 d4 e5                                      ldrb r3, [r4, #0x1d3]
005b6b9c  00 00 53 e3                                      cmp r3, #0
005b6ba0  2f 00 00 1a                                      bne #0x5b6c64
005b6ba4  11 0c 00 e3                                      movw r0, #0xc11
005b6ba8  ee 5c f5 eb                                      bl #0x30df68
005b6bac  14 c0 8d e2                                      add ip, sp, #0x14
005b6bb0  00 c0 8d e5                                      str ip, [sp]
005b6bb4  10 c0 8d e2                                      add ip, sp, #0x10
005b6bb8  04 c0 8d e5                                      str ip, [sp, #4]
005b6bbc  01 c0 a0 e3                                      mov ip, #1
005b6bc0  8b 1f 84 e2                                      add r1, r4, #0x22c
005b6bc4  1c 20 8d e2                                      add r2, sp, #0x1c
005b6bc8  18 30 8d e2                                      add r3, sp, #0x18
005b6bcc  08 c0 8d e5                                      str ip, [sp, #8]
005b6bd0  04 00 a0 e1                                      mov r0, r4
005b6bd4  00 c0 a0 e3                                      mov ip, #0
005b6bd8  0c c0 8d e5                                      str ip, [sp, #0xc]
005b6bdc  28 9b 04 eb                                      bl #0x6dd884
005b6be0  10 30 9d e5                                      ldr r3, [sp, #0x10]
005b6be4  1c 00 9d e5                                      ldr r0, [sp, #0x1c]
005b6be8  18 10 9d e5                                      ldr r1, [sp, #0x18]
005b6bec  14 20 9d e5                                      ldr r2, [sp, #0x14]
005b6bf0  3e 5e f5 eb                                      bl #0x30e4f0
005b6bf4  d4 31 d4 e5                                      ldrb r3, [r4, #0x1d4]
005b6bf8  00 00 53 e3                                      cmp r3, #0
005b6bfc  15 00 00 1a                                      bne #0x5b6c58
005b6c00  b9 0e a0 e3                                      mov r0, #0xb90
005b6c04  d7 5c f5 eb                                      bl #0x30df68
005b6c08  54 12 94 e5                                      ldr r1, [r4, #0x254]
005b6c0c  92 08 08 e3                                      movw r0, #0x8892
005b6c10  77 5c f5 eb                                      bl #0x30ddf4
005b6c14  58 12 94 e5                                      ldr r1, [r4, #0x258]
005b6c18  93 08 08 e3                                      movw r0, #0x8893
005b6c1c  74 5c f5 eb                                      bl #0x30ddf4
005b6c20  24 d0 8d e2                                      add sp, sp, #0x24
005b6c24  f0 80 bd e8                                      pop {r4, r5, r6, r7, pc}
005b6c28  e2 0b 00 e3                                      movw r0, #0xbe2
005b6c2c  44 5e f5 eb                                      bl #0x30e544
005b6c30  9c 30 94 e5                                      ldr r3, [r4, #0x9c]
005b6c34  01 07 13 e3                                      tst r3, #0x40000
005b6c38  6b ff ff 0a                                      beq #0x5b69ec
005b6c3c  84 30 9f e5                                      ldr r3, [pc, #0x84]
005b6c40  fc 21 94 e5                                      ldr r2, [r4, #0x1fc]
005b6c44  03 30 8f e0                                      add r3, pc, r3
005b6c48  3c 30 83 e2                                      add r3, r3, #0x3c
005b6c4c  02 01 93 e7                                      ldr r0, [r3, r2, lsl #2]
005b6c50  af 5c f5 eb                                      bl #0x30df14
005b6c54  64 ff ff ea                                      b #0x5b69ec
005b6c58  b9 0e a0 e3                                      mov r0, #0xb90
005b6c5c  38 5e f5 eb                                      bl #0x30e544
005b6c60  e8 ff ff ea                                      b #0x5b6c08
005b6c64  11 0c 00 e3                                      movw r0, #0xc11
005b6c68  35 5e f5 eb                                      bl #0x30e544
005b6c6c  ce ff ff ea                                      b #0x5b6bac
005b6c70  a0 00 08 e3                                      movw r0, #0x80a0
005b6c74  32 5e f5 eb                                      bl #0x30e544
005b6c78  c3 ff ff ea                                      b #0x5b6b8c
005b6c7c  9e 00 08 e3                                      movw r0, #0x809e
005b6c80  2f 5e f5 eb                                      bl #0x30e544
005b6c84  bb ff ff ea                                      b #0x5b6b78
005b6c88  37 00 08 e3                                      movw r0, #0x8037
005b6c8c  2c 5e f5 eb                                      bl #0x30e544
005b6c90  b0 ff ff ea                                      b #0x5b6b58
005b6c94  bd 0e a0 e3                                      mov r0, #0xbd0
005b6c98  29 5e f5 eb                                      bl #0x30e544
005b6c9c  a6 ff ff ea                                      b #0x5b6b3c
005b6ca0  71 0b 00 e3                                      movw r0, #0xb71
005b6ca4  26 5e f5 eb                                      bl #0x30e544
005b6ca8  91 ff ff ea                                      b #0x5b6af4
005b6cac  44 0b 00 e3                                      movw r0, #0xb44
005b6cb0  23 5e f5 eb                                      bl #0x30e544
005b6cb4  7a ff ff ea                                      b #0x5b6aa4
005b6cb8  3c 96 32 00 88 95 32 00 64 95 32 00 38 95 32 00  .byte 0x3c, 0x96, 0x32, 0x00, 0x88, 0x95, 0x32, 0x00, 0x64, 0x95, 0x32, 0x00, 0x38, 0x95, 0x32, 0x00
005b6cc8  f0 93 32 00                                      .byte 0xf0, 0x93, 0x32, 0x00

; FUNCTION 0x005b6ccc, declared_size=340, range_size=340, mode=arm
; class-group: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>
; alias: _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE18restoreShadowStateEv
; demangled: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>::restoreShadowState()
; decoder-mode: arm
005b6ccc  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
005b6cd0  00 50 a0 e1                                      mov r5, r0
005b6cd4  08 d0 4d e2                                      sub sp, sp, #8
005b6cd8  38 ff ff eb                                      bl #0x5b69c0
005b6cdc  f4 30 95 e5                                      ldr r3, [r5, #0xf4]
005b6ce0  00 00 53 e3                                      cmp r3, #0
005b6ce4  1a 00 00 0a                                      beq #0x5b6d54
005b6ce8  4c 00 93 e5                                      ldr r0, [r3, #0x4c]
005b6cec  f2 5e f5 eb                                      bl #0x30e8bc
005b6cf0  24 38 95 e5                                      ldr r3, [r5, #0x824]
005b6cf4  1f 00 53 e3                                      cmp r3, #0x1f
005b6cf8  39 00 00 da                                      ble #0x5b6de4
005b6cfc  20 80 a0 e3                                      mov r8, #0x20
005b6d00  08 60 a0 e1                                      mov r6, r8
005b6d04  00 40 a0 e3                                      mov r4, #0
005b6d08  01 70 a0 e3                                      mov r7, #1
005b6d0c  04 00 00 ea                                      b #0x5b6d24
005b6d10  01 40 84 e2                                      add r4, r4, #1
005b6d14  6f 5c f5 eb                                      bl #0x30ded8
005b6d18  74 30 ff e6                                      uxth r3, r4
005b6d1c  03 00 56 e1                                      cmp r6, r3
005b6d20  08 00 00 9a                                      bls #0x5b6d48
005b6d24  70 32 95 e5                                      ldr r3, [r5, #0x270]
005b6d28  04 00 a0 e1                                      mov r0, r4
005b6d2c  17 34 13 e0                                      ands r3, r3, r7, lsl r4
005b6d30  f6 ff ff 1a                                      bne #0x5b6d10
005b6d34  01 40 84 e2                                      add r4, r4, #1
005b6d38  ed 5f f5 eb                                      bl #0x30ecf4
005b6d3c  74 30 ff e6                                      uxth r3, r4
005b6d40  03 00 56 e1                                      cmp r6, r3
005b6d44  f6 ff ff 8a                                      bhi #0x5b6d24
005b6d48  24 38 95 e5                                      ldr r3, [r5, #0x824]
005b6d4c  03 00 58 e1                                      cmp r8, r3
005b6d50  29 00 00 ba                                      blt #0x5b6dfc
005b6d54  01 80 a0 e3                                      mov r8, #1
005b6d58  00 70 a0 e3                                      mov r7, #0
005b6d5c  4c 60 95 e5                                      ldr r6, [r5, #0x4c]
005b6d60  00 00 56 e3                                      cmp r6, #0
005b6d64  08 00 00 0a                                      beq #0x5b6d8c
005b6d68  00 40 a0 e3                                      mov r4, #0
005b6d6c  04 10 a0 e1                                      mov r1, r4
005b6d70  05 00 a0 e1                                      mov r0, r5
005b6d74  01 40 84 e2                                      add r4, r4, #1
005b6d78  00 20 a0 e3                                      mov r2, #0
005b6d7c  07 30 a0 e1                                      mov r3, r7
005b6d80  5a ee ff eb                                      bl #0x5b26f0
005b6d84  06 00 54 e1                                      cmp r4, r6
005b6d88  f7 ff ff 1a                                      bne #0x5b6d6c
005b6d8c  08 70 a0 e1                                      mov r7, r8
005b6d90  01 80 88 e2                                      add r8, r8, #1
005b6d94  05 00 58 e3                                      cmp r8, #5
005b6d98  ef ff ff 1a                                      bne #0x5b6d5c
005b6d9c  ec 20 95 e5                                      ldr r2, [r5, #0xec]
005b6da0  00 00 52 e3                                      cmp r2, #0
005b6da4  0c 00 00 0a                                      beq #0x5b6ddc
005b6da8  04 10 92 e5                                      ldr r1, [r2, #4]
005b6dac  f8 30 d5 e5                                      ldrb r3, [r5, #0xf8]
005b6db0  0c e0 a0 e3                                      mov lr, #0xc
005b6db4  18 c0 91 e5                                      ldr ip, [r1, #0x18]
005b6db8  05 00 a0 e1                                      mov r0, r5
005b6dbc  f4 10 95 e5                                      ldr r1, [r5, #0xf4]
005b6dc0  9e c3 23 e0                                      mla r3, lr, r3, ip
005b6dc4  08 30 93 e5                                      ldr r3, [r3, #8]
005b6dc8  bc c2 d3 e1                                      ldrh ip, [r3, #0x2c]
005b6dcc  28 30 93 e5                                      ldr r3, [r3, #0x28]
005b6dd0  0c c1 83 e0                                      add ip, r3, ip, lsl #2
005b6dd4  00 c0 8d e5                                      str ip, [sp]
005b6dd8  f0 f7 ff eb                                      bl #0x5b4da0
005b6ddc  08 d0 8d e2                                      add sp, sp, #8
005b6de0  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}
005b6de4  73 60 ff e6                                      uxth r6, r3
005b6de8  00 00 56 e3                                      cmp r6, #0
005b6dec  06 80 a0 01                                      moveq r8, r6
005b6df0  06 80 a0 11                                      movne r8, r6
005b6df4  c2 ff ff 1a                                      bne #0x5b6d04
005b6df8  d3 ff ff ea                                      b #0x5b6d4c
005b6dfc  06 40 a0 e1                                      mov r4, r6
005b6e00  04 00 a0 e1                                      mov r0, r4
005b6e04  ba 5f f5 eb                                      bl #0x30ecf4
005b6e08  24 38 95 e5                                      ldr r3, [r5, #0x824]
005b6e0c  01 40 84 e2                                      add r4, r4, #1
005b6e10  74 40 ff e6                                      uxth r4, r4
005b6e14  03 00 54 e1                                      cmp r4, r3
005b6e18  f8 ff ff ba                                      blt #0x5b6e00
005b6e1c  cc ff ff ea                                      b #0x5b6d54

; FUNCTION 0x005b6e20, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE20setStencilTestEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setStencilTestEnable(bool)
; decoder-mode: arm
005b6e20  70 40 2d e9                                      push {r4, r5, r6, lr}
005b6e24  d4 31 d0 e5                                      ldrb r3, [r0, #0x1d4]
005b6e28  00 40 a0 e1                                      mov r4, r0
005b6e2c  01 50 a0 e1                                      mov r5, r1
005b6e30  01 00 53 e1                                      cmp r3, r1
005b6e34  07 00 00 0a                                      beq #0x5b6e58
005b6e38  00 30 90 e5                                      ldr r3, [r0]
005b6e3c  0f e0 a0 e1                                      mov lr, pc
005b6e40  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b6e44  00 00 55 e3                                      cmp r5, #0
005b6e48  03 00 00 1a                                      bne #0x5b6e5c
005b6e4c  b9 0e a0 e3                                      mov r0, #0xb90
005b6e50  44 5c f5 eb                                      bl #0x30df68
005b6e54  d4 51 c4 e5                                      strb r5, [r4, #0x1d4]
005b6e58  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b6e5c  b9 0e a0 e3                                      mov r0, #0xb90
005b6e60  b7 5d f5 eb                                      bl #0x30e544
005b6e64  d4 51 c4 e5                                      strb r5, [r4, #0x1d4]
005b6e68  fa ff ff ea                                      b #0x5b6e58

; FUNCTION 0x005b6eb8, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE23setSampleCoverageEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setSampleCoverageEnable(bool)
; decoder-mode: arm
005b6eb8  70 40 2d e9                                      push {r4, r5, r6, lr}
005b6ebc  d1 31 d0 e5                                      ldrb r3, [r0, #0x1d1]
005b6ec0  00 40 a0 e1                                      mov r4, r0
005b6ec4  01 50 a0 e1                                      mov r5, r1
005b6ec8  01 00 53 e1                                      cmp r3, r1
005b6ecc  07 00 00 0a                                      beq #0x5b6ef0
005b6ed0  00 30 90 e5                                      ldr r3, [r0]
005b6ed4  0f e0 a0 e1                                      mov lr, pc
005b6ed8  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b6edc  00 00 55 e3                                      cmp r5, #0
005b6ee0  03 00 00 1a                                      bne #0x5b6ef4
005b6ee4  a0 00 08 e3                                      movw r0, #0x80a0
005b6ee8  1e 5c f5 eb                                      bl #0x30df68
005b6eec  d1 51 c4 e5                                      strb r5, [r4, #0x1d1]
005b6ef0  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b6ef4  a0 00 08 e3                                      movw r0, #0x80a0
005b6ef8  91 5d f5 eb                                      bl #0x30e544
005b6efc  d1 51 c4 e5                                      strb r5, [r4, #0x1d1]
005b6f00  fa ff ff ea                                      b #0x5b6ef0

; FUNCTION 0x005b6f04, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE30setSampleAlphaToCoverageEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setSampleAlphaToCoverageEnable(bool)
; decoder-mode: arm
005b6f04  70 40 2d e9                                      push {r4, r5, r6, lr}
005b6f08  d0 31 d0 e5                                      ldrb r3, [r0, #0x1d0]
005b6f0c  00 40 a0 e1                                      mov r4, r0
005b6f10  01 50 a0 e1                                      mov r5, r1
005b6f14  01 00 53 e1                                      cmp r3, r1
005b6f18  07 00 00 0a                                      beq #0x5b6f3c
005b6f1c  00 30 90 e5                                      ldr r3, [r0]
005b6f20  0f e0 a0 e1                                      mov lr, pc
005b6f24  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b6f28  00 00 55 e3                                      cmp r5, #0
005b6f2c  03 00 00 1a                                      bne #0x5b6f40
005b6f30  9e 00 08 e3                                      movw r0, #0x809e
005b6f34  0b 5c f5 eb                                      bl #0x30df68
005b6f38  d0 51 c4 e5                                      strb r5, [r4, #0x1d0]
005b6f3c  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b6f40  9e 00 08 e3                                      movw r0, #0x809e
005b6f44  7e 5d f5 eb                                      bl #0x30e544
005b6f48  d0 51 c4 e5                                      strb r5, [r4, #0x1d0]
005b6f4c  fa ff ff ea                                      b #0x5b6f3c

; FUNCTION 0x005b6f50, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE26setPolygonOffsetFillEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setPolygonOffsetFillEnable(bool)
; decoder-mode: arm
005b6f50  70 40 2d e9                                      push {r4, r5, r6, lr}
005b6f54  cc 31 d0 e5                                      ldrb r3, [r0, #0x1cc]
005b6f58  00 40 a0 e1                                      mov r4, r0
005b6f5c  01 50 a0 e1                                      mov r5, r1
005b6f60  01 00 53 e1                                      cmp r3, r1
005b6f64  07 00 00 0a                                      beq #0x5b6f88
005b6f68  00 30 90 e5                                      ldr r3, [r0]
005b6f6c  0f e0 a0 e1                                      mov lr, pc
005b6f70  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b6f74  00 00 55 e3                                      cmp r5, #0
005b6f78  03 00 00 1a                                      bne #0x5b6f8c
005b6f7c  37 00 08 e3                                      movw r0, #0x8037
005b6f80  f8 5b f5 eb                                      bl #0x30df68
005b6f84  cc 51 c4 e5                                      strb r5, [r4, #0x1cc]
005b6f88  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b6f8c  37 00 08 e3                                      movw r0, #0x8037
005b6f90  6b 5d f5 eb                                      bl #0x30e544
005b6f94  cc 51 c4 e5                                      strb r5, [r4, #0x1cc]
005b6f98  fa ff ff ea                                      b #0x5b6f88

; FUNCTION 0x005b6fe8, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18setDepthTestEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setDepthTestEnable(bool)
; decoder-mode: arm
005b6fe8  70 40 2d e9                                      push {r4, r5, r6, lr}
005b6fec  c6 31 d0 e5                                      ldrb r3, [r0, #0x1c6]
005b6ff0  00 40 a0 e1                                      mov r4, r0
005b6ff4  01 50 a0 e1                                      mov r5, r1
005b6ff8  01 00 53 e1                                      cmp r3, r1
005b6ffc  07 00 00 0a                                      beq #0x5b7020
005b7000  00 30 90 e5                                      ldr r3, [r0]
005b7004  0f e0 a0 e1                                      mov lr, pc
005b7008  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b700c  00 00 55 e3                                      cmp r5, #0
005b7010  03 00 00 1a                                      bne #0x5b7024
005b7014  71 0b 00 e3                                      movw r0, #0xb71
005b7018  d2 5b f5 eb                                      bl #0x30df68
005b701c  c6 51 c4 e5                                      strb r5, [r4, #0x1c6]
005b7020  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b7024  71 0b 00 e3                                      movw r0, #0xb71
005b7028  45 5d f5 eb                                      bl #0x30e544
005b702c  c6 51 c4 e5                                      strb r5, [r4, #0x1c6]
005b7030  fa ff ff ea                                      b #0x5b7020

; FUNCTION 0x005b7034, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setCullFaceEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setCullFaceEnable(bool)
; decoder-mode: arm
005b7034  70 40 2d e9                                      push {r4, r5, r6, lr}
005b7038  c5 31 d0 e5                                      ldrb r3, [r0, #0x1c5]
005b703c  00 40 a0 e1                                      mov r4, r0
005b7040  01 50 a0 e1                                      mov r5, r1
005b7044  01 00 53 e1                                      cmp r3, r1
005b7048  07 00 00 0a                                      beq #0x5b706c
005b704c  00 30 90 e5                                      ldr r3, [r0]
005b7050  0f e0 a0 e1                                      mov lr, pc
005b7054  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b7058  00 00 55 e3                                      cmp r5, #0
005b705c  03 00 00 1a                                      bne #0x5b7070
005b7060  44 0b 00 e3                                      movw r0, #0xb44
005b7064  bf 5b f5 eb                                      bl #0x30df68
005b7068  c5 51 c4 e5                                      strb r5, [r4, #0x1c5]
005b706c  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b7070  44 0b 00 e3                                      movw r0, #0xb44
005b7074  32 5d f5 eb                                      bl #0x30e544
005b7078  c5 51 c4 e5                                      strb r5, [r4, #0x1c5]
005b707c  fa ff ff ea                                      b #0x5b706c

; FUNCTION 0x005b7080, declared_size=76, range_size=76, mode=arm
; class-group: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14setBlendEnableEb
; demangled: glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::setBlendEnable(bool)
; decoder-mode: arm
005b7080  70 40 2d e9                                      push {r4, r5, r6, lr}
005b7084  c4 31 d0 e5                                      ldrb r3, [r0, #0x1c4]
005b7088  00 40 a0 e1                                      mov r4, r0
005b708c  01 50 a0 e1                                      mov r5, r1
005b7090  01 00 53 e1                                      cmp r3, r1
005b7094  07 00 00 0a                                      beq #0x5b70b8
005b7098  00 30 90 e5                                      ldr r3, [r0]
005b709c  0f e0 a0 e1                                      mov lr, pc
005b70a0  fc f1 93 e5                                      ldr pc, [r3, #0x1fc]
005b70a4  00 00 55 e3                                      cmp r5, #0
005b70a8  03 00 00 1a                                      bne #0x5b70bc
005b70ac  e2 0b 00 e3                                      movw r0, #0xbe2
005b70b0  ac 5b f5 eb                                      bl #0x30df68
005b70b4  c4 51 c4 e5                                      strb r5, [r4, #0x1c4]
005b70b8  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b70bc  e2 0b 00 e3                                      movw r0, #0xbe2
005b70c0  1f 5d f5 eb                                      bl #0x30e544
005b70c4  c4 51 c4 e5                                      strb r5, [r4, #0x1c4]
005b70c8  fa ff ff ea                                      b #0x5b70b8

; FUNCTION 0x005b5c40, declared_size=312, range_size=312, mode=arm
; class-group: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>
; alias: _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEC2EPNS_7IDeviceE
; demangled: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>::CProgrammableGLDriver(glitch::IDevice*)
; decoder-mode: arm
005b5c40  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
005b5c44  00 80 a0 e1                                      mov r8, r0
005b5c48  01 50 a0 e1                                      mov r5, r1
005b5c4c  84 00 a0 e3                                      mov r0, #0x84
005b5c50  00 10 a0 e3                                      mov r1, #0
005b5c54  54 f9 fd eb                                      bl #0x5341ac
005b5c58  00 40 a0 e1                                      mov r4, r0
005b5c5c  08 61 9f e5                                      ldr r6, [pc, #0x108]
005b5c60  21 aa 04 eb                                      bl #0x6e04ec
005b5c64  05 10 a0 e1                                      mov r1, r5
005b5c68  04 20 a0 e1                                      mov r2, r4
005b5c6c  08 00 a0 e1                                      mov r0, r8
005b5c70  60 a2 04 eb                                      bl #0x6de5f8
005b5c74  f4 20 9f e5                                      ldr r2, [pc, #0xf4]
005b5c78  06 60 8f e0                                      add r6, pc, r6
005b5c7c  00 70 a0 e3                                      mov r7, #0
005b5c80  02 20 96 e7                                      ldr r2, [r6, r2]
005b5c84  08 30 a0 e1                                      mov r3, r8
005b5c88  82 4e 88 e2                                      add r4, r8, #0x820
005b5c8c  08 20 82 e2                                      add r2, r2, #8
005b5c90  f8 77 88 e5                                      str r7, [r8, #0x7f8]
005b5c94  00 20 88 e5                                      str r2, [r8]
005b5c98  fc 77 88 e5                                      str r7, [r8, #0x7fc]
005b5c9c  04 78 88 e5                                      str r7, [r8, #0x804]
005b5ca0  00 78 e3 e5                                      strb r7, [r3, #0x800]!
005b5ca4  0c 38 88 e5                                      str r3, [r8, #0x80c]
005b5ca8  08 38 88 e5                                      str r3, [r8, #0x808]
005b5cac  04 00 a0 e1                                      mov r0, r4
005b5cb0  10 78 88 e5                                      str r7, [r8, #0x810]
005b5cb4  3f a7 04 eb                                      bl #0x6df9b8
005b5cb8  b4 30 9f e5                                      ldr r3, [pc, #0xb4]
005b5cbc  d4 a0 84 e2                                      add sl, r4, #0xd4
005b5cc0  fe 55 a0 e3                                      mov r5, #0x3f800000
005b5cc4  03 30 96 e7                                      ldr r3, [r6, r3]
005b5cc8  24 78 88 e5                                      str r7, [r8, #0x824]
005b5ccc  08 40 84 e2                                      add r4, r4, #8
005b5cd0  08 30 83 e2                                      add r3, r3, #8
005b5cd4  00 30 88 e5                                      str r3, [r8]
005b5cd8  01 60 a0 e3                                      mov r6, #1
005b5cdc  40 70 c4 e5                                      strb r7, [r4, #0x40]
005b5ce0  04 00 a0 e1                                      mov r0, r4
005b5ce4  00 10 a0 e3                                      mov r1, #0
005b5ce8  40 20 a0 e3                                      mov r2, #0x40
005b5cec  db 61 f5 eb                                      bl #0x30e460
005b5cf0  00 50 84 e5                                      str r5, [r4]
005b5cf4  14 50 84 e5                                      str r5, [r4, #0x14]
005b5cf8  28 50 84 e5                                      str r5, [r4, #0x28]
005b5cfc  3c 50 84 e5                                      str r5, [r4, #0x3c]
005b5d00  40 60 c4 e5                                      strb r6, [r4, #0x40]
005b5d04  44 40 84 e2                                      add r4, r4, #0x44
005b5d08  0a 00 54 e1                                      cmp r4, sl
005b5d0c  f2 ff ff 1a                                      bne #0x5b5cdc
005b5d10  8f 4e 88 e2                                      add r4, r8, #0x8f0
005b5d14  04 40 84 e2                                      add r4, r4, #4
005b5d18  13 6d 84 e2                                      add r6, r4, #0x4c0
005b5d1c  08 60 86 e2                                      add r6, r6, #8
005b5d20  00 a0 a0 e3                                      mov sl, #0
005b5d24  01 70 a0 e3                                      mov r7, #1
005b5d28  40 a0 c4 e5                                      strb sl, [r4, #0x40]
005b5d2c  04 00 a0 e1                                      mov r0, r4
005b5d30  00 10 a0 e3                                      mov r1, #0
005b5d34  40 20 a0 e3                                      mov r2, #0x40
005b5d38  c8 61 f5 eb                                      bl #0x30e460
005b5d3c  00 50 84 e5                                      str r5, [r4]
005b5d40  14 50 84 e5                                      str r5, [r4, #0x14]
005b5d44  28 50 84 e5                                      str r5, [r4, #0x28]
005b5d48  3c 50 84 e5                                      str r5, [r4, #0x3c]
005b5d4c  40 70 c4 e5                                      strb r7, [r4, #0x40]
005b5d50  44 40 84 e2                                      add r4, r4, #0x44
005b5d54  06 00 54 e1                                      cmp r4, r6
005b5d58  f2 ff ff 1a                                      bne #0x5b5d28
005b5d5c  00 30 e0 e3                                      mvn r3, #0
005b5d60  bc 3d 88 e5                                      str r3, [r8, #0xdbc]
005b5d64  08 00 a0 e1                                      mov r0, r8
005b5d68  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b5d6c  18 ee 3d 00 74 45 00 00 18 14 00 00              .byte 0x18, 0xee, 0x3d, 0x00, 0x74, 0x45, 0x00, 0x00, 0x18, 0x14, 0x00, 0x00

; FUNCTION 0x005b5e34, declared_size=312, range_size=312, mode=arm
; class-group: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>
; alias: _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEC1EPNS_7IDeviceE
; demangled: glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>::CProgrammableGLDriver(glitch::IDevice*)
; decoder-mode: arm
005b5e34  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
005b5e38  00 80 a0 e1                                      mov r8, r0
005b5e3c  01 50 a0 e1                                      mov r5, r1
005b5e40  84 00 a0 e3                                      mov r0, #0x84
005b5e44  00 10 a0 e3                                      mov r1, #0
005b5e48  d7 f8 fd eb                                      bl #0x5341ac
005b5e4c  00 40 a0 e1                                      mov r4, r0
005b5e50  08 61 9f e5                                      ldr r6, [pc, #0x108]
005b5e54  a4 a9 04 eb                                      bl #0x6e04ec
005b5e58  05 10 a0 e1                                      mov r1, r5
005b5e5c  04 20 a0 e1                                      mov r2, r4
005b5e60  08 00 a0 e1                                      mov r0, r8
005b5e64  e3 a1 04 eb                                      bl #0x6de5f8
005b5e68  f4 20 9f e5                                      ldr r2, [pc, #0xf4]
005b5e6c  06 60 8f e0                                      add r6, pc, r6
005b5e70  00 70 a0 e3                                      mov r7, #0
005b5e74  02 20 96 e7                                      ldr r2, [r6, r2]
005b5e78  08 30 a0 e1                                      mov r3, r8
005b5e7c  82 4e 88 e2                                      add r4, r8, #0x820
005b5e80  08 20 82 e2                                      add r2, r2, #8
005b5e84  f8 77 88 e5                                      str r7, [r8, #0x7f8]
005b5e88  00 20 88 e5                                      str r2, [r8]
005b5e8c  fc 77 88 e5                                      str r7, [r8, #0x7fc]
005b5e90  04 78 88 e5                                      str r7, [r8, #0x804]
005b5e94  00 78 e3 e5                                      strb r7, [r3, #0x800]!
005b5e98  0c 38 88 e5                                      str r3, [r8, #0x80c]
005b5e9c  08 38 88 e5                                      str r3, [r8, #0x808]
005b5ea0  04 00 a0 e1                                      mov r0, r4
005b5ea4  10 78 88 e5                                      str r7, [r8, #0x810]
005b5ea8  c2 a6 04 eb                                      bl #0x6df9b8
005b5eac  b4 30 9f e5                                      ldr r3, [pc, #0xb4]
005b5eb0  d4 a0 84 e2                                      add sl, r4, #0xd4
005b5eb4  fe 55 a0 e3                                      mov r5, #0x3f800000
005b5eb8  03 30 96 e7                                      ldr r3, [r6, r3]
005b5ebc  24 78 88 e5                                      str r7, [r8, #0x824]
005b5ec0  08 40 84 e2                                      add r4, r4, #8
005b5ec4  08 30 83 e2                                      add r3, r3, #8
005b5ec8  00 30 88 e5                                      str r3, [r8]
005b5ecc  01 60 a0 e3                                      mov r6, #1
005b5ed0  40 70 c4 e5                                      strb r7, [r4, #0x40]
005b5ed4  04 00 a0 e1                                      mov r0, r4
005b5ed8  00 10 a0 e3                                      mov r1, #0
005b5edc  40 20 a0 e3                                      mov r2, #0x40
005b5ee0  5e 61 f5 eb                                      bl #0x30e460
005b5ee4  00 50 84 e5                                      str r5, [r4]
005b5ee8  14 50 84 e5                                      str r5, [r4, #0x14]
005b5eec  28 50 84 e5                                      str r5, [r4, #0x28]
005b5ef0  3c 50 84 e5                                      str r5, [r4, #0x3c]
005b5ef4  40 60 c4 e5                                      strb r6, [r4, #0x40]
005b5ef8  44 40 84 e2                                      add r4, r4, #0x44
005b5efc  0a 00 54 e1                                      cmp r4, sl
005b5f00  f2 ff ff 1a                                      bne #0x5b5ed0
005b5f04  8f 4e 88 e2                                      add r4, r8, #0x8f0
005b5f08  04 40 84 e2                                      add r4, r4, #4
005b5f0c  13 6d 84 e2                                      add r6, r4, #0x4c0
005b5f10  08 60 86 e2                                      add r6, r6, #8
005b5f14  00 a0 a0 e3                                      mov sl, #0
005b5f18  01 70 a0 e3                                      mov r7, #1
005b5f1c  40 a0 c4 e5                                      strb sl, [r4, #0x40]
005b5f20  04 00 a0 e1                                      mov r0, r4
005b5f24  00 10 a0 e3                                      mov r1, #0
005b5f28  40 20 a0 e3                                      mov r2, #0x40
005b5f2c  4b 61 f5 eb                                      bl #0x30e460
005b5f30  00 50 84 e5                                      str r5, [r4]
005b5f34  14 50 84 e5                                      str r5, [r4, #0x14]
005b5f38  28 50 84 e5                                      str r5, [r4, #0x28]
005b5f3c  3c 50 84 e5                                      str r5, [r4, #0x3c]
005b5f40  40 70 c4 e5                                      strb r7, [r4, #0x40]
005b5f44  44 40 84 e2                                      add r4, r4, #0x44
005b5f48  06 00 54 e1                                      cmp r4, r6
005b5f4c  f2 ff ff 1a                                      bne #0x5b5f1c
005b5f50  00 30 e0 e3                                      mvn r3, #0
005b5f54  bc 3d 88 e5                                      str r3, [r8, #0xdbc]
005b5f58  08 00 a0 e1                                      mov r0, r8
005b5f5c  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b5f60  24 ec 3d 00 74 45 00 00 18 14 00 00              .byte 0x24, 0xec, 0x3d, 0x00, 0x74, 0x45, 0x00, 0x00, 0x18, 0x14, 0x00, 0x00
