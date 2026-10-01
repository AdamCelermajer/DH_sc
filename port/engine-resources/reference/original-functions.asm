; Original ELF virtual addresses. Annotated evidence, not assembler-ready source.

; FUNCTION 0x0056eb6c, declared_size=20, range_size=20, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZN6glitch2io15CMemoryReadFile9getBufferEPl
; demangled: glitch::io::CMemoryReadFile::getBuffer(long*)
; decoder-mode: arm
0056eb6c  00 00 51 e3                                      cmp r1, #0
0056eb70  1c 30 90 15                                      ldrne r3, [r0, #0x1c]
0056eb74  00 30 81 15                                      strne r3, [r1]
0056eb78  0c 00 90 e5                                      ldr r0, [r0, #0xc]
0056eb7c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056eb80, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile13isAllInMemoryEv
; demangled: glitch::io::CMemoryReadFile::isAllInMemory() const
; decoder-mode: arm
0056eb80  01 00 a0 e3                                      mov r0, #1
0056eb84  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056eb8c, declared_size=32, range_size=32, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile7isValidEv
; demangled: glitch::io::CMemoryReadFile::isValid() const
; decoder-mode: arm
0056eb8c  0c 30 90 e5                                      ldr r3, [r0, #0xc]
0056eb90  00 00 53 e3                                      cmp r3, #0
0056eb94  03 00 a0 01                                      moveq r0, r3
0056eb98  1e ff 2f 01                                      bxeq lr
0056eb9c  18 00 90 e5                                      ldr r0, [r0, #0x18]
0056eba0  00 00 e0 e1                                      mvn r0, r0
0056eba4  a0 0f a0 e1                                      lsr r0, r0, #0x1f
0056eba8  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ebac, declared_size=52, range_size=52, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZN6glitch2io15CMemoryReadFile9readAsyncEPvjPFviiPNS0_9IReadFileES2_ES2_
; demangled: glitch::io::CMemoryReadFile::readAsync(void*, unsigned int, void (*)(int, int, glitch::io::IReadFile*, void*), void*)
; decoder-mode: arm
0056ebac  70 40 2d e9                                      push {r4, r5, r6, lr}
0056ebb0  00 c0 90 e5                                      ldr ip, [r0]
0056ebb4  03 50 a0 e1                                      mov r5, r3
0056ebb8  00 40 a0 e1                                      mov r4, r0
0056ebbc  0f e0 a0 e1                                      mov lr, pc
0056ebc0  0c f0 9c e5                                      ldr pc, [ip, #0xc]
0056ebc4  04 20 a0 e1                                      mov r2, r4
0056ebc8  01 10 70 e2                                      rsbs r1, r0, #1
0056ebcc  00 10 a0 33                                      movlo r1, #0
0056ebd0  10 30 9d e5                                      ldr r3, [sp, #0x10]
0056ebd4  35 ff 2f e1                                      blx r5
0056ebd8  01 00 a0 e3                                      mov r0, #1
0056ebdc  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0056ebe0, declared_size=92, range_size=92, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZN6glitch2io15CMemoryReadFile9readAsyncEPvjlPFviiPNS0_9IReadFileES2_ES2_
; demangled: glitch::io::CMemoryReadFile::readAsync(void*, unsigned int, long, void (*)(int, int, glitch::io::IReadFile*, void*), void*)
; decoder-mode: arm
0056ebe0  70 40 2d e9                                      push {r4, r5, r6, lr}
0056ebe4  00 40 a0 e1                                      mov r4, r0
0056ebe8  01 60 a0 e1                                      mov r6, r1
0056ebec  02 50 a0 e1                                      mov r5, r2
0056ebf0  03 10 a0 e1                                      mov r1, r3
0056ebf4  00 20 a0 e3                                      mov r2, #0
0056ebf8  00 30 90 e5                                      ldr r3, [r0]
0056ebfc  0f e0 a0 e1                                      mov lr, pc
0056ec00  18 f0 93 e5                                      ldr pc, [r3, #0x18]
0056ec04  06 10 a0 e1                                      mov r1, r6
0056ec08  05 20 a0 e1                                      mov r2, r5
0056ec0c  00 30 94 e5                                      ldr r3, [r4]
0056ec10  04 00 a0 e1                                      mov r0, r4
0056ec14  0f e0 a0 e1                                      mov lr, pc
0056ec18  0c f0 93 e5                                      ldr pc, [r3, #0xc]
0056ec1c  04 20 a0 e1                                      mov r2, r4
0056ec20  01 10 70 e2                                      rsbs r1, r0, #1
0056ec24  00 10 a0 33                                      movlo r1, #0
0056ec28  14 30 9d e5                                      ldr r3, [sp, #0x14]
0056ec2c  0f e0 a0 e1                                      mov lr, pc
0056ec30  10 f0 9d e5                                      ldr pc, [sp, #0x10]
0056ec34  01 00 a0 e3                                      mov r0, #1
0056ec38  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0056ec3c, declared_size=60, range_size=60, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZN6glitch2io15CMemoryReadFile4seekElb
; demangled: glitch::io::CMemoryReadFile::seek(long, bool)
; decoder-mode: arm
0056ec3c  00 00 52 e3                                      cmp r2, #0
0056ec40  06 00 00 0a                                      beq #0x56ec60
0056ec44  1c 20 90 e5                                      ldr r2, [r0, #0x1c]
0056ec48  18 30 90 e5                                      ldr r3, [r0, #0x18]
0056ec4c  02 10 81 e0                                      add r1, r1, r2
0056ec50  03 00 51 e1                                      cmp r1, r3
0056ec54  04 00 00 da                                      ble #0x56ec6c
0056ec58  00 00 a0 e3                                      mov r0, #0
0056ec5c  1e ff 2f e1                                      bx lr
0056ec60  18 30 90 e5                                      ldr r3, [r0, #0x18]
0056ec64  03 00 51 e1                                      cmp r1, r3
0056ec68  fa ff ff ca                                      bgt #0x56ec58
0056ec6c  1c 10 80 e5                                      str r1, [r0, #0x1c]
0056ec70  01 00 a0 e3                                      mov r0, #1
0056ec74  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ec78, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile7getSizeEv
; demangled: glitch::io::CMemoryReadFile::getSize() const
; decoder-mode: arm
0056ec78  18 00 90 e5                                      ldr r0, [r0, #0x18]
0056ec7c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ec80, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile6getPosEv
; demangled: glitch::io::CMemoryReadFile::getPos() const
; decoder-mode: arm
0056ec80  1c 00 90 e5                                      ldr r0, [r0, #0x1c]
0056ec84  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ec88, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile11getFileNameEv
; demangled: glitch::io::CMemoryReadFile::getFileName() const
; decoder-mode: arm
0056ec88  34 00 90 e5                                      ldr r0, [r0, #0x34]
0056ec8c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ec90, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZNK6glitch2io15CMemoryReadFile11getFullPathEv
; demangled: glitch::io::CMemoryReadFile::getFullPath() const
; decoder-mode: arm
0056ec90  34 00 90 e5                                      ldr r0, [r0, #0x34]
0056ec94  1e ff 2f e1                                      bx lr


; FUNCTION 0x0056ecc4, declared_size=88, range_size=88, mode=arm
; class-group: glitch::io::CMemoryReadFile
; alias: _ZN6glitch2io15CMemoryReadFile4readEPvj
; demangled: glitch::io::CMemoryReadFile::read(void*, unsigned int)
; decoder-mode: arm
0056ecc4  70 40 2d e9                                      push {r4, r5, r6, lr}
0056ecc8  1c 30 90 e5                                      ldr r3, [r0, #0x1c]
0056eccc  18 c0 90 e5                                      ldr ip, [r0, #0x18]
0056ecd0  02 40 a0 e1                                      mov r4, r2
0056ecd4  03 20 82 e0                                      add r2, r2, r3
0056ecd8  0c 00 52 e1                                      cmp r2, ip
0056ecdc  0c 40 84 c0                                      addgt r4, r4, ip
0056ece0  04 40 62 c0                                      rsbgt r4, r2, r4
0056ece4  00 00 54 e3                                      cmp r4, #0
0056ece8  00 50 a0 e1                                      mov r5, r0
0056ecec  00 40 a0 d3                                      movle r4, #0
0056ecf0  07 00 00 da                                      ble #0x56ed14
0056ecf4  0c c0 90 e5                                      ldr ip, [r0, #0xc]
0056ecf8  04 20 a0 e1                                      mov r2, r4
0056ecfc  01 00 a0 e1                                      mov r0, r1
0056ed00  03 10 8c e0                                      add r1, ip, r3
0056ed04  d7 7e f6 eb                                      bl #0x30e868
0056ed08  1c 30 95 e5                                      ldr r3, [r5, #0x1c]
0056ed0c  04 30 83 e0                                      add r3, r3, r4
0056ed10  1c 30 85 e5                                      str r3, [r5, #0x1c]
0056ed14  04 00 a0 e1                                      mov r0, r4
0056ed18  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0060e2a8, declared_size=20, range_size=20, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase10getVersionEv
; demangled: glitch::collada::CColladaDatabase::getVersion() const
; decoder-mode: arm
0060e2a8  00 30 90 e5                                      ldr r3, [r0]
0060e2ac  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e2b0  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e2b4  00 00 93 e5                                      ldr r0, [r3]
0060e2b8  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e334, declared_size=20, range_size=20, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase23getAnimationClipLibraryEv
; demangled: glitch::collada::CColladaDatabase::getAnimationClipLibrary() const
; decoder-mode: arm
0060e334  00 30 90 e5                                      ldr r3, [r0]
0060e338  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e33c  20 00 93 e5                                      ldr r0, [r3, #0x20]
0060e340  34 00 80 e2                                      add r0, r0, #0x34
0060e344  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e348, declared_size=20, range_size=20, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase8getSceneEv
; demangled: glitch::collada::CColladaDatabase::getScene() const
; decoder-mode: arm
0060e348  00 30 90 e5                                      ldr r3, [r0]
0060e34c  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e350  20 00 93 e5                                      ldr r0, [r3, #0x20]
0060e354  b8 00 80 e2                                      add r0, r0, #0xb8
0060e358  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e35c, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase12getAnimationEi
; demangled: glitch::collada::CColladaDatabase::getAnimation(int) const
; decoder-mode: arm
0060e35c  00 30 90 e5                                      ldr r3, [r0]
0060e360  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e364  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e368  28 00 93 e5                                      ldr r0, [r3, #0x28]
0060e36c  81 02 80 e0                                      add r0, r0, r1, lsl #5
0060e370  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e374, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase16getAnimationClipEi
; demangled: glitch::collada::CColladaDatabase::getAnimationClip(int) const
; decoder-mode: arm
0060e374  00 30 90 e5                                      ldr r3, [r0]
0060e378  0c 00 a0 e3                                      mov r0, #0xc
0060e37c  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e380  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e384  38 30 93 e5                                      ldr r3, [r3, #0x38]
0060e388  90 31 20 e0                                      mla r0, r0, r1, r3
0060e38c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e390, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase9getCameraEi
; demangled: glitch::collada::CColladaDatabase::getCamera(int) const
; decoder-mode: arm
0060e390  00 30 90 e5                                      ldr r3, [r0]
0060e394  1c 00 a0 e3                                      mov r0, #0x1c
0060e398  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e39c  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e3a0  40 30 93 e5                                      ldr r3, [r3, #0x40]
0060e3a4  90 31 20 e0                                      mla r0, r0, r1, r3
0060e3a8  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e3ac, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase8getLightEi
; demangled: glitch::collada::CColladaDatabase::getLight(int) const
; decoder-mode: arm
0060e3ac  00 30 90 e5                                      ldr r3, [r0]
0060e3b0  18 00 a0 e3                                      mov r0, #0x18
0060e3b4  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e3b8  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e3bc  48 30 93 e5                                      ldr r3, [r3, #0x48]
0060e3c0  90 31 20 e0                                      mla r0, r0, r1, r3
0060e3c4  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e3c8, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase8getImageEi
; demangled: glitch::collada::CColladaDatabase::getImage(int) const
; decoder-mode: arm
0060e3c8  00 30 90 e5                                      ldr r3, [r0]
0060e3cc  14 00 a0 e3                                      mov r0, #0x14
0060e3d0  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e3d4  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e3d8  50 30 93 e5                                      ldr r3, [r3, #0x50]
0060e3dc  90 31 20 e0                                      mla r0, r0, r1, r3
0060e3e0  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e3e4, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase9getEffectEi
; demangled: glitch::collada::CColladaDatabase::getEffect(int) const
; decoder-mode: arm
0060e3e4  00 30 90 e5                                      ldr r3, [r0]
0060e3e8  74 00 a0 e3                                      mov r0, #0x74
0060e3ec  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e3f0  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e3f4  58 30 93 e5                                      ldr r3, [r3, #0x58]
0060e3f8  90 31 20 e0                                      mla r0, r0, r1, r3
0060e3fc  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e400, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase11getMaterialEi
; demangled: glitch::collada::CColladaDatabase::getMaterial(int) const
; decoder-mode: arm
0060e400  00 30 90 e5                                      ldr r3, [r0]
0060e404  24 00 a0 e3                                      mov r0, #0x24
0060e408  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e40c  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e410  60 30 93 e5                                      ldr r3, [r3, #0x60]
0060e414  90 31 20 e0                                      mla r0, r0, r1, r3
0060e418  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e41c, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase11getGeometryEi
; demangled: glitch::collada::CColladaDatabase::getGeometry(int) const
; decoder-mode: arm
0060e41c  00 30 90 e5                                      ldr r3, [r0]
0060e420  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e424  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e428  6c 00 93 e5                                      ldr r0, [r3, #0x6c]
0060e42c  01 02 80 e0                                      add r0, r0, r1, lsl #4
0060e430  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e434, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase13getControllerEi
; demangled: glitch::collada::CColladaDatabase::getController(int) const
; decoder-mode: arm
0060e434  00 30 90 e5                                      ldr r3, [r0]
0060e438  0c 00 a0 e3                                      mov r0, #0xc
0060e43c  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e440  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e444  74 30 93 e5                                      ldr r3, [r3, #0x74]
0060e448  90 31 20 e0                                      mla r0, r0, r1, r3
0060e44c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e450, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase8getForceEi
; demangled: glitch::collada::CColladaDatabase::getForce(int) const
; decoder-mode: arm
0060e450  00 30 90 e5                                      ldr r3, [r0]
0060e454  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e458  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e45c  8c 00 93 e5                                      ldr r0, [r3, #0x8c]
0060e460  01 02 80 e0                                      add r0, r0, r1, lsl #4
0060e464  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e468, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase10getEmitterEi
; demangled: glitch::collada::CColladaDatabase::getEmitter(int) const
; decoder-mode: arm
0060e468  00 30 90 e5                                      ldr r3, [r0]
0060e46c  90 00 a0 e3                                      mov r0, #0x90
0060e470  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e474  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e478  7c 30 93 e5                                      ldr r3, [r3, #0x7c]
0060e47c  90 31 20 e0                                      mla r0, r0, r1, r3
0060e480  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e484, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase14getGNPSEmitterEi
; demangled: glitch::collada::CColladaDatabase::getGNPSEmitter(int) const
; decoder-mode: arm
0060e484  00 30 90 e5                                      ldr r3, [r0]
0060e488  e8 00 a0 e3                                      mov r0, #0xe8
0060e48c  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e490  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e494  84 30 93 e5                                      ldr r3, [r3, #0x84]
0060e498  90 31 20 e0                                      mla r0, r0, r1, r3
0060e49c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0060e4a0, declared_size=28, range_size=28, mode=arm
; class-group: glitch::collada::CColladaDatabase
; alias: _ZNK6glitch7collada16CColladaDatabase10getCoronasEi
; demangled: glitch::collada::CColladaDatabase::getCoronas(int) const
; decoder-mode: arm
0060e4a0  00 30 90 e5                                      ldr r3, [r0]
0060e4a4  24 00 a0 e3                                      mov r0, #0x24
0060e4a8  24 30 93 e5                                      ldr r3, [r3, #0x24]
0060e4ac  20 30 93 e5                                      ldr r3, [r3, #0x20]
0060e4b0  94 30 93 e5                                      ldr r3, [r3, #0x94]
0060e4b4  90 31 20 e0                                      mla r0, r0, r1, r3
0060e4b8  1e ff 2f e1                                      bx lr


; FUNCTION 0x0069a40c, declared_size=1108, range_size=1108, mode=arm
; class-group: glitch::res::File
; alias: _ZN6glitch3res4File4InitEv
; demangled: glitch::res::File::Init()
; decoder-mode: arm
0069a40c  f0 0f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp}
0069a410  00 30 90 e5                                      ldr r3, [r0]
0069a414  34 24 9f e5                                      ldr r2, [pc, #0x434]
0069a418  34 14 9f e5                                      ldr r1, [pc, #0x434]
0069a41c  0c 40 93 e5                                      ldr r4, [r3, #0xc]
0069a420  02 20 8f e0                                      add r2, pc, r2
0069a424  01 50 92 e7                                      ldr r5, [r2, r1]
0069a428  0c 40 80 e5                                      str r4, [r0, #0xc]
0069a42c  38 c0 93 e5                                      ldr ip, [r3, #0x38]
0069a430  28 d0 4d e2                                      sub sp, sp, #0x28
0069a434  28 c0 80 e5                                      str ip, [r0, #0x28]
0069a438  2c c0 93 e5                                      ldr ip, [r3, #0x2c]
0069a43c  10 c0 80 e5                                      str ip, [r0, #0x10]
0069a440  38 60 93 e5                                      ldr r6, [r3, #0x38]
0069a444  04 c0 6c e0                                      rsb ip, ip, r4
0069a448  0c c0 66 e0                                      rsb ip, r6, ip
0069a44c  18 c0 80 e5                                      str ip, [r0, #0x18]
0069a450  30 c0 93 e5                                      ldr ip, [r3, #0x30]
0069a454  14 c0 80 e5                                      str ip, [r0, #0x14]
0069a458  14 c0 93 e5                                      ldr ip, [r3, #0x14]
0069a45c  ac cf a0 e1                                      lsr ip, ip, #0x1f
0069a460  0c 31 85 e7                                      str r3, [r5, ip, lsl #2]
0069a464  d0 c0 d3 e1                                      ldrsb ip, [r3]
0069a468  42 00 5c e3                                      cmp ip, #0x42
0069a46c  03 00 00 0a                                      beq #0x69a480
0069a470  00 00 e0 e3                                      mvn r0, #0
0069a474  28 d0 8d e2                                      add sp, sp, #0x28
0069a478  f0 0f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp}
0069a47c  1e ff 2f e1                                      bx lr
0069a480  d1 c0 d3 e1                                      ldrsb ip, [r3, #1]
0069a484  52 00 5c e3                                      cmp ip, #0x52
0069a488  f8 ff ff 1a                                      bne #0x69a470
0069a48c  d2 c0 d3 e1                                      ldrsb ip, [r3, #2]
0069a490  45 00 5c e3                                      cmp ip, #0x45
0069a494  f5 ff ff 1a                                      bne #0x69a470
0069a498  d3 c0 d3 e1                                      ldrsb ip, [r3, #3]
0069a49c  53 00 5c e3                                      cmp ip, #0x53
0069a4a0  f2 ff ff 1a                                      bne #0x69a470
0069a4a4  b6 c0 d3 e1                                      ldrh ip, [r3, #6]
0069a4a8  02 09 1c e3                                      tst ip, #0x8000
0069a4ac  e5 00 00 1a                                      bne #0x69a848
0069a4b0  8c c8 e0 e1                                      mvn ip, ip, lsl #17
0069a4b4  ac c8 e0 e1                                      mvn ip, ip, lsr #17
0069a4b8  b6 c0 c3 e1                                      strh ip, [r3, #6]
0069a4bc  08 c0 90 e5                                      ldr ip, [r0, #8]
0069a4c0  00 00 5c e3                                      cmp ip, #0
0069a4c4  ce 00 00 0a                                      beq #0x69a804
0069a4c8  88 43 9f e5                                      ldr r4, [pc, #0x388]
0069a4cc  88 53 9f e5                                      ldr r5, [pc, #0x388]
0069a4d0  18 40 8d e5                                      str r4, [sp, #0x18]
0069a4d4  18 60 9d e5                                      ldr r6, [sp, #0x18]
0069a4d8  08 40 93 e5                                      ldr r4, [r3, #8]
0069a4dc  1c 50 8d e5                                      str r5, [sp, #0x1c]
0069a4e0  06 50 92 e7                                      ldr r5, [r2, r6]
0069a4e4  18 c0 83 e5                                      str ip, [r3, #0x18]
0069a4e8  1c 70 9d e5                                      ldr r7, [sp, #0x1c]
0069a4ec  10 60 93 e5                                      ldr r6, [r3, #0x10]
0069a4f0  00 40 85 e5                                      str r4, [r5]
0069a4f4  14 50 93 e5                                      ldr r5, [r3, #0x14]
0069a4f8  07 c0 92 e7                                      ldr ip, [r2, r7]
0069a4fc  06 61 84 e0                                      add r6, r4, r6, lsl #2
0069a500  a5 4f a0 e1                                      lsr r4, r5, #0x1f
0069a504  14 60 8d e5                                      str r6, [sp, #0x14]
0069a508  04 61 8c e7                                      str r6, [ip, r4, lsl #2]
0069a50c  10 40 93 e5                                      ldr r4, [r3, #0x10]
0069a510  00 50 a0 e3                                      mov r5, #0
0069a514  04 00 55 e1                                      cmp r5, r4
0069a518  ca 00 00 2a                                      bhs #0x69a848
0069a51c  18 70 93 e5                                      ldr r7, [r3, #0x18]
0069a520  14 80 93 e5                                      ldr r8, [r3, #0x14]
0069a524  0c a0 90 e5                                      ldr sl, [r0, #0xc]
0069a528  05 61 97 e7                                      ldr r6, [r7, r5, lsl #2]
0069a52c  14 90 9d e5                                      ldr sb, [sp, #0x14]
0069a530  05 b1 a0 e1                                      lsl fp, r5, #2
0069a534  06 c0 68 e0                                      rsb ip, r8, r6
0069a538  0a 00 5c e1                                      cmp ip, sl
0069a53c  10 90 8d e5                                      str sb, [sp, #0x10]
0069a540  a1 00 00 8a                                      bhi #0x69a7cc
0069a544  09 a0 a0 e1                                      mov sl, sb
0069a548  00 90 a0 e3                                      mov sb, #0
0069a54c  24 30 8d e5                                      str r3, [sp, #0x24]
0069a550  20 90 8d e5                                      str sb, [sp, #0x20]
0069a554  0c 00 5a e1                                      cmp sl, ip
0069a558  4f 00 00 8a                                      bhi #0x69a69c
0069a55c  18 90 90 e5                                      ldr sb, [r0, #0x18]
0069a560  09 00 5c e1                                      cmp ip, sb
0069a564  0c 90 8d e5                                      str sb, [sp, #0xc]
0069a568  7b 00 00 9a                                      bls #0x69a75c
0069a56c  fc 8f 0f e3                                      movw r8, #0xfffc
0069a570  ff 8f 4f e3                                      movt r8, #0xffff
0069a574  14 a0 90 e5                                      ldr sl, [r0, #0x14]
0069a578  08 80 69 e0                                      rsb r8, sb, r8
0069a57c  0c 80 88 e0                                      add r8, r8, ip
0069a580  a8 01 5a e1                                      cmp sl, r8, lsr #3
0069a584  42 00 00 aa                                      bge #0x69a694
0069a588  01 a0 4a e2                                      sub sl, sl, #1
0069a58c  24 a0 8d e5                                      str sl, [sp, #0x24]
0069a590  1c 40 90 e5                                      ldr r4, [r0, #0x1c]
0069a594  04 a0 a0 e3                                      mov sl, #4
0069a598  00 80 a0 e3                                      mov r8, #0
0069a59c  05 90 a0 e1                                      mov sb, r5
0069a5a0  04 30 8d e5                                      str r3, [sp, #4]
0069a5a4  24 30 9d e5                                      ldr r3, [sp, #0x24]
0069a5a8  0a 50 84 e0                                      add r5, r4, sl
0069a5ac  0c 50 8d e5                                      str r5, [sp, #0xc]
0069a5b0  03 00 58 e1                                      cmp r8, r3
0069a5b4  09 00 00 aa                                      bge #0x69a5e0
0069a5b8  0a 50 94 e7                                      ldr r5, [r4, sl]
0069a5bc  08 a0 8a e2                                      add sl, sl, #8
0069a5c0  05 00 5c e1                                      cmp ip, r5
0069a5c4  03 00 00 9a                                      bls #0x69a5d8
0069a5c8  0c 30 9d e5                                      ldr r3, [sp, #0xc]
0069a5cc  08 50 93 e5                                      ldr r5, [r3, #8]
0069a5d0  05 00 5c e1                                      cmp ip, r5
0069a5d4  01 00 00 3a                                      blo #0x69a5e0
0069a5d8  01 80 88 e2                                      add r8, r8, #1
0069a5dc  f0 ff ff ea                                      b #0x69a5a4
0069a5e0  20 a0 90 e5                                      ldr sl, [r0, #0x20]
0069a5e4  88 41 84 e0                                      add r4, r4, r8, lsl #3
0069a5e8  04 40 94 e5                                      ldr r4, [r4, #4]
0069a5ec  08 81 9a e7                                      ldr r8, [sl, r8, lsl #2]
0069a5f0  04 30 9d e5                                      ldr r3, [sp, #4]
0069a5f4  09 50 a0 e1                                      mov r5, sb
0069a5f8  08 80 64 e0                                      rsb r8, r4, r8
0069a5fc  06 40 88 e0                                      add r4, r8, r6
0069a600  0b 40 87 e7                                      str r4, [r7, fp]
0069a604  06 40 98 e7                                      ldr r4, [r8, r6]
0069a608  0c 40 8d e5                                      str r4, [sp, #0xc]
0069a60c  14 40 93 e5                                      ldr r4, [r3, #0x14]
0069a610  0c 90 9d e5                                      ldr sb, [sp, #0xc]
0069a614  18 a0 90 e5                                      ldr sl, [r0, #0x18]
0069a618  09 40 64 e0                                      rsb r4, r4, sb
0069a61c  0a 00 54 e1                                      cmp r4, sl
0069a620  21 00 00 9a                                      bls #0x69a6ac
0069a624  14 40 90 e5                                      ldr r4, [r0, #0x14]
0069a628  04 a0 a0 e3                                      mov sl, #4
0069a62c  00 70 a0 e3                                      mov r7, #0
0069a630  01 40 44 e2                                      sub r4, r4, #1
0069a634  10 40 8d e5                                      str r4, [sp, #0x10]
0069a638  1c 40 90 e5                                      ldr r4, [r0, #0x1c]
0069a63c  10 90 9d e5                                      ldr sb, [sp, #0x10]
0069a640  0a b0 84 e0                                      add fp, r4, sl
0069a644  09 00 57 e1                                      cmp r7, sb
0069a648  08 00 00 aa                                      bge #0x69a670
0069a64c  0a 90 94 e7                                      ldr sb, [r4, sl]
0069a650  08 a0 8a e2                                      add sl, sl, #8
0069a654  09 00 5c e1                                      cmp ip, sb
0069a658  02 00 00 9a                                      bls #0x69a668
0069a65c  08 90 9b e5                                      ldr sb, [fp, #8]
0069a660  09 00 5c e1                                      cmp ip, sb
0069a664  01 00 00 3a                                      blo #0x69a670
0069a668  01 70 87 e2                                      add r7, r7, #1
0069a66c  f2 ff ff ea                                      b #0x69a63c
0069a670  20 c0 90 e5                                      ldr ip, [r0, #0x20]
0069a674  87 41 84 e0                                      add r4, r4, r7, lsl #3
0069a678  04 40 94 e5                                      ldr r4, [r4, #4]
0069a67c  07 c1 9c e7                                      ldr ip, [ip, r7, lsl #2]
0069a680  0c a0 9d e5                                      ldr sl, [sp, #0xc]
0069a684  0c c0 64 e0                                      rsb ip, r4, ip
0069a688  0a c0 8c e0                                      add ip, ip, sl
0069a68c  06 c0 88 e7                                      str ip, [r8, r6]
0069a690  10 40 93 e5                                      ldr r4, [r3, #0x10]
0069a694  01 50 85 e2                                      add r5, r5, #1
0069a698  9d ff ff ea                                      b #0x69a514
0069a69c  24 a0 9d e5                                      ldr sl, [sp, #0x24]
0069a6a0  0a 80 68 e0                                      rsb r8, r8, sl
0069a6a4  06 60 88 e0                                      add r6, r8, r6
0069a6a8  0b 60 87 e7                                      str r6, [r7, fp]
0069a6ac  20 c0 9d e5                                      ldr ip, [sp, #0x20]
0069a6b0  00 00 5c e3                                      cmp ip, #0
0069a6b4  3b 00 00 1a                                      bne #0x69a7a8
0069a6b8  00 00 55 e3                                      cmp r5, #0
0069a6bc  39 00 00 0a                                      beq #0x69a7a8
0069a6c0  0b 60 97 e7                                      ldr r6, [r7, fp]
0069a6c4  14 40 93 e5                                      ldr r4, [r3, #0x14]
0069a6c8  0c 80 90 e5                                      ldr r8, [r0, #0xc]
0069a6cc  00 70 96 e5                                      ldr r7, [r6]
0069a6d0  07 c0 64 e0                                      rsb ip, r4, r7
0069a6d4  08 00 5c e1                                      cmp ip, r8
0069a6d8  1c 90 9d 85                                      ldrhi sb, [sp, #0x1c]
0069a6dc  04 c0 8c 80                                      addhi ip, ip, r4
0069a6e0  ac 4f a0 81                                      lsrhi r4, ip, #0x1f
0069a6e4  09 a0 92 87                                      ldrhi sl, [r2, sb]
0069a6e8  01 80 92 87                                      ldrhi r8, [r2, r1]
0069a6ec  03 80 a0 91                                      movls r8, r3
0069a6f0  04 a1 9a 87                                      ldrhi sl, [sl, r4, lsl #2]
0069a6f4  04 81 98 87                                      ldrhi r8, [r8, r4, lsl #2]
0069a6f8  02 41 0c 82                                      andhi r4, ip, #0x80000000
0069a6fc  10 a0 8d 85                                      strhi sl, [sp, #0x10]
0069a700  10 a0 9d e5                                      ldr sl, [sp, #0x10]
0069a704  0c 00 5a e1                                      cmp sl, ip
0069a708  08 40 64 80                                      rsbhi r4, r4, r8
0069a70c  07 70 84 80                                      addhi r7, r4, r7
0069a710  00 70 86 85                                      strhi r7, [r6]
0069a714  23 00 00 8a                                      bhi #0x69a7a8
0069a718  18 a0 90 e5                                      ldr sl, [r0, #0x18]
0069a71c  0a 00 5c e1                                      cmp ip, sl
0069a720  14 a0 90 85                                      ldrhi sl, [r0, #0x14]
0069a724  04 80 a0 83                                      movhi r8, #4
0069a728  00 40 a0 83                                      movhi r4, #0
0069a72c  14 00 00 8a                                      bhi #0x69a784
0069a730  18 90 9d e5                                      ldr sb, [sp, #0x18]
0069a734  10 a0 9d e5                                      ldr sl, [sp, #0x10]
0069a738  09 c0 92 e7                                      ldr ip, [r2, sb]
0069a73c  00 c0 9c e5                                      ldr ip, [ip]
0069a740  0c c0 6a e0                                      rsb ip, sl, ip
0069a744  0c c0 64 e0                                      rsb ip, r4, ip
0069a748  0c 80 88 e0                                      add r8, r8, ip
0069a74c  07 70 88 e0                                      add r7, r8, r7
0069a750  00 70 86 e5                                      str r7, [r6]
0069a754  10 40 93 e5                                      ldr r4, [r3, #0x10]
0069a758  cd ff ff ea                                      b #0x69a694
0069a75c  18 40 9d e5                                      ldr r4, [sp, #0x18]
0069a760  24 90 9d e5                                      ldr sb, [sp, #0x24]
0069a764  04 c0 92 e7                                      ldr ip, [r2, r4]
0069a768  00 c0 9c e5                                      ldr ip, [ip]
0069a76c  0c c0 6a e0                                      rsb ip, sl, ip
0069a770  0c c0 68 e0                                      rsb ip, r8, ip
0069a774  0c c0 89 e0                                      add ip, sb, ip
0069a778  06 c0 8c e0                                      add ip, ip, r6
0069a77c  0b c0 87 e7                                      str ip, [r7, fp]
0069a780  c9 ff ff ea                                      b #0x69a6ac
0069a784  0a 00 54 e1                                      cmp r4, sl
0069a788  08 00 00 aa                                      bge #0x69a7b0
0069a78c  1c 90 90 e5                                      ldr sb, [r0, #0x1c]
0069a790  08 90 99 e7                                      ldr sb, [sb, r8]
0069a794  08 80 88 e2                                      add r8, r8, #8
0069a798  0c 00 59 e1                                      cmp sb, ip
0069a79c  03 00 00 0a                                      beq #0x69a7b0
0069a7a0  01 40 84 e2                                      add r4, r4, #1
0069a7a4  f6 ff ff ea                                      b #0x69a784
0069a7a8  10 40 93 e5                                      ldr r4, [r3, #0x10]
0069a7ac  b8 ff ff ea                                      b #0x69a694
0069a7b0  20 80 90 e5                                      ldr r8, [r0, #0x20]
0069a7b4  04 41 98 e7                                      ldr r4, [r8, r4, lsl #2]
0069a7b8  04 c0 6c e0                                      rsb ip, ip, r4
0069a7bc  07 70 8c e0                                      add r7, ip, r7
0069a7c0  00 70 86 e5                                      str r7, [r6]
0069a7c4  10 40 93 e5                                      ldr r4, [r3, #0x10]
0069a7c8  b1 ff ff ea                                      b #0x69a694
0069a7cc  01 a0 92 e7                                      ldr sl, [r2, r1]
0069a7d0  1c 90 9d e5                                      ldr sb, [sp, #0x1c]
0069a7d4  08 c0 8c e0                                      add ip, ip, r8
0069a7d8  0c a0 8d e5                                      str sl, [sp, #0xc]
0069a7dc  09 a0 92 e7                                      ldr sl, [r2, sb]
0069a7e0  0c 90 9d e5                                      ldr sb, [sp, #0xc]
0069a7e4  ac 8f a0 e1                                      lsr r8, ip, #0x1f
0069a7e8  08 a1 9a e7                                      ldr sl, [sl, r8, lsl #2]
0069a7ec  08 81 99 e7                                      ldr r8, [sb, r8, lsl #2]
0069a7f0  01 90 a0 e3                                      mov sb, #1
0069a7f4  20 90 8d e5                                      str sb, [sp, #0x20]
0069a7f8  24 80 8d e5                                      str r8, [sp, #0x24]
0069a7fc  02 81 0c e2                                      and r8, ip, #0x80000000
0069a800  53 ff ff ea                                      b #0x69a554
0069a804  18 10 93 e5                                      ldr r1, [r3, #0x18]
0069a808  0c 20 a0 e1                                      mov r2, ip
0069a80c  01 10 83 e0                                      add r1, r3, r1
0069a810  18 10 83 e5                                      str r1, [r3, #0x18]
0069a814  10 10 93 e5                                      ldr r1, [r3, #0x10]
0069a818  01 00 52 e1                                      cmp r2, r1
0069a81c  09 00 00 2a                                      bhs #0x69a848
0069a820  18 00 93 e5                                      ldr r0, [r3, #0x18]
0069a824  00 00 52 e3                                      cmp r2, #0
0069a828  02 11 90 e7                                      ldr r1, [r0, r2, lsl #2]
0069a82c  01 c0 83 e0                                      add ip, r3, r1
0069a830  02 c1 80 e7                                      str ip, [r0, r2, lsl #2]
0069a834  01 00 93 17                                      ldrne r0, [r3, r1]
0069a838  01 20 82 e2                                      add r2, r2, #1
0069a83c  00 00 83 10                                      addne r0, r3, r0
0069a840  01 00 83 17                                      strne r0, [r3, r1]
0069a844  f2 ff ff ea                                      b #0x69a814
0069a848  00 00 a0 e3                                      mov r0, #0
0069a84c  08 ff ff ea                                      b #0x69a474
; mapping-symbol data/literal pool
0069a850  70 a6 2f 00 b4 22 00 00 84 10 00 00 34 39 00 00  .byte 0x70, 0xa6, 0x2f, 0x00, 0xb4, 0x22, 0x00, 0x00, 0x84, 0x10, 0x00, 0x00, 0x34, 0x39, 0x00, 0x00


; FUNCTION 0x006b44e8, declared_size=152, range_size=152, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZN6glitch2io14CLimitReadFile4readEPvj
; demangled: glitch::io::CLimitReadFile::read(void*, unsigned int)
; decoder-mode: arm
006b44e8  70 40 2d e9                                      push {r4, r5, r6, lr}
006b44ec  48 30 90 e5                                      ldr r3, [r0, #0x48]
006b44f0  00 40 a0 e1                                      mov r4, r0
006b44f4  01 60 a0 e1                                      mov r6, r1
006b44f8  03 00 a0 e1                                      mov r0, r3
006b44fc  00 30 93 e5                                      ldr r3, [r3]
006b4500  02 50 a0 e1                                      mov r5, r2
006b4504  0f e0 a0 e1                                      mov lr, pc
006b4508  24 f0 93 e5                                      ldr pc, [r3, #0x24]
006b450c  4c 10 94 e5                                      ldr r1, [r4, #0x4c]
006b4510  01 00 50 e1                                      cmp r0, r1
006b4514  06 00 00 0a                                      beq #0x6b4534
006b4518  48 30 94 e5                                      ldr r3, [r4, #0x48]
006b451c  00 20 a0 e3                                      mov r2, #0
006b4520  03 00 a0 e1                                      mov r0, r3
006b4524  00 30 93 e5                                      ldr r3, [r3]
006b4528  0f e0 a0 e1                                      mov lr, pc
006b452c  18 f0 93 e5                                      ldr pc, [r3, #0x18]
006b4530  4c 00 94 e5                                      ldr r0, [r4, #0x4c]
006b4534  44 30 94 e5                                      ldr r3, [r4, #0x44]
006b4538  03 00 50 e1                                      cmp r0, r3
006b453c  0d 00 00 aa                                      bge #0x6b4578
006b4540  05 20 80 e0                                      add r2, r0, r5
006b4544  02 00 53 e1                                      cmp r3, r2
006b4548  03 50 60 d0                                      rsble r5, r0, r3
006b454c  48 30 94 e5                                      ldr r3, [r4, #0x48]
006b4550  06 10 a0 e1                                      mov r1, r6
006b4554  05 20 a0 e1                                      mov r2, r5
006b4558  03 00 a0 e1                                      mov r0, r3
006b455c  00 30 93 e5                                      ldr r3, [r3]
006b4560  0f e0 a0 e1                                      mov lr, pc
006b4564  0c f0 93 e5                                      ldr pc, [r3, #0xc]
006b4568  4c 30 94 e5                                      ldr r3, [r4, #0x4c]
006b456c  00 30 83 e0                                      add r3, r3, r0
006b4570  4c 30 84 e5                                      str r3, [r4, #0x4c]
006b4574  70 80 bd e8                                      pop {r4, r5, r6, pc}
006b4578  00 00 a0 e3                                      mov r0, #0
006b457c  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x006b4580, declared_size=80, range_size=80, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZN6glitch2io14CLimitReadFile9readAsyncEPvjPFviiPNS0_9IReadFileES2_ES2_
; demangled: glitch::io::CLimitReadFile::readAsync(void*, unsigned int, void (*)(int, int, glitch::io::IReadFile*, void*), void*)
; decoder-mode: arm
006b4580  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
006b4584  00 c0 90 e5                                      ldr ip, [r0]
006b4588  08 d0 4d e2                                      sub sp, sp, #8
006b458c  03 60 a0 e1                                      mov r6, r3
006b4590  02 70 a0 e1                                      mov r7, r2
006b4594  00 40 a0 e1                                      mov r4, r0
006b4598  01 80 a0 e1                                      mov r8, r1
006b459c  14 50 9c e5                                      ldr r5, [ip, #0x14]
006b45a0  0f e0 a0 e1                                      mov lr, pc
006b45a4  24 f0 9c e5                                      ldr pc, [ip, #0x24]
006b45a8  20 20 9d e5                                      ldr r2, [sp, #0x20]
006b45ac  00 30 a0 e1                                      mov r3, r0
006b45b0  00 60 8d e5                                      str r6, [sp]
006b45b4  04 20 8d e5                                      str r2, [sp, #4]
006b45b8  04 00 a0 e1                                      mov r0, r4
006b45bc  08 10 a0 e1                                      mov r1, r8
006b45c0  07 20 a0 e1                                      mov r2, r7
006b45c4  35 ff 2f e1                                      blx r5
006b45c8  08 d0 8d e2                                      add sp, sp, #8
006b45cc  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}


; FUNCTION 0x006b45d0, declared_size=116, range_size=116, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZN6glitch2io14CLimitReadFile9readAsyncEPvjlPFviiPNS0_9IReadFileES2_ES2_
; demangled: glitch::io::CLimitReadFile::readAsync(void*, unsigned int, long, void (*)(int, int, glitch::io::IReadFile*, void*), void*)
; decoder-mode: arm
006b45d0  30 40 2d e9                                      push {r4, r5, lr}
006b45d4  40 c0 90 e5                                      ldr ip, [r0, #0x40]
006b45d8  00 40 a0 e1                                      mov r4, r0
006b45dc  44 00 90 e5                                      ldr r0, [r0, #0x44]
006b45e0  0c 30 83 e0                                      add r3, r3, ip
006b45e4  0c d0 4d e2                                      sub sp, sp, #0xc
006b45e8  00 00 53 e1                                      cmp r3, r0
006b45ec  02 50 a0 e1                                      mov r5, r2
006b45f0  4c 30 84 e5                                      str r3, [r4, #0x4c]
006b45f4  00 00 a0 a3                                      movge r0, #0
006b45f8  0f 00 00 aa                                      bge #0x6b463c
006b45fc  02 20 83 e0                                      add r2, r3, r2
006b4600  02 00 50 e1                                      cmp r0, r2
006b4604  48 20 94 e5                                      ldr r2, [r4, #0x48]
006b4608  00 50 63 d0                                      rsble r5, r3, r0
006b460c  00 c0 92 e5                                      ldr ip, [r2]
006b4610  02 00 a0 e1                                      mov r0, r2
006b4614  18 20 9d e5                                      ldr r2, [sp, #0x18]
006b4618  00 20 8d e5                                      str r2, [sp]
006b461c  1c 20 9d e5                                      ldr r2, [sp, #0x1c]
006b4620  04 20 8d e5                                      str r2, [sp, #4]
006b4624  05 20 a0 e1                                      mov r2, r5
006b4628  0f e0 a0 e1                                      mov lr, pc
006b462c  14 f0 9c e5                                      ldr pc, [ip, #0x14]
006b4630  4c 30 94 e5                                      ldr r3, [r4, #0x4c]
006b4634  05 50 83 e0                                      add r5, r3, r5
006b4638  4c 50 84 e5                                      str r5, [r4, #0x4c]
006b463c  0c d0 8d e2                                      add sp, sp, #0xc
006b4640  30 80 bd e8                                      pop {r4, r5, pc}


; FUNCTION 0x006b4644, declared_size=140, range_size=140, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZN6glitch2io14CLimitReadFile4seekElb
; demangled: glitch::io::CLimitReadFile::seek(long, bool)
; decoder-mode: arm
006b4644  70 40 2d e9                                      push {r4, r5, r6, lr}
006b4648  48 30 90 e5                                      ldr r3, [r0, #0x48]
006b464c  00 40 a0 e1                                      mov r4, r0
006b4650  01 60 a0 e1                                      mov r6, r1
006b4654  03 00 a0 e1                                      mov r0, r3
006b4658  00 30 93 e5                                      ldr r3, [r3]
006b465c  02 50 a0 e1                                      mov r5, r2
006b4660  0f e0 a0 e1                                      mov lr, pc
006b4664  24 f0 93 e5                                      ldr pc, [r3, #0x24]
006b4668  4c 30 94 e5                                      ldr r3, [r4, #0x4c]
006b466c  00 00 55 e3                                      cmp r5, #0
006b4670  06 60 63 e0                                      rsb r6, r3, r6
006b4674  00 10 86 e0                                      add r1, r6, r0
006b4678  0c 00 00 0a                                      beq #0x6b46b0
006b467c  44 20 94 e5                                      ldr r2, [r4, #0x44]
006b4680  03 30 81 e0                                      add r3, r1, r3
006b4684  02 00 53 e1                                      cmp r3, r2
006b4688  02 10 60 c0                                      rsbgt r1, r0, r2
006b468c  00 00 81 e0                                      add r0, r1, r0
006b4690  4c 00 84 e5                                      str r0, [r4, #0x4c]
006b4694  48 30 94 e5                                      ldr r3, [r4, #0x48]
006b4698  05 20 a0 e1                                      mov r2, r5
006b469c  03 00 a0 e1                                      mov r0, r3
006b46a0  00 30 93 e5                                      ldr r3, [r3]
006b46a4  0f e0 a0 e1                                      mov lr, pc
006b46a8  18 f0 93 e5                                      ldr pc, [r3, #0x18]
006b46ac  70 80 bd e8                                      pop {r4, r5, r6, pc}
006b46b0  40 20 94 e5                                      ldr r2, [r4, #0x40]
006b46b4  44 30 94 e5                                      ldr r3, [r4, #0x44]
006b46b8  02 10 81 e0                                      add r1, r1, r2
006b46bc  03 00 51 e1                                      cmp r1, r3
006b46c0  4c 10 84 d5                                      strle r1, [r4, #0x4c]
006b46c4  f2 ff ff da                                      ble #0x6b4694
006b46c8  05 00 a0 e1                                      mov r0, r5
006b46cc  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x006b46d0, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZNK6glitch2io14CLimitReadFile7getSizeEv
; demangled: glitch::io::CLimitReadFile::getSize() const
; decoder-mode: arm
006b46d0  3c 00 90 e5                                      ldr r0, [r0, #0x3c]
006b46d4  1e ff 2f e1                                      bx lr


; FUNCTION 0x006b46d8, declared_size=16, range_size=16, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZNK6glitch2io14CLimitReadFile6getPosEv
; demangled: glitch::io::CLimitReadFile::getPos() const
; decoder-mode: arm
006b46d8  40 30 90 e5                                      ldr r3, [r0, #0x40]
006b46dc  4c 00 90 e5                                      ldr r0, [r0, #0x4c]
006b46e0  00 00 63 e0                                      rsb r0, r3, r0
006b46e4  1e ff 2f e1                                      bx lr


; FUNCTION 0x006b46e8, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZNK6glitch2io14CLimitReadFile11getFileNameEv
; demangled: glitch::io::CLimitReadFile::getFileName() const
; decoder-mode: arm
006b46e8  20 00 90 e5                                      ldr r0, [r0, #0x20]
006b46ec  1e ff 2f e1                                      bx lr


; FUNCTION 0x006b46f0, declared_size=8, range_size=8, mode=arm
; class-group: glitch::io::CLimitReadFile
; alias: _ZNK6glitch2io14CLimitReadFile11getFullPathEv
; demangled: glitch::io::CLimitReadFile::getFullPath() const
; decoder-mode: arm
006b46f0  38 00 90 e5                                      ldr r0, [r0, #0x38]
006b46f4  1e ff 2f e1                                      bx lr

