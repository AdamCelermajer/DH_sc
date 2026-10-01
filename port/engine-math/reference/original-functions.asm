; FUNCTION 0x0035e8e0, declared_size=184, range_size=184, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZN6glitch4core8vector3dIfE9normalizeEv
; demangled: glitch::core::vector3d<float>::normalize()
; decoder-mode: arm
0035e8e0  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
0035e8e4  00 40 a0 e1                                      mov r4, r0
0035e8e8  00 00 90 e5                                      ldr r0, [r0]
0035e8ec  04 70 94 e5                                      ldr r7, [r4, #4]
0035e8f0  08 60 94 e5                                      ldr r6, [r4, #8]
0035e8f4  00 10 a0 e1                                      mov r1, r0
0035e8f8  1b c1 fe eb                                      bl #0x30ed6c
0035e8fc  07 10 a0 e1                                      mov r1, r7
0035e900  00 50 a0 e1                                      mov r5, r0
0035e904  07 00 a0 e1                                      mov r0, r7
0035e908  17 c1 fe eb                                      bl #0x30ed6c
0035e90c  00 10 a0 e1                                      mov r1, r0
0035e910  05 00 a0 e1                                      mov r0, r5
0035e914  a2 c0 fe eb                                      bl #0x30eba4
0035e918  06 10 a0 e1                                      mov r1, r6
0035e91c  00 50 a0 e1                                      mov r5, r0
0035e920  06 00 a0 e1                                      mov r0, r6
0035e924  10 c1 fe eb                                      bl #0x30ed6c
0035e928  00 10 a0 e1                                      mov r1, r0
0035e92c  05 00 a0 e1                                      mov r0, r5
0035e930  9b c0 fe eb                                      bl #0x30eba4
0035e934  00 10 a0 e3                                      mov r1, #0
0035e938  00 50 a0 e1                                      mov r5, r0
0035e93c  92 bd fe eb                                      bl #0x30df8c
0035e940  00 00 50 e3                                      cmp r0, #0
0035e944  11 00 00 1a                                      bne #0x35e990
0035e948  05 00 a0 e1                                      mov r0, r5
0035e94c  f4 bd fe eb                                      bl #0x30e124
0035e950  00 10 a0 e1                                      mov r1, r0
0035e954  fe 05 a0 e3                                      mov r0, #0x3f800000
0035e958  cd c0 fe eb                                      bl #0x30ec94
0035e95c  00 50 a0 e1                                      mov r5, r0
0035e960  00 10 a0 e1                                      mov r1, r0
0035e964  00 00 94 e5                                      ldr r0, [r4]
0035e968  ff c0 fe eb                                      bl #0x30ed6c
0035e96c  05 10 a0 e1                                      mov r1, r5
0035e970  00 00 84 e5                                      str r0, [r4]
0035e974  04 00 94 e5                                      ldr r0, [r4, #4]
0035e978  fb c0 fe eb                                      bl #0x30ed6c
0035e97c  05 10 a0 e1                                      mov r1, r5
0035e980  04 00 84 e5                                      str r0, [r4, #4]
0035e984  08 00 94 e5                                      ldr r0, [r4, #8]
0035e988  f7 c0 fe eb                                      bl #0x30ed6c
0035e98c  08 00 84 e5                                      str r0, [r4, #8]
0035e990  04 00 a0 e1                                      mov r0, r4
0035e994  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x0059a294, declared_size=80, range_size=80, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZNK6glitch4core8vector3dIfEdvERKS2_
; demangled: glitch::core::vector3d<float>::operator/(glitch::core::vector3d<float> const&) const
; decoder-mode: arm
0059a294  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
0059a298  01 50 a0 e1                                      mov r5, r1
0059a29c  00 40 a0 e1                                      mov r4, r0
0059a2a0  04 10 92 e5                                      ldr r1, [r2, #4]
0059a2a4  04 00 95 e5                                      ldr r0, [r5, #4]
0059a2a8  02 60 a0 e1                                      mov r6, r2
0059a2ac  78 d2 f5 eb                                      bl #0x30ec94
0059a2b0  08 10 96 e5                                      ldr r1, [r6, #8]
0059a2b4  00 80 a0 e1                                      mov r8, r0
0059a2b8  08 00 95 e5                                      ldr r0, [r5, #8]
0059a2bc  74 d2 f5 eb                                      bl #0x30ec94
0059a2c0  00 10 96 e5                                      ldr r1, [r6]
0059a2c4  00 70 a0 e1                                      mov r7, r0
0059a2c8  00 00 95 e5                                      ldr r0, [r5]
0059a2cc  70 d2 f5 eb                                      bl #0x30ec94
0059a2d0  04 80 84 e5                                      str r8, [r4, #4]
0059a2d4  00 00 84 e5                                      str r0, [r4]
0059a2d8  08 70 84 e5                                      str r7, [r4, #8]
0059a2dc  04 00 a0 e1                                      mov r0, r4
0059a2e0  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x005a24b4, declared_size=68, range_size=68, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZN6glitch4core8vector3dIfEdVERKS2_
; demangled: glitch::core::vector3d<float>::operator/=(glitch::core::vector3d<float> const&)
; decoder-mode: arm
005a24b4  70 40 2d e9                                      push {r4, r5, r6, lr}
005a24b8  00 40 a0 e1                                      mov r4, r0
005a24bc  01 50 a0 e1                                      mov r5, r1
005a24c0  00 00 90 e5                                      ldr r0, [r0]
005a24c4  00 10 91 e5                                      ldr r1, [r1]
005a24c8  f1 b1 f5 eb                                      bl #0x30ec94
005a24cc  00 00 84 e5                                      str r0, [r4]
005a24d0  04 10 95 e5                                      ldr r1, [r5, #4]
005a24d4  04 00 94 e5                                      ldr r0, [r4, #4]
005a24d8  ed b1 f5 eb                                      bl #0x30ec94
005a24dc  04 00 84 e5                                      str r0, [r4, #4]
005a24e0  08 10 95 e5                                      ldr r1, [r5, #8]
005a24e4  08 00 94 e5                                      ldr r0, [r4, #8]
005a24e8  e9 b1 f5 eb                                      bl #0x30ec94
005a24ec  08 00 84 e5                                      str r0, [r4, #8]
005a24f0  04 00 a0 e1                                      mov r0, r4
005a24f4  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x00630bfc, declared_size=252, range_size=252, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZN6glitch4core8vector3dIfE10rotateXYByEdRKS2_
; demangled: glitch::core::vector3d<float>::rotateXYBy(double, glitch::core::vector3d<float> const&)
; decoder-mode: arm
00630bfc  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
00630c00  03 10 a0 e1                                      mov r1, r3
00630c04  00 40 a0 e1                                      mov r4, r0
00630c08  46 3f 0d e3                                      movw r3, #0xdf46
00630c0c  02 00 a0 e1                                      mov r0, r2
00630c10  39 2d 09 e3                                      movw r2, #0x9d39
00630c14  52 22 4a e3                                      movt r2, #0xa252
00630c18  91 3f 43 e3                                      movt r3, #0x3f91
00630c1c  a4 77 f3 eb                                      bl #0x30eab4
00630c20  00 60 a0 e1                                      mov r6, r0
00630c24  01 70 a0 e1                                      mov r7, r1
00630c28  46 75 f3 eb                                      bl #0x30e148
00630c2c  9b 76 f3 eb                                      bl #0x30e6a0
00630c30  07 10 a0 e1                                      mov r1, r7
00630c34  00 a0 a0 e1                                      mov sl, r0
00630c38  06 00 a0 e1                                      mov r0, r6
00630c3c  ea 74 f3 eb                                      bl #0x30dfec
00630c40  96 76 f3 eb                                      bl #0x30e6a0
00630c44  20 50 9d e5                                      ldr r5, [sp, #0x20]
00630c48  00 90 a0 e1                                      mov sb, r0
00630c4c  00 00 94 e5                                      ldr r0, [r4]
00630c50  00 10 95 e5                                      ldr r1, [r5]
00630c54  d4 75 f3 eb                                      bl #0x30e3ac
00630c58  00 00 84 e5                                      str r0, [r4]
00630c5c  04 10 95 e5                                      ldr r1, [r5, #4]
00630c60  00 70 a0 e1                                      mov r7, r0
00630c64  04 00 94 e5                                      ldr r0, [r4, #4]
00630c68  cf 75 f3 eb                                      bl #0x30e3ac
00630c6c  07 10 a0 e1                                      mov r1, r7
00630c70  00 60 a0 e1                                      mov r6, r0
00630c74  04 00 84 e5                                      str r0, [r4, #4]
00630c78  0a 00 a0 e1                                      mov r0, sl
00630c7c  3a 78 f3 eb                                      bl #0x30ed6c
00630c80  06 10 a0 e1                                      mov r1, r6
00630c84  00 80 a0 e1                                      mov r8, r0
00630c88  09 00 a0 e1                                      mov r0, sb
00630c8c  36 78 f3 eb                                      bl #0x30ed6c
00630c90  00 10 a0 e1                                      mov r1, r0
00630c94  08 00 a0 e1                                      mov r0, r8
00630c98  c3 75 f3 eb                                      bl #0x30e3ac
00630c9c  07 10 a0 e1                                      mov r1, r7
00630ca0  00 80 a0 e1                                      mov r8, r0
00630ca4  09 00 a0 e1                                      mov r0, sb
00630ca8  2f 78 f3 eb                                      bl #0x30ed6c
00630cac  06 10 a0 e1                                      mov r1, r6
00630cb0  00 70 a0 e1                                      mov r7, r0
00630cb4  0a 00 a0 e1                                      mov r0, sl
00630cb8  2b 78 f3 eb                                      bl #0x30ed6c
00630cbc  00 10 a0 e1                                      mov r1, r0
00630cc0  07 00 a0 e1                                      mov r0, r7
00630cc4  b6 77 f3 eb                                      bl #0x30eba4
00630cc8  00 80 84 e5                                      str r8, [r4]
00630ccc  04 00 84 e5                                      str r0, [r4, #4]
00630cd0  00 10 95 e5                                      ldr r1, [r5]
00630cd4  00 60 a0 e1                                      mov r6, r0
00630cd8  08 00 a0 e1                                      mov r0, r8
00630cdc  b0 77 f3 eb                                      bl #0x30eba4
00630ce0  00 00 84 e5                                      str r0, [r4]
00630ce4  04 10 95 e5                                      ldr r1, [r5, #4]
00630ce8  06 00 a0 e1                                      mov r0, r6
00630cec  ac 77 f3 eb                                      bl #0x30eba4
00630cf0  04 00 84 e5                                      str r0, [r4, #4]
00630cf4  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}


; FUNCTION 0x00630cf8, declared_size=240, range_size=240, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZN6glitch4core8vector3dIfE10rotateYZByEdRKS2_
; demangled: glitch::core::vector3d<float>::rotateYZBy(double, glitch::core::vector3d<float> const&)
; decoder-mode: arm
00630cf8  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
00630cfc  03 10 a0 e1                                      mov r1, r3
00630d00  00 40 a0 e1                                      mov r4, r0
00630d04  46 3f 0d e3                                      movw r3, #0xdf46
00630d08  02 00 a0 e1                                      mov r0, r2
00630d0c  39 2d 09 e3                                      movw r2, #0x9d39
00630d10  52 22 4a e3                                      movt r2, #0xa252
00630d14  91 3f 43 e3                                      movt r3, #0x3f91
00630d18  65 77 f3 eb                                      bl #0x30eab4
00630d1c  00 60 a0 e1                                      mov r6, r0
00630d20  01 70 a0 e1                                      mov r7, r1
00630d24  07 75 f3 eb                                      bl #0x30e148
00630d28  5c 76 f3 eb                                      bl #0x30e6a0
00630d2c  07 10 a0 e1                                      mov r1, r7
00630d30  00 80 a0 e1                                      mov r8, r0
00630d34  06 00 a0 e1                                      mov r0, r6
00630d38  ab 74 f3 eb                                      bl #0x30dfec
00630d3c  57 76 f3 eb                                      bl #0x30e6a0
00630d40  20 50 9d e5                                      ldr r5, [sp, #0x20]
00630d44  00 90 a0 e1                                      mov sb, r0
00630d48  08 00 94 e5                                      ldr r0, [r4, #8]
00630d4c  08 10 95 e5                                      ldr r1, [r5, #8]
00630d50  95 75 f3 eb                                      bl #0x30e3ac
00630d54  08 00 84 e5                                      str r0, [r4, #8]
00630d58  04 10 95 e5                                      ldr r1, [r5, #4]
00630d5c  00 60 a0 e1                                      mov r6, r0
00630d60  04 00 94 e5                                      ldr r0, [r4, #4]
00630d64  90 75 f3 eb                                      bl #0x30e3ac
00630d68  00 a0 a0 e1                                      mov sl, r0
00630d6c  0a 10 a0 e1                                      mov r1, sl
00630d70  08 00 a0 e1                                      mov r0, r8
00630d74  fc 77 f3 eb                                      bl #0x30ed6c
00630d78  06 10 a0 e1                                      mov r1, r6
00630d7c  00 70 a0 e1                                      mov r7, r0
00630d80  09 00 a0 e1                                      mov r0, sb
00630d84  f8 77 f3 eb                                      bl #0x30ed6c
00630d88  00 10 a0 e1                                      mov r1, r0
00630d8c  07 00 a0 e1                                      mov r0, r7
00630d90  85 75 f3 eb                                      bl #0x30e3ac
00630d94  0a 10 a0 e1                                      mov r1, sl
00630d98  00 70 a0 e1                                      mov r7, r0
00630d9c  09 00 a0 e1                                      mov r0, sb
00630da0  f1 77 f3 eb                                      bl #0x30ed6c
00630da4  06 10 a0 e1                                      mov r1, r6
00630da8  00 a0 a0 e1                                      mov sl, r0
00630dac  08 00 a0 e1                                      mov r0, r8
00630db0  ed 77 f3 eb                                      bl #0x30ed6c
00630db4  00 10 a0 e1                                      mov r1, r0
00630db8  0a 00 a0 e1                                      mov r0, sl
00630dbc  78 77 f3 eb                                      bl #0x30eba4
00630dc0  04 70 84 e5                                      str r7, [r4, #4]
00630dc4  08 00 84 e5                                      str r0, [r4, #8]
00630dc8  08 10 95 e5                                      ldr r1, [r5, #8]
00630dcc  74 77 f3 eb                                      bl #0x30eba4
00630dd0  08 00 84 e5                                      str r0, [r4, #8]
00630dd4  04 10 95 e5                                      ldr r1, [r5, #4]
00630dd8  07 00 a0 e1                                      mov r0, r7
00630ddc  70 77 f3 eb                                      bl #0x30eba4
00630de0  04 00 84 e5                                      str r0, [r4, #4]
00630de4  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}


; FUNCTION 0x00630de8, declared_size=256, range_size=256, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZN6glitch4core8vector3dIfE10rotateXZByEdRKS2_
; demangled: glitch::core::vector3d<float>::rotateXZBy(double, glitch::core::vector3d<float> const&)
; decoder-mode: arm
00630de8  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
00630dec  03 10 a0 e1                                      mov r1, r3
00630df0  00 40 a0 e1                                      mov r4, r0
00630df4  46 3f 0d e3                                      movw r3, #0xdf46
00630df8  02 00 a0 e1                                      mov r0, r2
00630dfc  39 2d 09 e3                                      movw r2, #0x9d39
00630e00  52 22 4a e3                                      movt r2, #0xa252
00630e04  91 3f 43 e3                                      movt r3, #0x3f91
00630e08  29 77 f3 eb                                      bl #0x30eab4
00630e0c  00 60 a0 e1                                      mov r6, r0
00630e10  01 70 a0 e1                                      mov r7, r1
00630e14  cb 74 f3 eb                                      bl #0x30e148
00630e18  20 76 f3 eb                                      bl #0x30e6a0
00630e1c  07 10 a0 e1                                      mov r1, r7
00630e20  00 a0 a0 e1                                      mov sl, r0
00630e24  06 00 a0 e1                                      mov r0, r6
00630e28  6f 74 f3 eb                                      bl #0x30dfec
00630e2c  1b 76 f3 eb                                      bl #0x30e6a0
00630e30  20 50 9d e5                                      ldr r5, [sp, #0x20]
00630e34  00 90 a0 e1                                      mov sb, r0
00630e38  00 00 94 e5                                      ldr r0, [r4]
00630e3c  00 10 95 e5                                      ldr r1, [r5]
00630e40  59 75 f3 eb                                      bl #0x30e3ac
00630e44  00 00 84 e5                                      str r0, [r4]
00630e48  08 10 95 e5                                      ldr r1, [r5, #8]
00630e4c  00 70 a0 e1                                      mov r7, r0
00630e50  08 00 94 e5                                      ldr r0, [r4, #8]
00630e54  54 75 f3 eb                                      bl #0x30e3ac
00630e58  07 10 a0 e1                                      mov r1, r7
00630e5c  00 60 a0 e1                                      mov r6, r0
00630e60  08 00 84 e5                                      str r0, [r4, #8]
00630e64  0a 00 a0 e1                                      mov r0, sl
00630e68  bf 77 f3 eb                                      bl #0x30ed6c
00630e6c  06 10 a0 e1                                      mov r1, r6
00630e70  00 80 a0 e1                                      mov r8, r0
00630e74  09 00 a0 e1                                      mov r0, sb
00630e78  bb 77 f3 eb                                      bl #0x30ed6c
00630e7c  00 10 a0 e1                                      mov r1, r0
00630e80  08 00 a0 e1                                      mov r0, r8
00630e84  48 75 f3 eb                                      bl #0x30e3ac
00630e88  07 10 a0 e1                                      mov r1, r7
00630e8c  00 80 a0 e1                                      mov r8, r0
00630e90  09 00 a0 e1                                      mov r0, sb
00630e94  b4 77 f3 eb                                      bl #0x30ed6c
00630e98  06 10 a0 e1                                      mov r1, r6
00630e9c  00 90 a0 e1                                      mov sb, r0
00630ea0  0a 00 a0 e1                                      mov r0, sl
00630ea4  b0 77 f3 eb                                      bl #0x30ed6c
00630ea8  00 10 a0 e1                                      mov r1, r0
00630eac  09 00 a0 e1                                      mov r0, sb
00630eb0  3b 77 f3 eb                                      bl #0x30eba4
00630eb4  00 80 84 e5                                      str r8, [r4]
00630eb8  08 00 84 e5                                      str r0, [r4, #8]
00630ebc  00 10 95 e5                                      ldr r1, [r5]
00630ec0  00 60 a0 e1                                      mov r6, r0
00630ec4  08 00 a0 e1                                      mov r0, r8
00630ec8  35 77 f3 eb                                      bl #0x30eba4
00630ecc  00 00 84 e5                                      str r0, [r4]
00630ed0  08 10 95 e5                                      ldr r1, [r5, #8]
00630ed4  06 00 a0 e1                                      mov r0, r6
00630ed8  31 77 f3 eb                                      bl #0x30eba4
00630edc  04 70 94 e5                                      ldr r7, [r4, #4]
00630ee0  08 00 84 e5                                      str r0, [r4, #8]
00630ee4  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}


; FUNCTION 0x006c7b6c, declared_size=388, range_size=388, mode=arm
; class-group: glitch::core::vector3d<float>
; alias: _ZNK6glitch4core8vector3dIfE18getHorizontalAngleEv
; demangled: glitch::core::vector3d<float>::getHorizontalAngle() const
; decoder-mode: arm
006c7b6c  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
006c7b70  00 60 a0 e3                                      mov r6, #0
006c7b74  00 60 80 e5                                      str r6, [r0]
006c7b78  04 60 80 e5                                      str r6, [r0, #4]
006c7b7c  08 60 80 e5                                      str r6, [r0, #8]
006c7b80  01 50 a0 e1                                      mov r5, r1
006c7b84  00 40 a0 e1                                      mov r4, r0
006c7b88  08 10 91 e5                                      ldr r1, [r1, #8]
006c7b8c  00 00 95 e5                                      ldr r0, [r5]
006c7b90  88 18 f1 eb                                      bl #0x30ddb8
006c7b94  42 1b f1 eb                                      bl #0x30e8a4
006c7b98  f8 21 0c e3                                      movw r2, #0xc1f8
006c7b9c  dc 35 0a e3                                      movw r3, #0xa5dc
006c7ba0  63 2a 41 e3                                      movt r2, #0x1a63
006c7ba4  4c 30 44 e3                                      movt r3, #0x404c
006c7ba8  c1 1b f1 eb                                      bl #0x30eab4
006c7bac  bb 1a f1 eb                                      bl #0x30e6a0
006c7bb0  06 10 a0 e1                                      mov r1, r6
006c7bb4  04 00 84 e5                                      str r0, [r4, #4]
006c7bb8  00 70 a0 e1                                      mov r7, r0
006c7bbc  d2 1a f1 eb                                      bl #0x30e70c
006c7bc0  00 00 50 e3                                      cmp r0, #0
006c7bc4  05 00 00 0a                                      beq #0x6c7be0
006c7bc8  43 14 a0 e3                                      mov r1, #0x43000000
006c7bcc  07 00 a0 e1                                      mov r0, r7
006c7bd0  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7bd4  f2 1b f1 eb                                      bl #0x30eba4
006c7bd8  00 70 a0 e1                                      mov r7, r0
006c7bdc  04 00 84 e5                                      str r0, [r4, #4]
006c7be0  43 14 a0 e3                                      mov r1, #0x43000000
006c7be4  07 00 a0 e1                                      mov r0, r7
006c7be8  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7bec  30 1a f1 eb                                      bl #0x30e4b4
006c7bf0  00 00 50 e3                                      cmp r0, #0
006c7bf4  04 00 00 0a                                      beq #0x6c7c0c
006c7bf8  43 14 a0 e3                                      mov r1, #0x43000000
006c7bfc  07 00 a0 e1                                      mov r0, r7
006c7c00  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7c04  e8 19 f1 eb                                      bl #0x30e3ac
006c7c08  04 00 84 e5                                      str r0, [r4, #4]
006c7c0c  00 00 95 e5                                      ldr r0, [r5]
006c7c10  08 70 95 e5                                      ldr r7, [r5, #8]
006c7c14  00 10 a0 e1                                      mov r1, r0
006c7c18  53 1c f1 eb                                      bl #0x30ed6c
006c7c1c  07 10 a0 e1                                      mov r1, r7
006c7c20  00 60 a0 e1                                      mov r6, r0
006c7c24  07 00 a0 e1                                      mov r0, r7
006c7c28  4f 1c f1 eb                                      bl #0x30ed6c
006c7c2c  00 10 a0 e1                                      mov r1, r0
006c7c30  06 00 a0 e1                                      mov r0, r6
006c7c34  da 1b f1 eb                                      bl #0x30eba4
006c7c38  39 19 f1 eb                                      bl #0x30e124
006c7c3c  18 1b f1 eb                                      bl #0x30e8a4
006c7c40  00 60 a0 e1                                      mov r6, r0
006c7c44  04 00 95 e5                                      ldr r0, [r5, #4]
006c7c48  01 70 a0 e1                                      mov r7, r1
006c7c4c  14 1b f1 eb                                      bl #0x30e8a4
006c7c50  00 20 a0 e1                                      mov r2, r0
006c7c54  01 30 a0 e1                                      mov r3, r1
006c7c58  06 00 a0 e1                                      mov r0, r6
006c7c5c  07 10 a0 e1                                      mov r1, r7
006c7c60  ae 18 f1 eb                                      bl #0x30df20
006c7c64  f8 21 0c e3                                      movw r2, #0xc1f8
006c7c68  dc 35 0a e3                                      movw r3, #0xa5dc
006c7c6c  63 2a 41 e3                                      movt r2, #0x1a63
006c7c70  4c 30 44 e3                                      movt r3, #0x404c
006c7c74  8e 1b f1 eb                                      bl #0x30eab4
006c7c78  00 30 08 e3                                      movw r3, #0x8000
006c7c7c  00 20 a0 e3                                      mov r2, #0
006c7c80  56 30 44 e3                                      movt r3, #0x4056
006c7c84  28 1a f1 eb                                      bl #0x30e52c
006c7c88  84 1a f1 eb                                      bl #0x30e6a0
006c7c8c  00 10 a0 e3                                      mov r1, #0
006c7c90  00 00 84 e5                                      str r0, [r4]
006c7c94  00 50 a0 e1                                      mov r5, r0
006c7c98  9b 1a f1 eb                                      bl #0x30e70c
006c7c9c  00 00 50 e3                                      cmp r0, #0
006c7ca0  05 00 00 0a                                      beq #0x6c7cbc
006c7ca4  43 14 a0 e3                                      mov r1, #0x43000000
006c7ca8  05 00 a0 e1                                      mov r0, r5
006c7cac  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7cb0  bb 1b f1 eb                                      bl #0x30eba4
006c7cb4  00 50 a0 e1                                      mov r5, r0
006c7cb8  00 00 84 e5                                      str r0, [r4]
006c7cbc  43 14 a0 e3                                      mov r1, #0x43000000
006c7cc0  05 00 a0 e1                                      mov r0, r5
006c7cc4  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7cc8  f9 19 f1 eb                                      bl #0x30e4b4
006c7ccc  00 00 50 e3                                      cmp r0, #0
006c7cd0  04 00 00 0a                                      beq #0x6c7ce8
006c7cd4  43 14 a0 e3                                      mov r1, #0x43000000
006c7cd8  05 00 a0 e1                                      mov r0, r5
006c7cdc  2d 17 81 e2                                      add r1, r1, #0xb40000
006c7ce0  b1 19 f1 eb                                      bl #0x30e3ac
006c7ce4  00 00 84 e5                                      str r0, [r4]
006c7ce8  04 00 a0 e1                                      mov r0, r4
006c7cec  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}

; FUNCTION 0x0035c8f0, declared_size=232, range_size=232, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternion9normalizeEv
; demangled: glitch::core::quaternion::normalize()
; decoder-mode: arm
0035c8f0  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
0035c8f4  00 40 a0 e1                                      mov r4, r0
0035c8f8  00 00 90 e5                                      ldr r0, [r0]
0035c8fc  04 70 94 e5                                      ldr r7, [r4, #4]
0035c900  08 60 94 e5                                      ldr r6, [r4, #8]
0035c904  00 10 a0 e1                                      mov r1, r0
0035c908  17 c9 fe eb                                      bl #0x30ed6c
0035c90c  07 10 a0 e1                                      mov r1, r7
0035c910  00 50 a0 e1                                      mov r5, r0
0035c914  07 00 a0 e1                                      mov r0, r7
0035c918  13 c9 fe eb                                      bl #0x30ed6c
0035c91c  00 10 a0 e1                                      mov r1, r0
0035c920  05 00 a0 e1                                      mov r0, r5
0035c924  9e c8 fe eb                                      bl #0x30eba4
0035c928  06 10 a0 e1                                      mov r1, r6
0035c92c  00 50 a0 e1                                      mov r5, r0
0035c930  06 00 a0 e1                                      mov r0, r6
0035c934  0c c9 fe eb                                      bl #0x30ed6c
0035c938  00 10 a0 e1                                      mov r1, r0
0035c93c  05 00 a0 e1                                      mov r0, r5
0035c940  97 c8 fe eb                                      bl #0x30eba4
0035c944  0c 60 94 e5                                      ldr r6, [r4, #0xc]
0035c948  00 50 a0 e1                                      mov r5, r0
0035c94c  06 10 a0 e1                                      mov r1, r6
0035c950  06 00 a0 e1                                      mov r0, r6
0035c954  04 c9 fe eb                                      bl #0x30ed6c
0035c958  00 10 a0 e1                                      mov r1, r0
0035c95c  05 00 a0 e1                                      mov r0, r5
0035c960  8f c8 fe eb                                      bl #0x30eba4
0035c964  fe 15 a0 e3                                      mov r1, #0x3f800000
0035c968  00 50 a0 e1                                      mov r5, r0
0035c96c  86 c5 fe eb                                      bl #0x30df8c
0035c970  00 00 50 e3                                      cmp r0, #0
0035c974  15 00 00 1a                                      bne #0x35c9d0
0035c978  05 00 a0 e1                                      mov r0, r5
0035c97c  e8 c5 fe eb                                      bl #0x30e124
0035c980  00 10 a0 e1                                      mov r1, r0
0035c984  fe 05 a0 e3                                      mov r0, #0x3f800000
0035c988  c1 c8 fe eb                                      bl #0x30ec94
0035c98c  00 50 a0 e1                                      mov r5, r0
0035c990  00 10 a0 e1                                      mov r1, r0
0035c994  00 00 94 e5                                      ldr r0, [r4]
0035c998  f3 c8 fe eb                                      bl #0x30ed6c
0035c99c  05 10 a0 e1                                      mov r1, r5
0035c9a0  00 00 84 e5                                      str r0, [r4]
0035c9a4  04 00 94 e5                                      ldr r0, [r4, #4]
0035c9a8  ef c8 fe eb                                      bl #0x30ed6c
0035c9ac  05 10 a0 e1                                      mov r1, r5
0035c9b0  04 00 84 e5                                      str r0, [r4, #4]
0035c9b4  08 00 94 e5                                      ldr r0, [r4, #8]
0035c9b8  eb c8 fe eb                                      bl #0x30ed6c
0035c9bc  05 10 a0 e1                                      mov r1, r5
0035c9c0  08 00 84 e5                                      str r0, [r4, #8]
0035c9c4  0c 00 94 e5                                      ldr r0, [r4, #0xc]
0035c9c8  e7 c8 fe eb                                      bl #0x30ed6c
0035c9cc  0c 00 84 e5                                      str r0, [r4, #0xc]
0035c9d0  04 00 a0 e1                                      mov r0, r4
0035c9d4  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x0035c9d8, declared_size=564, range_size=564, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternion3setEfff
; demangled: glitch::core::quaternion::set(float, float, float)
; decoder-mode: arm
0035c9d8  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035c9dc  00 40 a0 e1                                      mov r4, r0
0035c9e0  34 d0 4d e2                                      sub sp, sp, #0x34
0035c9e4  01 00 a0 e1                                      mov r0, r1
0035c9e8  02 50 a0 e1                                      mov r5, r2
0035c9ec  03 a0 a0 e1                                      mov sl, r3
0035c9f0  ab c7 fe eb                                      bl #0x30e8a4
0035c9f4  ff 35 a0 e3                                      mov r3, #0x3fc00000
0035c9f8  00 20 a0 e3                                      mov r2, #0
0035c9fc  02 36 83 e2                                      add r3, r3, #0x200000
0035ca00  2b c8 fe eb                                      bl #0x30eab4
0035ca04  00 60 a0 e1                                      mov r6, r0
0035ca08  01 70 a0 e1                                      mov r7, r1
0035ca0c  76 c5 fe eb                                      bl #0x30dfec
0035ca10  00 80 a0 e1                                      mov r8, r0
0035ca14  01 90 a0 e1                                      mov sb, r1
0035ca18  06 00 a0 e1                                      mov r0, r6
0035ca1c  07 10 a0 e1                                      mov r1, r7
0035ca20  c8 c5 fe eb                                      bl #0x30e148
0035ca24  00 60 a0 e1                                      mov r6, r0
0035ca28  05 00 a0 e1                                      mov r0, r5
0035ca2c  01 70 a0 e1                                      mov r7, r1
0035ca30  9b c7 fe eb                                      bl #0x30e8a4
0035ca34  ff 35 a0 e3                                      mov r3, #0x3fc00000
0035ca38  00 20 a0 e3                                      mov r2, #0
0035ca3c  02 36 83 e2                                      add r3, r3, #0x200000
0035ca40  1b c8 fe eb                                      bl #0x30eab4
0035ca44  04 00 8d e5                                      str r0, [sp, #4]
0035ca48  00 10 8d e5                                      str r1, [sp]
0035ca4c  66 c5 fe eb                                      bl #0x30dfec
0035ca50  04 20 9d e5                                      ldr r2, [sp, #4]
0035ca54  00 30 9d e5                                      ldr r3, [sp]
0035ca58  f8 01 cd e1                                      strd r0, r1, [sp, #0x18]
0035ca5c  02 00 a0 e1                                      mov r0, r2
0035ca60  03 10 a0 e1                                      mov r1, r3
0035ca64  b7 c5 fe eb                                      bl #0x30e148
0035ca68  f8 00 cd e1                                      strd r0, r1, [sp, #8]
0035ca6c  0a 00 a0 e1                                      mov r0, sl
0035ca70  8b c7 fe eb                                      bl #0x30e8a4
0035ca74  ff 35 a0 e3                                      mov r3, #0x3fc00000
0035ca78  00 20 a0 e3                                      mov r2, #0
0035ca7c  02 36 83 e2                                      add r3, r3, #0x200000
0035ca80  0b c8 fe eb                                      bl #0x30eab4
0035ca84  00 a0 a0 e1                                      mov sl, r0
0035ca88  01 b0 a0 e1                                      mov fp, r1
0035ca8c  56 c5 fe eb                                      bl #0x30dfec
0035ca90  f0 01 cd e1                                      strd r0, r1, [sp, #0x10]
0035ca94  0a 00 a0 e1                                      mov r0, sl
0035ca98  0b 10 a0 e1                                      mov r1, fp
0035ca9c  a9 c5 fe eb                                      bl #0x30e148
0035caa0  00 a0 a0 e1                                      mov sl, r0
0035caa4  01 b0 a0 e1                                      mov fp, r1
0035caa8  0a 20 a0 e1                                      mov r2, sl
0035caac  d8 00 cd e1                                      ldrd r0, r1, [sp, #8]
0035cab0  0b 30 a0 e1                                      mov r3, fp
0035cab4  fe c7 fe eb                                      bl #0x30eab4
0035cab8  0a 20 a0 e1                                      mov r2, sl
0035cabc  f0 02 cd e1                                      strd r0, r1, [sp, #0x20]
0035cac0  d8 01 cd e1                                      ldrd r0, r1, [sp, #0x18]
0035cac4  0b 30 a0 e1                                      mov r3, fp
0035cac8  f9 c7 fe eb                                      bl #0x30eab4
0035cacc  d0 21 cd e1                                      ldrd r2, r3, [sp, #0x10]
0035cad0  f8 02 cd e1                                      strd r0, r1, [sp, #0x28]
0035cad4  d8 00 cd e1                                      ldrd r0, r1, [sp, #8]
0035cad8  f5 c7 fe eb                                      bl #0x30eab4
0035cadc  d0 21 cd e1                                      ldrd r2, r3, [sp, #0x10]
0035cae0  00 a0 a0 e1                                      mov sl, r0
0035cae4  01 b0 a0 e1                                      mov fp, r1
0035cae8  d8 01 cd e1                                      ldrd r0, r1, [sp, #0x18]
0035caec  f0 c7 fe eb                                      bl #0x30eab4
0035caf0  d0 22 cd e1                                      ldrd r2, r3, [sp, #0x20]
0035caf4  f0 01 cd e1                                      strd r0, r1, [sp, #0x10]
0035caf8  08 00 a0 e1                                      mov r0, r8
0035cafc  09 10 a0 e1                                      mov r1, sb
0035cb00  eb c7 fe eb                                      bl #0x30eab4
0035cb04  d0 21 cd e1                                      ldrd r2, r3, [sp, #0x10]
0035cb08  f8 00 cd e1                                      strd r0, r1, [sp, #8]
0035cb0c  06 00 a0 e1                                      mov r0, r6
0035cb10  07 10 a0 e1                                      mov r1, r7
0035cb14  e6 c7 fe eb                                      bl #0x30eab4
0035cb18  00 20 a0 e1                                      mov r2, r0
0035cb1c  01 30 a0 e1                                      mov r3, r1
0035cb20  d8 00 cd e1                                      ldrd r0, r1, [sp, #8]
0035cb24  80 c6 fe eb                                      bl #0x30e52c
0035cb28  dc c6 fe eb                                      bl #0x30e6a0
0035cb2c  00 00 84 e5                                      str r0, [r4]
0035cb30  d8 22 cd e1                                      ldrd r2, r3, [sp, #0x28]
0035cb34  06 00 a0 e1                                      mov r0, r6
0035cb38  07 10 a0 e1                                      mov r1, r7
0035cb3c  dc c7 fe eb                                      bl #0x30eab4
0035cb40  0a 20 a0 e1                                      mov r2, sl
0035cb44  f8 00 cd e1                                      strd r0, r1, [sp, #8]
0035cb48  0b 30 a0 e1                                      mov r3, fp
0035cb4c  08 00 a0 e1                                      mov r0, r8
0035cb50  09 10 a0 e1                                      mov r1, sb
0035cb54  d6 c7 fe eb                                      bl #0x30eab4
0035cb58  00 20 a0 e1                                      mov r2, r0
0035cb5c  01 30 a0 e1                                      mov r3, r1
0035cb60  d8 00 cd e1                                      ldrd r0, r1, [sp, #8]
0035cb64  f6 c7 fe eb                                      bl #0x30eb44
0035cb68  cc c6 fe eb                                      bl #0x30e6a0
0035cb6c  0a 20 a0 e1                                      mov r2, sl
0035cb70  04 00 84 e5                                      str r0, [r4, #4]
0035cb74  0b 30 a0 e1                                      mov r3, fp
0035cb78  06 00 a0 e1                                      mov r0, r6
0035cb7c  07 10 a0 e1                                      mov r1, r7
0035cb80  cb c7 fe eb                                      bl #0x30eab4
0035cb84  d8 22 cd e1                                      ldrd r2, r3, [sp, #0x28]
0035cb88  00 a0 a0 e1                                      mov sl, r0
0035cb8c  01 b0 a0 e1                                      mov fp, r1
0035cb90  08 00 a0 e1                                      mov r0, r8
0035cb94  09 10 a0 e1                                      mov r1, sb
0035cb98  c5 c7 fe eb                                      bl #0x30eab4
0035cb9c  00 20 a0 e1                                      mov r2, r0
0035cba0  01 30 a0 e1                                      mov r3, r1
0035cba4  0a 00 a0 e1                                      mov r0, sl
0035cba8  0b 10 a0 e1                                      mov r1, fp
0035cbac  5e c6 fe eb                                      bl #0x30e52c
0035cbb0  ba c6 fe eb                                      bl #0x30e6a0
0035cbb4  08 00 84 e5                                      str r0, [r4, #8]
0035cbb8  d0 22 cd e1                                      ldrd r2, r3, [sp, #0x20]
0035cbbc  06 00 a0 e1                                      mov r0, r6
0035cbc0  07 10 a0 e1                                      mov r1, r7
0035cbc4  ba c7 fe eb                                      bl #0x30eab4
0035cbc8  d0 21 cd e1                                      ldrd r2, r3, [sp, #0x10]
0035cbcc  00 60 a0 e1                                      mov r6, r0
0035cbd0  01 70 a0 e1                                      mov r7, r1
0035cbd4  08 00 a0 e1                                      mov r0, r8
0035cbd8  09 10 a0 e1                                      mov r1, sb
0035cbdc  b4 c7 fe eb                                      bl #0x30eab4
0035cbe0  00 20 a0 e1                                      mov r2, r0
0035cbe4  01 30 a0 e1                                      mov r3, r1
0035cbe8  06 00 a0 e1                                      mov r0, r6
0035cbec  07 10 a0 e1                                      mov r1, r7
0035cbf0  d3 c7 fe eb                                      bl #0x30eb44
0035cbf4  a9 c6 fe eb                                      bl #0x30e6a0
0035cbf8  0c 00 84 e5                                      str r0, [r4, #0xc]
0035cbfc  04 00 a0 e1                                      mov r0, r4
0035cc00  34 d0 8d e2                                      add sp, sp, #0x34
0035cc04  f0 4f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035cc08  38 ff ff ea                                      b #0x35c8f0


; FUNCTION 0x0060cdbc, declared_size=104, range_size=104, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternion13fromAngleAxisEfRKNS0_8vector3dIfEE
; demangled: glitch::core::quaternion::fromAngleAxis(float, glitch::core::vector3d<float> const&)
; decoder-mode: arm
0060cdbc  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
0060cdc0  00 40 a0 e1                                      mov r4, r0
0060cdc4  01 00 a0 e1                                      mov r0, r1
0060cdc8  3f 14 a0 e3                                      mov r1, #0x3f000000
0060cdcc  02 60 a0 e1                                      mov r6, r2
0060cdd0  e5 07 f4 eb                                      bl #0x30ed6c
0060cdd4  00 70 a0 e1                                      mov r7, r0
0060cdd8  4a 07 f4 eb                                      bl #0x30eb08
0060cddc  00 50 a0 e1                                      mov r5, r0
0060cde0  07 00 a0 e1                                      mov r0, r7
0060cde4  5a 06 f4 eb                                      bl #0x30e754
0060cde8  0c 00 84 e5                                      str r0, [r4, #0xc]
0060cdec  00 00 96 e5                                      ldr r0, [r6]
0060cdf0  05 10 a0 e1                                      mov r1, r5
0060cdf4  dc 07 f4 eb                                      bl #0x30ed6c
0060cdf8  00 00 84 e5                                      str r0, [r4]
0060cdfc  04 00 96 e5                                      ldr r0, [r6, #4]
0060ce00  05 10 a0 e1                                      mov r1, r5
0060ce04  d8 07 f4 eb                                      bl #0x30ed6c
0060ce08  04 00 84 e5                                      str r0, [r4, #4]
0060ce0c  08 00 96 e5                                      ldr r0, [r6, #8]
0060ce10  05 10 a0 e1                                      mov r1, r5
0060ce14  d4 07 f4 eb                                      bl #0x30ed6c
0060ce18  08 00 84 e5                                      str r0, [r4, #8]
0060ce1c  04 00 a0 e1                                      mov r0, r4
0060ce20  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x00410e48, declared_size=92, range_size=92, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternion13fromAngleAxisEfRKNS0_8vector3dIfEE.clone.1
; demangled: glitch::core::quaternion::fromAngleAxis(float, glitch::core::vector3d<float> const&) [clone .clone.1]
; decoder-mode: arm
00410e48  70 40 2d e9                                      push {r4, r5, r6, lr}
00410e4c  80 3b 07 e3                                      movw r3, #0x7b80
00410e50  06 3f 43 e3                                      movt r3, #0x3f06
00410e54  0c 30 80 e5                                      str r3, [r0, #0xc]
00410e58  00 40 a0 e1                                      mov r4, r0
00410e5c  01 50 a0 e1                                      mov r5, r1
00410e60  00 00 91 e5                                      ldr r0, [r1]
00410e64  d0 14 0d e3                                      movw r1, #0xd4d0
00410e68  59 1f 43 e3                                      movt r1, #0x3f59
00410e6c  be f7 fb eb                                      bl #0x30ed6c
00410e70  d0 14 0d e3                                      movw r1, #0xd4d0
00410e74  00 00 84 e5                                      str r0, [r4]
00410e78  04 00 95 e5                                      ldr r0, [r5, #4]
00410e7c  59 1f 43 e3                                      movt r1, #0x3f59
00410e80  b9 f7 fb eb                                      bl #0x30ed6c
00410e84  d0 14 0d e3                                      movw r1, #0xd4d0
00410e88  04 00 84 e5                                      str r0, [r4, #4]
00410e8c  08 00 95 e5                                      ldr r0, [r5, #8]
00410e90  59 1f 43 e3                                      movt r1, #0x3f59
00410e94  b4 f7 fb eb                                      bl #0x30ed6c
00410e98  08 00 84 e5                                      str r0, [r4, #8]
00410e9c  04 00 a0 e1                                      mov r0, r4
00410ea0  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0060dd34, declared_size=448, range_size=448, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternionmlERKS1_
; demangled: glitch::core::quaternion::operator*(glitch::core::quaternion const&) const
; decoder-mode: arm
0060dd34  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
0060dd38  00 30 a0 e3                                      mov r3, #0
0060dd3c  08 30 80 e5                                      str r3, [r0, #8]
0060dd40  00 40 a0 e1                                      mov r4, r0
0060dd44  fe 05 a0 e3                                      mov r0, #0x3f800000
0060dd48  00 30 84 e5                                      str r3, [r4]
0060dd4c  04 30 84 e5                                      str r3, [r4, #4]
0060dd50  01 50 a0 e1                                      mov r5, r1
0060dd54  0c 00 84 e5                                      str r0, [r4, #0xc]
0060dd58  0c 10 92 e5                                      ldr r1, [r2, #0xc]
0060dd5c  0c 00 95 e5                                      ldr r0, [r5, #0xc]
0060dd60  02 60 a0 e1                                      mov r6, r2
0060dd64  00 04 f4 eb                                      bl #0x30ed6c
0060dd68  00 10 96 e5                                      ldr r1, [r6]
0060dd6c  00 70 a0 e1                                      mov r7, r0
0060dd70  00 00 95 e5                                      ldr r0, [r5]
0060dd74  fc 03 f4 eb                                      bl #0x30ed6c
0060dd78  00 10 a0 e1                                      mov r1, r0
0060dd7c  07 00 a0 e1                                      mov r0, r7
0060dd80  89 01 f4 eb                                      bl #0x30e3ac
0060dd84  04 10 96 e5                                      ldr r1, [r6, #4]
0060dd88  00 70 a0 e1                                      mov r7, r0
0060dd8c  04 00 95 e5                                      ldr r0, [r5, #4]
0060dd90  f5 03 f4 eb                                      bl #0x30ed6c
0060dd94  00 10 a0 e1                                      mov r1, r0
0060dd98  07 00 a0 e1                                      mov r0, r7
0060dd9c  82 01 f4 eb                                      bl #0x30e3ac
0060dda0  08 10 96 e5                                      ldr r1, [r6, #8]
0060dda4  00 70 a0 e1                                      mov r7, r0
0060dda8  08 00 95 e5                                      ldr r0, [r5, #8]
0060ddac  ee 03 f4 eb                                      bl #0x30ed6c
0060ddb0  00 10 a0 e1                                      mov r1, r0
0060ddb4  07 00 a0 e1                                      mov r0, r7
0060ddb8  7b 01 f4 eb                                      bl #0x30e3ac
0060ddbc  0c 00 84 e5                                      str r0, [r4, #0xc]
0060ddc0  0c 10 96 e5                                      ldr r1, [r6, #0xc]
0060ddc4  00 00 95 e5                                      ldr r0, [r5]
0060ddc8  e7 03 f4 eb                                      bl #0x30ed6c
0060ddcc  00 10 96 e5                                      ldr r1, [r6]
0060ddd0  00 70 a0 e1                                      mov r7, r0
0060ddd4  0c 00 95 e5                                      ldr r0, [r5, #0xc]
0060ddd8  e3 03 f4 eb                                      bl #0x30ed6c
0060dddc  00 10 a0 e1                                      mov r1, r0
0060dde0  07 00 a0 e1                                      mov r0, r7
0060dde4  6e 03 f4 eb                                      bl #0x30eba4
0060dde8  04 10 96 e5                                      ldr r1, [r6, #4]
0060ddec  00 70 a0 e1                                      mov r7, r0
0060ddf0  08 00 95 e5                                      ldr r0, [r5, #8]
0060ddf4  dc 03 f4 eb                                      bl #0x30ed6c
0060ddf8  00 10 a0 e1                                      mov r1, r0
0060ddfc  07 00 a0 e1                                      mov r0, r7
0060de00  67 03 f4 eb                                      bl #0x30eba4
0060de04  08 10 96 e5                                      ldr r1, [r6, #8]
0060de08  00 70 a0 e1                                      mov r7, r0
0060de0c  04 00 95 e5                                      ldr r0, [r5, #4]
0060de10  d5 03 f4 eb                                      bl #0x30ed6c
0060de14  00 10 a0 e1                                      mov r1, r0
0060de18  07 00 a0 e1                                      mov r0, r7
0060de1c  62 01 f4 eb                                      bl #0x30e3ac
0060de20  00 00 84 e5                                      str r0, [r4]
0060de24  0c 10 96 e5                                      ldr r1, [r6, #0xc]
0060de28  04 00 95 e5                                      ldr r0, [r5, #4]
0060de2c  ce 03 f4 eb                                      bl #0x30ed6c
0060de30  04 10 96 e5                                      ldr r1, [r6, #4]
0060de34  00 70 a0 e1                                      mov r7, r0
0060de38  0c 00 95 e5                                      ldr r0, [r5, #0xc]
0060de3c  ca 03 f4 eb                                      bl #0x30ed6c
0060de40  00 10 a0 e1                                      mov r1, r0
0060de44  07 00 a0 e1                                      mov r0, r7
0060de48  55 03 f4 eb                                      bl #0x30eba4
0060de4c  08 10 96 e5                                      ldr r1, [r6, #8]
0060de50  00 70 a0 e1                                      mov r7, r0
0060de54  00 00 95 e5                                      ldr r0, [r5]
0060de58  c3 03 f4 eb                                      bl #0x30ed6c
0060de5c  00 10 a0 e1                                      mov r1, r0
0060de60  07 00 a0 e1                                      mov r0, r7
0060de64  4e 03 f4 eb                                      bl #0x30eba4
0060de68  00 10 96 e5                                      ldr r1, [r6]
0060de6c  00 70 a0 e1                                      mov r7, r0
0060de70  08 00 95 e5                                      ldr r0, [r5, #8]
0060de74  bc 03 f4 eb                                      bl #0x30ed6c
0060de78  00 10 a0 e1                                      mov r1, r0
0060de7c  07 00 a0 e1                                      mov r0, r7
0060de80  49 01 f4 eb                                      bl #0x30e3ac
0060de84  04 00 84 e5                                      str r0, [r4, #4]
0060de88  0c 10 96 e5                                      ldr r1, [r6, #0xc]
0060de8c  08 00 95 e5                                      ldr r0, [r5, #8]
0060de90  b5 03 f4 eb                                      bl #0x30ed6c
0060de94  08 10 96 e5                                      ldr r1, [r6, #8]
0060de98  00 70 a0 e1                                      mov r7, r0
0060de9c  0c 00 95 e5                                      ldr r0, [r5, #0xc]
0060dea0  b1 03 f4 eb                                      bl #0x30ed6c
0060dea4  00 10 a0 e1                                      mov r1, r0
0060dea8  07 00 a0 e1                                      mov r0, r7
0060deac  3c 03 f4 eb                                      bl #0x30eba4
0060deb0  00 10 96 e5                                      ldr r1, [r6]
0060deb4  00 70 a0 e1                                      mov r7, r0
0060deb8  04 00 95 e5                                      ldr r0, [r5, #4]
0060debc  aa 03 f4 eb                                      bl #0x30ed6c
0060dec0  00 10 a0 e1                                      mov r1, r0
0060dec4  07 00 a0 e1                                      mov r0, r7
0060dec8  35 03 f4 eb                                      bl #0x30eba4
0060decc  04 10 96 e5                                      ldr r1, [r6, #4]
0060ded0  00 70 a0 e1                                      mov r7, r0
0060ded4  00 00 95 e5                                      ldr r0, [r5]
0060ded8  a3 03 f4 eb                                      bl #0x30ed6c
0060dedc  00 10 a0 e1                                      mov r1, r0
0060dee0  07 00 a0 e1                                      mov r0, r7
0060dee4  30 01 f4 eb                                      bl #0x30e3ac
0060dee8  08 00 84 e5                                      str r0, [r4, #8]
0060deec  04 00 a0 e1                                      mov r0, r4
0060def0  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x0035bc90, declared_size=532, range_size=532, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternionmlERKNS0_8vector3dIfEE
; demangled: glitch::core::quaternion::operator*(glitch::core::vector3d<float> const&) const
; decoder-mode: arm
0035bc90  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035bc94  04 60 91 e5                                      ldr r6, [r1, #4]
0035bc98  08 30 92 e5                                      ldr r3, [r2, #8]
0035bc9c  1c d0 4d e2                                      sub sp, sp, #0x1c
0035bca0  02 c1 86 e2                                      add ip, r6, #0x80000000
0035bca4  08 b0 91 e5                                      ldr fp, [r1, #8]
0035bca8  00 50 a0 e1                                      mov r5, r0
0035bcac  01 40 a0 e1                                      mov r4, r1
0035bcb0  0c 00 a0 e1                                      mov r0, ip
0035bcb4  03 10 a0 e1                                      mov r1, r3
0035bcb8  04 80 92 e5                                      ldr r8, [r2, #4]
0035bcbc  02 70 a0 e1                                      mov r7, r2
0035bcc0  08 c0 8d e5                                      str ip, [sp, #8]
0035bcc4  00 30 8d e5                                      str r3, [sp]
0035bcc8  27 cc fe eb                                      bl #0x30ed6c
0035bccc  08 10 a0 e1                                      mov r1, r8
0035bcd0  00 a0 a0 e1                                      mov sl, r0
0035bcd4  0b 00 a0 e1                                      mov r0, fp
0035bcd8  23 cc fe eb                                      bl #0x30ed6c
0035bcdc  00 10 a0 e1                                      mov r1, r0
0035bce0  0a 00 a0 e1                                      mov r0, sl
0035bce4  ae cb fe eb                                      bl #0x30eba4
0035bce8  10 00 8d e5                                      str r0, [sp, #0x10]
0035bcec  00 90 97 e5                                      ldr sb, [r7]
0035bcf0  02 21 8b e2                                      add r2, fp, #0x80000000
0035bcf4  02 00 a0 e1                                      mov r0, r2
0035bcf8  09 10 a0 e1                                      mov r1, sb
0035bcfc  00 a0 94 e5                                      ldr sl, [r4]
0035bd00  04 20 8d e5                                      str r2, [sp, #4]
0035bd04  18 cc fe eb                                      bl #0x30ed6c
0035bd08  00 30 9d e5                                      ldr r3, [sp]
0035bd0c  00 70 a0 e1                                      mov r7, r0
0035bd10  0a 00 a0 e1                                      mov r0, sl
0035bd14  03 10 a0 e1                                      mov r1, r3
0035bd18  13 cc fe eb                                      bl #0x30ed6c
0035bd1c  02 e1 8a e2                                      add lr, sl, #0x80000000
0035bd20  00 10 a0 e1                                      mov r1, r0
0035bd24  07 00 a0 e1                                      mov r0, r7
0035bd28  14 e0 8d e5                                      str lr, [sp, #0x14]
0035bd2c  9c cb fe eb                                      bl #0x30eba4
0035bd30  14 10 9d e5                                      ldr r1, [sp, #0x14]
0035bd34  00 70 a0 e1                                      mov r7, r0
0035bd38  08 00 a0 e1                                      mov r0, r8
0035bd3c  0a cc fe eb                                      bl #0x30ed6c
0035bd40  09 10 a0 e1                                      mov r1, sb
0035bd44  0c 00 8d e5                                      str r0, [sp, #0xc]
0035bd48  06 00 a0 e1                                      mov r0, r6
0035bd4c  06 cc fe eb                                      bl #0x30ed6c
0035bd50  00 10 a0 e1                                      mov r1, r0
0035bd54  0c 00 9d e5                                      ldr r0, [sp, #0xc]
0035bd58  91 cb fe eb                                      bl #0x30eba4
0035bd5c  0c 00 8d e5                                      str r0, [sp, #0xc]
0035bd60  0c 00 94 e5                                      ldr r0, [r4, #0xc]
0035bd64  00 10 a0 e1                                      mov r1, r0
0035bd68  8d cb fe eb                                      bl #0x30eba4
0035bd6c  08 c0 9d e5                                      ldr ip, [sp, #8]
0035bd70  00 40 a0 e1                                      mov r4, r0
0035bd74  0c 10 9d e5                                      ldr r1, [sp, #0xc]
0035bd78  0c 00 a0 e1                                      mov r0, ip
0035bd7c  fa cb fe eb                                      bl #0x30ed6c
0035bd80  07 10 a0 e1                                      mov r1, r7
0035bd84  00 c0 a0 e1                                      mov ip, r0
0035bd88  0b 00 a0 e1                                      mov r0, fp
0035bd8c  08 c0 8d e5                                      str ip, [sp, #8]
0035bd90  f5 cb fe eb                                      bl #0x30ed6c
0035bd94  08 c0 9d e5                                      ldr ip, [sp, #8]
0035bd98  00 10 a0 e1                                      mov r1, r0
0035bd9c  0c 00 a0 e1                                      mov r0, ip
0035bda0  7f cb fe eb                                      bl #0x30eba4
0035bda4  00 10 a0 e1                                      mov r1, r0
0035bda8  7d cb fe eb                                      bl #0x30eba4
0035bdac  10 10 9d e5                                      ldr r1, [sp, #0x10]
0035bdb0  00 b0 a0 e1                                      mov fp, r0
0035bdb4  04 00 a0 e1                                      mov r0, r4
0035bdb8  eb cb fe eb                                      bl #0x30ed6c
0035bdbc  00 10 a0 e1                                      mov r1, r0
0035bdc0  09 00 a0 e1                                      mov r0, sb
0035bdc4  76 cb fe eb                                      bl #0x30eba4
0035bdc8  00 10 a0 e1                                      mov r1, r0
0035bdcc  0b 00 a0 e1                                      mov r0, fp
0035bdd0  73 cb fe eb                                      bl #0x30eba4
0035bdd4  00 00 85 e5                                      str r0, [r5]
0035bdd8  04 20 9d e5                                      ldr r2, [sp, #4]
0035bddc  10 00 9d e5                                      ldr r0, [sp, #0x10]
0035bde0  02 10 a0 e1                                      mov r1, r2
0035bde4  e0 cb fe eb                                      bl #0x30ed6c
0035bde8  0c 10 9d e5                                      ldr r1, [sp, #0xc]
0035bdec  00 90 a0 e1                                      mov sb, r0
0035bdf0  0a 00 a0 e1                                      mov r0, sl
0035bdf4  dc cb fe eb                                      bl #0x30ed6c
0035bdf8  00 10 a0 e1                                      mov r1, r0
0035bdfc  09 00 a0 e1                                      mov r0, sb
0035be00  67 cb fe eb                                      bl #0x30eba4
0035be04  00 10 a0 e1                                      mov r1, r0
0035be08  65 cb fe eb                                      bl #0x30eba4
0035be0c  07 10 a0 e1                                      mov r1, r7
0035be10  00 a0 a0 e1                                      mov sl, r0
0035be14  04 00 a0 e1                                      mov r0, r4
0035be18  d3 cb fe eb                                      bl #0x30ed6c
0035be1c  00 10 a0 e1                                      mov r1, r0
0035be20  08 00 a0 e1                                      mov r0, r8
0035be24  5e cb fe eb                                      bl #0x30eba4
0035be28  00 10 a0 e1                                      mov r1, r0
0035be2c  0a 00 a0 e1                                      mov r0, sl
0035be30  5b cb fe eb                                      bl #0x30eba4
0035be34  04 00 85 e5                                      str r0, [r5, #4]
0035be38  14 10 9d e5                                      ldr r1, [sp, #0x14]
0035be3c  07 00 a0 e1                                      mov r0, r7
0035be40  c9 cb fe eb                                      bl #0x30ed6c
0035be44  10 10 9d e5                                      ldr r1, [sp, #0x10]
0035be48  00 70 a0 e1                                      mov r7, r0
0035be4c  06 00 a0 e1                                      mov r0, r6
0035be50  c5 cb fe eb                                      bl #0x30ed6c
0035be54  00 10 a0 e1                                      mov r1, r0
0035be58  07 00 a0 e1                                      mov r0, r7
0035be5c  50 cb fe eb                                      bl #0x30eba4
0035be60  00 10 a0 e1                                      mov r1, r0
0035be64  4e cb fe eb                                      bl #0x30eba4
0035be68  0c 10 9d e5                                      ldr r1, [sp, #0xc]
0035be6c  00 60 a0 e1                                      mov r6, r0
0035be70  04 00 a0 e1                                      mov r0, r4
0035be74  bc cb fe eb                                      bl #0x30ed6c
0035be78  00 30 9d e5                                      ldr r3, [sp]
0035be7c  00 10 a0 e1                                      mov r1, r0
0035be80  03 00 a0 e1                                      mov r0, r3
0035be84  46 cb fe eb                                      bl #0x30eba4
0035be88  00 10 a0 e1                                      mov r1, r0
0035be8c  06 00 a0 e1                                      mov r0, r6
0035be90  43 cb fe eb                                      bl #0x30eba4
0035be94  08 00 85 e5                                      str r0, [r5, #8]
0035be98  05 00 a0 e1                                      mov r0, r5
0035be9c  1c d0 8d e2                                      add sp, sp, #0x1c
0035bea0  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x00432984, declared_size=436, range_size=436, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternion9getMatrixERNS0_8CMatrix4IfEE
; demangled: glitch::core::quaternion::getMatrix(glitch::core::CMatrix4<float>&) const
; decoder-mode: arm
00432984  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00432988  00 60 90 e5                                      ldr r6, [r0]
0043298c  1c d0 4d e2                                      sub sp, sp, #0x1c
00432990  01 40 a0 e1                                      mov r4, r1
00432994  00 80 a0 e1                                      mov r8, r0
00432998  06 10 a0 e1                                      mov r1, r6
0043299c  06 00 a0 e1                                      mov r0, r6
004329a0  7f 70 fb eb                                      bl #0x30eba4
004329a4  00 a0 a0 e1                                      mov sl, r0
004329a8  0a 10 a0 e1                                      mov r1, sl
004329ac  06 00 a0 e1                                      mov r0, r6
004329b0  ed 70 fb eb                                      bl #0x30ed6c
004329b4  04 00 8d e5                                      str r0, [sp, #4]
004329b8  04 60 98 e5                                      ldr r6, [r8, #4]
004329bc  00 50 a0 e3                                      mov r5, #0
004329c0  06 10 a0 e1                                      mov r1, r6
004329c4  06 00 a0 e1                                      mov r0, r6
004329c8  75 70 fb eb                                      bl #0x30eba4
004329cc  08 90 98 e5                                      ldr sb, [r8, #8]
004329d0  00 70 a0 e1                                      mov r7, r0
004329d4  09 10 a0 e1                                      mov r1, sb
004329d8  09 00 a0 e1                                      mov r0, sb
004329dc  70 70 fb eb                                      bl #0x30eba4
004329e0  00 b0 a0 e1                                      mov fp, r0
004329e4  0b 10 a0 e1                                      mov r1, fp
004329e8  09 00 a0 e1                                      mov r0, sb
004329ec  de 70 fb eb                                      bl #0x30ed6c
004329f0  06 10 a0 e1                                      mov r1, r6
004329f4  08 00 8d e5                                      str r0, [sp, #8]
004329f8  0a 00 a0 e1                                      mov r0, sl
004329fc  da 70 fb eb                                      bl #0x30ed6c
00432a00  09 10 a0 e1                                      mov r1, sb
00432a04  0c 00 8d e5                                      str r0, [sp, #0xc]
00432a08  0a 00 a0 e1                                      mov r0, sl
00432a0c  d6 70 fb eb                                      bl #0x30ed6c
00432a10  10 00 8d e5                                      str r0, [sp, #0x10]
00432a14  0c 80 98 e5                                      ldr r8, [r8, #0xc]
00432a18  0a 00 a0 e1                                      mov r0, sl
00432a1c  08 10 a0 e1                                      mov r1, r8
00432a20  d1 70 fb eb                                      bl #0x30ed6c
00432a24  09 10 a0 e1                                      mov r1, sb
00432a28  00 a0 a0 e1                                      mov sl, r0
00432a2c  07 00 a0 e1                                      mov r0, r7
00432a30  cd 70 fb eb                                      bl #0x30ed6c
00432a34  08 10 a0 e1                                      mov r1, r8
00432a38  14 00 8d e5                                      str r0, [sp, #0x14]
00432a3c  07 00 a0 e1                                      mov r0, r7
00432a40  c9 70 fb eb                                      bl #0x30ed6c
00432a44  08 10 a0 e1                                      mov r1, r8
00432a48  00 90 a0 e1                                      mov sb, r0
00432a4c  0b 00 a0 e1                                      mov r0, fp
00432a50  c5 70 fb eb                                      bl #0x30ed6c
00432a54  00 30 a0 e3                                      mov r3, #0
00432a58  40 30 c4 e5                                      strb r3, [r4, #0x40]
00432a5c  00 80 a0 e1                                      mov r8, r0
00432a60  07 10 a0 e1                                      mov r1, r7
00432a64  06 00 a0 e1                                      mov r0, r6
00432a68  bf 70 fb eb                                      bl #0x30ed6c
00432a6c  00 10 a0 e1                                      mov r1, r0
00432a70  fe 05 a0 e3                                      mov r0, #0x3f800000
00432a74  4c 6e fb eb                                      bl #0x30e3ac
00432a78  08 10 9d e5                                      ldr r1, [sp, #8]
00432a7c  00 60 a0 e1                                      mov r6, r0
00432a80  49 6e fb eb                                      bl #0x30e3ac
00432a84  00 00 84 e5                                      str r0, [r4]
00432a88  0c 00 9d e5                                      ldr r0, [sp, #0xc]
00432a8c  08 10 a0 e1                                      mov r1, r8
00432a90  43 70 fb eb                                      bl #0x30eba4
00432a94  04 00 84 e5                                      str r0, [r4, #4]
00432a98  10 00 9d e5                                      ldr r0, [sp, #0x10]
00432a9c  09 10 a0 e1                                      mov r1, sb
00432aa0  41 6e fb eb                                      bl #0x30e3ac
00432aa4  0c 50 84 e5                                      str r5, [r4, #0xc]
00432aa8  08 00 84 e5                                      str r0, [r4, #8]
00432aac  0c 00 9d e5                                      ldr r0, [sp, #0xc]
00432ab0  08 10 a0 e1                                      mov r1, r8
00432ab4  3c 6e fb eb                                      bl #0x30e3ac
00432ab8  10 00 84 e5                                      str r0, [r4, #0x10]
00432abc  04 10 9d e5                                      ldr r1, [sp, #4]
00432ac0  fe 05 a0 e3                                      mov r0, #0x3f800000
00432ac4  38 6e fb eb                                      bl #0x30e3ac
00432ac8  08 10 9d e5                                      ldr r1, [sp, #8]
00432acc  36 6e fb eb                                      bl #0x30e3ac
00432ad0  14 00 84 e5                                      str r0, [r4, #0x14]
00432ad4  14 00 9d e5                                      ldr r0, [sp, #0x14]
00432ad8  0a 10 a0 e1                                      mov r1, sl
00432adc  30 70 fb eb                                      bl #0x30eba4
00432ae0  1c 50 84 e5                                      str r5, [r4, #0x1c]
00432ae4  18 00 84 e5                                      str r0, [r4, #0x18]
00432ae8  09 10 a0 e1                                      mov r1, sb
00432aec  10 00 9d e5                                      ldr r0, [sp, #0x10]
00432af0  2b 70 fb eb                                      bl #0x30eba4
00432af4  0a 10 a0 e1                                      mov r1, sl
00432af8  20 00 84 e5                                      str r0, [r4, #0x20]
00432afc  14 00 9d e5                                      ldr r0, [sp, #0x14]
00432b00  29 6e fb eb                                      bl #0x30e3ac
00432b04  24 00 84 e5                                      str r0, [r4, #0x24]
00432b08  04 10 9d e5                                      ldr r1, [sp, #4]
00432b0c  06 00 a0 e1                                      mov r0, r6
00432b10  25 6e fb eb                                      bl #0x30e3ac
00432b14  fe 35 a0 e3                                      mov r3, #0x3f800000
00432b18  28 00 84 e5                                      str r0, [r4, #0x28]
00432b1c  38 50 84 e5                                      str r5, [r4, #0x38]
00432b20  3c 30 84 e5                                      str r3, [r4, #0x3c]
00432b24  2c 50 84 e5                                      str r5, [r4, #0x2c]
00432b28  30 50 84 e5                                      str r5, [r4, #0x30]
00432b2c  34 50 84 e5                                      str r5, [r4, #0x34]
00432b30  1c d0 8d e2                                      add sp, sp, #0x1c
00432b34  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x005602d0, declared_size=436, range_size=436, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternion20getMatrix_transposedERNS0_8CMatrix4IfEE
; demangled: glitch::core::quaternion::getMatrix_transposed(glitch::core::CMatrix4<float>&) const
; decoder-mode: arm
005602d0  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005602d4  00 60 90 e5                                      ldr r6, [r0]
005602d8  1c d0 4d e2                                      sub sp, sp, #0x1c
005602dc  01 40 a0 e1                                      mov r4, r1
005602e0  00 80 a0 e1                                      mov r8, r0
005602e4  06 10 a0 e1                                      mov r1, r6
005602e8  06 00 a0 e1                                      mov r0, r6
005602ec  2c ba f6 eb                                      bl #0x30eba4
005602f0  00 a0 a0 e1                                      mov sl, r0
005602f4  0a 10 a0 e1                                      mov r1, sl
005602f8  06 00 a0 e1                                      mov r0, r6
005602fc  9a ba f6 eb                                      bl #0x30ed6c
00560300  04 00 8d e5                                      str r0, [sp, #4]
00560304  04 60 98 e5                                      ldr r6, [r8, #4]
00560308  00 50 a0 e3                                      mov r5, #0
0056030c  06 10 a0 e1                                      mov r1, r6
00560310  06 00 a0 e1                                      mov r0, r6
00560314  22 ba f6 eb                                      bl #0x30eba4
00560318  08 90 98 e5                                      ldr sb, [r8, #8]
0056031c  00 70 a0 e1                                      mov r7, r0
00560320  09 10 a0 e1                                      mov r1, sb
00560324  09 00 a0 e1                                      mov r0, sb
00560328  1d ba f6 eb                                      bl #0x30eba4
0056032c  00 b0 a0 e1                                      mov fp, r0
00560330  0b 10 a0 e1                                      mov r1, fp
00560334  09 00 a0 e1                                      mov r0, sb
00560338  8b ba f6 eb                                      bl #0x30ed6c
0056033c  06 10 a0 e1                                      mov r1, r6
00560340  08 00 8d e5                                      str r0, [sp, #8]
00560344  0a 00 a0 e1                                      mov r0, sl
00560348  87 ba f6 eb                                      bl #0x30ed6c
0056034c  09 10 a0 e1                                      mov r1, sb
00560350  0c 00 8d e5                                      str r0, [sp, #0xc]
00560354  0a 00 a0 e1                                      mov r0, sl
00560358  83 ba f6 eb                                      bl #0x30ed6c
0056035c  10 00 8d e5                                      str r0, [sp, #0x10]
00560360  0c 80 98 e5                                      ldr r8, [r8, #0xc]
00560364  0a 00 a0 e1                                      mov r0, sl
00560368  08 10 a0 e1                                      mov r1, r8
0056036c  7e ba f6 eb                                      bl #0x30ed6c
00560370  09 10 a0 e1                                      mov r1, sb
00560374  00 a0 a0 e1                                      mov sl, r0
00560378  07 00 a0 e1                                      mov r0, r7
0056037c  7a ba f6 eb                                      bl #0x30ed6c
00560380  08 10 a0 e1                                      mov r1, r8
00560384  14 00 8d e5                                      str r0, [sp, #0x14]
00560388  07 00 a0 e1                                      mov r0, r7
0056038c  76 ba f6 eb                                      bl #0x30ed6c
00560390  08 10 a0 e1                                      mov r1, r8
00560394  00 90 a0 e1                                      mov sb, r0
00560398  0b 00 a0 e1                                      mov r0, fp
0056039c  72 ba f6 eb                                      bl #0x30ed6c
005603a0  00 30 a0 e3                                      mov r3, #0
005603a4  40 30 c4 e5                                      strb r3, [r4, #0x40]
005603a8  00 80 a0 e1                                      mov r8, r0
005603ac  07 10 a0 e1                                      mov r1, r7
005603b0  06 00 a0 e1                                      mov r0, r6
005603b4  6c ba f6 eb                                      bl #0x30ed6c
005603b8  00 10 a0 e1                                      mov r1, r0
005603bc  fe 05 a0 e3                                      mov r0, #0x3f800000
005603c0  f9 b7 f6 eb                                      bl #0x30e3ac
005603c4  08 10 9d e5                                      ldr r1, [sp, #8]
005603c8  00 60 a0 e1                                      mov r6, r0
005603cc  f6 b7 f6 eb                                      bl #0x30e3ac
005603d0  00 00 84 e5                                      str r0, [r4]
005603d4  0c 00 9d e5                                      ldr r0, [sp, #0xc]
005603d8  08 10 a0 e1                                      mov r1, r8
005603dc  f0 b9 f6 eb                                      bl #0x30eba4
005603e0  10 00 84 e5                                      str r0, [r4, #0x10]
005603e4  10 00 9d e5                                      ldr r0, [sp, #0x10]
005603e8  09 10 a0 e1                                      mov r1, sb
005603ec  ee b7 f6 eb                                      bl #0x30e3ac
005603f0  30 50 84 e5                                      str r5, [r4, #0x30]
005603f4  20 00 84 e5                                      str r0, [r4, #0x20]
005603f8  0c 00 9d e5                                      ldr r0, [sp, #0xc]
005603fc  08 10 a0 e1                                      mov r1, r8
00560400  e9 b7 f6 eb                                      bl #0x30e3ac
00560404  04 00 84 e5                                      str r0, [r4, #4]
00560408  04 10 9d e5                                      ldr r1, [sp, #4]
0056040c  fe 05 a0 e3                                      mov r0, #0x3f800000
00560410  e5 b7 f6 eb                                      bl #0x30e3ac
00560414  08 10 9d e5                                      ldr r1, [sp, #8]
00560418  e3 b7 f6 eb                                      bl #0x30e3ac
0056041c  14 00 84 e5                                      str r0, [r4, #0x14]
00560420  14 00 9d e5                                      ldr r0, [sp, #0x14]
00560424  0a 10 a0 e1                                      mov r1, sl
00560428  dd b9 f6 eb                                      bl #0x30eba4
0056042c  34 50 84 e5                                      str r5, [r4, #0x34]
00560430  24 00 84 e5                                      str r0, [r4, #0x24]
00560434  09 10 a0 e1                                      mov r1, sb
00560438  10 00 9d e5                                      ldr r0, [sp, #0x10]
0056043c  d8 b9 f6 eb                                      bl #0x30eba4
00560440  0a 10 a0 e1                                      mov r1, sl
00560444  08 00 84 e5                                      str r0, [r4, #8]
00560448  14 00 9d e5                                      ldr r0, [sp, #0x14]
0056044c  d6 b7 f6 eb                                      bl #0x30e3ac
00560450  18 00 84 e5                                      str r0, [r4, #0x18]
00560454  04 10 9d e5                                      ldr r1, [sp, #4]
00560458  06 00 a0 e1                                      mov r0, r6
0056045c  d2 b7 f6 eb                                      bl #0x30e3ac
00560460  fe 35 a0 e3                                      mov r3, #0x3f800000
00560464  28 00 84 e5                                      str r0, [r4, #0x28]
00560468  2c 50 84 e5                                      str r5, [r4, #0x2c]
0056046c  3c 30 84 e5                                      str r3, [r4, #0x3c]
00560470  38 50 84 e5                                      str r5, [r4, #0x38]
00560474  0c 50 84 e5                                      str r5, [r4, #0xc]
00560478  1c 50 84 e5                                      str r5, [r4, #0x1c]
0056047c  1c d0 8d e2                                      add sp, sp, #0x1c
00560480  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x006d162c, declared_size=36, range_size=36, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternion9getMatrixEv
; demangled: glitch::core::quaternion::getMatrix() const
; decoder-mode: arm
006d162c  10 40 2d e9                                      push {r4, lr}
006d1630  00 30 a0 e3                                      mov r3, #0
006d1634  00 40 a0 e1                                      mov r4, r0
006d1638  40 30 c0 e5                                      strb r3, [r0, #0x40]
006d163c  01 00 a0 e1                                      mov r0, r1
006d1640  04 10 a0 e1                                      mov r1, r4
006d1644  21 3b fa eb                                      bl #0x5602d0
006d1648  04 00 a0 e1                                      mov r0, r4
006d164c  10 80 bd e8                                      pop {r4, pc}

; FUNCTION 0x0050eeb4, declared_size=676, range_size=676, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternionaSERKNS0_8CMatrix4IfEE
; demangled: glitch::core::quaternion::operator=(glitch::core::CMatrix4<float> const&)
; decoder-mode: arm
0050eeb4  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
0050eeb8  00 70 91 e5                                      ldr r7, [r1]
0050eebc  14 60 91 e5                                      ldr r6, [r1, #0x14]
0050eec0  28 80 91 e5                                      ldr r8, [r1, #0x28]
0050eec4  01 40 a0 e1                                      mov r4, r1
0050eec8  00 50 a0 e1                                      mov r5, r0
0050eecc  06 10 a0 e1                                      mov r1, r6
0050eed0  07 00 a0 e1                                      mov r0, r7
0050eed4  32 ff f7 eb                                      bl #0x30eba4
0050eed8  08 10 a0 e1                                      mov r1, r8
0050eedc  30 ff f7 eb                                      bl #0x30eba4
0050eee0  00 10 a0 e3                                      mov r1, #0
0050eee4  00 a0 a0 e1                                      mov sl, r0
0050eee8  02 fd f7 eb                                      bl #0x30e2f8
0050eeec  00 00 50 e3                                      cmp r0, #0
0050eef0  79 00 00 1a                                      bne #0x50f0dc
0050eef4  07 00 a0 e1                                      mov r0, r7
0050eef8  06 10 a0 e1                                      mov r1, r6
0050eefc  fd fc f7 eb                                      bl #0x30e2f8
0050ef00  00 00 50 e3                                      cmp r0, #0
0050ef04  27 00 00 0a                                      beq #0x50efa8
0050ef08  07 00 a0 e1                                      mov r0, r7
0050ef0c  08 10 a0 e1                                      mov r1, r8
0050ef10  f8 fc f7 eb                                      bl #0x30e2f8
0050ef14  00 00 50 e3                                      cmp r0, #0
0050ef18  22 00 00 0a                                      beq #0x50efa8
0050ef1c  fe 15 a0 e3                                      mov r1, #0x3f800000
0050ef20  07 00 a0 e1                                      mov r0, r7
0050ef24  1e ff f7 eb                                      bl #0x30eba4
0050ef28  06 10 a0 e1                                      mov r1, r6
0050ef2c  1e fd f7 eb                                      bl #0x30e3ac
0050ef30  08 10 a0 e1                                      mov r1, r8
0050ef34  1c fd f7 eb                                      bl #0x30e3ac
0050ef38  79 fc f7 eb                                      bl #0x30e124
0050ef3c  3f 14 a0 e3                                      mov r1, #0x3f000000
0050ef40  00 60 a0 e1                                      mov r6, r0
0050ef44  88 ff f7 eb                                      bl #0x30ed6c
0050ef48  06 10 a0 e1                                      mov r1, r6
0050ef4c  00 00 85 e5                                      str r0, [r5]
0050ef50  3f 04 a0 e3                                      mov r0, #0x3f000000
0050ef54  4e ff f7 eb                                      bl #0x30ec94
0050ef58  10 10 94 e5                                      ldr r1, [r4, #0x10]
0050ef5c  00 60 a0 e1                                      mov r6, r0
0050ef60  04 00 94 e5                                      ldr r0, [r4, #4]
0050ef64  0e ff f7 eb                                      bl #0x30eba4
0050ef68  06 10 a0 e1                                      mov r1, r6
0050ef6c  7e ff f7 eb                                      bl #0x30ed6c
0050ef70  04 00 85 e5                                      str r0, [r5, #4]
0050ef74  08 10 94 e5                                      ldr r1, [r4, #8]
0050ef78  20 00 94 e5                                      ldr r0, [r4, #0x20]
0050ef7c  08 ff f7 eb                                      bl #0x30eba4
0050ef80  06 10 a0 e1                                      mov r1, r6
0050ef84  78 ff f7 eb                                      bl #0x30ed6c
0050ef88  08 00 85 e5                                      str r0, [r5, #8]
0050ef8c  18 10 94 e5                                      ldr r1, [r4, #0x18]
0050ef90  24 00 94 e5                                      ldr r0, [r4, #0x24]
0050ef94  04 fd f7 eb                                      bl #0x30e3ac
0050ef98  06 10 a0 e1                                      mov r1, r6
0050ef9c  72 ff f7 eb                                      bl #0x30ed6c
0050efa0  0c 00 85 e5                                      str r0, [r5, #0xc]
0050efa4  26 00 00 ea                                      b #0x50f044
0050efa8  06 00 a0 e1                                      mov r0, r6
0050efac  08 10 a0 e1                                      mov r1, r8
0050efb0  d0 fc f7 eb                                      bl #0x30e2f8
0050efb4  00 00 50 e3                                      cmp r0, #0
0050efb8  24 00 00 1a                                      bne #0x50f050
0050efbc  fe 15 a0 e3                                      mov r1, #0x3f800000
0050efc0  08 00 a0 e1                                      mov r0, r8
0050efc4  f6 fe f7 eb                                      bl #0x30eba4
0050efc8  07 10 a0 e1                                      mov r1, r7
0050efcc  f6 fc f7 eb                                      bl #0x30e3ac
0050efd0  06 10 a0 e1                                      mov r1, r6
0050efd4  f4 fc f7 eb                                      bl #0x30e3ac
0050efd8  51 fc f7 eb                                      bl #0x30e124
0050efdc  3f 14 a0 e3                                      mov r1, #0x3f000000
0050efe0  00 60 a0 e1                                      mov r6, r0
0050efe4  60 ff f7 eb                                      bl #0x30ed6c
0050efe8  06 10 a0 e1                                      mov r1, r6
0050efec  08 00 85 e5                                      str r0, [r5, #8]
0050eff0  3f 04 a0 e3                                      mov r0, #0x3f000000
0050eff4  26 ff f7 eb                                      bl #0x30ec94
0050eff8  20 10 94 e5                                      ldr r1, [r4, #0x20]
0050effc  00 60 a0 e1                                      mov r6, r0
0050f000  08 00 94 e5                                      ldr r0, [r4, #8]
0050f004  e6 fe f7 eb                                      bl #0x30eba4
0050f008  06 10 a0 e1                                      mov r1, r6
0050f00c  56 ff f7 eb                                      bl #0x30ed6c
0050f010  00 00 85 e5                                      str r0, [r5]
0050f014  24 10 94 e5                                      ldr r1, [r4, #0x24]
0050f018  18 00 94 e5                                      ldr r0, [r4, #0x18]
0050f01c  e0 fe f7 eb                                      bl #0x30eba4
0050f020  06 10 a0 e1                                      mov r1, r6
0050f024  50 ff f7 eb                                      bl #0x30ed6c
0050f028  04 00 85 e5                                      str r0, [r5, #4]
0050f02c  04 10 94 e5                                      ldr r1, [r4, #4]
0050f030  10 00 94 e5                                      ldr r0, [r4, #0x10]
0050f034  dc fc f7 eb                                      bl #0x30e3ac
0050f038  06 10 a0 e1                                      mov r1, r6
0050f03c  4a ff f7 eb                                      bl #0x30ed6c
0050f040  0c 00 85 e5                                      str r0, [r5, #0xc]
0050f044  05 00 a0 e1                                      mov r0, r5
0050f048  f0 47 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, lr}
0050f04c  27 36 f9 ea                                      b #0x35c8f0
0050f050  06 00 a0 e1                                      mov r0, r6
0050f054  fe 15 a0 e3                                      mov r1, #0x3f800000
0050f058  d1 fe f7 eb                                      bl #0x30eba4
0050f05c  07 10 a0 e1                                      mov r1, r7
0050f060  d1 fc f7 eb                                      bl #0x30e3ac
0050f064  08 10 a0 e1                                      mov r1, r8
0050f068  cf fc f7 eb                                      bl #0x30e3ac
0050f06c  2c fc f7 eb                                      bl #0x30e124
0050f070  3f 14 a0 e3                                      mov r1, #0x3f000000
0050f074  00 60 a0 e1                                      mov r6, r0
0050f078  3b ff f7 eb                                      bl #0x30ed6c
0050f07c  06 10 a0 e1                                      mov r1, r6
0050f080  04 00 85 e5                                      str r0, [r5, #4]
0050f084  3f 04 a0 e3                                      mov r0, #0x3f000000
0050f088  01 ff f7 eb                                      bl #0x30ec94
0050f08c  10 10 94 e5                                      ldr r1, [r4, #0x10]
0050f090  00 60 a0 e1                                      mov r6, r0
0050f094  04 00 94 e5                                      ldr r0, [r4, #4]
0050f098  c1 fe f7 eb                                      bl #0x30eba4
0050f09c  06 10 a0 e1                                      mov r1, r6
0050f0a0  31 ff f7 eb                                      bl #0x30ed6c
0050f0a4  00 00 85 e5                                      str r0, [r5]
0050f0a8  24 10 94 e5                                      ldr r1, [r4, #0x24]
0050f0ac  18 00 94 e5                                      ldr r0, [r4, #0x18]
0050f0b0  bb fe f7 eb                                      bl #0x30eba4
0050f0b4  06 10 a0 e1                                      mov r1, r6
0050f0b8  2b ff f7 eb                                      bl #0x30ed6c
0050f0bc  08 00 85 e5                                      str r0, [r5, #8]
0050f0c0  20 10 94 e5                                      ldr r1, [r4, #0x20]
0050f0c4  08 00 94 e5                                      ldr r0, [r4, #8]
0050f0c8  b7 fc f7 eb                                      bl #0x30e3ac
0050f0cc  06 10 a0 e1                                      mov r1, r6
0050f0d0  25 ff f7 eb                                      bl #0x30ed6c
0050f0d4  0c 00 85 e5                                      str r0, [r5, #0xc]
0050f0d8  d9 ff ff ea                                      b #0x50f044
0050f0dc  fe 15 a0 e3                                      mov r1, #0x3f800000
0050f0e0  0a 00 a0 e1                                      mov r0, sl
0050f0e4  ae fe f7 eb                                      bl #0x30eba4
0050f0e8  0d fc f7 eb                                      bl #0x30e124
0050f0ec  3f 14 a0 e3                                      mov r1, #0x3f000000
0050f0f0  00 60 a0 e1                                      mov r6, r0
0050f0f4  1c ff f7 eb                                      bl #0x30ed6c
0050f0f8  06 10 a0 e1                                      mov r1, r6
0050f0fc  0c 00 85 e5                                      str r0, [r5, #0xc]
0050f100  3f 04 a0 e3                                      mov r0, #0x3f000000
0050f104  e2 fe f7 eb                                      bl #0x30ec94
0050f108  18 10 94 e5                                      ldr r1, [r4, #0x18]
0050f10c  00 60 a0 e1                                      mov r6, r0
0050f110  24 00 94 e5                                      ldr r0, [r4, #0x24]
0050f114  a4 fc f7 eb                                      bl #0x30e3ac
0050f118  06 10 a0 e1                                      mov r1, r6
0050f11c  12 ff f7 eb                                      bl #0x30ed6c
0050f120  00 00 85 e5                                      str r0, [r5]
0050f124  20 10 94 e5                                      ldr r1, [r4, #0x20]
0050f128  08 00 94 e5                                      ldr r0, [r4, #8]
0050f12c  9e fc f7 eb                                      bl #0x30e3ac
0050f130  06 10 a0 e1                                      mov r1, r6
0050f134  0c ff f7 eb                                      bl #0x30ed6c
0050f138  04 00 85 e5                                      str r0, [r5, #4]
0050f13c  04 10 94 e5                                      ldr r1, [r4, #4]
0050f140  10 00 94 e5                                      ldr r0, [r4, #0x10]
0050f144  98 fc f7 eb                                      bl #0x30e3ac
0050f148  06 10 a0 e1                                      mov r1, r6
0050f14c  06 ff f7 eb                                      bl #0x30ed6c
0050f150  08 00 85 e5                                      str r0, [r5, #8]
0050f154  ba ff ff ea                                      b #0x50f044


; FUNCTION 0x00432bbc, declared_size=668, range_size=668, mode=arm
; class-group: glitch::core::CMatrix4<float>
; alias: _ZNK6glitch4core8CMatrix4IfE18getRotationDegreesEv
; demangled: glitch::core::CMatrix4<float>::getRotationDegrees() const
; decoder-mode: arm
00432bbc  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00432bc0  00 40 a0 e1                                      mov r4, r0
00432bc4  0c d0 4d e2                                      sub sp, sp, #0xc
00432bc8  08 00 91 e5                                      ldr r0, [r1, #8]
00432bcc  01 50 a0 e1                                      mov r5, r1
00432bd0  6c 6c fb eb                                      bl #0x30dd88
00432bd4  02 01 80 e2                                      add r0, r0, #0x80000000
00432bd8  31 6f fb eb                                      bl #0x30e8a4
00432bdc  00 80 a0 e1                                      mov r8, r0
00432be0  01 90 a0 e1                                      mov sb, r1
00432be4  57 6d fb eb                                      bl #0x30e148
00432be8  f8 21 0c e3                                      movw r2, #0xc1f8
00432bec  dc 35 0a e3                                      movw r3, #0xa5dc
00432bf0  00 70 a0 e1                                      mov r7, r0
00432bf4  01 60 a0 e1                                      mov r6, r1
00432bf8  63 2a 41 e3                                      movt r2, #0x1a63
00432bfc  4c 30 44 e3                                      movt r3, #0x404c
00432c00  08 00 a0 e1                                      mov r0, r8
00432c04  09 10 a0 e1                                      mov r1, sb
00432c08  a9 6f fb eb                                      bl #0x30eab4
00432c0c  3a 2c 08 e3                                      movw r2, #0x8c3a
00432c10  8e 39 07 e3                                      movw r3, #0x798e
00432c14  00 a0 a0 e1                                      mov sl, r0
00432c18  01 b0 a0 e1                                      mov fp, r1
00432c1c  07 00 a0 e1                                      mov r0, r7
00432c20  02 11 c6 e3                                      bic r1, r6, #0x80000000
00432c24  30 22 4e e3                                      movt r2, #0xe230
00432c28  45 3e 43 e3                                      movt r3, #0x3e45
00432c2c  8b 6c fb eb                                      bl #0x30de60
00432c30  00 00 50 e3                                      cmp r0, #0
00432c34  71 00 00 0a                                      beq #0x432e00
00432c38  ff 15 a0 e3                                      mov r1, #0x3fc00000
00432c3c  07 20 a0 e1                                      mov r2, r7
00432c40  06 30 a0 e1                                      mov r3, r6
00432c44  00 00 a0 e3                                      mov r0, #0
00432c48  03 16 81 e2                                      add r1, r1, #0x300000
00432c4c  bb 6d fb eb                                      bl #0x30e340
00432c50  00 60 a0 e1                                      mov r6, r0
00432c54  01 70 a0 e1                                      mov r7, r1
00432c58  18 00 95 e5                                      ldr r0, [r5, #0x18]
00432c5c  10 6f fb eb                                      bl #0x30e8a4
00432c60  06 20 a0 e1                                      mov r2, r6
00432c64  07 30 a0 e1                                      mov r3, r7
00432c68  91 6f fb eb                                      bl #0x30eab4
00432c6c  00 80 a0 e1                                      mov r8, r0
00432c70  28 00 95 e5                                      ldr r0, [r5, #0x28]
00432c74  01 90 a0 e1                                      mov sb, r1
00432c78  09 6f fb eb                                      bl #0x30e8a4
00432c7c  06 20 a0 e1                                      mov r2, r6
00432c80  07 30 a0 e1                                      mov r3, r7
00432c84  8a 6f fb eb                                      bl #0x30eab4
00432c88  00 20 a0 e1                                      mov r2, r0
00432c8c  01 30 a0 e1                                      mov r3, r1
00432c90  08 00 a0 e1                                      mov r0, r8
00432c94  09 10 a0 e1                                      mov r1, sb
00432c98  a0 6c fb eb                                      bl #0x30df20
00432c9c  f8 21 0c e3                                      movw r2, #0xc1f8
00432ca0  dc 35 0a e3                                      movw r3, #0xa5dc
00432ca4  63 2a 41 e3                                      movt r2, #0x1a63
00432ca8  4c 30 44 e3                                      movt r3, #0x404c
00432cac  80 6f fb eb                                      bl #0x30eab4
00432cb0  00 80 a0 e1                                      mov r8, r0
00432cb4  04 00 95 e5                                      ldr r0, [r5, #4]
00432cb8  01 90 a0 e1                                      mov sb, r1
00432cbc  f8 6e fb eb                                      bl #0x30e8a4
00432cc0  06 20 a0 e1                                      mov r2, r6
00432cc4  07 30 a0 e1                                      mov r3, r7
00432cc8  79 6f fb eb                                      bl #0x30eab4
00432ccc  f0 00 cd e1                                      strd r0, r1, [sp]
00432cd0  00 00 95 e5                                      ldr r0, [r5]
00432cd4  f2 6e fb eb                                      bl #0x30e8a4
00432cd8  06 20 a0 e1                                      mov r2, r6
00432cdc  07 30 a0 e1                                      mov r3, r7
00432ce0  73 6f fb eb                                      bl #0x30eab4
00432ce4  00 20 a0 e1                                      mov r2, r0
00432ce8  01 30 a0 e1                                      mov r3, r1
00432cec  d0 00 cd e1                                      ldrd r0, r1, [sp]
00432cf0  8a 6c fb eb                                      bl #0x30df20
00432cf4  f8 21 0c e3                                      movw r2, #0xc1f8
00432cf8  dc 35 0a e3                                      movw r3, #0xa5dc
00432cfc  63 2a 41 e3                                      movt r2, #0x1a63
00432d00  4c 30 44 e3                                      movt r3, #0x404c
00432d04  6a 6f fb eb                                      bl #0x30eab4
00432d08  00 20 a0 e3                                      mov r2, #0
00432d0c  00 60 a0 e1                                      mov r6, r0
00432d10  01 70 a0 e1                                      mov r7, r1
00432d14  08 00 a0 e1                                      mov r0, r8
00432d18  09 10 a0 e1                                      mov r1, sb
00432d1c  00 30 a0 e3                                      mov r3, #0
00432d20  8e 6e fb eb                                      bl #0x30e760
00432d24  00 00 50 e3                                      cmp r0, #0
00432d28  07 00 00 0a                                      beq #0x432d4c
00432d2c  00 30 08 e3                                      movw r3, #0x8000
00432d30  08 00 a0 e1                                      mov r0, r8
00432d34  09 10 a0 e1                                      mov r1, sb
00432d38  00 20 a0 e3                                      mov r2, #0
00432d3c  76 30 44 e3                                      movt r3, #0x4076
00432d40  7f 6f fb eb                                      bl #0x30eb44
00432d44  00 80 a0 e1                                      mov r8, r0
00432d48  01 90 a0 e1                                      mov sb, r1
00432d4c  0a 00 a0 e1                                      mov r0, sl
00432d50  0b 10 a0 e1                                      mov r1, fp
00432d54  00 20 a0 e3                                      mov r2, #0
00432d58  00 30 a0 e3                                      mov r3, #0
00432d5c  7f 6e fb eb                                      bl #0x30e760
00432d60  00 00 50 e3                                      cmp r0, #0
00432d64  07 00 00 0a                                      beq #0x432d88
00432d68  00 30 08 e3                                      movw r3, #0x8000
00432d6c  0a 00 a0 e1                                      mov r0, sl
00432d70  0b 10 a0 e1                                      mov r1, fp
00432d74  00 20 a0 e3                                      mov r2, #0
00432d78  76 30 44 e3                                      movt r3, #0x4076
00432d7c  70 6f fb eb                                      bl #0x30eb44
00432d80  00 a0 a0 e1                                      mov sl, r0
00432d84  01 b0 a0 e1                                      mov fp, r1
00432d88  06 00 a0 e1                                      mov r0, r6
00432d8c  07 10 a0 e1                                      mov r1, r7
00432d90  00 20 a0 e3                                      mov r2, #0
00432d94  00 30 a0 e3                                      mov r3, #0
00432d98  70 6e fb eb                                      bl #0x30e760
00432d9c  00 00 50 e3                                      cmp r0, #0
00432da0  07 00 00 0a                                      beq #0x432dc4
00432da4  00 30 08 e3                                      movw r3, #0x8000
00432da8  06 00 a0 e1                                      mov r0, r6
00432dac  07 10 a0 e1                                      mov r1, r7
00432db0  00 20 a0 e3                                      mov r2, #0
00432db4  76 30 44 e3                                      movt r3, #0x4076
00432db8  61 6f fb eb                                      bl #0x30eb44
00432dbc  00 60 a0 e1                                      mov r6, r0
00432dc0  01 70 a0 e1                                      mov r7, r1
00432dc4  09 10 a0 e1                                      mov r1, sb
00432dc8  08 00 a0 e1                                      mov r0, r8
00432dcc  33 6e fb eb                                      bl #0x30e6a0
00432dd0  0b 10 a0 e1                                      mov r1, fp
00432dd4  00 00 84 e5                                      str r0, [r4]
00432dd8  0a 00 a0 e1                                      mov r0, sl
00432ddc  2f 6e fb eb                                      bl #0x30e6a0
00432de0  07 10 a0 e1                                      mov r1, r7
00432de4  04 00 84 e5                                      str r0, [r4, #4]
00432de8  06 00 a0 e1                                      mov r0, r6
00432dec  2b 6e fb eb                                      bl #0x30e6a0
00432df0  08 00 84 e5                                      str r0, [r4, #8]
00432df4  04 00 a0 e1                                      mov r0, r4
00432df8  0c d0 8d e2                                      add sp, sp, #0xc
00432dfc  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00432e00  10 00 95 e5                                      ldr r0, [r5, #0x10]
00432e04  00 80 a0 e3                                      mov r8, #0
00432e08  00 90 a0 e3                                      mov sb, #0
00432e0c  02 01 80 e2                                      add r0, r0, #0x80000000
00432e10  a3 6e fb eb                                      bl #0x30e8a4
00432e14  00 60 a0 e1                                      mov r6, r0
00432e18  14 00 95 e5                                      ldr r0, [r5, #0x14]
00432e1c  01 70 a0 e1                                      mov r7, r1
00432e20  9f 6e fb eb                                      bl #0x30e8a4
00432e24  00 20 a0 e1                                      mov r2, r0
00432e28  01 30 a0 e1                                      mov r3, r1
00432e2c  06 00 a0 e1                                      mov r0, r6
00432e30  07 10 a0 e1                                      mov r1, r7
00432e34  39 6c fb eb                                      bl #0x30df20
00432e38  f8 21 0c e3                                      movw r2, #0xc1f8
00432e3c  dc 35 0a e3                                      movw r3, #0xa5dc
00432e40  63 2a 41 e3                                      movt r2, #0x1a63
00432e44  4c 30 44 e3                                      movt r3, #0x404c
00432e48  19 6f fb eb                                      bl #0x30eab4
00432e4c  00 60 a0 e1                                      mov r6, r0
00432e50  01 70 a0 e1                                      mov r7, r1
00432e54  bc ff ff ea                                      b #0x432d4c


; FUNCTION 0x00432e58, declared_size=144, range_size=144, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZNK6glitch4core10quaternion13toEulerDegreeERNS0_8vector3dIfEE
; demangled: glitch::core::quaternion::toEulerDegree(glitch::core::vector3d<float>&) const
; decoder-mode: arm
00432e58  f0 45 2d e9                                      push {r4, r5, r6, r7, r8, sl, lr}
00432e5c  00 70 a0 e3                                      mov r7, #0
00432e60  54 d0 4d e2                                      sub sp, sp, #0x54
00432e64  40 80 a0 e3                                      mov r8, #0x40
00432e68  01 60 a0 e1                                      mov r6, r1
00432e6c  00 a0 a0 e1                                      mov sl, r0
00432e70  07 10 a0 e1                                      mov r1, r7
00432e74  08 20 a0 e1                                      mov r2, r8
00432e78  0d 00 a0 e1                                      mov r0, sp
00432e7c  77 6d fb eb                                      bl #0x30e460
00432e80  08 20 a0 e1                                      mov r2, r8
00432e84  07 10 a0 e1                                      mov r1, r7
00432e88  0d 00 a0 e1                                      mov r0, sp
00432e8c  73 6d fb eb                                      bl #0x30e460
00432e90  fe 55 a0 e3                                      mov r5, #0x3f800000
00432e94  01 30 a0 e3                                      mov r3, #1
00432e98  0a 00 a0 e1                                      mov r0, sl
00432e9c  0d 10 a0 e1                                      mov r1, sp
00432ea0  40 30 cd e5                                      strb r3, [sp, #0x40]
00432ea4  3c 50 8d e5                                      str r5, [sp, #0x3c]
00432ea8  00 50 8d e5                                      str r5, [sp]
00432eac  14 50 8d e5                                      str r5, [sp, #0x14]
00432eb0  28 50 8d e5                                      str r5, [sp, #0x28]
00432eb4  b2 fe ff eb                                      bl #0x432984
00432eb8  0d 10 a0 e1                                      mov r1, sp
00432ebc  44 00 8d e2                                      add r0, sp, #0x44
00432ec0  3d ff ff eb                                      bl #0x432bbc
00432ec4  48 20 9d e5                                      ldr r2, [sp, #0x48]
00432ec8  4c 30 9d e5                                      ldr r3, [sp, #0x4c]
00432ecc  44 10 9d e5                                      ldr r1, [sp, #0x44]
00432ed0  0d 40 a0 e1                                      mov r4, sp
00432ed4  04 20 86 e5                                      str r2, [r6, #4]
00432ed8  00 10 86 e5                                      str r1, [r6]
00432edc  08 30 86 e5                                      str r3, [r6, #8]
00432ee0  54 d0 8d e2                                      add sp, sp, #0x54
00432ee4  f0 85 bd e8                                      pop {r4, r5, r6, r7, r8, sl, pc}


; FUNCTION 0x00612d00, declared_size=980, range_size=980, mode=arm
; class-group: glitch::core::quaternion
; alias: _ZN6glitch4core10quaternion5slerpES1_S1_f
; demangled: glitch::core::quaternion::slerp(glitch::core::quaternion, glitch::core::quaternion, float)
; decoder-mode: arm
00612d00  10 d0 4d e2                                      sub sp, sp, #0x10
00612d04  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00612d08  1c d0 4d e2                                      sub sp, sp, #0x1c
00612d0c  44 c0 8d e2                                      add ip, sp, #0x44
00612d10  0e 00 8c e8                                      stm ip, {r1, r2, r3}
00612d14  54 b0 9d e5                                      ldr fp, [sp, #0x54]
00612d18  44 90 9d e5                                      ldr sb, [sp, #0x44]
00612d1c  58 30 9d e5                                      ldr r3, [sp, #0x58]
00612d20  0b 10 a0 e1                                      mov r1, fp
00612d24  00 40 a0 e1                                      mov r4, r0
00612d28  09 00 a0 e1                                      mov r0, sb
00612d2c  0c 30 8d e5                                      str r3, [sp, #0xc]
00612d30  0d f0 f3 eb                                      bl #0x30ed6c
00612d34  48 a0 9d e5                                      ldr sl, [sp, #0x48]
00612d38  00 50 a0 e1                                      mov r5, r0
00612d3c  0c 10 9d e5                                      ldr r1, [sp, #0xc]
00612d40  0a 00 a0 e1                                      mov r0, sl
00612d44  08 f0 f3 eb                                      bl #0x30ed6c
00612d48  5c 30 9d e5                                      ldr r3, [sp, #0x5c]
00612d4c  00 10 a0 e1                                      mov r1, r0
00612d50  05 00 a0 e1                                      mov r0, r5
00612d54  08 30 8d e5                                      str r3, [sp, #8]
00612d58  91 ef f3 eb                                      bl #0x30eba4
00612d5c  4c 80 9d e5                                      ldr r8, [sp, #0x4c]
00612d60  00 50 a0 e1                                      mov r5, r0
00612d64  08 10 9d e5                                      ldr r1, [sp, #8]
00612d68  08 00 a0 e1                                      mov r0, r8
00612d6c  fe ef f3 eb                                      bl #0x30ed6c
00612d70  60 30 9d e5                                      ldr r3, [sp, #0x60]
00612d74  00 10 a0 e1                                      mov r1, r0
00612d78  05 00 a0 e1                                      mov r0, r5
00612d7c  04 30 8d e5                                      str r3, [sp, #4]
00612d80  87 ef f3 eb                                      bl #0x30eba4
00612d84  50 70 9d e5                                      ldr r7, [sp, #0x50]
00612d88  00 50 a0 e1                                      mov r5, r0
00612d8c  04 10 9d e5                                      ldr r1, [sp, #4]
00612d90  07 00 a0 e1                                      mov r0, r7
00612d94  f4 ef f3 eb                                      bl #0x30ed6c
00612d98  00 10 a0 e1                                      mov r1, r0
00612d9c  05 00 a0 e1                                      mov r0, r5
00612da0  7f ef f3 eb                                      bl #0x30eba4
00612da4  00 10 a0 e3                                      mov r1, #0
00612da8  00 60 a0 e1                                      mov r6, r0
00612dac  56 ee f3 eb                                      bl #0x30e70c
00612db0  00 00 50 e3                                      cmp r0, #0
00612db4  02 61 86 12                                      addne r6, r6, #0x80000000
00612db8  fe 15 a0 e3                                      mov r1, #0x3f800000
00612dbc  06 00 a0 e1                                      mov r0, r6
00612dc0  02 91 89 12                                      addne sb, sb, #0x80000000
00612dc4  02 a1 8a 12                                      addne sl, sl, #0x80000000
00612dc8  02 81 88 12                                      addne r8, r8, #0x80000000
00612dcc  02 71 87 12                                      addne r7, r7, #0x80000000
00612dd0  73 ef f3 eb                                      bl #0x30eba4
00612dd4  cd 1c 0c e3                                      movw r1, #0xcccd
00612dd8  4c 1d 43 e3                                      movt r1, #0x3d4c
00612ddc  45 ed f3 eb                                      bl #0x30e2f8
00612de0  00 00 50 e3                                      cmp r0, #0
00612de4  64 50 9d e5                                      ldr r5, [sp, #0x64]
00612de8  48 00 00 0a                                      beq #0x612f10
00612dec  06 10 a0 e1                                      mov r1, r6
00612df0  fe 05 a0 e3                                      mov r0, #0x3f800000
00612df4  6c ed f3 eb                                      bl #0x30e3ac
00612df8  cd 1c 0c e3                                      movw r1, #0xcccd
00612dfc  4c 1d 43 e3                                      movt r1, #0x3d4c
00612e00  ab ed f3 eb                                      bl #0x30e4b4
00612e04  00 00 50 e3                                      cmp r0, #0
00612e08  7f 00 00 0a                                      beq #0x61300c
00612e0c  06 00 a0 e1                                      mov r0, r6
00612e10  71 ed f3 eb                                      bl #0x30e3dc
00612e14  10 00 8d e5                                      str r0, [sp, #0x10]
00612e18  3a ef f3 eb                                      bl #0x30eb08
00612e1c  00 10 a0 e1                                      mov r1, r0
00612e20  fe 05 a0 e3                                      mov r0, #0x3f800000
00612e24  9a ef f3 eb                                      bl #0x30ec94
00612e28  05 10 a0 e1                                      mov r1, r5
00612e2c  14 00 8d e5                                      str r0, [sp, #0x14]
00612e30  fe 05 a0 e3                                      mov r0, #0x3f800000
00612e34  5c ed f3 eb                                      bl #0x30e3ac
00612e38  00 10 a0 e1                                      mov r1, r0
00612e3c  10 00 9d e5                                      ldr r0, [sp, #0x10]
00612e40  c9 ef f3 eb                                      bl #0x30ed6c
00612e44  2f ef f3 eb                                      bl #0x30eb08
00612e48  14 10 9d e5                                      ldr r1, [sp, #0x14]
00612e4c  c6 ef f3 eb                                      bl #0x30ed6c
00612e50  05 10 a0 e1                                      mov r1, r5
00612e54  00 60 a0 e1                                      mov r6, r0
00612e58  10 00 9d e5                                      ldr r0, [sp, #0x10]
00612e5c  c2 ef f3 eb                                      bl #0x30ed6c
00612e60  28 ef f3 eb                                      bl #0x30eb08
00612e64  14 10 9d e5                                      ldr r1, [sp, #0x14]
00612e68  bf ef f3 eb                                      bl #0x30ed6c
00612e6c  09 10 a0 e1                                      mov r1, sb
00612e70  00 50 a0 e1                                      mov r5, r0
00612e74  06 00 a0 e1                                      mov r0, r6
00612e78  bb ef f3 eb                                      bl #0x30ed6c
00612e7c  0b 10 a0 e1                                      mov r1, fp
00612e80  00 90 a0 e1                                      mov sb, r0
00612e84  05 00 a0 e1                                      mov r0, r5
00612e88  b7 ef f3 eb                                      bl #0x30ed6c
00612e8c  00 10 a0 e1                                      mov r1, r0
00612e90  09 00 a0 e1                                      mov r0, sb
00612e94  42 ef f3 eb                                      bl #0x30eba4
00612e98  0a 10 a0 e1                                      mov r1, sl
00612e9c  00 00 84 e5                                      str r0, [r4]
00612ea0  06 00 a0 e1                                      mov r0, r6
00612ea4  b0 ef f3 eb                                      bl #0x30ed6c
00612ea8  0c 10 9d e5                                      ldr r1, [sp, #0xc]
00612eac  00 a0 a0 e1                                      mov sl, r0
00612eb0  05 00 a0 e1                                      mov r0, r5
00612eb4  ac ef f3 eb                                      bl #0x30ed6c
00612eb8  00 10 a0 e1                                      mov r1, r0
00612ebc  0a 00 a0 e1                                      mov r0, sl
00612ec0  37 ef f3 eb                                      bl #0x30eba4
00612ec4  08 10 a0 e1                                      mov r1, r8
00612ec8  04 00 84 e5                                      str r0, [r4, #4]
00612ecc  06 00 a0 e1                                      mov r0, r6
00612ed0  a5 ef f3 eb                                      bl #0x30ed6c
00612ed4  08 10 9d e5                                      ldr r1, [sp, #8]
00612ed8  00 80 a0 e1                                      mov r8, r0
00612edc  05 00 a0 e1                                      mov r0, r5
00612ee0  a1 ef f3 eb                                      bl #0x30ed6c
00612ee4  00 10 a0 e1                                      mov r1, r0
00612ee8  08 00 a0 e1                                      mov r0, r8
00612eec  2c ef f3 eb                                      bl #0x30eba4
00612ef0  07 10 a0 e1                                      mov r1, r7
00612ef4  08 00 84 e5                                      str r0, [r4, #8]
00612ef8  06 00 a0 e1                                      mov r0, r6
00612efc  9a ef f3 eb                                      bl #0x30ed6c
00612f00  04 10 9d e5                                      ldr r1, [sp, #4]
00612f04  00 60 a0 e1                                      mov r6, r0
00612f08  05 00 a0 e1                                      mov r0, r5
00612f0c  34 00 00 ea                                      b #0x612fe4
00612f10  05 10 a0 e1                                      mov r1, r5
00612f14  3f 04 a0 e3                                      mov r0, #0x3f000000
00612f18  23 ed f3 eb                                      bl #0x30e3ac
00612f1c  db 1f 00 e3                                      movw r1, #0xfdb
00612f20  49 10 44 e3                                      movt r1, #0x4049
00612f24  90 ef f3 eb                                      bl #0x30ed6c
00612f28  f6 ee f3 eb                                      bl #0x30eb08
00612f2c  db 1f 00 e3                                      movw r1, #0xfdb
00612f30  00 60 a0 e1                                      mov r6, r0
00612f34  49 10 44 e3                                      movt r1, #0x4049
00612f38  05 00 a0 e1                                      mov r0, r5
00612f3c  8a ef f3 eb                                      bl #0x30ed6c
00612f40  f0 ee f3 eb                                      bl #0x30eb08
00612f44  09 10 a0 e1                                      mov r1, sb
00612f48  00 50 a0 e1                                      mov r5, r0
00612f4c  06 00 a0 e1                                      mov r0, r6
00612f50  85 ef f3 eb                                      bl #0x30ed6c
00612f54  05 10 a0 e1                                      mov r1, r5
00612f58  00 b0 a0 e1                                      mov fp, r0
00612f5c  02 01 8a e2                                      add r0, sl, #0x80000000
00612f60  81 ef f3 eb                                      bl #0x30ed6c
00612f64  00 10 a0 e1                                      mov r1, r0
00612f68  0b 00 a0 e1                                      mov r0, fp
00612f6c  0c ef f3 eb                                      bl #0x30eba4
00612f70  0a 10 a0 e1                                      mov r1, sl
00612f74  00 00 84 e5                                      str r0, [r4]
00612f78  06 00 a0 e1                                      mov r0, r6
00612f7c  7a ef f3 eb                                      bl #0x30ed6c
00612f80  09 10 a0 e1                                      mov r1, sb
00612f84  00 a0 a0 e1                                      mov sl, r0
00612f88  05 00 a0 e1                                      mov r0, r5
00612f8c  76 ef f3 eb                                      bl #0x30ed6c
00612f90  00 10 a0 e1                                      mov r1, r0
00612f94  0a 00 a0 e1                                      mov r0, sl
00612f98  01 ef f3 eb                                      bl #0x30eba4
00612f9c  08 10 a0 e1                                      mov r1, r8
00612fa0  04 00 84 e5                                      str r0, [r4, #4]
00612fa4  06 00 a0 e1                                      mov r0, r6
00612fa8  6f ef f3 eb                                      bl #0x30ed6c
00612fac  05 10 a0 e1                                      mov r1, r5
00612fb0  00 a0 a0 e1                                      mov sl, r0
00612fb4  02 01 87 e2                                      add r0, r7, #0x80000000
00612fb8  6b ef f3 eb                                      bl #0x30ed6c
00612fbc  00 10 a0 e1                                      mov r1, r0
00612fc0  0a 00 a0 e1                                      mov r0, sl
00612fc4  f6 ee f3 eb                                      bl #0x30eba4
00612fc8  07 10 a0 e1                                      mov r1, r7
00612fcc  08 00 84 e5                                      str r0, [r4, #8]
00612fd0  06 00 a0 e1                                      mov r0, r6
00612fd4  64 ef f3 eb                                      bl #0x30ed6c
00612fd8  08 10 a0 e1                                      mov r1, r8
00612fdc  00 60 a0 e1                                      mov r6, r0
00612fe0  05 00 a0 e1                                      mov r0, r5
00612fe4  60 ef f3 eb                                      bl #0x30ed6c
00612fe8  00 10 a0 e1                                      mov r1, r0
00612fec  06 00 a0 e1                                      mov r0, r6
00612ff0  eb ee f3 eb                                      bl #0x30eba4
00612ff4  0c 00 84 e5                                      str r0, [r4, #0xc]
00612ff8  04 00 a0 e1                                      mov r0, r4
00612ffc  1c d0 8d e2                                      add sp, sp, #0x1c
00613000  f0 4f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00613004  10 d0 8d e2                                      add sp, sp, #0x10
00613008  1e ff 2f e1                                      bx lr
0061300c  05 10 a0 e1                                      mov r1, r5
00613010  fe 05 a0 e3                                      mov r0, #0x3f800000
00613014  e4 ec f3 eb                                      bl #0x30e3ac
00613018  09 10 a0 e1                                      mov r1, sb
0061301c  00 60 a0 e1                                      mov r6, r0
00613020  51 ef f3 eb                                      bl #0x30ed6c
00613024  0b 10 a0 e1                                      mov r1, fp
00613028  00 90 a0 e1                                      mov sb, r0
0061302c  05 00 a0 e1                                      mov r0, r5
00613030  4d ef f3 eb                                      bl #0x30ed6c
00613034  00 10 a0 e1                                      mov r1, r0
00613038  09 00 a0 e1                                      mov r0, sb
0061303c  d8 ee f3 eb                                      bl #0x30eba4
00613040  0a 10 a0 e1                                      mov r1, sl
00613044  00 00 84 e5                                      str r0, [r4]
00613048  06 00 a0 e1                                      mov r0, r6
0061304c  46 ef f3 eb                                      bl #0x30ed6c
00613050  0c 10 9d e5                                      ldr r1, [sp, #0xc]
00613054  00 a0 a0 e1                                      mov sl, r0
00613058  05 00 a0 e1                                      mov r0, r5
0061305c  42 ef f3 eb                                      bl #0x30ed6c
00613060  00 10 a0 e1                                      mov r1, r0
00613064  0a 00 a0 e1                                      mov r0, sl
00613068  cd ee f3 eb                                      bl #0x30eba4
0061306c  08 10 a0 e1                                      mov r1, r8
00613070  04 00 84 e5                                      str r0, [r4, #4]
00613074  06 00 a0 e1                                      mov r0, r6
00613078  3b ef f3 eb                                      bl #0x30ed6c
0061307c  08 10 9d e5                                      ldr r1, [sp, #8]
00613080  00 80 a0 e1                                      mov r8, r0
00613084  05 00 a0 e1                                      mov r0, r5
00613088  37 ef f3 eb                                      bl #0x30ed6c
0061308c  00 10 a0 e1                                      mov r1, r0
00613090  08 00 a0 e1                                      mov r0, r8
00613094  c2 ee f3 eb                                      bl #0x30eba4
00613098  07 10 a0 e1                                      mov r1, r7
0061309c  08 00 84 e5                                      str r0, [r4, #8]
006130a0  06 00 a0 e1                                      mov r0, r6
006130a4  30 ef f3 eb                                      bl #0x30ed6c
006130a8  04 10 9d e5                                      ldr r1, [sp, #4]
006130ac  00 60 a0 e1                                      mov r6, r0
006130b0  05 00 a0 e1                                      mov r0, r5
006130b4  2c ef f3 eb                                      bl #0x30ed6c
006130b8  00 10 a0 e1                                      mov r1, r0
006130bc  06 00 a0 e1                                      mov r0, r6
006130c0  b7 ee f3 eb                                      bl #0x30eba4
006130c4  0c 00 84 e5                                      str r0, [r4, #0xc]
006130c8  04 00 a0 e1                                      mov r0, r4
006130cc  07 26 f5 eb                                      bl #0x35c8f0
006130d0  c8 ff ff ea                                      b #0x612ff8

